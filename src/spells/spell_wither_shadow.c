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
#include "skills.h"

bool spell_wither_shadow(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{
   ROOM_INDEX_DATA *room;
   ROOM_AFFECT_DATA raf;

   room = ch->in_room;

   if (room == NULL)
      return FALSE;

   if (IS_SET(room->affected_by, ROOM_BV_HEAL_STEAL))
   {
      send_to_char("@@NThere is already a @@dWithering Shadow@@N operating here!\n\r", ch);
      return FALSE;
   }

   if (IS_SET(room->area->flags, AREA_NO_ROOM_AFF))
   {
      send_to_char(
          "A strange @@eDeamon@@N appears, waggles it's finger at you, and shakes its(?) head!\n\r",
          ch);
      return FALSE;
   }

   int combo_count = get_combo_count(ch);
   int combo_bonus = combo_count > 0 ? combo_count : 0;

   if (combo_count > 0)
      reset_combo(ch);

   act("@@NA deadly @@dWithering Shadow@@N fills the room.", ch, NULL, NULL, TO_ROOM);
   send_to_char("@@NYou fill the room with a deadly @@dWithering Shadow@@N.\n\r", ch);

   if (combo_bonus > 0)
      send_to_char("@@yYour combo intensifies the shadow!@@N\n\r", ch);

   raf.type = sn;
   raf.duration = (level / 8) + number_range(2, 10) + combo_bonus;
   raf.level = level;
   raf.bitvector = ROOM_BV_HEAL_STEAL;
   raf.caster = ch;
   raf.modifier = 0;

   affect_to_room(room, &raf);
   return TRUE;
}
