#include <assert.h>
#include <string.h>

void spendqp_copy_text(char *dest, size_t dest_size, const char *src);
void spendqp_build_move_message(char *dest, size_t dest_size, const char *title, const char *message);
void spendqp_append_word_with_space(char *dest, size_t dest_size, const char *word);

static void test_copy_text_handles_null_and_truncation(void)
{
    char buf[8];

    spendqp_copy_text(buf, sizeof(buf), "abcdefghijk");
    assert(strcmp(buf, "abcdefg") == 0);

    spendqp_copy_text(buf, sizeof(buf), NULL);
    assert(strcmp(buf, "") == 0);
}

static void test_build_move_message_format_and_truncation(void)
{
    char buf[32];

    spendqp_build_move_message(buf, sizeof(buf), "Lord ", "walks in");
    assert(strcmp(buf, "$LLord $n walks in $T.") == 0);

    spendqp_build_move_message(buf, 12, "VeryLongTitle", "message");
    assert(buf[11] == '\0');
    assert(strncmp(buf, "$LVeryLongT", 10) == 0);
}

static void test_append_word_with_space_appends_and_caps(void)
{
    char buf[10];

    strcpy(buf, "hi ");
    spendqp_append_word_with_space(buf, sizeof(buf), "all");
    assert(strcmp(buf, "hi all ") == 0);

    spendqp_append_word_with_space(buf, sizeof(buf), "overflow");
    assert(strcmp(buf, "hi all ov") == 0);
}

int main(void)
{
    test_copy_text_handles_null_and_truncation();
    test_build_move_message_format_and_truncation();
    test_append_word_with_space_appends_and_caps();
    return 0;
}
