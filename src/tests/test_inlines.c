#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#include "config.h"
#define DEC_GLOBALS_H 1
#include "ack.h"

/* ── UMIN ─────────────────────────────────────────────────────────────── */

static void test_umin(void)
{
   assert(UMIN(3, 5) == 3);
   assert(UMIN(5, 3) == 3);
   assert(UMIN(4, 4) == 4);
   assert(UMIN(-1, 0) == -1);
   assert(UMIN(0, -1) == -1);
   printf("PASS: test_umin\n");
}

/* ── UMAX ─────────────────────────────────────────────────────────────── */

static void test_umax(void)
{
   assert(UMAX(3, 5) == 5);
   assert(UMAX(5, 3) == 5);
   assert(UMAX(4, 4) == 4);
   assert(UMAX(-1, 0) == 0);
   assert(UMAX(0, -1) == 0);
   printf("PASS: test_umax\n");
}

/* ── URANGE ───────────────────────────────────────────────────────────── */

static void test_urange(void)
{
   /* clamp low */
   assert(URANGE(5, 3, 10) == 5);
   /* in range */
   assert(URANGE(5, 7, 10) == 7);
   /* clamp high */
   assert(URANGE(5, 12, 10) == 10);
   /* exact boundaries */
   assert(URANGE(5, 5, 10) == 5);
   assert(URANGE(5, 10, 10) == 10);
   printf("PASS: test_urange\n");
}

/* ── LOWER ────────────────────────────────────────────────────────────── */

static void test_lower(void)
{
   assert(LOWER('A') == 'a');
   assert(LOWER('Z') == 'z');
   assert(LOWER('M') == 'm');
   /* already lowercase: unchanged */
   assert(LOWER('a') == 'a');
   assert(LOWER('z') == 'z');
   /* non-alpha: unchanged */
   assert(LOWER('5') == '5');
   assert(LOWER(' ') == ' ');
   assert(LOWER('!') == '!');
   printf("PASS: test_lower\n");
}

/* ── UPPER ────────────────────────────────────────────────────────────── */

static void test_upper(void)
{
   assert(UPPER('a') == 'A');
   assert(UPPER('z') == 'Z');
   assert(UPPER('m') == 'M');
   /* already uppercase: unchanged */
   assert(UPPER('A') == 'A');
   assert(UPPER('Z') == 'Z');
   /* non-alpha: unchanged */
   assert(UPPER('5') == '5');
   assert(UPPER(' ') == ' ');
   assert(UPPER('!') == '!');
   printf("PASS: test_upper\n");
}

/* ── IS_LETTER ────────────────────────────────────────────────────────── */

static void test_is_letter(void)
{
   assert(IS_LETTER('a'));
   assert(IS_LETTER('z'));
   assert(IS_LETTER('A'));
   assert(IS_LETTER('Z'));
   assert(IS_LETTER('m'));
   assert(IS_LETTER('M'));
   assert(!IS_LETTER('0'));
   assert(!IS_LETTER('9'));
   assert(!IS_LETTER(' '));
   assert(!IS_LETTER('!'));
   assert(!IS_LETTER('\0'));
   printf("PASS: test_is_letter\n");
}

int main(void)
{
   test_umin();
   test_umax();
   test_urange();
   test_lower();
   test_upper();
   test_is_letter();
   printf("All inlines tests passed.\n");
   return 0;
}
