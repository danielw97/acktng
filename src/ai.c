#include "globals.h"
#include "tables.h"

bool check_skills(CHAR_DATA *ch);
bool check_cast(CHAR_DATA *ch);
bool round_ai_update(CHAR_DATA *ch);

void ai_update()
{
   CHAR_DATA *ch;
   CHAR_DATA *ch_next;
   for (ch = first_char; ch; ch = ch_next)
   {
      ch_next = ch->next;

      if (IS_NPC(ch) && ch->fighting != NULL)
      {
         check_skills(ch);
         check_cast(ch);
      }

      round_ai_update(ch);
   }
}

bool check_skills(CHAR_DATA *ch)
{
   int cnt, check;

   if (!IS_NPC(ch))
      return FALSE;

   /*
    * Count how many of the attack skills are available
    */

   cnt = 0;
   if (IS_SET(ch->skills, MOB_PUNCH))
      cnt++;
   if (IS_SET(ch->skills, MOB_HEADBUTT))
      cnt++;
   if (IS_SET(ch->skills, MOB_KNEE))
      cnt++;
   if (IS_SET(ch->skills, MOB_KICK))
      cnt++;
   if (IS_SET(ch->skills, MOB_DIRT))
      cnt++;
   if (IS_SET(ch->skills, MOB_CHARGE))
      cnt++;

   if (cnt == 0)
      return FALSE; /* There were no attack skills set */

   check = number_range(1, cnt);

   cnt = 0;
   if (IS_SET(ch->skills, MOB_KICK) && (++cnt == check))
   {
      do_kick(ch, "");
      return TRUE;
   }
   if (IS_SET(ch->skills, MOB_PUNCH) && (++cnt == check))
   {
      do_punch(ch, "");
      return TRUE;
   }
   if (IS_SET(ch->skills, MOB_HEADBUTT) && (++cnt == check))
   {
      do_headbutt(ch, "");
      return TRUE;
   }
   if (IS_SET(ch->skills, MOB_KNEE) && (++cnt == check))
   {
      do_knee(ch, "");
      return TRUE;
   }
   if (IS_SET(ch->skills, MOB_DIRT) && (++cnt == check))
   {
      do_dirt(ch, "");
      return TRUE;
   }
   if (IS_SET(ch->skills, MOB_CHARGE) && (++cnt == check))
   {
      do_charge(ch, "");
      return TRUE;
   }

   return FALSE;
}

bool round_ai_update(CHAR_DATA *ch)
{
   if (!IS_NPC(ch))
      return FALSE;

   if ((ch->is_free == FALSE) && (IS_NPC(ch)) && (!IS_SET(ch->def, DEF_NONE)) && (ch->hit > 0) && (ch->first_shield == NULL) && (ch->fighting == NULL))
   {
      if ((IS_SET(ch->def, DEF_FIRESHIELD)) && (!is_affected(ch, skill_lookup("fireshield"))) && (ch->mana > mana_cost(ch, skill_lookup("fireshield"))))
      {
         do_cast(ch, "fireshield");
      }
      else if ((IS_SET(ch->def, DEF_ICESHIELD)) && (!is_affected(ch, skill_lookup("iceshield"))) && (ch->mana > mana_cost(ch, skill_lookup("iceshield"))))
      {
         do_cast(ch, "iceshield");
      }
      else if ((IS_SET(ch->def, DEF_SHOCKSHIELD)) && (!is_affected(ch, skill_lookup("shockshield"))) && (ch->mana > mana_cost(ch, skill_lookup("shockshield"))))
      {
         do_cast(ch, "shockshield");
      }
   }
   if (!is_fighting(ch) && (ch->is_free == FALSE) && (IS_NPC(ch)) && IS_SET(ch->act, ACT_SOLO) && ch->hit > 0)
   {
      if ((ch->hit < ch->max_hit * 3 / 4) && (ch->mana > mana_cost(ch, skill_lookup("heal"))))
      {
         do_cast(ch, "heal self");
      }

      if ((ch->is_free == FALSE) && (IS_NPC(ch)) && (!IS_SET(ch->def, DEF_NONE)) && ch->hit > 0)
      {
         if (ch->hit < ch->max_hit * 2 / 3)
         {
            if (IS_SET(ch->def, DEF_CURE_LIGHT))
            {
               if (ch->mana > mana_cost(ch, skill_lookup("cure light")))
               {
                  do_cast(ch, "\'cure light\' self");
               }
            }
            if (IS_SET(ch->def, DEF_CURE_SERIOUS))
            {
               if (ch->mana > mana_cost(ch, skill_lookup("cure serious")))
               {
                  do_cast(ch, "\'cure serious\' self");
               }
            }
            if (IS_SET(ch->def, DEF_CURE_CRITIC))
            {
               if (ch->mana > mana_cost(ch, skill_lookup("cure critical")))
               {
                  do_cast(ch, "\'cure critical\' self");
               }
            }
            if (IS_SET(ch->def, DEF_HEAL))
            {
               if (ch->mana > mana_cost(ch, skill_lookup("heal")))
               {
                  do_cast(ch, "heal self");
               }
            }
         }
      }
   }

   return TRUE;
}

