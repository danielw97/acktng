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

bool spell_visit(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{

   /*
    * Psi spell.  Kinda like summon, but almost done in reverse.
    * * allows the Psi to try and transfer themself to the 'victim'
    * * --Stephen
    */

   CHAR_DATA *victim = (CHAR_DATA *)vo;

   if (deathmatch)
   {
      send_to_char("Not during a @@eDeath Match@@N!!\n\r", ch);
      return FALSE;
   }
   if ((victim = get_char_world(ch, target_name)) == NULL || victim == ch || IS_NPC(victim) ||
       victim->in_room == NULL || IS_SET(victim->in_room->room_flags, ROOM_PRIVATE) ||
       IS_SET(victim->in_room->room_flags, ROOM_SOLITARY) ||
       IS_SET(victim->in_room->room_flags, ROOM_SAFE) ||
       IS_SET(ch->in_room->room_flags, ROOM_NO_RECALL) ||
       !IS_SET(victim->in_room->area->flags, AREA_TELEPORT))
   {
      send_to_char("You failed.\n\r", ch);
      return TRUE;
   }

   if (IS_SET(victim->act, PLR_NOVISIT))
   {
      send_to_char("You seem unable to visit your target!\n\r", ch);
      return TRUE;
   }

   /*
    * Check is ch screws up, and ends up in limbo... <grin>
    */

   if (number_percent() < 25) /* 25% chance */
   {
      send_to_char("You get distracted, and appear in the middle of nowhere!\n\r", ch);
      act("$n disappears suddenly.", ch, NULL, NULL, TO_ROOM);
      char_from_room(ch);
      char_to_room(ch, get_room_index(ROOM_VNUM_LIMBO));
      act("$n arrives suddenly.", ch, NULL, NULL, TO_ROOM);
      do_look(ch, "auto");
      return TRUE;
   }

   act("$n disappears suddenly.", ch, NULL, NULL, TO_ROOM);
   char_from_room(ch);
   char_to_room(ch, victim->in_room);
   act("$n arrives suddenly.", ch, NULL, NULL, TO_ROOM);
   send_to_char("You change locations!\n\r", ch);
   do_look(ch, "auto");
   return TRUE;
}
