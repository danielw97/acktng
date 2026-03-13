#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_cast_mage(CHAR_DATA *ch)
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
         min_level = 8;
         spell = "teleport";
         break;
      case 4:
         min_level = 11;
         spell = "color spray";
         break;
      case 5:
         min_level = 12;
         spell = "change sex";
         break;
      case 6:
      case 7:
      case 8:
      case 9:
         min_level = 15;
         spell = "fireball";
         break;
      default:
         min_level = 20;
         spell = "acid blast";
         break;
      }

      if (ch->level >= min_level)
         break;
      /*
       * Can add infinite loop checking here, but its kinda pointless,
       * * as no mob can have ch->level < 0.  -- Alty
       */
   }

   if ((sn = skill_lookup(spell)) < 0)
      return FALSE;

   act("An eerie sound comes from $n as $e stares at $N!!", ch, NULL, victim, TO_ROOM);
   (*skill_table[sn].spell_fun)(sn, ch->level, ch, victim, NULL);
   return TRUE;
}
