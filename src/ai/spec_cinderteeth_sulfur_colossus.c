#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

/*
 * spec_cinderteeth_sulfur_colossus — Glasswash Sulfur Fan Colossus (level 156, mini-boss).
 *
 * Lore: A geological construct — centuries of sulfur crystal deposition in the
 * Yellow Garden that achieved animate coherence.  Attacks by slamming massive
 * crystalline limbs (earth damage + seismic disruption) and by opening body
 * fissures to release toxic volcanic gas.  The culminating attack ignites
 * accumulated body-gas with subterranean heat for a combined toxic fire burst.
 *
 * Cycling pattern (spec_behavior % 6):
 *   1 — Sulfur stomp: EARTH crush + earthquake spell
 *   3 — Gas fissure eruption: room-wide volcanic gas breath
 *   5 — Fan collapse: FIRE+POISON primary target + secondary POISON AOE on others
 */

bool spec_cinderteeth_sulfur_colossus(CHAR_DATA *ch)
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

    switch (ch->spec_behavior % 6)
    {
    case 1:
        {
            int dam = number_range(5000, 8000);
            sprintf(buf,
                "@@G$n raises a crystalline limb and drives it into the floor —\r\n"
                "the impact shatters sulfur shards into $N in a grinding tremor!@@N");
            act(buf, ch, NULL, victim, TO_ROOM);
            calculate_damage(ch, victim, dam, gsn_holystrike, ELEMENT_EARTH, TRUE);
            if ((sn = skill_lookup("earthquake")) >= 0)
                (*skill_table[sn].spell_fun)(sn, ch->level, ch, NULL, NULL);
        }
        break;

    case 3:
        sprintf(buf,
            "@@G$n's crystalline carapace cracks along a dozen fault lines —\r\n"
            "choking volcanic gas pours from the fissures and floods the chamber!@@N");
        act(buf, ch, NULL, NULL, TO_ROOM);
        if ((sn = skill_lookup("gas breath")) >= 0)
            (*skill_table[sn].spell_fun)(sn, ch->level, ch, NULL, NULL);
        break;

    case 5:
        {
            int dam = number_range(5500, 8500);
            sprintf(buf,
                "@@G$n's body-gas ignites from the geothermal heat below —\r\n"
                "a searing burst of sulfur-fire engulfs $N in toxic flame!@@N");
            act(buf, ch, NULL, victim, TO_ROOM);
            calculate_damage(ch, victim, dam, gsn_holystrike,
                             ELEMENT_FIRE | ELEMENT_POISON, TRUE);
            /* Secondary: toxic fallout on everyone else in room */
            for (vch = ch->in_room->first_person; vch != NULL; vch = vch_next)
            {
                vch_next = vch->next_in_room;
                if (IS_NPC(vch) || vch == victim)
                    continue;
                calculate_damage(ch, vch, number_range(2000, 3500),
                                 gsn_holystrike, ELEMENT_POISON, TRUE);
            }
        }
        break;
    }

    return TRUE;
}
