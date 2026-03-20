#include "globals.h"
#include "magic.h"
#include "skills.h"

void do_claws_judgment(CHAR_DATA *ch, char *argument)
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];
   int dam;

   if (!can_use_pub_society_skill(ch, gsn_claws_judgment))
   {
      send_to_char("You don't know how to use this skill!\n\r", ch);
      return;
   }

   /* Combo finisher gate - same as holystrike/fleche */
   if (!is_valid_finisher(ch))
   {
      send_to_char("You are not prepared for a finisher!\n\r", ch);
      return;
   }

   if (!can_use_skill_message(ch, gsn_claws_judgment))
      return;

   if (!is_fighting(ch))
   {
      send_to_char("You must be fighting to execute judgment!\n\r", ch);
      return;
   }

   one_argument(argument, arg);
   if (arg[0] == '\0')
      victim = ch->fighting;
   else
   {
      if ((victim = get_char_room(ch, arg)) == NULL)
      {
         send_to_char("They aren't here.\n\r", ch);
         return;
      }
   }

   /* Oath mark gate - must be marked */
   if (!is_affected(victim, gsn_oath_mark))
   {
      send_to_char("Your target bears no oath mark. The Shrine's judgment requires it.\n\r", ch);
      return;
   }

   if (!subtract_energy_cost(ch, gsn_claws_judgment))
      return;

   /* Massive damage with 2x multiplier against marked target */
   dam = number_range(get_psuedo_level(ch) * get_curr_str(ch) / 4,
                      get_psuedo_level(ch) * get_curr_str(ch) / 3);
   dam *= 2;
   if (dam < 1)
      dam = 1;

   WAIT_STATE(ch, skill_table[gsn_claws_judgment].beats);
   ch->cooldown[gsn_claws_judgment] = 20;
   raise_skill(ch, gsn_claws_judgment);

   check_killer(ch, victim);

   if (can_hit_skill(ch, victim, gsn_claws_judgment))
   {
      act("@@W$n executes the Shrine's judgment on $N!@@N", ch, NULL, victim, TO_NOTVICT);
      act("@@W$n executes the Shrine's judgment on you!@@N", ch, NULL, victim, TO_VICT);
      send_to_char("@@WYou execute the Shrine's judgment!@@N\n\r", ch);

      calculate_damage(ch, victim, dam, gsn_claws_judgment, ELE_PHYSICAL, TRUE);

      /* Trigger finisher chain as combo finisher */
      combo(ch, victim, gsn_claws_judgment);
   }
   else
   {
      send_to_char("Your judgment strike misses!\n\r", ch);
      set_fighting(victim, ch, TRUE);
   }
}
