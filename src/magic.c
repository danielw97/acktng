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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "globals.h"
#include "tables.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif

/*
 * Local functions.
 */
void say_spell args((CHAR_DATA * ch, int sn));
void aoe_damage(CHAR_DATA *ch, OBJ_DATA *obj, int sn, int level, int min_dam, int max_dam,
                int element, int flags);
#define AOE_SAVES (1 << 0)
#define AOE_SKIP_GROUP (1 << 1)

static bool is_healing_spell(int sn)
{
   if (sn < 0 || sn >= MAX_SKILL)
      return FALSE;

   if (skill_table[sn].name == NULL || skill_table[sn].spell_fun == NULL)
      return FALSE;

   return !str_cmp(skill_table[sn].name, "cure light") ||
          !str_cmp(skill_table[sn].name, "cure serious") ||
          !str_cmp(skill_table[sn].name, "cure critical") ||
          !str_cmp(skill_table[sn].name, "heal") || !str_cmp(skill_table[sn].name, "group heal") ||
          !str_cmp(skill_table[sn].name, "psionic recovery") ||
          !str_cmp(skill_table[sn].name, "regen") || !str_cmp(skill_table[sn].name, "influx") ||
          !str_cmp(skill_table[sn].name, "healing light");
}

static void gain_holy_power_from_healing_spell(CHAR_DATA *ch, int sn)
{
   if (ch == NULL || IS_NPC(ch))
      return;

   if (!is_healing_spell(sn))
      return;

   ch->holy_power = UMIN(ch->holy_power + 1, 100 / 5);
}

static bool npc_remort_cast_blocked(const CHAR_DATA *ch, int sn)
{
   if (!IS_NPC(ch))
      return FALSE;

   if (SKILL_TIER(sn) != REMORT && SKILL_TIER(sn) != ADEPT)
      return FALSE;

   return IS_SET(ch->act, ACT_PET) || IS_AFFECTED(ch, AFF_CHARM);
}

#ifdef UNIT_TEST_MAGIC
bool magic_test_npc_remort_cast_blocked_flags(int flag1, int act_bits, int affected_by_bits)
{
   if (flag1 != REMORT && flag1 != ADEPT)
      return FALSE;

   return IS_SET(act_bits, ACT_PET) || IS_SET(affected_by_bits, AFF_CHARM);
}
#endif

/* Calculate mana cost */
int mana_cost(CHAR_DATA *ch, int sn)
{
   bool can_cast = can_use_skill(ch, sn);
   int cost;

   if (skill_table[sn].min_mana < 1)
      return 0;

   if ((!can_cast) && (IS_NPC(ch)))
      return 1000;
   else if ((!can_cast) &&
            ((!IS_NPC(ch)) && (!is_name(skill_table[sn].name, race_table[ch->race].skill))))
      return 1000;

   cost = 1000;

   if (can_cast)
      cost = skill_table[sn].min_mana;

   if (IS_NPC(ch))
      cost /= 2;
   if (!IS_NPC(ch) && (skill_table[sn].flag2 == NORM))
   {
      if (IS_SET(race_table[ch->race].race_flags, RACE_MOD_NO_MAGIC))
         cost *= 1.75;
      else if (IS_SET(race_table[ch->race].race_flags, RACE_MOD_WEAK_MAGIC))
         cost *= 1.25;
      else if (IS_SET(race_table[ch->race].race_flags, RACE_MOD_STRONG_MAGIC))
         cost *= .60;
   }

   if ((skill_table[sn].flag2 == NORM) && (is_affected(ch, skill_lookup("mystical focus"))))
   {
      cost *= 2.5;
   }

   if (cost < 50)
      cost = 50;

   cost -= cost * (get_curr_wis(ch) + get_curr_int(ch)) / 100;

   /* Mental power: 2% mana reduction per stack for psi/nec/ego/kin spells */
   if (!IS_NPC(ch) && is_mental_power_spell(sn) && ch->mental_power > 0)
      cost -= cost * (ch->mental_power * 2) / 100;

   if ((!IS_NPC(ch)) && (is_name(skill_table[sn].name, race_table[ch->race].skill)))
      cost = 10;

   return cost;
}

