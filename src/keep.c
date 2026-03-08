#include "globals.h"

ROOM_INDEX_DATA *new_room(AREA_DATA *pArea, sh_int vnum, sh_int sector);

extern int top_room;
extern int top_exit;
extern int top_obj_index;
extern int top_mob_index;
extern int top_reset;
extern char str_empty[1];
extern MOB_INDEX_DATA *mob_index_hash[MAX_KEY_HASH];



int keep_chest_max_items(void)
{
    return 50;
}

void keep_format_chest_short_descr(const char *owner_name, char *dest, size_t dest_size)
{
    const char *safe_owner = (owner_name != NULL && owner_name[0] != '\0') ? owner_name : "Unknown";

    if (dest == NULL || dest_size == 0)
        return;

    snprintf(dest, dest_size, "%s's Keep Chest", safe_owner);
}

void keep_format_room_name(const char *owner_name, char *dest, size_t dest_size)
{
    const char *safe_owner = (owner_name != NULL && owner_name[0] != '\0') ? owner_name : "Unknown";

    if (dest == NULL || dest_size == 0)
        return;

    snprintf(dest, dest_size, "%s's Keep", safe_owner);
}

void keep_format_room_description(const char *owner_name, char *dest, size_t dest_size)
{
    const char *safe_owner = (owner_name != NULL && owner_name[0] != '\0') ? owner_name : "Unknown";

    if (dest == NULL || dest_size == 0)
        return;

    snprintf(dest, dest_size, "Keep of %s", safe_owner);
}

int keep_is_customization_command(const char *arg)
{
    return (arg != NULL && (!str_cmp(arg, "title") || !str_cmp(arg, "desc")));
}

int keep_is_upgrade_command(const char *arg)
{
    return (arg != NULL && (!str_cmp(arg, "regen") || !str_cmp(arg, "inside")));
}

int keep_player_can_customize(const CHAR_DATA *ch)
{
    if (ch == NULL || ch->pcdata == NULL || ch->in_room == NULL)
        return 0;

    if (ch->pcdata->keep_vnum <= 0)
        return 0;

    return (ch->in_room->vnum == ch->pcdata->keep_vnum);
}


static void keep_format_healer_name(const char *owner_name, char *dest, size_t dest_size)
{
    const char *safe_owner = (owner_name != NULL && owner_name[0] != '\0') ? owner_name : "Unknown";

    if (dest == NULL || dest_size == 0)
        return;

    snprintf(dest, dest_size, "%s's healer", safe_owner);
}

static MOB_INDEX_DATA *create_keep_healer_index(AREA_DATA *pArea, int vnum, const char *owner_name)
{
    int iHash;
    MOB_INDEX_DATA *pMobIndex;
    BUILD_DATA_LIST *pList;
    char name_buf[MAX_INPUT_LENGTH];
    char short_buf[MAX_STRING_LENGTH];
    char long_buf[MAX_STRING_LENGTH];
    char themed_name[256];

    GET_FREE(pMobIndex, mid_free);
    pMobIndex->vnum = vnum;
    pMobIndex->area = pArea;

    snprintf(name_buf, sizeof(name_buf), "%s keep healer", owner_name);
    pMobIndex->player_name = str_dup(name_buf);

    keep_format_healer_name(owner_name, themed_name, sizeof(themed_name));
    snprintf(short_buf, sizeof(short_buf), "%s", themed_name);
    pMobIndex->short_descr = str_dup(short_buf);

    snprintf(long_buf, sizeof(long_buf), "%.200s is here, ready to heal and aid you.\n\r", themed_name);
    pMobIndex->long_descr = str_dup(long_buf);
    pMobIndex->description = str_dup(long_buf);

    pMobIndex->act = ACT_IS_NPC | ACT_SENTINEL;
    pMobIndex->affected_by = 0;
    pMobIndex->pShop = NULL;
    pMobIndex->alignment = 1000;
    pMobIndex->level = 1;
    pMobIndex->sex = SEX_NEUTRAL;
    pMobIndex->ac_mod = 0;
    pMobIndex->hr_mod = 0;
    pMobIndex->dr_mod = 0;
    pMobIndex->spec_fun = spec_lookup("spec_cast_adept");
    pMobIndex->count = 0;
    pMobIndex->killed = 0;
    pMobIndex->target = NULL;
    pMobIndex->first_mprog = NULL;
    pMobIndex->last_mprog = NULL;
    pMobIndex->progtypes = 0;

    iHash = vnum % MAX_KEY_HASH;
    SING_TOPLINK(pMobIndex, mob_index_hash[iHash], next);

    GET_FREE(pList, build_free);
    pList->data = pMobIndex;
    LINK(pList, pArea->first_area_mobile, pArea->last_area_mobile, next, prev);

    top_mob_index++;

    return pMobIndex;
}

