#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_rr_byways_ferryman(CHAR_DATA *ch)
{
   static const char *acts[] = {
      "$n checks the river level against the dock marker, making a brief notation on the crossing board.",
      "$n runs a hand along the mooring rope, checking for fraying near the iron ring.",
      "$n shades $s eyes and scans the far bank, assessing conditions before the next run.",
      "$n adjusts the punt's position against the dock, compensating for the current's pull.",
      "$n wipes river water from $s hands on a coarse cloth tucked through $s belt."
   };
   static char *says[] = {
      "Morning crossing is smoother. Current's fastest between noon and the third hour past. Take that into account.",
      "I've been running this crossing for twenty-two years. The ford stones haven't shifted. What I worry about is the north bank — it undercuts after floods.",
      "Fare's flat rate. Kiess coin or Midgaard mint, I take both at current exchange. No credit, no barter — I've heard every version of that offer.",
      "The road market brings more traffic to this crossing. Used to be two, three travelers a day. Now it's twenty on a good market morning.",
      "There's a water gate at Kiess that river barges use. I've done that run. Good work but different entirely from a ford crossing.",
      "The village council keeps the dock platform maintained. When I need repairs they move fast. Good arrangement."
   };

   if (!IS_AWAKE(ch) || is_fighting(ch))
      return FALSE;

   if (number_bits(3) != 0)
      return FALSE;

   if (number_bits(1) == 0)
      act(acts[number_range(0, 4)], ch, NULL, NULL, TO_ROOM);
   else
      do_say(ch, says[number_range(0, 5)]);

   return FALSE;
}
