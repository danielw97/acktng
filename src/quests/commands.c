/***************************************************************************
 * commands.c  --  Player-facing quest commands and quest generation.
 *
 * Handles do_quest (the player command), quest status display, quest
 * completion turn-in, template quest listing and acceptance, and dynamic
 * quest generation (random mob/object quests assigned by postmen).
 ***************************************************************************/

#include "quest_internal.h"

extern AREA_DATA *area_used[MAX_AREAS];

/* ------------------------------------------------------------------
 * Reward scaling helpers (dynamic quests)
 * ------------------------------------------------------------------ */

static int quest_gold(int psuedo_level)
{
   int lvl = UMAX(1, UMIN(150, psuedo_level));
   return 10 + (990 * (lvl - 1) / 149);
}

static int quest_qp(int psuedo_level)
{
   int lvl = UMAX(1, UMIN(150, psuedo_level));
   return 1 + (19 * (lvl - 1) / 149);
}

static int quest_exp(CHAR_DATA *ch)
{
   int lvl;
   int exp;

   if (ch == NULL)
      return 0;

   lvl = UMAX(1, UMIN(MAX_MOB_LEVEL - 1, get_psuedo_level(ch)));
   exp = (int)(exp_table[lvl].mob_base * 3L);

   if (is_adept(ch))
      exp /= 1000;

   exp = UMIN(exp, 5000000);

   if (happy_hour)
      exp *= 2;

   return UMAX(1, exp);
}

/* ------------------------------------------------------------------
 * Dynamic quest mob/object selection
 * ------------------------------------------------------------------ */

static MOB_INDEX_DATA *find_quest_mob_index(int min_level, int max_level)
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
      if (wch->in_room != NULL && IS_SET(wch->in_room->area->flags, AREA_NOSHOW))
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

static OBJ_INDEX_DATA *find_quest_obj_index(int min_level, int max_level)
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
      if (pObj->vnum >= OBJ_VNUM_QUEST_MIN && pObj->vnum <= OBJ_VNUM_QUEST_MAX)
         continue;

      if (--skip <= 0 && number_percent() < 20)
         return pObj;
   }

   return NULL;
}

/* ------------------------------------------------------------------
 * Reward preview (shared by request, accept, status)
 * ------------------------------------------------------------------ */

static void show_reward_preview(CHAR_DATA *ch, QUEST_DATA *prop)
{
   char buf[MAX_STRING_LENGTH];
   const QUEST_TEMPLATE *tpl =
       prop->quest_template_id >= 0 ? find_quest_template(prop->quest_template_id) : NULL;
   int gold =
       prop->quest_template_id >= 0 ? prop->quest_reward_gold : quest_gold(get_psuedo_level(ch));
   int qp = prop->quest_template_id >= 0 ? prop->quest_reward_qp : quest_qp(get_psuedo_level(ch));
   int exp;
   if (tpl != NULL && tpl->reward_exp > 0)
      exp = tpl->reward_exp;
   else if (tpl != NULL)
   {
      exp = calc_quest_exp(tpl->max_level, quest_template_has_boss_target(tpl),
                           tpl->type == QUEST_TYPE_CARTOGRAPHY);
      if (is_adept(ch))
         exp /= 1000;
      exp = UMIN(exp, 5000000);
      if (happy_hour)
         exp *= 2;
      exp = UMAX(1, exp);
   }
   else
      exp = quest_exp(ch);

   sprintf(buf, "@@WReward on completion:@@N @@Y%d@@N gold, @@Y%d@@N exp, @@Y%d@@N quest point%s",
           gold, exp, qp, qp == 1 ? "" : "s");
   if (prop->quest_template_id >= 0)
   {
      if (quest_reward_item_is_valid(tpl))
         sprintf(buf + strlen(buf), ", @@Y1@@N x %s", tpl->reward_obj_short);
   }
   strcat(buf, ".\n\r");
   send_to_char(buf, ch);
}

