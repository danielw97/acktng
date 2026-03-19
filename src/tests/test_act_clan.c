#include <assert.h>
#include <stdio.h>
#include <string.h>

#define DEC_GLOBALS_H 1
#include "ack.h"

/* Stubs */
const struct clan_type clan_table[MAX_CLAN] = {
    {"None", "None ", 0, 0, "N/A", " ", {-1, -1, -1, -1, -1, -1}},
    {"The Lantern Accord", "@@yACCRD@@N", 0, 0, "N/A", " ", {-1, -1, -1, -1, -1, -1}},
    {"The Open Ledger", "@@WLEDGR@@N", 0, 0, "N/A", " ", {-1, -1, -1, -1, -1, -1}},
    {"The Jade Compact", "@@aJADEC@@N", 0, 0, "N/A", " ", {-1, -1, -1, -1, -1, -1}},
    {"The Ember Speakers", "@@REMBER@@N", 0, 0, "N/A", " ", {-1, -1, -1, -1, -1, -1}},
    {"The Red Sand Accounts", "@@eRDSND@@N", 0, 0, "N/A", " ", {-1, -1, -1, -1, -1, -1}},
    {"The Charter Wardens", "@@lCHRTR@@N", 0, 0, "N/A", " ", {-1, -1, -1, -1, -1, -1}},
    {"The Jackal Synod", "@@dJCKAL@@N", 0, 0, "N/A", " ", {-1, -1, -1, -1, -1, -1}},
    {"The Root Covenant", "@@gROOTC@@N", 0, 0, "N/A", " ", {-1, -1, -1, -1, -1, -1}},
    {"The Waystone Circle", "@@GWYSTN@@N", 0, 0, "N/A", " ", {-1, -1, -1, -1, -1, -1}},
    {"The Oathbound March", "@@pOATHB@@N", 0, 0, "N/A", " ", {-1, -1, -1, -1, -1, -1}},
};

/* Mirror of claneq_weight_name from act_clan.c for testing */
static const char *claneq_weight_name(int obj_weight)
{
   if (obj_weight == CLANEQ_WEIGHT_CASTER)
      return "caster";
   if (obj_weight == CLANEQ_WEIGHT_TANK)
      return "tank";
   return "melee";
}

/* find_clan_eq: searches carry list for CLAN_EQ + CLAN_COLORS */
OBJ_DATA *find_clan_eq(CHAR_DATA *ch)
{
   OBJ_DATA *obj;
   if (ch == NULL)
      return NULL;
   for (obj = ch->first_carry; obj != NULL; obj = obj->next_in_carry_list)
   {
      if (IS_SET(obj->extra_flags, ITEM_CLAN_EQ) && IS_SET(obj->wear_flags, ITEM_WEAR_CLAN_COLORS))
         return obj;
   }
   return NULL;
}

/* ── Tests ─────────────────────────────────────────────────── */

static void test_constants_are_obj_weights(void)
{
   /* Constants should be actual obj->weight values used by set_obj_stat_auto */
   assert(CLANEQ_WEIGHT_MELEE == 8);
   assert(CLANEQ_WEIGHT_CASTER == 3);
   assert(CLANEQ_WEIGHT_TANK == 13);
   printf("  PASS: CLANEQ_WEIGHT constants are actual obj weight values\n");
}

static void test_claneq_weight_name_melee(void)
{
   assert(strcmp(claneq_weight_name(CLANEQ_WEIGHT_MELEE), "melee") == 0);
   assert(strcmp(claneq_weight_name(8), "melee") == 0);
   printf("  PASS: claneq_weight_name melee\n");
}

static void test_claneq_weight_name_caster(void)
{
   assert(strcmp(claneq_weight_name(CLANEQ_WEIGHT_CASTER), "caster") == 0);
   assert(strcmp(claneq_weight_name(3), "caster") == 0);
   printf("  PASS: claneq_weight_name caster\n");
}

static void test_claneq_weight_name_tank(void)
{
   assert(strcmp(claneq_weight_name(CLANEQ_WEIGHT_TANK), "tank") == 0);
   assert(strcmp(claneq_weight_name(13), "tank") == 0);
   printf("  PASS: claneq_weight_name tank\n");
}

