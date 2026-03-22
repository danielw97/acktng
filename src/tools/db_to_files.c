/*
 * db_to_files.c — Standalone tool to export ACK!TNG PostgreSQL data back to flat files.
 *
 * Usage: db_to_files <connstr> [--players <dir>] [--help <dir>] [--shelp <dir>]
 *
 * Reads records from the PostgreSQL database and writes them as flat files,
 * mirroring the directory layout expected by the server.
 *
 * This tool is compiled separately from the server binary and is only built
 * when USE_DB_LOAD is defined (make USE_DB_LOAD=1 tools/db_to_files).
 *
 * Only compiled when USE_DB_LOAD is defined.
 */

#ifdef USE_DB_LOAD

#include <libpq-fe.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

/* ------------------------------------------------------------------ */
/* Helpers                                                              */
/* ------------------------------------------------------------------ */

static void die(PGconn *conn, const char *msg)
{
   if (conn)
      fprintf(stderr, "db_to_files: %s: %s\n", msg, PQerrorMessage(conn));
   else
      fprintf(stderr, "db_to_files: %s\n", msg);
   if (conn)
      PQfinish(conn);
   exit(1);
}

static void check_result(PGconn *conn, PGresult *res, const char *ctx)
{
   ExecStatusType st = PQresultStatus(res);
   if (st != PGRES_COMMAND_OK && st != PGRES_TUPLES_OK)
   {
      fprintf(stderr, "db_to_files: %s: %s\n", ctx, PQerrorMessage(conn));
      PQclear(res);
      PQfinish(conn);
      exit(1);
   }
}

static int mkdir_p(const char *path)
{
   char tmp[512];
   snprintf(tmp, sizeof(tmp), "%s", path);
   for (char *p = tmp + 1; *p; p++)
   {
      if (*p == '/')
      {
         *p = '\0';
         mkdir(tmp, 0755);
         *p = '/';
      }
   }
   return mkdir(tmp, 0755);
}

static char tolower_c(char c)
{
   return (c >= 'A' && c <= 'Z') ? (char)(c + 32) : c;
}

/* ------------------------------------------------------------------ */
/* Export player files                                                  */
/* ------------------------------------------------------------------ */

static int export_players(PGconn *conn, const char *out_dir)
{
   PGresult *res = PQexec(conn, "SELECT name, raw_save FROM players WHERE raw_save IS NOT NULL");
   check_result(conn, res, "SELECT players");

   int count = 0;
   int n = PQntuples(res);
   for (int i = 0; i < n; i++)
   {
      const char *name = PQgetvalue(res, i, 0);
      const char *raw = PQgetvalue(res, i, 1);
      if (!name || !*name || !raw || !*raw)
         continue;

      char first[2] = {tolower_c(name[0]), '\0'};
      char subdir[512];
      snprintf(subdir, sizeof(subdir), "%s/%s", out_dir, first);
      mkdir_p(subdir);

      /* Lower-case the name for the filename. */
      char lcname[256];
      int j;
      for (j = 0; name[j] && j < 255; j++)
         lcname[j] = tolower_c(name[j]);
      lcname[j] = '\0';

      char path[768];
      snprintf(path, sizeof(path), "%s/%s", subdir, lcname);

      FILE *fp = fopen(path, "w");
      if (!fp)
      {
         fprintf(stderr, "db_to_files: cannot write %s\n", path);
         continue;
      }
      fputs(raw, fp);
      fclose(fp);
      count++;
   }

   PQclear(res);
   return count;
}

/* ------------------------------------------------------------------ */
/* Export help entries                                                  */
/* ------------------------------------------------------------------ */

