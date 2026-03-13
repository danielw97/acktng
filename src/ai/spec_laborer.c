#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_laborer(CHAR_DATA *ch)
{
   static const char *acts[] = {
      "$n flexes $s hands, cracked and calloused, then readjusts $s grip on a heavy load.",
      "$n pauses to check $s day-token against the stamp mark on $s wrist, then sighs.",
      "$n sets down $s load for a moment, rolling $s shoulders before picking it back up.",
      "$n wipes sweat from $s face with a cloth that is not much cleaner than $s hands.",
      "$n studies the route board on the corner wall, tracing the chalk marks with a finger."
   };
   static char *says[] = {
      "Token came late today. One bell late means shadow-class by the Registry clock. Makes no sense.",
      "Levy goes up, day-rate stays flat. Cinder debt, they call it — compounds whether you pay or not.",
      "Work-broker docked two coppers off my token for 'route variance.' I don't even know what that means.",
      "You can be legal here your whole life and one missing stamp makes you undocumented. One stamp.",
      "The Oath says no levy taken that cannot be borne. I'd like to see them try carrying this load."
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
