#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

/*
 * spec_cinderteeth_patriarch — The Ember Throne Patriarch (level 170, final boss).
 *
 * Lore: A geological intelligence that has been accumulating consciousness from
 * the Firsttooth caldera's eruption-dormancy cycles for an indeterminate number
 * of centuries.  The Ash Cult worships it as a judge delivering verdicts; it
 * is indifferent to this.  It fights by expressing the Ember Crown's geology
 * directly: magma surges, tectonic releases, gas venting, atmospheric memory,
 * and a culminating Ember Verdict that combines all geological forces at once.
 *
 * Cycling pattern (spec_behavior % 12):
 *   1  — Mountain stirs: strip defenses from all players (dispel magic AOE)
 *   2  — Magma surge building (flavor)
 *   3  — Magma surge building (warning)
 *   4  — Magma eruption: heavy FIRE+EARTH single-target blast
 *   5  — Tectonic pressure (flavor)
 *   6  — Tectonic release: EARTH AOE on all players
 *   7  — Volcanic gas venting: room-wide gas breath
 *   8  — Mountain's memory: MENTAL single-target (accumulated centuries of will)
 *   9  — Geological verdict building (flavor)
 *   10 — Geological verdict building (warning — all elements incoming)
 *   11 — Geological verdict building (final warning)
 *   0  — EMBER VERDICT: massive FIRE+EARTH+AIR AOE on all players
 */

/* ---------- helpers ---------- */

static void do_dispel_all(CHAR_DATA *ch)
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    char buf[MSL];
    int sn;

    sprintf(buf,
        "@@R$n's geological consciousness turns its attention to the intruders —\r\n"
        "the Ember Crown's full awareness strips every artificial protection!@@N");
    act(buf, ch, NULL, NULL, TO_ROOM);

    if ((sn = skill_lookup("dispel magic")) < 0)
        return;

    for (vch = ch->in_room->first_person; vch != NULL; vch = vch_next)
    {
        vch_next = vch->next_in_room;
        if (IS_NPC(vch))
            continue;
        (*skill_table[sn].spell_fun)(sn, ch->level, ch, vch, NULL);
    }
}

static void do_tectonic_release(CHAR_DATA *ch)
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    char buf[MSL];

    sprintf(buf,
        "@@R$n releases centuries of tectonic pressure — the caldera floor\r\n"
        "heaves and the walls crack as EARTH force hammers everyone!@@N");
    act(buf, ch, NULL, NULL, TO_ROOM);

    for (vch = ch->in_room->first_person; vch != NULL; vch = vch_next)
    {
        vch_next = vch->next_in_room;
        if (IS_NPC(vch))
            continue;
        calculate_damage(ch, vch, number_range(6000, 9000),
                         gsn_holystrike, ELEMENT_EARTH, TRUE);
    }
}

static void do_ember_verdict(CHAR_DATA *ch)
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    char buf[MSL];

    sprintf(buf,
        "@@R$n pronounces the EMBER VERDICT — fire, stone, and volcanic wind\r\n"
        "combine in a cataclysmic eruption that engulfs the entire chamber!\r\n"
        "The mountain has rendered its judgment on ALL present!@@N");
    act(buf, ch, NULL, NULL, TO_ROOM);

    for (vch = ch->in_room->first_person; vch != NULL; vch = vch_next)
    {
        vch_next = vch->next_in_room;
        if (IS_NPC(vch))
            continue;
        calculate_damage(ch, vch, number_range(8000, 12000),
                         gsn_holystrike,
                         ELEMENT_FIRE | ELEMENT_EARTH | ELEMENT_AIR, TRUE);
    }
}

/* ---------- special ---------- */

bool spec_cinderteeth_patriarch(CHAR_DATA *ch)
{
    CHAR_DATA *victim;
    char buf[MSL];
    int sn;
    int round;

    if (!is_fighting(ch))
    {
        ch->spec_behavior = 0;
        return FALSE;
    }

    if ((victim = ch->fighting) == NULL)
        return FALSE;

    ch->spec_behavior++;
    round = ch->spec_behavior % 12;

    switch (round)
    {
    case 1:
        do_dispel_all(ch);
        break;

    case 2:
        sprintf(buf,
            "@@R$n's presence deepens — the caldera walls radiate red and\r\n"
            "the magma chamber below rumbles with building pressure.@@N");
        act(buf, ch, NULL, NULL, TO_ROOM);
        break;

    case 3:
        sprintf(buf,
            "@@R$n's attention narrows on $N — the magma surge is almost\r\n"
            "ready.  The floor radiates heat too intense to touch.@@N");
        act(buf, ch, NULL, victim, TO_ROOM);
        break;

    case 4:
        {
            int dam = number_range(12000, 16000);
            sprintf(buf,
                "@@R$n releases the magma surge — fire and volcanic stone\r\n"
                "erupt from the floor directly beneath $N in a towering column!@@N");
            act(buf, ch, NULL, victim, TO_ROOM);
            calculate_damage(ch, victim, dam, gsn_holystrike,
                             ELEMENT_FIRE | ELEMENT_EARTH, TRUE);
        }
        break;

    case 5:
        sprintf(buf,
            "@@R$n settles — the caldera vents hiss and the fault-lines\r\n"
            "beneath the floor shift with slow, irresistible geological force.@@N");
        act(buf, ch, NULL, NULL, TO_ROOM);
        if ((sn = skill_lookup("earthquake")) >= 0)
            (*skill_table[sn].spell_fun)(sn, ch->level, ch, NULL, NULL);
        break;

    case 6:
        do_tectonic_release(ch);
        break;

    case 7:
        sprintf(buf,
            "@@R$n exhales — the caldera's volcanic gas reserves vent in a\r\n"
            "choking wave that fills the chamber with ancient sulfur memory!@@N");
        act(buf, ch, NULL, NULL, TO_ROOM);
        if ((sn = skill_lookup("gas breath")) >= 0)
            (*skill_table[sn].spell_fun)(sn, ch->level, ch, NULL, NULL);
        break;

    case 8:
        {
            int dam = number_range(10000, 14000);
            sprintf(buf,
                "@@R$n's accumulated centuries press directly against $N's mind —\r\n"
                "the weight of geological time is not survivable without strength!@@N");
            act(buf, ch, NULL, victim, TO_ROOM);
            calculate_damage(ch, victim, dam, gsn_holystrike, ELEMENT_MENTAL, TRUE);
        }
        break;

    case 9:
        sprintf(buf,
            "@@R$n's entire caldera body glows — the Firsttooth, Secondtooth,\r\n"
            "and every vent in the Ember Crown begin to pulse in unison.@@N");
        act(buf, ch, NULL, NULL, TO_ROOM);
        break;

    case 10:
        sprintf(buf,
            "@@R$n raises $s presence to its full geological scope — fire\r\n"
            "and earth and volcanic wind are all building toward something.@@N");
        act(buf, ch, NULL, NULL, TO_ROOM);
        break;

    case 11:
        sprintf(buf,
            "@@RALL ELEMENTS CONVERGE — $n's geological verdict takes final shape!\r\n"
            "There is no counter to what the mountain has decided.@@N");
        act(buf, ch, NULL, NULL, TO_ROOM);
        break;

    case 0:
        do_ember_verdict(ch);
        break;
    }

    return TRUE;
}
