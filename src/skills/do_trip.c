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

void do_trip(CHAR_DATA *ch, char *argument)
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   int best;
   /*    int cnt;  */

   best = -1;

   if (!IS_NPC(ch))
   {
      /*
       * for ( cnt = 0; cnt < MAX_CLASS; cnt++ )
       * if ( ch->class_level[cnt] >= skill_table[gsn_trip].skill_level[cnt]
       * && ch->class_level[cnt] >= best )
       * best = cnt;
       */
      if (ch->pcdata->learned[gsn_trip] > 75)
         best = UMAX(79, get_psuedo_level(ch));
   }
   else
      best = ch->level;

   if (best == -1)
   {
      send_to_char("You don't know of such a skill.\n\r", ch);
      return;
   }

   one_argument(argument, arg);

   if (arg[0] == '\0')
   {
      send_to_char("Trip whom?\n\r", ch);
      return;
   }

   if ((victim = get_char_room(ch, arg)) == NULL)
   {
      send_to_char("They aren't here.\n\r", ch);
      return;
   }

   if (victim == ch)
   {
      send_to_char("That wouldn't even be funny.\n\r", ch);
      return;
   }

   if (!subtract_energy_cost(ch, gsn_trip))
      return;

   if (is_safe(ch, victim))
      return;

   if (victim->fighting == NULL)
   {
      send_to_char("You can't trip someone who isn't fighting.\n\r", ch);
      return;
   }

   check_killer(ch, victim);

   if (number_percent() < (IS_NPC(ch) ? 50 : 2 * ch->pcdata->learned[gsn_trip]))
   {
      check_killer(ch, victim);
      trip(ch, victim);
      WAIT_STATE(ch, skill_table[gsn_trip].beats);
   }

   return;
}
