#include <assert.h>
#include <stdio.h>

int summon_master_heal_chance_for_test(int master_hit, int master_max_hp, int thematic_bonus);

static void test_no_chance_when_master_full_or_invalid(void)
{
    assert(summon_master_heal_chance_for_test(100, 100, 30) == 0);
    assert(summon_master_heal_chance_for_test(50, 0, 30) == 0);
}

static void test_chance_increases_as_master_hp_drops(void)
{
    int high_hp = summon_master_heal_chance_for_test(90, 100, 20);
    int low_hp = summon_master_heal_chance_for_test(25, 100, 20);

    assert(high_hp == 30);
    assert(low_hp == 95);
    assert(low_hp > high_hp);
}

static void test_chance_respects_bounds(void)
{
    assert(summon_master_heal_chance_for_test(99, 100, -50) == 5);
    assert(summon_master_heal_chance_for_test(-10, 100, 0) == 95);
}

int main(void)
{
    test_no_chance_when_master_full_or_invalid();
    test_chance_increases_as_master_hp_drops();
    test_chance_respects_bounds();

    puts("test_special: all tests passed");
    return 0;
}
