#include <assert.h>
#include <stddef.h>

void act_obj_testable_format_missing(char *dest, size_t dest_size, const char *name);

int main(void)
{
    char buf[20];
    act_obj_testable_format_missing(buf, sizeof(buf), "this-name-is-way-too-long-to-fit");
    assert(buf[19] == '\0');
    return 0;
}
