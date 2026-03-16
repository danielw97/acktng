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

bool spell_burning_hands(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{
   CHAR_DATA *victim = (CHAR_DATA *)vo;
   static const sh_int dam_each[] = {0,  0,  0,  0,  0,  14, 17, 20, 23, 26, 29, 29, 29,
                                     30, 30, 31, 31, 32, 32, 33, 33, 34, 34, 35, 35, 36,
                                     36, 37, 37, 38, 38, 39, 39, 40, 40, 41, 41, 42, 42,
                                     43, 43, 44, 44, 45, 45, 46, 46, 47, 47, 48, 48};
   int dam;

   level = UMIN(level, sizeof(dam_each) / sizeof(dam_each[0]) - 1);
   level = UMAX(0, level);
   dam = number_range(dam_each[level] / 2, dam_each[level] * 2);
   if (saves_spell(level, victim))
      dam /= 2;

   sp_damage(obj, ch, victim, dam, ELE_FIRE, sn, TRUE);
   return TRUE;
}
