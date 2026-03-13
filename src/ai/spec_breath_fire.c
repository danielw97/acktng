#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"
#include "spec_shared.h"

bool spec_breath_fire(CHAR_DATA *ch)
{
   return dragon(ch, "fire breath");
}
