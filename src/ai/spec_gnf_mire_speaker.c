#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_gnf_mire_speaker(CHAR_DATA *ch)
{
   static const char *acts[] = {
      "$n tests the tension on $s reed fish-trap with a careful pull, nodding at the result.",
      "$n bundles a handful of fen herbs with a length of reed-cord, hanging them to dry.",
      "$n crouches to read the dark water's surface, noting the color and current beneath.",
      "$n watches a distant heron with concentrated attention, reading something in its slow movement.",
      "$n splices two sections of woven reed cord together with quick, sure movements."
   };
   static const char *says[] = {
      "The black channels here run east-west. Follow a heron going south and you'll find a ford. That's old fen-reading.",
      "Quickmud shows white at the edge before it swallows. Step only where the reeds grow double-thick.",
      "Fever-bark grows on the north side of the hummocks. Two inches of bark soaked overnight. Mire-Speakers have used it for three generations.",
      "We were here before the road. We'll be here after. The Mire-Speakers gave permission for the bridge. That permission can be withdrawn.",
      "Passage tithe in the Bends isn't coin. It's salt, iron, lamp oil, or honest labor. We remember every deal we've made."
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
