#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_mino_guard(CHAR_DATA *ch)
{
   CHAR_DATA *victim;
   OBJ_DATA *pass;

   if (!IS_AWAKE(ch) || is_fighting(ch))
      return FALSE;

   for (victim = ch->in_room->first_person; victim != NULL; victim = victim->next_in_room)
   {
      if (victim == NULL || ch == NULL)
         continue;
      if ((IS_NPC(victim)) || (ch == victim) || (IS_IMMORTAL(victim)))
         continue;
      pass = get_eq_char(victim, WEAR_HOLD_HAND_R);
      if (pass != NULL && pass->pIndexData->vnum == OBJ_VNUM_MINO_PASS)
      {
         do_say(ch, "@@eLet this one pass@@N");
         continue;
      }

      pass = get_eq_char(victim, WEAR_HOLD_HAND_L);
      if (pass != NULL && pass->pIndexData->vnum == OBJ_VNUM_MINO_PASS)
      {
         do_say(ch, "@@eLet this one pass@@N");
         continue;
      }

      do_yell(ch, "Intruder Alert! Man the Walls!");
      multi_hit(ch, victim, TYPE_UNDEFINED);

      return TRUE;
   }

   return FALSE;
}
