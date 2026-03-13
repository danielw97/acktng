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
 * of the oases' slow corruption.  Its attacks draw on anti-solar void-fire,
 * the antithesis of every life-giving ray that falls on the desert above.
 *
 * Combat pattern (mod-12 cycle):
 *   round  1 — absorbs ambient light; taunts with a sand-veil
 *   round  4 — begins drawing in solar energy; oases wither in sympathy
 *   round  5 — WIND-UP WARNING 1: "The Shard eclipses itself, building..."
 *   round  6 — WIND-UP WARNING 2: "The inversion peaks..."
 *   round  7 — STRIKE: void-fire blast against the target
 *   round 10 — oasis-drain pulse: shadow damage aura
 */

static void do_void_fire_strike(CHAR_DATA *ch)
{
    int dam = number_range(14000, 18000);

    calculate_damage(ch, ch->fighting, dam, gsn_holystrike,
                     ELEMENT_FIRE | ELEMENT_SHADOW, TRUE);
}

static void do_oasis_drain(CHAR_DATA *ch)
{
    int dam = number_range(5000, 8000);

    calculate_damage(ch, ch->fighting, dam, gsn_holystrike,
                     ELEMENT_SHADOW, TRUE);
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
            "@@D$n drinks in the torchlight, the shadows in the sanctum\r\n"
            "deepening into an absolute void.@@N");
        act(buf, ch, NULL, NULL, TO_ROOM);
        break;

    case 4:
        sprintf(buf,
            "@@y$n pulses with cold inverted fire — somewhere far above, the\r\n"
            "oases shudder as warmth drains downward into the Shard.@@N");
        act(buf, ch, NULL, NULL, TO_ROOM);
        break;

    case 5:
        sprintf(buf,
            "@@R$n begins coiling stolen sunlight into itself, the air\r\n"
            "scorching cold as the eclipse builds toward detonation!@@N");
        act(buf, ch, NULL, NULL, TO_ROOM);
        break;

    case 6:
        sprintf(buf,
            "@@R$n's eclipse reaches totality — a halo of void-fire crowns\r\n"
            "the Shard, trembling at the threshold of catastrophic release!@@N");
        act(buf, ch, NULL, NULL, TO_ROOM);
        break;

    case 7:
        sprintf(buf,
            "@@RThe Black Sun detonates!  $n unleashes a pillar of void-fire\r\n"
            "straight into $N — the anti-sun burns everything it touches!@@N");
        act(buf, ch, NULL, ch->fighting, TO_ROOM);
        do_void_fire_strike(ch);
        break;

    case 10:
        sprintf(buf,
            "@@D$n exhales a wave of oasis-death, the memory of drowned\r\n"
            "palms and poisoned springs washing over $N!@@N");
        act(buf, ch, NULL, ch->fighting, TO_ROOM);
        do_oasis_drain(ch);
        break;
    }

    return TRUE;
}
