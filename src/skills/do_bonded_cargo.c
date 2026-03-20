#include "globals.h"
#include "magic.h"
#include "skills.h"

void do_bonded_cargo(CHAR_DATA *ch, char *argument)
{
   char arg[MAX_INPUT_LENGTH];
   OBJ_DATA *obj;
   AFFECT_DATA af;

   if (!can_use_pub_society_skill(ch, gsn_bonded_cargo))
   {
      send_to_char("You don't know how to use this skill!\n\r", ch);
      return;
   }

   if (!can_use_skill_message(ch, gsn_bonded_cargo))
      return;

   one_argument(argument, arg);
   if (arg[0] == '\0')
   {
      send_to_char("Seal which item in a bonded cargo container?\n\r", ch);
      return;
   }

   if ((obj = get_obj_carry(ch, arg)) == NULL)
   {
      send_to_char("You aren't carrying that.\n\r", ch);
      return;
   }

   WAIT_STATE(ch, skill_table[gsn_bonded_cargo].beats);
   raise_skill(ch, gsn_bonded_cargo);

   /* Apply the bonded seal - makes item no_drop, no_remove for protection */
   SET_BIT(obj->extra_flags, ITEM_NODROP);

   /* Apply a timed affect to the owner so we can track the seal */
   af.type = gsn_bonded_cargo;
   af.duration = 60; /* ~1 hour */
   af.duration_type = DURATION_HOUR;
   af.location = APPLY_NONE;
   af.modifier = 0;
   af.bitvector = 0;
   af.caster = ch;
   affect_to_char(ch, &af);

   act("You seal $p in a bonded cargo container.", ch, obj, NULL, TO_CHAR);
   act("$n seals $p with an official Syndics bond.", ch, obj, NULL, TO_ROOM);
}
