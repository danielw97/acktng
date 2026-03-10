#include <assert.h>
#include <string.h>

/* Skip globals.h from ack.h to avoid incomplete-type extern arrays in unit test TU. */
#define DEC_GLOBALS_H 1
#include "ack.h"
#include "item_generation_tables.h"

int get_obj_stat_weight_index(const OBJ_DATA *obj);
char *get_suffix(OBJ_DATA *obj);
char *get_prefix(OBJ_DATA *obj);
char *get_wear_name(OBJ_DATA *obj);
bool is_jewelry(OBJ_DATA *obj);



static void test_item_generation_tables_anchor_values(void)
{
    assert(weapon_table[1].hr_div == 6);
    assert(weapon_table[15].ac_div == 8.0f);

    assert(armor_table[1].ac_div == 2.6f);
    assert(armor_table[15].hp_div == 2.0f);

    assert(jewelry_table[3].spellpower_div > 6.66f);
    assert(jewelry_table[3].spellpower_div < 6.68f);
    assert(jewelry_table[12].mana_div == 7);
}

static void test_get_obj_stat_weight_index_clamps_bounds(void)
{
    OBJ_DATA obj;

    obj.weight = -10;
    assert(get_obj_stat_weight_index(&obj) == 0);

    obj.weight = 0;
    assert(get_obj_stat_weight_index(&obj) == 0);

    obj.weight = 8;
    assert(get_obj_stat_weight_index(&obj) == 8);

    obj.weight = 15;
    assert(get_obj_stat_weight_index(&obj) == 15);

    obj.weight = 99;
    assert(get_obj_stat_weight_index(&obj) == 15);
}

static void assert_weight_band_progression(const struct obj_stat_type *table, int start, int end)
{
    int weight;

    for (weight = start + 1; weight <= end; weight++)
    {
        const struct obj_stat_type *prev = &table[weight - 1];
        const struct obj_stat_type *curr = &table[weight];

        /* As weight rises within a band, offensive gains should taper. */
        assert(curr->hr_div > prev->hr_div);
        assert(curr->dr_div > prev->dr_div);

        /* AC divisor decreases so heavier pieces grant stronger AC. */
        assert(curr->ac_div < prev->ac_div);

        /* Vital stat divisors decrease so heavier pieces scale better. */
        assert(curr->hp_div < prev->hp_div);
        assert(curr->move_div < prev->move_div);
        assert(curr->mana_div < prev->mana_div);
    }
}

static void test_weight_progression_tables_are_consistent(void)
{
    const int band_starts[] = {1, 6, 11};
    const int band_ends[] = {5, 10, 15};
    int band;

    for (band = 0; band < 3; band++)
    {
        assert_weight_band_progression(weapon_table, band_starts[band], band_ends[band]);
        assert_weight_band_progression(armor_table, band_starts[band], band_ends[band]);
        assert_weight_band_progression(jewelry_table, band_starts[band], band_ends[band]);
    }
}

static void test_item_strings_follow_weight_thresholds(void)
{
    OBJ_DATA obj;
    memset(&obj, 0, sizeof(obj));

    obj.weight = 5;
    assert(strcmp(get_suffix(&obj), "of the Arcanist") == 0);
    obj.weight = 6;
    assert(strcmp(get_suffix(&obj), "of the Fighter") == 0);
    obj.weight = 10;
    assert(strcmp(get_suffix(&obj), "of the Fighter") == 0);
    obj.weight = 11;
    assert(strcmp(get_suffix(&obj), "of the Defender") == 0);

    obj.weight = 2;
    assert(strcmp(get_prefix(&obj), "Magus ") == 0);
    obj.weight = 4;
    assert(strcmp(get_prefix(&obj), "Tricksters ") == 0);
    obj.weight = 8;
    assert(strcmp(get_prefix(&obj), "Berserkers ") == 0);
    obj.weight = 12;
    assert(strcmp(get_prefix(&obj), "Protectors ") == 0);
    obj.weight = 14;
    assert(strcmp(get_prefix(&obj), "Conquerors ") == 0);
    obj.weight = 9;
    assert(strcmp(get_prefix(&obj), "") == 0);
}

