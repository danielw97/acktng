#include <assert.h>
#include <string.h>

void build_testable_join_command_and_argument(char *dest, size_t dest_size, const char *command, const char *argument);
void build_testable_format_vnum_usage_summary(char *dest, size_t dest_size, const char *label, const char *used, const char *free_text);

int main(void)
{
    char buf[128];

    build_testable_join_command_and_argument(buf, sizeof(buf), "set", "name");
    assert(strcmp(buf, "set name") == 0);

    build_testable_format_vnum_usage_summary(buf, sizeof(buf), "Room", "(Used)", "(Free)");
    assert(strstr(buf, "Room vnum usage summary") != NULL);

    build_testable_join_command_and_argument(buf, 8, "abcdef", "ghijk");
    assert(buf[7] == '\0');

    build_testable_join_command_and_argument(buf, sizeof(buf), NULL, "name");
    assert(strcmp(buf, " name") == 0);
    return 0;
}
