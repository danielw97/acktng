#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_reach_assessor(CHAR_DATA *ch)
{
   static const char *acts[] = {
      "$n weighs an imaginary cargo load in both hands, lips moving through conversion figures.",
      "$n stamps an invisible seal in mid-air and nods as if the mark took cleanly.",
      "$n flips through a cracked ledger and taps a line with a dry quill.",
      "$n mutters old legal formulae while aligning phantom manifests in neat stacks.",
      "$n dips a quill into an empty inkwell, frowns, and continues writing anyway."
   };
   static char *says[] = {
      "Water-debt: forty-seven measures, offset against charter series nine. Shore conversion pending at the Seal.",
      "The Tables do not lie. What inland weighs in water, the sea weighs in risk.",
      "Dual attestation requires both marks -- inland witness and maritime seal. One without the other is void.",
      "Swift Line cargo clears first by statute. Heavy Road bulk follows at assessed delay.",
      "Discharge before assumption, assumption before seal. The order is law, not suggestion."
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
