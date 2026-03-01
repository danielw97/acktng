#include "globals.h"
#include "tables.h"

bool check_skills(CHAR_DATA *ch);
bool check_cast(CHAR_DATA *ch);
bool round_ai_update(CHAR_DATA *ch);
bool generate_phys(CHAR_DATA *ch);
bool generate_offensive_cast(CHAR_DATA *ch);
bool generate_defensive_cast(CHAR_DATA *ch);

void ai_update()
{
   CHAR_DATA *ch;
   CHAR_DATA *ch_next;

   for (ch = first_char; ch != NULL; ch = ch_next)
   {
      if (ch == NULL && ch_next == NULL)
         break;
      ch_next = ch->next;

      if (!IS_NPC(ch))
         continue;

      if (ch->stunTimer > 0 || ch->position != POS_FIGHTING)
         continue;

      ch_next = ch->next;

      if (IS_NPC(ch) && ch->fighting != NULL)
      {
         if (!check_skills(ch))
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
   if (IS_SET(ch->skills, MOB_DISARM))
      cnt++;
   if (IS_SET(ch->skills, MOB_TRIP))
      cnt++;

   if (cnt == 0)
      return FALSE; /* There were no attack skills set */

   check = number_range(1, cnt);

   cnt = 0;
   if (IS_SET(ch->skills, MOB_KICK) && (++cnt <= check))
   {
      do_kick(ch, "");
      return TRUE;
   }
   if (IS_SET(ch->skills, MOB_PUNCH) && (++cnt <= check))
   {
      do_punch(ch, "");
      return TRUE;
   }
   if (IS_SET(ch->skills, MOB_HEADBUTT) && (++cnt <= check))
   {
      do_headbutt(ch, "");
      return TRUE;
   }
   if (IS_SET(ch->skills, MOB_KNEE) && (++cnt <= check))
   {
      do_knee(ch, "");
      return TRUE;
   }
   if (IS_SET(ch->skills, MOB_DIRT) && (++cnt <= check))
   {
      do_dirt(ch, "");
      return TRUE;
   }
   if (IS_SET(ch->skills, MOB_CHARGE) && (++cnt <= check))
   {
      do_charge(ch, "");
      return TRUE;
   }
   if (IS_SET(ch->skills, MOB_DISARM) && (++cnt <= check))
   {
      do_disarm(ch, "");
      return TRUE;
   }
   if (IS_SET(ch->skills, MOB_TRIP) && (++cnt <= check))
   {
      do_trip(ch, "");
      return TRUE;
   }

   return FALSE;
}

bool round_ai_update(CHAR_DATA *ch)
{
   if (ch == NULL || !IS_NPC(ch))
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

   if (IS_SET(ch->act, ACT_SOLO) && (ch->hit < ch->max_hit * 3 / 4) && (ch->mana > mana_cost(ch, skill_lookup("heal"))) && ch->level > 80)
   {
      do_cast(ch, "heal self");
   }
   else if ((ch->is_free == FALSE) && (IS_NPC(ch)) && (!IS_SET(ch->def, DEF_NONE)) && ch->hit > 0)
   {
      if (ch->hit < ch->max_hit * 2 / 3)
      {
         if (IS_SET(ch->def, DEF_HEAL))
         {
            if (ch->mana > mana_cost(ch, skill_lookup("heal")))
            {
               do_cast(ch, "heal self");
            }
         }
         else if (IS_SET(ch->def, DEF_CURE_SERIOUS))
         {
            if (ch->mana > mana_cost(ch, skill_lookup("cure serious")))
            {
               do_cast(ch, "\'cure serious\' self");
            }
         }
         else if (IS_SET(ch->def, DEF_CURE_CRITIC))
         {
            if (ch->mana > mana_cost(ch, skill_lookup("cure critical")))
            {
               do_cast(ch, "\'cure critical\' self");
            }
         }
         else if (IS_SET(ch->def, DEF_CURE_LIGHT))
         {
            if (ch->mana > mana_cost(ch, skill_lookup("cure light")))
            {
               do_cast(ch, "\'cure light\' self");
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

   /* Offensive spell handler, only use when actually fighting.. */
   if ((IS_NPC(ch)) && (ch->is_free == FALSE) && (ch->cast > 1) && (ch->position > POS_RESTING) && (ch->fighting != NULL) && (ch->fighting->is_free != TRUE) && (ch->in_room != NULL) && (ch->hit > 1) && (ch->position == POS_FIGHTING))
   {
      sh_int cast_frequency;
      sh_int index;

      cast_frequency = get_psuedo_level(ch) / 3; /* maybe set in olc later? */
      if ((number_range(0, 99) < cast_frequency) && (ch->mana >= (40 * ch->max_mana / 100)))
      {
         for (index = 1; index < 32; index++)
         {
            if ((IS_SET(ch->cast, (1 << index))) && (number_range(0, 99) < (index * 3 + number_range(0, 25))) && (ch->mana > mana_cost(ch, skill_lookup(rev_table_lookup(tab_cast_name, (1 << index))))))
            {
               char cast_name[MSL];
               sprintf(cast_name, "%s %s", rev_table_lookup(tab_cast_name, (1 << index)), ch->fighting->name);
               do_cast(ch, cast_name);
               return TRUE;
            }
         }
      }
   }

   return FALSE;
}

bool reset_skills(CHAR_DATA *ch)
{
   if (!IS_NPC(ch))
      return FALSE;

   return TRUE;
}

bool generate_ai_spawn(CHAR_DATA *ch)
{
   if (ch->level < 25)
      return FALSE;

   if (ch->skills || ch->cast || ch->def)
      return FALSE;

   int min_chance = 1;
   int max_chance = 10;

//   ch->spellpower_mod = 0;

   if (get_psuedo_level(ch) > 30)
      min_chance++;

   if (get_psuedo_level(ch) > 70)
      min_chance++;

   if (get_psuedo_level(ch) > 110)
      min_chance++;

   if (get_psuedo_level(ch) > 150)
      min_chance++;

   if (number_range(min_chance,max_chance)==10)
      SET_BIT(ch->act, ACT_SOLO);

   int ai = number_range(1, 10);

   switch (ai)
   {
   case 1:
      // Pure phys
      generate_phys(ch);
      break;
   case 2:
      // Mixed phys and defensive spells
      generate_phys(ch);
      generate_defensive_cast(ch);
      break;
   case 3:
      // Pure offensive spells
      generate_offensive_cast(ch);
      break;
   case 4:
      // Mixed phys and offensive spells
      generate_phys(ch);
      generate_offensive_cast(ch);
      break;
   case 5:
      // Mixed offensive and defensive spells
      generate_offensive_cast(ch);
      generate_defensive_cast(ch);
      break;
   case 6:
      generate_defensive_cast(ch);
      break;
   case 7:
      generate_phys(ch);
      generate_offensive_cast(ch);
      generate_defensive_cast(ch);
   default:
      // Nothing
      break;
   }

   return TRUE;
}

bool generate_phys(CHAR_DATA *ch)
{
   int max_skills = get_psuedo_level(ch) / 30;
   if (get_psuedo_level(ch) > 150)
      max_skills += number_range(0, 5);

   ch->hr_mod += number_range(get_psuedo_level(ch), get_psuedo_level(ch)*2);
   ch->dr_mod += number_range(0, get_psuedo_level(ch)*2);
   ch->ac_mod -= number_range(0, get_psuedo_level(ch)*2);
   ch->hp_mod += number_range(0, get_psuedo_level(ch)*5);

   int total_skills = number_range(0, max_skills);

   for (int i = 0; i < total_skills; i++)
   {
      int skill_pool;

      if (get_psuedo_level(ch) > 120)
         skill_pool = number_range(1, 17);
      else
         skill_pool = number_range(1, 14);

      switch (skill_pool)
      {
      case 1:
         SET_BIT(ch->skills, MOB_SECOND);
         break;
      case 2:
         SET_BIT(ch->skills, MOB_THIRD);
         break;
      case 3:
         SET_BIT(ch->skills, MOB_FOURTH);
         break;
      case 4:
         SET_BIT(ch->skills, MOB_PUNCH);
         break;
      case 5:
         SET_BIT(ch->skills, MOB_HEADBUTT);
         break;
      case 6:
         SET_BIT(ch->skills, MOB_KNEE);
         break;
      case 7:
         if (number_percent() < 25)
            SET_BIT(ch->skills, MOB_DISARM);
         break;
      case 8:
         SET_BIT(ch->skills, MOB_DODGE);
         break;
      case 9:
         SET_BIT(ch->skills, MOB_PARRY);
         break;
      case 10:
         SET_BIT(ch->skills, MOB_MARTIAL);
         break;
      case 11:
         SET_BIT(ch->skills, MOB_ENHANCED);
         break;
      case 12:
         SET_BIT(ch->skills, MOB_DIRT);
         break;
      case 13:
         SET_BIT(ch->skills, MOB_COUNTER);
         break;
      case 14:
         SET_BIT(ch->skills, MOB_KICK);
         break;
      case 15:
         SET_BIT(ch->skills, MOB_FIFTH);
         break;
      case 16:
         SET_BIT(ch->skills, MOB_SIXTH);
         break;
      case 17:
         SET_BIT(ch->skills, MOB_CHARGE);
         break;
      }
   }

   return TRUE;
}

bool generate_offensive_cast(CHAR_DATA *ch)
{
   int skill_pool = 0;
   int max_skills = get_psuedo_level(ch)/30;

   ch->spellpower_mod += number_range(get_psuedo_level(ch)/2, get_psuedo_level(ch));
   ch->hp_mod += number_range(0, get_psuedo_level(ch)*5);

   if (get_psuedo_level(ch) > 150)
      max_skills += 3;

   int total_skills = number_range(0,max_skills);

   for(int i = 0; i < total_skills; i++)
   {
      if (get_psuedo_level(ch) > 150)
         skill_pool = number_range(1,19);
      else if (get_psuedo_level(ch) > 120)
         skill_pool = number_range(1,17);
      else if (get_psuedo_level(ch) > 80)
         skill_pool = number_range(1,16);
      else if (get_psuedo_level(ch) > 50)
         skill_pool = number_range(1,15);
      else
         skill_pool = number_range(1,9);

      switch(skill_pool)
      {
         case 1:
            SET_BIT(ch->cast, CAST_MAGIC_MISSILE);
            break;
         case 2:
            SET_BIT(ch->cast, CAST_SHOCKING_GRASP);
            break;
         case 3:
            SET_BIT(ch->cast, CAST_BURNING_HANDS);
            break;
         case 4:
            SET_BIT(ch->cast, CAST_COLOR_SPRAY);
            break;
         case 5:
            SET_BIT(ch->cast, CAST_FIREBALL);
            break;
         case 6:
            SET_BIT(ch->cast, CAST_MIND_BOLT);
            break;
         case 7:
            SET_BIT(ch->cast, CAST_MIND_FLAIL);
            break;
         case 8:
            SET_BIT(ch->cast, CAST_PLANERGY);
            break;
         case 9:
            SET_BIT(ch->cast, CAST_LIGHTNING_BOLT);
            break;
         case 10:
            SET_BIT(ch->cast, CAST_ACID_BLAST);
            break;
         case 11:
            SET_BIT(ch->cast, CAST_HELLSPAWN);
            break;
         case 12:
            SET_BIT(ch->cast, CAST_BLOODY_TEARS);
            break;
         case 13:
            SET_BIT(ch->cast, CAST_EGO_WHIP);
            break;
         case 14:
            SET_BIT(ch->cast, CAST_STATIC);
            break;
         case 15:
            SET_BIT(ch->cast, CAST_SUFFOCATE);
            break;
         case 16:
            SET_BIT(ch->cast, CAST_FLARE);
            break;
         case 17:
            SET_BIT(ch->cast, CAST_FLAMESTRIKE);
            break;
         case 18:
            SET_BIT(ch->cast, CAST_HEAT_ARMOR);
            break;
         case 19:
            SET_BIT(ch->cast, CAST_LAVA_BURST);
            break;
      }
   }

   return TRUE;
}

bool generate_defensive_cast(CHAR_DATA *ch)
{
   int max_skills = 0;

   ch->healing_mod += number_range(get_psuedo_level(ch)/2, get_psuedo_level(ch)*2);
   ch->hp_mod += number_range(0, get_psuedo_level(ch)*5);

   if (get_psuedo_level(ch) > 20)
      max_skills++;

   if (get_psuedo_level(ch) > 120)
      max_skills++;

   if (get_psuedo_level(ch) > 150)
      max_skills += 2;

   int total_skills = number_range(0, max_skills);

   for (int i = 0; i < total_skills; i++)
   {
      int skill_pool;

      if (get_psuedo_level(ch) > 120)
         skill_pool = number_range(1, 7);
      else if (get_psuedo_level(ch) > 80)
         skill_pool = number_range(1, 4);
      else if (get_psuedo_level(ch) > 60)
         skill_pool = number_range(1, 3);
      else if (get_psuedo_level(ch) > 40)
         skill_pool = number_range(1, 2);
      else
         skill_pool = 1;

      switch (skill_pool)
      {
      case 1:
         SET_BIT(ch->def, DEF_CURE_LIGHT);
         break;
      case 2:
         SET_BIT(ch->def, DEF_CURE_SERIOUS);
         break;
      case 3:
         SET_BIT(ch->def, DEF_CURE_CRITIC);
         break;
      case 4:
         SET_BIT(ch->def, DEF_HEAL);
         break;
      case 5:
         SET_BIT(ch->def, DEF_ICESHIELD);
         break;
      case 6:
         SET_BIT(ch->def, DEF_FIRESHIELD);
         break;
      case 7:
         SET_BIT(ch->def, DEF_SHOCKSHIELD);
         break;
      }
   }

   return TRUE;
}
