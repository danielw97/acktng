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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "globals.h"
#include "tables.h"
#include "magic.h"

bool spell_static(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{
   /*
    * Psi Spell.
    * * Uses ch's movement, which is transfered to static electricity
    * * which is then chanelled at the poor victim <laugh>
    * * --Stephen
    */

   CHAR_DATA *victim = (CHAR_DATA *)vo;
   int dam;
   int loss;

   send_to_char("You transfer kinetic to static energy!\n\r", ch);

   loss = dice(2, level) + dice(2, level);

   dam = number_range(6, 8) * (level / 3);
   if ((ch->move - loss) <= 0)
   {
      send_to_char("You have no kinetic energy left!!\n\r", ch);
      return FALSE;
   }

   ch->move = UMAX(1, ch->move - loss);

   if (saves_spell(level, victim))
      dam /= 2;

   act("$n transfers kinetic to static energy.", ch, NULL, NULL, TO_ROOM);
   sp_damage(obj, ch, victim, dam, ELEMENT_AIR | NO_REFLECT | NO_ABSORB, sn, TRUE);

   return TRUE;
}
