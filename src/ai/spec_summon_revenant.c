/*
 * spec_summon_revenant.c — AI behavior for revenants.
 *
 * Adapts behavior based on the revenant's primary track:
 *   Dread/Shadow:  melee focus, occasional offensive spell
 *   Bone:          defensive, heals master more aggressively
 *   Soul:          caster focus, uses spells from mob's cast flags
 *   Plague:        poison/weaken/curse debuffs
 *   Blood:         harm + heal self, lifesteal theme
 *   None/default:  balanced (same as old animate behavior)
 */
#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"
#include "spec_shared.h"
#include "revenant.h"

bool spec_summon_revenant(CHAR_DATA *ch)
{
   REVENANT_DATA *rd;

   if (!is_revenant(ch))
      return FALSE;

   rd = ch->revenant;

   switch (rd->primary_track)
   {
   case REV_TRACK_SOUL:
   {
      /* Caster-focused: heal master aggressively, cast offensive spells */
      static const char *const spells[] = {"harm", "curse", "weaken"};

      if (spec_summon_heal_master(ch, 30))
         return TRUE;
      return spec_summon_cast_random(ch, ch->fighting, spells, 3);
   }

   case REV_TRACK_PLAGUE:
   {
      /* Debuff-focused: poison, weaken, curse */
      static const char *const spells[] = {"poison", "weaken", "curse"};

      if (spec_summon_heal_master(ch, 5))
         return TRUE;
      return spec_summon_cast_random(ch, ch->fighting, spells, 3);
   }

   case REV_TRACK_BLOOD:
   {
      /* Lifesteal: harm for damage, heal master */
      static const char *const spells[] = {"harm", "energy drain", "poison"};

      if (spec_summon_heal_master(ch, 20))
         return TRUE;
      return spec_summon_cast_random(ch, ch->fighting, spells, 3);
   }

   case REV_TRACK_BONE:
   {
      /* Tank: heal master very aggressively */
      static const char *const spells[] = {"harm", "weaken", "curse"};

      if (spec_summon_heal_master(ch, 40))
         return TRUE;
      return spec_summon_cast_random(ch, ch->fighting, spells, 3);
   }

   case REV_TRACK_DREAD:
   case REV_TRACK_SHADOW:
   default:
   {
      /* Melee-focused: mostly just fights, occasional spell */
      static const char *const spells[] = {"harm", "poison", "weaken"};

      if (spec_summon_heal_master(ch, 8))
         return TRUE;
      return spec_summon_cast_random(ch, ch->fighting, spells, 3);
   }
   }
}
