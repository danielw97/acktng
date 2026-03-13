#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_cast_bigtime(CHAR_DATA *ch)
{
   CHAR_DATA *victim;
   char *spell;
   int sn;
   int crun;

   if (!is_fighting(ch))
      return FALSE;

   for (victim = ch->in_room->first_person; victim; victim = victim->next_in_room)
      if (victim->fighting == ch && number_bits(2) == 0)
         break;

   if (victim == NULL)
      return FALSE;

   for (crun = 0;; crun++)
   {
      int min_level;

      switch (number_bits(4))
      {
      case 0:
         min_level = 45;
         spell = "dispel magic";
         break;
      case 1:
         min_level = 46;
         spell = "acid blast";
         break;
      case 2:
         min_level = 30;
         spell = "phobia";
         break;
      case 3:
         min_level = 55;
         spell = "chain lightning";
         break;
      case 4:
      case 5:
         min_level = 43;
         spell = "mindflame";
         break;
      case 6:
         min_level = 12;
         spell = "lava burst";
         break;
      case 7:
         min_level = 44;
         spell = "static";
         break;
      default:
         min_level = 25;
         spell = "hellspawn";
         break;
      }

      if (ch->level >= min_level)
         break;
      /*
       * Stop infinite recursion on low level mobs.. (ch->level < 12)
       * * -- Alty
       */
      if (crun > 10)
         return FALSE;
   }

   if ((sn = skill_lookup(spell)) < 0)
      return FALSE;
   act("$n invokes the dark powers!!", ch, NULL, NULL, TO_ROOM);
   (*skill_table[sn].spell_fun)(sn, ch->level, ch, victim, NULL);
   return TRUE;
}
