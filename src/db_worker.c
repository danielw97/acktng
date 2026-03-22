/*
 * db_worker.c — Async PostgreSQL worker thread.
 *
 * The game loop is single-threaded.  This module runs a dedicated background
 * thread that owns the live PGconn* and serialises all runtime DB access so
 * the game thread never blocks on a network call.
 *
 * Boot-time loads (areas, rooms, mobiles, objects, lore, data/) are
 * performed synchronously before game_loop() starts and use a separate
 * connection; they do not go through this worker.
 *
 * Only compiled when USE_DB_LOAD is defined.
 */

#ifdef USE_DB_LOAD

#include <libpq-fe.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "globals.h"
#include "db_worker.h"

/* ------------------------------------------------------------------ */
/* Shared globals                                                        */
/* ------------------------------------------------------------------ */

volatile sig_atomic_t db_worker_failed = 0; /* set to 1 on unrecoverable DB error */

/*
 * Connection string read from area/db.conf at boot.  Set by
 * db_open_boot_connection() in db.c before db_worker_start() is called.
 */
char *db_worker_connstr = NULL;

/* ------------------------------------------------------------------ */
/* Internal types                                                        */
/* ------------------------------------------------------------------ */

typedef struct db_queue_entry DB_QUEUE_ENTRY;
struct db_queue_entry
{
   DB_OP_TYPE type;
   char *name;            /* character name (player ops) or NULL       */
   char *raw_text;        /* flat-file text payload (writes) or NULL   */
   DESCRIPTOR_DATA *desc; /* descriptor (reads only)                  */
   /* results queue fields */
   char *result_text; /* raw_save from DB (may be NULL)             */
   int result_found;  /* 1 = row found, 0 = not found               */
   DB_QUEUE_ENTRY *next;
};

/* ------------------------------------------------------------------ */
/* Request queue (game thread → worker thread)                          */
/* ------------------------------------------------------------------ */

static DB_QUEUE_ENTRY *req_head = NULL;
static DB_QUEUE_ENTRY *req_tail = NULL;
static pthread_mutex_t req_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t req_cond = PTHREAD_COND_INITIALIZER;

/* ------------------------------------------------------------------ */
/* Results queue (worker thread → game thread)                          */
/* ------------------------------------------------------------------ */

static DB_QUEUE_ENTRY *res_head = NULL;
static DB_QUEUE_ENTRY *res_tail = NULL;
static pthread_mutex_t res_mutex = PTHREAD_MUTEX_INITIALIZER;

/* ------------------------------------------------------------------ */
/* Worker thread state                                                   */
/* ------------------------------------------------------------------ */

static pthread_t worker_thread;
static PGconn *worker_conn = NULL;
static int worker_error_count = 0;
#define WORKER_MAX_ERRORS 3

/* ------------------------------------------------------------------ */
/* Helpers                                                               */
/* ------------------------------------------------------------------ */

static DB_QUEUE_ENTRY *make_entry(DB_OP_TYPE type, const char *name, const char *raw_text,
                                  DESCRIPTOR_DATA *desc)
{
   DB_QUEUE_ENTRY *e = calloc(1, sizeof(DB_QUEUE_ENTRY));
   if (!e)
      return NULL;
   e->type = type;
   e->name = name ? strdup(name) : NULL;
   e->raw_text = raw_text ? strdup(raw_text) : NULL;
   e->desc = desc;
   return e;
}

static void free_entry(DB_QUEUE_ENTRY *e)
{
   if (!e)
      return;
   free(e->name);
   free(e->raw_text);
   free(e->result_text);
   free(e);
}

static void post_to_results(DB_QUEUE_ENTRY *result)
{
   pthread_mutex_lock(&res_mutex);
   if (!res_tail)
   {
      res_head = res_tail = result;
   }
   else
   {
      res_tail->next = result;
      res_tail = result;
   }
   pthread_mutex_unlock(&res_mutex);
}

/* ------------------------------------------------------------------ */
/* Worker: write operations                                              */
/* ------------------------------------------------------------------ */

static void worker_exec_write_player(const char *name, const char *raw_text)
{
   if (!worker_conn || !name || !raw_text)
      return;

   const char *params[2];
   params[0] = name;
   params[1] = raw_text;

   /* Store the flat-file text in the raw_save column.
    * pwd_hash will be populated properly in Phase 7+ when we parse
    * the individual fields.  For now, preserve whatever is already
    * there via ON CONFLICT ... DO UPDATE. */
   PGresult *res = PQexecParams(worker_conn,
                                "INSERT INTO players (name, pwd_hash, raw_save)"
                                " VALUES ($1, '', $2)"
                                " ON CONFLICT (name)"
                                " DO UPDATE SET raw_save = EXCLUDED.raw_save",
                                2, NULL, params, NULL, NULL, 0);

   if (PQresultStatus(res) != PGRES_COMMAND_OK)
   {
      log_f("db_worker: write_player(%s): %s", name, PQerrorMessage(worker_conn));
      worker_error_count++;
   }
   else
   {
      worker_error_count = 0;
   }
   PQclear(res);
}

