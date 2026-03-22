#ifdef HAVE_LIBPQ

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libpq-fe.h>

#define DEC_GLOBALS_H 1
#include "ack.h"
#include "db_worker.h"

/* -----------------------------------------------------------------------
 * Internal types
 * ----------------------------------------------------------------------- */

typedef struct db_request
{
   DB_OP_TYPE type;
   void *buf;                 /* serialised data blob (heap-allocated copy) */
   size_t len;                /* byte length of buf */
   char name[64];             /* player name for WRITE_PLAYER coalescing */
   struct descriptor_data *d; /* for DB_READ_PLAYER */
   struct db_request *next;
} DB_REQUEST;

/* -----------------------------------------------------------------------
 * Globals
 * ----------------------------------------------------------------------- */

int db_worker_failed = 0;

static pthread_t worker_thread;
static pthread_mutex_t req_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t req_cond = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t res_mutex = PTHREAD_MUTEX_INITIALIZER;

static DB_REQUEST *req_head = NULL;
static DB_REQUEST *req_tail = NULL;

static DB_PLAYER_RESULT *res_head = NULL;

static char worker_connstr[512] = "";
static PGconn *worker_conn = NULL;

/* Consecutive failure counter for emergency fallback */
#define DB_MAX_FAILURES 3
static int consecutive_failures = 0;

/* -----------------------------------------------------------------------
 * Internal helpers
 * ----------------------------------------------------------------------- */

static void post_result(struct descriptor_data *d, struct char_data *ch, int error)
{
   DB_PLAYER_RESULT *r = calloc(1, sizeof(DB_PLAYER_RESULT));
   if (!r)
      return;
   r->d = d;
   r->ch = ch;
   r->error = error;
   r->next = NULL;
   pthread_mutex_lock(&res_mutex);
   r->next = res_head;
   res_head = r;
   pthread_mutex_unlock(&res_mutex);
}

/* Ensure the worker has an open connection.  Returns 1 on success. */
static int ensure_connected(void)
{
   if (worker_conn && PQstatus(worker_conn) == CONNECTION_OK)
      return 1;
   if (worker_conn)
   {
      PQfinish(worker_conn);
      worker_conn = NULL;
   }
   worker_conn = PQconnectdb(worker_connstr);
   if (PQstatus(worker_conn) != CONNECTION_OK)
   {
      fprintf(stderr, "DB worker: reconnect failed: %s\n", PQerrorMessage(worker_conn));
      PQfinish(worker_conn);
      worker_conn = NULL;
      return 0;
   }
   return 1;
}

/* -----------------------------------------------------------------------
 * Per-operation handlers (called on worker thread)
 * ----------------------------------------------------------------------- */

/* Execute a simple parameterised statement that returns no rows.
 * Returns 1 on success, 0 on error. */
static int worker_exec(const char *sql, int nParams, const char *const *params)
{
   PGresult *res;
   ExecStatusType status;
   int ok;

   if (!ensure_connected())
      return 0;

   res = PQexecParams(worker_conn, sql, nParams, NULL, params, NULL, NULL, 0);
   status = PQresultStatus(res);
   ok = (status == PGRES_COMMAND_OK || status == PGRES_TUPLES_OK);
   if (!ok)
      fprintf(stderr, "DB worker exec error [%s]: %s\n", sql, PQresultErrorMessage(res));
   PQclear(res);
   return ok;
}

/* DB_WRITE_PLAYER — buf is a NUL-terminated JSON blob.
 * Uses INSERT ... ON CONFLICT (name) DO UPDATE to upsert. */
static int handle_write_player(DB_REQUEST *req)
{
   /* TODO: deserialise buf into individual columns and upsert.
    * For now store the raw JSON as raw_save only, which serves as a
    * transitional fallback until the full serialiser is implemented. */
   const char *params[2];
   params[0] = req->name;
   params[1] = (const char *)req->buf;
   return worker_exec("INSERT INTO players (name, pwd_hash, raw_save) "
                      "VALUES ($1, '', $2) "
                      "ON CONFLICT (name) DO UPDATE SET raw_save = EXCLUDED.raw_save",
                      2, params);
}

/* DB_READ_PLAYER — SELECT player row and reconstruct CHAR_DATA.
 * Posts result to the results queue when done. */
