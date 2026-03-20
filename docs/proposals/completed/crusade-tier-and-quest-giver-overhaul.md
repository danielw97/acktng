# Crusade Tier & Quest Giver Overhaul

Supersedes `require-ai-quest-givers.md` (which will be deleted once this is implemented).

## Problems

1. **Tier ranges are not fixed.** `quest_set_effective_crusade_level_range()` caps the
   maximum of a tier's range at `highest_player_level + 20`. This causes a crusade
   announced as tier 1 to use range `1–71` (for a server with only level-51 mortals)
   instead of the correct full-tier range `1–100`. Tiers should have immutable ranges:
   `1–100`, `101–149`, `150–170` (mob levels extend to 170; player max is 150 but
   tier 3 content spans 150–170).

2. **Quest tier is determined by the quest giver's level.** The manual-crusade path calls
   `quest_tier_from_level(quest_mob->level)` to decide the personality/tier, then fetches
   a quest target inside that tier. This is backwards: the giver is whoever happened to be
   available. Tier should reflect the difficulty of the target, not an unrelated NPC.

3. **Quest givers are not required to be AI-enabled.** Non-dialogue NPCs make poor quest
   givers; the crusade channel Q&A depends on the giver being able to respond.

## Approach

### 1. Make tier ranges immutable — remove level capping

Delete `crusade_level_cap_for_range()`.

Simplify `quest_set_effective_crusade_level_range()` to be a pure passthrough to
`quest_set_crusade_level_range_for_tier()`, ignoring `highest_level_in_range`:

```c
void quest_set_effective_crusade_level_range(sh_int tier, int highest_level_in_range,
                                             int *minimum_level, int *maximum_level)
{
    quest_set_crusade_level_range_for_tier(tier, minimum_level, maximum_level);
}
```

(The signature is kept to avoid touching callers; the parameter becomes unused.)

Tier ranges remain unchanged: `1–100`, `101–149`, `150–170`.

### 2. Derive tier from the quest target, not the giver

**Manual path** (`do_crusade` / `iquest crusade`): replace the giver-level-driven tier
selection with player-pool-based tier selection (matching the auto-quest path):

```
average_level = total_player_levels / player_count
tier = quest_tier_from_level(average_level)
[a, b] = fixed range for that tier
quest_target = get_quest_target(a, b)
quest_personality = quest_tier_from_level(quest_target->level)
quest_mob = get_quest_giver()   ← any AI NPC; level not constrained to tier range
```

Remove:
- `highest_mortal / highest_remortal / highest_adept` tracking in the manual path (no
  longer needed; capping is gone)
- The retry block that re-fetches the giver if its level falls outside `[a, b]`

**Auto-quest path** (`generate_auto_crusade`): already selects a tier from the player
pool. Changes needed:
- Remove `highest_mortal / highest_remortal / highest_adept` variables (only `has_mortal
  / has_remortal / has_adept` are needed)
- Remove `highest_level_for_range`
- Remove the giver-level-range guard (lines 1611–1616): aborts if giver level is outside
  `[a,b]`. With the new design, giver level is irrelevant to the range.
- Set `quest_personality = quest_tier_from_level(quest_target->level)` after target is
  selected (instead of using `selected_range` directly).

### 3. Require `ACT_AI_DIALOGUE` in `get_quest_giver()`; fix selection algorithm

The current probabilistic 2%-pick algorithm fails badly with small pools: with ~23
AI-enabled NPCs the NULL rate is ~63% even when valid candidates exist.

Replace with a two-pass collect-then-pick:

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

Note: `min_level` and `max_level` parameters are removed from the selection criteria
(the giver's level no longer matters). The parameters can be removed from the signature
or left as unused — removing them is cleaner since they appear in the forward declaration
and both call sites.

## Affected Files

| File | Changes |
|------|---------|
| `src/quests/crusade.c` | All of the above |
| `src/tests/test_crusade.c` | Update tests to match new behaviour |
| `docs/proposals/require-ai-quest-givers.md` | Delete (superseded) |

## Updated Test Cases

Tests to **update**:
- `test_effective_range_helper`: effective range now equals tier range (no capping);
  e.g. `quest_set_effective_crusade_level_range(1, 80, …)` returns `1–100` not `1–100`
  (tier 1 was already uncapped in that test), and
  `quest_set_effective_crusade_level_range(2, 120, …)` returns `101–149` not `101–140`
- `test_crusade_level_cap_for_range`: **delete** (function removed)

Tests to **add**:
- `test_effective_range_ignores_player_level`: confirm
  `quest_set_effective_crusade_level_range(1, 40, …)` returns `1–100` (not `1–60`) and
  `quest_set_effective_crusade_level_range(2, 110, …)` returns `101–149` (not `101–130`)

## Trade-offs

| Consideration | Notes |
|--------------|-------|
| Giver level no longer constrains range | Quest givers can be any level. A level-5 guard can announce a crusade for adept-tier mobs. Lore-wise this is fine: the guard is reporting a theft, not fighting anyone. |
| Fewer giver candidates per crusade | Removing the level filter means the full AI NPC pool (~23 NPCs) is always eligible. The collect-then-pick algorithm ensures reliable selection. |
| Fixed tier ranges | `1–100`, `101–149`, `150–170`. Mob levels extend to 170; tier 3 covers all adept-appropriate content. A mortal-only server will always spawn tier-1 targets across the full 1–100 band. |
| `quest_set_effective_crusade_level_range` kept | Signature preserved to minimise caller changes; the body becomes a one-liner. Can be fully removed in a follow-up cleanup. |
