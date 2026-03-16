#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

/*
 * spec_keep_elemental_captain — Guard Captain 170 (elemental superboss).
 *
 * The Captain cycles through four elements in a 12-round pattern,
 * telegraphing each spell 2 rounds before it fires.
 *
 * KEY MECHANIC: taking opposing-element damage during the 2-round
 * wind-up interrupts the cast and forces the Captain to restart the
 * cycle from the next element.
 *
 *   Opposing pairs:  FIRE ↔ WATER,  AIR ↔ EARTH
 *
 *   Round cycle (spec_behavior % 12):
 *     1  — FIRE  telegraph-1  (clear interrupt flag)
 *     2  — FIRE  telegraph-2  (warn players)
 *     3  — FIRE  cast          (or fizzle if WATER damage was taken)
 *     4  — AIR   telegraph-1
 *     5  — AIR   telegraph-2
 *     6  — AIR   cast          (or fizzle if EARTH damage was taken)
 *     7  — WATER telegraph-1
 *     8  — WATER telegraph-2
 *     9  — WATER cast          (or fizzle if FIRE damage was taken)
 *     10 — EARTH telegraph-1
 *     11 — EARTH telegraph-2
 *     0  — EARTH cast          (or fizzle if AIR damage was taken)
 *
 *   spec_behavior  — round counter (incremented every combat pulse,
 *                    reset to 0 when not fighting)
 *   spec_behavior2 — bitmask of element flags received since the last
 *                    telegraph-1 round; set by the damage system
 *                    (damage.c: do_damage) whenever a mob with a
 *                    spec_fun takes a hit.
 */

/* ---------- damage handler ---------- */

/*
 * Called by special.c:spec_handle_damage() whenever this mob takes a hit.
 * ORs the incoming element into spec_behavior2 so the cast-round check
 * can detect whether the opposing element was applied during the charge.
 */
void spec_keep_elemental_captain_damage(CHAR_DATA *ch, int element, int dam)
{
   (void)dam;
   ch->spec_behavior2 |= element;
}

/* ---------- element casts ---------- */

static void do_fire_blast(CHAR_DATA *ch)
{
   int dam = number_range(10000, 14000);
   char buf[MSL];

   sprintf(buf, "@@R$n drives a pillar of searing flame into $N — the air ignites\r\n"
                "and the stone floor glows red!@@N");
   act(buf, ch, NULL, ch->fighting, TO_ROOM);
   calculate_damage(ch, ch->fighting, dam, gsn_holystrike, ELEMENT_FIRE, TRUE);
}

static void do_air_blast(CHAR_DATA *ch)
{
   int dam = number_range(9000, 13000);
   char buf[MSL];

   sprintf(buf, "@@C$n calls down a lance of pure lightning onto $N — every hair\r\n"
                "stands on end as the air cracks with thunder!@@N");
   act(buf, ch, NULL, ch->fighting, TO_ROOM);
   calculate_damage(ch, ch->fighting, dam, gsn_holystrike, ELEMENT_AIR, TRUE);
}

static void do_water_blast(CHAR_DATA *ch)
{
   int dam = number_range(9000, 13000);
   char buf[MSL];

   sprintf(buf, "@@B$n releases a crushing torrent of icy water at $N — the cold\r\n"
                "cuts to the bone and the ground floods ankle-deep!@@N");
   act(buf, ch, NULL, ch->fighting, TO_ROOM);
   calculate_damage(ch, ch->fighting, dam, gsn_holystrike, ELEMENT_WATER, TRUE);
}

static void do_earth_blast(CHAR_DATA *ch)
{
   int dam = number_range(10000, 14000);
   char buf[MSL];

   sprintf(buf, "@@g$n slams both fists down and the earth erupts beneath $N —\r\n"
                "a geyser of stone and acid hurls $M skyward!@@N");
   act(buf, ch, NULL, ch->fighting, TO_ROOM);
   calculate_damage(ch, ch->fighting, dam, gsn_holystrike, ELEMENT_EARTH, TRUE);
}

/* ---------- special ---------- */

