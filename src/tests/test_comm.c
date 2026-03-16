#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#include "config.h"
#define DEC_GLOBALS_H 1
#include "ack.h"
#include "comm_login_test.h"

bool is_parse_name_syntax_valid(const char *name);
bool is_login_name_format_valid(const char *name);
long prompt_max_value_for_code(CHAR_DATA *ch, char code);
void comm_testable_format_builder_prompt(char *dest, size_t dest_size, const char *mode,
                                         const char *details);
void comm_testable_format_class_menu_line(char *dest, size_t dest_size, const char *who_name,
                                          const char *stat, const char *class_name);
bool shortfight_should_suppress_watched_autoattack(int observer_is_npc, int observer_has_shortfight,
                                                   int observer_is_fighting);

static int stub_max_hp = 100;
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

static void test_parse_name_enforces_length_bounds(void)
{
   assert(!is_parse_name_syntax_valid("ab"));
   assert(is_parse_name_syntax_valid("abc"));
   assert(is_parse_name_syntax_valid("abcdefghijkl"));
   assert(!is_parse_name_syntax_valid("abcdefghijklm"));
}

static void test_parse_name_rejects_non_alpha(void)
{
   assert(!is_parse_name_syntax_valid("ab3"));
   assert(!is_parse_name_syntax_valid("ab-"));
   assert(!is_parse_name_syntax_valid("name!"));
}

static void test_parse_name_rejects_all_i_or_l_names(void)
{
   assert(!is_parse_name_syntax_valid("iii"));
   assert(!is_parse_name_syntax_valid("lil"));
   assert(!is_parse_name_syntax_valid("IlIl"));
}

static void test_parse_name_accepts_normal_alphabetic_names(void)
{
   assert(is_parse_name_syntax_valid("Zen"));
   assert(is_parse_name_syntax_valid("Tanker"));
   assert(is_parse_name_syntax_valid("Illidan"));
}

static void test_existing_player_login_happy_path_reaches_playing(void)
{
   int connected = CON_GET_NAME;

   assert(simulate_existing_player_login_transition(&connected, "Zen", true, true, false) ==
          LOGIN_TRANSITION_PASSWORD_PROMPTED);
   assert(connected == CON_GET_OLD_PASSWORD);

   assert(simulate_existing_player_login_transition(&connected, "hunter2", true, true, true) ==
          LOGIN_TRANSITION_SHOW_MOTD);
   assert(connected == CON_READ_MOTD);

   assert(simulate_existing_player_login_transition(&connected, "", true, true, true) ==
          LOGIN_TRANSITION_ENTERED_GAME);
   assert(connected == CON_PLAYING);
}

static void test_existing_player_login_rejects_invalid_name(void)
{
   int connected = CON_GET_NAME;

   assert(simulate_existing_player_login_transition(&connected, "x!", false, true, false) ==
          LOGIN_TRANSITION_NAME_REJECTED);
   assert(connected == CON_GET_NAME);
}

static void test_existing_player_login_rejects_new_character_at_old_player_path(void)
{
   int connected = CON_GET_NAME;

   assert(simulate_existing_player_login_transition(&connected, "Newbie", true, false, false) ==
          LOGIN_TRANSITION_NAME_REJECTED);
   assert(connected == CON_GET_NAME);
}

static void test_existing_player_login_rejects_wrong_password_without_state_advance(void)
{
   int connected = CON_GET_OLD_PASSWORD;

   assert(simulate_existing_player_login_transition(&connected, "badpass", true, true, false) ==
          LOGIN_TRANSITION_WRONG_PASSWORD);
   assert(connected == CON_GET_OLD_PASSWORD);
}

static void test_existing_player_login_rejects_null_inputs(void)
{
   int connected = CON_GET_NAME;

   assert(simulate_existing_player_login_transition(NULL, "Zen", true, true, true) ==
          LOGIN_TRANSITION_NAME_REJECTED);
   assert(simulate_existing_player_login_transition(&connected, NULL, true, true, true) ==
          LOGIN_TRANSITION_NAME_REJECTED);
   assert(connected == CON_GET_NAME);
}

static void test_login_name_rejects_reserved_words(void)
{
   assert(!is_login_name_format_valid("all"));
   assert(!is_login_name_format_valid("Self"));
   assert(!is_login_name_format_valid("enemy"));
   assert(!is_login_name_format_valid("tank"));
}

static void test_login_name_accepts_valid_player_names(void)
{
   assert(is_login_name_format_valid("Zenithar"));
   assert(is_login_name_format_valid("Knight"));
}

static void test_prompt_thresholds_use_max_helpers(void)
{
   CHAR_DATA ch;

   memset(&ch, 0, sizeof(ch));

   ch.hit = 99;
   stub_max_hp = 100;
   assert(should_show_default_prompt_hp(&ch));
   stub_max_hp = 90;
   assert(!should_show_default_prompt_hp(&ch));

   ch.mana = 40;
   stub_max_mana = 50;
   assert(should_show_default_prompt_mana(&ch));
   stub_max_mana = 35;
   assert(!should_show_default_prompt_mana(&ch));

   ch.move = 70;
   stub_max_move = 100;
   assert(should_show_default_prompt_move(&ch));
   stub_max_move = 60;
   assert(!should_show_default_prompt_move(&ch));
}

static void test_comm_safe_formatters_truncate(void)
{
   char buf[16];

   comm_testable_format_builder_prompt(buf, sizeof(buf), "Mode: Redit",
                                       "A very very long description");
   assert(buf[15] == '\0');

   comm_testable_format_class_menu_line(buf, sizeof(buf), "warden", "strength",
                                        "verylongclassname");
   assert(buf[15] == '\0');

   comm_testable_format_builder_prompt(buf, sizeof(buf), NULL, NULL);
   assert(buf[15] == '\0');
}

static void test_prompt_max_tokens_use_max_helpers(void)
{
   CHAR_DATA ch;

   memset(&ch, 0, sizeof(ch));

   stub_max_hp = 111;
   stub_max_mana = 222;
   stub_max_move = 333;

   assert(prompt_max_value_for_code(&ch, 'H') == 111);
   assert(prompt_max_value_for_code(&ch, 'M') == 222);
   assert(prompt_max_value_for_code(&ch, 'V') == 333);
   assert(prompt_max_value_for_code(&ch, 'X') == 0);
}

static void test_shortfight_watched_autoattack_suppression(void)
{
   assert(shortfight_should_suppress_watched_autoattack(false, true, false));
   assert(!shortfight_should_suppress_watched_autoattack(false, true, true));
   assert(!shortfight_should_suppress_watched_autoattack(false, false, false));
   assert(!shortfight_should_suppress_watched_autoattack(true, true, false));
}

int main(void)
{
   test_parse_name_enforces_length_bounds();
   test_parse_name_rejects_non_alpha();
   test_parse_name_rejects_all_i_or_l_names();
   test_parse_name_accepts_normal_alphabetic_names();
   test_login_name_rejects_reserved_words();
   test_login_name_accepts_valid_player_names();
   test_prompt_thresholds_use_max_helpers();
   test_prompt_max_tokens_use_max_helpers();
   test_comm_safe_formatters_truncate();
   test_shortfight_watched_autoattack_suppression();

   test_existing_player_login_happy_path_reaches_playing();
   test_existing_player_login_rejects_invalid_name();
   test_existing_player_login_rejects_new_character_at_old_player_path();
   test_existing_player_login_rejects_wrong_password_without_state_advance();
   test_existing_player_login_rejects_null_inputs();

   puts("test_comm: all tests passed");
   return 0;
}
