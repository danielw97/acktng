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

/**** Local Functions ****/
CHAR_DATA *get_quest_target args((int min_level, int max_level));
CHAR_DATA *get_quest_giver args((int min_level, int max_level));
OBJ_DATA *load_quest_object args((CHAR_DATA * target));
int quest_is_valid_crusade_mobile args((CHAR_DATA * target, int min_level, int max_level));
void clear_crusade args((void));
sh_int quest_tier_from_level args((int mob_level));
void quest_set_crusade_level_range_for_tier args((sh_int tier, int *minimum_level,
                                                  int *maximum_level));
void quest_set_effective_crusade_level_range args((sh_int tier, int highest_level_in_range,
                                                   int *minimum_level, int *maximum_level));
int crusade_level_cap_for_range args((int range_minimum, int range_maximum,
                                      int highest_level_in_range));

static void format_quest_message(char *dest, const char *message, const char *value1,
                                 const char *value2)
{
   const char *scan;
   char *out;
   const char *values[2];
   int value_index;

   if (dest == NULL)
      return;

   if (message == NULL)
   {
      dest[0] = '\0';
      return;
   }

   values[0] = (value1 != NULL) ? value1 : "";
   values[1] = (value2 != NULL) ? value2 : "";
   value_index = 0;
   scan = message;
   out = dest;

   while ((*scan != '\0') && (out < (dest + MAX_STRING_LENGTH - 1)))
   {
      if (scan[0] == '%' && scan[1] == 's' && value_index < 2)
      {
         const char *replacement;

         replacement = values[value_index++];
         while ((*replacement != '\0') && (out < (dest + MAX_STRING_LENGTH - 1)))
            *out++ = *replacement++;

         scan += 2;
         continue;
      }

      *out++ = *scan++;
   }

   *out = '\0';
}

void quest_set_crusade_level_range_for_mob_level(int mob_level, int *minimum_level,
                                                 int *maximum_level)
{
   int resolved_level;
   sh_int tier;

   if (minimum_level == NULL || maximum_level == NULL)
      return;

   resolved_level = URANGE(1, mob_level, 170);
   tier = quest_tier_from_level(resolved_level);
   quest_set_crusade_level_range_for_tier(tier, minimum_level, maximum_level);
}

sh_int quest_tier_from_level(int mob_level)
{
   if (mob_level <= 100)
      return 1;

   if (mob_level <= 149)
      return 2;

   return 3;
}

void quest_set_crusade_level_range_for_tier(sh_int tier, int *minimum_level, int *maximum_level)
{
   if (minimum_level == NULL || maximum_level == NULL)
      return;

   if (tier == 1)
   {
      *minimum_level = 1;
      *maximum_level = 100;
      return;
   }

   if (tier == 2)
   {
      *minimum_level = 101;
      *maximum_level = 149;
      return;
   }

   *minimum_level = 150;
   *maximum_level = 170;
}

void quest_set_effective_crusade_level_range(sh_int tier, int highest_level_in_range,
                                             int *minimum_level, int *maximum_level)
{
   int capped_max;

   quest_set_crusade_level_range_for_tier(tier, minimum_level, maximum_level);

   if (minimum_level == NULL || maximum_level == NULL)
      return;

   if (highest_level_in_range <= 0)
      return;

   capped_max = crusade_level_cap_for_range(*minimum_level, *maximum_level, highest_level_in_range);
   *maximum_level = capped_max;
}

int crusade_level_cap_for_range(int range_minimum, int range_maximum, int highest_level_in_range)
{
   int cap_level;

   cap_level = UMIN(range_maximum, highest_level_in_range + 20);
   cap_level = UMAX(range_minimum, cap_level);

   return cap_level;
}

void quest_note_player_crusade_range(int pseudo_level, int *highest_mortal, int *highest_remortal,
                                     int *highest_adept)
{
   if ((highest_mortal == NULL) || (highest_remortal == NULL) || (highest_adept == NULL))
      return;

   if (pseudo_level <= 100)
   {
      if (pseudo_level > *highest_mortal)
         *highest_mortal = pseudo_level;
      return;
   }

   if (pseudo_level <= 149)
   {
      if (pseudo_level > *highest_remortal)
         *highest_remortal = pseudo_level;
      return;
   }

   if (pseudo_level > *highest_adept)
      *highest_adept = pseudo_level;
}

sh_int quest_resolve_crusade_personality(sh_int personality, int mob_level)
{
   int resolved_level;

   if (personality >= 1 && personality <= 3)
      return personality;

   resolved_level = URANGE(1, mob_level, 170);
   return quest_tier_from_level(resolved_level);
}

/* 17 messages, organised by blocks for each personality
   indented messages are for when the target mob gets killed  */
struct qmessage_type
{
   char *const message1;
   char *const message2;
};

