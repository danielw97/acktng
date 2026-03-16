#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "config.h"
#define DEC_GLOBALS_H 1
#include "ack.h"

void init_string_space(void);

bool fBootDb = FALSE;

void tail_chain(void)
{
}

void bugf(char *fmt, ...)
{
   (void)fmt;
}

void log_f(char *fmt, ...)
{
   (void)fmt;
}

static FILE *open_repeated_stream(char fill, int count, int terminator)
{
   FILE *fp = tmpfile();
   int i;

   assert(fp != NULL);
   for (i = 0; i < count; i++)
      fputc(fill, fp);
   fputc(terminator, fp);
   rewind(fp);
   return fp;
}

static void test_fread_string_truncates_safely(void)
{
   FILE *fp = open_repeated_stream('a', (MAX_STRING_LENGTH * 4) + 512, '~');
   char *value;
   size_t max_len = (MAX_STRING_LENGTH * 4) - 2;

   value = _fread_string(fp, "test_fread_string_truncates_safely");

   assert(value != NULL);
   assert(strlen(value) == max_len);
   assert(value[0] == 'a');
   assert(value[max_len - 1] == 'a');

   _free_string(value, "test");
   fclose(fp);
}

static void test_fread_string_eol_truncates_safely(void)
{
   FILE *fp = open_repeated_stream('b', (MAX_STRING_LENGTH * 4) + 700, '\n');
   char *value;
   size_t max_len = (MAX_STRING_LENGTH * 4) - 1;

   value = _fread_string_eol(fp, "test_fread_string_eol_truncates_safely");

   assert(value != NULL);
   assert(strlen(value) == max_len);
   assert(value[0] == 'b');
   assert(value[max_len - 1] == 'b');

   _free_string(value, "test");
   fclose(fp);
}

int main(void)
{
   init_string_space();

   test_fread_string_truncates_safely();
   test_fread_string_eol_truncates_safely();

   puts("test_ssm: all tests passed");
   return 0;
}
