#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_janitor(CHAR_DATA *ch)
{
   OBJ_DATA *trash;

   if (!IS_AWAKE(ch))
      return FALSE;

   for (trash = ch->in_room->first_content; trash != NULL; trash = trash->next_in_room)
   {
      if (!IS_SET(trash->wear_flags, ITEM_TAKE))
         continue;
      if (trash->item_type == ITEM_TRASH || trash->cost < 10)
      {
         act("$n picks up some trash.", ch, NULL, NULL, TO_ROOM);
         obj_from_room(trash);
         obj_to_char(trash, ch);
         return TRUE;
      }
   }

   return FALSE;
}
