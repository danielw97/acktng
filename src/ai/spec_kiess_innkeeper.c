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

   /* Level-based area hints: random pick from eligible zones */
   {
      CHAR_DATA *hint_plr = NULL;
      for (plr = ch->in_room->first_person; plr != NULL; plr = plr->next_in_room)
      {
         if (!IS_NPC(plr) && plr->pcdata != NULL)
         {
            hint_plr = plr;
            break;
         }
      }
      if (hint_plr != NULL)
      {
         const char *hints[5];
         int hint_count = 0;
         int lv = hint_plr->level;
         if (lv >= 5  && lv <= 20) hints[hint_count++] = "If you haven't been to the Gloamvault yet, it's worth the walk. Northwest of the city — old cult ruin, dangerous enough to matter. Good place to calibrate yourself before heading to the forest.";
         if (lv >= 10 && lv <= 25) hints[hint_count++] = "Had a guest through last month who'd just come back from the Nightfall Catacombs. Said it was dark work — undead, vault traps, layered down. Appropriate for someone who's already done the Gloamvault run.";
         if (lv >= 15 && lv <= 30) hints[hint_count++] = "Heading out east? Sepulcher Pasture is past the crossroads — burial ground territory. Guests who've made the run say it's difficult but worthwhile if you're in the right range for it.";
         if (lv >= 20 && lv <= 35) hints[hint_count++] = "The Cathedral of the Violet Eclipse comes up in traveler conversation sometimes. Eclipse-cult site, relic guardians. Guests who've been through it tend to need a full night's rest afterward.";
         if (lv >= 25 && lv <= 40) hints[hint_count++] = "Umbra Heartspire isn't a name I hear often, but the guests who mention it are always the experienced ones. Shadow-bound structure, deep void elements. Not for someone who hasn't done serious dungeon work.";
         if (hint_count > 0)
         {
            do_say(ch, hints[number_range(0, hint_count - 1)]);
            return FALSE;
         }
      }
   }

   /* Normal random dialogue */
   if (number_bits(1) == 0)
      act(acts[number_range(0, 6)], ch, NULL, NULL, TO_ROOM);
   else
      do_say(ch, says[number_range(0, 6)]);

   return FALSE;
}
