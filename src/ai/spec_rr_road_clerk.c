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
      "$n affixes a notation slip to the tally-board with a copper tack, adding to layers of records."
   };
   static const char *says[] = {
      "Road funding is calculated from documented traffic counts. Every unmarked wagon is coin the charter never sees.",
      "The Ledger Watch requires three verification marks per day. I've already filed two. The third goes out at sundown.",
      "Midgaard's Magistrate Ledgerhouse receives monthly summaries. My daily tallies form the base data for road repair appropriations.",
      "A blank entry on the census roll is a gap in the civic record. I don't leave gaps.",
      "Yes, I count every traveler. No, you are not exempt. The hash-mark takes two seconds. Stand still."
   };

   if (!IS_AWAKE(ch) || is_fighting(ch))
      return FALSE;

   if (number_bits(3) != 0)
      return FALSE;

   if (number_bits(1) == 0)
      act(acts[number_range(0, 5)], ch, NULL, NULL, TO_ROOM);
   else
      do_say(ch, says[number_range(0, 4)]);

   return FALSE;
}