/* ------------------------------------------------------------------
 * quest_request — dynamic quest assignment from a postman
 * ------------------------------------------------------------------ */

void quest_request(CHAR_DATA *ch, CHAR_DATA *postman)
{
   int psuedo_lvl;
   int min_lvl, max_lvl;
   int type;
   int i;
   int slot;
   QUEST_DATA *prop;
   char buf[MAX_STRING_LENGTH];

   if (IS_NPC(ch))
      return;

   if (ch->pcdata->quest_dynamic_cooldown_until > (int)current_time)
   {
      int wait_seconds = ch->pcdata->quest_dynamic_cooldown_until - (int)current_time;
      char wait_buf[MAX_STRING_LENGTH];
      sprintf(wait_buf, "You must wait %d more second%s before requesting a new dynamic quest.\n\r",
              wait_seconds, wait_seconds == 1 ? "" : "s");
      send_to_char(wait_buf, ch);
      return;
   }

   slot = find_free_quest_slot(ch);
   if (slot < 0)
   {
      send_to_char("You already have the maximum number of active quests.\n\r", ch);
      return;
   }

   psuedo_lvl = get_psuedo_level(ch);
   min_lvl = psuedo_lvl;
   max_lvl = psuedo_lvl + 15;

   type = number_range(QUEST_TYPE_KILL_VARIETY, QUEST_TYPE_KILL_COUNT);

   clear_quest_slot(ch, slot);
   prop = &ch->pcdata->quests[slot];
   prop->quest_type = type;

   switch (type)
   {
   case QUEST_TYPE_KILL_VARIETY:
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
         midx = find_quest_mob_index(min_lvl, max_lvl);
         if (midx == NULL)
            continue;

         for (k = 0; k < found; k++)
            if (prop->quest_target_vnum[k] == midx->vnum)
            {
               dup = TRUE;
               break;
            }
         if (dup)
            continue;

         prop->quest_target_vnum[found] = midx->vnum;
         prop->quest_target_done[found] = FALSE;
         found++;
      }

      prop->quest_num_targets = found;

      if (found == 0)
      {
         clear_quest_slot(ch, slot);
         send_to_char("The postman shrugs. 'Sorry, I can't find any work for you right now.'\n\r",
                      ch);
         return;
      }

      send_to_char("\n\r@@GYou have received a new quest!@@N\n\r\n\r", ch);
      sprintf(buf, "Assigned to slot @@Y%d@@N.\n\r", slot + 1);
      send_to_char(buf, ch);
      send_to_char("Hunt down each of the following enemies:\n\r", ch);
      for (i = 0; i < prop->quest_num_targets; i++)
      {
         MOB_INDEX_DATA *midx = get_mob_index(prop->quest_target_vnum[i]);
         if (midx != NULL)
         {
            sprintf(buf, "  @@Y- %s@@N\n\r", midx->short_descr);
            send_to_char(buf, ch);
         }
      }
      break;
   }

   case QUEST_TYPE_COLLECT_ITEMS:
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
         oidx = find_quest_obj_index(obj_min, obj_max);
         if (oidx == NULL)
            continue;

         for (k = 0; k < found; k++)
            if (prop->quest_target_vnum[k] == oidx->vnum)
            {
               dup = TRUE;
               break;
            }
         if (dup)
            continue;

         prop->quest_target_vnum[found] = oidx->vnum;
         prop->quest_target_done[found] = FALSE;
         found++;
      }

      prop->quest_num_targets = found;

      if (found == 0)
      {
         clear_quest_slot(ch, slot);
         send_to_char("The postman shrugs. 'Sorry, I can't find any work for you right now.'\n\r",
                      ch);
         return;
      }

      send_to_char("\n\r@@GYou have received a new quest!@@N\n\r\n\r", ch);
      sprintf(buf, "Assigned to slot @@Y%d@@N.\n\r", slot + 1);
      send_to_char(buf, ch);
      send_to_char(
          "Acquire each of the following items (credited when they enter your inventory):\n\r", ch);
      for (i = 0; i < prop->quest_num_targets; i++)
      {
         OBJ_INDEX_DATA *oidx = get_obj_index(prop->quest_target_vnum[i]);
         if (oidx != NULL)
         {
            sprintf(buf, "  @@C- %s @@N[level %d]\n\r", oidx->short_descr, oidx->level);
            send_to_char(buf, ch);
         }
      }
      break;
   }

   case QUEST_TYPE_KILL_COUNT:
   {
      MOB_INDEX_DATA *midx;
      int kill_need = number_range(5, 15);

      midx = find_quest_mob_index(psuedo_lvl + 5, psuedo_lvl + 15);
      if (midx == NULL)
      {
         clear_quest_slot(ch, slot);
         send_to_char("The postman shrugs. 'Sorry, I can't find any work for you right now.'\n\r",
                      ch);
         return;
      }

      prop->quest_num_targets = 1;
      prop->quest_target_vnum[0] = midx->vnum;
      prop->quest_target_done[0] = FALSE;
      prop->quest_kill_needed = kill_need;
      prop->quest_kill_count = 0;

      send_to_char("\n\r@@GYou have received a new quest!@@N\n\r\n\r", ch);
      sprintf(buf, "Assigned to slot @@Y%d@@N.\n\r", slot + 1);
      send_to_char(buf, ch);
      sprintf(buf, "Slay @@Y%d@@N of @@R%s@@N.\n\r", kill_need, midx->short_descr);
      send_to_char(buf, ch);
      break;
   }
   }

   show_reward_preview(ch, prop);
   send_to_char("Return to any postman when done to claim your reward.\n\r", ch);

   act("$N hands you a sealed note outlining your quest.", ch, NULL, postman, TO_CHAR);
   act("$N hands $n a sealed quest.", ch, NULL, postman, TO_ROOM);

   do_save(ch, "");
}