static void handle_read_player(DB_REQUEST *req)
{
   PGresult *res;
   const char *params[1];
   int nrows;

   if (!ensure_connected())
   {
      post_result(req->d, NULL, 1);
      return;
   }

   params[0] = req->name;
   res = PQexecParams(worker_conn,
                      "SELECT name, pwd_hash, raw_save FROM players "
                      "WHERE name = $1",
                      1, NULL, params, NULL, NULL, 0);

   if (PQresultStatus(res) != PGRES_TUPLES_OK)
   {
      fprintf(stderr, "DB worker: read_player query failed: %s\n", PQresultErrorMessage(res));
      PQclear(res);
      post_result(req->d, NULL, 1);
      return;
   }

   nrows = PQntuples(res);
   PQclear(res);

   if (nrows == 0)
   {
      /* New player */
      post_result(req->d, NULL, 0);
      return;
   }

   /* TODO: hydrate CHAR_DATA from columns.
    * For now signal "found" with a NULL ch; the poll handler will fall
    * back to load_char_obj() from the flat file or raw_save. */
   post_result(req->d, NULL, 0);
}

/* Stub handlers for write-only types (expand per-phase). */
static int handle_write_clans(DB_REQUEST *req)
{
   (void)req;
   return 1; /* TODO */
}
static int handle_write_bans(DB_REQUEST *req)
{
   (void)req;
   return 1; /* TODO */
}
static int handle_write_socials(DB_REQUEST *req)
{
   (void)req;
   return 1; /* TODO */
}
static int handle_write_corpses(DB_REQUEST *req)
{
   (void)req;
   return 1; /* TODO */
}
static int handle_write_sysdata(DB_REQUEST *req)
{
   (void)req;
   return 1; /* TODO */
}
static int handle_write_rulers(DB_REQUEST *req)
{
   (void)req;
   return 1; /* TODO */
}
static int handle_write_brands(DB_REQUEST *req)
{
   (void)req;
   return 1; /* TODO */
}
static int handle_write_room_marks(DB_REQUEST *req)
{
   (void)req;
   return 1; /* TODO */
}

static int dispatch(DB_REQUEST *req)
{
   switch (req->type)
   {
   case DB_WRITE_PLAYER:
      return handle_write_player(req);
   case DB_WRITE_CLANS:
      return handle_write_clans(req);
   case DB_WRITE_BANS:
      return handle_write_bans(req);
   case DB_WRITE_SOCIALS:
      return handle_write_socials(req);
   case DB_WRITE_CORPSES:
      return handle_write_corpses(req);
   case DB_WRITE_SYSDATA:
      return handle_write_sysdata(req);
   case DB_WRITE_RULERS:
      return handle_write_rulers(req);
   case DB_WRITE_BRANDS:
      return handle_write_brands(req);
   case DB_WRITE_ROOM_MARKS:
      return handle_write_room_marks(req);
   case DB_READ_PLAYER:
      handle_read_player(req);
      return 1;
   case DB_OP_SHUTDOWN:
      return 1;
   default:
      fprintf(stderr, "DB worker: unknown op type %d\n", (int)req->type);
      return 0;
   }
}

/* -----------------------------------------------------------------------
 * Worker thread main loop
 * ----------------------------------------------------------------------- */

static void *worker_main(void *arg)
{
   (void)arg;

   for (;;)
   {
      DB_REQUEST *req;
      int ok;

      /* Wait for work */
      pthread_mutex_lock(&req_mutex);
      while (req_head == NULL)
         pthread_cond_wait(&req_cond, &req_mutex);

      /* Dequeue from head */
      req = req_head;
      req_head = req->next;
      if (req_head == NULL)
         req_tail = NULL;
      pthread_mutex_unlock(&req_mutex);

      if (req->type == DB_OP_SHUTDOWN)
      {
         free(req->buf);
         free(req);
         break;
      }

      ok = dispatch(req);
      if (!ok)
      {
         consecutive_failures++;
         if (consecutive_failures >= DB_MAX_FAILURES && !db_worker_failed)
         {
            fprintf(stderr,
                    "DB worker: %d consecutive failures — entering "
                    "emergency fallback mode\n",
                    consecutive_failures);
            db_worker_failed = 1;
         }
      }
      else
      {
         consecutive_failures = 0;
      }

      free(req->buf);
      free(req);
   }

   if (worker_conn)
   {
      PQfinish(worker_conn);
      worker_conn = NULL;
   }
   return NULL;
}

/* -----------------------------------------------------------------------
 * Public API
 * ----------------------------------------------------------------------- */

void db_worker_start(const char *connstr)
{
   if (connstr)
      strncpy(worker_connstr, connstr, sizeof(worker_connstr) - 1);

   if (pthread_create(&worker_thread, NULL, worker_main, NULL) != 0)
   {
      perror("DB worker: pthread_create failed");
      db_worker_failed = 1;
   }
}

