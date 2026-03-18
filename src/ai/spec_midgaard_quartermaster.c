#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_midgaard_quartermaster(CHAR_DATA *ch)
{
   static const char *acts[] = {
       "$n draws a careful line under a stock column and checks the sum against the requisition "
       "record.",
       "$n opens a new supply folio, creases the spine, and writes the date at the top in formal "
       "script.",
       "$n lifts a crate lid, counts the contents, and marks the tally on $s wax tablet.",
       "$n taps a ledger page with the blunt end of $s stylus, calculating a replacement order in "
       "$s head.",
       "$n slides a completed inventory sheet into the district register and seals the clasp."};
   static char *says[] = {"The quartermaster post covers strategic supply for the central "
                          "district. Perishables, equipment, restorative stock — all of it.",
                          "Requisition forms are on the counter. Anything above a standard "
                          "allotment requires a warden countersignature.",
                          "The invasion stockpile is maintained separately. If you're looking for "
                          "those items, the armory post is around the service street.",
                          "Supply lines run through the Roc Road compact and the eastern desert "
                          "caravan network. Delays on either line show up here first.",
                          "The Registry requires a full stock count every five days. I keep mine "
                          "current. Most of my counterparts in other districts do not."};

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
