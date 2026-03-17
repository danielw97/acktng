#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

/*
 * spec_cinderteeth_warden_cmd — Ironpost Warden-Commander (level 152, mini-boss).
 *
 * Lore: The undead ghost of the Ironpost Lodge's last warden, still operating
 * on centuries-old emergency-powers authority.  Strips magical defenses from
 * opponents and escalates to lethal force when wounded — invoking the Warden
 * Commission's Emergency Compact to justify the escalation.
 *
 * Behavior:
 *   Each tick: 35% chance to pick from an authority spell pool.
 *   Once, on first tick below 40% HP: Emergency Compact invocation
 *   (announcement + high explosive on target regardless of 35% chance).
 *
 *   spec_behavior2: 0 = emergency powers not yet triggered,
 *                   1 = already triggered (do not repeat).
 */

bool spec_cinderteeth_warden_cmd(CHAR_DATA *ch)
{
    CHAR_DATA *victim;
    char buf[MSL];
    char *spell;
    int sn;

    if (!is_fighting(ch))
    {
        ch->spec_behavior2 = 0;
        return FALSE;
    }

    if ((victim = ch->fighting) == NULL)
        return FALSE;

    /* Emergency Compact: fires once when HP drops below 40% */
    if (ch->spec_behavior2 == 0 && ch->max_hit > 0 &&
        ch->hit * 100 / ch->max_hit < 40)
    {
        ch->spec_behavior2 = 1;
        sprintf(buf,
            "@@W$n straightens with terrible authority — a burning commission\r\n"
            "document blazes in $s hand: '@@yEMERGENCY COMPACT — ARTICLE 7!@@W'@@N");
        act(buf, ch, NULL, NULL, TO_ROOM);
        if ((sn = skill_lookup("high explosive")) >= 0)
            (*skill_table[sn].spell_fun)(sn, ch->level, ch, victim, NULL);
        return TRUE;
    }

    /* Normal tick: 35% chance */
    if (number_percent() > 35)
        return FALSE;

    switch (number_bits(2))
    {
    case 0:
        spell = "dispel magic";
        sprintf(buf,
            "@@W$n raises $s commission seal and invokes the Compact's\r\n"
            "arcane-restriction clause — $N's magical protections are stripped!@@N");
        break;

    case 1:
        spell = "chain lightning";
        sprintf(buf,
            "@@W$n pronounces a warden enforcement citation and fires a\r\n"
            "bolt of punitive lightning into $N!@@N");
        break;

    case 2:
        spell = "flamestrike";
        sprintf(buf,
            "@@W$n reads from the penalty register — $N is listed in\r\n"
            "material violation, and the sentence is flame!@@N");
        break;

    default:
        spell = "harm";
        sprintf(buf,
            "@@W$n levels a skeletal finger at $N — the warden's authority\r\n"
            "tears at $S vitality!@@N");
        break;
    }

    if ((sn = skill_lookup(spell)) < 0)
        return FALSE;

    act(buf, ch, NULL, victim, TO_ROOM);
    (*skill_table[sn].spell_fun)(sn, ch->level, ch, victim, NULL);
    return TRUE;
}
