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

bool spell_beacon(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{
   /*
    * Create a beacon, ready for a portal to 'goto'.
    * * I owe someone thanks for this, but forgot the name... D'oh.
    * * How this works: (+ for portals, etc )
    * * a) Caster makes beacon: ownership set, obj->name = target_name
    * * b) Someone goes off with beacon, drops it
    * * c) Owner can then cast portal spell to it, beacon extracted
    * * -S-
    */

   char buf[MAX_STRING_LENGTH];
   char arg[MAX_STRING_LENGTH];
   OBJ_DATA *ob;

   one_argument(target_name, arg);

   ob = get_obj_world(ch, arg);
   if (ob != NULL)
   {
      send_to_char("There is already an object with that keyword.\n\r", ch);
      return FALSE;
   }

   ob = create_object(get_obj_index(OBJ_VNUM_BEACON), level);
   sprintf(buf, "%s", arg);
   free_string(ob->name);
   ob->name = str_dup(arg);
   sprintf(buf, "%s", ch->name);
   free_string(ob->owner);
   ob->owner = str_dup(buf);
   ob->timer = number_range(30, 45);
   obj_to_room(ob, ch->in_room);
   act("$n magically produces $p!", ch, ob, NULL, TO_ROOM);
   act("You magically produce $p!", ch, ob, NULL, TO_CHAR);
   return TRUE;
}
