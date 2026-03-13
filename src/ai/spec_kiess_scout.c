#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_kiess_scout(CHAR_DATA *ch)
{
   static const char *acts[] = {
      "$n marks notations on a field map with practiced efficiency, cross-referencing route cards.",
      "$n checks the rope markers on $s belt, counting the knots against $s field notes.",
      "$n scans the room with the alert wariness of someone who hasn't fully decompressed from the forest.",
      "$n consults a compass bearing against $s posted route notes, making a small correction.",
      "$n oils the edge of $s blade with a cloth, eyes distant, working from ingrained habit."
   };
   static char *says[] = {
      "The mist line's moved another three hundred paces north since last week. That's fast for this season.",
      "Bell-post seven is down. Wall Command's rerouting the southern approach until it's back up.",
      "Saw centaur sign at the forest edge. Prints were fresh — probably overnight. Command's been told.",
      "The confusion doesn't hit immediately. You get maybe two hundred paces before the mist starts working. Stay on the rope lines.",
      "Bring samples from the mist-boundary if you go in. The healers want the spore data. Sealed container, waxed shut."
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
