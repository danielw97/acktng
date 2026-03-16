#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_mayor(CHAR_DATA *ch)
{
   static const char open_path[] = "W3a3003b33000c111d0d111Oe333333O22c222112212111a1S.";

   static const char close_path[] = "W3a3003b33000c111d0d111CE333333C22c222112212111a1S.";

   static const char *path;
   static int pos;
   static bool move;
   static int last_route_start_hour = -1;

   if (!move)
   {
      if (time_info.hour == 6 && last_route_start_hour != 6)
      {
         path = open_path;
         move = TRUE;
         pos = 0;
         last_route_start_hour = 6;
      }

      if (time_info.hour == 20 && last_route_start_hour != 20)
      {
         path = close_path;
         move = TRUE;
         pos = 0;
         last_route_start_hour = 20;
      }
   }

   if (is_fighting(ch))
      return spec_cast_cleric(ch);
   if (!move || ch->position < POS_SLEEPING)
      return FALSE;

   switch (path[pos])
   {
   case '0':
   case '1':
   case '2':
   case '3':
      move_char(ch, path[pos] - '0');
      break;

   case 'W':
      ch->position = POS_STANDING;
      act("$n awakens and groans loudly.", ch, NULL, NULL, TO_ROOM);
      break;

   case 'S':
      ch->position = POS_SLEEPING;
      act("$n lies down and falls asleep.", ch, NULL, NULL, TO_ROOM);
      break;

   case 'a':
      act("$n says 'Hello Honey!'", ch, NULL, NULL, TO_ROOM);
      break;

   case 'b':
      act("$n says 'What a view!  I must do something about that dump!'", ch, NULL, NULL, TO_ROOM);
      break;

   case 'c':
      act("$n says 'Vandals!  Youngsters have no respect for anything!'", ch, NULL, NULL, TO_ROOM);
      break;

   case 'd':
      act("$n says 'Good day, citizens!'", ch, NULL, NULL, TO_ROOM);
      break;

   case 'e':
      do_yell(ch, "I hereby declare the city of Midgaard open!");
      break;

   case 'E':
      do_yell(ch, "I hereby declare the city of Midgaard closed!");
      break;

   case 'O':
      do_unlock(ch, "gate");
      do_open(ch, "gate");
      break;

   case 'C':
      do_close(ch, "gate");
      do_lock(ch, "gate");
      break;

   case '.':
      move = FALSE;
      break;
   }

   pos++;
   return FALSE;
}
