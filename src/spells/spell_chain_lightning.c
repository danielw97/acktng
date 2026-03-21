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

bool spell_chain_lightning(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{
   /*
    * affects all in a room, can also hit caster <grin>
    */

   CHAR_DATA *vch;
   CHAR_DATA *vch_next;
   int dam;

   /*
    * Work out starting damage.
    */
   dam = dice(10, (get_psuedo_level(ch) / 2));

   if (obj == NULL)
   {
      send_to_char("Lightning flashes from your fingers!\n\r", ch);
      act("$n unleashes lightning from $s fingers!", ch, NULL, NULL, TO_ROOM);
   }
   else
   {
      act("A lightning bolt flashes from $p!", ch, obj, NULL, TO_ROOM);
      act("A lightning bolt flashes from $p!", ch, obj, NULL, TO_CHAR);
   }
   CREF(vch_next, CHAR_NEXTROOM);

   for (vch = ch->in_room->first_person; vch != NULL; vch = vch_next)
   {
      vch_next = vch->next_in_room;

      if (vch != ch && IS_NPC(ch) ? !IS_NPC(vch) : IS_NPC(vch))
      {
         send_to_char("The lightning bolt strikes you!\n\r", vch);
         act("The lightning bolt strikes $n!", vch, NULL, NULL, TO_ROOM);
         sp_damage(obj, ch, vch, dam, ELEMENT_AIR | NO_REFLECT, sn, TRUE);
         dam = (4 * dam / 5);
      }
   }
   char_unreference(&vch_next);

   /*
    * Now see if caster gets hit.
    */

   if (saves_spell(level, ch))
   {
      act("The lightning bolt hits the ground, and is GONE!", ch, NULL, NULL, TO_ROOM);
      send_to_char("Your lightning bolts hits the ground and is GONE!\n\r", ch);
   }
   else
   {
      act("The lightning bolt strikes $n and vanishes!", ch, NULL, NULL, TO_ROOM);
      send_to_char("Your lightning returns, and hits you!\n\r", ch);
      dam = UMIN(level / 2, dam);
      sp_damage(obj, ch, ch, dam, ELEMENT_AIR | NO_REFLECT | NO_ABSORB, sn, TRUE);
   }
   return TRUE;
}
