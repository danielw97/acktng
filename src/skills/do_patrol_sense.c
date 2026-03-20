#include "globals.h"
#include "magic.h"
#include "skills.h"

void do_patrol_sense(CHAR_DATA *ch, char *argument)
{
   char buf[MAX_STRING_LENGTH];
   EXIT_DATA *pexit;
   int door;
   int found = 0;

   if (!can_use_pub_society_skill(ch, gsn_patrol_sense))
   {
      send_to_char("You don't know how to use this skill!\n\r", ch);
      return;
   }

   if (!can_use_skill_message(ch, gsn_patrol_sense))
      return;

   WAIT_STATE(ch, skill_table[gsn_patrol_sense].beats);
   raise_skill(ch, gsn_patrol_sense);

   send_to_char("You scan the area for threats...\n\r", ch);

   for (door = 0; door < 6; door++)
   {
      if ((pexit = ch->in_room->exit[door]) == NULL)
         continue;
      if (pexit->to_room == NULL)
         continue;

      CHAR_DATA *mob;
      for (mob = pexit->to_room->first_person; mob != NULL; mob = mob->next_in_room)
      {
         if (!IS_NPC(mob))
            continue;
         if (!IS_SET(mob->act, ACT_AGGRESSIVE))
            continue;

         char *strength;
         if (mob->level < ch->level - 20)
            strength = "@@dweak@@N";
         else if (mob->level < ch->level + 10)
            strength = "@@ymoderate@@N";
         else
            strength = "@@Rdangerous@@N";

         sprintf(buf, "  %s: %s threat detected (%s)\n\r", dir_name[door], strength,
                 mob->short_descr);
         send_to_char(buf, ch);
         found++;
      }
   }

   if (found == 0)
      send_to_char("  No immediate threats detected nearby.\n\r", ch);
}
