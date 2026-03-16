#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_sylai_priest(CHAR_DATA *ch)
{
   /*
    * For sylai priest only.  Pushes player from overhang into pit below.
    * * -Players are gonna love this.  :)
    * * -S-
    */

   CHAR_DATA *victim;
   ROOM_INDEX_DATA *location;

   if (!is_fighting(ch))
      return FALSE;

   if (ch->in_room->vnum != 439)
      return FALSE;

   if (number_percent() > 30)
      return FALSE;

   for (victim = ch->in_room->first_person; victim != NULL; victim = victim->next_in_room)
   {
      if (victim->fighting == ch && number_bits(2) == 0)
         break;
   }

   if (victim == NULL)
      return FALSE;

   if (number_percent() > 60)
   {
      act("$n tries to knock $N off the outcrop!", ch, NULL, victim, TO_NOTVICT);
      act("$N tries to knock you off the outcrop!", victim, NULL, ch, TO_CHAR);
      return FALSE;
   }

   if (number_percent() > 70)
      location = get_room_index(ROOM_VNUM_LIMBO);
   else
      location = get_room_index(ROOM_VNUM_LIMBO);

   if (location == NULL)
   {
      notify("Fucked up spec_sylai_priest.\n\r", 82);
      return FALSE;
   }

   act("$n rushes towards $N and shoves $M off the outcrop!", ch, NULL, victim, TO_NOTVICT);
   act("$N rushes at you, and shoves you off the cliff!", victim, NULL, ch, TO_CHAR);
   stop_fighting(ch, TRUE);
   char_from_room(victim);

   char_to_room(victim, location);
   act("$n lands on the floor, having fallen from the outcrop above!", victim, NULL, NULL,
       TO_NOTVICT);
   send_to_char("You crash to the floor below!\n\r", victim);
   do_look(victim, "");
   return TRUE;
}
