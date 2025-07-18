#include "globals.h"

int stat_calculated_value(int val);

void do_reincarnate(CHAR_DATA *ch, char *argument)
{
   char arg[MSL];
   char buf[MSL];

   argument = one_argument(argument, arg);

   if (!str_prefix(arg, "information"))
   {
      send_to_char("================REINCARNATION==============\n\r", ch);

      send_to_char("Mortal reincarnations: ", ch);
      for (int i = 0; i < MAX_CLASS; i++)
      {
         sprintf(buf, "%s: %d ", class_table[i].who_name, ch->pcdata->reincarnations[i]);
         send_to_char(buf, ch);
      }
      send_to_char("\n\r", ch);

      send_to_char("Remort reincarnations: ", ch);
      for (int i = 0; i < MAX_REMORT; i++)
      {
         sprintf(buf, "%s: %d ", remort_table[i].who_name, ch->pcdata->remort_reincarnations[i]);
         send_to_char(buf, ch);
      }
      send_to_char("\n\r", ch);

      send_to_char("Adept reincarnations: ", ch);
      for (int i = 0; i < MAX_CLASS; i++)
      {
         sprintf(buf, "%s: %d ", adept_table[i].who_name, ch->pcdata->adept_reincarnations[i]);
         send_to_char(buf, ch);
      }
      send_to_char("\n\r", ch);
   }
   else if (!str_prefix(arg, "buy"))
   {
      /* #define REINCARNATION_DAMCAP 0
      #define REINCARNATION_RMOD 1
      #define REINCARNATION_STATS 2
      #define REINCARNATION_HP 3
      #define REINCARNATION_MANA 4
      #define REINCARNATION_MOVE 5
      #define REINCARNATION_POINTS 6*/

      send_to_char("Buy\n\r", ch);
   }
   else if (!str_prefix(arg, "set"))
   {
      if (get_adept_level(ch) < 20)
      {
         send_to_char("Only realm lords can reincarnate set!\n\r", ch);
         return;
      }
      argument = one_argument(argument, arg);
      if (!str_prefix(arg, "show"))
      {
         send_to_char("Current reincarnation info:\n\r", ch);
         sprintf(buf, "Race: %s\n\r", race_table[ch->pcdata->reincarnate_race].race_name);
         send_to_char(buf, ch);
         sprintf(buf, "Class Order: %s %s %s %s\n\r", class_table[ch->pcdata->reincarnate_order[0]].who_name,
                 class_table[ch->pcdata->reincarnate_order[1]].who_name, class_table[ch->pcdata->reincarnate_order[2]].who_name,
                 class_table[ch->pcdata->reincarnate_order[3]].who_name);
         send_to_char(buf, ch);
         if (ch->pcdata->reincarnate_confirm)
            send_to_char("Reincarnation CONFIRMED, reincarnate set done to reincarnate.\n\r", ch);
      }
      else if (!str_prefix(arg, "race"))
      {
         argument = one_argument(argument, arg);
         ch->pcdata->reincarnate_race = -1;
         REMOVE_BIT(ch->pcdata->reincarnate_confirm, REINCARNATE_RACE);
         REMOVE_BIT(ch->pcdata->reincarnate_confirm, REINCARNATE_CONFIRM);

         for (int i = 0; i < MAX_RACE; i++)
         {
            if (!str_cmp(arg, race_table[i].race_name) || !str_cmp(arg, race_table[i].race_title))
            {
               if (!race_table[i].player_allowed)
               {
                  send_to_char("Players may not select this race.\n\r", ch);
                  return;
               }
               ch->pcdata->reincarnate_race = i;
               sprintf(buf, "%s selected as reincarnation race.\n\r", race_table[i].race_name);
               send_to_char(buf, ch);
            }
         }

         if (ch->pcdata->reincarnate_race == -1)
         {
            send_to_char("Invalid race selection, see rlist for options.\n\r", ch);
            return;
         }

         SET_BIT(ch->pcdata->reincarnate_confirm, REINCARNATE_RACE);
      }
      else if (!str_prefix(arg, "order"))
      {
         ch->pcdata->reincarnate_order[0] = -1;
         ch->pcdata->reincarnate_order[1] = -1;
         ch->pcdata->reincarnate_order[2] = -1;
         ch->pcdata->reincarnate_order[3] = -1;
         REMOVE_BIT(ch->pcdata->reincarnate_confirm, REINCARNATE_ORDER);
         REMOVE_BIT(ch->pcdata->reincarnate_confirm, REINCARNATE_CONFIRM);

         for (int cnt = 0; cnt < MAX_PC_CLASS; cnt++)
         {
            bool found = FALSE;
            argument = one_argument(argument, arg);
            if (arg[0] == '\0')
            {
               send_to_char("Invalid class option. Ex: Cle thi war psi\n\r", ch);
               return;
            }
            for (int foo = 0; foo < MAX_CLASS; foo++)
            {
               if (!str_cmp(arg, class_table[foo].who_name))
               {
                  ch->pcdata->reincarnate_order[cnt] = foo;
                  found = TRUE;
                  break;
               }
            }

            if (!found)
            {
               send_to_char("Invalid class option. Ex: Cle thi war psi\n\r", ch);
               return;
            }
         }

         SET_BIT(ch->pcdata->reincarnate_confirm, REINCARNATE_ORDER);
      }
      else if (!str_prefix(arg, "confirm"))
      {
         for (int i = 0; i < MAX_PC_CLASS; i++)
         {
            if (ch->pcdata->reincarnate_order[i] == -1)
            {
               send_to_char("Incorrect class order.\n\r", ch);
               REMOVE_BIT(ch->pcdata->reincarnate_confirm, REINCARNATE_ORDER);
               REMOVE_BIT(ch->pcdata->reincarnate_confirm, REINCARNATE_CONFIRM);
               return;
            }
         }
         if (ch->pcdata->reincarnate_race == -1)
         {
            send_to_char("Incorrect race.\n\r", ch);
            REMOVE_BIT(ch->pcdata->reincarnate_confirm, REINCARNATE_RACE);
            REMOVE_BIT(ch->pcdata->reincarnate_confirm, REINCARNATE_CONFIRM);
            return;
         }
         send_to_char("Reincarnation confirm set!\n\r", ch);
         SET_BIT(ch->pcdata->reincarnate_confirm, REINCARNATE_CONFIRM);
      }
      else if (!str_prefix(arg, "done"))
      {
         if (!IS_SET(ch->pcdata->reincarnate_confirm, REINCARNATE_ORDER) ||
             !IS_SET(ch->pcdata->reincarnate_confirm, REINCARNATE_RACE) ||
             !IS_SET(ch->pcdata->reincarnate_confirm, REINCARNATE_CONFIRM))
         {
            send_to_char("You haven't confirmed your reincarnation yet!\n\r", ch);
            return;
         }

         remove_all(ch);

         ch->pcdata->reincarnation_data[REINCARNATION_HP] += ch->pcdata->hp_from_gain;
         ch->pcdata->hp_from_gain = 0;
         ch->pcdata->reincarnation_data[REINCARNATION_MANA] += ch->pcdata->mana_from_gain;
         ch->pcdata->mana_from_gain = 0;
         ch->pcdata->reincarnation_data[REINCARNATION_MOVE] += ch->pcdata->move_from_gain;
         ch->pcdata->reincarnation_data[REINCARNATION_POINTS]++;
         ch->pcdata->move_from_gain = 0;
         ch->pcdata->reincarnations[ch->pcdata->order[0]]++;
         ch->race = ch->pcdata->reincarnate_race;
         ch->level = 1;
         send_to_char("Done\n\r", ch);
         for (int i = 0; i < MAX_CLASS; i++)
         {
            if (ch->adept[i] > 0)
               ch->pcdata->adept_reincarnations[i]++;
         }
         for (int i = 0; i < MAX_REMORT; i++)
         {
            if (ch->remort[i] > 0)
               ch->pcdata->remort_reincarnations[i]++;
         }
         for (int i = 0; i < MAX_CLASS; i++)
         {
            ch->lvl[i] = 0;
            ch->adept[i] = 0;
            ch->remort[i] = 0;
            ch->remort[i + MAX_CLASS] = 0;
            ch->pcdata->order[i] = ch->pcdata->reincarnate_order[i];
            ch->pcdata->index[ch->pcdata->reincarnate_order[i]] = i;
         }
         ch->lvl[ch->pcdata->order[0]] = 2;
         reset_gain_stats(ch);
         ch->level = 1;
         do_save(ch, "");
      }
      else
      {
         sprintf(buf, "Invalid input %s\n\r", arg);
         send_to_char(buf, ch);
         send_to_char("Valid set args: show, race, order, confirm, done", ch);
      }
   }
   else
      send_to_char("Valid args: information, buy, set\n\r", ch);
}

