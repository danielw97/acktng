#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include "globals.h"

bool should_abort_for_checkpoint_timeout(int usage_now, int checkpoint, int threshold, bool disable_abort);
int hit_gain(CHAR_DATA *ch);
int mana_gain(CHAR_DATA *ch);
int move_gain(CHAR_DATA *ch);

const struct class_type class_table[MAX_CLASS] = {0};
const struct class_type remort_table[MAX_REMORT] = {0};
const struct class_type adept_table[MAX_CLASS] = {0};
const struct race_type race_table[MAX_RACE] = {0};

int get_curr_int(CHAR_DATA *ch) { (void)ch; return 10; }
int get_curr_wis(CHAR_DATA *ch) { (void)ch; return 10; }
int get_curr_dex(CHAR_DATA *ch) { (void)ch; return 10; }
int get_curr_con(CHAR_DATA *ch) { (void)ch; return 10; }
sh_int get_psuedo_level(CHAR_DATA *ch) { return ch->level; }
long get_max_hp(CHAR_DATA *ch) { (void)ch; return 100; }
long get_max_mana(CHAR_DATA *ch) { (void)ch; return 120; }
long get_max_move(CHAR_DATA *ch) { (void)ch; return 130; }
bool is_fighting(CHAR_DATA *ch) { return ch->fighting != NULL; }

static CHAR_DATA make_test_character(void)
{
    static PC_DATA pcdata;
    static ROOM_INDEX_DATA room;

    CHAR_DATA ch = {0};
    pcdata = (PC_DATA){0};
    room = (ROOM_INDEX_DATA){0};

    ch.pcdata = &pcdata;
    ch.in_room = &room;
    ch.level = 20;
    ch.position = POS_STANDING;
    ch.race = 0;
    ch.hit = 50;
    ch.mana = 60;
    ch.move = 70;

    return ch;
}

static void test_timeout_disabled_never_aborts(void)
{
    assert(!should_abort_for_checkpoint_timeout(100, 1, 0, true));
    assert(!should_abort_for_checkpoint_timeout(100, 95, 3, true));
}

static void test_timeout_requires_strictly_greater_than_threshold(void)
{
    assert(!should_abort_for_checkpoint_timeout(10, 0, 10, false));
    assert(should_abort_for_checkpoint_timeout(11, 0, 10, false));
}

static void test_negative_and_small_deltas_do_not_abort(void)
{
    assert(!should_abort_for_checkpoint_timeout(5, 8, 1, false));
    assert(!should_abort_for_checkpoint_timeout(8, 8, 1, false));
}

static void test_regen_returns_delta_out_of_combat(void)
{
    CHAR_DATA ch = make_test_character();

    assert(hit_gain(&ch) == 30);
    assert(mana_gain(&ch) == 30);
    assert(move_gain(&ch) == 75);
}


static void test_npc_regen_uses_standard_formula(void)
{
    CHAR_DATA ch = make_test_character();
    ch.act = ACT_IS_NPC;

    assert(hit_gain(&ch) == 30);
    assert(mana_gain(&ch) == 30);
}

static void test_regen_is_zero_in_combat(void)
{
    CHAR_DATA ch = make_test_character();
    CHAR_DATA opponent = {0};
    ch.fighting = &opponent;

    assert(hit_gain(&ch) == 0);
    assert(mana_gain(&ch) == 0);
    assert(move_gain(&ch) == 0);
}

int main(void)
{
    test_timeout_disabled_never_aborts();
    test_timeout_requires_strictly_greater_than_threshold();
    test_negative_and_small_deltas_do_not_abort();
    test_regen_returns_delta_out_of_combat();
    test_npc_regen_uses_standard_formula();
    test_regen_is_zero_in_combat();

    puts("test_update: all tests passed");
    return 0;
}