static void add_keep_healer_reset(AREA_DATA *pArea, ROOM_INDEX_DATA *room, int healer_vnum)
{
    RESET_DATA *pReset;
    BUILD_DATA_LIST *pList;

    GET_FREE(pReset, reset_free);
    pReset->command = 'M';
    pReset->arg1 = healer_vnum;
    pReset->arg2 = 1;
    pReset->arg3 = room->vnum;

    GET_FREE(pList, build_free);
    pList->data = pReset;

    LINK(pReset, pArea->first_reset, pArea->last_reset, next, prev);
    LINK(pList, room->first_room_reset, room->last_room_reset, next, prev);

    top_reset++;
}

static void add_keep_chest_reset(AREA_DATA *pArea, ROOM_INDEX_DATA *room, int chest_vnum)
{
    RESET_DATA *pReset;
    BUILD_DATA_LIST *pList;

    GET_FREE(pReset, reset_free);
    pReset->command = 'O';
    pReset->arg1 = chest_vnum;
    pReset->arg2 = 1;
    pReset->arg3 = room->vnum;

    GET_FREE(pList, build_free);
    pList->data = pReset;

    LINK(pReset, pArea->first_reset, pArea->last_reset, next, prev);
    LINK(pList, room->first_room_reset, room->last_room_reset, next, prev);

    top_reset++;
}

static OBJ_INDEX_DATA *create_keep_chest_index(AREA_DATA *pArea, int vnum, const char *owner_name)
{
    int iHash;
    int looper;
    OBJ_INDEX_DATA *pObjIndex;
    BUILD_DATA_LIST *pList;
    char name_buf[MAX_INPUT_LENGTH];
    char short_buf[MAX_STRING_LENGTH];
    char desc_buf[MAX_STRING_LENGTH];

    GET_FREE(pObjIndex, oid_free);
    pObjIndex->vnum = vnum;
    pObjIndex->area = pArea;

    snprintf(name_buf, sizeof(name_buf), "%s keep_chest", owner_name);
    pObjIndex->name = str_dup(name_buf);

    keep_format_chest_short_descr(owner_name, short_buf, sizeof(short_buf));
    pObjIndex->short_descr = str_dup(short_buf);

    snprintf(desc_buf, sizeof(desc_buf), "%s's Keep Chest rests here.", owner_name);
    pObjIndex->description = str_dup(desc_buf);

    pObjIndex->owner = str_dup(owner_name);
    pObjIndex->level = 1;
    pObjIndex->item_type = ITEM_CONTAINER;
    pObjIndex->extra_flags = ITEM_NOSAC;
    pObjIndex->wear_flags = 0;
    pObjIndex->item_apply = 1;

    for (looper = 0; looper < 10; looper++)
        pObjIndex->value[looper] = 0;

    pObjIndex->value[0] = 10000;
    pObjIndex->value[1] = CONT_CLOSEABLE | CONT_CLOSED | CONT_KEEP_CHEST;
    pObjIndex->value[2] = -1;
    pObjIndex->value[3] = keep_chest_max_items();

    pObjIndex->weight = 100;
    pObjIndex->cost = 0;

    pObjIndex->first_exdesc = NULL;
    pObjIndex->last_exdesc = NULL;
    pObjIndex->first_apply = NULL;
    pObjIndex->last_apply = NULL;

    iHash = vnum % MAX_KEY_HASH;
    SING_TOPLINK(pObjIndex, obj_index_hash[iHash], next);

    GET_FREE(pList, build_free);
    pList->data = pObjIndex;
    LINK(pList, pArea->first_area_object, pArea->last_area_object, next, prev);

    top_obj_index++;

    return pObjIndex;
}

