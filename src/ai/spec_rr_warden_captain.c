#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_rr_warden_captain(CHAR_DATA *ch)
{
   static const char *acts[] = {
      "$n studies a patrol assignment board, adjusting shift notations with a chalk stick.",
      "$n paces the warden post perimeter, checking sight-lines along both stretches of road.",
      "$n reads a dispatch report, folds it precisely, and slides it into the locked post-box.",
      "$n reviews a wanted notice against $s register, then pins it to the duty-board with practiced authority.",
      "$n speaks a quiet word with a junior warden, gesturing toward the road's western stretch.",
      "$n polishes the badge of rank on $s chest with a corner of the patrol cloak, force of long habit.",
      "$n updates the convoy route chart with the latest road condition report from the eastern segment.",
      "$n reviews the past week's incident log and marks three entries for follow-up."
   };
   static char *says[] = {
      "This post covers the full charter march from the gate to the boundary stone. Three wardens per shift, rotating dawn and dusk.",
      "The Three Civic Oaths are not decoration. Every warden recites them at shift change. Every single shift.",
      "I've requisitioned repair crews for the drainage channel at the grain field crossing. The order is six weeks old. It's still pending.",
      "Any report of trouble west of the boundary stone goes to both my log and the convoy compact. Joint jurisdiction means joint notification.",
      "The warden who doesn't know every face on their stretch doesn't know their stretch. Walk it until you do.",
      "Gateworks vermin have been interfering with early-morning cargo inspection. Anyone capable of running a purge would be doing this post a service.",
      "Gloamvault runs northwest off the Kiess approach. New field operatives who've cleared it tend to handle road incidents better than those who haven't. Worth noting if you're still building your range."
   };
   /* Roc Road main route from Midgaard (east) to Kiess (west) */
   static const int route[] = {
      3201, 3202, 3203, 3204, 3205, 3206, 3207, 3208, 3209, 3210,
      3211, 3212, 3213, 3214, 3215, 3216, 3217, 3218, 3219, 3220,
      3221, 3222, 3223, 3224, 3225, 3226, 3227, 3228, 3229, 3230,
      3231, 3232, 3233, 3234, 3235, 3236, 3237, 3238, 3239, 3240,
      3241, 3242, 3243
   };
   static const int route_len = sizeof(route) / sizeof(route[0]);
   static const int endpoint_a = 3201; /* Midgaard end */
   static const int endpoint_b = 3243; /* Kiess end */

   int cur_vnum, cur_idx, tgt_idx, next_vnum, door, i;
   EXIT_DATA *pexit;
   CHAR_DATA *plr;

   if (!IS_AWAKE(ch) || is_fighting(ch))
      return FALSE;

   /* Occasional flavor — with quest-reactive and level-hint priority */
   if (number_bits(3) == 0)
   {
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
               if (plr->pcdata->completed_static_quests[8])  { qplr[nq] = plr; qid[nq++] = 8;  }
               if (plr->pcdata->completed_static_quests[14]) { qplr[nq] = plr; qid[nq++] = 14; }
               if (plr->pcdata->completed_static_quests[15]) { qplr[nq] = plr; qid[nq++] = 15; }
            }

            if (nq > 0)
            {
               int pick = number_range(0, nq - 1);
               plr = qplr[pick];
               switch (qid[pick])
               {
                  case 8:
                     act("$n straightens and acknowledges $N with the formal nod of a warden to a cleared operative.", ch, NULL, plr, TO_NOTVICT);
                     act("$n straightens and acknowledges you with the formal nod of a warden to a cleared operative.", ch, NULL, plr, TO_VICT);
                     do_say(ch, "Dustward predator sweep filed. Convoy marshals rescheduled two route segments based on your clearance confirmation. Traffic is moving without the blind delays we had on that approach. Good work.");
                     return FALSE;
                  case 14:
                     act("$n marks $N's passing in $s incident log with a positive notation.", ch, NULL, plr, TO_NOTVICT);
                     act("$n marks your passing in $s incident log with a positive notation.", ch, NULL, plr, TO_VICT);
                     do_say(ch, "Gateworks vermin problem is cleared. Cargo inspection resumed at the standard schedule. I've updated the duty log — the disruption cost us eleven hours of census time. Your work put that right.");
                     return FALSE;
                  case 15:
                     act("$n pauses $s post walk and acknowledges $N with the straightened posture of formal recognition.", ch, NULL, plr, TO_NOTVICT);
                     act("$n pauses $s post walk and acknowledges you with the straightened posture of formal recognition.", ch, NULL, plr, TO_VICT);
                     do_say(ch, "Lantern Road wolf numbers are down. Courier relay has been running without delay-flag entries since the cull. I've filed a commendation notation in the district record. It counts.");
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
               if (lv >= 5  && lv <= 20) hints[hint_count++] = "Building field experience? The Gloamvault northwest of Kiess is where most capable travelers start before they're ready for road-level incidents. Cult ruins, defined perimeter. A solid first posting.";
               if (lv >= 10 && lv <= 25) hints[hint_count++] = "The Nightfall Catacombs are logged in the warden cross-file — undead vault, layered structure. Operatives who've cleared it tend to perform better on the road's secondary threat incidents.";
               if (lv >= 15 && lv <= 30) hints[hint_count++] = "Sepulcher Pasture is east of the crossroads — outside warden jurisdiction, but we track departures. Burial cult territory, bone-remnant activity. If you're in the right range for that kind of work, it's worth adding to your route.";
               if (lv >= 20 && lv <= 35) hints[hint_count++] = "The Cathedral of the Violet Eclipse is logged in the warden district cross-file — eclipse cult site, high-threat sacred architecture. Operatives at your tier should have it on their route planning.";
               if (lv >= 25 && lv <= 40) hints[hint_count++] = "Umbra Heartspire is on the warden long-range threat assessment — shadow constructs, void-bound defenders. High-capability work. Make sure your field record reflects that level before you approach it.";
               if (hint_count > 0)
               {
                  do_say(ch, hints[number_range(0, hint_count - 1)]);
                  return FALSE;
               }
            }
            break;
         }
      }

      /* Normal random dialogue */
      if (number_bits(1) == 0)
         act(acts[number_range(0, 7)], ch, NULL, NULL, TO_ROOM);
      else
         do_say(ch, says[number_range(0, 6)]);
   }

   /* Patrol movement: find current position on route */
   cur_vnum = ch->in_room->vnum;
   cur_idx = -1;
   for (i = 0; i < route_len; i++)
   {
      if (route[i] == cur_vnum)
      {
         cur_idx = i;
         break;
      }
   }

   if (cur_idx < 0)
      return FALSE; /* Not on patrol route, allow normal behavior */

   /* Initialize or flip patrol target when arriving at an endpoint */
   if (ch->hunt_home == NULL || ch->hunt_home->vnum == cur_vnum)
   {
      int new_target = (cur_vnum == endpoint_b) ? endpoint_a : endpoint_b;
      ch->hunt_home = get_room_index(new_target);
      if (ch->hunt_home == NULL)
         return FALSE;
   }

   /* Find target position in route */
   tgt_idx = -1;
   for (i = 0; i < route_len; i++)
   {
      if (route[i] == ch->hunt_home->vnum)
      {
         tgt_idx = i;
         break;
      }
   }

   if (tgt_idx < 0)
      return FALSE;

   /* Determine next room along the route */
   next_vnum = (cur_idx < tgt_idx) ? route[cur_idx + 1] : route[cur_idx - 1];

   /* Find the exit that leads to the next room and move */
   for (door = 0; door <= 5; door++)
   {
      pexit = ch->in_room->exit[door];
      if (pexit != NULL && pexit->to_room != NULL
          && pexit->to_room->vnum == next_vnum
          && !IS_SET(pexit->exit_info, EX_CLOSED))
      {
         move_char(ch, door);
         return TRUE;
      }
   }

   return TRUE; /* On route but couldn't move; prevent random wandering */
}
