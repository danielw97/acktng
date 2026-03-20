# Design Proposal: Replace "Immortal/Imm" with "Staff" Throughout Codebase

**Date:** 2026-03-20
**Status:** Pending approval
**Scope:** Source code, help files, area files, documentation, data files

---

## Problem

The codebase uses "immortal", "imm", and "imms" as the canonical terms for privileged staff
accounts — players with levels at or above `LEVEL_IMMORTAL` (102). These terms are embedded
throughout identifiers, string literals, help files, and area files. The goal is to
consistently replace all staff-role references with "staff", while leaving genuine in-world
uses of the word "immortal" (thematic lore, NPC descriptions, etc.) untouched.

---

## Approach

The rename is divided into three layers:

1. **Code identifiers** — macros, constants, struct fields, function names
2. **Player-visible strings** — WHO display, channel names, help files, in-game messages
3. **Non-source files** — area files (room descriptions referencing the staff hall), help
   files, data files, docs

Lore/thematic uses of "immortal" — e.g. "immortal architect of the pyramid", "immortality
research", "pharaohs immortal" — are **not** renamed, as they describe a concept in the game
world rather than a staff role.

---

## Section 1: Source Identifiers (src/)

### Macros and constants (`src/headers/config.h`)

| Old | New |
|-----|-----|
| `LEVEL_IMMORTAL` | `LEVEL_STAFF` |
| `MAX_NUM_IMMS` | `MAX_NUM_STAFF` |
| `C_TYPE_IMM` | `C_TYPE_STAFF` |
| `EX_IMMORTAL` | `EX_STAFF` |
| `CHANNEL_IMMTALK` | `CHANNEL_STAFFTALK` |
| `MONITOR_GEN_IMM` | `MONITOR_GEN_STAFF` |
| `MONITOR_CONNECT` | unchanged (not imm-specific) |
| `SHOW_IMMORT` | `SHOW_STAFF` |

### Macro (`src/headers/utils.h`)

| Old | New |
|-----|-----|
| `IS_IMMORTAL(ch)` | `IS_STAFF(ch)` |

### Data fields (`src/headers/ack.h`)

| Old | New |
|-----|-----|
| `imms[MAX_NUM_IMMS]` | `staff[MAX_NUM_STAFF]` |
| `immskll` (PC_DATA) | `staffskll` |

### Functions and commands

| Old | New |
|-----|-----|
| `do_immtalk` | `do_stafftalk` |
| `do_immbrand` | `do_staffbrand` |
| `immtalk` (command string) | `stafftalk` |
| `imtlset` (command string) | `staffset` |
| `immbrand` (command string) | `staffbrand` |

All `C_TYPE_IMM` entries in `interp.c` will be updated to `C_TYPE_STAFF`.
Every use of `IS_IMMORTAL`, `LEVEL_IMMORTAL`, `EX_IMMORTAL`, `CHANNEL_IMMTALK`,
`MONITOR_GEN_IMM`, and `SHOW_IMMORT` across all `.c` files will be updated.

**Not renamed:** `wizify`, `wizbit`, `invis` (wizinvis) — these refer to the wizard-invisibility
mechanic, which is a distinct concept with its own established name.

**Not renamed:** `iquest`, `iwhere`, `iscore` — these are command-name strings where the `i`
prefix means "immortal-restricted" but renaming them would break any existing player muscle
memory. These can be addressed separately if the user wishes.

**Not renamed:** `ROOM_VNUM_HALLS` constant — the constant name is a generic identifier; the
room's in-game *name* and *description* are updated in the area file (see Section 3).

---

## Section 2: Player-Visible Strings in Source

Files affected: `act_info.c`, `act_wiz.c`, `act_comm.c`, `interp.c`, `login.c`, and others.

Changes:
- WHO listing header: `"lImmortals@@R-----"` → `"lStaff@@R---------"`
- Help category label: `"@@yImm"` → `"@@yStaff"`
- Help topic routing: `if (!str_prefix(arg1, "imm"))` → `if (!str_prefix(arg1, "staff"))`
- Monitor channel label: `"IMM_GEN"` → `"STAFF_GEN"`
- `do_stafftalk` channel messages: update "immtalk" → "stafftalk" in all format strings
- `imtlset` / `do_staffset` prompts and help strings: "immortal" → "staff"
- "immortal" in reserved-name list (`login.c`) → add "staff" as reserved name, keep
  "immortal" as reserved as well (both should be blocked)
- `act_wiz.c` comment "Not on other Immortal / Adept players!" → "Not on other Staff / Adept players!"

---

## Section 3: Help Files (help/)

