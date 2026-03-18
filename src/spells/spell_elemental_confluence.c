#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "globals.h"
#include "tables.h"
#include "magic.h"

bool spell_elemental_confluence(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{
   CHAR_DATA *victim = (CHAR_DATA *)vo;
   int dam;
   int base = ch->class_level[CLASS_GMA];

   if (ch->class_level[CLASS_SOR] > base)
      base = ch->class_level[CLASS_SOR];
   if (ch->class_level[CLASS_WIZ] > base)
      base = ch->class_level[CLASS_WIZ];

   dam = 300 + dice(base, 25);

   act("@@p$n invokes the Convergence Codices, unleashing all four elements at once!@@N", ch, NULL,
       NULL, TO_ROOM);
   send_to_char(
       "@@pYou invoke the Convergence Codices, unleashing all four elements at once!@@N\n\r", ch);
   act("@@pFire, storm, tide, and stone converge upon $n in a devastating confluence!@@N", victim,
       NULL, NULL, TO_ROOM);
   send_to_char(
       "@@pFire, storm, tide, and stone converge upon you in a devastating confluence!@@N\n\r",
       victim);

   sp_damage(obj, ch, victim, dam, ELEMENT_FIRE | ELEMENT_AIR | ELEMENT_WATER | ELEMENT_EARTH, sn,
             TRUE);
   return TRUE;
}
