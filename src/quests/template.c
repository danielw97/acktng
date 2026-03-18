/***************************************************************************
 * template.c  --  Quest template loading and management.
 *
 * Loads quest definitions from .prop files and provides lookup and
 * reward helpers used throughout the quest module.
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include "quest_internal.h"

void set_obj_stat_auto(OBJ_DATA *obj);

#define QUEST_TEMPLATE_DIR "../quests"

QUEST_TEMPLATE *quest_template_table = NULL;
int quest_template_count = 0;

static bool read_prop_line(FILE *fp, char *buf, size_t size)
{
   while (fgets(buf, (int)size, fp) != NULL)
   {
      char *p;
      size_t len;

      while (*buf != '\0' && isspace(*buf))
         memmove(buf, buf + 1, strlen(buf));
      if (buf[0] == '\0' || buf[0] == '\n' || buf[0] == '#')
         continue;

      len = strlen(buf);
      while (len > 0 && (buf[len - 1] == '\n' || buf[len - 1] == '\r'))
         buf[--len] = '\0';

      p = buf + strlen(buf);
      while (p > buf && isspace(*(p - 1)))
         *(--p) = '\0';

      if (buf[0] == '\0')
         continue;

      return TRUE;
   }

   return FALSE;
}

static bool load_quest_template_file(const char *path, int id)
{
   FILE *fp;
   char line[MAX_STRING_LENGTH];
   QUEST_TEMPLATE tpl;
   QUEST_TEMPLATE *grown;
   int i;
   char *token;

   fp = fopen(path, "r");
   if (fp == NULL)
      return FALSE;

   memset(&tpl, 0, sizeof(tpl));
   tpl.id = id;
   tpl.prerequisite_template_id = -1;
   tpl.reward_obj_short = str_dup("");
   tpl.reward_obj_name = str_dup("");
   tpl.reward_obj_long = str_dup("");
   tpl.reward_obj_wear_flags = 0;
   tpl.reward_obj_extra_flags = 0;
   tpl.reward_obj_weight = 0;
   tpl.reward_obj_item_apply = 0;
   tpl.accept_message = str_dup("");
   tpl.completion_message = str_dup("");

   if (!read_prop_line(fp, line, sizeof(line)))
   {
      bugf("load_quest_template_file: missing title in %s", path);
      free_string(tpl.reward_obj_short);
      free_string(tpl.reward_obj_name);
      free_string(tpl.reward_obj_long);
      free_string(tpl.accept_message);
      free_string(tpl.completion_message);
      fclose(fp);
      return FALSE;
   }
   tpl.title = str_dup(line);

   if (!read_prop_line(fp, line, sizeof(line)))
   {
      bugf("load_quest_template_file: missing numeric line in %s", path);
      free_string(tpl.title);
      free_string(tpl.reward_obj_short);
      free_string(tpl.reward_obj_name);
      free_string(tpl.reward_obj_long);
      free_string(tpl.accept_message);
      free_string(tpl.completion_message);
      fclose(fp);
      return FALSE;
   }

   tpl.max_level = 170;
   if (sscanf(line, "%d %d %d %d %d %d %d %d %d %d %*d", &tpl.prerequisite_template_id,
              &tpl.type, &tpl.num_targets, &tpl.kill_needed, &tpl.min_level, &tpl.max_level,
              &tpl.offerer_vnum, &tpl.reward_gold, &tpl.reward_qp, &tpl.reward_exp) != 10)
   {
      tpl.reward_exp = 0;
      if (sscanf(line, "%d %d %d %d %d %d %d %d %d %*d", &tpl.prerequisite_template_id,
                 &tpl.type, &tpl.num_targets, &tpl.kill_needed, &tpl.min_level, &tpl.offerer_vnum,
                 &tpl.reward_gold, &tpl.reward_qp, &tpl.reward_exp) != 9)
      {
         bugf("load_quest_template_file: bad numeric line in %s", path);
         free_string(tpl.title);
         free_string(tpl.reward_obj_short);
         free_string(tpl.reward_obj_name);
         free_string(tpl.reward_obj_long);
         free_string(tpl.accept_message);
         free_string(tpl.completion_message);
         fclose(fp);
         return FALSE;
      }
   }

   if (!read_prop_line(fp, line, sizeof(line)))
   {
      bugf("load_quest_template_file: missing target line in %s", path);
      free_string(tpl.title);
      free_string(tpl.reward_obj_short);
      free_string(tpl.reward_obj_name);
      free_string(tpl.reward_obj_long);
      free_string(tpl.accept_message);
      free_string(tpl.completion_message);
      fclose(fp);
      return FALSE;
   }

   token = strtok(line, " \t");
   for (i = 0; i < QUEST_MAX_TARGETS && token != NULL; i++)
   {
      tpl.target_vnum[i] = atoi(token);
      token = strtok(NULL, " \t");
   }

   if (tpl.num_targets < 0 || tpl.num_targets > QUEST_MAX_TARGETS)
   {
      bugf("load_quest_template_file: invalid target count in %s", path);
      free_string(tpl.title);
      free_string(tpl.reward_obj_short);
      free_string(tpl.reward_obj_name);
      free_string(tpl.reward_obj_long);
      free_string(tpl.accept_message);
      free_string(tpl.completion_message);
      fclose(fp);
      return FALSE;
   }

   if (read_prop_line(fp, line, sizeof(line)))
   {
      free_string(tpl.accept_message);
      tpl.accept_message = str_dup(line);
   }

   if (read_prop_line(fp, line, sizeof(line)))
   {
      free_string(tpl.completion_message);
      tpl.completion_message = str_dup(line);
   }

   if (read_prop_line(fp, line, sizeof(line)))
   {
      free_string(tpl.reward_obj_short);
      tpl.reward_obj_short = str_dup(line);
   }

   if (read_prop_line(fp, line, sizeof(line)))
   {
      free_string(tpl.reward_obj_name);
      tpl.reward_obj_name = str_dup(line);
   }

   if (read_prop_line(fp, line, sizeof(line)))
   {
      free_string(tpl.reward_obj_long);
      tpl.reward_obj_long = str_dup(line);
   }

   if (read_prop_line(fp, line, sizeof(line)))
      tpl.reward_obj_wear_flags = atoi(line);

   if (read_prop_line(fp, line, sizeof(line)))
      tpl.reward_obj_extra_flags = atoi(line);

   if (read_prop_line(fp, line, sizeof(line)))
      tpl.reward_obj_weight = atoi(line);

   if (read_prop_line(fp, line, sizeof(line)))
      tpl.reward_obj_item_apply = atoi(line);

   grown = realloc(quest_template_table, sizeof(*grown) * (quest_template_count + 1));
   if (grown == NULL)
   {
      bug("load_quest_template_file: out of memory growing quest template table", 0);
      free_string(tpl.title);
      free_string(tpl.reward_obj_short);
      free_string(tpl.reward_obj_name);
      free_string(tpl.reward_obj_long);
      free_string(tpl.accept_message);
      free_string(tpl.completion_message);
      fclose(fp);
      return FALSE;
   }

   quest_template_table = grown;
   quest_template_table[quest_template_count++] = tpl;
   fclose(fp);
   return TRUE;
}

void quest_load_templates(void)
{
   char path[MAX_STRING_LENGTH];
   char buf[MAX_STRING_LENGTH];
   int i;

   if (quest_template_table != NULL)
   {
      for (i = 0; i < quest_template_count; i++)
      {
         free_string(quest_template_table[i].title);
         free_string(quest_template_table[i].reward_obj_short);
         free_string(quest_template_table[i].reward_obj_name);
         free_string(quest_template_table[i].reward_obj_long);
         free_string(quest_template_table[i].accept_message);
         free_string(quest_template_table[i].completion_message);
      }
      free(quest_template_table);
      quest_template_table = NULL;
      quest_template_count = 0;
   }

   for (i = 1; i <= QUEST_MAX_TEMPLATES; i++)
   {
      snprintf(path, sizeof(path), "%s/%d.prop", QUEST_TEMPLATE_DIR, i);
      load_quest_template_file(path, i - 1);
   }

   snprintf(buf, sizeof(buf), "Loaded %d quest template%s from %s.", quest_template_count,
            quest_template_count == 1 ? "" : "s", QUEST_TEMPLATE_DIR);
   log_string(buf);
}

const QUEST_TEMPLATE *find_quest_template(int template_id)
{
   int i;

   for (i = 0; i < quest_template_count; i++)
      if (quest_template_table[i].id == template_id)
         return &quest_template_table[i];

   return NULL;
}

/*
 * Raw exp reward for a quest template: 3x the mob_base for a mob at max_level.
 * Boss quests (any target has ACT_BOSS) get double; cartography quests get 10x.
 * Character modifiers (adept, happy hour) are applied by the caller.
 */
