/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *                                                                         *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  Ack 2.2 improvements copyright (C) 1994 by Stephen Dooley              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc       *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.                                               *
 *                                                                         *
 *       _/          _/_/_/     _/    _/     _/    ACK! MUD is modified    *
 *      _/_/        _/          _/  _/       _/    Merc2.0/2.1/2.2 code    *
 *     _/  _/      _/           _/_/         _/    (c)Stephen Zepp 1998    *
 *    _/_/_/_/      _/          _/  _/             Version #: 4.3          *
 *   _/      _/      _/_/_/     _/    _/     _/                            *
 *                                                                         *
 *                        http://ackmud.nuc.net/                           *
 *                        zenithar@ackmud.nuc.net                          *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "globals.h"

void set_obj_stat_auto(OBJ_DATA *obj);
void set_aff_to_obj(OBJ_DATA *obj, int location, int modifier);

/* Map obj->weight to a human-readable style name.
 * melee=8, caster=3, tank=13 (matches emblem system). */
static const char *claneq_weight_name(int obj_weight)
{
   if (obj_weight == CLANEQ_WEIGHT_CASTER)
      return "caster";
   if (obj_weight == CLANEQ_WEIGHT_TANK)
      return "tank";
   return "melee";
}

/* Create a clan equipment piece for a character. */
OBJ_DATA *create_clan_eq(CHAR_DATA *ch)
{
   char buf[MAX_STRING_LENGTH];
   char color[4];
   OBJ_DATA *obj;
   int clan_id;
   int level;

   if (ch == NULL || IS_NPC(ch))
      return NULL;

   clan_id = ch->pcdata->clan;
   if (clan_id < 1 || clan_id >= MAX_CLAN)
      return NULL;

   level = get_psuedo_level(ch);
   if (level < 1)
      level = 1;

   /* Extract the two-char color code (e.g. "@@y" -> "y") */
   {
      const char *abbr = clan_table[clan_id].clan_abbr;
      if (abbr[0] == '@' && abbr[1] == '@' && abbr[2] != '\0')
      {
         snprintf(color, sizeof(color), "@@%c", abbr[2]);
      }
      else
         snprintf(color, sizeof(color), "@@N");
   }

   obj = create_object(get_obj_index(OBJ_VNUM_MUSHROOM), 0);
   if (obj == NULL)
      return NULL;

   obj->item_type = ITEM_ARMOR;
   obj->level = level;
   obj->wear_flags = ITEM_TAKE | ITEM_WEAR_CLAN_COLORS;
   obj->weight = CLANEQ_WEIGHT_MELEE;
   obj->cost = 0;
   obj->obj_fun = obj_fun_lookup("objfun_clan");

   /* Set flags: NODROP, NOLOOT, CLAN_EQ, NOSAC, NOREMOVE */
   REMOVE_BIT(obj->extra_flags, ITEM_NOSAVE);
   REMOVE_BIT(obj->extra_flags, ITEM_BIND_EQUIP);
   SET_BIT(obj->extra_flags, ITEM_NODROP);
   SET_BIT(obj->extra_flags, ITEM_NOLOOT);
   SET_BIT(obj->extra_flags, ITEM_CLAN_EQ);
   SET_BIT(obj->extra_flags, ITEM_NOSAC);

   /* Name / descriptions themed to the clan */
   snprintf(buf, sizeof(buf), "clan colors %s", clan_table[clan_id].clan_name);
   free_string(obj->name);
   obj->name = str_dup(buf);

   snprintf(buf, sizeof(buf), "%sthe colors of %s@@N", color, clan_table[clan_id].clan_name);
   free_string(obj->short_descr);
   obj->short_descr = str_dup(buf);

   snprintf(buf, sizeof(buf), "%sThe colors of %s@@N lie here.", color,
            clan_table[clan_id].clan_name);
   free_string(obj->description);
   obj->description = str_dup(buf);

   set_obj_stat_auto(obj);

   return obj;
}

/* Adjust an existing clan eq piece to match the owner's current level.
 * The object's weight (stat focus) is preserved. */
void claneq_adjust(OBJ_DATA *obj, CHAR_DATA *ch)
{
   AFFECT_DATA *paf;
   AFFECT_DATA *paf_next;
   int level;

   if (obj == NULL || ch == NULL || IS_NPC(ch))
      return;

   level = get_psuedo_level(ch);
   if (level < 1)
      level = 1;

   obj->level = level;

   /* Strip existing affects before recalculating */
   for (paf = obj->first_apply; paf != NULL; paf = paf_next)
   {
      paf_next = paf->next;
      UNLINK(paf, obj->first_apply, obj->last_apply, next, prev);
      PUT_FREE(paf, affect_free);
   }
   obj->first_apply = NULL;
   obj->last_apply = NULL;

   set_obj_stat_auto(obj);
}

/* Find the character's clan eq (worn or carried). */
OBJ_DATA *find_clan_eq(CHAR_DATA *ch)
{
   OBJ_DATA *obj;

   if (ch == NULL)
      return NULL;

   for (obj = ch->first_carry; obj != NULL; obj = obj->next_in_carry_list)
   {
      if (IS_SET(obj->extra_flags, ITEM_CLAN_EQ) && IS_SET(obj->wear_flags, ITEM_WEAR_CLAN_COLORS))
         return obj;
   }
   return NULL;
}

