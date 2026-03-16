#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_ss_cinder_broker(CHAR_DATA *ch)
{
   CHAR_DATA *victim;
   int gold;
   static const char *acts[] = {
       "$n weighs two transit tokens on a thumb scale, then palms the heavier one with a practiced "
       "motion.",
       "$n whispers a route update to a caravan hand and marks a private tally in $s ledger strip.",
       "$n taps a crate seal and mutters, 'Uncertified additions cost extra at Cinder Gate.'",
       "$n drapes a rope canopy tag over one shoulder and scans the queue for nervous faces.",
       "$n flips a wax capsule in one hand, smiling as if every dispute has a price."};
   static char *says[] = {
       "Cinder Gate is a chokepoint, friend. You can pay in coin, favor, or time.",
       "Heat-cert marks matter more than declarations once the wardens start opening crates.",
       "I sell certainty in uncertain weather. That's worth more than water on bad days.",
       "No paper is perfect. I provide corrections before the next checkpoint sees the flaw.",
       "The queue is law, but the queue can be persuaded."};

   if (!IS_AWAKE(ch) || is_fighting(ch))
      return FALSE;

   for (victim = ch->in_room->first_person; victim != NULL; victim = victim->next_in_room)
   {
      if (IS_NPC(victim) || victim->level >= LEVEL_IMMORTAL || number_bits(3) != 0 ||
          !can_see(ch, victim))
         continue;

      if (IS_AWAKE(victim) && number_range(0, ch->level + 2) == 0)
      {
         act("You catch $n trying to swap one of your transit tokens!", ch, NULL, victim, TO_VICT);
         act("$N catches $n attempting a token swap!", ch, NULL, victim, TO_NOTVICT);
         return TRUE;
      }

      gold = victim->gold * number_range(1, 12) / 120;
      ch->gold += UMAX(0, gold);
      victim->gold -= UMAX(0, gold);
      return TRUE;
   }

   if (number_bits(3) != 0)
      return FALSE;

   if (number_bits(1) == 0)
      act(acts[number_range(0, 4)], ch, NULL, NULL, TO_ROOM);
   else
      do_say(ch, says[number_range(0, 4)]);

   return FALSE;
}
