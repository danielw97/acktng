#include <assert.h>
#include <stdio.h>

void *hash_ref_from_vnum(int vnum);
int vnum_from_hash_ref(void *ref);

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

int main(void)
{
    test_round_trip_positive_vnum();
    test_round_trip_zero_vnum();
    test_round_trip_negative_vnum();

    puts("test_save: all tests passed");
    return 0;
}
