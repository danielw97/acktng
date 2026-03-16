#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_undead(CHAR_DATA *ch)
{
   CHAR_DATA *victim;
   CHAR_DATA *ach;
   char *spell;
   int sn, sum_lev;

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
         min_level = 1;
         spell = "summon";
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

   if (!str_cmp(spell, "summon")) /* CHECK FOR NPC!!!!!!!!!!!!! */
   {
      sum_lev = ch->level * 2 / 3;
      for (ach = first_char; ach != NULL; ach = ach->next)
      {
         if (!IS_SET(ach->act, ACT_UNDEAD) || ach->level > sum_lev ||
             !IS_NPC(ach) /* Kavir got summoned!  :P */
             || ach->in_room == ch->in_room || !can_see(ch, ach) || number_bits(2) != 0)
            continue;

         victim = ach;
         break;
      }
      if (ach == NULL)
         return FALSE;

      act("$n's eyes turn black for an instant!", ch, NULL, NULL, TO_ROOM);
      act("$n disappears suddenly.", victim, NULL, NULL, TO_ROOM);
      char_from_room(victim);
      char_to_room(victim, ch->in_room);
      act("$n arrives suddenly.", victim, NULL, NULL, TO_ROOM);
      multi_hit(victim, ch->fighting, TYPE_UNDEFINED);
   }
   else
   {
      (*skill_table[sn].spell_fun)(sn, ch->level, ch, victim, NULL);
   }

   return TRUE;
}
