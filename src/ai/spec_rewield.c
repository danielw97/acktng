#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_rewield(CHAR_DATA *ch)
{
   /*
    * If mob is fighting, try and get weapon and wield it
    * * if not fighting, check room and inv to see if any better weapons
    * * -Stephen
    */

   OBJ_DATA *obj;
   OBJ_DATA *weapon = NULL;
   int dam;
   int chance;
   bool pickup;
   char buf[MAX_STRING_LENGTH];

   pickup = TRUE;
   dam = 0;

   chance = (!is_fighting(ch) ? 35 : 60);

   if (number_percent() < chance)
   {
      for (obj = ch->first_carry; obj != NULL; obj = obj->next_in_carry_list)
         if (obj->item_type == ITEM_WEAPON && dam < obj->value[2])
         {
            dam = obj->value[2];
            pickup = FALSE;
            weapon = obj;
         }

      /*
       * Then check inventory and room for any weapons
       */
      for (obj = ch->in_room->first_content; obj != NULL; obj = obj->next_in_room)
      {
         if (obj->item_type == ITEM_WEAPON)
         {
            if (obj->value[2] > dam)
            {
               dam = obj->value[2];
               weapon = obj;
               pickup = TRUE;
            }
         }
      }

      if (weapon == NULL)
         return FALSE;

      if (weapon->wear_loc == WEAR_HOLD_HAND_L)
         return FALSE;

      if (pickup)
      {
         sprintf(buf, "Great!  %s!  Just what i've always wanted!", weapon->short_descr);
         do_say(ch, buf);
      }

      if (weapon != NULL)
      {
         /*
          * Now make the mob get the weapon
          */
         if (pickup)
            get_obj(ch, weapon, NULL);

         do_wear(ch, weapon->name);

         /*
          * Check is mob wielded weapon ok...
          */
         if (weapon->wear_loc == WEAR_NONE)
         {
            act("$n sniffs sadly.  'Baah!  It's no good to me!'", ch, NULL, NULL, TO_ROOM);
            extract_obj(weapon);
            act("$n sacrifices $p.", ch, weapon, NULL, TO_ROOM);
         }

         return TRUE;
      }
   }

   return FALSE;
}
