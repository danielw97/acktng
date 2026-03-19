#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define DEC_GLOBALS_H 1
#include "ack.h"

/* Minimal fread_number matching db.c implementation */
static long test_fread_number(FILE *fp)
{
   long number;
   int sign;
   char c;

   do
   {
      c = getc(fp);
   } while (isspace(c));

   number = 0;
   sign = 0;
   if (c == '+')
      c = getc(fp);
   else if (c == '-')
   {
      sign = 1;
      c = getc(fp);
   }

   assert(isdigit(c));

   while (isdigit(c))
   {
      number = number * 10 + c - '0';
      c = getc(fp);
   }

   if (sign)
      number = 0 - number;

   if (c == '|')
      number += test_fread_number(fp);
   else if (c != ' ')
      ungetc(c, fp);

   return number;
}

/* Write clandata in the same format as save_clan_table() */
static void write_clandata(const char *path, int max_clan, POL_DATA *pd)
{
   FILE *fp = fopen(path, "w");
   assert(fp != NULL);
   int x, y;

   fprintf(fp, "%d\n", max_clan);

   for (x = 1; x < max_clan; x++)
   {
      for (y = 1; y < max_clan; y++)
         fprintf(fp, "  %5d  ", pd->diplomacy[x][y]);
      fprintf(fp, "\n");
   }

   for (x = 1; x < max_clan; x++)
      fprintf(fp, "%li\n", pd->treasury[x]);

   for (x = 1; x < max_clan; x++)
   {
      for (y = 1; y < max_clan; y++)
         fprintf(fp, "  %5d  ", pd->end_current_state[x][y]);
      fprintf(fp, "\n");
   }

   fclose(fp);
}

/* Read clandata in the same format as the load code in db.c.
 * Returns 0 on success, -1 on header mismatch. */
static int read_clandata(const char *path, int expected_max_clan, POL_DATA *pd)
{
   FILE *fp = fopen(path, "r");
   assert(fp != NULL);
   int x, y;

   int file_max_clan = (int)test_fread_number(fp);
   if (file_max_clan != expected_max_clan)
   {
      fclose(fp);
      return -1;
   }

   for (x = 1; x < expected_max_clan; x++)
      for (y = 1; y < expected_max_clan; y++)
         pd->diplomacy[x][y] = (sh_int)test_fread_number(fp);

   for (x = 1; x < expected_max_clan; x++)
      pd->treasury[x] = test_fread_number(fp);

   for (x = 1; x < expected_max_clan; x++)
      for (y = 1; y < expected_max_clan; y++)
         pd->end_current_state[x][y] = (sh_int)test_fread_number(fp);

   fclose(fp);
   return 0;
}

static const char *TEST_FILE = "/tmp/test_clandata.dat";

static void test_roundtrip_zeros(void)
{
   POL_DATA pd_write;
   POL_DATA pd_read;
   memset(&pd_write, 0, sizeof(pd_write));
   memset(&pd_read, 0, sizeof(pd_read));

   write_clandata(TEST_FILE, MAX_CLAN, &pd_write);
   int rc = read_clandata(TEST_FILE, MAX_CLAN, &pd_read);
   assert(rc == 0);
   assert(memcmp(&pd_write, &pd_read, sizeof(POL_DATA)) == 0);

   remove(TEST_FILE);
   printf("  PASS: round-trip with all zeros\n");
}

static void test_roundtrip_nonzero_data(void)
{
   POL_DATA pd_write;
   POL_DATA pd_read;
   memset(&pd_write, 0, sizeof(pd_write));
   memset(&pd_read, 0, sizeof(pd_read));

   /* Set some non-zero values */
   pd_write.diplomacy[1][2] = 100;
   pd_write.diplomacy[3][4] = -200;
   pd_write.diplomacy[MAX_CLAN - 1][MAX_CLAN - 1] = 500;
   pd_write.treasury[1] = 10000;
   pd_write.treasury[MAX_CLAN - 1] = 99999;
   pd_write.end_current_state[2][3] = 1;
   pd_write.end_current_state[MAX_CLAN - 1][1] = 1;

   write_clandata(TEST_FILE, MAX_CLAN, &pd_write);
   int rc = read_clandata(TEST_FILE, MAX_CLAN, &pd_read);
   assert(rc == 0);

   assert(pd_read.diplomacy[1][2] == 100);
   assert(pd_read.diplomacy[3][4] == -200);
   assert(pd_read.diplomacy[MAX_CLAN - 1][MAX_CLAN - 1] == 500);
   assert(pd_read.treasury[1] == 10000);
   assert(pd_read.treasury[MAX_CLAN - 1] == 99999);
   assert(pd_read.end_current_state[2][3] == 1);
   assert(pd_read.end_current_state[MAX_CLAN - 1][1] == 1);

   remove(TEST_FILE);
   printf("  PASS: round-trip with non-zero data\n");
}

static void test_header_mismatch_detected(void)
{
   POL_DATA pd_write;
   POL_DATA pd_read;
   memset(&pd_write, 0, sizeof(pd_write));
   memset(&pd_read, 0, sizeof(pd_read));

   /* Write with a different MAX_CLAN than current */
   write_clandata(TEST_FILE, 9, &pd_write);

   /* Try to read with current MAX_CLAN — should fail */
   int rc = read_clandata(TEST_FILE, MAX_CLAN, &pd_read);
   assert(rc == -1);

   remove(TEST_FILE);
   printf("  PASS: header mismatch detected (file=9, code=%d)\n", MAX_CLAN);
}

static void test_file_has_header(void)
{
   POL_DATA pd;
   memset(&pd, 0, sizeof(pd));

   write_clandata(TEST_FILE, MAX_CLAN, &pd);

   /* First value in the file should be MAX_CLAN */
   FILE *fp = fopen(TEST_FILE, "r");
   assert(fp != NULL);
   long header = test_fread_number(fp);
   assert(header == MAX_CLAN);
   fclose(fp);

   remove(TEST_FILE);
   printf("  PASS: file starts with MAX_CLAN header (%d)\n", MAX_CLAN);
}

static void test_value_count(void)
{
   POL_DATA pd;
   memset(&pd, 0, sizeof(pd));

   write_clandata(TEST_FILE, MAX_CLAN, &pd);

   /* Count all numbers in the file */
   FILE *fp = fopen(TEST_FILE, "r");
   assert(fp != NULL);
   int count = 0;
   int c;
   int in_number = 0;

   while ((c = fgetc(fp)) != EOF)
   {
      if (isdigit(c) || (c == '-' && !in_number))
         in_number = 1;
      else if (in_number && !isdigit(c))
      {
         count++;
         in_number = 0;
      }
   }
   if (in_number)
      count++;
   fclose(fp);

   /* Expected: 1 header + (MAX_CLAN-1)^2 diplomacy + (MAX_CLAN-1) treasury + (MAX_CLAN-1)^2 end_state */
   int n = MAX_CLAN - 1;
   int expected = 1 + n * n + n + n * n;
   assert(count == expected);

   remove(TEST_FILE);
   printf("  PASS: file contains exactly %d values (1 header + %d diplomacy + %d treasury + %d end_state)\n", expected,
          n * n, n, n * n);
}

int main(void)
{
   printf("test_clandata: running tests...\n");

   test_file_has_header();
   test_roundtrip_zeros();
   test_roundtrip_nonzero_data();
   test_header_mismatch_detected();
   test_value_count();

   printf("test_clandata: all tests passed!\n");
   return 0;
}
