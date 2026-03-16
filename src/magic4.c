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
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif

extern bool deathmatch;
void round_update_dot(CHAR_DATA *ch);

static bool is_grand_magi_elemental_spell(int sn)
{
   return sn == skill_lookup("elemental inferno") || sn == skill_lookup("elemental shock") ||
          sn == skill_lookup("elemental deluge") || sn == skill_lookup("elemental rupture");
}

int spell_regen_base_heal(int mage_level, int sorcerer_level, int wizard_level, int spellpower)
{
   int base_heal = 10 + (mage_level / 2);
   int remort_level = UMAX(sorcerer_level, wizard_level);

   if (remort_level > 0)
      base_heal += 3 + remort_level;

   base_heal += spellpower / 4;

   return base_heal;
}

bool cast_wizard_elemental_dot_spell(int sn, int level, CHAR_DATA *ch, CHAR_DATA *victim,
                                     OBJ_DATA *obj, const char *cast_msg, const char *hit_msg,
                                     int element)
{
   AFFECT_DATA af;
   int base_damage;
   int up_front_damage;
   int dot_tick_damage;

   if (is_affected(victim, sn))
   {
      send_to_char("They are already affected by that spell.\n\r", ch);
      return FALSE;
   }

   if (obj == NULL)
   {
      int base = ch->class_level[CLASS_WIZ];

      if (ch->class_level[CLASS_SOR] > base)
         base = ch->class_level[CLASS_SOR];

      base_damage = 150 + dice(base / 2, 20);
      act(cast_msg, ch, NULL, NULL, TO_ROOM);
      act(cast_msg, ch, NULL, NULL, TO_CHAR);
   }
   else
   {
      base_damage = dice(level / 4, 20);
      act(cast_msg, ch, obj, NULL, TO_ROOM);
      act(cast_msg, ch, obj, NULL, TO_CHAR);
   }

   if (is_grand_magi_elemental_spell(sn))
   {
      up_front_damage = base_damage * 90 / 100;
      dot_tick_damage = (base_damage * 25 / 100) / 3;
   }
   else
   {
      up_front_damage = base_damage * 60 / 100;
      dot_tick_damage = (base_damage * 50 / 100) / 3;
   }

   act(hit_msg, victim, NULL, NULL, TO_ROOM);
   act(hit_msg, victim, NULL, NULL, TO_CHAR);
   sp_damage(obj, ch, victim, up_front_damage, element, sn, TRUE);

   af.type = sn;
   af.duration = 3;
   af.duration_type = DURATION_ROUND;
   af.location = APPLY_DOT;
   af.modifier = dot_tick_damage;
   af.bitvector = 0;
   af.caster = ch;
   af.element = element;
   affect_to_char(victim, &af);

   return TRUE;
}

enum elemental_debuff_type
{
   ELEMENTAL_DEBUFF_NONE = 0,
   ELEMENTAL_DEBUFF_ICE,
   ELEMENTAL_DEBUFF_FIRE,
   ELEMENTAL_DEBUFF_SHOCK,
   ELEMENTAL_DEBUFF_EARTH
};

static int get_elemental_debuff_type(int sn)
{
   if (sn == skill_lookup("Ice Bolt") || sn == skill_lookup("Cone of cold") ||
       sn == skill_lookup("tidal lash") || sn == skill_lookup("elemental deluge"))
      return ELEMENTAL_DEBUFF_ICE;

   if (sn == skill_lookup("Fire Blast") || sn == skill_lookup("phoenix flare") ||
       sn == skill_lookup("elemental inferno"))
      return ELEMENTAL_DEBUFF_FIRE;

   if (sn == skill_lookup("Shock Storm") || sn == skill_lookup("arc lightning") ||
       sn == skill_lookup("elemental shock"))
      return ELEMENTAL_DEBUFF_SHOCK;

   if (sn == skill_lookup("Earth Shatter") || sn == skill_lookup("terra rend") ||
       sn == skill_lookup("elemental rupture"))
      return ELEMENTAL_DEBUFF_EARTH;

   return ELEMENTAL_DEBUFF_NONE;
}

static void apply_spell_debuff_dot(CHAR_DATA *ch, CHAR_DATA *victim, int sn, int element,
                                   int duration, int dot_tick_damage, const char *debuff_msg)
{
   AFFECT_DATA af;

   af.type = sn;
   af.duration = duration;
   af.duration_type = DURATION_ROUND;
   af.location = APPLY_DOT;
   af.modifier = dot_tick_damage;
   af.bitvector = 0;
   af.caster = ch;
   af.element = element;
   affect_join(victim, &af);

   if (debuff_msg != NULL && debuff_msg[0] != '\0')
      act((char *)debuff_msg, victim, NULL, NULL, TO_CHAR);
}

void apply_elemental_spell_debuff(CHAR_DATA *ch, CHAR_DATA *victim, int sn, const char *debuff_msg)
{
   switch (get_elemental_debuff_type(sn))
   {
   case ELEMENTAL_DEBUFF_ICE:
      apply_spell_debuff_dot(ch, victim, sn, ELEMENT_WATER, 3, 20, debuff_msg);
      return;

   case ELEMENTAL_DEBUFF_FIRE:
      apply_spell_debuff_dot(ch, victim, sn, ELEMENT_FIRE, 3, 24, debuff_msg);
      return;

   case ELEMENTAL_DEBUFF_SHOCK:
      apply_spell_debuff_dot(ch, victim, sn, ELEMENT_AIR, 3, 22, debuff_msg);
      return;

   case ELEMENTAL_DEBUFF_EARTH:
      apply_spell_debuff_dot(ch, victim, sn, ELE_EARTH, 3, 18, debuff_msg);

   default:
      return;
   }
}

