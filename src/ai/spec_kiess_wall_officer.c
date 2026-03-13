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

   /* Quest-completion reactions */
   for (plr = ch->in_room->first_person; plr != NULL; plr = plr->next_in_room)
   {
      if (IS_NPC(plr) || plr->pcdata == NULL)
         continue;

      /* Quest 10 (id 9): Greenveil Spur reconnaissance: spirit clearance — offered by this NPC */
      if (plr->pcdata->completed_static_quests[9])
      {
         act("$n looks up from the duty board and acknowledges $N with a direct nod.", ch, NULL, plr, TO_NOTVICT);
         act("$n looks up from the duty board and acknowledges you with a direct nod.", ch, NULL, plr, TO_VICT);
         do_say(ch, "Greenveil Spur is back on the contingency chart. Convoy marshals verified it this morning. The postmaster confirmed your clearance work held. Good outcome.");
         return FALSE;
      }

      /* Quest 11 (id 10): Inlet smuggler crackdown: cave network purge — offered by this NPC */
      if (plr->pcdata->completed_static_quests[10])
      {
         act("$n straightens slightly as $e notices $N, the posture of someone acknowledging a completed contract.", ch, NULL, plr, TO_NOTVICT);
         act("$n straightens slightly as $e notices you, the posture of someone acknowledging a completed contract.", ch, NULL, plr, TO_VICT);
         do_say(ch, "Inlet cave network is clear. Harbor Syndics filed the confirmation last dispatch cycle. The contraband routes haven't re-established since your purge. That's a clean result.");
         return FALSE;
      }

      /* Quest 1 (id 0): Route reconnaissance: Forest of Confusion */
      if (plr->pcdata->completed_static_quests[0])
      {
         act("$n glances at $N with the attention of someone reviewing a filed report in their head.", ch, NULL, plr, TO_NOTVICT);
         act("$n glances at you with the attention of someone reviewing a filed report in their head.", ch, NULL, plr, TO_VICT);
         do_say(ch, "Your forest route data gave dispatch better information than they'd had in two seasons. The catrat pattern update has already filtered into our corridor risk assessments.");
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
         do_say(ch, "If you're not yet ready for the northern forest, the Gloamvault is worth your attention first. Cult ruins northwest of the city — active threats, manageable scale. Good assessment ground.");
         return FALSE;
      }

      /* Sepulcher Pasture hint for mid-level players */
      if (plr->level >= 15 && plr->level <= 30
         && !plr->pcdata->completed_static_quests[68]) /* Quest 69: Sepulcher Pasture cartography */
      {
         do_say(ch, "Sepulcher Pasture is outside our direct jurisdiction, but Wall Command tracks it. Bone-remnant activity, burial cult residue — mid-range threat profile. East of the crossroads if you're ready for that work.");
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
