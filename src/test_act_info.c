#include <assert.h>
#include <ctype.h>
#include <stdio.h>

#include "globals.h"

int find_race_index_by_name(const char *name);
int score_should_show_invasion_rewards(CHAR_DATA *ch);

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
    test_score_invasion_rewards_visibility();

    puts("test_act_info: all tests passed");
    return 0;
}