static bool is_necromancer_direct_damage_spell(int sn)
{
   return sn == skill_lookup("black hand") || sn == skill_lookup("wraith touch");
}

void apply_necromancer_damage_debuff(CHAR_DATA *ch, CHAR_DATA *victim, int sn, int direct_damage,
                                     OBJ_DATA *obj)
{
   AFFECT_DATA af;
   AFFECT_DATA *paf;
   AFFECT_DATA *paf_next;
   int marker_sn;
   int debuff_count = 0;

   (void)obj;

   if (ch == NULL || victim == NULL || direct_damage <= 0)
      return;

   if (!is_necromancer_direct_damage_spell(sn))
      return;

   marker_sn = skill_lookup("wither shadow");
   if (marker_sn <= 0)
      return;

   af.type = marker_sn;
   af.duration = 6;
   af.duration_type = DURATION_ROUND;
   af.location = APPLY_NONE;
   af.modifier = 0;
   af.bitvector = 0;
   af.caster = ch;
   af.element = ELEMENT_SHADOW;
   affect_to_char(victim, &af);

   for (paf = victim->first_affect; paf != NULL; paf = paf->next)
   {
      if (paf->type == marker_sn && paf->location == APPLY_NONE)
         debuff_count++;
   }

   if (debuff_count < 3)
      return;

   for (paf = victim->first_affect; paf != NULL; paf = paf_next)
   {
      paf_next = paf->next;
      if (paf->type == marker_sn && paf->location == APPLY_NONE)
         affect_remove(victim, paf);
   }

   act("@@dNecrotic marks @@Non $n rupture in a burst of shadow!", victim, NULL, NULL, TO_ROOM);
   send_to_char("@@dNecrotic marks@@N on you rupture in a burst of shadow!\n\r", victim);
   if (ch != victim)
      send_to_char("@@dYour necrotic marks rupture violently!@@N\n\r", ch);

   round_update_dot(victim);
   round_update_dot(victim);
}

bool trigger_elemental_spell_combo(CHAR_DATA *ch, CHAR_DATA *victim, OBJ_DATA *obj, int sn,
                                   int level)
{
   AFFECT_DATA *paf;
   AFFECT_DATA *paf_next;
   bool has_ice_bolt = FALSE;
   bool has_fire_blast = FALSE;
   bool has_shock_storm = FALSE;
   bool has_earth_debuff = FALSE;
   int sn_type = get_elemental_debuff_type(sn);

   for (paf = victim->first_affect; paf != NULL; paf = paf->next)
   {
      if (paf->type == sn)
         continue;
      if (get_elemental_debuff_type(paf->type) == ELEMENTAL_DEBUFF_ICE)
         has_ice_bolt = TRUE;
      else if (get_elemental_debuff_type(paf->type) == ELEMENTAL_DEBUFF_FIRE)
         has_fire_blast = TRUE;
      else if (get_elemental_debuff_type(paf->type) == ELEMENTAL_DEBUFF_SHOCK)
         has_shock_storm = TRUE;
      else if (get_elemental_debuff_type(paf->type) == ELEMENTAL_DEBUFF_EARTH)
         has_earth_debuff = TRUE;
   }

   if (sn_type == ELEMENTAL_DEBUFF_FIRE && (has_ice_bolt || has_earth_debuff))
   {
      int combo_dam = dice(level / 4, 24) + 35;
      act("@@eSteam erupts@@N as fire meets frost around $n!", victim, NULL, NULL, TO_ROOM);
      send_to_char("@@eSteam erupts@@N around you as fire meets frost!\n\r", victim);
      sp_damage(obj, ch, victim, combo_dam, ELEMENT_FIRE | ELEMENT_WATER, sn, TRUE);
   }
   else if (sn_type == ELEMENTAL_DEBUFF_ICE && (has_fire_blast || has_shock_storm))
   {
      int combo_dam = dice(level / 5, 20) + 25;
      act("@@lRime crackles@@N over $n, locking in elemental energy!", victim, NULL, NULL, TO_ROOM);
      send_to_char("@@lRime crackles@@N over you, locking in elemental energy!\n\r", victim);
      sp_damage(obj, ch, victim, combo_dam, ELEMENT_WATER | ELEMENT_AIR, sn, TRUE);
   }
   else if (sn_type == ELEMENTAL_DEBUFF_SHOCK && (has_ice_bolt || has_earth_debuff))
   {
      int combo_dam = dice(level / 4, 22) + 30;
      act("@@lLightning chains@@N through frost coating $n!", victim, NULL, NULL, TO_ROOM);
      send_to_char("@@lLightning chains@@N through the frost coating you!\n\r", victim);
      sp_damage(obj, ch, victim, combo_dam, ELEMENT_AIR | ELEMENT_WATER, sn, TRUE);
   }
   else if (sn_type == ELEMENTAL_DEBUFF_EARTH && (has_fire_blast || has_shock_storm))
   {
      int combo_dam = dice(level / 5, 24) + 28;
      act("@@aFrozen shards@@N explode from the elemental backlash around $n!", victim, NULL, NULL,
          TO_ROOM);
      send_to_char("@@aFrozen shards@@N explode from the elemental backlash around you!\n\r",
                   victim);
      sp_damage(obj, ch, victim, combo_dam, ELE_EARTH, sn, TRUE);
   }
   else
      return FALSE;

   for (paf = victim->first_affect; paf != NULL; paf = paf_next)
   {
      paf_next = paf->next;
      if (get_elemental_debuff_type(paf->type) != ELEMENTAL_DEBUFF_NONE)
         affect_remove(victim, paf);
   }

   return TRUE;
}

/*
 * This file should contain:
 *	o Adept Spells
 *	o additional remort spells
 */
