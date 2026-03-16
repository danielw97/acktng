#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_gnf_lamplighter(CHAR_DATA *ch)
{
   static const char *acts[] = {
       "$n refills a waystone lantern from an oil flask, steadying the reservoir with practiced "
       "hands.",
       "$n trims a charred lamp wick with small snips, working down the post-line.",
       "$n checks the iron bracket of a Lantern Road post, testing for rust and loosening.",
       "$n marks a post number in $s maintenance log, counting the day's route from south to "
       "north.",
       "$n patches a cracked lamp-glass with pitch and hemp cord, a temporary fix until Midgaard "
       "sends glass."};
   static char *says[] = {"The Lantern Road gets its name because the posts stay lit. My job is "
                          "making sure that's true every night.",
                          "Rootbound mischief knocked two posts over near the Mosswater ford. I've "
                          "been filing incident reports for months.",
                          "Midgaard treasury funds the oil. Kowloon provides the lamp glass in the "
                          "north. We share the maintenance. That's the compact.",
                          "No ward unlit. That's the first road oath. I've walked this line in "
                          "winter storms to keep it.",
                          "Count seventeen from the south gate — that's the post where the "
                          "original iron was cast by Ilren Voss herself. Still holding."};

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