/* ------------------------------------------------------------------
 * quest_status — display active quest progress
 * ------------------------------------------------------------------ */

void quest_status(CHAR_DATA *ch)
{
   int slot, i;
   char buf[MAX_STRING_LENGTH];

   if (IS_NPC(ch))
      return;

   if (!quest_active(ch))
   {
      send_to_char("You have no active quest. Visit a postman to request one.\n\r", ch);
      return;
   }

   send_to_char("@@Y===  Quest Status  ===@@N\n\r\n\r", ch);

   for (slot = 0; slot < QUEST_MAX_QUESTS; slot++)
   {
      QUEST_DATA *prop = &ch->pcdata->quests[slot];
      if (prop->quest_type == QUEST_TYPE_NONE)
         continue;

      sprintf(buf, "@@W[Slot %d]@@N %s\n\r", slot + 1,
              prop->quest_template_id >= 0 ? "(quest)" : "(dynamic)");
      send_to_char(buf, ch);

      if (prop->quest_completed)
      {
         send_to_char("@@GStatus: COMPLETE@@N - turn in at a postman.\n\r\n\r", ch);
         continue;
      }

      switch (prop->quest_type)
      {
      case QUEST_TYPE_KILL_VARIETY:
         send_to_char("@@WTask:@@N Hunt down each of the following enemies:\n\r", ch);
         for (i = 0; i < prop->quest_num_targets; i++)
         {
            MOB_INDEX_DATA *midx = get_mob_index(prop->quest_target_vnum[i]);
            if (midx != NULL)
            {
               const char *area_name = midx->area ? midx->area->name : "Unknown Area";
               sprintf(buf, "  %s%-30s@@N  %s  (%s)\n\r",
                       prop->quest_target_done[i] ? "@@G" : "@@R", midx->short_descr,
                       prop->quest_target_done[i] ? "[DONE]" : "[pending]", area_name);
               send_to_char(buf, ch);
            }
         }
         break;

      case QUEST_TYPE_COLLECT_ITEMS:
         send_to_char("@@WTask:@@N Acquire each of the following items:\n\r", ch);
         for (i = 0; i < prop->quest_num_targets; i++)
         {
            OBJ_INDEX_DATA *oidx = get_obj_index(prop->quest_target_vnum[i]);
            if (oidx != NULL)
            {
               const char *area_name = oidx->area ? oidx->area->name : "Unknown Area";
               sprintf(buf, "  %s%-30s@@N  [lvl %-3d]  %s  (%s)\n\r",
                       prop->quest_target_done[i] ? "@@G" : "@@C", oidx->short_descr, oidx->level,
                       prop->quest_target_done[i] ? "[OBTAINED]" : "[needed]", area_name);
               send_to_char(buf, ch);
            }
         }
         break;

      case QUEST_TYPE_KILL_COUNT:
      {
         MOB_INDEX_DATA *midx = get_mob_index(prop->quest_target_vnum[0]);
         const char *area_name = (midx && midx->area) ? midx->area->name : "Unknown Area";
         sprintf(buf, "@@WTask:@@N Slay %s (%s)\n\r", midx ? midx->short_descr : "(unknown)",
                 area_name);
         send_to_char(buf, ch);
         sprintf(buf, "  Progress: @@Y%d@@N / @@Y%d@@N\n\r", prop->quest_kill_count,
                 prop->quest_kill_needed);
         send_to_char(buf, ch);
         break;
      }

      case QUEST_TYPE_CARTOGRAPHY:
      {
         AREA_DATA *carto_area = prop->quest_cartography_area_num >= 0
                                     ? area_used[prop->quest_cartography_area_num]
                                     : NULL;
         const char *area_name = carto_area ? carto_area->name : "Unknown Area";
         sprintf(buf, "@@WTask:@@N Explore every room in the target area (%s).\n\r", area_name);
         send_to_char(buf, ch);
         sprintf(buf, "  Progress: @@Y%d@@N / @@Y%d@@N rooms explored\n\r",
                 prop->quest_cartography_explored_count, prop->quest_cartography_room_count);
         send_to_char(buf, ch);
         break;
      }
      }

      show_reward_preview(ch, prop);
      send_to_char("\n\r", ch);
   }
}