/*
 * Lookup a skill by name.
 */
int skill_lookup(const char *name)
{
   int sn;

   if (strlen(name) < 1)
      return -1;

   for (sn = 0; sn < MAX_SKILL; sn++)
   {
      if (skill_table[sn].name == NULL)
         continue;
      if (!str_cmp(name, skill_table[sn].name))
         return sn;
   }

   for (sn = 0; sn < MAX_SKILL; sn++)
   {
      if (skill_table[sn].name == NULL)
         continue;
      if (!str_prefix(name, skill_table[sn].name))
         return sn;
   }

   return -1;
}

bool is_arcane_spell(int sn)
{
   if (sn < 0 || sn >= MAX_SKILL)
      return FALSE;

   if (skill_table[sn].name == NULL || skill_table[sn].spell_fun == NULL)
      return FALSE;

   return skill_table[sn].skill_level[CLASS_MAG] >= 0 ||
          skill_table[sn].skill_level[CLASS_SOR] >= 0 ||
          skill_table[sn].skill_level[CLASS_WIZ] >= 0 ||
          skill_table[sn].skill_level[CLASS_GMA] >= 0;
}

bool is_holy_power_spell(int sn)
{
   if (sn < 0 || sn >= MAX_SKILL)
      return FALSE;

   if (skill_table[sn].name == NULL || skill_table[sn].spell_fun == NULL)
      return FALSE;

   return skill_table[sn].skill_level[CLASS_CLE] >= 0 ||
          skill_table[sn].skill_level[CLASS_PRI] >= 0 ||
          skill_table[sn].skill_level[CLASS_PAL] >= 0 ||
          skill_table[sn].skill_level[CLASS_TEM] >= 0;
}
bool is_mental_power_spell(int sn)
{
   if (sn < 0 || sn >= MAX_SKILL)
      return FALSE;

   if (skill_table[sn].name == NULL || skill_table[sn].spell_fun == NULL)
      return FALSE;

   /* CLASS_PSI=4 (mortal), CLASS_NEC=4 (remort), CLASS_KIN=4 (adept) share index 4.
    * CLASS_EGO=10 (remort) has a unique index. */
   return skill_table[sn].skill_level[CLASS_PSI] >= 0 ||
          skill_table[sn].skill_level[CLASS_EGO] >= 0;
}

/*
 * Lookup a skill by slot number.
 * Used for object loading.
 */
int slot_lookup(int slot)
{
   extern bool fBootDb;
   int sn;

   if (slot <= 0)
      return -1;

   for (sn = 0; sn < MAX_SKILL; sn++)
   {
      if (slot == skill_table[sn].slot)
         return sn;
   }

   if (fBootDb)
   {
      bug("Slot_lookup: bad slot %d.", slot);
   }

   return -1;
}

/*
 * Utter mystical words for an sn.
 */
static void format_spell_utterance(char *dest, size_t dest_size, const char *words)
{
   snprintf(dest, dest_size, "$n utters the words, '%.*s'.", (int)dest_size - 26, words);
}

#ifdef UNIT_TEST_MAGIC
void magic_test_format_spell_utterance(char *dest, size_t dest_size, const char *words)
{
   format_spell_utterance(dest, dest_size, words);
}
#endif

