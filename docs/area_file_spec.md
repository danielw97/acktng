# Area File Specification

**Version:** strict ingestion format, aligned with the PostgreSQL migration (see `docs/proposals/database-schema-areas.md`).

This document is the authoritative specification for `.are` area files in ACK!TNG. Every rule here is a hard requirement. The ingestion validator (`src/areaingest.c`) rejects any file that violates any rule. There are no warnings; a file either passes all rules and is imported, or it fails with an error message and is moved to `area/incoming/failed/`.

---

## 1. File Structure

An area file is a plain-text UTF-8 file. Lines must end with `\n` (Unix). `\r\n` is stripped silently on read; bare `\r` is a rejection. Null bytes are a rejection. Maximum file size: 4 MB.

The file consists of named **sections**. Each section starts with `#<NAME>` on its own line. Sections are parsed by name; order is flexible for the loader but the canonical saver order (see §14) must be used for authored files.

Required section: `#ROOMS`
Optional sections: `#AREA`, `#MOBILES`, `#OBJECTS`, `#SHOPS`, `#RESETS`, `#SPECIALS`, `#OBJFUNS`
End-of-file marker: `#$`

### Rejection rules

- Any section name not in the list above is a rejection.
- `#ROOMS` absent → rejection.
- `#ROOMS` empty (no rooms) → rejection.
- File larger than 4 MB → rejection.
- Non-UTF-8 bytes → rejection.
- Bare `\r` → rejection.
- Null bytes → rejection.
- Comments of any kind anywhere in the file → rejection. No section accepts comment lines.

---

## 2. String Encoding

Most text fields are tilde-terminated strings. A tilde-terminated string is read until the next bare `~` character. Rules:

- The terminating `~` is mandatory. A missing `~` is a parse error and a rejection.
- Embedded bare tildes are forbidden within string content.
- Embedded `\n\n` (blank line within a string) is forbidden — rejection.
- Maximum length per string: 4096 bytes excluding the terminator.
- `long_descr` strict format: exactly one text line (non-empty), followed by `\n`, followed by `~` on its own line. Multi-line `long_descr` is a rejection.
- `description` (for mobiles): must end with exactly one `\n` immediately before the terminating `~`. Double-newline before `~` is a rejection; missing trailing newline before `~` is a rejection.
- Room/extra description `<description>`: must end with exactly one `\n` immediately before the terminating `~`. Same rules as mobile description.
- Vnums must never appear in any in-world description text (room descriptions, mobile descriptions, object descriptions, extra descriptions, exit descriptions). A vnum in descriptive text is a rejection.

### 2.1. Color Codes

Area strings may include inline color escapes using `@@<code>`. Each code consists of `@@` followed by exactly one character:

| Code | Color |
|------|-------|
| `g` | gray |
| `R` | red |
| `G` | green |
| `b` | brown |
| `B` | blue |
| `m` | magenta |
| `c` | cyan |
| `k` | black foreground (**forbidden in area files — see below**) |
| `y` | yellow |
| `W` | white |
| `N` | normal (reset) |
| `p` | purple |
| `d` | dark grey |
| `l` | light blue |
| `r` | light green |
| `a` | light cyan |
| `e` | light red |
| `x` | bold |
| `f` | flashing (**forbidden in area files — see below**) |
| `i` | inverse |
| `2` | background red (**forbidden in area files — see below**) |
| `3` | background green (**forbidden in area files — see below**) |
| `4` | background yellow (**forbidden in area files — see below**) |
| `1` | background blue (**forbidden in area files — see below**) |
| `5` | background magenta (**forbidden in area files — see below**) |
| `6` | background cyan (**forbidden in area files — see below**) |
| `0` | background black (**forbidden in area files — see below**) |
| `7` | background white (**forbidden in area files — see below**) |

**Rejection rules for color codes:**
- `@@k` (black foreground) is forbidden in all area-authored strings → rejection.
- `@@f` (flashing) is forbidden in all area-authored strings → rejection.
- All background codes (`@@0`–`@@7`) are forbidden in all area-authored strings → rejection.
- An unrecognized `@@<x>` code where `x` is not in the table above → rejection.

---

## 3. `#AREA` Section

Required. Must appear once. Must appear before any other section in files authored for ingestion.

```
#AREA
<name>~
<directives...>
```

`<name>` is a tilde-terminated string: the area's display name.

### 3.1. Directives

Directives are single-character identifiers followed by their argument(s). They are case-sensitive. Parsing continues until the next `#` section header.

