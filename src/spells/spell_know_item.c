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

bool spell_know_item(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{
   OBJ_DATA *ob = (OBJ_DATA *)vo;
   char buf[MAX_STRING_LENGTH];
   AFFECT_DATA *paf;

   sprintf(buf, "Object '%s' is type %s, extra flags %s.\n\rWeight is %d.\n\r", ob->name,
           item_type_name(ob), extra_bit_name(ob->extra_flags), ob->weight);
   send_to_char(buf, ch);

   switch (ob->item_type)
   {
   case ITEM_SCROLL:
   case ITEM_POTION:
      sprintf(buf, "Level %d spells of:", ob->value[0]);
      send_to_char(buf, ch);

      if (ob->value[1] >= 0 && ob->value[1] < MAX_SKILL)
      {
         send_to_char(" '", ch);
         send_to_char(skill_table[ob->value[1]].name, ch);
         send_to_char("'", ch);
      }

      if (ob->value[2] >= 0 && ob->value[2] < MAX_SKILL)
      {
         send_to_char(" '", ch);
         send_to_char(skill_table[ob->value[2]].name, ch);
         send_to_char("'", ch);
      }

      if (ob->value[3] >= 0 && ob->value[3] < MAX_SKILL)
      {
         send_to_char(" '", ch);
         send_to_char(skill_table[ob->value[3]].name, ch);
         send_to_char("'", ch);
      }

      send_to_char(".\n\r", ch);
      break;

   case ITEM_STAFF:
      sprintf(buf, "Has %d(%d) charges of level %d", ob->value[1], ob->value[2], ob->value[0]);
      send_to_char(buf, ch);

      if (ob->value[3] >= 0 && ob->value[3] < MAX_SKILL)
      {
         send_to_char(" '", ch);
         send_to_char(skill_table[ob->value[3]].name, ch);
         send_to_char("'", ch);
      }

      send_to_char(".\n\r", ch);
      break;

   case ITEM_WEAPON:
      sprintf(buf, " Average damage is %d.\n\r", (ob->value[1] + ob->value[2]) / 2);
      send_to_char(buf, ch);
      break;

   case ITEM_ARMOR:
      sprintf(buf, "Armor class is %d.\n\r", ob->value[0]);
      send_to_char(buf, ch);
      break;
   }

   for (paf = ob->first_apply; paf != NULL; paf = paf->next)
   {
      if (paf->location != APPLY_NONE && paf->modifier != 0)
      {
         sprintf(buf, "Affects %s by %d.\n\r", affect_loc_name(paf->location), paf->modifier);
         send_to_char(buf, ch);
      }
   }

   return TRUE;
}
