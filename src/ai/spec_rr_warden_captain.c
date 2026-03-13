#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_rr_warden_captain(CHAR_DATA *ch)
{
   static const char *acts[] = {
      "$n studies a patrol assignment board, adjusting shift notations with a chalk stick.",
      "$n paces the warden post perimeter, checking sight-lines along both stretches of road.",
      "$n reads a dispatch report, folds it precisely, and slides it into the locked post-box.",
      "$n reviews a wanted notice against $s register, then pins it to the duty-board with practiced authority.",
      "$n speaks a quiet word with a junior warden, gesturing toward the road's western stretch.",
      "$n polishes the badge of rank on $s chest with a corner of the patrol cloak, force of long habit."
   };
   static char *says[] = {
      "This post covers the full charter march from the gate to the boundary stone. Three wardens per shift, rotating dawn and dusk.",
      "The Three Civic Oaths are not decoration. Every warden recites them at shift change. Every single shift.",
      "I've requisitioned repair crews for the drainage channel at the grain field crossing. The order is six weeks old. It's still pending.",
      "Any report of trouble west of the boundary stone goes to both my log and the convoy compact. Joint jurisdiction means joint notification.",
      "The warden who doesn't know every face on their stretch doesn't know their stretch. Walk it until you do."
   };
   /* Roc Road main route from Midgaard (east) to Kiess (west) */
   static const int route[] = {
      3201, 3202, 3203, 3204, 3205, 3206, 3207, 3208, 3209, 3210,
      3211, 3212, 3213, 3214, 3215, 3216, 3217, 3218, 3219, 3220,
      3221, 3222, 3223, 3224, 3225, 3226, 3227, 3228, 3229, 3230,
      3231, 3232, 3233, 3234, 3235, 3236, 3237, 3238, 3239, 3240,
      3241, 3242, 3243
   };
   static const int route_len = sizeof(route) / sizeof(route[0]);
   static const int endpoint_a = 3201; /* Midgaard end */
   static const int endpoint_b = 3243; /* Kiess end */

   int cur_vnum, cur_idx, tgt_idx, next_vnum, door, i;
   EXIT_DATA *pexit;

   if (!IS_AWAKE(ch) || is_fighting(ch))
      return FALSE;

   /* Occasional flavor */
   if (number_bits(3) == 0)
   {
      if (number_bits(1) == 0)
         act(acts[number_range(0, 5)], ch, NULL, NULL, TO_ROOM);
      else
         do_say(ch, says[number_range(0, 4)]);
   }

   /* Patrol movement: find current position on route */
   cur_vnum = ch->in_room->vnum;
   cur_idx = -1;
   for (i = 0; i < route_len; i++)
   {
      if (route[i] == cur_vnum)
      {
         cur_idx = i;
         break;
      }
   }

   if (cur_idx < 0)
      return FALSE; /* Not on patrol route, allow normal behavior */

   /* Initialize or flip patrol target when arriving at an endpoint */
   if (ch->hunt_home == NULL || ch->hunt_home->vnum == cur_vnum)
   {
      int new_target = (cur_vnum == endpoint_b) ? endpoint_a : endpoint_b;
      ch->hunt_home = get_room_index(new_target);
      if (ch->hunt_home == NULL)
         return FALSE;
   }

   /* Find target position in route */
   tgt_idx = -1;
   for (i = 0; i < route_len; i++)
   {
      if (route[i] == ch->hunt_home->vnum)
      {
         tgt_idx = i;
         break;
      }
   }

   if (tgt_idx < 0)
      return FALSE;

   /* Determine next room along the route */
   next_vnum = (cur_idx < tgt_idx) ? route[cur_idx + 1] : route[cur_idx - 1];

   /* Find the exit that leads to the next room and move */
   for (door = 0; door <= 5; door++)
   {
      pexit = ch->in_room->exit[door];
      if (pexit != NULL && pexit->to_room != NULL
          && pexit->to_room->vnum == next_vnum
          && !IS_SET(pexit->exit_info, EX_CLOSED))
      {
         move_char(ch, door);
         return TRUE;
      }
   }

   return TRUE; /* On route but couldn't move; prevent random wandering */
}
