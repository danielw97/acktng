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

/*
 * The following special functions are available for mobiles.
 */
DECLARE_SPEC_FUN(spec_breath_any);
DECLARE_SPEC_FUN(spec_breath_acid);
DECLARE_SPEC_FUN(spec_breath_fire);
DECLARE_SPEC_FUN(spec_breath_frost);
DECLARE_SPEC_FUN(spec_breath_gas);
DECLARE_SPEC_FUN(spec_breath_lightning);
DECLARE_SPEC_FUN(spec_cast_adept);
DECLARE_SPEC_FUN(spec_cast_cleric);
DECLARE_SPEC_FUN(spec_cast_judge);
DECLARE_SPEC_FUN(spec_cast_mage);
DECLARE_SPEC_FUN(spec_cast_undead);
DECLARE_SPEC_FUN(spec_executioner);
DECLARE_SPEC_FUN(spec_fido);
DECLARE_SPEC_FUN(spec_guard);
DECLARE_SPEC_FUN(spec_janitor);
DECLARE_SPEC_FUN(spec_mayor);
DECLARE_SPEC_FUN(spec_poison);
DECLARE_SPEC_FUN(spec_thief);
DECLARE_SPEC_FUN(spec_policeman);
DECLARE_SPEC_FUN(spec_cast_cadaver);
DECLARE_SPEC_FUN(spec_undead);
DECLARE_SPEC_FUN(spec_stephen);
DECLARE_SPEC_FUN(spec_rewield);
DECLARE_SPEC_FUN(spec_sylai_priest);
DECLARE_SPEC_FUN(spec_cast_bigtime);
DECLARE_SPEC_FUN(spec_sage);
DECLARE_SPEC_FUN(spec_wizardofoz);
DECLARE_SPEC_FUN(spec_vamp_hunter);
DECLARE_SPEC_FUN(spec_mino_guard);
DECLARE_SPEC_FUN(spec_tax_man);
DECLARE_SPEC_FUN(spec_keep_physical_captain);
DECLARE_SPEC_FUN(spec_summon_water);
DECLARE_SPEC_FUN(spec_summon_fire);
DECLARE_SPEC_FUN(spec_summon_earth);
DECLARE_SPEC_FUN(spec_summon_undead);
DECLARE_SPEC_FUN(spec_summon_holy);
DECLARE_SPEC_FUN(spec_summon_shadow);
DECLARE_SPEC_FUN(spec_summon_metal);
DECLARE_SPEC_FUN(spec_summon_animate);
DECLARE_SPEC_FUN(spec_summon_thought);
DECLARE_SPEC_FUN(spec_lamplighter);
DECLARE_SPEC_FUN(spec_warden);
DECLARE_SPEC_FUN(spec_vendor);
DECLARE_SPEC_FUN(spec_lay_sister);
DECLARE_SPEC_FUN(spec_laborer);
DECLARE_SPEC_FUN(spec_kiess_shopkeeper);
DECLARE_SPEC_FUN(spec_kiess_innkeeper);
DECLARE_SPEC_FUN(spec_kiess_scout);
DECLARE_SPEC_FUN(spec_kiess_orator);
DECLARE_SPEC_FUN(spec_kiess_wall_officer);

void do_massivestrike(CHAR_DATA *ch);

/*
 * Given a name, return the appropriate spec fun.
 */

SPEC_FUN *spec_lookup(const char *name)
{
   if (!str_cmp(name, "spec_breath_any"))
      return spec_breath_any;
   if (!str_cmp(name, "spec_breath_acid"))
      return spec_breath_acid;
   if (!str_cmp(name, "spec_breath_fire"))
      return spec_breath_fire;
   if (!str_cmp(name, "spec_breath_frost"))
      return spec_breath_frost;
   if (!str_cmp(name, "spec_breath_gas"))
      return spec_breath_gas;
   if (!str_cmp(name, "spec_breath_lightning"))
      return spec_breath_lightning;
   if (!str_cmp(name, "spec_cast_adept"))
      return spec_cast_adept;
   if (!str_cmp(name, "spec_cast_cleric"))
      return spec_cast_cleric;
   if (!str_cmp(name, "spec_cast_judge"))
      return spec_cast_judge;
   if (!str_cmp(name, "spec_cast_mage"))
      return spec_cast_mage;
   if (!str_cmp(name, "spec_cast_undead"))
      return spec_cast_undead;
   if (!str_cmp(name, "spec_executioner"))
      return spec_executioner;
   if (!str_cmp(name, "spec_fido"))
      return spec_fido;
   if (!str_cmp(name, "spec_guard"))
      return spec_policeman;
   if (!str_cmp(name, "spec_janitor"))
      return spec_janitor;
   if (!str_cmp(name, "spec_mayor"))
      return spec_mayor;
   if (!str_cmp(name, "spec_poison"))
      return spec_poison;
   if (!str_cmp(name, "spec_thief"))
      return spec_thief;
   if (!str_cmp(name, "spec_policeman"))
      return spec_policeman;
   if (!str_cmp(name, "spec_cast_cadaver"))
      return spec_cast_cadaver;
   if (!str_cmp(name, "spec_undead"))
      return spec_undead;
   if (!str_cmp(name, "spec_stephen"))
      return spec_stephen;
   if (!str_cmp(name, "spec_rewield"))
      return spec_rewield;
   if (!str_cmp(name, "spec_sylai_priest"))
      return spec_sylai_priest;
   if (!str_cmp(name, "spec_cast_bigtime"))
      return spec_cast_bigtime;
   if (!str_cmp(name, "spec_sage"))
      return spec_sage;
   if (!str_cmp(name, "spec_wizardofoz"))
      return spec_wizardofoz;
   if (!str_cmp(name, "spec_vamp_hunter"))
      return spec_vamp_hunter;
   if (!str_cmp(name, "spec_mino_guard"))
      return spec_mino_guard;
   if (!str_cmp(name, "spec_tax_man"))
      return spec_tax_man;
   if (!str_cmp(name, "spec_keep_physical_captain"))
      return spec_keep_physical_captain;
   if (!str_cmp(name, "spec_summon_water"))
      return spec_summon_water;
   if (!str_cmp(name, "spec_summon_fire"))
      return spec_summon_fire;
   if (!str_cmp(name, "spec_summon_earth"))
      return spec_summon_earth;
   if (!str_cmp(name, "spec_summon_undead"))
      return spec_summon_undead;
   if (!str_cmp(name, "spec_summon_holy"))
      return spec_summon_holy;
   if (!str_cmp(name, "spec_summon_shadow"))
      return spec_summon_shadow;
   if (!str_cmp(name, "spec_summon_metal"))
      return spec_summon_metal;
   if (!str_cmp(name, "spec_summon_animate"))
      return spec_summon_animate;
   if (!str_cmp(name, "spec_summon_thought"))
      return spec_summon_thought;
   if (!str_cmp(name, "spec_lamplighter"))
      return spec_lamplighter;
   if (!str_cmp(name, "spec_warden"))
      return spec_warden;
   if (!str_cmp(name, "spec_vendor"))
      return spec_vendor;
   if (!str_cmp(name, "spec_lay_sister"))
      return spec_lay_sister;
   if (!str_cmp(name, "spec_laborer"))
      return spec_laborer;
   if (!str_cmp(name, "spec_kiess_shopkeeper"))
      return spec_kiess_shopkeeper;
   if (!str_cmp(name, "spec_kiess_innkeeper"))
      return spec_kiess_innkeeper;
   if (!str_cmp(name, "spec_kiess_scout"))
      return spec_kiess_scout;
   if (!str_cmp(name, "spec_kiess_orator"))
      return spec_kiess_orator;
   if (!str_cmp(name, "spec_kiess_wall_officer"))
      return spec_kiess_wall_officer;

   return 0;
}

/* MAG Mod */

char *rev_spec_lookup(void *func)
{
   if (func == spec_breath_any)
      return "spec_breath_any";
   if (func == spec_breath_acid)
      return "spec_breath_acid";
   if (func == spec_breath_fire)
      return "spec_breath_fire";
   if (func == spec_breath_frost)
      return "spec_breath_frost";
   if (func == spec_breath_gas)
      return "spec_breath_gas";
   if (func == spec_breath_lightning)
      return "spec_breath_lightning";
   if (func == spec_cast_adept)
      return "spec_cast_adept";
   if (func == spec_cast_cleric)
      return "spec_cast_cleric";
   if (func == spec_cast_judge)
      return "spec_cast_judge";
   if (func == spec_cast_mage)
      return "spec_cast_mage";
   if (func == spec_cast_undead)
      return "spec_cast_undead";
   if (func == spec_executioner)
      return "spec_executioner";
   if (func == spec_fido)
      return "spec_fido";
   if (func == spec_guard)
      return "spec_policeman";
   if (func == spec_janitor)
      return "spec_janitor";
   if (func == spec_mayor)
      return "spec_mayor";
   if (func == spec_poison)
      return "spec_poison";
   if (func == spec_thief)
      return "spec_thief";
   if (func == spec_policeman)
      return "spec_policeman";
   if (func == spec_cast_cadaver)
      return "spec_cast_cadaver";
   if (func == spec_undead)
      return "spec_undead";
   if (func == spec_stephen)
      return "spec_stephen";
   if (func == spec_rewield)
      return "spec_rewield";
   if (func == spec_sylai_priest)
      return "spec_sylai_priest";
   if (func == spec_cast_bigtime)
      return "spec_cast_bigtime";
   if (func == spec_sage)
      return "spec_sage";
   if (func == spec_wizardofoz)
      return "spec_wizardofoz";
   if (func == spec_vamp_hunter)
      return "spec_vamp_hunter";
   if (func == spec_mino_guard)
      return "spec_mino_guard";
   if (func == spec_tax_man)
      return "spec_tax_man";
   if (func == spec_keep_physical_captain)
      return "spec_keep_physical_captain";
   if (func == spec_summon_water)
      return "spec_summon_water";
   if (func == spec_summon_fire)
      return "spec_summon_fire";
   if (func == spec_summon_earth)
      return "spec_summon_earth";
   if (func == spec_summon_undead)
      return "spec_summon_undead";
   if (func == spec_summon_holy)
      return "spec_summon_holy";
   if (func == spec_summon_shadow)
      return "spec_summon_shadow";
   if (func == spec_summon_metal)
      return "spec_summon_metal";
   if (func == spec_summon_animate)
      return "spec_summon_animate";
   if (func == spec_summon_thought)
      return "spec_summon_thought";
   if (func == spec_lamplighter)
      return "spec_lamplighter";
   if (func == spec_warden)
      return "spec_warden";
   if (func == spec_vendor)
      return "spec_vendor";
   if (func == spec_lay_sister)
      return "spec_lay_sister";
   if (func == spec_laborer)
      return "spec_laborer";
   if (func == spec_kiess_shopkeeper)
      return "spec_kiess_shopkeeper";
   if (func == spec_kiess_innkeeper)
      return "spec_kiess_innkeeper";
   if (func == spec_kiess_scout)
      return "spec_kiess_scout";
   if (func == spec_kiess_orator)
      return "spec_kiess_orator";
   if (func == spec_kiess_wall_officer)
      return "spec_kiess_wall_officer";

   return 0;
}

