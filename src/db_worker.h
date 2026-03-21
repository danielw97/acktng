/*
 * db_worker.h — Async PostgreSQL worker thread.
 *
 * The MUD runs a single-threaded game loop.  All runtime database I/O
 * (player saves, player loads at login, and runtime data saves) is handed
 * to a dedicated worker thread so the game loop is never blocked on a
 * network call.
 *
 * Boot-time loads (areas, rooms, mobiles, objects, lore, data/) remain
 * synchronous — they run before game_loop() starts.
 *
 * This header is only meaningful when USE_DB_LOAD is defined.  The rest of
 * the codebase should guard its #include with #ifdef USE_DB_LOAD or check
 * the db_worker_failed global when deciding whether to fall back to flat
 * files.
 */

#ifndef DEC_DB_WORKER_H
#define DEC_DB_WORKER_H 1

#ifdef USE_DB_LOAD

/* Operation types enqueued to / posted from the worker thread. */
typedef enum
{
   /* Write operations */
   DB_WRITE_PLAYER,     /* save_char_obj   */
   DB_WRITE_CLANS,      /* save_clans      */
   DB_WRITE_BANS,       /* save_bans       */
   DB_WRITE_SOCIALS,    /* save_socials    */
   DB_WRITE_CORPSES,    /* save_corpses    */
   DB_WRITE_SYSDATA,    /* save_sysdata    */
   DB_WRITE_RULERS,     /* save_rulers     */
   DB_WRITE_BRANDS,     /* save_brands     */
   DB_WRITE_ROOM_MARKS, /* save_room_marks */
   /* Read operations */
   DB_READ_PLAYER, /* load_char_obj at login */
   /* Control */
   DB_OP_SHUTDOWN,
} DB_OP_TYPE;

/*
 * db_worker_start()
 *   Called once at server startup, after all boot-time db_load_* calls
 *   complete.  Opens the persistent worker PGconn and starts the thread.
 */
void db_worker_start(void);

/*
 * db_worker_stop()
 *   Called at server shutdown.  Enqueues DB_OP_SHUTDOWN at the tail of the
 *   request queue, then pthread_join()s — all pending writes are flushed
 *   before the thread exits.
 */
void db_worker_stop(void);

/*
 * db_worker_enqueue_write()
 *   Enqueue a write operation.  Returns immediately; the game thread never
 *   blocks.  raw_text is the serialised flat-file representation of the
 *   data (character save, social table dump, etc.).  The worker deep-copies
 *   raw_text into the queue entry; the caller retains ownership.
 *
 *   For DB_WRITE_PLAYER the name field is used for last-write-wins
 *   coalescing: if a pending entry for the same name already exists, its
 *   raw_text is replaced in-place.
 */
void db_worker_enqueue_write(DB_OP_TYPE type, const char *name, const char *raw_text);

/*
 * db_worker_enqueue_load_player()
 *   Enqueue an async player load.  Sets d->connected = CON_LOADING_FROM_DB
 *   and inserts a DB_READ_PLAYER request at the head of the queue (reads
 *   take priority over pending writes).  Returns immediately.
 */
struct descriptor_data;
void db_worker_enqueue_load_player(struct descriptor_data *d, const char *name);

/*
 * db_worker_poll_results()
 *   Called once per tick from game_loop() on the game thread.  Drains the
 *   results queue: for each completed DB_READ_PLAYER result, advances the
 *   login state machine (sets d->connected = CON_GET_OLD_PASSWORD on
 *   success, or closes the descriptor on error).
 */
void db_worker_poll_results(void);

/*
 * Global flag set by the worker when it encounters an unrecoverable DB
 * error.  When TRUE, the game thread falls back to flat-file saves.
 */
extern int db_worker_failed;

#endif /* USE_DB_LOAD */

#endif /* DEC_DB_WORKER_H */
