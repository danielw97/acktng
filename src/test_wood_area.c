#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
static void consume_tilde_terminated_string(FILE *fp, char *line, size_t line_size)
{
    do
    {
        assert(fgets(line, line_size, fp) != NULL);
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
    assert(fp != NULL);
    while (fgets(line, sizeof(line), fp) != NULL)
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
        consume_tilde_terminated_string(fp, line, sizeof(line));
        consume_tilde_terminated_string(fp, line, sizeof(line));
        assert(fgets(line, sizeof(line), fp) != NULL);
        for (;;)
        {
            assert(fgets(line, sizeof(line), fp) != NULL);
            trimmed = skip_space(line);
            if (trimmed[0] == 'S')
                break;
            if (trimmed[0] == 'D')
            {
                consume_tilde_terminated_string(fp, line, sizeof(line));
                consume_tilde_terminated_string(fp, line, sizeof(line));
                assert(fgets(line, sizeof(line), fp) != NULL);
                assert(is_exit_destination_line_valid(line));
                continue;
            }
            if (trimmed[0] == 'E')
            {
                consume_tilde_terminated_string(fp, line, sizeof(line));
                consume_tilde_terminated_string(fp, line, sizeof(line));
                continue;
            }
            assert(0 && "Room entry had unexpected token (expected D/E/S)");
        }
    }
    fclose(fp);
    if (saw_rooms)
        assert(saw_room);
}
int main(void)
{
    FILE *list_fp = fopen("../area/area.lst", "r");
    char area_name[1024];
    int area_count = 0;
    if (list_fp == NULL)
        list_fp = fopen("area/area.lst", "r");
    assert(list_fp != NULL);
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
            assert(check != NULL);
            fclose(check);
        }
        assert_area_exit_destinations_are_numeric(area_path);
        area_count++;
    }
    fclose(list_fp);
    assert(area_count > 0);
    puts("test_wood_area: all tests passed");
    return 0;
}
