/***************************************************************************
 * proposition.c  --  Personal proposition quest system for ACK! MUD
 *
 * All proposition state is stored directly in PC_DATA (no sub-struct).
 * Fields used (add these to struct pc_data in ack.h):
 *
 *   int   prop_type;                          // PROP_TYPE_* constant
 *   bool  prop_completed;                     // TRUE = ready to claim
 *   int   prop_num_targets;                   // slots in use
 *   int   prop_target_vnum[PROP_MAX_TARGETS]; // mob or obj vnum
 *   bool  prop_target_done[PROP_MAX_TARGETS]; // per-slot completion
 *   int   prop_kill_needed;                   // type 3: goal count
 *   int   prop_kill_count;                    // type 3: current count
 *
 * Quest types:
 *   PROP_TYPE_KILL_VARIETY  (1)
 *       Kill 3-5 different mob vnums, each between player_level and
 *       player_level+15.  Each distinct mob awards one completion tick.
 *
 *   PROP_TYPE_COLLECT_ITEMS (2)
 *       Obtain 2-4 items whose level is within player_level +/- 10.
 *       Credit fires automatically when the item enters inventory.
 *
 *   PROP_TYPE_KILL_COUNT    (3)
 *       Kill 5-15 of a single mob type, between player_level+5 and
 *       player_level+15.
 *
 * Rewards (via get_psuedo_level):
 *   Gold  :  10 at psuedo-level 1, scaling to 1000 at pseudo-level 150
 *   QP    :   1 at psuedo-level 1, scaling to   20 at pseudo-level 150
 *
 * Player commands:
 *   proposition request   -- at a postman, generate a new proposition
 *   proposition status    -- check progress anywhere
 *   proposition complete  -- at any postman, claim reward
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "globals.h"

/* =========================================================================
 * Internal helpers
 * ========================================================================= */

/*
 * proposition_active()
 * Returns TRUE if ch has a proposition (of any type) in progress or complete.
 */
static bool proposition_active(CHAR_DATA *ch)
{
    return (!IS_NPC(ch) && ch->pcdata->prop_type != PROP_TYPE_NONE);
}

/*
 * proposition_gold() / proposition_qp()
 * Linear scaling rewards based on get_pseudo_level().
 *   Gold: 10 (level 1)  ->  1000 (level 150)
 *   QP :  1  (level 1)  ->    20 (level 150)
 */
static int proposition_gold(int psuedo_level)
{
    int lvl = UMAX(1, UMIN(150, psuedo_level));
    return 10 + (990 * (lvl - 1) / 149);
}

static int proposition_qp(int psuedo_level)
{
    int lvl = UMAX(1, UMIN(150, psuedo_level));
    return 1 + (19 * (lvl - 1) / 149);
}

/*
 * find_prop_mob_index()
 * Walk the live char list looking for a suitable NPC index in the given
 * level band.  Uses a random skip so results vary each call.
 */
static MOB_INDEX_DATA *find_prop_mob_index(int min_level, int max_level)
{
    CHAR_DATA      *wch;
    int             skip;
    int             tries = 0;

    if (max_level > 170) max_level = 170;
    skip = number_range(1, 500);

    /* First pass – stricter filter */
    for (wch = first_char; wch != NULL; wch = wch->next)
    {
        if (!IS_NPC(wch))                                    continue;
        if (wch->level < min_level || wch->level > max_level) continue;
        if (IS_SET(wch->act, ACT_SENTINEL))                  continue;
        if (IS_SET(wch->act, ACT_PET))                       continue;
        if (IS_SET(wch->act, ACT_POSTMAN))                   continue;
        if (IS_SET(wch->act, ACT_TRAIN))                     continue;
        if (IS_SET(wch->act, ACT_PRACTICE))                  continue;
        if (IS_SET(wch->act, ACT_HEAL))                      continue;
        if (wch->in_room != NULL &&
            IS_SET(wch->in_room->area->flags, AREA_NOSHOW))  continue;

        if (--skip <= 0 && number_percent() < 15)
            return wch->pIndexData;

        if (++tries > 2000) break;
    }

    /* Second pass – relaxed */
    for (wch = first_char; wch != NULL; wch = wch->next)
    {
        if (!IS_NPC(wch))                                    continue;
        if (wch->level < min_level || wch->level > max_level) continue;
        if (IS_SET(wch->act, ACT_PET))                       continue;
        if (IS_SET(wch->act, ACT_POSTMAN))                   continue;
        if (number_percent() < 5)
            return wch->pIndexData;
    }

    return NULL;
}