void print_spec_lookup(char *buf)
{
   strcat(buf, "       spec_breath_any         \n\r");
   strcat(buf, "       spec_breath_acid        \n\r");
   strcat(buf, "       spec_breath_fire        \n\r");
   strcat(buf, "       spec_breath_frost       \n\r");
   strcat(buf, "       spec_breath_gas         \n\r");
   strcat(buf, "       spec_breath_lightning   \n\r");
   strcat(buf, "       spec_cast_adept         \n\r");
   strcat(buf, "       spec_cast_cleric        \n\r");
   strcat(buf, "       spec_cast_judge         \n\r");
   strcat(buf, "       spec_cast_mage          \n\r");
   strcat(buf, "       spec_cast_undead        \n\r");
   strcat(buf, "       spec_executioner        \n\r");
   strcat(buf, "       spec_fido               \n\r");
   strcat(buf, "       spec_guard              \n\r");
   strcat(buf, "       spec_janitor            \n\r");
   strcat(buf, "       spec_mayor              \n\r");
   strcat(buf, "       spec_poison             \n\r");
   strcat(buf, "       spec_thief              \n\r");
   strcat(buf, "       spec_policeman          \n\r");
   strcat(buf, "       spec_cast_cadaver       \n\r");
   strcat(buf, "       spec_undead             \n\r");
   strcat(buf, "       spec_rewield            \n\r");
   strcat(buf, "	     spec_cast_bigtime       \n\r");
   strcat(buf, "       spec_wizardofoz         \n\r");
   strcat(buf, "       spec_vamp_hunter (Int mobs only) \n\r");
   strcat(buf, "       spec_mino_guard \n\r");
   strcat(buf, "       spec_tax_man \n\r");
   strcat(buf, "       spec_keep_physical_captain\n\r");
   strcat(buf, "       spec_summon_water        \n\r");
   strcat(buf, "       spec_summon_fire         \n\r");
   strcat(buf, "       spec_summon_earth        \n\r");
   strcat(buf, "       spec_summon_undead       \n\r");
   strcat(buf, "       spec_summon_holy         \n\r");
   strcat(buf, "       spec_summon_shadow       \n\r");
   strcat(buf, "       spec_summon_metal        \n\r");
   strcat(buf, "       spec_summon_animate      \n\r");
   strcat(buf, "       spec_summon_thought      \n\r");
   strcat(buf, "       spec_lamplighter         \n\r");
   strcat(buf, "       spec_warden              \n\r");
   strcat(buf, "       spec_vendor              \n\r");
   strcat(buf, "       spec_lay_sister          \n\r");
   strcat(buf, "       spec_laborer             \n\r");
   strcat(buf, "       spec_kiess_shopkeeper   \n\r");
   strcat(buf, "       spec_kiess_innkeeper    \n\r");
   strcat(buf, "       spec_kiess_scout        \n\r");
   strcat(buf, "       spec_kiess_orator       \n\r");
   strcat(buf, "       spec_kiess_wall_officer \n\r");

   return;
}

/*
 * Core procedure for dragons.
 */
bool dragon(CHAR_DATA *ch, char *spell_name)
{
   CHAR_DATA *victim;
   int sn;

   if (!is_fighting(ch))
      return FALSE;

   for (victim = ch->in_room->first_person; victim != NULL; victim = victim->next_in_room)
      if (victim->fighting == ch && number_bits(2) == 0)
         break;

   if (victim == NULL)
      return FALSE;

   if ((sn = skill_lookup(spell_name)) < 0)
      return FALSE;
   (*skill_table[sn].spell_fun)(sn, ch->level, ch, victim, NULL);
   return TRUE;
}

bool is_player_summon_special(SPEC_FUN *spec_fun)
{
   return spec_fun == spec_summon_water
      || spec_fun == spec_summon_fire
      || spec_fun == spec_summon_earth
      || spec_fun == spec_summon_undead
      || spec_fun == spec_summon_holy
      || spec_fun == spec_summon_shadow
      || spec_fun == spec_summon_metal
      || spec_fun == spec_summon_animate
      || spec_fun == spec_summon_thought;
}

static bool spec_summon_cast_random(CHAR_DATA *ch, CHAR_DATA *target, const char *const *spells, int spell_count)
{
   int i;
   int start;

   if (!is_fighting(ch) || target == NULL || spell_count <= 0)
      return FALSE;

   start = number_range(0, spell_count - 1);

   for (i = 0; i < spell_count; i++)
   {
      int index = (start + i) % spell_count;
      int sn = skill_lookup(spells[index]);

      if (sn < 0)
         continue;

      (*skill_table[sn].spell_fun)(sn, ch->level, ch, target, NULL);
      return TRUE;
   }

   return FALSE;
}

static int summon_master_heal_chance(int master_hit, int master_max_hp, int thematic_bonus)
{
   int master_hp_pct;
   int chance_to_heal;

   if (master_max_hp <= 0 || master_hit >= master_max_hp)
      return 0;

   master_hp_pct = UMAX(0, (master_hit * 100) / master_max_hp);
   chance_to_heal = thematic_bonus + (100 - master_hp_pct);

   return URANGE(5, chance_to_heal, 95);
}

#ifdef UNIT_TEST_SPECIAL
int summon_master_heal_chance_for_test(int master_hit, int master_max_hp, int thematic_bonus)
{
   return summon_master_heal_chance(master_hit, master_max_hp, thematic_bonus);
}

int summon_special_count_for_test(void)
{
   return 9;
}

bool summon_special_casts_in_combat_for_test(CHAR_DATA *ch, int index)
{
   switch (index)
   {
   case 0:
      return spec_summon_water(ch);
   case 1:
      return spec_summon_fire(ch);
   case 2:
      return spec_summon_earth(ch);
   case 3:
      return spec_summon_undead(ch);
   case 4:
      return spec_summon_holy(ch);
   case 5:
      return spec_summon_shadow(ch);
   case 6:
      return spec_summon_metal(ch);
   case 7:
      return spec_summon_animate(ch);
   case 8:
      return spec_summon_thought(ch);
   default:
      return FALSE;
   }
}
#endif

static bool spec_summon_heal_master(CHAR_DATA *ch, int thematic_bonus)
{
   int sn;
   int max_hp;
   int chance_to_heal;

   if (ch->master == NULL || ch->master->in_room != ch->in_room)
      return FALSE;

   max_hp = get_max_hp(ch->master);
   if (max_hp <= 0)
      return FALSE;

   if (ch->master->hit >= max_hp)
      return FALSE;

   chance_to_heal = summon_master_heal_chance(ch->master->hit, max_hp, thematic_bonus);

   if (chance_to_heal <= 0 || number_range(1, 100) > chance_to_heal)
      return FALSE;

   sn = skill_lookup("heal");
   if (sn < 0)
      return FALSE;

   (*skill_table[sn].spell_fun)(sn, ch->level, ch, ch->master, NULL);
   return TRUE;
}

bool spec_summon_water(CHAR_DATA *ch)
{
   static const char *const spells[] = {"chill touch", "acid blast", "weaken"};

   if (spec_summon_heal_master(ch, 30))
      return TRUE;

   return spec_summon_cast_random(ch, ch->fighting, spells, 3);
}

bool spec_summon_fire(CHAR_DATA *ch)
{
   static const char *const spells[] = {"fireball", "high explosive", "curse"};

   if (spec_summon_heal_master(ch, 12))
      return TRUE;

   return spec_summon_cast_random(ch, ch->fighting, spells, 3);
}

bool spec_summon_earth(CHAR_DATA *ch)
{
   static const char *const spells[] = {"earthquake", "acid blast", "weaken"};

   if (spec_summon_heal_master(ch, 20))
      return TRUE;

   return spec_summon_cast_random(ch, ch->fighting, spells, 3);
}

bool spec_summon_undead(CHAR_DATA *ch)
{
   static const char *const spells[] = {"harm", "chill touch", "poison"};

   if (spec_summon_heal_master(ch, 8))
      return TRUE;

   return spec_summon_cast_random(ch, ch->fighting, spells, 3);
}

