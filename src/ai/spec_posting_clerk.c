#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"
#include "quest.h"

/* Quest ID for "Chapel Approach Clearance" offered by this mob */
#define POSTING_CLERK_QUEST_ID 75

bool spec_posting_clerk(CHAR_DATA *ch)
{
   static const char *acts[] = {
       "$n tacks a fresh civic notice to the posting board with crisp, practiced strikes.",
       "$n removes an expired notice, folds it neatly, and slides it into a canvas document "
       "sleeve.",
       "$n straightens a row of notices on the board, checking dates and order.",
       "$n holds a notice up to the light, checking the registry seal before posting it.",
       "$n reads aloud from a new dispatch in a clear carrying voice, then pins it to the board."};
   static const char *says[] = {
       "Every notice on this board is a registered civic communication. Tearing one down is a "
       "Compact violation.",
       "The posting circuit covers all primary intersections. If you don't see a notice here, "
       "check the Temple forecourt.",
       "Civic notices expire by bell-date. I pull and archive everything past its stamp.",
       "The Registry requires five days of public posting before an ordinance takes legal effect. "
       "Count the dates yourself if you doubt it.",
       "Three posting boards were damaged last week in the chapel district. I've filed the "
       "incident report. Now I'm waiting for someone to do something about it."};
   static const char *quest_hints[] = {
       "The chapel district posting boards keep getting damaged by the undead things that have "
       "come up from the catacombs. The processional ghouls and acolyte remnants are pushing "
       "toward the surface approaches. If you're capable in a fight, the Postmaster can write you "
       "a clearance contract.",
       "I've replaced those chapel-district boards twice this month. Every time I fix them, the "
       "vermin from the old catacomb approaches wreck them again. Speak to the Postmaster if you "
       "want to take the clearance work officially.",
       "My job gets a lot easier when the chapel approaches are clear. Right now they're overrun "
       "with catacomb remnants. The Postmaster has a contract available if you want it."};
   static const char *quest_active[] = {
       "You're working the chapel clearance? Good. The boards stay intact a lot longer when the "
       "approaches are clear.",
       "How far have you pushed the catacomb remnants back? Every yard of clear corridor is a yard "
       "closer to getting those boards permanent.",
       "The chapel district citizens have noticed a difference already. Keep going."};

   CHAR_DATA *vch;
   bool hint_given = FALSE;

   if (!IS_AWAKE(ch) || is_fighting(ch))
      return FALSE;

   if (number_bits(3) != 0)
      return FALSE;

   for (vch = ch->in_room->first_person; vch != NULL; vch = vch->next_in_room)
   {
      int slot;
      bool is_quest_active = FALSE;
      bool quest_done = FALSE;

      if (IS_NPC(vch) || vch->pcdata == NULL)
         continue;

      quest_done = vch->pcdata->completed_quests[POSTING_CLERK_QUEST_ID];

      for (slot = 0; slot < QUEST_MAX_QUESTS; slot++)
      {
         if (vch->pcdata->quests[slot].quest_template_id == POSTING_CLERK_QUEST_ID &&
             vch->pcdata->quests[slot].quest_type != QUEST_TYPE_NONE)
         {
            is_quest_active = TRUE;
            break;
         }
      }

      if (!quest_done && !is_quest_active && number_bits(1) == 0)
      {
         do_say(ch, (char *)quest_hints[number_range(0, 2)]);
         hint_given = TRUE;
         break;
      }
      else if (is_quest_active && !quest_done && number_range(0, 3) == 0)
      {
         do_say(ch, (char *)quest_active[number_range(0, 2)]);
         hint_given = TRUE;
         break;
      }
   }

   if (!hint_given)
   {
      if (number_bits(1) == 0)
         act(acts[number_range(0, 4)], ch, NULL, NULL, TO_ROOM);
      else
         do_say(ch, (char *)says[number_range(0, 4)]);
   }

   return FALSE;
}
