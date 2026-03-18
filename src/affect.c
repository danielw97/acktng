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
#include <string.h>
#include "globals.h"
#include "quest.h"

static char buf[MAX_STRING_LENGTH];

void affect_modify(CHAR_DATA *ch, AFFECT_DATA *paf, bool fAdd)
{
   OBJ_DATA *wield;
   int mod;

   mod = paf->modifier;

   if (fAdd)
   {
      SET_BIT(ch->affected_by, paf->bitvector);
   }
   else
   {
      REMOVE_BIT(ch->affected_by, paf->bitvector);
      mod = 0 - mod;
   }

   /*
    * Check for weapon wielding.
    * Guard against recursion (for weapons with affects).
    */
   if ((ch->is_quitting == FALSE) && (ch->desc != NULL) &&
       (ch->desc->connected != CON_SETTING_STATS))
   {
      sh_int i;
      for (i = 0; i < MAX_WEAR; i++)
      {
         if (((wield = get_eq_char(ch, i)) != NULL) &&
             (get_obj_weight(wield) > get_wear_weight(ch) * 20))
         {
            static int depth;

            if (depth == 0)
            {
               depth++;
               act("You stop using $p since it is too heavy.", ch, wield, NULL, TO_CHAR);
               act("$n stops using $p. since it is too heavy", ch, wield, NULL, TO_ROOM);
               unequip_char(ch, wield);
               depth--;
            }
         }
      }
   }
   return;
}

void mark_to_room(int this_room_vnum, MARK_DATA *mark)
{
   /*
    * this assumes that the mark_data is good, and has
    * * been constructed properly. It will link it to the room
    * * and the main list. Caller must get the struct memory.
    * *
    */
   MARK_LIST_MEMBER *mlist;
   MARK_LIST_MEMBER *rlist;
   ROOM_INDEX_DATA *this_room;

   if ((this_room = get_room_index(this_room_vnum)) == NULL)
   {
      PUT_FREE(mark, mark_free);
      return;
   }

   GET_FREE(mlist, mark_list_free);
   GET_FREE(rlist, mark_list_free);

   mlist->mark = mark;
   rlist->mark = mark;

   LINK(mlist, first_mark_list, last_mark_list, next, prev);
   LINK(rlist, this_room->first_mark_list, this_room->last_mark_list, next, prev);
   if (!booting_up)
      save_marks();

   return;
}

void mark_from_room(int this_room_vnum, MARK_DATA *mark)
{

   MARK_LIST_MEMBER *mlist;
   MARK_LIST_MEMBER *rlist;
   ROOM_INDEX_DATA *this_room;

   this_room = get_room_index(this_room_vnum);

   for (rlist = this_room->first_mark_list; rlist != NULL; rlist = rlist->next)
   {
      if (rlist->mark == mark)
      {
         UNLINK(rlist, this_room->first_mark_list, this_room->last_mark_list, next, prev);
         break;
      }
   }
   for (mlist = first_mark_list; mlist != NULL; mlist = mlist->next)
   {
      if (mlist->mark == mark)
      {
         UNLINK(mlist, first_mark_list, last_mark_list, next, prev);
         break;
      }
   }

   PUT_FREE(mark, mark_free);
   PUT_FREE(rlist, mark_list_free);
   PUT_FREE(mlist, mark_list_free);
   if (!booting_up)
      save_marks();

   return;
}

/* Give an affect to a room */
void affect_to_room(ROOM_INDEX_DATA *room, ROOM_AFFECT_DATA *raf)
{
   ROOM_AFFECT_DATA *raf_new;

   GET_FREE(raf_new, raffect_free);
   /* Ramias... Don't copy uninitialized fields: next, prev, is_free */
   raf_new->duration = raf->duration;
   raf_new->level = raf->level;
   raf_new->type = raf->type;
   raf_new->bitvector = raf->bitvector;
   raf_new->applies_spell = raf->applies_spell;
   raf_new->modifier = raf->modifier;
   raf_new->location = raf->location;
   raf_new->caster = raf->caster;

   LINK(raf_new, room->first_room_affect, room->last_room_affect, next, prev);

   SET_BIT(room->affected_by, raf->bitvector);

   sprintf(buf, "@@e%s@@N has cast @@d%s@@N in @@Narea: @@r%s@@N, @@Nroom: @@r%d@@N.",
           raf->caster->name, raffect_bit_name(raf->bitvector), room->area->name, room->vnum);
   monitor_chan(buf, MONITOR_GEN_MORT);

   return;
}

/* Remove an affect from a room */
void r_affect_remove(ROOM_INDEX_DATA *room, ROOM_AFFECT_DATA *raf)
{
   if (room->first_room_affect == NULL)
   {
      sprintf(buf, "r_affect_remove: no affect to remove from room %d.", room->vnum);
      monitor_chan(buf, MONITOR_ROOM);

      bug("R_affect_remove: no affect for room: %d.", room->vnum);
      return;
   }

   REMOVE_BIT(room->affected_by, raf->bitvector);

   UNLINK(raf, room->first_room_affect, room->last_room_affect, next, prev);
   PUT_FREE(raf, raffect_free);
   return;
}

