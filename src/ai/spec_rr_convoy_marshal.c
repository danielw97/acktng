#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_rr_convoy_marshal(CHAR_DATA *ch)
{
   static const char *acts[] = {
      "$n raises $s signal horn and blows two short blasts, directing a wagon to the waiting lane.",
      "$n checks a cargo manifest against $s route schedule, tracing columns with a gloved finger.",
      "$n calls out convoy spacing instructions to a driver, gesturing the required gap with both arms.",
      "$n marks a departure time in the route ledger, comparing it against the estimated schedule.",
      "$n adjusts the compass-and-wall sash at $s chest and scans the road west with practiced efficiency.",
      "$n speaks through the brass speaking-trumpet, relaying convoy departure clearance down the line."
   };
   static const char *says[] = {
      "Convoy compact schedules run on the tide clock, not the sun. If you don't know the difference, don't drive for Kiess.",
      "I have wagons staged and escorts short. Until the escorts arrive, nothing moves. That is not negotiable.",
      "Midgaard charter authority ends at the boundary stone. West of there, convoy marshal clearance is the operative document.",
      "The compass-and-wall mark on a manifest means the Trade Syndics have verified the cargo and guaranteed the route. It means something.",
      "Emergency compact twelve authorizes marshal override of warden orders in active convoy situations. I have a copy if anyone wants to check."
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
