#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_kowloon_shrine(CHAR_DATA *ch)
{
   static const char *acts[] = {"$n sweeps the offering bowl at the base of the shrine, clearing "
                                "wilted petals with practiced care.",
                                "$n relights a stick of incense from a small striker, holding it "
                                "steady until the smoke rises evenly.",
                                "$n refills the lamp oil in the shrine's base lantern, checking "
                                "the level with a careful fingertip.",
                                "$n traces the carved dedication text with one finger, lips moving "
                                "silently in memorization.",
                                "$n places a fresh offering of pressed flowers at the shrine base, "
                                "straightening the arrangement."};
   static char *says[] = {
       "First Oath: No ward unlit, no traveler uncounted. The Temple observes this at every "
       "shrine, whether or not the city does.",
       "Second Oath: No judgment hidden, no sentence unrecorded. We remind the city of what it "
       "promised, one shrine at a time.",
       "Third Oath: No levy taken that cannot be borne. The hardest one to keep. We hear the "
       "disputes — we don't resolve them, but we remember them.",
       "The Temple answers to no secular authority while tending the sick. That exemption has been "
       "in the Covenant since the founding. We don't let people forget it.",
       "There is a shrine for Yen-Mak in every district. The delta remembers what the ledgers "
       "removed."};

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
