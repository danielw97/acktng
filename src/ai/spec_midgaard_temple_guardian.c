#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_midgaard_temple_guardian(CHAR_DATA *ch)
{
   static const char *acts[] = {
       "$n traces the ward-inscription on $s shield with a thumb, murmuring a consecration phrase.",
       "$n takes a measured position at the sanctum threshold, weight balanced, eyes forward.",
       "$n bows toward the altar in a brief, formal gesture of observance before resuming the "
       "watch.",
       "$n adjusts the ward-band on $s wrist, the reliquary sigil catching the brazier light.",
       "$n scans the hall with a practised sweep, noting every doorway and every face."};
   static char *says[] = {"The temple holds sanctuary status under the First Stone Accord. That "
                          "status has conditions. I enforce them.",
                          "The inner sanctum is accessible to the faithful and the registered. "
                          "Everyone else speaks with me first.",
                          "The ward sigils in this floor are older than the current city charter. "
                          "They have their own authority.",
                          "Weapons drawn without cause in the temple precinct are a Compact "
                          "violation. The Compact has teeth.",
                          "The guardian order has held this threshold since the Containment Era. "
                          "We do not abandon posts."};

   CHAR_DATA *victim;
   char *spell;
   int sn;

   /* Combat: cast cleric spells */
   if (is_fighting(ch))
   {
      for (victim = ch->in_room->first_person; victim != NULL; victim = victim->next_in_room)
         if (victim->fighting == ch && number_bits(2) == 0)
            break;

      if (victim != NULL)
      {
         for (;;)
         {
            int min_level;
            switch (number_bits(4))
            {
            case 0:
               min_level = 0;
               spell = "blindness";
               break;
            case 1:
               min_level = 3;
               spell = "cause serious";
               break;
            case 2:
               min_level = 7;
               spell = "earthquake";
               break;
            case 3:
               min_level = 9;
               spell = "cause critical";
               break;
            case 4:
               min_level = 10;
               spell = "dispel evil";
               break;
            case 5:
               min_level = 12;
               spell = "curse";
               break;
            case 6:
               min_level = 12;
               spell = "change sex";
               break;
            case 7:
               min_level = 13;
               spell = "flamestrike";
               break;
            case 8:
            case 9:
            case 10:
               min_level = 15;
               spell = "harm";
               break;
            default:
               min_level = 16;
               spell = "dispel magic";
               break;
            }
            if (ch->level >= min_level)
               break;
         }

         if ((sn = skill_lookup(spell)) >= 0)
         {
            act("The ward-inscriptions on $n's shield flare with sudden light!", ch, NULL, NULL,
                TO_ROOM);
            (*skill_table[sn].spell_fun)(sn, ch->level, ch, victim, NULL);
            return TRUE;
         }
      }
      return FALSE;
   }

   /* Ambient */
   if (!IS_AWAKE(ch))
      return FALSE;

   if (number_bits(3) != 0)
      return FALSE;

   if (number_bits(1) == 0)
      act(acts[number_range(0, 4)], ch, NULL, NULL, TO_ROOM);
   else
      do_say(ch, says[number_range(0, 4)]);

   return FALSE;
}
