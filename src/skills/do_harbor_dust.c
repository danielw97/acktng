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

void do_harbor_dust(CHAR_DATA *ch, char *argument)
{
   AFFECT_DATA af;
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;

   if (!can_use_skill_message(ch, gsn_harbor_dust))
      return;

   one_argument(argument, arg);

   if (arg[0] == '\0' && !is_fighting(ch))
   {
      send_to_char("Throw harbor dust at whom?\n\r", ch);
      return;
   }

   if ((victim = get_char_room(ch, arg)) == NULL && !is_fighting(ch))
   {
      send_to_char("They aren't here.\n\r", ch);
      return;
   }

   if (!subtract_energy_cost(ch, gsn_harbor_dust))
      return;

   if (victim == NULL)
      victim = ch->fighting;

   if (victim == ch)
   {
      send_to_char("That wouldn't be too smart, would it??.\n\r", ch);
      return;
   }

   if (is_safe(ch, victim))
   {
      send_to_char("They are safe!\n\r", ch);
      return;
   }

   if (IS_AFFECTED(victim, AFF_BLIND))
   {
      send_to_char("Your target is already blinded!\n\r", ch);
      return;
   }

   WAIT_STATE(ch, skill_table[gsn_harbor_dust].beats);

   raise_skill(ch, gsn_harbor_dust);

   if (!can_hit_skill(ch, victim, gsn_harbor_dust))
   {
      act("You fling harbor dust at $M but the wind takes it!", ch, NULL, victim, TO_CHAR);
      act("$n flings harbor dust at you but misses!", ch, NULL, victim, TO_VICT);
      act("$n flings harbor dust at $N but misses!", ch, NULL, victim, TO_NOTVICT);
      return;
   }
   else
   {
      act("You fling a fistful of harbor dust into $S eyes!", ch, NULL, victim, TO_CHAR);
      act("$n flings a fistful of harbor dust into your eyes!", ch, NULL, victim, TO_VICT);
      act("$n flings harbor dust at $N's eyes!", ch, NULL, victim, TO_NOTVICT);

      af.type = skill_lookup("blindness");
      af.location = APPLY_HITROLL;
      af.modifier = -2;
      af.duration = 3;
      af.duration_type = DURATION_ROUND;
      af.bitvector = AFF_BLIND;
      affect_to_char(victim, &af);
   }

   set_fighting(victim, ch, TRUE);

   combo(ch, victim, gsn_harbor_dust);
   return;
}
