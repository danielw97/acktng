#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_kiess_wall_officer(CHAR_DATA *ch)
{
   static const char *acts[] = {
      "$n reviews the duty roster on the briefing board, making a notation in the margin.",
      "$n traces a patrol route on the tactical map with one finger, checking times against the duty log.",
      "$n steps to the window and scans the wall walk before returning to the desk.",
      "$n initials the bottom of a stack of patrol reports and files them in the duty log.",
      "$n sends a courier off with a sealed dispatch, watching until they clear the corridor.",
      "$n updates the contingency routing board, marking the Greenveil Spur status with a chalk notation.",
      "$n reviews a field sketch from the last scout rotation, correcting a waypoint label in the margin."
   };
   static char *says[] = {
      "East gate rotation is covered through Dusk Watch. North gate I want two sentries until the mist-line report comes in.",
      "Any scout returning from the northern verge reports to this office directly before the general debrief. Standing order.",
      "Wall Command doesn't guess at threat levels. We assess, we document, we act on evidence. That is the doctrine.",
      "The Withered Depths blight is moving. Slowly — but moving. Scouts returning with blight-edge data get full debrief priority.",
      "Kiess survives because its walls are manned and its gates have discipline. Everything else this city does depends on that foundation.",
      "The Greenveil Spur is our contingency bypass if the main road is compromised. If you have the capability, clearing it would matter to convoy routing.",
      "Gloamvault northwest of the city is where I'd send newer adventurers before they attempt the forest. Cult structure, active hostiles, contained perimeter. Good orientation ground."
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
            if (plr->pcdata->completed_static_quests[9])  { qplr[nq] = plr; qid[nq++] = 9;  }
            if (plr->pcdata->completed_static_quests[10]) { qplr[nq] = plr; qid[nq++] = 10; }
            if (plr->pcdata->completed_static_quests[0])  { qplr[nq] = plr; qid[nq++] = 0;  }
         }

         if (nq > 0)
         {
            int pick = number_range(0, nq - 1);
            plr = qplr[pick];
            switch (qid[pick])
            {
               case 9:
                  act("$n looks up from the duty board and acknowledges $N with a direct nod.", ch, NULL, plr, TO_NOTVICT);
                  act("$n looks up from the duty board and acknowledges you with a direct nod.", ch, NULL, plr, TO_VICT);
                  do_say(ch, "Greenveil Spur is back on the contingency chart. Convoy marshals verified it this morning. The postmaster confirmed your clearance work held. Good outcome.");
                  return FALSE;
               case 10:
                  act("$n straightens slightly as $e notices $N, the posture of someone acknowledging a completed contract.", ch, NULL, plr, TO_NOTVICT);
                  act("$n straightens slightly as $e notices you, the posture of someone acknowledging a completed contract.", ch, NULL, plr, TO_VICT);
                  do_say(ch, "Inlet cave network is clear. Harbor Syndics filed the confirmation last dispatch cycle. The contraband routes haven't re-established since your purge. That's a clean result.");
                  return FALSE;
               case 0:
                  act("$n glances at $N with the attention of someone reviewing a filed report in their head.", ch, NULL, plr, TO_NOTVICT);
                  act("$n glances at you with the attention of someone reviewing a filed report in their head.", ch, NULL, plr, TO_VICT);
                  do_say(ch, "Your forest route data gave dispatch better information than they'd had in two seasons. The catrat pattern update has already filtered into our corridor risk assessments.");
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
            if (lv >= 5  && lv <= 20) hints[hint_count++] = "If you're not yet ready for the northern forest, the Gloamvault is worth your attention first. Cult ruins northwest of the city — active threats, manageable scale. Good assessment ground.";
            if (lv >= 10 && lv <= 25) hints[hint_count++] = "The Nightfall Catacombs are on Wall Command's secondary threat assessment list. Undead-held vault complex — not our jurisdiction, but we cross-file reports. Worth your time if you've outgrown the Gloamvault.";
            if (lv >= 15 && lv <= 30) hints[hint_count++] = "Sepulcher Pasture is outside our direct jurisdiction, but Wall Command tracks it. Bone-remnant activity, burial cult residue — mid-range threat profile. East of the crossroads if you're ready for that work.";
            if (lv >= 20 && lv <= 35) hints[hint_count++] = "The Cathedral of the Violet Eclipse is an eclipse-cult site — Wall Command has filed reports from field operatives who cleared sections of it. High threat, sacred architecture, relic guardians. Worth investigating if you're at that tier.";
            if (lv >= 25 && lv <= 40) hints[hint_count++] = "The Umbra Heartspire is on our long-range threat registry. Shadow-aligned construction, void-bound defenders. Recommend you have solid field experience before approaching it.";
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

   return FALSE;
}