int calc_quest_exp(int max_level, bool is_boss, bool is_cartography)
{
   int lvl = UMAX(0, UMIN(MAX_MOB_LEVEL - 1, max_level));
   int exp = (int)(exp_table[lvl].mob_base * 3L);
   if (is_boss)
      exp *= 2;
   if (is_cartography)
      exp *= 10;
   return UMAX(1, exp);
}

bool quest_template_has_boss_target(const QUEST_TEMPLATE *tpl)
{
   int i;
   if (tpl == NULL)
      return FALSE;
   for (i = 0; i < tpl->num_targets; i++)
   {
      MOB_INDEX_DATA *mob = get_mob_index(tpl->target_vnum[i]);
      if (mob != NULL && IS_SET(mob->act, ACT_BOSS))
         return TRUE;
   }
   return FALSE;
}

bool quest_reward_item_is_valid(const QUEST_TEMPLATE *tpl)
{
   if (tpl == NULL)
      return FALSE;

   if (tpl->reward_obj_short == NULL || tpl->reward_obj_short[0] == '\0' ||
       tpl->reward_obj_name == NULL || tpl->reward_obj_name[0] == '\0' ||
       tpl->reward_obj_long == NULL || tpl->reward_obj_long[0] == '\0')
      return FALSE;

   if (tpl->reward_obj_wear_flags == 0)
      return FALSE;

   if (tpl->reward_obj_weight <= 0)
      return FALSE;

   return TRUE;
}