/*
 * find_prop_obj_index()
 * Scan obj vnums looking for a suitable item in the level band.
 */
static OBJ_INDEX_DATA *find_prop_obj_index(int min_level, int max_level)
{
    extern int      top_obj_index;
    OBJ_INDEX_DATA *pObj;
    int             vnum;
    int             tries   = 0;
    int             skip;

    if (max_level > 160) max_level = 160;
    if (min_level < 1)   min_level = 1;

    skip = number_range(1, 300);

    for (vnum = 0; tries < top_obj_index * 2; vnum++, tries++)
    {
        if (vnum > 65535) vnum = 0;

        pObj = get_obj_index(vnum);
        if (pObj == NULL)                                          continue;
        if (pObj->level < min_level || pObj->level > max_level)    continue;
        if (pObj->item_type == ITEM_CORPSE_NPC)                    continue;
        if (pObj->item_type == ITEM_CORPSE_PC)                     continue;
        if (pObj->item_type == ITEM_MONEY)                         continue;
        if (pObj->item_type == ITEM_TRASH)                         continue;
        if (IS_SET(pObj->extra_flags, ITEM_INVENTORY))             continue;
        if (pObj->vnum >= OBJ_VNUM_QUEST_MIN &&
            pObj->vnum <= OBJ_VNUM_QUEST_MAX)                      continue;

        if (--skip <= 0 && number_percent() < 20)
            return pObj;
    }

    return NULL;
}

/*
 * check_all_done()
 * Returns TRUE if every target slot in pcdata is flagged done.
 */
static bool check_all_done(CHAR_DATA *ch)
{
    int i;
    for (i = 0; i < ch->pcdata->prop_num_targets; i++)
        if (!ch->pcdata->prop_target_done[i])
            return FALSE;
    return TRUE;
}

/* =========================================================================
 * Public functions
 * ========================================================================= */

/*
 * clear_proposition()
 * Reset all proposition fields back to their "no quest" defaults.
 */
void clear_proposition(CHAR_DATA *ch)
{
    int i;

    if (IS_NPC(ch) || ch->pcdata == NULL)
        return;

    ch->pcdata->prop_type        = PROP_TYPE_NONE;
    ch->pcdata->prop_completed   = FALSE;
    ch->pcdata->prop_num_targets = 0;
    ch->pcdata->prop_kill_needed = 0;
    ch->pcdata->prop_kill_count  = 0;

    for (i = 0; i < PROP_MAX_TARGETS; i++)
    {
        ch->pcdata->prop_target_vnum[i] = 0;
        ch->pcdata->prop_target_done[i] = FALSE;
    }
}

/*
 * proposition_request()
 * Called when a player types 'proposition request' at a postman.
 * Picks a random type, populates the pcdata fields, and tells the player.
 */
