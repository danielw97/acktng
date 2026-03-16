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

bool spell_influx(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
/* -- Stephen */
{
   CHAR_DATA *vch;

   if (obj == NULL)
   {
      send_to_char("You fill the room with healing energy!\n\r", ch);
      act("$n spreads $s arms and heals the room!", ch, NULL, NULL, TO_ROOM);
   }
   else
   {
      act("$p glows blindingly bright in $n's hand!", ch, obj, NULL, TO_ROOM);
      act("$p glows blindingly in your hand!", ch, obj, NULL, TO_CHAR);
   }
   /*
    * need to do this on everyone!
    */
   /*
    * for loop taken from spell_earthquake
    */
   /*
    * Dont need to do on everyone.. earthquake does because it needs to
    * send messages to people in other rooms as well.. (whole area sees
    * "the earth trembles and shivers").. -- Alty
    */

   for (vch = ch->in_room->first_person; vch; vch = vch->next_in_room)
   {
      vch->hit = UMIN(vch->hit + 40 + dice(20, level / 10), get_max_hp(vch));
      update_pos(vch);
   }

   /*    for ( vch=first_char; vch != NULL; vch = vch->next )
       {

      if ( vch->in_room == NULL )
          continue;
      if ( vch->in_room == ch->in_room )
      {
          CHAR_DATA *victim = (CHAR_DATA *) vo;
          victim->hit = UMIN( victim->hit + 40, victim-> );
          update_pos( victim );
      }
       }*/
   return TRUE;
}
