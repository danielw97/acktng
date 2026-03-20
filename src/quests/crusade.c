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
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include "globals.h"
#include "../npc_dialogue.h"

/**** Local Functions ****/
CHAR_DATA *get_quest_target args((int min_level, int max_level));
CHAR_DATA *get_quest_giver args((void));
OBJ_DATA *load_quest_object args((CHAR_DATA * target));
int quest_is_valid_crusade_mobile args((CHAR_DATA * target, int min_level, int max_level));
void clear_crusade args((void));
sh_int quest_tier_from_level args((int mob_level));
void quest_set_crusade_level_range_for_tier args((sh_int tier, int *minimum_level,
                                                  int *maximum_level));
void quest_set_effective_crusade_level_range args((sh_int tier, int highest_level_in_range,
                                                   int *minimum_level, int *maximum_level));
static void crusade_finalize_quest_start args((void));
static void start_ai_crusade_generation args((void));
static void crusade_build_conversation_context args((void));

/* =========================================================================
 * AI-generated message storage (module-level).
 * ========================================================================= */

static bool using_ai_messages = FALSE;
static char *ai_msg_m1[17];
static char *ai_msg_m2[17];
static char ai_item_short[256];
static char ai_item_long[256];
static char quest_conversation_context[4096];

/* Fallback item names used when AI generation fails or times out */
static const char *fallback_item_names[] = {
    "a worn cloth doll",     "a carved wooden idol",  "a dueling commission",
    "a keeper's survey rod", "a campaign seal",       "an ashfall shard",
    "a faded ledger page",   "a sealed proclamation", NULL};

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
   quest_set_crusade_level_range_for_tier(tier, minimum_level, maximum_level);
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

   if (personality >= 1 && personality <= 8)
      return personality;

   resolved_level = URANGE(1, mob_level, 170);
   return quest_tier_from_level(resolved_level);
}

/* 17 messages, organised by blocks for each personality
   indented messages are for when the target mob gets killed
   Personalities 1-3: original tier-based (wimpy/cool/mean)
   Personalities 4-8: additional fallback styles (scholarly/martial/mercantile/mystical/weary) */
struct qmessage_type
{
   char *const message1;
   char *const message2;
};

