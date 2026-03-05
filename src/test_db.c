#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
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

static int read_non_blank_content_line(FILE *fp, char *buf, size_t buf_size)
{
    while (read_non_empty_line(fp, buf, buf_size))
    {
        const char *p = buf;

        while (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n')
            p++;

        if (*p != '\0')
            return 1;
    }

    return 0;
}

static void fail_area_validation(const char *area_name, int area_line, const char *fmt, ...)
{
    va_list args;

    fprintf(stderr, "test_db area validation failure in %s", area_name != NULL ? area_name : "<unknown area>");
    if (area_line > 0)
        fprintf(stderr, " at line %d", area_line);
    fprintf(stderr, ": ");

    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);

    fputc('\n', stderr);
    exit(1);
}

static int current_area_line(FILE *fp)
{
    int line_number = 1;
    long marker = ftell(fp);
    int c;

    if (marker < 0)
        return 0;

    assert(fseek(fp, 0, SEEK_SET) == 0);
    while (ftell(fp) < marker && (c = fgetc(fp)) != EOF)
    {
        if (c == '\n')
            line_number++;
    }
    assert(fseek(fp, marker, SEEK_SET) == 0);

    return line_number;
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

static void validate_vnum_in_area_range(const char *area_name, int area_line, int area_min_vnum, int area_max_vnum, int vnum,
                                        const char *context)
{
    if (vnum < area_min_vnum || vnum > area_max_vnum)
    {
        fail_area_validation(area_name, area_line, "%s vnum %d is outside assigned area range %d-%d", context, vnum,
                             area_min_vnum, area_max_vnum);
    }
}

static void validate_area_uses_only_assigned_vnums(FILE *fp, const char *area_name)
{
    char line[4096];
    int area_min_vnum = 0;
    int area_max_vnum = 0;
    int has_vnum_range = 0;
    enum
    {
        AREA_SECTION_OTHER,
        AREA_SECTION_HEADER,
        AREA_SECTION_ROOMS,
        AREA_SECTION_MOBILES,
        AREA_SECTION_OBJECTS,
        AREA_SECTION_SHOPS,
        AREA_SECTION_SPECIALS
    } section = AREA_SECTION_OTHER;

    rewind(fp);

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        const char *trimmed = skip_space(line);
        int vnum;
        int parsed_max;

        if (line_starts_with(trimmed, "#AREA"))
        {
            section = AREA_SECTION_HEADER;
            continue;
        }
        if (line_starts_with(trimmed, "#ROOMS"))
        {
            section = AREA_SECTION_ROOMS;
            continue;
        }
        if (line_starts_with(trimmed, "#MOBILES"))
        {
            section = AREA_SECTION_MOBILES;
            continue;
        }
        if (line_starts_with(trimmed, "#OBJECTS"))
        {
            section = AREA_SECTION_OBJECTS;
            continue;
        }
        if (line_starts_with(trimmed, "#SHOPS"))
        {
            section = AREA_SECTION_SHOPS;
            continue;
        }
        if (line_starts_with(trimmed, "#SPECIALS"))
        {
            section = AREA_SECTION_SPECIALS;
            continue;
        }
        if (trimmed[0] == '#' && section != AREA_SECTION_HEADER)
            section = AREA_SECTION_OTHER;

        if (section == AREA_SECTION_HEADER && sscanf(trimmed, "V %d %d", &area_min_vnum, &parsed_max) == 2)
        {
            area_max_vnum = parsed_max;
            has_vnum_range = 1;
            if (area_min_vnum > area_max_vnum)
                fail_area_validation(area_name, current_area_line(fp), "invalid V vnum range %d-%d", area_min_vnum,
                                     area_max_vnum);
            continue;
        }

        if (!has_vnum_range)
            continue;

        if ((section == AREA_SECTION_ROOMS || section == AREA_SECTION_MOBILES || section == AREA_SECTION_OBJECTS)
            && sscanf(trimmed, "#%d", &vnum) == 1 && vnum != 0)
        {
            validate_vnum_in_area_range(area_name, current_area_line(fp), area_min_vnum, area_max_vnum, vnum,
                                        "section entry");
            continue;
        }

        if (section == AREA_SECTION_SHOPS && sscanf(trimmed, "%d", &vnum) == 1 && vnum != 0)
        {
            validate_vnum_in_area_range(area_name, current_area_line(fp), area_min_vnum, area_max_vnum, vnum,
                                        "shopkeeper");
            continue;
        }

        if (section == AREA_SECTION_SPECIALS
            && ((sscanf(trimmed, "M %d", &vnum) == 1) || (sscanf(trimmed, "O %d", &vnum) == 1)))
        {
            validate_vnum_in_area_range(area_name, current_area_line(fp), area_min_vnum, area_max_vnum, vnum,
                                        "special target");
            continue;
        }
    }

    if (!has_vnum_range)
        fail_area_validation(area_name, 0, "missing V <min> <max> directive in #AREA section");

    rewind(fp);
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

static int consume_tilde_terminated_string(FILE *fp, int require_same_line)
{
    int c;
    int saw_newline = 0;

    while ((c = fgetc(fp)) != EOF)
    {
        if (c == '\n' || c == '\r')
            saw_newline = 1;

        if (c == '~')
        {
            if (require_same_line && saw_newline)
                return 0;

            while (c != EOF && c != '\n')
                c = fgetc(fp);
            return 1;
        }
    }

    return 0;
}

static void validate_rooms_section_exit_format(FILE *fp, const char *area_name)
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
                if (!consume_tilde_terminated_string(fp, 0) || !consume_tilde_terminated_string(fp, 0))
                    fail_area_validation(area_name, current_area_line(fp), "unterminated exit description in #ROOMS entry");
                if (!read_non_blank_content_line(fp, line, sizeof(line)))
                    fail_area_validation(area_name, current_area_line(fp), "unexpected EOF while reading room exit destination line");
                if (!is_exit_triple_line(skip_space(line)))
                    fail_area_validation(area_name, current_area_line(fp), "invalid room exit destination line: %s", line);
            }
        }
    }

    fail_area_validation(area_name, current_area_line(fp), "unterminated #ROOMS section (expected #0)");
}

