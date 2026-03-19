#include "globals.h"
#include "special.h"
#include "npc_dialogue.h"

/*
 * spec_mudschool_guide — SPEC_FUN for the Academy Guide (mob vnum 4910).
 *
 * Called each game tick while the mob is alive.  When a player is present
 * in the room and no dialogue request is already pending, the guide greets
 * the first player it finds.
 */
bool spec_mudschool_guide(CHAR_DATA *ch)
{
   CHAR_DATA *victim;

   if (ch->dlg_pending)
      return FALSE;

   for (victim = ch->in_room->first_person; victim != NULL; victim = victim->next_in_room)
   {
      if (!IS_NPC(victim))
      {
         npc_dialogue_dispatch(ch, victim, "[GREET] A new adventurer has arrived.");
         return TRUE;
      }
   }

   return FALSE;
}
