#include <assert.h>
#include <stdio.h>

int spell_regen_base_heal(int mage_level, int sorcerer_level, int wizard_level, int spellpower);

static void test_base_and_mage_level_scaling(void)
{
    assert(spell_regen_base_heal(0, 0, 0, 0) == 10);
    assert(spell_regen_base_heal(11, 0, 0, 0) == 15);
}

static void test_remort_bonus_uses_higher_sorcerer_or_wizard(void)
{
    assert(spell_regen_base_heal(20, 4, 0, 0) == 27);
    assert(spell_regen_base_heal(20, 4, 9, 0) == 32);
}

static void test_spellpower_quarter_added(void)
{
    assert(spell_regen_base_heal(20, 0, 0, 19) == 24);
    assert(spell_regen_base_heal(20, 0, 0, 20) == 25);
}

int main(void)
{
    test_base_and_mage_level_scaling();
    test_remort_bonus_uses_higher_sorcerer_or_wizard();
    test_spellpower_quarter_added();

    puts("test_magic4: all tests passed");
    return 0;
}