bool spec_summon_holy(CHAR_DATA *ch)
{
   static const char *const spells[] = {"holy wrath", "dispel evil", "curse"};

   if (spec_summon_heal_master(ch, 40))
      return TRUE;

   return spec_summon_cast_random(ch, ch->fighting, spells, 3);
}

bool spec_summon_shadow(CHAR_DATA *ch)
{
   static const char *const spells[] = {"energy drain", "curse", "weaken"};

   if (spec_summon_heal_master(ch, 10))
      return TRUE;

   return spec_summon_cast_random(ch, ch->fighting, spells, 3);
}

bool spec_summon_metal(CHAR_DATA *ch)
{
   static const char *const spells[] = {"acid blast", "lightning bolt", "high explosive"};

   if (spec_summon_heal_master(ch, 15))
      return TRUE;

   return spec_summon_cast_random(ch, ch->fighting, spells, 3);
}

bool spec_summon_animate(CHAR_DATA *ch)
{
   static const char *const spells[] = {"harm", "poison", "weaken"};

   if (spec_summon_heal_master(ch, 8))
      return TRUE;

   return spec_summon_cast_random(ch, ch->fighting, spells, 3);
}

bool spec_summon_thought(CHAR_DATA *ch)
{
   static const char *const spells[] = {"thought vise", "mind bolt", "feeble mind"};

   if (spec_summon_heal_master(ch, 12))
      return TRUE;

   return spec_summon_cast_random(ch, ch->fighting, spells, 3);
}

/*
 * Special procedures for mobiles.
 */
bool spec_breath_any(CHAR_DATA *ch)
{
   if (!is_fighting(ch))
      return FALSE;

   switch (number_bits(3))
   {
   case 0:
      return spec_breath_fire(ch);
   case 1:
   case 2:
      return spec_breath_lightning(ch);
   case 3:
      return spec_breath_gas(ch);
   case 4:
      return spec_breath_acid(ch);
   case 5:
   case 6:
   case 7:
      return spec_breath_frost(ch);
   }

   return FALSE;
}

bool spec_breath_acid(CHAR_DATA *ch)
{
   return dragon(ch, "acid breath");
}

bool spec_breath_fire(CHAR_DATA *ch)
{
   return dragon(ch, "fire breath");
}

bool spec_breath_frost(CHAR_DATA *ch)
{
   return dragon(ch, "frost breath");
}

bool spec_breath_gas(CHAR_DATA *ch)
{
   int sn;

   if (!is_fighting(ch))
      return FALSE;

   if ((sn = skill_lookup("gas breath")) < 0)
      return FALSE;
   (*skill_table[sn].spell_fun)(sn, ch->level, ch, NULL, NULL);
   return TRUE;
}

bool spec_breath_lightning(CHAR_DATA *ch)
{
   return dragon(ch, "lightning breath");
}

bool spec_cast_adept(CHAR_DATA *ch)
{
   CHAR_DATA *victim;
   int cl; /* cast level */

   if (!IS_AWAKE(ch))
      return FALSE;

   if (is_fighting(ch))
      return FALSE;

   if (ch->in_room != NULL)
   {
      if (!IS_SET(ch->in_room->affected_by, ROOM_BV_HEAL_REGEN))
         spell_healing_light(skill_lookup("healing light"), 79, ch, NULL, NULL);
      if (!IS_SET(ch->in_room->affected_by, ROOM_BV_MANA_REGEN))
         spell_mana_flare(skill_lookup("mana flare"), 79, ch, NULL, NULL);
   }

   for (victim = ch->in_room->first_person; victim != NULL; victim = victim->next_in_room)
      if (victim != ch && can_see(ch, victim) && number_bits(1) == 0)
         break;

   if (victim == NULL)
      return FALSE;

   cl = number_range(1, ch->level);

   switch (number_bits(3))
   {
   case 0:
      act("$n utters the word 'tehctah'.", ch, NULL, NULL, TO_ROOM);
      spell_armor(skill_lookup("armor"), cl, ch, victim, NULL);
      return TRUE;

   case 1:
      act("$n utters the word 'nhak'.", ch, NULL, NULL, TO_ROOM);
      spell_bless(skill_lookup("bless"), cl, ch, victim, NULL);
      return TRUE;

   case 2:
      act("$n utters the word 'yeruf'.", ch, NULL, NULL, TO_ROOM);
      spell_cure_blindness(skill_lookup("cure blindness"), cl, ch, victim, NULL);
      return TRUE;

   case 3:
      act("$n utters the word 'garf'.", ch, NULL, NULL, TO_ROOM);
      spell_cure_light(skill_lookup("cure light"), cl, ch, victim, NULL);
      return TRUE;

   case 4:
      act("$n utters the words 'rozar'.", ch, NULL, NULL, TO_ROOM);
      spell_cure_poison(skill_lookup("cure poison"), cl, ch, victim, NULL);
      return TRUE;

   case 5:
   {
      char buffer[MSL];
      double seconds;
      int days;
      time_t currentDate;
      struct tm *xmas, today;

      time(&currentDate);

      today = *localtime(&currentDate);

      xmas = localtime(&currentDate);
      xmas->tm_mon = 11;
      xmas->tm_mday = 25;
      if (today.tm_mday > 25 && today.tm_mon == 11)
         xmas->tm_year = today.tm_year + 1;

      seconds = difftime(mktime(xmas), currentDate);
      days = seconds / 86400;

      sprintf(buffer, "$n utters the words '%d days to Christmas!'.", days);

      act(buffer, ch, NULL, NULL, TO_ROOM);
      spell_refresh(skill_lookup("refresh"), ch->level, ch, victim, NULL);
      return TRUE;
   }
   }

   return FALSE;
}

bool spec_cast_cleric(CHAR_DATA *ch)
{
   CHAR_DATA *victim;
   char *spell;
   int sn;

   if (!is_fighting(ch))
      return FALSE;

   for (victim = ch->in_room->first_person; victim != NULL; victim = victim->next_in_room)
      if (victim->fighting == ch && number_bits(2) == 0)
         break;

   if (victim == NULL)
      return FALSE;

   for (;;)
   {
      int min_level;

      switch (number_bits(4))
      {
      case 0:
         min_level = 0;
         spell = "blindness";
         break;
      case 1:
         min_level = 3;
         spell = "cause serious";
         break;
      case 2:
         min_level = 7;
         spell = "earthquake";
         break;
      case 3:
         min_level = 9;
         spell = "cause critical";
         break;
      case 4:
         min_level = 10;
         spell = "dispel evil";
         break;
      case 5:
         min_level = 12;
         spell = "curse";
         break;
      case 6:
         min_level = 12;
         spell = "change sex";
         break;
      case 7:
         min_level = 13;
         spell = "flamestrike";
         break;
      case 8:
      case 9:
      case 10:
         min_level = 15;
         spell = "harm";
         break;
      default:
         min_level = 16;
         spell = "dispel magic";
         break;
      }

      if (ch->level >= min_level)
         break;
      /*
       * Can add infinite loop checking here, but its kinda pointless,
       * * as no mob can have ch->level < 0
       */
   }

   if ((sn = skill_lookup(spell)) < 0)
      return FALSE;

   act("The eyes of $n glow brightly!", ch, NULL, NULL, TO_ROOM);
   (*skill_table[sn].spell_fun)(sn, ch->level, ch, victim, NULL);
   return TRUE;
}

bool spec_cast_judge(CHAR_DATA *ch)
{
   CHAR_DATA *victim;
   char *spell;
   int sn;

   if (!is_fighting(ch))
      return FALSE;

   for (victim = ch->in_room->first_person; victim != NULL; victim = victim->next_in_room)
      if (victim->fighting == ch && number_bits(2) == 0)
         break;

   if (victim == NULL)
      return FALSE;

   spell = "high explosive";
   if ((sn = skill_lookup(spell)) < 0)
      return FALSE;
   (*skill_table[sn].spell_fun)(sn, ch->level, ch, victim, NULL);
   return TRUE;
}

bool spec_cast_mage(CHAR_DATA *ch)
{
   CHAR_DATA *victim;
   char *spell;
   int sn;

   if (!is_fighting(ch))
      return FALSE;

   for (victim = ch->in_room->first_person; victim != NULL; victim = victim->next_in_room)
      if (victim->fighting == ch && number_bits(2) == 0)
         break;

   if (victim == NULL)
      return FALSE;

   for (;;)
   {
      int min_level;

      switch (number_bits(4))
      {
      case 0:
         min_level = 0;
         spell = "blindness";
         break;
      case 1:
         min_level = 3;
         spell = "chill touch";
         break;
      case 2:
         min_level = 7;
         spell = "weaken";
         break;
      case 3:
         min_level = 8;
         spell = "teleport";
         break;
      case 4:
         min_level = 11;
         spell = "color spray";
         break;
      case 5:
         min_level = 12;
         spell = "change sex";
         break;
      case 6:
      case 7:
      case 8:
      case 9:
         min_level = 15;
         spell = "fireball";
         break;
      default:
         min_level = 20;
         spell = "acid blast";
         break;
      }

      if (ch->level >= min_level)
         break;
      /*
       * Can add infinite loop checking here, but its kinda pointless,
       * * as no mob can have ch->level < 0.  -- Alty
       */
   }

   if ((sn = skill_lookup(spell)) < 0)
      return FALSE;

   act("An eerie sound comes from $n as $e stares at $N!!", ch, NULL, victim, TO_ROOM);
   (*skill_table[sn].spell_fun)(sn, ch->level, ch, victim, NULL);
   return TRUE;
}

