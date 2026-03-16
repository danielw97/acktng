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

bool spell_animate(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{
   OBJ_DATA *ob;
   OBJ_DATA *bits;
   CHAR_DATA *corpse;
   SPEC_FUN *summon_special = spec_lookup("spec_summon_animate");

   if (summon_special == NULL)
   {
      bug("Spell_animate: missing spec_summon_animate.", 0);
      send_to_char("Necromantic energies waver and fail.\n\r", ch);
      return FALSE;
   }

   if (IS_NPC(ch))
      return FALSE;

   ob = get_obj_here(ch, target_name);

   if (ob == NULL)
   {
      send_to_char("Couldn't find it.\n\r", ch);
      return FALSE;
   }

   if (ob->item_type != ITEM_CORPSE_NPC)
   {
      send_to_char("You can't animate that!\n\r", ch);
      return FALSE;
   }

   act("$n lays $s hands onto the $p!", ch, ob, NULL, TO_ROOM);
   act("You lay your hands upon the $p.", ch, ob, NULL, TO_CHAR);

   act("Bright bolts of lightning fly out from $p!!", ch, ob, NULL, TO_ROOM);
   act("Bright bolts of lightning fly out from $p!!", ch, ob, NULL, TO_CHAR);

   corpse = create_mobile(get_mob_index(MOB_VNUM_ZOMBIE));
   char_to_room(corpse, ch->in_room);
   act("$n stands up, and stretches slowly.", corpse, NULL, NULL, TO_ROOM);

   if (ob->item_type == ITEM_CORPSE_NPC)
   {
      SET_BIT(corpse->act, ACT_UNDEAD);
      act("$n's eyes glow black!", corpse, NULL, NULL, TO_ROOM);
   }

   corpse->level = ob->level; /* Level of (N)PC before death */
   corpse->hit = get_max_hp(corpse);
   corpse->exp = 0;
   corpse->intell_exp = 0;
   corpse->max_mana = corpse->level * 25;
   corpse->mana = corpse->max_mana;
   corpse->spec_fun = summon_special;

   for (;;)
   {
      if (ob->first_in_carry_list == NULL)
         break;
      bits = ob->first_in_carry_list;
      obj_from_obj(bits);
      obj_to_char(bits, corpse);
   }

   extract_obj(ob);
   do_wear(corpse, "all"); /* FIXME: better to check items, then wear... */
   SET_BIT(corpse->act, ACT_PET);
   SET_BIT(corpse->affected_by, AFF_CHARM);
   corpse->extract_timer = get_psuedo_level(ch) / 4;

   add_follower(corpse, ch);
   return TRUE;
}
