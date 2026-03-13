#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_kowloon_corsair(CHAR_DATA *ch)
{
   static const char *acts[] = {
      "$n watches the harbor approaches with calculating eyes, noting the patrol timing.",
      "$n marks a rough sketch of dock timing in charcoal on the back of a tariff card.",
      "$n exchanges a brief, quiet gesture with a passing figure and continues without stopping.",
      "$n scans a Harbor Syndic notice board, memorizing the updated patrol rotation.",
      "$n moves to a shadowed position near the dock approach, tracking a Warden guard's circuit timing."
   };
   static const char *says[] = {
      "Shen knows these channels better than the Syndic charts do. Every gate timing, every patrol gap. Patience is strategy.",
      "This isn't a raid — this is a probe. We learn their timing tonight. We act when we know what they know.",
      "The Iron Gate convoy runs at third bell. Twice a week, same route. Someone in the Syndic is selling information.",
      "Blacktide Shen has been watching this city for three years. Every gate, every shift change, every weakness.",
      "The Harbor Syndics posted a bounty. They can post all the gold they want — Shen doesn't come out of the channels for that."
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
