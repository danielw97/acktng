#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "config.h"
#define DEC_GLOBALS_H 1
#include "ack.h"

void magic_test_format_spell_utterance(char *dest, size_t dest_size, const char *words);

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

int main(void)
{
    test_spell_utterance_format();
    test_spell_utterance_truncates_safely();

    puts("test_magic: all tests passed");
    return 0;
}
