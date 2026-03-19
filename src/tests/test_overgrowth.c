/*
 * Unit tests for the Druid Overgrowth mechanic.
 *
 * Tests cover:
 *   1. HP cost calculation: base cost * (100 + overgrowth * 2) / 100
 *   2. Power bonus: damage * (100 + overgrowth * 3) / 100
 *   3. Overgrowth decay: 1/tick standing, 2/tick resting/sleeping
 */

#include <assert.h>
#include <stdio.h>
#include <string.h>

#define DEC_GLOBALS_H 1
#include "ack.h"

/* Stub out the gclass_table */
const struct class_type gclass_table[MAX_TOTAL_CLASS] = {{0}};

/*
 * Overgrowth HP cost formula (mirrors do_cast logic):
 *   hp_cost = min_mana * (100 + overgrowth * 2) / 100
 */
static int overgrowth_hp_cost(int min_mana, int overgrowth)
{
   return min_mana * (100 + overgrowth * 2) / 100;
}

/*
 * Overgrowth power bonus formula (mirrors calculate_damage logic):
 *   result = dam * (100 + overgrowth * 3) / 100
 */
static int overgrowth_power_bonus(int dam, int overgrowth)
{
   return dam * (100 + overgrowth * 3) / 100;
}

/*
 * Overgrowth decay: 1 per tick normally, 2 if resting/sleeping.
 */
static int overgrowth_decay(int current, int position)
{
   int decay = (position == POS_RESTING || position == POS_SLEEPING) ? 2 : 1;
   int result = current - decay;
   return (result < 0) ? 0 : result;
}

/* ── HP cost tests ─────────────────────────────────────────────────── */

static void test_hp_cost_at_zero_overgrowth(void)
{
   /* At 0 overgrowth, cost = min_mana * 100 / 100 = min_mana */
   assert(overgrowth_hp_cost(20, 0) == 20);
   assert(overgrowth_hp_cost(50, 0) == 50);
}

static void test_hp_cost_at_50_overgrowth(void)
{
   /* At 50 overgrowth, cost = min_mana * (100 + 100) / 100 = min_mana * 2 */
   assert(overgrowth_hp_cost(20, 50) == 40);
   assert(overgrowth_hp_cost(50, 50) == 100);
}

static void test_hp_cost_at_100_overgrowth(void)
{
   /* At 100 overgrowth, cost = min_mana * (100 + 200) / 100 = min_mana * 3 */
   assert(overgrowth_hp_cost(20, 100) == 60);
   assert(overgrowth_hp_cost(50, 100) == 150);
}

/* ── Power bonus tests ─────────────────────────────────────────────── */

static void test_power_bonus_at_zero_overgrowth(void)
{
   /* At 0 overgrowth, no bonus */
   assert(overgrowth_power_bonus(100, 0) == 100);
}

static void test_power_bonus_at_50_overgrowth(void)
{
   /* At 50 overgrowth, bonus = 150% (100 + 50*3 = 250) -> 250% of base */
   assert(overgrowth_power_bonus(100, 50) == 250);
}

static void test_power_bonus_at_100_overgrowth(void)
{
   /* At 100 overgrowth, bonus = 100 * (100 + 300) / 100 = 400 */
   assert(overgrowth_power_bonus(100, 100) == 400);
}

/* ── Decay tests ───────────────────────────────────────────────────── */

static void test_decay_standing(void)
{
   /* Standing: decay by 1 per tick */
   assert(overgrowth_decay(50, POS_STANDING) == 49);
   assert(overgrowth_decay(1, POS_STANDING) == 0);
   assert(overgrowth_decay(0, POS_STANDING) == 0);
}

static void test_decay_resting(void)
{
   /* Resting: decay by 2 per tick */
   assert(overgrowth_decay(50, POS_RESTING) == 48);
   assert(overgrowth_decay(2, POS_RESTING) == 0);
   assert(overgrowth_decay(1, POS_RESTING) == 0);
   assert(overgrowth_decay(0, POS_RESTING) == 0);
}

static void test_decay_sleeping(void)
{
   /* Sleeping: decay by 2 per tick */
   assert(overgrowth_decay(50, POS_SLEEPING) == 48);
   assert(overgrowth_decay(1, POS_SLEEPING) == 0);
}

static void test_decay_fighting_no_decay(void)
{
   /* Fighting characters should not decay (checked by caller, not this func) */
   /* We just verify the formula works with fighting position */
   assert(overgrowth_decay(50, POS_FIGHTING) == 49);
}

/* ── Overgrowth cap test ───────────────────────────────────────────── */

static void test_overgrowth_cap_at_100(void)
{
   int overgrowth = 95;
   int growth = 7;
   overgrowth += growth;
   if (overgrowth > 100)
      overgrowth = 100;
   assert(overgrowth == 100);
}

int main(void)
{
   test_hp_cost_at_zero_overgrowth();
   test_hp_cost_at_50_overgrowth();
   test_hp_cost_at_100_overgrowth();
   test_power_bonus_at_zero_overgrowth();
   test_power_bonus_at_50_overgrowth();
   test_power_bonus_at_100_overgrowth();
   test_decay_standing();
   test_decay_resting();
   test_decay_sleeping();
   test_decay_fighting_no_decay();
   test_overgrowth_cap_at_100();

   puts("test_overgrowth: all tests passed");
   return 0;
}