void do_keep(CHAR_DATA *ch, char *argument)
{
    char arg1[MAX_INPUT_LENGTH];
    int original_recall_vnum;
    int vnum = 0;
    int iHash;
    ROOM_INDEX_DATA *topRoom = NULL;
    ROOM_INDEX_DATA *RoomIndex;
    ROOM_INDEX_DATA *templeRoom;
    OBJ_INDEX_DATA *keepChestIndex;
    OBJ_DATA *keepChest;
    EXIT_DATA *pExit;
    BUILD_DATA_LIST *pList;
    AREA_DATA *pArea = NULL;
    char room_name[MAX_STRING_LENGTH];
    char room_description[MAX_STRING_LENGTH];

    argument = one_argument(argument, arg1);

    if (arg1[0] == '\0')
    {
        if (ch->pcdata->keep_vnum > 0)
        {
            if (get_room_index(ch->pcdata->keep_vnum) == NULL)
            {
                send_to_char("Your keep cannot be found. Contact an immortal.\n\r", ch);
                return;
            }

            original_recall_vnum = ch->pcdata->recall_vnum;
            ch->pcdata->recall_vnum = ch->pcdata->keep_vnum;
            do_recall(ch, "");
            ch->pcdata->recall_vnum = original_recall_vnum;
            return;
        }

        send_to_char("Syntax: keep create | keep title <string> | keep desc <string> | keep regen | keep inside | keep healer\n\r", ch);
        return;
    }

    if (keep_is_upgrade_command(arg1))
    {
        int qp_cost;
        int room_flag;
        const char *flag_name;

        if (ch->pcdata->keep_vnum <= 0)
        {
            send_to_char("You do not have a keep yet.\n\r", ch);
            return;
        }

        if (!keep_player_can_customize(ch))
        {
            send_to_char("You must be in your keep to do that.\n\r", ch);
            return;
        }

        if (!str_cmp(arg1, "regen"))
        {
            qp_cost = 100;
            room_flag = ROOM_REGEN;
            flag_name = "regen";
        }
        else
        {
            qp_cost = 50;
            room_flag = ROOM_INDOORS;
            flag_name = "inside";
        }

        if (IS_SET(ch->in_room->room_flags, room_flag))
        {
            send_to_char("That keep upgrade has already been applied.\n\r", ch);
            return;
        }

        if (ch->quest_points < qp_cost)
        {
            send_to_char("You do not have enough quest points for that upgrade.\n\r", ch);
            return;
        }

        ch->quest_points -= qp_cost;
        SET_BIT(ch->in_room->room_flags, room_flag);
        do_savearea(NULL, (char *)ch->in_room->area);

        send_to_char("Keep upgraded.\n\r", ch);
        act("$n upgrades the keep with the $T flag.", ch, NULL, (char *)flag_name, TO_ROOM);
        return;
    }

    if (!str_cmp(arg1, "healer"))
    {
        ROOM_INDEX_DATA *keep_room;
        int healer_vnum;
        MOB_INDEX_DATA *keepHealerIndex;
        CHAR_DATA *keepHealer;

        if (ch->pcdata->keep_vnum <= 0)
        {
            send_to_char("You do not have a keep yet.\n\r", ch);
            return;
        }

        keep_room = get_room_index(ch->pcdata->keep_vnum);
        if (keep_room == NULL)
        {
            send_to_char("Your keep cannot be found. Contact an immortal.\n\r", ch);
            return;
        }

        if (!keep_player_can_customize(ch))
        {
            send_to_char("You must be in your keep to do that.\n\r", ch);
            return;
        }

        if (ch->pcdata->keep_healer_bought)
        {
            send_to_char("Your keep healer has already been purchased.\n\r", ch);
            return;
        }

        if (ch->quest_points < 250)
        {
            send_to_char("You do not have enough quest points for that upgrade.\n\r", ch);
            return;
        }

        for (healer_vnum = keep_room->area->min_vnum; healer_vnum <= keep_room->area->max_vnum; healer_vnum++)
        {
            if (get_room_index(healer_vnum) == NULL && get_obj_index(healer_vnum) == NULL && get_mob_index(healer_vnum) == NULL)
                break;
        }

        if (healer_vnum > keep_room->area->max_vnum)
        {
            send_to_char("Couldn't find an open vnum to create your keep healer in!\n\r", ch);
            return;
        }

        ch->quest_points -= 250;

        keepHealerIndex = create_keep_healer_index(keep_room->area, healer_vnum, ch->name);
        keepHealer = create_mobile(keepHealerIndex);
        char_to_room(keepHealer, keep_room);
        add_keep_healer_reset(keep_room->area, keep_room, healer_vnum);

        ch->pcdata->keep_healer_bought = 1;
        ch->pcdata->keep_healer_vnum = healer_vnum;

        do_savearea(NULL, (char *)keep_room->area);

        send_to_char("A personal healer now tends your keep.\n\r", ch);
        act("$n purchases a personal keep healer.", ch, NULL, NULL, TO_ROOM);
        return;
    }

    if (keep_is_customization_command(arg1))
    {
        if (!keep_player_can_customize(ch))
        {
            send_to_char("You must be in your keep to do that.\n\r", ch);
            return;
        }

        if (argument == NULL || argument[0] == '\0')
        {
            if (!str_cmp(arg1, "title"))
                send_to_char("Syntax: keep title <string>\n\r", ch);
            else
                send_to_char("Syntax: keep desc <string>\n\r", ch);
            return;
        }

        if (!str_cmp(arg1, "title"))
        {
            free_string(ch->in_room->name);
            ch->in_room->name = str_dup(argument);
            send_to_char("Keep title updated.\n\r", ch);
        }
        else
        {
            free_string(ch->in_room->description);
            ch->in_room->description = str_dup(argument);
            send_to_char("Keep description updated.\n\r", ch);
        }

        do_savearea(NULL, (char *)ch->in_room->area);
        return;
    }

    if (str_cmp(arg1, "create"))
    {
        send_to_char("Syntax: keep create | keep title <string> | keep desc <string> | keep regen | keep inside | keep healer\n\r", ch);
        return;
    }

    if (get_adept_level(ch) < 20 || ch->pcdata->keep_vnum > 0)
    {
        send_to_char("You must be a level 20 adept without an existing keep to do that.\n\r", ch);
        return;
    }

    for (pArea = first_area; pArea != NULL; pArea = pArea->next)
    {
        if (!str_cmp(pArea->filename, "playerhousing.are"))
            break;
    }

    if (pArea == NULL)
    {
        send_to_char("Could not find playerhousing.are to create your keep.\n\r", ch);
        return;
    }

    topRoom = get_room_index(PLAYER_HOUSING_START_VNUM);
    if (topRoom != NULL)
        pArea = topRoom->area;

    for (vnum = pArea->min_vnum; vnum <= pArea->max_vnum; vnum++)
    {
        if (get_room_index(vnum) == NULL && get_obj_index(vnum) == NULL)
            break;
    }

    if (vnum > pArea->max_vnum)
    {
        send_to_char("Couldn't find an open vnum to create your keep in!\n\r", ch);
        return;
    }

    RoomIndex = new_room(pArea, vnum, SECT_INSIDE);

    SET_BIT(RoomIndex->room_flags, ROOM_NO_TELEPORT);
    SET_BIT(RoomIndex->room_flags, ROOM_NOBLOODWALK);
    SET_BIT(RoomIndex->room_flags, ROOM_NO_PORTAL);
    SET_BIT(RoomIndex->room_flags, ROOM_NO_MOB);
    SET_BIT(RoomIndex->room_flags, ROOM_SAFE);

    keep_format_room_name(ch->name, room_name, sizeof(room_name));
    keep_format_room_description(ch->name, room_description, sizeof(room_description));
    free_string(RoomIndex->name);
    free_string(RoomIndex->description);
    RoomIndex->name = str_dup(room_name);
    RoomIndex->description = str_dup(room_description);

    iHash = vnum % MAX_KEY_HASH;
    SING_TOPLINK(RoomIndex, room_index_hash[iHash], next);

    GET_FREE(pList, build_free);
    pList->data = RoomIndex;
    LINK(pList, pArea->first_area_room, pArea->last_area_room, next, prev);
    top_room++;

    templeRoom = get_room_index(3001);
    if (templeRoom != NULL)
    {
        GET_FREE(pExit, exit_free);
        pExit->to_room = templeRoom;
        pExit->vnum = templeRoom->vnum;
        pExit->description = &str_empty[0];
        pExit->keyword = &str_empty[0];
        pExit->exit_info = 0;
        pExit->key = -1;
        RoomIndex->exit[DIR_DOWN] = pExit;
        top_exit++;
    }

    keepChestIndex = create_keep_chest_index(pArea, vnum, ch->name);
    keepChest = create_object(keepChestIndex, ch->level);
    obj_to_room(keepChest, RoomIndex);
    add_keep_chest_reset(pArea, RoomIndex, vnum);
    save_chest(keepChest);

    ch->pcdata->keep_vnum = vnum;
    do_savearea(NULL, (char *)pArea);

    send_to_char("Your keep has been created.\n\r", ch);
}
