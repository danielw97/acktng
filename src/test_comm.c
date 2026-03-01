#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

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

int main(void)
{
    test_parse_name_enforces_length_bounds();
    test_parse_name_rejects_non_alpha();
    test_parse_name_rejects_all_i_or_l_names();
    test_parse_name_accepts_normal_alphabetic_names();

    puts("test_comm: all tests passed");
    return 0;
}
