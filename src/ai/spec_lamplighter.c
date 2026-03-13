#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_lamplighter(CHAR_DATA *ch)
{
   static const char *acts[] = {
      "$n trims a lamp wick with practiced hands, a small curl of smoke rising as the flame steadies.",
      "$n checks the oil level in a street lantern, tilting the reservoir with a careful eye.",
      "$n adjusts the reflector on a wall-mounted lamp, angling it outward toward the street.",
      "$n taps a lamp bracket with $s hook pole, testing the iron fitting for rust.",
      "$n consults a worn route card from $s coat pocket, muttering a count of posts.",
      "$n refills a lamp from one of the oil flasks on $s shoulder satchel."
   };
   static const char *says[] = {
      "No ward unlit, no traveler uncounted — that's the Oath, and that's the work.",
      "Kindling Watch runs till the Ledger bells. All posts on this lamp-line are checked.",
      "You see a dark corner in this city, report it to the Lantern Registry. I'll have it lit by Ash Watch.",
      "Ash Watch runs long when the wind comes off the western road. Outer posts need double oil.",
      "The Registry lists every lamp-post in the city. Every one. I've walked them all."
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
