/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *                                                                         *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  Ack 2.2 improvements copyright (C) 1994 by Stephen Dooley              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc       *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.                                               *
 *                                                                         *
 *       _/          _/_/_/     _/    _/     _/    ACK! MUD is modified    *
 *      _/_/        _/          _/  _/       _/    Merc2.0/2.1/2.2 code    *
 *     _/  _/      _/           _/_/         _/    (c)Stephen Zepp 1998    *
 *    _/_/_/_/      _/          _/  _/             Version #: 4.3          *
 *   _/      _/      _/_/_/     _/    _/     _/                            *
 *                                                                         *
 *                        http://ackmud.nuc.net/                           *
 *                        zenithar@ackmud.nuc.net                          *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "globals.h"
#include "tables.h"
#include "magic.h"

bool spell_retri_strike(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{
   CHAR_DATA *vch;
   CHAR_DATA *vch_next;
   OBJ_DATA *staff_obj = NULL;

   if ((staff_obj = get_eq_char(ch, WEAR_HOLD_HAND_R)) == NULL)
      if ((staff_obj = get_eq_char(ch, WEAR_HOLD_HAND_L)) == NULL)
      {
         send_to_char("You must be holding a @@rstaff@@N for this spell!\n\r", ch);
         return FALSE;
      }

   if (staff_obj->item_type != ITEM_STAFF)
   {
      send_to_char("That is not a staff you are holding, you fool!!\n\r", ch);
      return FALSE;
   }

   if (obj == NULL)
   {
      send_to_char("The earth trembles beneath your feet!\n\r", ch);
      act("$n makes the earth tremble and shiver.", ch, NULL, NULL, TO_ROOM);
   }
   else
   {
      act("$p vibrates violently, making the earth tremble!", ch, obj, NULL, TO_CHAR);
      act("$p vibrates violenty, making the earth around $n tremble!", ch, obj, NULL, TO_ROOM);
   }
   CREF(vch_next, CHAR_NEXT);

   for (vch = first_char; vch != NULL; vch = vch_next)
   {
      vch_next = vch->next;
      if (vch->in_room == NULL)
         continue;
      if (vch->in_room == ch->in_room)
      {
         if (vch != ch)
         {
            OBJ_DATA *heated_item = NULL;

            act("@@W$n @@Wis @@eimmolated@@N by the fury released from the $p@@W, and falls to the "
                "ground!",
                vch, staff_obj, NULL, TO_ROOM);
            act("@@WYou are @@eimmolated@@N by the fury released from the $p@@W, and fall to the "
                "ground!",
                vch, staff_obj, NULL, TO_CHAR);
            if (!sp_damage(obj, ch, vch, number_range(staff_obj->level * 6, staff_obj->level * 10),
                           ELEMENT_HOLY, sn, FALSE))
               continue;
            for (heated_item = vch->first_carry; heated_item != NULL;
                 heated_item = heated_item->next_in_carry_list)
            {
               if (heated_item->wear_loc == WEAR_NONE)
                  continue;
               if (number_range(0, 100) < 25)
                  continue;
               else
                  break;
            }
         }
         continue;
      }
      else
      {
         act("@@W$n @@Wis protected by the fury of the @@W$p@@W.", vch, staff_obj, NULL, TO_ROOM);
         continue;
      }
      if (vch->in_room->area == ch->in_room->area)
         send_to_char("@@WSuddenly, a @@ybright flash@@W sears your eyes, then is gone.@@N\n\r",
                      vch);
   }

   CUREF(vch_next);
   extract_obj(staff_obj);
   return TRUE;
}
