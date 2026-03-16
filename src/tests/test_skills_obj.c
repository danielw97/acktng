#include <assert.h>

#include "config.h"

int disarm_select_weapon_slot(int left_type, int right_type, int two_handed_type);

static void test_disarm_select_weapon_slot_prefers_left_hand(void)
{
   assert(disarm_select_weapon_slot(ITEM_WEAPON, ITEM_WEAPON, ITEM_WEAPON) == WEAR_HOLD_HAND_L);
   assert(disarm_select_weapon_slot(ITEM_WEAPON, 1, 1) == WEAR_HOLD_HAND_L);
}

static void test_disarm_select_weapon_slot_uses_right_when_left_not_weapon(void)
{
   assert(disarm_select_weapon_slot(1, ITEM_WEAPON, ITEM_WEAPON) == WEAR_HOLD_HAND_R);
}

static void test_disarm_select_weapon_slot_uses_two_handed_last(void)
{
   assert(disarm_select_weapon_slot(1, 1, ITEM_WEAPON) == WEAR_TWO_HANDED);
}

static void test_disarm_select_weapon_slot_returns_none_when_no_weapon(void)
{
   assert(disarm_select_weapon_slot(1, 2, 3) == -1);
}

int main(void)
{
   test_disarm_select_weapon_slot_prefers_left_hand();
   test_disarm_select_weapon_slot_uses_right_when_left_not_weapon();
   test_disarm_select_weapon_slot_uses_two_handed_last();
   test_disarm_select_weapon_slot_returns_none_when_no_weapon();
   return 0;
}
