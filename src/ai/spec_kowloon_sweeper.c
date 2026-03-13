#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_kowloon_sweeper(CHAR_DATA *ch)
{
   static const char *acts[] = {
      "$n pushes debris toward a storm-drain grate with steady strokes of a bamboo broom.",
      "$n pauses to empty a small collection bin into the waste cart at the corner.",
      "$n marks a chalk tally on the district maintenance board, noting the sector complete.",
      "$n sweeps around the base of a color-marked post, working methodically toward the curb.",
      "$n clears accumulated silt from a drain grate with the broom's edge, checking the flow."
   };
   static char *says[] = {
      "Dawn sweep runs from first bell to Bronze Watch. Sector's mine from the courier crossing south to the flood terrace.",
      "The drainage has to stay clear. Flood season, one blocked grate backs up a whole district quadrant. Ask anyone who was here for the third monsoon.",
      "Six districts, twelve sweepers, one rotation schedule. The Magistracy posts it at the registry each month.",
      "People think this is just cleaning. It's flood control. Every grate I clear is a grate that drains when the rain comes.",
      "Bell Watch marks the shift change. I'm here before the market opens, gone before the crowds arrive. That's how it works."
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
