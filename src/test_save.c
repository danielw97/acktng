#include <assert.h>
#include <stdio.h>

void *hash_ref_from_vnum(int vnum);
int vnum_from_hash_ref(void *ref);
int resolve_persistent_container_room_vnum_for_test(int room_vnum);

struct room_index_data
{
    int vnum;
};

typedef struct room_index_data ROOM_INDEX_DATA;

#define ROOM_VNUM_LIMBO 2

static ROOM_INDEX_DATA requested_room = {1234};
static ROOM_INDEX_DATA limbo_room = {ROOM_VNUM_LIMBO};
static int requested_room_exists = 0;
static int limbo_room_exists = 0;

ROOM_INDEX_DATA *get_room_index(int vnum)
{
    if (requested_room_exists && vnum == requested_room.vnum)
        return &requested_room;

    if (limbo_room_exists && vnum == ROOM_VNUM_LIMBO)
        return &limbo_room;

    return NULL;
}

static void test_round_trip_positive_vnum(void)
{
    int vnum = 12345;
    assert(vnum_from_hash_ref(hash_ref_from_vnum(vnum)) == vnum);
}

static void test_round_trip_zero_vnum(void)
{
    assert(vnum_from_hash_ref(hash_ref_from_vnum(0)) == 0);
}

static void test_round_trip_negative_vnum(void)
{
    int vnum = -42;
    assert(vnum_from_hash_ref(hash_ref_from_vnum(vnum)) == vnum);
}

static void test_resolve_persistent_room_uses_requested_room_when_present(void)
{
    requested_room_exists = 1;
    limbo_room_exists = 1;

    assert(resolve_persistent_container_room_vnum_for_test(requested_room.vnum) == requested_room.vnum);
}

static void test_keep_chest_corpse_load_falls_back_to_limbo_room(void)
{
    requested_room_exists = 0;
    limbo_room_exists = 1;

    assert(resolve_persistent_container_room_vnum_for_test(0) == ROOM_VNUM_LIMBO);
}

static void test_keep_chest_corpse_load_uses_limbo_vnum_when_requested_room_is_missing(void)
{
    requested_room_exists = 0;
    limbo_room_exists = 0;

    assert(resolve_persistent_container_room_vnum_for_test(requested_room.vnum) == ROOM_VNUM_LIMBO);
}

int main(void)
{
    test_round_trip_positive_vnum();
    test_round_trip_zero_vnum();
    test_round_trip_negative_vnum();
    test_resolve_persistent_room_uses_requested_room_when_present();
    test_keep_chest_corpse_load_falls_back_to_limbo_room();
    test_keep_chest_corpse_load_uses_limbo_vnum_when_requested_room_is_missing();

    puts("test_save: all tests passed");
    return 0;
}
