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

void do_spinfist(CHAR_DATA *ch, char *argument)
{
   CHAR_DATA *vch;
   CHAR_DATA *vch_next;

   if (!can_use_skill_message(ch, gsn_spinfist))
      return;

   int cost = chi_skill_cost(5, ch->cooldown[gsn_spinfist]);

   if (get_chi(ch) < cost)
   {
      send_to_char("You do not have sufficient chi to use spinfist.\n\r", ch);
      return;
   }

   act("You perform a spinfist!", ch, NULL, NULL, TO_CHAR);
   act("$n performs a spinfist!", ch, NULL, NULL, TO_ROOM);
   for (vch = ch->in_room->first_person; vch != NULL; vch = vch_next)
   {
      vch_next = vch->next_in_room;
      if (vch->in_room == NULL)
         continue;

      if (vch->in_room == ch->in_room)
      {
         if (vch != ch && (vch->in_room == ch->in_room) &&
             (IS_NPC(ch) ? !IS_NPC(vch) : IS_NPC(vch)) && (vch->master != ch) &&
             (!is_same_group(ch, vch)))
         {
            if (can_hit_skill(ch, vch, gsn_spinfist))
               war_attack(ch, vch->name, gsn_spinfist);
         }
      }
   }

   ch->cooldown[gsn_spinfist] = 10;

   return;
}