/* ------------------------------------------------------------------
 * quest_complete — turn in completed quests to the appropriate NPC
 * ------------------------------------------------------------------ */

void quest_complete(CHAR_DATA *ch, CHAR_DATA *postman)
{
   int slot;
   bool has_completed = FALSE;
   char buf[MAX_STRING_LENGTH];

   if (IS_NPC(ch))
      return;

   for (slot = 0; slot < QUEST_MAX_QUESTS; slot++)
   {
      QUEST_DATA *prop = &ch->pcdata->quests[slot];
      int gold_reward;
      int exp_reward;
      int qp_reward;
      const QUEST_TEMPLATE *tpl = NULL;
      CHAR_DATA *turnin_npc = NULL;

      if (prop->quest_type == QUEST_TYPE_NONE || !prop->quest_completed)
         continue;

      has_completed = TRUE;

      if (prop->quest_template_id >= 0)
      {
         int required_vnum = prop->quest_offerer_vnum;
         tpl = find_quest_template(prop->quest_template_id);
         if (required_vnum <= 0 && tpl != NULL)
            required_vnum = canonical_postmaster_vnum(tpl->offerer_vnum);
         turnin_npc = find_npc_by_canonical_vnum(ch, required_vnum);
         if (turnin_npc == NULL)
            continue;
      }
      else
      {
         turnin_npc = postman;
         if (turnin_npc == NULL)
            continue;
      }

      gold_reward =
          prop->quest_template_id >= 0 ? prop->quest_reward_gold : quest_gold(get_psuedo_level(ch));
      if (prop->quest_template_id >= 0 && tpl != NULL && tpl->reward_exp > 0)
         exp_reward = tpl->reward_exp;
      else if (prop->quest_template_id >= 0 && tpl != NULL)
      {
         exp_reward = calc_quest_exp(tpl->max_level, quest_template_has_boss_target(tpl),
                                     tpl->type == QUEST_TYPE_CARTOGRAPHY);
         if (is_adept(ch))
            exp_reward /= 1000;
         exp_reward = UMIN(exp_reward, 5000000);
         if (happy_hour)
            exp_reward *= 2;
         exp_reward = UMAX(1, exp_reward);
      }
      else
         exp_reward = quest_exp(ch);
      qp_reward =
          prop->quest_template_id >= 0 ? prop->quest_reward_qp : quest_qp(get_psuedo_level(ch));

      act("$N reviews your completed quest and nods approvingly.", ch, NULL, turnin_npc, TO_CHAR);
      act("$N reviews $n's completed quest and nods approvingly.", ch, NULL, turnin_npc, TO_ROOM);

      send_to_char("\n\r@@GQuest complete! You receive:@@N\n\r", ch);
      sprintf(buf, "  @@Y%d@@N gold coins\n\r", gold_reward);
      send_to_char(buf, ch);
      sprintf(buf, "  @@Y%d@@N experience\n\r", exp_reward);
      send_to_char(buf, ch);
      sprintf(buf, "  @@Y%d@@N quest point%s\n\r", qp_reward, qp_reward == 1 ? "" : "s");
      send_to_char(buf, ch);
      send_to_char("  @@Y1@@N quest point\n\r", ch);

      if (prop->quest_template_id >= 0)
      {
         tpl = find_quest_template(prop->quest_template_id);
         if (tpl != NULL && tpl->completion_message != NULL && tpl->completion_message[0] != '\0')
         {
            sprintf(buf, "@@WCompletion report:@@N %s\n\r", tpl->completion_message);
            send_to_char(buf, ch);
         }
      }

      ch->gold += gold_reward;
      gain_exp(ch, exp_reward);
      ch->quest_points += qp_reward;
      ch->pcdata->post_quest_points += 1;

      if (prop->quest_template_id >= 0)
      {
         OBJ_DATA *reward;

         reward = create_quest_reward_object(ch, tpl);
         if (reward != NULL)
         {
            obj_to_char(reward, ch);
            sprintf(buf, "  @@Y1@@N x %s\n\r", reward->short_descr);
            send_to_char(buf, ch);
         }
      }

      if (prop->quest_template_id >= 0 && prop->quest_template_id < QUEST_MAX_TEMPLATES)
         ch->pcdata->completed_quests[prop->quest_template_id] = TRUE;

      clear_quest_slot(ch, slot);
      do_save(ch, "");
      return;
   }

   if (!quest_active(ch))
      send_to_char("You have no quest to turn in.\n\r", ch);
   else if (has_completed)
      send_to_char("You cannot turn that in here yet. Dynamic quests need a postman; quests "
                   "must be turned in to their offering mob.\n\r",
                   ch);
   else
      send_to_char("You have no completed quest to turn in yet.\n\r", ch);
}

