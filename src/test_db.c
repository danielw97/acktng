#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "config.h"
#define DEC_GLOBALS_H 1
#include "ack.h"

void db_test_format_status(char *dest, size_t dest_size, const char *prefix, const char *file_name);
void db_test_set_area_name(const char *file_name);
const char *db_test_get_area_name(void);

static void test_format_status_builds_expected_message(void)
{
    char buf[64];

    db_test_format_status(buf, sizeof(buf), "Loading", "foo.dat");
    assert(strcmp(buf, "Loading foo.dat") == 0);
}

static void test_set_area_name_truncates_and_terminates(void)
{
    char long_name[MAX_STRING_LENGTH + 64];

    memset(long_name, 'a', sizeof(long_name) - 1);
    long_name[sizeof(long_name) - 1] = '\0';

    db_test_set_area_name(long_name);

    assert(db_test_get_area_name()[MAX_INPUT_LENGTH - 1] == '\0');
    assert(strlen(db_test_get_area_name()) == MAX_INPUT_LENGTH - 1);
}

int main(void)
{
    test_format_status_builds_expected_message();
    test_set_area_name_truncates_and_terminates();

    puts("test_db: all tests passed");
    return 0;
}
