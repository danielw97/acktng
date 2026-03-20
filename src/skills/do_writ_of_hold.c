#include "globals.h"
#include "magic.h"
#include "skills.h"

void do_writ_of_hold(CHAR_DATA *ch, char *argument)
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   AFFECT_DATA af;

   if (!can_use_pub_society_skill(ch, gsn_writ_of_hold))
   {
      send_to_char("You don't know how to use this skill!\n\r", ch);
      return;
   }

   if (!can_use_skill_message(ch, gsn_writ_of_hold))
      return;

   one_argument(argument, arg);
   if (arg[0] == '\0')
   {
      send_to_char("Serve a writ of hold on whom?\n\r", ch);
      return;
   }

   if ((victim = get_char_room(ch, arg)) == NULL)
   {
      send_to_char("They aren't here.\n\r", ch);
      return;
   }

   if (!IS_NPC(victim))
   {
      send_to_char("The writ of hold only applies to NPCs.\n\r", ch);
      return;
   }

   if (IS_SET(victim->act, ACT_BOSS))
   {
      send_to_char("This creature is too powerful for a writ of hold.\n\r", ch);
      return;
   }

   if (is_affected(victim, gsn_writ_of_hold))
   {
      send_to_char("They are already under a writ of hold.\n\r", ch);
      return;
   }

   WAIT_STATE(ch, skill_table[gsn_writ_of_hold].beats);
   raise_skill(ch, gsn_writ_of_hold);

   af.type = gsn_writ_of_hold;
   af.duration = 5; /* ~5 minutes */
   af.duration_type = DURATION_HOUR;
   af.location = APPLY_NONE;
   af.modifier = 0;
   af.bitvector = AFF_HOLD;
   af.caster = ch;
   affect_to_char(victim, &af);

   act("You present a writ of hold to $N. They are bound in place.", ch, NULL, victim, TO_CHAR);
   act("$n presents an official writ. $N is bound in place.", ch, NULL, victim, TO_ROOM);
}
