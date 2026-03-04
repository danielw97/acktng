#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "config.h"
#define DEC_GLOBALS_H 1
#include "ack.h"

int invasion_boss_spawn_count_for_tick(int boss_ticks_up);
int invasion_spawn_mode_for_respawn_index(int spawns_this_reset);
int invasion_wave_progress_tier_for_respawn_index(int spawns_this_reset);
int invasion_wave_hp_bonus_for_progress_tier(int level, int wave_progress_tier);
int invasion_wave_drhr_bonus_for_progress_tier(int level, int wave_progress_tier);
int invasion_wave_ac_bonus_for_progress_tier(int level, int wave_progress_tier);
int invasion_reward_index_for_kill(bool is_boss, int mob_level);
int invasion_should_advance_for_room_tick_count(int room_tick_count);
int invasion_gertrude_explosions_after_tick(int current_count, int had_explosion_this_tick);
const char *invasion_gertrude_quest_message_for_explosions(int explosion_count);
bool invasion_gertrude_should_fall_for_explosions(int explosion_count);
int invasion_boss_pending_trash_talks_for_respawns(int respawn_count);
bool invasion_should_emit_pending_boss_trash_talk(int pending_talk_count, int room_tick_advanced, int crusade_activity_this_tick);
int invasion_is_hidden_mobile(CHAR_DATA *ch);
const char *invasion_door_command_argument(const char *keyword, sh_int dir,
                                           char *buf, size_t buf_len);
void invasion_force_unlock_exit(CHAR_DATA *ch, EXIT_DATA *pexit);

static void test_boss_spawn_count_scales_with_uptime(void)
{
    assert(invasion_boss_spawn_count_for_tick(-1) == 1);
    assert(invasion_boss_spawn_count_for_tick(0) == 1);
    assert(invasion_boss_spawn_count_for_tick(14) == 1);
    assert(invasion_boss_spawn_count_for_tick(15) == 2);
    assert(invasion_boss_spawn_count_for_tick(29) == 2);
    assert(invasion_boss_spawn_count_for_tick(30) == 3);
    assert(invasion_boss_spawn_count_for_tick(44) == 3);
    assert(invasion_boss_spawn_count_for_tick(45) == 4);
    assert(invasion_boss_spawn_count_for_tick(135) == 10);
    assert(invasion_boss_spawn_count_for_tick(2000) == 10);
}

static void test_spawn_mode_for_respawn_index(void)
{
    assert(invasion_spawn_mode_for_respawn_index(-1) == 0);
    assert(invasion_spawn_mode_for_respawn_index(0) == 0);
    assert(invasion_spawn_mode_for_respawn_index(1) == 1);
    assert(invasion_spawn_mode_for_respawn_index(8) == 1);
    assert(invasion_spawn_mode_for_respawn_index(9) == 2);
    assert(invasion_spawn_mode_for_respawn_index(20) == 2);
}


static void test_wave_progress_tier_for_respawn_index(void)
{
    assert(invasion_wave_progress_tier_for_respawn_index(-1) == 0);
    assert(invasion_wave_progress_tier_for_respawn_index(0) == 0);
    assert(invasion_wave_progress_tier_for_respawn_index(2) == 0);
    assert(invasion_wave_progress_tier_for_respawn_index(3) == 1);
    assert(invasion_wave_progress_tier_for_respawn_index(8) == 1);
    assert(invasion_wave_progress_tier_for_respawn_index(9) == 2);
    assert(invasion_wave_progress_tier_for_respawn_index(17) == 2);
    assert(invasion_wave_progress_tier_for_respawn_index(18) == 3);
    assert(invasion_wave_progress_tier_for_respawn_index(500) == 3);
}

