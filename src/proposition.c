/***************************************************************************
 * proposition.c  --  Personal proposition quest system for ACK! MUD
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "globals.h"

typedef struct static_prop_template_data STATIC_PROP_TEMPLATE;
struct static_prop_template_data
{
    int id;
    const char *title;
    int prerequisite_static_id;
    int type;
    int num_targets;
    int target_vnum[PROP_MAX_TARGETS];
    int kill_needed;
    int min_level;
    int offerer_vnum;
    int reward_gold;
    int reward_qp;
    int reward_item_vnum;
    int reward_item_count;
};

static const STATIC_PROP_TEMPLATE static_prop_table[] = {
    {
        .id = 0,
        .title = "Route reconnaissance: Forest of Confusion",
        .prerequisite_static_id = -1,
        .type = PROP_TYPE_KILL_VARIETY,
        .num_targets = 3,
        .target_vnum = {9601, 9602, 9603, 0, 0},
        .kill_needed = 0,
        .min_level = 20,
        .offerer_vnum = 13001,
        .reward_gold = 1200,
        .reward_qp = 2,
        .reward_item_vnum = 0,
        .reward_item_count = 0,
    },
    {
        .id = 1,
        .title = "Conclave residue containment",
        .prerequisite_static_id = 0,
        .type = PROP_TYPE_KILL_COUNT,
        .num_targets = 1,
        .target_vnum = {1011, 0, 0, 0, 0},
        .kill_needed = 8,
        .min_level = 40,
        .offerer_vnum = 13001,
        .reward_gold = 2200,
        .reward_qp = 3,
        .reward_item_vnum = 0,
        .reward_item_count = 0,
    },
    {
        .id = 2,
        .title = "Eastern trade road interdiction",
        .prerequisite_static_id = 1,
        .type = PROP_TYPE_KILL_VARIETY,
        .num_targets = 3,
        .target_vnum = {29904, 29909, 29932, 0, 0},
        .kill_needed = 0,
        .min_level = 55,
        .offerer_vnum = 3015,
        .reward_gold = 3600,
        .reward_qp = 4,
        .reward_item_vnum = 0,
        .reward_item_count = 0,
    },
    {
        .id = 3,
        .title = "Sealed-warrant courier disruption",
        .prerequisite_static_id = 2,
        .type = PROP_TYPE_KILL_VARIETY,
        .num_targets = 3,
        .target_vnum = {8500, 10159, 11485, 0, 0},
        .kill_needed = 0,
        .min_level = 70,
        .offerer_vnum = 3015,
        .reward_gold = 5000,
        .reward_qp = 5,
        .reward_item_vnum = 0,
        .reward_item_count = 0,
    },
    {
        .id = 4,
        .title = "Blightfront quarantine sweep",
        .prerequisite_static_id = 3,
        .type = PROP_TYPE_KILL_VARIETY,
        .num_targets = 3,
        .target_vnum = {10012, 1030, 1050, 0, 0},
        .kill_needed = 0,
        .min_level = 85,
        .offerer_vnum = 3015,
        .reward_gold = 7000,
        .reward_qp = 6,
        .reward_item_vnum = 0,
        .reward_item_count = 0,
    },
};

#define STATIC_PROP_COUNT (sizeof(static_prop_table) / sizeof(static_prop_table[0]))

typedef struct static_prop_lore_data STATIC_PROP_LORE;
struct static_prop_lore_data
{
    int id;
    const char *accept_message;
    const char *completion_message;
};

static const STATIC_PROP_LORE static_prop_lore_table[] = {
    {0,
     "Kiess route clerks need fresh threat marks from the Forest of Confusion approaches, and the courier lantern office wants your field notes attached to each kill report. Verify the catrat packs currently disrupting waypoint runners so dispatch can stop guessing at which trails are still safe. Mark where you engaged them so Wall Command can update the next road watch rotation.",
     "Your report confirms the catrat pattern and gives Kiess dispatchers a safer outbound schedule along Roc Road staging routes. Couriers are already re-timing lantern relays using your notes, and the post has reopened two suspended handoff points. For now, caravan messages are moving without the blind delays that had begun to stack up at the gate offices."},
    {1,
     "Compact archives flagged new fungal migration near former Conclave transfer trails, and quartermasters fear the bloom is following old prisoner routes toward active supply lines. Thin the crawler spread before it reaches Kiess convoy camps and contaminates bandage, ration, and reagent stores. Temple wardens will compare your kill tallies against archive maps to locate the next containment burn line.",
     "Containment teams now have room to burn out the bloom edge, and Kiess quartermasters can move healers and rations without fungal overruns. Archive clerks logged your sweep against sealed transfer maps, confirming the worst pockets were on the same legacy corridor. The city is treating this as proof that Conclave-era routes are still a living hazard, not just a historical stain."},
    {2,
     "Midgaard's post office is prioritizing eastern manifests after repeated raids on caravans carrying civic cargo, legal packets, and temple-certified inventories. Interdict the known raider cells striking desert-linked commerce before the lane is written off as uninsurable. Bring back enough confirmation that convoy masters can justify reopening regular departures under escort instead of emergency-only runs.",
     "The raid pattern has been broken long enough for convoy masters to reopen regular eastern departures under guarded schedules. Posted manifest queues are moving again, and bonded couriers have stopped requesting double hazard surcharges for every desert consignment. Midgaard's ledgerhouse has already issued a revised risk bulletin citing your interdiction work as the turning point."},
    {3,
     "Reckoning clerks suspect old sealed-warrant networks survive through cult and courier intermediaries who still move testimony, names, and transfer records off-book. Disrupt each linked courier faction before they can purge caches or reroute through shrine safehouses. The postmaster wants this handled quickly: once they realize the chain is mapped, they'll burn evidence rather than defend it.",
     "The courier chain is fractured and several hidden routes are now exposed, including relay points thought lost since the Unindexed Years. Midgaard archivists finally have time to cross-reference recovered testimony before handlers can falsify replacement ledgers. For the first time in years, Reckoning clerks believe they can prove custody lines instead of arguing from rumor."},
    {4,
     "Scouts report blight pressure advancing along routes shared by Midgaard and Kiess caravans, with outbreak vectors converging on the same logistical choke points. Cull the identified vectors before the fronts merge into a single corridor-wide quarantine event. If this line breaks, both cities lose clean movement for medicine, grain, and reinforcement traffic.",
     "Blightfront pressure has eased and the quarantine cordon is holding across the shared trade corridor. Route wardens report clean transit windows long enough to rotate supplies and rebuild burned checkpoints. Trade can move again while containment crews reinforce the line, and both post offices have shifted from crisis dispatches back to scheduled service."}};

#define STATIC_PROP_LORE_COUNT (sizeof(static_prop_lore_table) / sizeof(static_prop_lore_table[0]))

static const STATIC_PROP_LORE *find_static_prop_lore(int static_id)
{
    int i;

    for (i = 0; i < (int)STATIC_PROP_LORE_COUNT; i++)
    {
        if (static_prop_lore_table[i].id == static_id)
            return &static_prop_lore_table[i];
    }

    return NULL;
}


static bool static_prop_prerequisite_met(CHAR_DATA *ch, const STATIC_PROP_TEMPLATE *tpl);

static PROPOSITION_DATA *get_prop_slot(CHAR_DATA *ch, int slot)
{
    if (IS_NPC(ch) || ch->pcdata == NULL)
        return NULL;
    if (slot < 0 || slot >= PROP_MAX_PROPOSITIONS)
        return NULL;
    return &ch->pcdata->propositions[slot];
}

static int find_free_prop_slot(CHAR_DATA *ch)
{
    int i;
    for (i = 0; i < PROP_MAX_PROPOSITIONS; i++)
    {
        PROPOSITION_DATA *prop = get_prop_slot(ch, i);
        if (prop != NULL && prop->prop_type == PROP_TYPE_NONE)
            return i;
    }
    return -1;
}

static bool proposition_active(CHAR_DATA *ch)
{
    int i;

    if (IS_NPC(ch) || ch->pcdata == NULL)
        return FALSE;

    for (i = 0; i < PROP_MAX_PROPOSITIONS; i++)
        if (ch->pcdata->propositions[i].prop_type != PROP_TYPE_NONE)
            return TRUE;

    return FALSE;
}

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

static MOB_INDEX_DATA *find_prop_mob_index(int min_level, int max_level)
{
    CHAR_DATA *wch;
    int skip;
    int tries = 0;

    if (max_level > 170)
        max_level = 170;
    skip = number_range(1, 500);

    for (wch = first_char; wch != NULL; wch = wch->next)
    {
        if (!IS_NPC(wch))
            continue;
        if (wch->level < min_level || wch->level > max_level)
            continue;
        if (IS_SET(wch->act, ACT_SENTINEL))
            continue;
        if (IS_SET(wch->act, ACT_PET))
            continue;
        if (IS_SET(wch->act, ACT_POSTMAN))
            continue;
        if (IS_SET(wch->act, ACT_TRAIN))
            continue;
        if (IS_SET(wch->act, ACT_PRACTICE))
            continue;
        if (IS_SET(wch->act, ACT_HEAL))
            continue;
        if (wch->in_room != NULL &&
            IS_SET(wch->in_room->area->flags, AREA_NOSHOW))
            continue;

        if (--skip <= 0 && number_percent() < 15)
            return wch->pIndexData;

        if (++tries > 2000)
            break;
    }

    for (wch = first_char; wch != NULL; wch = wch->next)
    {
        if (!IS_NPC(wch))
            continue;
        if (wch->level < min_level || wch->level > max_level)
            continue;
        if (IS_SET(wch->act, ACT_PET))
            continue;
        if (IS_SET(wch->act, ACT_POSTMAN))
            continue;
        if (number_percent() < 5)
            return wch->pIndexData;
    }

    return NULL;
}

static OBJ_INDEX_DATA *find_prop_obj_index(int min_level, int max_level)
{
    extern int top_obj_index;
    OBJ_INDEX_DATA *pObj;
    int vnum;
    int tries = 0;
    int skip;

    if (max_level > 160)
        max_level = 160;
    if (min_level < 1)
        min_level = 1;

    skip = number_range(1, 300);

    for (vnum = 0; tries < top_obj_index * 2; vnum++, tries++)
    {
        if (vnum > 65535)
            vnum = 0;

        pObj = get_obj_index(vnum);
        if (pObj == NULL)
            continue;
        if (pObj->level < min_level || pObj->level > max_level)
            continue;
        if (pObj->item_type == ITEM_CORPSE_NPC)
            continue;
        if (pObj->item_type == ITEM_CORPSE_PC)
            continue;
        if (pObj->item_type == ITEM_MONEY)
            continue;
        if (pObj->item_type == ITEM_TRASH)
            continue;
        if (IS_SET(pObj->extra_flags, ITEM_INVENTORY))
            continue;
        if (pObj->vnum >= OBJ_VNUM_QUEST_MIN &&
            pObj->vnum <= OBJ_VNUM_QUEST_MAX)
            continue;

        if (--skip <= 0 && number_percent() < 20)
            return pObj;
    }

    return NULL;
}

static bool check_all_done(PROPOSITION_DATA *prop)
{
    int i;
    for (i = 0; i < prop->prop_num_targets; i++)
        if (!prop->prop_target_done[i])
            return FALSE;
    return TRUE;
}

static void clear_proposition_slot(CHAR_DATA *ch, int slot)
{
    int i;
    PROPOSITION_DATA *prop = get_prop_slot(ch, slot);

    if (prop == NULL)
        return;

    prop->prop_type = PROP_TYPE_NONE;
    prop->prop_completed = FALSE;
    prop->prop_num_targets = 0;
    prop->prop_kill_needed = 0;
    prop->prop_kill_count = 0;
    prop->prop_static_id = -1;
    prop->prop_reward_gold = 0;
    prop->prop_reward_qp = 0;
    prop->prop_reward_item_vnum = 0;
    prop->prop_reward_item_count = 0;

    for (i = 0; i < PROP_MAX_TARGETS; i++)
    {
        prop->prop_target_vnum[i] = 0;
        prop->prop_target_done[i] = FALSE;
    }
}

void clear_proposition(CHAR_DATA *ch)
{
    int i;

    if (IS_NPC(ch) || ch->pcdata == NULL)
        return;

    for (i = 0; i < PROP_MAX_PROPOSITIONS; i++)
        clear_proposition_slot(ch, i);
}

static void show_reward_preview(CHAR_DATA *ch, PROPOSITION_DATA *prop)
{
    char buf[MAX_STRING_LENGTH];
    int gold = prop->prop_static_id >= 0 ? prop->prop_reward_gold : proposition_gold(get_psuedo_level(ch));
    int qp = prop->prop_static_id >= 0 ? prop->prop_reward_qp : proposition_qp(get_psuedo_level(ch));

    sprintf(buf, "@@WReward on completion:@@N @@Y%d@@N gold, @@Y%d@@N quest point%s",
            gold, qp, qp == 1 ? "" : "s");
    if (prop->prop_reward_item_vnum > 0 && prop->prop_reward_item_count > 0)
    {
        OBJ_INDEX_DATA *oidx = get_obj_index(prop->prop_reward_item_vnum);
        if (oidx != NULL)
            sprintf(buf + strlen(buf), ", @@Y%d@@N x %s",
                    prop->prop_reward_item_count,
                    oidx->short_descr);
    }
    strcat(buf, ".\n\r");
    send_to_char(buf, ch);
}

static void proposition_list_static(CHAR_DATA *ch, CHAR_DATA *postman)
{
    int i;
    int ps_lvl;
    char buf[MAX_STRING_LENGTH];

    ps_lvl = get_psuedo_level(ch);
    send_to_char("@@Y=== Available Static Propositions ===@@N\n\r", ch);

    for (i = 0; i < (int)STATIC_PROP_COUNT; i++)
    {
        const STATIC_PROP_TEMPLATE *tpl = &static_prop_table[i];
        if (tpl->id >= 0 && tpl->id < PROP_MAX_STATIC_PROPOSITIONS &&
            ch->pcdata->completed_static_props[tpl->id])
            continue;
        if (postman != NULL && postman->pIndexData != NULL &&
            tpl->offerer_vnum != postman->pIndexData->vnum)
            continue;
        if (!static_prop_prerequisite_met(ch, tpl))
            continue;

        if (ps_lvl < tpl->min_level)
            sprintf(buf, "@@W%2d)@@N %s @@r[requires pseudo-level %d]@@N\n\r", i + 1, tpl->title, tpl->min_level);
        else
            sprintf(buf, "@@W%2d)@@N %s @@g[min pseudo-level %d]@@N\n\r", i + 1, tpl->title, tpl->min_level);
        send_to_char(buf, ch);
    }
}

static bool static_prop_already_active(CHAR_DATA *ch, int static_id)
{
    int i;

    for (i = 0; i < PROP_MAX_PROPOSITIONS; i++)
    {
        PROPOSITION_DATA *prop = &ch->pcdata->propositions[i];
        if (prop->prop_type != PROP_TYPE_NONE && prop->prop_static_id == static_id)
            return TRUE;
    }

    return FALSE;
}

static bool static_prop_prerequisite_met(CHAR_DATA *ch, const STATIC_PROP_TEMPLATE *tpl)
{
    if (tpl->prerequisite_static_id < 0)
        return TRUE;

    if (tpl->prerequisite_static_id >= PROP_MAX_STATIC_PROPOSITIONS)
        return FALSE;

    return ch->pcdata->completed_static_props[tpl->prerequisite_static_id];
}

static void proposition_accept_static(CHAR_DATA *ch, CHAR_DATA *postman, int list_number)
{
    int slot, i;
    const STATIC_PROP_TEMPLATE *tpl;
    const STATIC_PROP_LORE *lore;
    PROPOSITION_DATA *prop;
    char buf[MAX_STRING_LENGTH];

    if (list_number < 1 || list_number > (int)STATIC_PROP_COUNT)
    {
        send_to_char("That proposition number is not valid.\n\r", ch);
        return;
    }

    tpl = &static_prop_table[list_number - 1];

    if (postman == NULL || postman->pIndexData == NULL ||
        tpl->offerer_vnum != postman->pIndexData->vnum)
    {
        send_to_char("That postmaster does not offer that static proposition.\n\r", ch);
        return;
    }

    if (tpl->id >= 0 && tpl->id < PROP_MAX_STATIC_PROPOSITIONS &&
        ch->pcdata->completed_static_props[tpl->id])
    {
        send_to_char("You have already completed that static proposition.\n\r", ch);
        return;
    }

    if (!static_prop_prerequisite_met(ch, tpl))
    {
        send_to_char("You have not unlocked that static proposition yet.\n\r", ch);
        return;
    }

    if (static_prop_already_active(ch, tpl->id))
    {
        send_to_char("You already have that static proposition active.\n\r", ch);
        return;
    }

    if (get_psuedo_level(ch) < tpl->min_level)
    {
        sprintf(buf, "You must be at least pseudo-level %d for that static proposition.\n\r", tpl->min_level);
        send_to_char(buf, ch);
        return;
    }

    slot = find_free_prop_slot(ch);
    if (slot < 0)
    {
        send_to_char("You cannot carry any more propositions right now.\n\r", ch);
        return;
    }

    clear_proposition_slot(ch, slot);
    prop = &ch->pcdata->propositions[slot];
    prop->prop_type = tpl->type;
    prop->prop_num_targets = tpl->num_targets;
    prop->prop_kill_needed = tpl->kill_needed;
    prop->prop_static_id = tpl->id;
    prop->prop_reward_gold = tpl->reward_gold;
    prop->prop_reward_qp = tpl->reward_qp;
    prop->prop_reward_item_vnum = tpl->reward_item_vnum;
    prop->prop_reward_item_count = tpl->reward_item_count;

    for (i = 0; i < tpl->num_targets; i++)
    {
        prop->prop_target_vnum[i] = tpl->target_vnum[i];
        prop->prop_target_done[i] = FALSE;
    }

    sprintf(buf, "@@GYou accepted static proposition [%d] in slot %d:@@N %s\n\r",
            list_number, slot + 1, tpl->title);
    send_to_char(buf, ch);
    lore = find_static_prop_lore(tpl->id);
    if (lore != NULL && lore->accept_message != NULL && lore->accept_message[0] != '\0')
    {
        sprintf(buf, "@@WQuest briefing:@@N %s\n\r", lore->accept_message);
        send_to_char(buf, ch);
    }
    show_reward_preview(ch, prop);
    do_save(ch, "");
}

void proposition_request(CHAR_DATA *ch, CHAR_DATA *postman)
{
    int psuedo_lvl;
    int min_lvl, max_lvl;
    int type;
    int i;
    int slot;
    PROPOSITION_DATA *prop;
    char buf[MAX_STRING_LENGTH];

    if (IS_NPC(ch))
        return;

    if (ch->pcdata->prop_dynamic_cooldown_until > (int)current_time)
    {
        int wait_seconds = ch->pcdata->prop_dynamic_cooldown_until - (int)current_time;
        char wait_buf[MAX_STRING_LENGTH];
        sprintf(wait_buf, "You must wait %d more second%s before requesting a new dynamic proposition.\n\r",
                wait_seconds,
                wait_seconds == 1 ? "" : "s");
        send_to_char(wait_buf, ch);
        return;
    }

    slot = find_free_prop_slot(ch);
    if (slot < 0)
    {
        send_to_char("You already have the maximum number of active propositions.\n\r", ch);
        return;
    }

    psuedo_lvl = get_psuedo_level(ch);
    min_lvl = psuedo_lvl;
    max_lvl = psuedo_lvl + 15;

    type = number_range(PROP_TYPE_KILL_VARIETY, PROP_TYPE_KILL_COUNT);

    clear_proposition_slot(ch, slot);
    prop = &ch->pcdata->propositions[slot];
    prop->prop_type = type;

    switch (type)
    {
    case PROP_TYPE_KILL_VARIETY:
    {
        int needed = number_range(3, 5);
        int found = 0;
        int loop = 0;

        while (found < needed && loop < 500)
        {
            MOB_INDEX_DATA *midx;
            bool dup = FALSE;
            int k;

            loop++;
            midx = find_prop_mob_index(min_lvl, max_lvl);
            if (midx == NULL)
                continue;

            for (k = 0; k < found; k++)
                if (prop->prop_target_vnum[k] == midx->vnum)
                {
                    dup = TRUE;
                    break;
                }
            if (dup)
                continue;

            prop->prop_target_vnum[found] = midx->vnum;
            prop->prop_target_done[found] = FALSE;
            found++;
        }

        prop->prop_num_targets = found;

        if (found == 0)
        {
            clear_proposition_slot(ch, slot);
            send_to_char("The postman shrugs. 'Sorry, I can't find any work for you right now.'\n\r", ch);
            return;
        }

        send_to_char("\n\r@@GYou have received a new proposition!@@N\n\r\n\r", ch);
        sprintf(buf, "Assigned to slot @@Y%d@@N.\n\r", slot + 1);
        send_to_char(buf, ch);
        send_to_char("Hunt down each of the following enemies:\n\r", ch);
        for (i = 0; i < prop->prop_num_targets; i++)
        {
            MOB_INDEX_DATA *midx = get_mob_index(prop->prop_target_vnum[i]);
            if (midx != NULL)
            {
                sprintf(buf, "  @@Y- %s@@N\n\r", midx->short_descr);
                send_to_char(buf, ch);
            }
        }
        break;
    }

    case PROP_TYPE_COLLECT_ITEMS:
    {
        int obj_min = UMAX(1, psuedo_lvl - 10);
        int obj_max = psuedo_lvl + 10;
        int needed = number_range(2, 4);
        int found = 0;
        int loop = 0;

        while (found < needed && loop < 500)
        {
            OBJ_INDEX_DATA *oidx;
            bool dup = FALSE;
            int k;

            loop++;
            oidx = find_prop_obj_index(obj_min, obj_max);
            if (oidx == NULL)
                continue;

            for (k = 0; k < found; k++)
                if (prop->prop_target_vnum[k] == oidx->vnum)
                {
                    dup = TRUE;
                    break;
                }
            if (dup)
                continue;

            prop->prop_target_vnum[found] = oidx->vnum;
            prop->prop_target_done[found] = FALSE;
            found++;
        }

        prop->prop_num_targets = found;

        if (found == 0)
        {
            clear_proposition_slot(ch, slot);
            send_to_char("The postman shrugs. 'Sorry, I can't find any work for you right now.'\n\r", ch);
            return;
        }

        send_to_char("\n\r@@GYou have received a new proposition!@@N\n\r\n\r", ch);
        sprintf(buf, "Assigned to slot @@Y%d@@N.\n\r", slot + 1);
        send_to_char(buf, ch);
        send_to_char("Acquire each of the following items (credited when they enter your inventory):\n\r", ch);
        for (i = 0; i < prop->prop_num_targets; i++)
        {
            OBJ_INDEX_DATA *oidx = get_obj_index(prop->prop_target_vnum[i]);
            if (oidx != NULL)
            {
                sprintf(buf, "  @@C- %s @@N[level %d]\n\r", oidx->short_descr, oidx->level);
                send_to_char(buf, ch);
            }
        }
        break;
    }

    case PROP_TYPE_KILL_COUNT:
    {
        MOB_INDEX_DATA *midx;
        int kill_need = number_range(5, 15);

        midx = find_prop_mob_index(psuedo_lvl + 5, psuedo_lvl + 15);
        if (midx == NULL)
        {
            clear_proposition_slot(ch, slot);
            send_to_char("The postman shrugs. 'Sorry, I can't find any work for you right now.'\n\r", ch);
            return;
        }

        prop->prop_num_targets = 1;
        prop->prop_target_vnum[0] = midx->vnum;
        prop->prop_target_done[0] = FALSE;
        prop->prop_kill_needed = kill_need;
        prop->prop_kill_count = 0;

        send_to_char("\n\r@@GYou have received a new proposition!@@N\n\r\n\r", ch);
        sprintf(buf, "Assigned to slot @@Y%d@@N.\n\r", slot + 1);
        send_to_char(buf, ch);
        sprintf(buf, "Slay @@Y%d@@N of @@R%s@@N.\n\r", kill_need, midx->short_descr);
        send_to_char(buf, ch);
        break;
    }
    }

    show_reward_preview(ch, prop);
    send_to_char("Return to any postman when done to claim your reward.\n\r", ch);

    act("$N hands you a sealed note outlining your proposition.", ch, NULL, postman, TO_CHAR);
    act("$N hands $n a sealed proposition.", ch, NULL, postman, TO_ROOM);

    do_save(ch, "");
}

void proposition_status(CHAR_DATA *ch)
{
    int slot, i;
    char buf[MAX_STRING_LENGTH];

    if (IS_NPC(ch))
        return;

    if (!proposition_active(ch))
    {
        send_to_char("You have no active proposition. Visit a postman to request one.\n\r", ch);
        return;
    }

    send_to_char("@@Y===  Proposition Status  ===@@N\n\r\n\r", ch);

    for (slot = 0; slot < PROP_MAX_PROPOSITIONS; slot++)
    {
        PROPOSITION_DATA *prop = &ch->pcdata->propositions[slot];
        if (prop->prop_type == PROP_TYPE_NONE)
            continue;

        sprintf(buf, "@@W[Slot %d]@@N %s\n\r", slot + 1,
                prop->prop_static_id >= 0 ? "(static)" : "(dynamic)");
        send_to_char(buf, ch);

        if (prop->prop_completed)
        {
            send_to_char("@@GStatus: COMPLETE@@N - turn in at a postman.\n\r\n\r", ch);
            continue;
        }

        switch (prop->prop_type)
        {
        case PROP_TYPE_KILL_VARIETY:
            send_to_char("@@WTask:@@N Hunt down each of the following enemies:\n\r", ch);
            for (i = 0; i < prop->prop_num_targets; i++)
            {
                MOB_INDEX_DATA *midx = get_mob_index(prop->prop_target_vnum[i]);
                if (midx != NULL)
                {
                    sprintf(buf, "  %s%-30s@@N  %s\n\r",
                            prop->prop_target_done[i] ? "@@G" : "@@R",
                            midx->short_descr,
                            prop->prop_target_done[i] ? "[DONE]" : "[pending]");
                    send_to_char(buf, ch);
                }
            }
            break;

        case PROP_TYPE_COLLECT_ITEMS:
            send_to_char("@@WTask:@@N Acquire each of the following items:\n\r", ch);
            for (i = 0; i < prop->prop_num_targets; i++)
            {
                OBJ_INDEX_DATA *oidx = get_obj_index(prop->prop_target_vnum[i]);
                if (oidx != NULL)
                {
                    sprintf(buf, "  %s%-30s@@N  [lvl %-3d]  %s\n\r",
                            prop->prop_target_done[i] ? "@@G" : "@@C",
                            oidx->short_descr,
                            oidx->level,
                            prop->prop_target_done[i] ? "[OBTAINED]" : "[needed]");
                    send_to_char(buf, ch);
                }
            }
            break;

        case PROP_TYPE_KILL_COUNT:
        {
            MOB_INDEX_DATA *midx = get_mob_index(prop->prop_target_vnum[0]);
            sprintf(buf, "@@WTask:@@N Slay %s\n\r", midx ? midx->short_descr : "(unknown)");
            send_to_char(buf, ch);
            sprintf(buf, "  Progress: @@Y%d@@N / @@Y%d@@N\n\r",
                    prop->prop_kill_count,
                    prop->prop_kill_needed);
            send_to_char(buf, ch);
            break;
        }
        }

        show_reward_preview(ch, prop);
        send_to_char("\n\r", ch);
    }
}

void proposition_complete(CHAR_DATA *ch, CHAR_DATA *postman)
{
    int slot;
    char buf[MAX_STRING_LENGTH];

    if (IS_NPC(ch))
        return;

    for (slot = 0; slot < PROP_MAX_PROPOSITIONS; slot++)
    {
        PROPOSITION_DATA *prop = &ch->pcdata->propositions[slot];
        const STATIC_PROP_LORE *lore;
        int gold_reward;
        int qp_reward;
        int i;

        if (prop->prop_type == PROP_TYPE_NONE || !prop->prop_completed)
            continue;

        gold_reward = prop->prop_static_id >= 0 ? prop->prop_reward_gold : proposition_gold(get_psuedo_level(ch));
        qp_reward = prop->prop_static_id >= 0 ? prop->prop_reward_qp : proposition_qp(get_psuedo_level(ch));

        act("$N reviews your completed proposition and nods approvingly.", ch, NULL, postman, TO_CHAR);
        act("$N reviews $n's completed proposition and nods approvingly.", ch, NULL, postman, TO_ROOM);

        send_to_char("\n\r@@GProposition complete! You receive:@@N\n\r", ch);
        sprintf(buf, "  @@Y%d@@N gold coins\n\r", gold_reward);
        send_to_char(buf, ch);
        sprintf(buf, "  @@Y%d@@N quest point%s\n\r", qp_reward, qp_reward == 1 ? "" : "s");
        send_to_char(buf, ch);
        send_to_char("  @@Y1@@N proposition point\n\r", ch);

        if (prop->prop_static_id >= 0)
        {
            lore = find_static_prop_lore(prop->prop_static_id);
            if (lore != NULL && lore->completion_message != NULL && lore->completion_message[0] != '\0')
            {
                sprintf(buf, "@@WCompletion report:@@N %s\n\r", lore->completion_message);
                send_to_char(buf, ch);
            }
        }

        ch->gold += gold_reward;
        ch->quest_points += qp_reward;
        ch->pcdata->proposition_points += 1;

        if (prop->prop_reward_item_vnum > 0 && prop->prop_reward_item_count > 0)
        {
            OBJ_INDEX_DATA *oidx = get_obj_index(prop->prop_reward_item_vnum);
            if (oidx != NULL)
            {
                for (i = 0; i < prop->prop_reward_item_count; i++)
                {
                    OBJ_DATA *reward = create_object(oidx, oidx->level);
                    if (reward != NULL)
                        obj_to_char(reward, ch);
                }
                sprintf(buf, "  @@Y%d@@N x %s\n\r", prop->prop_reward_item_count, oidx->short_descr);
                send_to_char(buf, ch);
            }
        }

        if (prop->prop_static_id >= 0 && prop->prop_static_id < PROP_MAX_STATIC_PROPOSITIONS)
            ch->pcdata->completed_static_props[prop->prop_static_id] = TRUE;

        clear_proposition_slot(ch, slot);
        do_save(ch, "");
        return;
    }

    if (!proposition_active(ch))
        send_to_char("You have no proposition to turn in.\n\r", ch);
    else
        send_to_char("You have no completed proposition to turn in yet.\n\r", ch);
}

void proposition_kill_notify(CHAR_DATA *ch, CHAR_DATA *victim)
{
    int slot;

    if (IS_NPC(ch) || !IS_NPC(victim) || ch->pcdata == NULL)
        return;

    for (slot = 0; slot < PROP_MAX_PROPOSITIONS; slot++)
    {
        PROPOSITION_DATA *prop = &ch->pcdata->propositions[slot];
        int i;
        bool progress = FALSE;
        char buf[MAX_STRING_LENGTH];

        if (prop->prop_type == PROP_TYPE_NONE || prop->prop_completed)
            continue;

        switch (prop->prop_type)
        {
        case PROP_TYPE_KILL_VARIETY:
            for (i = 0; i < prop->prop_num_targets; i++)
            {
                if (!prop->prop_target_done[i] &&
                    prop->prop_target_vnum[i] == victim->pIndexData->vnum)
                {
                    prop->prop_target_done[i] = TRUE;
                    progress = TRUE;
                    sprintf(buf, "@@GProposition slot %d progress:@@N %s killed!\n\r",
                            slot + 1, victim->short_descr);
                    send_to_char(buf, ch);
                    break;
                }
            }
            if (progress && check_all_done(prop))
            {
                prop->prop_completed = TRUE;
                send_to_char("\n\r@@G*** Proposition complete! Visit any postman to claim your reward. ***@@N\n\r\n\r", ch);
            }
            break;

        case PROP_TYPE_KILL_COUNT:
            if (prop->prop_target_vnum[0] == victim->pIndexData->vnum)
            {
                prop->prop_kill_count++;
                progress = TRUE;
                sprintf(buf, "@@GProposition slot %d progress:@@N %d/%d %s slain.\n\r",
                        slot + 1,
                        prop->prop_kill_count,
                        prop->prop_kill_needed,
                        victim->short_descr);
                send_to_char(buf, ch);
                if (prop->prop_kill_count >= prop->prop_kill_needed)
                {
                    prop->prop_completed = TRUE;
                    send_to_char("\n\r@@G*** Proposition complete! Visit any postman to claim your reward. ***@@N\n\r\n\r", ch);
                }
            }
            break;
        }

        if (progress)
            do_save(ch, "");
    }
}

void proposition_obj_notify(CHAR_DATA *ch, OBJ_DATA *obj)
{
    int slot;

    if (IS_NPC(ch) || ch->pcdata == NULL)
        return;

    for (slot = 0; slot < PROP_MAX_PROPOSITIONS; slot++)
    {
        PROPOSITION_DATA *prop = &ch->pcdata->propositions[slot];
        int i;
        char buf[MAX_STRING_LENGTH];

        if (prop->prop_type != PROP_TYPE_COLLECT_ITEMS || prop->prop_completed)
            continue;

        for (i = 0; i < prop->prop_num_targets; i++)
        {
            if (!prop->prop_target_done[i] &&
                prop->prop_target_vnum[i] == obj->pIndexData->vnum)
            {
                prop->prop_target_done[i] = TRUE;
                sprintf(buf, "@@GProposition slot %d progress:@@N %s obtained!\n\r",
                        slot + 1, obj->short_descr);
                send_to_char(buf, ch);

                if (check_all_done(prop))
                {
                    prop->prop_completed = TRUE;
                    send_to_char("\n\r@@G*** Proposition complete! Visit any postman to claim your reward. ***@@N\n\r\n\r", ch);
                }

                extract_obj(obj);
                do_save(ch, "");
                return;
            }
        }
    }
}


void proposition_cancel(CHAR_DATA *ch, int slot)
{
    PROPOSITION_DATA *prop;
    char buf[MAX_STRING_LENGTH];

    if (IS_NPC(ch) || ch->pcdata == NULL)
        return;

    if (slot < 0 || slot >= PROP_MAX_PROPOSITIONS)
    {
        send_to_char("Usage: proposition cancel <slot# 1-3>\n\r", ch);
        return;
    }

    prop = &ch->pcdata->propositions[slot];
    if (prop->prop_type == PROP_TYPE_NONE)
    {
        send_to_char("That proposition slot is already empty.\n\r", ch);
        return;
    }

    if (prop->prop_static_id < 0)
        ch->pcdata->prop_dynamic_cooldown_until = (int)current_time + 15 * 60;

    clear_proposition_slot(ch, slot);

    sprintf(buf, "You cancel proposition slot @@Y%d@@N.\n\r", slot + 1);
    send_to_char(buf, ch);

    if (ch->pcdata->prop_dynamic_cooldown_until > (int)current_time)
        send_to_char("Cancelling a dynamic proposition starts a 15 minute request cooldown.\n\r", ch);

    do_save(ch, "");
}

void do_proposition(CHAR_DATA *ch, char *argument)
{
    char arg[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    CHAR_DATA *postman = NULL;
    CHAR_DATA *wch;

    if (IS_NPC(ch))
        return;

    argument = one_argument(argument, arg);
    one_argument(argument, arg2);

    if (arg[0] == '\0')
    {
        send_to_char("Usage: proposition <request|status|complete|list|accept #|cancel #>\n\r", ch);
        return;
    }

    if (!str_prefix(arg, "status"))
    {
        proposition_status(ch);
        return;
    }

    if (!str_prefix(arg, "cancel"))
    {
        if (!is_number(arg2))
        {
            send_to_char("Usage: proposition cancel <slot# 1-3>\n\r", ch);
            return;
        }
        proposition_cancel(ch, atoi(arg2) - 1);
        return;
    }

    if (!str_prefix(arg, "request") || !str_prefix(arg, "complete") ||
        !str_prefix(arg, "list") || !str_prefix(arg, "accept"))
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
        else if (!str_prefix(arg, "complete"))
            proposition_complete(ch, postman);
        else if (!str_prefix(arg, "list"))
            proposition_list_static(ch, postman);
        else
        {
            if (!is_number(arg2))
            {
                send_to_char("Usage: proposition accept <number>\n\r", ch);
                return;
            }
            proposition_accept_static(ch, postman, atoi(arg2));
        }

        return;
    }

    send_to_char("Usage: proposition <request|status|complete|list|accept #|cancel #>\n\r", ch);
}
