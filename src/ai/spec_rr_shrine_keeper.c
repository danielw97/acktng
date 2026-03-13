#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_rr_shrine_keeper(CHAR_DATA *ch)
{
   static const char *acts[] = {
      "$n fills the shrine's lamp hollow with a careful measure of oil from a small flask.",
      "$n traces the carved blessing text with one finger, lips moving silently.",
      "$n places a spray of fresh wildflowers in the shrine's hollow, replacing the dried ones.",
      "$n sweeps fallen leaves away from the shrine base with a small broom.",
      "$n checks the carved lettering for weathering damage, noting affected characters with a small mark."
   };
   static char *says[] = {
      "The blessing is older than the road. The shrine was here first. The road builders worked around it.",
      "I serve the road, not either city. Midgaard wardens leave coin. Kiess marshals leave rations. I thank both and serve neither.",
      "The text reads: safe passage in, safe passage out, and honest account of what passes between. That's the whole blessing.",
      "Travelers used to leave tokens and written names here. It's less common now. The road feels less sacred than it once did.",
      "Three waystone shrines between here and the hills. I tend all three. The oil cache is at the base of each one."
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