const struct qmessage_type qmessages[4][17] = {
    {{"", ""},
     {"", ""},
     {"", ""},
     {"", ""},
     {"", ""},
     {"", ""},
     {"", ""},
     {"", ""},
     {"", ""},
     {"", ""},
     {"", ""},
     {"", ""},
     {"", ""},
     {"", ""},
     {"", ""},
     {"", ""},
     {"", ""}},
    {{"@@aOh my! My %s @@ahas been stolen from me, and I am too young to find it!", ""},
     {"@@aWould someone please search for my %s@@a? I'm sure that it will be much too hard for me "
      "to find.",
      ""},
     {"@@aWhat will I do without my %s@@a?", ""},
     {"@@aCan't anybody find my %s @@afor me?", ""},
     {"@@aHelp me! My %s @@ais still missing!", ""},
     {"@@aKeep searching for my %s@@a, and i'll ask my mom if she knows who stole it!", ""},
     {"@@aDoes anyone know who stole my %s@@a?", ""},
     {"@@aMy mom says %s@@a stole my %s@@a. I know it is too hard for me to get back. Oh my, what "
      "will I do?",
      "@@aYeay!! It looks like whoever stole my %s @@ais now dead!! Thank you very much!"},
     {"@@aPlease, can you go kill %s@@a? I need my %s@@a!! I can't sleep without it!!",
      "@@aWell, thank you for killing the nasty thief, but can you please return my %s @@ato me?"},
     {"@@aMy mom says %s@@a stole my %s@@a. I know it is too hard for me to get back. Oh my, what "
      "will I do?",
      "@@aYeay!! It looks like whoever stole my %s @@ais now dead!! Thank you very much!"},
     {"@@aIt's time for my nap now, and %s still has my %s@@a!! Can anyone please get it back for "
      "me?",
      "@@aPlease, time is running out! Return my %s @@ato me!"},
     {"@@a%s@@a is a real meanie for stealing my %s@@a! Can you pretty pretty pretty please get it "
      "back for me?",
      "@@aPlease, time is running out! Return my %s @@ato me!"},
     {"@@aIt's time for my nap now, and %s still has my %s@@a!! Can anyone please get it back for "
      "me?",
      "@@aPlease, time is running out! Return my %s @@ato me!"},
     {"@@aOh my, I'll never get up to watch cartoons tomorrow now!  %s @@ais still holding my "
      "%s@@a ransom, and I need it for my nap!",
      "@@aHow can I sleep without my %s@@a?"},
     {"@@aI give up! %s @@acan keep my %s @@afor all I care! I didn't want to take a nap, anyway!",
      "@@aI give up! I never want to see my %s @@aagain!"},
     {"@@aMommy, can I watch cartoons now, instead of taking a nap?? PLEASE??", ""},
     {"@@aOh THANK YOU, %s@@a!! Now that I have my %s @@aagain, I can go sleepy sleep!", ""}

    },

    {{"Hmm, I seem to have lost my %s@@l. Oh well, didn't really need it much, anyway.", ""},
     {"I wonder where I could find another %s@@l? Anyone have any ideas?", ""},
     {"Where can my %s @@lhave gone?", ""},
     {"I guess no one really cares, anyway, but I think I might need a %s @@llater.", ""},
     {"I guess I should try and find my %s@@l, but I really don't feel like it.", ""},
     {"If anyone has an extra %s@@l, I might be willing to reward them for it.", ""},
     {"Doesn't anyone out there know where to find a %s@@l?", ""},
     {"Hmm, maybe %s@@l knew something I didn't, and thought it was a good idea to steal my %s@@l. "
      "Maybe he could use it, I know I can't.",
      "I guess my %s @@ldidn't help him much, since he is now dead!  I do miss it though."},
     {"Hmm, maybe it IS worth something.  Could someone go kill %s@@l and get my %s@@l back for "
      "me?",
      "I guess my %s @@ldidn't help him much, since he is now dead!  I do miss it though."},
     {"I would pay a lot if someone would kill %s @@land get my %s@@l back. I don't really know "
      "where it went.",
      "Even though it's not worth very much, my %s @@lis kind of important to me. Oh, well, guess "
      "I will never see it again."},
     {"Hmm, maybe it IS worth something.  Could someone go kill %s@@l and get my %s@@l back for "
      "me?",
      "I guess my %s @@ldidn't help him much, since he is now dead!  I do miss it though."},
     {"I would pay a lot if someone would kill %s @@land get my %s@@l back. I don't really know "
      "where it went.",
      "Even though it's not worth very much, my %s @@lis kind of important to me. Oh, well, guess "
      "I will never see it again."},
     {"Oh well, since no one will help me, I guess %s @@lcan keep my %s@@l.",
      "It must be cursed, since everyone who has it is dead. I don't think I want my %s @@lafter "
      "all!"},
     {"Oh well, since no one will help me, I guess %s @@lcan keep my %s@@l.",
      "It must be cursed, since everyone who has it is dead. I don't think I want my %s @@lafter "
      "all!"},
     {"I give up! %s @@lcan keep my %s @@lfor all I care!",
      "I give up! I never want to see my %s @@lagain!"},
     {"Well, I will stop asking now, but don't ever ask ME for any favors, ok?", ""},
     {"Well, looks like %s @@lhas recovered my %s @@lfor me. Not sure I want it anymore, but "
      "thanks anyway.",
      ""}

    },

    /*
    {
       { "BANZAI! My %s @@lhas been stolen from me!  Will someone brave recover it?", 	"" },
       { "Oh! Has no one found my %s @@lfor me yet?", 							"" },
       { "Where can my %s @@lhave gone?", 									"" },
       { "Can't anybody find my %s @@lfor me?",  							"" },
       { "Help me! My %s @@lhas not yet been recovered!", 						"" },
       { "Keep searching for my %s@@l, and i'll find out who stole it!", 			"" },
       { "Were there no witnesses to the theft of my %s?@@l", 					"" },
       { "It was %s @@lwho stole my %s @@lfrom me!  Someone help me!",
          "It looks like whoever stole my %s @@lis now dead!!" 					},
       { "Please, time is running out! Recover my %s @@lfrom %s @@lfor me NOW!",
          "Please, time is running out! Return my %s @@lto me!" 				},
       { "Please, time is running out! Recover my %s @@lfrom %s @@lfor me NOW!",
          "Please, time is running out! Return my %s @@lto me!" 				},
       { "Please, time is running out! Recover my %s @@lfrom %s @@lfor me NOW!",
          "Please, time is running out! Return my %s @@lto me!" 				},
       { "Please, time is running out! Recover my %s @@lfrom %s @@lfor me NOW!",
          "Please, time is running out! Return my %s @@lto me!" 				},
       { "I give up! %s @@lcan keep my %s @@lfor all I care!",
          "I give up! I never want to see my %s @@lagain!" 					},
       { "I give up! %s @@lcan keep my %s @@lfor all I care!",
          "I give up! I never want to see my %s @@lagain!"					},
       { "I give up! %s @@lcan keep my %s @@lfor all I care!",
          "I give up! I never want to see my %s @@lagain!"					},
       { "Shoot! Just forget about recovering ANYTHING for me, ok?" , 				"" },
       { "At Last! %s @@lhas recovered %s @@lfor me!", 						"" }

        },  */

    {{"@@eMuuaahhhaaahaaaa! Some puny mortal has stolen my %s@@e!  I shall seek revenge!!", ""},
     {"@@eI shall send many minions to seek my %s@@e! All that steal from me shall die!!", ""},
     {"@@eSO, you have defeated my servants.  I shall still regain my %s@@e!!", ""},
     {"@@eI am prepared to reward well anyone that aids the return of my %s@@e. Are any of you "
      "puny mortals willing to attempt my challenge?",
      ""},
     {"@@eIf you are worthy, I will grant many favors upon anyone that returns my %s@@e.", ""},
     {"@@mMethlok@@e, By the dark powers, I command you to seek my %s@@e! Now, if any of you "
      "worthless mortals wish to attempt to return it, I shall grant you many powers!",
      ""},
     {"@@eI sense that @@mMethlok@@e is nearing the witless thief who stole my %s@@e. Now, my "
      "vengence shall be sweet!",
      ""},
     {"@@eAhhh, my servant has returned, and informs me that %s @@estole my %s@@e. They shall be "
      "incinerated by the powers that I command!!!",
      "@@mMethlok@@e has informed me that the weakling that stole my %s @@lhas met his maker!!"},
     {"@@eAre none of you powerful enough to kill %s @@eand regain my %s@@e? Bah!! Mortals are "
      "useless, except as side dishes!!",
      "@@eThough my taste for blood has been satiated, my %s @@estill evades my grasp!"},
     {"@@eAre none of you powerful enough to kill %s @@e and regain my %s @@e? Bah!! Mortals are "
      "useless, except as side dishes!!",
      "@@eThough my taste for blood has been satiated, my %s @@estill evades my grasp!"},
     {"@@eAre none of you powerful enough to kill %s @@e and regain my %s @@e? Bah!! Mortals are "
      "useless, except as side dishes!!",
      "@@eThough my taste for blood has been satiated, my %s @@estill evades my grasp!"},
     {"@@eI should have known that a powerless, puny mortal could never be the servant of my "
      "vengence against %s@@e, or regain my %s@@e!!",
      "@@eI shall rain death upon all of you for refusing to return my %s@@e!!!"},
     {"@@eI should have known that a powerless, puny mortal could never be the servant of my "
      "vengence against %s@@e, or regain my %s@@e!!",
      "@@eI shall rain death upon all of you for refusing to return my %s@@e!!!"},
     {"@@eI shall slay your brothers and poison your fields for refusing to seek %s @@eand return "
      "my %s@@e!!!!",
      "@@eThough my vengeance has been served, I shall drink your souls for your failure to return "
      "my %s@@e!!!"},
     {"@@eI shall slay your brothers and poison your fields for refusing to seek %s @@eand return "
      "my %s@@e!!!!",
      "@@eThough my vengeance has been served, I shall drink your souls for your failure to return "
      "my %s@@e!!!"},
     {"@@eDeath and great suffering shall be your punishment for failing me!!!?", ""},
     {"@@eWell done.  It seems that %s @@eat least has a modicum of strength, unlike you worthless "
      "idiots who failed to return my %s@@e! My curse shall lie upon you for the rest of your "
      "short days!",
      ""}

    }

};

