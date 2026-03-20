#include "globals.h"
#include "magic.h"
#include "skills.h"

void do_forage(CHAR_DATA *ch, char *argument)
{
   if (!can_use_pub_society_skill(ch, gsn_forage))
   {
      send_to_char("You don't know how to use this skill!\n\r", ch);
      return;
   }

   if (!can_use_skill_message(ch, gsn_forage))
      return;

   if (ch->in_room->sector_type != SECT_FOREST && ch->in_room->sector_type != SECT_FIELD &&
       ch->in_room->sector_type != SECT_HILLS && ch->in_room->sector_type != SECT_MOUNTAIN)
   {
      send_to_char("You can only forage in outdoor wilderness areas.\n\r", ch);
      return;
   }

   WAIT_STATE(ch, skill_table[gsn_forage].beats);
   ch->cooldown[gsn_forage] = 45; /* ~3 minutes */
   raise_skill(ch, gsn_forage);

   /* Simple forage: restore some move and provide flavor */
   int restore = dice(2, 10) + 10;
   ch->move = UMIN(ch->move + restore, ch->max_move);

   send_to_char("You forage through the area and find useful botanical materials.\n\r", ch);
   act("$n forages through the undergrowth.", ch, NULL, NULL, TO_ROOM);

   /* Small chance of finding a healing herb effect */
   if (number_percent() < 25)
   {
      int heal = dice(2, 8) + 5;
      ch->hit = UMIN(ch->hit + heal, ch->max_hit);
      send_to_char("You find a medicinal herb and chew it for a small boost.\n\r", ch);
   }
}
