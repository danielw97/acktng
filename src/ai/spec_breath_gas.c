#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_breath_gas(CHAR_DATA *ch)
{
   int sn;

   if (!is_fighting(ch))
      return FALSE;

   if ((sn = skill_lookup("gas breath")) < 0)
      return FALSE;
   (*skill_table[sn].spell_fun)(sn, ch->level, ch, NULL, NULL);
   return TRUE;
}