void do_iquest(CHAR_DATA *ch, char *argument)
{
   extern bool quest;
   extern bool auto_quest;
   extern CHAR_DATA *quest_mob;
   extern CHAR_DATA *quest_target;
   extern OBJ_DATA *quest_object;
   extern int quest_timer;
   extern int quest_wait;
   extern int quest_level_min;
   extern int quest_level_max;
   extern sh_int quest_personality;

   char buf[MAX_STRING_LENGTH];
   char new_long_desc[MAX_STRING_LENGTH];

   if (argument[0] == '\0') /* Display status */
   {
      if (!quest)
      {
         send_to_char("There is no quest currently running.\n\r", ch);
         if (auto_quest)
            send_to_char("Quests are currently running automatically.\n\r", ch);
         if (quest_wait > 0)
         {
            sprintf(buf, "The next quest may occur in %d minutes.\n\r", quest_wait);
            send_to_char(buf, ch);
         }
         return;
      }
      else
         send_to_char("There is currently a quest running ", ch);

      if (auto_quest)
         send_to_char("(Automatically)", ch);

      send_to_char("\n\rQuest Details:\n\r\n\r", ch);
      if (quest_mob)
      {

         sprintf(buf, "The questing mobile is: %s [In Room %d]\n\r", quest_mob->short_descr,
                 quest_mob->in_room->vnum);
         send_to_char(buf, ch);
      }
      else
      {
         send_to_char("The questing mobile is dead!\n\r", ch);
      }
      if (quest_target)
      {
         sprintf(buf, "Target Mobile is: %s [In Room %d]\n\r", quest_target->short_descr,
                 quest_target->in_room->vnum);
         send_to_char(buf, ch);
      }
      else
         send_to_char("The target mobile is dead!\n\r", ch);

      sprintf(buf, "Target Object is: %s.\n\r", quest_object->short_descr);
      send_to_char(buf, ch);

      sprintf(buf, "Quest Object is worth: %d QP, %d Prac, %d GP\n\r", quest_object->value[0],
              quest_object->value[1], quest_object->value[2]);
      send_to_char(buf, ch);

      sprintf(buf, "The Quest has been running for %d/15 minutes.\n\r", quest_timer);
      send_to_char(buf, ch);

      sprintf(buf, "Quest level range is: %d to %d.\n\r", quest_level_min, quest_level_max);
      send_to_char(buf, ch);

      return;
   }
   if (!strcmp(argument, "stop"))
   {
      if (quest)
      {
         sprintf(buf, "@@NThe quest has been stopped by an @@mImmortal@@N. Please speak up if you "
                      "have already gotten the item.\n\r");
         do_echo(ch, buf);
         clear_crusade();
      }
      return;
   }

   if (!strcmp(argument, "start"))
   {
      DESCRIPTOR_DATA *d;
      int a = 80;
      int b = 0;
      sh_int player_count = 0, average_level = 0, total_levels = 0;
      int highest_mortal = 0, highest_remortal = 0, highest_adept = 0;
      int highest_level_for_range = 0;

      /*
       * generate a new quest!
       */
      if (quest)
      {
         send_to_char("There is already a quest running...\n\r", ch);
         return;
      }

      if (auto_quest)
      {
         auto_quest = FALSE;
         send_to_char("Automatic Quests now OFF.\n\r", ch);
      }

      /*
       * Work out levels of currently playing folks
       */
      for (d = first_desc; d; d = d->next)
      {
         int pseudo_level;

         if ((d->connected != CON_PLAYING) || (d->character == NULL) || (IS_IMMORTAL(d->character)))
            continue;
         player_count += 1;
         total_levels += d->character->level;

         pseudo_level = get_psuedo_level(d->character);
         quest_note_player_crusade_range(pseudo_level, &highest_mortal, &highest_remortal,
                                         &highest_adept);
      }
      average_level =
          (((total_levels == 0) ? 30 : total_levels) / ((player_count == 0) ? 1 : player_count));
      a = average_level - 20;
      b = average_level + 20;
      quest_mob = get_quest_giver(a, b);
      if (quest_mob == NULL)
      {
         send_to_char("Failed to find a quest mob\n\r", ch);
         return;
      }
      quest_personality = quest_tier_from_level(quest_mob->level);

      if (quest_personality == 1)
         highest_level_for_range = highest_mortal;
      else if (quest_personality == 2)
         highest_level_for_range = highest_remortal;
      else
         highest_level_for_range = highest_adept;

      quest_set_effective_crusade_level_range(quest_personality, highest_level_for_range, &a, &b);

      quest_level_min = a;
      quest_level_max = b;

      if ((quest_mob->level < a) || (quest_mob->level > b))
      {
         quest_mob = get_quest_giver(a, b);
         if (quest_mob == NULL)
         {
            send_to_char("Failed to find a quest mob\n\r", ch);
            return;
         }
      }

      quest_target = get_quest_target(a, b);
      if ((quest_target == NULL) || (quest_target == quest_mob))
      {
         send_to_char("Failed to find a quest target\n\r", ch);
         return;
      }
      quest_object = load_quest_object(quest_target);
      if (quest_object == NULL)
      {
         send_to_char("An invalid quest object was encountered.  Check log files.\n\r", ch);
         quest = FALSE;
         return;
      }
      /*
       * Set values on quest item for Qp, Pracs, Exp, Gold
       */
      quest_object->value[0] = UMAX(1, (quest_target->level / 30));
      quest_object->value[1] = UMAX(1, (quest_target->level / 25));
      quest_object->value[2] = (quest_target->level * 20);

      if (number_percent() < 10)
      {
         quest_object->value[0] += 2;
         quest_object->value[1] += 3;
         quest_object->value[2] *= 2;
      }

      quest_timer = 0;
      quest = TRUE;
      new_long_desc[0] = '\0';
      if (quest_mob->long_descr_orig != NULL)
         free_string(quest_mob->long_descr_orig);
      quest_mob->long_descr_orig = str_dup(quest_mob->long_descr);
      sprintf(new_long_desc, "%s @@Nsays have you found my %s ?\n\r", quest_mob->short_descr,
              quest_object->short_descr);
      if (quest_mob->long_descr != NULL)
         free_string(quest_mob->long_descr);
      quest_mob->long_descr = str_dup(new_long_desc);
      SET_BIT(quest_mob->act, PLR_NOSUMMON);
      SET_BIT(quest_mob->act, PLR_NOVISIT);
      SET_BIT(quest_mob->act, ACT_NOBLOOD);
      SET_BIT(quest_mob->act, ACT_NO_HUNT);

      new_long_desc[0] = '\0';
      if (quest_target->long_descr_orig != NULL)
         free_string(quest_target->long_descr_orig);
      quest_target->long_descr_orig = str_dup(quest_target->long_descr);
      sprintf(new_long_desc, "%s @@Nsays I stole the %s !!!\n\r", quest_target->short_descr,
              quest_object->short_descr);
      if (quest_target->long_descr != NULL)
         free_string(quest_target->long_descr);
      quest_target->long_descr = str_dup(new_long_desc);

      SET_BIT(quest_target->act, PLR_NOSUMMON);
      SET_BIT(quest_target->act, PLR_NOVISIT);
      SET_BIT(quest_target->act, ACT_NOBLOOD);
      SET_BIT(quest_target->act, ACT_NO_HUNT);

      send_to_char("QUEST STARTED!\n\r\n\r", ch);

      sprintf(buf, "The questing mobile is: %s [In Room %d]\n\r", quest_mob->short_descr,
              quest_mob->in_room->vnum);
      send_to_char(buf, ch);

      sprintf(buf, "Target Mobile is: %s [In Room %d]\n\r", quest_target->short_descr,
              quest_target->in_room->vnum);
      send_to_char(buf, ch);

      sprintf(buf, "Target Object is: %s.\n\r", quest_object->short_descr);
      send_to_char(buf, ch);

      sprintf(buf, "Quest Object is worth: %d QP, %d Prac, %d GP\n\r", quest_object->value[0],
              quest_object->value[1], quest_object->value[2]);
      send_to_char(buf, ch);

      sprintf(buf, "Quest level range is: %d to %d.\n\r", quest_level_min, quest_level_max);
      send_to_char(buf, ch);

      return;
   }
   if (!str_cmp(argument, "auto"))
   {
      send_to_char("AutoQuest now initiated!\n\r", ch);
      auto_quest = TRUE;
      return;
   }

   return;
}

