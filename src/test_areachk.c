#include <assert.h>
#include <stddef.h>
#include <stdint.h>

void *areachk_testable_vnum_to_hash_value(int vnum);
int areachk_testable_hash_value_to_vnum(void *value);

int main(void)
{
    int vnum = 123456;
    void *encoded = areachk_testable_vnum_to_hash_value(vnum);
    assert((intptr_t)encoded == (intptr_t)vnum);
    assert(areachk_testable_hash_value_to_vnum(encoded) == vnum);
    assert(areachk_testable_hash_value_to_vnum(NULL) == 0);
    return 0;
}
