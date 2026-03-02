#include <assert.h>
#include <stdio.h>
#include <string.h>

void keep_format_chest_short_descr(const char *owner_name, char *dest, size_t dest_size);
int keep_chest_max_items(void);

static void test_keep_chest_name_uses_creator(void)
{
    char buf[128];

    keep_format_chest_short_descr("Creator", buf, sizeof(buf));
    assert(strcmp(buf, "Creator's Keep Chest") == 0);
}

static void test_keep_chest_name_handles_missing_owner(void)
{
    char buf[128];

    keep_format_chest_short_descr(NULL, buf, sizeof(buf));
    assert(strcmp(buf, "Unknown's Keep Chest") == 0);
}

static void test_keep_chest_name_handles_empty_owner(void)
{
    char buf[128];

    keep_format_chest_short_descr("", buf, sizeof(buf));
    assert(strcmp(buf, "Unknown's Keep Chest") == 0);
}

static void test_keep_chest_name_truncates_to_fit_buffer(void)
{
    char buf[8];

    keep_format_chest_short_descr("Creator", buf, sizeof(buf));
    assert(strcmp(buf, "Creator") == 0);
}

static void test_keep_chest_name_noops_for_null_dest(void)
{
    keep_format_chest_short_descr("Creator", NULL, 16);
}

static void test_keep_chest_name_noops_for_zero_dest_size(void)
{
    char buf[8] = "init";

    keep_format_chest_short_descr("Creator", buf, 0);
    assert(strcmp(buf, "init") == 0);
}

static void test_keep_chest_max_items(void)
{
    assert(keep_chest_max_items() == 50);
}

int main(void)
{
    test_keep_chest_name_uses_creator();
    test_keep_chest_name_handles_missing_owner();
    test_keep_chest_name_handles_empty_owner();
    test_keep_chest_name_truncates_to_fit_buffer();
    test_keep_chest_name_noops_for_null_dest();
    test_keep_chest_name_noops_for_zero_dest_size();
    test_keep_chest_max_items();
    puts("test_keep: all tests passed");
    return 0;
}
