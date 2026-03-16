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

bool spell_teleport(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{
   CHAR_DATA *victim = (CHAR_DATA *)vo;
   ROOM_INDEX_DATA *pRoomIndex;

   if (deathmatch)
   {
      send_to_char("Not during a @@eDeath Match@@N!!\n\r", ch);
      return FALSE;
   }
   if (victim->in_room == NULL || IS_SET(victim->in_room->room_flags, ROOM_NO_RECALL) ||
       (!IS_NPC(ch) && victim->fighting != NULL) ||
       (victim != ch && (saves_spell(level, victim) || saves_spell(level, victim))))
   {
      send_to_char("You failed.\n\r", ch);
      return TRUE;
   }

   for (;;)
   {
      pRoomIndex = get_room_index(number_range(0, MAX_VNUM));
      if (pRoomIndex == NULL)
         continue;
      if (!IS_SET(pRoomIndex->room_flags, ROOM_PRIVATE) &&
          !IS_SET(pRoomIndex->room_flags, ROOM_SOLITARY) &&
          IS_SET(pRoomIndex->area->flags, AREA_TELEPORT))
         break;
   }

   act("$n slowly fades out of existence.", victim, NULL, NULL, TO_ROOM);
   char_from_room(victim);
   char_to_room(victim, pRoomIndex);
   act("$n slowly fades into existence.", victim, NULL, NULL, TO_ROOM);
   do_look(victim, "auto");
   return TRUE;
}
