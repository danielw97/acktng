#include <assert.h>
#include <stdio.h>
#include <string.h>

/*
 * test_comm.c — unit tests for comm.c
 *
 * The functions that were previously tested here have moved:
 *   - is_parse_name_syntax_valid, is_login_name_format_valid,
 *     comm_format_class_menu_line  →  login.c / test_login.c
 *   - shortfight_should_suppress_watched_autoattack  →  fight.c / test_fight.c
 *
 * The remaining public surface of comm.c (send_to_char, show_string, act,
 * list_who_to_output, write_gsgp_data) all require a live descriptor/character
 * tree and are covered by the integration test instead.
 *
 * get_gsgp_json() and get_wholist_html() return in-memory buffers that are
 * zero-initialized at startup and do not require a live descriptor tree.
 */

#define DEC_GLOBALS_H 1
#include "ack.h"

static void test_gsgp_json_initial_empty(void)
{
   const char *json = get_gsgp_json();
   assert(json != NULL);
   assert(json[0] == '\0');
   puts("PASS: test_gsgp_json_initial_empty");
}

static void test_wholist_html_initial_empty(void)
{
   const char *html = get_wholist_html();
   assert(html != NULL);
   assert(html[0] == '\0');
   puts("PASS: test_wholist_html_initial_empty");
}

int main(void)
{
   test_gsgp_json_initial_empty();
   test_wholist_html_initial_empty();
   puts("test_comm: all tests passed");
   return 0;
}