bool spec_cast_undead(CHAR_DATA *ch)
{
   CHAR_DATA *victim;
   char *spell;
   int sn;

   if (!is_fighting(ch))
      return FALSE;

   for (victim = ch->in_room->first_person; victim != NULL; victim = victim->next_in_room)
      if (victim->fighting == ch && number_bits(2) == 0)
         break;

   if (victim == NULL)
      return FALSE;

   for (;;)
   {
      int min_level;

      switch (number_bits(4))
      {
      case 0:
         min_level = 0;
         spell = "curse";
         break;
      case 1:
         min_level = 3;
         spell = "weaken";
         break;
      case 2:
         min_level = 6;
         spell = "chill touch";
         break;
      case 3:
         min_level = 9;
         spell = "blindness";
         break;
      case 4:
         min_level = 12;
         spell = "poison";
         break;
      case 5:
      case 6:
         min_level = 18;
         spell = "harm";
         break;
      case 7:
         min_level = 21;
         spell = "teleport";
         break;
      default:
         min_level = 24;
         spell = "gate";
         break;
      }

      if (ch->level >= min_level)
         break;
      /*
       * Can add infinite loop checking here, but its kinda pointless,
       * * as no mob can have ch->level < 0. -- Alty
       */
   }

   if ((sn = skill_lookup(spell)) < 0)
      return FALSE;
   act("$n summons forth the powers of darkness!", ch, NULL, NULL, TO_ROOM);
   (*skill_table[sn].spell_fun)(sn, ch->level, ch, victim, NULL);
   return TRUE;
}

bool spec_executioner(CHAR_DATA *ch)
{
   char buf[MAX_STRING_LENGTH];
   CHAR_DATA *victim;
   char *crime;
   bool undead;

   undead = FALSE;

   if (!IS_AWAKE(ch) || is_fighting(ch))
      return FALSE;

   crime = "";
   for (victim = ch->in_room->first_person; victim != NULL; victim = victim->next_in_room)
   {
      if (IS_VAMP(victim) && IS_AFFECTED(victim, AFF_VAMP_HEALING))
         continue;

      if (!IS_NPC(victim) && IS_SET(victim->act, PLR_KILLER))
      {
         crime = "KILLER";
         break;
      }

      if (!IS_NPC(victim) && IS_SET(victim->act, PLR_THIEF))
      {
         crime = "THIEF";
         break;
      }
      if (IS_NPC(victim) && IS_SET(victim->act, ACT_UNDEAD))
      {
         crime = "UNDEAD";
         undead = TRUE;
         break;
      }
   }

   if (victim == NULL)
      return FALSE;

   if (undead)
      sprintf(buf, "BANZAI!!! UNDEAD HAVE ARRIVED!!! PROTECT THE LIVING!!!");
   else
      sprintf(buf, "%s is a %s!  PROTECT THE INNOCENT!  MORE BLOOOOD!!!", victim->name, crime);

   do_yell(ch, buf);
   multi_hit(ch, victim, TYPE_UNDEFINED);
   char_to_room(create_mobile(get_mob_index(MOB_VNUM_CITYGUARD)), ch->in_room);
   char_to_room(create_mobile(get_mob_index(MOB_VNUM_CITYGUARD)), ch->in_room);
   return TRUE;
}

bool spec_mino_guard(CHAR_DATA *ch)
{
   CHAR_DATA *victim;
   OBJ_DATA *pass;

   if (!IS_AWAKE(ch) || is_fighting(ch))
      return FALSE;

   for (victim = ch->in_room->first_person; victim != NULL; victim = victim->next_in_room)
   {
      if (victim == NULL || ch == NULL)
         continue;
      if (IS_VAMP(victim) && IS_AFFECTED(victim, AFF_VAMP_HEALING))
         continue;
      if ((IS_NPC(victim)) || (ch == victim) || (IS_IMMORTAL(victim)))
         continue;
      pass = get_eq_char(victim, WEAR_HOLD_HAND_R);
      if (pass != NULL && pass->pIndexData->vnum == OBJ_VNUM_MINO_PASS)
      {
         do_say(ch, "@@eLet this one pass@@N");
         continue;
      }

      pass = get_eq_char(victim, WEAR_HOLD_HAND_L);
      if (pass != NULL && pass->pIndexData->vnum == OBJ_VNUM_MINO_PASS)
      {
         do_say(ch, "@@eLet this one pass@@N");
         continue;
      }

      do_yell(ch, "Intruder Alert! Man the Walls!");
      multi_hit(ch, victim, TYPE_UNDEFINED);

      return TRUE;
   }

   return FALSE;
}

bool spec_fido(CHAR_DATA *ch)
{
   OBJ_DATA *corpse;
   OBJ_DATA *obj;
   OBJ_DATA *obj_next;

   if (!IS_AWAKE(ch))
      return FALSE;

   for (corpse = ch->in_room->first_content; corpse != NULL; corpse = corpse->next_in_room)
   {
      if (corpse->item_type != ITEM_CORPSE_NPC || number_bits(1) != 0)
         continue;

      act("$n savagely devours a corpse.", ch, NULL, NULL, TO_ROOM);
      for (obj = corpse->first_in_carry_list; obj; obj = obj_next)
      {
         obj_next = obj->next_in_carry_list;
         obj_from_obj(obj);
         obj_to_room(obj, ch->in_room);
      }
      extract_obj(corpse);
      return TRUE;
   }

   return FALSE;
}

bool spec_guard(CHAR_DATA *ch)
{
   return spec_policeman(ch);
}

bool spec_janitor(CHAR_DATA *ch)
{
   OBJ_DATA *trash;

   if (!IS_AWAKE(ch))
      return FALSE;

   for (trash = ch->in_room->first_content; trash != NULL; trash = trash->next_in_room)
   {
      if (!IS_SET(trash->wear_flags, ITEM_TAKE))
         continue;
      if (trash->item_type == ITEM_DRINK_CON || trash->item_type == ITEM_TRASH || trash->cost < 10)
      {
         act("$n picks up some trash.", ch, NULL, NULL, TO_ROOM);
         obj_from_room(trash);
         obj_to_char(trash, ch);
         return TRUE;
      }
   }

   return FALSE;
}

bool spec_mayor(CHAR_DATA *ch)
{
   static const char open_path[] = "W3a3003b33000c111d0d111Oe333333Oe22c222112212111a1S.";

   static const char close_path[] = "W3a3003b33000c111d0d111CE333333CE22c222112212111a1S.";

   static const char *path;
   static int pos;
   static bool move;
   static int last_route_start_hour = -1;

   if (!move)
   {
      if (time_info.hour == 6 && last_route_start_hour != 6)
      {
         path = open_path;
         move = TRUE;
         pos = 0;
         last_route_start_hour = 6;
      }

      if (time_info.hour == 20 && last_route_start_hour != 20)
      {
         path = close_path;
         move = TRUE;
         pos = 0;
         last_route_start_hour = 20;
      }
   }

   if (is_fighting(ch))
      return spec_cast_cleric(ch);
   if (!move || ch->position < POS_SLEEPING)
      return FALSE;

   switch (path[pos])
   {
   case '0':
   case '1':
   case '2':
   case '3':
      move_char(ch, path[pos] - '0');
      break;

   case 'W':
      ch->position = POS_STANDING;
      act("$n awakens and groans loudly.", ch, NULL, NULL, TO_ROOM);
      break;

   case 'S':
      ch->position = POS_SLEEPING;
      act("$n lies down and falls asleep.", ch, NULL, NULL, TO_ROOM);
      break;

   case 'a':
      act("$n says 'Hello Honey!'", ch, NULL, NULL, TO_ROOM);
      break;

   case 'b':
      act("$n says 'What a view!  I must do something about that dump!'", ch, NULL, NULL, TO_ROOM);
      break;

   case 'c':
      act("$n says 'Vandals!  Youngsters have no respect for anything!'", ch, NULL, NULL, TO_ROOM);
      break;

   case 'd':
      act("$n says 'Good day, citizens!'", ch, NULL, NULL, TO_ROOM);
      break;

   case 'e':
      do_yell(ch, "I hereby declare the city of Midgaard open!");
      break;

   case 'E':
      do_yell(ch, "I hereby declare the city of Midgaard closed!");
      break;

   case 'O':
      do_unlock(ch, "gate");
      do_open(ch, "gate");
      break;

   case 'C':
      do_close(ch, "gate");
      do_lock(ch, "gate");
      break;

   case '.':
      move = FALSE;
      break;
   }

   pos++;
   return FALSE;
}

bool spec_poison(CHAR_DATA *ch)
{
   CHAR_DATA *victim;

   if (!is_fighting(ch) || (victim = ch->fighting) == NULL || number_percent() > 2 * ch->level)
      return FALSE;

   act("You bite $N!", ch, NULL, victim, TO_CHAR);
   act("$n bites $N!", ch, NULL, victim, TO_NOTVICT);
   act("$n bites you!", ch, NULL, victim, TO_VICT);
   spell_poison(gsn_poison, ch->level, ch, victim, NULL);
   one_hit(ch, victim, TYPE_HIT + 8);
   return TRUE;
}

bool spec_thief(CHAR_DATA *ch)
{
   CHAR_DATA *victim;
   int gold;

   if (ch->position != POS_STANDING)
      return FALSE;

   for (victim = ch->in_room->first_person; victim != NULL; victim = victim->next_in_room)
   {
      if (IS_NPC(victim) || victim->level >= LEVEL_IMMORTAL || number_bits(2) != 0 || !can_see(ch, victim)) /* Thx Glop */
         continue;

      if (IS_AWAKE(victim) && number_range(0, ch->level) == 0)
      {
         act("You discover $n's hands in your wallet!", ch, NULL, victim, TO_VICT);
         act("$N discovers $n's hands in $S wallet!", ch, NULL, victim, TO_NOTVICT);
         return TRUE;
      }
      else
      {
         gold = victim->gold * number_range(1, 20) / 80;
         ch->gold += 7 * gold / 8;
         victim->gold -= gold;
         return TRUE;
      }
   }

   return FALSE;
}

