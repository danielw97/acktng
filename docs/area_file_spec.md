# Area File Specification

**Version:** YAML directory format. All rules here are hard requirements enforced by the ingestion
validator (`src/areaingest.c`). Violations are rejections; there are no warnings. A directory either
passes all rules and is imported, or it fails with an error file and is moved to
`area/incoming/failed/`.

---

## I. Sections

### I.1. Directory Structure

A new area is submitted as a **directory** placed inside `area/incoming/`. The directory name must
match the area's `keyword` field exactly. Each file within the directory covers one concern.

```
area/incoming/
  <keyword>/
    area.yaml       ← required
    rooms.yaml      ← required
    mobs.yaml       ← optional
    objects.yaml    ← optional
    shops.yaml      ← optional
    resets.yaml     ← optional
    specials.yaml   ← optional
    objfuns.yaml    ← optional
```

The directory name must be lowercase, contain only `a–z`, `0–9`, and `_`, and must match the
`keyword` field inside `area.yaml` exactly → rejection if they differ.

No files other than those listed above may be present in the directory → rejection if any
unrecognized file is found.

### I.2. Required and Optional Files

| File | Required | Contents |
|------|----------|----------|
| `area.yaml` | Yes | Area header: name, vnum range, level range, flags, metadata |
| `rooms.yaml` | Yes | Room definitions, exits, extra descriptions |
| `mobs.yaml` | No | Mobile definitions, loot tables, AI prompts, scripts |
| `objects.yaml` | No | Object definitions, value arrays, affects, extra descriptions |
| `shops.yaml` | No | Shop keepers and trade configuration |
| `resets.yaml` | No | Reset commands: mob/object spawns, door states, equip |
| `specials.yaml` | No | Mob special function assignments |
| `objfuns.yaml` | No | Object function assignments |

If `rooms.yaml` is absent the directory is rejected immediately without reading any other file.

If `resets.yaml` is present, any mob or object vnums it references must also be present in
`mobs.yaml` or `objects.yaml` in this directory, or already exist in the database → rejection
if a referenced vnum cannot be resolved.

### I.3. Operations

Three operations are defined. The operation is determined by context, not by a separate
command file.

#### Import (new area)

Drop a directory into `area/incoming/`. The area's `keyword` must not already exist in the
database. The server detects the directory on the next `PULSE_AREA` tick (or immediately
when `areaingest run <keyword>` is issued), validates all files, imports them into the
database in a single transaction, hot-loads the area into the live world, and deletes the
directory.

If `update_existing` is absent or `false` and the keyword already exists → rejection with
message naming the conflict.

#### Update (replace existing area)

Same as import, but `update_existing: true` must be set in `area.yaml`. The area's `keyword`
must already exist in the database. On success, all existing database rows for that area
(rooms, exits, extra descriptions, mobiles, objects, affects, shops, resets, specials,
objfuns) are deleted and replaced with the contents of the submitted directory. The area is
hot-reloaded into the running world.

If `update_existing: true` is set but the keyword does not exist → rejection.

Update is an all-or-nothing replace. Partial updates (submitting only `rooms.yaml` to patch
rooms while leaving mobs unchanged) are not supported. Submit the complete set of files each
time.

#### Export

Export is initiated via the `areaingest export <keyword>` wizard command or
`db_to_files --yaml --area <keyword>`. The server reads all rows for the named area from the
database and writes a complete directory to `area/export/<keyword>/`, creating all eight
files regardless of whether optional sections contain any rows. Empty optional files contain
only the top-level key with an empty list (e.g., `mobs: []`).

Exported directories pass ingestion validation without modification (round-trip fidelity is a
hard requirement of the export tool).

### I.4. YAML Conventions Used Throughout This Spec

- All files are UTF-8, Unix line endings (`\n`). `\r\n` is stripped on read; bare `\r` is
  a rejection. Null bytes are a rejection.
- YAML 1.2 is the target dialect. The parser must handle block scalars (`|`), flow sequences
  (`[a, b, c]`), multi-line scalars, and quoted strings.
- YAML comments (`#`) are permitted in authored files and are stripped on parse.
- All string flags are **lowercase names** from the tables in §VI. Integers are never
  accepted in flag fields → rejection if an integer appears where a flag name list is expected.
- Enum fields (single string values such as `sector`, `sex`, `item_type`) accept only the
  exact lowercase names from the corresponding table. Any unrecognized value → rejection.
- `null` and the empty string `""` are equivalent for optional text fields.
- Integer fields must be valid signed 32-bit integers unless otherwise noted. Out-of-range
  values → rejection.
- Block scalars for multi-line text (`|`) preserve newlines. The trailing newline added by
  YAML's block scalar is the required trailing `\n` for description fields. A description
  that, after parsing, does not end with exactly one `\n` → rejection.
- List fields that are absent from a file are treated as empty lists, not as a rejection,
  unless the field is marked required.

### I.5. Vnum Constraints

All vnum constraints are checked before the database transaction opens.

- Every vnum in all eight files must fall within `[vnum_min, vnum_max]` from `area.yaml`
  (inclusive) → rejection if any vnum falls outside the range.
- The `[vnum_min, vnum_max]` range must not overlap any area already in the database.
  Overlap is tested as `new_min <= existing_max AND new_max >= existing_min` → rejection
  naming the conflicting area. (For updates, the existing area being updated is excluded
  from overlap checking.)
