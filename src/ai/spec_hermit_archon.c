#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

/*
 * spec_hermit_archon — Hermit Archon superboss (mob 183), level 166.
 *
 * Lore: An ancient guardian who has meditated in solitude for centuries,
 * drawing power from undisturbed stillness.  His fury at being disturbed
 * manifests as devastating arcane storms — but centuries of isolation have
 * also made his concentration brittle.
 *
 * INTERACTIVE MECHANIC — "Break the Ward":
 *   The Archon charges a ward over 3 rounds, telegraphing which element
 *   will fuel the blast.  Players must deal the OPPOSING element during
 *   the charge to shatter the ward and prevent the attack.
 *
 *   Ward cycle (15-round, 3 wards per cycle):
 *
 *     Rounds  1-2: HOLY ward charges    → counter with SHADOW damage
 *     Round   3:   HOLY blast fires     (or fizzles if countered)
 *     Round   4:   Archon seethes (flavor)
 *     Round   5:   Archon recovers
 *
 *     Rounds  6-7: SHADOW ward charges  → counter with HOLY damage
 *     Round   8:   SHADOW blast fires   (or fizzles if countered)
 *     Round   9:   Archon seethes (flavor)
 *     Round  10:   Archon recovers
 *
 *     Rounds 11-12: MENTAL ward charges → counter with PHYSICAL damage
 *     Round  13:   MENTAL blast fires   (or fizzles if countered)
 *     Round  14:   Archon channels ambient fury
 *     Round   0:   SOLITUDE BURST — room-wide AOE (always fires,
 *                  but damage scales with how many wards went un-broken
 *                  this cycle, tracked in spec_behavior2 bits 0-2)
 *
 *   spec_behavior  — round counter (mod 15)
 *   spec_behavior2 — bits 0-2: count of unbroken wards this cycle
 *                    bits 8+: element flags received during current charge
 *
 *   Opposing pairs:  HOLY ↔ SHADOW,  MENTAL ↔ PHYSICAL
 */

/* Bitmask helpers for spec_behavior2 */
#define ARCHON_WARD_COUNT_MASK 0x07 /* bits 0-2: unbroken ward count */
#define ARCHON_ELEM_SHIFT 8
#define ARCHON_ELEM_MASK 0xFF00 /* bits 8-15: element flags received */

static int archon_ward_count(CHAR_DATA *ch)
{
   return ch->spec_behavior2 & ARCHON_WARD_COUNT_MASK;
}

static void archon_inc_ward_count(CHAR_DATA *ch)
{
   int count = archon_ward_count(ch);
   if (count < 3)
      ch->spec_behavior2 = (ch->spec_behavior2 & ~ARCHON_WARD_COUNT_MASK) | (count + 1);
}

static int archon_elem_received(CHAR_DATA *ch)
{
   return (ch->spec_behavior2 >> ARCHON_ELEM_SHIFT) & 0xFF;
}

static void archon_clear_elem(CHAR_DATA *ch)
{
   ch->spec_behavior2 &= ~ARCHON_ELEM_MASK;
}

/* ---------- damage handler ---------- */

/*
 * Called by spec_handle_damage() whenever this mob takes a hit.
 * ORs the incoming element into the upper byte of spec_behavior2.
 */
void spec_hermit_archon_damage(CHAR_DATA *ch, int element, int dam)
{
   (void)dam;
   ch->spec_behavior2 |= (element << ARCHON_ELEM_SHIFT);
}

/* ---------- attacks ---------- */

static void do_holy_blast(CHAR_DATA *ch)
{
   int dam = number_range(12000, 16000);
   char buf[MSL];

   sprintf(buf, "@@W$n's ward detonates in a blinding column of holy light —\r\n"
                "purifying radiance sears $N to the bone!@@N");
   act(buf, ch, NULL, ch->fighting, TO_ROOM);
   calculate_damage(ch, ch->fighting, dam, gsn_holystrike, ELEMENT_HOLY, TRUE);
}

