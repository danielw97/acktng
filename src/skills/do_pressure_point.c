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

void do_pressure_point(CHAR_DATA *ch, char *argument)
{
   CHAR_DATA *victim;
   AFFECT_DATA af;
   int choice;

   if (IS_NPC(ch))
      return;

   if (!can_use_skill(ch, gsn_pressure_point))
   {
      send_to_char("You don't know how to use this skill!\n\r", ch);
      return;
   }

   if (!is_fighting(ch) || (victim = ch->fighting) == NULL)
   {
      send_to_char("You must be fighting someone!\n\r", ch);
      return;
   }

   WAIT_STATE(ch, skill_table[gsn_pressure_point].beats);
   raise_skill(ch, gsn_pressure_point);

   choice = number_range(0, 2);

   af.type = gsn_pressure_point;
   af.duration = 3;
   af.bitvector = 0;

   if (choice == 0)
   {
      af.location = APPLY_STR;
      af.modifier = -(ch->level / 6);
      affect_to_char(victim, &af);
      act("You strike a nerve cluster in $N's arm!", ch, NULL, victim, TO_CHAR);
      act("$n strikes a nerve cluster in $N's arm!", ch, NULL, victim, TO_NOTVICT);
      act("$n strikes a nerve cluster in your arm!", ch, NULL, victim, TO_VICT);
   }
   else if (choice == 1)
   {
      af.location = APPLY_DEX;
      af.modifier = -(ch->level / 6);
      affect_to_char(victim, &af);
      act("You strike a nerve cluster in $N's leg!", ch, NULL, victim, TO_CHAR);
      act("$n strikes a nerve cluster in $N's leg!", ch, NULL, victim, TO_NOTVICT);
      act("$n strikes a nerve cluster in your leg!", ch, NULL, victim, TO_VICT);
   }
   else
   {
      af.location = APPLY_DAMROLL;
      af.modifier = -(ch->level / 5);
      affect_to_char(victim, &af);
      act("You strike the nerve cluster controlling $N's strike force!", ch, NULL, victim, TO_CHAR);
      act("$n strikes the nerve cluster controlling $N's strike force!", ch, NULL, victim,
          TO_NOTVICT);
      act("$n strikes the nerve cluster controlling your strike force!", ch, NULL, victim, TO_VICT);
   }

   /* Deal small damage */
   damage(ch, victim, dice(1, ch->level / 4 + 1), gsn_pressure_point);
}
