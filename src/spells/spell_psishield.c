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

bool spell_psishield(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{
   MAGIC_SHIELD *shield;
   AFFECT_DATA af;
   bool char_login = FALSE;

   if ((!IS_NPC(ch)) && (ch->desc != NULL) && (ch->desc->connected == CON_SETTING_STATS))
      char_login = TRUE;

   if (ch->first_shield != NULL)
   {
      return FALSE;
   }

   af.type = sn;
   af.duration = get_psuedo_level(ch) / 10;
   if (char_login)
      af.duration /= 2;
   af.location = 0;
   af.modifier = 0;
   af.bitvector = 0;
   affect_to_char(ch, &af);

   GET_FREE(shield, shield_free);
   shield->name = str_dup("@@mPSI@@N");
   shield->type = PSI_SHIELD;
   shield->harmfull = TRUE;
   shield->attack_dam = number_range((level * 3), (level * 5));
   shield->percent = 10 + (get_psuedo_level(ch) / 10);
   shield->hits = 5000 + (get_psuedo_level(ch) * 10);
   shield->sn = sn;
   if (char_login)
      shield->hits /= (number_range(2, 10));

   shield->absorb_message_room =
       str_dup("@@N$n's @@mshield@@N pulses and assaults $N's @@mmind@@N!!");
   shield->absorb_message_victim =
       str_dup("@@N$N's @@mshield@@N pulses, and assaults your @@mmind@@N!!");
   shield->absorb_message_self =
       str_dup("@@NYour @@mshield@@N pulses, and assaults $N's @@mmind@@N!!!");
   shield->wearoff_room = str_dup("@@N$n's @@mshield@@N @@yDISSIPATES@@N!!!!!");
   shield->wearoff_self = str_dup("@@NYour @@mshield@@N @@yDISSIPATES@@N!!!!!");

   LINK(shield, ch->first_shield, ch->last_shield, next, prev);

   send_to_char("@@NYou are surrounded by a @@mpsionic field@@N!\n\r", ch);
   act("@@N$n is surrounded by a @@mpsionic field@@N!", ch, NULL, NULL, TO_ROOM);

   return TRUE;
}
