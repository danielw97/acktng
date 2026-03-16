#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_midgaard_pilgrim(CHAR_DATA *ch)
{
   static const char *acts[] = {
      "$n kneels at the base of the altar, hands folded, eyes closed in long silence.",
      "$n reaches out and traces the carved text of the First Stone Accord with careful fingers.",
      "$n reads from a worn devotional pamphlet, lips moving soundlessly in the brazier light.",
      "$n sets a small oil lamp at the altar base and watches the flame steady before stepping back.",
      "$n stands in the ward-sigil glow with eyes downcast, motionless for a full minute before breathing out."
   };
   static char *says[] = {
      "I walked eighteen days to reach this. It is exactly as the texts described and nothing like I expected.",
      "The ward sigils in the floor resonate differently when you stand inside them. The books don't prepare you for that.",
      "The First Stone Accord is carved on the altar face. It is shorter than I thought it would be. Most true things are.",
      "The temple keepers let you stand here as long as you need. I have been here since the morning bell.",
      "The journey matters. But standing here, I understand why the destination matters too."
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
