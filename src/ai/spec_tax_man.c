#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_tax_man(CHAR_DATA *ch)
{

   ROOM_INDEX_DATA *room;
   CHAR_DATA *victim;
   int count;
   int vic_cnt;
   int sn = 0;
   int remainder = 0;
   int loss = 0, bank_loss = 0, char_loss = 0, old_char = 0, old_bank = 0;
   char mon_buf[MSL];
   char cat_buf[MSL];

   /*    if ( is_fighting(ch) )
          return FALSE;*/

   /*
    * Uhh.. sleeping mobs and the like shouldn't either. -- Alty
    */
   if (ch->position < POS_STANDING)
      return FALSE;

   /*    for ( ; ; )
       {
          room = get_room_index( number_range( 0, 65535 ) );
          if ( room == NULL )
        continue;*/
   /*
    * No checking for private rooms, etc. :P
    */
   /*       break;
       }*/

   /*
    * Check this loop.. -- Alty
    */
   for (room = get_room_index(number_range(0, 65535)); !room; room = get_room_index(number_range(0, 65535)))
      ;

   act("$n suddenly vanishes!", ch, NULL, NULL, TO_ROOM);
   char_from_room(ch);

   char_to_room(ch, room);
   act("$n suddenly appears in the room!", ch, NULL, NULL, TO_ROOM);

   count = 0;
   for (victim = ch->in_room->first_person; victim != NULL; victim = victim->next_in_room)
      if (!IS_NPC(victim))
         count++;

   if (count == 0)
      return FALSE;

   vic_cnt = number_range(1, count);

   count = 0;
   for (victim = ch->in_room->first_person; victim != NULL; victim = victim->next_in_room)
   {
      if (!IS_NPC(victim) && (++count) == vic_cnt)
         break;
   }

   if (victim == NULL)
      return FALSE; /* just in case :) */

   if ((victim->gold < 100000) && (victim->balance < 1000000))
      return FALSE;

   act("$n says 'Excuse me, $N, but our records state you haven't payed your taxes recently.", ch, NULL, victim, TO_ROOM);

   act("$n says, 'Since you refuse to pay what you owe, it has automatically been deducted from your funds'.", ch, NULL,
       victim, TO_ROOM);
   act("$n says, 'Have a nice day'.", ch, NULL, ch, TO_ROOM);

   sn = (victim->balance + victim->gold) / 100;
   old_char = victim->gold;
   old_bank = victim->balance;

   if (victim->balance > sn)
   {
      loss = victim->balance - sn;
      victim->balance = UMIN(victim->balance, loss);
      bank_loss = sn;
      char_loss = 0;
   }
   else
   {
      remainder = sn - victim->balance;
      bank_loss = victim->balance;
      char_loss = remainder;
      loss = victim->gold - remainder;
      victim->balance = 0;
      if (remainder > 0)
         victim->gold = UMIN(victim->gold, loss);
   }
   sprintf(mon_buf, "Tax Collector visited %s ", victim->name);
   sprintf(cat_buf, "Collected %i from bank, and %i from gold on hand.\n\r", bank_loss, char_loss);
   safe_strcat(MSL, mon_buf, cat_buf);
   sprintf(cat_buf, "New totals are balance: %i/%i  on hand: %i/%i\n\r",
           victim->balance, old_bank, victim->gold, old_char);
   safe_strcat(MSL, mon_buf, cat_buf);
   monitor_chan(mon_buf, MONITOR_MOB);

   do_save(victim, "");
   return TRUE;
}
