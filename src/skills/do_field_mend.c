#include "globals.h"
#include "magic.h"
#include "skills.h"

void do_field_mend(CHAR_DATA *ch, char *argument)
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   int heal_amount;

   if (!can_use_pub_society_skill(ch, gsn_field_mend))
   {
      send_to_char("You don't know how to use this skill!\n\r", ch);
      return;
   }

   if (!can_use_skill_message(ch, gsn_field_mend))
      return;

   if (is_fighting(ch))
   {
      send_to_char("You cannot perform field medicine while fighting!\n\r", ch);
      return;
   }

   one_argument(argument, arg);

   if (arg[0] == '\0')
      victim = ch;
   else
   {
      if ((victim = get_char_room(ch, arg)) == NULL)
      {
         send_to_char("They aren't here.\n\r", ch);
         return;
      }
   }

   WAIT_STATE(ch, 60);
   raise_skill(ch, gsn_field_mend);

   /* Small, flat heal that doesn't scale with level */
   heal_amount = dice(3, 10) + 20;
   victim->hit = UMIN(victim->hit + heal_amount, victim->max_hit);

   if (victim == ch)
   {
      send_to_char("You perform basic field medicine on your wounds.\n\r", ch);
      act("$n performs basic field medicine on $mself.", ch, NULL, NULL, TO_ROOM);
   }
   else
   {
      act("You perform basic field medicine on $N's wounds.", ch, NULL, victim, TO_CHAR);
      act("$n performs basic field medicine on your wounds.", ch, NULL, victim, TO_VICT);
      act("$n performs basic field medicine on $N's wounds.", ch, NULL, victim, TO_NOTVICT);
   }
}
