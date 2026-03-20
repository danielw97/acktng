#include "globals.h"
#include "magic.h"
#include "skills.h"

void do_dig_in(CHAR_DATA *ch, char *argument)
{
   AFFECT_DATA af;

   if (!can_use_pub_society_skill(ch, gsn_dig_in))
   {
      send_to_char("You don't know how to use this skill!\n\r", ch);
      return;
   }

   if (!can_use_skill_message(ch, gsn_dig_in))
      return;

   if (is_affected(ch, gsn_dig_in))
   {
      send_to_char("You are already dug in at a position.\n\r", ch);
      return;
   }

   WAIT_STATE(ch, skill_table[gsn_dig_in].beats);
   raise_skill(ch, gsn_dig_in);

   af.type = gsn_dig_in;
   af.duration = 10; /* ~10 minutes */
   af.duration_type = DURATION_HOUR;
   af.location = APPLY_AC;
   af.modifier = -5;
   af.bitvector = 0;
   af.caster = ch;
   affect_to_char(ch, &af);

   send_to_char("You fortify your position and dig in.\n\r", ch);
   act("$n fortifies $s position.", ch, NULL, NULL, TO_ROOM);
}
