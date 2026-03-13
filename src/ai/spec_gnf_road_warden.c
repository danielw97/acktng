#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_gnf_road_warden(CHAR_DATA *ch)
{
   static const char *acts[] = {
      "$n scans the treeline with practiced eyes, one hand resting on $s blade hilt.",
      "$n checks the condition of a nearby waystone, testing the iron lantern bracket for rust.",
      "$n marks something in a field ledger, then tucks it back into $s patrol coat.",
      "$n adjusts the joint-commission sash at $s waist, straightening the dual-city insignia.",
      "$n pauses at a road drainage ditch and kicks a clump of leaf debris clear with a boot.",
      "$n taps a waystone twice with a knuckle — two taps for 'passage clear', the old road signal."
   };
   static const char *says[] = {
      "Road's open. Commission checked it this morning. Travel north with your documents visible.",
      "Split-pine badge means joint authority. Midgaard law south, Kowloon compact north, forest charter in between.",
      "If you see ash-painted faces off the road, don't engage. Signal any warden and move clear.",
      "Every warden on this road answers to the joint commission. We report to both cities equally. That's the charter.",
      "Lantern posts need oil at the next bend. If you pass before dark, the oil cache is at the base of the stone."
   };
   /* Great Northern Forest main road from Midgaard (south) to Kowloon (north) */
   static const int route[] = {
      15512, 15511, 15510, 15530, 15550, 15570, 15590, 15610, 15630, 15650,
      15670, 15690, 15710, 15730, 15750, 15770, 15771, 15772, 15792, 15793,
      15794, 15814, 15813, 15833, 15853, 15873, 15893, 15913, 15912, 15932,
      15952, 15972, 15992, 15993, 15994
   };
   static const int route_len = sizeof(route) / sizeof(route[0]);
   static const int endpoint_a = 15512; /* Midgaard end */
   static const int endpoint_b = 15994; /* Kowloon end */

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
