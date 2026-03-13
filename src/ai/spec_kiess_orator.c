#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_kiess_orator(CHAR_DATA *ch)
{
   static const char *speeches[] = {
      "Evermeet was not destroyed in a single night. It was abandoned in a thousand small decisions. Kiess was built so those decisions cannot be made quietly.",
      "The Compact Flame has burned without interruption since the first foundation stone was laid. That continuity is deliberate. Watch it — it is doing something.",
      "Three factions, one peace code. Not because they agree — they rarely agree — but because the alternative is Evermeet. We have seen the alternative.",
      "The Pillar of Returning Caravans adds a mark for every safe return from the frontier. Count them sometime. That count is what this city is actually defending.",
      "Roc Road connects us east. The Forest of Confusion lies north. Wall Command holds the perimeter. The Compact holds the center. This is not accidental — this is the design.",
      "Every waystone on these avenues marks a district that once existed in Evermeet. Every one of those districts fell silent. We read those names so we know what we are protecting against."
   };

   if (!IS_AWAKE(ch) || is_fighting(ch))
      return FALSE;

   if (number_bits(3) != 0)
      return FALSE;

   act("$n addresses the assembled listeners with the measured cadence of a prepared civic speaker.", ch, NULL, NULL, TO_ROOM);
   do_say(ch, speeches[number_range(0, 5)]);

   return FALSE;
}
