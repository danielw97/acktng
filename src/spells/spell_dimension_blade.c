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

bool spell_dimension_blade(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{
   OBJ_DATA *ob = (OBJ_DATA *)vo;
   AFFECT_DATA *paf;

   /*
    * Quick way to stop staff (Bash?) enchanting weapons for players
    */
   if (IS_STAFF(ch) && ch->level != 85)
   {
      send_to_char("Nothing Happens.\n\r", ch);
      return FALSE;
   }

   if (dice(4, 5) == 20)
   {
      act("$p shatters into pieces!", ch, ob, NULL, TO_CHAR);
      act("$p carried by $n shatters!", ch, ob, NULL, TO_ROOM);
      extract_obj(ob);
      return TRUE;
   }

   if (ob->item_type != ITEM_WEAPON || IS_OBJ_STAT(ob, ITEM_MAGIC) || ob->first_apply != NULL)
      return TRUE;

   GET_FREE(paf, affect_free);
   paf->type = sn;
   paf->duration = -1;
   paf->location = APPLY_HITROLL;
   paf->modifier = 1 + (level >= 50) + (level >= 60) + (level >= 70);
   paf->bitvector = 0;
   LINK(paf, ob->first_apply, ob->last_apply, next, prev);

   GET_FREE(paf, affect_free);
   paf->type = -1;
   paf->duration = -1;
   paf->location = APPLY_DAMROLL;
   paf->modifier = 1 + (level >= 55) + (level >= 70);
   paf->bitvector = 0;
   LINK(paf, ob->first_apply, ob->last_apply, next, prev);

   act("Part of $p switches into a different plane.", ch, ob, NULL, TO_CHAR);
   act("$n makes $p into a dimension blade.", ch, ob, NULL, TO_ROOM);
   return TRUE;
}
