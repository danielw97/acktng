#include <assert.h>
#include <stdio.h>

#include "cloak_drain.h"

static void test_damage_matches_psuedo_level(void)
{
    assert(cloak_drain_damage_from_level(47) == 47);
    assert(cloak_drain_damage_from_level(0) == 0);
    assert(cloak_drain_damage_from_level(-10) == 0);
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

int main(void)
{
    test_damage_matches_psuedo_level();
    test_attacker_hp_has_floor();
    test_victim_hp_heals_half_and_caps_at_max();

    puts("test_cloak_drain: all tests passed");
    return 0;
}
