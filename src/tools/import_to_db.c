/*
 * import_to_db.c — Standalone tool to import ACK!TNG flat files into PostgreSQL.
 *
 * Usage: import_to_db <connstr> [--areas] [--help] [--shelp] [--lore]
 *                               [--players] [--data] [--all]
 *
 * Reads flat files from the area/ directory tree and imports them into the
 * PostgreSQL database described by <connstr>.  The schema must already exist
 * (apply area/schema.sql first).
 *
 * This tool is compiled separately from the server binary and is only built
 * when USE_DB_LOAD is defined (make USE_DB_LOAD=1 tools/import_to_db).
 *
 * Only compiled when USE_DB_LOAD is defined.
 */

#ifdef USE_DB_LOAD

#include <dirent.h>
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
      fprintf(stderr, "import_to_db: %s: %s\n", msg, PQerrorMessage(conn));
   else
      fprintf(stderr, "import_to_db: %s\n", msg);
   if (conn)
      PQfinish(conn);
   exit(1);
}

static void check_result(PGconn *conn, PGresult *res, const char *ctx)
{
   ExecStatusType st = PQresultStatus(res);
   if (st != PGRES_COMMAND_OK && st != PGRES_TUPLES_OK)
   {
      fprintf(stderr, "import_to_db: %s: %s\n", ctx, PQerrorMessage(conn));
      PQclear(res);
      PQfinish(conn);
      exit(1);
   }
}

/* Read an entire file into a malloc'd buffer (caller must free). */
static char *slurp_file(const char *path, long *out_len)
{
   FILE *fp = fopen(path, "r");
   if (!fp)
      return NULL;
   fseek(fp, 0, SEEK_END);
   long len = ftell(fp);
   if (len <= 0)
   {
      fclose(fp);
      if (out_len)
         *out_len = 0;
      return NULL;
   }
   rewind(fp);
   char *buf = malloc((size_t)len + 1);
   if (!buf)
   {
      fclose(fp);
      return NULL;
   }
   size_t nr = fread(buf, 1, (size_t)len, fp);
   buf[nr] = '\0';
   fclose(fp);
   if (out_len)
      *out_len = (long)nr;
   return buf;
}

/* ------------------------------------------------------------------ */
/* Import player files                                                  */
/* ------------------------------------------------------------------ */

static int import_players(PGconn *conn, const char *player_dir)
{
   int count = 0;
   char subdir[512];

   for (char c = 'a'; c <= 'z'; c++)
   {
      snprintf(subdir, sizeof(subdir), "%s/%c", player_dir, c);
      DIR *d = opendir(subdir);
      if (!d)
         continue;

      struct dirent *ent;
      while ((ent = readdir(d)) != NULL)
      {
         if (ent->d_name[0] == '.')
            continue;

         char path[1024];
         snprintf(path, sizeof(path), "%s/%s", subdir, ent->d_name);

         long len;
         char *raw = slurp_file(path, &len);
         if (!raw || len == 0)
         {
            free(raw);
            continue;
         }

         /* Use filename (lowercased) as the player name. */
         const char *params[3];
         params[0] = ent->d_name;
         params[1] = ""; /* pwd_hash: not imported from flat files */
         params[2] = raw;

         PGresult *res = PQexecParams(conn,
                                      "INSERT INTO players (name, pwd_hash, raw_save)"
                                      " VALUES ($1, $2, $3)"
                                      " ON CONFLICT (name)"
                                      " DO UPDATE SET raw_save = EXCLUDED.raw_save",
                                      3, NULL, params, NULL, NULL, 0);
         check_result(conn, res, path);
         PQclear(res);
         free(raw);
         count++;
      }
      closedir(d);
   }
   return count;
}

/* ------------------------------------------------------------------ */
/* Import area list and area files (metadata only — not parsed)         */
/* ------------------------------------------------------------------ */

static int import_area_file_raw(PGconn *conn, const char *area_dir, const char *filename)
{
   char path[1024];
   snprintf(path, sizeof(path), "%s/%s", area_dir, filename);

   long len;
   char *raw = slurp_file(path, &len);
   if (!raw || len == 0)
   {
      free(raw);
      return 0;
   }

   /* Strip .are extension for the area name. */
   char name[256];
   snprintf(name, sizeof(name), "%s", filename);
   char *dot = strrchr(name, '.');
   if (dot)
      *dot = '\0';

   const char *params[2];
   params[0] = name;
   params[1] = raw;

   /* Store raw file content in a temporary staging column for manual review.
    * Full normalised import requires parsing the area file format — use the
    * server's db_boot_from_postgres() path instead for production imports.
    * Here we just record the filename so the area row exists. */
   PGresult *res =
       PQexecParams(conn,
                    "INSERT INTO areas (filename, name, title, author, min_level, max_level,"
                    " first_vnum, last_vnum, reset_msg, flags)"
                    " VALUES ($1, $1, $1, 'imported', 0, 100, 0, 0, 'Area has been reset.', 0)"
                    " ON CONFLICT DO NOTHING",
                    1, NULL, params, NULL, NULL, 0);
   check_result(conn, res, path);
   PQclear(res);
   free(raw);
   return 1;
}