void proposition_request(CHAR_DATA *ch, CHAR_DATA *postman)
{
    int  psuedo_lvl;
    int  min_lvl, max_lvl;
    int  type;
    int  i;
    char buf[MAX_STRING_LENGTH];

    if (IS_NPC(ch))
        return;

    /* Already have one? */
    if (proposition_active(ch))
    {
        if (ch->pcdata->prop_completed)
            send_to_char("You already have a completed proposition waiting! Claim it first.\n\r", ch);
        else
            send_to_char("You already have an active proposition. Use '@@Yproposition status@@N' to check it.\n\r", ch);
        return;
    }

    psuedo_lvl = get_psuedo_level(ch);
    min_lvl    = psuedo_lvl;
    max_lvl    = psuedo_lvl + 15;

    /* Choose a random type */
    type = number_range(PROP_TYPE_KILL_VARIETY, PROP_TYPE_KILL_COUNT);

    /* Clear fields before populating */
    clear_proposition(ch);
    ch->pcdata->prop_type = type;

    switch (type)
    {
    /* ----------------------------------------------------------------
     * Type 1: Kill 3-5 different mob types
     * -------------------------------------------------------------- */
    case PROP_TYPE_KILL_VARIETY:
    {
        int needed = number_range(3, 5);
        int found  = 0;
        int loop   = 0;

        while (found < needed && loop < 500)
        {
            MOB_INDEX_DATA *midx;
            bool            dup = FALSE;
            int             k;

            loop++;
            midx = find_prop_mob_index(min_lvl, max_lvl);
            if (midx == NULL) continue;

            /* Reject duplicates */
            for (k = 0; k < found; k++)
                if (ch->pcdata->prop_target_vnum[k] == midx->vnum)
                    { dup = TRUE; break; }
            if (dup) continue;

            ch->pcdata->prop_target_vnum[found] = midx->vnum;
            ch->pcdata->prop_target_done[found] = FALSE;
            found++;
        }

        ch->pcdata->prop_num_targets = found;

        if (found == 0)
        {
            clear_proposition(ch);
            send_to_char("The postman shrugs. 'Sorry, I can't find any work for you right now.'\n\r", ch);
            return;
        }

        send_to_char("\n\r@@GYou have received a new proposition!@@N\n\r\n\r", ch);
        send_to_char("Hunt down each of the following enemies:\n\r", ch);
        for (i = 0; i < ch->pcdata->prop_num_targets; i++)
        {
            MOB_INDEX_DATA *midx = get_mob_index(ch->pcdata->prop_target_vnum[i]);
            if (midx != NULL)
            {
                sprintf(buf, "  @@Y- %s@@N\n\r", midx->short_descr);
                send_to_char(buf, ch);
            }
        }
        break;
    }

    /* ----------------------------------------------------------------
     * Type 2: Collect items at player_level +/- 10
     * -------------------------------------------------------------- */
    case PROP_TYPE_COLLECT_ITEMS:
    {
        int obj_min = UMAX(1, psuedo_lvl - 10);
        int obj_max = psuedo_lvl + 10;
        int needed  = number_range(2, 4);
        int found   = 0;
        int loop    = 0;

        while (found < needed && loop < 500)
        {
            OBJ_INDEX_DATA *oidx;
            bool            dup = FALSE;
            int             k;

            loop++;
            oidx = find_prop_obj_index(obj_min, obj_max);
            if (oidx == NULL) continue;

            for (k = 0; k < found; k++)
                if (ch->pcdata->prop_target_vnum[k] == oidx->vnum)
                    { dup = TRUE; break; }
            if (dup) continue;

            ch->pcdata->prop_target_vnum[found] = oidx->vnum;
            ch->pcdata->prop_target_done[found] = FALSE;
            found++;
        }

        ch->pcdata->prop_num_targets = found;

        if (found == 0)
        {
            clear_proposition(ch);
            send_to_char("The postman shrugs. 'Sorry, I can't find any work for you right now.'\n\r", ch);
            return;
        }

        send_to_char("\n\r@@GYou have received a new proposition!@@N\n\r\n\r", ch);
        send_to_char("Acquire each of the following items (credited when they enter your inventory):\n\r", ch);
        for (i = 0; i < ch->pcdata->prop_num_targets; i++)
        {
            OBJ_INDEX_DATA *oidx = get_obj_index(ch->pcdata->prop_target_vnum[i]);
            if (oidx != NULL)
            {
                sprintf(buf, "  @@C- %s @@N[level %d]\n\r", oidx->short_descr, oidx->level);
                send_to_char(buf, ch);
            }
        }
        break;
    }

    /* ----------------------------------------------------------------
     * Type 3: Kill 5-15 of a single mob type (level+5 to level+15)
     * -------------------------------------------------------------- */
    case PROP_TYPE_KILL_COUNT:
    {
        int             kill_min  = psuedo_lvl + 5;
        int             kill_max  = psuedo_lvl + 15;
        int             kill_need = number_range(5, 15);
        MOB_INDEX_DATA *midx      = NULL;
        int             loop      = 0;

        while (midx == NULL && loop < 300)
        {
            loop++;
            midx = find_prop_mob_index(kill_min, kill_max);
        }

        if (midx == NULL)
        {
            clear_proposition(ch);
            send_to_char("The postman shrugs. 'Sorry, I can't find any work for you right now.'\n\r", ch);
            return;
        }

        ch->pcdata->prop_num_targets        = 1;
        ch->pcdata->prop_target_vnum[0]     = midx->vnum;
        ch->pcdata->prop_target_done[0]     = FALSE;
        ch->pcdata->prop_kill_needed        = kill_need;
        ch->pcdata->prop_kill_count         = 0;

        send_to_char("\n\r@@GYou have received a new proposition!@@N\n\r\n\r", ch);
        sprintf(buf, "Slay @@Y%d@@N of @@R%s@@N.\n\r", kill_need, midx->short_descr);
        send_to_char(buf, ch);
        break;
    }
    } /* end switch */

    /* Common footer */
    {
        int pslvl = get_psuedo_level(ch);
        sprintf(buf,
                "\n\r@@WReward on completion:@@N @@Y%d@@N gold, @@Y%d@@N quest point%s.\n\r",
                proposition_gold(pslvl),
                proposition_qp(pslvl),
                proposition_qp(pslvl) == 1 ? "" : "s");
        send_to_char(buf, ch);
    }
    send_to_char("Return to any postman when done to claim your reward.\n\r", ch);

    act("$N hands you a sealed note outlining your proposition.", ch, NULL, postman, TO_CHAR);
    act("$N hands $n a sealed proposition.",                      ch, NULL, postman, TO_ROOM);

    do_save(ch, "");
}

