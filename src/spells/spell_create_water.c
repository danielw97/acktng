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

bool spell_create_water(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{
   OBJ_DATA *ob = (OBJ_DATA *)vo;
   int water;

   if (ob->item_type != ITEM_DRINK_CON)
   {
      send_to_char("It is unable to hold water.\n\r", ch);
      return FALSE;
   }

   if (ob->value[2] != LIQ_WATER && ob->value[1] != 0)
   {
      send_to_char("It contains some other liquid.\n\r", ch);
      return FALSE;
   }

   water = UMIN(level * (weather_info.sky >= SKY_RAINING ? 4 : 2), ob->value[0] - ob->value[1]);

   if (water > 0)
   {
      ob->value[2] = LIQ_WATER;
      ob->value[1] += water;
      if (!is_name("water", ob->name))
      {
         char buf[MAX_STRING_LENGTH];
         sprintf(buf, "%s water", ob->name);
         free_string(ob->name);
         ob->name = str_dup(buf);
      }
      act("$p is filled.", ch, ob, NULL, TO_CHAR);
   }

   return TRUE;
}
