#include "globals.h"

ROOM_INDEX_DATA *new_room(AREA_DATA *pArea, sh_int vnum, sh_int sector);

extern int top_room;

void do_makekeep(CHAR_DATA *ch, char *argument)
{
    int vnum = 0;
    int iHash;
    ROOM_INDEX_DATA *topRoom = get_room_index(PLAYER_HOUSING_START_VNUM);
    ROOM_INDEX_DATA *RoomIndex;
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

    pArea = topRoom->area;

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
    RoomIndex = new_room(pArea, vnum, SECT_INSIDE);

    /*
     * Add room to hash table
     */

    iHash = vnum % MAX_KEY_HASH;
    SING_TOPLINK(RoomIndex, room_index_hash[iHash], next);

    /*
     * Add room into area list.
     */
    GET_FREE(pList, build_free);
    pList->data = RoomIndex;
    LINK(pList, topRoom->area->first_area_room, topRoom->area->last_area_room, next, prev);
    top_room++;

    ch->pcdata->keep_vnum = vnum;
    /*
   pRoomIndex->name = str_dup("New room");
   pRoomIndex->description = str_dup("No description");
   pRoomIndex->first_room_reset = NULL;
   pRoomIndex->last_room_reset = NULL;
   */

    send_to_char("Your keep has been created\n\r",ch);
}
