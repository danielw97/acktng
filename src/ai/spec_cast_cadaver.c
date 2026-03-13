#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_cast_cadaver(CHAR_DATA *ch)
{
   /*
    * Quick rip-off of spec_cast_adept, to make cadaver cast detect invis
    * * with short duration. -S-
    */

   CHAR_DATA *victim;
   /*    CHAR_DATA *vch;    */

   if (!IS_AWAKE(ch))
      return FALSE;

   for (victim = ch->in_room->first_person; victim != NULL; victim = victim->next_in_room)
      if (victim != ch && can_see(ch, victim) && number_bits(1) == 0)
         break;

   if (victim == NULL)
      return FALSE;

   /*
    * for ( vch = ch->in_room->first_person; vch != NULL; vch = vch->next )
    * {
    * OBJ_DATA *obj;
    * sh_int    count = 0;
    * bool    match = FALSE;
    *
    *
    * if ( IS_NPC( vch ) )
    * continue;
    *
    *
    * for ( obj = ch->in_room->first_content; obj != NULL; obj = obj->in_room->first_content->next )
    * {
    * if ( obj->item_type != ITEM_CORPSE_PC )
    * continue;
    * count++;
    * if (  ( !str_cmp( vch->name, obj->owner ) )
    * && ( obj->first_content )    )
    * {
    * match = TRUE;
    * break;
    * }
    * }
    * if ( match )
    * {
    * char   buf[MAX_STRING_LENGTH];
    *
    * sprintf( buf, "Try 'get all %d.corpse' to recover your belongings, %s.\n\r",
    * count, PERS( ch, vch ) );
    * do_say( ch, buf );
    * }
    * }
    */

   switch (number_bits(3))
   {
   case 0:
   case 1:
   case 2:
   case 3:
   case 4:
   case 5:
      if (!IS_AFFECTED(victim, AFF_DETECT_INVIS))
      {
         act("$n utters the word 'Sight'.", ch, NULL, NULL, TO_ROOM);
         spell_detect_invis(skill_lookup("detect invis"), 0, ch, victim, NULL);
         return TRUE;
      }
   }
   return FALSE;
}
