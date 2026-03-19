/*
 * Unit tests for adept skill registration — verifies that all 6 new
 * Martial Artist and Crusader adept skills are correctly registered in
 * the skill table with proper class assignments and gsn linkage.
 */

#include <assert.h>
#include <stdio.h>
#include <string.h>

#define DEC_GLOBALS_H 1
#include "ack.h"

extern const struct skill_type skill_table[MAX_SKILL];

/* ── We only need the skill_table and gsn variables from the real build.
 *    The skill_table is defined in const.o (linked in).
 *    The gsn variables are defined in db.c but we don't link that,
 *    so we define them here — the pgsn pointers reference these. ────── */

sh_int gsn_chi_surge;
sh_int gsn_breath_of_endurance;
sh_int gsn_fist_interior;
sh_int gsn_momentum_chain;
sh_int gsn_iron_resolve;
sh_int gsn_overwhelming_assault;
sh_int gsn_anti_magic_shell;
sh_int gsn_enhanced_heal;
sh_int gsn_poison_nightshade;

/* Stubs for all the other gsn variables referenced by skill_table entries */
sh_int gsn_backstab, gsn_circle, gsn_dodge, gsn_hide, gsn_peek, gsn_pick_lock;
sh_int gsn_sneak, gsn_steal, gsn_hunt, gsn_enhanced_damage, gsn_kick, gsn_rescue;
sh_int gsn_disarm, gsn_poison, gsn_bash, gsn_stun, gsn_second_attack;
sh_int gsn_third_attack, gsn_fourth_attack, gsn_parry, gsn_dirt, gsn_beserk;
sh_int gsn_martial_arts, gsn_dualwield, gsn_counter, gsn_two_handed;
sh_int gsn_nodisarm, gsn_enhanced_critical, gsn_potency;
sh_int gsn_shield_block, gsn_spell_critical, gsn_spell_critical_damage;
sh_int gsn_equip_buckler, gsn_equip_fist, gsn_dual_fist, gsn_equip_wand;
sh_int gsn_headbutt, gsn_knee, gsn_punch, gsn_adrenaline, gsn_frenzy;
sh_int gsn_morale, gsn_leadership, gsn_target, gsn_scout, gsn_unit_tactics;
sh_int gsn_charge, gsn_cage, gsn_disguise, gsn_throwing_star, gsn_throwing_needle;
sh_int gsn_rune_fire, gsn_rune_shock, gsn_rune_poison;
sh_int gsn_mana_flare, gsn_healing_light, gsn_divine_intervention;
sh_int gsn_holy_armor, gsn_cripple, gsn_shieldblock, gsn_chiblock;
sh_int gsn_holystrike, gsn_fleche, gsn_riposte;
sh_int gsn_pummel, gsn_aurabolt, gsn_spinfist, gsn_phantomfist;
sh_int gsn_palmstrike, gsn_flurry, gsn_mindoverbody, gsn_chakra;
sh_int gsn_warcry, gsn_enhanced_sword_damage, gsn_enhanced_sword_critical;
sh_int gsn_enhanced_purify, gsn_smash, gsn_notrip, gsn_conditioning;
sh_int gsn_garrote, gsn_feign_death, gsn_shadow_step, gsn_mark_target;
sh_int gsn_conceal, gsn_set_trap, gsn_read_intent;
sh_int gsn_cleave, gsn_fortify, gsn_taunt, gsn_rend, gsn_field_patch;
sh_int gsn_weapon_mastery;
sh_int gsn_grapple, gsn_choke_hold, gsn_leg_sweep, gsn_iron_skin;
sh_int gsn_roll_with_blow, gsn_pressure_point, gsn_feint;
sh_int gsn_poison_quinine, gsn_poison_arsenic;
sh_int gsn_bare_hand;
sh_int gsn_elemental_attunement;
sh_int gsn_oathshield, gsn_sanctified_strike;
sh_int gsn_shadow_reading, gsn_hex_ward, gsn_reflex_disruption;
sh_int gsn_substrate_piercing;

