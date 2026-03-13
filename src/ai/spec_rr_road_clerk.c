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
      "$n affixes a notation slip to the tally-board with a copper tack, adding to layers of records.",
      "$n cross-references a departing traveler's name against the registry of cleared individuals.",
      "$n opens the incident log and adds a brief entry, initialing the date column without looking up."
   };
   static char *says[] = {
      "Road funding is calculated from documented traffic counts. Every unmarked wagon is coin the charter never sees.",
      "The Ledger Watch requires three verification marks per day. I've already filed two. The third goes out at sundown.",
      "Midgaard's Magistrate Ledgerhouse receives monthly summaries. My daily tallies form the base data for road repair appropriations.",
      "A blank entry on the census roll is a gap in the civic record. I don't leave gaps.",
      "Yes, I count every traveler. No, you are not exempt. The hash-mark takes two seconds. Stand still.",
      "Gateworks vermin have been disrupting traffic counts near the entry. Cleared sections show normal flow. Uncleared sections are a census problem.",
      "Gloamvault travelers typically log departure from the Kiess checkpoint. If you're heading that way, make sure your entry is current."
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

      /* Quest 15 (id 14): Gateworks vermin purge */
      if (plr->pcdata->completed_static_quests[14])
      {
         act("$n looks up from the tally-board and marks $N's entry with a second notation.", ch, NULL, plr, TO_NOTVICT);
         act("$n looks up from the tally-board and marks your entry with a second notation.", ch, NULL, plr, TO_VICT);
         do_say(ch, "Gateworks vermin count is down. Traffic has resumed at standard rate through that section. My census numbers normalized within two days of the purge. Good work — the data shows it.");
         return FALSE;
      }

      /* Quest 16 (id 15): Lantern Road wolf cull */
      if (plr->pcdata->completed_static_quests[15])
      {
         act("$n pauses $s tally work and acknowledges $N with a precise nod.", ch, NULL, plr, TO_NOTVICT);
         act("$n pauses $s tally work and acknowledges you with a precise nod.", ch, NULL, plr, TO_VICT);
         do_say(ch, "Lantern Road wolf toll is down. I can confirm it in the courier relay times — delivery delays on that section dropped by nearly a third since the cull. The census reflects improved throughput.");
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
         do_say(ch, "New to the road? Travelers who log Gloamvault as a prior destination tend to show up on the census in better shape than those who don't. Northwest of Kiess — old cult ruin, good first field posting.");
         return FALSE;
      }

      /* Sepulcher Pasture hint for mid-level players */
      if (plr->level >= 15 && plr->level <= 30
         && !plr->pcdata->completed_static_quests[68]) /* Quest 69: Sepulcher Pasture cartography */
      {
         do_say(ch, "Sepulcher Pasture sees regular traveler traffic in your range. East of the crossroads, past the banner hills. Make sure your census entry is current before you head out — I note the returns.");
         return FALSE;
      }
   }

   /* Normal random dialogue */
   if (number_bits(1) == 0)
      act(acts[number_range(0, 7)], ch, NULL, NULL, TO_ROOM);
   else
      do_say(ch, says[number_range(0, 6)]);

   return FALSE;
}
