#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_kowloon_gate_captain(CHAR_DATA *ch)
{
   static const char *acts[] = {
      "$n reviews a manifest against the Syndic's inspection ledger, marking entries with a precise notation.",
      "$n steps to the threshold and scans the approaches beyond the gate with hard, practiced eyes.",
      "$n signals a subordinate guard with a brief hand gesture, directing attention to a passing figure.",
      "$n checks a courier's travel permit against the day's registered messengers, then waves them through.",
      "$n consults the rotation schedule posted on the gate wall, confirming the incoming shift timing."
   };
   static char *says[] = {
      "Seasonal rotation isn't tradition — it's policy. No one builds loyalty at a chokepoint. The Covenant was specific.",
      "Every departure logged, every arrival registered. If you're leaving through this gate, your name goes in the ledger.",
      "Corsair probes have been testing the eastern approaches. Tide Gate is watching for pattern timing. We share intelligence between posts.",
      "The gate's character changes with who's posting, but the protocol doesn't. That's the point of a protocol.",
      "Carry your documents visible from the checkpoint. I don't have time to search your pack, and neither do you."
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
