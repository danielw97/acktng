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

bool spell_dispel_object(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{
   if (obj != NULL || (obj = get_obj_carry(ch, target_name)) != NULL)
   {
      /*
       * NOTE: Must also remove ALL affects, otherwise players
       * * will be able to enchant, dispel, enchant.... -S-
       * * No they wouldnt, enchant has a check for obj->first_apply.
       * * -- Alty
       */

      if (IS_SET(obj->extra_flags, ITEM_EVIL))
      {
         REMOVE_BIT(obj->extra_flags, ITEM_EVIL);
         act("$p looks less evil.", ch, obj, NULL, TO_CHAR);
         act("$p looks less evil.", ch, obj, NULL, TO_ROOM);
      }
      if (IS_SET(obj->extra_flags, ITEM_NODROP))
         REMOVE_BIT(obj->extra_flags, ITEM_NODROP);

      if (IS_SET(obj->extra_flags, ITEM_INVIS))
      {
         REMOVE_BIT(obj->extra_flags, ITEM_INVIS);
         act("$p fades back into view.", ch, obj, NULL, TO_CHAR);
         act("$p fades back into view.", ch, obj, NULL, TO_ROOM);
      }
      if (IS_SET(obj->extra_flags, ITEM_MAGIC))
      {
         REMOVE_BIT(obj->extra_flags, ITEM_MAGIC);
         act("$p looks less magical.", ch, obj, NULL, TO_CHAR);
         act("$p looks less magical.", ch, obj, NULL, TO_ROOM);
      }
      if (IS_SET(obj->extra_flags, ITEM_BLESS))
      {
         REMOVE_BIT(obj->extra_flags, ITEM_BLESS);
         act("$p looks less Holy.", ch, obj, NULL, TO_CHAR);
         act("$p looks less Holy.", ch, obj, NULL, TO_ROOM);
      }
      if (IS_SET(obj->extra_flags, ITEM_NOREMOVE))
         REMOVE_BIT(obj->extra_flags, ITEM_NOREMOVE);

      obj->first_apply = NULL;
      obj->last_apply = NULL;

      return TRUE;
   }
   send_to_char("Dispel what??\n\r", ch);
   return FALSE;
}
