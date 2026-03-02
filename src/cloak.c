#ifndef DEC_ACK_H
#include "globals.h"
#endif
#include "magic.h"
#include "cloak.h"

bool cloak_reactive_can_trigger(int element)
{
    return !IS_SET(element, ELE_PHYSICAL);
}
