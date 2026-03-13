#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_rr_charter_keeper(CHAR_DATA *ch)
{
   static const char *acts[] = {
      "$n works a stiff brush along a charter marker slab, clearing lichen from the carved letters.",
      "$n dips a small brush into a pot of blue pigment and carefully repaints a faded inscription.",
      "$n leans close to a worn marker, tracing the oldest layer of text with one finger.",
      "$n marks a slab number in a maintenance register, noting the date and condition.",
      "$n straightens from a finished slab and moves to the next, adjusting the pigment pot on $s belt."
   };
   static char *says[] = {
      "Three eras of script on most of these slabs. The oldest predates Midgaard's current charter system by at least two centuries.",
      "The inscriptions are the road's legal memory. As long as they're legible, the charter is defensible in arbitration.",
      "Someone tried to chip off a slab last season. Probably a collector. The Magistrate Ledgerhouse treats it as property destruction.",
      "I repaint in Midgaard blue because that's the authorized pigment. Using the wrong blue is a recordkeeping violation. I know how that sounds.",
      "The pre-charter text underneath — mostly boundary claims and toll schedules. Very old, very clear about who was here first."
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