- `vnum_max - vnum_min` must be `>= 9` → rejection if the range is too narrow.
- Within each file, vnums must appear in ascending order → rejection if out of order.
- Cross-type vnum overlap (same integer used as a room vnum and a mob vnum) is permitted.
- No vnum may duplicate an existing vnum of the same entity type in the database → rejection
  naming the conflict. (For updates, the area's own existing vnums are excluded.)

### I.6. Staging and Failure Paths

```
area/incoming/                ← author drops <keyword>/ directory here
area/incoming/failed/         ← failed ingestions moved here
area/incoming/failed/<keyword>.error  ← error report for a failed ingestion
area/export/                  ← export output root
area/export/<keyword>/        ← exported area directory
```

Both `area/incoming/` and `area/incoming/failed/` are created at server boot if absent.
Both are gitignored.

On failure:
- The directory is moved to `area/incoming/failed/<keyword>/`.
- A companion `area/incoming/failed/<keyword>.error` is written containing the phase that
  failed, the exact error message, the filename and line number (for parse failures), and a
  timestamp.
- The database transaction is rolled back. No partial data is left.
- The in-memory world is not modified.

### I.7. Canonical Section Order and File Encoding

Files within the directory may be submitted in any order; the server reads them by name.

Each YAML file has a single top-level key matching its filename without extension
(`area`, `rooms`, `mobiles`, `objects`, `shops`, `resets`, `specials`, `objfuns`). The value
under that key is either a mapping (for `area.yaml`) or a sequence of mappings (for all
others). A file whose top-level key does not match → rejection.

Maximum file size per file: 4 MB → rejection. Maximum total directory size: 16 MB → rejection.

---

*Sections III through VI follow: flag and enum reference tables (§III), content validation rules
(§IV), cross-section consistency rules (§V), and ingestion pipeline mechanics (§VI).*

---

## II. Per-File Schemas

### II.1. `area.yaml`

Top-level key: `area`. Value: a single mapping (not a sequence).

```yaml
area:
  keyword:        midgaard             # required — string
  name:           "@@WMidgaard City Center@@N"  # required — string
  quality:        16                   # required — integer 1–100
  level_min:      1                    # required — integer 1–155
  level_max:      145                  # required — integer 1–155
  npop_max:       100                  # required — integer >= 1
  vnum_min:       1100                 # required — integer
  vnum_max:       1299                 # required — integer
  flags:          0                    # required — integer (area flags bitvector; use 0)
  extra_flags:    0                    # required — integer (reserved; use 0)
  ai_summary:     "Midgaard's lamplit streets bustle with merchants, guards, and pilgrims."
                                       # required — plain string (no color codes), 1–255 chars
  open_to:        [all]                # required — list of one or more strings; see §II.1.1
  race_restrict:  [all]                # required — list; see §II.1.1
  world_restrict: [all]                # required — list; see §II.1.1
  music:          midgaard.mp3         # optional — filename string or null
  update_existing: false               # optional — boolean; default false
```

#### II.1.1. Field Definitions

**`keyword`** — Unique identifier for the area. Lowercase, `a–z 0–9 _` only, 1–32 characters.
Must match the directory name exactly → rejection if they differ.

**`name`** — Display name shown in the area list and room prompts. May contain color codes
(`@@W`, `@@N`, etc.). Non-empty, max 80 characters.

**`quality`** — Builder quality rating. Integer `1–100`. Used internally for area tracking.
No in-game effect.

**`level_min`** / **`level_max`** — Intended player level range for this area. Both
`1–155`. `level_min <= level_max` → rejection if reversed.

**`npop_max`** — Maximum number of times per reset tick the area may repopulate. Integer
`>= 1`.

**`vnum_min`** / **`vnum_max`** — Inclusive vnum range for all entities in this area.
`vnum_min <= vnum_max`. `vnum_max - vnum_min >= 9` → rejection if range is too narrow.
Overlap with any existing area → rejection.

**`flags`** — Area flags bitvector. Currently reserved; must be `0` → rejection if non-zero.

**`extra_flags`** — Extended area flags. Currently reserved; must be `0` → rejection if
non-zero.

**`ai_summary`** — One-sentence plain-text description used by the NPC dialogue AI as
world-context for this area. No color codes. 10–255 characters → rejection outside range.

**`open_to`** / **`race_restrict`** / **`world_restrict`** — Access control lists.
Each is a YAML sequence of strings. The only currently valid value is `all` (no
restriction). Future values reserved. Unknown string → rejection.

**`music`** — Filename (basename only, no path) of the background music track for this area,
or `null`/omitted. If present, must match `/^[a-z0-9_\-]+\.(mp3|ogg|wav)$/` → rejection
if format does not match.

**`update_existing`** — Boolean. `false` (default) for new areas; `true` for area-replace
updates. See §I.3.

---

### II.2. `rooms.yaml`

Top-level key: `rooms`. Value: a sequence of room mappings, ascending by `vnum`.

```yaml
rooms:
  - vnum:        1100
    name:        "@@WNorthwestern Watch-Post@@N"
    description: |
      @@WMidgaard's@@N northwestern corner is anchored by a squat sandstone watch-post
      whose @@y lantern@@N brackets burn amber on iron hooks at each cardinal corner.
      The wide intersection was laid out during the @@yLantern Reform@@N era to
      accommodate patrol-wagon turns, and the worn flagstones carry faded chalk lines
      from successive watch rotations. A limestone water-trough occupies the curb corner
      where porters and off-duty guards pause to refill their skins.
    flags:   []
    sector:  city
    exits:
      - direction:   east
        description: ""
        keywords:    ""
        locks:       []
        key_vnum:    -1
        to_vnum:     1101
      - direction:   south
        description: ""
        keywords:    ""
        locks:       []
        key_vnum:    -1
        to_vnum:     1120
    extra_descs:
      - keywords:    "water-trough trough"
        description: |
          A limestone trough worn smooth at the lip by years of use.
```

#### II.2.1. Room Fields

**`vnum`** — Integer. Must fall within `[vnum_min, vnum_max]`. Ascending order required.

**`name`** — Non-empty string, max 80 characters. May contain color codes. Placeholder names
(`Room 1`, `Area N`, `Staging Corridor 12`, etc.) → rejection. Must not be all-uppercase.

**`description`** — Block scalar. After YAML parsing, must end with exactly one `\n` →
rejection. Must contain at least 3 sentences. Must be unique within the file (no two rooms
may share an identical description). Vnums must not appear in the text.

**`flags`** — Sequence of string flag names from the `room_flags` table (§III.1). May be
empty list `[]`. Integer values → rejection.

**`sector`** — String enum from the sector-type table (§III.2). Required.

**`exits`** — Sequence of exit mappings. May be empty. Each exit:

| Field | Type | Notes |
|-------|------|-------|
| `direction` | string | One of: `north east south west up down` |
| `description` | string | May be empty string `""` |
| `keywords` | string | May be empty string `""`. Space-separated keyword list. |
| `locks` | list of strings | Flag names from the door-lock table (§III.3). `closed` and `locked` → rejection. |
| `key_vnum` | integer | `-1` if no key. If `>= 0`, must reference a valid key object. |
| `to_vnum` | integer | Target room vnum. Must exist in this file or the database. |

Each direction may appear at most once per room → rejection if duplicated.

**`extra_descs`** — Sequence of extra-description mappings. Each:

| Field | Type | Notes |
|-------|------|-------|
| `keywords` | string | Space-separated keywords. Non-empty. Must appear in the room's `description` or in another extra-desc keyword chain in this room. |
| `description` | string | Block scalar. Must end with exactly one `\n`. Non-empty. |

---

### II.3. `mobs.yaml`

Top-level key: `mobs`. Value: a sequence of mob mappings, ascending by `vnum`.

```yaml
mobs:
  - vnum:         1100
    player_name:  "guard city warrior"
    short_descr:  "a city guard"
    long_descr:   "A city guard stands here, watching the intersection."
    description: |
      A well-armoured guard wearing the livery of the city watch. Her expression is
      alert but not hostile; she is here to deter trouble, not to start it.
    act:          [is_npc, sentinel, stay_area]
    affected_by:  []
    alignment:    500
    level:        30
    sex:          male
    hp_mod:       0
    ac_mod:       0
    hr_mod:       0
    dr_mod:       0
    class_line:                   # optional; the '!' extension line
      class:    0
      clan:     0
      race:     0
      position: 7
      skills:   [2_attack, dodge]
      cast:     []
      def:      [nada]
    element_line:                 # optional; the '|' extension line
      strong_magic: []
      weak_magic:   []
      race_mods:    0
      resist:       [physical]
      suscept:      []
    combat_line:                  # optional; the '+' extension line
      spellpower_mod: 0
      crit_mod:       0
      crit_mult_mod:  0
      spell_crit_mod: 0
      spell_mult_mod: 0
      parry_mod:      0
      dodge_mod:      0
      block_mod:      0
      pierce_mod:     0
    loot:                         # optional; the 'l'/'L' extension lines (both or neither)
      loot_amount: 100
      slots:
        - vnum:   1200
          chance: 50
        - vnum:   1201
          chance: 50
    ai_line:                      # optional; the 'a' extension line
      ai_knowledge: 0
      accent:       0
      prompt:       "You are a city guard of Midgaard."
    lore_flags: []                # optional; the '^' extension line. list of flag name strings.
    scripts:                      # optional; list of inline script blocks
      - trigger:  greet_prog
        args:     "100"
        commands: |
          say Welcome to Midgaard, traveller.
```

#### II.3.1. Mob Core Fields

**`vnum`** — Integer. Must fall within `[vnum_min, vnum_max]`. Ascending order required.

**`player_name`** — Keyword list, all lowercase, space-separated. Non-empty. No vnums.

**`short_descr`** — Short description. Non-empty. Must not begin with uppercase. No vnums.

**`long_descr`** — Exactly one line of text (no embedded newlines). Non-empty. No vnums.
The trailing `\n` is supplied by YAML; do not include it in the value. Exact duplicates
within the file → rejection.

**`description`** — Block scalar. Must end with exactly one `\n`. 1–8 sentences. No vnums.
No exact duplicates within the file.

**`act`** — Sequence of act-flag name strings (§III.4). `is_npc` must be present →
rejection if missing. `stay_area` must be present → rejection if missing. `invasion` →
rejection. `day_only` and `night_only` together → rejection.

**`affected_by`** — Sequence of affect-flag name strings (§III.5). May be empty.

**`alignment`** — Integer `[-1000, 1000]`.

**`level`** — Integer `1–155`.

**`sex`** — String enum: `neutral`, `male`, `female`.

**`hp_mod`** / **`ac_mod`** / **`hr_mod`** / **`dr_mod`** — Signed integers within the
ranges defined in `mob_spec.md` §2.7.

#### II.3.2. `class_line` (optional)

Maps directly to the `!` extension. All fields required if the mapping is present:

| Field | Type | Constraint |
|-------|------|------------|
| `class` | integer | `>= 0` |
| `clan` | integer | `>= 0` |
| `race` | integer | `>= 0` |
| `position` | integer | `0–9`; always stored as 7 at runtime. Use `7`. |
| `skills` | list of strings | Flag names from §III.6 |
| `cast` | list of strings | Flag names from §III.7 |
| `def` | list of strings | Flag names from §III.8; must include exactly one of `nada` or one or more heal spells |

#### II.3.3. `element_line` (optional)

Maps to the `|` extension:

| Field | Type | Constraint |
|-------|------|------------|
| `strong_magic` | list of strings | Element flag names (§III.9). No overlap with `weak_magic`. |
| `weak_magic` | list of strings | Element flag names. No overlap with `strong_magic`. |
| `race_mods` | integer | `>= 0` |
| `resist` | list of strings | Element flag names. No overlap with `suscept`. |
| `suscept` | list of strings | Element flag names. No overlap with `resist`. |

#### II.3.4. `combat_line` (optional)

Maps to the `+` extension. Nine signed integers, all `[-500, 500]`.

#### II.3.5. `loot` (optional)

Maps to the `l`/`L` extension pair. Both present or both absent.

**`loot_amount`** — Integer `>= 0`. Semantics: `50` = 50% chance one item; `100` = one
guaranteed; `200` = two guaranteed; etc.

**`slots`** — Sequence of up to 9 items:

| Field | Type | Constraint |
|-------|------|------------|
| `vnum` | integer | Must reference an object with `item_loot` in `extra_flags`. Boss mob loot vnums must also have `item_boss`. |
| `chance` | integer | `0–100`. Sum of all chances across the sequence `<= 100`. |

Slots beyond the 9th → rejection. Sum of `chance` values `> 100` → rejection.

#### II.3.6. `ai_line` (optional)

Maps to the `a` extension:

| Field | Type | Constraint |
|-------|------|------------|
| `ai_knowledge` | integer | `>= 0` |
| `accent` | integer | `>= 0` |
| `prompt` | string | Non-empty string; the NPC dialogue system prompt. |

#### II.3.7. `lore_flags` (optional)

List of lore-flag name strings. See §III.10. May be empty list or omitted.

#### II.3.8. `scripts` (optional)

Sequence of inline mob programs. Each:

| Field | Type | Constraint |
|-------|------|------------|
| `trigger` | string | One of the valid trigger names listed in §III.11. |
| `args` | string | Argument string (may be empty `""`). |
| `commands` | string | Block scalar of MUD script commands. Non-empty. |

---

### II.4. `objects.yaml`

Top-level key: `objects`. Value: a sequence of object mappings, ascending by `vnum`.

```yaml
objects:
  - vnum:        1200
    name:        "blade black coral weapon"
    short_descr: "a blade of black coral"
    description: |
      The blade is carved from a single length of black coral harvested from the
      sunless deeps beneath the Saltglass Reach.
    item_type:   weapon
    extra_flags: [item_magic, item_loot, item_boss]
    wear_flags:  [hold, take]
    item_apply:  [nada]
    values:      [0, 0, 0, 3, 0, 0, 0, 0, 0, 0]
    weight:      6
    level:       80            # optional; the 'L' trailing entry
    affects:                   # optional; 'A' trailing entries
      - location: apply_str
        modifier: 0
    extra_descs:               # optional; 'E' trailing entries
      - keywords:    "blade coral"
        description: |
          The black coral has been polished to a mirror finish.
```

#### II.4.1. Object Fields

**`vnum`** — Integer. Must fall within `[vnum_min, vnum_max]`. Ascending order.

**`name`** — Keyword list. Lowercase only. Non-empty. Unique within the file. No vnums.

**`short_descr`** — Must not begin with uppercase. Non-empty. No vnums.

**`description`** — Block scalar. Must end with exactly one `\n`. Non-empty. No vnums.

**`item_type`** — String enum from §III.12. `corpse_npc` and `corpse_pc` → rejection.

**`extra_flags`** — List of extra-flag name strings from §III.13. `item_generated` →
rejection. `item_lifestealer` without `item_anti_good` → rejection.

**`wear_flags`** — List of wear-flag name strings from §III.14. `take` must be present →
rejection. `clan_colors` or `invasion_emblem` → rejection.

**`item_apply`** — List of item-apply flag name strings from §III.15. `nada` is the
baseline; include it when no other flags are set.

**`values`** — List of exactly 10 integers. Unused slots must be `0`. Semantics are
`item_type`-dependent (see `object_spec.md` §8).

**`weight`** — Integer `1–15`. Encodes archetype: `1–5` caster, `6–10` melee, `11–15`
tank. Must be consistent with object identity.

**`level`** (optional) — Integer `1–105`. Sets effective item level for runtime stat
generation.

**`affects`** (optional) — List of affect entries, each:

| Field | Type | Constraint |
|-------|------|------------|
| `location` | string | `APPLY_*` name from §III.16 |
| `modifier` | integer | Must be `0`. Stats are runtime-generated; non-zero → rejection. |

**`extra_descs`** (optional) — List of extra-description entries, each:

| Field | Type | Notes |
|-------|------|-------|
| `keywords` | string | Non-empty. |
| `description` | string | Block scalar ending with exactly one `\n`. |

---

### II.5. `shops.yaml`

Top-level key: `shops`. Value: a sequence of shop mappings.

```yaml
shops:
  - keeper_vnum:  1113
    buy_types:    [armor]
    sell_profit:  100
    buy_profit:   120
    open_hour:    0
    close_hour:   24
```

#### II.5.1. Shop Fields

**`keeper_vnum`** — Integer. Must reference a mob defined in `mobs.yaml` in this directory
or already in the database → rejection if not found.

**`buy_types`** — List of 1–5 item-type name strings from §III.12. Items of these types
are traded by this shop. Use `[]` for no buying preference (shop sells only). Actually, a
shop with an empty `buy_types` will not buy anything from players; use `null` entries to
leave a slot empty if needed. Maximum 5 entries → rejection if more than 5.

**`sell_profit`** — Integer `50–300`. Percentage of base value the shop charges players
when selling. `100` = sell at face value.

**`buy_profit`** — Integer `50–300`. Percentage of base value the shop pays players when
buying. `buy_profit >= sell_profit` → rejection (shop must buy low, sell high or equal).

**`open_hour`** / **`close_hour`** — Integers `0–24`. `open_hour < close_hour` → rejection.
`open_hour = 0` and `close_hour = 24` means the shop is always open.

---

### II.6. `resets.yaml`

Top-level key: `resets`. Value: a sequence of reset command mappings, in the order they
should execute. Order matters: `G` and `E` commands apply to the most recently spawned mob.

```yaml
resets:
  - cmd: M
    mob_vnum:  1100
    max_count: 20
    room_vnum: 1120

  - cmd: G
    obj_vnum:  1105
    max_count: 1

  - cmd: E
    obj_vnum:  1106
    max_count: 1
    wear_loc:  body

  - cmd: O
    obj_vnum:  1200
    max_count: 0
    room_vnum: 1150

  - cmd: D
    room_vnum: 1104
    direction: down
    state:     closed

  - cmd: P
    obj_vnum:       1201
    max_count:      0
    container_vnum: 1200
```

#### II.6.1. `cmd: M` — Spawn Mob

| Field | Type | Notes |
|-------|------|-------|
| `mob_vnum` | integer | Must reference a mob in this file or the database. |
| `max_count` | integer | `>= 1`. Maximum number of this mob alive in the world at reset time. |
| `room_vnum` | integer | Room to spawn in. Must exist in this file or the database. |

#### II.6.2. `cmd: G` — Give Object to Last-Spawned Mob

| Field | Type | Notes |
|-------|------|-------|
| `obj_vnum` | integer | Object to give. Must exist in this file or the database. |
| `max_count` | integer | `>= 0`. `0` = no limit. |

`G` must follow an `M` command (possibly with intervening `G` or `E` commands) → rejection
if `G` appears before any `M` in the reset list.

#### II.6.3. `cmd: E` — Equip Object on Last-Spawned Mob

| Field | Type | Notes |
|-------|------|-------|
| `obj_vnum` | integer | Object to equip. Must exist in this file or the database. |
| `max_count` | integer | `>= 0`. |
| `wear_loc` | string | Wear-location name from §III.17. `clan_colors` and `invasion_emblem` → rejection. |

Must follow an `M` (possibly with intervening `G`/`E`) → rejection otherwise.

#### II.6.4. `cmd: O` — Place Object in Room

| Field | Type | Notes |
|-------|------|-------|
| `obj_vnum` | integer | Object to place. Must exist in this file or the database. |
| `max_count` | integer | `>= 0`. `0` = no limit. |
| `room_vnum` | integer | Room to place in. Must exist in this file or the database. |

#### II.6.5. `cmd: D` — Set Door State

| Field | Type | Notes |
|-------|------|-------|
| `room_vnum` | integer | Room containing the door. Must exist in this file or the database. |
| `direction` | string | One of: `north east south west up down`. |
| `state` | string | One of: `open`, `closed`, `locked`. |

The named exit in the specified room must have the `door` lock flag set → rejection if the
exit is not a door. If `state: locked`, the exit's `key_vnum` must be `>= 0` → rejection.

#### II.6.6. `cmd: P` — Put Object in Container

| Field | Type | Notes |
|-------|------|-------|
| `obj_vnum` | integer | Object to place inside the container. |
| `max_count` | integer | `>= 0`. |
| `container_vnum` | integer | Object vnum of the container. Must be `item_type: container`. |

---

### II.7. `specials.yaml`

Top-level key: `specials`. Value: a sequence of mob-special assignment mappings.

```yaml
specials:
  - mob_vnum: 1100
    spec_fun:  spec_midgaard_city_guard

  - mob_vnum: 1112
    spec_fun:  spec_cast_adept
```

#### II.7.1. Special Fields

**`mob_vnum`** — Integer. Must reference a mob in `mobs.yaml` in this directory or in the
database. Each mob may appear at most once → rejection if the same vnum appears twice.

**`spec_fun`** — String. The special function name. Must be a name present in the
`spec_lookup` registry (`src/special.c`). Builder-restricted functions are rejected:
`spec_summon_*` may never be authored; `spec_keep_*` may not appear in generated area files
(human builders only). Any unrecognized name → rejection.

---

### II.8. `objfuns.yaml`

Top-level key: `objfuns`. Value: a sequence of object-function assignment mappings.

```yaml
objfuns:
  - obj_vnum: 32
    objfun:   objfun_healing

  - obj_vnum: 2559
    objfun:   objfun_cast_fight
```

#### II.8.1. Objfun Fields

**`obj_vnum`** — Integer. Must reference an object in `objects.yaml` in this directory or
in the database. Each object may appear at most once → rejection if the same vnum appears
twice.

**`objfun`** — String. The object function name. Must be registered in the objfun lookup
table (`src/special.c` or equivalent). Currently valid names: `objfun_healing`,
`objfun_cast_fight`. Any unrecognized name → rejection.

---
## III. Flag and Enum Reference Tables

All string values used in flag lists and enum fields throughout §II are defined here.
Every table lists the **exact lowercase name** to use in YAML. Any name not present in the
relevant table is a rejection. Integer values are never accepted in place of names.

### III.1. Room Flags (`flags` in `rooms.yaml`)

Source: `tab_room_flags` in `src/buildtab.c`.

| Name | Bit value | Notes |
|------|----------:|-------|
| `nada` | 0 | No flags (equivalent to empty list `[]`) |
| `dark` | 1 | Room is always dark |
| `regen` | 2 | Enhanced HP/mana regeneration |
| `no_mob` | 4 | Mobs cannot enter or wander in; required for boss rooms |
| `indoors` | 8 | Indoor room (weather not shown) |
| `no_magic` | 16 | Spells cannot be cast here |
| `hot` | 32 | Room is hot — players lose HP each tick |
| `cold` | 64 | Room is cold — players lose HP each tick |
| `pk` | 128 | PK-enabled room |
| `quiet` | 256 | Suppresses some ambient messages |
| `private` | 512 | Limited-entry room |
| `safe` | 1024 | Safe room — no combat |
| `solitary` | 2048 | Maximum 1 player |
| `pet_shop` | 4096 | Pet shop room |
| `no_recall` | 8192 | Players cannot recall from here |
| `no_teleport` | 16384 | Players cannot teleport to/from here |
| `hunt_hunt` | 32768 | Mobs hunt aggressively here |
| `no_bloodwalk` | 65536 | Bloodwalk travel blocked |
| `no_portal` | 131072 | Portal creation/entry blocked |
| `no_repop` | 524288 | Area resets do not repopulate this room |
| `maze` | 1048576 | Part of a maze — all rooms in the set must have this flag |

**Rejection rules:**
- Any name not in this table → rejection.
- `maze` set: every room in the maze vnum group must also have `maze` → rejection if any
  room in the set lacks it.
- Boss mob spawn rooms (any room receiving a boss mob via a reset `M` command) must have
  `no_mob` → rejection if missing.

---

### III.2. Sector Type (`sector` in `rooms.yaml`)

Source: `tab_sector_types` in `src/buildtab.c`. Single string value (enum, not a list).

| Name | Integer |
|------|--------:|
| `nada` | 0 |
| `city` | 1 |
| `field` | 2 |
| `forest` | 3 |
| `hills` | 4 |
| `mountain` | 5 |
| `water_swim` | 6 |
| `water_noswim` | 7 |
| `recall_set` | 8 |
| `air` | 9 |
| `desert` | 10 |
| `inside` | 11 |

Any name not in this table → rejection.

---

### III.3. Door Lock Flags (`locks` in room exits)

Source: `tab_door_types` in `src/buildtab.c`.

| Name | Bit value | Notes |
|------|----------:|-------|
| `door` | 1 | Exit behaves as a door (can open/close/lock) |
| `closed` | 2 | **Runtime state — must not be authored → rejection** |
| `locked` | 4 | **Runtime state — must not be authored → rejection** |
| `climb` | 8 | Requires climbing to traverse |
| `staff` | 16 | Staff-only exit |
| `pickproof` | 32 | Cannot be picked |
| `smashproof` | 64 | Cannot be bashed open |
| `passproof` | 128 | Pass-door spell does not work here |
| `nodetect` | 256 | Exit not shown in `exits` command |

`closed` or `locked` authored in an exit → rejection. Set initial door state via
`resets.yaml` `D` commands instead.

---

### III.4. Act Flags (`act` in `mobs.yaml`)

Source: `tab_mob_flags` in `src/buildtab.c`.

| Name | Notes |
|------|-------|
| `is_npc` | **Required on every mob — rejection if absent** |
| `sentinel` | Mob does not wander; required on all boss mobs |
| `scavenger` | Mob picks up objects |
| `remember` | Mob remembers attackers |
| `no_flee` | Mob never flees |
| `aggressive` | Mob attacks players on sight |
| `stay_area` | **Required on every mob — rejection if absent** |
| `wimpy` | Mob flees at low HP |
| `pet` | Mob is a pet |
| `train` | Mob can train player stats |
| `practice` | Mob can teach skills |
| `mercenary` | Mob is a mercenary |
| `heal` | Mob sells heals |
| `adapt` | Mob adapts to player damage types |
| `undead` | Mob is undead |
| `bank` | Mob is a banker |
| `no_body` | Mob leaves no corpse |
| `hunter` | Mob actively hunts enemies |
| `no_mind` | Mob is mindless (immune to some psionic spells) |
| `postman` | Mob handles letters |
| `rewield` | Mob rewields weapons if disarmed |
| `reequip` | Mob reequips gear if removed |
| `no_hunt` | Mob cannot be hunted by the `hunt` command |
| `solo` | Mob is designed to fight solo; required on level ≥ 50 non-boss mobs |
| `no_blood` | Mob does not bleed |
| `boss` | Boss mob; requires `sentinel` and a loot table |
| `quartermaster` | Mob is a quartermaster NPC |
| `invasion` | **Runtime-only — must not be authored → rejection** |
| `noassist` | Mob does not assist allies |
| `day_only` | Mob is daytime-only |
| `night_only` | Mob is nighttime-only |
| `ai_dialogue` | Mob uses AI dialogue |

**Rejection rules:**
- `is_npc` absent → rejection.
- `stay_area` absent → rejection.
- `invasion` present → rejection.
- `day_only` + `night_only` both present → rejection.
- `boss` present without `sentinel` → rejection.
- `boss` present without a `loot` block → rejection.
- Level ≥ 50, `boss` absent, `solo` absent → rejection.

---

### III.5. Affected-By Flags (`affected_by` in `mobs.yaml`)

Source: `tab_affected_by` in `src/buildtab.c` and `AFF_*` defines in
`src/headers/config.h`.

| Name | Bit value |
|------|----------:|
| `blind` | 1 |
| `invisible` | 2 |
| `detect_evil` | 4 |
| `detect_invis` | 8 |
| `detect_magic` | 16 |
| `detect_hidden` | 32 |
| `cloak:reflection` | 64 |
| `sanctuary` | 128 |
| `faerie_fire` | 256 |
| `infrared` | 512 |
| `curse` | 1024 |
| `cloak:flaming` | 2048 |
| `poison` | 4096 |
| `protect` | 8192 |
| `cloak:absorption` | 16384 |
| `sneak` | 32768 |
| `hide` | 65536 |
| `sleep` | 131072 |
| `charm` | 262144 |
| `flying` | 524288 |
| `pass_door` | 1048576 |
| `anti_magic` | 2097152 |
| `blasted` | 4194304 |
| `remort_curse` | 8388608 |
| `confused` | 16777216 |
| `hold` | 67108864 |
| `paralysis` | 134217728 |
| `cloak:adept` | 268435456 |

Any name not in this table → rejection. Colon-containing names (`cloak:reflection`,
`cloak:flaming`, `cloak:absorption`, `cloak:adept`) must be quoted in YAML if used as
bare strings; flow-sequence form is recommended: `["cloak:reflection"]`.

---

### III.6. Skills Bitvector (`skills` in `class_line`)

Source: `tab_mob_skill` in `src/buildtab.c`.

| Name | Min level |
|------|----------:|
| `2_attack` | 1 |
| `3_attack` | 1 |
| `4_attack` | 20 |
| `punch` | 1 |
| `headbutt` | 1 |
| `knee` | 1 |
| `disarm` | 1 |
| `trip` | 1 |
| `nodisarm` | 1 |
| `notrip` | 1 |
| `dodge` | 1 |
| `parry` | 1 |
| `martial` | 1 |
| `enhanced` | 1 |
| `dualwield` | 1 |
| `dirt` | 1 |
| `5_attack` | 30 |
| `6_attack` | 50 |
| `charge` | 1 |
| `counter` | 1 |
| `kick` | 1 |

Skill set on a mob below the minimum level → rejection.

---

### III.7. Cast Bitvector (`cast` in `class_line`)

Source: `tab_mob_cast` in `src/buildtab.c`.

| Name |
|------|
| `mag_missile` |
| `shock_grasp` |
| `burn_hands` |
| `col_spray` |
| `fireball` |
| `hellspawn` |
| `acid_blast` |
| `chain_light` |
| `faerie_fire` |
| `flare` |
| `flamestrike` |
| `earthquake` |
| `mind_flail` |
| `planergy` |
| `phobia` |
| `mind_bolt` |
| `static` |
| `ego_whip` |
| `bloody_tears` |
| `mindflame` |
| `suffocate` |
| `nerve_fire` |
| `light_bolt` |
| `heat_armor` |
| `lava_burst` |

Empty list `[]` means no offensive spells. `nada` and `placeholder` from the legacy format
are not valid YAML cast entries; use `[]` instead.

---

### III.8. Def Bitvector (`def` in `class_line`)

Source: `tab_mob_def` in `src/buildtab.c`.

| Name | Notes |
|------|-------|
| `nada` | No defensive spells. Must be the only entry if present. |
| `cure_light` | |
| `cure_serious` | |
| `cure_critic` | |
| `heal` | |
| `fireshield` | |
| `iceshield` | |
| `shockshield` | |

Use `[nada]` when a mob has no defensive spells. `[]` (empty list) → rejection; at least
one entry is required when `class_line` is present.

---

### III.9. Element Bitvectors (`strong_magic`, `weak_magic`, `resist`, `suscept`)

Source: `tab_magic_realms` in `src/buildtab.c`.

| Name |
|------|
| `nada` |
| `fire` |
| `shock` |
| `light` |
| `gas` |
| `poison` |
| `cold` |
| `sound` |
| `acid` |
| `negation` |
| `impact` |
| `psionic` |
| `holy` |

`nada` in any of these lists means "none" (equivalent to `[]`). Do not combine `nada` with
other elements → rejection. `strong_magic` and `weak_magic` must not share any element →
rejection. `resist` and `suscept` must not share any element → rejection.

---

### III.10. Race Mods (`race_mods` in `element_line`)

Source: `tab_mob_race_mods` in `src/buildtab.c`. The `race_mods` field in `element_line`
accepts either the integer `0` (for none) or a list of the following names.

| Name |
|------|
| `nada` |
| `fast_heal` |
| `slow_heal` |
| `strong_magic` |
| `weak_magic` |
| `no_magic` |
| `immune_poison` |
| `resist_spell` |
| `woodland` |
| `darkness` |
| `huge` |
| `large` |
| `tiny` |
| `small` |
| `tail` |
| `tough_skin` |
| `stone_skin` |
| `iron_skin` |

When provided as a list, the YAML schema is: `race_mods: [tough_skin, large]`. When
provided as the integer `0`, it is treated as `nada`. Mixing integer and list forms →
rejection.

---

### III.11. AI Knowledge (`ai_knowledge` in `ai_line`)

Source: `tab_knowledge` in `src/buildtab.c`. The `ai_knowledge` field accepts either the
integer `0` (for none) or a list of the following names.

| Name |
|------|
| `weapons` |
| `trade` |
| `magic` |
| `temple` |
| `underworld` |
| `harbor` |
| `guard` |
| `history` |
| `wilderness` |
| `politics` |
| `helps` |

---

### III.12. Accent (`accent` in `ai_line`)

Source: `tab_accent` in `src/buildtab.c`. String enum.

| Name |
|------|
| `none` |
| `midgaard` |
| `kowloon` |
| `mafdet` |
| `kiess` |
| `rakuen` |

---

### III.13. Lore Flags (`lore_flags` in `mobs.yaml`)

Source: `LORE_FLAG_*` defines in `src/headers/typedefs.h`.

| Name | Bit |
|------|----:|
| `midgaard` | 1 |
| `kiess` | 2 |
| `kowloon` | 4 |
| `rakuen` | 8 |
| `mafdet` | 16 |
| `human` | 32 |
| `khenari` | 64 |
| `khephari` | 128 |
| `ashborn` | 256 |
| `umbral` | 512 |
| `rivennid` | 1024 |
| `deltari` | 2048 |
| `ushabti` | 4096 |
| `serathi` | 8192 |
| `kethari` | 16384 |

---

### III.14. Script Trigger Names (`trigger` in mob `scripts`)

| Name |
|------|
| `in_file_prog` |
| `act_prog` |
| `speech_prog` |
| `rand_prog` |
| `fight_prog` |
| `hitprcnt_prog` |
| `death_prog` |
| `entry_prog` |
| `greet_prog` |
| `all_greet_prog` |
| `give_prog` |
| `bribe_prog` |

Any other name → rejection.

---

### III.15. Item Type (`item_type` in `objects.yaml`)

Source: `tab_item_types` in `src/buildtab.c`. Single string value (enum).

| Name | Integer | Notes |
|------|--------:|-------|
| `light` | 1 | |
| `scroll` | 2 | |
| `null` | 3 | Legacy placeholder; no effect |
| `staff` | 4 | |
| `weapon` | 5 | Requires `hold` and `take` in `wear_flags` |
| `beacon` | 6 | |
| `portal` | 7 | |
| `treasure` | 8 | |
| `armor` | 9 | |
| `potion` | 10 | |
| `clutch` | 11 | |
| `furniture` | 12 | |
| `trash` | 13 | |
| `trigger` | 14 | |
| `container` | 15 | |
| `quest` | 16 | |
| `drink_con` | 17 | |
| `key` | 18 | |
| `food` | 19 | |
| `money` | 20 | |
| `stake` | 21 | |
| `boat` | 22 | |
| `corpse_npc` | 23 | **Must not be authored → rejection** |
| `corpse_pc` | 24 | **Must not be authored → rejection** |
| `fountain` | 25 | |
| `pill` | 26 | |
| `board` | 27 | |
| `soul` | 28 | |
| `piece` | 29 | |
| `matrix` | 30 | |
| `enchantment` | 31 | |
| `present` | 32 | |
| `stash` | 33 | |

---

### III.16. Extra Flags (`extra_flags` in `objects.yaml`)

Source: `tab_obj_flags` in `src/buildtab.c`.

| Name | Notes |
|------|-------|
| `generated` | **Runtime-only → rejection** |
| `bind-on-equip` | Binds on equip |
| `nodisarm` | Cannot be disarmed |
| `lock` | Locked item |
| `evil` | Evil-aligned object |
| `invis` | Invisible object |
| `magic` | Magical object |
| `nodrop` | Cannot be dropped |
| `bless` | Blessed object |
| `anti_good` | Good-aligned characters cannot use |
| `anti_evil` | Evil-aligned characters cannot use |
| `noremove` | Cannot be removed once equipped |
| `inventory` | Part of mob inventory |
| `nosave` | Not saved to player file |
| `trigger:destroy` | Destroyed on use trigger |
| `no_auction` | Cannot be auctioned |
| `mythic` | Mythic rarity |
| `legendary` | Legendary rarity |
| `rare` | Rare rarity |
| `vamp` | Vampire-associated item |
| `noloot` | Cannot be looted from corpse |
| `nosac` | Cannot be sacrificed |
| `unique` | Unique (one per player) |
| `lifestealer` | Lifestealer weapon; requires `anti_good` |
| `loot` | Loot-table item; required on all mob loot objects |
| `boss` | Boss drop; required on all boss loot objects |
| `buckler` | Buckler shield type |
| `wand` | Wand extra flag |
| `fist` | Fist weapon; required when `value3 = 0` (hit) |
| `two-handed` | Two-handed weapon |
| `bonded` | Bonded item |

Flags `claneq`, `unused_anti_neutral` are present in the legacy format but marked
`NO_USE` in the builder table. They must not be authored → rejection.

**Rejection rules:**
- `generated` → rejection.
- `lifestealer` without `anti_good` → rejection.
- Any object on a mob loot table without `loot` → rejection.
- Any object on a boss mob loot table without `boss` → rejection.

---

### III.17. Wear Flags (`wear_flags` in `objects.yaml`)

Source: `tab_wear_flags` in `src/buildtab.c`.

| Name | Slot |
|------|------|
| `halo` | Halo |
| `aura` | Aura |
| `horns` | Horns |
| `head` | Head |
| `face` | Face |
| `beak` | Beak |
| `ear` | Ear |
| `neck` | Neck |
| `wings` | Wings |
| `shoulders` | Shoulders |
| `arms` | Arms |
| `wrist` | Wrist |
| `hands` | Hands |
| `finger` | Finger |
| `claws` | Claws |
| `hold` | Held in hand |
| `about` | About body (cloak slot) |
| `waist` | Waist |
| `body` | Body |
| `tail` | Tail |
| `legs` | Legs |
| `feet` | Feet |
| `hooves` | Hooves |
| `take` | Takeable; **required on every authored object** |
| `clan_colors` | **Runtime-only → rejection** |
| `invasion_emblem` | **Runtime-only → rejection** |

---

### III.18. Item Apply (`item_apply` in `objects.yaml`)

Source: `tab_item_apply` in `src/buildtab.c`.

| Name |
|------|
| `nada` |
| `infra` |
| `invis` |
| `det_invis` |
| `sanc` |
| `sneak` |
| `hide` |
| `prot` |
| `enhanced` |
| `det_mag` |
| `det_hid` |
| `det_evil` |
| `pass_door` |
| `det_poison` |
| `fly` |
| `know_align` |
| `detect_undead` |
| `heated` |

Use `[nada]` when no apply effects are needed. `[]` (empty list) is also accepted and
treated as `nada`.

---

### III.19. Object Affect Locations (`location` in `affects`)

Source: `tab_obj_aff` in `src/buildtab.c`.

| Name | Notes |
|------|-------|
| `nada` | No location (no-op) |
| `str` | Strength |
| `dex` | Dexterity |
| `int` | Intelligence |
| `wis` | Wisdom |
| `con` | Constitution |
| `sex` | Sex |
| `class` | Class |
| `level` | Level |
| `age` | Age |
| `height` | Height |
| `weight` | Weight stat |
| `mana` | Mana |
| `hit` | HP |
| `move` | Movement |
| `gold` | Gold |
| `exp` | Experience |
| `ac` | Armor class |
| `hitroll` | Hit roll |
| `damroll` | Damage roll |
| `saving_para` | Save vs paralysis |
| `saving_rod` | Save vs rod |
| `saving_petri` | Save vs petrification |
| `saving_breath` | Save vs breath |
| `saving_spell` | Save vs spell |
| `spellpower` | Spell power |

`modifier` must be `0` for all authored objects. Area files must not set fixed stat
modifiers → rejection if non-zero.

---

### III.20. Wear Locations (`wear_loc` in reset `E` commands)

Source: `tab_wear_loc` in `src/buildtab.c`.

| Name | Slot |
|------|------|
| `halo` | Halo |
| `aura` | Aura |
| `horns` | Horns |
| `head` | Head |
| `face` | Face |
| `beak` | Beak |
| `ear_l` | Left ear |
| `ear_r` | Right ear |
| `neck_1` | First neck |
| `neck_2` | Second neck |
| `wings` | Wings |
| `shoulders` | Shoulders |
| `arms` | Arms |
| `wrist_l` | Left wrist |
| `wrist_r` | Right wrist |
| `hands` | Hands |
| `finger_l` | Left finger |
| `finger_r` | Right finger |
| `claws` | Claws |
| `hold_l` | Left hand |
| `hold_r` | Right hand |
| `cape` | Cape/cloak |
| `waist` | Waist |
| `body` | Body |
| `tail` | Tail |
| `legs` | Legs |
| `feet` | Feet |
| `hooves` | Hooves |
| `clan_colors` | **Must not be authored → rejection** |
| `invasion_emblem` | **Must not be authored → rejection** |

---

### III.21. Weapon Attack Types (`values[3]` for `item_type: weapon`)

Source: `tab_weapon_types` in `src/buildtab.c`.

| Name | Integer | Notes |
|------|--------:|-------|
| `hit` | 0 | **Only valid when `fist` is in `extra_flags` → rejection otherwise** |
| `slice` | 1 | |
| `stab` | 2 | |
| `slash` | 3 | |
| `whip` | 4 | |
| `claw` | 5 | |
| `blast` | 6 | |
| `pound` | 7 | |
| `crush` | 8 | |
| `rend` | 9 | |
| `bite` | 10 | |
| `pierce` | 11 | |
| `drain` | 12 | |
| `sear` | 13 | |

For weapon objects, `values[3]` accepts either the integer value or the name string. Name
strings are preferred in YAML. Must be thematically consistent with the weapon's name and
description → rejection if clearly inconsistent.

---

### III.22. Mob Class (`class` in `class_line`)

Source: `tab_mob_class` in `src/buildtab.c`. Single string name or integer.

| Name | Integer |
|------|--------:|
| `magi` | 0 |
| `cleric` | 1 |
| `cipher` | 2 |
| `warden` | 3 |
| `psionicist` | 4 |
| `sorcerer` | 5 |
| `assassin` | 6 |
| `knight` | 7 |
| `necromancer` | 8 |
| `monk` | 9 |

Legacy aliases `mage` (→ `magi`), `thief` (→ `cipher`), `warrior` (→ `warden`) are
accepted on import but normalized to the canonical name on export. Use canonical names
in new files.

---

### III.23. Valid Special Functions (`spec_fun` in `specials.yaml`)

All functions currently registered in `src/headers/special.h`. Functions marked
**restricted** may not be used in area files.

**Generic / cross-area:**
`spec_breath_any`, `spec_breath_acid`, `spec_breath_fire`, `spec_breath_frost`,
`spec_breath_gas`, `spec_breath_lightning`, `spec_cast_adept`, `spec_cast_bigtime`,
`spec_cast_cadaver`, `spec_cast_cleric`, `spec_cast_judge`, `spec_cast_mage`,
`spec_cast_undead`, `spec_executioner`, `spec_fido`, `spec_guard`, `spec_janitor`,
`spec_lamplighter`, `spec_laborer`, `spec_lay_sister`, `spec_mayor`, `spec_mino_guard`,
`spec_poison`, `spec_policeman`, `spec_postman`, `spec_posting_clerk`, `spec_records_keeper`,
`spec_rewield`, `spec_sage`, `spec_tax_man`, `spec_thief`, `spec_undead`,
`spec_vamp_hunter`, `spec_vendor`, `spec_warden`, `spec_city_messenger`,
`spec_harbor_hand`, `spec_park_keeper`, `spec_wizardofoz`, `spec_hermit_archon`,
`spec_mudschool_guide`, `spec_sanctum_keeper`

**Midgaard-specific:**
`spec_midgaard_beggar`, `spec_midgaard_caravan_master`, `spec_midgaard_city_guard`,
`spec_midgaard_executioner`, `spec_midgaard_gate_warden`, `spec_midgaard_healer`,
`spec_midgaard_invasion_warden`, `spec_midgaard_pilgrim`, `spec_midgaard_postmaster`,
`spec_midgaard_quartermaster`, `spec_midgaard_shopkeeper`, `spec_midgaard_street_vendor`,
`spec_midgaard_temple_guardian`, `spec_midgaard_temple_priest`

**Kiess-specific:**
`spec_kiess_innkeeper`, `spec_kiess_orator`, `spec_kiess_scout`, `spec_kiess_shopkeeper`,
`spec_kiess_wall_officer`

**Kowloon-specific:**
`spec_kowloon_corsair`, `spec_kowloon_courier`, `spec_kowloon_gate_captain`,
`spec_kowloon_innkeeper`, `spec_kowloon_laborer`, `spec_kowloon_magistracy`,
`spec_kowloon_shopkeeper`, `spec_kowloon_shrine`, `spec_kowloon_sweeper`,
`spec_kowloon_vendor`

**Great Northern Forest-specific:**
`spec_gnf_courier`, `spec_gnf_customs`, `spec_gnf_everkeeper`, `spec_gnf_joint_scout`,
`spec_gnf_lamplighter`, `spec_gnf_mire_speaker`, `spec_gnf_peddler`,
`spec_gnf_road_warden`, `spec_gnf_toll_collector`

**Roc Road-specific:**
`spec_rr_byways_ferryman`, `spec_rr_byways_hermit`, `spec_rr_byways_innkeeper`,
`spec_rr_byways_kiess_clerk`, `spec_rr_byways_market`, `spec_rr_byways_midgaard_guard`,
`spec_rr_byways_scholar`, `spec_rr_byways_warden`, `spec_rr_camp_cook`,
`spec_rr_charter_keeper`, `spec_rr_convoy_marshal`, `spec_rr_ferryman`, `spec_rr_peddler`,
`spec_rr_pilgrim`, `spec_rr_road_clerk`, `spec_rr_ruin_scavenger`, `spec_rr_shrine_keeper`,
`spec_rr_warden_captain`

**Saltglass Reach-specific:**
`spec_reach_assessor`, `spec_reach_carter`, `spec_reach_guide`, `spec_reach_outrider`,
`spec_reach_scavenger`, `spec_reach_smuggler`, `spec_reach_tidewright`, `spec_reach_warden`

**Cinderteeth Mountains-specific:**
`spec_cinderteeth_anchor`, `spec_cinderteeth_caldera_watcher`, `spec_cinderteeth_oracle`,
`spec_cinderteeth_patriarch`, `spec_cinderteeth_sulfur_colossus`,
`spec_cinderteeth_ventspeaker`, `spec_cinderteeth_warden_cmd`, `spec_cinderteeth_warlord`

**Saltglass Salt-and-Sinter:**
`spec_ss_cinder_broker`, `spec_ss_kiln_overseer`, `spec_ss_manifest_warden`

**Pyramid:**
`spec_pyramid_black_sun_shard`

**Summon specials — RESTRICTED, must not be authored:**
`spec_summon_water`, `spec_summon_fire`, `spec_summon_earth`, `spec_summon_undead`,
`spec_summon_holy`, `spec_summon_shadow`, `spec_summon_metal`, `spec_summon_animate`,
`spec_summon_thought`, `spec_summon_revenant`

**Keep specials — human builders only, must not appear in generated files:**
`spec_keep_physical_captain`, `spec_keep_elemental_captain`

Any name not in the above lists → rejection. `spec_summon_*` → rejection. `spec_keep_*`
in generated files → rejection.

**Area-specific specials should only be used in the area they were written for.** Using
a Midgaard-specific special in a Kiess area produces anachronistic dialogue.

---

### III.24. Valid Object Functions (`objfun` in `objfuns.yaml`)

Currently registered names:

| Name | Effect |
|------|--------|
| `objfun_healing` | Object provides a healing effect when used |
| `objfun_cast_fight` | Object casts a fight spell when triggered |

Any other name → rejection.

---
## IV. Content Validation Rules

These rules are enforced after parsing and flag resolution, before the database transaction
opens. They apply across all files in a single submission. Failures produce a rejection
with the offending file, entity type, and vnum named.

### IV.1. Text Quality Rules (all text fields)

These rules apply to every `name`, `short_descr`, `long_descr`, `description`, and
`extra_desc` field across all files.

#### IV.1.1. Placeholder Detection

Any name or description that matches a placeholder or procedural pattern → rejection.
Patterns that trigger rejection:

- Contains a bare integer suffix matching the entity's own vnum or a sequential index
  (e.g., `Room 7`, `Guard 12`, `Passage 3`).
- Follows the pattern `<word> <integer>` where `<word>` is a generic category word
  (`room`, `area`, `mob`, `npc`, `corridor`, `passage`, `object`, `item`, `guard`,
  `monster`, `creature`, `zone`, `region`, `chamber`) → rejection.
- Is identical to a field on a different entity of the same type within the same file
  (exact-duplicate check). Applies to: room `description`, room `name`, mob `long_descr`,
  mob `description`, object `name` → rejection on duplicate.

#### IV.1.2. Vnum Leak

Any text field containing a bare integer that matches any vnum in the file's declared
range `[vnum_min, vnum_max]` → rejection. This prevents vnums from leaking into player-
visible text.

#### IV.1.3. Description Length

| Field | Minimum | Maximum |
|-------|---------|---------|
| Room `description` | 3 sentences | — |
| Mob `description` | 1 sentence | 8 sentences |
| Object `description` | 1 sentence | — |
| Mob `long_descr` | 1 line (no `\n`) | 1 line |

Sentence count is estimated by counting terminal punctuation (`.`, `!`, `?`) followed by
whitespace or end-of-string.

#### IV.1.4. Case Requirements

| Field | Rule |
|-------|------|
| Room `name` | May begin with any case. Must not be entirely uppercase. |
| Mob `player_name` | All lowercase → rejection if any uppercase. |
| Mob `short_descr` | Must not begin with uppercase → rejection. |
| Object `name` | All lowercase → rejection if any uppercase. |
| Object `short_descr` | Must not begin with uppercase → rejection. |

#### IV.1.5. Trailing Newline

All `description` block scalars must end with exactly one `\n` after YAML parsing →
rejection if missing or doubled. This applies to: room `description`, mob `description`,
object `description`, room extra-desc `description`, object extra-desc `description`,
mob `long_descr` does *not* require a trailing newline (single line only).

---

### IV.2. Room Validation

#### IV.2.1. Exits

- `direction` must be one of: `north east south west up down` → rejection otherwise.
- Each direction may appear at most once per room → rejection if duplicated.
- `to_vnum` must reference a room in `rooms.yaml` in this directory or in the database →
  rejection if the target room cannot be found.
- `closed` or `locked` in the `locks` list → rejection (runtime state bits).
- If `key_vnum >= 0`, the referenced object must be `item_type: key` and must exist in
  `objects.yaml` in this directory or in the database → rejection otherwise.

#### IV.2.2. Bidirectionality

Except in rooms flagged `maze`:

- If room A has an exit in direction X to room B, room B must have the opposite exit back
  to room A. Opposite directions: north↔south, east↔west, up↔down → rejection if the
  reverse exit is missing.
- Exception: cross-area exits where room B is not in this directory and not in the
  database. These exits are permitted but must be documented in `docs/world_links.md` as
  **Planned**.
- Rooms must not form one-directional loops (going north repeatedly and arriving back
  at the start without a matching reverse path) → rejection.

#### IV.2.3. Extra Descriptions

Each extra-desc `keywords` string must have at least one word that appears in the room's
main `description` text or in the keyword string of another extra-desc in the same room
→ rejection if no anchor can be found.

#### IV.2.4. Boss Room Flag

Any room that receives a boss mob (a mob with `boss` in `act`) via a reset `M` command
must have `no_mob` in `flags` → rejection if the room lacks `no_mob`.

---

### IV.3. Mob Validation

#### IV.3.1. Required Flags

- `is_npc` must be in `act` → rejection.
- `stay_area` must be in `act` → rejection.
- `invasion` must not be in `act` → rejection.
- `day_only` and `night_only` must not both be in `act` → rejection.
- If `boss` in `act`: `sentinel` must also be in `act` → rejection.
- If `boss` in `act`: a `loot` block must be present → rejection.
- If `level >= 50` and `boss` not in `act`: `solo` must be in `act` → rejection.

#### IV.3.2. Stat Ranges

| Field | Range |
|-------|-------|
| `alignment` | −1000 to 1000 |
| `level` | 1 to 155 |
| `hp_mod` | −500 to 500 |
| `ac_mod` | −300 to 300 |
| `hr_mod` | −50 to 50 |
| `dr_mod` | −50 to 50 |

Values outside range → rejection.

#### IV.3.3. `class_line` Validation

When present:

- `race >= 0` → rejection if negative.
- `position` must be `0–9` → rejection outside range.
- Skills listed in `skills` must be appropriate for the mob's `level`; see minimum levels
  in §III.6 → rejection if skill requires higher level than mob has.
- `def` must contain at least one entry; `[nada]` is the explicit "no defensive spells"
  choice → rejection if `def` is an empty list.
- If `def` contains `nada`, no other entries may be present → rejection.

#### IV.3.4. `element_line` Validation

When present:

- `strong_magic` ∩ `weak_magic` must be empty → rejection if any element appears in both.
- `resist` ∩ `suscept` must be empty → rejection if any element appears in both.

#### IV.3.5. `loot` Validation

When present:

- `loot_amount >= 0` → rejection if negative.
- Maximum 9 slots → rejection if more than 9 items in `slots`.
- Sum of all `chance` values across `slots` must be `<= 100` → rejection if sum exceeds 100.
- Each `vnum` in `slots` must reference an object that exists in `objects.yaml` in this
  directory or in the database → rejection if not found.
- Each loot object must have `loot` in `extra_flags` → rejection if missing.
- If the mob has `boss` in `act`, each loot object must also have `boss` in `extra_flags`
  → rejection if missing.

#### IV.3.6. Scripts Validation

Each script block:

- `trigger` must be a name from §III.14 → rejection if unrecognized.
- `commands` must be non-empty → rejection.
- `args` field is optional; empty string `""` is valid.

---

### IV.4. Object Validation

#### IV.4.1. Required Flags

- `take` must be in `wear_flags` → rejection if missing.
- `generated` must not be in `extra_flags` → rejection.
- `clan_colors` must not be in `wear_flags` → rejection.
- `invasion_emblem` must not be in `wear_flags` → rejection.

#### IV.4.2. Item Type Restrictions

- `item_type: corpse_npc` or `item_type: corpse_pc` → rejection (runtime-generated only).

#### IV.4.3. Weapon Rules

When `item_type: weapon`:

- `hold` and `take` must both be in `wear_flags` → rejection if either is missing.
- `values[3]` (the attack type) must be a valid weapon attack type name or integer (§III.21)
  → rejection if not recognized.
- `values[3]: hit` (integer `0`) requires `fist` in `extra_flags` → rejection otherwise.
- `values[3]` must be thematically consistent with the weapon's `name`, `short_descr`,
  and `description`. A longsword with `bite` is a rejection. A fang with `slash` is a
  rejection. The validator checks a fixed list of thematic-conflict patterns.
- If `name`, `short_descr`, or `description` contains a two-handed archetype keyword
  (`great sword`, `greatsword`, `halberd`, `greataxe`, `great axe`, `war scythe`,
  `two-handed`, `two handed`), `two-handed` must be in `extra_flags` → rejection if missing.

#### IV.4.4. Shield and Buckler Rules

When `item_type: armor` and `hold` in `wear_flags`:

- Object must not have `two-handed` in `extra_flags` → rejection.
- If `buckler` is in `extra_flags`, the object's `name`/`short_descr` must not describe a
  full shield → rejection.

#### IV.4.5. `lifestealer` Rule

`lifestealer` in `extra_flags` without `anti_good` also in `extra_flags` → rejection.

#### IV.4.6. Loot Flag Consistency

Any object referenced in a mob's `loot.slots` must have `loot` in `extra_flags` →
rejection if not set. Any object referenced in a boss mob's loot table must have `boss`
in `extra_flags` → rejection if not set.

#### IV.4.7. Weight Archetype Consistency

`weight` must be consistent with the object's evident purpose as determined by its
`name`, `short_descr`, `wear_flags`, and `item_type`. The validator checks:

- A weapon (`item_type: weapon`) whose `name`/`short_descr` contains a heavy-weapon
  keyword (`greatsword`, `halberd`, `greataxe`, `maul`, `warhammer`) must have
  `weight >= 6` → rejection if lighter.
- An explicitly magical item (`name`/`short_descr` contains `arcane`, `mage`, `sorcerer`,
  `wizard`, `runic`, `spellbound`, `enchanted staff`, `grimoire`) must have `weight <= 5`
  → rejection if heavier.

---

### IV.5. Shop Validation

- `keeper_vnum` must reference a mob in `mobs.yaml` in this directory or the database →
  rejection if not found.
- `buy_types` may contain at most 5 entries → rejection if more than 5.
- Each `buy_type` name must be a valid `item_type` name from §III.15 → rejection if
  unrecognized.
- `sell_profit` must be `50–300` → rejection outside range.
- `buy_profit` must be `50–300` → rejection outside range.
- `buy_profit >= sell_profit` → rejection. The shop must buy for less than (or equal to)
  what it sells for.
- `open_hour` must be `0–24` → rejection outside range.
- `close_hour` must be `0–24` → rejection outside range.
- `open_hour < close_hour` → rejection if reversed.

---

### IV.6. Reset Validation

#### IV.6.1. Ordering Constraints

- `G` (give object to mob) must follow an `M` (spawn mob) command somewhere earlier in
  the reset list, with only `G` or `E` commands intervening since the most recent `M` →
  rejection if `G` appears before any `M` or after a non-`G`/non-`E` command since the
  last `M`.
- `E` (equip object on mob) follows the same ordering rule as `G` → rejection if `E`
  appears out of context.

#### IV.6.2. Vnum References

- `cmd: M` — `mob_vnum` must reference a mob in `mobs.yaml` or the database; `room_vnum`
  must reference a room in `rooms.yaml` or the database → rejection if either not found.
- `cmd: G` — `obj_vnum` must reference an object in `objects.yaml` or the database →
  rejection if not found.
- `cmd: E` — `obj_vnum` must exist; `wear_loc` must be a valid name from §III.20;
  `clan_colors` and `invasion_emblem` → rejection → rejection if either used.
- `cmd: O` — `obj_vnum` and `room_vnum` must exist → rejection if not found.
- `cmd: D` — `room_vnum` must exist; the named direction must exist as an exit in that
  room; the exit must have `door` in `locks` → rejection if the exit is not a door.
- `cmd: P` — `obj_vnum` and `container_vnum` must exist; `container_vnum` must reference
  an object with `item_type: container` → rejection otherwise.

#### IV.6.3. `cmd: D` Door State Cross-Check

- If `state: locked`, the exit must have `key_vnum >= 0` and `door` in `locks` →
  rejection if key is absent or exit is not a door.
- If `state: closed` or `state: locked`, the exit must have `door` in `locks` →
  rejection otherwise.

#### IV.6.4. Max Count Rules

- `max_count >= 1` for `cmd: M` → rejection if `< 1`.
- `max_count >= 0` for all other commands → rejection if negative.

---

### IV.7. Specials Validation

- `mob_vnum` must reference a mob in `mobs.yaml` in this directory or the database →
  rejection if not found.
- Each `mob_vnum` may appear at most once in `specials.yaml` → rejection if duplicated.
- `spec_fun` must be a name from §III.23 → rejection if unrecognized.
- `spec_summon_*` → rejection. `spec_keep_*` → rejection.
- Area-specific specials used in the wrong area produce a validator warning (not a
  rejection) naming the mismatch.

---

### IV.8. Objfuns Validation

- `obj_vnum` must reference an object in `objects.yaml` in this directory or the database
  → rejection if not found.
- Each `obj_vnum` may appear at most once in `objfuns.yaml` → rejection if duplicated.
- `objfun` must be a name from §III.24 → rejection if unrecognized.

---
## V. Cross-Section Consistency Rules

These rules are checked after all individual files have been validated. They verify that
entities referenced across files form a consistent, coherent set. All cross-section checks
run before the database transaction opens.

### V.1. Vnum Range Compliance (all files)

Every vnum that appears in any field of any file — including `to_vnum` in exits, `mob_vnum`
and `room_vnum` in resets, `keeper_vnum` in shops, `mob_vnum` in specials, `obj_vnum` in
objfuns, and all cross-references in object `values[]` — must either:

1. Fall within `[vnum_min, vnum_max]` from `area.yaml` (it belongs to this area), **or**
2. Already exist in the database (it is a cross-area reference to an already-imported area).

Any vnum that satisfies neither condition → rejection naming the field, file, and referencing
entity.

---

### V.2. Room ↔ Reset Consistency

#### V.2.1. Room Receives a Mob

Every room referenced in a `cmd: M` reset must exist in `rooms.yaml` or the database.

#### V.2.2. Boss Mob Placement

If any mob referenced in a `cmd: M` reset has `boss` in its `act` flags:

- The target room must have `no_mob` in `flags` → rejection if the room lacks `no_mob`.

This cross-check is run after both `rooms.yaml` and `mobs.yaml` are loaded. It catches
cases where `no_mob` was omitted from a room that receives a boss mob.

#### V.2.3. Object Placement in Rooms

Every room referenced in a `cmd: O` reset must exist in `rooms.yaml` or the database.

#### V.2.4. Door State Application

Every `cmd: D` reset must target a room that exists in `rooms.yaml` or the database, and
the named direction must be an exit in that room that has `door` in its `locks` list →
rejection if the exit is not a door.

---

### V.3. Mob ↔ Object (Loot Table)

For every mob in `mobs.yaml` that has a `loot` block:

- Every non-zero `vnum` in `slots` must resolve to an object in `objects.yaml` in this
  directory or in the database → rejection if the object cannot be found.
- Every such object must have `loot` in `extra_flags` → rejection if missing.
- If the mob has `boss` in `act`, every loot-slot object must also have `boss` in
  `extra_flags` → rejection if missing.

---

### V.4. Shop ↔ Mob

For every shop in `shops.yaml`:

- `keeper_vnum` must reference a mob that exists in `mobs.yaml` in this directory or in
  the database → rejection if not found.
- The keeper mob must not have `boss` in `act` — boss mobs cannot be shopkeepers →
  rejection.
- The keeper mob should have either `sentinel` in `act` (it stays in one room) or a
  matching `cmd: M` reset that places it in a fixed room. A shop whose keeper has no
  `sentinel` and no reset → validator warning (not rejection).

---

### V.5. Specials ↔ Mob

For every entry in `specials.yaml`:

- `mob_vnum` must reference a mob that exists in `mobs.yaml` in this directory or in the
  database → rejection if not found.
- The same `mob_vnum` must not appear more than once → rejection.
- If the spec_fun name is area-specific (contains a known area keyword like `midgaard_`,
  `kiess_`, `kowloon_`, `gnf_`, `rr_`, `reach_`, `cinderteeth_`, `ss_`, `pyramid_`),
  the spec_fun's area prefix must match the area's `keyword` → validation warning naming
  the mismatch. This is a warning, not a rejection, to allow intentional cross-area NPCs.

---

### V.6. Objfuns ↔ Object

For every entry in `objfuns.yaml`:

- `obj_vnum` must reference an object that exists in `objects.yaml` in this directory or
  in the database → rejection if not found.
- The same `obj_vnum` must not appear more than once → rejection.

---

### V.7. Reset Sequencing (Mob-Object Association)

The `G` and `E` reset commands implicitly target the mob spawned by the most recent `M`
command in the sequence. The cross-section rule:

- A `G` command's `obj_vnum` must reference an object that exists (in this directory or
  the database) → rejection if not found.
