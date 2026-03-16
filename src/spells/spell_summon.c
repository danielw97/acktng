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

bool spell_summon(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{
   CHAR_DATA *victim;
   char buf[MAX_STRING_LENGTH];

   if ((victim = get_char_world(ch, target_name)) == NULL || victim == ch ||
       victim->in_room == NULL || IS_SET(victim->in_room->room_flags, ROOM_SAFE) ||
       IS_SET(victim->in_room->room_flags, ROOM_NO_RECALL) ||
       !(IS_SET(victim->in_room->area->flags, AREA_TELEPORT)) || victim->level >= level + 10 ||
       victim->fighting != NULL || (IS_NPC(victim) && saves_spell(level, victim)) ||
       room_is_private(ch->in_room) || (ch->in_room->area->max_level < (victim->level - 15)))
   {
      send_to_char("You failed.\n\r", ch);
      return TRUE;
   }
   if ((IS_SET(victim->act, PLR_NOSUMMON)) || (IS_NPC(victim) && (victim->level > (level - 21))))
   {
      send_to_char("You seemed unable to snatch your victim!\n\r", ch);
      send_to_char("You feel a slight tugging sensation.\n\r", victim);
      return TRUE;
   }

   act("$n disappears suddenly.", victim, NULL, NULL, TO_ROOM);
   char_from_room(victim);
   char_to_room(victim, ch->in_room);
   act("$n arrives suddenly.", victim, NULL, NULL, TO_ROOM);
   sprintf(buf, "%s has summoned you!!", ch->name);
   send_to_char(buf, victim);
   do_look(victim, "auto");
   return TRUE;
}
