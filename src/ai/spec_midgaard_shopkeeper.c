#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_midgaard_shopkeeper(CHAR_DATA *ch)
{
   static const char *acts[] = {
       "$n straightens the display on the counter and checks that each item faces forward.",
       "$n reviews $s ledger entry for the morning, tracing the column of incoming stock with a "
       "finger.",
       "$n polishes a piece of displayed equipment with a cloth, checking the finish before "
       "setting it back.",
       "$n adjusts the lamp angle to better illuminate the counter stock.",
       "$n marks a small price tag and attaches it to a newly received item."};
   static char *says[] = {"The Guild Concordat sets minimum pricing for registered shop-row "
                          "vendors. My prices meet that floor — no lower.",
                          "Everything in this shop is sourced through the armorer quarter network "
                          "or the Roc Road trade line. Registry-documented.",
                          "The shop row operates under the district warden's jurisdiction. Any "
                          "dispute goes to the warden post at the checkpoint arch.",
                          "Midgaard coin or equivalent trade credit accepted. The Ledgerhouse "
                          "exchange rate is posted at the outer arch if you need it.",
                          "If I don't have it, the Quartermaster's post around the service street "
                          "usually does. We share a supply line."};

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
