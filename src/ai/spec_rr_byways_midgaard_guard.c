#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_rr_byways_midgaard_guard(CHAR_DATA *ch)
{
   static const char *acts[] = {
       "$n scans the civic approach road from the gate post, hand resting on $s weapon hilt.",
       "$n checks the duty rotation board at the guardhouse entrance and makes a small mark.",
       "$n exchanges a brief report with a passing patrol, both speaking in the clipped shorthand "
       "of long practice.",
       "$n tests the gate mechanism, lifting and resetting the bar twice to confirm it moves "
       "freely.",
       "$n steps to the corner of the post and eyes the byway junction with professional "
       "attention."};
   static char *says[] = {"Civic North gate is operational. Traffic moves through on the standard "
                          "schedule — announce yourself and keep moving.",
                          "The byways connection has increased our patrol radius. We cover to the "
                          "ford crossing now during peak convoy season.",
                          "Midgaard maintains the civic road surface. The byways authority handles "
                          "everything past the junction marker — different jurisdiction.",
                          "Anyone causing trouble on the approach road gets walked to the civic "
                          "post. That's been the policy since the byways extension opened.",
                          "The records office is north of here if you need route documentation "
                          "stamped. They handle that — we handle the gate.",
                          "Night patrol runs the byway to the first waystation marker. Beyond that "
                          "is warden territory. We coordinate the handoff."};

   CHAR_DATA *plr;

   if (!IS_AWAKE(ch) || is_fighting(ch))
      return FALSE;

   if (number_bits(3) != 0)
      return FALSE;

   switch (number_range(0, 2))
   {
   case 0: /* Quest completion recognition */
   {
      CHAR_DATA *qplr[8];
      int qid[8];
      int nq = 0;

      for (plr = ch->in_room->first_person; plr != NULL; plr = plr->next_in_room)
      {
         if (IS_NPC(plr) || plr->pcdata == NULL)
            continue;
         if (plr->pcdata->completed_quests[75])
         {
            qplr[nq] = plr;
            qid[nq++] = 75;
         }
         if (plr->pcdata->completed_quests[79])
         {
            qplr[nq] = plr;
            qid[nq++] = 79;
         }
      }

      if (nq > 0)
      {
         int pick = number_range(0, nq - 1);
         plr = qplr[pick];
         switch (qid[pick])
         {
         case 75:
            act("$n gives $N a nod of recognition, a small departure from the usual neutral gate "
                "posture.",
                ch, NULL, plr, TO_NOTVICT);
            act("$n gives you a nod of recognition, a small departure from the usual neutral gate "
                "posture.",
                ch, NULL, plr, TO_VICT);
            do_say(ch,
                   "The records office logged the byways survey completion. We were briefed — the "
                   "updated route data feeds into our patrol planning. Proper documentation of the "
                   "roads is something Midgaard takes seriously. Appreciated.");
            return FALSE;
         case 79:
            act("$n acknowledges $N with a brief, direct look — the kind reserved for people "
                "who've done useful work.",
                ch, NULL, plr, TO_NOTVICT);
            act("$n acknowledges you with a brief, direct look — the kind reserved for people "
                "who've done useful work.",
                ch, NULL, plr, TO_VICT);
            do_say(ch, "The cave approach has been flagged as cleared in the patrol log. The "
                       "postmaster's office forwarded the notice. One less hazard on the eastern "
                       "approach is good for everyone running that road.");
            return FALSE;
         }
      }
      break;
   }
   case 1: /* Area hint */
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
         const char *hints[4];
         int hint_count = 0;
         int lv = hint_plr->level;
         if (lv >= 10 && lv <= 50)
            hints[hint_count++] = "The records keeper north of here handles route documentation "
                                  "requests. If you've done byways survey work the Midgaard "
                                  "authority recognizes, that's the desk to visit.";
         if (lv >= 20 && lv <= 55)
            hints[hint_count++] =
                "The eastern approach past the junction has a cave entrance. Our patrol log has "
                "had spider bite incidents from travelers who investigated it. The postmaster's "
                "office has been watching for someone to address that.";
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
