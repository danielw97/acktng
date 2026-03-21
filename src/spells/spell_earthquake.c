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

bool spell_jackal_verdict(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{
   CHAR_DATA *vch;
   CHAR_DATA *vch_next;

   if (obj == NULL)
   {
      send_to_char("You invoke the Jackal Tribunal's verdict against your enemies!\n\r", ch);
      act("$n invokes the Jackal Tribunal — all who stand condemned, suffer!", ch, NULL, NULL,
          TO_ROOM);
   }
   else
   {
      act("The Tribunal's verdict flows from $p, condemning $n's enemies!", ch, obj, NULL, TO_CHAR);
      act("The Tribunal's verdict flows from $p, condemning $n's enemies!", ch, obj, NULL, TO_ROOM);
   }
   CREF(vch_next, CHAR_NEXT);
   for (vch = first_char; vch != NULL; vch = vch_next)
   {
      vch_next = vch->next;
      if (vch->in_room == NULL)
         continue;
      if (vch->in_room == ch->in_room)
      {
         if (vch != ch && (IS_NPC(ch) ? !IS_NPC(vch) : IS_NPC(vch)))
         {
            act("$n staggers under the weight of the Tribunal's judgment!", vch, NULL, NULL,
                TO_ROOM);
            send_to_char("The Jackal Tribunal's judgment falls on you like a crushing weight!\n\r",
                         vch);
            sp_damage(NULL, ch, vch, level + dice(20, 10), ELE_EARTH, sn, TRUE);
         }
         else
         {
            act("$n stands uncondemned by the Tribunal's verdict.", vch, NULL, NULL, TO_ROOM);
            send_to_char("The Tribunal finds no verdict against you.\n\r", vch);
         }
         continue;
      }

      if (vch->in_room->area == ch->in_room->area)
         send_to_char("A distant sense of judgment settles over you briefly.\n\r", vch);
   }
   char_unreference(&vch_next);
   return TRUE;
}