static void do_shadow_blast(CHAR_DATA *ch)
{
   int dam = number_range(12000, 16000);
   char buf[MSL];

   sprintf(buf, "@@D$n's ward collapses inward, releasing a vortex of devouring\r\n"
                "shadow that tears at $N's very essence!@@N");
   act(buf, ch, NULL, ch->fighting, TO_ROOM);
   calculate_damage(ch, ch->fighting, dam, gsn_holystrike, ELEMENT_SHADOW, TRUE);
}

static void do_mental_blast(CHAR_DATA *ch)
{
   int dam = number_range(11000, 15000);
   char buf[MSL];

   sprintf(buf, "@@P$n's ward shatters outward — a psychic shockwave hammers\r\n"
                "$N's mind with centuries of accumulated solitary madness!@@N");
   act(buf, ch, NULL, ch->fighting, TO_ROOM);
   calculate_damage(ch, ch->fighting, dam, gsn_holystrike, ELEMENT_MENTAL, TRUE);
}

static void do_solitude_burst(CHAR_DATA *ch, int unbroken)
{
   CHAR_DATA *vch;
   CHAR_DATA *vch_next;
   int base_dam;
   int dam;
   char buf[MSL];

   /* Base damage scales with number of unbroken wards (0-3) */
   base_dam = 3000 + (unbroken * 3000);

   if (unbroken == 0)
   {
      sprintf(buf, "@@a$n shudders with frustrated rage, but $s solitude is too\r\n"
                   "fractured to channel — the burst fizzles harmlessly!@@N");
      act(buf, ch, NULL, NULL, TO_ROOM);
      return;
   }

   if (unbroken >= 3)
   {
      sprintf(buf, "@@R$n howls with the fury of unbroken centuries!  The air itself\r\n"
                   "SCREAMS as $s three wards combine into a cataclysmic storm of\r\n"
                   "light, shadow, and madness that engulfs EVERYTHING!@@N");
   }
   else if (unbroken == 2)
   {
      sprintf(buf, "@@R$n channels $s remaining fury — twin wards explode outward\r\n"
                   "in a devastating wave of arcane destruction!@@N");
   }
   else
   {
      sprintf(buf, "@@e$n gathers $s last unbroken ward and hurls it outward —\r\n"
                   "a pulse of bitter arcane force washes over the room!@@N");
   }

   act(buf, ch, NULL, NULL, TO_ROOM);

   for (vch = ch->in_room->first_person; vch != NULL; vch = vch_next)
   {
      vch_next = vch->next_in_room;

      if (IS_NPC(vch))
         continue;

      dam = number_range(base_dam - 1000, base_dam + 1000);
      calculate_damage(ch, vch, dam, gsn_holystrike, ELEMENT_HOLY | ELEMENT_SHADOW | ELEMENT_MENTAL,
                       TRUE);
   }
}

/* ---------- special ---------- */