/*
 * proposition_status()
 * Show the player their current proposition progress. Works anywhere.
 */
void proposition_status(CHAR_DATA *ch)
{
    int  i;
    char buf[MAX_STRING_LENGTH];

    if (IS_NPC(ch))
        return;

    if (!proposition_active(ch))
    {
        send_to_char("You have no active proposition. Visit a postman to request one.\n\r", ch);
        return;
    }

    if (ch->pcdata->prop_completed)
    {
        send_to_char("@@GYour proposition is COMPLETE!@@N Visit any postman to claim your reward.\n\r", ch);
        return;
    }

    send_to_char("@@Y===  Proposition Status  ===@@N\n\r\n\r", ch);

    switch (ch->pcdata->prop_type)
    {
    case PROP_TYPE_KILL_VARIETY:
        send_to_char("@@WTask:@@N Hunt down each of the following enemies:\n\r", ch);
        for (i = 0; i < ch->pcdata->prop_num_targets; i++)
        {
            MOB_INDEX_DATA *midx = get_mob_index(ch->pcdata->prop_target_vnum[i]);
            if (midx != NULL)
            {
                sprintf(buf, "  %s%-30s@@N  %s\n\r",
                        ch->pcdata->prop_target_done[i] ? "@@G" : "@@R",
                        midx->short_descr,
                        ch->pcdata->prop_target_done[i] ? "[DONE]" : "[pending]");
                send_to_char(buf, ch);
            }
        }
        break;

    case PROP_TYPE_COLLECT_ITEMS:
        send_to_char("@@WTask:@@N Acquire each of the following items:\n\r", ch);
        for (i = 0; i < ch->pcdata->prop_num_targets; i++)
        {
            OBJ_INDEX_DATA *oidx = get_obj_index(ch->pcdata->prop_target_vnum[i]);
            if (oidx != NULL)
            {
                sprintf(buf, "  %s%-30s@@N  [lvl %-3d]  %s\n\r",
                        ch->pcdata->prop_target_done[i] ? "@@G" : "@@C",
                        oidx->short_descr,
                        oidx->level,
                        ch->pcdata->prop_target_done[i] ? "[OBTAINED]" : "[needed]");
                send_to_char(buf, ch);
            }
        }
        break;

    case PROP_TYPE_KILL_COUNT:
    {
        MOB_INDEX_DATA *midx = get_mob_index(ch->pcdata->prop_target_vnum[0]);
        sprintf(buf, "@@WTask:@@N Slay %s\n\r",
                midx ? midx->short_descr : "(unknown)");
        send_to_char(buf, ch);
        sprintf(buf, "  Progress: @@Y%d@@N / @@Y%d@@N\n\r",
                ch->pcdata->prop_kill_count,
                ch->pcdata->prop_kill_needed);
        send_to_char(buf, ch);
        break;
    }
    }

    /* Reward preview */
    {
        int pslvl = get_psuedo_level(ch);
        sprintf(buf,
                "\n\r@@WReward on completion:@@N @@Y%d@@N gold, @@Y%d@@N quest point%s.\n\r",
                proposition_gold(pslvl),
                proposition_qp(pslvl),
                proposition_qp(pslvl) == 1 ? "" : "s");
        send_to_char(buf, ch);
    }
}

