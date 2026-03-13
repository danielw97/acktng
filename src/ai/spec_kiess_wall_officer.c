#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_kiess_wall_officer(CHAR_DATA *ch)
{
   static const char *acts[] = {
      "$n reviews the duty roster on the briefing board, making a notation in the margin.",
      "$n traces a patrol route on the tactical map with one finger, checking times against the duty log.",
      "$n steps to the window and scans the wall walk before returning to the desk.",
      "$n initials the bottom of a stack of patrol reports and files them in the duty log.",
      "$n sends a courier off with a sealed dispatch, watching until they clear the corridor."
   };
   static const char *says[] = {
      "East gate rotation is covered through Dusk Watch. North gate I want two sentries until the mist-line report comes in.",
      "Any scout returning from the northern verge reports to this office directly before the general debrief. Standing order.",
      "Wall Command doesn't guess at threat levels. We assess, we document, we act on evidence. That is the doctrine.",
      "The Withered Depths blight is moving. Slowly — but moving. Scouts returning with blight-edge data get full debrief priority.",
      "Kiess survives because its walls are manned and its gates have discipline. Everything else this city does depends on that foundation."
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
