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

bool spell_seal_room(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{
   ROOM_INDEX_DATA *room;
   ROOM_AFFECT_DATA raf;

   room = ch->in_room;

   if (room == NULL)
      return FALSE;

   if (IS_SET(room->affected_by, ROOM_BV_ENCAPS))
   {
      send_to_char("This room is already sealed!\n\r", ch);
      return FALSE;
   }
   if (IS_SET(room->area->flags, AREA_NO_ROOM_AFF))
   {
      send_to_char(
          "A strange @@eDeamon@@N appears, waggles it's finger at you, and shakes its(?) head!\n\r",
          ch);
      return FALSE;
   }
   if (deathmatch)
   {
      if (ch->hit < 150)
      {
         send_to_char("You don't have enough life force remaining!\n\r", ch);
         return FALSE;
      }
      else
      {
         send_to_char("@@eYou feel some of your life force rip from your soul!@@N\n\r", ch);
         ch->hit -= 100;
      }
   }
   act("$n spreads his hands into the air and ejects a web of energy!", ch, NULL, NULL, TO_ROOM);
   send_to_char("You spread your hands into the air and eject a web of energy!\n\r", ch);

   raf.type = sn;
   raf.level = level;
   raf.duration = (level / 20) + 2;
   raf.bitvector = ROOM_BV_ENCAPS;
   raf.caster = ch;
   affect_to_room(room, &raf);
   return TRUE;
}
