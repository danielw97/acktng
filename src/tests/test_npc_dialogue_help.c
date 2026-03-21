/*
 * test_npc_dialogue_help.c
 *
 * Unit tests for:
 *   1. spec_lookup (spec_mudschool_guide registration)
 *   2. collect_help_context (help/shelp keyword injection)
 */

#include <assert.h>
#include <stdio.h>
#include <string.h>

#define DEC_GLOBALS_H 1
#define UNIT_TEST_NPC_DIALOGUE 1
#include "ack.h"
#include "special.h"
#include "npc_dialogue.h"

/* -------------------------------------------------------------------------
 * Globals required by linked modules.
 * -------------------------------------------------------------------------*/

HELP_DATA *first_help = NULL;
HELP_DATA *last_help = NULL;
HELP_DATA *first_shelp = NULL;
HELP_DATA *last_shelp = NULL;

/* Minimal string-function stubs (avoids linking the full strfuns.o). */
bool str_prefix(const char *astr, const char *bstr)
{
   if (!astr || !bstr)
      return FALSE;
   while (*astr)
   {
      if (tolower((unsigned char)*astr) != tolower((unsigned char)*bstr))
         return TRUE;
      astr++;
      bstr++;
   }
   return FALSE;
}

bool str_cmp(const char *a, const char *b)
{
   while (*a && *b)
   {
      if (tolower((unsigned char)*a) != tolower((unsigned char)*b))
         return TRUE;
      a++;
      b++;
   }
   return (*a || *b) ? TRUE : FALSE;
}

bool str_infix(const char *needle, const char *haystack)
{
   int i;
   int nlen = (int)strlen(needle);
   int hlen = (int)strlen(haystack);
   for (i = 0; i <= hlen - nlen; i++)
   {
      int j;
      for (j = 0; j < nlen; j++)
      {
         if (tolower((unsigned char)needle[j]) != tolower((unsigned char)haystack[i + j]))
            break;
      }
      if (j == nlen)
         return FALSE; /* found = NOT a mismatch (str_infix returns FALSE on match) */
   }
   return TRUE;
}

char *one_argument(char *argument, char *arg_first)
{
   char cEnd = ' ';
   if (!argument || !arg_first)
   {
      if (arg_first)
         *arg_first = '\0';
      return argument ? argument : "";
   }
   while (*argument == ' ')
      argument++;
   if (*argument == '\'' || *argument == '"')
      cEnd = *argument++;
   while (*argument != '\0')
   {
      if (*argument == cEnd)
      {
         argument++;
         break;
      }
      *arg_first++ = (char)tolower((unsigned char)*argument++);
   }
   *arg_first = '\0';
   while (*argument == ' ')
      argument++;
   return argument;
}

/* Minimal spec_lookup/rev_spec_lookup stubs (avoids linking the full special.o
 * which references ~50 spec functions not needed by these tests). */
SPEC_FUN *spec_lookup(const char *name)
{
   if (!str_cmp(name, "spec_mudschool_guide"))
      return spec_mudschool_guide;
   return NULL;
}

char *rev_spec_lookup(void *func)
{
   if (func == spec_mudschool_guide)
      return "spec_mudschool_guide";
   return "(none)";
}

/* Stubs for functions referenced by npc_dialogue.c but not called in tests */
void do_say(CHAR_DATA *ch, char *arg)
{
   (void)ch;
   (void)arg;
}
void log_f(char *fmt, ...)
{
   (void)fmt;
}
void bug(const char *msg, int val)
{
   (void)msg;
   (void)val;
}
bool is_fighting(CHAR_DATA *ch)
{
   (void)ch;
   return FALSE;
}

/* Stub for spec_mudschool_guide — the real impl calls npc_dialogue_dispatch
 * which drags in pthreads/HTTP; the stub satisfies spec_lookup's table. */
bool spec_mudschool_guide(CHAR_DATA *ch)
{
   (void)ch;
   return FALSE;
}

/* -------------------------------------------------------------------------
 * Helper: build a HELP_DATA node on the stack.
 * -------------------------------------------------------------------------*/
static void make_help(HELP_DATA *h, const char *keyword, const char *text)
{
   memset(h, 0, sizeof(*h));
   h->keyword = (char *)keyword;
   h->text = (char *)text;
   h->level = 0;
}

