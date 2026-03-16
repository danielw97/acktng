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

bool spell_calm(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{
   CHAR_DATA *ppl;
   int chance;

   for (ppl = ch->in_room->first_person; ppl != NULL; ppl = ppl->next_in_room)
      if (IS_NPC(ppl) && ppl != ch)
      {
         chance = ((IS_NPC(ch) ? 50 : ch->pcdata->learned[sn] / 2) + (5 * (level - ppl->level)));
         if (number_percent() < chance)
            stop_fighting(ppl, TRUE);
      }
   if (obj == NULL)
   {
      act("$n emits a great feeling of calm around you.", ch, NULL, NULL, TO_ROOM);
      send_to_char("You emit a great feeling of calm around you.\n\r", ch);
   }
   else
   {
      act("$p glows with a clam light.", ch, obj, NULL, TO_ROOM);
      act("$p glows with a clam light.", ch, obj, NULL, TO_CHAR);
   }
   return TRUE;
}
