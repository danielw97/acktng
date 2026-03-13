#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_rr_ruin_scavenger(CHAR_DATA *ch)
{
   static const char *acts[] = {
      "$n taps a fragment of carved stone with $s small hammer, listening to the ring.",
      "$n makes a charcoal rubbing of an inscription, pressing the cloth carefully against the stone.",
      "$n sifts loose rubble through gloved fingers, setting aside pieces that show worked edges.",
      "$n compares a rubbing against a reference sketch in $s field notebook, nodding at a match.",
      "$n sweeps debris from a section of ancient flagstone, revealing the worn relief beneath."
   };
   static const char *says[] = {
      "The Granite Arcade in Midgaard pays for authenticated rubbings. Authenticated means I witnessed the inscription in place.",
      "This road bed has three layers underneath. The deepest layer predates everything with a name. I've found script nobody can read.",
      "Don't disturb the marked cairns further in. The ruin custodians react to that. I learned by watching someone else learn.",
      "The best fragments are the ones that were buried, not exposed. Burial preserves the script. Counterintuitive but true.",
      "I'm not a thief. Every piece I recover goes to the Midgaard archive with full provenance documentation."
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