void say_spell(CHAR_DATA *ch, int sn)
{
   char buf[MAX_STRING_LENGTH];
   char buf2[MAX_STRING_LENGTH];
   char msg[MAX_STRING_LENGTH];
   char msg2[MAX_STRING_LENGTH];
   CHAR_DATA *rch;
   char *pName;
   int iSyl;
   int length;

   struct syl_type
   {
      char *old;
      char *new;
   };

   static const struct syl_type syl_table[] = {
       {" ", " "},      {"ar", "abra"},  {"au", "kada"},   {"bless", "fido"}, {"blind", "nose"},
       {"bur", "mosa"}, {"cu", "judi"},  {"de", "oculo"},  {"en", "unso"},    {"light", "dies"},
       {"lo", "hi"},    {"mor", "zak"},  {"move", "sido"}, {"ness", "lacri"}, {"ning", "illa"},
       {"per", "duda"}, {"ra", "gru"},   {"re", "candus"}, {"son", "sabru"},  {"tect", "infra"},
       {"tri", "cula"}, {"ven", "nofo"}, {"a", "a"},       {"b", "b"},        {"c", "q"},
       {"d", "e"},      {"e", "z"},      {"f", "y"},       {"g", "o"},        {"h", "p"},
       {"i", "u"},      {"j", "y"},      {"k", "t"},       {"l", "r"},        {"m", "w"},
       {"n", "i"},      {"o", "a"},      {"p", "s"},       {"q", "d"},        {"r", "f"},
       {"s", "g"},      {"t", "h"},      {"u", "j"},       {"v", "z"},        {"w", "x"},
       {"x", "n"},      {"y", "l"},      {"z", "k"},       {"", ""}};

   buf[0] = '\0';
   for (pName = skill_table[sn].name; *pName != '\0'; pName += length)
   {
      for (iSyl = 0; (length = strlen(syl_table[iSyl].old)) != 0; iSyl++)
      {
         if (!str_prefix(syl_table[iSyl].old, pName))
         {
            strcat(buf, syl_table[iSyl].new);
            break;
         }
      }

      if (length == 0)
         length = 1;
   }
   /*
    * Give some other message, according to TAR type and if !NPC
    */
   switch (skill_table[sn].target)
   {
   case TAR_IGNORE:
      sprintf(msg, "$n closes $s eyes, and prays.");
      sprintf(msg2, "You close your eyes and pray.\n\r");
      break;
   case TAR_CHAR_OFFENSIVE:
      sprintf(msg, "$n's eyes glow red for an instant!");
      sprintf(msg2, "Your eyes glow red for an instant!\n\r");
      break;
   case TAR_CHAR_DEFENSIVE:
      sprintf(msg, "$n falls silent as $e meditates.");
      sprintf(msg2, "You become silent as you meditate.\n\r");
      break;
   case TAR_CHAR_SELF:
      sprintf(msg, "$n motions toward $mself.");
      sprintf(msg2, "You motion towards yourself.\n\r");
      break;
   case TAR_CHAR_NOTSELF:
      sprintf(msg, "$n makes hand motions.");
      sprintf(msg2, "You make hand motions.\n\r");
      break;
   case TAR_OBJ_INV:
      sprintf(msg, "$n's hands briefly glow magically!");
      sprintf(msg2, "Your hands briefly glow magically!\n\r");
   }
   act(msg, ch, NULL, ch, TO_NOTVICT);
   send_to_char(msg2, ch);

   format_spell_utterance(buf2, sizeof(buf2), buf);
   format_spell_utterance(buf, sizeof(buf), skill_table[sn].name);

   for (rch = ch->in_room->first_person; rch; rch = rch->next_in_room)
   {
      if (rch != ch)
         act(ch->class == rch->class ? buf : buf2, ch, NULL, rch, TO_VICT);
   }

   return;
}

/*
 * Compute a saving throw.
 * Negative apply's make saving throw better.
 */