int get_reincarnate_hp_level(CHAR_DATA *ch)
{
   return stat_calculated_value(ch->pcdata->reincarnation_data[REINCARNATION_HP]);
}

int get_reincarnate_mana_level(CHAR_DATA *ch)
{
   return stat_calculated_value(ch->pcdata->reincarnation_data[REINCARNATION_MANA]);
}

int get_reincarnate_move_level(CHAR_DATA *ch)
{
   return stat_calculated_value(ch->pcdata->reincarnation_data[REINCARNATION_MOVE]);
}

int stat_calculated_value(int val)
{
   int steps = 5000;
   int steps_1 = 3;
   int steps_2 = 6;
   int steps_3 = 10;
   int value = val / 10;
   int calculated_value = 0;

   if (value > steps)
   {
      calculated_value += steps;
      value -= steps;
   }
   else
   {
      calculated_value += value;
      value = 0;
   }

   if (value > steps * steps_1)
   {
      calculated_value += steps;
      value -= steps * steps_1;
   }
   else
   {
      calculated_value += value / steps_1;
      value = 0;
   }

   if (value > steps * steps_2)
   {
      calculated_value += steps;
      value -= steps * steps_2;
   }
   else
   {
      calculated_value += value / steps_2;
      value = 0;
   }

   if (value > 0)
      calculated_value += value / steps_3;

   return calculated_value / 100;
}
