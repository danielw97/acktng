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

void do_target(CHAR_DATA *ch, char *argument)
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   if ((!IS_NPC(ch)) && (ch->pcdata->learned[gsn_target] < 65))
   {
      send_to_char("You are not trained enough in this skill!!\n\r", ch);
      return;
   }
   one_argument(argument, arg);

   if (arg[0] == '\0')
   {
      send_to_char("Target whom?\n\r", ch);
      return;
   }

   if ((victim = get_char_room(ch, arg)) == NULL)
   {
      send_to_char("They aren't here.\n\r", ch);
      return;
   }

   if (!IS_NPC(victim) && !(deathmatch))
   {
      if (!IS_SET(victim->act, PLR_KILLER) && !IS_SET(victim->act, PLR_THIEF))
      {
         send_to_char("You must MURDER a player.\n\r", ch);
         return;
      }
   }
   else
   {
      if (IS_AFFECTED(victim, AFF_CHARM) && victim->master != NULL)
      {
         send_to_char("You must MURDER a charmed creature.\n\r", ch);
         return;
      }
   }

   if (victim == ch)
   {
      send_to_char("You hit yourself.  Ouch!\n\r", ch);
      one_hit(ch, ch, TYPE_UNDEFINED);
      return;
   }

   if (is_safe(ch, victim))
      return;

   if (IS_AFFECTED(ch, AFF_CHARM) && ch->master == victim)
   {
      act("$N is your beloved master.", ch, NULL, victim, TO_CHAR);
      return;
   }

   if (is_fighting(ch))
   {
      send_to_char("@@rTracking, tracking, tracking...@@eGOT HIM!!!@@N\n\r", ch);
      stop_fighting(ch, FALSE);
   }

   WAIT_STATE(ch, 1 * PULSE_VIOLENCE);
   check_killer(ch, victim);
   one_hit(ch, victim, TYPE_UNDEFINED);
   return;
}
