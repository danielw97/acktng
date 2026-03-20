#include "globals.h"
#include "magic.h"
#include "skills.h"

void do_dockside_audit(CHAR_DATA *ch, char *argument)
{
   char arg[MAX_INPUT_LENGTH];
   char buf[MAX_STRING_LENGTH];
   CHAR_DATA *victim;
   OBJ_DATA *obj;
   int total_value = 0;
   int item_count = 0;

   if (!can_use_pub_society_skill(ch, gsn_dockside_audit))
   {
      send_to_char("You don't know how to use this skill!\n\r", ch);
      return;
   }

   if (!can_use_skill_message(ch, gsn_dockside_audit))
      return;

   one_argument(argument, arg);
   if (arg[0] == '\0')
   {
      send_to_char("Audit whom?\n\r", ch);
      return;
   }

   if ((victim = get_char_room(ch, arg)) == NULL)
   {
      send_to_char("They aren't here.\n\r", ch);
      return;
   }

   WAIT_STATE(ch, skill_table[gsn_dockside_audit].beats);
   raise_skill(ch, gsn_dockside_audit);

   sprintf(buf, "\n\r@@yDockside Audit: %s@@N\n\r",
           IS_NPC(victim) ? victim->short_descr : victim->name);
   send_to_char(buf, ch);

   /* Show equipped items */
   for (obj = victim->first_carry; obj != NULL; obj = obj->next_in_carry_list)
   {
      if (obj->wear_loc == WEAR_NONE)
         continue;
      /* For player targets, only show visibly worn gear */
      if (!IS_NPC(victim) && !can_see_obj(ch, obj))
         continue;

      sprintf(buf, "  @@W%-25s@@N (value: %d gold)\n\r",
              obj->short_descr ? obj->short_descr : obj->name, obj->cost);
      send_to_char(buf, ch);
      total_value += obj->cost;
      item_count++;
   }

   /* For NPCs, also show carried items */
   if (IS_NPC(victim))
   {
      for (obj = victim->first_carry; obj != NULL; obj = obj->next_in_carry_list)
      {
         if (obj->wear_loc != WEAR_NONE)
            continue;
         sprintf(buf, "  @@d%-25s@@N (carried, value: %d gold)\n\r",
                 obj->short_descr ? obj->short_descr : obj->name, obj->cost);
         send_to_char(buf, ch);
         total_value += obj->cost;
         item_count++;
      }
   }

   if (item_count == 0)
      send_to_char("  No items detected.\n\r", ch);
   else
   {
      sprintf(buf, "  @@yTotal assessed value: %d gold (%d items)@@N\n\r", total_value, item_count);
      send_to_char(buf, ch);
   }

   if (IS_NPC(victim))
   {
      sprintf(buf, "  Gold carried: %d\n\r", victim->gold);
      send_to_char(buf, ch);
   }
}
