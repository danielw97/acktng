#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_reach_smuggler(CHAR_DATA *ch)
{
   static const char *acts[] = {
      "$n peers through a glass-shard screen and listens for patrol boots on stone.",
      "$n wraps a small relic component in dark liturgical cloth.",
      "$n traces an eclipsed glyph in the sand and scuffs it away with a heel.",
      "$n checks a dead-drop niche behind broken masonry, then reseals it.",
      "$n counts muffled footsteps before signaling a hidden passage with two taps."
   };
   static char *says[] = {
      "The fire that made this glass was judgment, not accident. We keep what it sanctified.",
      "Warden patrols watch the roads. We move beneath them.",
      "Every sealed relic has a buyer, if you know which whisper to answer.",
      "The Ninth Meridian failed for them. For us, it opened the way.",
      "Quiet hands, quiet tongues, quick feet. That's how the Cell survives."
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
