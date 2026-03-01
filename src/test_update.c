#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

bool should_abort_for_checkpoint_timeout(int usage_now, int checkpoint, int threshold, bool disable_abort);

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

int main(void)
{
    test_timeout_disabled_never_aborts();
    test_timeout_requires_strictly_greater_than_threshold();
    test_negative_and_small_deltas_do_not_abort();

    puts("test_update: all tests passed");
    return 0;
}
