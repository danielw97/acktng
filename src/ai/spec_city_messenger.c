#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"
#include "quest.h"

/* Quest ID for "Catacomb Entry Clearance" offered by this mob */
#define CITY_MESSENGER_QUEST_ID 72

bool spec_city_messenger(CHAR_DATA *ch)
{
   static const char *acts[] = {
       "$n shifts $s leather dispatch satchel to the other shoulder, checking the clasp.",
       "$n pulls a folded manifest from $s satchel, reads it quickly, and stuffs it back.",
       "$n glances at the posted route board on the corner wall, tracing a chalk line with one "
       "finger.",
       "$n counts a stack of sealed envelopes under $s arm, lips moving silently.",
       "$n pauses to tie a loose knot on $s satchel strap before moving on."};
   static const char *says[] = {
       "Three circuits already today and I still haven't cleared the eastern manifests.",
       "Dispatch backlog at the gate lodge is three days deep. Nobody wants that route since the "
       "catacomb trouble.",
       "Postmaster says anyone who wants route clearance work should ask at the sorting counter.",
       "Running dispatches under the registry clock means no slack time. Every bell counts.",
       "The Ledgerhouse wants gate confirmations by Ash Watch. I'm already behind."};
   static const char *quest_hints[] = {
       "You look like someone who handles themselves in a fight. The catacomb approaches need "
       "clearing before my dispatches can move safely. Ask the Postmaster.",
       "Gate dispatches are backing up because the approaches below are crawling with vermin. If "
       "you're looking for work, the Postmaster handles clearance contracts.",
       "I'd pay my own copper if I could — the dispatch queue to the east gate is a mess until "
       "someone clears the catacombs. Postmaster has the paperwork."};
   static const char *quest_active[] = {
       "Still at it? The sooner those approaches are clear, the sooner my dispatches move.",
       "Good hunting down there. Every report I hear brings the queue one step closer to normal.",
       "The gate lodges are counting on you. I've got parcels sitting here three days already."};

   CHAR_DATA *vch;
   bool hint_given = FALSE;

   if (!IS_AWAKE(ch) || is_fighting(ch))
      return FALSE;

   if (number_bits(3) != 0)
      return FALSE;

   /* Check for players in room who haven't completed or started quest */
   for (vch = ch->in_room->first_person; vch != NULL; vch = vch->next_in_room)
   {
      int slot;
      bool is_quest_active = FALSE;
      bool quest_done = FALSE;

      if (IS_NPC(vch) || vch->pcdata == NULL)
         continue;

      quest_done = vch->pcdata->completed_static_quests[CITY_MESSENGER_QUEST_ID];

      for (slot = 0; slot < QUEST_MAX_QUESTS; slot++)
      {
         if (vch->pcdata->quests[slot].quest_static_id == CITY_MESSENGER_QUEST_ID &&
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
