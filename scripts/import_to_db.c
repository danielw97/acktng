/* import_to_db.c — ACK!TNG flat-file → PostgreSQL migration tool.
 *
 * Standalone binary (not linked with the server).  Reads all six content
 * stores from the filesystem and inserts them into PostgreSQL.
 *
 * Usage:
 *   ./tools/import_to_db [connstr]
 *
 * connstr defaults to the contents of data/db.conf (relative to the area/
 * directory, which is the CWD at runtime).  If absent, libpq PG* env vars
 * are used as a fallback.
 *
 * Build: make tools/import_to_db  (from src/)
 */

#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include <libpq-fe.h>

/* -----------------------------------------------------------------------
 * Connection helpers
 * ----------------------------------------------------------------------- */

static PGconn *conn;
static int errors;

static int exec_sql(const char *sql)
{
   PGresult *res = PQexec(conn, sql);
   int ok = (PQresultStatus(res) == PGRES_COMMAND_OK || PQresultStatus(res) == PGRES_TUPLES_OK);
   if (!ok)
      fprintf(stderr, "SQL error: %s\n--- %s\n", PQresultErrorMessage(res), sql);
   PQclear(res);
   return ok;
}

static int exec_params(const char *sql, int n, const char *const *vals)
{
   PGresult *res = PQexecParams(conn, sql, n, NULL, vals, NULL, NULL, 0);
   ExecStatusType s = PQresultStatus(res);
   int ok = (s == PGRES_COMMAND_OK || s == PGRES_TUPLES_OK);
   if (!ok)
   {
      fprintf(stderr, "ERROR [params]: %s\n", PQresultErrorMessage(res));
      errors++;
   }
   PQclear(res);
   return ok;
}

/* -----------------------------------------------------------------------
 * File utilities
 * ----------------------------------------------------------------------- */

/* Read an entire file into a heap buffer.  Caller must free().
 * Returns NULL on error. */
static char *read_file(const char *path)
{
   FILE *fp;
   long size;
   char *buf;

   fp = fopen(path, "r");
   if (!fp)
      return NULL;
   fseek(fp, 0, SEEK_END);
   size = ftell(fp);
   rewind(fp);
   buf = malloc((size_t)size + 1);
   if (!buf)
   {
      fclose(fp);
      return NULL;
   }
   size = (long)fread(buf, 1, (size_t)size, fp);
   buf[size] = '\0';
   fclose(fp);
   return buf;
}

/* Strip trailing newline / carriage return from s in-place. */
static void strip_trailing(char *s)
{
   size_t n = strlen(s);
   while (n > 0 && (s[n - 1] == '\n' || s[n - 1] == '\r' || s[n - 1] == ' ' || s[n - 1] == '\t'))
      s[--n] = '\0';
}

/* -----------------------------------------------------------------------
 * Help / shelp file importer
 *
 * File format (one entry per file):
 *   level <N>
 *   keywords <words...>
 *   ---
 *   <body text>
 * ----------------------------------------------------------------------- */

