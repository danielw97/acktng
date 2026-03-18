/*
 * quest_unit.h -- Internal quest types and symbols exposed for unit tests.
 *
 * quest.c compiles with QUEST_INTERNAL=static in production.  When built
 * with -DUNIT_TEST_QUEST, QUEST_INTERNAL expands to nothing, giving these
 * symbols external linkage so the test binary can reach them directly.
 *
 * Include this header only from test files (test_quest.c).
 */

#ifndef QUEST_UNIT_H
#define QUEST_UNIT_H

#include "quest.h"

typedef struct static_quest_template_data STATIC_PROP_TEMPLATE;
struct static_quest_template_data
{
   int id;
   char *title;
   int prerequisite_static_id;
   int type;
   int num_targets;
   int target_vnum[QUEST_MAX_TARGETS];
   int kill_needed;
   int min_level;
   int max_level;
   int offerer_vnum;
   int reward_gold;
   int reward_qp;
   int reward_exp;
   char *reward_obj_short;
   char *reward_obj_name;
   char *reward_obj_long;
   int reward_obj_wear_flags;
   int reward_obj_extra_flags;
   int reward_obj_weight;
   int reward_obj_item_apply;
   char *accept_message;
   char *completion_message;
};

extern STATIC_PROP_TEMPLATE *static_quest_table;
extern int static_quest_count;
extern const STATIC_PROP_TEMPLATE *find_static_quest_template(int static_id);
extern int canonical_postmaster_vnum(int vnum);
extern int calc_static_quest_exp(int max_level, bool is_boss, bool is_cartography);

#endif /* QUEST_UNIT_H */
