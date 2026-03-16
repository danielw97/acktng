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

bool spell_word_of_recall(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{
   CHAR_DATA *victim = (CHAR_DATA *)vo;
   ROOM_INDEX_DATA *location;

   act("$n requests Holy transport!", victim, NULL, NULL, TO_ROOM);
   if (IS_AFFECTED(victim, AFF_CURSE))
   {
      send_to_char("The Gods do not transport cursed players!\n\r", ch);
      return FALSE;
   }

   if (IS_NPC(victim) || victim->level < 6)
      location = get_room_index(ROOM_VNUM_TEMPLE);
   else
      location = get_room_index(race_table[victim->race].recall);

   if (location == NULL)
   {
      send_to_char("You are completely lost.\n\r", victim);
      return FALSE;
   }

   if (victim->in_room == location)
      return FALSE;

   if (IS_SET(victim->in_room->room_flags, ROOM_NO_RECALL))
   {
      send_to_char("Some strange force prevents your transport.\n\r", victim);
      return TRUE;
   }

   if (victim->fighting != NULL)
      stop_fighting(victim, TRUE);

   act("$n is engulfed by a stream of blue light!", victim, NULL, NULL, TO_ROOM);
   char_from_room(victim);
   char_to_room(victim, location);
   act("$n arrives, carried by a blue stream of light!", victim, NULL, NULL, TO_ROOM);
   do_look(victim, "auto");
   return TRUE;
}
