#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "config.h"
#define DEC_GLOBALS_H 1
#include "ack.h"

void magic_test_format_spell_utterance(char *dest, size_t dest_size, const char *words);
bool magic_test_npc_remort_cast_blocked_flags(int flag1, int act_bits, int affected_by_bits);
bool is_arcane_spell(int sn);

struct skill_type skill_table[MAX_SKILL];

static bool dummy_spell(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{
    return TRUE;
}

static void test_spell_utterance_format(void)
{
    char buf[64];

    magic_test_format_spell_utterance(buf, sizeof(buf), "abra");
    assert(strcmp(buf, "$n utters the words, 'abra'.") == 0);
}

static void test_spell_utterance_truncates_safely(void)
{
    char words[200];
    char buf[32];

    memset(words, 'x', sizeof(words) - 1);
    words[sizeof(words) - 1] = '\0';

    magic_test_format_spell_utterance(buf, sizeof(buf), words);
    assert(buf[sizeof(buf) - 1] == '\0');
}



static void test_is_arcane_spell_matches_arcane_classes(void)
{
    memset(skill_table, 0, sizeof(skill_table));

    skill_table[1].name = "arcane";
    skill_table[1].spell_fun = dummy_spell;
    for (int i = 0; i < MAX_REMORT; i++)
        skill_table[1].skill_level[i] = NO_USE;
    skill_table[1].skill_level[CLASS_MAG] = 1;
    assert(is_arcane_spell(1) == TRUE);

    skill_table[2].name = "not_arcane";
    skill_table[2].spell_fun = dummy_spell;
    for (int i = 0; i < MAX_REMORT; i++)
        skill_table[2].skill_level[i] = NO_USE;
    skill_table[2].skill_level[CLASS_CLE] = 1;
    assert(is_arcane_spell(2) == FALSE);
}

static void test_is_arcane_spell_rejects_invalid_or_non_spell_entries(void)
{
    memset(skill_table, 0, sizeof(skill_table));

    assert(is_arcane_spell(-1) == FALSE);
    assert(is_arcane_spell(MAX_SKILL) == FALSE);

    skill_table[3].name = "no_spell_fun";
    for (int i = 0; i < MAX_REMORT; i++)
        skill_table[3].skill_level[i] = NO_USE;
    skill_table[3].skill_level[CLASS_MAG] = 1;
    assert(is_arcane_spell(3) == FALSE);
}
static void test_npc_remort_cast_blocked_only_for_pet_or_charm(void)
{
    assert(magic_test_npc_remort_cast_blocked_flags(REMORT, 0, 0) == FALSE);
    assert(magic_test_npc_remort_cast_blocked_flags(REMORT, ACT_PET, 0) == TRUE);
    assert(magic_test_npc_remort_cast_blocked_flags(REMORT, 0, AFF_CHARM) == TRUE);
    assert(magic_test_npc_remort_cast_blocked_flags(ADEPT, ACT_PET, 0) == TRUE);
    assert(magic_test_npc_remort_cast_blocked_flags(MORTAL, ACT_PET, AFF_CHARM) == FALSE);
}

int main(void)
{
    test_spell_utterance_format();
    test_spell_utterance_truncates_safely();
    test_npc_remort_cast_blocked_only_for_pet_or_charm();
    test_is_arcane_spell_matches_arcane_classes();
    test_is_arcane_spell_rejects_invalid_or_non_spell_entries();

    puts("test_magic: all tests passed");
    return 0;
}
