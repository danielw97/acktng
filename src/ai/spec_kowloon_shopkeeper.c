#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_kowloon_shopkeeper(CHAR_DATA *ch)
{
   static const char *acts[] = {
      "$n hangs a display item on a wall hook rather than setting it on the table — an old floating-market habit.",
      "$n checks a displayed item's jade inspection ribbon, confirming it's still current-season certified.",
      "$n glances up as you enter and offers a practiced nod of professional welcome.",
      "$n straightens the display counter, aligning items with the precision of someone who measures by eye.",
      "$n reviews the day's inventory tally against stock, marking each confirmed item with a practiced stroke."
   };
   static char *says[] = {
      "Everything on this counter is Ledgerhouse-cleared. Tariff paid, weights checked, ribbon current.",
      "My family's been selling on hooks since before these walls were built. Tables are for the new arrivals.",
      "The Guild Concordat sets our floor prices. I can't go below that — but I can throw in the wrap at no charge.",
      "Jade ribbon expired, we don't sell it. Magistracy comes through quarterly and they check. I don't need that trouble.",
      "Supply run from the western trade came in two days ago. You're getting first-season goods, not leftovers."
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
