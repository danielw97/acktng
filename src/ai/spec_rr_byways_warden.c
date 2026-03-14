#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_rr_byways_warden(CHAR_DATA *ch)
{
   static const char *acts[] = {
      "$n checks the gate manifest against $s patrol assignment board.",
      "$n adjusts the ward mark on $s sash — the oak-and-road emblem of the barracks corps.",
      "$n consults the duty roster pinned to the barracks post, running a finger down the day's shifts.",
      "$n scans the yard perimeter with a steady, unhurried sweep before returning to $s post.",
      "$n tests the edge of $s blade with a thumbnail — satisfied, sheathes it without ceremony."
   };
   static char *says[] = {
      "The barracks reports to the road authority, not the city watch. Different charter, different mandate.",
      "Patrol rotation changes at the watch bell. If you need a warden, wait for relief; I don't leave post.",
      "The Banner Hills north ridge has had wolf activity. Travelers who go up there without preparation don't come back clean.",
      "Convoy manifests get checked here before they clear the eastern stretch. That's what the charter requires.",
      "The Inlet cave to the south-east has vermin — cave spiders, bats. The road authority tracks reported incursions.",
      "We've had increased traffic since the market opened at the crossroads. More eyes needed, same number of wardens."
   };

   CHAR_DATA *plr;

   if (!IS_AWAKE(ch) || is_fighting(ch))
      return FALSE;

   if (number_bits(3) != 0)
      return FALSE;

   switch (number_range(0, 2))
   {
      case 0:  /* Quest completion recognition */
      {
         CHAR_DATA *qplr[8];
         int qid[8];
         int nq = 0;

         for (plr = ch->in_room->first_person; plr != NULL; plr = plr->next_in_room)
         {
            if (IS_NPC(plr) || plr->pcdata == NULL)
               continue;
            if (plr->pcdata->completed_static_quests[76]) { qplr[nq] = plr; qid[nq++] = 76; }
            if (plr->pcdata->completed_static_quests[78]) { qplr[nq] = plr; qid[nq++] = 78; }
            if (plr->pcdata->completed_static_quests[79]) { qplr[nq] = plr; qid[nq++] = 79; }
         }

         if (nq > 0)
         {
            int pick = number_range(0, nq - 1);
            plr = qplr[pick];
            switch (qid[pick])
            {
               case 76:
                  act("$n glances at $N and marks a notation in the patrol incident log.", ch, NULL, plr, TO_NOTVICT);
                  act("$n glances at you and marks a notation in the patrol incident log.", ch, NULL, plr, TO_VICT);
                  do_say(ch, "Wolf count on the north ridge is down. The road authority got a preliminary report from the ridge lookout. Good outcome for convoy traffic on that stretch.");
                  return FALSE;
               case 78:
                  act("$n straightens $s posture and gives $N a measured nod of acknowledgment.", ch, NULL, plr, TO_NOTVICT);
                  act("$n straightens $s posture and gives you a measured nod of acknowledgment.", ch, NULL, plr, TO_VICT);
                  do_say(ch, "The underfoundation purge is on the patrol incident record. Unusual category. The wardens don't go down there — jurisdiction doesn't extend below road level — but the route is quieter for it.");
                  return FALSE;
               case 79:
                  act("$n consults $s incident log and marks $N's name against the cave clearance entry.", ch, NULL, plr, TO_NOTVICT);
                  act("$n consults $s incident log and marks your name against the cave clearance entry.", ch, NULL, plr, TO_VICT);
                  do_say(ch, "Cave pest traffic into the road margin has dropped. Fewer bite-wound reports from travelers camping near the Inlet. The census track confirms it.");
                  return FALSE;
            }
         }
         break;
      }
      case 1:  /* Area hint */
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
            if (lv >= 10 && lv <= 35) hints[hint_count++] = "The Inlet cave east of the crossroads is accessible and worth checking. Cave spiders and bats, manageable at your range. The road authority has posted incursion notices.";
            if (lv >= 20 && lv <= 50) hints[hint_count++] = "The Banner Hills north ridge has wolf packs that disrupted convoy traffic last season. The postmaster in Kiess had a notice up about it.";
            if (lv >= 40 && lv <= 80) hints[hint_count++] = "The Evermeet underfoundation, accessible from the Kiess gate road, has undead activity. Unusual category for this stretch — the Kiess guild registrar has an interest in the site being kept clear.";
            if (hint_count > 0)
            {
               do_say(ch, (char *)hints[number_range(0, hint_count - 1)]);
               return FALSE;
            }
         }
         break;
      }
   }

   if (number_bits(1) == 0)
      act(acts[number_range(0, 4)], ch, NULL, NULL, TO_ROOM);
   else
      do_say(ch, says[number_range(0, 5)]);

   return FALSE;
}
