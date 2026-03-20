#include "globals.h"
#include "magic.h"
#include "skills.h"

void do_forced_march(CHAR_DATA *ch, char *argument)
{
   AFFECT_DATA af;

   if (!can_use_pub_society_skill(ch, gsn_forced_march))
   {
      send_to_char("You don't know how to use this skill!\n\r", ch);
      return;
   }

   if (!can_use_skill_message(ch, gsn_forced_march))
      return;

   if (is_affected(ch, gsn_forced_march))
   {
      send_to_char("You are already on a forced march.\n\r", ch);
      return;
   }

   WAIT_STATE(ch, skill_table[gsn_forced_march].beats);
   ch->cooldown[gsn_forced_march] = 450; /* ~30 minutes */
   raise_skill(ch, gsn_forced_march);

   af.type = gsn_forced_march;
   af.duration = 5; /* ~5 minutes */
   af.duration_type = DURATION_HOUR;
   af.location = APPLY_MOVE;
   af.modifier = 1000; /* large move bonus */
   af.bitvector = 0;
   af.caster = ch;
   affect_to_char(ch, &af);

   send_to_char("You set out on a forced march! Movement costs are reduced.\n\r", ch);
   act("$n sets out at a punishing pace.", ch, NULL, NULL, TO_ROOM);
}
