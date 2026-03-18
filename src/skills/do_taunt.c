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

void do_taunt(CHAR_DATA *ch, char *argument)
{
   char arg[MSL];
   CHAR_DATA *victim;
   AFFECT_DATA af;

   if (IS_NPC(ch))
      return;

   if (!can_use_skill(ch, gsn_taunt))
   {
      send_to_char("You don't know how to use this skill!\n\r", ch);
      return;
   }

   one_argument(argument, arg);

   if (arg[0] == '\0')
   {
      send_to_char("Taunt whom?\n\r", ch);
      return;
   }

   if ((victim = get_char_room(ch, arg)) == NULL)
   {
      send_to_char("They aren't here.\n\r", ch);
      return;
   }

   if (victim == ch)
   {
      send_to_char("You taunt yourself. Very impressive.\n\r", ch);
      return;
   }

   if (is_safe(ch, victim))
      return;

   WAIT_STATE(ch, skill_table[gsn_taunt].beats);
   raise_skill(ch, gsn_taunt);

   act("You taunt $N with insults!", ch, NULL, victim, TO_CHAR);
   act("$n taunts $N with insults!", ch, NULL, victim, TO_NOTVICT);
   act("$n taunts you with insults!", ch, NULL, victim, TO_VICT);

   /* Apply hitroll debuff to victim */
   af.type = gsn_taunt;
   af.duration = 3;
   af.location = APPLY_HITROLL;
   af.modifier = -(ch->level / 8);
   af.bitvector = 0;
   affect_to_char(victim, &af);

   /* Force NPC to fight ch if not already */
   if (IS_NPC(victim) && victim->fighting == NULL)
   {
      check_killer(ch, victim);
      set_fighting(victim, ch, TRUE);
      set_fighting(ch, victim, TRUE);
   }
}
