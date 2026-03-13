#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_kiess_innkeeper(CHAR_DATA *ch)
{
   static const char *acts[] = {
      "$n checks the room ledger, running a finger down the current guest list.",
      "$n tops off a traveler's cup from a clay pitcher without being asked.",
      "$n straightens the traveler memorial plaque by the door, tracing a name quietly.",
      "$n glances at the city map mural and points out a route to a newly arrived guest.",
      "$n sets a fresh lamp on the counter, adjusting the wick for the evening hours.",
      "$n notes a recent return in the ledger and marks it with a small check — one more safe arrival.",
      "$n wipes down the counter after a traveler departs, then scans the common room to assess how full the night will be."
   };
   static char *says[] = {
      "First night in Kiess? The memorial plaque by the door has names — not to alarm you, but to remind you to come back.",
      "Roc Road is steady right now. Caravans in both directions. Good time to be traveling.",
      "The inn keeps a posted watch. You'll sleep undisturbed. City compact law holds inside these walls.",
      "If you're heading into the Forest of Confusion, check the cartographer's office east of the promenade first.",
      "Leave a forwarding note at the Postmaster's if you'll be away more than three days. Standard practice here.",
      "New here? The Gloamvault northwest of the city is a reasonable early target — cult ruins, compact layout. Worth your attention before you push toward the forest.",
      "Sepulcher Pasture sits east of the crossroads, past the banner hills. Burial cult remnants. Guests who've been there say it's difficult but navigable at the right level."
   };

   CHAR_DATA *plr;

   if (!IS_AWAKE(ch) || is_fighting(ch))
      return FALSE;

   if (number_bits(3) != 0)
      return FALSE;

   /* Level-based area hints for guests at appropriate experience levels */
   for (plr = ch->in_room->first_person; plr != NULL; plr = plr->next_in_room)
   {
      if (IS_NPC(plr) || plr->pcdata == NULL)
         continue;

      /* Gloamvault hint for newer adventurers */
      if (plr->level >= 5 && plr->level <= 15
         && !plr->pcdata->completed_static_quests[54]  /* Quest 55: Gloamvault threshold audit */
         && !plr->pcdata->completed_static_quests[66]) /* Quest 67: Gloamvault cartography */
      {
         do_say(ch, "If you haven't been to the Gloamvault yet, it's worth the walk. Northwest of the city — old cult ruin, dangerous enough to matter. Good place to calibrate yourself before heading to the forest.");
         return FALSE;
      }

      /* Sepulcher Pasture hint for mid-level adventurers */
      if (plr->level >= 15 && plr->level <= 28
         && !plr->pcdata->completed_static_quests[68]) /* Quest 69: Sepulcher Pasture cartography */
      {
         do_say(ch, "Heading out east? Sepulcher Pasture is past the crossroads — burial ground territory. Guests who've made the run say it's difficult but worthwhile if you're in the right range for it.");
         return FALSE;
      }
   }

   /* Normal random dialogue */
   if (number_bits(1) == 0)
      act(acts[number_range(0, 6)], ch, NULL, NULL, TO_ROOM);
   else
      do_say(ch, says[number_range(0, 6)]);

   return FALSE;
}
