#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_kowloon_laborer(CHAR_DATA *ch)
{
   static const char *acts[] = {
      "$n shifts a heavy load from one shoulder to the other, finding a better balance point.",
      "$n pauses to check the day-token stamp on $s wrist against the dock's posting board.",
      "$n wipes sweat from $s face with a cloth tucked in $s belt, then picks the work back up.",
      "$n drags a rope coil to a new position at the dock edge, moving with the weight-smart efficiency of long practice.",
      "$n tests a tie-down knot with a hard pull before stepping back, satisfied with the result."
   };
   static const char *says[] = {
      "Day-token says Coppersalt-route work today. That means the heavy haul from the Tide Gate wharf. Long shift.",
      "Syndic pay scale is set by cargo weight, not hours. You move fast, you earn. You move slow, you starve.",
      "Flood season they pay a hazard rate. You'd think that'd be worth celebrating, but it means the drainage channels are backing up.",
      "Pump rotation is every two bells. I'm on third circuit — I don't sit down until Bell Watch, and even then maybe not.",
      "Harbor work feeds the city. The cargo comes in, the city eats. Simple as that. They just don't pay us like it matters."
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
