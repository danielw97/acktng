#include <assert.h>
#include <string.h>
#include <stdbool.h>

void spell_dam_damage_params(int flag1, int best_level, int *base, int *d1, int *d2);
int spell_dam_base_penalty_for_summon(int summon);
const char *spell_dam_special_for_summon(int summon);

enum
{
   MORTAL = 1,
   REMORT = 2,
   ADEPT = 3,
   WATER_ELEMENTAL = 1,
   FIRE_ELEMENTAL = 2,
   EARTH_ELEMENTAL = 3,
   IRON_GOLEM = 4,
   DIAMOND_GOLEM = 5,
   SKELETON = 6,
   SOUL_THIEF = 7,
   HOLY_AVENGER = 8,
   THOUGHT_DEVOURER = 9
};

static void test_damage_params_for_all_spell_tiers(void)
{
   int base;
   int d1;
   int d2;

   spell_dam_damage_params(MORTAL, 55, &base, &d1, &d2);
   assert(base == 10);
   assert(d1 == 11);
   assert(d2 == 10);

   spell_dam_damage_params(REMORT, 40, &base, &d1, &d2);
   assert(base == 150);
   assert(d1 == 20);
   assert(d2 == 20);

   spell_dam_damage_params(ADEPT, 21, &base, &d1, &d2);
   assert(base == 350);
   assert(d1 == 63);
   assert(d2 == 25);
}

static void test_damage_params_for_unknown_tier_defaults_to_zero(void)
{
   int base = -1;
   int d1 = -1;
   int d2 = -1;

   spell_dam_damage_params(0, 99, &base, &d1, &d2);
   assert(base == 0);
   assert(d1 == 0);
   assert(d2 == 0);
}

static void test_summon_special_mapping(void)
{
   assert(strcmp(spell_dam_special_for_summon(WATER_ELEMENTAL), "spec_summon_water") == 0);
   assert(strcmp(spell_dam_special_for_summon(FIRE_ELEMENTAL), "spec_summon_fire") == 0);
   assert(strcmp(spell_dam_special_for_summon(EARTH_ELEMENTAL), "spec_summon_earth") == 0);
   assert(strcmp(spell_dam_special_for_summon(SKELETON), "spec_summon_undead") == 0);
   assert(strcmp(spell_dam_special_for_summon(HOLY_AVENGER), "spec_summon_holy") == 0);
   assert(strcmp(spell_dam_special_for_summon(SOUL_THIEF), "spec_summon_shadow") == 0);
   assert(strcmp(spell_dam_special_for_summon(IRON_GOLEM), "spec_summon_metal") == 0);
   assert(strcmp(spell_dam_special_for_summon(DIAMOND_GOLEM), "spec_summon_metal") == 0);
   assert(strcmp(spell_dam_special_for_summon(THOUGHT_DEVOURER), "spec_summon_thought") == 0);
   assert(spell_dam_special_for_summon(-1) == NULL);
}

static void test_summon_penalty_mapping(void)
{
   assert(spell_dam_base_penalty_for_summon(WATER_ELEMENTAL) == 40);
   assert(spell_dam_base_penalty_for_summon(FIRE_ELEMENTAL) == 40);
   assert(spell_dam_base_penalty_for_summon(EARTH_ELEMENTAL) == 35);
   assert(spell_dam_base_penalty_for_summon(SKELETON) == 50);
   assert(spell_dam_base_penalty_for_summon(HOLY_AVENGER) == 40);
   assert(spell_dam_base_penalty_for_summon(SOUL_THIEF) == 35);
   assert(spell_dam_base_penalty_for_summon(IRON_GOLEM) == 25);
   assert(spell_dam_base_penalty_for_summon(DIAMOND_GOLEM) == 15);
   assert(spell_dam_base_penalty_for_summon(THOUGHT_DEVOURER) == 15);
   assert(spell_dam_base_penalty_for_summon(-1) == 0);
}

int main(void)
{
   test_damage_params_for_all_spell_tiers();
   test_damage_params_for_unknown_tier_defaults_to_zero();
   test_summon_penalty_mapping();
   test_summon_special_mapping();
   return 0;
}
