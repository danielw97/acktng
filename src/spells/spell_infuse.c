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

bool spell_infuse(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{
   OBJ_DATA *ob = (OBJ_DATA *)vo;
   AFFECT_DATA *paf;
   OBJ_DATA *obj_soul;
   if (IS_NPC(ch))
      return FALSE;

   /*
    * Quick way to stop imms (Bash?) enchanting weapons for players
    */
   if (IS_IMMORTAL(ch) && ch->level != 85)
   {
      send_to_char("Nothing Happens.\n\r", ch);
      return FALSE;
   }
   if (IS_NPC(ch))
      return FALSE;
   if ((obj_soul = get_eq_char(ch, WEAR_HOLD_HAND_L)) == NULL)
      if ((obj_soul = get_eq_char(ch, WEAR_HOLD_HAND_R)) == NULL)
      {
         send_to_char("@@NYou must be holding a @@eSoul@@N to cast this spell!!\n\r", ch);
         return FALSE;
      }
   if (obj_soul->item_type != ITEM_SOUL)
   {
      send_to_char("@@NYou are not holding a @@eSoul@@N!!\n\r", ch);
      return FALSE;
   }

   if (ob->item_type != ITEM_WEAPON)
   {
      send_to_char("That is not a weapon!.\n\r", ch);
      return FALSE;
   }

   for (paf = ob->first_apply; paf != NULL; paf = paf->next)
   {
      if (paf->type == sn)
      {
         send_to_char("That weapon is already infused with a soul!.\n\r", ch);
         return FALSE;
      }
   }
   if (ch->pcdata->hp_from_gain < 100)
   {
      send_to_char("You have exhausted your life force, and are unable to"
                   " control the necromantic forces necessary for this act.\n\r",
                   ch);
      return FALSE;
   }
   GET_FREE(paf, affect_free);
   paf->type = sn;
   paf->duration = -1;
   paf->location = APPLY_DAMROLL;
   paf->modifier = (obj_soul->level / 8);
   paf->bitvector = 0;
   paf->caster = NULL;
   LINK(paf, ob->first_apply, ob->last_apply, next, prev);

   GET_FREE(paf, affect_free);
   paf->type = sn;
   paf->duration = -1;
   paf->location = APPLY_AC;
   paf->modifier = -(obj_soul->level / 2);
   paf->bitvector = 0;
   paf->caster = NULL;
   LINK(paf, ob->first_apply, ob->last_apply, next, prev);

   GET_FREE(paf, affect_free);
   paf->type = sn;
   paf->duration = -1;
   paf->location = APPLY_MANA;
   paf->modifier = obj_soul->level / 3;
   paf->bitvector = 0;
   paf->caster = NULL;
   LINK(paf, ob->first_apply, ob->last_apply, next, prev);

   GET_FREE(paf, affect_free);
   paf->type = sn;
   paf->duration = -1;
   paf->location = APPLY_HITROLL;
   paf->modifier = (obj_soul->level / 8);
   paf->bitvector = 0;
   paf->caster = NULL;
   LINK(paf, ob->first_apply, ob->last_apply, next, prev);

   SET_BIT(ob->extra_flags, ITEM_NODISARM);
   SET_BIT(ob->extra_flags, ITEM_NODROP);
   SET_BIT(ob->extra_flags, ITEM_NO_AUCTION);
   SET_BIT(ob->extra_flags, ITEM_NOSAC);
   SET_BIT(ob->extra_flags, ITEM_NOLOOT);
   SET_BIT(ob->extra_flags, ITEM_NOLOOT);

   ob->obj_fun = obj_fun_lookup("objfun_infused_soul");
   extract_obj(obj_soul);

   act("@@N$n gestures diabolically, and his captured @@esoul@@N is infused into his $P.", ch, NULL,
       ob, TO_ROOM);
   send_to_char("@@NYou condense the soul and some of your life force into your weapon.\n\r", ch);
   do_save(ch, "");
   return TRUE;
}
