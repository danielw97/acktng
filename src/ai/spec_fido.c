#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_fido(CHAR_DATA *ch)
{
   OBJ_DATA *corpse;
   OBJ_DATA *obj;
   OBJ_DATA *obj_next;

   if (!IS_AWAKE(ch))
      return FALSE;

   for (corpse = ch->in_room->first_content; corpse != NULL; corpse = corpse->next_in_room)
   {
      if (corpse->item_type != ITEM_CORPSE_NPC || number_bits(1) != 0)
         continue;

      act("$n savagely devours a corpse.", ch, NULL, NULL, TO_ROOM);
      for (obj = corpse->first_in_carry_list; obj; obj = obj_next)
      {
         obj_next = obj->next_in_carry_list;
         obj_from_obj(obj);
         obj_to_room(obj, ch->in_room);
      }
      extract_obj(corpse);
      return TRUE;
   }

   return FALSE;
}