/* ------------------------------------------------------------------
 * quest_list_available — list template quests offered at this location
 * ------------------------------------------------------------------ */

static void quest_list_available(CHAR_DATA *ch)
{
   int i;
   int ps_lvl;
   char buf[MAX_STRING_LENGTH];

   ps_lvl = get_psuedo_level(ch);
   send_to_char("@@Y=== Available Quests ===@@N\n\r", ch);

   for (i = 0; i < quest_template_count; i++)
   {
      const QUEST_TEMPLATE *tpl = &quest_template_table[i];
      if (tpl->id >= 0 && tpl->id < QUEST_MAX_TEMPLATES && ch->pcdata->completed_quests[tpl->id])
         continue;
      if (find_visible_npc_by_canonical_vnum(ch, tpl->offerer_vnum) == NULL)
         continue;
      if (!quest_prerequisite_met(ch, tpl))
         continue;

      if (ps_lvl < tpl->min_level || (tpl->max_level > 0 && ps_lvl > tpl->max_level))
         sprintf(buf, "@@W%2d)@@N %s @@r[requires pseudo-level %d-%d]@@N\n\r", i + 1, tpl->title,
                 tpl->min_level, tpl->max_level);
      else
         sprintf(buf, "@@W%2d)@@N %s @@g[pseudo-level %d-%d]@@N\n\r", i + 1, tpl->title,
                 tpl->min_level, tpl->max_level);
      send_to_char(buf, ch);
   }
}

