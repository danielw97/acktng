#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_thief(CHAR_DATA *ch)
{
   CHAR_DATA *victim;
   int gold;

   if (ch->position != POS_STANDING)
      return FALSE;

   for (victim = ch->in_room->first_person; victim != NULL; victim = victim->next_in_room)
   {
      if (IS_NPC(victim) || victim->level >= LEVEL_IMMORTAL || number_bits(2) != 0 ||
          !can_see(ch, victim)) /* Thx Glop */
         continue;

      if (IS_AWAKE(victim) && number_range(0, ch->level) == 0)
      {
         act("You discover $n's hands in your wallet!", ch, NULL, victim, TO_VICT);
         act("$N discovers $n's hands in $S wallet!", ch, NULL, victim, TO_NOTVICT);
         return TRUE;
      }
      else
      {
         gold = victim->gold * number_range(1, 20) / 80;
         ch->gold += 7 * gold / 8;
         victim->gold -= gold;
         return TRUE;
      }
   }

   return FALSE;
}
