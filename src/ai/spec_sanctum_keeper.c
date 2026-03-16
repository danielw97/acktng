#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

/*
 * spec_sanctum_keeper — Keeper of the Sanctum superboss (mob 181), level 163.
 *
 * Lore: An ageless guardian whose robes are etched with living runes that
 * shift and writhe as though alive.  The runes draw power from every school
 * of magic, and to overwhelm them you must strike with VARIETY — flooding
 * the runes with too many conflicting energies at once causes them to
 * overload and backfire on the Keeper.
 *
 * INTERACTIVE MECHANIC — "Runic Overload":
 *   Unlike the interrupt mechanics of other superbosses, the Keeper's runes
 *   periodically destabilize, opening a window where the group must deal
 *   damage using 3 or more DIFFERENT element types.  If enough elemental
 *   diversity is achieved, the runes overload and the Keeper's attack
 *   fizzles (or backfires).  If the group fails to diversify, the runes
 *   absorb the energy and the Keeper unleashes a devastating blast.
 *
 *   This rewards group coordination and varied builds rather than a single
 *   counter-element response.
 *
 *   Round cycle (spec_behavior % 15):
 *
 *     Round  1: Runes destabilize — window opens (clear element tracking)
 *     Round  2: Runes pulse — warn players to strike with VARIED elements
 *     Round  3: Evaluation — if 3+ distinct elements received, OVERLOAD
 *               (runes backfire, reduced/no blast); else ABSORPTION
 *               (full-power Arcane Cascade: 12-16k single-target)
 *     Round  4: Keeper reacts (flavor text)
 *     Round  5: Rest
 *
 *     Round  6: Second destabilization window opens
 *     Round  7: Runes surge — warn players
 *     Round  8: Second evaluation (same check)
 *     Round  9: Keeper reacts
 *     Round 10: Rest
 *
 *     Round 11: Third destabilization window opens
 *     Round 12: Runes critical — final warning
 *     Round 13: Third evaluation (same check)
 *     Round 14: Keeper channels for finale
 *     Round  0: SANCTUM COLLAPSE — room AOE, damage scales with number
 *               of failed overloads (absorptions) this cycle (0-3)
 *
 *   spec_behavior  — round counter (mod 15)
 *   spec_behavior2 — bits 0-8:  element flags received during current window
 *                    bits 12-13: count of absorptions (failed overloads)
 *                                this cycle
 *
 *   Overload threshold: 3 distinct elements (out of 9 possible)
 */

/* Bitmask helpers for spec_behavior2 */
#define KEEPER_ELEM_MASK 0x01FF /* bits 0-8: 9 element flags */
#define KEEPER_ABSORB_SHIFT 12
#define KEEPER_ABSORB_MASK 0x3000 /* bits 12-13: absorption count (0-3) */
#define KEEPER_OVERLOAD_THRESHOLD 3

static int keeper_count_elements(CHAR_DATA *ch)
{
   int flags = ch->spec_behavior2 & KEEPER_ELEM_MASK;
   int count = 0;
   while (flags)
   {
      count += flags & 1;
      flags >>= 1;
   }
   return count;
}

static int keeper_absorb_count(CHAR_DATA *ch)
{
   return (ch->spec_behavior2 >> KEEPER_ABSORB_SHIFT) & 0x03;
}

static void keeper_inc_absorb(CHAR_DATA *ch)
{
   int count = keeper_absorb_count(ch);
   if (count < 3)
      ch->spec_behavior2 =
          (ch->spec_behavior2 & ~KEEPER_ABSORB_MASK) | ((count + 1) << KEEPER_ABSORB_SHIFT);
}

static void keeper_clear_elem(CHAR_DATA *ch)
{
   ch->spec_behavior2 &= ~KEEPER_ELEM_MASK;
}

/* ---------- damage handler ---------- */

/*
 * Called by spec_handle_damage() whenever this mob takes a hit.
 * ORs the incoming element into the lower 9 bits of spec_behavior2.
 */
void spec_sanctum_keeper_damage(CHAR_DATA *ch, int element, int dam)
{
   (void)dam;
   ch->spec_behavior2 |= (element & KEEPER_ELEM_MASK);
}

/* ---------- attacks ---------- */

static void do_arcane_cascade(CHAR_DATA *ch)
{
   int dam = number_range(12000, 16000);
   char buf[MSL];

   sprintf(buf, "@@P$n's runes flare in unison — a cascade of raw arcane force\r\n"
                "slams into $N, the living runes burning with absorbed power!@@N");
   act(buf, ch, NULL, ch->fighting, TO_ROOM);
   calculate_damage(ch, ch->fighting, dam, gsn_holystrike, ELEMENT_MENTAL | ELEMENT_HOLY, TRUE);
}

