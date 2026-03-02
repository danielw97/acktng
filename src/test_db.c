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

static int is_exit_triple_line(const char *line)
{
    int locks;
    int key;
    int to_room;
    char trailing;

    return sscanf(line, " %d %d %d %c", &locks, &key, &to_room, &trailing) == 3;
}

static int is_four_number_line(const char *line)
{
    int a;
    int b;
    int c;
    int d;
    char trailing;

    return sscanf(line, " %d %d %d %d %c", &a, &b, &c, &d, &trailing) == 4;
}

static int is_ten_number_line(const char *line)
{
    int a[10];
    char trailing;

    return sscanf(line,
                  " %d %d %d %d %d %d %d %d %d %d %c",
                  &a[0], &a[1], &a[2], &a[3], &a[4], &a[5], &a[6], &a[7], &a[8], &a[9], &trailing) == 10;
}

static int is_single_number_line(const char *line)
{
    int value;
    char trailing;

    return sscanf(line, " %d %c", &value, &trailing) == 1;
}

static int is_two_number_line(const char *line)
{
    int a;
    int b;
    char trailing;

    return sscanf(line, " %d %d %c", &a, &b, &trailing) == 2;
}

static int read_non_empty_line(FILE *fp, char *buf, size_t buf_size)
{
    while (fgets(buf, (int)buf_size, fp) != NULL)
    {
        if (buf[0] != '\n' && buf[0] != '\r')
            return 1;
    }

    return 0;
}

static const char *skip_space(const char *line)
{
    while (*line == ' ' || *line == '\t' || *line == '\r' || *line == '\n')
        line++;
    return line;
}

static int line_starts_with(const char *line, const char *prefix)
{
    line = skip_space(line);
    return strncmp(line, prefix, strlen(prefix)) == 0;
}


static int has_prefixed_int_count(const char *line, char prefix, int count)
{
    const char *p = skip_space(line);
    int value;

    if (*p != prefix)
        return 0;

    p++;
    for (int i = 0; i < count; i++)
    {
        if (sscanf(p, " %d", &value) != 1)
            return 0;

        while (*p == ' ' || *p == '	')
            p++;
        if (*p == '-' || *p == '+')
            p++;
        while (*p >= '0' && *p <= '9')
            p++;
    }

    p = skip_space(p);
    return *p == '\0';
}

static int consume_tilde_terminated_string(FILE *fp)
{
    int c;

    while ((c = fgetc(fp)) != EOF)
    {
        if (c == '~')
        {
            while (c != EOF && c != '\n')
                c = fgetc(fp);
            return 1;
        }
    }

    return 0;
}

static void validate_rooms_section_exit_format(FILE *fp)
{
    char line[4096];

    while (read_non_empty_line(fp, line, sizeof(line)))
    {
        if (line_starts_with(line, "#0"))
            return;

        if (line[0] != '#')
            continue;

        while (read_non_empty_line(fp, line, sizeof(line)))
        {
            const char *trimmed = skip_space(line);

            if (trimmed[0] == 'S')
                break;

            if (trimmed[0] == 'D' && trimmed[1] >= '0' && trimmed[1] <= '5')
            {
                assert(consume_tilde_terminated_string(fp));
                assert(consume_tilde_terminated_string(fp));
                assert(read_non_empty_line(fp, line, sizeof(line)));
                assert(is_exit_triple_line(skip_space(line)));
            }
        }
    }

    assert(!"unterminated #ROOMS section (expected #0)");
}

static void validate_mobiles_section_format(FILE *fp)
{
    char line[4096];

    while (read_non_empty_line(fp, line, sizeof(line)))
    {
        const char *trimmed = skip_space(line);

        if (line_starts_with(trimmed, "#0"))
            return;

        assert(trimmed[0] == '#');

        assert(consume_tilde_terminated_string(fp));
        assert(consume_tilde_terminated_string(fp));
        assert(consume_tilde_terminated_string(fp));
        assert(consume_tilde_terminated_string(fp));

        assert(read_non_empty_line(fp, line, sizeof(line)));
        assert(strchr(line, 'S') != NULL);
        assert(read_non_empty_line(fp, line, sizeof(line)));
        assert(is_two_number_line(skip_space(line)));
        assert(read_non_empty_line(fp, line, sizeof(line)));
        assert(is_four_number_line(skip_space(line)));

        for (;;)
        {
            long marker;

            marker = ftell(fp);
            if (!read_non_empty_line(fp, line, sizeof(line)))
                return;

            trimmed = skip_space(line);
            if (trimmed[0] == '#')
            {
                assert(fseek(fp, marker, SEEK_SET) == 0);
                break;
            }

            if (trimmed[0] == '!')
                assert(has_prefixed_int_count(trimmed, '!', 7));
            else if (trimmed[0] == '|')
                assert(has_prefixed_int_count(trimmed, '|', 7));
            else if (trimmed[0] == '+')
                assert(has_prefixed_int_count(trimmed, '+', 9));
            else if (trimmed[0] == 'l')
                assert(has_prefixed_int_count(trimmed, 'l', 10));
            else if (trimmed[0] == 'L')
                assert(has_prefixed_int_count(trimmed, 'L', 9));
            else if (trimmed[0] == '>')
            {
                while (read_non_empty_line(fp, line, sizeof(line)))
                {
                    if (skip_space(line)[0] == '|')
                        break;
                }
            }
            else
                assert(!"Unexpected trailing line in #MOBILES entry");
        }
    }

    assert(!"unterminated #MOBILES section (expected #0)");
}

