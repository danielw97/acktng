#include "globals.h"
#include "magic.h"
#include "skills.h"

void do_signal_tower(CHAR_DATA *ch, char *argument)
{
   ROOM_AFFECT_DATA raf;

   if (!can_use_pub_society_skill(ch, gsn_signal_tower))
   {
      send_to_char("You don't know how to use this skill!\n\r", ch);
      return;
   }

   if (!can_use_skill_message(ch, gsn_signal_tower))
      return;

   if (ch->in_room->sector_type == SECT_INSIDE)
   {
      send_to_char("You can only erect a signal tower outdoors.\n\r", ch);
      return;
   }

   WAIT_STATE(ch, skill_table[gsn_signal_tower].beats);
   raise_skill(ch, gsn_signal_tower);

   raf.type = gsn_signal_tower;
   raf.duration = 30; /* ~30 minutes */
   raf.modifier = 0;
   raf.bitvector = 0;
   raf.caster = ch;
   affect_to_room(ch->in_room, &raf);

   send_to_char("You erect a temporary signal tower.\n\r", ch);
   act("$n erects a temporary signal tower.", ch, NULL, NULL, TO_ROOM);
}
