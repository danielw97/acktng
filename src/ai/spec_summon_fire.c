#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"
#include "spec_shared.h"

bool spec_summon_fire(CHAR_DATA *ch)
{
   static const char *const spells[] = {"fireball", "high explosive", "curse"};

   if (spec_summon_heal_master(ch, 12))
      return TRUE;

   return spec_summon_cast_random(ch, ch->fighting, spells, 3);
}
