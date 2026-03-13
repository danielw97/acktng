#include <stdio.h>
#include "config.h"

int main(void)
{
#ifdef ACT_WEREWOLF
#error "ACT_WEREWOLF should not be defined"
#endif
    puts("test_act_flags: all tests passed");
    return 0;
}
