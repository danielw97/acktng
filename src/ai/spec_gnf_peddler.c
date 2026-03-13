#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_gnf_peddler(CHAR_DATA *ch)
{
   static const char *acts[] = {
      "$n rummages through $s pack, reorganizing the goods inside with practiced efficiency.",
      "$n holds up a small vial to the light: northern resin, sealed and fresh.",
      "$n notes a price in $s trade ledger, tallying the day's sales with a stub of charcoal.",
      "$n shifts $s pack to the other shoulder, wincing at the familiar ache.",
      "$n calls out to a passing figure, offering a wave and a quick display of wares."
   };
   static char *says[] = {
      "Northern resin from the Ironpine Rise is what moves. Wardens need it for sealing dispatches, healers for wounds.",
      "Ashfang trouble north of the Bends. I came down two days ago and heard raiding horns in the Ironpine. Travel cautious.",
      "Mire-clan trade works if you know their system. They don't want coin — they want iron nails, lamp oil, and salt.",
      "The road's cheaper than you'd think if you know who to ask for passage tithe. I pay in dried fish and everyone's happy.",
      "Three Kowloon caravans came south last week, all moving fast. Best gossip on the road, and it only costs you a copper."
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