/* Destroy all clan eq on a character. */
void destroy_clan_eq(CHAR_DATA *ch)
{
   OBJ_DATA *obj;
   OBJ_DATA *obj_next;

   if (ch == NULL)
      return;

   for (obj = ch->first_carry; obj != NULL; obj = obj_next)
   {
      obj_next = obj->next_in_carry_list;
      if (IS_SET(obj->extra_flags, ITEM_CLAN_EQ) && IS_SET(obj->wear_flags, ITEM_WEAR_CLAN_COLORS))
      {
         if (obj->wear_loc != WEAR_NONE)
            unequip_char(ch, obj);
         act("$p shatters into dust!", ch, obj, NULL, TO_CHAR);
         act("$p carried by $n shatters into dust!", ch, obj, NULL, TO_ROOM);
         extract_obj(obj);
      }
   }
}

/* Player command: claneq <melee|caster|tank> */
void do_claneq(CHAR_DATA *ch, char *argument)
{
   char buf[MAX_STRING_LENGTH];
   OBJ_DATA *obj;
   int new_weight;

   if (IS_NPC(ch))
      return;

   if (ch->pcdata->clan == 0)
   {
      send_to_char("You are not in a clan.\n\r", ch);
      return;
   }

   obj = find_clan_eq(ch);

   if (argument[0] == '\0')
   {
      const char *current = obj != NULL ? claneq_weight_name(obj->weight) : "melee";
      snprintf(buf, sizeof(buf),
               "Your clan equipment is currently set to: @@W%s@@N.\n\r"
               "Syntax: claneq <melee|caster|tank>\n\r",
               current);
      send_to_char(buf, ch);
      return;
   }

   if (!str_prefix(argument, "melee"))
      new_weight = CLANEQ_WEIGHT_MELEE;
   else if (!str_prefix(argument, "caster"))
      new_weight = CLANEQ_WEIGHT_CASTER;
   else if (!str_prefix(argument, "tank"))
      new_weight = CLANEQ_WEIGHT_TANK;
   else
   {
      send_to_char("Valid options: melee, caster, tank.\n\r", ch);
      return;
   }

   if (obj == NULL)
   {
      send_to_char("You don't have your clan colors.\n\r", ch);
      return;
   }

   {
      bool was_worn = (obj->wear_loc != WEAR_NONE);

      if (was_worn)
         unequip_char(ch, obj);

      obj->weight = new_weight;
      claneq_adjust(obj, ch);

      if (was_worn)
         equip_char(ch, obj, WEAR_CLAN_COLORS);

      snprintf(buf, sizeof(buf), "Your clan colors shimmer and reform with a %s focus!\n\r",
               claneq_weight_name(new_weight));
      send_to_char(buf, ch);
   }
}

/* These set of clan functions by Zen */

void save_clan_table()
{

   FILE *fp;
   char clan_file_name[MAX_STRING_LENGTH];
   sh_int x, y;

   if (fpReserve != NULL)
   {
      fclose(fpReserve);
      fpReserve = NULL;
   }

   sprintf(clan_file_name, "%s", CLAN_FILE);

   if ((fp = fopen(clan_file_name, "w")) == NULL)
   {
      bug("Save Clan Table: fopen", 0);
      perror("failed open of clan_table.dat in save_clan_table");
   }
   else
   {
      fprintf(fp, "%d\n", MAX_CLAN);

      for (x = 1; x < MAX_CLAN; x++)
      {
         for (y = 1; y < MAX_CLAN; y++)
         {
            fprintf(fp, "  %5d  ", politics_data.diplomacy[x][y]);
         }

         fprintf(fp, "\n");
      }

      for (x = 1; x < MAX_CLAN; x++)
      {
         fprintf(fp, "%li\n", politics_data.treasury[x]);
      }

      for (x = 1; x < MAX_CLAN; x++)
      {
         for (y = 1; y < MAX_CLAN; y++)
         {
            fprintf(fp, "  %5d  ", politics_data.end_current_state[x][y]);
         }

         fprintf(fp, "\n");
      }

      fflush(fp);
      if (fp != NULL)
      {
         fclose(fp);
         fp = NULL;
      }
   }
   return;
}

