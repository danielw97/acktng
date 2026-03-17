#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

/*
 * spec_cinderteeth_anchor — Bellspine Resonance Anchor Construct (level 160, boss).
 *
 * Lore: A Spirebound Conclave resonance-monitoring construct, reactivated by
 * centuries of accumulated crystalline energy in the Bellspine passes.  Tracks
 * vibration signatures, maps standing frequencies, and discharges excess energy
 * as lethal resonance pulses.  The discharge cycle can be disrupted by grounding
 * the crystalline conduit with earth or physical force.
 *
 * INTERACTIVE MECHANIC — "Ground the Resonance":
 *   The construct charges a resonance pulse over 2 rounds.  Taking EARTH or
 *   PHYSICAL element damage during the charge disrupts the conduit and cancels
 *   the blast.  Failure allows the full pulse to fire.
 *
 * Cycling pattern (spec_behavior % 10):
 *   1 — Resonance charge begins (clear element tracking via spec_behavior2)
 *   2 — Conduit building: warn players to strike with EARTH or PHYSICAL
 *   3 — Resonance pulse fires (MENTAL+AIR AOE) or fizzles if grounded
 *   5 — Conduit strike: MENTAL+AIR single-target blast
 *   7 — Structural tremor: EARTH single-target
 *   9 — Anchor overload: heavy MENTAL AOE on all players
 *   (other rounds: no special action)
 *
 * spec_behavior  — round counter (reset to 0 when not fighting)
 * spec_behavior2 — element flags received since round 1 (damage callback)
 */

/* ---------- damage handler ---------- */

void spec_cinderteeth_anchor_damage(CHAR_DATA *ch, int element, int dam)
{
    (void)dam;
    ch->spec_behavior2 |= element;
}

/* ---------- attacks ---------- */

static void do_resonance_pulse_aoe(CHAR_DATA *ch)
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    char buf[MSL];

    sprintf(buf,
        "@@l$n's crystal conduits discharge in unison — a RESONANCE PULSE\r\n"
        "hammers every mind in the chamber with standing-wave force!@@N");
    act(buf, ch, NULL, NULL, TO_ROOM);

    for (vch = ch->in_room->first_person; vch != NULL; vch = vch_next)
    {
        vch_next = vch->next_in_room;
        if (IS_NPC(vch))
            continue;
        calculate_damage(ch, vch, number_range(4000, 7000),
                         gsn_holystrike, ELEMENT_MENTAL | ELEMENT_AIR, TRUE);
    }
}

static void do_anchor_overload_aoe(CHAR_DATA *ch)
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    char buf[MSL];

    sprintf(buf,
        "@@l$n's crystal lattice reaches critical saturation — an ANCHOR\r\n"
        "OVERLOAD tears through every mind in the chamber simultaneously!@@N");
    act(buf, ch, NULL, NULL, TO_ROOM);

    for (vch = ch->in_room->first_person; vch != NULL; vch = vch_next)
    {
        vch_next = vch->next_in_room;
        if (IS_NPC(vch))
            continue;
        calculate_damage(ch, vch, number_range(6000, 9000),
                         gsn_holystrike, ELEMENT_MENTAL, TRUE);
    }
}

/* ---------- special ---------- */

bool spec_cinderteeth_anchor(CHAR_DATA *ch)
{
    CHAR_DATA *victim;
    char buf[MSL];
    int round;

    if (!is_fighting(ch))
    {
        ch->spec_behavior  = 0;
        ch->spec_behavior2 = 0;
        return FALSE;
    }

    if ((victim = ch->fighting) == NULL)
        return FALSE;

    ch->spec_behavior++;
    round = ch->spec_behavior % 10;

    switch (round)
    {
    case 1:
        ch->spec_behavior2 = 0;
        sprintf(buf,
            "@@l$n's crystal conduits begin to resonate — standing waves\r\n"
            "build in the chamber walls with a low, bone-deep hum!@@N");
        act(buf, ch, NULL, NULL, TO_ROOM);
        break;

    case 2:
        sprintf(buf,
            "@@l$n's resonance charge intensifies — the hum becomes painful!\r\n"
            "@@WStrike with @@yEARTH@@W or @@yPHYSICAL@@W force to ground the conduit!@@N");
        act(buf, ch, NULL, NULL, TO_ROOM);
        break;

    case 3:
        if (IS_SET(ch->spec_behavior2, ELEMENT_EARTH) ||
            IS_SET(ch->spec_behavior2, ELEMENT_PHYSICAL))
        {
            sprintf(buf,
                "@@yYour earthen/physical blow disrupts $n's crystalline conduit!\r\n"
                "The resonance charge scatters harmlessly — the construct resets!@@N");
            act(buf, ch, NULL, NULL, TO_ROOM);
        }
        else
        {
            do_resonance_pulse_aoe(ch);
        }
        break;

    case 5:
        {
            int dam = number_range(7000, 10000);
            sprintf(buf,
                "@@l$n focuses $s standing-wave output into a single conduit\r\n"
                "and fires a piercing resonance lance directly into $N!@@N");
            act(buf, ch, NULL, victim, TO_ROOM);
            calculate_damage(ch, victim, dam, gsn_holystrike,
                             ELEMENT_MENTAL | ELEMENT_AIR, TRUE);
        }
        break;

    case 7:
        {
            int dam = number_range(8000, 11000);
            sprintf(buf,
                "@@l$n triggers a structural discharge — a deep seismic shudder\r\n"
                "erupts beneath $N as the Bellspine's fault-face answers!@@N");
            act(buf, ch, NULL, victim, TO_ROOM);
            calculate_damage(ch, victim, dam, gsn_holystrike, ELEMENT_EARTH, TRUE);
        }
        break;

    case 9:
        do_anchor_overload_aoe(ch);
        break;
    }

    return TRUE;
}