int quest_is_valid_crusade_mobile(CHAR_DATA *target, int min_level, int max_level)
{
   if (target == NULL || !IS_NPC(target) || target->in_room == NULL ||
       target->in_room->area == NULL)
      return 0;

   if ((target->level < min_level) || (target->level > max_level) ||
       (IS_SET(target->in_room->area->flags, AREA_NOSHOW)) || (IS_SET(target->act, ACT_SENTINEL)) ||
       (IS_SET(target->act, ACT_PET)) || (IS_SET(target->act, ACT_INVASION)))
      return 0;

   return 1;
}

/*
 * get_quest_target : This attempts to pick a random mobile to hold the quest
 * item for the player (questor).  Various checks are made to ensure that the
 * questor has a chance of killing the mobile, etc.
 * Returns NULL if it didn't get a mobile this time.
 */

CHAR_DATA *get_quest_target(int min_level, int max_level)
{
   CHAR_DATA *target;
   int min_index = 0; /* the minimum number of times to go through the list */

   /*   int min_distance = 50; unused */
   /*   char *dirs = NULL; unused */

   if (max_level > 170)
      max_level = 170;
   min_index = number_range(1, 1000);

   for (target = first_char; target != NULL; target = target->next)
   {
      if (!IS_NPC(target))
         continue;
      min_index -= 1;

      if (min_index > 0)
         continue;

      /*
       * Check if mobile is suitable for the quest -
       * Ignore mobs that are likely to be in well known places, such
       * as train/prac mobs, healers, etc
       */

      if (!quest_is_valid_crusade_mobile(target, min_level, max_level))
         continue;

      if ((!str_cmp(rev_spec_lookup(target->spec_fun), "spec_stephen")) ||
          (!str_cmp(rev_spec_lookup(target->spec_fun), "spec_tax_man")))
         continue;

      /*
       * Lastly, some random choice
       */
      if (number_percent() < 2)
         break;
   }

   return target;
}