void do_ctoggle(CHAR_DATA *ch, char *argument)
{
   /*
    * Toggles clan members various clan job flags
    * * Zen
    */

   CHAR_DATA *victim;
   int job;

   char arg1[MAX_STRING_LENGTH];
   char arg2[MAX_STRING_LENGTH];

   argument = one_argument(argument, arg1);
   argument = one_argument(argument, arg2);

   if (IS_NPC(ch) || !IS_SET(ch->pcdata->pflags, PFLAG_CLAN_BOSS))
   {
      send_to_char("This command is for clan bosses only.\n\r", ch);
      return;
   }

   if (arg1[0] == '\0' || arg2[0] == '\0')
   {
      send_to_char("Syntax: ctoggle <clan number> <armourer/diplomat/treasurer/leader>\n\r", ch);
      return;
   }

   if ((victim = get_char_world(ch, arg1)) == NULL)
   {
      send_to_char("They aren't here.\n\r", ch);
      return;
   }

   if (IS_NPC(victim))
   {
      send_to_char("Not on NPCs!\n\r", ch);
      return;
   }

   if (ch->pcdata->clan != victim->pcdata->clan)
   {
      send_to_char("Only on members of YOUR clan!\n\r", ch);
      return;
   }

   if (!str_prefix(arg2, "armourer"))
      job = PFLAG_CLAN_ARMOURER;
   else if (!str_prefix(arg2, "diplomat"))
      job = PFLAG_CLAN_DIPLOMAT;
   else if (!str_prefix(arg2, "treasurer"))
      job = PFLAG_CLAN_TREASURER;
   else if (!str_prefix(arg2, "leader"))
      job = PFLAG_CLAN_LEADER;

   else
   {
      send_to_char("That's not a legal clan job!!\n\r", ch);
      return;
   }

   switch (job)
   {
   case PFLAG_CLAN_TREASURER:

      if (!IS_SET(victim->pcdata->pflags, PFLAG_CLAN_TREASURER))
      {
         SET_BIT(victim->pcdata->pflags, PFLAG_CLAN_TREASURER);
         send_to_char("Job set as CLAN TREASURER.\n\r", ch);
         send_to_char("You have been set as a clan treasurer.\n\r", victim);
      }
      else
      {
         REMOVE_BIT(victim->pcdata->pflags, PFLAG_CLAN_TREASURER);
         send_to_char("Job cleared: CLAN TREASURER.\n\r", ch);
         send_to_char("You are no longer a clan treasurer.\n\r", victim);
      }
      break;

   case PFLAG_CLAN_DIPLOMAT:

      if (!IS_SET(victim->pcdata->pflags, PFLAG_CLAN_DIPLOMAT))
      {
         SET_BIT(victim->pcdata->pflags, PFLAG_CLAN_DIPLOMAT);
         send_to_char("Job set as CLAN DIPLOMAT.\n\r", ch);
         send_to_char("You have been set as a clan diplomat.\n\r", victim);
      }
      else
      {
         REMOVE_BIT(victim->pcdata->pflags, PFLAG_CLAN_DIPLOMAT);
         send_to_char("Job cleared: CLAN DIPLOMAT.\n\r", ch);
         send_to_char("You are no longer a clan diplomat.\n\r", victim);
      }
      break;

   case PFLAG_CLAN_ARMOURER:

      if (!IS_SET(victim->pcdata->pflags, PFLAG_CLAN_ARMOURER))
      {
         SET_BIT(victim->pcdata->pflags, PFLAG_CLAN_ARMOURER);
         send_to_char("Job set as CLAN ARMOURER.\n\r", ch);
         send_to_char("You have been set as a clan armourer.\n\r", victim);
      }
      else
      {
         REMOVE_BIT(victim->pcdata->pflags, PFLAG_CLAN_ARMOURER);
         send_to_char("Job cleared: CLAN ARMOURER.\n\r", ch);
         send_to_char("You are no longer a clan armourer.\n\r", victim);
      }
      break;

   case PFLAG_CLAN_LEADER:

      if (!IS_SET(victim->pcdata->pflags, PFLAG_CLAN_LEADER))
      {
         SET_BIT(victim->pcdata->pflags, PFLAG_CLAN_LEADER);
         send_to_char("Job set as CLAN LEADER.\n\r", ch);
         send_to_char("You have been set as a clan leader.\n\r", victim);
      }
      else
      {
         REMOVE_BIT(victim->pcdata->pflags, PFLAG_CLAN_LEADER);
         send_to_char("Job cleared: CLAN LEADER.\n\r", ch);
         send_to_char("You are no longer a clan leader.\n\r", victim);
      }
      break;
   } /* end of switch( job )  */

   return;
}

char *get_diplo_name(sh_int value)
{

   char *name = '\0';

   if (value < -450)
      name = "@@R  WAR  @@N";
   else if (value < -300)
      name = "@@e HATRED@@N";
   else if (value < -150)
      name = "@@dDISLIKE@@N";
   else if (value < 150)
      name = "@@WNEUTRAL@@N";
   else if (value < 300)
      name = "@@aRESPECT@@N";
   else if (value < 450)
      name = "@@l TRUST @@N";
   else
      name = "@@B ALLY  @@N";

   return (name);
}

void do_politics(CHAR_DATA *ch, char *argument)
{
   sh_int x, y;
   char buf[MAX_STRING_LENGTH], buf2[MAX_STRING_LENGTH];

   if (IS_NPC(ch))
   {
      send_to_char("Not for Npcs.\n\r", ch);
      return;
   }

   send_to_char("@@NCurrent Politics of " mudnamecolor "\n\r\n\r", ch);
   buf[0] = '\0';
   buf2[0] = '\0';

   sprintf(buf, "        ");
   safe_strcat(MAX_STRING_LENGTH, buf2, buf);

   for (x = 1; x < MAX_CLAN; x++)
   {
      sprintf(buf, " %s  ", clan_table[x].clan_abbr);
      safe_strcat(MAX_STRING_LENGTH, buf2, buf);
   }
   buf[0] = '\0';
   sprintf(buf, "\n\r\n\r");
   safe_strcat(MAX_STRING_LENGTH, buf2, buf);

   send_to_char(buf2, ch);

   for (x = 1; x < MAX_CLAN; x++)
   {
      /*
       * clan symbol here
       */
      buf[0] = '\0';
      buf2[0] = '\0';
      sprintf(buf, "%1i %s ", x, clan_table[x].clan_abbr);
      safe_strcat(MAX_STRING_LENGTH, buf2, buf);

      for (y = 1; y < MAX_CLAN; y++)
      {
         buf[0] = '\0';
         if (x != y)
         {
            sprintf(buf, "%s ", get_diplo_name(politics_data.diplomacy[x][y]));
            safe_strcat(MAX_STRING_LENGTH, buf2, buf);
         }
         else
         {
            sprintf(buf, "        ");
            safe_strcat(MAX_STRING_LENGTH, buf2, buf);
         }
      }
      sprintf(buf, "\n\r\n\r");
      safe_strcat(MAX_STRING_LENGTH, buf2, buf);
      send_to_char(buf2, ch);
   }
   if (IS_SET(ch->pcdata->pflags, PFLAG_CLAN_DIPLOMAT))
      for (x = 1; x < MAX_CLAN; x++)
      {
         if (politics_data.end_current_state[ch->pcdata->clan][x])
         {
            sprintf(buf, "%s has requested an end to your current state of affairs",
                    clan_table[x].clan_name);
            send_to_char(buf, ch);
         }
      }
}

