#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_rr_byways_hermit(CHAR_DATA *ch)
{
   static const char *acts[] = {
       "$n crouches at the hollow entrance, watching the trail below with calm, unblinking "
       "attention.",
       "$n turns a smooth ridge-stone over in $s hands, reading its grain before setting it aside.",
       "$n leans back against the hollow wall and closes $s eyes — resting, not sleeping.",
       "$n traces a weather-pattern in the soil with one finger, muttering something too quiet to "
       "hear.",
       "$n lifts $s head sharply at a sound from the tree line, waits, then relaxes again."};
   static char *says[] = {"The pack territory extends from the wolf den down through the western "
                          "cave approach. They don't leave it without a reason.",
                          "I've watched this ridge for years. The wolves here are older than most "
                          "people bother to track. The pack leader is exceptional.",
                          "Rain comes from the north-west when the summit mist drops before noon. "
                          "Give it three hours after that.",
                          "The cave system below the road is old. Older than the road. The water "
                          "sounds change before weather does — I use it as a barometer.",
                          "If you're heading to the summit, watch the east ledge. The drop on the "
                          "far side is deceptive — looks shorter than it is.",
                          "I've seen the glade below fill with a particular light on certain "
                          "mornings. Something lives there. Not hostile, but particular."};

   CHAR_DATA *plr;

   if (!IS_AWAKE(ch) || is_fighting(ch))
      return FALSE;

   if (number_bits(3) != 0)
      return FALSE;

   switch (number_range(0, 2))
   {
   case 0: /* Quest completion recognition */
   {
      for (plr = ch->in_room->first_person; plr != NULL; plr = plr->next_in_room)
      {
         if (IS_NPC(plr) || plr->pcdata == NULL)
            continue;
         if (plr->pcdata->completed_static_quests[76])
         {
            act("$n regards $N with the quiet attention of someone revising an assessment.", ch,
                NULL, plr, TO_NOTVICT);
            act("$n regards you with the quiet attention of someone revising an assessment.", ch,
                NULL, plr, TO_VICT);
            do_say(ch,
                   "Pack is thinner on the ridge now. I noticed it before anyone reported it. The "
                   "territory markers have shifted — the survivors are reorganizing further north. "
                   "You changed the balance. It'll hold for a season, maybe two.");
            return FALSE;
         }
      }
      break;
   }
   case 1: /* Area hint */
   {
      CHAR_DATA *hint_plr = NULL;
      for (plr = ch->in_room->first_person; plr != NULL; plr = plr->next_in_room)
      {
         if (!IS_NPC(plr) && plr->pcdata != NULL)
         {
            hint_plr = plr;
            break;
         }
      }
      if (hint_plr != NULL)
      {
         const char *hints[4];
         int hint_count = 0;
         int lv = hint_plr->level;
         if (lv >= 20 && lv <= 55)
            hints[hint_count++] = "The wolf den is at the far end of the western cave passage. The "
                                  "pack leader doesn't come out without the whole pack behind it. "
                                  "You'd want to clear the approach first.";
         if (lv >= 30 && lv <= 70)
            hints[hint_count++] = "There's something in the glade below that doesn't like "
                                  "disruption. It won't bother you if you're quiet about it, but "
                                  "if you make a mess of the sanctum, expect a response.";
         if (hint_count > 0)
         {
            do_say(ch, (char *)hints[number_range(0, hint_count - 1)]);
            return FALSE;
         }
      }
      break;
   }
   }

   if (number_bits(1) == 0)
      act(acts[number_range(0, 4)], ch, NULL, NULL, TO_ROOM);
   else
      do_say(ch, says[number_range(0, 5)]);

   return FALSE;
}