const struct qmessage_type qmessages[9][17] = {
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

    },

    /* Personality 4: Scholarly — measured academic, references records and precedent */
    {{"@@lBy my ledger, my %s @@lhas gone missing. I have noted the matter formally.", ""},
     {"@@lThe absence of my %s @@lviolates established protocol. Is anyone investigating?", ""},
     {"@@lAccording to my records, my %s @@lhas not yet been recovered. A disconcerting entry.",
      ""},
     {"@@lI would appreciate a status report on the recovery of my %s@@l.", ""},
     {"@@lThe matter of my %s @@lremains unresolved. I have filed the appropriate notices.", ""},
     {"@@lFor reference: my %s @@lwas last documented in my possession. Leads are lacking.", ""},
     {"@@lI have consulted three precedents for recovering stolen %s@@l. None were encouraging.",
      ""},
     {"@@lRecords confirm: %s @@ltook my %s@@l. I have annotated the relevant section.",
      "@@lThe file on my %s @@lnotes: whoever held it has been struck from the roster."},
     {"@@lI would pay for a certified account of how %s @@lcame to possess my %s@@l.",
      "@@lDeath has closed the file on the thief. My %s @@lstill requires returning."},
     {"@@lI have cross-referenced %s @@lagainst known associates. They hold my %s@@l.",
      "@@lThe thief has been removed from my records. My %s @@lawaits its column."},
     {"@@lThe clock runs on borrowed time. %s @@lstill holds my %s@@l, unlogged.",
      "@@lThe hour grows late for recovering my %s@@l. Update the ledger when it returns."},
     {"@@lI am prepared to award academic credit to whoever extracts my %s @@lfrom %s@@l.",
      "@@lTime remaining is insufficient. Return my %s @@land I will close the account."},
     {"@@lThe case grows cold. %s @@lretains my %s @@lbeyond all documented precedent.",
      "@@lMy %s @@lhas not surfaced. The record will reflect this failure."},
     {"@@lI have little hope remaining for the recovery of my %s @@lfrom %s@@l.",
      "@@lMy %s @@lmay be lost to the archive. A sad footnote."},
     {"@@lI shall close this matter. %s @@lmay retain my %s@@l pending appeals.",
      "@@lI give up the inquiry. My %s @@lshall be filed under: lost."},
     {"@@lI have concluded my formal request. This entry is now closed.", ""},
     {"@@lMy records show that %s @@lhas returned my %s@@l. The account is settled.", ""}},

    /* Personality 5: Martial — soldier/guard, duty-focused, clipped military speech */
    {{"@@gCONTACT. My %s @@ghas been taken. All units be advised.", ""},
     {"@@gStatus: my %s @@gstill missing. Anyone with intel, report in.", ""},
     {"@@gThird report. My %s @@gremains unrecovered. This is unacceptable.", ""},
     {"@@gNeeds recovery: my %s@@g. Form up and search.", ""},
     {"@@gThis is an ongoing action. My %s @@ghas not been secured.", ""},
     {"@@gI need eyes on my %s@@g. Who is on task?", ""},
     {"@@gNo update on my %s@@g. Assign a team.", ""},
     {"@@gIntel confirms: %s @@ghas my %s@@g. Proceed to extract.",
      "@@gContact down. My %s @@gstill outstanding."},
     {"@@gTarget acquired: %s@@g. They hold my %s@@g. Move in.",
      "@@gTarget eliminated. Return my %s @@gto base."},
     {"@@gTarget is %s@@g. Holding my %s@@g. Engage and recover.",
      "@@gTarget down. My %s @@gnot yet returned. Complete the mission."},
     {"@@gTime on target running short. %s @@gstill has my %s@@g.",
      "@@gHalf your window is gone. Return my %s@@g, soldier."},
     {"@@gFinal call: take down %s @@gand return my %s@@g.",
      "@@gTime is critical. Return my %s @@gor this op is a wash."},
     {"@@gMission is compromised. %s @@gstill holds my %s@@g.",
      "@@gMission failure likely. Return my %s @@gnow."},
     {"@@gThis op is falling apart. %s @@gwon't give up my %s@@g.",
      "@@gStand down. My %s @@gis as good as gone."},
     {"@@gCancelling field operation. %s @@gcan keep my %s@@g.",
      "@@gAbort. I never want to see my %s @@gagain."},
     {"@@gAll units: stand down. Op cancelled.", ""},
     {"@@gMission accomplished. %s @@gbrought back my %s@@g. Well done.", ""}},

    /* Personality 6: Mercantile — trader, everything measured in value and debt */
    {{"@@yMy %s @@yhas been taken! At current market rate, this is a significant loss!", ""},
     {"@@yNo takers on recovering my %s@@y? I am offering favourable terms.", ""},
     {"@@yMy %s @@yremains unreturned. The longer this drags, the higher the interest.", ""},
     {"@@yThe value of my %s @@yis appreciating by the minute, and not in my favour.", ""},
     {"@@yI am still open to negotiating a commission for the safe return of my %s@@y.", ""},
     {"@@yAnybody who retrieves my %s @@ywill receive a fair market return. I keep my word.", ""},
     {"@@yNo movement on my %s@@y. The market is not responding.", ""},
     {"@@yI have marked %s @@yas the debtor holding my %s@@y. Time to collect.",
      "@@yThe debtor is deceased. My %s @@yremains the outstanding balance."},
     {"@@yKill %s @@yand recover my %s@@y. Consider it a debt collection.",
      "@@yDebt settled by death. Return my %s @@yand close the ledger."},
     {"@@yThe margin narrows. %s @@yis holding my %s @@yat a loss to everyone.",
      "@@yLoss recorded. Return my %s @@yand I will write it off."},
     {"@@yMy %s @@yis depreciating by the hour while %s @@yhas it.",
      "@@yThe clock is costing me. Liquidate my %s @@ynow."},
     {"@@yFinal offer: kill %s@@y, return my %s@@y, and I'll double the bounty.",
      "@@yLast call. Return my %s @@yor we all lose the investment."},
     {"@@yAt this rate the cost of recovery exceeds the value of my %s@@y.",
      "@@yThis deal is going south. Return my %s @@ybefore it's worthless."},
     {"@@yI am writing this off as a loss. %s @@ycan keep my %s@@y.",
      "@@yBad debt. My %s @@yis off the books."},
     {"@@yI give up! %s @@ycan keep my %s@@y! I'm claiming the insurance!",
      "@@yInsurance claimed. I never want to see my %s @@yagain."},
     {"@@ySettled at a loss. Moving on.", ""},
     {"@@yExcellent transaction, %s@@y! My %s @@yhas been returned. Books balanced.", ""}},

    /* Personality 7: Mystical — prophetic, cryptic, speaks in riddles and visions */
    {{"@@mI have seen it in the smoke: my %s @@mhas been carried off into the dark.", ""},
     {"@@mThe threads unravel. My %s @@mhas passed beyond my sight.", ""},
     {"@@mThe pattern insists: my %s @@mwill return, but only if one brave enough steps forward.",
      ""},
     {"@@mI hear the echo of my %s @@macross the void. It calls out to be found.", ""},
     {"@@mA shadow moves with my %s@@m. Can any among you perceive its path?", ""},
     {"@@mThe stars have aligned in a configuration that suggests my %s @@mmay yet return.", ""},
     {"@@mI have cast three lots. All indicate my %s @@mis near, yet unreachable.", ""},
     {"@@mThe veil has parted: %s @@mholds my %s@@m. The vision is clear.",
      "@@mThe shadow that held my %s @@mhas dissolved. Yet it does not return to my hands."},
     {"@@mSeek the one the smoke names: %s@@m. They carry my %s@@m into the grey.",
      "@@mThe one who walked in shadow is gone. Return my %s @@mand the circle closes."},
     {"@@mI have dreamed %s @@mby name. They hold my %s@@m. The dream does not lie.",
      "@@mThe dreamer who took my %s @@mis no more. The circle waits to close."},
     {"@@mTime thins. %s @@mand my %s @@mmove further into the dark.",
      "@@mThe hour glass empties. Return my %s @@mbefore it is lost to the grey."},
     {"@@mThe pattern frays. %s @@mwill not yield my %s @@mwithout blood.",
      "@@mThe weave holds one thread: return my %s@@m. Do not let it slip."},
     {"@@mI grow weary of reading this particular thread. %s @@mstill holds my %s@@m.",
      "@@mThe vision grows dark. My %s @@mmay not return from where it has gone."},
     {"@@mI release this thread. %s @@mmay keep my %s@@m. The pattern adjusts.",
      "@@mThe smoke does not show my %s @@mreturning. So be it."},
     {"@@mI give up! %s @@mcan keep my %s @@mfor all I care!",
      "@@mI give up! I never want to see my %s @@magain!"},
     {"@@mThe signs have gone silent. I seek no further.", ""},
     {"@@mThe circle is complete, %s@@m. My %s @@mhas returned. The vision is fulfilled.", ""}},

    /* Personality 8: Weary — exhausted bureaucrat, resigned and matter-of-fact */
    {{"My %s is gone. Yes. Again. I have filed the appropriate paperwork.", ""},
     {"Still no sign of my %s. I expected as much.", ""},
     {"My %s remains missing. I find I am not surprised.", ""},
     {"Would anyone care to recover my %s? I cannot say I hold high hopes.", ""},
     {"My %s has not returned. Nor, I suspect, will it any time soon.", ""},
     {"I have put out a notice regarding my %s. I do not expect a response.", ""},
     {"This whole business with my %s is exactly why I stopped expecting things.", ""},
     {"It was %s who took my %s. I note this without expectation that it will matter.",
      "Whoever had my %s has apparently died. I cannot say this surprises me either."},
     {"If someone were to kill %s and return my %s, I would be mildly grateful.",
      "The thief is dead. My %s still hasn't come back. I note this for the record."},
     {"My %s is in the hands of %s. I have not the energy to pursue the matter personally.",
      "The thief is gone. My %s is still out there somewhere. Wonderful."},
     {"%s still has my %s. The hours pass. Nothing changes.",
      "Time is running short. I would like my %s back, for what it's worth."},
     {"I have sent three requests regarding my %s, stolen by %s. All unanswered.",
      "Please return my %s. I am tired and would like to go home."},
     {"%s can keep my %s for all I care at this point.",
      "My %s is gone. I have accepted this. You should too."},
     {"I give up. %s may have my %s. I give up on most things, eventually.",
      "My %s is as good as lost. I will note it and move on."},
     {"I give up! %s can keep my %s for all I care! I'm going to lie down.",
      "I give up. I never want to see my %s again. It was more trouble than it was worth."},
     {"Forget it. I'm done asking. Good day.", ""},
     {"Oh. %s actually returned my %s. How unexpected. Thank you, I suppose.", ""}}

};

