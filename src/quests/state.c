/***************************************************************************
 * state.c  --  Quest slot state management and postmaster utilities.
 *
 * Manages per-character quest slot lifecycle: allocation, clearing,
 * cancellation, and prerequisite checks.  Also provides postmaster NPC
 * lookup helpers used across the quest module.
 ***************************************************************************/

#include "quest_internal.h"

int canonical_postmaster_vnum(int vnum)
{
   switch (vnum)
   {
   case 3360:
      return 3340; /* legacy Kiess postmaster vnum */
   case 3460:
   case 0:
      return 3440; /* legacy/placeholder Kowloon postmaster vnum */
   default:
      return vnum;
   }
}

CHAR_DATA *find_visible_npc_by_canonical_vnum(CHAR_DATA *ch, int vnum)
{
   CHAR_DATA *wch;

   if (ch == NULL || ch->in_room == NULL || vnum <= 0)
      return NULL;

   for (wch = ch->in_room->first_person; wch != NULL; wch = wch->next_in_room)
   {
      if (!IS_NPC(wch) || wch->pIndexData == NULL)
         continue;
      if (!can_see(ch, wch))
         continue;
      if (canonical_postmaster_vnum(wch->pIndexData->vnum) == canonical_postmaster_vnum(vnum))
         return wch;
   }

   return NULL;
}

CHAR_DATA *find_npc_by_canonical_vnum(CHAR_DATA *ch, int vnum)
{
   CHAR_DATA *wch;

   if (ch == NULL || ch->in_room == NULL || vnum <= 0)
      return NULL;

   for (wch = ch->in_room->first_person; wch != NULL; wch = wch->next_in_room)
   {
      if (!IS_NPC(wch) || wch->pIndexData == NULL)
         continue;
      if (canonical_postmaster_vnum(wch->pIndexData->vnum) == canonical_postmaster_vnum(vnum))
         return wch;
   }

   return NULL;
}

QUEST_DATA *get_quest_slot(CHAR_DATA *ch, int slot)
{
   if (IS_NPC(ch) || ch->pcdata == NULL)
      return NULL;
   if (slot < 0 || slot >= QUEST_MAX_QUESTS)
      return NULL;
   return &ch->pcdata->quests[slot];
}

int find_free_quest_slot(CHAR_DATA *ch)
{
   int i;
   for (i = 0; i < QUEST_MAX_QUESTS; i++)
   {
      QUEST_DATA *prop = get_quest_slot(ch, i);
      if (prop != NULL && prop->quest_type == QUEST_TYPE_NONE)
         return i;
   }
   return -1;
}

bool quest_active(CHAR_DATA *ch)
{
   int i;

   if (IS_NPC(ch) || ch->pcdata == NULL)
      return FALSE;

   for (i = 0; i < QUEST_MAX_QUESTS; i++)
      if (ch->pcdata->quests[i].quest_type != QUEST_TYPE_NONE)
         return TRUE;

   return FALSE;
}

void clear_quest_slot(CHAR_DATA *ch, int slot)
{
   int i;
   QUEST_DATA *prop = get_quest_slot(ch, slot);

   if (prop == NULL)
      return;

   prop->quest_type = QUEST_TYPE_NONE;
   prop->quest_completed = FALSE;
   prop->quest_num_targets = 0;
   prop->quest_kill_needed = 0;
   prop->quest_kill_count = 0;
   prop->quest_template_id = -1;
   prop->quest_reward_gold = 0;
   prop->quest_reward_qp = 0;
   prop->quest_reward_item_vnum = 0;
   prop->quest_reward_item_count = 0;
   prop->quest_offerer_vnum = 0;
   prop->quest_cartography_area_num = -1;
   prop->quest_cartography_room_count = 0;
   prop->quest_cartography_explored_count = 0;
   memset(prop->quest_cartography_bits, 0, sizeof(prop->quest_cartography_bits));

   for (i = 0; i < QUEST_MAX_TARGETS; i++)
   {
      prop->quest_target_vnum[i] = 0;
      prop->quest_target_done[i] = FALSE;
   }
}

void clear_quest(CHAR_DATA *ch)
{
   int i;

   if (IS_NPC(ch) || ch->pcdata == NULL)
      return;

   for (i = 0; i < QUEST_MAX_QUESTS; i++)
      clear_quest_slot(ch, i);
}

void quest_cancel(CHAR_DATA *ch, int slot)
{
   QUEST_DATA *prop;
   char buf[MAX_STRING_LENGTH];

   if (IS_NPC(ch) || ch->pcdata == NULL)
      return;

   if (slot < 0 || slot >= QUEST_MAX_QUESTS)
   {
      send_to_char("Usage: quest cancel <slot# 1-5>\n\r", ch);
      return;
   }

   prop = &ch->pcdata->quests[slot];
   if (prop->quest_type == QUEST_TYPE_NONE)
   {
      send_to_char("That quest slot is already empty.\n\r", ch);
      return;
   }

   if (prop->quest_template_id < 0)
      ch->pcdata->quest_dynamic_cooldown_until = (int)current_time + 15 * 60;

   clear_quest_slot(ch, slot);

   sprintf(buf, "You cancel quest slot @@Y%d@@N.\n\r", slot + 1);
   send_to_char(buf, ch);

   if (ch->pcdata->quest_dynamic_cooldown_until > (int)current_time)
      send_to_char("Cancelling a dynamic quest starts a 15 minute request cooldown.\n\r", ch);

   do_save(ch, "");
}

bool quest_prerequisite_met(CHAR_DATA *ch, const QUEST_TEMPLATE *tpl)
{
   if (tpl->prerequisite_template_id < 0)
      return TRUE;

   if (tpl->prerequisite_template_id >= QUEST_MAX_TEMPLATES)
      return FALSE;

   return ch->pcdata->completed_quests[tpl->prerequisite_template_id];
}

bool quest_template_already_active(CHAR_DATA *ch, int template_id)
{
   int i;

   for (i = 0; i < QUEST_MAX_QUESTS; i++)
   {
      QUEST_DATA *prop = &ch->pcdata->quests[i];
      if (prop->quest_type != QUEST_TYPE_NONE && prop->quest_template_id == template_id)
         return TRUE;
   }

   return FALSE;
}
