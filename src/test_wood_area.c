#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void fail_area_test(const char *area_path, int line_number, const char *message)
{
    fprintf(stderr, "wood area test failure in %s", area_path != NULL ? area_path : "<unknown area>");
    if (line_number > 0)
        fprintf(stderr, " at line %d", line_number);
    fprintf(stderr, ": %s\n", message);
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
static void consume_tilde_terminated_string(FILE *fp, char *line, size_t line_size, int *line_number, const char *area_path)
{
    do
    {
        if (!read_area_line(fp, line, line_size, line_number))
            fail_area_test(area_path, *line_number, "unexpected EOF while reading ~-terminated string");
    } while (strchr(line, '~') == NULL);
}
static int is_exit_destination_line_valid(const char *line)
{
    char *end = NULL;
    int count = 0;
    const char *cursor = skip_space(line);
    while (*cursor != '\0' && *cursor != '\r' && *cursor != '\n')
    {
        strtol(cursor, &end, 10);
        if (end == cursor)
            return 0;
        count++;
        cursor = skip_space(end);
    }
    return count == 3;
}
static void assert_area_exit_destinations_are_numeric(const char *area_path)
{
    FILE *fp = fopen(area_path, "r");
    char line[4096];
    int saw_rooms = 0;
    int saw_room = 0;
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
        saw_room = 1;
        consume_tilde_terminated_string(fp, line, sizeof(line), &line_number, area_path);
        consume_tilde_terminated_string(fp, line, sizeof(line), &line_number, area_path);
        if (!read_area_line(fp, line, sizeof(line), &line_number))
            fail_area_test(area_path, line_number, "unexpected EOF while reading room flags line");
        for (;;)
        {
            if (!read_area_line(fp, line, sizeof(line), &line_number))
                fail_area_test(area_path, line_number, "unexpected EOF while reading room token");
            trimmed = skip_space(line);
            if (trimmed[0] == 'S')
                break;
            if (trimmed[0] == 'D')
            {
                consume_tilde_terminated_string(fp, line, sizeof(line), &line_number, area_path);
                consume_tilde_terminated_string(fp, line, sizeof(line), &line_number, area_path);
                if (!read_area_line(fp, line, sizeof(line), &line_number))
                    fail_area_test(area_path, line_number, "unexpected EOF while reading exit destination line");
                if (!is_exit_destination_line_valid(line))
                    fail_area_test(area_path, line_number, "exit destination line must contain exactly three integers");
                continue;
            }
            if (trimmed[0] == 'E')
            {
                consume_tilde_terminated_string(fp, line, sizeof(line), &line_number, area_path);
                consume_tilde_terminated_string(fp, line, sizeof(line), &line_number, area_path);
                continue;
            }
            fail_area_test(area_path, line_number, "room entry had unexpected token (expected D/E/S)");
        }
    }
    fclose(fp);
    if (saw_rooms)
    {
        if (!saw_room)
            fail_area_test(area_path, line_number, "#ROOMS section has no rooms");
    }
}
int main(void)
{
    FILE *list_fp = fopen("../area/area.lst", "r");
    char area_name[1024];
    int area_count = 0;
    if (list_fp == NULL)
        list_fp = fopen("area/area.lst", "r");
    if (list_fp == NULL)
        fail_area_test("area.lst", 0, "unable to open area list");
    while (fgets(area_name, sizeof(area_name), list_fp) != NULL)
    {
        char area_path[2048];
        char *nl = strpbrk(area_name, "\r\n");
        if (nl != NULL)
            *nl = '\0';
        if (area_name[0] == '\0')
            continue;
        if (strcmp(area_name, "$") == 0)
            break;
        snprintf(area_path, sizeof(area_path), "../area/%s", area_name);
        {
            FILE *check = fopen(area_path, "r");
            if (check == NULL)
            {
                snprintf(area_path, sizeof(area_path), "area/%s", area_name);
                check = fopen(area_path, "r");
            }
            if (check == NULL)
                fail_area_test(area_name, 0, "listed area file could not be opened");
            fclose(check);
        }
        assert_area_exit_destinations_are_numeric(area_path);
        area_count++;
    }
    fclose(list_fp);
    if (area_count <= 0)
        fail_area_test("area.lst", 0, "area list was empty");
    puts("test_wood_area: all tests passed");
    return 0;
}