bool spec_policeman(CHAR_DATA *ch)
{
   char buf[MAX_STRING_LENGTH];
   CHAR_DATA *victim;
   char *crime;

   if (!IS_AWAKE(ch) || is_fighting(ch))
      return FALSE;

   crime = "";
   for (victim = ch->in_room->first_person; victim != NULL; victim = victim->next_in_room)
   {
      if (IS_VAMP(victim) && IS_AFFECTED(victim, AFF_VAMP_HEALING))
         continue;

      if (!IS_NPC(victim) && IS_SET(victim->act, PLR_KILLER))
      {
         crime = "KILLER";
         break;
      }

      if (!IS_NPC(victim) && IS_SET(victim->act, PLR_THIEF))
      {
         crime = "THIEF";
         break;
      }
   }

   if (victim == NULL)
      return FALSE;

   if (ch->hunting || ch->hunt_obj)
      /*
       * if ( (int) (ch->hunting) > 0)
       */
      /*
       * Don't hunt someone if already hunting
       */
      return FALSE;

   sprintf(buf, "%s is a %s, I shall not rest till justice is done!", victim->name, crime);
   do_yell(ch, buf);

   set_hunt(ch, NULL, victim, NULL, 0, 0);

   return TRUE;
}

bool spec_cast_cadaver(CHAR_DATA *ch)
{
   /*
    * Quick rip-off of spec_cast_adept, to make cadaver cast detect invis
    * * with short duration. -S-
    */

   CHAR_DATA *victim;
   /*    CHAR_DATA *vch;    */

   if (!IS_AWAKE(ch))
      return FALSE;

   for (victim = ch->in_room->first_person; victim != NULL; victim = victim->next_in_room)
      if (victim != ch && can_see(ch, victim) && number_bits(1) == 0)
         break;

   if (victim == NULL)
      return FALSE;

   /*
    * for ( vch = ch->in_room->first_person; vch != NULL; vch = vch->next )
    * {
    * OBJ_DATA *obj;
    * sh_int    count = 0;
    * bool    match = FALSE;
    *
    *
    * if ( IS_NPC( vch ) )
    * continue;
    *
    *
    * for ( obj = ch->in_room->first_content; obj != NULL; obj = obj->in_room->first_content->next )
    * {
    * if ( obj->item_type != ITEM_CORPSE_PC )
    * continue;
    * count++;
    * if (  ( !str_cmp( vch->name, obj->owner ) )
    * && ( obj->first_content )    )
    * {
    * match = TRUE;
    * break;
    * }
    * }
    * if ( match )
    * {
    * char   buf[MAX_STRING_LENGTH];
    *
    * sprintf( buf, "Try 'get all %d.corpse' to recover your belongings, %s.\n\r",
    * count, PERS( ch, vch ) );
    * do_say( ch, buf );
    * }
    * }
    */

   switch (number_bits(3))
   {
   case 0:
   case 1:
   case 2:
   case 3:
   case 4:
   case 5:
      if (!IS_AFFECTED(victim, AFF_DETECT_INVIS))
      {
         act("$n utters the word 'Sight'.", ch, NULL, NULL, TO_ROOM);
         spell_detect_invis(skill_lookup("detect invis"), 0, ch, victim, NULL);
         return TRUE;
      }
   }
   return FALSE;
}

bool spec_undead(CHAR_DATA *ch)
{
   CHAR_DATA *victim;
   CHAR_DATA *ach;
   char *spell;
   int sn, sum_lev;

   if (!is_fighting(ch))
      return FALSE;

   for (victim = ch->in_room->first_person; victim != NULL; victim = victim->next_in_room)
      if (victim->fighting == ch && number_bits(2) != 0)
         break;

   if (victim == NULL)
      return FALSE;

   for (;;)
   {
      int min_level;

      switch (number_bits(4))
      {
      case 0:
         min_level = 0;
         spell = "blindness";
         break;
      case 1:
         min_level = 3;
         spell = "chill touch";
         break;
      case 2:
         min_level = 7;
         spell = "weaken";
         break;
      case 3:
         min_level = 30;
         spell = "earthquake";
         break;
      case 4:
         min_level = 60;
         spell = "dispel magic";
         break;
      case 5:
         min_level = 12;
         spell = "curse";
         break;
      case 6:
      case 7:
         min_level = 40;
         spell = "gate";
         break;
      case 8:
         min_level = 30;
         spell = "fireball";
         break;
      default:
         min_level = 1;
         spell = "summon";
         break;
      }

      if (ch->level >= min_level)
         break;
      /*
       * Can put infinite loop checking here if you want, but its pointless
       * * since nothing can have a ch->level < 0. -- Alty
       */
   }

   if ((sn = skill_lookup(spell)) < 0)
      return FALSE;

   if (!str_cmp(spell, "summon")) /* CHECK FOR NPC!!!!!!!!!!!!! */
   {
      sum_lev = ch->level * 2 / 3;
      for (ach = first_char; ach != NULL; ach = ach->next)
      {
         if (!IS_SET(ach->act, ACT_UNDEAD) || ach->level > sum_lev || !IS_NPC(ach) /* Kavir got summoned!  :P */
             || ach->in_room == ch->in_room || !can_see(ch, ach) || number_bits(2) != 0)
            continue;

         victim = ach;
         break;
      }
      if (ach == NULL)
         return FALSE;

      act("$n's eyes turn black for an instant!", ch, NULL, NULL, TO_ROOM);
      act("$n disappears suddenly.", victim, NULL, NULL, TO_ROOM);
      char_from_room(victim);
      char_to_room(victim, ch->in_room);
      act("$n arrives suddenly.", victim, NULL, NULL, TO_ROOM);
      multi_hit(victim, ch->fighting, TYPE_UNDEFINED);
   }
   else
   {
      (*skill_table[sn].spell_fun)(sn, ch->level, ch, victim, NULL);
   }

   return TRUE;
}

bool spec_stephen(CHAR_DATA *ch)
{
   /*
    * Award winner for daftest spec_fun of the week contest.
    * * mobs hops around, if finds PC, kisses them and gives
    * * a cure light.  -S-
    */

   ROOM_INDEX_DATA *room;
   CHAR_DATA *victim;
   int count;
   int vic_cnt;
   //    int sn;

   /*    if ( is_fighting(ch) )
          return FALSE;*/

   /*
    * Uhh.. sleeping mobs and the like shouldn't either. -- Alty
    */
   if (ch->position < POS_STANDING)
      return FALSE;

   /*    for ( ; ; )
       {
          room = get_room_index( number_range( 0, 65535 ) );
          if ( room == NULL )
        continue;*/
   /*
    * No checking for private rooms, etc. :P
    */
   /*       break;
       }*/

   /*
    * Check this loop.. -- Alty
    */
   for (room = get_room_index(number_range(0, 65535)); !room; room = get_room_index(number_range(0, 65535)))
      ;

   act("$n climbs up the chimney.", ch, NULL, NULL, TO_ROOM);
   char_from_room(ch);

   char_to_room(ch, room);
   act("$n emerges from the fireplace, carrying a huge sack of gifts!", ch, NULL, NULL, TO_ROOM);

   count = 0;
   for (victim = ch->in_room->first_person; victim != NULL; victim = victim->next_in_room)
      if (!IS_NPC(victim))
         count++;

   if (count == 0)
      return FALSE;

   vic_cnt = number_range(1, count);

   count = 0;
   for (victim = ch->in_room->first_person; victim != NULL; victim = victim->next_in_room)
   {
      if (!IS_NPC(victim) && (++count) == vic_cnt)
         break;
   }

   if (victim == NULL)
      return FALSE; /* just in case :) */

   act("$n says 'Ho Ho Ho, Merry Christmas, $N!", ch, NULL, victim, TO_ROOM);

   act("$n smiles at $N, and hands $M a present!.", ch, NULL, victim, TO_NOTVICT);
   act("$N smiles at you, and hands you a present!.", victim, NULL, ch, TO_CHAR);

   return TRUE;
}

bool spec_rewield(CHAR_DATA *ch)
{
   /*
    * If mob is fighting, try and get weapon and wield it
    * * if not fighting, check room and inv to see if any better weapons
    * * -Stephen
    */

   OBJ_DATA *obj;
   OBJ_DATA *weapon = NULL;
   int dam;
   int chance;
   bool pickup;
   char buf[MAX_STRING_LENGTH];

   pickup = TRUE;
   dam = 0;

   chance = (!is_fighting(ch) ? 35 : 60);

   if (number_percent() < chance)
   {
      for (obj = ch->first_carry; obj != NULL; obj = obj->next_in_carry_list)
         if (obj->item_type == ITEM_WEAPON && dam < obj->value[2])
         {
            dam = obj->value[2];
            pickup = FALSE;
            weapon = obj;
         }

      /*
       * Then check inventory and room for any weapons
       */
      for (obj = ch->in_room->first_content; obj != NULL; obj = obj->next_in_room)
      {
         if (obj->item_type == ITEM_WEAPON)
         {
            if (obj->value[2] > dam)
            {
               dam = obj->value[2];
               weapon = obj;
               pickup = TRUE;
            }
         }
      }

      if (weapon == NULL)
         return FALSE;

      if (weapon->wear_loc == WEAR_HOLD_HAND_L)
         return FALSE;

      if (pickup)
      {
         sprintf(buf, "Great!  %s!  Just what i've always wanted!", weapon->short_descr);
         do_say(ch, buf);
      }

      if (weapon != NULL)
      {
         /*
          * Now make the mob get the weapon
          */
         if (pickup)
            get_obj(ch, weapon, NULL);

         do_wear(ch, weapon->name);

         /*
          * Check is mob wielded weapon ok...
          */
         if (weapon->wear_loc == WEAR_NONE)
         {
            act("$n sniffs sadly.  'Baah!  It's no good to me!'", ch, NULL, NULL, TO_ROOM);
            extract_obj(weapon);
            act("$n sacrifices $p.", ch, weapon, NULL, TO_ROOM);
         }

         return TRUE;
      }
   }

   return FALSE;
}

