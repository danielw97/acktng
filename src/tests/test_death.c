#include <assert.h>

#include "globals.h"

bool are_clans_hostile(int victim_clan, int killer_clan, const POL_DATA *politics);
bool should_extract_npc_on_death(const CHAR_DATA *victim);

static void test_are_clans_hostile_rejects_out_of_range_clans(void)
{
   POL_DATA politics = {0};

   politics.diplomacy[1][2] = -500;

   assert(!are_clans_hostile(0, 2, &politics));
   assert(!are_clans_hostile(1, 0, &politics));
   assert(!are_clans_hostile(MAX_CLAN, 2, &politics));
   assert(!are_clans_hostile(1, MAX_CLAN, &politics));
}

static void test_are_clans_hostile_checks_threshold(void)
{
   POL_DATA politics = {0};

   politics.diplomacy[3][4] = -450;
   assert(!are_clans_hostile(3, 4, &politics));

   politics.diplomacy[3][4] = -451;
   assert(are_clans_hostile(3, 4, &politics));
}

static void test_should_extract_npc_on_death_for_npc(void)
{
   CHAR_DATA victim = {0};

   SET_BIT(victim.act, ACT_IS_NPC);
   assert(should_extract_npc_on_death(&victim));
}

static void test_should_extract_npc_on_death_for_player_or_null(void)
{
   CHAR_DATA player = {0};

   assert(!should_extract_npc_on_death(&player));
   assert(!should_extract_npc_on_death(NULL));
}

int main(void)
{
   test_are_clans_hostile_rejects_out_of_range_clans();
   test_are_clans_hostile_checks_threshold();
   test_should_extract_npc_on_death_for_npc();
   test_should_extract_npc_on_death_for_player_or_null();
   return 0;
}
