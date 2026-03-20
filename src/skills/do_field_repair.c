#include "globals.h"
#include "magic.h"
#include "skills.h"

void do_field_repair(CHAR_DATA *ch, char *argument)
{
   char arg[MAX_INPUT_LENGTH];
   OBJ_DATA *obj;

   if (!can_use_pub_society_skill(ch, gsn_field_repair))
   {
      send_to_char("You don't know how to use this skill!\n\r", ch);
      return;
   }

   if (!can_use_skill_message(ch, gsn_field_repair))
      return;

   if (is_fighting(ch))
   {
      send_to_char("You cannot perform repairs while fighting!\n\r", ch);
      return;
   }

   one_argument(argument, arg);
   if (arg[0] == '\0')
   {
      send_to_char("Repair which item?\n\r", ch);
      return;
   }

   if ((obj = get_obj_carry(ch, arg)) == NULL)
   {
      send_to_char("You aren't carrying that.\n\r", ch);
      return;
   }

   WAIT_STATE(ch, 60);
   ch->cooldown[gsn_field_repair] = 150; /* ~10 minutes */
   raise_skill(ch, gsn_field_repair);

   act("You carefully repair $p.", ch, obj, NULL, TO_CHAR);
   act("$n carefully repairs $p.", ch, obj, NULL, TO_ROOM);
}