void do_negotiate(CHAR_DATA *ch, char *argument)
{
   char buf[MAX_STRING_LENGTH];
   char arg1[MAX_STRING_LENGTH];
   char arg2[MAX_STRING_LENGTH];
   sh_int target_clan;

   argument = one_argument(argument, arg1);
   argument = one_argument(argument, arg2);

   if (IS_NPC(ch) || !IS_SET(ch->pcdata->pflags, PFLAG_CLAN_DIPLOMAT))
   {
      send_to_char("This command is for clan diplomats only.\n\r", ch);
      return;
   }

   if (!is_number(arg1) || arg2[0] == '\0')
   {
      send_to_char("Syntax: Negotiate <clan number> <peace/war/end>\n\r", ch);
      return;
   }

   target_clan = atoi(arg1);
   if ((target_clan < 1) || (target_clan > (MAX_CLAN - 1)))
   {
      send_to_char("That is not a clan!\n\r", ch);
      return;
   }
   if (target_clan == ch->pcdata->clan)
   {
      send_to_char(
          "Well, that will accomplish a lot..you must be of two minds about the whole thing!\n\r",
          ch);
      return;
   }

   if (politics_data.daily_negotiate_table[ch->pcdata->clan][target_clan])
   {
      sprintf(buf, "Your clan has already negotiated with %s today.\n\r",
              clan_table[target_clan].clan_name);
      send_to_char(buf, ch);
      return;
   }

   if (!str_prefix(arg2, "peace"))
   {
      if (politics_data.diplomacy[ch->pcdata->clan][target_clan] < -450)
      {
         if (politics_data.end_current_state[ch->pcdata->clan][target_clan] &&
             politics_data.end_current_state[target_clan][ch->pcdata->clan])
         {
            politics_data.diplomacy[ch->pcdata->clan][target_clan] = -425;
            politics_data.diplomacy[target_clan][ch->pcdata->clan] = -425;
            send_to_char("@@NYou have successfully negotiated an end to this dreaded @@eCLAN "
                         "WAR@@N. Great Job!!\n\r",
                         ch);
            sprintf(buf,
                    "@@eCLAN:@@N The war between %s and %s has ended. They may no longer PKILL "
                    "each other!!\n\r",
                    clan_table[ch->pcdata->clan].clan_name, clan_table[target_clan].clan_name);
            info(buf, 1);

            politics_data.end_current_state[ch->pcdata->clan][target_clan] = FALSE;
            politics_data.end_current_state[target_clan][ch->pcdata->clan] = FALSE;

            return;
         }
         else
         {
            sprintf(buf,
                    "@@NYou are currently at @@RWAR@@N with %s. Both clans must negotiate an end "
                    "to the war first.\n\r",
                    clan_table[target_clan].clan_name);
            send_to_char(buf, ch);
            return;
         }
      }
      else
      {
         politics_data.diplomacy[ch->pcdata->clan][target_clan] += 25;
         politics_data.diplomacy[target_clan][ch->pcdata->clan] += 25;

         sprintf(buf, "You are requesting a more peaceful state of affairs with %s.",
                 clan_table[target_clan].clan_name);
         send_to_char(buf, ch);
      }
   }

   else if (!str_prefix(arg2, "war"))
   {
      if ((politics_data.diplomacy[ch->pcdata->clan][target_clan] - 50) < -450)

      {
         sprintf(buf, "@@NYou have started a @@eWAR@@N with %s! Watch out!.\n\r",
                 clan_table[target_clan].clan_name);
         send_to_char(buf, ch);

         sprintf(
             buf,
             "@@eCLAN:@@N A war has started between %s and %s. They may now PKILL each other!!\n\r",
             clan_table[ch->pcdata->clan].clan_name, clan_table[target_clan].clan_name);
         info(buf, 1);

         politics_data.diplomacy[ch->pcdata->clan][target_clan] = -460;
         politics_data.diplomacy[target_clan][ch->pcdata->clan] = -460;
         politics_data.end_current_state[ch->pcdata->clan][target_clan] = FALSE;
         politics_data.end_current_state[target_clan][ch->pcdata->clan] = FALSE;
         save_clan_table();
         return;
      }

      else
      {
         politics_data.diplomacy[ch->pcdata->clan][target_clan] -= 50;
         politics_data.diplomacy[target_clan][ch->pcdata->clan] -= 50;

         sprintf(buf, "You are requesting a more aggressive state of affairs with %s.",
                 clan_table[target_clan].clan_name);
         send_to_char(buf, ch);
      }
   }
   else if (!str_prefix(arg2, "end"))
   {
      if (politics_data.diplomacy[ch->pcdata->clan][target_clan] > 450)
      {
         send_to_char("You are negotiating an and to your alliance.\n\r", ch);
         politics_data.end_current_state[ch->pcdata->clan][target_clan] = FALSE;
         politics_data.end_current_state[target_clan][ch->pcdata->clan] = FALSE;
         politics_data.diplomacy[ch->pcdata->clan][target_clan] = 100;
         politics_data.diplomacy[target_clan][ch->pcdata->clan] = 100;
      }
      if (politics_data.diplomacy[ch->pcdata->clan][target_clan] < -450)
      {
         politics_data.end_current_state[ch->pcdata->clan][target_clan] = TRUE;

         if (politics_data.end_current_state[ch->pcdata->clan][target_clan] &&
             politics_data.end_current_state[target_clan][ch->pcdata->clan])
         {
            send_to_char("Both clans have successfully negotiated and end to the war.  Negotiate "
                         "peace to seal your treaty!\n\r",
                         ch);
            save_clan_table();
            return;
         }
         else
         {
            send_to_char(" You have requested an end to this dreaded war, but the other clan has "
                         "not yet agreed.\n\r",
                         ch);
            save_clan_table();
            return;
         }
      }
      else
      {
         send_to_char(
             "You must be either at war or in an alliance with a clan before you can END it.\n\r",
             ch);
         return;
      }
   }
   else
   {
      send_to_char("That is not a legal diplomatic negotiation!\n\r", ch);
      return;
   }
   politics_data.daily_negotiate_table[ch->pcdata->clan][target_clan] = TRUE;

   /* save table to file here */
   save_clan_table();
   return;
}

