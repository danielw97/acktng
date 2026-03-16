#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include "globals.h"

bool shortfight_should_emit_before_victim_raw_kill(int shortfight_round_active_now,
                                                   int victim_is_npc, int victim_is_dead_position);
bool should_handle_victim_death(const CHAR_DATA *victim);

bool deathmatch = false;

void damage_build_hit_messages(char *buf1, size_t buf1_size, char *buf2, size_t buf2_size,
                               char *buf3, size_t buf3_size, const char *col, const char *vp,
                               const char *vs, const char *str, const char *attack, char punct,
                               int dam, const char *critical_message);

static void test_damage_message_builder_formats_expected_lines(void)
{
   char buf1[256], buf2[256], buf3[256];

   damage_build_hit_messages(buf1, sizeof(buf1), buf2, sizeof(buf2), buf3, sizeof(buf3), "@@r",
                             "maul", "mauls", " hard", "slash", '!', 42, "crit");

   assert(strstr(buf1, "@@r$n @@rmaul") != NULL);
   assert(strstr(buf2, "You @@rmauls") != NULL);
   assert(strstr(buf3, "you@@r hard") != NULL);
}

static void test_damage_message_builder_handles_apostrophe_and_truncates(void)
{
   char buf1[32], buf2[32], buf3[32];

   damage_build_hit_messages(buf1, sizeof(buf1), buf2, sizeof(buf2), buf3, sizeof(buf3), "", "hits",
                             "hit", "'s", "attack", '.', 7, "");

   assert(strstr(buf3, "your") != NULL);

   damage_build_hit_messages(buf1, 8, buf2, 8, buf3, 8, "verylong", "verylong", "verylong",
                             "verylong", "verylong", '!', 999, "verylong");
   assert(buf1[7] == '\0');
   assert(buf2[7] == '\0');
   assert(buf3[7] == '\0');
}

static void test_shortfight_should_emit_before_victim_raw_kill(void)
{
   assert(shortfight_should_emit_before_victim_raw_kill(true, true, true));
   assert(!shortfight_should_emit_before_victim_raw_kill(false, true, true));
   assert(!shortfight_should_emit_before_victim_raw_kill(true, false, true));
   assert(!shortfight_should_emit_before_victim_raw_kill(true, true, false));
}

static void test_should_handle_victim_death(void)
{
   CHAR_DATA victim = {0};
   PC_DATA pcdata = {0};

   victim.pcdata = &pcdata;

   victim.position = POS_DEAD;
   SET_BIT(victim.act, ACT_IS_NPC);
   assert(should_handle_victim_death(&victim));

   victim.act = 0;
   assert(should_handle_victim_death(&victim));

   victim.act = 0;
   victim.position = POS_FIGHTING;
   assert(!should_handle_victim_death(&victim));

   assert(!should_handle_victim_death(NULL));
}

int main(void)
{
   test_damage_message_builder_formats_expected_lines();
   test_damage_message_builder_handles_apostrophe_and_truncates();
   test_shortfight_should_emit_before_victim_raw_kill();
   test_should_handle_victim_death();
   return 0;
}
