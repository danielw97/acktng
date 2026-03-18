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

void do_shadow_step(CHAR_DATA *ch, char *argument)
{
   char arg[MSL];
   int door;
   EXIT_DATA *pExit;

   if (IS_NPC(ch))
      return;

   if (!can_use_skill(ch, gsn_shadow_step))
   {
      send_to_char("You don't know how to use this skill!\n\r", ch);
      return;
   }

   if (is_fighting(ch))
   {
      send_to_char("You cannot shadow step while fighting!\n\r", ch);
      return;
   }

   one_argument(argument, arg);

   if (arg[0] == '\0')
   {
      send_to_char("Shadow step in which direction?\n\r", ch);
      return;
   }

   /* Map direction argument to door number */
   if (!str_prefix(arg, "north"))
      door = 0;
   else if (!str_prefix(arg, "east"))
      door = 1;
   else if (!str_prefix(arg, "south"))
      door = 2;
   else if (!str_prefix(arg, "west"))
      door = 3;
   else if (!str_prefix(arg, "up"))
      door = 4;
   else if (!str_prefix(arg, "down"))
      door = 5;
   else
   {
      send_to_char("That is not a valid direction.\n\r", ch);
      return;
   }

   if ((pExit = ch->in_room->exit[door]) == NULL || pExit->to_room == NULL)
   {
      send_to_char("There is no exit in that direction.\n\r", ch);
      return;
   }

   WAIT_STATE(ch, skill_table[gsn_shadow_step].beats);
   raise_skill(ch, gsn_shadow_step);

   send_to_char("You step through the shadows.\n\r", ch);

   char_from_room(ch);
   char_to_room(ch, pExit->to_room);
}
