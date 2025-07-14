#include "globals.h"

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
    LINK(pList, topRoom->area->first_area_room, pCurRoom->area->last_area_room, next, prev);
    top_room++;

    ch->pcdata->keep_vnum = vnum;
    /*   pRoomIndex->first_person = NULL;
   pRoomIndex->last_person = NULL;
   pRoomIndex->first_content = NULL;
   pRoomIndex->last_content = NULL;
   pRoomIndex->first_exdesc = NULL;
   pRoomIndex->last_exdesc = NULL;
   pRoomIndex->area = pArea;
   pRoomIndex->vnum = vnum;
   pRoomIndex->name = str_dup("New room");
   pRoomIndex->description = str_dup("No description");
   pRoomIndex->room_flags = 0;
   pRoomIndex->sector_type = sector;
   pRoomIndex->light = 0;
   for (door = 0; door <= 5; door++)
      pRoomIndex->exit[door] = NULL;
   pRoomIndex->first_room_reset = NULL;
   pRoomIndex->last_room_reset = NULL;*/

    send_to_char("Your keep has been created\n\r",ch);
}
