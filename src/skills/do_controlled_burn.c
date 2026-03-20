#include "globals.h"
#include "magic.h"
#include "skills.h"

void do_controlled_burn(CHAR_DATA *ch, char *argument)
{
   CHAR_DATA *victim;
   AFFECT_DATA af;
   char arg[MAX_INPUT_LENGTH];
   int dam;
   int dot_duration = 3;

   if (!can_use_pub_society_skill(ch, gsn_controlled_burn))
   {
      send_to_char("You don't know how to use this skill!\n\r", ch);
      return;
   }

   if (!can_use_skill_message(ch, gsn_controlled_burn))
      return;

   if (!is_fighting(ch))
   {
      send_to_char("You must be fighting to use controlled burn!\n\r", ch);
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

   if (!subtract_energy_cost(ch, gsn_controlled_burn))
      return;

   dam = number_range(get_psuedo_level(ch) * get_curr_str(ch) / 5,
                      get_psuedo_level(ch) * get_curr_str(ch) / 4);
   if (dam < 1)
      dam = 1;

   /* Chi enhancement: spend 5 chi to double DOT duration */
   if (!IS_NPC(ch) && get_chi(ch) >= 5)
   {
      ch->chi -= 5;
      dot_duration = 6;
      send_to_char("@@eYou channel your chi into the flames!@@N\n\r", ch);
   }

   WAIT_STATE(ch, skill_table[gsn_controlled_burn].beats);
   ch->cooldown[gsn_controlled_burn] = 20;
   raise_skill(ch, gsn_controlled_burn);

   check_killer(ch, victim);

   if (can_hit_skill(ch, victim, gsn_controlled_burn))
   {
      act("@@e$n unleashes a searing strike on $N!@@N", ch, NULL, victim, TO_NOTVICT);
      act("@@e$n unleashes a searing strike on you!@@N", ch, NULL, victim, TO_VICT);
      send_to_char("@@eYou unleash a searing strike!@@N\n\r", ch);

      calculate_damage(ch, victim, dam, gsn_controlled_burn, ELE_FIRE, TRUE);

      /* Apply fire DOT */
      if (!is_affected(victim, gsn_controlled_burn))
      {
         af.type = gsn_controlled_burn;
         af.duration = dot_duration;
         af.duration_type = DURATION_ROUND;
         af.location = APPLY_DOT;
         af.modifier = dam / 10;
         af.bitvector = 0;
         af.caster = ch;
         affect_to_char(victim, &af);
      }
   }
   else
   {
      send_to_char("Your searing strike misses!\n\r", ch);
      set_fighting(victim, ch, TRUE);
   }
}
