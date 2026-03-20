# Proposal: Thematic Crusade Target Objects

## Problem

The six quest objects used by the crusade system (vnums 65446–65451, defined by
`OBJ_VNUM_QUEST_MIN`/`OBJ_VNUM_QUEST_MAX` in `src/headers/config.h`) are all
identical generic items named "a quest token". They carry no flavour, break
immersion when named in crusade messages ("My **a quest token** has been stolen
from me!"), and have no connection to the world's lore.

Additionally, `load_quest_object()` picks uniformly at random across all six
vnums regardless of which tier (wimpy/cool/mean) is running, so the item named
in a tearful child-NPC's plea is indistinguishable from the one a wrathful
demigod-NPC thunders about.

## Approach

### 1. Replace the six area objects with tier-grouped lore items

The six vnums are split into three pairs — two per crusade tier — so the item
selected can match the personality and narrative weight of the quest:

| Tier | Personality | Vnums | Items |
|------|-------------|-------|-------|
| 1 | Wimpy (child-like, tearful) | 65446–65447 | humble personal possessions |
| 2 | Cool (detached, philosophical) | 65448–65449 | scholarly / legal artefacts |
| 3 | Mean (wrathful, dramatic) | 65450–65451 | significant campaign relics |

**Proposed items:**

**Tier 1 — common folk objects**
- **65446** `a worn cloth doll` — a much-mended cloth doll, its button eyes lopsided
  from years of love. Keywords: `doll cloth worn`
- **65447** `a carved wooden idol` — a palm-sized figurine of a local protective
  spirit, smoothed by handling. Keywords: `idol wooden carved`

**Tier 2 — scholarly / institutional artefacts**
- **65448** `a dueling commission` — a folded parchment bearing an official
  Swordsman dueling commission, sealed in wax. Keywords: `commission dueling parchment`
- **65449** `a keeper's survey rod` — a short brass measuring rod etched with
  Measure Script, used by the old Keepers of Measure. Keywords: `rod survey keeper brass`

**Tier 3 — Solar Court campaign relics**
- **65450** `a campaign seal` — a heavy iron disc stamped with Solar Court
  campaign-honor markings, worn smooth at the grip. Keywords: `seal campaign iron solar`
- **65451** `an ashfall shard` — a jagged crystal fragment from the Cinderteeth
  Mountains' Ashfall, still faintly warm to the touch. Keywords: `shard ashfall crystal cinderteeth`

All six retain item type `ITEM_QUEST` (16), wear flags 16384 (TAKE only), and
`ITEM_INVENTORY` (16777216) so they cannot be dropped by players mid-quest.
Long description ("lies here") is cosmetic only since these objects are always
carried, never in a room.

### 2. Add tier-specific vnum defines to `src/headers/config.h`

```c
/* Per-tier crusade target object ranges within the overall quest-item band */
#define OBJ_VNUM_QUEST_T1_MIN 65446
#define OBJ_VNUM_QUEST_T1_MAX 65447
#define OBJ_VNUM_QUEST_T2_MIN 65448
#define OBJ_VNUM_QUEST_T2_MAX 65449
#define OBJ_VNUM_QUEST_T3_MIN 65450
#define OBJ_VNUM_QUEST_T3_MAX 65451
```

`OBJ_VNUM_QUEST_MIN` (65446) and `OBJ_VNUM_QUEST_MAX` (65451) are kept
unchanged so the exclusion filter in `src/quests/commands.c:145` continues to
work without modification.

### 3. Update `load_quest_object` to accept and use the active tier

**`src/quests/crusade.c`**

Change the forward declaration and function signature:

```c
/* before */
OBJ_DATA *load_quest_object args((CHAR_DATA *target));

/* after */
OBJ_DATA *load_quest_object args((CHAR_DATA *target, sh_int tier));
```

Updated function body selects from the matching vnum pair:

```c
OBJ_DATA *load_quest_object(CHAR_DATA *target, sh_int tier)
{
    int vnum_min, vnum_max;

    if (tier == 1)      { vnum_min = OBJ_VNUM_QUEST_T1_MIN; vnum_max = OBJ_VNUM_QUEST_T1_MAX; }
    else if (tier == 2) { vnum_min = OBJ_VNUM_QUEST_T2_MIN; vnum_max = OBJ_VNUM_QUEST_T2_MAX; }
    else                { vnum_min = OBJ_VNUM_QUEST_T3_MIN; vnum_max = OBJ_VNUM_QUEST_T3_MAX; }

    int foo = number_range(vnum_min, vnum_max);
    OBJ_INDEX_DATA *pObj = get_obj_index(foo);

    if (pObj == NULL)
    {
        bug("load_quest_object : Invalid object vnum %d.", foo);
        return NULL;
    }

    OBJ_DATA *object = create_object(pObj, 1);
    obj_to_char(object, target);
    return object;
}
```

Both call sites already have `quest_personality` set before calling
`load_quest_object`, so they become:

```c
/* do_iquest "start" path — quest_personality already set from quest_mob->level */
quest_object = load_quest_object(quest_target, quest_personality);

/* generate_auto_crusade — quest_personality = selected_range */
quest_object = load_quest_object(quest_target, quest_personality);
```

## Affected Files

| File | Change |
|------|--------|
| `area/ceiling.are` | Replace objects 65446–65451 with the six themed items |
| `src/headers/config.h` | Add six `OBJ_VNUM_QUEST_Tn_MIN/MAX` defines |
| `src/quests/crusade.c` | Add `tier` parameter to `load_quest_object`; update forward declaration and both call sites |

No other files require changes. `src/quests/commands.c` uses the overall
`OBJ_VNUM_QUEST_MIN`/`MAX` range which is unaffected. `handler.c`, `death.c`,
`act_obj.c`, and `update.c` all compare by pointer (`obj == quest_object`), not
by vnum or item type, so they are unaffected.

## Trade-offs

- **No unit test impact.** Existing unit tests in `test_crusade.c` test level
  ranges and personality resolution, not object loading. A new test for
  `load_quest_object` tier dispatch would be good to add.
- **Tier fallback.** If `quest_personality` is somehow 0 at the call site (a
  bug in the caller), the `else` branch picks from tier 3 vnums. This is
  consistent with the existing `quest_resolve_crusade_personality` fallback
  logic that defaults to tier 3 for out-of-range personalities.
- **Item type stays `ITEM_QUEST`.** Changing it would have no functional effect
  (the crusade system tracks objects by pointer, and `db.c`'s case for
  `ITEM_QUEST` only controls object loading defaults). Keeping it is the most
  semantically honest choice.
