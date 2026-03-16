#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_gnf_toll_collector(CHAR_DATA *ch)
{
   static const char *acts[] = {
       "$n reviews a passage-tithe ledger, running a finger down columns of goods received.",
       "$n weighs a measure of salt against a small balance scale on $s table.",
       "$n marks an entry in $s ledger with careful notation, dating and signing it.",
       "$n checks the seal on a cargo crate, comparing it against $s manifest sheet.",
       "$n stamps a travel permit with the bridge-crossing seal, adding it to a day's stack."};
   static char *says[] = {
       "Passage tithe at this crossing: one measure salt, two iron nails, or one lamp oil flask. "
       "Relic rate. Set when the road was built.",
       "I maintain ledgers going back forty years at this post. The tithe records are the road's "
       "memory — everything passes, everything's counted.",
       "Sealed cargo gets a crossing but goes in the inspection queue. The fen charter is specific "
       "about undeclared goods.",
       "You'd be surprised how many people try to argue about the tithe. It hasn't changed in two "
       "centuries. I have documentation.",
       "The Passage Tithe kept this road alive through the Ash Winters. Grain and salt kept the "
       "waystations running when coin was worthless."};

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
