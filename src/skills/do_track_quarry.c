#include "globals.h"
#include "magic.h"
#include "skills.h"

void do_track_quarry(CHAR_DATA *ch, char *argument)
{
   char arg[MAX_INPUT_LENGTH];
   char buf[MAX_STRING_LENGTH];
   CHAR_DATA *victim;

   if (!can_use_pub_society_skill(ch, gsn_track_quarry))
   {
      send_to_char("You don't know how to use this skill!\n\r", ch);
      return;
   }

   if (!can_use_skill_message(ch, gsn_track_quarry))
      return;

   one_argument(argument, arg);
   if (arg[0] == '\0')
   {
      send_to_char("Track whom?\n\r", ch);
      return;
   }

   WAIT_STATE(ch, skill_table[gsn_track_quarry].beats);
   ch->cooldown[gsn_track_quarry] = 75; /* ~5 minutes */
   raise_skill(ch, gsn_track_quarry);

   /* Search for the target in nearby rooms */
   for (int door = 0; door < 6; door++)
   {
      EXIT_DATA *pexit = ch->in_room->exit[door];
      if (pexit == NULL || pexit->to_room == NULL)
         continue;

      for (victim = pexit->to_room->first_person; victim != NULL; victim = victim->next_in_room)
      {
         if (!can_see(ch, victim))
            continue;
         if (!is_name(arg, victim->name))
            continue;

         sprintf(buf, "You detect signs of %s to the %s.\n\r",
                 IS_NPC(victim) ? victim->short_descr : victim->name, dir_name[door]);
         send_to_char(buf, ch);
         return;
      }
   }

   send_to_char("You find no trace of your quarry nearby.\n\r", ch);
}
