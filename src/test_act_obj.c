#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "globals.h"

int keep_chest_put_denial_reason(const OBJ_DATA *container_obj, const OBJ_DATA *obj);

#define KEEP_CHEST_PUT_ALLOWED 0
#define KEEP_CHEST_PUT_ERR_FULL 1
#define KEEP_CHEST_PUT_ERR_INVALID_ITEM 2

static void init_obj(OBJ_DATA *obj)
{
    memset(obj, 0, sizeof(*obj));
}

static void test_keep_chest_rejects_corpse_pc(void)
{
    OBJ_DATA chest;
    OBJ_DATA corpse;

    init_obj(&chest);
    init_obj(&corpse);
    chest.item_type = ITEM_CONTAINER;
    chest.value[1] = CONT_KEEP_CHEST;
    corpse.item_type = ITEM_CORPSE_PC;

    assert(keep_chest_put_denial_reason(&chest, &corpse) == KEEP_CHEST_PUT_ERR_INVALID_ITEM);
}

static void test_keep_chest_rejects_container_items(void)
{
    OBJ_DATA chest;
    OBJ_DATA inner_container;

    init_obj(&chest);
    init_obj(&inner_container);
    chest.item_type = ITEM_CONTAINER;
    chest.value[1] = CONT_KEEP_CHEST;
    inner_container.item_type = ITEM_CONTAINER;

    assert(keep_chest_put_denial_reason(&chest, &inner_container) == KEEP_CHEST_PUT_ERR_INVALID_ITEM);
}

static void test_keep_chest_rejects_more_than_fifty_items(void)
{
    OBJ_DATA chest;
    OBJ_DATA candidate;
    OBJ_DATA items[50];
    int i;

    init_obj(&chest);
    init_obj(&candidate);
    chest.item_type = ITEM_CONTAINER;
    chest.value[1] = CONT_KEEP_CHEST;
    candidate.item_type = ITEM_WEAPON;

    for (i = 0; i < 50; i++)
    {
        init_obj(&items[i]);
        if (i < 49)
            items[i].next_in_carry_list = &items[i + 1];
    }
    chest.first_in_carry_list = &items[0];

    assert(keep_chest_put_denial_reason(&chest, &candidate) == KEEP_CHEST_PUT_ERR_FULL);
}

int main(void)
{
    test_keep_chest_rejects_corpse_pc();
    test_keep_chest_rejects_container_items();
    test_keep_chest_rejects_more_than_fifty_items();

    puts("test_act_obj: all tests passed");
    return 0;
}
