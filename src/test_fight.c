#include <assert.h>
#include <stdio.h>
#include <string.h>

/* Skip globals.h from ack.h to avoid incomplete-type extern arrays in unit test TU. */
#define DEC_GLOBALS_H 1
#include "ack.h"

/* Test doubles for external dependencies used by get_counter/get_evasion_piercing. */
sh_int gsn_counter = 1;
const struct stance_app_type stance_app[MAX_STANCE] = {
    [2] = {.speed_mod = 3},
};

static bool skill_available = TRUE;
static OBJ_DATA *left_hand = NULL;
static OBJ_DATA *right_hand = NULL;

int get_curr_dex(CHAR_DATA *ch)
{
    return ch->hitroll;
}

int get_speed(CHAR_DATA *ch)
{
    return ch->wait;
}

bool can_use_skill(CHAR_DATA *ch, int gsn)
{
    (void)ch;
    (void)gsn;
    return skill_available;
}

OBJ_DATA *get_eq_char(CHAR_DATA *ch, int iWear)
{
    (void)ch;

    if (iWear == WEAR_HOLD_HAND_L)
        return left_hand;
    if (iWear == WEAR_HOLD_HAND_R)
        return right_hand;

    return NULL;
}

sh_int get_psuedo_level(CHAR_DATA *ch)
{
    return ch->level;
}

int get_counter(CHAR_DATA *ch);
int get_evasion_piercing(CHAR_DATA *ch);

static void clear_character(CHAR_DATA *ch)
{
    memset(ch, 0, sizeof(*ch));
    ch->position = POS_STANDING;
}

static void test_get_counter_requires_awake(void)
{
    CHAR_DATA ch;

    clear_character(&ch);
    ch.position = POS_SLEEPING;
    ch.act = ACT_IS_NPC;
    ch.skills = MOB_COUNTER;

    assert(get_counter(&ch) == 0);
}

static void test_get_counter_requires_training(void)
{
    CHAR_DATA npc;
    CHAR_DATA pc;

    clear_character(&npc);
    npc.act = ACT_IS_NPC;
    assert(get_counter(&npc) == 0);

    clear_character(&pc);
    skill_available = FALSE;
    assert(get_counter(&pc) == 0);
    skill_available = TRUE;
}

static void test_get_counter_applies_modifiers(void)
{
    CHAR_DATA ch;
    OBJ_DATA fist_left;
    OBJ_DATA fist_right;

    clear_character(&ch);
    ch.act = ACT_IS_NPC | ACT_SOLO;
    ch.skills = MOB_COUNTER;
    ch.level = 62;
    ch.hitroll = 25;
    ch.wait = 2;
    ch.affected_by = AFF_CLOAK_ADEPT;
    ch.remort[CLASS_MON] = 1;
    ch.adept[CLASS_MAR] = 1;

    memset(&fist_left, 0, sizeof(fist_left));
    memset(&fist_right, 0, sizeof(fist_right));
    fist_left.extra_flags = ITEM_FIST;
    fist_right.extra_flags = ITEM_FIST;

    left_hand = &fist_left;
    right_hand = &fist_right;

    assert(get_counter(&ch) == 72);

    left_hand = NULL;
    right_hand = NULL;
}

static void test_get_evasion_piercing_composition(void)
{
    CHAR_DATA ch;

    clear_character(&ch);
    ch.act = ACT_IS_NPC | ACT_SOLO;
    ch.wait = 2;
    ch.stance = 2;
    ch.pierce_mod = 7;

    assert(get_evasion_piercing(&ch) == 25);

    ch.position = POS_SLEEPING;
    assert(get_evasion_piercing(&ch) == 0);
}

int main(void)
{
    test_get_counter_requires_awake();
    test_get_counter_requires_training();
    test_get_counter_applies_modifiers();
    test_get_evasion_piercing_composition();

    puts("test_fight: all tests passed");
    return 0;
}
