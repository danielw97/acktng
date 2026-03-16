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
#include "globals.h"
#include "magic.h"
#include "skills.h"

void do_chiblock(CHAR_DATA *ch, char *argument)
{
   AFFECT_DATA af;

   if (IS_NPC(ch))
      return;

   if (!is_fighting(ch))
   {
      send_to_char("You can only prepare for a chi block when fightingg!\n\r", ch);
      return;
   }

   if (is_affected(ch, gsn_chiblock))
   {
      send_to_char("You already are prepared to perform a chi block!!\n\r", ch);
      return;
   }

   if (!can_use_skill_message(ch, gsn_chiblock))
      return;

   int cost = chi_skill_cost(5, ch->cooldown[gsn_chiblock]);

   if (get_chi(ch) < cost)
   {
      send_to_char("You do not have sufficient chi to use chiblock.\n\r", ch);
      return;
   }

   ch->chi -= cost;

   raise_skill(ch, gsn_chiblock);

   WAIT_STATE(ch, skill_table[gsn_chiblock].beats);

   ch->cooldown[gsn_chiblock] = 10;

   af.type = gsn_chiblock;
   af.duration = get_max_combo(ch) - 3;
   af.duration_type = DURATION_ROUND;
   af.location = APPLY_AC;
   af.modifier = -1;
   af.bitvector = 0;
   affect_to_char(ch, &af);

   send_to_char("You prepare to block with your chi!\n\r", ch);
   act("$n prepares to block with their chi!", ch, NULL, NULL, TO_ROOM);
}