bool spec_sylai_priest(CHAR_DATA *ch)
{
   /*
    * For sylai priest only.  Pushes player from overhang into pit below.
    * * -Players are gonna love this.  :)
    * * -S-
    */

   CHAR_DATA *victim;
   ROOM_INDEX_DATA *location;

   if (!is_fighting(ch))
      return FALSE;

   if (ch->in_room->vnum != 439)
      return FALSE;

   if (number_percent() > 30)
      return FALSE;

   for (victim = ch->in_room->first_person; victim != NULL; victim = victim->next_in_room)
   {
      if (victim->fighting == ch && number_bits(2) == 0)
         break;
   }

   if (victim == NULL)
      return FALSE;

   if (number_percent() > 60)
   {
      act("$n tries to knock $N off the outcrop!", ch, NULL, victim, TO_NOTVICT);
      act("$N tries to knock you off the outcrop!", victim, NULL, ch, TO_CHAR);
      return FALSE;
   }

   if (number_percent() > 70)
      location = get_room_index(436);
   else
      location = get_room_index(433);

   if (location == NULL)
   {
      notify("Fucked up spec_sylai_priest.\n\r", 82);
      return FALSE;
   }

   act("$n rushes towards $N and shoves $M off the outcrop!", ch, NULL, victim, TO_NOTVICT);
   act("$N rushes at you, and shoves you off the cliff!", victim, NULL, ch, TO_CHAR);
   stop_fighting(ch, TRUE);
   char_from_room(victim);

   char_to_room(victim, location);
   act("$n lands on the floor, having fallen from the outcrop above!", victim, NULL, NULL, TO_NOTVICT);
   send_to_char("You crash to the floor below!\n\r", victim);
   do_look(victim, "");
   return TRUE;
}

bool spec_cast_bigtime(CHAR_DATA *ch)
{
   CHAR_DATA *victim;
   char *spell;
   int sn;
   int crun;

   if (!is_fighting(ch))
      return FALSE;

   for (victim = ch->in_room->first_person; victim; victim = victim->next_in_room)
      if (victim->fighting == ch && number_bits(2) == 0)
         break;

   if (victim == NULL)
      return FALSE;

   for (crun = 0;; crun++)
   {
      int min_level;

      switch (number_bits(4))
      {
      case 0:
         min_level = 45;
         spell = "dispel magic";
         break;
      case 1:
         min_level = 46;
         spell = "acid blast";
         break;
      case 2:
         min_level = 30;
         spell = "phobia";
         break;
      case 3:
         min_level = 55;
         spell = "chain lightning";
         break;
      case 4:
      case 5:
         min_level = 43;
         spell = "mindflame";
         break;
      case 6:
         min_level = 12;
         spell = "lava burst";
         break;
      case 7:
         min_level = 44;
         spell = "static";
         break;
      default:
         min_level = 25;
         spell = "hellspawn";
         break;
      }

      if (ch->level >= min_level)
         break;
      /*
       * Stop infinite recursion on low level mobs.. (ch->level < 12)
       * * -- Alty
       */
      if (crun > 10)
         return FALSE;
   }

   if ((sn = skill_lookup(spell)) < 0)
      return FALSE;
   act("$n invokes the dark powers!!", ch, NULL, NULL, TO_ROOM);
   (*skill_table[sn].spell_fun)(sn, ch->level, ch, victim, NULL);
   return TRUE;
}

bool spec_sage(CHAR_DATA *ch)
{

   /*
    * Give the Sage some life.  He should chat with player, heal too, etc
    */

   /*
    * What the options are:
    * * W - Wake;   H - Greet;       C - make some food
    * * 1 - First Part of Story;      2-Second Part;    3-3rd part;
    * * 4- 4th part    5- 5th part;
    * * 6- 6th part    7-7th part;       8-8th part;
    * * F - rake the fire I - cast influx;
    * * G - Gossip     S - sleep.
    * * D - Do nothing.          X - Smile.    Y - Sigh.
    * * B - say byebye
    * * U - get food V - Eat food
    */

   static const char open_path[] = "WDDDHDFDCUVD1XDD2YDD3DD4DFD5DGDD6D7XDD7YDD8DFD9XDDDBDFD.";

   static const char *path;
   static int pos;
   static bool move;

   if (!move)
   {
      if (time_info.hour == 6)
      {
         path = open_path;
         move = TRUE;
         pos = 0;
      }
   }

   if (is_fighting(ch))
      return spec_cast_cleric(ch);
   if (!move || ch->position < POS_SLEEPING)
      return FALSE;

   switch (path[pos])
   {
   case 'D':
      /*
       * Do Nothing.
       */
      break;
   case 'W':
      ch->position = POS_RESTING;
      act("$n awakens and groans loudly.", ch, NULL, NULL, TO_ROOM);
      break;

   case 'S':
      ch->position = POS_SLEEPING;
      act("$n lies down and falls asleep.", ch, NULL, NULL, TO_ROOM);
      break;

   case 'H':
      do_say(ch, "Hello There!  Welcome to my humble cave.");
      break;
   case 'F':
      act("$n leans forward, and rakes the fire.", ch, NULL, NULL, TO_ROOM);
      break;
   case 'C':
      do_say(ch, "You must be hungry.  Share some food with me.");
      spell_produce_food(skill_lookup("produce food"), 1, ch, NULL, NULL);
      spell_produce_food(skill_lookup("produce food"), 1, ch, NULL, NULL);
      break;
   case 'U':
      do_get(ch, "bundle");
      break;
   case 'V':
      do_eat(ch, "bundle");
      break;
   case 'X':
      act("$n smiles sadly, remembering better days.", ch, NULL, NULL, TO_ROOM);
      break;
   case 'Y':
      act("$n sighes sadly.", ch, NULL, NULL, TO_ROOM);
      break;
   case '1':
      do_say(ch, "Rest yourselves, and listen to what i have to tell you about the Sylai beasts.");
      break;
   case '2':
      do_say(ch, "Many decades ago, my race lived in these tunnels, living in peace.");
      do_say(ch, "Then, one day the Sylai came.  No one knows where they originated from.");
      do_say(ch, "But when they came, they brought the Dark Powers with them.");
      break;
   case '3':
      do_say(ch, "My race did not believe in killing.  We tried to reason with the Sylai.");
      do_say(ch, "but they retaliated with violence, killing us one by one.");
      break;
   case '4':
      do_say(ch, "The Sylai had no mercy.  They killed to satisfy the Dark Powers.");
      do_say(ch, "They enjoyed killing us.  My kind were enslaved to the Sylai.");
      do_say(ch, "They used us for their evil experiments, but we started to fight back.");
      break;
   case '5':
      do_say(ch, "But we were weak.  The Dark Powers within the Sylai was too much.");
      do_say(ch, "Slowly, they butchered my kind.  Some of us fled, and hid here.");
      break;
   case '6':
      do_say(ch, "There were originally five.  Now there is just me.");
      do_say(ch, "I grow weak now - i know i don't have long left.");
      break;
   case '7':
      do_say(ch, "I just wish that we had struck back when stronger in number.");
      do_say(ch, "But now there is just me.  And what can one old fool do?");
      break;
   case '8':
      do_say(ch, "If you have come this far, you must be strong.");
      do_say(ch, "Seek revenge for my race.  Search out their leader, Sylar.");
      do_say(ch, "And wipe him from this plane of existence.");
      break;
   case '9':
      do_say(ch, "Sylar stole my race's greatest power.  Our artifacts.");
      do_say(ch, "Without those, we were powerless.  But they would help you greatly.");
      do_say(ch, "Take them, and remember my race.");
      break;
   case 'G':
      do_say(ch, "I remember when old Caryl was still with me.  Oh, the fun we had.");
      do_say(ch, "But that was years ago.  And now she's gone, like all the rest.");
      break;
   case 'B':
      do_say(ch, "I grow tired.  It is time for me to sleep.");
      do_say(ch, "Go now, and strike back at Sylar");
      act("$n waves goodbye to you.", ch, NULL, NULL, TO_ROOM);
      break;
   case '.':
      act("$n lies down slowly, and falls into a troubled sleep.", ch, NULL, NULL, TO_ROOM);
      ch->position = POS_SLEEPING;
      move = FALSE;
      break;
   }

   pos++;
   return FALSE;
}

bool spec_wizardofoz(CHAR_DATA *ch)
{
   /*
    * This disabled - intel mobs dropped
    */
   return FALSE;
}