bool saves_spell(int level, CHAR_DATA *victim)
{
   int save;
   bool saved = FALSE;
   save = 40 + ((get_psuedo_level(victim) > 60 ? get_psuedo_level(victim) * 2 / 3
                                               : get_psuedo_level(victim)) -
                level - URANGE(-40, victim->saving_throw, 40));
   if ((IS_NPC(victim)) && (IS_SET(victim->act, ACT_SOLO)))
      save += 20;
   if ((IS_NPC(victim)) && (IS_SET(victim->act, ACT_BOSS)))
      save += 30;
   if (!IS_NPC(victim) && (IS_SET(race_table[victim->race].race_flags, RACE_MOD_RESIST_MAGIC)))
      save += 20;
   save = URANGE(5, save, 95);

   if (number_percent() < save)
      saved = TRUE;
#if 0
   sprintf( log_buf, "%s lvl %d wismod %d savemod %d save total %d against level %d, %s ).",
            victim->name, get_psuedo_level( victim ),
            wis_app[get_curr_wis( victim )].spell_save,
            victim->saving_throw, save, level, ( saved ? "@@aSAVED@@N" : "@@eFAILED@@N" ) );
   monitor_chan( log_buf, MONITOR_MAGIC );
#endif

   return (saved);
}

/*
 * The kludgy global is for spells who want more stuff from command line.
 */
char *target_name;

