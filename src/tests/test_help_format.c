#include <assert.h>
#include <ctype.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"

static int has_invalid_filename_chars(const char *name)
{
    const unsigned char *p = (const unsigned char *)name;
    for (; *p; p++)
    {
        if (!(isalnum(*p) || *p == '.' || *p == '_' || *p == '-'))
            return 1;
    }
    return 0;
}

static void validate_help_file(const char *path)
{
    FILE *fp = fopen(path, "r");
    char line[8192];
    long level;
    char extra;
    size_t i;

    assert(fp != NULL);

    assert(fgets(line, sizeof(line), fp) != NULL);
    assert(sscanf(line, "level %ld %c", &level, &extra) == 1);
    (void)level;

    assert(fgets(line, sizeof(line), fp) != NULL);
    assert(strncmp(line, "keywords ", 9) == 0);
    {
        const char *keywords = line + 9;
        size_t keyword_len = strlen(keywords);
        assert(*keywords != '\0' && *keywords != '\n' && *keywords != '\r');
        while (keyword_len > 0 &&
               (keywords[keyword_len - 1] == '\n' || keywords[keyword_len - 1] == '\r'))
        {
            keyword_len--;
        }
        assert(keyword_len > 0);
        assert(!isspace((unsigned char)keywords[0]));
        assert(!isspace((unsigned char)keywords[keyword_len - 1]));
    }

    assert(fgets(line, sizeof(line), fp) != NULL);
    assert(strcmp(line, "---\n") == 0 || strcmp(line, "---\r\n") == 0 || strcmp(line, "---") == 0);

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        i = strlen(line);
        assert(i < sizeof(line) - 1 || line[i - 1] == '\n');
    }

    fclose(fp);
}

static int validate_help_directory(const char *directory)
{
    DIR *dir = opendir(directory);
    struct dirent *entry;
    int files_seen = 0;

    assert(dir != NULL);

    while ((entry = readdir(dir)) != NULL)
    {
        char path[4096];
        const char *name = entry->d_name;
        size_t len;

        if (name[0] == '.')
            continue;

        len = strlen(name);
        assert(len > 0);
        if (name[len - 1] == '~')
            continue;

        assert(has_invalid_filename_chars(name) == 0);

        snprintf(path, sizeof(path), "%s%s", directory, name);
        validate_help_file(path);
        files_seen++;
    }

    closedir(dir);
    return files_seen;
}

int main(void)
{
    int help_files = validate_help_directory(HELP_DIR);
    int shelp_files = validate_help_directory(SHELP_DIR);

    assert(help_files > 0);
    assert(shelp_files > 0);

    return 0;
}
