#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_undead(CHAR_DATA *ch)
{
   CHAR_DATA *victim;
   char *spell;
   int sn;

   if (!is_fighting(ch))
      return FALSE;

   for (victim = ch->in_room->first_person; victim != NULL; victim = victim->next_in_room)
      if (victim->fighting == ch && number_bits(2) != 0)
         break;

   if (victim == NULL)
      return FALSE;

   for (;;)
   {
      int min_level;

      switch (number_bits(4))
      {
      case 0:
         min_level = 0;
         spell = "blindness";
         break;
      case 1:
         min_level = 3;
         spell = "chill touch";
         break;
      case 2:
         min_level = 7;
         spell = "weaken";
         break;
      case 3:
         min_level = 30;
         spell = "earthquake";
         break;
      case 4:
         min_level = 60;
         spell = "dispel magic";
         break;
      case 5:
         min_level = 12;
         spell = "curse";
         break;
      case 6:
      case 7:
         min_level = 40;
         spell = "gate";
         break;
      case 8:
         min_level = 30;
         spell = "fireball";
         break;
      default:
         min_level = 0;
         spell = "blindness";
         break;
      }

      if (ch->level >= min_level)
         break;
      /*
       * Can put infinite loop checking here if you want, but its pointless
       * * since nothing can have a ch->level < 0. -- Alty
       */
   }

   if ((sn = skill_lookup(spell)) < 0)
      return FALSE;

   (*skill_table[sn].spell_fun)(sn, ch->level, ch, victim, NULL);

   return TRUE;
}
