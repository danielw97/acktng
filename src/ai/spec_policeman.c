#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_policeman(CHAR_DATA *ch)
{
   char buf[MAX_STRING_LENGTH];
   CHAR_DATA *victim;
   char *crime;

   if (!IS_AWAKE(ch) || is_fighting(ch))
      return FALSE;

   crime = "";
   for (victim = ch->in_room->first_person; victim != NULL; victim = victim->next_in_room)
   {
      if (!IS_NPC(victim) && IS_SET(victim->act, PLR_KILLER))
      {
         crime = "KILLER";
         break;
      }

      if (!IS_NPC(victim) && IS_SET(victim->act, PLR_THIEF))
      {
         crime = "THIEF";
         break;
      }
   }

   if (victim == NULL)
      return FALSE;

   if (ch->hunting || ch->hunt_obj)
      /*
       * if ( (int) (ch->hunting) > 0)
       */
      /*
       * Don't hunt someone if already hunting
       */
      return FALSE;

   sprintf(buf, "%s is a %s, I shall not rest till justice is done!", victim->name, crime);
   do_yell(ch, buf);

   set_hunt(ch, NULL, victim, NULL, 0, 0);

   return TRUE;
}