/*
 * load_quest_object : This takes a pointer to OBJ_INDEX_DATA and places the
 * object onto the target.
 */

OBJ_DATA *load_quest_object(CHAR_DATA *target)
{
   OBJ_INDEX_DATA *pObj;
   OBJ_DATA *object;
   int foo;

   foo = number_range(OBJ_VNUM_QUEST_MIN, OBJ_VNUM_QUEST_MAX);

   pObj = get_obj_index(foo);

   if (pObj == NULL)
   {
      bug("load_quest_object : Invalid object vnum %d.", foo);
      return NULL;
   }

   object = create_object(pObj, 1);
   obj_to_char(object, target);

   return object;
}

CHAR_DATA *get_quest_giver(int min_level, int max_level)
{
   CHAR_DATA *target;
   int min_index = 0;

   /*   int max_distance = 20; unused */
   /*   char *dirs = NULL; unused */
   min_index = number_range(0, 1000);

   for (target = first_char; target != NULL; target = target->next)
   {
      if (!IS_NPC(target))
         continue;
      min_index -= 1;

      if (min_index > 0)
         continue;

      /*
       * Check if mobile is suitable for the quest -
       * Ignore mobs that are likely to be in well known places, such
       * as train/prac mobs, healers, etc
       */

      if ((target->level < min_level) || (target->level > max_level) ||
          (IS_SET(target->in_room->area->flags, AREA_NOSHOW)) ||
          (IS_SET(target->act, ACT_SENTINEL)) || (IS_SET(target->act, ACT_PET)) ||
          (IS_SET(target->act, ACT_INVASION)) ||
          (!str_cmp(rev_spec_lookup(target->spec_fun), "spec_stephen")) ||
          (!str_cmp(rev_spec_lookup(target->spec_fun), "spec_tax_man")))

         continue;
      {
         if (number_percent() < 2)
            break;
      }
   }

   return target;
}