bool spec_vamp_hunter(CHAR_DATA *ch)
{

   char buf[MAX_STRING_LENGTH];
   char buf1[MAX_STRING_LENGTH];

   if (ch->hunting == NULL)
      return FALSE;

   if ((!IS_VAMP(ch->hunting)) || (IS_NPC(ch->hunting)))
      return FALSE;

   switch (number_range(1, 21))
   {

   case 1:
   case 2:
   case 3:
   case 4:
   case 5:
      break;
   case 6:
      sprintf(buf, "%s", ch->hunting->name);
      sprintf(buf1, "@@eI know that you are a Vampyre, and I shall not rest until your are destroyed!!!@@N\n\r");
      safe_strcat(MAX_STRING_LENGTH, buf, buf1);
      do_tell(ch, buf);
      break;
   case 7:
   case 8:
   case 9:
   case 10:
   case 11:
      break;
   case 12:
   case 13:
      sprintf(buf, "My informants have told me that %s is a Vampyre, and I have vowed to destroy him!!\n\r",
              ch->hunting->name);
      do_yell(ch, buf);
      break;

   case 14:
   case 15:
   case 16:
      break;
   case 17:
      sprintf(buf, " %s ", ch->hunting->name);
      sprintf(buf1, "@@Do you finally know fear? I shall not rest until ALL of your kind are destroyed!!!@@N\n\r");
      safe_strcat(MAX_STRING_LENGTH, buf, buf1);
      do_tell(ch, buf);
      break;

   case 18:
   case 19:
      break;
   case 20:
      sprintf(buf,
              "All the realm shall know that %s is a Vampyre, and I shall eridicate all of these vile creatures!!\n\r",
              ch->hunting->name);
      do_yell(ch, buf);
      break;
   default:
      break;
   }

   return TRUE;
}

bool spec_tax_man(CHAR_DATA *ch)
{

   ROOM_INDEX_DATA *room;
   CHAR_DATA *victim;
   int count;
   int vic_cnt;
   int sn = 0;
   int remainder = 0;
   int loss = 0, bank_loss = 0, char_loss = 0, old_char = 0, old_bank = 0;
   char mon_buf[MSL];
   char cat_buf[MSL];

   /*    if ( is_fighting(ch) )
          return FALSE;*/

   /*
    * Uhh.. sleeping mobs and the like shouldn't either. -- Alty
    */
   if (ch->position < POS_STANDING)
      return FALSE;

   /*    for ( ; ; )
       {
          room = get_room_index( number_range( 0, 65535 ) );
          if ( room == NULL )
        continue;*/
   /*
    * No checking for private rooms, etc. :P
    */
   /*       break;
       }*/

   /*
    * Check this loop.. -- Alty
    */
   for (room = get_room_index(number_range(0, 65535)); !room; room = get_room_index(number_range(0, 65535)))
      ;

   act("$n suddenly vanishes!", ch, NULL, NULL, TO_ROOM);
   char_from_room(ch);

   char_to_room(ch, room);
   act("$n suddenly appears in the room!", ch, NULL, NULL, TO_ROOM);

   count = 0;
   for (victim = ch->in_room->first_person; victim != NULL; victim = victim->next_in_room)
      if (!IS_NPC(victim))
         count++;

   if (count == 0)
      return FALSE;

   vic_cnt = number_range(1, count);

   count = 0;
   for (victim = ch->in_room->first_person; victim != NULL; victim = victim->next_in_room)
   {
      if (!IS_NPC(victim) && (++count) == vic_cnt)
         break;
   }

   if (victim == NULL)
      return FALSE; /* just in case :) */

   if ((victim->gold < 100000) && (victim->balance < 1000000))
      return FALSE;

   act("$n says 'Excuse me, $N, but our records state you haven't payed your taxes recently.", ch, NULL, victim, TO_ROOM);

   act("$n says, 'Since you refuse to pay what you owe, it has automatically been deducted from your funds'.", ch, NULL,
       victim, TO_ROOM);
   act("$n says, 'Have a nice day'.", ch, NULL, ch, TO_ROOM);

   sn = (victim->balance + victim->gold) / 100;
   old_char = victim->gold;
   old_bank = victim->balance;

   if (victim->balance > sn)
   {
      loss = victim->balance - sn;
      victim->balance = UMIN(victim->balance, loss);
      bank_loss = sn;
      char_loss = 0;
   }
   else
   {
      remainder = sn - victim->balance;
      bank_loss = victim->balance;
      char_loss = remainder;
      loss = victim->gold - remainder;
      victim->balance = 0;
      if (remainder > 0)
         victim->gold = UMIN(victim->gold, loss);
   }
   sprintf(mon_buf, "Tax Collector visited %s ", victim->name);
   sprintf(cat_buf, "Collected %i from bank, and %i from gold on hand.\n\r", bank_loss, char_loss);
   safe_strcat(MSL, mon_buf, cat_buf);
   sprintf(cat_buf, "New totals are balance: %i/%i  on hand: %i/%i\n\r",
           victim->balance, old_bank, victim->gold, old_char);
   safe_strcat(MSL, mon_buf, cat_buf);
   monitor_chan(mon_buf, MONITOR_MOB);

   do_save(victim, "");
   return TRUE;
}

/*
 * Midgaard ambient citizen specials — Lantern Reforms, Violet Compact,
 * crossroads market, Temple of the Resounding Heart, and day-mark labor.
 */

bool spec_lamplighter(CHAR_DATA *ch)
{
   static const char *acts[] = {
      "$n trims a lamp wick with practiced hands, a small curl of smoke rising as the flame steadies.",
      "$n checks the oil level in a street lantern, tilting the reservoir with a careful eye.",
      "$n adjusts the reflector on a wall-mounted lamp, angling it outward toward the street.",
      "$n taps a lamp bracket with $s hook pole, testing the iron fitting for rust.",
      "$n consults a worn route card from $s coat pocket, muttering a count of posts.",
      "$n refills a lamp from one of the oil flasks on $s shoulder satchel."
   };
   static const char *says[] = {
      "No ward unlit, no traveler uncounted — that's the Oath, and that's the work.",
      "Kindling Watch runs till the Ledger bells. All posts on this lamp-line are checked.",
      "You see a dark corner in this city, report it to the Lantern Registry. I'll have it lit by Ash Watch.",
      "Ash Watch runs long when the wind comes off the western road. Outer posts need double oil.",
      "The Registry lists every lamp-post in the city. Every one. I've walked them all."
   };

   if (!IS_AWAKE(ch) || is_fighting(ch))
      return FALSE;

   if (number_bits(3) != 0)
      return FALSE;

   if (number_bits(1) == 0)
      act(acts[number_range(0, 5)], ch, NULL, NULL, TO_ROOM);
   else
      do_say(ch, says[number_range(0, 4)]);

   return FALSE;
}

bool spec_warden(CHAR_DATA *ch)
{
   static const char *acts[] = {
      "$n traces a finger along the ward sigils carved above the archway, checking for any that have faded.",
      "$n consults $s bound registry ledger, running a finger down the day's approved manifests.",
      "$n adjusts $s violet-banded sash, the mark of a licensed Compact warden.",
      "$n glances at a passing figure, then makes a brief notation in $s ledger.",
      "$n taps the checkpoint placard with one finger: 'Arcane materials — registry notation required.'"
   };
   static const char *says[] = {
      "All arcane goods require registry notation before this threshold. The Compact is clear.",
      "If you carry unregistered materials, declare them now. The ledger is open.",
      "The Compact protects you as much as the city, traveler. Documentation is not the enemy.",
      "Containment over destruction — that is the Compact's doctrine. We seal; we do not burn.",
      "Every passage to the below is a civic jurisdiction. Remember that before you descend."
   };

   if (!IS_AWAKE(ch) || is_fighting(ch))
      return FALSE;

   if (number_bits(3) != 0)
      return FALSE;

   if (number_bits(1) == 0)
      act(acts[number_range(0, 4)], ch, NULL, NULL, TO_ROOM);
   else
      do_say(ch, says[number_range(0, 4)]);

   return FALSE;
}

bool spec_vendor(CHAR_DATA *ch)
{
   static const char *calls[] = {
      "$n calls out, 'Timber carvings off the northern routes — finest grain in the city!'",
      "$n holds up a small jar: 'Eastern spice, fresh off the desert caravan — smell that!'",
      "$n gestures at $s table: 'Day-mark stamped, tariff-cleared — everything here paid its Ledgerhouse due!'",
      "$n calls out, 'Good leather out of the armorer quarter — don't pay shop prices for off-cuts!'",
      "$n waves at a passing pedestrian: 'Fair price, honest weight, Registry-notated goods, citizen!'"
   };
   static const char *says[] = {
      "Tariff board raised the western-goods levy again. Prices have to reflect it, nothing personal.",
      "The Guild Concordat sets minimums, but we independent vendors set our own ceiling. Fair is fair.",
      "You want the Roc Road goods, you pay Roc Road prices. Simple arithmetic.",
      "Northern shipment was late — ashfang trouble on the forest roads, the warden said.",
      "Come back at Bell Watch — I knock a tenth off anything left. No point carrying it home."
   };

   if (!IS_AWAKE(ch) || is_fighting(ch))
      return FALSE;

   if (number_bits(3) != 0)
      return FALSE;

   if (number_bits(1) == 0)
      act(calls[number_range(0, 4)], ch, NULL, NULL, TO_ROOM);
   else
      do_say(ch, says[number_range(0, 4)]);

   return FALSE;
}

bool spec_lay_sister(CHAR_DATA *ch)
{
   static const char *acts[] = {
      "$n pauses at a memorial marker, adjusting a small oil lamp at its base.",
      "$n opens $s worn civic liturgy booklet to a marked page, lips moving silently.",
      "$n traces a carved name on the memorial wall with one careful finger.",
      "$n sets a fresh tallow stub beside the others at the base of the memorial stone.",
      "$n folds $s hands and stands in quiet observance before the carved dedication."
   };
   static const char *says[] = {
      "No ward unlit, no traveler uncounted. First Oath. Spoken at every memorial by the book.",
      "No judgment hidden, no sentence unrecorded. Second Oath. The Temple holds to it even when the courts do not.",
      "No levy taken that cannot be borne. Third Oath. The hardest to keep, in my experience.",
      "The Temple maintains rolls of the uncounted. Those removed from the ledgers are still remembered here.",
      "Two bells after midnight are not alarm. They are memorial. The city remembers, even when it pretends not to."
   };

   if (!IS_AWAKE(ch) || is_fighting(ch))
      return FALSE;

   if (number_bits(3) != 0)
      return FALSE;

   if (number_bits(1) == 0)
      act(acts[number_range(0, 4)], ch, NULL, NULL, TO_ROOM);
   else
      do_say(ch, says[number_range(0, 4)]);

   return FALSE;
}

