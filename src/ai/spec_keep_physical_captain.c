#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

static void do_massivestrike(CHAR_DATA *ch)
{
   int dam = number_range(9000, 11000);

   calculate_damage(ch, ch->fighting, dam, gsn_holystrike, ELEMENT_PHYSICAL | ELEMENT_HOLY, TRUE);
}

bool spec_keep_physical_captain(CHAR_DATA *ch)
{
   char buf[MSL];

   if (!is_fighting(ch))
   {
      ch->spec_behavior = 0;
      return FALSE;
   }

   ch->spec_behavior++;

   switch (ch->spec_behavior % 10)
   {
   case 1:
      if (number_percent() < 50)
         do_morale(ch, "");
      else
         do_leadership(ch, "");
      break;
   case 4:
      if (number_percent() < 50)
         do_warcry(ch, "");
      else
         do_beserk(ch, "");
      break;
   case 5:
      sprintf(buf, "@@y$n brings his weapon over his head, preparing for a massive strike!@@N");
      act(buf, ch, NULL, NULL, TO_ROOM);
      break;
   case 7:
      sprintf(buf, "@@y$n brings his weapon down in a massive strike against $N!@@N");
      act(buf, ch, NULL, ch->fighting, TO_ROOM);
      do_massivestrike(ch);
      break;
   }

   return TRUE;
}
