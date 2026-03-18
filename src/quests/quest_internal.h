/***************************************************************************
 * quest_internal.h -- Internal types and declarations shared across the
 * quest module (src/quests/).  Include this only from quest module source
 * files and unit-test helpers.
 ***************************************************************************/

#ifndef QUEST_INTERNAL_H
#define QUEST_INTERNAL_H

#include "globals.h"

/* ------------------------------------------------------------------
 * QUEST_TEMPLATE - template data loaded from quest .prop files.
 * (Previously named STATIC_PROP_TEMPLATE / "static quest".)
 * ------------------------------------------------------------------ */
typedef struct quest_template_data QUEST_TEMPLATE;
struct quest_template_data
{
   int id;
   char *title;
   int prerequisite_template_id;
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

/* ------------------------------------------------------------------
 * template.c — template table and helpers
 * ------------------------------------------------------------------ */
extern QUEST_TEMPLATE *quest_template_table;
extern int quest_template_count;

const QUEST_TEMPLATE *find_quest_template(int template_id);
int calc_quest_exp(int max_level, bool is_boss, bool is_cartography);
bool quest_reward_item_is_valid(const QUEST_TEMPLATE *tpl);
bool quest_template_has_boss_target(const QUEST_TEMPLATE *tpl);
OBJ_DATA *create_quest_reward_object(CHAR_DATA *ch, const QUEST_TEMPLATE *tpl);

/* ------------------------------------------------------------------
 * state.c — slot management and postmaster utilities
 * ------------------------------------------------------------------ */
int canonical_postmaster_vnum(int vnum);
CHAR_DATA *find_visible_npc_by_canonical_vnum(CHAR_DATA *ch, int vnum);
CHAR_DATA *find_npc_by_canonical_vnum(CHAR_DATA *ch, int vnum);
QUEST_DATA *get_quest_slot(CHAR_DATA *ch, int slot);
int find_free_quest_slot(CHAR_DATA *ch);
bool quest_active(CHAR_DATA *ch);
void clear_quest_slot(CHAR_DATA *ch, int slot);
bool quest_prerequisite_met(CHAR_DATA *ch, const QUEST_TEMPLATE *tpl);
bool quest_template_already_active(CHAR_DATA *ch, int template_id);

/* ------------------------------------------------------------------
 * cartography.c — area room index helpers
 * ------------------------------------------------------------------ */
int quest_cartography_area_room_count(const AREA_DATA *area);
int quest_cartography_room_index(const AREA_DATA *area, const ROOM_INDEX_DATA *room);

#endif /* QUEST_INTERNAL_H */