bool spec_keep_elemental_captain(CHAR_DATA *ch)
{
   char buf[MSL];

   if (!is_fighting(ch))
   {
      ch->spec_behavior = 0;
      ch->spec_behavior2 = 0;
      return FALSE;
   }

   ch->spec_behavior++;

   switch (ch->spec_behavior % 12)
   {
   /* ---- FIRE phase ---- */
   case 1:
      ch->spec_behavior2 = 0;
      sprintf(buf, "@@R$n's gauntlets begin to glow a deep crimson — the air shimmers\r\n"
                   "with rising heat as FIRE energy builds!@@N");
      act(buf, ch, NULL, NULL, TO_ROOM);
      break;

   case 2:
      sprintf(buf, "@@R$n's armour blazes red-hot!  The fire is almost ready —\r\n"
                   "douse it with @@BWATER@@R before it's too late!@@N");
      act(buf, ch, NULL, NULL, TO_ROOM);
      break;

   case 3:
      if (IS_SET(ch->spec_behavior2, ELEMENT_WATER))
      {
         sprintf(buf, "@@B$n's flames gutter and die, snuffed out by the water you\r\n"
                      "hurled at $m!  The Captain snarls and resets!@@N");
         act(buf, ch, NULL, NULL, TO_ROOM);
      }
      else
         do_fire_blast(ch);
      break;

   /* ---- AIR phase ---- */
   case 4:
      ch->spec_behavior2 = 0;
      sprintf(buf, "@@C$n raises $s sword and wind screams around the blade —\r\n"
                   "AIR energy crackles through $s armour!@@N");
      act(buf, ch, NULL, NULL, TO_ROOM);
      break;

   case 5:
      sprintf(buf, "@@C$n's sword hums with building lightning!  Counter with\r\n"
                   "@@gEARTH@@C to ground the charge!@@N");
      act(buf, ch, NULL, NULL, TO_ROOM);
      break;

   case 6:
      if (IS_SET(ch->spec_behavior2, ELEMENT_EARTH))
      {
         sprintf(buf, "@@g$n's lightning arcs harmlessly into the earth as you ground\r\n"
                      "it!  The Captain's charge collapses — $e starts again!@@N");
         act(buf, ch, NULL, NULL, TO_ROOM);
      }
      else
         do_air_blast(ch);
      break;

   /* ---- WATER phase ---- */
   case 7:
      ch->spec_behavior2 = 0;
      sprintf(buf, "@@B$n draws moisture from the air — frost crawls across $s\r\n"
                   "armour as WATER energy coils around $m!@@N");
      act(buf, ch, NULL, NULL, TO_ROOM);
      break;

   case 8:
      sprintf(buf, "@@B$n's breath fogs white; ice forms at $s feet!  Strike with\r\n"
                   "@@RFIRE@@B now to shatter the freeze before it's unleashed!@@N");
      act(buf, ch, NULL, NULL, TO_ROOM);
      break;

   case 9:
      if (IS_SET(ch->spec_behavior2, ELEMENT_FIRE))
      {
         sprintf(buf, "@@RYour fire melts $n's icy charge in a burst of steam!\r\n"
                      "The Captain staggers — the spell is broken!@@N");
         act(buf, ch, NULL, NULL, TO_ROOM);
      }
      else
         do_water_blast(ch);
      break;

   /* ---- EARTH phase ---- */
   case 10:
      ch->spec_behavior2 = 0;
      sprintf(buf, "@@g$n plants $s feet wide and the floor trembles — stone dust\r\n"
                   "rises as EARTH power surges up through $m!@@N");
      act(buf, ch, NULL, NULL, TO_ROOM);
      break;

   case 11:
      sprintf(buf, "@@g$n's fists glow with the weight of mountains!  Scatter the\r\n"
                   "charge with @@CAIR@@g — a gust can blow it apart!@@N");
      act(buf, ch, NULL, NULL, TO_ROOM);
      break;

   case 0:
      if (IS_SET(ch->spec_behavior2, ELEMENT_AIR))
      {
         sprintf(buf, "@@CThe wind you sent tears through $n's earthen charge!\r\n"
                      "Dust scatters harmlessly — the Captain resets!@@N");
         act(buf, ch, NULL, NULL, TO_ROOM);
      }
      else
         do_earth_blast(ch);
      break;
   }

   return TRUE;
}
