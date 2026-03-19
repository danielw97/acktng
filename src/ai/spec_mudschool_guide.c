#include "globals.h"
#include "special.h"

/*
 * spec_mudschool_guide — SPEC_FUN for the Academy Guide (mob vnum 4910).
 *
 * Called directly from the new-character login path (login.c) when a brand-new
 * player enters the mudschool for the first time.  Delivers a one-time static
 * greeting and clears is_new_player so subsequent tick-based calls are no-ops.
 */
bool spec_mudschool_guide(CHAR_DATA *ch)
{
   CHAR_DATA *victim;

   for (victim = ch->in_room->first_person; victim != NULL; victim = victim->next_in_room)
   {
      if (!IS_NPC(victim) && victim->pcdata->is_new_player)
      {
         victim->pcdata->is_new_player = FALSE;
         act("$n says 'Welcome, $N!  I am the Academy Guide.  I am an AI-enabled NPC"
             " — feel free to ask me any questions you have about the game.'",
             ch, NULL, victim, TO_ROOM);
         act("$n says 'Welcome, $N!  I am the Academy Guide.  I am an AI-enabled NPC"
             " — feel free to ask me any questions you have about the game.'",
             ch, NULL, victim, TO_VICT);
         return TRUE;
      }
   }

   return FALSE;
}