/* ------------------------------------------------------------------ */
/* Worker: read operations                                               */
/* ------------------------------------------------------------------ */

static void worker_exec_read_player(DB_QUEUE_ENTRY *entry)
{
   DB_QUEUE_ENTRY *result = calloc(1, sizeof(DB_QUEUE_ENTRY));
   if (!result)
      return;

   result->type = DB_READ_PLAYER;
   result->desc = entry->desc;
   result->name = entry->name ? strdup(entry->name) : NULL;
   result->result_found = 0;
   result->result_text = NULL;

   if (worker_conn && entry->name)
   {
      const char *params[1];
      params[0] = entry->name;

      PGresult *res = PQexecParams(worker_conn, "SELECT raw_save FROM players WHERE name = $1", 1,
                                   NULL, params, NULL, NULL, 0);

      if (PQresultStatus(res) == PGRES_TUPLES_OK)
      {
         if (PQntuples(res) > 0 && !PQgetisnull(res, 0, 0))
         {
            const char *val = PQgetvalue(res, 0, 0);
            if (val && *val)
            {
               result->result_text = strdup(val);
               result->result_found = 1;
            }
         }
         worker_error_count = 0;
      }
      else
      {
         log_f("db_worker: read_player(%s): %s", entry->name, PQerrorMessage(worker_conn));
         worker_error_count++;
      }
      PQclear(res);
   }

   post_to_results(result);
}

/* ------------------------------------------------------------------ */
/* Worker: unrecoverable error — switch to flat-file fallback            */
/* ------------------------------------------------------------------ */

static void worker_handle_fatal_error(void)
{
   log_string("db_worker: FATAL — too many consecutive DB errors; "
              "switching to flat-file emergency mode");
   db_worker_failed = 1;
   if (worker_conn)
   {
      PQfinish(worker_conn);
      worker_conn = NULL;
   }
}

/* ------------------------------------------------------------------ */
/* Worker thread main loop                                               */
/* ------------------------------------------------------------------ */

static void *worker_main(void *arg)
{
   (void)arg;

   if (db_worker_connstr)
   {
      worker_conn = PQconnectdb(db_worker_connstr);
      if (PQstatus(worker_conn) != CONNECTION_OK)
      {
         log_f("db_worker: worker connection failed: %s", PQerrorMessage(worker_conn));
         PQfinish(worker_conn);
         worker_conn = NULL;
         db_worker_failed = 1;
      }
   }

   for (;;)
   {
      pthread_mutex_lock(&req_mutex);
      while (!req_head)
         pthread_cond_wait(&req_cond, &req_mutex);

      DB_QUEUE_ENTRY *entry = req_head;
      req_head = entry->next;
      if (!req_head)
         req_tail = NULL;
      pthread_mutex_unlock(&req_mutex);

      if (entry->type == DB_OP_SHUTDOWN)
      {
         free_entry(entry);
         break;
      }

      /* Check fatal error threshold. */
      if (worker_error_count >= WORKER_MAX_ERRORS)
      {
         worker_handle_fatal_error();
         /* For pending reads: post an error result so the login flow
          * can fall back to flat-file load. */
         if (entry->type == DB_READ_PLAYER)
            worker_exec_read_player(entry); /* will post result with found=0 */
         free_entry(entry);
         continue;
      }

      /* Attempt reconnect if connection dropped. */
      if (worker_conn && PQstatus(worker_conn) != CONNECTION_OK)
      {
         PQreset(worker_conn);
         if (PQstatus(worker_conn) != CONNECTION_OK)
         {
            log_f("db_worker: reconnect failed: %s", PQerrorMessage(worker_conn));
            worker_error_count++;
            if (entry->type == DB_READ_PLAYER)
               worker_exec_read_player(entry);
            free_entry(entry);
            continue;
         }
      }

      switch (entry->type)
      {
      case DB_WRITE_PLAYER:
         worker_exec_write_player(entry->name, entry->raw_text);
         break;
      case DB_WRITE_CLANS:
      case DB_WRITE_BANS:
      case DB_WRITE_SOCIALS:
      case DB_WRITE_CORPSES:
      case DB_WRITE_SYSDATA:
      case DB_WRITE_RULERS:
      case DB_WRITE_BRANDS:
      case DB_WRITE_ROOM_MARKS:
         /* Phase 6: normalised writes for these tables are implemented
          * in Phase 7+.  The flat-file writers remain active. */
         log_f("db_worker: write op type=%d logged (flat-file fallback active)", (int)entry->type);
         break;
      case DB_READ_PLAYER:
         worker_exec_read_player(entry);
         break;
      default:
         log_f("db_worker: unknown op type=%d", (int)entry->type);
         break;
      }

      free_entry(entry);
   }

   if (worker_conn)
   {
      PQfinish(worker_conn);
      worker_conn = NULL;
   }
   return NULL;
}

