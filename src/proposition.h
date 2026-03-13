/***************************************************************************
 * quest.h
 *
 * Defines for the quest quest system.
 * Quest state is stored in QUEST_DATA entries that live in
 * PC_DATA (ack.h). Each PC can store up to PROP_MAX_QUESTS entries.
 *
 ***************************************************************************/

#ifndef QUEST_H
#define QUEST_H

/* ------------------------------------------------------------------
 * Quest type constants (stored in pcdata->prop_type)
 * ------------------------------------------------------------------ */
#define PROP_TYPE_NONE          0   /* No active quest             */
#define PROP_TYPE_KILL_VARIETY  1   /* Kill 3-5 different mob types      */
#define PROP_TYPE_COLLECT_ITEMS 2   /* Collect 2-4 items of right level  */
#define PROP_TYPE_KILL_COUNT    3   /* Kill 5-15 of one mob type         */

/* Maximum number of distinct targets (largest type uses 5 slots) */
#define PROP_MAX_TARGETS        5

/* Maximum simultaneous quests on a player */
#define PROP_MAX_QUESTS   3
#define PROP_MAX_STATIC_QUESTS 48

/* ------------------------------------------------------------------
 * Function prototypes
 * ------------------------------------------------------------------ */
void do_quest          args((CHAR_DATA *ch, char *argument));
void quest_request     args((CHAR_DATA *ch, CHAR_DATA *postman));
void quest_status      args((CHAR_DATA *ch));
void quest_complete    args((CHAR_DATA *ch, CHAR_DATA *postman));
void quest_kill_notify args((CHAR_DATA *ch, CHAR_DATA *victim));
void quest_obj_notify  args((CHAR_DATA *ch, OBJ_DATA  *obj));
void quest_cancel      args((CHAR_DATA *ch, int slot));
void clear_quest       args((CHAR_DATA *ch));
void quest_load_static_templates args((void));

#endif /* QUEST_H */
