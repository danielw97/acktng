#include <assert.h>
#include <stdio.h>

#include "config.h"
#define DEC_GLOBALS_H 1
#include "ack.h"

void quest_set_crusade_level_range_for_mob_level(int mob_level, int *minimum_level, int *maximum_level);
short int quest_resolve_crusade_personality(short int personality, int mob_level);
short int quest_tier_from_level(int mob_level);
void quest_set_crusade_level_range_for_tier(short int tier, int *minimum_level, int *maximum_level);
void quest_set_effective_crusade_level_range(short int tier, int highest_level_in_range, int *minimum_level, int *maximum_level);
int quest_crusade_level_cap_for_range(int range_minimum, int range_maximum, int highest_level_in_range);
void quest_note_player_crusade_range(int pseudo_level, int *highest_mortal, int *highest_remortal, int *highest_adept);
int quest_is_valid_crusade_mobile(CHAR_DATA *target, int min_level, int max_level);

static void test_range_for_normal_mob_level(void)
{
   int min_level = 0;
   int max_level = 0;

   quest_set_crusade_level_range_for_mob_level(50, &min_level, &max_level);

   assert(min_level == 1);
   assert(max_level == 100);

   quest_set_crusade_level_range_for_mob_level(120, &min_level, &max_level);

   assert(min_level == 101);
   assert(max_level == 149);

   quest_set_crusade_level_range_for_mob_level(160, &min_level, &max_level);

   assert(min_level == 150);
   assert(max_level == 170);
}

static void test_range_clamps_to_valid_bounds(void)
{
   int min_level = 0;
   int max_level = 0;

   quest_set_crusade_level_range_for_mob_level(10, &min_level, &max_level);
   assert(min_level == 1);
   assert(max_level == 100);

   quest_set_crusade_level_range_for_mob_level(1000, &min_level, &max_level);
   assert(min_level == 150);
   assert(max_level == 170);
}

static void test_invalid_personality_resolves_from_mob_level(void)
{
   assert(quest_resolve_crusade_personality(0, 10) == 1);
   assert(quest_resolve_crusade_personality(0, 75) == 1);
   assert(quest_resolve_crusade_personality(0, 100) == 1);
   assert(quest_resolve_crusade_personality(0, 120) == 2);
   assert(quest_resolve_crusade_personality(0, 150) == 3);
}


static void test_crusade_level_cap_for_range(void)
{
   assert(quest_crusade_level_cap_for_range(1, 100, 40) == 60);
   assert(quest_crusade_level_cap_for_range(101, 149, 120) == 140);
   assert(quest_crusade_level_cap_for_range(150, 170, 160) == 170);
   assert(quest_crusade_level_cap_for_range(101, 149, 90) == 110);
}

static void test_note_player_crusade_range(void)
{
   int highest_mortal = 0;
   int highest_remortal = 0;
   int highest_adept = 0;

   quest_note_player_crusade_range(50, &highest_mortal, &highest_remortal, &highest_adept);
   quest_note_player_crusade_range(120, &highest_mortal, &highest_remortal, &highest_adept);
   quest_note_player_crusade_range(155, &highest_mortal, &highest_remortal, &highest_adept);
   quest_note_player_crusade_range(98, &highest_mortal, &highest_remortal, &highest_adept);

   assert(highest_mortal == 98);
   assert(highest_remortal == 120);
   assert(highest_adept == 155);
}


static void test_tier_mapping_helpers(void)
{
   int min_level = 0;
   int max_level = 0;

   assert(quest_tier_from_level(1) == 1);
   assert(quest_tier_from_level(100) == 1);
   assert(quest_tier_from_level(101) == 2);
   assert(quest_tier_from_level(149) == 2);
   assert(quest_tier_from_level(150) == 3);

   quest_set_crusade_level_range_for_tier(1, &min_level, &max_level);
   assert(min_level == 1 && max_level == 100);

   quest_set_crusade_level_range_for_tier(2, &min_level, &max_level);
   assert(min_level == 101 && max_level == 149);

   quest_set_crusade_level_range_for_tier(3, &min_level, &max_level);
   assert(min_level == 150 && max_level == 170);
}

static void test_effective_range_helper(void)
{
   int min_level = 0;
   int max_level = 0;

   quest_set_effective_crusade_level_range(1, 80, &min_level, &max_level);
   assert(min_level == 1 && max_level == 100);

   quest_set_effective_crusade_level_range(2, 120, &min_level, &max_level);
   assert(min_level == 101 && max_level == 140);

   quest_set_effective_crusade_level_range(3, 155, &min_level, &max_level);
   assert(min_level == 150 && max_level == 170);
}

static void test_valid_personality_is_preserved(void)
{
   assert(quest_resolve_crusade_personality(1, 120) == 1);
   assert(quest_resolve_crusade_personality(2, 10) == 2);
   assert(quest_resolve_crusade_personality(3, 75) == 3);
}


static void test_crusade_mobile_validation_excludes_invaders(void)
{
   AREA_DATA area = {0};
   ROOM_INDEX_DATA room = {0};
   CHAR_DATA mob = {0};

   room.area = &area;
   mob.act = ACT_IS_NPC;
   mob.level = 75;
   mob.in_room = &room;

   assert(quest_is_valid_crusade_mobile(&mob, 1, 100) == 1);

   mob.act |= ACT_INVASION;
   assert(quest_is_valid_crusade_mobile(&mob, 1, 100) == 0);
}

static void test_no_blood_flag_does_not_overlap_invasion_act_bit(void)
{
   assert(ACT_NOBLOOD != ACT_INVASION);
   assert(PLR_NOBLOOD == ACT_INVASION);
}

int main(void)
{
   test_range_for_normal_mob_level();
   test_range_clamps_to_valid_bounds();
   test_invalid_personality_resolves_from_mob_level();
   test_valid_personality_is_preserved();
   test_crusade_level_cap_for_range();
   test_note_player_crusade_range();
   test_tier_mapping_helpers();
   test_effective_range_helper();
   test_crusade_mobile_validation_excludes_invaders();
   test_no_blood_flag_does_not_overlap_invasion_act_bit();

   puts("test_quest: all tests passed");
   return 0;
}