bool spec_laborer(CHAR_DATA *ch)
{
   static const char *acts[] = {
      "$n flexes $s hands, cracked and calloused, then readjusts $s grip on a heavy load.",
      "$n pauses to check $s day-token against the stamp mark on $s wrist, then sighs.",
      "$n sets down $s load for a moment, rolling $s shoulders before picking it back up.",
      "$n wipes sweat from $s face with a cloth that is not much cleaner than $s hands.",
      "$n studies the route board on the corner wall, tracing the chalk marks with a finger."
   };
   static const char *says[] = {
      "Token came late today. One bell late means shadow-class by the Registry clock. Makes no sense.",
      "Levy goes up, day-rate stays flat. Cinder debt, they call it — compounds whether you pay or not.",
      "Work-broker docked two coppers off my token for 'route variance.' I don't even know what that means.",
      "You can be legal here your whole life and one missing stamp makes you undocumented. One stamp.",
      "The Oath says no levy taken that cannot be borne. I'd like to see them try carrying this load."
   };

   if (!IS_AWAKE(ch) || is_fighting(ch))
      return FALSE;

   if (number_bits(3) != 0)
      return FALSE;

   if (number_bits(1) == 0)
      act(acts[number_range(0, 4)], ch, NULL, NULL, TO_ROOM);
   else
      do_say(ch, says[number_range(0, 4)]);

   return FALSE;
}

bool spec_keep_physical_captain(CHAR_DATA *ch)
{
   char buf[MSL];

   if (!is_fighting(ch))
   {
      ch->spec_behavior = 0;
      return FALSE;
   }

   ch->spec_behavior++;

   switch(ch->spec_behavior%10)
   {
   case 1:
      if (number_percent() < 50)
         do_morale(ch, "");
      else
         do_leadership(ch, "");
      break;
   case 4:
      if (number_percent() < 50)
         do_warcry(ch,"");
      else
         do_beserk(ch,"");
      break;
   case 5:
      sprintf(buf, "@@y$n brings his weapon over his head, preparing for a massive strike!@@N");
      act(buf, ch, NULL, NULL, TO_ROOM);
      break;
   case 7:
      sprintf(buf, "@@y$n brings his weapon down in a massive strike against $N!@@N");
      act(buf, ch, NULL, ch->fighting, TO_ROOM);
      do_massivestrike(ch);
      break;
   }

   return TRUE;
}

void do_massivestrike(CHAR_DATA *ch)
{
     int dam = number_range(9000,11000);

     calculate_damage(ch, ch->fighting, dam, gsn_holystrike, ELEMENT_PHYSICAL | ELEMENT_HOLY, TRUE);
}

/*
 * Kiess ambient NPC specials — shopkeepers, inn staff, scouts,
 * civic orators, and Wall Command officers.
 */

bool spec_kiess_shopkeeper(CHAR_DATA *ch)
{
   static const char *acts[] = {
      "$n straightens the display counter, making sure the best pieces are visible from the door.",
      "$n polishes a displayed item with a soft cloth, holding it briefly to the light.",
      "$n glances up as you enter and offers a practiced nod of professional welcome.",
      "$n checks the day's inventory list against the stock behind the counter.",
      "$n sets a fresh lamp on the counter for the afternoon trade hours."
   };
   static const char *says[] = {
      "Most of what you need for the frontier is right here. We stock for the road, not the shelf.",
      "Supply shipment came in yesterday — good selection at the moment. Won't last the week.",
      "The Exchange backs the quality of everything on this counter. Kiess doesn't traffic in inferior goods.",
      "If you're heading north into the Forest, talk to the cartographer first. Know what you're walking into.",
      "Roc Road caravans bring good stock from Midgaard. Come back in a few days if I'm short on something."
   };

   if (!IS_AWAKE(ch) || is_fighting(ch))
      return FALSE;

   if (number_bits(3) != 0)
      return FALSE;

   if (number_bits(1) == 0)
      act(acts[number_range(0, 4)], ch, NULL, NULL, TO_ROOM);
   else
      do_say(ch, says[number_range(0, 4)]);

   return FALSE;
}

bool spec_kiess_innkeeper(CHAR_DATA *ch)
{
   static const char *acts[] = {
      "$n checks the room ledger, running a finger down the current guest list.",
      "$n tops off a traveler's cup from a clay pitcher without being asked.",
      "$n straightens the traveler memorial plaque by the door, tracing a name quietly.",
      "$n glances at the city map mural and points out a route to a newly arrived guest.",
      "$n sets a fresh lamp on the counter, adjusting the wick for the evening hours."
   };
   static const char *says[] = {
      "First night in Kiess? The memorial plaque by the door has names — not to alarm you, but to remind you to come back.",
      "Roc Road is steady right now. Caravans in both directions. Good time to be traveling.",
      "The inn keeps a posted watch. You'll sleep undisturbed. City compact law holds inside these walls.",
      "If you're heading into the Forest of Confusion, check the cartographer's office east of the promenade first.",
      "Leave a forwarding note at the Postmaster's if you'll be away more than three days. Standard practice here."
   };

   if (!IS_AWAKE(ch) || is_fighting(ch))
      return FALSE;

   if (number_bits(3) != 0)
      return FALSE;

   if (number_bits(1) == 0)
      act(acts[number_range(0, 4)], ch, NULL, NULL, TO_ROOM);
   else
      do_say(ch, says[number_range(0, 4)]);

   return FALSE;
}

bool spec_kiess_scout(CHAR_DATA *ch)
{
   static const char *acts[] = {
      "$n marks notations on a field map with practiced efficiency, cross-referencing route cards.",
      "$n checks the rope markers on $s belt, counting the knots against $s field notes.",
      "$n scans the room with the alert wariness of someone who hasn't fully decompressed from the forest.",
      "$n consults a compass bearing against $s posted route notes, making a small correction.",
      "$n oils the edge of $s blade with a cloth, eyes distant, working from ingrained habit."
   };
   static const char *says[] = {
      "The mist line's moved another three hundred paces north since last week. That's fast for this season.",
      "Bell-post seven is down. Wall Command's rerouting the southern approach until it's back up.",
      "Saw centaur sign at the forest edge. Prints were fresh — probably overnight. Command's been told.",
      "The confusion doesn't hit immediately. You get maybe two hundred paces before the mist starts working. Stay on the rope lines.",
      "Bring samples from the mist-boundary if you go in. The healers want the spore data. Sealed container, waxed shut."
   };

   if (!IS_AWAKE(ch) || is_fighting(ch))
      return FALSE;

   if (number_bits(3) != 0)
      return FALSE;

   if (number_bits(1) == 0)
      act(acts[number_range(0, 4)], ch, NULL, NULL, TO_ROOM);
   else
      do_say(ch, says[number_range(0, 4)]);

   return FALSE;
}

bool spec_kiess_orator(CHAR_DATA *ch)
{
   static const char *speeches[] = {
      "Evermeet was not destroyed in a single night. It was abandoned in a thousand small decisions. Kiess was built so those decisions cannot be made quietly.",
      "The Compact Flame has burned without interruption since the first foundation stone was laid. That continuity is deliberate. Watch it — it is doing something.",
      "Three factions, one peace code. Not because they agree — they rarely agree — but because the alternative is Evermeet. We have seen the alternative.",
      "The Pillar of Returning Caravans adds a mark for every safe return from the frontier. Count them sometime. That count is what this city is actually defending.",
      "Roc Road connects us east. The Forest of Confusion lies north. Wall Command holds the perimeter. The Compact holds the center. This is not accidental — this is the design.",
      "Every waystone on these avenues marks a district that once existed in Evermeet. Every one of those districts fell silent. We read those names so we know what we are protecting against."
   };

   if (!IS_AWAKE(ch) || is_fighting(ch))
      return FALSE;

   if (number_bits(3) != 0)
      return FALSE;

   act("$n addresses the assembled listeners with the measured cadence of a prepared civic speaker.", ch, NULL, NULL, TO_ROOM);
   do_say(ch, speeches[number_range(0, 5)]);

   return FALSE;
}

bool spec_kiess_wall_officer(CHAR_DATA *ch)
{
   static const char *acts[] = {
      "$n reviews the duty roster on the briefing board, making a notation in the margin.",
      "$n traces a patrol route on the tactical map with one finger, checking times against the duty log.",
      "$n steps to the window and scans the wall walk before returning to the desk.",
      "$n initials the bottom of a stack of patrol reports and files them in the duty log.",
      "$n sends a courier off with a sealed dispatch, watching until they clear the corridor."
   };
   static const char *says[] = {
      "East gate rotation is covered through Dusk Watch. North gate I want two sentries until the mist-line report comes in.",
      "Any scout returning from the northern verge reports to this office directly before the general debrief. Standing order.",
      "Wall Command doesn't guess at threat levels. We assess, we document, we act on evidence. That is the doctrine.",
      "The Withered Depths blight is moving. Slowly — but moving. Scouts returning with blight-edge data get full debrief priority.",
      "Kiess survives because its walls are manned and its gates have discipline. Everything else this city does depends on that foundation."
   };

   if (!IS_AWAKE(ch) || is_fighting(ch))
      return FALSE;

   if (number_bits(3) != 0)
      return FALSE;

   if (number_bits(1) == 0)
      act(acts[number_range(0, 4)], ch, NULL, NULL, TO_ROOM);
   else
      do_say(ch, says[number_range(0, 4)]);

   return FALSE;
}
