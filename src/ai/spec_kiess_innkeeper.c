#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_kiess_innkeeper(CHAR_DATA *ch)
{
   static const char *acts[] = {
      "$n checks the room ledger, running a finger down the current guest list.",
      "$n tops off a traveler's cup from a clay pitcher without being asked.",
      "$n straightens the traveler memorial plaque by the door, tracing a name quietly.",
      "$n glances at the city map mural and points out a route to a newly arrived guest.",
      "$n sets a fresh lamp on the counter, adjusting the wick for the evening hours."
   };
   static char *says[] = {
      "First night in Kiess? The memorial plaque by the door has names — not to alarm you, but to remind you to come back.",
      "Roc Road is steady right now. Caravans in both directions. Good time to be traveling.",
      "The inn keeps a posted watch. You'll sleep undisturbed. City compact law holds inside these walls.",
      "If you're heading into the Forest of Confusion, check the cartographer's office east of the promenade first.",
      "Leave a forwarding note at the Postmaster's if you'll be away more than three days. Standard practice here."
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
