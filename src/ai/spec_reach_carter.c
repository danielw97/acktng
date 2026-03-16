#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_reach_carter(CHAR_DATA *ch)
{
   static const char *acts[] = {
       "$n checks a sled runner for stress lines and tightens the lashings.",
       "$n retarps a cargo stack against salt spray rolling in from the coast.",
       "$n splits manifests into inland and maritime bundles, then ties each with twine.",
       "$n tests rope tension with a hard pull and nods to approaching teamsters.",
       "$n marks a convoy slate with departure order and weather notes."};
   static char *says[] = {
       "Sled to dock, dock to hold. That's the Carter's road.",
       "Without Guild transfer crews, manifests rot in crates and ships sail half-empty.",
       "Three more convoys this season, then Scour winds close the Flats.",
       "You want priority unload? Pay for priority load first.",
       "Everyone claims the Reach. We move the cargo while they argue."};

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
