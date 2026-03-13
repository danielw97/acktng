#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_gnf_courier(CHAR_DATA *ch)
{
   static const char *acts[] = {
      "$n checks the seal on $s dispatch pouch before tucking it firmly under $s arm.",
      "$n marks a waystation signature in $s relay log, confirming arrival at this point.",
      "$n scans the road ahead and behind before setting off again at a measured pace.",
      "$n checks $s bronze CLO badge at $s collar, the standard-bearer of Courier Lantern passage rights.",
      "$n adjusts $s pack to balance the weight of sealed dispatches against the distance remaining."
   };
   static const char *says[] = {
      "CLO relay runs from the Granite Arcade to the Iron Gate. Eight waystation stops. I run the northern half.",
      "Sealed dispatches mean no reading, no delay, no questions. I deliver to the named recipient only.",
      "The relay keeps both cities informed on road conditions, faction activity, and travel safety. Better than waiting for a rider weekly.",
      "Lantern badge gets me through both city gates and the forest checkpoints at any hour. That's the compact's guarantee.",
      "Urgent dispatch, I run it in one stage. No overnight. I've done the full road in fourteen hours when needed."
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