static int export_helps(PGconn *conn, const char *out_dir, int is_shelp)
{
   const char *sql = is_shelp ? "SELECT keywords, min_level, body FROM shelp_entries"
                              : "SELECT keywords, min_level, body FROM help_entries";

   PGresult *res = PQexec(conn, sql);
   check_result(conn, res, "SELECT helps");

   mkdir_p(out_dir);

   int count = 0;
   int n = PQntuples(res);
   for (int i = 0; i < n; i++)
   {
      const char *keywords = PQgetvalue(res, i, 0);
      const char *level_s = PQgetvalue(res, i, 1);
      const char *body = PQgetvalue(res, i, 2);

      if (!keywords || !*keywords)
         continue;

      /* Use first keyword as filename, replacing spaces with underscores. */
      char fname[256];
      int j;
      for (j = 0; keywords[j] && keywords[j] != ' ' && j < 255; j++)
         fname[j] = tolower_c(keywords[j]);
      fname[j] = '\0';

      /* Sanitise: replace / with _ */
      for (j = 0; fname[j]; j++)
         if (fname[j] == '/')
            fname[j] = '_';

      if (!fname[0])
         continue;

      char path[512];
      snprintf(path, sizeof(path), "%s/%s", out_dir, fname);

      FILE *fp = fopen(path, "w");
      if (!fp)
      {
         fprintf(stderr, "db_to_files: cannot write %s\n", path);
         continue;
      }
      /* Reconstruct the standard help file header line. */
      fprintf(fp, "%s %s~\n", level_s ? level_s : "0", keywords);
      if (body && *body)
         fputs(body, fp);
      fprintf(fp, "~\n");
      fclose(fp);
      count++;
   }

   PQclear(res);
   return count;
}

/* ------------------------------------------------------------------ */
/* Main                                                                 */
/* ------------------------------------------------------------------ */

static void usage(const char *prog)
{
   fprintf(stderr,
           "Usage: %s <connstr> [--players <dir>] [--help <dir>] [--shelp <dir>] [--all]\n"
           "\n"
           "  <connstr>     PostgreSQL connection string\n"
           "  --players <dir>  Export player files to <dir> (subdirs a-z created)\n"
           "  --help <dir>  Export help entries to <dir>\n"
           "  --shelp <dir> Export shelp entries to <dir>\n"
           "  --all         Export everything to default paths\n"
           "                (player/ → player/, help/ → help/, shelp/ → shelp/)\n",
           prog);
   exit(1);
}

int main(int argc, char **argv)
{
   if (argc < 3)
      usage(argv[0]);

   const char *connstr = argv[1];

   const char *players_dir = NULL;
   const char *help_dir = NULL;
   const char *shelp_dir = NULL;

   for (int i = 2; i < argc; i++)
   {
      if (!strcmp(argv[i], "--all"))
      {
         players_dir = "player";
         help_dir = "help";
         shelp_dir = "shelp";
      }
      else if (!strcmp(argv[i], "--players") && i + 1 < argc)
         players_dir = argv[++i];
      else if (!strcmp(argv[i], "--help") && i + 1 < argc)
         help_dir = argv[++i];
      else if (!strcmp(argv[i], "--shelp") && i + 1 < argc)
         shelp_dir = argv[++i];
      else
      {
         fprintf(stderr, "Unknown option or missing argument: %s\n", argv[i]);
         usage(argv[0]);
      }
   }

   if (!players_dir && !help_dir && !shelp_dir)
   {
      fprintf(stderr, "db_to_files: nothing to export. Use --all or specify targets.\n");
      usage(argv[0]);
   }

   PGconn *conn = PQconnectdb(connstr);
   if (PQstatus(conn) != CONNECTION_OK)
      die(conn, "connection failed");

   if (players_dir)
   {
      int n = export_players(conn, players_dir);
      printf("  players: %d files written to %s/\n", n, players_dir);
   }

   if (help_dir)
   {
      int n = export_helps(conn, help_dir, 0);
      printf("  help: %d entries written to %s/\n", n, help_dir);
   }

   if (shelp_dir)
   {
      int n = export_helps(conn, shelp_dir, 1);
      printf("  shelp: %d entries written to %s/\n", n, shelp_dir);
   }

   PQfinish(conn);
   printf("db_to_files: done.\n");
   return 0;
}

#else /* USE_DB_LOAD */

#include <stdio.h>
int main(void)
{
   fprintf(stderr, "db_to_files: built without USE_DB_LOAD — no-op.\n");
   return 1;
}

#endif /* USE_DB_LOAD */
