#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "globals.h"
#include "tables.h"
#include "magic.h"

bool spell_predictive_collapse(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{
   CHAR_DATA *victim = (CHAR_DATA *)vo;
   AFFECT_DATA af;
   int dam;

   dam = dice(ch->class_level[CLASS_KIN] + ch->class_level[CLASS_NEC] + ch->class_level[CLASS_EGO],
              10);

   af.type = sn;
   af.duration = 2 + ch->class_level[CLASS_KIN] / 5;
   af.duration_type = DURATION_ROUND;
   af.location = APPLY_HITROLL;
   af.modifier = -(ch->class_level[CLASS_KIN] * 3);
   af.bitvector = 0;
   af.caster = ch;
   affect_to_char(victim, &af);

   af.type = sn;
   af.duration = 2 + ch->class_level[CLASS_KIN] / 5;
   af.duration_type = DURATION_ROUND;
   af.location = APPLY_DAMROLL;
   af.modifier = -(ch->class_level[CLASS_KIN] * 2);
   af.bitvector = 0;
   af.caster = ch;
   affect_to_char(victim, &af);

   act("@@G$n models $N's responses and collapses $S combat patterns!@@N", ch, NULL, victim,
       TO_NOTVICT);
   act("@@G$n collapses your combat patterns -- every response you attempt was predicted!@@N", ch, NULL,
       victim, TO_VICT);
   act("@@GYou model $N's responses and collapse $S combat patterns!@@N", ch, NULL, victim, TO_CHAR);

   sp_damage(obj, ch, victim, dam, ELEMENT_MENTAL | ELEMENT_SHADOW, sn, TRUE);
   return TRUE;
}
