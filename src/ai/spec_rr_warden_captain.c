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
      /* Quest-completion reactions */
      for (plr = ch->in_room->first_person; plr != NULL; plr = plr->next_in_room)
      {
         if (IS_NPC(plr) || plr->pcdata == NULL)
            continue;

         /* Quest 9 (id 8): Convoy route security: Dustward predator sweep */
         if (plr->pcdata->completed_static_quests[8])
         {
            act("$n straightens and acknowledges $N with the formal nod of a warden to a cleared operative.", ch, NULL, plr, TO_NOTVICT);
            act("$n straightens and acknowledges you with the formal nod of a warden to a cleared operative.", ch, NULL, plr, TO_VICT);
            do_say(ch, "Dustward predator sweep filed. Convoy marshals rescheduled two route segments based on your clearance confirmation. Traffic is moving without the blind delays we had on that approach. Good work.");
            return FALSE;
         }

         /* Quest 15 (id 14): Gateworks vermin purge */
         if (plr->pcdata->completed_static_quests[14])
         {
            act("$n marks $N's passing in $s incident log with a positive notation.", ch, NULL, plr, TO_NOTVICT);
            act("$n marks your passing in $s incident log with a positive notation.", ch, NULL, plr, TO_VICT);
            do_say(ch, "Gateworks vermin problem is cleared. Cargo inspection resumed at the standard schedule. I've updated the duty log — the disruption cost us eleven hours of census time. Your work put that right.");
            return FALSE;
         }

         /* Quest 16 (id 15): Lantern Road wolf cull */
         if (plr->pcdata->completed_static_quests[15])
         {
            act("$n pauses $s post walk and acknowledges $N with the straightened posture of formal recognition.", ch, NULL, plr, TO_NOTVICT);
            act("$n pauses $s post walk and acknowledges you with the straightened posture of formal recognition.", ch, NULL, plr, TO_VICT);
            do_say(ch, "Lantern Road wolf numbers are down. Courier relay has been running without delay-flag entries since the cull. I've filed a commendation notation in the district record. It counts.");
            return FALSE;
         }
      }

      /* Level-based area hints */
      for (plr = ch->in_room->first_person; plr != NULL; plr = plr->next_in_room)
      {
         if (IS_NPC(plr) || plr->pcdata == NULL)
            continue;

         /* Gloamvault hint for low-level players */
         if (plr->level >= 5 && plr->level <= 20
            && !plr->pcdata->completed_static_quests[54]  /* Quest 55: Gloamvault threshold audit */
            && !plr->pcdata->completed_static_quests[66]) /* Quest 67: Gloamvault cartography */
         {
            do_say(ch, "Building field experience? The Gloamvault northwest of Kiess is where most capable travelers start before they're ready for road-level incidents. Cult ruins, defined perimeter. A solid first posting.");
            return FALSE;
         }

         /* Sepulcher Pasture hint for mid-level players */
         if (plr->level >= 15 && plr->level <= 30
            && !plr->pcdata->completed_static_quests[68]) /* Quest 69: Sepulcher Pasture cartography */
         {
            do_say(ch, "Sepulcher Pasture is east of the crossroads — outside warden jurisdiction, but we track departures. Burial cult territory, bone-remnant activity. If you're in the right range for that kind of work, it's worth adding to your route.");
            return FALSE;
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
