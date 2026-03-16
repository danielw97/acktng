#include <assert.h>
#include <stdbool.h>

int chi_skill_cost(int base_cost, int cooldown);
bool chi_should_block_mindoverbody(bool has_mindoverbody, bool has_named_mindoverbody);

static void test_chi_skill_cost_uses_base_when_not_on_cooldown(void)
{
   assert(chi_skill_cost(5, 0) == 5);
   assert(chi_skill_cost(7, -1) == 7);
}

static void test_chi_skill_cost_doubles_on_cooldown(void)
{
   assert(chi_skill_cost(5, 1) == 10);
   assert(chi_skill_cost(9, 3) == 18);
}

static void test_chi_should_block_mindoverbody_if_any_affect_exists(void)
{
   assert(chi_should_block_mindoverbody(false, false) == false);
   assert(chi_should_block_mindoverbody(true, false) == true);
   assert(chi_should_block_mindoverbody(false, true) == true);
   assert(chi_should_block_mindoverbody(true, true) == true);
}

int main(void)
{
   test_chi_skill_cost_uses_base_when_not_on_cooldown();
   test_chi_skill_cost_doubles_on_cooldown();
   test_chi_should_block_mindoverbody_if_any_affect_exists();
   return 0;
}
