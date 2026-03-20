#include "globals.h"
#include "magic.h"
#include "skills.h"

void do_containment_ward(CHAR_DATA *ch, char *argument)
{
   ROOM_AFFECT_DATA raf;
   char arg[MAX_INPUT_LENGTH];
   int door;

   if (!can_use_pub_society_skill(ch, gsn_containment_ward))
   {
      send_to_char("You don't know how to use this skill!\n\r", ch);
      return;
   }

   if (!can_use_skill_message(ch, gsn_containment_ward))
      return;

   one_argument(argument, arg);
   if (arg[0] == '\0')
   {
      send_to_char("Place a containment ward on which exit? (n/e/s/w/u/d)\n\r", ch);
      return;
   }

   /* Parse direction */
   door = -1;
   if (!str_prefix(arg, "north"))
      door = 0;
   else if (!str_prefix(arg, "east"))
      door = 1;
   else if (!str_prefix(arg, "south"))
      door = 2;
   else if (!str_prefix(arg, "west"))
      door = 3;
   else if (!str_prefix(arg, "up"))
      door = 4;
   else if (!str_prefix(arg, "down"))
      door = 5;

   if (door < 0 || ch->in_room->exit[door] == NULL)
   {
      send_to_char("There is no exit in that direction.\n\r", ch);
      return;
   }

   WAIT_STATE(ch, skill_table[gsn_containment_ward].beats);
   raise_skill(ch, gsn_containment_ward);

   /* Apply a room affect to mark the ward */
   raf.type = gsn_containment_ward;
   raf.duration = 10; /* ~10 minutes */
   raf.modifier = door;
   raf.bitvector = 0;
   raf.caster = ch;
   affect_to_room(ch->in_room, &raf);

   send_to_char("You place a containment ward on the exit.\n\r", ch);
   act("$n places a shimmering ward on an exit.", ch, NULL, NULL, TO_ROOM);
}
