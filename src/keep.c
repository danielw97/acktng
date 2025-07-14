#include "globals.h"

void do_makekeep(CHAR_DATA *ch, char *argument)
{
    int vnum = 0;
    ROOM_INDEX_DATA *pRoomIndex = get_room_index(PLAYER_HOUSING_START_VNUM);
    BUILD_DATA_LIST *pList;
    AREA_DATA *pArea;

    if (get_adept_level(ch) < 20)
    {
        send_to_char("You must be a realm lord to do that.\n\r",ch);
        return;
    }
    
    if (ch->pcdata->keep_vnum != 0)
    {
        send_to_char("You already have a keep!\n\r", ch);
        return;
    }

    pArea = pRoomIndex->area;

    for(int i = 0; i < 300; i++)
    {
        if (get_room_index(PLAYER_HOUSING_START_VNUM+i) == NULL)
        {
            vnum = PLAYER_HOUSING_START_VNUM+i;
            break;
        }
    }

    if (vnum == 0)
    {
        send_to_char("Couldn't find an open vnum to create your Keep in!\n\r",ch);
        return;
    }

    /*
     * Create room
     */
    pRoomIndex = new_room(pArea, vnum, SECT_INSIDE);

    /*
     * Add room to hash table
     */

    iHash = vnum % MAX_KEY_HASH;
    SING_TOPLINK(pRoomIndex, room_index_hash[iHash], next);

    /*
     * Add room into area list.
     */
    GET_FREE(pList, build_free);
    pList->data = pRoomIndex;
    LINK(pList, pCurRoom->area->first_area_room, pCurRoom->area->last_area_room, next, prev);
    top_room++;

    ch->pcdata->keep_vnum = vnum;

    send_to_char("Your keep has been created\n\r",ch);
}