void db_worker_stop(void)
{
   /* Enqueue shutdown sentinel at the tail so pending writes are flushed */
   DB_REQUEST *req = calloc(1, sizeof(DB_REQUEST));
   if (!req)
      return;
   req->type = DB_OP_SHUTDOWN;

   pthread_mutex_lock(&req_mutex);
   if (req_tail)
   {
      req_tail->next = req;
      req_tail = req;
   }
   else
   {
      req_head = req_tail = req;
   }
   pthread_cond_signal(&req_cond);
   pthread_mutex_unlock(&req_mutex);

   pthread_join(worker_thread, NULL);
}

void db_worker_enqueue_write(DB_OP_TYPE type, const void *buf, size_t len, const char *name)
{
   DB_REQUEST *req;

   pthread_mutex_lock(&req_mutex);

   /* Coalesce player saves: if a pending DB_WRITE_PLAYER for the same name
    * exists, replace its payload in-place rather than appending a new entry.
    */
   if (type == DB_WRITE_PLAYER && name && name[0])
   {
      DB_REQUEST *p;
      for (p = req_head; p; p = p->next)
      {
         if (p->type == DB_WRITE_PLAYER && strcmp(p->name, name) == 0)
         {
            /* Replace buffer */
            free(p->buf);
            p->buf = malloc(len + 1);
            if (p->buf)
            {
               memcpy(p->buf, buf, len);
               ((char *)p->buf)[len] = '\0';
               p->len = len;
            }
            pthread_mutex_unlock(&req_mutex);
            return;
         }
      }
   }

   req = calloc(1, sizeof(DB_REQUEST));
   if (!req)
   {
      pthread_mutex_unlock(&req_mutex);
      return;
   }
   req->type = type;
   req->len = len;
   req->buf = malloc(len + 1);
   if (req->buf)
   {
      memcpy(req->buf, buf, len);
      ((char *)req->buf)[len] = '\0';
   }
   if (name)
      strncpy(req->name, name, sizeof(req->name) - 1);

   /* Writes go to the tail */
   req->next = NULL;
   if (req_tail)
   {
      req_tail->next = req;
      req_tail = req;
   }
   else
   {
      req_head = req_tail = req;
   }

   pthread_cond_signal(&req_cond);
   pthread_mutex_unlock(&req_mutex);
}

void db_worker_enqueue_load_player(struct descriptor_data *d, const char *name)
{
   DB_REQUEST *req = calloc(1, sizeof(DB_REQUEST));
   if (!req)
      return;
   req->type = DB_READ_PLAYER;
   req->d = d;
   strncpy(req->name, name, sizeof(req->name) - 1);
   req->buf = NULL;
   req->len = 0;

   d->connected = CON_LOADING_FROM_DB;

   /* Reads go to the head for priority */
   pthread_mutex_lock(&req_mutex);
   req->next = req_head;
   req_head = req;
   if (req_tail == NULL)
      req_tail = req;
   pthread_cond_signal(&req_cond);
   pthread_mutex_unlock(&req_mutex);
}

void db_worker_poll_results(void)
{
   DB_PLAYER_RESULT *list;
   DB_PLAYER_RESULT *r;
   DB_PLAYER_RESULT *next;

   pthread_mutex_lock(&res_mutex);
   list = res_head;
   res_head = NULL;
   pthread_mutex_unlock(&res_mutex);

   for (r = list; r; r = next)
   {
      next = r->next;

      if (r->error)
      {
         /* DB error — tell the player to try again and close */
         if (r->d && r->d->connected == CON_LOADING_FROM_DB)
         {
            write_to_buffer(r->d, "Database error — please try again later.\n\r", 0);
            close_socket(r->d);
         }
      }
      else if (r->ch == NULL)
      {
         /* Row not found: new player — proceed to name confirmation */
         if (r->d && r->d->connected == CON_LOADING_FROM_DB)
            r->d->connected = CON_CONFIRM_NEW_NAME;
      }
      else
      {
         /* Existing player loaded */
         if (r->d && r->d->connected == CON_LOADING_FROM_DB)
         {
            r->d->character = r->ch;
            r->ch->desc = r->d;
            r->d->connected = CON_GET_OLD_PASSWORD;
            write_to_buffer(r->d, "Password: ", 0);
         }
      }

      free(r);
   }
}

#endif /* HAVE_LIBPQ */
