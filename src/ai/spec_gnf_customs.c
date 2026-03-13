#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_gnf_customs(CHAR_DATA *ch)
{
   static const char *acts[] = {
      "$n checks a caravan manifest against $s customs registry, verifying the dual-marks.",
      "$n examines a cargo seal for the correct thread-knot pattern on the Kowloon side.",
      "$n calls out to an approaching traveler, gesturing toward the inspection lane.",
      "$n updates the chalkboard behind $s post, erasing old tariff rates and chalking new ones.",
      "$n stamps a document with the northern customs seal, adding the day's date in precise script."
   };
   static const char *says[] = {
      "All northbound manifests require dual marks: Midgaard wax and Kowloon thread-knot. That's the fifth article of the forest charter.",
      "Inspection wait time is posted on the board. Sealed diplomatic pouches go straight through. Livestock takes longest.",
      "Smuggling through the fen channels bypasses my post — not the Kowloon magistracy. Their reach is longer than mine.",
      "Current tariff on processed timber: four measures per cart. Raw timber: two measures. Treaty-fixed. I don't negotiate.",
      "The road has been open two hundred years. This customs post has been here for one hundred and sixty. We know what shouldn't be crossing."
   };

   if (!IS_AWAKE(ch) || is_fighting(ch))
      return FALSE;

   if (number_bits(3) != 0)
      return FALSE;

   if (number_bits(1) == 0)
      act(acts[number_range(0, 4)], ch, NULL, NULL, TO_ROOM);
   else
      do_say(ch, says[number_range(0, 4)]);

   return FALSE;
}
