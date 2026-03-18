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

void do_grapple(CHAR_DATA *ch, char *argument)
{
   char arg[MSL];
   CHAR_DATA *victim;
   AFFECT_DATA af;

   if (IS_NPC(ch))
      return;

   if (!can_use_skill(ch, gsn_grapple))
   {
      send_to_char("You don't know how to use this skill!\n\r", ch);
      return;
   }

   if (!is_fighting(ch))
   {
      send_to_char("You must be fighting to grapple!\n\r", ch);
      return;
   }

   one_argument(argument, arg);

   if (arg[0] != '\0')
      victim = get_char_room(ch, arg);
   else
      victim = ch->fighting;

   if (victim == NULL)
   {
      send_to_char("Grapple whom?\n\r", ch);
      return;
   }

   if (is_safe(ch, victim))
      return;

   WAIT_STATE(ch, skill_table[gsn_grapple].beats);
   raise_skill(ch, gsn_grapple);

   /* Apply AC debuff to victim as "engaged/clinch" effect */
   af.type = gsn_grapple;
   af.duration = 2;
   af.location = APPLY_AC;
   af.modifier = -(ch->level / 5);
   af.bitvector = AFF_HOLD;
   affect_to_char(victim, &af);

   /* Caster is also grappling */
   af.type = gsn_grapple;
   af.duration = 2;
   af.location = APPLY_AC;
   af.modifier = -(ch->level / 5);
   af.bitvector = 0;
   affect_to_char(ch, &af);

   act("You grab $N and pull them into a clinch!", ch, NULL, victim, TO_CHAR);
   act("$n grabs $N and pulls them into a clinch!", ch, NULL, victim, TO_NOTVICT);
   act("$n grabs you and pulls you into a clinch!", ch, NULL, victim, TO_VICT);
}