static int import_one_helpfile(const char *path, const char *filename, const char *table)
{
   char *buf;
   char *p;
   char *line_end;
   char level_str[32] = "0";
   char keywords[256] = "";
   char *body_start;
   int in_header;
   const char *vals[4];
   char lev_copy[32];

   buf = read_file(path);
   if (!buf)
   {
      fprintf(stderr, "  WARN: cannot read %s\n", path);
      return 0;
   }

   /* Parse header lines until "---" */
   p = buf;
   in_header = 1;
   while (in_header && *p)
   {
      char *eol;
      line_end = strchr(p, '\n');
      eol = line_end ? line_end : p + strlen(p);
      *eol = '\0';
      strip_trailing(p);

      if (strncmp(p, "level ", 6) == 0)
      {
         strncpy(level_str, p + 6, sizeof(level_str) - 1);
         strip_trailing(level_str);
      }
      else if (strncmp(p, "keywords ", 9) == 0)
      {
         strncpy(keywords, p + 9, sizeof(keywords) - 1);
         strip_trailing(keywords);
      }
      else if (strcmp(p, "---") == 0)
      {
         in_header = 0;
         if (line_end)
            *line_end = '\n'; /* restore for body extraction */
         p = line_end ? line_end + 1 : eol;
         break;
      }

      if (line_end)
         *line_end = '\n';
      p = line_end ? line_end + 1 : eol;
   }

   body_start = p;
   /* body_start points to the first line after "---" */

   strncpy(lev_copy, level_str, sizeof(lev_copy) - 1);

   vals[0] = filename;
   vals[1] = lev_copy;
   vals[2] = keywords;
   vals[3] = body_start;

   {
      char sql[256];
      snprintf(sql, sizeof(sql),
               "INSERT INTO %s (filename, level, keywords, body) "
               "VALUES ($1, $2, $3, $4) "
               "ON CONFLICT (filename) DO UPDATE SET "
               "level = EXCLUDED.level, "
               "keywords = EXCLUDED.keywords, "
               "body = EXCLUDED.body",
               table);
      exec_params(sql, 4, vals);
   }

   free(buf);
   return 1;
}

static int import_helpdir(const char *dirpath, const char *table)
{
   DIR *dp;
   struct dirent *de;
   int count = 0;
   char path[512];

   dp = opendir(dirpath);
   if (!dp)
   {
      fprintf(stderr, "WARN: cannot open %s: %s\n", dirpath, strerror(errno));
      return 0;
   }

   exec_sql("BEGIN");
   while ((de = readdir(dp)) != NULL)
   {
      struct stat st;
      if (de->d_name[0] == '.')
         continue;
      snprintf(path, sizeof(path), "%s/%s", dirpath, de->d_name);
      if (stat(path, &st) != 0 || S_ISDIR(st.st_mode))
         continue;
      import_one_helpfile(path, de->d_name, table);
      count++;
   }
   exec_sql("COMMIT");
   closedir(dp);
   return count;
}

/* -----------------------------------------------------------------------
 * Lore file importer
 *
 * File format:
 *   keywords <words...>
 *   ---
 *   <default body>
 *   [flags <N>
 *   ---
 *   <flagged body>]  (may repeat)
 * ----------------------------------------------------------------------- */

