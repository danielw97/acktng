#include "globals.h"
#include "magic.h"
#include "skills.h"

void do_waylay(CHAR_DATA *ch, char *argument)
{
   CHAR_DATA *victim;
   AFFECT_DATA af;
   char arg[MAX_INPUT_LENGTH];
   int dam;

   if (!can_use_pub_society_skill(ch, gsn_waylay))
   {
      send_to_char("You don't know how to use this skill!\n\r", ch);
      return;
   }

   if (!can_use_skill_message(ch, gsn_waylay))
      return;

   if (!is_fighting(ch))
   {
      send_to_char("You must be fighting to use waylay!\n\r", ch);
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

   /* Positional gate: target must be fighting someone else (like circle) */
   if (victim->fighting == NULL || victim->fighting == ch)
   {
      send_to_char(
          "You need to strike from an angle -- your target must be fighting someone else.\n\r", ch);
      return;
   }

   if (!subtract_energy_cost(ch, gsn_waylay))
      return;

   dam = number_range(get_psuedo_level(ch) * get_curr_str(ch) / 4,
                      get_psuedo_level(ch) * get_curr_str(ch) / 3);
   if (dam < 1)
      dam = 1;

   WAIT_STATE(ch, skill_table[gsn_waylay].beats);
   ch->cooldown[gsn_waylay] = 20;
   raise_skill(ch, gsn_waylay);

   check_killer(ch, victim);

   if (can_hit_skill(ch, victim, gsn_waylay))
   {
      act("$n strikes $N from a cleared angle!", ch, NULL, victim, TO_NOTVICT);
      act("$n strikes you from your blind spot!", ch, NULL, victim, TO_VICT);
      send_to_char("You strike from a cleared angle, exploiting their blind spot!\n\r", ch);

      calculate_damage(ch, victim, dam, gsn_waylay, ELE_PHYSICAL, TRUE);

      /* Apply dodge and speed debuff */
      if (!is_affected(victim, gsn_waylay))
      {
         af.type = gsn_waylay;
         af.duration = 2;
         af.duration_type = DURATION_ROUND;
         af.location = APPLY_DODGE;
         af.modifier = -5;
         af.bitvector = 0;
         af.caster = ch;
         affect_to_char(victim, &af);

         af.location = APPLY_SPEED;
         af.modifier = -2;
         affect_to_char(victim, &af);
      }
   }
   else
   {
      send_to_char("Your waylay misses!\n\r", ch);
      set_fighting(victim, ch, TRUE);
   }
}