static void test_wave_progress_stat_bonus_helpers(void)
{
    assert(invasion_wave_hp_bonus_for_progress_tier(100, 0) == 0);
    assert(invasion_wave_hp_bonus_for_progress_tier(100, 1) == 200);
    assert(invasion_wave_hp_bonus_for_progress_tier(100, 2) == 400);
    assert(invasion_wave_hp_bonus_for_progress_tier(100, 3) == 600);
    assert(invasion_wave_hp_bonus_for_progress_tier(100, 99) == 600);
    assert(invasion_wave_hp_bonus_for_progress_tier(100, -9) == 0);
    assert(invasion_wave_hp_bonus_for_progress_tier(-5, 3) == 0);

    assert(invasion_wave_drhr_bonus_for_progress_tier(11, 1) == 1);
    assert(invasion_wave_drhr_bonus_for_progress_tier(12, 1) == 1);
    assert(invasion_wave_drhr_bonus_for_progress_tier(36, 2) == 6);
    assert(invasion_wave_drhr_bonus_for_progress_tier(120, 3) == 30);
    assert(invasion_wave_drhr_bonus_for_progress_tier(120, 99) == 30);
    assert(invasion_wave_drhr_bonus_for_progress_tier(120, -1) == 0);

    assert(invasion_wave_ac_bonus_for_progress_tier(6, 1) == 4);
    assert(invasion_wave_ac_bonus_for_progress_tier(60, 2) == 20);
    assert(invasion_wave_ac_bonus_for_progress_tier(120, 3) == 60);
    assert(invasion_wave_ac_bonus_for_progress_tier(120, 99) == 60);
    assert(invasion_wave_ac_bonus_for_progress_tier(120, -1) == 0);
}

static void test_hidden_mobile_matches_invasion_tagging(void)
{
    CHAR_DATA mob = {0};
    CHAR_DATA player = {0};

    mob.act = ACT_IS_NPC | ACT_INVASION;
    assert(invasion_is_hidden_mobile(&mob) == 1);

    mob.act &= ~ACT_INVASION;
    assert(invasion_is_hidden_mobile(&mob) == 0);

    player.act |= ACT_INVASION;
    assert(invasion_is_hidden_mobile(&player) == 0);
}

static void test_door_command_argument_prefers_exit_keyword(void)
{
    char buf[32];

    assert(strcmp(invasion_door_command_argument("vault door", DIR_NORTH,
                                                 buf, sizeof(buf)),
                  "vault") == 0);
    assert(strcmp(invasion_door_command_argument("  iron gate", DIR_EAST,
                                                 buf, sizeof(buf)),
                  "iron") == 0);
}

static void test_door_command_argument_falls_back_to_direction(void)
{
    char buf[32];

    assert(strcmp(invasion_door_command_argument("", DIR_UP,
                                                 buf, sizeof(buf)),
                  "up") == 0);
    assert(strcmp(invasion_door_command_argument(NULL, DIR_WEST,
                                                 buf, sizeof(buf)),
                  "west") == 0);
    assert(strcmp(invasion_door_command_argument(NULL, 99,
                                                 buf, sizeof(buf)),
                  "door") == 0);
}

static void test_force_unlock_exit_allows_invasion_mobs_without_key(void)
{
    ROOM_INDEX_DATA from_room = {0}, to_room = {0};
    EXIT_DATA forward = {0}, reverse = {0};
    CHAR_DATA mob = {0};

    from_room.exit[DIR_NORTH] = &forward;
    to_room.exit[DIR_SOUTH] = &reverse;

    forward.to_room = &to_room;
    forward.exit_info = EX_ISDOOR | EX_CLOSED | EX_LOCKED | EX_PICKPROOF;
    reverse.to_room = &from_room;
    reverse.exit_info = EX_ISDOOR | EX_CLOSED | EX_LOCKED | EX_PICKPROOF;

    mob.in_room = &from_room;
    mob.act = ACT_IS_NPC | ACT_INVASION;

    invasion_force_unlock_exit(&mob, &forward);

    assert(!IS_SET(forward.exit_info, EX_LOCKED));
    assert(!IS_SET(reverse.exit_info, EX_LOCKED));
}

static void test_force_unlock_exit_ignores_non_invasion_mobs(void)
{
    ROOM_INDEX_DATA from_room = {0}, to_room = {0};
    EXIT_DATA forward = {0}, reverse = {0};
    CHAR_DATA mob = {0};

    from_room.exit[DIR_EAST] = &forward;
    to_room.exit[DIR_WEST] = &reverse;

    forward.to_room = &to_room;
    forward.exit_info = EX_ISDOOR | EX_CLOSED | EX_LOCKED;
    reverse.to_room = &from_room;
    reverse.exit_info = EX_ISDOOR | EX_CLOSED | EX_LOCKED;

    mob.in_room = &from_room;
    mob.act = ACT_IS_NPC;

    invasion_force_unlock_exit(&mob, &forward);

    assert(IS_SET(forward.exit_info, EX_LOCKED));
    assert(IS_SET(reverse.exit_info, EX_LOCKED));
}

