#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void fail_area_test(const char *area_path, int line_number, const char *fmt, ...)
{
    va_list args;

    fprintf(stderr, "area format test failure in %s", area_path != NULL ? area_path : "<unknown area>");
    if (line_number > 0)
        fprintf(stderr, " at line %d", line_number);
    fprintf(stderr, ": ");

    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);

    fputc('\n', stderr);
    exit(1);
}

static int read_area_line(FILE *fp, char *line, size_t line_size, int *line_number)
{
    if (fgets(line, line_size, fp) == NULL)
        return 0;

    (*line_number)++;
    return 1;
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

static int is_section_header(const char *line)
{
    const char *trimmed = skip_space(line);

    if (trimmed[0] != '#')
        return 0;

    if (trimmed[1] == '0' || trimmed[1] == '$')
        return 0;

    return (trimmed[1] < '0' || trimmed[1] > '9');
}

static void consume_tilde_terminated_string(FILE *fp, char *line, size_t line_size, int *line_number, const char *area_path)
{
    do
    {
        if (!read_area_line(fp, line, line_size, line_number))
            fail_area_test(area_path, *line_number, "unexpected EOF while reading ~-terminated string");
    } while (strchr(line, '~') == NULL);
}

static void assert_area_rooms_section_is_terminated(const char *area_path)
{
    FILE *fp = fopen(area_path, "r");
    char line[4096];
    int line_number = 0;
    int saw_rooms = 0;
    int saw_rooms_end = 0;

    if (fp == NULL)
        fail_area_test(area_path, 0, "unable to open area file");

    while (read_area_line(fp, line, sizeof(line), &line_number))
    {
        const char *trimmed = skip_space(line);

        if (!saw_rooms)
        {
            if (line_starts_with(trimmed, "#ROOMS"))
                saw_rooms = 1;
            continue;
        }

        if (line_starts_with(trimmed, "#0"))
        {
            saw_rooms_end = 1;
            break;
        }

        if (is_section_header(trimmed))
            fail_area_test(area_path, line_number, "found new section header before #ROOMS terminator #0");
    }

    fclose(fp);

    if (!saw_rooms)
        fail_area_test(area_path, 0, "missing #ROOMS section");
    if (!saw_rooms_end)
        fail_area_test(area_path, line_number, "missing #0 terminator for #ROOMS section");
}

static void assert_area_room_entries_use_valid_tokens(const char *area_path)
{
    FILE *fp = fopen(area_path, "r");
    char line[4096];
    int saw_rooms = 0;
    int line_number = 0;

    if (fp == NULL)
        fail_area_test(area_path, 0, "unable to open area file");

    while (read_area_line(fp, line, sizeof(line), &line_number))
    {
        const char *trimmed = skip_space(line);

        if (!saw_rooms)
        {
            if (line_starts_with(trimmed, "#ROOMS"))
                saw_rooms = 1;
            continue;
        }

        if (line_starts_with(trimmed, "#0"))
            break;

        if (!(trimmed[0] == '#' && trimmed[1] >= '0' && trimmed[1] <= '9'))
            continue;

        consume_tilde_terminated_string(fp, line, sizeof(line), &line_number, area_path);
        consume_tilde_terminated_string(fp, line, sizeof(line), &line_number, area_path);
        if (!read_area_line(fp, line, sizeof(line), &line_number))
            fail_area_test(area_path, line_number, "unexpected EOF while reading room flags line");

        for (;;)
        {
            if (!read_area_line(fp, line, sizeof(line), &line_number))
                fail_area_test(area_path, line_number, "unexpected EOF while reading room entry token");
            trimmed = skip_space(line);

            if (trimmed[0] == 'S')
                break;

            if (trimmed[0] == 'D')
            {
                consume_tilde_terminated_string(fp, line, sizeof(line), &line_number, area_path);
                consume_tilde_terminated_string(fp, line, sizeof(line), &line_number, area_path);
                if (!read_area_line(fp, line, sizeof(line), &line_number))
                    fail_area_test(area_path, line_number, "unexpected EOF while reading exit destination line");
                continue;
            }

            if (trimmed[0] == 'E')
            {
                consume_tilde_terminated_string(fp, line, sizeof(line), &line_number, area_path);
                consume_tilde_terminated_string(fp, line, sizeof(line), &line_number, area_path);
                continue;
            }

            fail_area_test(area_path, line_number, "room entry had unexpected token (expected D/E/S), got: %s", trimmed);
        }
    }

    fclose(fp);

    if (!saw_rooms)
        fail_area_test(area_path, 0, "missing #ROOMS section");
}

static void assert_area_mobiles_section_is_terminated(const char *area_path)
{
    FILE *fp = fopen(area_path, "r");
    char line[4096];
    int saw_mobiles = 0;
    int saw_mobiles_end = 0;
    int line_number = 0;

    if (fp == NULL)
        fail_area_test(area_path, 0, "unable to open area file");

    while (read_area_line(fp, line, sizeof(line), &line_number))
    {
        const char *trimmed = skip_space(line);

        if (!saw_mobiles)
        {
            if (line_starts_with(trimmed, "#MOBILES"))
                saw_mobiles = 1;
            continue;
        }

        if (line_starts_with(trimmed, "#0"))
        {
            saw_mobiles_end = 1;
            break;
        }

        if (line_starts_with(trimmed, "#OBJECTS"))
            fail_area_test(area_path, line_number, "encountered #OBJECTS before #MOBILES terminator #0");
    }

    fclose(fp);

    if (!saw_mobiles)
        return;

    if (!saw_mobiles_end)
        fail_area_test(area_path, line_number, "missing #0 terminator for #MOBILES section");
}

static void assert_area_objects_section_is_terminated(const char *area_path)
{
    FILE *fp = fopen(area_path, "r");
    char line[4096];
    int saw_objects = 0;
    int saw_objects_end = 0;
    int line_number = 0;

    if (fp == NULL)
        fail_area_test(area_path, 0, "unable to open area file");

    while (read_area_line(fp, line, sizeof(line), &line_number))
    {
        const char *trimmed = skip_space(line);

        if (!saw_objects)
        {
            if (line_starts_with(trimmed, "#OBJECTS"))
                saw_objects = 1;
            continue;
        }

        if (line_starts_with(trimmed, "#0"))
        {
            saw_objects_end = 1;
            break;
        }

        if (line_starts_with(trimmed, "#RESETS"))
            fail_area_test(area_path, line_number, "encountered #RESETS before #OBJECTS terminator #0");
    }

    fclose(fp);

    if (!saw_objects)
        return;

    if (!saw_objects_end)
        fail_area_test(area_path, line_number, "missing #0 terminator for #OBJECTS section");
}

static void test_all_areas_rooms_section_has_terminator(void)
{
    FILE *list_fp = fopen("../area/area.lst", "r");
    char area_name[1024];

    if (list_fp == NULL)
        list_fp = fopen("area/area.lst", "r");

    if (list_fp == NULL)
        fail_area_test("area.lst", 0, "unable to open area list");

    while (fgets(area_name, sizeof(area_name), list_fp) != NULL)
    {
        char *nl = strpbrk(area_name, "\r\n");
        char area_path[2048];

        if (nl != NULL)
            *nl = '\0';

        if (area_name[0] == '\0')
            continue;

        if (strcmp(area_name, "$") == 0)
            break;

        snprintf(area_path, sizeof(area_path), "../area/%s", area_name);
        {
            FILE *area_fp = fopen(area_path, "r");

            if (area_fp == NULL)
            {
                snprintf(area_path, sizeof(area_path), "area/%s", area_name);
                area_fp = fopen(area_path, "r");
            }

            if (area_fp == NULL)
                fail_area_test(area_name, 0, "listed area file could not be opened");
            fclose(area_fp);
        }

        assert_area_rooms_section_is_terminated(area_path);
        assert_area_room_entries_use_valid_tokens(area_path);
        assert_area_mobiles_section_is_terminated(area_path);
        assert_area_objects_section_is_terminated(area_path);
    }

    fclose(list_fp);
}

int main(void)
{
    test_all_areas_rooms_section_has_terminator();
    puts("test_area_format: all tests passed");
    return 0;
}
