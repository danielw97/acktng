#include "globals.h"
#include "magic.h"
#include "skills.h"

void do_oath_mark(CHAR_DATA *ch, char *argument)
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   AFFECT_DATA af;

   if (!can_use_pub_society_skill(ch, gsn_oath_mark))
   {
      send_to_char("You don't know how to use this skill!\n\r", ch);
      return;
   }

   if (!can_use_skill_message(ch, gsn_oath_mark))
      return;

   one_argument(argument, arg);
   if (arg[0] == '\0')
   {
      send_to_char("Mark whom as an oath-subject?\n\r", ch);
      return;
   }

   if ((victim = get_char_room(ch, arg)) == NULL)
   {
      send_to_char("They aren't here.\n\r", ch);
      return;
   }

   if (victim == ch)
   {
      send_to_char("You cannot oath mark yourself.\n\r", ch);
      return;
   }

   if (is_affected(victim, gsn_oath_mark))
   {
      send_to_char("That target already bears an oath mark.\n\r", ch);
      return;
   }

   WAIT_STATE(ch, skill_table[gsn_oath_mark].beats);
   raise_skill(ch, gsn_oath_mark);

   af.type = gsn_oath_mark;
   af.duration = 60; /* ~1 hour */
   af.duration_type = DURATION_HOUR;
   af.location = APPLY_NONE;
   af.modifier = 0;
   af.bitvector = 0;
   af.caster = ch;
   affect_to_char(victim, &af);

   act("You mark $N as an oath-subject of the Shrine.", ch, NULL, victim, TO_CHAR);
   /* The mark is visible only to Shrine members */
   send_to_char("@@dYou feel a faint chill.@@N\n\r", victim);
}
