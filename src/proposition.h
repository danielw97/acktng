/***************************************************************************
 * proposition.h
 *
 * Defines for the proposition quest system.
 * All proposition state lives directly inside PC_DATA (ack.h).
 * No separate PROPOSITION_DATA structure exists.
 *
 * Fields to add to struct pc_data in ack.h:
 *
 *   -- Proposition quest system --
 *   int   prop_type;                        // 0 = none active
 *   bool  prop_completed;                   // ready to hand in
 *   int   prop_num_targets;                 // how many entries in arrays
 *   int   prop_target_vnum[PROP_MAX_TARGETS]; // mob or obj vnum per slot
 *   bool  prop_target_done[PROP_MAX_TARGETS]; // per-slot completion flag
 *   int   prop_kill_needed;                 // type 3: how many to kill
 *   int   prop_kill_count;                  // type 3: how many killed so far
 *
 ***************************************************************************/

#ifndef PROPOSITION_H
#define PROPOSITION_H

/* ------------------------------------------------------------------
 * Quest type constants (stored in pcdata->prop_type)
 * ------------------------------------------------------------------ */
#define PROP_TYPE_NONE          0   /* No active proposition             */
#define PROP_TYPE_KILL_VARIETY  1   /* Kill 3-5 different mob types      */
#define PROP_TYPE_COLLECT_ITEMS 2   /* Collect 2-4 items of right level  */
#define PROP_TYPE_KILL_COUNT    3   /* Kill 5-15 of one mob type         */

/* Maximum number of distinct targets (largest type uses 5 slots) */
#define PROP_MAX_TARGETS        5

/* ------------------------------------------------------------------
 * Function prototypes
 * ------------------------------------------------------------------ */
void do_proposition          args((CHAR_DATA *ch, char *argument));
void proposition_request     args((CHAR_DATA *ch, CHAR_DATA *postman));
void proposition_status      args((CHAR_DATA *ch));
void proposition_complete    args((CHAR_DATA *ch, CHAR_DATA *postman));
void proposition_kill_notify args((CHAR_DATA *ch, CHAR_DATA *victim));
void proposition_obj_notify  args((CHAR_DATA *ch, OBJ_DATA  *obj));
void clear_proposition       args((CHAR_DATA *ch));

#endif /* PROPOSITION_H */
