#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_gnf_joint_scout(CHAR_DATA *ch)
{
   static const char *acts[] = {
      "$n marks a notation on $s patrol map with the joint-commission cipher.",
      "$n scans the treeline with alert, resting eyes — the watchfulness of someone in field-recovery.",
      "$n checks $s commission writ, confirming the dual-city seal is still intact.",
      "$n counts boot-prints on the soft shoulder of the road with careful attention.",
      "$n exchanges a quiet hand-signal with a fellow patrol as they pass.",
      "$n adjusts the dual-city insignia on $s shoulder and resumes scanning the forest margin.",
      "$n reads a field notation from the last patrol and marks it filed with a precise initial."
   };
   static char *says[] = {
      "Joint commission patrol means dual authority. I write to Midgaard's rangers and Kowloon's wardens both. One incident, two ledgers.",
      "Ashfang sign in the Ironpine Rise. Prints were a day old. We've notified both city commands. Standard protocol.",
      "The road's safe if you stay on it. The moment you step off to investigate something interesting, the risk calculates differently.",
      "Three patrol pairs cover the road from gate to gate on stagger. You'll see wardens every few hours if the rotation holds.",
      "Forest charter says no army may march this road in war formation. One patrol at a time. That rule has held for two generations.",
      "Ironpine Rise is showing Ashfang activity again. Commission has an open task order if you're at that level of field capability — both cities want threat confirmation.",
      "Rootbound creatures have been pressing toward the forest's eastern edge. The commission needs a perimeter probe from someone who can handle contact. Not for the inexperienced."
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
               if (plr->pcdata->completed_static_quests[18]) { qplr[nq] = plr; qid[nq++] = 18; }
               if (plr->pcdata->completed_static_quests[19]) { qplr[nq] = plr; qid[nq++] = 19; }
               if (plr->pcdata->completed_static_quests[26]) { qplr[nq] = plr; qid[nq++] = 26; }
            }

            if (nq > 0)
            {
               int pick = number_range(0, nq - 1);
               plr = qplr[pick];
               switch (qid[pick])
               {
                  case 18:
                     act("$n pauses $s route scan and gives $N a scout's brief acknowledgment nod.", ch, NULL, plr, TO_NOTVICT);
                     act("$n pauses $s route scan and gives you a scout's brief acknowledgment nod.", ch, NULL, plr, TO_VICT);
                     do_say(ch, "Ironpine Rise sign has cleared. Commission updated the threat assessment — Ashfang pattern moved back to the deep north. Both city commands filed acknowledgment. Good work in there.");
                     return FALSE;
                  case 19:
                     act("$n stops $s patrol motion and acknowledges $N with the careful attention of someone who's read the after-action notes.", ch, NULL, plr, TO_NOTVICT);
                     act("$n stops $s patrol motion and acknowledges you with the careful attention of someone who's read the after-action notes.", ch, NULL, plr, TO_VICT);
                     do_say(ch, "Rootbound perimeter probe is filed. Commission adjusted the eastern patrol boundary based on your contact data. Blight markers are moving slower than last assessment — your count helps explain why.");
                     return FALSE;
                  case 26:
                     act("$n glances at $N with the practiced recognition of someone comparing a face to a field report.", ch, NULL, plr, TO_NOTVICT);
                     act("$n glances at you with the practiced recognition of someone comparing a face to a field report.", ch, NULL, plr, TO_VICT);
                     do_say(ch, "Mirrorbark census came through both ledgers. Commission updated the predator corridor map for that section — three species confirmed, behavior consistent with your report. Reliable data.");
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
               if (lv >= 5  && lv <= 20) hints[hint_count++] = "Not ready for the forest yet? The Gloamvault northwest of Kiess is where commission scouts calibrate before rotating into the main patrol area. Cult structure, limited footprint — good orientation ground.";
               if (lv >= 10 && lv <= 25) hints[hint_count++] = "The Nightfall Catacombs are off the commission's direct patrol zone, but we cross-file reports — undead vault complex, layered structure. Commission scouts who've run it say it's a solid step up from the Gloamvault.";
               if (lv >= 15 && lv <= 30) hints[hint_count++] = "Sepulcher Pasture sits east of the crossroads — outside the forest commission jurisdiction, but we cross-file threat data. Bone-remnant activity, burial cult. Good field experience if you're in that range.";
               if (lv >= 20 && lv <= 35) hints[hint_count++] = "The Cathedral of the Violet Eclipse is logged in commission records as a high-threat sacred site. Eclipse cult construction, relic guardians. Commission doesn't patrol it, but we track reports from operatives who do.";
               if (lv >= 25 && lv <= 40) hints[hint_count++] = "Umbra Heartspire is at the edge of the commission's eastern cross-file range — shadow alignment, void constructs. Commission assessment marks it as upper-tier field work. Check your capabilities before approaching.";
               if (hint_count > 0)
               {
                  do_say(ch, (char *)hints[number_range(0, hint_count - 1)]);
                  return FALSE;
               }
            }
            break;
         }
      }

      /* Normal random dialogue */
      if (number_bits(1) == 0)
         act(acts[number_range(0, 6)], ch, NULL, NULL, TO_ROOM);
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

   /* Slow patrol: move on average once every 16 seconds (1/8 ticks at 2s each) */
   if (number_bits(3) != 0)
      return TRUE;

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
