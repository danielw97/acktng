#include "globals.h"
#include "magic.h"
#include "skills.h"

void do_claws_pursuit(CHAR_DATA *ch, char *argument)
{
   AFFECT_DATA af;

   if (!can_use_pub_society_skill(ch, gsn_claws_pursuit))
   {
      send_to_char("You don't know how to use this skill!\n\r", ch);
      return;
   }

   if (!can_use_skill_message(ch, gsn_claws_pursuit))
      return;

   if (is_affected(ch, gsn_claws_pursuit))
   {
      send_to_char("You are already in pursuit.\n\r", ch);
      return;
   }

   WAIT_STATE(ch, skill_table[gsn_claws_pursuit].beats);
   ch->cooldown[gsn_claws_pursuit] = 150; /* ~10 minutes */
   raise_skill(ch, gsn_claws_pursuit);

   af.type = gsn_claws_pursuit;
   af.duration = 3;
   af.duration_type = DURATION_HOUR;
   af.location = APPLY_MOVE;
   af.modifier = 500; /* bonus move points for pursuit */
   af.bitvector = 0;
   af.caster = ch;
   affect_to_char(ch, &af);

   send_to_char("@@WYou invoke the Shrine's pursuit rite. Your senses sharpen.@@N\n\r", ch);
   act("$n's eyes narrow with predatory focus.", ch, NULL, NULL, TO_ROOM);
}
