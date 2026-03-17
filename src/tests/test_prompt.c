#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "config.h"
#define DEC_GLOBALS_H 1
#include "ack.h"
#include "prompt_test.h"

static int stub_max_hp   = 100;
static int stub_max_mana = 100;
static int stub_max_move = 100;

int get_max_hp(CHAR_DATA *ch)
{
   (void)ch;
   return stub_max_hp;
}
int get_max_mana(CHAR_DATA *ch)
{
   (void)ch;
   return stub_max_mana;
}
int get_max_move(CHAR_DATA *ch)
{
   (void)ch;
   return stub_max_move;
}

/* ------------------------------------------------------------------ */
/* prompt_should_show_hp/mana/move                            */
/* ------------------------------------------------------------------ */

static void test_show_hp_when_below_max(void)
{
   CHAR_DATA ch;
   memset(&ch, 0, sizeof(ch));

   ch.hit    = 99;
   stub_max_hp = 100;
   assert(prompt_should_show_hp(&ch));

   stub_max_hp = 90;
   assert(!prompt_should_show_hp(&ch));

   /* Exactly at max: no display */
   ch.hit    = 100;
   stub_max_hp = 100;
   assert(!prompt_should_show_hp(&ch));
}

static void test_show_mana_when_below_max(void)
{
   CHAR_DATA ch;
   memset(&ch, 0, sizeof(ch));

   ch.mana     = 40;
   stub_max_mana = 50;
   assert(prompt_should_show_mana(&ch));

   stub_max_mana = 35;
   assert(!prompt_should_show_mana(&ch));

   /* Exactly at max: no display */
   ch.mana     = 50;
   stub_max_mana = 50;
   assert(!prompt_should_show_mana(&ch));
}

static void test_show_move_when_below_max(void)
{
   CHAR_DATA ch;
   memset(&ch, 0, sizeof(ch));

   ch.move     = 70;
   stub_max_move = 100;
   assert(prompt_should_show_move(&ch));

   stub_max_move = 60;
   assert(!prompt_should_show_move(&ch));

   /* Exactly at max: no display */
   ch.move     = 100;
   stub_max_move = 100;
   assert(!prompt_should_show_move(&ch));
}

static void test_null_char_never_shows(void)
{
   assert(!prompt_should_show_hp(NULL));
   assert(!prompt_should_show_mana(NULL));
   assert(!prompt_should_show_move(NULL));
}

/* ------------------------------------------------------------------ */
/* prompt_max_value_for_code                                          */
/* ------------------------------------------------------------------ */

static void test_max_value_codes_use_max_helpers(void)
{
   CHAR_DATA ch;
   memset(&ch, 0, sizeof(ch));

   stub_max_hp   = 111;
   stub_max_mana = 222;
   stub_max_move = 333;

   assert(prompt_max_value_for_code(&ch, 'H') == 111);
   assert(prompt_max_value_for_code(&ch, 'M') == 222);
   assert(prompt_max_value_for_code(&ch, 'V') == 333);
}

static void test_max_value_unknown_code_returns_zero(void)
{
   CHAR_DATA ch;
   memset(&ch, 0, sizeof(ch));

   assert(prompt_max_value_for_code(&ch, 'X') == 0);
   assert(prompt_max_value_for_code(&ch, 'h') == 0);
   assert(prompt_max_value_for_code(&ch, '\0') == 0);
}

static void test_max_value_null_char_returns_zero(void)
{
   assert(prompt_max_value_for_code(NULL, 'H') == 0);
   assert(prompt_max_value_for_code(NULL, 'M') == 0);
   assert(prompt_max_value_for_code(NULL, 'V') == 0);
}

/* ------------------------------------------------------------------ */
/* format_builder_prompt                                     */
/* ------------------------------------------------------------------ */

static void test_builder_prompt_basic_format(void)
{
   char buf[64];

   format_builder_prompt(buf, sizeof(buf), "Mode: Redit", "[12345]: A room");
   assert(strcmp(buf, "< Mode: Redit [12345]: A room >") == 0);
}

static void test_builder_prompt_null_mode_and_details(void)
{
   char buf[64];

   format_builder_prompt(buf, sizeof(buf), NULL, NULL);
   assert(strcmp(buf, "<   >") == 0);
}

static void test_builder_prompt_truncates_to_buffer_size(void)
{
   char buf[16];

   format_builder_prompt(buf, sizeof(buf), "Mode: Redit", "A very very long description");
   assert(buf[15] == '\0');
}

static void test_builder_prompt_null_dest_is_safe(void)
{
   /* Must not crash */
   format_builder_prompt(NULL, 32, "Mode", "Details");
   format_builder_prompt(NULL, 0, "Mode", "Details");
}

static void test_builder_prompt_zero_size_is_safe(void)
{
   char buf[8] = "XXXXXXXX";

   format_builder_prompt(buf, 0, "Mode", "Details");
   /* buf should be untouched */
   assert(buf[0] == 'X');
}

int main(void)
{
   test_show_hp_when_below_max();
   test_show_mana_when_below_max();
   test_show_move_when_below_max();
   test_null_char_never_shows();
   test_max_value_codes_use_max_helpers();
   test_max_value_unknown_code_returns_zero();
   test_max_value_null_char_returns_zero();
   test_builder_prompt_basic_format();
   test_builder_prompt_null_mode_and_details();
   test_builder_prompt_truncates_to_buffer_size();
   test_builder_prompt_null_dest_is_safe();
   test_builder_prompt_zero_size_is_safe();

   puts("test_prompt: all tests passed");
   return 0;
}