static int import_one_lorefile(const char *path, const char *filename)
{
   char *buf;
   char *p;
   char *line_end;
   char keywords[512] = "";
   char topic_id_str[32];
   int seq = 1;
   int in_header = 1;
   int in_entry = 0;
   char flags_str[64] = "0";
   char *entry_start = NULL;
   PGresult *res;
   const char *vals[3];

   buf = read_file(path);
   if (!buf)
   {
      fprintf(stderr, "  WARN: cannot read lore %s\n", path);
      return 0;
   }

   /* Insert or get topic row */
   p = buf;
   while (in_header && *p)
   {
      char *eol;
      line_end = strchr(p, '\n');
      eol = line_end ? line_end : p + strlen(p);
      *eol = '\0';
      strip_trailing(p);
      if (strncmp(p, "keywords ", 9) == 0)
      {
         strncpy(keywords, p + 9, sizeof(keywords) - 1);
         strip_trailing(keywords);
      }
      else if (strcmp(p, "---") == 0)
      {
         in_header = 0;
         if (line_end)
            *line_end = '\n';
         p = line_end ? line_end + 1 : eol;
         break;
      }
      if (line_end)
         *line_end = '\n';
      p = line_end ? line_end + 1 : eol;
   }

   /* Upsert topic */
   vals[0] = filename;
   vals[1] = keywords;
   res = PQexecParams(conn,
                      "INSERT INTO lore_topics (filename, keywords) "
                      "VALUES ($1, $2) "
                      "ON CONFLICT (filename) DO UPDATE SET "
                      "keywords = EXCLUDED.keywords "
                      "RETURNING id",
                      2, NULL, vals, NULL, NULL, 0);
   if (PQresultStatus(res) != PGRES_TUPLES_OK || PQntuples(res) == 0)
   {
      fprintf(stderr, "ERROR: lore_topics upsert failed for %s: %s\n", filename,
              PQresultErrorMessage(res));
      PQclear(res);
      free(buf);
      errors++;
      return 0;
   }
   strncpy(topic_id_str, PQgetvalue(res, 0, 0), sizeof(topic_id_str) - 1);
   PQclear(res);

   /* Parse entries (default + flagged) */
   in_entry = 1;
   entry_start = p;
   strncpy(flags_str, "0", sizeof(flags_str) - 1);
   seq = 1;

   /* Collect the rest of the file into segments split by "flags <N>\n---\n" */
   while (*p)
   {
      char *seg_end;
      char *next_p;
      char line_buf[256];

      line_end = strchr(p, '\n');
      if (!line_end)
      {
         /* Last line without newline */
         break;
      }
      size_t linelen = (size_t)(line_end - p);
      if (linelen >= sizeof(line_buf))
         linelen = sizeof(line_buf) - 1;
      memcpy(line_buf, p, linelen);
      line_buf[linelen] = '\0';
      strip_trailing(line_buf);

      if (strncmp(line_buf, "flags ", 6) == 0)
      {
         /* We're about to start a new flagged entry.
          * First, flush the current entry. */
         seg_end = p; /* body ends just before "flags" line */
         if (in_entry && entry_start && entry_start < seg_end)
         {
            char *body = malloc((size_t)(seg_end - entry_start) + 1);
            if (body)
            {
               memcpy(body, entry_start, (size_t)(seg_end - entry_start));
               body[seg_end - entry_start] = '\0';
               strip_trailing(body);
               {
                  char seq_str[16];
                  snprintf(seq_str, sizeof(seq_str), "%d", seq);
                  vals[0] = topic_id_str;
                  vals[1] = seq_str;
                  vals[2] = flags_str;
                  /* 4th param for body */
                  {
                     const char *v4[4];
                     v4[0] = topic_id_str;
                     v4[1] = seq_str;
                     v4[2] = flags_str;
                     v4[3] = body;
                     exec_params("INSERT INTO lore_entries "
                                 "(topic_id, seq, flags, body) "
                                 "VALUES ($1, $2, $3, $4) "
                                 "ON CONFLICT (topic_id, seq) DO UPDATE SET "
                                 "flags = EXCLUDED.flags, "
                                 "body = EXCLUDED.body",
                                 4, v4);
                  }
                  seq++;
               }
               free(body);
            }
         }
         /* Parse new flags value */
         strncpy(flags_str, line_buf + 6, sizeof(flags_str) - 1);
         strip_trailing(flags_str);
         /* Skip past the following "---" line */
         next_p = line_end + 1;
         line_end = strchr(next_p, '\n');
         if (line_end)
            next_p = line_end + 1;
         entry_start = next_p;
         p = next_p;
         in_entry = 1;
         continue;
      }

      p = line_end + 1;
   }

   /* Flush last entry */
   if (in_entry && entry_start)
   {
      char *body = strdup(entry_start);
      if (body)
      {
         strip_trailing(body);
         char seq_str[16];
         snprintf(seq_str, sizeof(seq_str), "%d", seq);
         const char *v4[4];
         v4[0] = topic_id_str;
         v4[1] = seq_str;
         v4[2] = flags_str;
         v4[3] = body;
         exec_params("INSERT INTO lore_entries (topic_id, seq, flags, body) "
                     "VALUES ($1, $2, $3, $4) "
                     "ON CONFLICT (topic_id, seq) DO UPDATE SET "
                     "flags = EXCLUDED.flags, "
                     "body = EXCLUDED.body",
                     4, v4);
         free(body);
      }
   }

   free(buf);
   return 1;
}