/*
 * Give an affect to a char.
 */
void affect_to_char(CHAR_DATA *ch, AFFECT_DATA *paf)
{
   AFFECT_DATA *paf_new;

   /* Boss mobs are immune to blindness */
   if (IS_NPC(ch) && IS_SET(ch->act, ACT_BOSS) && (paf->bitvector & AFF_BLIND))
      return;

   GET_FREE(paf_new, affect_free);
   /* Ramias... Don't copy uninitialized fields: next, prev, is_free */
   paf_new->type = paf->type;
   paf_new->duration_type = paf->duration_type;
   if (paf_new->duration_type != DURATION_HOUR && paf_new->duration_type != DURATION_ROUND)
      paf_new->duration_type = DURATION_HOUR;
   paf_new->element = paf->element;
   paf_new->duration = paf->duration;
   paf_new->location = paf->location;
   paf_new->modifier = paf->modifier;
   paf_new->bitvector = paf->bitvector;
   if (paf->caster == NULL)
      paf_new->caster = ch;
   else
      paf_new->caster = paf->caster;
   paf_new->level = paf->level;
   LINK(paf_new, ch->first_affect, ch->last_affect, next, prev);

   affect_modify(ch, paf_new, TRUE);

   return;
}

/*
 * Remove an affect from a char.
 */
void affect_remove(CHAR_DATA *ch, AFFECT_DATA *paf)
{
   sh_int shield_type;

   if (ch->first_affect == NULL)
   {
      sprintf(buf, "affect_remove: %s did not have aff %d to remove.",
              IS_NPC(ch) ? ch->short_descr : ch->name, paf->type);
      monitor_chan(buf, MONITOR_MOB);

      bug("Affect_remove: no affect.", 0);
      return;
   }

   affect_modify(ch, paf, FALSE);
   shield_type = SHIELD_NONE;

   if (paf->type == skill_lookup("fireshield"))
      shield_type = FLAME_SHIELD;
   else if (paf->type == skill_lookup("iceshield"))
      shield_type = ICE_SHIELD;
   else if (paf->type == skill_lookup("shockshield"))
      shield_type = SHOCK_SHIELD;
   else if (paf->type == skill_lookup("shadowshield"))
      shield_type = SHADOW_SHIELD;
   else if (paf->type == skill_lookup("thoughtshield"))
      shield_type = PSI_SHIELD;
   if (shield_type > SHIELD_NONE)
   {
      MAGIC_SHIELD *this_shield;

      for (this_shield = ch->first_shield; this_shield != NULL; this_shield = this_shield->next)
         if (this_shield->type == shield_type)
            break;
      if (this_shield != NULL)
      {
         char buf1[MSL];
         char buf2[MSL];

         snprintf(buf1, sizeof(buf1), "%s", this_shield->wearoff_room);
         snprintf(buf2, sizeof(buf2), "%s", this_shield->wearoff_self);
         act(buf1, ch, NULL, NULL, TO_ROOM);
         act(buf2, ch, NULL, NULL, TO_CHAR);

         UNLINK(this_shield, ch->first_shield, ch->last_shield, next, prev);
         PUT_FREE(this_shield, shield_free);
      }
   }

   UNLINK(paf, ch->first_affect, ch->last_affect, next, prev);
   PUT_FREE(paf, affect_free);
   return;
}

/*
 * Strip all affects of a given sn.
 */
void affect_strip(CHAR_DATA *ch, int sn)
{
   AFFECT_DATA *paf;
   AFFECT_DATA *paf_next;

   for (paf = ch->first_affect; paf != NULL; paf = paf_next)
   {
      paf_next = paf->next;
      if (paf->type == sn)
         affect_remove(ch, paf);
   }

   return;
}

/*
 * Return true if a char is affected by a spell.
 */
bool is_affected(CHAR_DATA *ch, int sn)
{
   AFFECT_DATA *paf;

   for (paf = ch->first_affect; paf != NULL; paf = paf->next)
   {
      if (paf->type == sn)
         return TRUE;
   }

   return FALSE;
}

/*
 * Add or enhance an affect.
 */
void affect_join(CHAR_DATA *ch, AFFECT_DATA *paf)
{
   AFFECT_DATA *paf_old;

   for (paf_old = ch->first_affect; paf_old != NULL; paf_old = paf_old->next)
   {
      if ((paf_old->type == paf->type) && (paf_old->location == paf->location) &&
          (paf_old->bitvector == paf->bitvector))
      {
         paf->duration += paf_old->duration;
         paf->modifier += paf_old->modifier;
         affect_remove(ch, paf_old);
         break;
      }
   }

   affect_to_char(ch, paf);
   return;
}