- An `E` command's `obj_vnum` must reference an object that exists → rejection if not
  found.
- An `E` command's `wear_loc` must be compatible with the object's `wear_flags`. The
  YAML wear-loc name (e.g., `body`) must match one of the wear flags on the referenced
  object (e.g., the object must have `body` in `wear_flags`) → rejection if the object
  cannot be worn in the specified location.

---

### V.8. Container ↔ Object (`cmd: P`)

For every `cmd: P` reset:

- `container_vnum` must reference an object with `item_type: container` → rejection if
  the referenced object has any other item type.
- The contained object (`obj_vnum`) must have `take` in `wear_flags` → rejection otherwise.

---

### V.9. Piece Chain Completeness (`item_type: piece`)

For every object with `item_type: piece`:

- If `values[0]` (previous piece) is non-zero, the referenced object must also be
  `item_type: piece` and must exist in `objects.yaml` in this directory or the database →
  rejection.
- If `values[1]` (next piece) is non-zero, the same rule applies.
- If `values[2]` (replacement vnum on connect) is non-zero, the referenced object must
  exist in `objects.yaml` or the database → rejection.
- A piece with both `values[0]` and `values[1]` equal to `0` → rejection. A piece must
  connect to at least one other piece.

---

### V.10. Board Object ↔ Board Table (`item_type: board`)