| Old file | Action |
|----------|--------|
| `help/imm` | Rename to `help/staff`; update keywords to `STAFF`; update content |
| `help/immrules` | Rename to `help/staffrules`; update keywords and content |
| `help/imotd` | Rename to `help/staffmotd`; update keywords and content |
| `help/newbie` | Update "Immortals are staff" → "Staff are..." |
| `help/delete` | "Leader or immortal can delete" → "Leader or staff can delete" |
| `help/build_bmotd` | Update "Immortal in charge of building" → "Staff member in charge of building" |
| `help/law` | Update "Law immortal" → "Law staff member" |
| `help/qp` | "immortals creating unique equipment" → "staff creating unique equipment" |
| `help/house` | "contact immortal" → "contact staff" |
| `help/playtesting` | "immortal approval" → "staff approval" |
| `help/pray` | "ALL immortals" → "ALL staff" |
| `help/invasion` | "immortals control" → "staff control"; "immortal players" → "staff players" |
| `help/build_rexit` | `EX_IMMORTAL` → `EX_STAFF`; update description |
| `help/boards` | "Only immortals can write" → "Only staff can write" |
| `help/mana__4` | "immortals monitoring" → "staff monitoring" |
| `help/build_goto` | "immortal's goto" → "staff goto" |
| `help/halls` | "Halls of the Immortals" → "Halls of the Staff" |
| `help/clan` | "immortals via cset" → "staff via cset" |
| `help/rules` | "Respect the Immortals" → "Respect the Staff" |
| `help/motd` | "immortals stepping in" → "staff stepping in" |
| `help/immtalk` (if exists) | Rename to `help/stafftalk`; update content |
| `help/immbrand` (if exists) | Rename to `help/staffbrand`; update content |
| `help/imtlset` (if exists) | Rename to `help/staffset`; update content |

The shelp/ directory will also be checked for any immortal-specific staff help entries and
updated accordingly.

---

## Section 4: Area Files (area/)

Only room names and descriptions that refer to the **staff role** (not lore) are changed.

| File | Change |
|------|--------|
| `area/ceiling.are` | Room vnum 65403: rename "Halls of the Immortals" → "Halls of the Staff"; update room description |
| `area/newschool.are` | "Respect the Immortals (staff)" → "Respect the Staff" |

**Not changed:**
- `northern_pyramid.are` — "immortal architect" is lore describing a character
- `pyramid.are` — "kept pharaohs immortal" is thematic flavor
- `southern_pyramid.are` — "Moon Sovereign — immortal architect" is lore

---

## Section 5: Documentation (docs/)

| File | Change |
|------|--------|
| `docs/data_structures.md` | Update field names `immskll` → `staffskll`, `MAX_NUM_IMMS` → `MAX_NUM_STAFF`, `IS_IMMORTAL` → `IS_STAFF`, `LEVEL_IMMORTAL` → `LEVEL_STAFF` |
| `docs/proposals/completed/` files | Minor reference updates to completed proposals that mention the old terms in context of staff roles |

Lore files under `docs/lore/` that use "immortal" in a world-building sense are not changed.

---

## Section 6: Data Files (data/)

| File | Change |
|------|--------|
| `data/socials.txt` | "I want to be an immortal already" — **not changed** (flavor text/roleplay) |

---

## Affected Files Summary

**Source (require compile/test):**
- `src/headers/config.h`
- `src/headers/utils.h`
- `src/headers/ack.h`
- `src/headers/globals.h`
- `src/interp.c`
- `src/act_info.c`
- `src/act_wiz.c`
- `src/act_comm.c`
- `src/act_move.c`
- `src/fight.c`
- `src/login.c`
- `src/save/save_sysdata.c`
- `src/scheck.c`
- Any other `.c` file using `IS_IMMORTAL`, `LEVEL_IMMORTAL`, `EX_IMMORTAL`, `CHANNEL_IMMTALK`, `MAX_NUM_IMMS`, `C_TYPE_IMM`, `SHOW_IMMORT`, `MONITOR_GEN_IMM`

**Non-source (no compile needed):**
- ~20 help files in `help/`
- Shelp files in `shelp/` referencing staff role
- 2 area files
- `docs/data_structures.md`

---

## Trade-offs

| Decision | Rationale |
|----------|-----------|
| Rename `iquest`/`iwhere`/`iscore` command strings | **Not doing this** — player-facing command names are muscle memory; separate decision |
| Keep `wizify`/`wizbit`/`wizinvis` terms | Wizinvis is a distinct gameplay mechanic with its own established vocabulary |
| Keep `invis` field name | Used for wizard-invisibility level, separate from the staff/mortal concept |
| Block both "immortal" and "staff" as reserved login names | Both should remain reserved to prevent impersonation |
| Lore uses of "immortal" unchanged | In-world thematic language is unrelated to the staff role |

---

## Testing

After implementation:
```sh
cd src && make all
```

This runs lint, format check, full build, unit tests, and integration test. Any broken
identifier references will surface as compile errors.

A grep pass after implementation will verify no unintended `immortal`/`imm` staff-role
references remain.
