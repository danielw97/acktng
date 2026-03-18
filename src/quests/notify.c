/***************************************************************************
 * notify.c  --  Quest progress notification handlers.
 *
 * Called by the game engine when a player kills a mob, picks up an object,
 * or enters a room, to record progress on active quests.
 ***************************************************************************/

#include "quest_internal.h"

static bool check_all_done(QUEST_DATA *prop)
{
   int i;
   for (i = 0; i < prop->quest_num_targets; i++)
      if (!prop->quest_target_done[i])
         return FALSE;
   return TRUE;
}

void quest_kill_notify(CHAR_DATA *ch, CHAR_DATA *victim)
{
   int slot;

   if (IS_NPC(ch) || !IS_NPC(victim) || ch->pcdata == NULL)
      return;

   for (slot = 0; slot < QUEST_MAX_QUESTS; slot++)
   {
      QUEST_DATA *prop = &ch->pcdata->quests[slot];
      int i;
      bool progress = FALSE;
      char buf[MAX_STRING_LENGTH];

      if (prop->quest_type == QUEST_TYPE_NONE || prop->quest_completed)
         continue;

      switch (prop->quest_type)
      {
      case QUEST_TYPE_KILL_VARIETY:
         for (i = 0; i < prop->quest_num_targets; i++)
         {
            if (!prop->quest_target_done[i] &&
                prop->quest_target_vnum[i] == victim->pIndexData->vnum)
            {
               prop->quest_target_done[i] = TRUE;
               progress = TRUE;
               sprintf(buf, "@@GQuest slot %d progress:@@N %s killed!\n\r", slot + 1,
                       victim->short_descr);
               send_to_char(buf, ch);
               break;
            }
         }
         if (progress && check_all_done(prop))
         {
            prop->quest_completed = TRUE;
            send_to_char(
                "\n\r@@G*** Quest complete! Visit any postman to claim your reward. ***@@N\n\r\n\r",
                ch);
         }
         break;

      case QUEST_TYPE_KILL_COUNT:
         if (prop->quest_target_vnum[0] == victim->pIndexData->vnum)
         {
            prop->quest_kill_count++;
            progress = TRUE;
            sprintf(buf, "@@GQuest slot %d progress:@@N %d/%d %s slain.\n\r", slot + 1,
                    prop->quest_kill_count, prop->quest_kill_needed, victim->short_descr);
            send_to_char(buf, ch);
            if (prop->quest_kill_count >= prop->quest_kill_needed)
            {
               prop->quest_completed = TRUE;
               send_to_char("\n\r@@G*** Quest complete! Visit any postman to claim your reward. "
                            "***@@N\n\r\n\r",
                            ch);
            }
         }
         break;
      }

      if (progress)
         do_save(ch, "");
   }
}

void quest_obj_notify(CHAR_DATA *ch, OBJ_DATA *obj)
{
   int slot;

   if (IS_NPC(ch) || ch->pcdata == NULL)
      return;

   for (slot = 0; slot < QUEST_MAX_QUESTS; slot++)
   {
      QUEST_DATA *prop = &ch->pcdata->quests[slot];
      int i;
      char buf[MAX_STRING_LENGTH];

      if (prop->quest_type != QUEST_TYPE_COLLECT_ITEMS || prop->quest_completed)
         continue;

      for (i = 0; i < prop->quest_num_targets; i++)
      {
         if (!prop->quest_target_done[i] && prop->quest_target_vnum[i] == obj->pIndexData->vnum)
         {
            prop->quest_target_done[i] = TRUE;
            sprintf(buf, "@@GQuest slot %d progress:@@N %s obtained!\n\r", slot + 1,
                    obj->short_descr);
            send_to_char(buf, ch);

            if (check_all_done(prop))
            {
               prop->quest_completed = TRUE;
               send_to_char("\n\r@@G*** Quest complete! Visit any postman to claim your reward. "
                            "***@@N\n\r\n\r",
                            ch);
            }

            extract_obj(obj);
            do_save(ch, "");
            return;
         }
      }
   }
}

void quest_room_notify(CHAR_DATA *ch, ROOM_INDEX_DATA *room)
{
   int slot;

   if (IS_NPC(ch) || ch->pcdata == NULL || room == NULL || room->area == NULL)
      return;

   for (slot = 0; slot < QUEST_MAX_QUESTS; slot++)
   {
      QUEST_DATA *prop = &ch->pcdata->quests[slot];

      if (prop->quest_type != QUEST_TYPE_CARTOGRAPHY || prop->quest_completed)
         continue;

      if (prop->quest_cartography_area_num != room->area->area_num)
         continue;

      {
         int room_index = quest_cartography_room_index(room->area, room);
         int byte_idx;
         unsigned char bit;
         char buf[MAX_STRING_LENGTH];

         if (room_index < 0 || room_index >= QUEST_CARTOGRAPHY_MAX_ROOMS)
            continue;

         byte_idx = room_index / 8;
         bit = (unsigned char)(1u << (room_index % 8));

         if ((prop->quest_cartography_bits[byte_idx] & bit) != 0)
            continue;

         prop->quest_cartography_bits[byte_idx] |= bit;
         prop->quest_cartography_explored_count++;

         if (prop->quest_cartography_room_count > 0 &&
             prop->quest_cartography_explored_count >= prop->quest_cartography_room_count)
         {
            prop->quest_completed = TRUE;
            send_to_char("\n\r@@G*** Cartography quest complete! Return to the quest giver to "
                         "claim your reward. ***@@N\n\r\n\r",
                         ch);
         }
         else
         {
            sprintf(buf, "@@GQuest slot %d progress:@@N explored @@Y%d/%d@@N rooms.\n\r", slot + 1,
                    prop->quest_cartography_explored_count, prop->quest_cartography_room_count);
            send_to_char(buf, ch);
         }

         do_save(ch, "");
      }
   }
}
