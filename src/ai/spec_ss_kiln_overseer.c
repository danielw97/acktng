#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_ss_kiln_overseer(CHAR_DATA *ch)
{
   static const char *acts[] = {
      "$n opens a vent shutter a finger's width, listens to the draft, and closes it again.",
      "$n rotates a heat gauge ring and notes the new value on a cracked assay tile.",
      "$n knocks slag from a valve housing, exposing old calibration marks underneath.",
      "$n inspects a row of firing tags and discards one with a sharp click of disapproval.",
      "$n sweeps ash from the kiln lip and redraws a warning line in bright chalk."
   };
   static char *says[] = {
      "Seven-color assay was a safety protocol before it became a tax instrument.",
      "If the vent sings high, step back. If it goes silent, run.",
      "Re-firing a bad seal does not make a good seal. It makes a hotter argument.",
      "Kiln gas ignores rank, doctrine, and courage in exactly that order.",
      "The Crucible obeys pressure, not prayer. Keep the valves balanced or bury another crew."
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
