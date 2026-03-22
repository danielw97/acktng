# Room Specification

**Version:** strict ingestion format. All rules here are hard requirements enforced by the ingestion validator (`src/areaingest.c`). Violations are rejections; there are no warnings.

---

## 1. Section Structure

```
#ROOMS
#<vnum>
<name>~
<description>~
<room_flags> <sector_type>
[zero or more D/E entries]
S
...
#0
```

- Each room is introduced by `#<vnum>` on its own line.
- `<name>` is a tilde-terminated string.
- `<description>` is a tilde-terminated string. Must end with exactly one `\n` before the `~` (see §1.1).
- `<room_flags>` and `<sector_type>` are integers on a single line.
- Room entries consist of zero or more `D` (exit) and `E` (extra description) lines in any order.
- Each room record ends with `S` on its own line.
- The section ends with `#0`.

### 1.1. Description Format Requirements

- The `<description>~` must end with exactly one trailing `\n` immediately before the terminating `~`.
  - Missing trailing newline → rejection.
  - Double newline (`\n\n`) before `~` → rejection.
- The `<description>~` must contain at least 3 sentences of text → rejection if fewer.
- The `<description>~` must be unique to that room. Identical descriptions across two rooms in the same file → rejection.
- Placeholder or procedural naming is forbidden. Names and descriptions like `Room 7`, `Staging Corridor 12`, `Passage 3`, or any equivalent indexed-template pattern → rejection.
- Vnums must not appear in the description text → rejection.

### 1.2. Name Requirements

- `<name>` must be non-empty → rejection.
- Placeholder names (`Room 1`, `Area N`, etc.) → rejection.
- Maximum name length: 80 characters.

---

## 2. Exit Entry (`D`)

```
D<door>
<exit_description>~
<exit_keyword>~
<locks> <key_vnum> <to_room_vnum>
```

- `D` immediately followed by the door direction integer (no space): `D0`, `D1`, ..., `D5`.
- `<exit_description>` is a tilde-terminated string. May be empty (just `~`).
- `<exit_keyword>` is a tilde-terminated string. May be empty (just `~`).
- Both tilde-terminated lines are required. Omitting either is a rejection.
- `<locks> <key_vnum> <to_room_vnum>` is a line of exactly three integers.
- `<to_room_vnum>` must reference a room in this file or already in the database → rejection.

### 2.1. Door Direction Values

| Value | Direction |
|------:|-----------|
| 0 | north |
| 1 | east |
| 2 | south |
| 3 | west |
| 4 | up |
| 5 | down |

Direction outside 0–5 → rejection.

### 2.2. Exit Keyword Rules

- If `<exit_keyword>` is non-empty, it must appear in at least one of: the room's `<description>~`, an `E` extra description keyword in the same room, or an object that spawns in this room via `#RESETS`. → rejection if none of these apply.
- For door-style named exits (exits that are also doors), prefix the keyword with `^` (e.g., `^iron gate`) so movement messaging treats it as a standalone noun phrase.

### 2.3. `<locks>` Bitvector

`<locks>` is a bitvector. Bits are defined in `src/buildtab.c` (`tab_door_types`):

| Bit | Name | Meaning |
|----:|------|---------|
| 1 | `door` | Exit behaves as a door (can open/close/lock) |
| 2 | `closed` | **Runtime state — stripped on save; do not author** |
| 4 | `locked` | **Runtime state — stripped on save; do not author** |
| 8 | `climb` | Requires climbing to traverse |
| 16 | `immortal` | Immortal-only exit |
| 32 | `pickproof` | Cannot be picked |
| 64 | `smashproof` | Cannot be bashed open |
| 128 | `passproof` | Pass-door spell does not work here |
| 82 | `nodetect` | Exit not shown in `exits` command |

**Rejection rules:**
- `closed` (bit 2) or `locked` (bit 4) set in an authored area file → rejection. These are runtime state bits and must not be authored. Initial door state is set via `#RESETS` command `D`.
- Any exit reset to `locked` via `#RESETS D` state 2 must have `door` (bit 1) set in `<locks>` → rejection.
- Any undefined bit set → rejection.

### 2.4. `<key_vnum>` Rules

