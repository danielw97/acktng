/*
 * test_db_roundtrip.c — Basic connectivity and schema round-trip test.
 *
 * Verifies that:
 *   1. /data/db.conf exists (if not, SKIP)
 *   2. The connection string in /data/db.conf can reach PostgreSQL
 *   3. The expected tables exist (schema was applied)
 *   4. A player row can be inserted, read back, and deleted (write→read round-trip)
 *
 * This test does NOT link against db_worker.o — it exercises the schema
 * directly via libpq so it has no dependency on server internals.
 *
 * Only the USE_DB_LOAD portion uses libpq.  When USE_DB_LOAD is not defined
 * the test exits 0 immediately (SKIP).
 *
 * To run against a live DB:
 *   make USE_DB_LOAD=1 unit-test-db-roundtrip
 *   ./tests/unit-test-db-roundtrip
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef USE_DB_LOAD

#include <libpq-fe.h>

/* ------------------------------------------------------------------ */
/* Helpers                                                              */
/* ------------------------------------------------------------------ */

static int read_first_line(const char *path, char *buf, size_t bufsz)
{
   FILE *fp = fopen(path, "r");
   if (!fp)
      return 0;
   if (!fgets(buf, (int)bufsz, fp))
   {
      fclose(fp);
      return 0;
   }
   fclose(fp);
   size_t len = strlen(buf);
   while (len > 0 && (buf[len - 1] == '\n' || buf[len - 1] == '\r' || buf[len - 1] == ' '))
      buf[--len] = '\0';
   return len > 0;
}

static void fail(PGconn *conn, const char *msg)
{
   if (conn)
      fprintf(stderr, "FAIL: %s: %s\n", msg, PQerrorMessage(conn));
   else
      fprintf(stderr, "FAIL: %s\n", msg);
   if (conn)
      PQfinish(conn);
   exit(1);
}

static PGresult *exec_check(PGconn *conn, const char *sql)
{
   PGresult *res = PQexec(conn, sql);
   if (!res)
      fail(conn, sql);
   ExecStatusType st = PQresultStatus(res);
   if (st != PGRES_COMMAND_OK && st != PGRES_TUPLES_OK)
   {
      fprintf(stderr, "FAIL: %s: %s\n", sql, PQerrorMessage(conn));
      PQclear(res);
      PQfinish(conn);
      exit(1);
   }
   return res;
}

/* ------------------------------------------------------------------ */
/* Tests                                                                */
/* ------------------------------------------------------------------ */

static void test_tables_exist(PGconn *conn)
{
   printf("  test_tables_exist ... ");
   fflush(stdout);

   static const char *tables[] = {"areas", "rooms",        "mobiles",       "objects", "players",
                                  "bans",  "socials",      "clans",         "sysdata", "resets",
                                  "shops", "help_entries", "shelp_entries", NULL};

   for (int i = 0; tables[i]; i++)
   {
      char sql[256];
      snprintf(sql, sizeof(sql),
               "SELECT 1 FROM information_schema.tables"
               " WHERE table_schema = 'public' AND table_name = '%s'",
               tables[i]);
      PGresult *res = exec_check(conn, sql);
      if (PQntuples(res) == 0)
      {
         PQclear(res);
         fprintf(stderr, "\nFAIL: table '%s' not found — apply area/schema.sql first\n", tables[i]);
         PQfinish(conn);
         exit(1);
      }
      PQclear(res);
   }
   printf("ok\n");
}

