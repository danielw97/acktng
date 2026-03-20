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

bool spell_seal_calculus(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{
   CHAR_DATA *vch;
   AFFECT_DATA af;
   bool affected_any = FALSE;

   if (obj == NULL)
   {
      act("$n traces the Seal College's binding geometry across the floor!", ch, NULL, NULL,
          TO_ROOM);
      send_to_char("You lay the Seal College's geometric binding across the ground!\n\r", ch);
   }
   else
   {
      act("Binding geometry flows from $p across the floor!", ch, obj, NULL, TO_ROOM);
      act("Binding geometry flows from $p across the floor!", ch, obj, NULL, TO_CHAR);
   }

   for (vch = ch->in_room->first_person; vch != NULL; vch = vch->next_in_room)
   {
      if (vch == ch)
         continue;
      if (IS_NPC(ch) ? IS_NPC(vch) : !IS_NPC(vch))
         continue;
      if (saves_spell(level, vch))
         continue;

      af.type = sn;
      af.duration = level / 15 + 2;
      af.location = APPLY_SPEED;
      af.modifier = -(level / 10 + 1);
      af.bitvector = 0;
      affect_join(vch, &af);

      act("Geometric lines close around $n's feet!", vch, NULL, NULL, TO_ROOM);
      send_to_char("The Seal College's geometry closes around your feet!\n\r", vch);
      affected_any = TRUE;
   }

   if (!affected_any)
      send_to_char("The geometric binding finds no suitable targets.\n\r", ch);

   return TRUE;
}
