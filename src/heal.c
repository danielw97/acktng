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
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif

static bool is_healing_spell(int sn)
{
   if (sn < 0 || sn >= MAX_SKILL)
      return FALSE;

   if (skill_table[sn].name == NULL || skill_table[sn].spell_fun == NULL)
      return FALSE;

   return !str_cmp(skill_table[sn].name, "cure light") ||
          !str_cmp(skill_table[sn].name, "cure serious") ||
          !str_cmp(skill_table[sn].name, "cure critical") ||
          !str_cmp(skill_table[sn].name, "heal") || !str_cmp(skill_table[sn].name, "group heal") ||
          !str_cmp(skill_table[sn].name, "psionic recovery") ||
          !str_cmp(skill_table[sn].name, "regen") || !str_cmp(skill_table[sn].name, "influx") ||
          !str_cmp(skill_table[sn].name, "healing light");
}

void gain_holy_power_from_healing_spell(CHAR_DATA *ch, int sn)
{
   if (ch == NULL || IS_NPC(ch))
      return;

   if (!is_healing_spell(sn))
      return;

   ch->holy_power = UMIN(ch->holy_power + 1, 100 / 5);
}

/*
 * Spell for mega1.are from Glop/Erkenbrand.
 */
void do_spell_heal(CHAR_DATA *ch, CHAR_DATA *victim, int sn)
{
   int heal = class_heal_character(ch, victim, 50, sn, INDEX_CLE, FALSE);

   heal_character(ch, victim, heal, sn, FALSE);
}

int spell_regen_base_heal(int mage_level, int sorcerer_level, int wizard_level, int spellpower)
{
   int base_heal = 10 + (mage_level / 2);
   int remort_level = UMAX(sorcerer_level, wizard_level);

   if (remort_level > 0)
      base_heal += 3 + remort_level;

   base_heal += spellpower / 4;

   return base_heal;
}