/*
 * crusade_inform : Makes the questing mobile give out information to the
 * players on the mud.  Starts off real simple, and gets more helpful as
 * time runs out :P
 */

void crusade_inform(void)
{
   char buf[MAX_STRING_LENGTH];
   extern CHAR_DATA *quest_mob;
   extern CHAR_DATA *quest_target;
   extern OBJ_DATA *quest_object;
   extern int quest_timer;
   extern sh_int quest_personality;
   extern const struct qmessage_type qmessages[4][17];
   sh_int active_personality;

   active_personality =
       quest_resolve_crusade_personality(quest_personality, quest_mob ? quest_mob->level : 1);
   quest_personality = active_personality;

   /*
    * Work out what the mob should tell the players....
    */
   /*
    * Add random element to each case so quests look different each time?
    */
   if (quest_timer < 7)
   {
      format_quest_message(buf, qmessages[active_personality][quest_timer].message1,
                           quest_object->short_descr, NULL);
   }
   else
   {
      if (quest_target)
         format_quest_message(buf, qmessages[active_personality][quest_timer].message1,
                              quest_target->short_descr, quest_object->short_descr);
      else
         format_quest_message(buf, qmessages[active_personality][quest_timer].message2,
                              quest_object->short_descr, NULL);
   }

   quest_timer++;
   if (quest_mob && quest_timer < 16)
      do_crusade(quest_mob, buf);
   if (quest_timer == 1)
   {
      sprintf(buf, " %s is crusading for %s ", NAME(quest_mob), quest_object->short_descr);
      info(buf, 5);
   }
   if (!quest_mob)
   {
      clear_crusade();
   }
   return;
}

