#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_midgaard_postmaster(CHAR_DATA *ch)
{
   static const char *acts[] = {
       "$n tacks a fresh civic notice to the posting board with crisp, even strikes.",
       "$n removes an expired notice, folds it precisely, and slides it into a registry sleeve.",
       "$n holds a sealed dispatch to the light, checking the wax stamp before entering it in the "
       "log.",
       "$n sorts incoming correspondence into district pigeonholes, reading each address label "
       "aloud.",
       "$n stamps an outgoing packet with the day's date-mark and adds it to the departure stack."};
   static char *says[] = {"Every notice on this board is a registered civic communication. "
                          "Removing one is a Compact infringement.",
                          "The posting circuit covers all primary intersections in the district. "
                          "If a notice isn't here, check the temple forecourt board.",
                          "Civic notices require five days of public posting before an ordinance "
                          "takes legal effect. Count the date-stamp yourself.",
                          "The Ledgerhouse handles inter-district correspondence. This station "
                          "handles central-district and priority civic mail.",
                          "Sealed dispatches can be left at the counter. They go out with the next "
                          "courier rotation, which runs at Kindling Watch and Ash Watch."};

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
