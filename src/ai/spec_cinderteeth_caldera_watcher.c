#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

/*
 * spec_cinderteeth_caldera_watcher — Secondtooth Caldera Watcher (level 168, boss).
 *
 * Lore: An enormous heat-adapted apex predator that has established permanent
 * caldera territory on the Secondtooth.  Not a construct — a genuine creature
 * of extreme scale, with heat-absorption adaptations that make its strikes
 * carry latent volcanic energy.  Enters a territorial frenzy when wounded.
 *
 * Cycling pattern (spec_behavior % 8):
 *   0 — Territory circuit (flavor only, unless in frenzy: bonus strike)
 *   2 — Caldera charge: massive PHYSICAL crush
 *   4 — Heat-blast strike: FIRE single-target (latent volcanic heat)
 *   6 — Magma tremor: EARTH single-target + earthquake spell
 *   (other rounds: no special action)
 *
 * Frenzy: activates below 25% HP.  On first trigger, announces frenzy.
 *   spec_behavior2: 0 = normal, 1 = frenzy active.
 */

bool spec_cinderteeth_caldera_watcher(CHAR_DATA *ch)
{
   CHAR_DATA *victim;
   char buf[MSL];
   int sn;
   int in_frenzy;

   if (!is_fighting(ch))
   {
      ch->spec_behavior = 0;
      ch->spec_behavior2 = 0;
      return FALSE;
   }

   if ((victim = ch->fighting) == NULL)
      return FALSE;

   /* Check frenzy threshold */
   if (ch->spec_behavior2 == 0 && ch->max_hit > 0 && ch->hit * 100 / ch->max_hit < 25)
   {
      ch->spec_behavior2 = 1;
      sprintf(buf, "@@R$n's territorial fury erupts — the caldera watcher enters\r\n"
                   "a predator's last frenzy, moving with terrifying speed!@@N");
      act(buf, ch, NULL, NULL, TO_ROOM);
   }

   in_frenzy = (ch->spec_behavior2 == 1);
   ch->spec_behavior++;

   switch (ch->spec_behavior % 8)
   {
   case 0:
      if (in_frenzy)
      {
         int dam = number_range(7000, 10000);
         sprintf(buf, "@@R$n circles with primal speed and hammers $N from an\r\n"
                      "unexpected angle — frenzy-driven and merciless!@@N");
         act(buf, ch, NULL, victim, TO_ROOM);
         calculate_damage(ch, victim, dam, gsn_holystrike, ELEMENT_PHYSICAL, TRUE);
      }
      else
      {
         sprintf(buf, "@@R$n pivots to pace the caldera perimeter, $s heat-shimmer\r\n"
                      "trail marking $s territorial circuit.@@N");
         act(buf, ch, NULL, NULL, TO_ROOM);
      }
      break;

   case 2:
   {
      int dam = number_range(in_frenzy ? 11000 : 10000, in_frenzy ? 15000 : 13000);
      sprintf(buf, "@@R$n launches a full-weight caldera charge — $s entire body\r\n"
                   "hammers into $N with the momentum of the Secondtooth behind it!@@N");
      act(buf, ch, NULL, victim, TO_ROOM);
      calculate_damage(ch, victim, dam, gsn_holystrike, ELEMENT_PHYSICAL, TRUE);
   }
   break;

   case 4:
   {
      int dam = number_range(9000, 12000);
      sprintf(buf, "@@R$n exhales a blast of superheated air from $s caldera-adapted\r\n"
                   "lung chambers directly into $N — latent volcanic heat!@@N");
      act(buf, ch, NULL, victim, TO_ROOM);
      calculate_damage(ch, victim, dam, gsn_holystrike, ELEMENT_FIRE, TRUE);
   }
   break;

   case 6:
   {
      int dam = number_range(9000, 12000);
      sprintf(buf, "@@R$n drives $s weight into the coupled caldera floor —\r\n"
                   "the Secondtooth's shared magma tremor slams through $N!@@N");
      act(buf, ch, NULL, victim, TO_ROOM);
      calculate_damage(ch, victim, dam, gsn_holystrike, ELEMENT_EARTH, TRUE);
      if ((sn = skill_lookup("earthquake")) >= 0)
         (*skill_table[sn].spell_fun)(sn, ch->level, ch, NULL, NULL);
   }
   break;
   }

   return TRUE;
}
