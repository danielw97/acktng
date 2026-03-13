#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_lay_sister(CHAR_DATA *ch)
{
   static const char *acts[] = {
      "$n pauses at a memorial marker, adjusting a small oil lamp at its base.",
      "$n opens $s worn civic liturgy booklet to a marked page, lips moving silently.",
      "$n traces a carved name on the memorial wall with one careful finger.",
      "$n sets a fresh tallow stub beside the others at the base of the memorial stone.",
      "$n folds $s hands and stands in quiet observance before the carved dedication."
   };
   static char *says[] = {
      "No ward unlit, no traveler uncounted. First Oath. Spoken at every memorial by the book.",
      "No judgment hidden, no sentence unrecorded. Second Oath. The Temple holds to it even when the courts do not.",
      "No levy taken that cannot be borne. Third Oath. The hardest to keep, in my experience.",
      "The Temple maintains rolls of the uncounted. Those removed from the ledgers are still remembered here.",
      "Two bells after midnight are not alarm. They are memorial. The city remembers, even when it pretends not to."
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
