#include "globals.h"
#include "magic.h"
#include "skills.h"

void do_anchor_slam(CHAR_DATA *ch, char *argument)
{
   CHAR_DATA *victim;
   AFFECT_DATA af;
   char arg[MAX_INPUT_LENGTH];
   int dam;

   if (!can_use_pub_society_skill(ch, gsn_anchor_slam))
   {
      send_to_char("You don't know how to use this skill!\n\r", ch);
      return;
   }

   if (!can_use_skill_message(ch, gsn_anchor_slam))
      return;

   if (!is_fighting(ch))
   {
      send_to_char("You must be fighting to use anchor slam!\n\r", ch);
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

   if (!subtract_energy_cost(ch, gsn_anchor_slam))
      return;

   /* Heavy physical damage */
   dam = number_range(get_psuedo_level(ch) * get_curr_str(ch) / 4,
                      get_psuedo_level(ch) * get_curr_str(ch) / 3);
   if (dam < 1)
      dam = 1;

   WAIT_STATE(ch, skill_table[gsn_anchor_slam].beats);
   ch->cooldown[gsn_anchor_slam] = 20;
   raise_skill(ch, gsn_anchor_slam);

   check_killer(ch, victim);

   if (can_hit_skill(ch, victim, gsn_anchor_slam))
   {
      act("$n swings with dockyard brutality, slamming into $N!", ch, NULL, victim, TO_NOTVICT);
      act("$n slams into you with brutal force!", ch, NULL, victim, TO_VICT);
      send_to_char("You slam into your target with dockyard brutality!\n\r", ch);

      calculate_damage(ch, victim, dam, gsn_anchor_slam, ELE_PHYSICAL, TRUE);

      /* Apply speed debuff */
      if (!is_affected(victim, gsn_anchor_slam))
      {
         af.type = gsn_anchor_slam;
         af.duration = 2;
         af.duration_type = DURATION_ROUND;
         af.location = APPLY_SPEED;
         af.modifier = -3;
         af.bitvector = 0;
         af.caster = ch;
         affect_to_char(victim, &af);
      }

      combo(ch, victim, gsn_anchor_slam);
   }
   else
   {
      act("$n swings wildly but misses!", ch, NULL, victim, TO_NOTVICT);
      act("$n swings wildly but misses you!", ch, NULL, victim, TO_VICT);
      send_to_char("Your anchor slam misses!\n\r", ch);
      set_fighting(victim, ch, TRUE);
   }
}
