#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_ss_manifest_warden(CHAR_DATA *ch)
{
   static const char *acts[] = {
      "$n checks a manifest slate against a stack of witness sticks, confirming cohort marks one by one.",
      "$n traces an old Scorch Clause formula with a stylus, then stamps the margin with a gate sigil.",
      "$n holds a ceramic seal to the light, searching for reheated tampering around the rim.",
      "$n adjusts the striated pauldron at $s shoulder and signals the next caravan file forward.",
      "$n records a ration variance and circles it twice before returning the slate."
   };
   static char *says[] = {
      "Manifest mismatch means queue delay. Queue delay means heat exposure. Keep your papers straight.",
      "Westbound returns without unauthorized additions move quickly. Everything else gets opened.",
      "Your witness cohort mark and your ration ledger must agree. No exceptions at this gate.",
      "Scorch Clause segment liability transfers here. Read your own seal text before arguing with me.",
      "Mafdet relay clerks reject vague paperwork. I prefer to fix errors before they do."
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