/*
 * proposition_complete()
 * Called when player types 'proposition complete' at a postman.
 * Awards gold and QP, then clears all proposition fields.
 */
void proposition_complete(CHAR_DATA *ch, CHAR_DATA *postman)
{
    int  gold_reward, qp_reward, psuedo_lvl;
    char buf[MAX_STRING_LENGTH];

    if (IS_NPC(ch))
        return;

    if (!proposition_active(ch))
    {
        send_to_char("You have no proposition to turn in.\n\r", ch);
        return;
    }

    if (!ch->pcdata->prop_completed)
    {
        send_to_char("Your proposition isn't finished yet! Use '@@Yproposition status@@N' to check your progress.\n\r", ch);
        return;
    }

    psuedo_lvl  = get_psuedo_level(ch);
    gold_reward = proposition_gold(psuedo_lvl);
    qp_reward   = proposition_qp(psuedo_lvl);

    act("$N reviews your completed proposition and nods approvingly.", ch, NULL, postman, TO_CHAR);
    act("$N reviews $n's completed proposition and nods approvingly.", ch, NULL, postman, TO_ROOM);

    send_to_char("\n\r@@GProposition complete! You receive:@@N\n\r", ch);
    sprintf(buf, "  @@Y%d@@N gold coins\n\r", gold_reward);
    send_to_char(buf, ch);
    sprintf(buf, "  @@Y%d@@N quest point%s\n\r", qp_reward, qp_reward == 1 ? "" : "s");
    send_to_char(buf, ch);
    send_to_char("  @@Y1@@N proposition point\n\r", ch);

    ch->gold         += gold_reward;
    ch->quest_points += qp_reward;
    ch->pcdata->proposition_points += 1;

    clear_proposition(ch);
    do_save(ch, "");
}

/*
 * proposition_kill_notify()
 * Call this from fight.c/death.c when an NPC is killed by a PC.
 * Handles PROP_TYPE_KILL_VARIETY and PROP_TYPE_KILL_COUNT.
 */
