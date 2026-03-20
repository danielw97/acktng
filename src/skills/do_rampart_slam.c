#include "globals.h"
#include "magic.h"
#include "skills.h"

void do_rampart_slam(CHAR_DATA *ch, char *argument)
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];
   int dam;
   OBJ_DATA *shield;

   if (!can_use_pub_society_skill(ch, gsn_rampart_slam))
   {
      send_to_char("You don't know how to use this skill!\n\r", ch);
      return;
   }

   if (!can_use_skill_message(ch, gsn_rampart_slam))
      return;

   /* Require a shield */
   shield = get_eq_char(ch, WEAR_HOLD_HAND_R);
   if (shield == NULL || shield->item_type != ITEM_ARMOR)
      shield = get_eq_char(ch, WEAR_HOLD_HAND_L);
   if (shield == NULL || shield->item_type != ITEM_ARMOR)
      shield = get_eq_char(ch, WEAR_BUCKLER);
   if (shield == NULL)
   {
      send_to_char("You need a shield equipped to use rampart slam!\n\r", ch);
      return;
   }

   if (!is_fighting(ch))
   {
      send_to_char("You must be fighting to use rampart slam!\n\r", ch);
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

   if (!subtract_energy_cost(ch, gsn_rampart_slam))
      return;

   dam = number_range(get_psuedo_level(ch) * get_curr_str(ch) / 4,
                      get_psuedo_level(ch) * get_curr_str(ch) / 3);
   if (dam < 1)
      dam = 1;

   WAIT_STATE(ch, skill_table[gsn_rampart_slam].beats);
   ch->cooldown[gsn_rampart_slam] = 20;
   raise_skill(ch, gsn_rampart_slam);

   check_killer(ch, victim);

   if (can_hit_skill(ch, victim, gsn_rampart_slam))
   {
      act("$n drives $s shield into $N with fortress-wall force!", ch, NULL, victim, TO_NOTVICT);
      act("$n drives $s shield into you with crushing force!", ch, NULL, victim, TO_VICT);
      send_to_char("You drive your shield into your target with fortress-wall force!\n\r", ch);

      calculate_damage(ch, victim, dam, gsn_rampart_slam, ELE_PHYSICAL, TRUE);

      /* Stun for 1 round - same mechanic as bash */
      if (victim->position > POS_STUNNED)
      {
         victim->position = POS_STUNNED;
         victim->stunTimer += 1;
         act("$N is stunned by the impact!", ch, NULL, victim, TO_CHAR);
         act("You are stunned by the impact!", ch, NULL, victim, TO_VICT);
      }

      combo(ch, victim, gsn_rampart_slam);
   }
   else
   {
      send_to_char("Your rampart slam misses!\n\r", ch);
      set_fighting(victim, ch, TRUE);
   }
}
