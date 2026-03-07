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
   return sn == skill_lookup("elemental inferno") ||
          sn == skill_lookup("elemental shock") ||
          sn == skill_lookup("elemental deluge") ||
          sn == skill_lookup("elemental rupture");
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

static bool cast_wizard_elemental_dot_spell(int sn, int level, CHAR_DATA *ch, CHAR_DATA *victim,
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
      int base = ch->remort[CLASS_WIZ];

      if (ch->remort[CLASS_SOR] > base)
         base = ch->remort[CLASS_SOR];

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

   if (sn == skill_lookup("Fire Blast") || sn == skill_lookup("phoenix flare") || sn == skill_lookup("elemental inferno"))
      return ELEMENTAL_DEBUFF_FIRE;

   if (sn == skill_lookup("Shock Storm") || sn == skill_lookup("arc lightning") || sn == skill_lookup("elemental shock"))
      return ELEMENTAL_DEBUFF_SHOCK;

   if (sn == skill_lookup("Earth Shatter") || sn == skill_lookup("terra rend") || sn == skill_lookup("elemental rupture"))
      return ELEMENTAL_DEBUFF_EARTH;

   return ELEMENTAL_DEBUFF_NONE;
}


static void apply_spell_debuff_dot(CHAR_DATA *ch, CHAR_DATA *victim, int sn, int element, int duration, int dot_tick_damage,
                                  const char *debuff_msg)
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

void apply_necromancer_damage_debuff(CHAR_DATA *ch, CHAR_DATA *victim, int sn, int direct_damage, OBJ_DATA *obj)
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

bool trigger_elemental_spell_combo(CHAR_DATA *ch, CHAR_DATA *victim, OBJ_DATA *obj, int sn, int level)
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
      act("@@aFrozen shards@@N explode from the elemental backlash around $n!", victim, NULL, NULL, TO_ROOM);
      send_to_char("@@aFrozen shards@@N explode from the elemental backlash around you!\n\r", victim);
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

