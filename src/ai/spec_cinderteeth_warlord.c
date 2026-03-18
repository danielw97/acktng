#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

/*
 * spec_cinderteeth_warlord — Ashfang Remnant Warlord (level 148, mini-boss).
 *
 * Lore: Bandit chieftain who enforces a blood-oath compact among the Ashfang
 * Remnant.  Fights with tactical aggression: rallying warcries, a point-blank
 * explosive blast reserved for armored targets, a poisoned offhand blade, and
 * a rapid double-strike when cornered.
 *
 * Cycling pattern (spec_behavior % 8):
 *   1 — Blood-oath warcry (flavor + warcry)
 *   3 — Iron toll: PHYSICAL crush + high explosive spell on target
 *   5 — Poisoned blade: extra physical hit + poison application
 *   7 — Break formation: rapid double strike
 *   (other cases: no special action)
 */

bool spec_cinderteeth_warlord(CHAR_DATA *ch)
{
   CHAR_DATA *victim;
   char buf[MSL];
   int sn;

   if (!is_fighting(ch))
   {
      ch->spec_behavior = 0;
      return FALSE;
   }

   if ((victim = ch->fighting) == NULL)
      return FALSE;

   ch->spec_behavior++;

   switch (ch->spec_behavior % 8)
   {
   case 1:
      sprintf(buf, "@@r$n slams a fist against $s armour and barks the Ashfang blood-oath —\r\n"
                   "the canyon walls ring with iron syllables!@@N");
      act(buf, ch, NULL, NULL, TO_ROOM);
      do_warcry(ch, "");
      break;

   case 3:
   {
      int dam = number_range(3000, 5000);
      sprintf(buf, "@@r$n drives an armored shoulder into $N with bone-cracking force,\r\n"
                   "then detonates a point-blank iron charge against $S ribs!@@N");
      act(buf, ch, NULL, victim, TO_ROOM);
      calculate_damage(ch, victim, dam, gsn_holystrike, ELEMENT_PHYSICAL, TRUE);
      if ((sn = skill_lookup("high explosive")) >= 0)
         (*skill_table[sn].spell_fun)(sn, ch->level, ch, victim, NULL);
   }
   break;

   case 5:
      sprintf(buf, "@@r$n produces a pitch-blackened offhand blade and drags it across\r\n"
                   "$N — a thick volcanic residue clings to the wound!@@N");
      act(buf, ch, NULL, victim, TO_ROOM);
      one_hit(ch, victim, TYPE_HIT + 8);
      spell_poison(gsn_poison, ch->level, ch, victim, NULL);
      break;

   case 7:
      sprintf(buf, "@@r$n shouts '@@yBreak the line!@@r' and hammers $N with a\r\n"
                   "rapid succession of blows, each faster than the last!@@N");
      act(buf, ch, NULL, victim, TO_ROOM);
      one_hit(ch, victim, TYPE_HIT);
      one_hit(ch, victim, TYPE_HIT);
      break;
   }

   return TRUE;
}