static void show_iquest_status(CHAR_DATA *ch)
{
   char buf[MAX_STRING_LENGTH];

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
}

void do_iquest(CHAR_DATA *ch, char *argument)
{
   char buf[MAX_STRING_LENGTH];

   if (argument[0] == '\0' || !str_cmp(argument, "status"))
   {
      show_iquest_status(ch);
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
      int a = 0;
      int b = 0;
      sh_int player_count = 0, average_level = 0, total_levels = 0;
      sh_int tier;

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
         if ((d->connected != CON_PLAYING) || (d->character == NULL) || (IS_IMMORTAL(d->character)))
            continue;
         player_count += 1;
         total_levels += d->character->level;
      }
      average_level =
          (((total_levels == 0) ? 30 : total_levels) / ((player_count == 0) ? 1 : player_count));

      tier = quest_tier_from_level(average_level);
      quest_set_crusade_level_range_for_tier(tier, &a, &b);
      quest_level_min = a;
      quest_level_max = b;

      quest_target = get_quest_target(a, b);
      if (quest_target == NULL)
      {
         send_to_char("Failed to find a quest target\n\r", ch);
         return;
      }
      quest_personality = quest_tier_from_level(quest_target->level);

      quest_mob = get_quest_giver();
      if (quest_mob == NULL || quest_mob == quest_target)
      {
         send_to_char("Failed to find a quest mob\n\r", ch);
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

      send_to_char("QUEST STARTING — AI generation in progress...\n\r\n\r", ch);

      sprintf(buf, "The questing mobile is: %s [In Room %d]\n\r", quest_mob->short_descr,
              quest_mob->in_room->vnum);
      send_to_char(buf, ch);

      sprintf(buf, "Target Mobile is: %s [In Room %d]\n\r", quest_target->short_descr,
              quest_target->in_room->vnum);
      send_to_char(buf, ch);

      sprintf(buf, "Quest Object is worth: %d QP, %d Prac, %d GP (item name pending AI)\n\r",
              quest_object->value[0], quest_object->value[1], quest_object->value[2]);
      send_to_char(buf, ch);

      sprintf(buf, "Quest level range is: %d to %d.\n\r", quest_level_min, quest_level_max);
      send_to_char(buf, ch);

      /* Defer finalize to AI generation; crusade_ai_poll() will call
       * crusade_finalize_quest_start() once the child completes (or times out). */
      start_ai_crusade_generation();
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

   pObj = get_obj_index(OBJ_VNUM_QUEST_ITEM);

   if (pObj == NULL)
   {
      bug("load_quest_object : Invalid object vnum %d.", OBJ_VNUM_QUEST_ITEM);
      return NULL;
   }

   object = create_object(pObj, 1);
   obj_to_char(object, target);

   return object;
}

CHAR_DATA *get_quest_giver(void)
{
   CHAR_DATA *candidates[256];
   int count = 0;
   CHAR_DATA *t;

   for (t = first_char; t != NULL; t = t->next)
   {
      if (!IS_NPC(t))
         continue;
      if (!IS_SET(t->act, ACT_AI_DIALOGUE))
         continue;
      if (IS_SET(t->in_room->area->flags, AREA_NOSHOW))
         continue;
      if (IS_SET(t->act, ACT_SENTINEL))
         continue;
      if (IS_SET(t->act, ACT_PET))
         continue;
      if (IS_SET(t->act, ACT_INVASION))
         continue;
      if (!str_cmp(rev_spec_lookup(t->spec_fun), "spec_stephen"))
         continue;
      if (!str_cmp(rev_spec_lookup(t->spec_fun), "spec_tax_man"))
         continue;
      if (count < 256)
         candidates[count++] = t;
   }

   if (count == 0)
      return NULL;
   return candidates[number_range(0, count - 1)];
}

/* =========================================================================
 * Crusade AI generation: minimal JSON string extractor.
 * ========================================================================= */

static bool json_extract_string(const char *json, const char *key, char *buf, size_t cap)
{
   char search[128];
   const char *p;
   const char *end;
   size_t len;
   size_t di;
   size_t si;

   buf[0] = '\0';
   snprintf(search, sizeof(search), "\"%s\"", key);
   p = strstr(json, search);
   if (p == NULL)
      return FALSE;

   p += strlen(search);
   while (*p == ' ' || *p == '\t' || *p == ':' || *p == '\n' || *p == '\r')
      p++;
   if (*p != '"')
      return FALSE;

   p++; /* skip opening quote */
   end = p;
   while (*end && *end != '"')
   {
      if (*end == '\\')
         end++;
      if (*end)
         end++;
   }

   len = (size_t)(end - p);
   if (len >= cap)
      len = cap - 1;

   di = 0;
   for (si = 0; si < len && di + 1 < cap; si++)
   {
      if (p[si] == '\\' && si + 1 < len)
      {
         si++;
         if (p[si] == 'n')
            buf[di++] = '\n';
         else if (p[si] == '"')
            buf[di++] = '"';
         else if (p[si] == '\\')
            buf[di++] = '\\';
         else
            buf[di++] = p[si];
      }
      else
      {
         buf[di++] = p[si];
      }
   }
   buf[di] = '\0';
   return (di > 0);
}

/* =========================================================================
 * Crusade conversation context builder.
 * Called each crusade_inform() tick so dialogue dispatch has fresh context.
 * ========================================================================= */

static void crusade_build_conversation_context(void)
{
   const char *item_name = quest_object ? quest_object->short_descr : "the item";
   const char *target_name = quest_target ? quest_target->short_descr : "an unknown thief";
   const char *giver_area = (quest_mob != NULL && quest_mob->in_room != NULL && quest_mob->in_room->area != NULL)
                                ? quest_mob->in_room->area->name
                                : "an unknown area";
   const char *target_area = (quest_target != NULL && quest_target->in_room != NULL && quest_target->in_room->area != NULL)
                                 ? quest_target->in_room->area->name
                                 : "an unknown area";

   if (quest_target != NULL)
   {
      snprintf(quest_conversation_context, sizeof(quest_conversation_context),
               "You are the quest giver in an active crusade. "
               "You are currently located in '%s'. "
               "Your '%s' was stolen by '%s', who is in '%s'. "
               "You have been seeking its return for %d of 15 minutes. "
               "You want adventurers to kill the thief and return the item.",
               giver_area, item_name, target_name, target_area, quest_timer);
   }
   else
   {
      snprintf(quest_conversation_context, sizeof(quest_conversation_context),
               "You are the quest giver in an active crusade. "
               "You are currently located in '%s'. "
               "Your '%s' was stolen and the thief has been slain. "
               "You have been waiting %d of 15 minutes for someone to return it to you.",
               giver_area, item_name, quest_timer);
   }
}

/* =========================================================================
 * Crusade dialogue dispatch — route player crusade messages to quest NPC.
 * Called from do_crusade() when a player sends a message and a quest is live.
 * ========================================================================= */

void crusade_dialogue_dispatch(CHAR_DATA *npc, CHAR_DATA *ch, const char *argument)
{
   if (npc == NULL || ch == NULL || argument == NULL || argument[0] == '\0')
      return;

   crusade_build_conversation_context();
   npc_dialogue_dispatch_crusade(npc, ch, argument, quest_conversation_context);
}

/* =========================================================================
 * Read AI generation output from the temp file.
 * Returns TRUE on success; populates ai_msg_m1/m2 and ai_item_short/long.
 * ========================================================================= */

static bool read_ai_crusade_results(void)
{
   FILE *fp;
   long size;
   char *json;
   char key[32];
   char val[1024];
   int i;
   const char *p;

   fp = fopen(quest_ai_tmpfile, "r");
   if (fp == NULL)
      return FALSE;

   fseek(fp, 0, SEEK_END);
   size = ftell(fp);
   rewind(fp);

   if (size <= 0 || size > 1024 * 128)
   {
      fclose(fp);
      return FALSE;
   }

   json = malloc((size_t)size + 1);
   if (json == NULL)
   {
      fclose(fp);
      return FALSE;
   }

   size = (long)fread(json, 1, (size_t)size, fp);
   json[size] = '\0';
   fclose(fp);

   /* Extract personality (integer field) */
   p = strstr(json, "\"personality\":");
   if (p != NULL)
   {
      sh_int new_personality;
      p += strlen("\"personality\":");
      while (*p == ' ' || *p == '\t')
         p++;
      new_personality = (sh_int)atoi(p);
      if (new_personality >= 1 && new_personality <= 8)
         quest_personality = new_personality;
   }

   /* Extract item names */
   if (!json_extract_string(json, "item_short", ai_item_short, sizeof(ai_item_short)))
      ai_item_short[0] = '\0';
   if (!json_extract_string(json, "item_long", ai_item_long, sizeof(ai_item_long)))
      ai_item_long[0] = '\0';

   /* Extract 17 message pairs */
   for (i = 0; i < 17; i++)
   {
      free(ai_msg_m1[i]);
      free(ai_msg_m2[i]);
      ai_msg_m1[i] = NULL;
      ai_msg_m2[i] = NULL;

      snprintf(key, sizeof(key), "m1_%d", i);
      if (json_extract_string(json, key, val, sizeof(val)) && val[0] != '\0')
         ai_msg_m1[i] = strdup(val);

      snprintf(key, sizeof(key), "m2_%d", i);
      if (json_extract_string(json, key, val, sizeof(val)) && val[0] != '\0')
         ai_msg_m2[i] = strdup(val);
   }

   free(json);
   return TRUE;
}

/* =========================================================================
 * Finalize quest start: rename object, update long descs, set bits.
 * Called by crusade_ai_poll() once the child exits (or times out).
 * ========================================================================= */

static void crusade_finalize_quest_start(void)
{
   char new_long_desc[MAX_STRING_LENGTH];
   const char *item_name;
   int fallback_count = 0;

   /* Clean up AI generation state */
   quest_ai_pending = FALSE;
   if (quest_ai_tmpfile[0] != '\0')
   {
      unlink(quest_ai_tmpfile);
      quest_ai_tmpfile[0] = '\0';
   }

   /* Pick item name: AI-generated or random fallback */
   if (using_ai_messages && ai_item_short[0] != '\0')
   {
      item_name = ai_item_short;
   }
   else
   {
      while (fallback_item_names[fallback_count] != NULL)
         fallback_count++;
      item_name = fallback_item_names[number_range(0, fallback_count - 1)];
   }

   /* Rename quest object */
   if (quest_object != NULL)
   {
      free_string(quest_object->name);
      quest_object->name = str_dup(item_name);
      free_string(quest_object->short_descr);
      quest_object->short_descr = str_dup(item_name);
      if (using_ai_messages && ai_item_long[0] != '\0')
      {
         free_string(quest_object->description);
         quest_object->description = str_dup(ai_item_long);
      }
   }

   /* Update quest_mob long desc and flags */
   if (quest_mob != NULL)
   {
      if (quest_mob->long_descr_orig != NULL)
         free_string(quest_mob->long_descr_orig);
      quest_mob->long_descr_orig = str_dup(quest_mob->long_descr);
      snprintf(new_long_desc, sizeof(new_long_desc), "%s @@Nsays have you found my %s ?\n\r",
               quest_mob->short_descr, item_name);
      free_string(quest_mob->long_descr);
      quest_mob->long_descr = str_dup(new_long_desc);
      SET_BIT(quest_mob->act, PLR_NOSUMMON);
      SET_BIT(quest_mob->act, PLR_NOVISIT);
      SET_BIT(quest_mob->act, ACT_NOBLOOD);
      SET_BIT(quest_mob->act, ACT_NO_HUNT);
   }

   /* Update quest_target long desc and flags */
   if (quest_target != NULL)
   {
      if (quest_target->long_descr_orig != NULL)
         free_string(quest_target->long_descr_orig);
      quest_target->long_descr_orig = str_dup(quest_target->long_descr);
      snprintf(new_long_desc, sizeof(new_long_desc), "%s @@Nsays I stole the %s !!!\n\r",
               quest_target->short_descr, item_name);
      free_string(quest_target->long_descr);
      quest_target->long_descr = str_dup(new_long_desc);
      SET_BIT(quest_target->act, PLR_NOSUMMON);
      SET_BIT(quest_target->act, PLR_NOVISIT);
      SET_BIT(quest_target->act, ACT_NOBLOOD);
      SET_BIT(quest_target->act, ACT_NO_HUNT);
   }

   quest_timer = 0;
   quest = TRUE;
   log_f("crusade: quest started (item='%s', mob='%s', target='%s', ai=%s)", item_name,
         quest_mob ? quest_mob->short_descr : "NULL",
         quest_target ? quest_target->short_descr : "NULL", using_ai_messages ? "yes" : "no");
}

/* =========================================================================
 * Fork the AI generation script.
 * Populates quest_ai_pid, quest_ai_tmpfile, quest_ai_start_time.
 * Sets quest_ai_pending = TRUE on success.
 * ========================================================================= */

static void start_ai_crusade_generation(void)
{
   char tmpfile_template[256];
   char level_str[16];
   char tier_str[8];
   const char *mob_name;
   const char *area_name;
   pid_t pid;
   int fd;

   /* Create temp file for AI output */
   snprintf(tmpfile_template, sizeof(tmpfile_template), "/tmp/crusade_ai_XXXXXX");
   fd = mkstemp(tmpfile_template);
   if (fd < 0)
   {
      log_f("start_ai_crusade_generation: mkstemp failed");
      using_ai_messages = FALSE;
      crusade_finalize_quest_start();
      return;
   }
   close(fd);
   strncpy(quest_ai_tmpfile, tmpfile_template, sizeof(quest_ai_tmpfile) - 1);
   quest_ai_tmpfile[sizeof(quest_ai_tmpfile) - 1] = '\0';

   mob_name = (quest_mob != NULL) ? quest_mob->short_descr : "unknown";
   area_name = (quest_mob != NULL && quest_mob->in_room != NULL && quest_mob->in_room->area != NULL)
                   ? quest_mob->in_room->area->name
                   : "unknown";
   snprintf(level_str, sizeof(level_str), "%d", quest_mob ? quest_mob->level : 1);
   snprintf(tier_str, sizeof(tier_str), "%d", (int)quest_personality);

   pid = fork();
   if (pid < 0)
   {
      log_f("start_ai_crusade_generation: fork failed");
      unlink(quest_ai_tmpfile);
      quest_ai_tmpfile[0] = '\0';
      using_ai_messages = FALSE;
      crusade_finalize_quest_start();
      return;
   }

   if (pid == 0)
   {
      /* Child: exec python3 crusade_ai_gen.py mob_name level tier area_name output_path */
      execl("/usr/bin/python3", "python3", CRUSADE_AI_SCRIPT, mob_name, level_str, tier_str,
            area_name, quest_ai_tmpfile, NULL);
      /* exec failed — try python3 on PATH */
      execlp("python3", "python3", CRUSADE_AI_SCRIPT, mob_name, level_str, tier_str, area_name,
             quest_ai_tmpfile, NULL);
      _exit(1);
   }

   /* Parent */
   quest_ai_pid = pid;
   quest_ai_start_time = time(NULL);
   quest_ai_pending = TRUE;
   log_f("crusade_ai: generation started (pid %d, tmpfile %s)", (int)pid, quest_ai_tmpfile);
}

/* =========================================================================
 * Poll the AI generation child.  Called each tick by quest_update() while
 * quest_ai_pending is TRUE.
 * ========================================================================= */

void crusade_ai_poll(void)
{
   int status;
   pid_t result;

   if (!quest_ai_pending)
      return;

   /* Timeout check */
   if (quest_ai_pid > 0 && (time(NULL) - quest_ai_start_time) > CRUSADE_AI_TIMEOUT)
   {
      log_f("crusade_ai: timeout — killing child %d", (int)quest_ai_pid);
      kill(quest_ai_pid, SIGKILL);
      waitpid(quest_ai_pid, NULL, 0);
      quest_ai_pid = -1;
      using_ai_messages = FALSE;
      crusade_finalize_quest_start();
      return;
   }

   /* Non-blocking wait */
   result = waitpid(quest_ai_pid, &status, WNOHANG);
   if (result == 0)
      return; /* still running */

   quest_ai_pid = -1;

   if (result < 0)
   {
      log_f("crusade_ai: waitpid error");
      using_ai_messages = FALSE;
   }
   else if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
   {
      if (read_ai_crusade_results())
      {
         using_ai_messages = TRUE;
         log_f("crusade_ai: generation complete (personality %d, item '%s')",
               (int)quest_personality, ai_item_short);
      }
      else
      {
         log_f("crusade_ai: failed to read results, using fallback");
         using_ai_messages = FALSE;
      }
   }
   else
   {
      log_f("crusade_ai: child exited with error status, using fallback");
      using_ai_messages = FALSE;
   }

   crusade_finalize_quest_start();
}

/*
 * crusade_inform : Makes the questing mobile give out information to the
 * players on the mud.  Starts off real simple, and gets more helpful as
 * time runs out :P
 */

void crusade_inform(void)
{
   char buf[MAX_STRING_LENGTH];
   sh_int active_personality;

   active_personality =
       quest_resolve_crusade_personality(quest_personality, quest_mob ? quest_mob->level : 1);
   quest_personality = active_personality;

   if (using_ai_messages)
   {
      const char *m;

      /* AI messages already have the item/target names embedded (via %s substitution) */
      if (quest_timer < 7 || !quest_target)
      {
         m = ai_msg_m1[quest_timer] ? ai_msg_m1[quest_timer] : "";
         format_quest_message(buf, m, quest_target ? quest_target->short_descr : "",
                              quest_object ? quest_object->short_descr : "");
      }
      else
      {
         m = ai_msg_m1[quest_timer] ? ai_msg_m1[quest_timer] : "";
         format_quest_message(buf, m, quest_target->short_descr,
                              quest_object ? quest_object->short_descr : "");
      }
      /* If target just died use m2 */
      if (!quest_target && ai_msg_m2[quest_timer] && ai_msg_m2[quest_timer][0] != '\0')
      {
         m = ai_msg_m2[quest_timer];
         format_quest_message(buf, m, quest_object ? quest_object->short_descr : "", NULL);
      }
   }
   else
   {
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
   }

   /* Rebuild conversation context for player dialogue each tick */
   crusade_build_conversation_context();

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
   sh_int active_personality;
   char buf[MAX_STRING_LENGTH];
   const char *msg;

   active_personality =
       quest_resolve_crusade_personality(quest_personality, quest_mob ? quest_mob->level : 1);
   quest_personality = active_personality;

   if (using_ai_messages && ai_msg_m1[16] && ai_msg_m1[16][0] != '\0')
      msg = ai_msg_m1[16];
   else
      msg = qmessages[active_personality][16].message1;

   format_quest_message(buf, msg, NAME(ch), quest_object->short_descr);
   do_crusade(quest_mob, buf);
   clear_crusade();
   return;
}

void crusade_cancel()
{
   char buf[MAX_STRING_LENGTH];
   sh_int active_personality;
   const char *msg;

   if (quest_mob)
   {
      active_personality =
          quest_resolve_crusade_personality(quest_personality, quest_mob ? quest_mob->level : 1);
      if (using_ai_messages && ai_msg_m1[14] && ai_msg_m1[14][0] != '\0')
      {
         msg = ai_msg_m1[14];
         format_quest_message(buf, msg, quest_target ? quest_target->short_descr : "",
                              quest_object ? quest_object->short_descr : "");
      }
      else
      {
         msg = qmessages[active_personality][14].message1;
         format_quest_message(buf, msg, quest_target ? quest_target->short_descr : "",
                              quest_object ? quest_object->short_descr : "");
         if (buf[0] == '\0')
            strncpy(buf, "Shoot! Just forget about recovering ANYTHING for me, ok?",
                    sizeof(buf) - 1);
      }
      do_crusade(quest_mob, buf);
   }

   clear_crusade();
   return;
}

void clear_crusade()
{
   int i;

   /*
    * Free AI-generated messages
    */
   for (i = 0; i < 17; i++)
   {
      free(ai_msg_m1[i]);
      free(ai_msg_m2[i]);
      ai_msg_m1[i] = NULL;
      ai_msg_m2[i] = NULL;
   }
   using_ai_messages = FALSE;
   ai_item_short[0] = '\0';
   ai_item_long[0] = '\0';
   quest_conversation_context[0] = '\0';

   /* Kill any still-running AI child */
   if (quest_ai_pid > 0)
   {
      kill(quest_ai_pid, SIGKILL);
      waitpid(quest_ai_pid, NULL, 0);
      quest_ai_pid = -1;
   }
   if (quest_ai_tmpfile[0] != '\0')
   {
      unlink(quest_ai_tmpfile);
      quest_ai_tmpfile[0] = '\0';
   }
   quest_ai_pending = FALSE;
   quest_ai_start_time = 0;

   /*
    * Clear ALL values, ready for next quest
    */

   quest = FALSE;
   if (quest_object != NULL)
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

   int hunt_flags = 0;
   sh_int loop_counter = 0;

   int a = 0;
   int b = 0;
   sh_int player_count = 0;
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
      hunt_flags = HUNT_WORLD | HUNT_OPENDOOR;
   else if (selected_range == 2)
      hunt_flags = HUNT_WORLD | HUNT_OPENDOOR | HUNT_PICKDOOR;
   else
      hunt_flags = HUNT_WORLD | HUNT_OPENDOOR | HUNT_PICKDOOR | HUNT_UNLOCKDOOR;

   quest_set_crusade_level_range_for_tier(selected_range, &a, &b);
   quest_level_min = a;
   quest_level_max = b;

   loop_counter = 0;
   while ((quest_target == NULL) && (loop_counter < 500))
   {
      loop_counter++;
      quest_target = get_quest_target(a, b);
      if ((quest_target == NULL) ||
          ((h_find_dir(get_room_index(ROOM_VNUM_TEMPLE), quest_target->in_room, hunt_flags) < 0)))
         quest_target = NULL;
   }

   if (quest_target == NULL)
   {
      quest = FALSE;
      quest_wait = number_range(1, 3);
      return;
   }
   quest_personality = quest_tier_from_level(quest_target->level);

   quest_mob = get_quest_giver();
   if (quest_mob == NULL || quest_mob == quest_target)
   {
      quest = FALSE;
      quest_wait = number_range(1, 3);
      return;
   }

   if (h_find_dir(get_room_index(ROOM_VNUM_TEMPLE), quest_mob->in_room, hunt_flags) < 0)
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

   /* Defer finalize to AI generation; crusade_ai_poll() will call
    * crusade_finalize_quest_start() once the child completes (or times out). */
   start_ai_crusade_generation();
   return;
}