#### Required directives

All of the following are required. A missing required directive is a rejection.

| Directive | Syntax | Rules |
|-----------|--------|-------|
| `Q` | `Q 16` | Literal `Q 16`. Any other value is a rejection. |
| `V` | `V <min> <max>` | Two integers. `min < max` required. `max - min >= 9` required. Overlap with any existing area's V range is a rejection. |
| `K` | `K <keyword>~` | Non-empty, lowercase, no spaces, no special characters except `_`. Must be unique across all areas in the database. |
| `L` | `L <label>~` | Non-empty string. |
| `N` | `N <int>` | Integer `>= 0`. |
| `I` | `I <min_level> <max_level>` | Two integers. `0 <= min_level <= max_level <= 105`. |
| `O` | `O Virant~` | Literal `O Virant~`. Any other owner string is a rejection. |
| `F` | `F <rate>` | Integer `>= 1`. |

#### Optional directives

| Directive | Syntax | Rules |
|-----------|--------|-------|
| `U` | `U <message>~` | Reset message string. |
| `R` | `R <acl>~` | Read ACL string. |
| `W` | `W <acl>~` | Write ACL string. |
| `X` | `X <int>` | Map offset integer. |
| `P` | `P` | Pay-area flag. Rest of line ignored. |
| `T` | `T` | Teleport flag. Rest of line ignored. |
| `B` | `B` | Building flag. Rest of line ignored. |
| `S` | `S` | No-show flag. Rest of line ignored. |
| `M` | `M` | No-room-affects flag. Rest of line ignored. |
| `C` | `C <filename>~` | Theme music. Bare MP3 filename (no path, no directory component). Served from `/web/mp3/`. |

Any directive letter not in the table above is a rejection.

---

## 4. `#ROOMS` Section

Required. A list of room records terminated by `#0`:

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

See `docs/room_spec.md` for the complete room format specification, all bitvector definitions, and validation rules. All rules in `room_spec.md` are enforced by the ingestion validator.

---

## 5. `#MOBILES` Section

Optional. A list of mobile records terminated by `#0`.

See `docs/mob_spec.md` for the complete mobile format specification, all extension line formats, bitvector definitions, and validation rules. All rules in `mob_spec.md` are enforced by the ingestion validator.

---

## 6. `#OBJECTS` Section

Optional. A list of object records terminated by `#0`.

See `docs/object_spec.md` for the complete object format specification, all bitvector definitions, value semantics, and validation rules. All rules in `object_spec.md` are enforced by the ingestion validator.

---

## 7. `#SHOPS` Section

Optional. Terminated by a line containing only `0`:

```
#SHOPS
<keeper_vnum> <buy_type_0> <buy_type_1> <buy_type_2> <buy_type_3> <buy_type_4> <profit_buy> <profit_sell> <open_hour> <close_hour>
...
0
```

`MAX_TRADE = 5`. Each shop line has exactly 10 integer fields in the order above.

### Rejection rules

- `keeper_vnum` must be a mobile vnum defined in `#MOBILES` in this file or already in the database → rejection if not found.
- `keeper_vnum` must be within the area's `V` range → rejection if outside.
- Each `buy_type_N` must be a valid `item_type` integer (1–33) or `0` (any) → rejection for out-of-range values.
- `profit_buy` must be `>= 100` (shopkeepers buy at or below face value) → rejection.
- `profit_sell` must be `>= 100` (shopkeepers sell at or above face value) → rejection.
- `open_hour` must be `0–23`; `close_hour` must be `0–23` → rejection for out-of-range values.
- Fewer than 10 integer fields on a shop line → rejection.

---

## 8. `#SPECIALS` Section

Optional. Terminated by `S`:

```
#SPECIALS
M <mob_vnum> <spec_fun_name>
...
S
```

### Allowed `spec_fun_name` values

Any name not in this list is a rejection.

**Breath weapon:**
`spec_breath_any`, `spec_breath_acid`, `spec_breath_fire`, `spec_breath_frost`, `spec_breath_gas`, `spec_breath_lightning`

**Combat caster:**
`spec_cast_adept`, `spec_cast_cleric`, `spec_cast_judge`, `spec_cast_mage`, `spec_cast_undead`, `spec_cast_cadaver`, `spec_cast_bigtime`

**Law enforcement:**
`spec_executioner`, `spec_guard`, `spec_policeman`, `spec_mino_guard`

