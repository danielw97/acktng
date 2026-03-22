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
