#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_cast_judge(CHAR_DATA *ch)
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

   spell = "high explosive";
   if ((sn = skill_lookup(spell)) < 0)
      return FALSE;
   (*skill_table[sn].spell_fun)(sn, ch->level, ch, victim, NULL);
   return TRUE;
}
