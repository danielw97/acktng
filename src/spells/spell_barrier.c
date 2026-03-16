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

bool spell_barrier(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{

   /*
    * Psi spell, like shield, but slightly better.  Good idea to make
    * * sure Psi's don't have access to shield as well as this... ;)
    * * -- Stephen
    */

   CHAR_DATA *victim = (CHAR_DATA *)vo;
   AFFECT_DATA af;

   if (is_affected(victim, sn))
      return FALSE;

   af.type = sn;
   if (ch == victim)
      af.duration = -1;
   else
      af.duration = 4 + (level / 20);
   af.location = APPLY_AC;
   af.modifier = -20 - get_psuedo_level(ch) / 10;
   af.bitvector = 0;
   affect_to_char(victim, &af);

   act("$n is surrounded by a telekinetic barrier.", victim, NULL, NULL, TO_ROOM);
   send_to_char("You are surrounded by a telekinetic barrier.\n\r", victim);
   return TRUE;
}
