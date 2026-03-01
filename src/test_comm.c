#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#include "config.h"
#include "comm_login_test.h"

bool is_parse_name_syntax_valid(const char *name);


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

    assert(simulate_existing_player_login_transition(&connected, "Zen", true, true, false) == LOGIN_TRANSITION_PASSWORD_PROMPTED);
    assert(connected == CON_GET_OLD_PASSWORD);

    assert(simulate_existing_player_login_transition(&connected, "hunter2", true, true, true) == LOGIN_TRANSITION_SHOW_MOTD);
    assert(connected == CON_READ_MOTD);

    assert(simulate_existing_player_login_transition(&connected, "", true, true, true) == LOGIN_TRANSITION_ENTERED_GAME);
    assert(connected == CON_PLAYING);
}

static void test_existing_player_login_rejects_invalid_name(void)
{
    int connected = CON_GET_NAME;

    assert(simulate_existing_player_login_transition(&connected, "x!", false, true, false) == LOGIN_TRANSITION_NAME_REJECTED);
    assert(connected == CON_GET_NAME);
}

static void test_existing_player_login_rejects_new_character_at_old_player_path(void)
{
    int connected = CON_GET_NAME;

    assert(simulate_existing_player_login_transition(&connected, "Newbie", true, false, false) == LOGIN_TRANSITION_NAME_REJECTED);
    assert(connected == CON_GET_NAME);
}

static void test_existing_player_login_rejects_wrong_password_without_state_advance(void)
{
    int connected = CON_GET_OLD_PASSWORD;

    assert(simulate_existing_player_login_transition(&connected, "badpass", true, true, false) == LOGIN_TRANSITION_WRONG_PASSWORD);
    assert(connected == CON_GET_OLD_PASSWORD);
}

static void test_existing_player_login_rejects_null_inputs(void)
{
    int connected = CON_GET_NAME;

    assert(simulate_existing_player_login_transition(NULL, "Zen", true, true, true) == LOGIN_TRANSITION_NAME_REJECTED);
    assert(simulate_existing_player_login_transition(&connected, NULL, true, true, true) == LOGIN_TRANSITION_NAME_REJECTED);
    assert(connected == CON_GET_NAME);
}

int main(void)
{
    test_parse_name_enforces_length_bounds();
    test_parse_name_rejects_non_alpha();
    test_parse_name_rejects_all_i_or_l_names();
    test_parse_name_accepts_normal_alphabetic_names();

    test_existing_player_login_happy_path_reaches_playing();
    test_existing_player_login_rejects_invalid_name();
    test_existing_player_login_rejects_new_character_at_old_player_path();
    test_existing_player_login_rejects_wrong_password_without_state_advance();
    test_existing_player_login_rejects_null_inputs();

    puts("test_comm: all tests passed");
    return 0;
}
