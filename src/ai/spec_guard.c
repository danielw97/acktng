#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_guard(CHAR_DATA *ch)
{
   return spec_policeman(ch);
}
