#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "config.h"
#define DEC_GLOBALS_H 1
#include "ack.h"

/* Functions under test (defined in login.c, available outside UNIT_TEST_LOGIN). */
bool is_parse_name_syntax_valid(const char *name);
bool is_login_name_format_valid(const char *name);
void comm_format_class_menu_line(char *dest, size_t dest_size, const char *who_name,
                                 const char *stat, const char *class_name);

/* ---------------------------------------------------------------------------
 * is_parse_name_syntax_valid
 * --------------------------------------------------------------------------- */

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

static void test_parse_name_rejects_null(void)
{
   assert(!is_parse_name_syntax_valid(NULL));
}

/* ---------------------------------------------------------------------------
 * is_login_name_format_valid
 * --------------------------------------------------------------------------- */

static void test_login_name_rejects_reserved_words(void)
{
   assert(!is_login_name_format_valid("all"));
   assert(!is_login_name_format_valid("Self"));
   assert(!is_login_name_format_valid("enemy"));
   assert(!is_login_name_format_valid("tank"));
   assert(!is_login_name_format_valid("immortal"));
   assert(!is_login_name_format_valid("zen"));
   assert(!is_login_name_format_valid("someone"));
   assert(!is_login_name_format_valid("auto"));
}

static void test_login_name_accepts_valid_player_names(void)
{
   assert(is_login_name_format_valid("Zenithar"));
   assert(is_login_name_format_valid("Knight"));
   assert(is_login_name_format_valid("Flar"));
}

static void test_login_name_rejects_bad_syntax(void)
{
   assert(!is_login_name_format_valid("ab"));
   assert(!is_login_name_format_valid("ab3"));
   assert(!is_login_name_format_valid(NULL));
}

/* ---------------------------------------------------------------------------
 * comm_format_class_menu_line
 * --------------------------------------------------------------------------- */

static void test_comm_class_menu_formatter_truncates(void)
{
   char buf[16];

   comm_format_class_menu_line(buf, sizeof(buf), "warden", "strength", "verylongclassname");
   assert(buf[15] == '\0');
}

static void test_comm_class_menu_formatter_null_inputs_safe(void)
{
   char buf[64];

   comm_format_class_menu_line(buf, sizeof(buf), NULL, NULL, NULL);
   assert(buf[0] != '\0'); /* should produce some output */
}

static void test_comm_class_menu_formatter_zero_size_is_noop(void)
{
   char buf[4] = "xxx";

   comm_format_class_menu_line(buf, 0, "cle", "wis", "Cleric");
   assert(buf[0] == 'x'); /* buffer should be untouched */
}

int main(void)
{
   test_parse_name_enforces_length_bounds();
   test_parse_name_rejects_non_alpha();
   test_parse_name_rejects_all_i_or_l_names();
   test_parse_name_accepts_normal_alphabetic_names();
   test_parse_name_rejects_null();

   test_login_name_rejects_reserved_words();
   test_login_name_accepts_valid_player_names();
   test_login_name_rejects_bad_syntax();

   test_comm_class_menu_formatter_truncates();
   test_comm_class_menu_formatter_null_inputs_safe();
   test_comm_class_menu_formatter_zero_size_is_noop();

   puts("test_login: all tests passed");
   return 0;
}
