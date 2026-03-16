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

bool spell_window(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{
   /*
    * Create a magic window to look into another room
    */
   OBJ_DATA *beacon;
   OBJ_DATA *ob;

   if (target_name[0] == '\0')
   {
      send_to_char("Form a window to whom?\n\r", ch);
      return FALSE;
   }

   beacon = get_obj_world(ch, target_name);

   if (beacon == NULL || beacon->item_type != ITEM_BEACON)
   {
      send_to_char("Couldn't find target.  Sorry.\n\r", ch);
      return FALSE;
   }

   if (beacon->in_room == NULL)
   {
      send_to_char("It seems that someone is carrying it.\n\r", ch);
      return FALSE;
   }

   if (str_cmp(beacon->owner, ch->name))
   {
      send_to_char("That's not one of YOUR beacons!\n\r", ch);
      return FALSE;
   }

   act("$n creates a magic window with a wave of $s hand.", ch, NULL, NULL, TO_ROOM);
   send_to_char("You create a magic window with a wave of your hand.\n\r", ch);

   ob = create_object(get_obj_index(OBJ_VNUM_WINDOW), level);
   obj_to_room(ob, ch->in_room);
   ob->timer = 1 + (level / 30);
   ob->value[0] = beacon->in_room->vnum;
   ob->value[1] = 1;
   send_to_room("The beacon suddenly vanishes!\n\r", beacon->in_room);
   extract_obj(beacon);
   return TRUE;
}
