#include <assert.h>

int stat_calculated_value(int val, int statcap_bonus);

static void test_stat_calculated_value_handles_each_step(void)
{
   assert(stat_calculated_value(0, 0) == 0);
   assert(stat_calculated_value(50000, 0) == 50);
   assert(stat_calculated_value(200000, 0) == 100);
   assert(stat_calculated_value(500000, 0) == 150);
}

static void test_stat_calculated_value_scales_with_statcap_bonus(void)
{
   assert(stat_calculated_value(60000, 0) == 53);
   assert(stat_calculated_value(60000, 1) == 60);
   assert(stat_calculated_value(120000, 2) == 86);
}

int main(void)
{
   test_stat_calculated_value_handles_each_step();
   test_stat_calculated_value_scales_with_statcap_bonus();
   return 0;
}
