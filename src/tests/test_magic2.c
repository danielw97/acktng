#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define DEC_GLOBALS_H 1
#include "ack.h"

/* Function under test */
void aoe_damage(CHAR_DATA *ch, OBJ_DATA *obj, int sn, int level,
                int min_dam, int max_dam, int element, int flags);

#define AOE_SAVES      (1 << 0)
#define AOE_SKIP_GROUP (1 << 1)

/* ------------------------------------------------------------------ */
/* Test infrastructure                                                 */
/* ------------------------------------------------------------------ */

static int sp_damage_call_count;
static CHAR_DATA *sp_damage_last_victim;
static int sp_damage_last_dam;

/* Reset tracking state before each test. */
static void reset_tracking(void)
{
    sp_damage_call_count = 0;
    sp_damage_last_victim = NULL;
    sp_damage_last_dam = 0;
}

/* Helpers to zero-initialise a PC or NPC character. */
static void clear_character(CHAR_DATA *ch)
{
    memset(ch, 0, sizeof(*ch));
}

static void make_npc(CHAR_DATA *ch)
{
    clear_character(ch);
    ch->act = ACT_IS_NPC;
}

static void make_pc(CHAR_DATA *ch)
{
    clear_character(ch);
    /* act == 0: no ACT_IS_NPC bit */
}

/* Build a room whose first_person list is the NULL-terminated array
 * of CHAR_DATA pointers supplied. Each character's in_room is set to
 * point at the room and next_in_room is chained. */
static void build_room(ROOM_INDEX_DATA *room, CHAR_DATA **chars, int n)
{
    memset(room, 0, sizeof(*room));
    room->first_person = (n > 0) ? chars[0] : NULL;
    for (int i = 0; i < n; i++)
    {
        chars[i]->in_room = room;
        chars[i]->next_in_room = (i + 1 < n) ? chars[i + 1] : NULL;
    }
}

/* ------------------------------------------------------------------ */
/* Stubs for external functions called by aoe_damage()                */
/* ------------------------------------------------------------------ */

bool sp_damage(OBJ_DATA *obj, CHAR_DATA *ch, CHAR_DATA *victim,
               int dam, int type, int sn, bool show_msg)
{
    sp_damage_call_count++;
    sp_damage_last_victim = victim;
    sp_damage_last_dam = dam;
    return TRUE;
}

/* saves_spell stub: controlled per-test via this flag. */
static bool stub_saves_spell_result = FALSE;

bool saves_spell(int level, CHAR_DATA *victim)
{
    return stub_saves_spell_result;
}

/* number_range stub: always return max so damage is deterministic. */
int number_range(int from, int to)
{
    return to;
}

/* is_same_group stub: controlled per-test. */
static bool stub_is_same_group_result = FALSE;

bool is_same_group(CHAR_DATA *ach, CHAR_DATA *bch)
{
    return stub_is_same_group_result;
}

/* CREF / CUREF call char_reference / char_unreference. */
void char_reference(struct char_ref_type *ref)
{
    /* Stub: update the tracked variable to follow next_in_room. */
    if (ref->type == CHAR_NEXTROOM && *ref->var != NULL)
        /* nothing needed - vch_next is already set inside the loop */;
}

void char_unreference(CHAR_DATA **var)
{
    /* nothing needed in unit tests */
    (void)var;
}

/* ------------------------------------------------------------------ */
/* Tests                                                               */
/* ------------------------------------------------------------------ */

/* NPC caster hits both PCs in the room. */
static void test_npc_hits_all_pcs(void)
{
    ROOM_INDEX_DATA room;
    CHAR_DATA npc, pc1, pc2;
    CHAR_DATA *people[] = {&npc, &pc1, &pc2};

    make_npc(&npc);
    make_pc(&pc1);
    make_pc(&pc2);
    build_room(&room, people, 3);

    reset_tracking();
    stub_is_same_group_result = FALSE;
    stub_saves_spell_result = FALSE;
    aoe_damage(&npc, NULL, 1, 30, 50, 50, 0 /* element */, 0);

    assert(sp_damage_call_count == 2);
}

/* PC caster hits both NPCs in the room. */
static void test_pc_hits_all_npcs(void)
{
    ROOM_INDEX_DATA room;
    CHAR_DATA pc, npc1, npc2;
    CHAR_DATA *people[] = {&pc, &npc1, &npc2};

    make_pc(&pc);
    make_npc(&npc1);
    make_npc(&npc2);
    build_room(&room, people, 3);

    reset_tracking();
    stub_is_same_group_result = FALSE;
    stub_saves_spell_result = FALSE;
    aoe_damage(&pc, NULL, 1, 30, 50, 50, 0, 0);

    assert(sp_damage_call_count == 2);
}

/* Caster is never hit by its own AoE. */
static void test_caster_not_hit(void)
{
    ROOM_INDEX_DATA room;
    CHAR_DATA npc;
    CHAR_DATA *people[] = {&npc};

    make_npc(&npc);
    build_room(&room, people, 1);

    reset_tracking();
    aoe_damage(&npc, NULL, 1, 30, 50, 50, 0, 0);

    assert(sp_damage_call_count == 0);
}

