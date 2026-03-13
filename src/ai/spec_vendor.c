#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_vendor(CHAR_DATA *ch)
{
   static const char *calls[] = {
      "$n calls out, 'Timber carvings off the northern routes — finest grain in the city!'",
      "$n holds up a small jar: 'Eastern spice, fresh off the desert caravan — smell that!'",
      "$n gestures at $s table: 'Day-mark stamped, tariff-cleared — everything here paid its Ledgerhouse due!'",
      "$n calls out, 'Good leather out of the armorer quarter — don't pay shop prices for off-cuts!'",
      "$n waves at a passing pedestrian: 'Fair price, honest weight, Registry-notated goods, citizen!'"
   };
   static char *says[] = {
      "Tariff board raised the western-goods levy again. Prices have to reflect it, nothing personal.",
      "The Guild Concordat sets minimums, but we independent vendors set our own ceiling. Fair is fair.",
      "You want the Roc Road goods, you pay Roc Road prices. Simple arithmetic.",
      "Northern shipment was late — ashfang trouble on the forest roads, the warden said.",
      "Come back at Bell Watch — I knock a tenth off anything left. No point carrying it home."
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
