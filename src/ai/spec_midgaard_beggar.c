#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_midgaard_beggar(CHAR_DATA *ch)
{
   static const char *acts[] = {
       "$n extends a worn bowl toward a passing stranger with a practised, unhurried gesture.",
       "$n shifts position slightly, keeping $s regular pitch despite the foot traffic.",
       "$n pulls a thin cloak tighter and watches the watch patrol pass at a careful distance.",
       "$n counts the coins in $s bowl quietly, then tucks them away before resuming the look of "
       "vacancy.",
       "$n calls out in a low voice to a figure crossing the intersection, words lost in the "
       "street noise."};
   static char *says[] = {
       "The welfare office has me on the daily ration roll. Doesn't cover everything. Never has.",
       "I had a pitch near the caravan square until the Trade Syndic filed a nuisance notice. Now "
       "I'm here.",
       "The Watch knows my face. We have an understanding. I stay on this corner, they leave me to "
       "it.",
       "Every temple in this city has a bread table at Bell Watch. If you don't know where it is, "
       "you haven't needed it yet.",
       "I can tell you what's been through this intersection today. Might be worth something to "
       "the right person."};

   CHAR_DATA *victim;
   int gold;

   if (ch->position != POS_STANDING)
      return FALSE;

   /* Pickpocket first */
   for (victim = ch->in_room->first_person; victim != NULL; victim = victim->next_in_room)
   {
      if (IS_NPC(victim) || victim->level >= LEVEL_STAFF || number_bits(2) != 0 ||
          !can_see(ch, victim))
         continue;

      if (IS_AWAKE(victim) && number_range(0, ch->level) == 0)
      {
         act("You catch $n's hand reaching for your coin purse!", ch, NULL, victim, TO_VICT);
         act("$N catches $n's hand in $S coin purse!", ch, NULL, victim, TO_NOTVICT);
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
