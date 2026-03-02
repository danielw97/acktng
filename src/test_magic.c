#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "config.h"
#define DEC_GLOBALS_H 1
#include "ack.h"

void magic_test_format_spell_utterance(char *dest, size_t dest_size, const char *words);
bool magic_test_npc_remort_cast_blocked_flags(int flag1, int act_bits, int affected_by_bits);

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

    puts("test_magic: all tests passed");
    return 0;
}