/* ------------------------------------------------------------------ */
/* Import help files                                                    */
/* ------------------------------------------------------------------ */

static int import_help_dir(PGconn *conn, const char *help_dir, int is_shelp)
{
   DIR *d = opendir(help_dir);
   if (!d)
   {
      fprintf(stderr, "import_to_db: cannot open %s\n", help_dir);
      return 0;
   }

   int count = 0;
   struct dirent *ent;
   while ((ent = readdir(d)) != NULL)
   {
      if (ent->d_name[0] == '.')
         continue;

      char path[1024];
      snprintf(path, sizeof(path), "%s/%s", help_dir, ent->d_name);

      struct stat st;
      if (stat(path, &st) != 0 || !S_ISREG(st.st_mode))
         continue;

      long len;
      char *raw = slurp_file(path, &len);
      if (!raw || len == 0)
      {
         free(raw);
         continue;
      }

      /* Parse minimal header: first line is "level keyword~", rest is body. */
      char *p = raw;
      char *nl = strchr(p, '\n');
      char header[256] = {0};
      if (nl)
      {
         size_t hlen = (size_t)(nl - p);
         if (hlen >= sizeof(header))
            hlen = sizeof(header) - 1;
         memcpy(header, p, hlen);
         header[hlen] = '\0';
      }

      int level = 0;
      char keywords[256] = {0};
      sscanf(header, "%d %255[^\n]", &level, keywords);
      /* Strip trailing ~ from keywords */
      char *tilde = strchr(keywords, '~');
      if (tilde)
         *tilde = '\0';

      const char *params[4];
      char level_str[16];
      snprintf(level_str, sizeof(level_str), "%d", level);
      params[0] = keywords[0] ? keywords : ent->d_name;
      params[1] = level_str;
      params[2] = raw;
      params[3] = is_shelp ? "t" : "f";

      const char *sql = is_shelp ? "INSERT INTO shelp_entries (keywords, min_level, body)"
                                   " VALUES ($1, $2, $3)"
                                   " ON CONFLICT DO NOTHING"
                                 : "INSERT INTO help_entries (keywords, min_level, body)"
                                   " VALUES ($1, $2, $3)"
                                   " ON CONFLICT DO NOTHING";
      PGresult *res = PQexecParams(conn, sql, 3, NULL, params, NULL, NULL, 0);
      check_result(conn, res, path);
      PQclear(res);
      free(raw);
      count++;
   }
   closedir(d);
   return count;
}

/* ------------------------------------------------------------------ */
/* Import bans.lst → bans table                                        */
/* ------------------------------------------------------------------ */

/*
 * bans.lst format (written by save_bans()):
 *   #BAN~\n
 *   <0|1>\n          (0 = ban all, 1 = newbie-only ban)
 *   <address>~\n
 *   <banned_by>~\n
 *   ... (repeat)
 *   #END~\n
 */
static int import_bans(PGconn *conn, const char *data_dir)
{
   char path[1024];
   snprintf(path, sizeof(path), "%s/bans.lst", data_dir);

   FILE *fp = fopen(path, "r");
   if (!fp)
   {
      fprintf(stderr, "import_to_db: skipping %s (not found)\n", path);
      return 0;
   }

   /* Clear existing rows so this is a clean replace-all import. */
   PGresult *del = PQexec(conn, "DELETE FROM bans");
   check_result(conn, del, "DELETE FROM bans");
   PQclear(del);

   int count = 0;
   char line[1024];

   while (fgets(line, sizeof(line), fp))
   {
      /* Strip trailing newline. */
      line[strcspn(line, "\r\n")] = '\0';

      /* Strip trailing tilde (record markers end with ~). */
      size_t llen = strlen(line);
      if (llen > 0 && line[llen - 1] == '~')
         line[llen - 1] = '\0';

      if (strcmp(line, "#END") == 0)
         break;

      if (strcmp(line, "#BAN") != 0)
         continue; /* skip blank lines or unrecognised tokens */

      /* Read ban_type (0 or 1). */
      if (!fgets(line, sizeof(line), fp))
         break;
      line[strcspn(line, "\r\n")] = '\0';
      int ban_type = atoi(line);

      /* Read address (tilde-terminated). */
      char address[512] = {0};
      if (!fgets(address, sizeof(address), fp))
         break;
      address[strcspn(address, "\r\n")] = '\0';
      size_t alen = strlen(address);
      if (alen > 0 && address[alen - 1] == '~')
         address[alen - 1] = '\0';

      /* Read banned_by (tilde-terminated). */
      char banned_by[512] = {0};
      if (!fgets(banned_by, sizeof(banned_by), fp))
         break;
      banned_by[strcspn(banned_by, "\r\n")] = '\0';
      size_t blen = strlen(banned_by);
      if (blen > 0 && banned_by[blen - 1] == '~')
         banned_by[blen - 1] = '\0';

      if (address[0] == '\0')
         continue; /* malformed entry */

      char ban_type_str[8];
      snprintf(ban_type_str, sizeof(ban_type_str), "%d", ban_type);

      const char *params[3];
      params[0] = ban_type_str;
      params[1] = address;
      params[2] = banned_by;

      PGresult *res =
          PQexecParams(conn,
                       "INSERT INTO bans (ban_type, address, banned_by)"
                       " VALUES ($1, $2, $3)",
                       3, NULL, params, NULL, NULL, 0);
      check_result(conn, res, "INSERT INTO bans");
      PQclear(res);
      count++;
   }

   fclose(fp);
   return count;
}

