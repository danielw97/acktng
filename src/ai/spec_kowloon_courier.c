#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_kowloon_courier(CHAR_DATA *ch)
{
   static const char *acts[] = {
      "$n checks a dispatch pouch's seal for tampering before shouldering it — ingrained professional habit.",
      "$n consults a route card, noting a shortcut through the registry quarter with a quick finger-trace.",
      "$n polishes the brass lantern badge on $s vest, the mark of Courier Lantern Office passage rights.",
      "$n records a delivery confirmation in a small field ledger, then stows it without pausing stride.",
      "$n scans a bulletin board at the junction corner, checking for priority rerouting notices."
   };
   static char *says[] = {
      "The badge gets you through any gate at any hour. You'd be surprised how useful that is at second bell.",
      "CLO runs the census, the post, the recall registry, and the relay stations. We count everyone inside these walls.",
      "I know every shortcut between here and the Jade Gate. You need fast delivery anywhere in this city, come to us.",
      "Sealed dispatch means sealed. I don't read them, I don't hold them. I deliver, I confirm, I move.",
      "Last week I ran the Iron Gate route four times before Bell Watch. That's corsair intel — frequent updates."
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
