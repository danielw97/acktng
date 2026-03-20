#include "globals.h"
#include "magic.h"
#include "skills.h"

void do_trailblaze(CHAR_DATA *ch, char *argument)
{
   char arg[MAX_INPUT_LENGTH];
   char buf[MAX_STRING_LENGTH];
   int door;

   if (!can_use_pub_society_skill(ch, gsn_trailblaze))
   {
      send_to_char("You don't know how to use this skill!\n\r", ch);
      return;
   }

   if (!can_use_skill_message(ch, gsn_trailblaze))
      return;

   one_argument(argument, arg);
   if (arg[0] == '\0')
   {
      send_to_char("Mark a trail in which direction? (n/e/s/w/u/d)\n\r", ch);
      return;
   }

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

   WAIT_STATE(ch, skill_table[gsn_trailblaze].beats);
   raise_skill(ch, gsn_trailblaze);

   sprintf(buf, "You mark a trail pointing %s.\n\r", dir_name[door]);
   send_to_char(buf, ch);
   act("$n marks a trail on the ground.", ch, NULL, NULL, TO_ROOM);
}
