#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"
#include "spec_shared.h"

bool spec_breath_lightning(CHAR_DATA *ch)
{
   return dragon(ch, "lightning breath");
}
