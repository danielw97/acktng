#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"
#include "quest.h"

/* Quest ID for "Midgaard District Survey" offered by this mob */
#define RECORDS_KEEPER_QUEST_ID 74

bool spec_records_keeper(CHAR_DATA *ch)
{
   static const char *acts[] = {
       "$n draws a careful line under a column of figures and checks the sum twice.",
       "$n slides a document into an alphabetical pigeonhole, pausing to read the label aloud.",
       "$n taps a ledger page with the blunt end of $s quill, thinking.",
       "$n blows on fresh ink, then fans the page with a practiced wrist motion.",
       "$n opens a new folio, creases the spine, and writes the date at the top in formal script."};
   static const char *says[] = {
       "The district rolls are only as accurate as the last survey. This one is years overdue.",
       "A ledger that reflects old reality is worse than no ledger — it sends people down wrong "
       "roads.",
       "The Registry requires a full room count, not just the named locations. All corridors, all "
       "courts, all passages.",
       "Cartography is civic service. Every door you open and log is a door some future traveler "
       "doesn't walk past blind.",
       "The Ledgerhouse wants floor plans, not approximations. That requires someone willing to "
       "walk every room."};
   static const char *quest_hints[] = {
       "The Registry hasn't had a complete survey of the city districts since before the Lantern "
       "Reforms. If you're the exploring sort, I can issue you a commission through the Postmaster "
       "— walk every room you can find and report back.",
       "I've been trying to update the district rolls for two years. What I need is someone who "
       "will actually walk the entire city and log what they find. The Postmaster can formalize it "
       "as a survey commission if you're interested.",
       "Every ward, every alley, every civic passage — the Registry needs it all accounted for. If "
       "you have the patience to walk this city thoroughly, speak to the Postmaster about a "
       "cartography commission."};
   static const char *quest_active[] = {"Good. Mark everything — named rooms and unnamed passages "
                                        "both. The Registry counts all of them.",
                                        "The survey is progressing? Keep moving. The more complete "
                                        "the coverage, the better the commission pays.",
                                        "Every room you enter and log is data the Registry hasn't "
                                        "had in years. Take your time and be thorough."};

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

      quest_done = vch->pcdata->completed_quests[RECORDS_KEEPER_QUEST_ID];

      for (slot = 0; slot < QUEST_MAX_QUESTS; slot++)
      {
         if (vch->pcdata->quests[slot].quest_template_id == RECORDS_KEEPER_QUEST_ID &&
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