static void test_claneq_weight_name_default(void)
{
   /* Unknown weight falls back to melee */
   assert(strcmp(claneq_weight_name(99), "melee") == 0);
   assert(strcmp(claneq_weight_name(0), "melee") == 0);
   printf("  PASS: claneq_weight_name default = melee\n");
}

static void test_find_clan_eq_null_char(void)
{
   assert(find_clan_eq(NULL) == NULL);
   printf("  PASS: find_clan_eq(NULL) returns NULL\n");
}

static void test_find_clan_eq_no_eq(void)
{
   CHAR_DATA ch = {0};
   ch.first_carry = NULL;
   assert(find_clan_eq(&ch) == NULL);
   printf("  PASS: find_clan_eq with no items returns NULL\n");
}

static void test_find_clan_eq_finds_claneq(void)
{
   CHAR_DATA ch = {0};
   OBJ_DATA regular = {0};
   OBJ_DATA claneq = {0};

   regular.extra_flags = 0;
   regular.wear_flags = ITEM_TAKE | ITEM_WEAR_BODY;
   regular.next_in_carry_list = &claneq;

   claneq.extra_flags = ITEM_CLAN_EQ;
   claneq.wear_flags = ITEM_TAKE | ITEM_WEAR_CLAN_COLORS;
   claneq.weight = CLANEQ_WEIGHT_MELEE;
   claneq.next_in_carry_list = NULL;

   ch.first_carry = &regular;
   OBJ_DATA *found = find_clan_eq(&ch);
   assert(found == &claneq);
   assert(found->weight == CLANEQ_WEIGHT_MELEE);
   printf("  PASS: find_clan_eq finds clan eq in carry list\n");
}

static void test_find_clan_eq_skips_non_clan_colors(void)
{
   CHAR_DATA ch = {0};
   OBJ_DATA obj = {0};

   obj.extra_flags = ITEM_CLAN_EQ;
   obj.wear_flags = ITEM_TAKE | ITEM_WEAR_BODY;
   obj.next_in_carry_list = NULL;

   ch.first_carry = &obj;
   assert(find_clan_eq(&ch) == NULL);
   printf("  PASS: find_clan_eq skips non-clan-colors item\n");
}

static void test_clan_table_abbr_colors(void)
{
   for (int i = 1; i < MAX_CLAN; i++)
   {
      assert(clan_table[i].clan_abbr[0] == '@');
      assert(clan_table[i].clan_abbr[1] == '@');
   }
   printf("  PASS: All clan abbreviations have color codes\n");
}

static void test_weight_stored_on_object(void)
{
   /* The weight value on the object IS the stat focus — no pcdata field needed */
   OBJ_DATA obj = {0};
   obj.weight = CLANEQ_WEIGHT_CASTER;
   assert(obj.weight == 3);
   assert(strcmp(claneq_weight_name(obj.weight), "caster") == 0);

   obj.weight = CLANEQ_WEIGHT_TANK;
   assert(obj.weight == 13);
   assert(strcmp(claneq_weight_name(obj.weight), "tank") == 0);

   obj.weight = CLANEQ_WEIGHT_MELEE;
   assert(obj.weight == 8);
   assert(strcmp(claneq_weight_name(obj.weight), "melee") == 0);
   printf("  PASS: weight stored directly on object controls stat focus\n");
}

int main(void)
{
   printf("test_act_clan: running tests...\n");

   test_constants_are_obj_weights();
   test_claneq_weight_name_melee();
   test_claneq_weight_name_caster();
   test_claneq_weight_name_tank();
   test_claneq_weight_name_default();
   test_find_clan_eq_null_char();
   test_find_clan_eq_no_eq();
   test_find_clan_eq_finds_claneq();
   test_find_clan_eq_skips_non_clan_colors();
   test_clan_table_abbr_colors();
   test_weight_stored_on_object();

   printf("test_act_clan: all tests passed!\n");
   return 0;
}