For every object with `item_type: board`:

- `values[3]` (board vnum) must be non-zero → rejection.
- `values[3]` must not conflict with any existing board vnum in the database → rejection.
- `values[0]` (expiry days) must be `>= 1` → rejection.
- `values[1]` (min read level) must be `0–105` → rejection.
- `values[2]` (min write level) must be `0–105` → rejection.
- The ingestion pipeline creates the board database row as part of the import transaction.
  On update (replace), the existing board rows are dropped and re-created.

---

### V.11. Key Object Consistency

For every room exit with `key_vnum >= 0`:

- The referenced object must exist in `objects.yaml` in this directory or the database →
  rejection.
- The referenced object must have `item_type: key` → rejection if any other type.

For every `cmd: D` reset with `state: locked`:

- The exit must have `key_vnum >= 0` → rejection if no key is defined for the exit.
- The referenced key object must exist (same rule as above) → rejection.

---

### V.12. Maze Set Completeness

If any room in `rooms.yaml` has `maze` in `flags`:

- Every room in the contiguous vnum group that forms the maze must also have `maze` in
  `flags`. Contiguity is defined as: rooms that are reachable from one another via exits
  within the file. → rejection if any reachable room in a maze cluster lacks `maze`.
- Maze rooms are exempt from the bidirectionality rule (§IV.2.2).
- Maze rooms must not form exits that leave the maze set and point to non-maze rooms
  without the non-maze room being explicitly authored as the maze entrance/exit. The
  entrance/exit room does not require `maze` but must not be in the maze cluster.

---

### V.13. World Link Documentation

For every room exit whose `to_vnum` does not exist in `rooms.yaml` in this directory
and does not exist in the database:

- The exit must be documented in `docs/world_links.md` as **Planned**, with the source
  room vnum, direction, and intended target → rejection if the link is not documented.

This prevents dangling exits from accumulating silently. Cross-area exits to already-
imported areas (present in the database) do not require a `world_links.md` entry.

---
