# Require AI-Enabled Quest Givers for Crusades

## Problem

The crusade system's `get_quest_giver()` function currently selects any NPC that meets
the level range and basic eligibility criteria. Quest givers have no reliable dialogue
capability — players who ask them questions on the crusade channel get AI-generated
responses, but only because `npc_dialogue_dispatch_crusade()` is called regardless of
whether the NPC has `ACT_AI_DIALOGUE` set. Requiring the `ACT_AI_DIALOGUE` flag ties
the crusade quest giver role to NPCs that are explicitly designed to speak — giving
players a richer, lore-grounded interaction.

## Approach

Two changes are needed:

### 1. Filter: require `ACT_AI_DIALOGUE` in `get_quest_giver()`

Add `!IS_SET(target->act, ACT_AI_DIALOGUE)` to the skip conditions so that only
AI-enabled NPCs can be selected as quest givers.

### 2. Fix the selection algorithm for small pools

The current algorithm uses a probabilistic skip-then-2%-select approach designed for
large NPC populations. With only ~23 AI-enabled NPCs currently loaded, that algorithm
produces a very high NULL rate (each candidate has only 2% chance of being chosen,
so with 23 candidates roughly 63% of calls return NULL even when candidates exist).

Replace the algorithm with a two-pass reservoir approach:

1. Scan `first_char` once, collecting all qualifying NPCs into a fixed-size array
   (capped at 256 entries to bound memory use).
2. Return a uniformly random entry from that array, or NULL if empty.

This guarantees selection whenever any qualifying NPC exists, with uniform
distribution, and has the same O(n) cost.

**Sketch:**

```c
CHAR_DATA *get_quest_giver(int min_level, int max_level)
{
    CHAR_DATA *candidates[256];
    int        count = 0;
    CHAR_DATA *t;

    for (t = first_char; t != NULL; t = t->next)
    {
        if (!IS_NPC(t))                                    continue;
        if (!IS_SET(t->act, ACT_AI_DIALOGUE))              continue;
        if (t->level < min_level || t->level > max_level) continue;
        if (IS_SET(t->in_room->area->flags, AREA_NOSHOW))  continue;
        if (IS_SET(t->act, ACT_SENTINEL))                  continue;
        if (IS_SET(t->act, ACT_PET))                       continue;
        if (IS_SET(t->act, ACT_INVASION))                  continue;
        if (!str_cmp(rev_spec_lookup(t->spec_fun), "spec_stephen")) continue;
        if (!str_cmp(rev_spec_lookup(t->spec_fun), "spec_tax_man")) continue;
        if (count < 256)
            candidates[count++] = t;
    }
    if (count == 0)
        return NULL;
    return candidates[number_range(0, count - 1)];
}
```

## Affected Files

| File | Change |
|------|--------|
| `src/quests/crusade.c` | Replace `get_quest_giver()` body with the two-pass algorithm + `ACT_AI_DIALOGUE` filter |

No other files require changes. Callers already handle NULL returns from
`get_quest_giver()` with a graceful "Failed to find a quest mob" message.

## Trade-offs

| Consideration | Notes |
|--------------|-------|
| Pool size | 23 AI NPCs are currently loaded (22 in `midgaard.are`, 1 in `newschool.are`), spanning levels 5–150. This covers every level bracket the crusade system uses. |
| Fallback | If no AI NPC is in the required level range, `get_quest_giver()` returns NULL and the crusade start fails with the existing error message. This is acceptable — crusades should not launch with a non-speaking quest giver. |
| Non-AI crusade dialogue | `npc_dialogue_dispatch_crusade()` is called regardless of the `ACT_AI_DIALOGUE` flag today. After this change that remains true for the rare case where somehow a non-AI mob ends up as quest giver (e.g. hot-reboot state); no regression. |
| Algorithm change | The 2%-pick approach was presumably intended to produce a "random" selection across the entire NPC list. The two-pass approach achieves the same goal more reliably and more clearly. |

## Unit Test

A unit test in `src/tests/test_crusade.c` should verify:
- `get_quest_giver()` returns NULL when no AI NPCs exist in range
- `get_quest_giver()` returns a valid AI NPC when one exists in range
- `get_quest_giver()` never returns a non-AI NPC

This requires stubbing `first_char` with a small linked list of test mobs.
