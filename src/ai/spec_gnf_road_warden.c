#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_gnf_road_warden(CHAR_DATA *ch)
{
   static const char *acts[] = {
      "$n scans the treeline with practiced eyes, one hand resting on $s blade hilt.",
      "$n checks the condition of a nearby waystone, testing the iron lantern bracket for rust.",
      "$n marks something in a field ledger, then tucks it back into $s patrol coat.",
      "$n adjusts the joint-commission sash at $s waist, straightening the dual-city insignia.",
      "$n pauses at a road drainage ditch and kicks a clump of leaf debris clear with a boot.",
      "$n taps a waystone twice with a knuckle — two taps for 'passage clear', the old road signal.",
      "$n checks the lantern oil level at the nearest post and tops it from a flask on $s belt.",
      "$n updates a patrol notation on the waystone chalkboard, recording the hour and conditions."
   };
   static char *says[] = {
      "Road's open. Commission checked it this morning. Travel north with your documents visible.",
      "Split-pine badge means joint authority. Midgaard law south, Kowloon compact north, forest charter in between.",
      "If you see ash-painted faces off the road, don't engage. Signal any warden and move clear.",
      "Every warden on this road answers to the joint commission. We report to both cities equally. That's the charter.",
      "Lantern posts need oil at the next bend. If you pass before dark, the oil cache is at the base of the stone.",
      "Lantern Road wolf numbers have been elevated. If you're heading south, commission has an open task order for anyone with the skills to run a cull.",
      "Mirrorbark predator counts are running high this season. Commission wants a species census from someone who can handle the contact."
   };
   /* Great Northern Forest main road from Midgaard (south) to Kowloon (north) */
   static const int route[] = {
      15512, 15511, 15531, 15530, 15550, 15570, 15590, 15610, 15630, 15650,
      15670, 15690, 15710, 15730, 15750, 15751, 15752, 15753, 15773, 15774,
      15794, 15814, 15813, 15833, 15853, 15873, 15893, 15913, 15912, 15932,
      15952, 15972, 15992, 15993, 15994
   };
   static const int route_len = sizeof(route) / sizeof(route[0]);
   static const int endpoint_a = 15512; /* Midgaard end */
   static const int endpoint_b = 15994; /* Kowloon end */

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

         /* Quest 16 (id 15): Lantern Road wolf cull */
         if (plr->pcdata->completed_static_quests[15])
         {
            act("$n glances at $N and taps the waystone twice — an informal acknowledgment.", ch, NULL, plr, TO_NOTVICT);
            act("$n glances at you and taps the waystone twice — an informal acknowledgment.", ch, NULL, plr, TO_VICT);
            do_say(ch, "Lantern Road wolf count is down. Courier relay times improved by a quarter bell since the cull. Commission logged it. The road's safer for the work you did there.");
            return FALSE;
         }

         /* Quest 27 (id 26): Mirrorbark Predator Census */
         if (plr->pcdata->completed_static_quests[26])
         {
            act("$n adjusts $s patrol log and acknowledges $N with a warden's brief nod.", ch, NULL, plr, TO_NOTVICT);
            act("$n adjusts $s patrol log and acknowledges you with a warden's brief nod.", ch, NULL, plr, TO_VICT);
            do_say(ch, "Mirrorbark predator census filed with both city commands. Commission updated the corridor hazard ratings based on your count. Clean data — easier to patrol when the numbers are real.");
            return FALSE;
         }

         /* Quest 1 (id 0): Route reconnaissance — forest approach roads */
         if (plr->pcdata->completed_static_quests[0])
         {
            act("$n marks something in $s ledger and glances at $N with quiet recognition.", ch, NULL, plr, TO_NOTVICT);
            act("$n marks something in $s ledger and glances at you with quiet recognition.", ch, NULL, plr, TO_VICT);
            do_say(ch, "Your catrat threat data is in the commission route file now. Dispatch rescheduled two waypoint relays based on the pattern you confirmed. That's what field reports are for.");
            return FALSE;
         }
      }

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
            if (lv >= 5  && lv <= 20) hints[hint_count++] = "Not up to forest-level work yet? The Gloamvault north of Kiess is where most travelers find their range before hitting the main road. Cult ruin, manageable scope. Start there.";
            if (lv >= 10 && lv <= 25) hints[hint_count++] = "The Nightfall Catacombs are off the road commission's direct jurisdiction — undead vault complex, multi-layer structure. Commission logs departures in that direction from travelers past their early postings.";
            if (lv >= 15 && lv <= 30) hints[hint_count++] = "Sepulcher Pasture is east of the crossroads — off our patrol jurisdiction, but the commission cross-files data from there. Burial cult territory, bone activity. Worth your time if you're ready for mid-range work.";
            if (lv >= 20 && lv <= 35) hints[hint_count++] = "The Cathedral of the Violet Eclipse sits outside road commission jurisdiction but gets noted in the extended threat registry — eclipse cult site, relic guardians. Operatives at your range tend to log it before moving on to heavier work.";
            if (lv >= 25 && lv <= 40) hints[hint_count++] = "Umbra Heartspire is on the commission's long-range threat list — shadow constructs, void-alignment. Road wardens don't patrol near it, but we track reports from field operatives who do. High-capability territory.";
            if (hint_count > 0)
            {
               do_say(ch, hints[number_range(0, hint_count - 1)]);
               return FALSE;
            }
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
