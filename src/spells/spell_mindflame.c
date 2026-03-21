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

bool spell_recurrence_loop(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{
   /*
    * Void Citadel recurrence technology — iterative memory flooding that
    * collapses cognitive resistance through repetition rather than direct force.
    */

   CHAR_DATA *vch;
   CHAR_DATA *vch_next;

   if (obj == NULL)
   {
      send_to_char("You initiate the recurrence loop — memory floods against them!\n\r", ch);
      act("$n's eyes go distant as $e initiates the Void Citadel's recurrence loop!", ch, NULL,
          NULL, TO_ROOM);
   }
   else
   {
      act("$p pulses as the recurrence loop floods outward from $n!", ch, obj, NULL, TO_ROOM);
      act("$p pulses as the recurrence loop floods outward!", ch, obj, NULL, TO_CHAR);
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
            act("$n staggers as the recurrence loop floods $s mind with iterating memory!", vch,
                NULL, NULL, TO_ROOM);
            send_to_char("The same memory floods through your mind again — and again — resistance "
                         "collapsing!\n\r",
                         vch);
            sp_damage(obj, ch, vch, (get_psuedo_level(ch) / 2) + dice(6, 12),
                      ELEMENT_MENTAL | NO_REFLECT | NO_ABSORB, sn, TRUE);
         }
         continue;
      }

      if (vch->in_room->area == ch->in_room->area)
         send_to_char("A distant repetitive pressure brushes the edge of your mind.\n\r", vch);
   }
   char_unreference(&vch_next);
   return TRUE;
}
