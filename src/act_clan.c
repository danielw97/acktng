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

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "globals.h"

void set_obj_stat_auto(OBJ_DATA *obj);
void set_aff_to_obj(OBJ_DATA *obj, int location, int modifier);

/* Map obj->weight to a human-readable style name.
 * melee=8, caster=3, tank=13 (matches emblem system). */
static const char *claneq_weight_name(int obj_weight)
{
   if (obj_weight == CLANEQ_WEIGHT_CASTER)
      return "caster";
   if (obj_weight == CLANEQ_WEIGHT_TANK)
      return "tank";
   return "melee";
}

/* Create a clan equipment piece for a character. */
OBJ_DATA *create_clan_eq(CHAR_DATA *ch)
{
   char buf[MAX_STRING_LENGTH];
   char color[4];
   OBJ_DATA *obj;
   int clan_id;
   int level;

   if (ch == NULL || IS_NPC(ch))
      return NULL;

   clan_id = ch->pcdata->clan;
   if (clan_id < 1 || clan_id >= MAX_CLAN)
      return NULL;

   level = get_psuedo_level(ch);
   if (level < 1)
      level = 1;

   /* Extract the two-char color code (e.g. "@@y" -> "y") */
   {
      const char *abbr = clan_table[clan_id].clan_abbr;
      if (abbr[0] == '@' && abbr[1] == '@' && abbr[2] != '\0')
      {
         snprintf(color, sizeof(color), "@@%c", abbr[2]);
      }
      else
         snprintf(color, sizeof(color), "@@N");
   }

   obj = create_object(get_obj_index(OBJ_VNUM_MUSHROOM), 0);
   if (obj == NULL)
      return NULL;

   obj->item_type = ITEM_ARMOR;
   obj->level = level;
   obj->wear_flags = ITEM_TAKE | ITEM_WEAR_CLAN_COLORS;
   obj->weight = CLANEQ_WEIGHT_MELEE;
   obj->cost = 0;
   obj->obj_fun = obj_fun_lookup("objfun_clan");

   /* Set flags: NODROP, NOLOOT, CLAN_EQ, NOSAC, NOREMOVE */
   REMOVE_BIT(obj->extra_flags, ITEM_NOSAVE);
   REMOVE_BIT(obj->extra_flags, ITEM_BIND_EQUIP);
   SET_BIT(obj->extra_flags, ITEM_NODROP);
   SET_BIT(obj->extra_flags, ITEM_NOLOOT);
   SET_BIT(obj->extra_flags, ITEM_CLAN_EQ);
   SET_BIT(obj->extra_flags, ITEM_NOSAC);

   /* Name / descriptions themed to the clan */
   snprintf(buf, sizeof(buf), "clan colors %s", clan_table[clan_id].clan_name);
   free_string(obj->name);
   obj->name = str_dup(buf);

   snprintf(buf, sizeof(buf), "%sthe colors of %s@@N", color, clan_table[clan_id].clan_name);
   free_string(obj->short_descr);
   obj->short_descr = str_dup(buf);

   snprintf(buf, sizeof(buf), "%sThe colors of %s@@N lie here.", color,
            clan_table[clan_id].clan_name);
   free_string(obj->description);
   obj->description = str_dup(buf);

   set_obj_stat_auto(obj);

   return obj;
}

/* Adjust an existing clan eq piece to match the owner's current level.
 * The object's weight (stat focus) is preserved. */
void claneq_adjust(OBJ_DATA *obj, CHAR_DATA *ch)
{
   AFFECT_DATA *paf;
   AFFECT_DATA *paf_next;
   int level;

   if (obj == NULL || ch == NULL || IS_NPC(ch))
      return;

   level = get_psuedo_level(ch);
   if (level < 1)
      level = 1;

   obj->level = level;

   /* Strip existing affects before recalculating */
   for (paf = obj->first_apply; paf != NULL; paf = paf_next)
   {
      paf_next = paf->next;
      UNLINK(paf, obj->first_apply, obj->last_apply, next, prev);
      PUT_FREE(paf, affect_free);
   }
   obj->first_apply = NULL;
   obj->last_apply = NULL;

   set_obj_stat_auto(obj);
}

/* Find the character's clan eq (worn or carried). */
OBJ_DATA *find_clan_eq(CHAR_DATA *ch)
{
   OBJ_DATA *obj;

   if (ch == NULL)
      return NULL;

   for (obj = ch->first_carry; obj != NULL; obj = obj->next_in_carry_list)
   {
      if (IS_SET(obj->extra_flags, ITEM_CLAN_EQ) && IS_SET(obj->wear_flags, ITEM_WEAR_CLAN_COLORS))
         return obj;
   }
   return NULL;
}

