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

/* Original 'magic.c' now contains the first half of the file, while the
   second half is in this 'magic2.c' file.  -S- */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif

extern bool deathmatch;
void breath_damage(CHAR_DATA *ch, int sn, int element);
void aoe_damage(CHAR_DATA *ch, OBJ_DATA *obj, int sn, int level, int min_dam, int max_dam,
                int element, int flags);

/* Flags for aoe_damage() */
#define AOE_SAVES (1 << 0)      /* Half damage on successful saves_spell() */
#define AOE_SKIP_GROUP (1 << 1) /* Skip characters in the same group as ch */

/*
 * For Ack! : Level 66 Priest spell - allow 100% recall.
 */

/*
 * NPC spells.
 */

void breath_damage(CHAR_DATA *ch, int sn, int element)
{
   if (ch == NULL)
      return;

   int min_dam = get_max_hp(ch) / 32;
   int max_dam = get_max_hp(ch) / 16;

   int dam_mod = ((ch->hit * 50) / get_max_hp(ch)) + 50;
   min_dam = min_dam * dam_mod / 100;
   max_dam = max_dam * dam_mod / 100;

   aoe_damage(ch, NULL, sn, 0, min_dam, max_dam, element, AOE_SKIP_GROUP);
}

/*
 * aoe_damage: deal area-of-effect damage to all valid targets in ch's room.
 *
 * Targets enemies only (NPC attacks PCs, PC attacks NPCs). The caster is
 * never hit. Optional flags:
 *   AOE_SAVES      -- targets that pass saves_spell() take half damage.
 *   AOE_SKIP_GROUP -- characters in the same group as ch are skipped.
 */
void aoe_damage(CHAR_DATA *ch, OBJ_DATA *obj, int sn, int level, int min_dam, int max_dam,
                int element, int flags)
{
   CHAR_DATA *vch;
   CHAR_DATA *vch_next;
   int dam;

   if (ch == NULL || ch->in_room == NULL)
      return;

   CREF(vch_next, CHAR_NEXTROOM);
   for (vch = ch->in_room->first_person; vch != NULL; vch = vch_next)
   {
      vch_next = vch->next_in_room;

      if (vch == ch)
         continue;

      /* Only hit enemies: NPCs damage PCs and vice versa. */
      if (IS_NPC(ch) == IS_NPC(vch))
         continue;

      if ((flags & AOE_SKIP_GROUP) && is_same_group(ch, vch))
         continue;

      dam = (min_dam == max_dam) ? min_dam : number_range(min_dam, max_dam);

      if ((flags & AOE_SAVES) && saves_spell(level, vch))
         dam /= 2;

      sp_damage(obj, ch, vch, dam, element, sn, TRUE);
   }
   CUREF(vch_next);
}
