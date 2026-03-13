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

   switch (number_range(0, 2))
   {
      case 0:  /* Quest completion recognition */
      {
         int qid[4];
         int nq = 0;

         for (plr = ch->in_room->first_person; plr != NULL; plr = plr->next_in_room)
         {
            if (IS_NPC(plr) || plr->pcdata == NULL)
               continue;
            if (plr->pcdata->completed_static_quests[0]) qid[nq++] = 0;
            if (plr->pcdata->completed_static_quests[9]) qid[nq++] = 9;
         }

         if (nq > 0)
         {
            switch (qid[number_range(0, nq - 1)])
            {
               case 0:
                  act("$n raises $s voice to address the assembled crowd with particular emphasis.", ch, NULL, NULL, TO_ROOM);
                  do_say(ch, quest_complete_speeches[0]);
                  return FALSE;
               case 9:
                  act("$n raises $s voice to address the assembled crowd with particular emphasis.", ch, NULL, NULL, TO_ROOM);
                  do_say(ch, quest_complete_speeches[1]);
                  return FALSE;
            }
         }
         break;
      }
      case 1:  /* Area hint */
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
            const char *hints[5];
            int hint_count = 0;
            int lv = hint_plr->level;
            if (lv >= 5  && lv <= 20) hints[hint_count++] = "The Gloamvault to the northwest is not a ruin for the curious — it is a proving ground. Kel'Shadra's cult did not fall easily. Those who have cleared its threshold understand what Kiess's early compact was protecting against.";
            if (lv >= 10 && lv <= 25) hints[hint_count++] = "The Nightfall Catacombs are not simply an undead infestation — they are a record of what happens when burial doctrine becomes autonomous. The catacombs remember. Field experience there is an education in consequences.";
            if (lv >= 15 && lv <= 30) hints[hint_count++] = "The Sepulcher Pasture to the east is contested ground. Burial rites performed under competing doctrines, none of them resolved. What you see there is a jurisdiction dispute that became a haunting.";
            if (lv >= 20 && lv <= 35) hints[hint_count++] = "The Cathedral of the Violet Eclipse stands as a record of what devotion to astronomical doctrine produces when left unchecked. Eclipse cult, relic guardians, architecture built to outlast its architects. Worth witnessing if you are capable of surviving the visit.";
            if (lv >= 25 && lv <= 40) hints[hint_count++] = "The Umbra Heartspire is not a dungeon in the conventional sense — it is a void anchor, shadow-aligned, constructed to persist. Field operatives who have cleared it describe a structure that resists documentation. That resistance is itself significant.";
            if (hint_count > 0)
            {
               act("$n addresses the assembled listeners with the measured cadence of a prepared civic speaker.", ch, NULL, NULL, TO_ROOM);
               do_say(ch, (char *)hints[number_range(0, hint_count - 1)]);
               return FALSE;
            }
         }
         break;
      }
   }

   act("$n addresses the assembled listeners with the measured cadence of a prepared civic speaker.", ch, NULL, NULL, TO_ROOM);
   do_say(ch, speeches[number_range(0, 8)]);

   return FALSE;
}