void proposition_kill_notify(CHAR_DATA *ch, CHAR_DATA *victim)
{
    int  i;
    bool progress = FALSE;
    char buf[MAX_STRING_LENGTH];

    if (IS_NPC(ch) || !IS_NPC(victim) || ch->pcdata == NULL)
        return;
    if (!proposition_active(ch) || ch->pcdata->prop_completed)
        return;

    switch (ch->pcdata->prop_type)
    {
    case PROP_TYPE_KILL_VARIETY:
        for (i = 0; i < ch->pcdata->prop_num_targets; i++)
        {
            if (!ch->pcdata->prop_target_done[i] &&
                ch->pcdata->prop_target_vnum[i] == victim->pIndexData->vnum)
            {
                ch->pcdata->prop_target_done[i] = TRUE;
                progress = TRUE;
                sprintf(buf, "@@GProposition progress:@@N %s killed!\n\r",
                        victim->short_descr);
                send_to_char(buf, ch);
                break;
            }
        }
        if (progress && check_all_done(ch))
        {
            ch->pcdata->prop_completed = TRUE;
            send_to_char(
                "\n\r@@G*** Proposition complete! Visit any postman to claim your reward. ***@@N\n\r\n\r",
                ch);
        }
        break;

    case PROP_TYPE_KILL_COUNT:
        if (ch->pcdata->prop_target_vnum[0] == victim->pIndexData->vnum)
        {
            ch->pcdata->prop_kill_count++;
            progress = TRUE;
            sprintf(buf, "@@GProposition progress:@@N %d/%d %s slain.\n\r",
                    ch->pcdata->prop_kill_count,
                    ch->pcdata->prop_kill_needed,
                    victim->short_descr);
            send_to_char(buf, ch);
            if (ch->pcdata->prop_kill_count >= ch->pcdata->prop_kill_needed)
            {
                ch->pcdata->prop_completed = TRUE;
                send_to_char(
                    "\n\r@@G*** Proposition complete! Visit any postman to claim your reward. ***@@N\n\r\n\r",
                    ch);
            }
        }
        break;

    default:
        break;
    }

    if (progress)
        do_save(ch, "");
}

/*
 * proposition_obj_notify()
 * Call this from act_obj.c whenever an item enters a PC's inventory.
 * Handles PROP_TYPE_COLLECT_ITEMS.
 */
void proposition_obj_notify(CHAR_DATA *ch, OBJ_DATA *obj)
{
    int  i;
    char buf[MAX_STRING_LENGTH];

    if (IS_NPC(ch) || ch->pcdata == NULL)
        return;
    if (!proposition_active(ch) || ch->pcdata->prop_completed)
        return;
    if (ch->pcdata->prop_type != PROP_TYPE_COLLECT_ITEMS)
        return;

    for (i = 0; i < ch->pcdata->prop_num_targets; i++)
    {
        if (!ch->pcdata->prop_target_done[i] &&
            ch->pcdata->prop_target_vnum[i] == obj->pIndexData->vnum)
        {
            ch->pcdata->prop_target_done[i] = TRUE;
            sprintf(buf, "@@GProposition progress:@@N %s obtained!\n\r",
                    obj->short_descr);
            send_to_char(buf, ch);

            if (check_all_done(ch))
            {
                ch->pcdata->prop_completed = TRUE;
                send_to_char(
                    "\n\r@@G*** Proposition complete! Visit any postman to claim your reward. ***@@N\n\r\n\r",
                    ch);
            }

            extract_obj(obj);
            do_save(ch, "");
            break;
        }
    }
}

/*
 * do_proposition()
 * Main command handler. Dispatches to request / status / complete.
 *
 * Usage:
 *   proposition request    (at postman)
 *   proposition status     (anywhere)
 *   proposition complete   (at postman)
 */
void do_proposition(CHAR_DATA *ch, char *argument)
{
    char       arg[MAX_INPUT_LENGTH];
    CHAR_DATA *postman = NULL;
    CHAR_DATA *wch;

    if (IS_NPC(ch))
        return;

    one_argument(argument, arg);

    if (arg[0] == '\0')
    {
        send_to_char("Usage:  proposition <request|status|complete>\n\r", ch);
        return;
    }

    /* 'status' works anywhere, no postman needed */
    if (!str_prefix(arg, "status"))
    {
        proposition_status(ch);
        return;
    }

    /* 'request' and 'complete' both require a postman in the room */
    if (!str_prefix(arg, "request") || !str_prefix(arg, "complete"))
    {
        for (wch = ch->in_room->first_person; wch != NULL; wch = wch->next_in_room)
        {
            if (IS_NPC(wch) && IS_SET(wch->act, ACT_POSTMAN) && can_see(ch, wch))
            {
                postman = wch;
                break;
            }
        }

        if (postman == NULL)
        {
            send_to_char("You need to be with a postman to do that.\n\r", ch);
            return;
        }

        if (!str_prefix(arg, "request"))
            proposition_request(ch, postman);
        else
            proposition_complete(ch, postman);

        return;
    }

    send_to_char("Usage:  proposition <request|status|complete>\n\r", ch);
}