/* ── minimal stubs ─────────────────────────────────────────────────── */
const struct race_type race_table[MAX_RACE] = {[0] = {.skill = ""}};
const struct class_type gclass_table[MAX_TOTAL_CLASS] = {
    [0] = {.tier = MORTAL},  [1] = {.tier = MORTAL},  [2] = {.tier = MORTAL},
    [3] = {.tier = MORTAL},  [4] = {.tier = MORTAL},  [5] = {.tier = MORTAL},
    [6] = {.tier = REMORT},  [7] = {.tier = REMORT},  [8] = {.tier = REMORT},
    [9] = {.tier = REMORT},  [10] = {.tier = REMORT}, [11] = {.tier = REMORT},
    [12] = {.tier = REMORT}, [13] = {.tier = REMORT}, [14] = {.tier = REMORT},
    [15] = {.tier = REMORT}, [16] = {.tier = REMORT}, [17] = {.tier = REMORT},
    [18] = {.tier = ADEPT},  [19] = {.tier = ADEPT},  [20] = {.tier = ADEPT},
    [21] = {.tier = ADEPT},  [22] = {.tier = ADEPT},  [23] = {.tier = ADEPT},
    [24] = {.tier = MORTAL}, [25] = {.tier = REMORT}, [26] = {.tier = REMORT},
    [27] = {.tier = ADEPT},
};

/* ── skill_lookup helper ───────────────────────────────────────────── */
static int find_skill(const char *name)
{
   for (int sn = 0; sn < MAX_SKILL; sn++)
   {
      if (skill_table[sn].name == NULL)
         continue;
      if (strcmp(name, skill_table[sn].name) == 0)
         return sn;
   }
   return -1;
}

static void assert_skill_at_class_level(const char *name, int class_idx, int expected_level)
{
   int sn = find_skill(name);
   if (sn < 0)
   {
      printf("FAIL: skill '%s' not found in skill_table\n", name);
      assert(0);
   }
   if (skill_table[sn].skill_level[class_idx] != expected_level)
   {
      printf("FAIL: skill '%s' at class %d: expected level %d, got %d\n", name, class_idx,
             expected_level, skill_table[sn].skill_level[class_idx]);
      assert(0);
   }
}

static void assert_skill_has_gsn(const char *name)
{
   int sn = find_skill(name);
   assert(sn >= 0);
   assert(skill_table[sn].pgsn != NULL);
}

/* ── tests ─────────────────────────────────────────────────────────── */

static void test_martial_artist_skills_registered(void)
{
   assert_skill_at_class_level("chi surge", CLASS_MAR, 5);
   assert_skill_at_class_level("breath of endurance", CLASS_MAR, 10);
   assert_skill_at_class_level("fist of the interior form", CLASS_MAR, 15);
   printf("PASS: test_martial_artist_skills_registered\n");
}

static void test_crusader_skills_registered(void)
{
   assert_skill_at_class_level("momentum chain", CLASS_CRU, 5);
   assert_skill_at_class_level("iron resolve", CLASS_CRU, 10);
   assert_skill_at_class_level("overwhelming assault", CLASS_CRU, 15);
   printf("PASS: test_crusader_skills_registered\n");
}

static void test_grand_magi_skills_registered(void)
{
   assert_skill_at_class_level("elemental confluence", CLASS_GMA, 18);
   assert_skill_at_class_level("convergence shield", CLASS_GMA, 8);
   assert_skill_at_class_level("elemental attunement", CLASS_GMA, 3);
   printf("PASS: test_grand_magi_skills_registered\n");
}

static void test_templar_skills_registered(void)
{
   assert_skill_at_class_level("sacred ward", CLASS_TEM, 5);
   assert_skill_at_class_level("oathshield", CLASS_TEM, 10);
   assert_skill_at_class_level("sanctified strike", CLASS_TEM, 15);
   printf("PASS: test_templar_skills_registered\n");
}

