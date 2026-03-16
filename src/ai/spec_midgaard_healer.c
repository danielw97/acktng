#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_midgaard_healer(CHAR_DATA *ch)
{
   static const char *acts[] = {
      "$n prepares a fresh restorative draught, measuring herbs with experienced hands.",
      "$n closes $s eyes briefly, hands folded at the wrists, channelling a quiet restoration.",
      "$n checks a patient's dressing, rewraps it with practiced efficiency, and moves on.",
      "$n records a treatment note in $s ledger, the handwriting the product of forty years of use.",
      "$n refills the oil in the healing-light lamp, keeping the steady glow that marks the sanctuary."
   };
   static char *says[] = {
      "The temple sanctuary covers injuries sustained outside the ward as readily as those from within it. You need not explain.",
      "Restoration work runs on a sliding scale in this city. Speak to me before you assume you cannot afford it.",
      "I've been treating injuries in this hall since before the Lantern Reforms. I have not yet encountered a wound that surprised me.",
      "The three healing oaths I took include confidentiality. What is said in the sanctuary, stays in the sanctuary.",
      "Restorative channelling requires no payment from those bearing Watch credentials or pilgrimage tokens. The temple covers the cost."
   };

   CHAR_DATA *victim;
   char *spell;
   int sn;

   /* Combat: cast cleric spells in self-defence */
   if (is_fighting(ch))
   {
      for (victim = ch->in_room->first_person; victim != NULL; victim = victim->next_in_room)
         if (victim->fighting == ch && number_bits(2) == 0)
            break;

      if (victim != NULL)
      {
         for (;;)
         {
            int min_level;
            switch (number_bits(4))
            {
            case 0:  min_level = 0;  spell = "blindness";    break;
            case 1:  min_level = 3;  spell = "cause serious"; break;
            case 2:  min_level = 7;  spell = "earthquake";   break;
            case 3:  min_level = 9;  spell = "cause critical"; break;
            case 4:  min_level = 10; spell = "dispel evil";  break;
            case 5:  min_level = 12; spell = "curse";        break;
            case 6:  min_level = 12; spell = "change sex";   break;
            case 7:  min_level = 13; spell = "flamestrike";  break;
            case 8:
            case 9:
            case 10: min_level = 15; spell = "harm";         break;
            default: min_level = 16; spell = "dispel magic"; break;
            }
            if (ch->level >= min_level)
               break;
         }

         if ((sn = skill_lookup(spell)) >= 0)
         {
            act("$n's hands glow with a steady, purposeful light — then the energy strikes!", ch, NULL, NULL, TO_ROOM);
            (*skill_table[sn].spell_fun)(sn, ch->level, ch, victim, NULL);
            return TRUE;
         }
      }
      return FALSE;
   }

   /* Ambient */
   if (!IS_AWAKE(ch))
      return FALSE;

   if (number_bits(3) != 0)
      return FALSE;

   if (number_bits(1) == 0)
      act(acts[number_range(0, 4)], ch, NULL, NULL, TO_ROOM);
   else
      do_say(ch, says[number_range(0, 4)]);

   return FALSE;
}
