#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_gnf_road_warden(CHAR_DATA *ch)
{
   static const char *acts[] = {
      "$n scans the treeline with practiced eyes, one hand resting on $s blade hilt.",
      "$n checks the condition of a nearby waystone, testing the iron lantern bracket for rust.",
      "$n marks something in a field ledger, then tucks it back into $s patrol coat.",
      "$n adjusts the joint-commission sash at $s waist, straightening the dual-city insignia.",
      "$n pauses at a road drainage ditch and kicks a clump of leaf debris clear with a boot.",
      "$n taps a waystone twice with a knuckle — two taps for 'passage clear', the old road signal."
   };
   static char *says[] = {
      "Road's open. Commission checked it this morning. Travel north with your documents visible.",
      "Split-pine badge means joint authority. Midgaard law south, Kowloon compact north, forest charter in between.",
      "If you see ash-painted faces off the road, don't engage. Signal any warden and move clear.",
      "Every warden on this road answers to the joint commission. We report to both cities equally. That's the charter.",
      "Lantern posts need oil at the next bend. If you pass before dark, the oil cache is at the base of the stone."
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
