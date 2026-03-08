#include <assert.h>
#include <stdio.h>
#include <string.h>

void *hash_ref_from_vnum(int vnum);
int vnum_from_hash_ref(void *ref);
int resolve_persistent_container_room_vnum_for_test(int room_vnum);
int persistent_container_where_vnum_for_save_for_test(int in_room_vnum, int in_obj);

struct room_index_data
{
    int vnum;
};

typedef struct room_index_data ROOM_INDEX_DATA;

#define ROOM_VNUM_LIMBO 32501

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

static void test_keep_chest_corpse_save_uses_container_vnum_when_in_room(void)
{
    assert(persistent_container_where_vnum_for_save_for_test(4567, 0) == 4567);
}

static void test_keep_chest_corpse_save_uses_default_when_nested_in_object(void)
{
    assert(persistent_container_where_vnum_for_save_for_test(4567, 1) == 32555);
}

static void test_keep_chest_corpse_save_uses_default_when_room_missing(void)
{
    assert(persistent_container_where_vnum_for_save_for_test(0, 0) == 32555);
}

/*
 * Regression test for the fread_corpse linked-list corruption bug.
 *
 * The bug: fread_corpse called LINK(obj, first_obj, last_obj, ...) to add a
 * persistent container to the global object list, then on an error path called
 * PUT_FREE(obj, obj_free) WITHOUT calling UNLINK first.  This left last_obj
 * dangling — pointing at the now-free object.
 *
 * When create_object later called GET_FREE it reused that same memory address.
 * The subsequent LINK executed with last_obj == new_obj, setting
 * new_obj->prev = new_obj (a self-referencing pointer).  obj_update() then
 * called abort() with "UNLINK: link->next->prev corrupted!".
 *
 * The fix: call UNLINK(obj, ...) before PUT_FREE(obj, ...) in the error path.
 *
 * This test uses a minimal doubly-linked list to verify both that the buggy
 * sequence produces corruption and that the fixed sequence does not.
 */
struct fread_corpse_test_node
{
    struct fread_corpse_test_node *next;
    struct fread_corpse_test_node *prev;
    int is_free;
};

/* Simulate the LINK/UNLINK/PUT_FREE/GET_FREE operations used by the real code. */
static void test_link(struct fread_corpse_test_node *link,
                      struct fread_corpse_test_node **first,
                      struct fread_corpse_test_node **last)
{
    if (!*first)
        *first = link;
    else
        (*last)->next = link;
    link->next = NULL;
    link->prev = *last;
    *last = link;
}

static void test_unlink(struct fread_corpse_test_node *link,
                        struct fread_corpse_test_node **first,
                        struct fread_corpse_test_node **last)
{
    if (!link->next)
        *last = link->prev;
    else
        link->next->prev = link->prev;
    if (!link->prev)
        *first = link->next;
    else
        link->prev->next = link->next;
    link->next = link->prev = NULL;
}

static void test_put_free(struct fread_corpse_test_node *item,
                          struct fread_corpse_test_node **freelist)
{
    item->next = *freelist;
    item->is_free = 1;
    *freelist = item;
}

static void test_fread_corpse_put_free_without_unlink_corrupts_list(void)
{
    struct fread_corpse_test_node a;
    struct fread_corpse_test_node *first = NULL, *last = NULL, *freelist = NULL;
    struct fread_corpse_test_node *b;

    memset(&a, 0, sizeof(a));

    /* fread_corpse LINKs obj into the global list (save.c:1977) */
    test_link(&a, &first, &last);

    /* Buggy error path: PUT_FREE without UNLINK — last still points at &a */
    test_put_free(&a, &freelist);

    /* GET_FREE reuses &a (memset zeros is_free, next, prev) */
    b = freelist;
    freelist = b->next;
    memset(b, 0, sizeof(*b));

    /* LINK b: with last == &a == b, sets b->prev = b (self-reference) */
    test_link(b, &first, &last);

    /* Confirm the corruption — b->prev points at itself */
    assert(b->prev == b);
}

static void test_fread_corpse_unlink_before_put_free_preserves_list(void)
{
    struct fread_corpse_test_node a;
    struct fread_corpse_test_node *first = NULL, *last = NULL, *freelist = NULL;
    struct fread_corpse_test_node *b;

    memset(&a, 0, sizeof(a));

    /* fread_corpse LINKs obj into the global list (save.c:1977) */
    test_link(&a, &first, &last);

    /* Fixed error path: UNLINK before PUT_FREE (save.c:2044) */
    test_unlink(&a, &first, &last);
    test_put_free(&a, &freelist);

    /* last must not reference the freed object */
    assert(last != &a);

    /* GET_FREE reuses &a (memset zeros is_free, next, prev) */
    b = freelist;
    freelist = b->next;
    memset(b, 0, sizeof(*b));

    /* LINK b: last is NULL, so b->prev = NULL (no self-reference) */
    test_link(b, &first, &last);

    assert(b->prev != b);
    assert(last == b);
}

int main(void)
{
    test_round_trip_positive_vnum();
    test_round_trip_zero_vnum();
    test_round_trip_negative_vnum();
    test_resolve_persistent_room_uses_requested_room_when_present();
    test_keep_chest_corpse_load_falls_back_to_limbo_room();
    test_keep_chest_corpse_load_uses_limbo_vnum_when_requested_room_is_missing();
    test_keep_chest_corpse_save_uses_container_vnum_when_in_room();
    test_keep_chest_corpse_save_uses_default_when_nested_in_object();
    test_keep_chest_corpse_save_uses_default_when_room_missing();
    test_fread_corpse_put_free_without_unlink_corrupts_list();
    test_fread_corpse_unlink_before_put_free_preserves_list();

    puts("test_save: all tests passed");
    return 0;
}
