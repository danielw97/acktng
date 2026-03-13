#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_cast_undead(CHAR_DATA *ch)
{
   CHAR_DATA *victim;
   char *spell;
   int sn;

   if (!is_fighting(ch))
      return FALSE;

   for (victim = ch->in_room->first_person; victim != NULL; victim = victim->next_in_room)
      if (victim->fighting == ch && number_bits(2) == 0)
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
         spell = "curse";
         break;
      case 1:
         min_level = 3;
         spell = "weaken";
         break;
      case 2:
         min_level = 6;
         spell = "chill touch";
         break;
      case 3:
         min_level = 9;
         spell = "blindness";
         break;
      case 4:
         min_level = 12;
         spell = "poison";
         break;
      case 5:
      case 6:
         min_level = 18;
         spell = "harm";
         break;
      case 7:
         min_level = 21;
         spell = "teleport";
         break;
      default:
         min_level = 24;
         spell = "gate";
         break;
      }

      if (ch->level >= min_level)
         break;
      /*
       * Can add infinite loop checking here, but its kinda pointless,
       * * as no mob can have ch->level < 0. -- Alty
       */
   }

   if ((sn = skill_lookup(spell)) < 0)
      return FALSE;
   act("$n summons forth the powers of darkness!", ch, NULL, NULL, TO_ROOM);
   (*skill_table[sn].spell_fun)(sn, ch->level, ch, victim, NULL);
   return TRUE;
}
