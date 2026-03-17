#include <assert.h>
#include <ctype.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"

static int has_invalid_filename_chars(const char *name)
{
   const unsigned char *p = (const unsigned char *)name;
   for (; *p; p++)
   {
      if (!(isalnum(*p) || *p == '.' || *p == '_' || *p == '-'))
         return 1;
   }
   return 0;
}

static void validate_entry_header(FILE *fp, char *line, size_t line_size)
{
   long level;
   char extra;

   assert(sscanf(line, "level %ld %c", &level, &extra) == 1);
   (void)level;

   assert(fgets(line, line_size, fp) != NULL);
   assert(strncmp(line, "keywords ", 9) == 0);
   {
      const char *keywords = line + 9;
      size_t keyword_len = strlen(keywords);
      assert(*keywords != '\0' && *keywords != '\n' && *keywords != '\r');
      while (keyword_len > 0 &&
             (keywords[keyword_len - 1] == '\n' || keywords[keyword_len - 1] == '\r'))
      {
         keyword_len--;
      }
      assert(keyword_len > 0);
      assert(!isspace((unsigned char)keywords[0]));
      assert(!isspace((unsigned char)keywords[keyword_len - 1]));
   }

   assert(fgets(line, line_size, fp) != NULL);
   if (strncmp(line, "flags ", 6) == 0)
   {
      /* Named flags (e.g., "flags MIDGAARD KIESS") or numeric flags are both valid */
      assert(strlen(line) > 6);
      assert(fgets(line, line_size, fp) != NULL);
   }
   assert(strcmp(line, "---\n") == 0 || strcmp(line, "---\r\n") == 0 || strcmp(line, "---") == 0);
}

static void validate_help_file(const char *path, int multi_entry)
{
   FILE *fp = fopen(path, "r");
   char line[8192];
   size_t i;

   assert(fp != NULL);

   /* Read first entry header */
   assert(fgets(line, sizeof(line), fp) != NULL);
   validate_entry_header(fp, line, sizeof(line));

   /* Read text lines, watching for additional entry headers (lore only) */
   while (fgets(line, sizeof(line), fp) != NULL)
   {
      {
         long lvl;
         char xtra;
         if (multi_entry && strncmp(line, "level ", 6) == 0 &&
             sscanf(line, "level %ld %c", &lvl, &xtra) == 1)
         {
            /* Start of a new entry within the same file */
            validate_entry_header(fp, line, sizeof(line));
            continue;
         }
      }
      i = strlen(line);
      assert(i < sizeof(line) - 1 || line[i - 1] == '\n');
   }

   fclose(fp);
}

static int validate_help_directory(const char *directory, int multi_entry)
{
   DIR *dir = opendir(directory);
   struct dirent *entry;
   int files_seen = 0;

   assert(dir != NULL);

   while ((entry = readdir(dir)) != NULL)
   {
      char path[4096];
      const char *name = entry->d_name;
      size_t len;

      if (name[0] == '.')
         continue;

      len = strlen(name);
      assert(len > 0);
      if (name[len - 1] == '~')
         continue;

      assert(has_invalid_filename_chars(name) == 0);

      snprintf(path, sizeof(path), "%s%s", directory, name);
      validate_help_file(path, multi_entry);
      files_seen++;
   }

   closedir(dir);
   return files_seen;
}

int main(void)
{
   int help_files = validate_help_directory(HELP_DIR, 0);
   int shelp_files = validate_help_directory(SHELP_DIR, 0);
   int lore_files = validate_help_directory(LORE_DIR, 1);

   assert(help_files > 0);
   assert(shelp_files > 0);
   assert(lore_files > 0);

   return 0;
}
