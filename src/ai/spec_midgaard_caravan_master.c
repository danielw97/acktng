#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_midgaard_caravan_master(CHAR_DATA *ch)
{
   static const char *acts[] = {"$n surveys the wagon arrangement with a practised eye, adjusting "
                                "the staging order with a raised hand.",
                                "$n checks a cargo manifest against the route schedule, tracing "
                                "the columns with a gloved finger.",
                                "$n calls out spacing instructions to a driver, measuring the "
                                "required gap with both arms extended.",
                                "$n marks a departure time in the route ledger and compares it to "
                                "the estimated arrival window.",
                                "$n examines a route brand stamped on a pack flap, checking it "
                                "against the Trade Syndic registry list.",
                                "$n speaks into a brass speaking-cylinder, relaying staging "
                                "clearance to the far end of the square."};
   static char *says[] = {
       "The Caravan Square runs on the tide clock, not the sun. If you don't know the difference, "
       "don't bid for a driver's position.",
       "Nothing stages here without a verified cargo seal. The Trade Syndics set that rule during "
       "the Guild Concordat revision and I enforce it.",
       "Midgaard's charter authority covers everything within the gate radius. Past the boundary "
       "stone, convoy marshal clearance is the operative document.",
       "The route brands on those packs each represent a cleared trade line — eastern desert, Roc "
       "Road, southern passage. Every one documented at the Ledgerhouse.",
       "If you're looking for passage with a caravan, the booking desk is at the Ledgerhouse. I "
       "manage movement and staging. I don't manage bookings.",
       "A wagon that hasn't been weighed and manifested doesn't leave this square. That's not my "
       "rule — that's the Trade Syndic compact, and it predates both of us."};

   if (!IS_AWAKE(ch) || is_fighting(ch))
      return FALSE;

   if (number_bits(3) != 0)
      return FALSE;

   if (number_bits(1) == 0)
      act(acts[number_range(0, 5)], ch, NULL, NULL, TO_ROOM);
   else
      do_say(ch, says[number_range(0, 5)]);

   return FALSE;
}