bool check_cast(CHAR_DATA *ch)
{
   if (!IS_NPC(ch))
      return FALSE;

   if ((ch->is_free == FALSE) && (IS_NPC(ch)) && IS_SET(ch->act, ACT_SOLO) && ch->hit > 0)
   {
      if ((ch->hit < ch->max_hit * 3 / 4) && (ch->mana > mana_cost(ch, skill_lookup("heal"))))
      {
         do_cast(ch, "heal self");
      }
   }

   if ((ch->is_free == FALSE) && (IS_NPC(ch)) && (!IS_SET(ch->def, DEF_NONE)) && ch->hit > 0)
   {
      if (ch->hit < ch->max_hit * 2 / 3)
      {
         if (IS_SET(ch->def, DEF_CURE_LIGHT))
         {
            if (ch->mana > mana_cost(ch, skill_lookup("cure light")))
            {
               do_cast(ch, "\'cure light\' self");
            }
         }
         if (IS_SET(ch->def, DEF_CURE_SERIOUS))
         {
            if (ch->mana > mana_cost(ch, skill_lookup("cure serious")))
            {
               do_cast(ch, "\'cure serious\' self");
            }
         }
         if (IS_SET(ch->def, DEF_CURE_CRITIC))
         {
            if (ch->mana > mana_cost(ch, skill_lookup("cure critical")))
            {
               do_cast(ch, "\'cure critical\' self");
            }
         }
         if (IS_SET(ch->def, DEF_HEAL))
         {
            if (ch->mana > mana_cost(ch, skill_lookup("heal")))
            {
               do_cast(ch, "heal self");
            }
         }
      }
   }
   /* Offensive spell handler, only use when actually fighting.. */

   if ((IS_NPC(ch)) && (ch->is_free == FALSE) && (ch->cast > 1) && (ch->position > POS_RESTING) && (ch->fighting != NULL) && (ch->fighting->is_free != TRUE) && (ch->in_room != NULL) && (ch->hit > 1) && (ch->position == POS_FIGHTING))

   {
      sh_int cast_frequency;
      sh_int index;

      cast_frequency = get_psuedo_level(ch) / 2; /* maybe set in olc later? */
      if ((number_range(0, 99) < cast_frequency) && (ch->mana >= (40 * ch->max_mana / 100)))
      {
         for (index = 1; index < 32; index++)
         {
            if ((IS_SET(ch->cast, (1 << index))) && (number_range(0, 99) < (index * 3 + number_range(0, 25))) && (ch->mana > mana_cost(ch, skill_lookup(rev_table_lookup(tab_cast_name, (1 << index))))))
            {
               char cast_name[MSL];
               sprintf(cast_name, "%s %s", rev_table_lookup(tab_cast_name, (1 << index)), ch->fighting->name);
               do_cast(ch, cast_name);
               break;
            }
         }
      }
   }

   return TRUE;
}

bool reset_skills(CHAR_DATA *ch)
{
   if (!IS_NPC(ch))
      return FALSE;

   return TRUE;
}

/*
#define MOB_NONE                        BIT_1
#define MOB_SECOND                      BIT_2
#define MOB_THIRD                       BIT_3
#define MOB_FOURTH                      BIT_4
#define MOB_FIFTH                       BIT_5
#define MOB_SIXTH                       BIT_6
#define MOB_NODISARM                    BIT_7
#define MOB_NOTRIP                      BIT_8
#define MOB_PUNCH                       BIT_9
#define MOB_HEADBUTT                    BIT_10
#define MOB_KNEE                        BIT_11
#define MOB_DISARM                      BIT_12
#define MOB_TRIP                        BIT_13
#define MOB_NODISARM                    BIT_14
#define MOB_NOTRIP                      BIT_15
#define MOB_DODGE                       BIT_16
#define MOB_PARRY                       BIT_17
#define MOB_MARTIAL                     BIT_18
#define MOB_ENHANCED                    BIT_19
#define MOB_DUALWIELD                   BIT_20
#define MOB_DIRT                        BIT_21
#define MOB_FIFTH                       BIT_22
#define MOB_SIXTH                       BIT_23
#define MOB_CHARGE                      BIT_24
#define MOB_COUNTER                     BIT_25
#define MOB_KICK                        BIT_26
*/

/*
#define CAST_NONE                       BIT_1
#define CAST_MAGIC_MISSILE              BIT_2
#define CAST_SHOCKING_GRASP             BIT_3
#define CAST_BURNING_HANDS              BIT_4
#define CAST_COLOR_SPRAY                BIT_5
#define CAST_FIREBALL                   BIT_6
#define CAST_HELLSPAWN                  BIT_7
#define CAST_ACID_BLAST                 BIT_8
#define CAST_CHAIN_LIGHTNING            BIT_9
#define CAST_FLARE                      BIT_10
#define CAST_FLAMESTRIKE                BIT_11
#define CAST_EARTHQUAKE                 BIT_12
#define CAST_MIND_FLAIL                 BIT_13
#define CAST_PLANERGY                   BIT_14
#define CAST_PHOBIA                     BIT_15
#define CAST_MIND_BOLT                  BIT_16
#define CAST_STATIC                     BIT_17
#define CAST_EGO_WHIP                   BIT_18
#define CAST_BLOODY_TEARS               BIT_19
#define CAST_MINDFLAME                  BIT_20
#define CAST_SUFFOCATE                  BIT_21
#define CAST_NERVE_FIRE                 BIT_22
#define CAST_LIGHTNING_BOLT             BIT_23
#define CAST_HEAT_ARMOR                 BIT_24
#define CAST_LAVA_BURST                 BIT_25
*/
