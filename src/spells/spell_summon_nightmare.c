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

bool spell_summon_nightmare(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{
   CHAR_DATA *summoned;

   act("$n calls upon the unholy forces of @@eEvil@@N!", ch, obj, NULL, TO_ROOM);
   act("You call upon the unholy forces of @@eEvil@@N.", ch, obj, NULL, TO_CHAR);

   act("@@NA flaming pit appears, and a @@eDemonic @@dNightmare@@N steps from the heat!!", ch, obj,
       NULL, TO_ROOM);
   act("@@NA flaming pit appears, and a @@eDemonic @@dNightmare@@N steps from the heat!!", ch, obj,
       NULL, TO_CHAR);

   summoned = create_mobile(get_mob_index(MOB_VNUM_NIGHTMARE));
   char_to_room(summoned, ch->in_room);
   act("$n steps from the flames, drawing all the power into it's body.", summoned, NULL, NULL,
       TO_ROOM);

   /*
    * don't think we need these
    * summoned->level    = 40;
    * summoned->max_  = dice( 8, 40 );
    * summoned->hit      = summoned->max_;
    * summoned->max_move = dice( 10, 40 );
    * summoned->move     = summoned->max_move;
    *
    */
   SET_BIT(summoned->act, ACT_PET);
   SET_BIT(summoned->affected_by, AFF_CHARM);
   summoned->extract_timer = get_psuedo_level(ch) / 3;
   add_follower(summoned, ch);
   return TRUE;
}
