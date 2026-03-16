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

void do_disguise(CHAR_DATA *ch, char *argument)
{
   char arg[MAX_STRING_LENGTH];

   if (!IS_NPC(ch) && ch->pcdata->learned[gsn_disguise] == 0)
   {
      send_to_char("You are not trained in this skill!\n\r", ch);
      return;
   }

   one_argument(argument, arg);

   if (arg[0] == '\0')
   {
      send_to_char(" Enter the disguise name, or reset to reset your description to normal.\n\r",
                   ch);
      return;
   }

   if (!str_cmp(arg, "reset"))
   {
      free_string(ch->long_descr);
      ch->long_descr = str_dup(ch->long_descr_orig);
      return;
   }
   else
   {

      free_string(ch->long_descr);
      safe_strcat(MAX_STRING_LENGTH, argument, "\n\r");
      ch->long_descr = str_dup(argument);
      send_to_char("You are now Disguised!!!\n\r", ch);
      return;
   }

   return;
}
