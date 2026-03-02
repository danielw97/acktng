#include <assert.h>
#include <stdio.h>
#include <string.h>

#define DEC_GLOBALS_H 1
#include "magic.h"
#include "cloak.h"

static void test_cloak_reactive_can_trigger_only_for_non_physical(void)
{
    assert(cloak_reactive_can_trigger(ELE_PHYSICAL) == FALSE);
    assert(cloak_reactive_can_trigger(ELE_FIRE) == TRUE);
    assert(cloak_reactive_can_trigger(ELE_WATER | ELE_AIR) == TRUE);
    assert(cloak_reactive_can_trigger(ELE_PHYSICAL | ELE_FIRE) == FALSE);
    assert(cloak_reactive_can_trigger(ELE_NONE) == TRUE);
}

static void test_damage_matches_psuedo_level(void)
{
    assert(cloak_drain_damage_from_level(47) == 47);
    assert(cloak_drain_damage_from_level(0) == 0);
    assert(cloak_drain_damage_from_level(-10) == 0);
}

static void test_level_damage_reduction_is_half_level(void)
{
    assert(cloak_level_damage_reduction(48) == 24);
    assert(cloak_level_damage_reduction(47) == 23);
    assert(cloak_level_damage_reduction(0) == 0);
    assert(cloak_level_damage_reduction(1) == 0);
    assert(cloak_level_damage_reduction(101) == 50);
    assert(cloak_level_damage_reduction(-10) == 0);
}

static void test_oathbreaker_avoidance_chance_scales_from_level(void)
{
    assert(cloak_oathbreaker_avoidance_chance(48) == 4);
    assert(cloak_oathbreaker_avoidance_chance(99) == 9);
    assert(cloak_oathbreaker_avoidance_chance(0) == 0);
    assert(cloak_oathbreaker_avoidance_chance(-5) == 0);
}

static void test_transcendence_avoidance_chance_scales_from_level(void)
{
    assert(cloak_transcendence_avoidance_chance(48) == 3);
    assert(cloak_transcendence_avoidance_chance(150) == 10);
    assert(cloak_transcendence_avoidance_chance(0) == 0);
    assert(cloak_transcendence_avoidance_chance(-1) == 0);
}

static void test_oathbreaker_avoidance_conditions(void)
{
    assert(cloak_oathbreaker_avoids_spell_damage(100, ELE_FIRE, TRUE, 48, 3) == TRUE);
    assert(cloak_oathbreaker_avoids_spell_damage(100, ELE_FIRE, TRUE, 48, 4) == FALSE);
    assert(cloak_oathbreaker_avoids_spell_damage(100, ELE_PHYSICAL, TRUE, 48, 0) == FALSE);
    assert(cloak_oathbreaker_avoids_spell_damage(0, ELE_FIRE, TRUE, 48, 0) == FALSE);
    assert(cloak_oathbreaker_avoids_spell_damage(100, ELE_FIRE, FALSE, 48, 0) == FALSE);
}

static void test_transcendence_avoidance_conditions(void)
{
    assert(cloak_transcendence_avoids_physical_hit(100, ELE_PHYSICAL, TYPE_HIT, TRUE, 48, 2) == TRUE);
    assert(cloak_transcendence_avoids_physical_hit(100, ELE_PHYSICAL, TYPE_HIT, TRUE, 48, 3) == FALSE);
    assert(cloak_transcendence_avoids_physical_hit(100, ELE_FIRE, TYPE_HIT, TRUE, 48, 0) == FALSE);
    assert(cloak_transcendence_avoids_physical_hit(100, ELE_PHYSICAL, TYPE_UNDEFINED, TRUE, 48, 0) == TRUE);
    assert(cloak_transcendence_avoids_physical_hit(0, ELE_PHYSICAL, TYPE_HIT, TRUE, 48, 0) == FALSE);
    assert(cloak_transcendence_avoids_physical_hit(100, ELE_PHYSICAL, TYPE_HIT, FALSE, 48, 0) == FALSE);
}

static void test_attacker_hp_has_floor(void)
{
    assert(cloak_drain_attacker_hp_after_hit(100, 47) == 53);
    assert(cloak_drain_attacker_hp_after_hit(20, 25) == 10);
}

static void test_victim_hp_heals_half_and_caps_at_max(void)
{
    assert(cloak_drain_victim_hp_after_hit(100, 200, 47) == 123);
    assert(cloak_drain_victim_hp_after_hit(190, 200, 47) == 200);
}


static void test_cloak_adept_helpers(void)
{
    CHAR_DATA ch;
    memset(&ch, 0, sizeof(ch));
    ch.level = 50;

    assert(cloak_adept_hitroll_bonus(&ch) == 0);
    assert(cloak_adept_defense_bonus(&ch) == 0);
    assert(cloak_adept_scale_damage(&ch, 100) == 100);

    SET_BIT(ch.affected_by, AFF_CLOAK_ADEPT);
    assert(cloak_adept_hitroll_bonus(&ch) == 100);
    assert(cloak_adept_defense_bonus(&ch) == 5);
    assert(cloak_adept_scale_damage(&ch, 100) == 120);
}

int main(void)
{
    test_cloak_reactive_can_trigger_only_for_non_physical();
    test_damage_matches_psuedo_level();
    test_level_damage_reduction_is_half_level();
    test_oathbreaker_avoidance_chance_scales_from_level();
    test_transcendence_avoidance_chance_scales_from_level();
    test_oathbreaker_avoidance_conditions();
    test_transcendence_avoidance_conditions();
    test_attacker_hp_has_floor();
    test_victim_hp_heals_half_and_caps_at_max();
    test_cloak_adept_helpers();

    puts("test_cloak: all tests passed");
    return 0;
}