/* These set of clan functions by [ -S- ] Stephen */

void do_leav(CHAR_DATA *ch, char *argument)
{
   send_to_char("If you want to LEAVE, spell it out!!\n\r", ch);
   return;
}

/* CSET is an immortal command, used to set a players clan.
 * it is also used by other functions.
 */

void do_cset(CHAR_DATA *ch, char *argument)
{
   /*
    * This is basically do_mset, but with its guts ripped out,
    * and a slight change to what was left.
    * -- Stephen
    */

   char arg1[MAX_INPUT_LENGTH];
   char arg2[MAX_INPUT_LENGTH];
   char buf[MAX_STRING_LENGTH];
   CHAR_DATA *victim;
   int value;

   smash_tilde(argument);
   argument = one_argument(argument, arg1);
   strcpy(arg2, argument);
   if (IS_NPC(ch))
      return;

   if (arg1[0] == '\0' || arg2[0] == '\0')
   {
      send_to_char("Syntax: cset <player> <clan No.>\n\r\n\r", ch);
      do_clan_list(ch, "");
      return;
   }

   if ((victim = get_char_world(ch, arg1)) == NULL)
   {
      send_to_char("They aren't here.\n\r", ch);
      return;
   }

   value = is_number(arg2) ? atoi(arg2) : -1;

   if (value == -1)
   {
      send_to_char("Syntax: cset <player> <clan No.>\n\r\n\r", ch);
      do_clan_list(ch, "");
      return;
   }

   if (IS_NPC(victim))
   {
      send_to_char("Not on NPC's.\n\r", ch);
      return;
   }

   if (value < 0 || value >= MAX_CLAN)
   {
      sprintf(buf, "%d is not a valid value.\n\r", value);
      send_to_char(buf, ch);
      sprintf(buf, "Use a value between 0 and %d.\n\r\n\r", MAX_CLAN - 1);
      send_to_char(buf, ch);
      do_clan_list(ch, "");
      return;
   }

   victim->pcdata->clan = value;
   sprintf(buf, "%s now belongs to clan %s.\n\r", victim->name, clan_table[value].clan_name);
   send_to_char(buf, ch);

   sprintf(buf, "%s has cset %s into clan %s.", ch->name, victim->name,
           clan_table[value].clan_name);
   monitor_chan(buf, MONITOR_CLAN);

   return;
}

/* accept is a command available to players set as clan_leader.
 * after various checks, the victim's clan is set to the leader's.
 * note that the victim must NOT be in a clan to be accepted.
 */

void do_accept(CHAR_DATA *ch, char *argument)
{
   char buf[MAX_STRING_LENGTH];
   CHAR_DATA *victim;

   if (IS_NPC(ch))
      return;

   if (!IS_SET(ch->pcdata->pflags, PFLAG_CLAN_LEADER))
   {
      send_to_char("You must be a clan leader to use this command!\n\r", ch);
      return;
   }

   if ((victim = get_char_world(ch, argument)) == NULL)
   {
      send_to_char("They aren't here.\n\r", ch);
      return;
   }

   if (IS_NPC(victim))
   {
      send_to_char("Not on NPCs!\n\r", ch);
      return;
   }

   if (IS_IMMORTAL(victim))
   {
      send_to_char("You hear the gods laughing at you.  Nice try.\n\r", ch);
      return;
   }

   if (victim == ch)
   {
      send_to_char("You want to accept yourself... Strange.\n\r", ch);
      return;
   }

   if (victim->pcdata->clan != 0)
   {
      sprintf(buf, "%s is already in a clan.  Maybe they should leave it first?\n\r", victim->name);
      send_to_char(buf, ch);
      return;
   }

   if (victim->level < 20)
   {
      sprintf(buf, "%s must be at least 20th level to enter a clan.\n\r", victim->name);
      send_to_char(buf, ch);
      return;
   }
   victim->pcdata->clan = ch->pcdata->clan;
   act("$N accepts you into $S clan!", victim, NULL, ch, TO_VICT);
   act("You accept $N into your clan!", ch, NULL, victim, TO_VICT);

   /* Grant clan equipment */
   {
      OBJ_DATA *claneq = create_clan_eq(victim);
      if (claneq != NULL)
      {
         obj_to_char(claneq, victim);
         act("$p materializes before you, bearing the colors of your new clan!", victim, claneq,
             NULL, TO_CHAR);
         act("$p materializes before $n!", victim, claneq, NULL, TO_ROOM);
      }
   }

   sprintf(buf, "%s has accepted %s into clan %s.", ch->name, victim->name,
           clan_table[ch->pcdata->clan].clan_name);
   monitor_chan(buf, MONITOR_CLAN);

   return;
}