static void test_nightblade_skills_registered(void)
{
   assert_skill_at_class_level("shadow reading", CLASS_NIG, 5);
   assert_skill_at_class_level("hex ward", CLASS_NIG, 10);
   assert_skill_at_class_level("reflex disruption", CLASS_NIG, 15);
   printf("PASS: test_nightblade_skills_registered\n");
}

static void test_kinetimancer_skills_registered(void)
{
   assert_skill_at_class_level("momentum drain", CLASS_KIN, 5);
   assert_skill_at_class_level("entropic shield", CLASS_KIN, 8);
   assert_skill_at_class_level("cognitive disruption", CLASS_KIN, 12);
   assert_skill_at_class_level("predictive collapse", CLASS_KIN, 15);
   printf("PASS: test_kinetimancer_skills_registered\n");
}

static void test_adept_skills_have_gsn_pointers(void)
{
   assert_skill_has_gsn("chi surge");
   assert_skill_has_gsn("breath of endurance");
   assert_skill_has_gsn("fist of the interior form");
   assert_skill_has_gsn("momentum chain");
   assert_skill_has_gsn("iron resolve");
   assert_skill_has_gsn("overwhelming assault");
   assert_skill_has_gsn("elemental attunement");
   assert_skill_has_gsn("oathshield");
   assert_skill_has_gsn("sanctified strike");
   assert_skill_has_gsn("shadow reading");
   assert_skill_has_gsn("hex ward");
   assert_skill_has_gsn("reflex disruption");
   printf("PASS: test_adept_skills_have_gsn_pointers\n");
}

static void test_martial_artist_skills_exclusive_to_adept(void)
{
   int sn;
   const char *names[] = {"chi surge", "breath of endurance", "fist of the interior form"};

   for (int n = 0; n < 3; n++)
   {
      sn = find_skill(names[n]);
      assert(sn >= 0);
      for (int i = 0; i < MAX_CLASS + MAX_REMORT; i++)
         assert(skill_table[sn].skill_level[i] < 0);
   }
   printf("PASS: test_martial_artist_skills_exclusive_to_adept\n");
}

static void test_crusader_skills_exclusive_to_adept(void)
{
   int sn;
   const char *names[] = {"momentum chain", "iron resolve", "overwhelming assault"};

   for (int n = 0; n < 3; n++)
   {
      sn = find_skill(names[n]);
      assert(sn >= 0);
      for (int i = 0; i < MAX_CLASS + MAX_REMORT; i++)
         assert(skill_table[sn].skill_level[i] < 0);
   }
   printf("PASS: test_crusader_skills_exclusive_to_adept\n");
}

static void test_existing_adept_skills_still_registered(void)
{
   assert_skill_at_class_level("anti magic shell", CLASS_CRU, 2);
   assert_skill_at_class_level("enhanced heal", CLASS_TEM, 1);
   assert_skill_at_class_level("elemental inferno", CLASS_GMA, 1);
   assert_skill_at_class_level("Kinetic Reversion", CLASS_KIN, 2);
   assert_skill_at_class_level("poison:nightshade", CLASS_NIG, 2);
   printf("PASS: test_existing_adept_skills_still_registered\n");
}

int main(void)
{
   test_martial_artist_skills_registered();
   test_crusader_skills_registered();
   test_grand_magi_skills_registered();
   test_templar_skills_registered();
   test_nightblade_skills_registered();
   test_kinetimancer_skills_registered();
   test_adept_skills_have_gsn_pointers();
   test_martial_artist_skills_exclusive_to_adept();
   test_crusader_skills_exclusive_to_adept();
   test_existing_adept_skills_still_registered();

   printf("All adept skills unit tests passed.\n");
   return 0;
}
