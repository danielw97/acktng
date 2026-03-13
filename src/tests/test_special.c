#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define DEC_GLOBALS_H 1
#include "ack.h"

int summon_master_heal_chance_for_test(int master_hit, int master_max_hp, int thematic_bonus);
int summon_special_count_for_test(void);
bool summon_special_casts_in_combat_for_test(CHAR_DATA *ch, int index);

struct skill_type skill_table[MAX_SKILL];

static int g_cast_count = 0;

int get_max_hp(CHAR_DATA *ch)
{
    return ch != NULL ? ch->max_hit : 0;
}

int number_range(int from, int to)
{
    (void)to;
    return from;
}

static bool test_spell_fun(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{
    (void)sn;
    (void)level;
    (void)ch;
    (void)vo;
    (void)obj;

    g_cast_count++;
    return TRUE;
}

int skill_lookup(const char *name)
{
    static const char *const known[] = {
        "chill touch", "acid blast", "weaken", "fireball", "high explosive", "curse",
        "earthquake", "harm", "poison", "holy wrath", "dispel evil", "energy drain",
        "lightning bolt", "thought vise", "mind bolt", "feeble mind", "heal"
    };

    for (int i = 0; i < (int)(sizeof(known) / sizeof(known[0])); i++)
    {
        if (strcmp(name, known[i]) == 0)
            return i + 1;
    }

    return -1;
}

static void clear_character(CHAR_DATA *ch)
{
    memset(ch, 0, sizeof(*ch));
}

static void initialize_spell_table(void)
{
    for (int i = 0; i < MAX_SKILL; i++)
        skill_table[i].spell_fun = test_spell_fun;

    for (int i = 1; i <= 17; i++)
        skill_table[i].spell_fun = test_spell_fun;
}

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

static void test_all_summon_specs_cast_in_combat(void)
{
    CHAR_DATA summon;
    CHAR_DATA target;

    clear_character(&summon);
    clear_character(&target);

    initialize_spell_table();

    summon.position = POS_FIGHTING;
    summon.level = 80;
    summon.fighting = &target;

    for (int i = 0; i < summon_special_count_for_test(); i++)
    {
        g_cast_count = 0;
        assert(summon_special_casts_in_combat_for_test(&summon, i) == TRUE);
        assert(g_cast_count == 1);
    }
}

int main(void)
{
    test_no_chance_when_master_full_or_invalid();
    test_chance_increases_as_master_hp_drops();
    test_chance_respects_bounds();
    test_all_summon_specs_cast_in_combat();

    puts("test_special: all tests passed");
    return 0;
}
