/* New quest code */
#include "globals.h"

void do_proposition(CHAR_DATA *ch, char *argument)
{
/*    int bit;
   int step;
   int target[6];
   int amount[6];
   int complete[6];*/

   char buf[MSL];
   char arg[MSL];
   CHAR_DATA *qm;

   argument = one_argument(argument, arg);

   if (!str_prefix(arg, "help") )
   {
      send_to_char("Commands: Request, complete, status\n\r",ch);
      return;
   }

   for (qm = ch->in_room->first_person; qm != NULL; qm = qm->next_in_room)
      if (IS_NPC(qm) && IS_SET(qm->act, ACT_POSTMAN))
         break;

   if (qm == NULL)
   {
      send_to_char("But there is no PostMaster/questmaster here!\n\r", ch);
      return;
   }

   send_to_char("Postmaster/questmaster found!\n\r",ch);
}
