#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

/*
 * The Black Sun Shard — pyramid superboss, level 170.
 *
 * Lore: A fragment of Ra's corrupted dark aspect, imprisoned in the deepest
 * chamber beneath the pyramid long before the structure was built above it.
 * It inverts solar energy, consuming light and warmth, and is the true source
 * of the oases' slow corruption.
 *
 * Combat pattern (mod-12 cycle):
 *   round  1 — WIND-UP 1: "The Shard coils stolen sunlight..."
 *   round  2 — STRIKE: void-fire blast against primary target (single-target)
 *   round  5 — atmospheric warning (oasis corruption pulse incoming)
 *   round  7 — WIND-UP 2: "The sanctum fills with void-cold oasis-death..."
 *   round  9 — OASIS DRAIN: AOE shadow pulse hitting every enemy in the room
 */

static void do_void_fire_strike(CHAR_DATA *ch)
{
    int dam = number_range(14000, 18000);

    calculate_damage(ch, ch->fighting, dam, gsn_holystrike,
                     ELEMENT_FIRE | ELEMENT_SHADOW, TRUE);
}

static void do_oasis_drain_aoe(CHAR_DATA *ch)
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    int dam;

    for (vch = ch->in_room->first_person; vch != NULL; vch = vch_next)
    {
        vch_next = vch->next_in_room;

        if (IS_NPC(vch))
            continue;

        dam = number_range(5000, 8000);
        calculate_damage(ch, vch, dam, gsn_holystrike, ELEMENT_SHADOW, TRUE);
    }
}

bool spec_pyramid_black_sun_shard(CHAR_DATA *ch)
{
    char buf[MSL];

    if (!is_fighting(ch))
    {
        ch->spec_behavior = 0;
        return FALSE;
    }

    ch->spec_behavior++;

    switch (ch->spec_behavior % 12)
    {
    case 1:
        sprintf(buf,
            "@@R$n begins coiling stolen sunlight into itself — the air turns\r\n"
            "scorching cold as the eclipse builds toward detonation!@@N");
        act(buf, ch, NULL, NULL, TO_ROOM);
        break;

    case 2:
        sprintf(buf,
            "@@RThe Black Sun detonates!  $n unleashes a pillar of void-fire\r\n"
            "straight into $N — the anti-sun burns everything it touches!@@N");
        act(buf, ch, NULL, ch->fighting, TO_ROOM);
        do_void_fire_strike(ch);
        break;

    case 5:
        sprintf(buf,
            "@@D$n pulses with cold inverted fire — somewhere far above, the\r\n"
            "oases shudder as warmth drains downward into the Shard.@@N");
        act(buf, ch, NULL, NULL, TO_ROOM);
        break;

    case 7:
        sprintf(buf,
            "@@D$n's void deepens — the sanctum fills with the stench of\r\n"
            "poisoned springs and drowned palms.  Something vast is exhaling.@@N");
        act(buf, ch, NULL, NULL, TO_ROOM);
        break;

    case 9:
        sprintf(buf,
            "@@D$n releases a wave of oasis-death across the entire sanctum —\r\n"
            "the memory of every corrupted oasis washing over all present!@@N");
        act(buf, ch, NULL, NULL, TO_ROOM);
        do_oasis_drain_aoe(ch);
        break;
    }

    return TRUE;
}