static void test_room_tick_advance_rate(void)
{
    assert(invasion_should_advance_for_room_tick_count(0) == 0);
    assert(invasion_should_advance_for_room_tick_count(1) == 1);
    assert(invasion_should_advance_for_room_tick_count(2) == 1);
    assert(invasion_should_advance_for_room_tick_count(3) == 1);
    assert(invasion_should_advance_for_room_tick_count(4) == 1);
    assert(invasion_should_advance_for_room_tick_count(5) == 1);
    assert(invasion_should_advance_for_room_tick_count(8) == 1);
}

static void test_invasion_reward_tiers_and_boss_exclusion(void)
{
    assert(invasion_reward_index_for_kill(TRUE, 1) == -1);
    assert(invasion_reward_index_for_kill(TRUE, 250) == -1);

    assert(invasion_reward_index_for_kill(FALSE, 0) == -1);
    assert(invasion_reward_index_for_kill(FALSE, 1) == 0);
    assert(invasion_reward_index_for_kill(FALSE, 100) == 0);
    assert(invasion_reward_index_for_kill(FALSE, 101) == 1);
    assert(invasion_reward_index_for_kill(FALSE, 149) == 1);
    assert(invasion_reward_index_for_kill(FALSE, 150) == 2);
    assert(invasion_reward_index_for_kill(FALSE, 220) == 2);
}


static void test_boss_trash_talk_pending_schedule(void)
{
    assert(invasion_boss_pending_trash_talks_for_respawns(-2) == 0);
    assert(invasion_boss_pending_trash_talks_for_respawns(0) == 0);
    assert(invasion_boss_pending_trash_talks_for_respawns(1) == 0);
    assert(invasion_boss_pending_trash_talks_for_respawns(2) == 0);
    assert(invasion_boss_pending_trash_talks_for_respawns(3) == 1);
    assert(invasion_boss_pending_trash_talks_for_respawns(8) == 2);
    assert(invasion_boss_pending_trash_talks_for_respawns(9) == 3);

    assert(invasion_should_emit_pending_boss_trash_talk(0, 1, 0) == FALSE);
    assert(invasion_should_emit_pending_boss_trash_talk(1, 0, 0) == FALSE);
    assert(invasion_should_emit_pending_boss_trash_talk(1, 1, 1) == FALSE);
    assert(invasion_should_emit_pending_boss_trash_talk(1, 1, 0) == TRUE);
    assert(invasion_should_emit_pending_boss_trash_talk(4, 1, 0) == TRUE);
}

static void test_gertrude_explosion_counter_and_thresholds(void)
{
    assert(invasion_gertrude_explosions_after_tick(0, 0) == 0);
    assert(invasion_gertrude_explosions_after_tick(0, 1) == 1);
    assert(invasion_gertrude_explosions_after_tick(9, 1) == 10);
    assert(invasion_gertrude_explosions_after_tick(19, 1) == 20);
    assert(invasion_gertrude_explosions_after_tick(20, 1) == 20);
    assert(invasion_gertrude_explosions_after_tick(-2, 1) == 1);
    assert(invasion_gertrude_explosions_after_tick(0, 3) == 1);
    assert(invasion_gertrude_explosions_after_tick(18, 3) == 19);
    assert(invasion_gertrude_explosions_after_tick(5, -3) == 6);

    assert(invasion_gertrude_quest_message_for_explosions(9) == NULL);
    assert(invasion_gertrude_quest_message_for_explosions(10) != NULL);
    assert(invasion_gertrude_quest_message_for_explosions(15) != NULL);
    assert(invasion_gertrude_quest_message_for_explosions(19) != NULL);
    assert(invasion_gertrude_quest_message_for_explosions(20) == NULL);

    assert(invasion_gertrude_should_fall_for_explosions(19) == FALSE);
    assert(invasion_gertrude_should_fall_for_explosions(20) == TRUE);
    assert(invasion_gertrude_should_fall_for_explosions(25) == TRUE);
}

int main(void)
{
    test_boss_spawn_count_scales_with_uptime();
    test_spawn_mode_for_respawn_index();
    test_wave_progress_tier_for_respawn_index();
    test_wave_progress_stat_bonus_helpers();
    test_hidden_mobile_matches_invasion_tagging();
    test_door_command_argument_prefers_exit_keyword();
    test_door_command_argument_falls_back_to_direction();
    test_force_unlock_exit_allows_invasion_mobs_without_key();
    test_force_unlock_exit_ignores_non_invasion_mobs();
    test_room_tick_advance_rate();
    test_invasion_reward_tiers_and_boss_exclusion();
    test_boss_trash_talk_pending_schedule();
    test_gertrude_explosion_counter_and_thresholds();

    puts("test_invasion: all tests passed");
    return 0;
}
