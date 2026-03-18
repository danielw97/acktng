/***************************************************************************
 * cartography.c  --  Quest cartography helpers.
 *
 * Provides area room counting and indexing used by cartography quests.
 ***************************************************************************/

#include "quest_internal.h"

int quest_cartography_area_room_count(const AREA_DATA *area)
{
   BUILD_DATA_LIST *node;
   int count = 0;

   if (area == NULL)
      return 0;

   for (node = area->first_area_room; node != NULL; node = node->next)
   {
      if (node->data != NULL)
         count++;
   }

   return count;
}

int quest_cartography_room_index(const AREA_DATA *area, const ROOM_INDEX_DATA *room)
{
   BUILD_DATA_LIST *node;
   int index = 0;

   if (area == NULL || room == NULL)
      return -1;

   for (node = area->first_area_room; node != NULL; node = node->next)
   {
      ROOM_INDEX_DATA *candidate = (ROOM_INDEX_DATA *)node->data;

      if (candidate == NULL)
         continue;

      if (candidate == room)
         return index;

      index++;
   }

   return -1;
}
