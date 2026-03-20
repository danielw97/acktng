/***************************************************************************
 * act_pub_society.c
 *
 * Public society commands, membership management, and core utility functions.
 * Individual combat skill implementations live in src/skills/do_*.c files
 * following the existing codebase pattern.
 ***************************************************************************/

#include "globals.h"
#include "magic.h"

/*
 * Helper: check if a player belongs to a specific society and has
 * sufficient rank for a given skill gsn.
 */
bool can_use_pub_society_skill(CHAR_DATA *ch, int gsn)
{
   int i;

   if (IS_NPC(ch))
      return FALSE;
   if (ch->pcdata->pub_society <= PUB_SOCIETY_NONE)
      return FALSE;

   int soc = ch->pcdata->pub_society;
   if (soc >= MAX_PUB_SOCIETY)
      return FALSE;
   for (i = 0; i < PUB_SOCIETY_SKILLS_PER; i++)
   {
      if (pub_society_table[soc].skill_gsns[i] == gsn)
         return ch->pcdata->pub_society_rank >= pub_society_table[soc].skill_thresholds[i];
   }
   return FALSE;
}

/*
 * Return the number of skills unlocked at a given rank score.
 */
int pub_society_skills_available(int society, int rank_score)
{
   int i, count = 0;
   if (society <= PUB_SOCIETY_NONE || society >= MAX_PUB_SOCIETY)
      return 0;
   for (i = 0; i < PUB_SOCIETY_SKILLS_PER; i++)
   {
      if (rank_score >= pub_society_table[society].skill_thresholds[i])
         count++;
   }
   return count;
}

/*
 * Return the rank title for a given society and rank score.
 */
const char *pub_society_rank_title(int society, int rank_score)
{
   if (society <= PUB_SOCIETY_NONE || society >= MAX_PUB_SOCIETY)
      return "Unknown";

   if (rank_score >= 400)
      return pub_society_table[society].rank_titles[5];
   if (rank_score >= 300)
      return pub_society_table[society].rank_titles[4];
   if (rank_score >= 200)
      return pub_society_table[society].rank_titles[3];
   if (rank_score >= 100)
      return pub_society_table[society].rank_titles[2];
   if (rank_score >= 1)
      return pub_society_table[society].rank_titles[1];
   return pub_society_table[society].rank_titles[0];
}

/*
 * Check if a player can join a given society.
 */
bool can_join_pub_society(CHAR_DATA *ch, int society)
{
   if (IS_NPC(ch))
      return FALSE;
   if (society <= PUB_SOCIETY_NONE || society >= MAX_PUB_SOCIETY)
      return FALSE;
   if (ch->pcdata->pub_society > PUB_SOCIETY_NONE)
      return FALSE;
   if (ch->level < pub_society_table[society].min_level)
      return FALSE;
   /* Check cooldown from leaving a previous society */
   if (ch->pcdata->pub_society_left > 0)
   {
      int elapsed = (int)current_time - ch->pcdata->pub_society_left;
      if (elapsed < PUB_SOCIETY_REJOIN_COOLDOWN_DAYS * 24 * 60 * 60)
         return FALSE;
   }
   return TRUE;
}

/*
 * do_enlist: Join a public society.
 * Syntax: enlist <society-name>
 * Must be standing in the society's hall near a recruiter NPC.
 */