static void do_sanctum_collapse(CHAR_DATA *ch, int absorptions)
{
   CHAR_DATA *vch;
   CHAR_DATA *vch_next;
   int base_dam;
   int dam;
   char buf[MSL];

   /* Base damage scales with number of absorptions (0-3) */
   base_dam = 3000 + (absorptions * 3500);

   if (absorptions == 0)
   {
      sprintf(buf, "@@a$n's runes sputter and dim — every overload has left $m\r\n"
                   "drained!  The sanctum shudders, but the collapse fizzles!@@N");
      act(buf, ch, NULL, NULL, TO_ROOM);
      return;
   }

   if (absorptions >= 3)
   {
      sprintf(buf, "@@R$n howls as the sanctum's full power channels through $s runes!\r\n"
                   "Three cycles of absorbed energy ERUPT in a cataclysmic storm of\r\n"
                   "arcane fury that tears at everything within the sanctum!@@N");
   }
   else if (absorptions == 2)
   {
      sprintf(buf, "@@R$n releases twin waves of stored runic energy — the sanctum\r\n"
                   "shakes violently as arcane force washes over everyone!@@N");
   }
   else
   {
      sprintf(buf, "@@e$n gathers the last of $s absorbed energy and hurls it\r\n"
                   "outward — a pulse of runic force ripples through the room!@@N");
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

bool spec_sanctum_keeper(CHAR_DATA *ch)
{
   char buf[MSL];
   int round;
   int num_elements;

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
   /* ---- Phase 1: Runic Destabilization ---- */
   case 1:
      keeper_clear_elem(ch);
      sprintf(buf, "@@P$n's runes begin to writhe and shift — cracks of unstable\r\n"
                   "light spill from $s robes as the living glyphs destabilize!@@N");
      act(buf, ch, NULL, NULL, TO_ROOM);
      break;

   case 2:
      sprintf(buf, "@@P$n's runes pulse wildly, each glyph hungry for energy!\r\n"
                   "@@WStrike with @@PMANY DIFFERENT@@W elements to overload them!@@N");
      act(buf, ch, NULL, NULL, TO_ROOM);
      break;

   case 3:
      num_elements = keeper_count_elements(ch);
      if (num_elements >= KEEPER_OVERLOAD_THRESHOLD)
      {
         sprintf(buf, "@@W$n's runes OVERLOAD — conflicting energies tear through\r\n"
                      "the glyphs!  The Keeper staggers as $s own runes backfire!@@N");
         act(buf, ch, NULL, NULL, TO_ROOM);
      }
      else
      {
         keeper_inc_absorb(ch);
         do_arcane_cascade(ch);
      }
      break;

   case 4:
      if (keeper_absorb_count(ch) > 0)
      {
         sprintf(buf, "@@a$n traces a rune in the air with one finger, the glyph\r\n"
                      "pulsing with stored power.  The Keeper's eyes narrow coldly.@@N");
         act(buf, ch, NULL, NULL, TO_ROOM);
      }
      else
      {
         sprintf(buf, "@@a$n's runes flicker weakly, their energy spent.  The Keeper\r\n"
                      "snarls and begins to reweave the glyphs from nothing.@@N");
         act(buf, ch, NULL, NULL, TO_ROOM);
      }
      break;

   /* ---- Phase 2: Second Destabilization ---- */
   case 6:
      keeper_clear_elem(ch);
      sprintf(buf, "@@P$n's runes flare again — the glyphs spiral faster now,\r\n"
                   "each one cycling through colors as they draw in ambient power!@@N");
      act(buf, ch, NULL, NULL, TO_ROOM);
      break;

   case 7:
      sprintf(buf, "@@P$n's runes surge with building force — the air crackles!\r\n"
                   "@@WOverwhelm the runes with @@PDIVERSE@@W elements before they stabilize!@@N");
      act(buf, ch, NULL, NULL, TO_ROOM);
      break;

   case 8:
      num_elements = keeper_count_elements(ch);
      if (num_elements >= KEEPER_OVERLOAD_THRESHOLD)
      {
         sprintf(buf, "@@W$n's runes shatter with a blinding flash!  Too many\r\n"
                      "conflicting forces — the Keeper reels from the feedback!@@N");
         act(buf, ch, NULL, NULL, TO_ROOM);
      }
      else
      {
         keeper_inc_absorb(ch);
         do_arcane_cascade(ch);
      }
      break;

   case 9:
      sprintf(buf, "@@a$n draws $s robes tighter, fingers tracing protective\r\n"
                   "sigils as the living runes slowly knit themselves back together.@@N");
      act(buf, ch, NULL, NULL, TO_ROOM);
      break;

   /* ---- Phase 3: Third Destabilization ---- */
   case 11:
      keeper_clear_elem(ch);
      sprintf(buf, "@@R$n's runes ignite — every glyph blazes at once, the air\r\n"
                   "itself bending around the Keeper as reality warps!@@N");
      act(buf, ch, NULL, NULL, TO_ROOM);
      break;

   case 12:
      sprintf(buf, "@@R$n's runes reach critical mass — the sanctum itself trembles!\r\n"
                   "@@WThis is your last chance — flood the runes with @@RVARIED@@W elements!@@N");
      act(buf, ch, NULL, NULL, TO_ROOM);
      break;

   case 13:
      num_elements = keeper_count_elements(ch);
      if (num_elements >= KEEPER_OVERLOAD_THRESHOLD)
      {
         sprintf(buf, "@@W$n's runes DETONATE inward!  The cascade of conflicting\r\n"
                      "energies tears the glyphs apart — the Keeper howls in pain!@@N");
         act(buf, ch, NULL, NULL, TO_ROOM);
      }
      else
      {
         keeper_inc_absorb(ch);
         do_arcane_cascade(ch);
      }
      break;

   case 14:
      if (keeper_absorb_count(ch) > 0)
      {
         sprintf(buf, "@@R$n begins channeling every absorbed rune toward the sanctum\r\n"
                      "walls — the stones glow with terrible, building pressure!@@N");
         act(buf, ch, NULL, NULL, TO_ROOM);
      }
      else
      {
         sprintf(buf, "@@a$n falters, $s runes dim and cracked.  Without any stored\r\n"
                      "energy, the Keeper's final gambit has nothing to fuel it.@@N");
         act(buf, ch, NULL, NULL, TO_ROOM);
      }
      break;

   /* ---- SANCTUM COLLAPSE (end of cycle) ---- */
   case 0:
      do_sanctum_collapse(ch, keeper_absorb_count(ch));
      /* Reset absorption count for next cycle */
      ch->spec_behavior2 &= ~KEEPER_ABSORB_MASK;
      break;
   }

   return TRUE;
}
