#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_rr_ferryman(CHAR_DATA *ch)
{
   static const char *acts[] = {
      "$n tests the punt's mooring rope, checking the iron ring on the bank for rust.",
      "$n runs a hand along the punt's flat bottom, checking for splits in the planking.",
      "$n scans the river current upstream, reading the water's color and speed with a practiced eye.",
      "$n adjusts the fare board on its post, straightening the board against the wind.",
      "$n poles the punt a few feet out and back, checking the balance under $s own weight."
   };
   static const char *says[] = {
      "Current's running fast today. Add ten minutes to the crossing. I'll pole from the upstream side.",
      "Fare's posted in both Midgaard coin and Kiess trade credit. I take either. I don't take barter — too much argument.",
      "Downstream there's a spot where the bank undercuts. Don't walk the verge south of the ford at night. The ground gives.",
      "The contraband runners use the river shallows half a mile south. The wardens know and don't have enough people.",
      "I've crossed this river thousands of times. The ford stones haven't shifted in forty years. The crossing is sound."
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
