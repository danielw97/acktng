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

void do_feign_death(CHAR_DATA *ch, char *argument)
{
   CHAR_DATA *mob;
   CHAR_DATA *mob_next;

   if (IS_NPC(ch))
      return;

   if (!can_use_skill(ch, gsn_feign_death))
   {
      send_to_char("You don't know how to use this skill!\n\r", ch);
      return;
   }

   WAIT_STATE(ch, skill_table[gsn_feign_death].beats);
   raise_skill(ch, gsn_feign_death);

   send_to_char("You collapse to the ground, feigning death!\n\r", ch);
   act("$n collapses to the ground!", ch, NULL, NULL, TO_ROOM);

   /* Stop mobs from fighting ch */
   for (mob = ch->in_room->first_person; mob != NULL; mob = mob_next)
   {
      mob_next = mob->next_in_room;
      if (IS_NPC(mob) && mob->fighting == ch)
      {
         stop_fighting(mob, FALSE);
      }
   }

   /* Stop ch from fighting too */
   if (is_fighting(ch))
      stop_fighting(ch, FALSE);

   ch->position = POS_SLEEPING;
}
