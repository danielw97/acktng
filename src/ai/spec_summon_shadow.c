#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"
#include "spec_shared.h"

bool spec_summon_shadow(CHAR_DATA *ch)
{
   static const char *const spells[] = {"energy drain", "curse", "weaken"};

   if (spec_summon_heal_master(ch, 10))
      return TRUE;

   return spec_summon_cast_random(ch, ch->fighting, spells, 3);
}