static int import_loredir(const char *dirpath)
{
   DIR *dp;
   struct dirent *de;
   int count = 0;
   char path[512];

   dp = opendir(dirpath);
   if (!dp)
   {
      fprintf(stderr, "WARN: cannot open lore dir %s: %s\n", dirpath, strerror(errno));
      return 0;
   }

   exec_sql("BEGIN");
   while ((de = readdir(dp)) != NULL)
   {
      struct stat st;
      if (de->d_name[0] == '.')
         continue;
      snprintf(path, sizeof(path), "%s/%s", dirpath, de->d_name);
      if (stat(path, &st) != 0 || S_ISDIR(st.st_mode))
         continue;
      import_one_lorefile(path, de->d_name);
      count++;
   }
   exec_sql("COMMIT");
   closedir(dp);
   return count;
}

/* -----------------------------------------------------------------------
 * Area file importer (stubs for complex sections)
 *
 * For Phase 1 we import only the #AREA header row.  Subsequent phases
 * add rooms, mobiles, objects, resets, etc.
 * ----------------------------------------------------------------------- */

static int import_area_header(const char *path, int *area_db_id_out)
{
   FILE *fp;
   char line[512];
   char name[256] = "";
   int min_vnum = 0, max_vnum = 0;
   char keyword[256] = "";
   char level_label[64] = "";
   char owner[128] = "";
   int area_number = 0;
   int found_area = 0;
   PGresult *res;

   fp = fopen(path, "r");
   if (!fp)
      return 0;

   while (fgets(line, sizeof(line), fp))
   {
      /* Find #AREA section */
      if (strncmp(line, "#AREA", 5) == 0)
      {
         found_area = 1;
         continue;
      }
      if (!found_area)
         continue;
      /* End of AREA section */
      if (line[0] == '#')
         break;

      strip_trailing(line);

      if (strncmp(line, "N ", 2) == 0)
         sscanf(line + 2, "%d", &area_number);
      else if (strncmp(line, "K ", 2) == 0)
         strncpy(keyword, line + 2, sizeof(keyword) - 1);
      else if (strncmp(line, "L ", 2) == 0)
         strncpy(level_label, line + 2, sizeof(level_label) - 1);
      else if (strncmp(line, "O ", 2) == 0)
         strncpy(owner, line + 2, sizeof(owner) - 1);
      else if (strncmp(line, "V ", 2) == 0)
         sscanf(line + 2, "%d %d", &min_vnum, &max_vnum);
      else if (strncmp(line, "S ", 2) == 0)
      {
         /* Name line: "S <name>~" */
         char *tilde;
         strncpy(name, line + 2, sizeof(name) - 1);
         tilde = strchr(name, '~');
         if (tilde)
            *tilde = '\0';
      }
   }
   fclose(fp);

   if (!found_area || min_vnum == 0)
      return 0;

   {
      char min_str[16], max_str[16], num_str[16];
      const char *vals[7];
      snprintf(min_str, sizeof(min_str), "%d", min_vnum);
      snprintf(max_str, sizeof(max_str), "%d", max_vnum);
      snprintf(num_str, sizeof(num_str), "%d", area_number);
      vals[0] = name;
      vals[1] = min_str;
      vals[2] = max_str;
      vals[3] = keyword;
      vals[4] = level_label;
      vals[5] = num_str;
      vals[6] = owner;
      res = PQexecParams(conn,
                         "INSERT INTO areas (name, min_vnum, max_vnum, keyword, "
                         "level_label, area_number, owner) "
                         "VALUES ($1, $2, $3, $4, $5, $6, $7) "
                         "ON CONFLICT (min_vnum) DO UPDATE SET "
                         "name = EXCLUDED.name, "
                         "max_vnum = EXCLUDED.max_vnum, "
                         "keyword = EXCLUDED.keyword, "
                         "level_label = EXCLUDED.level_label, "
                         "area_number = EXCLUDED.area_number, "
                         "owner = EXCLUDED.owner "
                         "RETURNING id",
                         7, NULL, vals, NULL, NULL, 0);
      if (PQresultStatus(res) != PGRES_TUPLES_OK || PQntuples(res) == 0)
      {
         fprintf(stderr, "ERROR: areas insert failed for %s: %s\n", path,
                 PQresultErrorMessage(res));
         PQclear(res);
         errors++;
         return 0;
      }
      if (area_db_id_out)
         *area_db_id_out = atoi(PQgetvalue(res, 0, 0));
      PQclear(res);
   }
   return 1;
}