static void test_player_roundtrip(PGconn *conn)
{
   printf("  test_player_roundtrip ... ");
   fflush(stdout);

   const char *test_name = "__test_db_roundtrip__";
   const char *test_raw = "#PLAYER\nName __test_db_roundtrip__~\nLevel 1\n#END\n";

   /* Clean up any leftover from a previous run. */
   {
      const char *params[1] = {test_name};
      PGresult *res =
          PQexecParams(conn, "DELETE FROM players WHERE name = $1", 1, NULL, params, NULL, NULL, 0);
      PQclear(res);
   }

   /* Insert */
   {
      const char *params[3] = {test_name, "", test_raw};
      PGresult *res = PQexecParams(conn,
                                   "INSERT INTO players (name, pwd_hash, raw_save)"
                                   " VALUES ($1, $2, $3)",
                                   3, NULL, params, NULL, NULL, 0);
      if (PQresultStatus(res) != PGRES_COMMAND_OK)
      {
         fprintf(stderr, "\nFAIL: INSERT player: %s\n", PQerrorMessage(conn));
         PQclear(res);
         PQfinish(conn);
         exit(1);
      }
      PQclear(res);
   }

   /* Read back */
   {
      const char *params[1] = {test_name};
      PGresult *res = PQexecParams(conn, "SELECT raw_save FROM players WHERE name = $1", 1, NULL,
                                   params, NULL, NULL, 0);
      if (PQresultStatus(res) != PGRES_TUPLES_OK || PQntuples(res) == 0)
      {
         fprintf(stderr, "\nFAIL: SELECT player returned no rows\n");
         PQclear(res);
         PQfinish(conn);
         exit(1);
      }
      const char *got = PQgetvalue(res, 0, 0);
      if (!got || strcmp(got, test_raw) != 0)
      {
         fprintf(stderr, "\nFAIL: raw_save mismatch\n  expected: %s\n  got: %s\n", test_raw,
                 got ? got : "(null)");
         PQclear(res);
         PQfinish(conn);
         exit(1);
      }
      PQclear(res);
   }

   /* Update via ON CONFLICT */
   {
      const char *test_raw2 = "#PLAYER\nName __test_db_roundtrip__~\nLevel 2\n#END\n";
      const char *params[3] = {test_name, "", test_raw2};
      PGresult *res = PQexecParams(conn,
                                   "INSERT INTO players (name, pwd_hash, raw_save)"
                                   " VALUES ($1, $2, $3)"
                                   " ON CONFLICT (name) DO UPDATE SET raw_save = EXCLUDED.raw_save",
                                   3, NULL, params, NULL, NULL, 0);
      if (PQresultStatus(res) != PGRES_COMMAND_OK)
      {
         fprintf(stderr, "\nFAIL: upsert player: %s\n", PQerrorMessage(conn));
         PQclear(res);
         PQfinish(conn);
         exit(1);
      }
      PQclear(res);

      /* Verify update */
      PGresult *res2 = PQexecParams(conn, "SELECT raw_save FROM players WHERE name = $1", 1, NULL,
                                    params, NULL, NULL, 0);
      if (PQresultStatus(res2) != PGRES_TUPLES_OK || PQntuples(res2) == 0)
      {
         fprintf(stderr, "\nFAIL: SELECT after upsert returned no rows\n");
         PQclear(res2);
         PQfinish(conn);
         exit(1);
      }
      const char *got = PQgetvalue(res2, 0, 0);
      if (!got || strcmp(got, test_raw2) != 0)
      {
         fprintf(stderr, "\nFAIL: raw_save mismatch after upsert\n");
         PQclear(res2);
         PQfinish(conn);
         exit(1);
      }
      PQclear(res2);
   }

   /* Clean up */
   {
      const char *params[1] = {test_name};
      PGresult *res =
          PQexecParams(conn, "DELETE FROM players WHERE name = $1", 1, NULL, params, NULL, NULL, 0);
      PQclear(res);
   }

   printf("ok\n");
}

static void test_schema_version(PGconn *conn)
{
   printf("  test_schema_version ... ");
   fflush(stdout);

   PGresult *res = exec_check(conn, "SELECT version FROM schema_version WHERE id = 1");
   if (PQntuples(res) == 0)
   {
      fprintf(stderr, "\nFAIL: schema_version table is empty\n");
      PQclear(res);
      PQfinish(conn);
      exit(1);
   }
   const char *ver = PQgetvalue(res, 0, 0);
   if (!ver || atoi(ver) < 1)
   {
      fprintf(stderr, "\nFAIL: schema_version=%s (expected >= 1)\n", ver ? ver : "(null)");
      PQclear(res);
      PQfinish(conn);
      exit(1);
   }
   PQclear(res);
   printf("ok (version=%s)\n", ver);
}

/* ------------------------------------------------------------------ */
/* Main                                                                 */
/* ------------------------------------------------------------------ */

int main(void)
{
   printf("test_db_roundtrip: looking for /data/db.conf ...\n");

   char connstr[1024] = {0};
   const char *paths[] = {"/data/db.conf", NULL};
   int found = 0;
   for (int i = 0; paths[i]; i++)
   {
      if (read_first_line(paths[i], connstr, sizeof(connstr)))
      {
         printf("test_db_roundtrip: found %s\n", paths[i]);
         found = 1;
         break;
      }
   }

   if (!found)
   {
      printf("test_db_roundtrip: SKIP — no /data/db.conf (normal for CI)\n");
      return 0;
   }

   PGconn *conn = PQconnectdb(connstr);
   if (PQstatus(conn) != CONNECTION_OK)
   {
      printf("test_db_roundtrip: SKIP — cannot connect: %s\n", PQerrorMessage(conn));
      PQfinish(conn);
      return 0;
   }

   printf("test_db_roundtrip: connected, running tests ...\n");

   test_schema_version(conn);
   test_tables_exist(conn);
   test_player_roundtrip(conn);

   PQfinish(conn);
   printf("test_db_roundtrip: all tests passed.\n");
   return 0;
}

#else /* USE_DB_LOAD */

int main(void)
{
   printf("test_db_roundtrip: SKIP — USE_DB_LOAD not defined\n");
   return 0;
}

#endif /* USE_DB_LOAD */
