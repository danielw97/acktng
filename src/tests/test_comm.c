#include <assert.h>
#include <stdio.h>

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
 */

int main(void)
{
   puts("test_comm: all tests passed");
   return 0;
}
