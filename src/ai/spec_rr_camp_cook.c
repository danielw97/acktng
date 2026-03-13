#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_rr_camp_cook(CHAR_DATA *ch)
{
   static const char *acts[] = {
      "$n stirs the cookpot with a long ladle, adding a pinch of salt from the tin.",
      "$n sets a row of clay bowls ready beside the fire, wiping each one clean with a rag.",
      "$n fans the cookfire with a square of oiled cloth, building the heat evenly.",
      "$n tastes a spoonful of the stew, makes a face, and reaches for the salt tin again.",
      "$n breaks a dry root vegetable in half and drops both pieces into the bubbling pot."
   };
   static const char *says[] = {
      "Stew's ready when the wardens come off rotation. Enough for the convoy crew too if they brought bowls.",
      "Root vegetables from the farmland stretch, dried meat from the last Kiess resupply. Hot and it won't give you the gripes.",
      "I've been cooking for road crews since before the last road tax revision. Long time to learn what keeps people moving.",
      "Convoy marshals pay in Kiess credit. Wardens pay in Midgaard coin. I balance both columns and it comes out square most months.",
      "The fire's banked low at night. No need to announce yourself to every ridge wolf on the Banner Hills."
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