static void validate_mobiles_section_format(FILE *fp, const char *area_name)
{
    char line[4096];

    while (read_non_empty_line(fp, line, sizeof(line)))
    {
        const char *trimmed = skip_space(line);

        if (line_starts_with(trimmed, "#0"))
            return;

        if (trimmed[0] != '#')
            fail_area_validation(area_name, current_area_line(fp), "expected mobile entry vnum, got: %s", trimmed);

        if (!consume_tilde_terminated_string(fp, 0) || !consume_tilde_terminated_string(fp, 0)
            || !consume_tilde_terminated_string(fp, 0) || !consume_tilde_terminated_string(fp, 0))
            fail_area_validation(area_name, current_area_line(fp), "unterminated mobile string field");

        if (!read_non_blank_content_line(fp, line, sizeof(line)) || strchr(line, 'S') == NULL)
            fail_area_validation(area_name, current_area_line(fp), "mobile flags line missing expected 'S': %s", line);
        if (!read_non_blank_content_line(fp, line, sizeof(line)) || !is_two_number_line(skip_space(line)))
            fail_area_validation(area_name, current_area_line(fp), "mobile level/alignment line is invalid: %s", line);
        if (!read_non_blank_content_line(fp, line, sizeof(line)) || !is_four_number_line(skip_space(line)))
            fail_area_validation(area_name, current_area_line(fp), "mobile combat line is invalid: %s", line);

        for (;;)
        {
            long marker;

            marker = ftell(fp);
            if (!read_non_blank_content_line(fp, line, sizeof(line)))
                return;

            trimmed = skip_space(line);
            if (trimmed[0] == '#')
            {
                if (fseek(fp, marker, SEEK_SET) != 0)
                    fail_area_validation(area_name, current_area_line(fp), "unable to rewind to next mobile marker");
                break;
            }

            if (trimmed[0] == '!')
            {
                if (!has_prefixed_int_count(trimmed, '!', 7))
                    fail_area_validation(area_name, current_area_line(fp), "invalid ! trailer line: %s", trimmed);
            }
            else if (trimmed[0] == '|')
            {
                if (!has_prefixed_int_count(trimmed, '|', 7))
                    fail_area_validation(area_name, current_area_line(fp), "invalid | trailer line: %s", trimmed);
            }
            else if (trimmed[0] == '+')
            {
                if (!has_prefixed_int_count(trimmed, '+', 9))
                    fail_area_validation(area_name, current_area_line(fp), "invalid + trailer line: %s", trimmed);
            }
            else if (trimmed[0] == 'l')
            {
                if (!has_prefixed_int_count(trimmed, 'l', 10))
                    fail_area_validation(area_name, current_area_line(fp), "invalid l trailer line: %s", trimmed);
            }
            else if (trimmed[0] == 'L')
            {
                if (!has_prefixed_int_count(trimmed, 'L', 9))
                    fail_area_validation(area_name, current_area_line(fp), "invalid L trailer line: %s", trimmed);
            }
            else if (trimmed[0] == '>')
            {
                while (read_non_empty_line(fp, line, sizeof(line)))
                {
                    if (skip_space(line)[0] == '\0')
                        continue;

                    if (skip_space(line)[0] == '|')
                        break;
                }
            }
            else
                fail_area_validation(area_name, current_area_line(fp), "unexpected trailing line in #MOBILES entry: %s", trimmed);
        }
    }

    fail_area_validation(area_name, current_area_line(fp), "unterminated #MOBILES section (expected #0)");
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

        validate_area_uses_only_assigned_vnums(area_fp, area_name);

        while (read_non_empty_line(area_fp, line, sizeof(line)))
        {
            if (line_starts_with(line, "#ROOMS"))
                validate_rooms_section_exit_format(area_fp, area_name);
            else if (line_starts_with(line, "#MOBILES"))
                validate_mobiles_section_format(area_fp, area_name);
        }

        fclose(area_fp);
    }

    fclose(list_fp);
}