/* Destroy all clan eq on a character. */
void destroy_clan_eq(CHAR_DATA *ch)
{
   OBJ_DATA *obj;
   OBJ_DATA *obj_next;

   if (ch == NULL)
      return;

   for (obj = ch->first_carry; obj != NULL; obj = obj_next)
   {
      obj_next = obj->next_in_carry_list;
      if (IS_SET(obj->extra_flags, ITEM_CLAN_EQ) && IS_SET(obj->wear_flags, ITEM_WEAR_CLAN_COLORS))
      {
         if (obj->wear_loc != WEAR_NONE)
            unequip_char(ch, obj);
         act("$p shatters into dust!", ch, obj, NULL, TO_CHAR);
         act("$p carried by $n shatters into dust!", ch, obj, NULL, TO_ROOM);
         extract_obj(obj);
      }
   }
}

/* Player command: claneq <melee|caster|tank> */
void do_claneq(CHAR_DATA *ch, char *argument)
{
   char buf[MAX_STRING_LENGTH];
   OBJ_DATA *obj;
   int new_weight;

   if (IS_NPC(ch))
      return;

   if (ch->pcdata->clan == 0)
   {
      send_to_char("You are not in a clan.\n\r", ch);
      return;
   }

   obj = find_clan_eq(ch);

   if (argument[0] == '\0')
   {
      const char *current = obj != NULL ? claneq_weight_name(obj->weight) : "melee";
      snprintf(buf, sizeof(buf),
               "Your clan equipment is currently set to: @@W%s@@N.\n\r"
               "Syntax: claneq <melee|caster|tank>\n\r",
               current);
      send_to_char(buf, ch);
      return;
   }

   if (!str_prefix(argument, "melee"))
      new_weight = CLANEQ_WEIGHT_MELEE;
   else if (!str_prefix(argument, "caster"))
      new_weight = CLANEQ_WEIGHT_CASTER;
   else if (!str_prefix(argument, "tank"))
      new_weight = CLANEQ_WEIGHT_TANK;
   else
   {
      send_to_char("Valid options: melee, caster, tank.\n\r", ch);
      return;
   }

   if (obj == NULL)
   {
      send_to_char("You don't have your clan colors.\n\r", ch);
      return;
   }

   {
      bool was_worn = (obj->wear_loc != WEAR_NONE);

      if (was_worn)
         unequip_char(ch, obj);

      obj->weight = new_weight;
      claneq_adjust(obj, ch);

      if (was_worn)
         equip_char(ch, obj, WEAR_CLAN_COLORS);

      snprintf(buf, sizeof(buf), "Your clan colors shimmer and reform with a %s focus!\n\r",
               claneq_weight_name(new_weight));
      send_to_char(buf, ch);
   }
}

/* CSET is an immortal command, used to set a players clan. */

void do_cset(CHAR_DATA *ch, char *argument)
{
   /*
    * This is basically do_mset, but with its guts ripped out,
    * and a slight change to what was left.
    * -- Stephen
    */

   char arg1[MAX_INPUT_LENGTH];
   char arg2[MAX_INPUT_LENGTH];
   char buf[MAX_STRING_LENGTH];
   CHAR_DATA *victim;
   int value;

   smash_tilde(argument);
   argument = one_argument(argument, arg1);
   strcpy(arg2, argument);
   if (IS_NPC(ch))
      return;

   if (arg1[0] == '\0' || arg2[0] == '\0')
   {
      send_to_char("Syntax: cset <player> <clan No.>\n\r\n\r", ch);
      do_clan_list(ch, "");
      return;
   }

   if ((victim = get_char_world(ch, arg1)) == NULL)
   {
      send_to_char("They aren't here.\n\r", ch);
      return;
   }

   value = is_number(arg2) ? atoi(arg2) : -1;

   if (value == -1)
   {
      send_to_char("Syntax: cset <player> <clan No.>\n\r\n\r", ch);
      do_clan_list(ch, "");
      return;
   }

   if (IS_NPC(victim))
   {
      send_to_char("Not on NPC's.\n\r", ch);
      return;
   }

   if (value < 0 || value >= MAX_CLAN)
   {
      sprintf(buf, "%d is not a valid value.\n\r", value);
      send_to_char(buf, ch);
      sprintf(buf, "Use a value between 0 and %d.\n\r\n\r", MAX_CLAN - 1);
      send_to_char(buf, ch);
      do_clan_list(ch, "");
      return;
   }

   victim->pcdata->clan = value;
   sprintf(buf, "%s now belongs to clan %s.\n\r", victim->name, clan_table[value].clan_name);
   send_to_char(buf, ch);

   sprintf(buf, "%s has cset %s into clan %s.", ch->name, victim->name,
           clan_table[value].clan_name);
   monitor_chan(buf, MONITOR_CLAN);

   return;
}