void do_enlist(CHAR_DATA *ch, char *argument)
{
   char arg[MAX_INPUT_LENGTH];
   char buf[MAX_STRING_LENGTH];
   CHAR_DATA *recruiter = NULL;
   CHAR_DATA *mob;
   int society = PUB_SOCIETY_NONE;
   int i;

   if (IS_NPC(ch))
   {
      send_to_char("NPCs cannot join public societies.\n\r", ch);
      return;
   }

   one_argument(argument, arg);

   if (arg[0] == '\0')
   {
      send_to_char("Enlist in which society?\n\r", ch);
      send_to_char("Available societies:\n\r", ch);
      for (i = 1; i < MAX_PUB_SOCIETY; i++)
      {
         sprintf(buf, "  @@y%-30s@@N (@@W%s@@N, level %d+)\n\r", pub_society_table[i].name,
                 pub_society_table[i].base_city, pub_society_table[i].min_level);
         send_to_char(buf, ch);
      }
      return;
   }

   /* Match the argument to a society */
   for (i = 1; i < MAX_PUB_SOCIETY; i++)
   {
      if (!str_prefix(arg, pub_society_table[i].name) ||
          !str_prefix(arg, pub_society_table[i].abbr))
      {
         society = i;
         break;
      }
   }

   if (society == PUB_SOCIETY_NONE)
   {
      send_to_char("No such society. Type 'enlist' to see available options.\n\r", ch);
      return;
   }

   /* Check for a recruiter NPC in the room */
   for (mob = ch->in_room->first_person; mob != NULL; mob = mob->next_in_room)
   {
      if (IS_NPC(mob) && mob->pIndexData->pub_society == society &&
          mob->pIndexData->pub_society_npc_role == PUB_SOC_NPC_RECRUITER)
      {
         recruiter = mob;
         break;
      }
   }

   if (recruiter == NULL)
   {
      send_to_char("You need to find a recruiter for that society first.\n\r", ch);
      return;
   }

   if (ch->pcdata->pub_society > PUB_SOCIETY_NONE)
   {
      sprintf(buf, "You are already a member of %s. You must resign first.\n\r",
              pub_society_table[ch->pcdata->pub_society].name);
      send_to_char(buf, ch);
      return;
   }

   if (!can_join_pub_society(ch, society))
   {
      if (ch->level < pub_society_table[society].min_level)
      {
         sprintf(buf, "You must be at least level %d to join %s.\n\r",
                 pub_society_table[society].min_level, pub_society_table[society].name);
         send_to_char(buf, ch);
         return;
      }
      if (ch->pcdata->pub_society_left > 0)
      {
         int elapsed = (int)current_time - ch->pcdata->pub_society_left;
         int remaining = PUB_SOCIETY_REJOIN_COOLDOWN_DAYS * 24 * 60 * 60 - elapsed;
         int days = remaining / (24 * 60 * 60);
         sprintf(buf, "You must wait %d more day%s before joining a new society.\n\r", days + 1,
                 days > 0 ? "s" : "");
         send_to_char(buf, ch);
         return;
      }
      send_to_char("You cannot join that society right now.\n\r", ch);
      return;
   }

   /* Enroll the player */
   ch->pcdata->pub_society = society;
   ch->pcdata->pub_society_rank = 0;
   ch->pcdata->pub_society_tasks_done = 0;
   ch->pcdata->pub_society_joined = (int)current_time;

   sprintf(buf, "\n\r@@W%s@@N welcomes you to @@y%s@@N!\n\r", recruiter->short_descr,
           pub_society_table[society].name);
   send_to_char(buf, ch);
   sprintf(buf, "You hold the rank of @@W%s@@N.\n\r",
           pub_society_rank_title(society, ch->pcdata->pub_society_rank));
   send_to_char(buf, ch);

   sprintf(buf, "%s has joined %s.", ch->name, pub_society_table[society].name);
   act(buf, ch, NULL, NULL, TO_ROOM);

   save_char_obj(ch);
}

/*
 * do_society: Main society information command.
 * Syntax: society [info|who|resign]
 */
void do_society(CHAR_DATA *ch, char *argument)
{
   char arg[MAX_INPUT_LENGTH];
   char buf[MAX_STRING_LENGTH];
   int soc, i;

   if (IS_NPC(ch))
   {
      send_to_char("NPCs don't belong to societies.\n\r", ch);
      return;
   }

   one_argument(argument, arg);

   soc = ch->pcdata->pub_society;

   if (soc <= PUB_SOCIETY_NONE)
   {
      send_to_char("You are not a member of any public society.\n\r", ch);
      send_to_char("Use 'enlist' to join one.\n\r", ch);
      return;
   }

   if (arg[0] == '\0' || !str_prefix(arg, "info"))
   {
      sprintf(buf, "\n\r@@y%s@@N\n\r", pub_society_table[soc].name);
      send_to_char(buf, ch);
      sprintf(buf, "  Base City:  @@W%s@@N\n\r", pub_society_table[soc].base_city);
      send_to_char(buf, ch);
      sprintf(buf, "  Rank:       @@W%s@@N (score: %d)\n\r",
              pub_society_rank_title(soc, ch->pcdata->pub_society_rank),
              ch->pcdata->pub_society_rank);
      send_to_char(buf, ch);
      sprintf(buf, "  Tasks Done: %d\n\r", ch->pcdata->pub_society_tasks_done);
      send_to_char(buf, ch);

      send_to_char("  Skills:\n\r", ch);
      for (i = 0; i < PUB_SOCIETY_SKILLS_PER; i++)
      {
         int gsn_val = pub_society_table[soc].skill_gsns[i];
         int threshold = pub_society_table[soc].skill_thresholds[i];
         bool unlocked = ch->pcdata->pub_society_rank >= threshold;
         if (gsn_val > 0 && gsn_val < MAX_SKILL && skill_table[gsn_val].name != NULL)
         {
            sprintf(buf, "    %s%-20s@@N (rank %d+)%s\n\r", unlocked ? "@@W" : "@@d",
                    skill_table[gsn_val].name, threshold, unlocked ? "" : " @@d[locked]@@N");
            send_to_char(buf, ch);
         }
      }
      return;
   }

   if (!str_prefix(arg, "resign"))
   {
      pub_society_resign(ch);
      return;
   }

   if (!str_prefix(arg, "who"))
   {
      DESCRIPTOR_DATA *d;
      int count = 0;

      sprintf(buf, "\n\r@@yMembers of %s online:@@N\n\r", pub_society_table[soc].name);
      send_to_char(buf, ch);

      for (d = first_desc; d != NULL; d = d->next)
      {
         CHAR_DATA *wch;
         if (d->connected != CON_PLAYING)
            continue;
         wch = d->character;
         if (wch == NULL || IS_NPC(wch))
            continue;
         if (wch->pcdata->pub_society != soc)
            continue;
         sprintf(buf, "  @@W%-15s@@N %s\n\r", wch->name,
                 pub_society_rank_title(soc, wch->pcdata->pub_society_rank));
         send_to_char(buf, ch);
         count++;
      }

      if (count == 0)
         send_to_char("  No members online.\n\r", ch);
      else
      {
         sprintf(buf, "  %d member%s online.\n\r", count, count == 1 ? "" : "s");
         send_to_char(buf, ch);
      }
      return;
   }

   send_to_char("Society commands: info, who, resign\n\r", ch);
}

