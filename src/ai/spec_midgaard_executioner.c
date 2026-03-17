#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_midgaard_executioner(CHAR_DATA *ch)
{
   char buf[MAX_STRING_LENGTH];
   CHAR_DATA *victim;
   char *crime;
   bool undead;

   undead = FALSE;

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
      if (IS_NPC(victim) && IS_SET(victim->act, ACT_UNDEAD))
      {
         crime = "UNDEAD";
         undead = TRUE;
         break;
      }
   }

   if (victim == NULL)
      return FALSE;

   if (undead)
      sprintf(buf, "Undead in the temple ward! The Watch responds — clear the precinct!");
   else
      sprintf(buf, "%s stands condemned as a %s! The sentence is mine to carry out!", victim->name,
              crime);

   do_yell(ch, buf);
   multi_hit(ch, victim, TYPE_UNDEFINED);
   char_to_room(create_mobile(get_mob_index(MOB_VNUM_CITYGUARD)), ch->in_room);
   char_to_room(create_mobile(get_mob_index(MOB_VNUM_CITYGUARD)), ch->in_room);
   return TRUE;
}
