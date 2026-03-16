#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_kowloon_magistracy(CHAR_DATA *ch)
{
   static const char *acts[] = {"$n reviews a stack of permit filings, stamping each with a "
                                "jade-green seal in practiced succession.",
                                "$n marks a chalk annotation on the public notice board, citing a "
                                "licensing provision by section.",
                                "$n pauses to check a merchant's display ribbon against the "
                                "Magistracy's current audit register.",
                                "$n consults a pocket copy of the Covenant Schedule, "
                                "cross-referencing a statute by section number.",
                                "$n makes a notation in $s field ledger after observing the "
                                "square, expression professionally neutral."};
   static char *says[] = {
       "Jade ribbon means the Magistracy audited the weights this season. No ribbon, no Concordat "
       "standing.",
       "The Schedule is public record. Any citizen can cite it in a dispute. Most people don't "
       "bother learning it, which is how most disputes happen.",
       "Gold lacquer on the doorframe means the merchant posted bond against fraud. It's not "
       "decoration — it's a legal instrument.",
       "The Covenant established civic law above clan retaliation. That's not an opinion, it's the "
       "first article. Enforceable since the founding.",
       "Civil disputes unresolved by the seventh day escalate to the covenant stone. Most people "
       "settle on the sixth day."};

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
