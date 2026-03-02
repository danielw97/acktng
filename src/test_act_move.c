#include <assert.h>
#include <stddef.h>

void act_move_testable_format_scout_line(char *dest, size_t dest_size, const char *pre, const char *name);

int main(void)
{
    char buf[24];
    act_move_testable_format_scout_line(buf, sizeof(buf), "Very Far North : ", "long-target-name-that-will-truncate");
    assert(buf[23] == '\0');
    return 0;
}
