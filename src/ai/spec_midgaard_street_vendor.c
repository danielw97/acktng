#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_midgaard_street_vendor(CHAR_DATA *ch)
{
   static const char *calls[] = {
      "$n calls out, 'Eastern caravan goods — landed at the square this morning, tariff-cleared!'",
      "$n holds up a carved token: 'Reliquary-registered goods, every item — ask for the Registry mark!'",
      "$n gestures at $s table: 'Fair weight, Ledgerhouse-certified scales — check the stamp yourself!'",
      "$n waves at a passing pedestrian: 'Northern route spice, fresh batch — don't wait for the guild shops to mark it up!'",
      "$n calls toward the corner: 'Armorer quarter surplus — same source, half the shop price!'"
   };
   static char *says[] = {
      "The tariff board raised the eastern-goods levy again at the last Concordat session. My prices reflect it. Not my choice.",
      "Independent vendors in this district operate under the street-pitch permit. Mine is current and posted on that board.",
      "The guild shops get first rights on Roc Road shipments. We get what's left. Still good goods, just later in the cycle.",
      "Northern caravan was three days late — Gloamvault approach disruption, the Watch said. Prices are up until the route clears.",
      "Come back at Ash Watch — I discount anything still on the table. No point carrying unsold stock home across town."
   };

   if (!IS_AWAKE(ch) || is_fighting(ch))
      return FALSE;

   if (number_bits(3) != 0)
      return FALSE;

   if (number_bits(1) == 0)
      act(calls[number_range(0, 4)], ch, NULL, NULL, TO_ROOM);
   else
      do_say(ch, says[number_range(0, 4)]);

   return FALSE;
}
