#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_midgaard_gate_warden(CHAR_DATA *ch)
{
   static const char *acts[] = {
      "$n runs $s finger down the arrival log, checking each entry against the watch-stamp.",
      "$n steps to the gate arch and eyes the northern road, one hand resting on the portcullis chain.",
      "$n exchanges a brief word with the departing patrol, noting the rotation count in $s duty ledger.",
      "$n tests the gate bar mechanism, lifting and resetting it twice to confirm it moves freely.",
      "$n lights the duty brazier from a long-handled taper, the flame catching on the second strike.",
      "$n scans a travel document presented by an unseen arrival, holds it to the light, then marks it stamped."
   };
   static char *says[] = {
      "The Great Northern Road is torchlit to the first league. Past that, city maintenance doesn't follow — you carry your own light.",
      "Arrivals log on the left lectern, departures on the right. Every passage noted. That is the Northern Gate standing order.",
      "If you're heading north, announce yourself at the post before you cross the arch. The logbook needs a name and a destination.",
      "Gate hours are dawn to dusk standard. Night passage requires warden authorisation and a registered reason.",
      "Road conditions past the forest fringe come in on the courier relay. Current dispatch is at the Ledgerhouse — I only have what's posted to this gate.",
      "The portcullis stays chained open during day hours. If you hear the closure bell, clear the arch immediately. That order is not debatable."
   };

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
