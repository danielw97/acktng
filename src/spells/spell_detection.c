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

bool spell_detection(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{
   char buf[MAX_INPUT_LENGTH];
   OBJ_DATA *ob;
   OBJ_DATA *in_obj;
   bool found;

   found = FALSE;
   for (ob = first_obj; ob != NULL; ob = ob->next)
   {

      if (!can_see_obj(ch, ob) || !is_name(target_name, ob->name) ||
          IS_SET(ob->extra_flags, ITEM_RARE) || (ob->item_type == ITEM_PIECE) ||
          (IS_SET(ob->extra_flags, ITEM_UNIQUE)) || (!str_prefix(target_name, "unique")))
         continue;

      for (in_obj = ob; in_obj->in_obj != NULL; in_obj = in_obj->in_obj)
         ;
      if ((in_obj->carried_by != NULL) && IS_STAFF(in_obj->carried_by))
         break;

      if (in_obj->carried_by != NULL)
      {
         found = TRUE;
         sprintf(buf, "%s carried by %s.\n\r", ob->short_descr,
                 can_see(ch, in_obj->carried_by) ? PERS(in_obj->carried_by, ch) : "someone");
      }
      else
      {
         found = TRUE;
         sprintf(buf, "%s in %s.\n\r", ob->short_descr,
                 in_obj->in_room == NULL ? "somewhere" : in_obj->in_room->name);
      }

      buf[0] = UPPER(buf[0]);
      send_to_char(buf, ch);
   }

   if (!found)
      send_to_char("You fail to detect any such object.\n\r", ch);

   return TRUE;
}
