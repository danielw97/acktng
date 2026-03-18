#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_cast_adept(CHAR_DATA *ch)
{
   CHAR_DATA *victim;
   int cl; /* cast level */

   if (!IS_AWAKE(ch))
      return FALSE;

   if (is_fighting(ch))
      return FALSE;

   if (ch->in_room != NULL)
   {
      if (!IS_SET(ch->in_room->affected_by, ROOM_BV_HEAL_REGEN))
      {
         spell_healing_light(skill_lookup("healing light"), 79, ch, NULL, NULL);
         return TRUE;
      }
      if (!IS_SET(ch->in_room->affected_by, ROOM_BV_MANA_REGEN))
      {
         spell_mana_flare(skill_lookup("mana flare"), 79, ch, NULL, NULL);
         return TRUE;
      }
   }

   if (number_bits(4) != 0)
      return FALSE;

   for (victim = ch->in_room->first_person; victim != NULL; victim = victim->next_in_room)
      if (victim != ch && can_see(ch, victim) && number_bits(1) == 0)
         break;

   if (victim == NULL)
      return FALSE;

   cl = number_range(1, ch->level);

   switch (number_bits(3))
   {
   case 0:
      act("$n utters the word 'tehctah'.", ch, NULL, NULL, TO_ROOM);
      spell_armor(skill_lookup("armor"), cl, ch, victim, NULL);
      return TRUE;

   case 1:
      act("$n utters the word 'nhak'.", ch, NULL, NULL, TO_ROOM);
      spell_bless(skill_lookup("bless"), cl, ch, victim, NULL);
      return TRUE;

   case 2:
      act("$n utters the word 'yeruf'.", ch, NULL, NULL, TO_ROOM);
      spell_cure_blindness(skill_lookup("cure blindness"), cl, ch, victim, NULL);
      return TRUE;

   case 3:
      act("$n utters the word 'garf'.", ch, NULL, NULL, TO_ROOM);
      spell_cure_light(skill_lookup("cure light"), cl, ch, victim, NULL);
      return TRUE;

   case 4:
      act("$n utters the words 'rozar'.", ch, NULL, NULL, TO_ROOM);
      spell_cure_poison(skill_lookup("cure poison"), cl, ch, victim, NULL);
      return TRUE;

   case 5:
   {
      char buffer[MSL];
      double seconds;
      int days;
      time_t currentDate;
      struct tm *xmas, today;

      time(&currentDate);

      today = *localtime(&currentDate);

      xmas = localtime(&currentDate);
      xmas->tm_mon = 11;
      xmas->tm_mday = 25;
      if (today.tm_mday > 25 && today.tm_mon == 11)
         xmas->tm_year = today.tm_year + 1;

      seconds = difftime(mktime(xmas), currentDate);
      days = seconds / 86400;

      sprintf(buffer, "$n utters the words '%d days to Christmas!'.", days);

      act(buffer, ch, NULL, NULL, TO_ROOM);
      spell_refresh(skill_lookup("refresh"), ch->level, ch, victim, NULL);
      return TRUE;
   }
   }

   return FALSE;
}
