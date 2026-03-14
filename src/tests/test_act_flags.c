#include <assert.h>
#include <stdio.h>

#include "config.h"
#define DEC_GLOBALS_H 1
#include "ack.h"

int main(void)
{
#ifdef ACT_WEREWOLF
#error "ACT_WEREWOLF should not be defined"
#endif

    assert(sizeof(((MOB_INDEX_DATA *)0)->act) == sizeof(unsigned long long int));
    assert(sizeof(((CHAR_DATA *)0)->act) == sizeof(unsigned long long int));

    assert((ACT_DAYONLY & ACT_NIGHTONLY) == 0);
    assert(ACT_DAYONLY == 2147483648ULL);
    assert(ACT_NIGHTONLY > 0xFFFFFFFFULL);
    assert(ACT_DAYONLY == BIT_32);
    assert(ACT_NIGHTONLY == BIT_33);

    puts("test_act_flags: all tests passed");
    return 0;
}