static void validate_objects_section_format(FILE *fp)
{
    char line[4096];

    while (read_non_empty_line(fp, line, sizeof(line)))
    {
        const char *trimmed = skip_space(line);

        if (line_starts_with(trimmed, "#0"))
            return;

        assert(trimmed[0] == '#');

        assert(consume_tilde_terminated_string(fp));
        assert(consume_tilde_terminated_string(fp));
        assert(consume_tilde_terminated_string(fp));

        assert(read_non_empty_line(fp, line, sizeof(line)));
        assert(is_four_number_line(skip_space(line)));
        assert(read_non_empty_line(fp, line, sizeof(line)));
        assert(is_ten_number_line(skip_space(line)));
        assert(read_non_empty_line(fp, line, sizeof(line)));
        assert(is_single_number_line(skip_space(line)));

        for (;;)
        {
            long marker;

            marker = ftell(fp);
            if (!read_non_empty_line(fp, line, sizeof(line)))
                return;

            trimmed = skip_space(line);
            if (trimmed[0] == '#')
            {
                assert(fseek(fp, marker, SEEK_SET) == 0);
                break;
            }

            if (trimmed[0] == 'A')
            {
                assert(read_non_empty_line(fp, line, sizeof(line)));
                assert(is_two_number_line(skip_space(line)));
            }
            else if (trimmed[0] == 'E')
            {
                assert(consume_tilde_terminated_string(fp));
                assert(consume_tilde_terminated_string(fp));
            }
            else if (trimmed[0] == 'L')
            {
                assert(read_non_empty_line(fp, line, sizeof(line)));
                assert(is_single_number_line(skip_space(line)));
            }
            else
                assert(!"Unexpected trailing line in #OBJECTS entry");
        }
    }

    assert(!"unterminated #OBJECTS section (expected #0)");
}

static void test_mock_load_all_areas_and_validate_formats(void)
{
    FILE *list_fp = fopen("../area/area.lst", "r");
    char area_name[1024];

    if (list_fp == NULL)
        list_fp = fopen("area/area.lst", "r");

    assert(list_fp != NULL);

    while (fgets(area_name, sizeof(area_name), list_fp) != NULL)
    {
        char *nl = strpbrk(area_name, "\r\n");
        char area_path[2048];
        FILE *area_fp;
        char line[4096];

        if (nl != NULL)
            *nl = '\0';

        if (area_name[0] == '\0')
            continue;

        if (strcmp(area_name, "$") == 0)
            break;

        snprintf(area_path, sizeof(area_path), "../area/%s", area_name);
        area_fp = fopen(area_path, "r");
        if (area_fp == NULL)
        {
            snprintf(area_path, sizeof(area_path), "area/%s", area_name);
            area_fp = fopen(area_path, "r");
        }

        assert(area_fp != NULL);

        while (read_non_empty_line(area_fp, line, sizeof(line)))
        {
            if (line_starts_with(line, "#ROOMS"))
                validate_rooms_section_exit_format(area_fp);
            else if (line_starts_with(line, "#MOBILES"))
                validate_mobiles_section_format(area_fp);
            else if (line_starts_with(line, "#OBJECTS"))
                validate_objects_section_format(area_fp);
        }

        fclose(area_fp);
    }

    fclose(list_fp);
}

int main(void)
{
    test_format_status_builds_expected_message();
    test_set_area_name_truncates_and_terminates();
    test_try_read_help_level_accepts_numeric_prefix();
    test_try_read_help_level_rejects_non_numeric_prefix();
    test_mock_load_all_areas_and_validate_formats();

    puts("test_db: all tests passed");
    return 0;
}
