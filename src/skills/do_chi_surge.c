#include "globals.h"
#include "magic.h"
#include "skills.h"

void do_chi_surge(CHAR_DATA *ch, char *argument)
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];
   int dam;
   int spent;

   one_argument(argument, arg);

   if (!is_fighting(ch))
   {
      send_to_char("You can only perform a chi surge when fighting!\n\r", ch);
      return;
   }

   if (!can_use_skill_message(ch, gsn_chi_surge))
      return;

   if (get_chi(ch) < 10)
   {
      send_to_char("You need at least 10 chi to perform a chi surge!\n\r", ch);
      return;
   }

   if (arg[0] == '\0')
      victim = ch->fighting;
   else
      victim = get_char_room(ch, arg);

   if (victim == NULL)
   {
      send_to_char("No such victim!\n\r", ch);
      return;
   }

   spent = ch->chi;
   ch->chi = 0;

   raise_skill(ch, gsn_chi_surge);

   WAIT_STATE(ch, skill_table[gsn_chi_surge].beats);

   ch->cooldown[gsn_chi_surge] = 15;

   dam = number_range(get_psuedo_level(ch) * get_curr_str(ch) / 30,
                      get_psuedo_level(ch) * get_curr_str(ch) / 20);

   if (dam < 1)
      dam = 1;

   dam = dam * spent / 5;

   if (IS_NPC(ch))
      dam /= 2;

   check_killer(ch, victim);
   if (can_hit_skill(ch, victim, gsn_chi_surge))
   {
      act("$n channels all $s chi into a single devastating strike on $N!", ch, NULL, victim,
          TO_NOTVICT);
      act("$n channels all $s chi into a devastating strike that slams into you!", ch, NULL, victim,
          TO_VICT);
      act("You channel all your chi into a devastating strike on $N!", ch, NULL, victim, TO_CHAR);

      calculate_damage(ch, victim, dam, gsn_chi_surge, ELE_PHYSICAL, TRUE);
   }
   else
   {
      act("$n tries to channel a chi surge at $N, but misses!", ch, NULL, victim, TO_NOTVICT);
      act("$n tries to channel a chi surge at you, but misses!", ch, NULL, victim, TO_VICT);
      act("You try to channel a chi surge at $N, but miss!", ch, NULL, victim, TO_CHAR);
      set_fighting(victim, ch, TRUE);
   }
}
