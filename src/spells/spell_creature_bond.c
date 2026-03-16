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

bool spell_creature_bond(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{
   CHAR_DATA *victim = (CHAR_DATA *)vo;
   CHAR_DATA *master;
   if (IS_NPC(ch))
   {
      send_to_char("Not for Npcs.\n\r", ch);
      return FALSE;
   }

   if ((!IS_NPC(victim)) || (!IS_SET(victim->affected_by, AFF_CHARM)))
   {
      send_to_char("That is not a charmed creature!\n\r", ch);
      return FALSE;
   }
   master = victim->master ? victim->master : victim;
   if ((master != NULL) && (get_psuedo_level(ch) < (get_psuedo_level(master) - 20)))
   {
      send_to_char("The current bond is too strong for you to overcome.\n\r", ch);
      return FALSE;
   }

   if (number_range(0, 105) < (level + (get_psuedo_level(ch) - get_psuedo_level(master))))
   {
      if (saves_spell(level, victim))
      {
         do_say(victim, "How dare you!, I LOVE my master!");
         multi_hit(victim, ch, TYPE_UNDEFINED);
         return TRUE;
      }
      stop_follower(victim);
      SET_BIT(victim->act, ACT_PET);
      SET_BIT(victim->affected_by, AFF_CHARM);
      victim->extract_timer = get_psuedo_level(ch) / 3;
      add_follower(victim, ch);
   }
   else
   {
      do_say(victim, "How dare you!, I LOVE my master!");
      multi_hit(victim, ch, TYPE_UNDEFINED);
      return TRUE;
   }

   return TRUE;
}