**NPC behavior:**
`spec_fido`, `spec_janitor`, `spec_mayor`, `spec_poison`, `spec_thief`, `spec_rewield`, `spec_undead`, `spec_stephen`, `spec_sage`, `spec_wizardofoz`, `spec_tax_man`, `spec_sylai_priest`

**Ambient:**
`spec_lamplighter`, `spec_warden`, `spec_vendor`, `spec_lay_sister`, `spec_laborer`

**Scorching Sands:**
`spec_ss_cinder_broker`, `spec_ss_manifest_warden`, `spec_ss_kiln_overseer`

**Kiess:**
`spec_kiess_shopkeeper`, `spec_kiess_innkeeper`, `spec_kiess_scout`, `spec_kiess_orator`, `spec_kiess_wall_officer`

**Keep:**
`spec_keep_physical_captain` — **human-assignment only; must not be set by automated tools or Claude sessions**

**Forbidden (runtime-only — rejection if present in area file):**
`spec_summon_water`, `spec_summon_fire`, `spec_summon_earth`, `spec_summon_undead`, `spec_summon_holy`, `spec_summon_shadow`, `spec_summon_metal`, `spec_summon_animate`, `spec_summon_thought`

### Rejection rules

- `mob_vnum` must be within the area's `V` range → rejection.
- `mob_vnum` must be defined in `#MOBILES` in this file or already in the database → rejection.
- Any `spec_summon_*` name → rejection.
- Any name not in the allowed list above → rejection.

---

## 9. `#OBJFUNS` Section

Optional. Terminated by `S`:

```
#OBJFUNS
O <obj_vnum> <obj_fun_name>
...
S
```

### Allowed `obj_fun_name` values

Any name not in this list is a rejection: `objfun_giggle`, `objfun_cast_fight`, `objfun_sword_aggro`, `objfun_soul_moan`, `objfun_infused_soul`, `objfun_flaming`, `objfun_healing`, `objfun_dispeller`, `objfun_regen`, `objfun_clan`

### Rejection rules

- `obj_vnum` must be within the area's `V` range → rejection.
- `obj_vnum` must be defined in `#OBJECTS` in this file or already in the database → rejection.
- `objfun_healing` assigned to an object → that object must have `ITEM_ANTI_EVIL` in `extra_flags` → rejection if not.
- Any name not in the allowed list above → rejection.

---

## 10. `#RESETS` Section

Optional. Terminated by `S`. **No blank lines within `#RESETS`** — a blank line is a rejection.

```
#RESETS
<command> <ifflag> <arg1> <arg2> [<arg3>] [<notes...>]
...
S
```

For `G` and `R` commands there is no `arg3` in the file; the parser sets it to `0` internally. For all other commands `arg3` is required.

Remaining text on the line after all required integer fields is stored as `notes`. Notes are free-form text and may be empty.

### Commands

| Cmd | Meaning | `arg1` | `arg2` | `arg3` |
|-----|---------|--------|--------|--------|
| `M` | Load mob in room | mob vnum | world limit | room vnum |
| `O` | Load object in room | obj vnum | limit | room vnum |
| `G` | Give object to previous mob | obj vnum | limit | *(none)* |
| `E` | Equip object on previous mob | obj vnum | limit | wear_loc (0–30) |
| `D` | Set door state | room vnum | door dir (0–5) | state (0=open, 1=closed, 2=locked) |
| `R` | Randomize exits | room vnum | max door | *(none)* |
| `P` | Put object in container | obj vnum | limit | container obj vnum |
| `A` | Obsolete; recognized but ignored | — | — | — |

### `E` wear_loc values

| Value | Slot | Value | Slot |
|------:|------|------:|------|
| 0 | light | 16 | hands |
| 1 | halo | 17 | left finger |
| 2 | aura | 18 | right finger |
| 3 | horns | 19 | claws |
| 4 | head | 20 | left hold |
| 5 | face | 21 | right hold |
| 6 | beak | 22 | two-handed |
| 7 | left ear | 23 | buckler |
| 8 | right ear | 24 | about (cloak) |
| 9 | neck 1 | 25 | waist |
| 10 | neck 2 | 26 | body |
| 11 | wings | 27 | tail |
| 12 | shoulders | 28 | legs |
| 13 | arms | 29 | feet |
| 14 | left wrist | 30 | hooves |
| 15 | right wrist | | |

`wear_loc` 31 (`clan_colors`) and 32 (`invasion_emblem`) are runtime-only and must not appear in `E` resets → rejection.

### Rejection rules

