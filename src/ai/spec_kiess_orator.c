#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_kiess_orator(CHAR_DATA *ch)
{
   static char *speeches[] = {
      "Evermeet was not destroyed in a single night. It was abandoned in a thousand small decisions. Kiess was built so those decisions cannot be made quietly.",
      "The Compact Flame has burned without interruption since the first foundation stone was laid. That continuity is deliberate. Watch it — it is doing something.",
      "Three factions, one peace code. Not because they agree — they rarely agree — but because the alternative is Evermeet. We have seen the alternative.",
      "The Pillar of Returning Caravans adds a mark for every safe return from the frontier. Count them sometime. That count is what this city is actually defending.",
      "Roc Road connects us east. The Forest of Confusion lies north. Wall Command holds the perimeter. The Compact holds the center. This is not accidental — this is the design.",
      "Every waystone on these avenues marks a district that once existed in Evermeet. Every one of those districts fell silent. We read those names so we know what we are protecting against.",
      "The Gloamvault to the northwest is not a ruin. It is a reminder. Kel'Shadra's covenant failed from within — the cult didn't fall to an army, it collapsed under its own doctrine. Study that.",
      "Field work is not separate from civic security. Every cleared patrol route, every confirmed threat mark, every courier who returns — that is what the Pillar of Returning Caravans is counting. Do not underestimate what you contribute.",
      "The Sepulcher Pasture to the east is contested ground. Burial rites performed under competing doctrines, none of them resolved. What you see there is a jurisdiction dispute that became a haunting."
   };

   /* Quest-completion reactions: orator acknowledges returned field operatives with civic weight */
   static char *quest_complete_speeches[] = {
      "Field reports from the Forest of Confusion approaches are already reshaping Wall Command's routing doctrine. When independent agents provide accurate threat data, the whole city moves better. That is the compact in action.",
      "The Greenveil Spur has been recertified as an emergency bypass. I want those present to understand what that means: convoy marshals now have a contingency that did not exist last week. That is the product of individual effort becoming collective security."
   };

   CHAR_DATA *plr;

   if (!IS_AWAKE(ch) || is_fighting(ch))
      return FALSE;

   if (number_bits(3) != 0)
      return FALSE;

   /* Quest-completion reactions */
   for (plr = ch->in_room->first_person; plr != NULL; plr = plr->next_in_room)
   {
      if (IS_NPC(plr) || plr->pcdata == NULL)
         continue;

      /* Quest 1 (id 0): Route reconnaissance: Forest of Confusion */
      if (plr->pcdata->completed_static_quests[0])
      {
         act("$n raises $s voice to address the assembled crowd with particular emphasis.", ch, NULL, NULL, TO_ROOM);
         do_say(ch, quest_complete_speeches[0]);
         return FALSE;
      }

      /* Quest 10 (id 9): Greenveil Spur clearance */
      if (plr->pcdata->completed_static_quests[9])
      {
         act("$n raises $s voice to address the assembled crowd with particular emphasis.", ch, NULL, NULL, TO_ROOM);
         do_say(ch, quest_complete_speeches[1]);
         return FALSE;
      }
   }

   act("$n addresses the assembled listeners with the measured cadence of a prepared civic speaker.", ch, NULL, NULL, TO_ROOM);
   do_say(ch, speeches[number_range(0, 8)]);

   return FALSE;
}