/* NPC does not hit other NPCs (same type). */
static void test_npc_does_not_hit_npc(void)
{
    ROOM_INDEX_DATA room;
    CHAR_DATA npc1, npc2;
    CHAR_DATA *people[] = {&npc1, &npc2};

    make_npc(&npc1);
    make_npc(&npc2);
    build_room(&room, people, 2);

    reset_tracking();
    aoe_damage(&npc1, NULL, 1, 30, 50, 50, 0, 0);

    assert(sp_damage_call_count == 0);
}

/* PC does not hit other PCs (same type). */
static void test_pc_does_not_hit_pc(void)
{
    ROOM_INDEX_DATA room;
    CHAR_DATA pc1, pc2;
    CHAR_DATA *people[] = {&pc1, &pc2};

    make_pc(&pc1);
    make_pc(&pc2);
    build_room(&room, people, 2);

    reset_tracking();
    aoe_damage(&pc1, NULL, 1, 30, 50, 50, 0, 0);

    assert(sp_damage_call_count == 0);
}

/* AOE_SKIP_GROUP skips grouped targets. */
static void test_skip_group_flag_skips_grouped_targets(void)
{
    ROOM_INDEX_DATA room;
    CHAR_DATA npc, pc1, pc2;
    CHAR_DATA *people[] = {&npc, &pc1, &pc2};

    make_npc(&npc);
    make_pc(&pc1);
    make_pc(&pc2);
    build_room(&room, people, 3);

    reset_tracking();
    stub_is_same_group_result = TRUE;  /* all are "same group" */
    stub_saves_spell_result = FALSE;
    aoe_damage(&npc, NULL, 1, 30, 50, 50, 0, AOE_SKIP_GROUP);

    assert(sp_damage_call_count == 0);
}

/* Without AOE_SKIP_GROUP, grouped targets are still hit. */
static void test_no_skip_group_flag_hits_grouped_targets(void)
{
    ROOM_INDEX_DATA room;
    CHAR_DATA npc, pc1;
    CHAR_DATA *people[] = {&npc, &pc1};

    make_npc(&npc);
    make_pc(&pc1);
    build_room(&room, people, 2);

    reset_tracking();
    stub_is_same_group_result = TRUE;
    stub_saves_spell_result = FALSE;
    aoe_damage(&npc, NULL, 1, 30, 50, 50, 0, 0 /* no AOE_SKIP_GROUP */);

    assert(sp_damage_call_count == 1);
}

/* AOE_SAVES halves damage on a successful save. */
static void test_saves_halves_damage(void)
{
    ROOM_INDEX_DATA room;
    CHAR_DATA npc, pc1;
    CHAR_DATA *people[] = {&npc, &pc1};

    make_npc(&npc);
    make_pc(&pc1);
    build_room(&room, people, 2);

    reset_tracking();
    stub_saves_spell_result = TRUE;  /* save succeeds → half damage */
    stub_is_same_group_result = FALSE;
    aoe_damage(&npc, NULL, 1, 30, 100, 100, 0, AOE_SAVES);

    assert(sp_damage_call_count == 1);
    assert(sp_damage_last_dam == 50);
}

/* Without AOE_SAVES, saves_spell is not checked and full damage is dealt. */
static void test_no_saves_flag_deals_full_damage(void)
{
    ROOM_INDEX_DATA room;
    CHAR_DATA npc, pc1;
    CHAR_DATA *people[] = {&npc, &pc1};

    make_npc(&npc);
    make_pc(&pc1);
    build_room(&room, people, 2);

    reset_tracking();
    stub_saves_spell_result = TRUE;  /* would save if checked */
    stub_is_same_group_result = FALSE;
    aoe_damage(&npc, NULL, 1, 30, 100, 100, 0, 0 /* no AOE_SAVES */);

    assert(sp_damage_call_count == 1);
    assert(sp_damage_last_dam == 100);
}

/* Fixed damage (min == max) is passed through without calling number_range. */
static void test_fixed_damage_exact(void)
{
    ROOM_INDEX_DATA room;
    CHAR_DATA npc, pc1;
    CHAR_DATA *people[] = {&npc, &pc1};

    make_npc(&npc);
    make_pc(&pc1);
    build_room(&room, people, 2);

    reset_tracking();
    stub_saves_spell_result = FALSE;
    stub_is_same_group_result = FALSE;
    aoe_damage(&npc, NULL, 1, 0, 77, 77, 0, 0);

    assert(sp_damage_call_count == 1);
    assert(sp_damage_last_dam == 77);
}

/* NULL caster is handled gracefully. */
static void test_null_caster_is_safe(void)
{
    reset_tracking();
    aoe_damage(NULL, NULL, 1, 0, 50, 50, 0, 0);
    assert(sp_damage_call_count == 0);
}

int main(void)
{
    test_caster_not_hit();
    test_npc_hits_all_pcs();
    test_pc_hits_all_npcs();
    test_npc_does_not_hit_npc();
    test_pc_does_not_hit_pc();
    test_skip_group_flag_skips_grouped_targets();
    test_no_skip_group_flag_hits_grouped_targets();
    test_saves_halves_damage();
    test_no_saves_flag_deals_full_damage();
    test_fixed_damage_exact();
    test_null_caster_is_safe();

    puts("test_magic2: all tests passed");
    return 0;
}
