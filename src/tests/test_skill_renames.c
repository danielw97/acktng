/*
 * Unit tests for skill and stance renames — verifies that all four renamed
 * skills appear in the skill table under their new names, that the old names
 * are absent, and that the three renamed stances use their new names.
 */

#include <assert.h>
#include <stdio.h>
#include <string.h>

#define DEC_GLOBALS_H 1
#include "ack.h"

extern const struct skill_type skill_table[MAX_SKILL];
extern const struct stance_app_type stance_app[MAX_STANCE];

/* ── GSN stubs required by skill_table entries ─────────────────────── */
sh_int gsn_chi_surge;
sh_int gsn_breath_of_endurance;
sh_int gsn_fist_interior;
sh_int gsn_momentum_chain;
sh_int gsn_veterans_cadence;
sh_int gsn_overwhelming_assault;
sh_int gsn_anti_magic_shell;
sh_int gsn_enhanced_heal;
sh_int gsn_poison_nightshade;

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

/* ── helpers ────────────────────────────────────────────────────────── */
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

static int find_stance(const char *name)
{
   for (int i = 0; i < MAX_STANCE; i++)
   {
      if (stance_app[i].name != NULL && strcmp(name, stance_app[i].name) == 0)
         return i;
   }
   return -1;
}

/* ── skill rename tests ─────────────────────────────────────────────── */

static void test_new_skill_names_present(void)
{
   assert(find_skill("gap transit") >= 0);
   assert(find_skill("reach silence") >= 0);
   assert(find_skill("applied understanding") >= 0);
   assert(find_skill("seven shade hold") >= 0);
   printf("PASS: test_new_skill_names_present\n");
}

static void test_old_skill_names_absent(void)
{
   assert(find_skill("shadow step") < 0);
   assert(find_skill("garrote") < 0);
   assert(find_skill("pressure point") < 0);
   assert(find_skill("fortify") < 0);
   printf("PASS: test_old_skill_names_absent\n");
}

static void test_gap_transit_at_correct_class_level(void)
{
   int sn = find_skill("gap transit");
   assert(sn >= 0);
   assert(skill_table[sn].skill_level[CLASS_CIP] == 45);
   printf("PASS: test_gap_transit_at_correct_class_level\n");
}

static void test_reach_silence_at_correct_class_level(void)
{
   int sn = find_skill("reach silence");
   assert(sn >= 0);
   assert(skill_table[sn].skill_level[CLASS_CIP] == 30);
   printf("PASS: test_reach_silence_at_correct_class_level\n");
}

static void test_applied_understanding_at_correct_class_level(void)
{
   int sn = find_skill("applied understanding");
   assert(sn >= 0);
   assert(skill_table[sn].skill_level[CLASS_PUG] == 35);
   printf("PASS: test_applied_understanding_at_correct_class_level\n");
}

static void test_seven_shade_hold_at_correct_class_level(void)
{
   int sn = find_skill("seven shade hold");
   assert(sn >= 0);
   assert(skill_table[sn].skill_level[CLASS_WAR] == 22);
   printf("PASS: test_seven_shade_hold_at_correct_class_level\n");
}

/* ── stance rename tests ────────────────────────────────────────────── */

static void test_new_stance_names_present(void)
{
   assert(find_stance("Dunmar's Watch") >= 0);
   assert(find_stance("Gap Intuition") >= 0);
   assert(find_stance("Deepest Form") >= 0);
   printf("PASS: test_new_stance_names_present\n");
}

static void test_old_stance_names_absent(void)
{
   assert(find_stance("Warden") < 0);
   assert(find_stance("Cipher") < 0);
   assert(find_stance("Monk") < 0);
   printf("PASS: test_old_stance_names_absent\n");
}

int main(void)
{
   test_new_skill_names_present();
   test_old_skill_names_absent();
   test_gap_transit_at_correct_class_level();
   test_reach_silence_at_correct_class_level();
   test_applied_understanding_at_correct_class_level();
   test_seven_shade_hold_at_correct_class_level();
   test_new_stance_names_present();
   test_old_stance_names_absent();

   printf("All skill rename unit tests passed.\n");
   return 0;
}
