#include <assert.h>
#include <stdio.h>

void quest_set_crusade_level_range_for_mob_level(int mob_level, int *minimum_level, int *maximum_level);

static void test_range_for_normal_mob_level(void)
{
   int min_level = 0;
   int max_level = 0;

   quest_set_crusade_level_range_for_mob_level(50, &min_level, &max_level);

   assert(min_level == 20);
   assert(max_level == 70);
}

static void test_range_clamps_to_valid_bounds(void)
{
   int min_level = 0;
   int max_level = 0;

   quest_set_crusade_level_range_for_mob_level(10, &min_level, &max_level);
   assert(min_level == 1);
   assert(max_level == 30);

   quest_set_crusade_level_range_for_mob_level(1000, &min_level, &max_level);
   assert(min_level == 105);
   assert(max_level == 105);
}

int main(void)
{
   test_range_for_normal_mob_level();
   test_range_clamps_to_valid_bounds();

   puts("test_quest: all tests passed");
   return 0;
}