void do_cast(CHAR_DATA *ch, char *argument)
{
   char *typed = argument;
   char arg1[MAX_INPUT_LENGTH];
   char arg2[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   OBJ_DATA *obj;
   void *vo;
   int mana;
   int sn;
   int best; /* The best class to use for the job :P */
   bool char_login = FALSE;
   int cast_chance = 0;
   bool multi_cast = FALSE;
   if (ch->is_free == TRUE)
      return;
   /* ZENFIX --ch's are surviving multiple kills per combat round */

   if ((!IS_NPC(ch)) && (ch->desc != NULL) && (ch->desc->connected == CON_SETTING_STATS))
      char_login = TRUE;

   /*
    * Modified to reference ch's highest ch->class_level[] value which
    * * apllies to the spell.  -S-
    */

   /*
    * Only scripted mobs not charmed can cast spells
    * like PC's -
    * -S- Mod: intelligent mobs can cast too.    NOT ANYMORE :P  Zen
    */
   /*
    * if ( IS_NPC(ch)
    * && ( !IS_SET( ch->act, ACT_IS_NPC ) )
    * && ( !ch->pIndexData->progtypes
    * || IS_AFFECTED( ch, AFF_CHARM ) ) )
    * return;
    */

   target_name = one_argument(argument, arg1);
   one_argument(target_name, arg2);

   if (arg1[0] == '\0')
   {
      send_to_char("Cast which what where?\n\r", ch);
      return;
   }

   best = -1; /* Default 'no way' value */

   if ((sn = skill_lookup(arg1)) == -1)
   {
      send_to_char("Wiggle swiggle biggle?\n\r", ch);
      return;
   }

   /*
    * Check if in a no-magic room
    */
   if (!char_login && IS_SET(ch->in_room->room_flags, ROOM_NO_MAGIC))
   {
      send_to_char("Some strange force prevents you casting the spell!\n\r", ch);
      return;
   }

   if (ch->cooldown[sn] > 0)
   {
      send_to_char("That spell is still on cooldown.\n\r", ch);
      return;
   }

   if (npc_remort_cast_blocked(ch, sn))
      return;

   if (!can_use_skill(ch, sn))
   {
      send_to_char("You can't do that.\n\r", ch);
      return;
   }

   if (ch->position < skill_table[sn].minimum_position)
   {
      send_to_char("You can't concentrate enough.\n\r", ch);
      return;
   }
   mana = mana_cost(ch, sn);
   raise_skill(ch, sn);

   if (char_login)
      mana = 0;

   /*
    * Locate targets.
    */
   victim = NULL;
   obj = NULL;
   vo = NULL;

   switch (skill_table[sn].target)
   {
   default:
      bug("Do_cast: bad target for sn %d.", sn);
      return;

   case TAR_IGNORE:
      break;

   case TAR_CHAR_OFFENSIVE:
      if (arg2[0] == '\0')
      {
         if ((victim = ch->fighting) == NULL)
         {
            send_to_char("Cast the spell on whom?\n\r", ch);
            return;
         }
      }
      else
      {
         if ((victim = get_char_room(ch, arg2)) == NULL)
         {
            send_to_char("They aren't here.\n\r", ch);
            return;
         }
      }
      if (IS_AFFECTED(ch, AFF_CHARM) && !IS_NPC(victim))
      {
         check_social(ch, "sulk", "");
         return;
      }
      check_killer(ch, victim);

      vo = (void *)victim;
      break;

   case TAR_CHAR_DEFENSIVE:
      if (arg2[0] == '\0')
      {
         victim = ch;
      }
      else
      {
         if ((victim = get_char_room(ch, arg2)) == NULL)
         {
            send_to_char("They aren't here.\n\r", ch);
            return;
         }
      }

      vo = (void *)victim;
      break;

   case TAR_CHAR_SELF:
      if (arg2[0] != '\0' && !is_name(arg2, ch->name))
      {
         send_to_char("You cannot cast this spell on another.\n\r", ch);
         return;
      }

      vo = (void *)ch;
      break;
   case TAR_CHAR_NOTSELF:
      if (arg2[0] != '\0' && is_name(arg2, ch->name))
      {
         send_to_char("You cannot cast this spell on yourself.\n\r", ch);
         return;
      }

      if ((victim = get_char_room(ch, arg2)) == NULL)
      {
         send_to_char("They aren't here.\n\r", ch);
         return;
      }

      if (victim == ch)
      {
         send_to_char("You cannot cast this spell on yourself.\n\r", ch);
         return;
      }

      vo = (void *)victim;
      break;
   case TAR_OBJ_INV:
      if (arg2[0] == '\0')
      {
         send_to_char("What should the spell be cast upon?\n\r", ch);
         return;
      }

      if ((obj = get_obj_carry(ch, arg2)) == NULL)
      {
         send_to_char("You are not carrying that.\n\r", ch);
         return;
      }

      vo = (void *)obj;
      break;
   }

   if (ch->mana < mana)
   {
      send_to_char("You don't have enough mana.\n\r", ch);
      return;
   }

   if (str_cmp(skill_table[sn].name, "ventriloquate"))
      say_spell(ch, sn);

   WAIT_STATE(ch, skill_table[sn].beats);
   cast_chance = 0;

   if (!IS_NPC(ch) && (skill_table[sn].flag2 == NORM))
   {
      if (IS_SET(race_table[ch->race].race_flags, RACE_MOD_NO_MAGIC))
         cast_chance += -20;
      else if (IS_SET(race_table[ch->race].race_flags, RACE_MOD_WEAK_MAGIC))
         cast_chance += -10;
      else if (IS_SET(race_table[ch->race].race_flags, RACE_MOD_STRONG_MAGIC))
         cast_chance += 15;
   }
   if (cast_chance < 10)
      cast_chance += number_range(3, 7);

   if (!char_login && !IS_NPC(ch) && number_percent() > cast_chance)
   {
      bool good_cast = FALSE;

      if (!skill_success(ch, victim, sn, cast_chance))
         good_cast = FALSE;
      else
         good_cast = TRUE;

      if (!good_cast && is_fighting(ch))
      {
         send_to_char("You lost your concentration.\n\r", ch);

         ch->mana -= mana / 2;
         return;
      }
   }

   if (IS_SET(stance_app[ch->stance].specials, STANCE_MULTI_CAST) &&
       (skill_table[sn].target == TAR_CHAR_OFFENSIVE) &&
       (number_percent() < get_psuedo_level(ch) / 2))
   {
      mana = mana * 2 / 3;
      multi_cast = TRUE;
   }

   /* Mental power overload check: 1% chance per stack to shatter the mental focus */
   if (!IS_NPC(ch) && is_mental_power_spell(sn) && ch->mental_power > 0)
   {
      if (number_percent() < ch->mental_power)
      {
         send_to_char("@@mYour mental focus overloads and shatters! Your mental power collapses to "
                      "nothing.@@N\n\r",
                      ch);
         ch->mental_power = 0;
         return;
      }
   }

   if ((sn != skill_lookup("cure light")) && (sn != skill_lookup("cure serious")) &&
       (sn != skill_lookup("cure critical")) && (sn != skill_lookup("heal")))
   {
      sprintf(log_buf, "%s typed %s, Spell %s, room %s(%d), target %s", ch->name, typed,
              skill_table[sn].name, ch->in_room->name, ch->in_room->vnum,
              (victim != NULL ? victim->name
               : obj != NULL  ? obj->name
                              : "NONE"));
      monitor_chan(log_buf, MONITOR_MAGIC);
   }
   if ((*skill_table[sn].spell_fun)(sn, best, ch, vo, NULL))
   {
      if (is_arcane_spell(sn))
         ch->arcane_power++;
      gain_holy_power_from_healing_spell(ch, sn);
      if (!IS_NPC(ch) && is_mental_power_spell(sn))
         ch->mental_power++;

      ch->mana -= mana; /* Only use mana if spell was called correctly */
   }

   if (multi_cast)
   {
      bool still_here = TRUE;

      if (arg2[0] == '\0')
      {
         if (((victim = ch->fighting) == NULL) || (victim->in_room == NULL) ||
             (victim->is_free == TRUE))
         {
            ch->fighting = NULL;
            still_here = FALSE;
         }
      }
      else
      {
         if ((victim = get_char_room(ch, arg2)) == NULL)
         {
            still_here = FALSE;
         }
      }
      if (still_here)
      {
         vo = (void *)victim;
         if ((sn != skill_lookup("cure light")) && (sn != skill_lookup("cure serious")) &&
             (sn != skill_lookup("cure critical")) && (sn != skill_lookup("heal")))
         {
            sprintf(log_buf, "%s typed %s, Spell %s, room %s(%d), target %s", ch->name, typed,
                    skill_table[sn].name, ch->in_room->name, ch->in_room->vnum,
                    (victim != NULL ? victim->name
                     : obj != NULL  ? obj->name
                                    : "NONE"));
            monitor_chan(log_buf, MONITOR_MAGIC);
         }
         if ((*skill_table[sn].spell_fun)(sn, best, ch, vo, NULL))
         {
            if (is_arcane_spell(sn))
               ch->arcane_power++;
            gain_holy_power_from_healing_spell(ch, sn);
            if (!IS_NPC(ch) && is_mental_power_spell(sn))
               ch->mental_power++;

            ch->mana -= mana; /* Only use mana if spell was called correctly */
         }
      }
   }
   if (IS_SET(stance_app[ch->stance].specials, STANCE_DUAL_CAST) &&
       (skill_table[sn].target == TAR_CHAR_OFFENSIVE) &&
       (number_range(0, 99) < get_psuedo_level(ch) - 80))
   {
      bool still_here = TRUE;
      mana = mana * 1 / 2;

      if (arg2[0] == '\0')
      {
         if (((victim = ch->fighting) == NULL) || (victim->in_room == NULL) ||
             (victim->is_free == TRUE))
         {
            ch->fighting = NULL;
            still_here = FALSE;
         }
      }
      else
      {
         if ((victim = get_char_room(ch, arg2)) == NULL)
         {
            still_here = FALSE;
         }
      }
      if (still_here)
      {
         vo = (void *)victim;
         if ((sn != skill_lookup("cure light")) && (sn != skill_lookup("cure serious")) &&
             (sn != skill_lookup("cure critical")) && (sn != skill_lookup("heal")))
         {
            sprintf(log_buf, "%s typed %s, Spell %s, room %s(%d), target %s", ch->name, typed,
                    skill_table[sn].name, ch->in_room->name, ch->in_room->vnum,
                    (victim != NULL ? victim->name
                     : obj != NULL  ? obj->name
                                    : "NONE"));
            monitor_chan(log_buf, MONITOR_MAGIC);
         }
         if ((*skill_table[sn].spell_fun)(sn, best, ch, vo, NULL))
         {
            if (is_arcane_spell(sn))
               ch->arcane_power++;
            gain_holy_power_from_healing_spell(ch, sn);
            if (!IS_NPC(ch) && is_mental_power_spell(sn))
               ch->mental_power++;

            ch->mana -= mana; /* Only use mana if spell was called correctly */
         }
      }
   }

   if ((skill_table[sn].target == TAR_CHAR_OFFENSIVE) && (ch != NULL) && (victim != NULL) &&
       (ch != victim) && (ch->is_free == FALSE) && (victim->is_free == FALSE))
   {
      CHAR_DATA *vch;
      CHAR_DATA *vch_next;
      CREF(vch_next, CHAR_NEXTROOM);
      for (vch = ch->in_room->first_person; vch; vch = vch_next)
      {
         vch_next = vch->next_in_room;
         if ((vch == victim) && (vch->fighting == NULL) && (vch->master != ch) && (vch != ch))
         {
            check_killer(ch, vch);
            multi_hit(vch, ch, TYPE_UNDEFINED); /* SRZ--fixed bad call--swap ch&v */
            break;
         }
      }
      CUREF(vch_next);
   }

   return;
}

/*
 * Cast spells at targets using a magical object.
 */
void obj_cast_spell(int sn, int level, CHAR_DATA *ch, CHAR_DATA *victim, OBJ_DATA *obj)
{
   void *vo;

   if (sn <= 0)
      return;

   if (sn >= MAX_SKILL || skill_table[sn].spell_fun == 0)
   {
      bug("Obj_cast_spell: bad sn %d.", sn);
      return;
   }

   switch (skill_table[sn].target)
   {
   default:
      bug("Obj_cast_spell: bad target for sn %d.", sn);
      return;

   case TAR_IGNORE:
      vo = NULL;
      break;

   case TAR_CHAR_OFFENSIVE:
      if (victim == NULL)
         if (ch != NULL)
            victim = ch->fighting;
      if (victim == NULL)
      {
         if (ch != NULL)
            send_to_char("You can't do that.\n\r", ch);
         return;
      }
      vo = (void *)victim;
      break;

   case TAR_CHAR_DEFENSIVE:
      if (victim == NULL)
         if (ch != NULL)
            victim = ch;
      if (victim != NULL)
         vo = (void *)victim;
      else
         return;
      break;

   case TAR_CHAR_SELF:
      if (ch != NULL)
         vo = (void *)ch;
      else
         return;
      break;

   case TAR_OBJ_INV:
      if (obj == NULL)
      {
         if (ch != NULL)
            send_to_char("You can't do that.\n\r", ch);
         return;
      }
      vo = (void *)obj;
      break;
   }

   target_name = "";

   (*skill_table[sn].spell_fun)(sn, level, ch, vo, obj);

   if (skill_table[sn].target == TAR_CHAR_OFFENSIVE && (victim != NULL) && victim->master != ch)
   {
      CHAR_DATA *vch;
      CHAR_DATA *vch_next;

      for (vch = ch->in_room->first_person; vch; vch = vch_next)
      {
         vch_next = vch->next_in_room;
         if (victim == vch && victim->fighting == NULL)
         {
            multi_hit(ch, victim, TYPE_UNDEFINED); /* SRZ swapped ch& v */
            break;
         }
      }
   }

   return;
}

/* Spell functions. */

/*
 * Drain XP, MANA, HP.
 * Caster gains HP.
 */

/*
 * Spell for mega1.are from Glop/Erkenbrand.
 */

void do_spell_heal(CHAR_DATA *ch, CHAR_DATA *victim, int sn)
{
   int heal = class_heal_character(ch, victim, 50, sn, INDEX_CLE, FALSE);

   heal_character(ch, victim, heal, sn, FALSE);
}

/*
 * Spell for mega1.are from Glop/Erkenbrand.
 */