static int import_areas(const char *area_lst_path, const char *area_dir)
{
   FILE *fp;
   char line[256];
   int count = 0;
   char path[512];

   fp = fopen(area_lst_path, "r");
   if (!fp)
   {
      fprintf(stderr, "WARN: cannot open %s\n", area_lst_path);
      return 0;
   }

   exec_sql("BEGIN");
   while (fgets(line, sizeof(line), fp))
   {
      strip_trailing(line);
      if (line[0] == '\0' || line[0] == '$')
         break;
      snprintf(path, sizeof(path), "%s/%s", area_dir, line);
      if (import_area_header(path, NULL))
      {
         printf("  area: %s\n", line);
         count++;
      }
      else
      {
         fprintf(stderr, "  WARN: no #AREA header in %s\n", line);
      }
   }
   exec_sql("COMMIT");
   fclose(fp);
   return count;
}

/* -----------------------------------------------------------------------
 * Connection string helpers
 * ----------------------------------------------------------------------- */

static char *read_db_conf(const char *base_dir)
{
   char path[512];
   FILE *fp;
   char buf[1024];
   size_t n;

   snprintf(path, sizeof(path), "%s/../data/db.conf", base_dir);
   fp = fopen(path, "r");
   if (!fp)
      return NULL;
   n = fread(buf, 1, sizeof(buf) - 1, fp);
   fclose(fp);
   if (!n)
      return NULL;
   buf[n] = '\0';
   strip_trailing(buf);
   return strdup(buf);
}

/* -----------------------------------------------------------------------
 * main
 * ----------------------------------------------------------------------- */

int main(int argc, char *argv[])
{
   char *connstr = NULL;
   const char *area_dir = "../area";
   int n;

   printf("import_to_db — ACK!TNG database migration tool\n");

   if (argc > 1)
   {
      connstr = argv[1];
   }
   else
   {
      connstr = read_db_conf(area_dir);
   }

   conn = PQconnectdb(connstr ? connstr : "");
   if (connstr && argc <= 1)
      free(connstr);

   if (PQstatus(conn) != CONNECTION_OK)
   {
      fprintf(stderr, "Connection failed: %s\n", PQerrorMessage(conn));
      PQfinish(conn);
      return 1;
   }
   printf("Connected to database.\n");

   /* --- help files --- */
   printf("\nImporting help files...\n");
   n = import_helpdir("../help", "help_entries");
   printf("  %d help files imported.\n", n);

   /* --- shelp files --- */
   printf("\nImporting shelp files...\n");
   n = import_helpdir("../shelp", "shelp_entries");
   printf("  %d shelp files imported.\n", n);

   /* --- lore files --- */
   printf("\nImporting lore files...\n");
   n = import_loredir("../lore");
   printf("  %d lore files imported.\n", n);

   /* --- area headers --- */
   printf("\nImporting area headers from area.lst...\n");
   n = import_areas("../area/area.lst", "../area");
   printf("  %d area headers imported.\n", n);

   if (errors > 0)
      printf("\n%d error(s) encountered — check stderr output above.\n", errors);
   else
      printf("\nAll imports completed successfully.\n");

   PQfinish(conn);
   return errors > 0 ? 1 : 0;
}
