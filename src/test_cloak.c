#include <assert.h>
#include <stdio.h>

#define DEC_GLOBALS_H 1
#include "magic.h"
#include "cloak.h"

static void test_cloak_reactive_can_trigger_only_for_non_physical(void)
{
    assert(cloak_reactive_can_trigger(ELE_PHYSICAL) == FALSE);
    assert(cloak_reactive_can_trigger(ELE_FIRE) == TRUE);
    assert(cloak_reactive_can_trigger(ELE_WATER | ELE_AIR) == TRUE);
    assert(cloak_reactive_can_trigger(ELE_PHYSICAL | ELE_FIRE) == FALSE);
    assert(cloak_reactive_can_trigger(ELE_NONE) == TRUE);
}

int main(void)
{
    test_cloak_reactive_can_trigger_only_for_non_physical();

    puts("test_cloak: all tests passed");
    return 0;
}
