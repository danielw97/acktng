#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"
#include "quest.h"

/* Quest ID for "Catacomb Rat Culling" offered by this mob */
#define PARK_KEEPER_QUEST_ID 73

bool spec_park_keeper(CHAR_DATA *ch)
{
   static const char *acts[] = {
       "$n crouches beside a planting bed, pressing loose soil back around the base of a shrub.",
       "$n examines a gnawed flower stalk and frowns, tapping the damage mark with one finger.",
       "$n sweeps fallen leaves from the fountain basin with a short-handled broom.",
       "$n checks the irrigation channel running along the park wall, clearing a small blockage "
       "with a stick.",
       "$n marks something on a clipboard, shaking $s head slowly at what $e sees."};
   static const char *says[] = {
       "The lavender bed is half-dug out from below again. Third time this season.",
       "I've submitted three reports to the Registry about the burrowing problem. They sent me "
       "another clipboard.",
       "Groundskeeping used to mean planting and pruning. Now it's half excavation work.",
       "You want to do something useful in this city? Start in the catacombs beneath us.",
       "The fountain stone is in good shape. It's everything six inches underground that worries "
       "me."};
   static const char *quest_hints[] = {
       "The rat population beneath the park has gotten completely out of hand. They're tunneling "
       "up through the roots. I need someone to go down there and thin them out — speak to the "
       "Postmaster if you want to make it official.",
       "Every time I plant something new, the ash tunnel rats from the catacombs dig it up within "
       "a week. If you're willing to go down there and clear some of them out, the Postmaster can "
       "arrange a proper clearance contract.",
       "My planting schedule is four days behind because I keep having to repair rat damage. The "
       "catacombs beneath this park are the source. If you're a fighter looking for work, the "
       "Postmaster has a culling contract."};
   static const char *quest_active[] = {
       "You're the one working the culling contract? Good. The deeper you push them back, the "
       "longer my beds hold.",
       "I checked the tunnel entry this morning. Fewer tracks than yesterday. Keep at it.",
       "The park groundwork holds for now. I'd like to keep it that way — thank you for what "
       "you're doing down there."};

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

      quest_done = vch->pcdata->completed_static_quests[PARK_KEEPER_QUEST_ID];

      for (slot = 0; slot < QUEST_MAX_QUESTS; slot++)
      {
         if (vch->pcdata->quests[slot].quest_static_id == PARK_KEEPER_QUEST_ID &&
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