static void test_get_wear_name_varies_with_weight(void)
{
    OBJ_DATA obj;
    memset(&obj, 0, sizeof(obj));

    obj.item_type = ITEM_ARMOR;
    SET_BIT(obj.wear_flags, ITEM_WEAR_HEAD);

    obj.weight = 4;
    assert(strcmp(get_wear_name(&obj), "Hat") == 0);
    obj.weight = 8;
    assert(strcmp(get_wear_name(&obj), "Helm") == 0);
    obj.weight = 12;
    assert(strcmp(get_wear_name(&obj), "Helmet") == 0);

    obj.wear_flags = 0;
    SET_BIT(obj.wear_flags, ITEM_WEAR_BODY);
    obj.weight = 4;
    assert(strcmp(get_wear_name(&obj), "Robes") == 0);
    obj.weight = 8;
    assert(strcmp(get_wear_name(&obj), "Hauberk") == 0);
    obj.weight = 12;
    assert(strcmp(get_wear_name(&obj), "Platemail Armor") == 0);
}

static void test_get_wear_name_handles_weapon_light_and_jewelry(void)
{
    OBJ_DATA obj;
    memset(&obj, 0, sizeof(obj));

    obj.item_type = ITEM_WEAPON;
    obj.value[3] = 3;
    assert(strcmp(get_wear_name(&obj), "Sword") == 0);

    obj.item_type = ITEM_LIGHT;
    obj.level = 90;
    obj.value[2] = 0;
    assert(strcmp(get_wear_name(&obj), "Torch") == 0);
    assert(obj.value[2] == -1);

    obj.level = 120;
    assert(strcmp(get_wear_name(&obj), "Lantern") == 0);

    memset(&obj, 0, sizeof(obj));
    assert(!is_jewelry(&obj));
    SET_BIT(obj.wear_flags, ITEM_WEAR_NECK);
    assert(is_jewelry(&obj));
}


static void test_weapon_names_match_attack_types(void)
{
    OBJ_DATA obj;
    memset(&obj, 0, sizeof(obj));

    obj.item_type = ITEM_WEAPON;

    obj.value[3] = 1;
    assert(strcmp(get_wear_name(&obj), "Blade") == 0);
    obj.value[3] = 2;
    assert(strcmp(get_wear_name(&obj), "Dagger") == 0);
    obj.value[3] = 3;
    assert(strcmp(get_wear_name(&obj), "Sword") == 0);
    obj.value[3] = 4;
    assert(strcmp(get_wear_name(&obj), "Whip") == 0);
    obj.value[3] = 5;
    assert(strcmp(get_wear_name(&obj), "Claws") == 0);
    obj.value[3] = 6;
    assert(strcmp(get_wear_name(&obj), "Blaster") == 0);
    obj.value[3] = 7;
    assert(strcmp(get_wear_name(&obj), "Mace") == 0);
    obj.value[3] = 8;
    assert(strcmp(get_wear_name(&obj), "Hammer") == 0);
    obj.value[3] = 9;
    assert(strcmp(get_wear_name(&obj), "Hook") == 0);
    obj.value[3] = 10;
    assert(strcmp(get_wear_name(&obj), "Fangs") == 0);
    obj.value[3] = 11;
    assert(strcmp(get_wear_name(&obj), "Spear") == 0);
    obj.value[3] = 12;
    assert(strcmp(get_wear_name(&obj), "Siphon") == 0);

    obj.value[3] = 0;
    obj.extra_flags = 0;
    assert(strcmp(get_wear_name(&obj), "Cudgel") == 0);

    SET_BIT(obj.extra_flags, ITEM_FIST);
    assert(strcmp(get_wear_name(&obj), "Fists") == 0);
}
int main(void)
{
    test_item_generation_tables_anchor_values();
    test_get_obj_stat_weight_index_clamps_bounds();
    test_weight_progression_tables_are_consistent();
    test_item_strings_follow_weight_thresholds();
    test_get_wear_name_varies_with_weight();
    test_get_wear_name_handles_weapon_light_and_jewelry();
    test_weapon_names_match_attack_types();
    return 0;
}
