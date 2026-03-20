#include "globals.h"
#include "magic.h"
#include "skills.h"

void do_road_camp(CHAR_DATA *ch, char *argument)
{
   ROOM_AFFECT_DATA raf;

   if (!can_use_pub_society_skill(ch, gsn_road_camp))
   {
      send_to_char("You don't know how to use this skill!\n\r", ch);
      return;
   }

   if (!can_use_skill_message(ch, gsn_road_camp))
      return;

   if (ch->in_room->sector_type == SECT_INSIDE)
   {
      send_to_char("You can only establish a road camp outdoors.\n\r", ch);
      return;
   }

   WAIT_STATE(ch, skill_table[gsn_road_camp].beats);
   raise_skill(ch, gsn_road_camp);

   raf.type = gsn_road_camp;
   raf.duration = 30; /* ~30 minutes */
   raf.modifier = 0;
   raf.bitvector = ROOM_SAFE;
   raf.caster = ch;
   affect_to_room(ch->in_room, &raf);

   send_to_char("You establish a road camp. This area is temporarily safe.\n\r", ch);
   act("$n establishes a road camp.", ch, NULL, NULL, TO_ROOM);
}
