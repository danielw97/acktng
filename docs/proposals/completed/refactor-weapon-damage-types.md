# Design Proposal: Refactor Weapon Damage Types

## Problem

The weapon damage type table (`tab_weapon_types` in `src/buildtab.c`) contains two names that are inappropriate for a serious fantasy MUD:

- **Type 9: `"grep"`** — a Unix command name, clearly an accidental artefact of early development. Notably, the display function `get_dt_name()` in `skills.c` already renders this type as `"grip"`, creating a silent inconsistency between the builder table and combat output.
- **Type 12: `"suction"`** — labelled `/* kinky */` in the source, tongue-in-cheek and out of place. The default death message block in `damage.c` explicitly names it in a comment: `/* grep, bite, suction */`.

Neither type is currently assigned to any weapon in the area files, so no existing content is broken by these names — but they are unusable for builders because no thematically coherent weapon could plausibly display these attack strings in combat.

---

## Section 1 — Renaming Existing Types

### Type 9: `grep` / `grip` → `rend`

**Current state:**
- `buildtab.c` (builder table): `"grep"`
- `skills.c` `get_dt_name()` (combat display): `"grip"`

These two already disagree. Neither name is ideal:
- `"grep"` is a Unix command with zero fantasy relevance.
- `"grip"` is serviceable but generic and overlaps conceptually with `"crush"` and `"pound"` for bludgeoning weapons.

**Proposed replacement: `rend`**

*Rend* means to tear violently apart — the motion of hooks, chain-blades, serrated edges, spiked flails, or beast-claws designed to rip rather than cut. It is distinct from:
- `slash` / `slice` — clean cutting
- `claw` — natural weapon (NPC/monster flavour)
- `crush` / `pound` — blunt impact

`rend` gives builders a meaningful option for weapons like rusted meat-hooks, barbed chains, or jagged bone-blades consistent with the game's aesthetic.

The existing default death messages in `damage.c` (pulling hearts, snapping spines) suit `rend` perfectly and require no changes.

---

### Type 12: `suction` → `drain`

**Current state:** `"suction"` in both `buildtab.c` and `skills.c`.

**Proposed replacement: `drain`**

The game world supports necromantic and vampiric traditions (necromancer, warlock, dark-artefact lore across Gloamvault, Withered Depths, etc.). A `drain` damage type gives builders a string for life-stealing weapons, soul-siphoning blades, or vampiric implements. It:
- Reads naturally in combat messages ("drains your vitality", "drains the life from…")
- Fits the existing default death messages (pulling heart from ribcage, etc.)
- Provides a distinct identity from all existing physical damage types

---

## Section 2 — New Damage Type

### Type 13: `sear`

The current table ends at 12. The game's geography and faction lore are heavily fire-adjacent (Cinderteeth Mountains, Scorched Wastes, Ember Speakers clan, Khardaan, various fire-elemental areas). Builders have no weapon damage type for heat-based weapons: fire-forged brands, volcanic glass blades, sun-scorched artefacts, or alchemical flame-lances.

**Proposed addition: `sear`**

*Sear* is a sharp, burning contact — branding or scorching rather than the diffuse explosive quality of `blast`. It occupies a distinct niche:
- `blast` — explosion, impact, concussive force (e.g. wands, rune-canons)
- `sear` — concentrated heat contact (e.g. fire-brands, volcanic weapons, sun-cursed blades)

Adding a new type requires:
1. A new entry in `tab_weapon_types[]` in `buildtab.c` (index 13).
2. A new entry appended to `attack_table[]` in `get_dt_name()` in `skills.c`.
3. New death messages for case 13 in `death_message()` in `damage.c`.

The death messages for `sear` would describe incineration, charring, and immolation to match the tone of existing death message blocks.

---

## Section 3 — Affected Files

| File | Change |
|------|--------|
| `src/buildtab.c` | Rename index 9 from `"grep"` to `"rend"`; rename index 12 from `"suction"` to `"drain"`; add index 13 `"sear"` |
| `src/skills.c` | Rename index 9 in `attack_table[]` from `"grip"` to `"rend"`; rename index 12 from `"suction"` to `"drain"`; append `"sear"` |
| `src/damage.c` | Update comment on default death message block; add case 13 (`sear`) death messages |
| `docs/object_spec.md` | Update the `ITEM_WEAPON` value table to reflect new names and add type 13 |
| Unit tests | Add/update tests for `get_dt_name()` and `death_message()` if applicable |

No area files require changes (types 9 and 12 are not assigned to any current weapon objects).

---

## Section 4 — Trade-offs and Risks

**Renaming types 9 and 12:**
- Zero impact on existing area content — neither type is used in any `.are` file.
- Builder tool displays will immediately show the corrected names.
- No save-file impact (damage types are not stored in player saves).

**Adding type 13 (`sear`):**
- Grows the table by one entry. The `attack_table` array size is computed dynamically via `sizeof`, so no size constant needs updating.
- Existing area content is unaffected (no object currently has value[3] = 13).
- A new death message `case` must be written to avoid the default fallback.
- If the `sear` type is deemed out of scope for this pass, types 9 and 12 can be fixed independently without adding 13.

**The `"grip"` inconsistency:**
- `buildtab.c` used `"grep"` while `skills.c` used `"grip"`. Both become `"rend"`, resolving the inconsistency cleanly.