void crusade_complete(CHAR_DATA *ch)
{
   extern CHAR_DATA *quest_mob;
   extern OBJ_DATA *quest_object;
   extern sh_int quest_personality;
   extern const struct qmessage_type qmessages[4][17];
   sh_int active_personality;

   char buf[MAX_STRING_LENGTH];

   active_personality =
       quest_resolve_crusade_personality(quest_personality, quest_mob ? quest_mob->level : 1);
   quest_personality = active_personality;

   format_quest_message(buf, qmessages[active_personality][16].message1, NAME(ch),
                        quest_object->short_descr);
   do_crusade(quest_mob, buf);
   clear_crusade();
   return;
}

void crusade_cancel()
{
   extern CHAR_DATA *quest_mob;

   if (quest_mob)
      do_crusade(quest_mob, "Shoot! Just forget about recovering ANYTHING for me, ok?");

   clear_crusade();
   return;
}

void clear_crusade()
{
   extern bool quest;
   extern CHAR_DATA *quest_mob;
   extern CHAR_DATA *quest_target;
   extern OBJ_DATA *quest_object;
   extern int quest_timer;
   extern int quest_wait;
   extern sh_int quest_personality;
   extern int quest_level_min;
   extern int quest_level_max;

   /*
    * Clear ALL values, ready for next quest
    */

   quest = FALSE;
   extract_obj(quest_object);
   if (quest_mob)
   {
      free_string(quest_mob->long_descr);
      quest_mob->long_descr = str_dup(quest_mob->long_descr_orig);
      free_string(quest_mob->long_descr_orig);
      quest_mob->long_descr_orig = NULL;
      REMOVE_BIT(quest_mob->act, PLR_NOSUMMON);
      REMOVE_BIT(quest_mob->act, PLR_NOVISIT);
      REMOVE_BIT(quest_mob->act, ACT_NOBLOOD);
      REMOVE_BIT(quest_mob->act, ACT_NO_HUNT);
   }
   if (quest_target)
   {
      free_string(quest_target->long_descr);
      quest_target->long_descr = str_dup(quest_target->long_descr_orig);
      free_string(quest_target->long_descr_orig);
      quest_target->long_descr_orig = NULL;
      REMOVE_BIT(quest_target->act, PLR_NOSUMMON);
      REMOVE_BIT(quest_target->act, PLR_NOVISIT);
      REMOVE_BIT(quest_target->act, ACT_NOBLOOD);
      REMOVE_BIT(quest_target->act, ACT_NO_HUNT);
   };

   quest_mob = NULL;
   quest_target = NULL;
   quest_object = NULL;
   quest_timer = 0;
   quest_wait = 2 + number_range(1, 4);
   quest_personality = 0;
   quest_level_min = 0;
   quest_level_max = 0;

   return;
}

