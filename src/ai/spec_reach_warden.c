#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_reach_warden(CHAR_DATA *ch)
{
   static const char *acts[] = {
      "$n shades $s eyes and studies the horizon beyond the nearest cairn line.",
      "$n taps a cairn marker with a bronze staff, listening for fractures in the stone.",
      "$n updates a patrol ledger with careful strokes, then seals it shut.",
      "$n adjusts $s sun-hood and scans each traveler with practiced suspicion.",
      "$n checks a weathered route tablet and traces the old Keeper survey marks."
   };
   static char *says[] = {
      "Transit seal current? Good. The Reach still has law, whatever the Harbor Wardens claim.",
      "Every cairn between here and the Seal carries our mark. That is not tradition -- it is jurisdiction.",
      "Measure's Rest keeps tablets older than Mafdet's harbor chain. We were here first. We remain.",
      "Three routes, one authority. We keep the line so cargo reaches shore alive.",
      "A missing marker today is a dead caravan tomorrow. Patrol work is never ceremonial."
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
