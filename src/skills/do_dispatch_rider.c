#include "globals.h"
#include "magic.h"
#include "skills.h"

void do_dispatch_rider(CHAR_DATA *ch, char *argument)
{
   char arg[MAX_INPUT_LENGTH];
   char buf[MAX_STRING_LENGTH];
   DESCRIPTOR_DATA *d;
   CHAR_DATA *victim = NULL;
   char *message;

   if (!can_use_pub_society_skill(ch, gsn_dispatch_rider))
   {
      send_to_char("You don't know how to use this skill!\n\r", ch);
      return;
   }

   if (!can_use_skill_message(ch, gsn_dispatch_rider))
      return;

   argument = one_argument(argument, arg);
   message = argument;

   if (arg[0] == '\0' || message[0] == '\0')
   {
      send_to_char("Syntax: dispatch <road warden name> <message>\n\r", ch);
      return;
   }

   /* Find the target Road Warden */
   for (d = first_desc; d != NULL; d = d->next)
   {
      if (d->connected != CON_PLAYING)
         continue;
      if (d->character == NULL || IS_NPC(d->character))
         continue;
      if (d->character->pcdata->pub_society != PUB_SOCIETY_ROAD_WARDENS)
         continue;
      if (!is_name(arg, d->character->name))
         continue;
      victim = d->character;
      break;
   }

   if (victim == NULL)
   {
      send_to_char("No Road Warden by that name is online.\n\r", ch);
      return;
   }

   if (victim == ch)
   {
      send_to_char("You can't dispatch a message to yourself.\n\r", ch);
      return;
   }

   WAIT_STATE(ch, skill_table[gsn_dispatch_rider].beats);
   ch->cooldown[gsn_dispatch_rider] = 150; /* ~10 minutes */
   raise_skill(ch, gsn_dispatch_rider);

   sprintf(buf, "@@y[DISPATCH from %s]: %s@@N\n\r", ch->name, message);
   send_to_char(buf, victim);

   sprintf(buf, "You dispatch a message to %s.\n\r", victim->name);
   send_to_char(buf, ch);
}
