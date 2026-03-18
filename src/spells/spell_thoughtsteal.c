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

bool spell_thoughtsteal(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{
   CHAR_DATA *victim = (CHAR_DATA *)vo;
   AFFECT_DATA af;
   int stolen;

   stolen = level / 8;
   if (stolen < 1)
      stolen = 1;

   act("You tear fragments of thought from $N's mind!", ch, NULL, victim, TO_CHAR);
   act("$n tears fragments of thought from $N's mind!", ch, NULL, victim, TO_NOTVICT);
   act("$n tears fragments of thought from your mind!", ch, NULL, victim, TO_VICT);

   /* Debuff victim */
   af.type = sn;
   af.duration = level / 6;
   af.duration_type = DURATION_ROUND;
   af.location = APPLY_INT;
   af.modifier = -stolen;
   af.bitvector = 0;
   affect_to_char(victim, &af);

   af.location = APPLY_SAVING_SPELL;
   af.modifier = level / 6;
   affect_to_char(victim, &af);

   /* Buff caster */
   af.type = sn;
   af.duration = level / 6;
   af.duration_type = DURATION_ROUND;
   af.location = APPLY_INT;
   af.modifier = stolen / 2;
   af.bitvector = 0;
   affect_to_char(ch, &af);

   return TRUE;
}
