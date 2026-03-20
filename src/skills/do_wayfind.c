#include "globals.h"
#include "magic.h"
#include "skills.h"

void do_wayfind(CHAR_DATA *ch, char *argument)
{
   if (!can_use_pub_society_skill(ch, gsn_wayfind))
   {
      send_to_char("You don't know how to use this skill!\n\r", ch);
      return;
   }

   if (!can_use_skill_message(ch, gsn_wayfind))
      return;

   WAIT_STATE(ch, skill_table[gsn_wayfind].beats);
   raise_skill(ch, gsn_wayfind);

   send_to_char("You study the terrain and get your bearings...\n\r", ch);

   /* Show available exits with brief descriptions */
   int door;
   int found = 0;
   char buf[MAX_STRING_LENGTH];

   for (door = 0; door < 6; door++)
   {
      EXIT_DATA *pexit = ch->in_room->exit[door];
      if (pexit == NULL || pexit->to_room == NULL)
         continue;

      sprintf(buf, "  %s: %s\n\r", dir_name[door], pexit->to_room->name);
      send_to_char(buf, ch);
      found++;
   }

   if (found == 0)
      send_to_char("  No obvious exits from here.\n\r", ch);
}
