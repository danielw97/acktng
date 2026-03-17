#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_midgaard_invasion_warden(CHAR_DATA *ch)
{
   static const char *acts[] = {
      "$n checks the armory manifest against the current allocation list, marking off items without looking up.",
      "$n runs a hand along a row of racked equipment, checking each item is accounted for.",
      "$n reviews the last invasion incident log and makes a notation in the duty record.",
      "$n tests the lock on the restricted storage cabinet and confirms the seal is intact.",
      "$n marks the current patrol cycle on the wall chart and notes the Watch rotation schedule alongside it."
   };
   static char *says[] = {
      "The invasion armory opens to cleared personnel during active incursion alert and to licensed purchasers at all other times.",
      "Everything here is rated for incursion-level combat. If you're not expecting to use it, you're in the wrong post.",
      "The last major incursion pushed through the southern approach. The armory stock was drawn down sixty percent in four days. I keep it current.",
      "Compact doctrine is containment before engagement. But when containment fails, this armory is the response.",
      "Acquisition requires a Watch clearance or a district warden endorsement. The warden post is at the checkpoint arch if you don't have one."
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