static void test_area_list_has_no_duplicate_entries(void)
{
    FILE *list_fp = fopen("../area/area.lst", "r");
    char area_name[1024];
    char seen_names[512][1024];
    int seen_count = 0;

    if (list_fp == NULL)
        list_fp = fopen("area/area.lst", "r");

    assert(list_fp != NULL);

    while (fgets(area_name, sizeof(area_name), list_fp) != NULL)
    {
        char *nl = strpbrk(area_name, "\r\n");

        if (nl != NULL)
            *nl = '\0';

        if (area_name[0] == '\0')
            continue;

        if (strcmp(area_name, "$") == 0)
            break;

        for (int i = 0; i < seen_count; i++)
            assert(strcmp(seen_names[i], area_name) != 0);

        assert(seen_count < (int)(sizeof(seen_names) / sizeof(seen_names[0])));
        strncpy(seen_names[seen_count], area_name, sizeof(seen_names[0]) - 1);
        seen_names[seen_count][sizeof(seen_names[0]) - 1] = '\0';
        seen_count++;
    }

    fclose(list_fp);
}

static int parse_index_vnum_header(const char *line, int *out_vnum)
{
    int vnum;
    char trailing;

    if (sscanf(skip_space(line), "#%d %c", &vnum, &trailing) == 1)
    {
        *out_vnum = vnum;
        return 1;
    }

    return 0;
}

static void test_area_index_vnums_have_no_duplicates(void)
{
    enum
    {
        INDEX_SECTION_NONE,
        INDEX_SECTION_ROOMS,
        INDEX_SECTION_MOBILES,
        INDEX_SECTION_OBJECTS
    } section = INDEX_SECTION_NONE;
    FILE *list_fp = fopen("../area/area.lst", "r");
    char area_name[1024];
    static int seen_room_line[65536] = {0};
    static int seen_mob_line[65536] = {0};
    static int seen_obj_line[65536] = {0};
    static char seen_room_area[65536][64] = {{0}};
    static char seen_mob_area[65536][64] = {{0}};
    static char seen_obj_area[65536][64] = {{0}};

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
        section = INDEX_SECTION_NONE;

        while (fgets(line, sizeof(line), area_fp) != NULL)
        {
            int *seen_line = NULL;
            char (*seen_area)[64] = NULL;
            int vnum;
            int line_no;

            if (line_starts_with(line, "#ROOMS"))
            {
                section = INDEX_SECTION_ROOMS;
                continue;
            }
            if (line_starts_with(line, "#MOBILES"))
            {
                section = INDEX_SECTION_MOBILES;
                continue;
            }
            if (line_starts_with(line, "#OBJECTS"))
            {
                section = INDEX_SECTION_OBJECTS;
                continue;
            }

            if (section == INDEX_SECTION_NONE)
                continue;

            if (!parse_index_vnum_header(line, &vnum) || vnum == 0)
                continue;

            assert(vnum >= 0 && vnum <= 65535);

            if (section == INDEX_SECTION_ROOMS)
            {
                seen_line = seen_room_line;
                seen_area = seen_room_area;
            }
            else if (section == INDEX_SECTION_MOBILES)
            {
                seen_line = seen_mob_line;
                seen_area = seen_mob_area;
            }
            else
            {
                seen_line = seen_obj_line;
                seen_area = seen_obj_area;
            }

            line_no = current_area_line(area_fp);

            if (seen_line[vnum] != 0)
            {
                fail_area_validation(area_name, line_no,
                                     "duplicate index vnum %d in %s (first seen in %s line %d)",
                                     vnum,
                                     section == INDEX_SECTION_ROOMS ? "#ROOMS"
                                     : section == INDEX_SECTION_MOBILES ? "#MOBILES"
                                     : "#OBJECTS",
                                     seen_area[vnum], seen_line[vnum]);
            }

            seen_line[vnum] = line_no;
            strncpy(seen_area[vnum], area_name, sizeof(seen_area[vnum]) - 1);
            seen_area[vnum][sizeof(seen_area[vnum]) - 1] = '\0';
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
    test_area_list_has_no_duplicate_entries();
    test_area_index_vnums_have_no_duplicates();

    puts("test_db: all tests passed");
    return 0;
}
