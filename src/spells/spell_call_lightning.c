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

bool spell_call_lightning(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{
   CHAR_DATA *vch;
   CHAR_DATA *vch_next;
   int dam;

   if (!IS_OUTSIDE(ch))
   {
      send_to_char("You must be out of doors.\n\r", ch);
      return FALSE;
   }

   if (weather_info.sky < SKY_RAINING)
   {
      send_to_char("You need bad weather.\n\r", ch);
      return FALSE;
   }

   dam = dice(level, 8);

   if (obj == NULL)
   {
      act("$n calls God's lightning to strike $s foes!", ch, NULL, NULL, TO_ROOM);
      send_to_char("God's lightning strikes your foes!\n\r", ch);
   }
   else
   {
      act("$p summons lightning to strike $n's foes!", ch, obj, NULL, TO_ROOM);
      act("$p summons lightning to strike your foes!", ch, obj, NULL, TO_CHAR);
   }

   aoe_damage(ch, obj, sn, level, dam, dam, ELEMENT_AIR | NO_REFLECT | NO_ABSORB, AOE_SAVES);

   /* Notify others in the same area who are outdoors. */
   CREF(vch_next, CHAR_NEXTROOM);
   for (vch = first_char; vch != NULL; vch = vch_next)
   {
      vch_next = vch->next;
      if (vch->in_room == NULL || vch->in_room == ch->in_room)
         continue;
      if (vch->in_room->area == ch->in_room->area && IS_OUTSIDE(vch) && IS_AWAKE(vch))
         send_to_char("Lightning flashes in the sky.\n\r", vch);
   }
   CUREF(vch_next);
   return TRUE;
}
