#include "globals.h"
#include "magic.h"
#include "skills.h"

void do_ember_signal(CHAR_DATA *ch, char *argument)
{
   char buf[MAX_STRING_LENGTH];
   DESCRIPTOR_DATA *d;

   if (!can_use_pub_society_skill(ch, gsn_ember_signal))
   {
      send_to_char("You don't know how to use this skill!\n\r", ch);
      return;
   }

   if (!can_use_skill_message(ch, gsn_ember_signal))
      return;

   WAIT_STATE(ch, skill_table[gsn_ember_signal].beats);
   raise_skill(ch, gsn_ember_signal);

   send_to_char("@@eYou light an emergency ember signal!@@N\n\r", ch);
   act("@@e$n lights an emergency ember signal!@@N", ch, NULL, NULL, TO_ROOM);

   /* Notify all Ember Wardens in the same area */
   sprintf(buf, "@@e[EMBER SIGNAL] %s has lit a signal at %s!@@N\n\r", ch->name, ch->in_room->name);

   for (d = first_desc; d != NULL; d = d->next)
   {
      CHAR_DATA *wch;
      if (d->connected != CON_PLAYING)
         continue;
      wch = d->character;
      if (wch == NULL || IS_NPC(wch) || wch == ch)
         continue;
      if (wch->pcdata->pub_society != PUB_SOCIETY_EMBER_WARDENS)
         continue;
      if (wch->in_room == NULL || wch->in_room->area != ch->in_room->area)
         continue;
      send_to_char(buf, wch);
   }
}
