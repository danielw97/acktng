#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_midgaard_temple_priest(CHAR_DATA *ch)
{
   static const char *acts[] = {
      "$n lights each of the four altar braziers in the prescribed sequence, murmuring the dedication.",
      "$n opens the civic liturgy to the founding observance and reads the passage aloud in a clear, measured voice.",
      "$n traces the First Stone Accord inscription on the altar face with one careful finger.",
      "$n arranges fresh votive candles at the base of the memorial column with practiced precision.",
      "$n bows to the carved compass-rose at the hall's center — the survey datum from which Midgaard was measured."
   };
   static char *says[] = {
      "No ward unlit, no traveler uncounted. The First Oath. We speak it at every observance in this hall.",
      "No judgment hidden, no sentence unrecorded. The Second Oath. The temple holds to it even when the civic courts do not.",
      "No levy taken that cannot be borne. The Third Oath. The hardest to keep. The canon council debates it every year.",
      "The founding myths on the vault ceiling were painted during the year of the First Stone Accord. The artist's name is recorded in the temple rolls.",
      "The temple serves as sanctuary, registry of oaths, and consecrated civic space. All three functions are active at all times."
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
