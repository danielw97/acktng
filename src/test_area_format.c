#include <assert.h>
#include <stdio.h>
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

static int is_section_header(const char *line)
{
    const char *trimmed = skip_space(line);

    if (trimmed[0] != '#')
        return 0;

    if (trimmed[1] == '0' || trimmed[1] == '$')
        return 0;

    return (trimmed[1] < '0' || trimmed[1] > '9');
}

static void consume_tilde_terminated_string(FILE *fp, char *line, size_t line_size)
{
    do
    {
        assert(fgets(line, line_size, fp) != NULL);
    } while (strchr(line, '~') == NULL);
}

static void assert_area_rooms_section_is_terminated(const char *area_path)
{
    FILE *fp = fopen(area_path, "r");
    char line[4096];
    int saw_rooms = 0;
    int saw_rooms_end = 0;

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
        {
            saw_rooms_end = 1;
            break;
        }

        assert(!is_section_header(trimmed));
    }

    fclose(fp);

    assert(saw_rooms);
    assert(saw_rooms_end);
}

static void assert_area_room_entries_use_valid_tokens(const char *area_path)
{
    FILE *fp = fopen(area_path, "r");
    char line[4096];
    int saw_rooms = 0;

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

    assert(saw_rooms);
}

static void assert_area_mobiles_section_is_terminated(const char *area_path)
{
    FILE *fp = fopen(area_path, "r");
    char line[4096];
    int saw_mobiles = 0;
    int saw_mobiles_end = 0;

    assert(fp != NULL);

    while (fgets(line, sizeof(line), fp) != NULL)
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

        assert(!line_starts_with(trimmed, "#OBJECTS"));
    }

    fclose(fp);

    if (!saw_mobiles)
        return;

    assert(saw_mobiles_end);
}

static void assert_area_objects_section_is_terminated(const char *area_path)
{
    FILE *fp = fopen(area_path, "r");
    char line[4096];
    int saw_objects = 0;
    int saw_objects_end = 0;

    assert(fp != NULL);

    while (fgets(line, sizeof(line), fp) != NULL)
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

        assert(!line_starts_with(trimmed, "#RESETS"));
    }

    fclose(fp);

    if (!saw_objects)
        return;

    assert(saw_objects_end);
}

static void test_all_areas_rooms_section_has_terminator(void)
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

            assert(area_fp != NULL);
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