/* cwhere is an adapted copy of the do_where command, except it only shows
 * clan members, and operates globally.
 */

void do_cwhere(CHAR_DATA *ch, char *argument)
{

   char buf[MAX_STRING_LENGTH];
   CHAR_DATA *victim;
   DESCRIPTOR_DATA *d;
   bool found;
   if (IS_NPC(ch))
      return;

   if (ch->pcdata->clan == 0)
   {
      send_to_char("You don't belong to a clan!\n\r", ch);
      return;
   }

   send_to_char("Clan members visible to you:\n\r", ch);
   found = FALSE;
   for (d = first_desc; d != NULL; d = d->next)
   {
      if (d->connected == CON_PLAYING && (victim = d->character) != NULL && !IS_NPC(victim) &&
          victim->in_room != NULL &&
          (IS_SET(ch->pcdata->pflags, PFLAG_CLAN_BOSS) || can_see(ch, victim)))
      {
         if (victim->pcdata->clan != ch->pcdata->clan)
            continue;

         found = TRUE;
         sprintf(buf, "%-28s %s ", victim->name, victim->in_room->name);
         if (IS_SET(ch->pcdata->pflags, PFLAG_CLAN_BOSS))
         {
            if (IS_SET(victim->pcdata->pflags, PFLAG_CLAN_LEADER))
               safe_strcat(MAX_STRING_LENGTH, buf, " L ");

            if (IS_SET(victim->pcdata->pflags, PFLAG_CLAN_ARMOURER))
               safe_strcat(MAX_STRING_LENGTH, buf, " A ");

            if (IS_SET(victim->pcdata->pflags, PFLAG_CLAN_TREASURER))
               safe_strcat(MAX_STRING_LENGTH, buf, " T ");

            if (IS_SET(victim->pcdata->pflags, PFLAG_CLAN_DIPLOMAT))
               safe_strcat(MAX_STRING_LENGTH, buf, " D ");
         }
         safe_strcat(MAX_STRING_LENGTH, buf, "\n\r");

         send_to_char(buf, ch);
      }
   }

   if (!found)
      send_to_char("No other clan members were found.\n\r", ch);

   return;
}

/* leave allows a (non-leader) clan member to leave his/her clan.
 * their clan is set to 0.  Again, use do_cset to do the actual
 * stuff, and to generate the usage 'text'.  ??? ummm ???
 */

void do_leave(CHAR_DATA *ch, char *argument)
{
   char buf[MAX_STRING_LENGTH];

   if (IS_NPC(ch))
      return;

   if (ch->pcdata->clan == 0) /* No clan */
   {
      send_to_char("You must be IN a clan before you can leave it!\n\r", ch);
      return;
   }

   sprintf(buf, "%s has left clan %s.", ch->name, clan_table[ch->pcdata->clan].clan_name);
   monitor_chan(buf, MONITOR_CLAN);

   destroy_clan_eq(ch);

   ch->pcdata->clan = 0;
   send_to_char("You leave your clan.  Let's hope they don't get mad!\n\r", ch);
   if (IS_SET(ch->pcdata->pflags, PFLAG_CLAN_DIPLOMAT))
      REMOVE_BIT(ch->pcdata->pflags, PFLAG_CLAN_DIPLOMAT);

   if (IS_SET(ch->pcdata->pflags, PFLAG_CLAN_TREASURER))
      REMOVE_BIT(ch->pcdata->pflags, PFLAG_CLAN_TREASURER);

   if (IS_SET(ch->pcdata->pflags, PFLAG_CLAN_ARMOURER))
      REMOVE_BIT(ch->pcdata->pflags, PFLAG_CLAN_ARMOURER);

   if (IS_SET(ch->pcdata->pflags, PFLAG_CLAN_LEADER))
      REMOVE_BIT(ch->pcdata->pflags, PFLAG_CLAN_LEADER);

   if (IS_SET(ch->pcdata->pflags, PFLAG_CLAN_BOSS))
      REMOVE_BIT(ch->pcdata->pflags, PFLAG_CLAN_BOSS);

   return;
}