/* ------------------------------------------------------------------
 * quest_accept — accept a numbered template quest from this location
 * ------------------------------------------------------------------ */

void quest_accept(CHAR_DATA *ch, int list_number)
{
   int slot, i;
   const QUEST_TEMPLATE *tpl;
   QUEST_DATA *prop;
   char buf[MAX_STRING_LENGTH];

   if (list_number < 1 || list_number > quest_template_count)
   {
      send_to_char("That quest number is not valid.\n\r", ch);
      return;
   }

   tpl = &quest_template_table[list_number - 1];

   if (find_visible_npc_by_canonical_vnum(ch, tpl->offerer_vnum) == NULL)
   {
      send_to_char("That quest is not being offered here.\n\r", ch);
      return;
   }

   if (tpl->id >= 0 && tpl->id < QUEST_MAX_TEMPLATES && ch->pcdata->completed_quests[tpl->id])
   {
      send_to_char("You have already completed that quest.\n\r", ch);
      return;
   }

   if (!quest_prerequisite_met(ch, tpl))
   {
      send_to_char("You have not unlocked that quest yet.\n\r", ch);
      return;
   }

   if (quest_template_already_active(ch, tpl->id))
   {
      send_to_char("You already have that quest active.\n\r", ch);
      return;
   }

   if (!IS_STAFF(ch) && get_psuedo_level(ch) < tpl->min_level)
   {
      sprintf(buf, "You must be at least pseudo-level %d for that quest.\n\r", tpl->min_level);
      send_to_char(buf, ch);
      return;
   }

   if (!IS_STAFF(ch) && tpl->max_level > 0 && get_psuedo_level(ch) > tpl->max_level)
   {
      sprintf(buf, "You must be pseudo-level %d or lower for that quest.\n\r", tpl->max_level);
      send_to_char(buf, ch);
      return;
   }

   slot = find_free_quest_slot(ch);
   if (slot < 0)
   {
      send_to_char("You cannot carry any more quests right now.\n\r", ch);
      return;
   }

   if (tpl->type == QUEST_TYPE_CARTOGRAPHY)
   {
      ROOM_INDEX_DATA *target_room;
      AREA_DATA *target_area;
      int room_count;

      if (tpl->num_targets < 1 || tpl->target_vnum[0] <= 0)
      {
         send_to_char("That cartography quest is misconfigured.\n\r", ch);
         return;
      }

      target_room = get_room_index(tpl->target_vnum[0]);
      target_area = target_room != NULL ? target_room->area : NULL;
      if (target_area == NULL)
      {
         send_to_char("That cartography quest has an invalid area target.\n\r", ch);
         return;
      }

      room_count = quest_cartography_area_room_count(target_area);
      if (room_count <= 0)
      {
         send_to_char("That cartography quest has no explorable rooms.\n\r", ch);
         return;
      }
      if (room_count > QUEST_CARTOGRAPHY_MAX_ROOMS)
      {
         send_to_char("That cartography quest area is too large to track.\n\r", ch);
         return;
      }
   }

   clear_quest_slot(ch, slot);
   prop = &ch->pcdata->quests[slot];
   prop->quest_type = tpl->type;
   prop->quest_offerer_vnum = canonical_postmaster_vnum(tpl->offerer_vnum);
   prop->quest_num_targets = tpl->num_targets;
   prop->quest_kill_needed = tpl->kill_needed;
   prop->quest_template_id = tpl->id;
   prop->quest_reward_gold = tpl->reward_gold;
   prop->quest_reward_qp = tpl->reward_qp;
   prop->quest_reward_item_vnum = 0;
   prop->quest_reward_item_count = 0;

   for (i = 0; i < tpl->num_targets; i++)
   {
      prop->quest_target_vnum[i] = tpl->target_vnum[i];
      prop->quest_target_done[i] = FALSE;
   }

   if (tpl->type == QUEST_TYPE_CARTOGRAPHY)
   {
      ROOM_INDEX_DATA *target_room = get_room_index(tpl->target_vnum[0]);
      AREA_DATA *target_area = target_room != NULL ? target_room->area : NULL;

      prop->quest_cartography_area_num = target_area != NULL ? target_area->area_num : -1;
      prop->quest_cartography_room_count = quest_cartography_area_room_count(target_area);
      prop->quest_cartography_explored_count = 0;
      memset(prop->quest_cartography_bits, 0, sizeof(prop->quest_cartography_bits));
   }

   sprintf(buf, "@@GYou accepted quest [%d] in slot %d:@@N %s\n\r", list_number, slot + 1,
           tpl->title);
   send_to_char(buf, ch);
   if (tpl->accept_message != NULL && tpl->accept_message[0] != '\0')
   {
      sprintf(buf, "@@WQuest briefing:@@N %s\n\r", tpl->accept_message);
      send_to_char(buf, ch);
   }
   if (prop->quest_type == QUEST_TYPE_CARTOGRAPHY)
   {
      AREA_DATA *carto_area = prop->quest_cartography_area_num >= 0
                                  ? area_used[prop->quest_cartography_area_num]
                                  : NULL;
      const char *area_name = carto_area ? carto_area->name : "Unknown Area";
      sprintf(buf, "@@WTask:@@N Explore every room in the target area (@@Y%d/%d@@N) (%s).\n\r",
              prop->quest_cartography_explored_count, prop->quest_cartography_room_count,
              area_name);
      send_to_char(buf, ch);
   }
   show_reward_preview(ch, prop);
   do_save(ch, "");
}

