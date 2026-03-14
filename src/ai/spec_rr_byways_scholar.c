#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_rr_byways_scholar(CHAR_DATA *ch)
{
   static const char *acts[] = {
      "$n traces a finger along the wall inscription, moving $s lips in a silent attempt at the archaic script.",
      "$n takes a rubbing from the old stonework using charcoal and cloth, working in slow careful strokes.",
      "$n adjusts the portable lantern to illuminate a section of the passage wall from a different angle.",
      "$n makes an entry in $s field notebook, pausing twice to reconsider a word before committing it.",
      "$n peers closely at a carved detail in the stone, pulling out a small lens to examine the tool marks."
   };
   static char *says[] = {
      "The construction technique here predates Kiess by at least three centuries. The jointing method is Evermeet — no surviving workshop still uses it.",
      "The inscription hall is the most significant find in the underfoundation. The Society has three copies of the rubbing, none of them fully translated.",
      "Kiess was built above Evermeet because the Evermeet foundation was too valuable to demolish. They built over it. That was a deliberate decision.",
      "The remnant presences here are non-hostile on first approach. Stay calm, don't disturb the structural features, and they typically ignore you.",
      "The scholars' cache in the east vault is our people's work — two hundred years ago, another Antiquarian Society team. We're continuing their record.",
      "I'm trying to determine if the drainage systems are still functional. Some of them appear to be. That would be remarkable for construction of this age."
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
         for (plr = ch->in_room->first_person; plr != NULL; plr = plr->next_in_room)
         {
            if (IS_NPC(plr) || plr->pcdata == NULL)
               continue;
            if (plr->pcdata->completed_static_quests[77])
            {
               act("$n looks up from $s notebook with a sharper attention than usual.", ch, NULL, plr, TO_NOTVICT);
               act("$n looks up from $s notebook with a sharper attention than usual.", ch, NULL, plr, TO_VICT);
               do_say(ch, "The remnant activity in the lower passages has subsided. I noticed the change in ambient conditions — the deep chambers feel different. Whatever you disrupted, the site is considerably more accessible for it. The Society will note your contribution in the field record.");
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
            const char *hints[4];
            int hint_count = 0;
            int lv = hint_plr->level;
            if (lv >= 40 && lv <= 80) hints[hint_count++] = "The underfoundation has remnant presences — Evermeet shades and something more cohesive in the east vault. The Kiess guild registrar has noted an interest in having the site kept clear for research purposes.";
            if (lv >= 50 && lv <= 100) hints[hint_count++] = "The remnant guardian in the east vault is the most stable presence down here. It's associated with the alcove — protective behavior, almost certainly linked to whatever was stored there. Resolving that would be a meaningful contribution to the site.";
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
