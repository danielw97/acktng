#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_kowloon_gate_captain(CHAR_DATA *ch)
{
   static const char *acts[] = {
      "$n reviews a manifest against the Syndic's inspection ledger, marking entries with a precise notation.",
      "$n steps to the threshold and scans the approaches beyond the gate with hard, practiced eyes.",
      "$n signals a subordinate guard with a brief hand gesture, directing attention to a passing figure.",
      "$n checks a courier's travel permit against the day's registered messengers, then waves them through.",
      "$n consults the rotation schedule posted on the gate wall, confirming the incoming shift timing.",
      "$n updates the threat-assessment board with a new notation from the last patrol report.",
      "$n reviews a list of cleared travelers against the registry, initialing each entry with practiced speed."
   };
   static char *says[] = {
      "Seasonal rotation isn't tradition — it's policy. No one builds loyalty at a chokepoint. The Covenant was specific.",
      "Every departure logged, every arrival registered. If you're leaving through this gate, your name goes in the ledger.",
      "Corsair probes have been testing the eastern approaches. Tide Gate is watching for pattern timing. We share intelligence between posts.",
      "The gate's character changes with who's posting, but the protocol doesn't. That's the point of a protocol.",
      "Carry your documents visible from the checkpoint. I don't have time to search your pack, and neither do you.",
      "Mosswater route is showing increased traffic since the contraband disruption. The pattern shift is something we're tracking at all three gates.",
      "New to the region? The Gloamvault runs northwest off the Kiess road — registered travelers who've been through it tend to list it as their first field posting. Worth noting."
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

      /* Quest 17 (id 16): Mosswater smuggler interdiction */
      if (plr->pcdata->completed_static_quests[16])
      {
         act("$n marks $N's ledger entry with a second notation — the gate captain's acknowledgment mark.", ch, NULL, plr, TO_NOTVICT);
         act("$n marks your ledger entry with a second notation.", ch, NULL, plr, TO_VICT);
         do_say(ch, "Mosswater route has been showing cleaner traffic since your work there. We've updated the gate watch pattern accordingly. The Syndics noticed the cargo manifest discrepancy rate dropped.");
         return FALSE;
      }

      /* Quest 18 (id 17): Northern Crown predator survey */
      if (plr->pcdata->completed_static_quests[17])
      {
         act("$n glances at the registry and then at $N with the attention of someone cross-referencing a report.", ch, NULL, plr, TO_NOTVICT);
         act("$n glances at the registry and then at you with the attention of someone cross-referencing a report.", ch, NULL, plr, TO_VICT);
         do_say(ch, "Northern Crown survey is on file with the Gate Registry. We've marked the confirmed predator corridors in the outbound travel advisories. Your data reduced the guesswork on that approach considerably.");
         return FALSE;
      }
   }

   /* Level-based area hints: random pick from eligible zones */
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
         if (lv >= 5  && lv <= 20) hints[hint_count++] = "First posting in the region? The Gloamvault northwest of Kiess is where most field-capable travelers test themselves before pushing into the northern forest. Gate registry shows a lot of departures in that direction at your range.";
         if (lv >= 10 && lv <= 25) hints[hint_count++] = "The Nightfall Catacombs come up in outbound travel declarations — undead vault complex, multi-level. Gate registry logs departures in that direction for operatives past their initial field range.";
         if (lv >= 15 && lv <= 30) hints[hint_count++] = "Heading east? Sepulcher Pasture is past the crossroads. We log departures in that direction — it's active territory, burial cult remnants, bone-work. The registry shows returns from travelers in your range. Proceed with documentation current.";
         if (lv >= 20 && lv <= 35) hints[hint_count++] = "The Cathedral of the Violet Eclipse is on our extended threat registration list. Eclipse cult site — operatives clearing it tend to log significant engagement before departure. The registry shows returns from your level tier.";
         if (lv >= 25 && lv <= 40) hints[hint_count++] = "Umbra Heartspire departures require an extended absence declaration at the registry — shadow construct territory, deep engagement expected. Gate records show it draws experienced operatives. Log your departure if you're heading there.";
         if (hint_count > 0)
         {
            do_say(ch, hints[number_range(0, hint_count - 1)]);
            return FALSE;
         }
      }
   }

   /* Normal random dialogue */
   if (number_bits(1) == 0)
      act(acts[number_range(0, 6)], ch, NULL, NULL, TO_ROOM);
   else
      do_say(ch, says[number_range(0, 6)]);

   return FALSE;
}
