#include <assert.h>
#include <stdbool.h>
#include <string.h>

int combo_damage_multiplier_for_max_combo(int max);
bool combo_has_duplicate_skill(const int *combo_values, int max, int gsn);
const char *war_attack_miss_to_victim_format(void);

static void test_combo_damage_multiplier_for_supported_values(void)
{
    assert(combo_damage_multiplier_for_max_combo(4) == 23);
    assert(combo_damage_multiplier_for_max_combo(5) == 18);
    assert(combo_damage_multiplier_for_max_combo(6) == 15);
    assert(combo_damage_multiplier_for_max_combo(99) == 23);
}

static void test_combo_has_duplicate_skill(void)
{
    const int combo[] = {100, 200, 300, -1, -1, -1};

    assert(combo_has_duplicate_skill(combo, 6, 100) == true);
    assert(combo_has_duplicate_skill(combo, 6, 200) == true);
    assert(combo_has_duplicate_skill(combo, 6, 999) == false);
}


static void test_war_attack_miss_message_uses_attacker_token(void)
{
    assert(strcmp(war_attack_miss_to_victim_format(), "$n tries to %s you, but misses!") == 0);
}

int main(void)
{
    test_combo_damage_multiplier_for_supported_values();
    test_combo_has_duplicate_skill();
    test_war_attack_miss_message_uses_attacker_token();
    return 0;
}
