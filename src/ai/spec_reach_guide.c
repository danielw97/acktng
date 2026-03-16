#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_reach_guide(CHAR_DATA *ch)
{
   static const char *acts[] = {
       "$n kneels to inspect the glass underfoot, weighing its color before standing again.",
       "$n taps the crust with a chitin-soled staff and listens for a hollow ring.",
       "$n ties a fresh cord marker to a cairn spur for the next caravan behind $m.",
       "$n tosses a pinch of sand into the air and watches the wind's turn.",
       "$n hums an old route-song cadence and matches the beat to $s stride."};
   static char *says[] = {
       "Milky glass, firm step. Amber glass, watch your feet. Violet glass, turn back.",
       "We guided before the Keepers carved their first obelisk. The songs remember what stones "
       "forget.",
       "I guide anyone who pays. Warden, Outrider, Synod -- the glass does not care about your "
       "politics.",
       "The belts shift at dawn and dusk. If you walk by memory alone, the Reach buries you.",
       "Follow my pace and don't break the line. One bad step can open a whole sheet beneath us."};

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
