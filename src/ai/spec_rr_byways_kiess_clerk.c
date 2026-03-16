#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_rr_byways_kiess_clerk(CHAR_DATA *ch)
{
   static const char *acts[] = {
       "$n affixes a customs seal to a processed manifest and sets it in the completed stack "
       "without looking up.",
       "$n checks a declared cargo weight against the balance scale reading, noting the "
       "discrepancy with a neutral expression.",
       "$n consults the posted tariff schedule and makes a calculation on $s counting board.",
       "$n adjusts the queue barrier to manage the incoming convoy traffic more efficiently.",
       "$n opens a fresh ledger page and rules the columns with a straight edge before the next "
       "entry."};
   static char *says[] = {"Kiess customs operates on the full manifest system. Every item "
                          "declared, every item counted. The schedule takes as long as it takes.",
                          "The outer gate pass is good for one entry and one exit. If you need "
                          "extended access, that's a separate application.",
                          "The charter stone on the west path records the original Kiess founding "
                          "mandate. The customs system comes directly from that document.",
                          "Evermeet fell. Kiess was built on the foundation. The customs system is "
                          "part of what makes sure Kiess doesn't make the same mistakes.",
                          "The guild notary annex handles contract disputes. We handle tariffs. "
                          "Different desks, different ledgers, same building.",
                          "Route documentation from the road authority is accepted here in lieu of "
                          "a full manifest for convoys under charter weight limits."};

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
         if (plr->pcdata->completed_static_quests[75])
         {
            qplr[nq] = plr;
            qid[nq++] = 75;
         }
         if (plr->pcdata->completed_static_quests[77])
         {
            qplr[nq] = plr;
            qid[nq++] = 77;
         }
      }

      if (nq > 0)
      {
         int pick = number_range(0, nq - 1);
         plr = qplr[pick];
         switch (qid[pick])
         {
         case 75:
            act("$n pauses $s customs work and makes a notation in the pass ledger beside $N's "
                "name.",
                ch, NULL, plr, TO_NOTVICT);
            act("$n pauses $s customs work and makes a notation in the pass ledger beside your "
                "name.",
                ch, NULL, plr, TO_VICT);
            do_say(ch,
                   "The byways survey data has been forwarded to the route archive. I can see it "
                   "in the updated manifest references. The customs office operates on accurate "
                   "route data — this kind of contribution has direct practical value.");
            return FALSE;
         case 77:
            act("$n sets down $s pen and addresses $N directly, which is unusual behavior for the "
                "customs desk.",
                ch, NULL, plr, TO_NOTVICT);
            act("$n sets down $s pen and addresses you directly, which is unusual behavior for the "
                "customs desk.",
                ch, NULL, plr, TO_VICT);
            do_say(ch, "The underfoundation clearance was noted in the guild registrar's "
                       "correspondence. The Evermeet site is of significant historical interest to "
                       "Kiess. Keeping it accessible for the antiquarians is a civic priority, "
                       "even if customs doesn't say so publicly.");
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
            hints[hint_count++] =
                "The Midgaard records keeper has an interest in proper documentation of the road "
                "byways. If you have time for thorough work, that's a recognized contribution.";
         if (lv >= 45 && lv <= 90)
            hints[hint_count++] =
                "The Evermeet underfoundation, accessible from this gate road, has active remnant "
                "presences. The guild registrar at Kiess has noted the situation. The site would "
                "benefit from having those presences removed.";
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
