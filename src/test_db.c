#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "config.h"
#define DEC_GLOBALS_H 1
#include "ack.h"

void db_test_format_status(char *dest, size_t dest_size, const char *prefix, const char *file_name);
void db_test_set_area_name(const char *file_name);
const char *db_test_get_area_name(void);
bool db_test_try_read_help_level(FILE *fp, long *level);

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

static void test_try_read_help_level_accepts_numeric_prefix(void)
{
    FILE *fp = tmpfile();
    long level = -1;

    assert(fp != NULL);
    fputs("   42 keyword~\n", fp);
    rewind(fp);

    assert(db_test_try_read_help_level(fp, &level));
    assert(level == 42);

    fclose(fp);
}

static void test_try_read_help_level_rejects_non_numeric_prefix(void)
{
    FILE *fp = tmpfile();
    long level = -1;
    int c;

    assert(fp != NULL);
    fputs("keep title <string>\n", fp);
    rewind(fp);

    assert(!db_test_try_read_help_level(fp, &level));
    c = fgetc(fp);
    assert(c == 'k');

    fclose(fp);
}

int main(void)
{
    test_format_status_builds_expected_message();
    test_set_area_name_truncates_and_terminates();
    test_try_read_help_level_accepts_numeric_prefix();
    test_try_read_help_level_rejects_non_numeric_prefix();

    puts("test_db: all tests passed");
    return 0;
}