bool spec_hermit_archon(CHAR_DATA *ch)
{
   char buf[MSL];
   int round;

   if (!is_fighting(ch))
   {
      ch->spec_behavior = 0;
      ch->spec_behavior2 = 0;
      return FALSE;
   }

   ch->spec_behavior++;
   round = ch->spec_behavior % 15;

   switch (round)
   {
   /* ---- HOLY ward ---- */
   case 1:
      archon_clear_elem(ch);
      sprintf(buf, "@@W$n's eyes flare white — a shimmering ward of holy energy\r\n"
                   "begins to coalesce around $s hunched frame!@@N");
      act(buf, ch, NULL, NULL, TO_ROOM);
      break;

   case 2:
      sprintf(buf, "@@W$n's holy ward blazes brighter!  The air hums with purifying\r\n"
                   "force — strike with @@DSHADOW@@W to shatter it!@@N");
      act(buf, ch, NULL, NULL, TO_ROOM);
      break;

   case 3:
      if (archon_elem_received(ch) & ELE_SHADOW)
      {
         sprintf(buf, "@@DYour shadow tears through $n's holy ward!  The light gutters\r\n"
                      "and dies — the Archon snarls as $s concentration shatters!@@N");
         act(buf, ch, NULL, NULL, TO_ROOM);
      }
      else
      {
         do_holy_blast(ch);
         archon_inc_ward_count(ch);
      }
      break;

   case 4:
      sprintf(buf, "@@a$n mutters bitterly about $s broken solitude, ancient eyes\r\n"
                   "burning with quiet, mounting rage.@@N");
      act(buf, ch, NULL, NULL, TO_ROOM);
      break;

   /* ---- SHADOW ward ---- */
   case 6:
      archon_clear_elem(ch);
      sprintf(buf, "@@D$n draws darkness from the corners of the room — tendrils\r\n"
                   "of shadow spiral inward, forming a ward of consuming void!@@N");
      act(buf, ch, NULL, NULL, TO_ROOM);
      break;

   case 7:
      sprintf(buf, "@@D$n's shadow ward deepens — the darkness is almost solid!\r\n"
                   "Pierce it with @@WHOLY@@D light before it's unleashed!@@N");
      act(buf, ch, NULL, NULL, TO_ROOM);
      break;

   case 8:
      if (archon_elem_received(ch) & ELE_HOLY)
      {
         sprintf(buf, "@@WYour holy light burns through $n's shadow ward!  The\r\n"
                      "darkness evaporates with a shriek — the Archon recoils!@@N");
         act(buf, ch, NULL, NULL, TO_ROOM);
      }
      else
      {
         do_shadow_blast(ch);
         archon_inc_ward_count(ch);
      }
      break;

   case 9:
      sprintf(buf, "@@a$n straightens slowly, bones cracking, and fixes you with\r\n"
                   "a stare of ancient, implacable hatred.@@N");
      act(buf, ch, NULL, NULL, TO_ROOM);
      break;

   /* ---- MENTAL ward ---- */
   case 11:
      archon_clear_elem(ch);
      sprintf(buf, "@@P$n's temples pulse with visible psychic energy — the air\r\n"
                   "grows heavy with the pressure of $s focused will!@@N");
      act(buf, ch, NULL, NULL, TO_ROOM);
      break;

   case 12:
      sprintf(buf, "@@P$n's mental ward reaches critical mass — you can feel your\r\n"
                   "thoughts bending!  Strike with @@yPHYSICAL@@P force to break $s focus!@@N");
      act(buf, ch, NULL, NULL, TO_ROOM);
      break;

   case 13:
      if (archon_elem_received(ch) & ELE_PHYSICAL)
      {
         sprintf(buf, "@@yYour physical blow jars $n's concentration!  The psychic\r\n"
                      "pressure pops like a bubble — the Archon staggers!@@N");
         act(buf, ch, NULL, NULL, TO_ROOM);
      }
      else
      {
         do_mental_blast(ch);
         archon_inc_ward_count(ch);
      }
      break;

   case 14:
      if (archon_ward_count(ch) > 0)
      {
         sprintf(buf, "@@R$n begins drawing power from $s unbroken wards — the\r\n"
                      "residual energy crackles and builds toward something terrible!@@N");
         act(buf, ch, NULL, NULL, TO_ROOM);
      }
      else
      {
         sprintf(buf, "@@a$n paces in a tight circle, muttering.  Without any intact\r\n"
                      "wards to fuel $s fury, $e seems diminished... for now.@@N");
         act(buf, ch, NULL, NULL, TO_ROOM);
      }
      break;

   /* ---- SOLITUDE BURST (end of cycle) ---- */
   case 0:
      do_solitude_burst(ch, archon_ward_count(ch));
      /* Reset ward count for next cycle */
      ch->spec_behavior2 &= ~ARCHON_WARD_COUNT_MASK;
      break;
   }

   return TRUE;
}