/* ------------------------------------------------------------------ */
/* Main                                                                 */
/* ------------------------------------------------------------------ */

static void usage(const char *prog)
{
   fprintf(stderr,
           "Usage: %s <connstr> [--areas] [--help] [--shelp] [--lore]\n"
           "                   [--players] [--data] [--all]\n"
           "\n"
           "  <connstr>   PostgreSQL connection string, e.g.\n"
           "              \"host=localhost dbname=acktng user=mud\"\n"
           "  --areas     Import area metadata (filenames only)\n"
           "  --help      Import help/ directory\n"
           "  --shelp     Import shelp/ directory\n"
           "  --lore      Import lore/ directory (as shelp entries)\n"
           "  --players   Import player/ save files\n"
           "  --data      Import data/ directory (bans, socials, rulers, sysdata)\n"
           "  --all       Import everything\n"
           "\n"
           "Run from the repo root so that area/, help/, shelp/, lore/,\n"
           "data/, and player/ are accessible by relative path.\n",
           prog);
   exit(1);
}

int main(int argc, char **argv)
{
   if (argc < 3)
      usage(argv[0]);

   const char *connstr = argv[1];

   int do_areas = 0, do_help = 0, do_shelp = 0, do_lore = 0;
   int do_players = 0, do_data = 0;

   for (int i = 2; i < argc; i++)
   {
      if (!strcmp(argv[i], "--all"))
      {
         do_areas = do_help = do_shelp = do_lore = do_players = do_data = 1;
      }
      else if (!strcmp(argv[i], "--areas"))
         do_areas = 1;
      else if (!strcmp(argv[i], "--help"))
         do_help = 1;
      else if (!strcmp(argv[i], "--shelp"))
         do_shelp = 1;
      else if (!strcmp(argv[i], "--lore"))
         do_lore = 1;
      else if (!strcmp(argv[i], "--players"))
         do_players = 1;
      else if (!strcmp(argv[i], "--data"))
         do_data = 1;
      else
      {
         fprintf(stderr, "Unknown option: %s\n", argv[i]);
         usage(argv[0]);
      }
   }

   PGconn *conn = PQconnectdb(connstr);
   if (PQstatus(conn) != CONNECTION_OK)
      die(conn, "connection failed");

   PGresult *res = PQexec(conn, "BEGIN");
   check_result(conn, res, "BEGIN");
   PQclear(res);

   int total = 0;

   if (do_areas)
   {
      /* Read area.lst and import each listed area. */
      FILE *fp = fopen("area/area.lst", "r");
      if (!fp)
      {
         fprintf(stderr, "import_to_db: cannot open area/area.lst\n");
      }
      else
      {
         char line[256];
         while (fgets(line, sizeof(line), fp))
         {
            /* Strip newline */
            line[strcspn(line, "\r\n")] = '\0';
            if (!line[0] || line[0] == '$')
               break;
            int n = import_area_file_raw(conn, "area", line);
            total += n;
            if (n)
               printf("  areas: imported %s\n", line);
         }
         fclose(fp);
      }
   }

   if (do_help)
   {
      int n = import_help_dir(conn, "help", 0);
      printf("  help: %d entries imported\n", n);
      total += n;
   }

   if (do_shelp)
   {
      int n = import_help_dir(conn, "shelp", 1);
      printf("  shelp: %d entries imported\n", n);
      total += n;
   }

   if (do_lore)
   {
      int n = import_help_dir(conn, "lore", 1);
      printf("  lore: %d entries imported\n", n);
      total += n;
   }

   if (do_players)
   {
      int n = import_players(conn, "player");
      printf("  players: %d files imported\n", n);
      total += n;
   }

   if (do_data)
   {
      int n = import_bans(conn, "data");
      printf("  data: %d bans imported\n", n);
      total += n;
   }

   res = PQexec(conn, "COMMIT");
   check_result(conn, res, "COMMIT");
   PQclear(res);

   PQfinish(conn);
   printf("import_to_db: done. %d records imported.\n", total);
   return 0;
}

#else /* USE_DB_LOAD */

#include <stdio.h>
int main(void)
{
   fprintf(stderr, "import_to_db: built without USE_DB_LOAD — no-op.\n");
   return 1;
}

#endif /* USE_DB_LOAD */
