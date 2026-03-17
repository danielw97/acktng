#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

/*
 * spec_cinderteeth_oracle — Grand Vent-Oracle of the Ash Cult (level 166, boss).
 *
 * Lore: A cultist who has breathed the Speaking Vent's gases for decades,
 * developing genuine geological intuition warped by theological conviction.
 * Fights by delivering verdicts — pronouncements derived from vent-pattern
 * reading that carry actual physical force.  Volcanic fire and gas are both
 * weapons and sacraments.
 *
 * Cycling pattern (spec_behavior % 10):
 *   1 — Vent interpretation: lava burst spell on target (geological fire)
 *   3 — Speaking Vent communion: room-wide gas breath
 *   5 — Theological verdict: FIRE+EARTH combined single-target blast
 *   7 — Doctrinal dispensation: dispel magic + flamestrike on target
 *   9 — Final judgment: concentrated FIRE blast (the mountain's ruling)
 */

bool spec_cinderteeth_oracle(CHAR_DATA *ch)
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

    switch (ch->spec_behavior % 10)
    {
    case 1:
        sprintf(buf,
            "@@R$n reads the vent's pulse and pronounces — '@@pThe mountain\r\n"
            "has shown me the shape of your ending!@@R' — and fire answers!@@N");
        act(buf, ch, NULL, victim, TO_ROOM);
        if ((sn = skill_lookup("lava burst")) >= 0)
            (*skill_table[sn].spell_fun)(sn, ch->level, ch, victim, NULL);
        break;

    case 3:
        sprintf(buf,
            "@@R$n opens $s arms wide and communes with the Speaking Vent —\r\n"
            "a gust of volcanic gas rolls across the chamber from the vent above!@@N");
        act(buf, ch, NULL, NULL, TO_ROOM);
        if ((sn = skill_lookup("gas breath")) >= 0)
            (*skill_table[sn].spell_fun)(sn, ch->level, ch, NULL, NULL);
        break;

    case 5:
        {
            int dam = number_range(9000, 12000);
            sprintf(buf,
                "@@R$n issues a theological verdict — the vent and the stone both\r\n"
                "answer as fire and earth converge on $N in judgment!@@N");
            act(buf, ch, NULL, victim, TO_ROOM);
            calculate_damage(ch, victim, dam, gsn_holystrike,
                             ELEMENT_FIRE | ELEMENT_EARTH, TRUE);
        }
        break;

    case 7:
        sprintf(buf,
            "@@R$n strips $N of false protections before the speaking vent —\r\n"
            "then delivers doctrinal dispensation in fire!@@N");
        act(buf, ch, NULL, victim, TO_ROOM);
        if ((sn = skill_lookup("dispel magic")) >= 0)
            (*skill_table[sn].spell_fun)(sn, ch->level, ch, victim, NULL);
        if ((sn = skill_lookup("flamestrike")) >= 0)
            (*skill_table[sn].spell_fun)(sn, ch->level, ch, victim, NULL);
        break;

    case 9:
        {
            int dam = number_range(10000, 13000);
            sprintf(buf,
                "@@R$n raises both fists and pronounces the mountain's ruling —\r\n"
                "a column of volcanic fire descends on $N with absolute finality!@@N");
            act(buf, ch, NULL, victim, TO_ROOM);
            calculate_damage(ch, victim, dam, gsn_holystrike, ELEMENT_FIRE, TRUE);
        }
        break;
    }

    return TRUE;
}
