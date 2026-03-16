#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_warden(CHAR_DATA *ch)
{
   static const char *acts[] = {
       "$n traces a finger along the ward sigils carved above the archway, checking for any that "
       "have faded.",
       "$n consults $s bound registry ledger, running a finger down the day's approved manifests.",
       "$n adjusts $s violet-banded sash, the mark of a licensed Compact warden.",
       "$n glances at a passing figure, then makes a brief notation in $s ledger.",
       "$n taps the checkpoint placard with one finger: 'Arcane materials — registry notation "
       "required.'"};
   static char *says[] = {
       "All arcane goods require registry notation before this threshold. The Compact is clear.",
       "If you carry unregistered materials, declare them now. The ledger is open.",
       "The Compact protects you as much as the city, traveler. Documentation is not the enemy.",
       "Containment over destruction — that is the Compact's doctrine. We seal; we do not burn.",
       "Every passage to the below is a civic jurisdiction. Remember that before you descend."};

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
