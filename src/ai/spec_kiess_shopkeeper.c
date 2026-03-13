#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_kiess_shopkeeper(CHAR_DATA *ch)
{
   static const char *acts[] = {
      "$n straightens the display counter, making sure the best pieces are visible from the door.",
      "$n polishes a displayed item with a soft cloth, holding it briefly to the light.",
      "$n glances up as you enter and offers a practiced nod of professional welcome.",
      "$n checks the day's inventory list against the stock behind the counter.",
      "$n sets a fresh lamp on the counter for the afternoon trade hours."
   };
   static const char *says[] = {
      "Most of what you need for the frontier is right here. We stock for the road, not the shelf.",
      "Supply shipment came in yesterday — good selection at the moment. Won't last the week.",
      "The Exchange backs the quality of everything on this counter. Kiess doesn't traffic in inferior goods.",
      "If you're heading north into the Forest, talk to the cartographer first. Know what you're walking into.",
      "Roc Road caravans bring good stock from Midgaard. Come back in a few days if I'm short on something."
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