/* ------------------------------------------------------------------
 * do_quest — top-level player command
 * ------------------------------------------------------------------ */

void do_quest(CHAR_DATA *ch, char *argument)
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
      send_to_char("Usage: quest <request|status|complete|list|accept #|cancel #>\n\r", ch);
      return;
   }

   if (!str_prefix(arg, "status"))
   {
      quest_status(ch);
      return;
   }

   if (!str_prefix(arg, "cancel"))
   {
      if (!is_number(arg2))
      {
         send_to_char("Usage: quest cancel <slot# 1-5>\n\r", ch);
         return;
      }
      quest_cancel(ch, atoi(arg2) - 1);
      return;
   }

   if (!str_prefix(arg, "request") || !str_prefix(arg, "complete") || !str_prefix(arg, "list") ||
       !str_prefix(arg, "accept"))
   {
      for (wch = ch->in_room->first_person; wch != NULL; wch = wch->next_in_room)
      {
         if (IS_NPC(wch) && IS_SET(wch->act, ACT_POSTMAN) && can_see(ch, wch))
         {
            postman = wch;
            break;
         }
      }

      if (!str_prefix(arg, "request"))
      {
         if (postman == NULL)
         {
            send_to_char("You need to be with a postman to do that.\n\r", ch);
            return;
         }
         quest_request(ch, postman);
      }
      else if (!str_prefix(arg, "complete"))
         quest_complete(ch, postman);
      else if (!str_prefix(arg, "list"))
         quest_list_available(ch);
      else
      {
         if (!is_number(arg2))
         {
            send_to_char("Usage: quest accept <number>\n\r", ch);
            return;
         }
         quest_accept(ch, atoi(arg2));
      }

      return;
   }

   send_to_char("Usage: quest <request|status|complete|list|accept #|cancel #>\n\r", ch);
}
