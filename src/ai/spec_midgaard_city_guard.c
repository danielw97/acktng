#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_midgaard_city_guard(CHAR_DATA *ch)
{
   static const char *acts[] = {
       "$n pauses at the intersection and scans both approaches before resuming $s patrol.",
       "$n checks the chalked duty roster at the pillar post and notes $s shift's remaining time.",
       "$n rests one hand on $s weapon pommel and watches a passing cart with professional "
       "attention.",
       "$n exchanges a terse word with a patrol partner before continuing the circuit.",
       "$n tests the lamp bracket on the nearest post, satisfied it holds, and moves on."};
   static char *says[] = {
       "The Watch covers this district on a rotating four-bell circuit. Every corner, every alley.",
       "Trouble in the Gloamvault approaches gets escalated to the Warden authority. The Watch "
       "handles the street.",
       "Move along if you're not conducting civic business. Loitering in the temple ward draws "
       "attention.",
       "Carrying unregistered arcane materials in the northern district is a Compact violation. "
       "The warden post is at the checkpoint arch.",
       "Night patrol is heavier near the caravan square. Trade goods attract the wrong kind of "
       "interest after Bell Watch."};

   char buf[MAX_STRING_LENGTH];
   CHAR_DATA *victim;
   char *crime;

   if (!IS_AWAKE(ch) || is_fighting(ch))
      return FALSE;

   /* Criminal check first */
   crime = "";
   for (victim = ch->in_room->first_person; victim != NULL; victim = victim->next_in_room)
   {
      if (!IS_NPC(victim) && IS_SET(victim->act, PLR_KILLER))
      {
         crime = "KILLER";
         break;
      }
      if (!IS_NPC(victim) && IS_SET(victim->act, PLR_THIEF))
      {
         crime = "THIEF";
         break;
      }
   }

   if (victim != NULL)
   {
      if (ch->hunting || ch->hunt_obj)
         return FALSE;
      sprintf(buf, "Halt! %s is a %s — by order of the Watch, stand down!", victim->name, crime);
      do_yell(ch, buf);
      set_hunt(ch, NULL, victim, NULL, 0, 0);
      return TRUE;
   }

   /* Ambient patrol flavour */
   if (number_bits(3) != 0)
      return FALSE;

   if (number_bits(1) == 0)
      act(acts[number_range(0, 4)], ch, NULL, NULL, TO_ROOM);
   else
      do_say(ch, says[number_range(0, 4)]);

   return FALSE;
}