- Use `-1` when no key exists for the exit.
- If `<key_vnum>` is `>= 0`, the referenced object must be defined in `#OBJECTS` in this file or in the database → rejection.
- If a `D` reset sets this door to state 2 (locked), `<key_vnum>` must be `>= 0` and must reference a valid key object → rejection.

---

## 3. Extra Description Entry (`E`)

```
E
<keyword>~
<description>~
```

- `E` on its own line.
- `<keyword>` is a tilde-terminated string. Must be non-empty → rejection.
- `<description>` is a tilde-terminated string. Must end with exactly one `\n` before `~` → rejection.
- `<keyword>` must appear in the room's main `<description>~`, or in another `E` entry whose keyword chain anchors back to the main description. Unchored extra descriptions → rejection.

---

## 4. Room Record Terminator

Each room ends with `S` on its own line. Any other token in a room body (other than `D` or `E` entries and `S`) is a rejection.

---

## 5. `room_flags` Bitvector

`room_flags` is the first integer on the `<room_flags> <sector_type>` line. Defined by `tab_room_flags` in `src/buildtab.c`:

| Bit | Name | Meaning |
|----:|------|---------|
| 0 | `nada` | No flags |
| 1 | `dark` | Room is always dark |
| 2 | `regen` | Enhanced HP/mana regeneration |
| 4 | `no_mob` | Mobs cannot enter or wander into this room |
| 8 | `indoors` | Indoor room (weather not shown) |
| 16 | `no_magic` | Spells cannot be cast here |
| 32 | `hot` | Room is hot (environmental) |
| 64 | `cold` | Room is cold (environmental) |
| 128 | `pk` | PK-enabled room |
| 82 | `quiet` | Composite: suppresses some ambient messages |
| 512 | `private` | Room is private (limited entry) |
| 315 | `safe` | Composite: safe room (no combat) |
| 840 | `solitary` | Composite: maximum 1 player |
| 4096 | `pet_shop` | Pet shop room |
| 8192 | `no_recall` | Players cannot recall from here |
| 16384 | `no_teleport` | Players cannot teleport to/from here |
| 32768 | `hunt_hunt` | Mobs hunt aggressively here |
| 65536 | `no_bloodwalk` | Bloodwalk travel blocked |
| 131072 | `no_portal` | Portal creation/entry blocked |
| 262144 | `no_repop` | Area resets do not repopulate this room |
| 524288 | `maze` | Room is part of a maze (`ROOM_MAZE`) |

**Rejection rules:**
- Any bit not in the table above → rejection.
- `maze` (`524288`) set on a room → every room in the maze vnum set must also have `maze` set → rejection if any room in the set lacks it.
- Non-maze rooms with looping or non-reciprocal exits → rejection (see §7).

---

## 6. `sector_type` Enum

`sector_type` is the second integer on the `<room_flags> <sector_type>` line. Defined by `tab_sector_types` in `src/buildtab.c`:

| Value | Name |
|------:|------|
| 0 | `nada` (default) |
| 1 | `city` |
| 2 | `field` |
| 3 | `forest` |
| 4 | `hills` |
| 5 | `mountain` |
| 6 | `water_swim` |
| 7 | `water_noswim` |
| 8 | `recall_set` |
| 9 | `air` |
| 10 | `desert` |
| 11 | `inside` |

Value outside 0–11 → rejection.

---

## 7. Directional Traversal Constraints

- Except in maze rooms, exits must be bi-directional: if room A connects to room B via direction X, room B must have the opposite-direction exit back to room A → rejection if the reverse exit is missing (except for cross-area exits where the target area is not in this file).
- For planned cross-area exits where the target is not yet in the database, the exit must be omitted from the area file entirely. Document the planned connection in `docs/world_links.md` as **Planned**.
- Non-maze rooms must not form directional loops (e.g., going east from A → B → C → A) → rejection.
- All rooms in a maze set must have `ROOM_MAZE` set → rejection if any room in the set lacks it.
- Non-`ROOM_MAZE` rooms must not use asymmetric or one-way exits → rejection.

---

## 8. Boss Room Requirement

- Every boss mob (any mob with the `boss` act flag) must be placed in a room flagged `no_mob` → rejection if a boss mob's spawn room in `#RESETS` lacks `no_mob`.
