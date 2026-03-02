#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "config.h"
#define DEC_GLOBALS_H 1
#include "ack.h"

int invasion_test_count_regular_players(int *out_lo, int *out_hi);
int invasion_test_calculate_boss_hp_mod(int level);
int invasion_test_is_invasion_mob(CHAR_DATA *ch);

DESCRIPTOR_DATA *first_desc = NULL;

int get_trust(CHAR_DATA *ch)
{
    return ch->level;
}

sh_int get_psuedo_level(CHAR_DATA *ch)
{
    return ch->level;
}

static void test_count_regular_players_ignores_non_players_and_immortals(void)
{
    DESCRIPTOR_DATA d1 = {0}, d2 = {0}, d3 = {0}, d4 = {0};
    CHAR_DATA p1 = {0}, p2 = {0}, npc = {0}, imm = {0};
    int lo = -1, hi = -1;

    p1.level = 12;
    p2.level = 38;
    npc.level = 25;
    npc.act = ACT_IS_NPC;
    imm.level = LEVEL_IMMORTAL;

    d1.connected = CON_PLAYING;
    d1.character = &p1;
    d1.next = &d2;

    d2.connected = CON_PLAYING;
    d2.character = &npc;
    d2.next = &d3;

    d3.connected = CON_PLAYING;
    d3.character = &imm;
    d3.next = &d4;

    d4.connected = CON_PLAYING;
    d4.character = &p2;

    first_desc = &d1;

    assert(invasion_test_count_regular_players(&lo, &hi) == 2);
    assert(lo == 12);
    assert(hi == 38);
}

static void test_count_regular_players_no_regular_players_sets_bounds_to_zero(void)
{
    DESCRIPTOR_DATA d1 = {0};
    CHAR_DATA imm = {0};
    int lo = 99, hi = 99;

    imm.level = LEVEL_IMMORTAL;
    d1.connected = CON_PLAYING;
    d1.character = &imm;
    first_desc = &d1;

    assert(invasion_test_count_regular_players(&lo, &hi) == 0);
    assert(lo == 0);
    assert(hi == 0);
}

static void test_boss_hp_mod_scaling_and_caps(void)
{
    assert(invasion_test_calculate_boss_hp_mod(1) == 115);
    assert(invasion_test_calculate_boss_hp_mod(30) == 13600);
    assert(invasion_test_calculate_boss_hp_mod(129) == 249715);
    assert(invasion_test_calculate_boss_hp_mod(130) == 250000);
    assert(invasion_test_calculate_boss_hp_mod(-20) == 6100);
}

static void test_is_invasion_mob_requires_npc_tag(void)
{
    CHAR_DATA mob = {0};
    CHAR_DATA player = {0};

    mob.act = ACT_IS_NPC;
    mob.extract_timer = -999;
    assert(invasion_test_is_invasion_mob(&mob) != 0);

    mob.extract_timer = 0;
    assert(invasion_test_is_invasion_mob(&mob) == 0);

    player.extract_timer = -999;
    assert(invasion_test_is_invasion_mob(&player) == 0);
}

int main(void)
{
    test_count_regular_players_ignores_non_players_and_immortals();
    test_count_regular_players_no_regular_players_sets_bounds_to_zero();
    test_boss_hp_mod_scaling_and_caps();
    test_is_invasion_mob_requires_npc_tag();

    puts("test_invasion: all tests passed");
    return 0;
}
