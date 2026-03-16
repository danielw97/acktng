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

bool spell_faerie_fog(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{
   CHAR_DATA *ich;

   if (obj == NULL)
   {
      act("$n conjures a cloud of purple smoke.", ch, NULL, NULL, TO_ROOM);
      send_to_char("You conjure a cloud of purple smoke.\n\r", ch);
   }
   else
   {
      act("A cloud of purple smoke flows from $p.", ch, obj, NULL, TO_ROOM);
      act("A cloud of purple smoke flows from $p.", ch, obj, NULL, TO_ROOM);
   }
   for (ich = ch->in_room->first_person; ich != NULL; ich = ich->next_in_room)
   {
      if (!IS_NPC(ich) && IS_SET(ich->act, PLR_WIZINVIS))
         continue;

      if (ich == ch || saves_spell(level, ich))
         continue;

      affect_strip(ich, gsn_invis);
      affect_strip(ich, gsn_mass_invis);
      affect_strip(ich, gsn_sneak);
      REMOVE_BIT(ich->affected_by, AFF_HIDE);
      REMOVE_BIT(ich->affected_by, AFF_INVISIBLE);
      REMOVE_BIT(ich->affected_by, AFF_SNEAK);
      act("$n is revealed!", ich, NULL, NULL, TO_ROOM);
      send_to_char("You are revealed!\n\r", ich);
   }

   return TRUE;
}
