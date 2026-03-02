#include "globals.h"

ROOM_INDEX_DATA *new_room(AREA_DATA *pArea, sh_int vnum, sh_int sector);

extern int top_room;
extern int top_exit;
extern int top_obj_index;
extern char str_empty[1];


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
    pObjIndex->extra_flags = 0;
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

        send_to_char("Syntax: keep create\n\r", ch);
        return;
    }

    if (str_cmp(arg1, "create"))
    {
        send_to_char("Syntax: keep create\n\r", ch);
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
    save_corpses();

    ch->pcdata->keep_vnum = vnum;
    do_savearea(NULL, (char *)pArea);

    send_to_char("Your keep has been created.\n\r", ch);
}
