#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_rr_peddler(CHAR_DATA *ch)
{
   static const char *acts[] = {
      "$n reorganizes the wares on $s handcart, hanging the loudest pots where travelers will hear them.",
      "$n holds up a small sealed vial to the light, checking the contents before repacking it.",
      "$n scribbles a price adjustment in $s trade ledger, rubbing out the old figure with a thumb.",
      "$n shifts the handcart to the road verge as a convoy passes, then rattles back into position.",
      "$n counts coins from a belt pouch, rechecks the amount, and slides them back with practiced speed."
   };
   static char *says[] = {
      "Three days from Midgaard, two to Kiess if the road's clear. I run it twice a month. Know every pothole.",
      "Best thing I carry? Waterproofing wax and spare lamp wicks. Every patrol warden needs them. Regular customers.",
      "Heard there's tollbreak trouble at the ford. I came the long way around. Add a day, keep my goods.",
      "Kiess prices are better for finished goods. Midgaard pays more for raw materials and old script rubbings. I move both ways.",
      "The ridge hills are wolf territory after dark. I sleep at the mule rest now, not the open verge. Learned that the hard way."
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