/*
 * Resign from current public society.
 */
void pub_society_resign(CHAR_DATA *ch)
{
   char buf[MAX_STRING_LENGTH];
   int soc;

   if (IS_NPC(ch) || ch->pcdata->pub_society <= PUB_SOCIETY_NONE)
   {
      send_to_char("You are not a member of any society.\n\r", ch);
      return;
   }

   soc = ch->pcdata->pub_society;

   sprintf(buf, "You resign from %s. All society skills are now unavailable.\n\r",
           pub_society_table[soc].name);
   send_to_char(buf, ch);

   ch->pcdata->pub_society = PUB_SOCIETY_NONE;
   ch->pcdata->pub_society_rank = 0;
   ch->pcdata->pub_society_tasks_done = 0;
   ch->pcdata->pub_society_left = (int)current_time;

   save_char_obj(ch);
}

/* -------------------------------------------------------------------
 * Passive combat hooks — called from fight.c, damage.c, update.c
 * ------------------------------------------------------------------- */

/*
 * Road Wardens: road_grit — +5% dodge chance.
 * Called from get_dodge() in fight.c.
 */
int pub_society_dodge_bonus(CHAR_DATA *ch)
{
   if (IS_NPC(ch))
      return 0;
   if (!can_use_pub_society_skill(ch, gsn_road_grit))
      return 0;
   return 5;
}

/*
 * Wall Command: bulwark — 5% physical damage reduction.
 * Called from calculate_damage() in damage.c.
 */
int pub_society_damage_reduction(CHAR_DATA *ch, int dam, int element)
{
   if (IS_NPC(ch))
      return dam;
   if (!IS_SET(element, ELE_PHYSICAL))
      return dam;
   if (!can_use_pub_society_skill(ch, gsn_bulwark))
      return dam;
   return dam - (dam * 5 / 100);
}

/*
 * Harbor Syndics: longshoreman — one extra autoattack chance.
 * Called from multi_hit() in fight.c.
 * Returns 1 if the bonus attack should be granted, 0 otherwise.
 */
int pub_society_extra_attack(CHAR_DATA *ch)
{
   if (IS_NPC(ch))
      return 0;
   if (!can_use_pub_society_skill(ch, gsn_longshoreman))
      return 0;
   /* Same probability roll as class-based extra attacks */
   if (number_percent() < 80)
      return 1;
   return 0;
}

/*
 * Ember Wardens: ember_vitality — 1% max_hit regen per combat round.
 * Called from violence_update() in update.c.
 */
int pub_society_hp_regen(CHAR_DATA *ch)
{
   if (IS_NPC(ch))
      return 0;
   if (!can_use_pub_society_skill(ch, gsn_ember_vitality))
      return 0;
   return ch->max_hit / 100;
}

/*
 * Shrine of the First Claw: marked_strike — evasion piercing bonus
 * and avoidance bypass against oath_marked targets.
 * Called from check_avoidance() in damage.c.
 * Returns TRUE if the attack should bypass avoidance entirely.
 */
bool pub_society_marked_strike_check(CHAR_DATA *ch, CHAR_DATA *victim)
{
   if (IS_NPC(ch))
      return FALSE;
   if (!can_use_pub_society_skill(ch, gsn_marked_strike))
      return FALSE;
   if (!is_affected(victim, gsn_oath_mark))
      return FALSE;
   /* 10% chance to bypass parry/dodge/block entirely */
   if (number_percent() < 10)
      return TRUE;
   return FALSE;
}
