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

bool spell_restoration(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{
   CHAR_DATA *victim = (CHAR_DATA *)vo;

   /* Strip negative affects */
   if (is_affected(victim, gsn_blindness))
   {
      affect_strip(victim, gsn_blindness);
      REMOVE_BIT(victim->affected_by, AFF_BLIND);
   }

   if (is_affected(victim, gsn_curse))
   {
      affect_strip(victim, gsn_curse);
      REMOVE_BIT(victim->affected_by, AFF_CURSE);
   }

   if (is_affected(victim, gsn_poison))
   {
      affect_strip(victim, gsn_poison);
      REMOVE_BIT(victim->affected_by, AFF_POISON);
   }

   if (is_affected(victim, gsn_sleep))
   {
      affect_strip(victim, gsn_sleep);
      REMOVE_BIT(victim->affected_by, AFF_SLEEP);
   }

   /* Restore some hit points */
   victim->hit = UMIN(victim->hit + level, get_max_hp(victim));

   if (ch == victim)
   {
      send_to_char("Divine light washes over you, restoring your body and spirit!\n\r", victim);
   }
   else
   {
      act("Divine light washes over $n, restoring $m completely!", victim, NULL, NULL, TO_ROOM);
      act("Divine light washes over you, restoring your body and spirit!", victim, NULL, NULL,
          TO_CHAR);
      send_to_char("Ok.\n\r", ch);
   }

   return TRUE;
}
