#include <assert.h>

/* Skip globals.h from ack.h to avoid incomplete-type extern arrays in unit test TU. */
#define DEC_GLOBALS_H 1
#include "ack.h"

int get_obj_stat_weight_index(const OBJ_DATA *obj);

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

int main(void)
{
    test_get_obj_stat_weight_index_clamps_bounds();
    return 0;
}
