#include "globals.h"
#include "magic.h"
#include "skills.h"

void do_shelter(CHAR_DATA *ch, char *argument)
{
   ROOM_AFFECT_DATA raf;

   if (!can_use_pub_society_skill(ch, gsn_shelter))
   {
      send_to_char("You don't know how to use this skill!\n\r", ch);
      return;
   }

   if (!can_use_skill_message(ch, gsn_shelter))
      return;

   if (ch->in_room->sector_type == SECT_INSIDE)
   {
      send_to_char("You can only construct a shelter outdoors.\n\r", ch);
      return;
   }

   WAIT_STATE(ch, skill_table[gsn_shelter].beats);
   raise_skill(ch, gsn_shelter);

   raf.type = gsn_shelter;
   raf.duration = 60; /* ~1 hour */
   raf.modifier = 0;
   raf.bitvector = 0;
   raf.caster = ch;
   affect_to_room(ch->in_room, &raf);

   send_to_char("You construct a temporary shelter.\n\r", ch);
   act("$n constructs a temporary shelter.", ch, NULL, NULL, TO_ROOM);
}