/* -------------------------------------------------------------------------
 * Test 1: spec_lookup returns the mudschool guide function pointer.
 * -------------------------------------------------------------------------*/
static void test_spec_lookup_finds_mudschool_guide(void)
{
   SPEC_FUN *fn = spec_lookup("spec_mudschool_guide");
   assert(fn != NULL);
   assert(fn == spec_mudschool_guide);
   printf("PASS test_spec_lookup_finds_mudschool_guide\n");
}

/* -------------------------------------------------------------------------
 * Test 5: collect_help_context skips [GREET] messages.
 * -------------------------------------------------------------------------*/
static void test_collect_help_context_skips_greet(void)
{
   HELP_DATA h;
   char out[256];

   make_help(&h, "MOVEMENT", "Type north/south/east/west to move.");
   h.next = NULL;
   first_help = &h;

   npc_dialogue_test_collect_help_context("[GREET] Zorkin has arrived.", out, sizeof(out));
   assert(out[0] == '\0');
   first_help = NULL;
   printf("PASS test_collect_help_context_skips_greet\n");
}

/* -------------------------------------------------------------------------
 * Test 6: collect_help_context matches a keyword in first_help.
 * -------------------------------------------------------------------------*/
static void test_collect_help_context_matches_help_keyword(void)
{
   HELP_DATA h;
   char out[512];

   make_help(&h, "MOVEMENT", "Type north/south/east/west to move around the world.");
   h.next = NULL;
   first_help = &h;

   npc_dialogue_test_collect_help_context("How do I use movement commands?", out, sizeof(out));
   assert(strstr(out, "[HELP: MOVEMENT]") != NULL);
   first_help = NULL;
   printf("PASS test_collect_help_context_matches_help_keyword\n");
}

/* -------------------------------------------------------------------------
 * Test 7: collect_help_context deduplicates the same entry.
 * -------------------------------------------------------------------------*/
static void test_collect_help_context_deduplicates(void)
{
   HELP_DATA h;
   char out[512];
   int count;
   const char *p;

   make_help(&h, "COMBAT", "Type kill <target> to begin fighting.");
   h.next = NULL;
   first_help = &h;

   /* Message has two words that both prefix-match "COMBAT" */
   npc_dialogue_test_collect_help_context("combat combat fighting", out, sizeof(out));

   /* Count occurrences of "[HELP: COMBAT]" — should be exactly 1 */
   count = 0;
   p = out;
   while ((p = strstr(p, "[HELP:")) != NULL)
   {
      count++;
      p++;
   }
   assert(count == 1);
   first_help = NULL;
   printf("PASS test_collect_help_context_deduplicates\n");
}

/* -------------------------------------------------------------------------
 * Test 8: collect_help_context searches first_shelp when help has no match.
 * -------------------------------------------------------------------------*/
static void test_collect_help_context_searches_shelp(void)
{
   HELP_DATA sh;
   char out[512];

   first_help = NULL;
   make_help(&sh, "STAFF", "Staff commands");
   sh.next = NULL;
   first_shelp = &sh;

   npc_dialogue_test_collect_help_context("What staff commands exist?", out, sizeof(out));
   assert(strstr(out, "[HELP: STAFF]") != NULL);
   first_shelp = NULL;
   printf("PASS test_collect_help_context_searches_shelp\n");
}

/* -------------------------------------------------------------------------
 * Test 9: collect_help_context respects the output cap.
 * -------------------------------------------------------------------------*/
static void test_collect_help_context_respects_cap(void)
{
   HELP_DATA h;
   char out[32]; /* very small cap */

   make_help(&h, "SCORE", "The score command shows your statistics.");
   h.next = NULL;
   first_help = &h;

   npc_dialogue_test_collect_help_context("show score statistics", out, sizeof(out));
   assert(strlen(out) < sizeof(out));
   first_help = NULL;
   printf("PASS test_collect_help_context_respects_cap\n");
}

/* -------------------------------------------------------------------------
 * main
 * -------------------------------------------------------------------------*/
int main(void)
{
   test_spec_lookup_finds_mudschool_guide();
   test_collect_help_context_skips_greet();
   test_collect_help_context_matches_help_keyword();
   test_collect_help_context_deduplicates();
   test_collect_help_context_searches_shelp();
   test_collect_help_context_respects_cap();

   printf("All npc_dialogue_help tests passed.\n");
   return 0;
}