OBJ_DATA *create_quest_reward_object(CHAR_DATA *ch, const QUEST_TEMPLATE *tpl)
{
   OBJ_DATA *reward;
   int spawn_level;

   if (ch == NULL || !quest_reward_item_is_valid(tpl))
      return NULL;

   spawn_level = get_psuedo_level(ch);
   if (tpl->max_level > 0 && spawn_level > tpl->max_level)
      spawn_level = tpl->max_level;
   spawn_level = UMAX(1, spawn_level);

   reward = create_object(get_obj_index(OBJ_VNUM_MUSHROOM), 0);
   if (reward == NULL)
      return NULL;

   reward->item_type = ITEM_ARMOR;
   reward->level = spawn_level;
   reward->wear_flags = ITEM_TAKE | tpl->reward_obj_wear_flags;
   reward->extra_flags = tpl->reward_obj_extra_flags;
   reward->weight = tpl->reward_obj_weight;
   reward->item_apply = tpl->reward_obj_item_apply;

   free_string(reward->short_descr);
   reward->short_descr = str_dup(tpl->reward_obj_short);

   free_string(reward->name);
   reward->name = str_dup(tpl->reward_obj_name);

   free_string(reward->description);
   reward->description = str_dup(tpl->reward_obj_long);

   set_obj_stat_auto(reward);
   return reward;
}