void do_banish(CHAR_DATA *ch, char *argument)
{
   CHAR_DATA *victim;
   char buf[MAX_STRING_LENGTH];

   if (IS_NPC(ch))
      return;

   if (!IS_SET(ch->pcdata->pflags, PFLAG_CLAN_LEADER))
   {
      send_to_char("Only Clan Leaders may use this command.\n\r", ch);
      return;
   }

   if (argument[0] == '\0')
   {
      send_to_char("Banish WHO from your clan?\n\r", ch);
      return;
   }

   if ((victim = get_char_world(ch, argument)) == NULL)
   {
      send_to_char("No such Person.\n\r", ch);
      return;
   }

   if (victim == ch)
   {
      send_to_char("Dumb Idea!\n\r", ch);
      return;
   }

   if (IS_NPC(victim))
   {
      send_to_char("You can't banish a NPC!\n\r", ch);
      return;
   }

   if (victim->pcdata->clan != ch->pcdata->clan)
   {
      send_to_char("They're not in your clan!\n\r", ch);
      return;
   }

   if (IS_SET(victim->pcdata->pflags, PFLAG_CLAN_BOSS))
   {
      send_to_char("Nice Try.", ch);
      return;
   }

   destroy_clan_eq(victim);

   victim->pcdata->clan = 0;

   if (IS_SET(victim->pcdata->pflags, PFLAG_CLAN_DIPLOMAT))
      REMOVE_BIT(victim->pcdata->pflags, PFLAG_CLAN_DIPLOMAT);

   if (IS_SET(victim->pcdata->pflags, PFLAG_CLAN_TREASURER))
      REMOVE_BIT(victim->pcdata->pflags, PFLAG_CLAN_TREASURER);

   if (IS_SET(victim->pcdata->pflags, PFLAG_CLAN_ARMOURER))
      REMOVE_BIT(victim->pcdata->pflags, PFLAG_CLAN_ARMOURER);

   if (IS_SET(victim->pcdata->pflags, PFLAG_CLAN_LEADER))
      REMOVE_BIT(victim->pcdata->pflags, PFLAG_CLAN_LEADER);

   if (IS_SET(victim->pcdata->pflags, PFLAG_CLAN_BOSS))
      REMOVE_BIT(victim->pcdata->pflags, PFLAG_CLAN_BOSS);

   sprintf(buf, "%s has banished %s from clan %s.", ch->name, victim->name,
           clan_table[ch->pcdata->clan].clan_name);
   monitor_chan(buf, MONITOR_CLAN);

   sprintf(buf, "%s banishes you from clan %s!\n\r", ch->name,
           clan_table[ch->pcdata->clan].clan_name);
   send_to_char(buf, victim);
   act("$N has been banished.", ch, NULL, victim, TO_CHAR);
   return;
}

void do_make(CHAR_DATA *ch, char *argument)
{
   char buf[MAX_STRING_LENGTH];
   char arg1[MAX_STRING_LENGTH];
   char arg2[MAX_STRING_LENGTH];
   OBJ_INDEX_DATA *pObj;
   OBJ_DATA *obj;
   int cnt;
   int num;
   CHAR_DATA *target;

   argument = one_argument(argument, arg1);
   argument = one_argument(argument, arg2);

   if (IS_NPC(ch) || !IS_SET(ch->pcdata->pflags, PFLAG_CLAN_ARMOURER))
   {
      send_to_char("This command is for clan armourers only.\n\r", ch);
      return;
   }

   if (!is_number(arg1) || arg2[0] == '\0')
   {
      sprintf(buf, "Syntax: Make [0-%d] [Target]\n\r", MAX_CLAN_EQ - 1);
      send_to_char(buf, ch);
      send_to_char("Items you can currently make:\n\r\n\r", ch);

      for (cnt = 0; cnt < MAX_CLAN_EQ; cnt++)
      {
         if (clan_table[ch->pcdata->clan].eq[cnt] != -1 &&
             ((pObj = get_obj_index(clan_table[ch->pcdata->clan].eq[cnt])) != NULL))
         {
            sprintf(buf, "[%2d] : %s.\n\r", cnt, pObj->short_descr);
            send_to_char(buf, ch);
         }
         else
         {
            sprintf(buf, "[%2d] : No Object Set.\n\r", cnt);
            send_to_char(buf, ch);
         }
      }
      return;
   }

   target = get_char_room(ch, arg2);
   if (target == NULL)
   {
      send_to_char("No one with that name is here with you.\n\r", ch);
      return;
   }

   if (IS_NPC(target))
   {
      send_to_char("NOT on NPCs!\n\r", ch);
      return;
   }

   if (target->pcdata->clan != ch->pcdata->clan)
   {
      act("$N isn't in your clan...", ch, NULL, target, TO_CHAR);
      return;
   }

   num = atoi(arg1);
   if (num < 0 || num >= MAX_CLAN_EQ)
   {
      do_make(ch, "");
      return;
   }

   if (clan_table[ch->pcdata->clan].eq[num] == -1)
   {
      send_to_char("No object has been set for that slot.\n\r", ch);
      do_make(ch, "");
      return;
   }

   if ((pObj = get_obj_index(clan_table[ch->pcdata->clan].eq[num])) == NULL)
   {
      send_to_char("Couldn't find that object to load...\n\r", ch);
      do_make(ch, "");
      return;
   }

   if (!IS_SET(pObj->extra_flags, ITEM_CLAN_EQ))
   {
      send_to_char("Object exists, but not flagged as Clan Eq.\n\r", ch);
      return;
   }

   if (ch != target)
   {
      obj = create_object(pObj, target->level);
      act("$n creates $p, and hands it to $N.", ch, obj, target, TO_NOTVICT);
      act("You create $p, and hand it to $N.", ch, obj, target, TO_CHAR);
      act("$N creates $p, and hands it to you.", target, obj, ch, TO_CHAR);
      obj_to_char(obj, target);
      sprintf(buf, "%s has made %s for %s.", ch->name, obj->short_descr, target->name);
   }
   else
   {
      obj = create_object(pObj, ch->level);
      act("You create $p, and put it away.", ch, obj, NULL, TO_CHAR);
      act("$n creates $p, and puts it away.", ch, obj, NULL, TO_ROOM);
      obj_to_char(obj, ch);
      sprintf(buf, "%s has made themself %s.", ch->name, obj->short_descr);
   }
   monitor_chan(buf, MONITOR_CLAN);

   return;
}

