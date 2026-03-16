#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

/*
 * spec_cinderteeth_ventspeaker — Ghost of the Last Ventspeaker (level 163, boss).
 *
 * Lore: The bound spirit of Cairn-Mother Retha An, the last Ventspeaker before
 * the Conclave's arrival, still reading the Western Fold's vent rhythms and
 * issuing prophetic pronouncements.  Her attacks reflect accumulated geological
 * knowledge turned violent: volcanic sympathy, spiritual name-binding, and
 * the weight of the Ashfall Monsoon's memory.
 *
 * Cycling pattern (spec_behavior % 9):
 *   1 — Prophetic curse: applies weaken + curse to target via spells
 *   3 — Name-stone binding: MENTAL+SHADOW single-target blast
 *   5 — Ash Monsoon memory: room-wide PHYSICAL+AIR AOE (the mountain remembers)
 *   7 — Vent communion: earthquake spell + FIRE single-target (volcanic sympathy)
 *   8 — Hollow prophecy: MENTAL harm-style blast (the void behind the prophecy)
 *   (other rounds: no special action)
 */

bool spec_cinderteeth_ventspeaker(CHAR_DATA *ch)
{
    CHAR_DATA *victim;
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
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

    switch (ch->spec_behavior % 9)
    {
    case 1:
        sprintf(buf,
            "@@p$n intones in Old Cindertongue — the vent rhythms have named $N\r\n"
            "as the instrument of $s own ending, and the names take hold!@@N");
        act(buf, ch, NULL, victim, TO_ROOM);
        if ((sn = skill_lookup("weaken")) >= 0)
            (*skill_table[sn].spell_fun)(sn, ch->level, ch, victim, NULL);
        if ((sn = skill_lookup("curse")) >= 0)
            (*skill_table[sn].spell_fun)(sn, ch->level, ch, victim, NULL);
        break;

    case 3:
        {
            int dam = number_range(7000, 10000);
            sprintf(buf,
                "@@p$n raises a name-stone and speaks $N's true name into the vent —\r\n"
                "the sound binds like iron, tearing at $S spirit and shadow!@@N");
            act(buf, ch, NULL, victim, TO_ROOM);
            calculate_damage(ch, victim, dam, gsn_holystrike,
                             ELEMENT_MENTAL | ELEMENT_SHADOW, TRUE);
        }
        break;

    case 5:
        sprintf(buf,
            "@@p$n exhales the memory of the Ashfall Monsoon — three years\r\n"
            "of volcanic winter press down on EVERYONE in the chamber!@@N");
        act(buf, ch, NULL, NULL, TO_ROOM);
        for (vch = ch->in_room->first_person; vch != NULL; vch = vch_next)
        {
            vch_next = vch->next_in_room;
            if (IS_NPC(vch))
                continue;
            calculate_damage(ch, vch, number_range(3000, 5500),
                             gsn_holystrike, ELEMENT_PHYSICAL | ELEMENT_AIR, TRUE);
        }
        break;

    case 7:
        sprintf(buf,
            "@@p$n presses both hands against the volcanic floor and communes\r\n"
            "with the vent — the ground heaves and fire answers $s call!@@N");
        act(buf, ch, NULL, victim, TO_ROOM);
        if ((sn = skill_lookup("earthquake")) >= 0)
            (*skill_table[sn].spell_fun)(sn, ch->level, ch, NULL, NULL);
        {
            int dam = number_range(6000, 9000);
            calculate_damage(ch, victim, dam, gsn_holystrike, ELEMENT_FIRE, TRUE);
        }
        break;

    case 8:
        {
            int dam = number_range(7500, 10500);
            sprintf(buf,
                "@@p$n speaks a prophecy with no future in it — the hollow\r\n"
                "at the center of $s vision strikes $N's mind like a void!@@N");
            act(buf, ch, NULL, victim, TO_ROOM);
            calculate_damage(ch, victim, dam, gsn_holystrike, ELEMENT_MENTAL, TRUE);
        }
        break;
    }

    return TRUE;
}
