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

bool spell_turn_undead(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{
   CHAR_DATA *vch;
   CHAR_DATA *vch_next;
   bool found = FALSE;

   send_to_char("You channel divine power against the undead!\n\r", ch);
   act("$n raises $s holy symbol and channels divine power!", ch, NULL, NULL, TO_ROOM);

   for (vch = ch->in_room->first_person; vch != NULL; vch = vch_next)
   {
      vch_next = vch->next_in_room;

      if (!IS_NPC(vch) || vch->alignment >= -200)
         continue;

      found = TRUE;

      if (!saves_spell(level, vch))
      {
         int dam = dice(4, level);
         sp_damage(obj, ch, vch, dam, ELEMENT_HOLY, sn, TRUE);
      }
      else
      {
         act("$n recoils from the holy power!", vch, NULL, NULL, TO_ROOM);
         send_to_char("You recoil from the holy power!\n\r", vch);
      }
   }

   if (!found)
      send_to_char("There are no evil creatures here to turn.\n\r", ch);

   return TRUE;
}
