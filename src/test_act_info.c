#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "globals.h"

int find_race_index_by_name(const char *name);
int score_should_show_invasion_rewards(CHAR_DATA *ch);
void parse_shelp_query(char *argument, char *search_term, size_t search_term_size, char *full_argument,
                       size_t full_argument_size);

char *one_argument(char *argument, char *arg_first)
{
    char cEnd;

    if (argument == NULL)
        return NULL;

    while (isspace((unsigned char)*argument))
        argument++;

    cEnd = ' ';
    if (*argument == '\'' || *argument == '"')
        cEnd = *argument++;

    while (*argument != '\0') {
        if (*argument == cEnd) {
            argument++;
            break;
        }

        *arg_first = tolower((unsigned char)*argument);
        arg_first++;
        argument++;
    }
    *arg_first = '\0';

    while (isspace((unsigned char)*argument))
        argument++;

    return argument;
}

bool str_cmp(const char *astr, const char *bstr)
{
    if (astr == NULL)
        return bstr != NULL;
    if (bstr == NULL)
        return TRUE;

    while (*astr != '\0' || *bstr != '\0') {
        int a = tolower((unsigned char)*astr);
        int b = tolower((unsigned char)*bstr);
        if (a != b)
            return TRUE;
        if (*astr != '\0')
            astr++;
        if (*bstr != '\0')
            bstr++;
    }

    return FALSE;
}

bool str_prefix(const char *astr, const char *bstr)
{
    if (astr == NULL || bstr == NULL)
        return TRUE;

    while (*astr != '\0') {
        if (*bstr == '\0')
            return TRUE;
        if (tolower((unsigned char)*astr) != tolower((unsigned char)*bstr))
            return TRUE;
        astr++;
        bstr++;
    }

    return FALSE;
}

const struct race_type race_table[MAX_RACE] = {
    [0] = {.race_name = "hum", .race_title = "human"},
    [1] = {.race_name = "elf", .race_title = "elf"},
    [2] = {.race_name = "dwa", .race_title = "dwarf"},
};

static void test_find_race_index_matches_exact_name_or_title(void)
{
    assert(find_race_index_by_name("hum") == 0);
    assert(find_race_index_by_name("ELF") == 1);
    assert(find_race_index_by_name("Dwarf") == 2);
}

static void test_find_race_index_falls_back_to_prefix_matching(void)
{
    assert(find_race_index_by_name("hu") == 0);
    assert(find_race_index_by_name("el") == 1);
    assert(find_race_index_by_name("dwar") == 2);
}

static void test_find_race_index_rejects_unknown_or_empty_values(void)
{
    assert(find_race_index_by_name("orc") == -1);
    assert(find_race_index_by_name("") == -1);
    assert(find_race_index_by_name(NULL) == -1);
}

static void test_parse_shelp_query_defaults_to_shelp(void)
{
    char argument[] = "";
    char search_term[MAX_INPUT_LENGTH] = {0};
    char full_argument[MAX_INPUT_LENGTH] = {0};

    parse_shelp_query(argument, search_term, sizeof(search_term), full_argument, sizeof(full_argument));

    assert(strcmp(search_term, "shelp") == 0);
    assert(strcmp(full_argument, "shelp") == 0);
}

static void test_parse_shelp_query_handles_multi_word_arguments(void)
{
    char argument[] = "soul net";
    char search_term[MAX_INPUT_LENGTH] = {0};
    char full_argument[MAX_INPUT_LENGTH] = {0};

    parse_shelp_query(argument, search_term, sizeof(search_term), full_argument, sizeof(full_argument));

    assert(strcmp(search_term, "soul") == 0);
    assert(strcmp(full_argument, "soul net") == 0);
}

static void test_parse_shelp_query_unwraps_quoted_multi_word_arguments(void)
{
    char argument[] = "\"soul net\"";
    char search_term[MAX_INPUT_LENGTH] = {0};
    char full_argument[MAX_INPUT_LENGTH] = {0};

    parse_shelp_query(argument, search_term, sizeof(search_term), full_argument, sizeof(full_argument));

    assert(strcmp(search_term, "soul net") == 0);
    assert(strcmp(full_argument, "soul net") == 0);
}

static void test_score_invasion_rewards_visibility(void)
{
    CHAR_DATA player = {0};
    PC_DATA pc = {0};
    CHAR_DATA npc = {0};

    player.pcdata = &pc;
    npc.act = ACT_IS_NPC;

    assert(score_should_show_invasion_rewards(NULL) == 0);
    assert(score_should_show_invasion_rewards(&npc) == 0);
    assert(score_should_show_invasion_rewards(&player) == 0);

    pc.invasion_rewards[1] = 2;
    assert(score_should_show_invasion_rewards(&player) == 1);
}

int main(void)
{
    test_find_race_index_matches_exact_name_or_title();
    test_find_race_index_falls_back_to_prefix_matching();
    test_find_race_index_rejects_unknown_or_empty_values();
    test_parse_shelp_query_defaults_to_shelp();
    test_parse_shelp_query_handles_multi_word_arguments();
    test_parse_shelp_query_unwraps_quoted_multi_word_arguments();
    test_score_invasion_rewards_visibility();

    puts("test_act_info: all tests passed");
    return 0;
}
