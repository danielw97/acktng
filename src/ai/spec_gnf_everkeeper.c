#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_gnf_everkeeper(CHAR_DATA *ch)
{
   static const char *acts[] = {
      "$n kneels at the edge of a planting ring, pressing earth around a young ironpine seedling.",
      "$n runs $s hand along the bark of a standing ironpine, eyes closed as if listening.",
      "$n marks a seven-circle pattern in the earth with a carved stick, murmuring in an old tongue.",
      "$n examines a section of ironpine bark, noting the ring patterns with the attention of a scholar.",
      "$n sets seven small stones in a careful circle at the base of a great ironpine."
   };
   static char *says[] = {
      "Seven seedlings for every ironpine felled. That is the law of the grove. The Everkeepers made it the law of the sky.",
      "These trunks are the pillars. Without them the sky has no support. Laugh if you wish. The ironpine was here before your cities.",
      "The Rootbound corruption is spreading up the root-network. I've charted its progress for seven seasons. It moves toward the old groves.",
      "Ilren Voss understood. Her road bends around every Everkeeper grove still standing. She asked us before she built. Remember that.",
      "A dead ironpine grove is an open sky. The sky falls slowly, but it falls."
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