void generate_auto_crusade()
{
   DESCRIPTOR_DATA *d;
   extern bool quest;

   extern CHAR_DATA *quest_mob;
   extern CHAR_DATA *quest_target;
   extern OBJ_DATA *quest_object;
   extern int quest_timer;
   extern int quest_wait;
   extern sh_int quest_personality;
   extern int quest_level_min;
   extern int quest_level_max;
   int hunt_flags = 0;
   char new_long_desc[MAX_STRING_LENGTH];
   sh_int loop_counter = 0;

   int a = 170;
   int b = 0;
   sh_int player_count = 0;
   int highest_mortal = 0;
   int highest_remortal = 0;
   int highest_adept = 0;
   int highest_level_for_range = 0;
   bool has_mortal = FALSE;
   bool has_remortal = FALSE;
   bool has_adept = FALSE;
   sh_int available_ranges[3];
   sh_int available_count = 0;
   sh_int selected_range = 1;

   /*
    * generate a new quest!
    */
   if (quest)
   {
      return;
   }

   quest_mob = NULL;
   quest_target = NULL;

   /*
    * Work out levels of currently playing folks
    */
   for (d = first_desc; d; d = d->next)
   {
      int pseudo_level;

      if (d->connected != CON_PLAYING || d->character == NULL || IS_IMMORTAL(d->character))
         continue;

      player_count += 1;
      pseudo_level = get_psuedo_level(d->character);

      quest_note_player_crusade_range(pseudo_level, &highest_mortal, &highest_remortal,
                                      &highest_adept);

      if (pseudo_level <= 100)
         has_mortal = TRUE;
      else if (pseudo_level <= 149)
         has_remortal = TRUE;
      else
         has_adept = TRUE;
   }

   if (player_count == 0)
   {
      quest_wait = number_range(1, 3);
      return;
   }

   if (has_mortal)
      available_ranges[available_count++] = 1;
   if (has_remortal)
      available_ranges[available_count++] = 2;
   if (has_adept)
      available_ranges[available_count++] = 3;

   if (available_count == 0)
   {
      quest_wait = number_range(1, 3);
      return;
   }

   selected_range = available_ranges[number_range(0, available_count - 1)];

   if (selected_range == 1)
   {
      highest_level_for_range = highest_mortal;
      hunt_flags = HUNT_WORLD | HUNT_OPENDOOR;
   }
   else if (selected_range == 2)
   {
      highest_level_for_range = highest_remortal;
      hunt_flags = HUNT_WORLD | HUNT_OPENDOOR | HUNT_PICKDOOR;
   }
   else
   {
      highest_level_for_range = highest_adept;
      hunt_flags = HUNT_WORLD | HUNT_OPENDOOR | HUNT_PICKDOOR | HUNT_UNLOCKDOOR;
   }

   quest_personality = selected_range;
   quest_set_effective_crusade_level_range(selected_range, highest_level_for_range, &a, &b);

   while ((quest_mob == NULL) && (loop_counter < 500))
   {
      loop_counter++;
      quest_mob = get_quest_giver(a, b);
      if ((quest_mob == NULL) ||
          ((h_find_dir(get_room_index(ROOM_VNUM_TEMPLE), quest_mob->in_room, hunt_flags) < 0)))
         quest_mob = NULL;
   }

   if (quest_mob == NULL)
   {
      quest = FALSE;
      quest_wait = number_range(1, 3);
      return;
   }

   quest_set_effective_crusade_level_range(selected_range, highest_level_for_range, &a, &b);

   quest_level_min = a;
   quest_level_max = b;

   if ((quest_mob->level < a) || (quest_mob->level > b))
   {
      quest = FALSE;
      quest_wait = number_range(1, 3);
      return;
   }

   loop_counter = 0;
   while ((quest_target == NULL) && (loop_counter < 500))
   {
      loop_counter++;
      quest_target = get_quest_target(a, b);
      if ((quest_target == NULL) ||
          ((h_find_dir(get_room_index(ROOM_VNUM_TEMPLE), quest_target->in_room, hunt_flags) < 0)) ||
          (quest_target == quest_mob))
         quest_target = NULL;
   }

   if (quest_target == NULL)
   {
      quest = FALSE;
      quest_wait = number_range(1, 3);
      return;
   }
   quest_object = load_quest_object(quest_target);
   if (quest_object == NULL)
   {
      quest = FALSE;
      quest_wait = number_range(1, 3);
      return;
   }
   /*
    * Set values on quest item for Qp, Pracs, Exp, Gold
    */
   quest_object->value[0] = UMAX(1, (quest_target->level / 20));
   quest_object->value[1] = UMAX(1, (quest_target->level / 18));
   quest_object->value[2] = (quest_target->level * 20);
   quest_object->value[3] = selected_range;

   if (number_percent() < 10)
   {
      quest_object->value[0] += 2;
      quest_object->value[1] += 3;
      quest_object->value[2] *= 2;
   }

   quest_timer = 0;
   quest = TRUE;
   new_long_desc[0] = '\0';
   if (quest_mob->long_descr_orig != NULL)
      free_string(quest_mob->long_descr_orig);
   quest_mob->long_descr_orig = str_dup(quest_mob->long_descr);
   sprintf(new_long_desc, "%s @@Nsays have you found my %s ?\n\r", quest_mob->short_descr,
           quest_object->short_descr);
   if (quest_mob->long_descr != NULL)
      free_string(quest_mob->long_descr);
   quest_mob->long_descr = str_dup(new_long_desc);
   SET_BIT(quest_mob->act, PLR_NOSUMMON);
   SET_BIT(quest_mob->act, PLR_NOVISIT);
   SET_BIT(quest_mob->act, ACT_NOBLOOD);
   SET_BIT(quest_mob->act, ACT_NO_HUNT);

   new_long_desc[0] = '\0';
   if (quest_target->long_descr_orig != NULL)
      free_string(quest_target->long_descr_orig);
   quest_target->long_descr_orig = str_dup(quest_target->long_descr);
   sprintf(new_long_desc, "%s @@Nsays I stole the %s !!!\n\r", quest_target->short_descr,
           quest_object->short_descr);
   if (quest_target->long_descr != NULL)
      free_string(quest_target->long_descr);
   quest_target->long_descr = str_dup(new_long_desc);

   SET_BIT(quest_target->act, PLR_NOSUMMON);
   SET_BIT(quest_target->act, PLR_NOVISIT);
   SET_BIT(quest_target->act, ACT_NOBLOOD);
   SET_BIT(quest_target->act, ACT_NO_HUNT);

   return;
}