/* ------------------------------------------------------------------ */
/* Public API                                                            */
/* ------------------------------------------------------------------ */

void db_worker_start(void)
{
   if (db_worker_failed || !db_worker_connstr)
      return;

   if (pthread_create(&worker_thread, NULL, worker_main, NULL) != 0)
   {
      log_string("db_worker: pthread_create failed — falling back to flat-file saves");
      db_worker_failed = 1;
   }
}

void db_worker_stop(void)
{
   if (db_worker_failed)
      return;

   DB_QUEUE_ENTRY *e = calloc(1, sizeof(DB_QUEUE_ENTRY));
   if (e)
   {
      e->type = DB_OP_SHUTDOWN;
      pthread_mutex_lock(&req_mutex);
      if (!req_tail)
      {
         req_head = req_tail = e;
      }
      else
      {
         req_tail->next = e;
         req_tail = e;
      }
      pthread_cond_signal(&req_cond);
      pthread_mutex_unlock(&req_mutex);
   }

   pthread_join(worker_thread, NULL);
}

void db_worker_enqueue_write(DB_OP_TYPE type, const char *name, const char *raw_text)
{
   if (db_worker_failed)
      return;

   DB_QUEUE_ENTRY *e = make_entry(type, name, raw_text, NULL);
   if (!e)
      return;

   pthread_mutex_lock(&req_mutex);

   /* Last-write-wins coalescing for player saves. */
   if (type == DB_WRITE_PLAYER && name)
   {
      DB_QUEUE_ENTRY *scan;
      for (scan = req_head; scan; scan = scan->next)
      {
         if (scan->type == DB_WRITE_PLAYER && scan->name && strcmp(scan->name, name) == 0)
         {
            /* Replace raw_text in-place. */
            char *old = scan->raw_text;
            scan->raw_text = e->raw_text;
            e->raw_text = old; /* old goes into e and gets freed below */
            free_entry(e);
            e = NULL;
            break;
         }
      }
   }

   if (e)
   {
      if (!req_tail)
      {
         req_head = req_tail = e;
      }
      else
      {
         req_tail->next = e;
         req_tail = e;
      }
   }

   pthread_cond_signal(&req_cond);
   pthread_mutex_unlock(&req_mutex);
}

void db_worker_enqueue_load_player(DESCRIPTOR_DATA *d, const char *name)
{
   if (db_worker_failed || !d || !name)
      return;

   DB_QUEUE_ENTRY *e = make_entry(DB_READ_PLAYER, name, NULL, d);
   if (!e)
      return;

   /* Reads go to the head of the queue for lower login latency. */
   pthread_mutex_lock(&req_mutex);
   e->next = req_head;
   req_head = e;
   if (!req_tail)
      req_tail = e;
   pthread_cond_signal(&req_cond);
   pthread_mutex_unlock(&req_mutex);

   d->connected = CON_LOADING_FROM_DB;
}

/*
 * db_worker_poll_results()
 *
 * Called once per tick from game_loop() (in socket.c).  Drains the results
 * queue and advances the login state machine for each completed player load.
 *
 * In Phase 6 (transitional): we always fall back to the flat-file
 * load_char_obj() for the actual character data.  The DB query result
 * (raw_save) is available but not yet used as the authoritative source.
 */
void db_worker_poll_results(void)
{
   DB_QUEUE_ENTRY *e;
   DESCRIPTOR_DATA *d;

   for (;;)
   {
      pthread_mutex_lock(&res_mutex);
      e = res_head;
      if (e)
      {
         res_head = e->next;
         if (!res_head)
            res_tail = NULL;
      }
      pthread_mutex_unlock(&res_mutex);

      if (!e)
         break;

      if (e->type != DB_READ_PLAYER || !e->desc)
      {
         free_entry(e);
         continue;
      }

      d = e->desc;

      /* Descriptor may have been closed while the worker was running. */
      if (d->connected != CON_LOADING_FROM_DB)
      {
         free_entry(e);
         continue;
      }

      /* Phase 6: use flat-file load regardless of DB result.
       * The async path ensures the game loop was not blocked waiting
       * for the DB query.  Flat-file load is synchronous here but
       * on the game thread, so it's safe. */
      bool fOld = load_char_obj(d, e->name, FALSE);
      d->connected = fOld ? CON_GET_OLD_PASSWORD : CON_GET_NEW_PASSWORD;

      free_entry(e);
   }
}

#endif /* USE_DB_LOAD */
