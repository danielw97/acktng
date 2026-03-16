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

void do_flurry(CHAR_DATA *ch, char *argument)
{
   char buf[MSL];
   if (!is_fighting(ch))
   {
      send_to_char("You can only perform a flurry when fighting!\n\r", ch);
      return;
   }

   if (!can_use_skill_message(ch, gsn_flurry))
      return;

   if (get_chi(ch) < 5)
   {
      sprintf(buf, "You must have at least 5 chi to initiate a flurry, you only have %d!\n\r",
              ch->chi);
      send_to_char(buf, ch);
      return;
   }

   raise_skill(ch, gsn_flurry);

   WAIT_STATE(ch, skill_table[gsn_flurry].beats);

   act("$n focuses their chi, and prepares for a flurry!", ch, NULL, NULL, TO_ROOM);
   act("You focus your chi, and prepare for a flurry!", ch, NULL, NULL, TO_CHAR);

   ch->cooldown[gsn_flurry] = 30;

   for (; get_chi(ch) > 0; ch->chi--)
   {
      one_hit(ch, ch->fighting, TYPE_HIT);
   }
}