- Blank line within `#RESETS` → rejection.
- Unknown command letter → rejection.
- `M` arg1 (mob vnum): must be in this file or in the database → rejection.
- `M` arg3 (room vnum): must be in this file or in the database → rejection.
- `O` arg1 (obj vnum): must be in this file or in the database → rejection.
- `O` arg3 (room vnum): must be in this file or in the database → rejection.
- `G` arg1 (obj vnum): must be in this file or in the database → rejection.
- `E` arg1 (obj vnum): must be in this file or in the database → rejection.
- `E` arg3 (wear_loc): must be 0–30 → rejection.
- `E` arg1 object's `wear_flags` must include the bit for the specified `wear_loc` → rejection.
- `D` arg1 (room vnum): must be in this file or in the database → rejection.
- `D` arg2 (door): must be 0–5 → rejection.
- `D` arg3 (state): must be 0, 1, or 2 → rejection.
- `D` state 2 (locked): the exit at that room/direction must have `EX_ISDOOR` set, and a valid key vnum must be defined → rejection.
- `P` arg1 (obj vnum): must be in this file or in the database → rejection.
- `P` arg3 (container obj vnum): must be in this file or in the database → rejection.
- `G` or `E` with no preceding `M` reset in the same area's reset list → rejection.
- Door reset for state `closed` or `locked`: both sides of the door must have a matching `D` reset → rejection if only one side is reset.
- Any vnum in a reset command that is outside the area's `V` range and not in the database → rejection.

---

## 11. Vnum Constraints

All vnum constraints are enforced before the DB transaction.

- Every `#ROOMS`, `#MOBILES`, and `#OBJECTS` vnum must be within `[V_min, V_max]` inclusive → rejection.
- No vnum may duplicate an existing vnum of the same type in the database → rejection (with error naming the conflict).
- Cross-type overlap (same integer used as a room vnum and a mob vnum) is allowed.
- Vnums within a section must be assigned in ascending sequential order → rejection if out of order.
- Gaps in sequences are allowed but should be minimized.

---

## 12. Quest Design Requirements

Every area submitted via ingestion must satisfy all of the following → rejection otherwise:

- At least one mob in `#MOBILES` must be flagged `boss`.
- At least one mob flagged `boss` must have a loot table (`l`/`L` extension).
- At least one quest in the area's quest definitions must target a boss mob.
- At least one cartography quest must be defined, unless the area header includes a note explicitly waiving this (contact project maintainer).
- The final quest in any quest chain must reward a piece of equipment.
- Any quest targeting a boss must reward a piece of equipment.

---

## 13. Structural Constraints

### 13.1. Section termination

- `#ROOMS` must end with `#0` before the next section or `#$` → rejection.
- `#MOBILES`, if present, must end with `#0` → rejection.
- `#OBJECTS`, if present, must end with `#0` → rejection.
- `#SHOPS` must end with a line containing only `0` → rejection.
- `#SPECIALS` must end with a line containing only `S` → rejection.
- `#OBJFUNS` must end with a line containing only `S` → rejection.
- `#RESETS` must end with a line containing only `S` → rejection.

### 13.2. Blank lines

- Blank lines between section headers are allowed.
- Blank lines within `#RESETS` are a rejection.
- Blank lines within `#OBJECTS` between records are a rejection.
- Blank lines within `#MOBILES` between records are a rejection.
- Blank lines between other section content and their terminators are a rejection.

### 13.3. `area.lst` ordering

When the area is successfully ingested and added to the runtime area list, its position in the in-memory area list is determined by `V_min`. Areas are kept in non-decreasing order by `V_min`. The ingestion validator verifies that the new area's `V` range does not conflict with any existing area.

---

## 14. Canonical Section Order

Area files must be authored in this section order:

1. `#AREA`
2. `#ROOMS`
3. `#MOBILES`
4. `#OBJECTS`
5. `#SHOPS`
6. `#RESETS`
7. `#SPECIALS`
8. `#OBJFUNS`
9. `#$`

A file with sections in any other order is a rejection.

---

## 15. Database Authoring

Once the database migration is complete, new areas are authored directly via SQL and the ingestion pipeline. See the per-entity specs (`room_spec.md`, `mob_spec.md`, `object_spec.md`) for field-level SQL examples.

The `area/incoming/` pipeline accepts a `.are` file, validates it against this spec, imports it into the database transactionally, hot-loads it into the running server, and deletes the staging file. See `docs/proposals/database-schema-areas.md §10` for the full pipeline design.
