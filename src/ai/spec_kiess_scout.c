#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_kiess_scout(CHAR_DATA *ch)
{
   static const char *acts[] = {
      "$n marks notations on a field map with practiced efficiency, cross-referencing route cards.",
      "$n checks the rope markers on $s belt, counting the knots against $s field notes.",
      "$n scans the room with the alert wariness of someone who hasn't fully decompressed from the forest.",
      "$n consults a compass bearing against $s posted route notes, making a small correction.",
      "$n oils the edge of $s blade with a cloth, eyes distant, working from ingrained habit.",
      "$n pins a fresh waypoint sketch to the duty board, replacing the older one with a curt nod.",
      "$n traces the mist-line boundary on $s field map and circles a new notation in charcoal."
   };
   static char *says[] = {
      "The mist line's moved another three hundred paces north since last week. That's fast for this season.",
      "Bell-post seven is down. Wall Command's rerouting the southern approach until it's back up.",
      "Saw centaur sign at the forest edge. Prints were fresh — probably overnight. Command's been told.",
      "The confusion doesn't hit immediately. You get maybe two hundred paces before the mist starts working. Stay on the rope lines.",
      "Bring samples from the mist-boundary if you go in. The healers want the spore data. Sealed container, waxed shut.",
      "Wall Command needs threat marks from the Forest of Confusion approaches. Catrat packs have been disrupting waypoint runners — someone needs to verify the trail status.",
      "If you're just finding your footing out here, the Gloamvault northwest of the city is a reasonable first exercise. Old cult ruins — dangerous enough to matter, compact enough to navigate."
   };

   CHAR_DATA *plr;

   if (!IS_AWAKE(ch) || is_fighting(ch))
      return FALSE;

   if (number_bits(3) != 0)
      return FALSE;

   /* Quest-completion reactions: personalized acknowledgment when relevant quests are done */
   for (plr = ch->in_room->first_person; plr != NULL; plr = plr->next_in_room)
   {
      if (IS_NPC(plr) || plr->pcdata == NULL)
         continue;

      /* Quest 1 (id 0): Route reconnaissance: Forest of Confusion — catrat clearance */
      if (plr->pcdata->completed_static_quests[0])
      {
         act("$n catches $N's eye and gives a brief nod, the kind shared between people who've run the same ground.", ch, NULL, plr, TO_NOTVICT);
         act("$n catches your eye and gives a brief nod.", ch, NULL, plr, TO_VICT);
         do_say(ch, "Your catrat clearance gave dispatch real numbers to work from. Waypoint runners have been making better time on the northern approach since your report came in.");
         return FALSE;
      }

      /* Quest 10 (id 9): Greenveil Spur reconnaissance: spirit clearance */
      if (plr->pcdata->completed_static_quests[9])
      {
         act("$n glances at $N with the measured recognition of someone who has read the after-action report.", ch, NULL, plr, TO_NOTVICT);
         act("$n glances at you with the measured recognition of someone who has read the after-action report.", ch, NULL, plr, TO_VICT);
         do_say(ch, "Greenveil Spur's back on the routing charts. Convoy marshals recertified it as an emergency bypass. I've already updated my field map — good work clearing it.");
         return FALSE;
      }

      /* Quest 26 (id 25): Bell-Post Line Reopening */
      if (plr->pcdata->completed_static_quests[25])
      {
         act("$n pauses $s notation work and looks toward $N with quiet recognition.", ch, NULL, plr, TO_NOTVICT);
         act("$n pauses $s notation work and looks toward you with quiet recognition.", ch, NULL, plr, TO_VICT);
         do_say(ch, "Bell-post line is back in rotation. Relay timing came down by two bells on the northern circuit. I already filed the updated route card. Your work shows in the numbers.");
         return FALSE;
      }

      /* Quest 27 (id 26): Mirrorbark Predator Census */
      if (plr->pcdata->completed_static_quests[26])
      {
         act("$n sets down $s field notes and acknowledges $N with a look of professional respect.", ch, NULL, plr, TO_NOTVICT);
         act("$n sets down $s field notes and acknowledges you with a look of professional respect.", ch, NULL, plr, TO_VICT);
         do_say(ch, "The Mirrorbark census data came through. Three mob types confirmed, pattern consistent. Wall Command adjusted the northern patrol rotation based on your count. That's exactly what the post needed.");
         return FALSE;
      }
   }

   /* Level-based area hints for players who haven't explored nearby zones yet */
   for (plr = ch->in_room->first_person; plr != NULL; plr = plr->next_in_room)
   {
      if (IS_NPC(plr) || plr->pcdata == NULL)
         continue;

      /* Gloamvault hint for low-level players who haven't done Gloamvault quests */
      if (plr->level >= 5 && plr->level <= 20
         && !plr->pcdata->completed_static_quests[54]  /* Quest 55: Gloamvault threshold audit */
         && !plr->pcdata->completed_static_quests[66]) /* Quest 67: Gloamvault cartography */
      {
         do_say(ch, "You look like you're still finding your range. The Gloamvault northwest of the city is worth a run — old cult structure, cult remnants still active. Good first dungeon if you're at that level of capability.");
         return FALSE;
      }

      /* Sepulcher Pasture hint for mid-level players */
      if (plr->level >= 15 && plr->level <= 30
         && !plr->pcdata->completed_static_quests[68]) /* Quest 69: Sepulcher Pasture cartography */
      {
         do_say(ch, "Sepulcher Pasture is east of the crossroads — burial cult territory, bone-remnant activity. If you're operating in that level range, it's solid field work. Not my jurisdiction, but worth noting on your map.");
         return FALSE;
      }
   }

   /* Normal random dialogue */
   if (number_bits(1) == 0)
      act(acts[number_range(0, 6)], ch, NULL, NULL, TO_ROOM);
   else
      do_say(ch, says[number_range(0, 6)]);

   return FALSE;
}
