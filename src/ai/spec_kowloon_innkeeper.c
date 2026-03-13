#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_kowloon_innkeeper(CHAR_DATA *ch)
{
   static const char *acts[] = {
      "$n checks the guest ledger, running a finger down tonight's registered occupants.",
      "$n adjusts the lamp behind the counter, raising the wick for the evening trade.",
      "$n sets a fresh cup of tea on the counter without being asked, for a recently arrived traveler.",
      "$n notes a room key in the ledger with a careful hand, confirming the night's arrangement.",
      "$n wipes the counter with a cloth, keeping the workspace clear between transactions."
   };
   static char *says[] = {
      "Southern quarter sees more faces than any district in the city. We don't ask where you've been, just how many nights.",
      "Rooms are by the night, meals extra. Warden patrol comes through twice a bell — you'll sleep undisturbed.",
      "If you're recall-safe and breathing, Kowloon will receive you. That's the city's oldest promise. We're just where you start.",
      "The Inn district sits on old flood-refuge terraces. Solid ground, tested ground. Best sleep you'll get in the delta.",
      "Leave word at the Postmaster's if you're heading out for more than two days. CLO tracks departures — good to be on the list."
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
