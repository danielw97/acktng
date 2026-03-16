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

bool spell_phobia(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{
   /*
    * Psi Spell.
    * * Conjures victims's phobia in their mind, does dam (small) or
    * * causes them to flee.
    * * --Stephen
    */

   CHAR_DATA *victim = (CHAR_DATA *)vo;
   int dam;

   if ((victim->level) > (level + 8))
   {
      send_to_char("Your spell fails to take affect!\n\r", ch);
      return TRUE;
   }

   /*
    * figure out dam.... not too much as spell is meant to make vo flee
    */

   dam = dice(2, level / 8);
   if (obj == NULL)
   {
      act("$n projects nightmare images into $N's mind!", ch, NULL, victim, TO_NOTVICT);
      act("You project nightmare images into $N's mind!", ch, NULL, victim, TO_CHAR);
      act("$N projects nightmare images into your mind!", victim, NULL, ch, TO_CHAR);
   }
   else
   {
      act("$p projects nightmare images into $n's mind!", victim, obj, NULL, TO_ROOM);
      act("$p projects nightmare images into your mind!", victim, obj, NULL, TO_CHAR);
   }
   send_to_char("Your worst phobia springs to life in your mind. Arrrggghhh!\n\r", victim);
   act("$N suffers a mental phobia attack!", ch, NULL, victim, TO_NOTVICT);

   sp_damage(obj, ch, victim, dam, ELEMENT_MENTAL, sn, TRUE);
   return TRUE;
}
