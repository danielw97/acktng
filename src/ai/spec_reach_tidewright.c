#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_reach_tidewright(CHAR_DATA *ch)
{
   static const char *acts[] = {
       "$n traces archaic glyphs onto a practice tablet with deliberate precision.",
       "$n compares two notation columns, frowning at a subtle branch divergence.",
       "$n sets a shadow-rod in the sand and checks the angle against a tide chart.",
       "$n copies a faded inscription line by line, pausing only to steady $s hand.",
       "$n murmurs a sequence of tidal values as if reciting liturgy."};
   static char *says[] = {
       "The Seaward Tidewrights simplified until the Script lost its soul. We preserve the full "
       "form.",
       "Tidal glyph: third-quarter rise, strong northerly, draft clearance marginal. Precise to "
       "the hour.",
       "The Jade Clerks write with questions. We answer in Script.",
       "A single stroke can change levy to exemption. Copy cleanly or not at all.",
       "Littoral Branch and inland Measure are kin, not rivals. Bad politics made them enemies."};

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
