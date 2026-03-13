#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_stephen(CHAR_DATA *ch)
{
   /*
    * Award winner for daftest spec_fun of the week contest.
    * * mobs hops around, if finds PC, kisses them and gives
    * * a cure light.  -S-
    */

   ROOM_INDEX_DATA *room;
   CHAR_DATA *victim;
   int count;
   int vic_cnt;
   //    int sn;

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

   act("$n climbs up the chimney.", ch, NULL, NULL, TO_ROOM);
   char_from_room(ch);

   char_to_room(ch, room);
   act("$n emerges from the fireplace, carrying a huge sack of gifts!", ch, NULL, NULL, TO_ROOM);

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

   act("$n says 'Ho Ho Ho, Merry Christmas, $N!", ch, NULL, victim, TO_ROOM);

   act("$n smiles at $N, and hands $M a present!.", ch, NULL, victim, TO_NOTVICT);
   act("$N smiles at you, and hands you a present!.", victim, NULL, ch, TO_CHAR);

   return TRUE;
}
