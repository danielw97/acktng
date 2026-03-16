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

bool spell_invis(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{
   CHAR_DATA *victim;
   OBJ_DATA *ob;
   AFFECT_DATA af;

   if (target_name[0] == '\0')
   {
      send_to_char("Make who or what invisible?\n\r", ch);
      return FALSE;
   }

   if ((victim = get_char_room(ch, target_name)) != NULL)
   {
      if (IS_AFFECTED(victim, AFF_INVISIBLE) || item_has_apply(victim, ITEM_APPLY_INV))
         return (ch == victim ? FALSE : TRUE);

      act("$n fades out of existence.", victim, NULL, NULL, TO_ROOM);
      af.type = sn;
      if (ch == victim)
         af.duration = -1;
      else
         af.duration = 4 + (level / 5);
      af.location = APPLY_NONE;
      af.modifier = 0;
      af.bitvector = AFF_INVISIBLE;
      affect_to_char(victim, &af);
      send_to_char("You fade out of existence.\n\r", victim);
      return TRUE;
   }
   else if ((ob = get_obj_carry(ch, target_name)) != NULL)
   {
      if (!IS_SET(ob->extra_flags, ITEM_INVIS))
      {
         SET_BIT(ob->extra_flags, ITEM_INVIS);
         act("$p shimmers out of visibility.", ch, ob, NULL, TO_CHAR);
         act("$p shimmers out of visibility.", ch, ob, NULL, TO_ROOM);
         return TRUE;
      }
      else
      {
         act("$p is already invisible!", ch, ob, NULL, TO_CHAR);
         return FALSE;
      }
   }
   else
   {
      send_to_char("Couldn't find that person or object.\n\r", ch);
      return FALSE;
   }
   return FALSE;
}
