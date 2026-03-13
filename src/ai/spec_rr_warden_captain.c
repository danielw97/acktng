#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_rr_warden_captain(CHAR_DATA *ch)
{
   static const char *acts[] = {
      "$n studies a patrol assignment board, adjusting shift notations with a chalk stick.",
      "$n paces the warden post perimeter, checking sight-lines along both stretches of road.",
      "$n reads a dispatch report, folds it precisely, and slides it into the locked post-box.",
      "$n reviews a wanted notice against $s register, then pins it to the duty-board with practiced authority.",
      "$n speaks a quiet word with a junior warden, gesturing toward the road's western stretch.",
      "$n polishes the badge of rank on $s chest with a corner of the patrol cloak, force of long habit."
   };
   static const char *says[] = {
      "This post covers the full charter march from the gate to the boundary stone. Three wardens per shift, rotating dawn and dusk.",
      "The Three Civic Oaths are not decoration. Every warden recites them at shift change. Every single shift.",
      "I've requisitioned repair crews for the drainage channel at the grain field crossing. The order is six weeks old. It's still pending.",
      "Any report of trouble west of the boundary stone goes to both my log and the convoy compact. Joint jurisdiction means joint notification.",
      "The warden who doesn't know every face on their stretch doesn't know their stretch. Walk it until you do."
   };

   if (!IS_AWAKE(ch) || is_fighting(ch))
      return FALSE;

   if (number_bits(3) != 0)
      return FALSE;

   if (number_bits(1) == 0)
      act(acts[number_range(0, 5)], ch, NULL, NULL, TO_ROOM);
   else
      do_say(ch, says[number_range(0, 4)]);

   return FALSE;
}
