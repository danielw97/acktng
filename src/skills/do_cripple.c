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

void do_cripple(CHAR_DATA *ch, char *argument)
{
   char actbuf[MSL];
   char arg[MAX_INPUT_LENGTH];
   AFFECT_DATA af;
   CHAR_DATA *victim;
   int cnt = 0;

   if (IS_NPC(ch))
      return;

   if (!is_fighting(ch))
   {
      send_to_char("You can only prepare to cripple when fighting!\n\r", ch);
      return;
   }

   if (!can_use_skill(ch, gsn_cripple))
   {
      send_to_char("You don't know how to use this skill!\n\r", ch);
      return;
   }

   if (ch->cooldown[gsn_cripple] > 0)
   {
      send_to_char("Cripple is still on cooldown\n\r", ch);
      return;
   }

   one_argument(argument, arg);

   if (arg[0] == '\0')
   {
      send_to_char("Cripple whom?\n\r", ch);
      return;
   }

   victim = get_char_room(ch, arg);

   if (victim == NULL)
   {
      send_to_char("Cripple whom?\n\r", ch);
      return;
   }

   raise_skill(ch, gsn_cripple);
   ch->cooldown[gsn_cripple] = 10;

   WAIT_STATE(ch, skill_table[gsn_cripple].beats);

   if (is_affected(victim, skill_lookup("poison:quinine")))
   {
      cnt++;
      sprintf(actbuf, "$N screams as the quinine in $M veins is consumed!");
      act(actbuf, ch, NULL, victim, TO_NOTVICT);
      sprintf(actbuf, "$N screams as the quinine in $M veins is consumed!");
      act(actbuf, ch, NULL, victim, TO_CHAR);
      sprintf(actbuf, "You scream as the quinine in your veins is consumed!");
      act(actbuf, ch, NULL, victim, TO_VICT);
      affect_strip(victim, skill_lookup("poison:quinine"));
   }

   if (is_affected(victim, skill_lookup("poison:arsenic")))
   {
      cnt++;
      sprintf(actbuf, "$N screams as the arsenic in $M veins is consumed!");
      act(actbuf, ch, NULL, victim, TO_NOTVICT);
      sprintf(actbuf, "$N screams as the arsenic in $M veins is consumed!");
      act(actbuf, ch, NULL, victim, TO_CHAR);
      sprintf(actbuf, "You scream as the arsenic in your veins is consumed!");
      act(actbuf, ch, NULL, victim, TO_VICT);
      affect_strip(victim, skill_lookup("poison:arsenic"));
   }

   if (is_affected(victim, skill_lookup("poison:nightshade")))
   {
      cnt++;
      sprintf(actbuf, "$N screams as the nightshade in $M veins is consumed!");
      act(actbuf, ch, NULL, victim, TO_NOTVICT);
      sprintf(actbuf, "$N screams as the nightshade in $M veins is consumed!");
      act(actbuf, ch, NULL, victim, TO_CHAR);
      sprintf(actbuf, "You scream as the nightshade in your veins is consumed!");
      act(actbuf, ch, NULL, victim, TO_VICT);
      affect_strip(victim, skill_lookup("poison:nightshade"));
   }

   af.type = gsn_cripple;
   af.duration = 1;
   af.duration_type = DURATION_ROUND;
   af.location = APPLY_SAVING_PARA;
   af.modifier = 30 * cnt;
   af.bitvector = 0;
   affect_to_char(victim, &af);
}
