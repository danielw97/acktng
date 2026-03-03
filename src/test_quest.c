#include <assert.h>
#include <stdio.h>

void quest_set_crusade_level_range_for_mob_level(int mob_level, int *minimum_level, int *maximum_level);
short int quest_resolve_crusade_personality(short int personality, int mob_level);

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

static void test_invalid_personality_resolves_from_mob_level(void)
{
   assert(quest_resolve_crusade_personality(0, 10) == 1);
   assert(quest_resolve_crusade_personality(0, 75) == 2);
   assert(quest_resolve_crusade_personality(0, 120) == 3);
}

static void test_valid_personality_is_preserved(void)
{
   assert(quest_resolve_crusade_personality(1, 120) == 1);
   assert(quest_resolve_crusade_personality(2, 10) == 2);
   assert(quest_resolve_crusade_personality(3, 75) == 3);
}

int main(void)
{
   test_range_for_normal_mob_level();
   test_range_clamps_to_valid_bounds();
   test_invalid_personality_resolves_from_mob_level();
   test_valid_personality_is_preserved();

   puts("test_quest: all tests passed");
   return 0;
}
