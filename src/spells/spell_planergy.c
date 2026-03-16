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

bool spell_planergy(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{

   /*
    * Psi spell.  Work out which energy to summon, according to
    * * ch's level.  Each 9 levels, the Psi gets the use of a different
    * * spell, and more damage! ;)  (planergy = planar energy)
    * * --Stephen
    */

   CHAR_DATA *victim = (CHAR_DATA *)vo;
   int dam;
   int lvl;
   int plane;

   if (level <= 8)
   {
      lvl = 4;
      plane = ELEMENT_PHYSICAL;
   }
   else if (level <= 17)
   {
      lvl = 13;
      plane = ELEMENT_AIR;
   }
   else if (level <= 26)
   {
      lvl = 22;
      plane = ELEMENT_FIRE;
   }
   else if (level <= 35)
   {
      lvl = 31;
      plane = ELEMENT_WATER;
   }
   else if (level <= 44)
   {
      lvl = 40;
      plane = ELEMENT_EARTH;
   }
   else if (level <= 53)
   {
      lvl = 49;
      plane = ELEMENT_MENTAL;
   }
   else if (level <= 62)
   {
      lvl = 58;
      plane = ELEMENT_POISON;
   }
   else if (level <= 71)
   {
      lvl = 67;
      plane = ELEMENT_HOLY;
   }
   else
   {
      lvl = 80;
      plane = ELEMENT_SHADOW;
   }

   dam = dice(6, lvl / 2);

   if (saves_spell(level, victim))
      dam /= 2;
   SET_BIT(plane, NO_REFLECT);
   SET_BIT(plane, NO_ABSORB);

   sp_damage(obj, ch, victim, dam, plane, sn, TRUE);

   return TRUE;
}