void do_council(CHAR_DATA *ch, char *argument)
{
   char buf[MAX_STRING_LENGTH];
   char buf2[MAX_STRING_LENGTH];
   char arg1[MAX_STRING_LENGTH];
   char arg2[MAX_STRING_LENGTH];
   MEMBER_DATA *imember;
   MEMBER_DATA *member;
   sh_int this_council = SUPER_NONE;
   CHAR_DATA *victim;

   if ((IS_NPC(ch)) || (!IS_SET(ch->pcdata->pflags, PFLAG_SUPER_COUNCIL)))
   {
      send_to_char("You are not a council member!\n\r", ch);
      return;
   }

   GET_FREE(member, member_free);

   member->this_member = ch;
   member->next = NULL;
   member->prev = NULL;

   argument = one_argument(argument, arg1);
   argument = one_argument(argument, arg2);

   if (arg1[0] == '\0')
   {
      sprintf(buf, "%s", "");
      sprintf(buf2, "%s", "");
      sprintf(buf, "Members of the Council of %s\n\r\n\r",
              super_councils[this_council].council_name);

      for (imember = super_councils[this_council].first_member; imember != NULL;
           imember = imember->next)
      {
         sprintf(buf2, "%s\n\r", imember->this_member->name);
         safe_strcat(MAX_STRING_LENGTH, buf, buf2);
      }
      send_to_char(buf, ch);

      return;
   }

   if (!str_prefix(arg1, "join"))
   {
      for (imember = super_councils[this_council].first_member; imember != NULL;
           imember = imember->next)
      {
         if (imember->this_member == ch)
         {
            send_to_char("You have already joined the current Council!\n\r", ch);
            return;
         }
      }

      LINK(member, super_councils[this_council].first_member,
           super_councils[this_council].last_member, next, prev);
      sprintf(buf, "You have joined the Current Council of %s\n\r",
              super_councils[this_council].council_name);
      send_to_char(buf, ch);
      super_councils[this_council].council_time = 10;
      do_council(ch, "");
      return;
   }

   if (!str_prefix(arg1, "leave"))
   {
      bool in_council = FALSE;

      for (imember = super_councils[this_council].first_member; imember != NULL;
           imember = imember->next)
      {
         if (imember->this_member == ch)
         {
            in_council = TRUE;
            break;
         }
      }

      if (in_council)
      {
         UNLINK(imember, super_councils[this_council].first_member,
                super_councils[this_council].last_member, next, prev);
         imember->this_member = NULL;
         imember->next = NULL;
         imember->prev = NULL;
         PUT_FREE(imember, member_free);
         send_to_char("You have removed yourself from the current Council in Session!\n\r", ch);
      }
      else
         send_to_char("You are not part of the Current Council!\n\r", ch);

      return;
   }

   if (!str_prefix(arg1, "accept"))
   {
      if (!super_councils[this_council].quorum)
      {
         send_to_char("The council must have a quorum before this action may be taken.\n\r", ch);
         return;
      }
      else
      {
         if ((victim = get_char_room(ch, arg2)) != NULL)
         {
            if (IS_NPC(victim))
            {
               send_to_char("Not on NPCS!!\n\r", ch);
               return;
            }
            else
            {
               sprintf(buf, "%s is now a member of the Council of %s!\n\r", victim->name,
                       super_councils[this_council].council_name);
               send_to_char(buf, ch);
               sprintf(buf, "You are now a member of the Council of %s!\n\r",
                       super_councils[this_council].council_name);
               SET_BIT(victim->pcdata->pflags, PFLAG_SUPER_COUNCIL);
            }
         }
         else
         {
            send_to_char("They are not in this room!\n\r", ch);
         }
         return;
      }
   }

   if (!str_prefix(arg1, "outcast"))
   {

      if (!super_councils[this_council].quorum)
      {
         send_to_char("The council must have a quorum before this action may be taken.\n\r", ch);
         return;
      }
      else
      {
         if ((victim = get_char_world(ch, arg2)) != NULL)
         {
            if (IS_NPC(victim))
            {
               send_to_char("Not on NPCS!!\n\r", ch);
               return;
            }
            else
            {
               sprintf(buf, "%s has been outcast by the %s.\n\r", victim->name,
                       super_councils[this_council].council_name);
               send_to_char(buf, ch);
               sprintf(buf, "You have been outcast by the %s.\n\r",
                       super_councils[this_council].council_name);
               send_to_char(buf, victim);
               do_save(victim, "");
            }
         }
         else
         {
            send_to_char("They are not here!!\n\r", ch);
         }
         return;
      }
   }

   send_to_char("SYNTAX FOR COUNCIL:\n\r\n\r", ch);
   send_to_char(" COUNCIL <join/accept/outcast> <victim>\n\r", ch);
   return;
}
