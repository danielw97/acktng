#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_rr_road_clerk(CHAR_DATA *ch)
{
   static const char *acts[] = {
       "$n makes a precise tally-mark in $s census ledger, counting the day's wagon traffic.",
       "$n stamps a travel record with the Ledger Watch seal, adding it to the day's stack.",
       "$n checks the relay station dispatch box, comparing its contents against $s daily log.",
       "$n brushes road dust from $s ink-stained robes and returns to the portable writing desk.",
       "$n squints at a wax tablet notation, scratches out an error, and rewrites it cleanly.",
       "$n affixes a notation slip to the tally-board with a copper tack, adding to layers of "
       "records.",
       "$n cross-references a departing traveler's name against the registry of cleared "
       "individuals.",
       "$n opens the incident log and adds a brief entry, initialing the date column without "
       "looking up."};
   static char *says[] = {
       "Road funding is calculated from documented traffic counts. Every unmarked wagon is coin "
       "the charter never sees.",
       "The Ledger Watch requires three verification marks per day. I've already filed two. The "
       "third goes out at sundown.",
       "Midgaard's Magistrate Ledgerhouse receives monthly summaries. My daily tallies form the "
       "base data for road repair appropriations.",
       "A blank entry on the census roll is a gap in the civic record. I don't leave gaps.",
       "Yes, I count every traveler. No, you are not exempt. The hash-mark takes two seconds. "
       "Stand still.",
       "Gateworks vermin have been disrupting traffic counts near the entry. Cleared sections show "
       "normal flow. Uncleared sections are a census problem.",
       "Gloamvault travelers typically log departure from the Kiess checkpoint. If you're heading "
       "that way, make sure your entry is current."};

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
         if (plr->pcdata->completed_quests[14])
         {
            qplr[nq] = plr;
            qid[nq++] = 14;
         }
         if (plr->pcdata->completed_quests[15])
         {
            qplr[nq] = plr;
            qid[nq++] = 15;
         }
      }

      if (nq > 0)
      {
         int pick = number_range(0, nq - 1);
         plr = qplr[pick];
         switch (qid[pick])
         {
         case 14:
            act("$n looks up from the tally-board and marks $N's entry with a second notation.", ch,
                NULL, plr, TO_NOTVICT);
            act("$n looks up from the tally-board and marks your entry with a second notation.", ch,
                NULL, plr, TO_VICT);
            do_say(ch, "Gateworks vermin count is down. Traffic has resumed at standard rate "
                       "through that section. My census numbers normalized within two days of the "
                       "purge. Good work — the data shows it.");
            return FALSE;
         case 15:
            act("$n pauses $s tally work and acknowledges $N with a precise nod.", ch, NULL, plr,
                TO_NOTVICT);
            act("$n pauses $s tally work and acknowledges you with a precise nod.", ch, NULL, plr,
                TO_VICT);
            do_say(ch, "Lantern Road wolf toll is down. I can confirm it in the courier relay "
                       "times — delivery delays on that section dropped by nearly a third since "
                       "the cull. The census reflects improved throughput.");
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
         const char *hints[5];
         int hint_count = 0;
         int lv = hint_plr->level;
         if (lv >= 5 && lv <= 20)
            hints[hint_count++] =
                "New to the road? Travelers who log Gloamvault as a prior destination tend to show "
                "up on the census in better shape than those who don't. Northwest of Kiess — old "
                "cult ruin, good first field posting.";
         if (lv >= 10 && lv <= 25)
            hints[hint_count++] =
                "The Nightfall Catacombs show up in traveler declaration forms — undead "
                "concentration, vault access. Census records show returns from operatives past "
                "their initial field range. Worth noting on your route card.";
         if (lv >= 15 && lv <= 30)
            hints[hint_count++] =
                "Sepulcher Pasture sees regular traveler traffic in your range. East of the "
                "crossroads, past the banner hills. Make sure your census entry is current before "
                "you head out — I note the returns.";
         if (lv >= 20 && lv <= 35)
            hints[hint_count++] =
                "The Cathedral of the Violet Eclipse shows up in the extended departure "
                "declarations — eclipse cult site, relic guardians on approach. Census logs show "
                "returns from operatives in your capability range.";
         if (lv >= 25 && lv <= 40)
            hints[hint_count++] =
                "Umbra Heartspire departures require an extended declaration in the census record "
                "— shadow construct territory, void alignment. The data shows experienced "
                "operatives making that run. Document your departure if you're heading there.";
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
      act(acts[number_range(0, 7)], ch, NULL, NULL, TO_ROOM);
   else
      do_say(ch, says[number_range(0, 6)]);

   return FALSE;
}
