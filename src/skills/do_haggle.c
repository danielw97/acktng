#include "globals.h"
#include "magic.h"
#include "skills.h"

void do_haggle(CHAR_DATA *ch, char *argument)
{
   AFFECT_DATA af;

   if (!can_use_pub_society_skill(ch, gsn_haggle))
   {
      send_to_char("You don't know how to use this skill!\n\r", ch);
      return;
   }

   if (!can_use_skill_message(ch, gsn_haggle))
      return;

   if (is_affected(ch, gsn_haggle))
   {
      send_to_char("You are already haggling for better prices.\n\r", ch);
      return;
   }

   WAIT_STATE(ch, skill_table[gsn_haggle].beats);
   ch->cooldown[gsn_haggle] = 225; /* ~15 minutes */
   raise_skill(ch, gsn_haggle);

   /* Apply a temporary affect that shop buy routines can check */
   af.type = gsn_haggle;
   af.duration = 5;
   af.duration_type = DURATION_HOUR;
   af.location = APPLY_NONE;
   af.modifier = 10; /* 10% discount stored in modifier */
   af.bitvector = 0;
   af.caster = ch;
   affect_to_char(ch, &af);

   send_to_char("You prepare to haggle for better prices at shops.\n\r", ch);
}
