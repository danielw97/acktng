#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_poison(CHAR_DATA *ch)
{
   CHAR_DATA *victim;

   if (!is_fighting(ch) || (victim = ch->fighting) == NULL || number_percent() > 2 * ch->level)
      return FALSE;

   act("You bite $N!", ch, NULL, victim, TO_CHAR);
   act("$n bites $N!", ch, NULL, victim, TO_NOTVICT);
   act("$n bites you!", ch, NULL, victim, TO_VICT);
   spell_poison(gsn_poison, ch->level, ch, victim, NULL);
   one_hit(ch, victim, TYPE_HIT + 8);
   return TRUE;
}
