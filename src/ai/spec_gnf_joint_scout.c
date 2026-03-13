#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_gnf_joint_scout(CHAR_DATA *ch)
{
   static const char *acts[] = {
      "$n marks a notation on $s patrol map with the joint-commission cipher.",
      "$n scans the treeline with alert, resting eyes — the watchfulness of someone in field-recovery.",
      "$n checks $s commission writ, confirming the dual-city seal is still intact.",
      "$n counts boot-prints on the soft shoulder of the road with careful attention.",
      "$n exchanges a quiet hand-signal with a fellow patrol as they pass."
   };
   static char *says[] = {
      "Joint commission patrol means dual authority. I write to Midgaard's rangers and Kowloon's wardens both. One incident, two ledgers.",
      "Ashfang sign in the Ironpine Rise. Prints were a day old. We've notified both city commands. Standard protocol.",
      "The road's safe if you stay on it. The moment you step off to investigate something interesting, the risk calculates differently.",
      "Three patrol pairs cover the road from gate to gate on stagger. You'll see wardens every few hours if the rotation holds.",
      "Forest charter says no army may march this road in war formation. One patrol at a time. That rule has held for two generations."
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