bool spell_fireblast(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{
   CHAR_DATA *victim = (CHAR_DATA *)vo;
   int dam;

   if (obj == NULL)
   {
      int base = ch->remort[CLASS_SOR];
      if (ch->remort[CLASS_WIZ] > base)
         base = ch->remort[CLASS_WIZ];
      if (ch->remort[CLASS_NEC] > base)
         base = ch->remort[CLASS_NEC];
      dam = 150 + dice(base/2, 20);
      act("@@gA blast of @@efire@@g flies from $n's hands!@@N", ch, NULL, NULL, TO_ROOM);
      send_to_char("@@gA blast of @@efire@@g flies from your hands!@@N\n\r", ch);
   }
   else
   {
      dam = dice(level / 4, 15);
      act("@@gA blast of @@efire@@g flies from $p!@@N", ch, obj, NULL, TO_ROOM);
      act("@@gA blast of @@efire@@g flies from $p!@@N", ch, obj, NULL, TO_CHAR);
   }
   act("@@g$n is struck by the blast of @@efire@@g!!@@N", victim, NULL, NULL, TO_ROOM);
   send_to_char("@@gYou are struck by the @@efire @@gblast!!@@N\n\r", victim);
   sp_damage(obj, ch, victim, dam, ELE_FIRE, sn, TRUE);
   if (!trigger_elemental_spell_combo(ch, victim, obj, sn, level))
      apply_elemental_spell_debuff(ch, victim, sn, "@@rLingering flames@@N continue to scorch you.\n\r");
   return TRUE;
}

bool spell_shockstorm(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{
   CHAR_DATA *victim = (CHAR_DATA *)vo;
   int dam;

   if (obj == NULL)
   {
      int base = ch->remort[CLASS_SOR];
      if (ch->remort[CLASS_WIZ] > base)
         base = ch->remort[CLASS_WIZ];
      if (ch->remort[CLASS_SOR] > base)
         base = ch->remort[CLASS_SOR];
      if (ch->remort[CLASS_EGO] > base)
         base = ch->remort[CLASS_EGO];
      if (ch->remort[CLASS_NEC] > base)
         base = ch->remort[CLASS_NEC];

      dam = 150 + dice(base/2, 20);
      act("@@gA storm of @@lsparks@@g flies from $n's hands!@@N", ch, NULL, NULL, TO_ROOM);
      send_to_char("@@gA storm of @@lsparks@@g flies from your hands!@@N\n\r", ch);
   }
   else
   {
      dam = dice(level / 4, 20);
      act("@@gA storm of @@lsparks@@g flies from $p!@@N", ch, obj, NULL, TO_ROOM);
      act("@@gA storm of @@lsparks@@g flies from $p!@@N", ch, obj, NULL, TO_CHAR);
   }
   act("@@g$n is struck by the storm of @@lsparks@@g!!@@N", victim, NULL, NULL, TO_ROOM);
   send_to_char("@@gYou are struck by the storm of @@lsparks@@g!!@@N\n\r", victim);
   sp_damage(obj, ch, victim, dam, ELEMENT_AIR, sn, TRUE);
   if (!trigger_elemental_spell_combo(ch, victim, obj, sn, level))
      apply_elemental_spell_debuff(ch, victim, sn, "@@lStatic arcs@@N keep dancing across your skin.\n\r");
   return TRUE;
}

bool spell_cone_cold(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{
   CHAR_DATA *victim = (CHAR_DATA *)vo;
   int dam;

   if (obj == NULL)
   {
      int base = ch->remort[CLASS_SOR];
      if (ch->remort[CLASS_WIZ] > base)
         base = ch->remort[CLASS_WIZ];

      dam = 150 + dice(base/2, 20);
      act("@@gA cone of @@acold@@g bursts forth from $n's hands!@@N", ch, NULL, NULL, TO_ROOM);
      send_to_char("@@gA cone of @@acold@@g bursts forth from your hands!@@N\n\r", ch);
   }
   else
   {
      dam = dice(level / 4, 20);
      act("@@gA cone of @@acold@@g bursts forth from $p!@@N", ch, obj, NULL, TO_ROOM);
      act("@@gA cone of @@acold@@g bursts forth from $p!@@N", ch, obj, NULL, TO_CHAR);
   }
   act("@@g$n is struck by the cone of @@acold@@g!!@@N", victim, NULL, NULL, TO_ROOM);
   send_to_char("@@gYou are struck by the cone of @@acold@@g!!@@N\n\r", victim);
   sp_damage(obj, ch, victim, dam, ELEMENT_WATER, sn, TRUE);
   if (!trigger_elemental_spell_combo(ch, victim, obj, sn, level))
      apply_elemental_spell_debuff(ch, victim, sn, "@@aYou are rimed with deep frost.@@N\n\r");
   return TRUE;
}

bool spell_earth_shatter(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{
   CHAR_DATA *victim = (CHAR_DATA *)vo;
   int dam;

   if (obj == NULL)
   {
      int base = ch->remort[CLASS_SOR];
      if (ch->remort[CLASS_WIZ] > base)
         base = ch->remort[CLASS_WIZ];
      if (ch->remort[CLASS_NEC] > base)
         base = ch->remort[CLASS_NEC];

      dam = 150 + dice(base / 2, 20);
      act("@@gShattering @@bearth@@g erupts from $n's hands!@@N", ch, NULL, NULL, TO_ROOM);
      send_to_char("@@gShattering @@bearth@@g erupts from your hands!@@N\n\r", ch);
   }
   else
   {
      dam = dice(level / 4, 20);
      act("@@gShattering @@bearth@@g erupts from $p!@@N", ch, obj, NULL, TO_ROOM);
      act("@@gShattering @@bearth@@g erupts from $p!@@N", ch, obj, NULL, TO_CHAR);
   }
   act("@@g$n is shattered by jagged @@bearth@@g!!@@N", victim, NULL, NULL, TO_ROOM);
   send_to_char("@@gYou are shattered by jagged @@bearth@@g!!@@N\n\r", victim);
   sp_damage(obj, ch, victim, dam, ELE_EARTH, sn, TRUE);
   if (!trigger_elemental_spell_combo(ch, victim, obj, sn, level))
      apply_elemental_spell_debuff(ch, victim, sn, "@@aYou are rimed with deep frost.@@N\n\r");
   return TRUE;
}

bool spell_holy_wrath(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{
   CHAR_DATA *victim = (CHAR_DATA *)vo;
   int dam;

   if (obj == NULL)
   {
      int base = ch->remort[CLASS_PRI];
      if (ch->remort[CLASS_PAL] > base)
         base = ch->remort[CLASS_PAL];

      dam = 150 + dice(base/2, 20);
      act("@@gA coruscating sphere of @@ylight@@g bursts forth from $n's hands!@@N", ch, NULL, NULL, TO_ROOM);
      send_to_char("@@gA coruscating sphere of @@ylight@@g bursts forth from your hands!@@N\n\r", ch);
   }
   else
   {
      dam = dice(level / 5, 20);
      act("@@gA coruscating sphere of @@ylight@@g bursts forth from $p!@@N", ch, obj, NULL, TO_ROOM);
      act("@@gA coruscating sphere of @@ylight@@g bursts forth from $p!@@N", ch, obj, NULL, TO_CHAR);
   }
   act("@@g$n is struck by the coruscating sphere of @@ylight@@g!!@@N", victim, NULL, NULL, TO_ROOM);
   send_to_char("@@gYou are struck by the coruscating sphere of @@ylight@@g!!@@N\n\r", victim);
   sp_damage(obj, ch, victim, dam, ELE_HOLY, sn, TRUE);
   return TRUE;
}

bool spell_arc_lightning(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{
   CHAR_DATA *victim = (CHAR_DATA *)vo;
   bool combo_triggered;

   if (!cast_wizard_elemental_dot_spell(sn, level, ch, victim, obj,
                                        "@@gA crackling surge of @@ls h o c k@@g bursts from $n's hands!@@N",
                                        "@@g$n is scorched by crackling @@ls h o c k@@g!@@N",
                                        ELEMENT_AIR))
      return FALSE;

   combo_triggered = trigger_elemental_spell_combo(ch, victim, obj, sn, level);
   if (!combo_triggered)
      apply_elemental_spell_debuff(ch, victim, sn, "@@lStatic arcs@@N keep dancing across your skin.\n\r");

   return TRUE;
}

bool spell_terra_rend(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{
   CHAR_DATA *victim = (CHAR_DATA *)vo;
   bool combo_triggered;

   if (!cast_wizard_elemental_dot_spell(sn, level, ch, victim, obj,
                                        "@@gJagged @@bearth@@g erupts from $n's grasp!@@N",
                                        "@@g$n is ripped by jagged @@bearth@@g shards!@@N",
                                        ELE_EARTH))
      return FALSE;

   combo_triggered = trigger_elemental_spell_combo(ch, victim, obj, sn, level);
   if (!combo_triggered)
      apply_elemental_spell_debuff(ch, victim, sn, "@@aYou are rimed with deep frost.@@N\n\r");

   return TRUE;
}

bool spell_tidal_lash(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{
   CHAR_DATA *victim = (CHAR_DATA *)vo;
   bool combo_triggered;

   if (!cast_wizard_elemental_dot_spell(sn, level, ch, victim, obj,
                                        "@@gA crushing wave of @@lwater@@g pours from $n's hands!@@N",
                                        "@@g$n is battered by a crushing wave of @@lwater@@g!@@N",
                                        ELEMENT_WATER))
      return FALSE;

   combo_triggered = trigger_elemental_spell_combo(ch, victim, obj, sn, level);
   if (!combo_triggered)
      apply_elemental_spell_debuff(ch, victim, sn, "@@aFrost clings to your body, slowing your recovery.@@N\n\r");

   return TRUE;
}

bool spell_phoenix_flare(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{
   CHAR_DATA *victim = (CHAR_DATA *)vo;
   bool combo_triggered;

   if (!cast_wizard_elemental_dot_spell(sn, level, ch, victim, obj,
                                        "@@gA searing burst of @@efire@@g roars from $n's hands!@@N",
                                        "@@g$n is engulfed by searing @@efire@@g!@@N",
                                        ELE_FIRE))
      return FALSE;

   combo_triggered = trigger_elemental_spell_combo(ch, victim, obj, sn, level);
   if (!combo_triggered)
      apply_elemental_spell_debuff(ch, victim, sn, "@@rLingering flames@@N continue to scorch you.\n\r");

   return TRUE;
}

bool spell_elemental_inferno(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{
   CHAR_DATA *victim = (CHAR_DATA *)vo;
   bool combo_triggered;

   if (!cast_wizard_elemental_dot_spell(sn, level, ch, victim, obj,
                                        "@@gA grand inferno of @@efire@@g erupts from $n's hands!@@N",
                                        "@@g$n is consumed by the grand @@efire@@g inferno!@@N",
                                        ELE_FIRE))
      return FALSE;

   combo_triggered = trigger_elemental_spell_combo(ch, victim, obj, sn, level);
   if (!combo_triggered)
      apply_elemental_spell_debuff(ch, victim, sn, "@@rLingering flames@@N continue to scorch you.\n\r");

   return TRUE;
}

bool spell_elemental_shock(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{
   CHAR_DATA *victim = (CHAR_DATA *)vo;
   bool combo_triggered;

   if (!cast_wizard_elemental_dot_spell(sn, level, ch, victim, obj,
                                        "@@gA grand surge of @@ls h o c k@@g lashes from $n's hands!@@N",
                                        "@@g$n is blasted by grand @@ls h o c k@@g!@@N",
                                        ELEMENT_AIR))
      return FALSE;

   combo_triggered = trigger_elemental_spell_combo(ch, victim, obj, sn, level);
   if (!combo_triggered)
      apply_elemental_spell_debuff(ch, victim, sn, "@@lStatic arcs@@N keep dancing across your skin.\n\r");

   return TRUE;
}

bool spell_elemental_deluge(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{
   CHAR_DATA *victim = (CHAR_DATA *)vo;
   bool combo_triggered;

   if (!cast_wizard_elemental_dot_spell(sn, level, ch, victim, obj,
                                        "@@gA grand @@lwater@@g deluge crashes from $n's hands!@@N",
                                        "@@g$n is overwhelmed by a grand @@lwater@@g deluge!@@N",
                                        ELEMENT_WATER))
      return FALSE;

   combo_triggered = trigger_elemental_spell_combo(ch, victim, obj, sn, level);
   if (!combo_triggered)
      apply_elemental_spell_debuff(ch, victim, sn, "@@aFrost clings to your body, slowing your recovery.@@N\n\r");

   return TRUE;
}

bool spell_elemental_rupture(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{
   CHAR_DATA *victim = (CHAR_DATA *)vo;
   bool combo_triggered;

   if (!cast_wizard_elemental_dot_spell(sn, level, ch, victim, obj,
                                        "@@gA grand @@bearth@@g rupture tears outward from $n's hands!@@N",
                                        "@@g$n is torn by a grand @@bearth@@g rupture!@@N",
                                        ELE_EARTH))
      return FALSE;

   combo_triggered = trigger_elemental_spell_combo(ch, victim, obj, sn, level);
   if (!combo_triggered)
      apply_elemental_spell_debuff(ch, victim, sn, "@@aYou are rimed with deep frost.@@N\n\r");

   return TRUE;
}

bool spell_wraith_touch(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{
   CHAR_DATA *victim = (CHAR_DATA *)vo;
   int dam;
   float drain_mod = 1.0;

   if (obj == NULL)
   {
      dam = 150 + dice(ch->remort[CLASS_NEC]/2, 20);
      act("@@RA @@dwraithlike hand @@Rleaps forth from $n!@@N", ch, NULL, NULL, TO_ROOM);
      send_to_char("@@RA @@dwraithlike hand @@Rleaps forth from your hands!@@N\n\r", ch);
   }
   else
   {
      dam = 150 + dice(ch->remort[CLASS_NEC]/2, 20);
      act("@@RA @@dwraithlike hand @@Rleaps forth from $p!@@N", ch, obj, NULL, TO_ROOM);
      act("@@RA @@dwraithlike hand @@Rleaps forth from $p!@@N", ch, obj, NULL, TO_CHAR);
   }
   act("@@R$n is struck by the @@dwraithlike hand @@R!!@@N", victim, NULL, NULL, TO_ROOM);
   send_to_char("@@RYou are struck by a @@dwraithlike hand @@R!!@@N\n\r", victim);
   drain_mod = ch->remort[CLASS_NEC] * dam / 130;
   if (sp_damage(obj, ch, victim, dam, ELEMENT_SHADOW | NO_REFLECT | NO_ABSORB, sn, TRUE))
   {
      ch->hit = UMIN(get_max_hp(ch), (ch->hit + drain_mod));
      apply_necromancer_damage_debuff(ch, victim, sn, dam, obj);
   }
   return TRUE;
}

bool spell_thought_vise(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{
   CHAR_DATA *victim = (CHAR_DATA *)vo;
   int dam;

   if (obj == NULL)
   {
      int base = ch->remort[CLASS_EGO];
      dam = 250 + dice(base/2, 20);
      act("@@rA crushing weight brushes your mind, then is gone.@@N", ch, NULL, NULL, TO_ROOM);
      send_to_char("@@rYou clench your mind, crushing the thoughts of your foe.\n\r@@N", ch);
   }
   else
   {
      dam = dice(level / 5, 20);
      act("@@rA crushing weight fills your mind.@@N", ch, obj, NULL, TO_CHAR);
   }
   act("@@rA crushing weight brushes your mind, then is gone.@@N", victim, NULL, NULL, TO_ROOM);
   send_to_char("@@rA crushing weight fills your mind.@@N", victim);
   sp_damage(obj, ch, victim, dam, ELEMENT_MENTAL | NO_REFLECT | NO_ABSORB, sn, TRUE);
   return TRUE;
}

bool spell_black_curse(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{
   CHAR_DATA *victim = (CHAR_DATA *)vo;
   AFFECT_DATA af;

   if (IS_AFFECTED(victim, AFF_REMORT_CURSE))
   {
      send_to_char("They are already weakened!\n\r", ch);
      return FALSE;
   }
   af.type = sn;
   af.duration = 2 * (level / 8);
   af.location = APPLY_HITROLL;
   af.modifier = -1 * get_psuedo_level(ch);
   af.bitvector = AFF_REMORT_CURSE;
   af.caster = ch;
   affect_to_char(victim, &af);

   af.location = APPLY_AC;
   af.modifier = 7 * get_psuedo_level(ch);
   affect_to_char(victim, &af);

   af.location = APPLY_DOT;
   af.modifier = get_psuedo_level(ch);
   affect_to_char(victim, &af);

   send_to_char("@@RA Cloud of @@dDespair@@R washes over you.@@N\n\r", victim);
   if (ch != victim)
      send_to_char("Ok.\n\r", ch);
   return TRUE;
}

bool spell_spirit_curse(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{
   CHAR_DATA *victim = (CHAR_DATA *)vo;
   AFFECT_DATA af;

   if (IS_AFFECTED(victim, AFF_REMORT_CURSE))
   {
      send_to_char("They are already weakened!\n\r", ch);
      return FALSE;
   }
   af.type = sn;
   af.duration = 2 * (level / 8);
   af.location = APPLY_SPELLPOWER;
   af.modifier = -1 * get_psuedo_level(ch);
   af.bitvector = AFF_REMORT_CURSE;
   af.caster = ch;
   affect_to_char(victim, &af);

   af.location = APPLY_DOT;
   af.modifier = get_psuedo_level(ch);
   affect_to_char(victim, &af);

   send_to_char("@@RA Cloud of @@dDespair@@R washes over you.@@N\n\r", victim);
   if (ch != victim)
      send_to_char("Ok.\n\r", ch);
   return TRUE;
}

bool spell_rictus_curse(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{
   CHAR_DATA *victim = (CHAR_DATA *)vo;
   AFFECT_DATA af;

   if (IS_AFFECTED(victim, AFF_REMORT_CURSE))
   {
      send_to_char("They are already weakened!\n\r", ch);
      return FALSE;
   }
   af.type = sn;
   af.bitvector = AFF_REMORT_CURSE;
   af.duration = 2;
   af.caster = ch;
   af.location = APPLY_DOT;
   af.modifier = get_spellpower(ch) * 2 + get_psuedo_level(ch) * 3;
   affect_to_char(victim, &af);

   send_to_char("@@RA Cloud of @@dDespair@@R washes over you.@@N\n\r", victim);
   if (ch != victim)
      send_to_char("Ok.\n\r", ch);
   return TRUE;
}

bool spell_kinetic_reversion(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{
   CHAR_DATA *victim = (CHAR_DATA *)vo;
   AFFECT_DATA af;

   if (IS_AFFECTED(victim, AFF_REMORT_CURSE))
   {
      send_to_char("They are already weakened!\n\r", ch);
      return FALSE;
   }
   af.type = sn;
   af.bitvector = AFF_REMORT_CURSE;
   af.caster = ch;
   af.location = APPLY_DOT;
   af.modifier = get_spellpower(ch) * 3 + get_psuedo_level(ch) * 5;
   affect_to_char(victim, &af);

   send_to_char("@@RA Cloud of @@dDespair@@R washes over you.@@N\n\r", victim);
   if (ch != victim)
      send_to_char("Ok.\n\r", ch);
   return TRUE;
}

bool spell_cloak_misery(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{

   AFFECT_DATA af;

   if (is_affected(ch, sn))
      return FALSE;

   af.type = sn;
   af.duration = get_psuedo_level(ch) / 8;
   af.location = 0;
   af.modifier = 0;
   af.bitvector = 0;
   affect_to_char(ch, &af);

   return TRUE;
}

bool spell_cloak_drain(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{

   AFFECT_DATA af;

   if (is_affected(ch, sn))
      return FALSE;

   af.type = sn;
   af.duration = get_psuedo_level(ch) / 8;
   af.location = 0;
   af.modifier = 0;
   af.bitvector = 0;
   affect_to_char(ch, &af);

   return TRUE;
}

bool spell_cloak_iron(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{

   AFFECT_DATA af;

   if (is_affected(ch, sn))
      return FALSE;

   af.type = sn;
   af.duration = get_psuedo_level(ch) / 8;
   af.location = 0;
   af.modifier = 0;
   af.bitvector = 0;
   affect_to_char(ch, &af);

   return TRUE;
}

bool spell_cloak_mental(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{

   AFFECT_DATA af;

   if (is_affected(ch, sn))
      return FALSE;

   af.type = sn;
   af.duration = get_psuedo_level(ch) / 8;
   af.location = 0;
   af.modifier = 0;
   af.bitvector = 0;
   affect_to_char(ch, &af);

   return TRUE;
}

bool spell_cloak_transcendence(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{

   AFFECT_DATA af;

   if (is_affected(ch, sn))
      return FALSE;

   af.type = sn;
   af.duration = get_psuedo_level(ch) / 8;
   af.location = 0;
   af.modifier = 0;
   af.bitvector = 0;
   affect_to_char(ch, &af);

   return TRUE;
}

bool spell_cloak_oathbreaker(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{

   AFFECT_DATA af;

   if (is_affected(ch, sn))
      return FALSE;

   af.type = sn;
   af.duration = get_psuedo_level(ch) / 8;
   af.location = 0;
   af.modifier = 0;
   af.bitvector = 0;
   affect_to_char(ch, &af);

   return TRUE;
}

bool spell_poison_quinine(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{
   CHAR_DATA *victim = (CHAR_DATA *)vo;
   AFFECT_DATA af;

   if (is_affected(victim, sn))
   {
      send_to_char("They are already poisoned with quinine!\n\r", ch);
      return FALSE;
   }
   if (saves_spell(level, victim))
      if (saves_spell(level, victim))
         return TRUE;
   af.type = sn;
   af.duration = 2 * (level / 8);
   af.location = APPLY_HITROLL;
   af.modifier = -1 * get_psuedo_level(ch) / 20 * get_psuedo_level(victim) / 20;
   af.bitvector = AFF_POISON;
   affect_to_char(victim, &af);

   af.location = APPLY_AC;
   af.modifier = 7 * get_psuedo_level(ch) / 20 * get_psuedo_level(victim) / 20;
   affect_to_char(victim, &af);

   send_to_char("@@You feel a dart pierce your neck.@@N\n\r", victim);
   if (ch != victim)
      send_to_char("Ok.\n\r", ch);
   return TRUE;
}

bool spell_poison_arsenic(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{
   CHAR_DATA *victim = (CHAR_DATA *)vo;
   AFFECT_DATA af;

   if (is_affected(victim, sn))
   {
      send_to_char("They are already poisoned with arsenic!\n\r", ch);
      return FALSE;
   }
   if (saves_spell(level, victim))
      if (saves_spell(level, victim))
         return TRUE;
   af.type = sn;
   af.duration = 2 * (level / 8);
   af.location = APPLY_DAMROLL;
   af.modifier = -1 * get_psuedo_level(ch) / 20 * get_psuedo_level(victim) / 20;
   af.bitvector = AFF_POISON;
   affect_to_char(victim, &af);

   af.location = APPLY_AC;
   af.modifier = 5 * get_psuedo_level(ch) / 20 * get_psuedo_level(victim) / 20;
   affect_to_char(victim, &af);

   send_to_char("@@You feel a dart pierce your neck.@@N\n\r", victim);
   if (ch != victim)
      send_to_char("Ok.\n\r", ch);
   return TRUE;
}

bool spell_sonic_blast(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{
   CHAR_DATA *victim = (CHAR_DATA *)vo;
   AFFECT_DATA af;
   int dam;
   dam = 150 + dice(ch->remort[CLASS_EGO]/2, 20);
   if (sp_damage(obj, ch, victim, dam, ELEMENT_AIR | NO_REFLECT | NO_ABSORB, sn, TRUE))
   {
      if (is_affected(ch, sn))
         return TRUE;
      af.type = sn;
      af.duration = (level / 20);
      af.location = APPLY_DAMROLL;
      af.modifier = -1 * get_psuedo_level(ch) / 15 * get_psuedo_level(victim) / 15;
      af.bitvector = AFF_BLASTED;
      affect_to_char(victim, &af);

      af.type = sn;
      af.duration = (level / 20);
      af.location = APPLY_HITROLL;
      af.modifier = -1 * get_psuedo_level(ch) / 15 * get_psuedo_level(victim) / 15;
      af.bitvector = AFF_BLASTED;
      affect_to_char(victim, &af);
   }
   return TRUE;
}

bool spell_mystical_focus(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{

   AFFECT_DATA af;

   if (is_affected(ch, sn))
      return FALSE;

   af.type = sn;
   af.duration = get_psuedo_level(ch) / 7;
   af.location = 0;
   af.modifier = 0;
   af.bitvector = 0;
   affect_to_char(ch, &af);

   return TRUE;
}

bool spell_purify(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{
   if (is_affected(ch, gsn_poison))
   {
      affect_strip(ch, gsn_poison);
      act("$N looks better.", ch, NULL, ch, TO_NOTVICT);
      send_to_char("Your body feels purified of poison!\n\r", ch);
   }
   else
      send_to_char("You aren't poisoned.\r\n", ch);
   if (can_use_skill(ch, gsn_enhanced_purify) )
   {
      spell_cure_blindness(sn, level, ch, vo, obj);
      spell_remove_curse(sn, level, ch, vo, obj);
   }
   return TRUE;
}

bool spell_regen(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{
   CHAR_DATA *victim = (CHAR_DATA *)vo;
   AFFECT_DATA af;

   int base_heal = spell_regen_base_heal(ch->lvl[CLASS_MAG], ch->remort[CLASS_SOR], ch->remort[CLASS_WIZ], get_spellpower(ch));

   if (is_affected(ch, sn) || is_affected(ch, skill_lookup("regen")))
      return FALSE;
   af.type = sn;
   af.duration = 15 + ch->lvl[CLASS_MAG] / 4;
   af.location = APPLY_HOT;
   af.duration_type = DURATION_ROUND;
   af.modifier = class_heal_character(ch, victim, base_heal, sn, INDEX_MAG, TRUE);
   af.bitvector = 0;
   af.caster = ch;
   affect_to_char(victim, &af);
   act("$N begins to quickly regenerate from $n's spell.", ch, NULL, victim, TO_ROOM);
   send_to_char("You begin to quickly regenerate.\n\r", victim);
   return TRUE;
}

bool spell_psionic_recovery(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{
   CHAR_DATA *victim = (CHAR_DATA *)vo;

   int heal = class_heal_character(ch, victim, 60, sn, INDEX_PSI, FALSE);

   heal_character(ch, victim, heal, sn, FALSE);

   return TRUE;
}

bool spell_group_heal(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{
   CHAR_DATA *vch;

   act("$n casts group heal!", ch, NULL, NULL, TO_ROOM);
   send_to_char("You cast group heal.\n\r", ch);

   for (vch = ch->in_room->first_person; vch != NULL; vch = vch->next_in_room)
   {
      if (is_same_group(ch, vch))
      {
         do_spell_heal(ch, vch, sn);
      }
   }

   return TRUE;
}

bool spell_haste(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{
   CHAR_DATA *victim = (CHAR_DATA *)vo;
   AFFECT_DATA af;

   if (is_affected(ch, skill_lookup("overdrive")) || is_affected(ch, skill_lookup("haste")))
      return FALSE;
   af.type = sn;
   af.duration = 10;
   af.location = APPLY_SPEED;
   af.duration_type = DURATION_ROUND;
   af.modifier = 1;
   af.bitvector = 0;
   af.caster = ch;
   affect_to_char(victim, &af);
   act("$N begins to quickly move from $n's spell.", ch, NULL, victim, TO_ROOM);
   send_to_char("You begin to quickly move.\n\r", victim);
   return TRUE;
}

bool spell_slow(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{
   CHAR_DATA *victim = (CHAR_DATA *)vo;
   AFFECT_DATA af;

   if (is_affected(ch, sn) || is_affected(ch, skill_lookup("slow")))
      return FALSE;
   af.type = sn;
   af.duration = 10;
   af.location = APPLY_SPEED;
   af.duration_type = DURATION_ROUND;
   af.modifier = -1;
   af.bitvector = 0;
   af.caster = ch;
   affect_to_char(victim, &af);
   act("$N begins to slowly move from $n's spell.", ch, NULL, victim, TO_ROOM);
   send_to_char("You begin to slowly move.\n\r", victim);
   return TRUE;
}

bool spell_overdrive(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{
   CHAR_DATA *victim = (CHAR_DATA *)vo;
   AFFECT_DATA af;

   if (ch != victim)
   {
      send_to_char("You can't target someone else with overdrive!\n\r",ch);
      return FALSE;
   }

   if (is_affected(ch, skill_lookup("haste")) || is_affected(ch, skill_lookup("overdrive")))
      return FALSE;
   af.type = sn;
   af.duration = 15;
   af.location = APPLY_SPEED;
   af.duration_type = DURATION_ROUND;
   af.modifier = 1;
   af.bitvector = 0;
   af.caster = ch;
   affect_to_char(ch, &af);
   act("$N begins to quickly move from $n's spell.", ch, NULL, victim, TO_ROOM);
   send_to_char("You begin to quickly move.\n\r", victim);
   return TRUE;
}

bool spell_magical_supremacy(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{
   CHAR_DATA *victim = (CHAR_DATA *)vo;
   AFFECT_DATA af;
   int buff;

   if (ch != victim)
   {
      send_to_char("You can't target someone else for supremacy!\n\r",ch);
      return FALSE;
   }

   if (is_affected(ch, skill_lookup("magical supremacy")))
      return FALSE;

   if (ch->remort[CLASS_EGO] > 0)
      buff = ch->remort[CLASS_EGO] * 1.5;
   else if (ch->remort[CLASS_WIZ] > 0)
      buff = ch->remort[CLASS_WIZ] * 1.5;
   else
      buff = 20;

   af.type = sn;
   af.duration = 15;
   af.location = APPLY_SPELLPOWER;
   af.duration_type = DURATION_ROUND;
   af.modifier = buff;
   af.bitvector = 0;
   af.caster = ch;
   affect_to_char(ch, &af);
   act("$N gains supremacy from $n's spell.", ch, NULL, victim, TO_ROOM);
   send_to_char("You gain supremacy.\n\r", victim);
   return TRUE;
}

bool spell_refuge(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{
   CHAR_DATA *victim = (CHAR_DATA *)vo;
   AFFECT_DATA af;

   if (is_affected(ch, sn) || is_affected(ch, skill_lookup("refuge")))
      return FALSE;
   af.type = sn;
   af.duration = 3;
   af.location = APPLY_SPEED;
   af.duration_type = DURATION_ROUND;
   af.modifier = 1;
   if (ch->adept[CLASS_TEM] > 0)
      af.modifier = 2;
   af.bitvector = 0;
   af.caster = ch;
   affect_to_char(victim, &af);
   act("$N is surrounded by a rainbow shell from $n's spell.", ch, NULL, victim, TO_ROOM);
   send_to_char("You are surrounded by a rainbow shell.\n\r", victim);
   ch->cooldown[sn] = 10;
   return TRUE;
}

bool spell_feeble_mind(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{
   CHAR_DATA *victim = (CHAR_DATA *)vo;
   AFFECT_DATA af;

   if (ch->cooldown[skill_lookup("feeble mind")] > 0 || ch->cooldown[skill_lookup("feeble body")] > 0)
   {
      send_to_char("Your feeble spells are currently on cooldown\n\r",ch);
      return FALSE;
   }

   if (is_affected(ch, sn) || is_affected(ch, skill_lookup("feeble mind")) || is_affected(ch, skill_lookup("feeble body")))
   {
      send_to_char("Your target already is feebly afflicted!\n\r",ch);
      return FALSE;
   }

   ch->cooldown[skill_lookup("feeble mind")] = 10;
   ch->cooldown[skill_lookup("feeble body")] = 10;

   af.type = skill_lookup("feeble mind");
   af.duration = 3;
   af.location = APPLY_SPEED;
   af.duration_type = DURATION_ROUND;
   af.modifier = 0;
   af.bitvector = 0;
   af.caster = ch;
   affect_to_char(victim, &af);
   act("$N begins to think feebly from $n's spell.", ch, NULL, victim, TO_ROOM);
   send_to_char("You begin to think feebly.\n\r", victim);
   return TRUE;
}

bool spell_feeble_body(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{
   CHAR_DATA *victim = (CHAR_DATA *)vo;
   AFFECT_DATA af;

   if (ch->cooldown[skill_lookup("feeble mind")] > 0 || ch->cooldown[skill_lookup("feeble body")] > 0)
   {
      send_to_char("Your feeble spells are currently on cooldown\n\r",ch);
      return FALSE;
   }

   if (is_affected(ch, sn) || is_affected(ch, skill_lookup("feeble mind")) || is_affected(ch, skill_lookup("feeble body")))
   {
      send_to_char("Your target already is feebly afflicted!\n\r",ch);
      return FALSE;
   }


   ch->cooldown[skill_lookup("feeble mind")] = 10;
   ch->cooldown[skill_lookup("feeble body")] = 10;

   af.type = skill_lookup("feeble body");
   af.duration = 3;
   af.location = APPLY_SPEED;
   af.duration_type = DURATION_ROUND;
   af.modifier = 0;
   af.bitvector = 0;
   af.caster = ch;
   affect_to_char(victim, &af);
   act("$N begins to move feebly from $n's spell.", ch, NULL, victim, TO_ROOM);
   send_to_char("You begin to feebly move.\n\r", victim);
   return TRUE;
}
