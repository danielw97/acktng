#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_reach_scavenger(CHAR_DATA *ch)
{
   static const char *acts[] = {
      "$n chips mortar from a cairn seam with a narrow chisel.",
      "$n pries a bronze fitting loose and wraps it in oiled cloth.",
      "$n lifts a tablet shard to lamplight, squinting for legible script.",
      "$n glances over $s shoulder for patrol banners before returning to work.",
      "$n tests a seal-stone edge and brushes grit away with surprising care."
   };
   static char *says[] = {
      "Bronze fittings fetch six measures at the Strand. A legible tablet? Ten times that.",
      "Salvage law says abandoned structures are fair claim. The Wardens only argue the word 'abandoned'.",
      "Careful with that seal-stone. Cracked, it's gravel. Intact, a scholar pays a season's wage.",
      "History's only holy until someone posts a price. Then it's inventory.",
      "We strip what's already falling. The wind ruins faster than we do."
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
