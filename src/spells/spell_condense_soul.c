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

bool spell_condense_soul(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{
   OBJ_DATA *ob = (OBJ_DATA *)vo;

   if (IS_NPC(ch))
      return FALSE;

   if (ob->item_type != ITEM_SOUL)
   {
      send_to_char("That is not a soul!.\n\r", ch);
      return FALSE;
   }

   if (ob->level <= MAX_LEVEL)
   {
      send_to_char("Bah!! That soul is to weak to use!!\n\r", ch);
      return FALSE;
   }
   extract_obj(ob);
   ob = create_object(get_obj_index(OBJ_VNUM_SOUL_POTION), level);
   obj_to_char(ob, ch);

   act("@@N$n gestures diabolically, and his captured soul condenses into a@@dSoul Potion@@N.", ch,
       NULL, NULL, TO_ROOM);
   send_to_char(
       "@@NYou condense the soul and some of your life force into a @@dSoul potion@@N.\n\r", ch);

   return TRUE;
}
