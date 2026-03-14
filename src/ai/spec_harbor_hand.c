#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_harbor_hand(CHAR_DATA *ch)
{
   static const char *acts[] = {
      "$n rolls $s shoulders after setting down a heavy crate, knuckling the small of $s back.",
      "$n checks the painted load mark on a cargo bundle, comparing it against a ticket stub.",
      "$n coils a length of docking rope with practiced loops, securing it to a bollard.",
      "$n watches the river current for a moment before returning to the crates stacked along the wharf.",
      "$n lifts a corner of a canvas tarpaulin to check the cargo beneath, then drops it back."
   };
   static const char *says[] = {
      "Current's been running fast all week. Difficult barge work when the river is high.",
      "Levee manifest said three crates of cedar planks. I count two. Dock master's going to love that.",
      "River traffic is backed up from the southern bends. Something on the road approach, the boatmen say.",
      "The wharf team works bell to bell with no slack. Nobody complains if the boats keep moving.",
      "Cargo doesn't move itself. That's what they forget when they're writing the tariff schedules."
   };

   if (!IS_AWAKE(ch) || is_fighting(ch))
      return FALSE;

   if (number_bits(3) != 0)
      return FALSE;

   if (number_bits(1) == 0)
      act(acts[number_range(0, 4)], ch, NULL, NULL, TO_ROOM);
   else
      do_say(ch, (char *)says[number_range(0, 4)]);

   return FALSE;
}
