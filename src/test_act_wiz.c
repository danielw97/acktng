#include <assert.h>
#include <stddef.h>
#include <string.h>

void act_wiz_testable_format_findreset(char *dest, size_t dest_size, const char *arg1, int vnum);

int main(void)
{
    char buf[32];
    act_wiz_testable_format_findreset(buf, sizeof(buf), "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz", 9);
    assert(buf[31] == '\0');
    assert(strstr(buf, "Findresets for") != NULL);
    return 0;
}
