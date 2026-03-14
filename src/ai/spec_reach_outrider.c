#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_reach_outrider(CHAR_DATA *ch)
{
   static const char *acts[] = {
      "$n studies a cargo manifest with predatory focus, tracing debts with one finger.",
      "$n adjusts a set of brass scales and notes the balance point with a thin smile.",
      "$n scratches fresh interest figures onto a wax tablet and warms it closed.",
      "$n circles a caravan mark in red chalk and folds the ledger shut.",
      "$n whispers terms under $s breath while counting on $s knuckles."
   };
   static char *says[] = {
      "Distressed cargo at shore conversion rates? That interest compounds before you reach dock.",
      "The Red Sand Accounts extend credit to anyone. Repayment, however, is not optional.",
      "Your manifest shows water-debt outstanding. We can consolidate that. The terms are... reasonable.",
      "Late at the Seal means penalties at the Strand. Delay is a debt like any other.",
      "Sign today and keep your wagons. Refuse, and we'll collect what's left after weather and law."
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
