#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_rr_byways_market(CHAR_DATA *ch)
{
   static const char *acts[] = {
       "$n rearranges $s pitch goods, putting the better stock forward in the display.",
       "$n consults the market price board and makes a small adjustment to $s posted rates.",
       "$n tallies the day's transactions in a small ledger, squinting at the total with mild "
       "satisfaction.",
       "$n calls out current prices in a practiced voice that carries across the market ground.",
       "$n secures a flapping banner strip with a quick knot, restoring the pitch's color-coded "
       "display."};
   static char *says[] = {"Prices here are fair market rate — the guild notary posts the reference "
                          "schedule and we're all bound by it.",
                          "The crossroads market runs twice a week. You want to be here early on "
                          "market mornings — the good stock moves fast.",
                          "I take Kiess trade credit and Midgaard coin both. The notary handles "
                          "the conversion rate. No disputes about it.",
                          "The market supervisor has the lodge at the north end. Any serious "
                          "complaints go there. Minor ones I handle myself.",
                          "Convoy resupply is our main trade this season. The waystation to the "
                          "north takes the heavy freight; we handle portable provisions.",
                          "The overflow ground to the east is where the unusual finds come "
                          "through. Worth a look if you've got the time."};

   CHAR_DATA *plr;

   if (!IS_AWAKE(ch) || is_fighting(ch))
      return FALSE;

   if (number_bits(3) != 0)
      return FALSE;

   switch (number_range(0, 2))
   {
   case 0: /* Quest completion recognition */
   {
      for (plr = ch->in_room->first_person; plr != NULL; plr = plr->next_in_room)
      {
         if (IS_NPC(plr) || plr->pcdata == NULL)
            continue;
         if (plr->pcdata->completed_static_quests[75])
         {
            act("$n catches $N's eye and gives a brief, collegial nod.", ch, NULL, plr, TO_NOTVICT);
            act("$n catches your eye and gives a brief, collegial nod.", ch, NULL, plr, TO_VICT);
            do_say(ch, "Heard the road survey got done properly at last. We've been waiting on "
                       "that. The survey data feeds into the guild notary's route records — better "
                       "routes, better convoy planning, better market turnover. Good work.");
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
         if (lv >= 10 && lv <= 40)
            hints[hint_count++] = "The road authority at Midgaard has been interested in getting "
                                  "the byways properly mapped. The records keeper handles that "
                                  "kind of work if you want to make it official.";
         if (lv >= 20 && lv <= 50)
            hints[hint_count++] = "The Banner Hills north of the barracks have had wolf trouble "
                                  "this season. The Kiess postmaster had a notice about it. Worth "
                                  "looking into if you're heading that direction anyway.";
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
