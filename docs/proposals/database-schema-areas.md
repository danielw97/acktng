# Proposal: Move Area Data to SQLite Database

**Status:** Draft
**Branch:** `claude/database-schema-areas-vEeBw`

---

## 1. Problem Statement & Motivation

ACK!TNG currently stores all area data — rooms, mobs, objects, resets, shops, and specials — in 45 hand-edited `.are` text files. This works, but creates friction as the game world grows:

- **No referential integrity.** A reset can reference a mob vnum that doesn't exist; the server silently skips it or crashes. Cross-area exit vnums can point nowhere. There is no enforcement layer.
- **No queryability.** Finding all mobs with a given act flag, all rooms in a given sector type, or all objects of a given item type requires `grep` over 45 files.
- **No atomic updates.** Editing an area requires rewriting the entire `.are` file and hot-rebooting the server. There is no transactional safety.
- **No auditability.** Git diffs of `.are` files are human-readable but contain no structured metadata (who changed what, when, why).
- **Tooling friction.** Future Claude sessions, web-based area editors, or admin tools must all re-implement the same bespoke parser. A database provides a single source of truth with a standard access layer.
- **Performance ceiling.** At boot, the server parses all 45 files sequentially. A database allows indexed lookups and partial loading.

The goal of this proposal is to define a SQLite database schema and migration strategy that:

1. Captures 100% of the semantic content currently stored in `.are` files.
2. Allows future Claude sessions (and any other tooling) to read and write area data via standard SQL.
3. Keeps the C server's load path simple — replacing file I/O with SQLite calls using the existing `sqlite3` library.
4. Is fully reversible: the database can regenerate `.are` files for inspection or rollback.

---

## 2. Scope

### In Scope
- All 45 `.are` files listed in `area/area.lst`
- All section types: `#AREA`, `#ROOMS`, `#MOBILES`, `#OBJECTS`, `#SHOPS`, `#RESETS`, `#SPECIALS`, `#OBJFUNS`
- All sub-records: room exits, extra descriptions, object affects, mobile loot tables, AI prompts
- A migration tool (`src/tools/are_to_db.c`) to import existing `.are` files into the database
- An export tool (`src/tools/db_to_are.c`) to regenerate `.are` files from the database
- Updated `src/db.c` load functions to read from the database instead of flat files
- Unit tests for the migration and export round-trip

### Out of Scope
- Player save files (`player/` directory) — these remain as flat files
- Runtime data files (`data/` directory — bans, clans, socials, etc.)
- The `help/` and `shelp/` directories
- Online area editing commands (OLC) — a follow-on proposal

---

## 3. Technology Choice: SQLite

SQLite is chosen for the following reasons:

- **Zero server infrastructure.** The database is a single file (`area/areas.db`). No daemon, no authentication, no network port.
- **Single-file distribution.** The game already ships as a single directory; `areas.db` fits that model.
- **C API.** The `sqlite3` C library is well-documented, widely available, and links cleanly with GCC.
- **ACID transactions.** Writes are atomic; a partial update (e.g., mid-reset load crash) cannot corrupt the database.
- **Schema introspection.** Future tools can `SELECT * FROM sqlite_master` to discover the schema without reading source code.
- **No licensing friction.** SQLite is public domain.

The database file will live at `area/areas.db`. The existing `.are` files will be retained as a reference archive in `area/legacy/` after migration, but will not be read by the server.

---

## 4. Database Schema

The schema below maps every field from the current `.are` format to a normalized relational structure. All vnums are stored as `INTEGER` (SQLite's native 64-bit integer). All text strings are `TEXT`. Bitvectors are stored as `INTEGER` (64-bit where needed for the `act` field on mobiles).

### 4.1 `areas` Table

Corresponds to the `#AREA` section header.

```sql
CREATE TABLE areas (
    id          INTEGER PRIMARY KEY,  -- internal row id
    name        TEXT    NOT NULL,     -- display name (tilde-terminated in .are)
    min_vnum    INTEGER NOT NULL,     -- V directive lower bound
    max_vnum    INTEGER NOT NULL,     -- V directive upper bound
    keyword     TEXT,                 -- K directive
    level_label TEXT,                 -- L directive
    area_number INTEGER,              -- N directive
    level_min   INTEGER,              -- I directive lower bound
    level_max   INTEGER,              -- I directive upper bound
    map_offset  INTEGER,              -- X directive
    reset_rate  INTEGER,              -- F directive (ticks between resets)
    reset_msg   TEXT,                 -- U directive
    owner       TEXT,                 -- O directive (should be 'Virant')
    can_read    TEXT,                 -- R directive (ACL string)
    can_write   TEXT,                 -- W directive (ACL string)
    music       TEXT,                 -- C directive (mp3 filename)
    flags       INTEGER DEFAULT 0,   -- area flags bitvector (pay/teleport/building/etc.)
    UNIQUE(min_vnum),
    UNIQUE(max_vnum),
    CHECK(min_vnum <= max_vnum)
);
```

**Notes:**
- `flags` encodes the boolean directives `P` (pay-area), `T` (teleport), `B` (building), `S` (no-show), `M` (no-room-affects) as individual bits.
- `UNIQUE(min_vnum)` and `UNIQUE(max_vnum)` enforce non-overlapping vnum envelopes at the database level.

### 4.2 `rooms` Table

Corresponds to entries in `#ROOMS`.

```sql
CREATE TABLE rooms (
    vnum         INTEGER PRIMARY KEY,
    area_id      INTEGER NOT NULL REFERENCES areas(id),
    name         TEXT    NOT NULL,
    description  TEXT    NOT NULL,
    room_flags   INTEGER NOT NULL DEFAULT 0,  -- bitvector
    sector_type  INTEGER NOT NULL DEFAULT 0
);
```

### 4.3 `room_exits` Table

Corresponds to `D<n>` exit records within a room.

```sql
CREATE TABLE room_exits (
    id          INTEGER PRIMARY KEY,
    room_vnum   INTEGER NOT NULL REFERENCES rooms(vnum),
    direction   INTEGER NOT NULL,  -- 0=N, 1=E, 2=S, 3=W, 4=U, 5=D
    dest_vnum   INTEGER,           -- destination room vnum (may be cross-area; NULL = no exit)
    exit_flags  INTEGER NOT NULL DEFAULT 0,  -- locks bitvector (door/closed/locked/etc.)
    key_vnum    INTEGER,           -- key object vnum (0 = no key)
    keyword     TEXT,              -- door keyword(s)
    description TEXT,              -- exit description
    UNIQUE(room_vnum, direction)
);
```

**Notes:**
- `dest_vnum` is not a foreign key because exits may point to rooms in other areas that are loaded in a different order. Referential integrity is enforced at load time by the C code (existing behavior).
- `key_vnum` of 0 or NULL means no key required.

### 4.4 `room_extra_descs` Table

Corresponds to `E` extra-description records on rooms.

```sql
CREATE TABLE room_extra_descs (
    id          INTEGER PRIMARY KEY,
    room_vnum   INTEGER NOT NULL REFERENCES rooms(vnum),
    keyword     TEXT    NOT NULL,
    description TEXT    NOT NULL
);
```

### 4.5 `mobiles` Table

Corresponds to entries in `#MOBILES`.

```sql
CREATE TABLE mobiles (
    vnum          INTEGER PRIMARY KEY,
    area_id       INTEGER NOT NULL REFERENCES areas(id),
    player_name   TEXT    NOT NULL,   -- keyword list (e.g. "guard city")
    short_descr   TEXT    NOT NULL,   -- e.g. "a city guard"
    long_descr    TEXT    NOT NULL,   -- one-line room description + newline
    description   TEXT    NOT NULL,   -- full description block
    act_flags     INTEGER NOT NULL DEFAULT 0,   -- 64-bit act bitvector
    affected_by   INTEGER NOT NULL DEFAULT 0,   -- bitvector
    alignment     INTEGER NOT NULL DEFAULT 0,
    level         INTEGER NOT NULL DEFAULT 1,
    sex           INTEGER NOT NULL DEFAULT 0,   -- 0=neutral, 1=male, 2=female
    hp_mod        INTEGER NOT NULL DEFAULT 0,
    ac_mod        INTEGER NOT NULL DEFAULT 0,
    hr_mod        INTEGER NOT NULL DEFAULT 0,
    dr_mod        INTEGER NOT NULL DEFAULT 0,
    -- '!' extension fields
    class         INTEGER NOT NULL DEFAULT 0,
    clan          INTEGER NOT NULL DEFAULT 0,
    race          INTEGER NOT NULL DEFAULT 0,
    position      INTEGER NOT NULL DEFAULT 0,
    skills        INTEGER NOT NULL DEFAULT 0,   -- bitvector
    cast          INTEGER NOT NULL DEFAULT 0,   -- bitvector
    def           INTEGER NOT NULL DEFAULT 0,   -- bitvector
    -- '|' extension fields
    strong_magic  INTEGER NOT NULL DEFAULT 0,   -- bitvector
    weak_magic    INTEGER NOT NULL DEFAULT 0,   -- bitvector
    race_mods     INTEGER NOT NULL DEFAULT 0,
    power_skills  INTEGER NOT NULL DEFAULT 0,
    power_cast    INTEGER NOT NULL DEFAULT 0,
    resist        INTEGER NOT NULL DEFAULT 0,   -- bitvector
    suscept       INTEGER NOT NULL DEFAULT 0,   -- bitvector
    -- '+' extension fields
    spellpower    INTEGER NOT NULL DEFAULT 0,
    crit          INTEGER NOT NULL DEFAULT 0,
    crit_mult     INTEGER NOT NULL DEFAULT 0,
    spell_crit    INTEGER NOT NULL DEFAULT 0,
    spell_mult    INTEGER NOT NULL DEFAULT 0,
    parry         INTEGER NOT NULL DEFAULT 0,
    dodge         INTEGER NOT NULL DEFAULT 0,
    block         INTEGER NOT NULL DEFAULT 0,
    pierce        INTEGER NOT NULL DEFAULT 0,
    -- 'a' AI extension fields
    ai_knowledge  INTEGER NOT NULL DEFAULT 0,   -- bitmask
    ai_prompt     TEXT,                          -- NULL if no AI prompt
    -- loot table
    loot_amount   INTEGER NOT NULL DEFAULT 0,
    loot_0        INTEGER NOT NULL DEFAULT 0,
    loot_1        INTEGER NOT NULL DEFAULT 0,
    loot_2        INTEGER NOT NULL DEFAULT 0,
    loot_3        INTEGER NOT NULL DEFAULT 0,
    loot_4        INTEGER NOT NULL DEFAULT 0,
    loot_5        INTEGER NOT NULL DEFAULT 0,
    loot_6        INTEGER NOT NULL DEFAULT 0,
    loot_7        INTEGER NOT NULL DEFAULT 0,
    loot_8        INTEGER NOT NULL DEFAULT 0,
    loot_chance_0 INTEGER NOT NULL DEFAULT 0,
    loot_chance_1 INTEGER NOT NULL DEFAULT 0,
    loot_chance_2 INTEGER NOT NULL DEFAULT 0,
    loot_chance_3 INTEGER NOT NULL DEFAULT 0,
    loot_chance_4 INTEGER NOT NULL DEFAULT 0,
    loot_chance_5 INTEGER NOT NULL DEFAULT 0,
    loot_chance_6 INTEGER NOT NULL DEFAULT 0,
    loot_chance_7 INTEGER NOT NULL DEFAULT 0,
    loot_chance_8 INTEGER NOT NULL DEFAULT 0
);
```

**Notes:**
- `act_flags` is declared as `INTEGER` but holds a 64-bit value (SQLite integers are up to 8 bytes).
- The loot table is denormalized (9 fixed columns each for vnums and chances) rather than a separate join table, matching the fixed `MAX_LOOT=9` array in the C struct. This simplifies both import and load.
- Extension line fields (`!`, `|`, `+`, `a`) default to 0/NULL; if the extension was absent in the original `.are` file that is faithfully represented.

### 4.6 `objects` Table

Corresponds to entries in `#OBJECTS`.

```sql
CREATE TABLE objects (
    vnum         INTEGER PRIMARY KEY,
    area_id      INTEGER NOT NULL REFERENCES areas(id),
    name         TEXT    NOT NULL,        -- keyword list
    short_descr  TEXT    NOT NULL,        -- e.g. "a rusty sword"
    description  TEXT    NOT NULL,        -- room-look description
    item_type    INTEGER NOT NULL,        -- item type constant (1-33)
    extra_flags  INTEGER NOT NULL DEFAULT 0,  -- 64-bit bitvector
    wear_flags   INTEGER NOT NULL DEFAULT 0,  -- bitvector
    item_apply   INTEGER NOT NULL DEFAULT 0,  -- bitvector
    value_0      INTEGER NOT NULL DEFAULT 0,
    value_1      INTEGER NOT NULL DEFAULT 0,
    value_2      INTEGER NOT NULL DEFAULT 0,
    value_3      INTEGER NOT NULL DEFAULT 0,
    value_4      INTEGER NOT NULL DEFAULT 0,
    value_5      INTEGER NOT NULL DEFAULT 0,
    value_6      INTEGER NOT NULL DEFAULT 0,
    value_7      INTEGER NOT NULL DEFAULT 0,
    value_8      INTEGER NOT NULL DEFAULT 0,
    value_9      INTEGER NOT NULL DEFAULT 0,
    weight       INTEGER NOT NULL DEFAULT 0,
    level        INTEGER NOT NULL DEFAULT 0
);
```

**Notes:**
- `value_0` through `value_9` map to the 10-element `value[]` array. Semantics depend on `item_type` (e.g., for weapons, value_0=weapon type, value_1=num dice, value_2=size dice, value_3=damage type).
- `extra_flags` is 64-bit (stored as SQLite INTEGER).

### 4.7 `object_extra_descs` Table

Corresponds to `E` records on objects.

```sql
CREATE TABLE object_extra_descs (
    id          INTEGER PRIMARY KEY,
    obj_vnum    INTEGER NOT NULL REFERENCES objects(vnum),
    keyword     TEXT    NOT NULL,
    description TEXT    NOT NULL
);
```

### 4.8 `object_affects` Table

Corresponds to `A` records on objects (stat modifiers).

```sql
CREATE TABLE object_affects (
    id        INTEGER PRIMARY KEY,
    obj_vnum  INTEGER NOT NULL REFERENCES objects(vnum),
    location  INTEGER NOT NULL,  -- apply location constant (APPLY_STR, APPLY_DEX, etc.)
    modifier  INTEGER NOT NULL   -- amount added/subtracted
);
```

### 4.9 `shops` Table

Corresponds to entries in `#SHOPS`.

```sql
CREATE TABLE shops (
    id           INTEGER PRIMARY KEY,
    keeper_vnum  INTEGER NOT NULL REFERENCES mobiles(vnum),
    buy_type_0   INTEGER NOT NULL DEFAULT 0,
    buy_type_1   INTEGER NOT NULL DEFAULT 0,
    buy_type_2   INTEGER NOT NULL DEFAULT 0,
    buy_type_3   INTEGER NOT NULL DEFAULT 0,
    buy_type_4   INTEGER NOT NULL DEFAULT 0,
    profit_buy   INTEGER NOT NULL DEFAULT 100,
    profit_sell  INTEGER NOT NULL DEFAULT 100,
    open_hour    INTEGER NOT NULL DEFAULT 0,
    close_hour   INTEGER NOT NULL DEFAULT 23,
    UNIQUE(keeper_vnum)
);
```

**Notes:**
- `buy_type_0` through `buy_type_4` map to `buy_type[MAX_TRADE]`. The value of `MAX_TRADE` must be confirmed from `typedefs.h` — the schema uses 5 slots to match the current constant.
- `profit_buy`/`profit_sell` are percentage multipliers (100 = no markup/markdown).

### 4.10 `resets` Table

Corresponds to entries in `#RESETS`.

```sql
CREATE TABLE resets (
    id        INTEGER PRIMARY KEY,
    area_id   INTEGER NOT NULL REFERENCES areas(id),
    seq       INTEGER NOT NULL,   -- ordering within the area (1-based, preserves file order)
    command   TEXT    NOT NULL CHECK(command IN ('M','O','G','E','D','R','P','A')),
    ifflag    INTEGER NOT NULL DEFAULT 0,
    arg1      INTEGER NOT NULL DEFAULT 0,
    arg2      INTEGER NOT NULL DEFAULT 0,
    arg3      INTEGER NOT NULL DEFAULT 0,
    notes     TEXT,
    auto_msg  TEXT,
    UNIQUE(area_id, seq)
);
```

**Notes:**
- `seq` preserves the original file ordering of reset commands. This is critical because reset semantics are positional: `G` and `E` commands apply to the most recently loaded mob from a preceding `M` command.
- `command` is constrained to valid single-character codes.
- `arg3` is stored for all commands (defaulting to 0 for `G` and `R` which have no third argument).

### 4.11 `mobile_specials` Table

Corresponds to entries in `#SPECIALS`.

```sql
CREATE TABLE mobile_specials (
    mob_vnum   INTEGER PRIMARY KEY REFERENCES mobiles(vnum),
    spec_name  TEXT    NOT NULL   -- e.g. "spec_breath_fire"
);
```

**Notes:**
- One row per mobile (a mobile can have at most one special function). `PRIMARY KEY` on `mob_vnum` enforces this.
- `spec_name` is the string key used to look up the function pointer in the `spec_table` array (existing C infrastructure).

### 4.12 `object_functions` Table

Corresponds to entries in `#OBJFUNS`.

```sql
CREATE TABLE object_functions (
    obj_vnum   INTEGER PRIMARY KEY REFERENCES objects(vnum),
    fun_name   TEXT    NOT NULL   -- e.g. "objfun_flaming"
);
```

### 4.13 Schema Version Table

```sql
CREATE TABLE schema_version (
    version     INTEGER NOT NULL,
    applied_at  TEXT    NOT NULL DEFAULT (datetime('now'))
);
INSERT INTO schema_version (version) VALUES (1);
```

This allows future migrations to gate on the current version.

---

## 5. Full Schema DDL (for reference)

The complete `CREATE TABLE` statements above, assembled in dependency order, will be written to `area/schema.sql`. The server will create the database from this file on first boot if `areas.db` does not exist, then refuse to start if the schema version does not match the compiled-in expected version.

---

## 6. Migration Strategy

### 6.1 Import Tool: `src/tools/are_to_db.c`

A standalone C program (not linked into the server binary) that:

1. Reads `area/area.lst` to enumerate all `.are` files.
2. For each file, runs the same parsing logic as the current `db.c` loaders (factored into shared helper functions).
3. Writes all parsed data into `area/areas.db` using `sqlite3_exec()` and prepared statements.
4. Reports any validation errors (unknown vnums, duplicate vnums, missing cross-references) to stderr.

Build target: `make tools/are_to_db`

### 6.2 Export Tool: `src/tools/db_to_are.c`

A standalone C program that reads `areas.db` and regenerates `.are` files in `area/export/`. The output is semantically identical to the originals (same section order, same field layout, tilde terminators). This serves as:

- A regression test (import → export should round-trip with no semantic diff).
- A rollback path if the database approach is abandoned.
- A human-readable snapshot for debugging.

Build target: `make tools/db_to_are`

### 6.3 Server Load Path Changes: `src/db.c`

The existing functions `load_area()`, `load_rooms()`, `load_mobiles()`, `load_objects()`, `load_resets()`, `load_shops()`, `load_specials()` are replaced with database-backed equivalents:

```
db_load_areas_from_db()      -- replaces file enumeration + load_area()
db_load_rooms_from_db()      -- replaces load_rooms()
db_load_mobiles_from_db()    -- replaces load_mobiles()
db_load_objects_from_db()    -- replaces load_objects()
db_load_resets_from_db()     -- replaces load_resets()
db_load_shops_from_db()      -- replaces load_shops()
db_load_specials_from_db()   -- replaces load_specials()
db_load_objfuns_from_db()    -- replaces load_objfuns()
```

Each function opens a single `SELECT` statement, iterates rows, and populates the in-memory linked lists and hash tables exactly as before. The `AREA_DATA`, `ROOM_INDEX_DATA`, `MOB_INDEX_DATA`, etc. structs are unchanged — only the source of data changes.

The `area.lst` file is retired; the `areas` table sorted by `min_vnum` serves the same purpose.

### 6.4 Migration Phases

| Phase | Action | Reversible? |
|-------|--------|-------------|
| 1 | Add `sqlite3` as a build dependency (link `-lsqlite3`) | Yes |
| 2 | Write `are_to_db.c` migration tool | Yes |
| 3 | Run migration on all 45 `.are` files; verify round-trip with `db_to_are` | Yes |
| 4 | Write new `db.c` load functions reading from SQLite | Yes |
| 5 | Add `#ifdef USE_DB_LOAD` compile-time switch to toggle old vs. new path | Yes |
| 6 | Run integration tests with `USE_DB_LOAD` enabled | Yes |
| 7 | Remove `#ifdef` and old file-based load code | No (but `.are` files retained in `area/legacy/`) |

Phase 5 (compile-time flag) means both load paths exist simultaneously during testing, making rollback trivial.

---

## 7. Affected Files

| File | Change |
|------|--------|
| `src/db.c` | Replace file-based loaders with SQLite-backed equivalents |
| `src/Makefile` | Add `-lsqlite3` to `LIBS`; add `tools/` build targets |
| `src/tools/are_to_db.c` | New: import tool |
| `src/tools/db_to_are.c` | New: export/round-trip tool |
| `src/tests/test_db_roundtrip.c` | New: unit test for import→export round-trip |
| `area/schema.sql` | New: canonical schema DDL |
| `area/areas.db` | New: the database (gitignored; generated by migration) |
| `area/area.lst` | Retired (kept for reference, not read by server) |
| `area/legacy/` | New directory; all original `.are` files moved here |
| `docs/area_db_spec.md` | New: this document (promoted from proposal) |

The `area/areas.db` file will be added to `.gitignore`. The canonical source of truth becomes the `.are` files in `area/legacy/` (the migration input) plus the schema in `area/schema.sql`. The database is a derived artifact that any developer can regenerate with `make migrate`.

---

## 8. Trade-offs and Risks

### Advantages
- Referential integrity enforced at the database layer (foreign keys, unique constraints).
- Area data is queryable with standard SQL tools (`sqlite3` CLI, DB Browser for SQLite, etc.).
- Transactional writes: partial updates cannot corrupt area data.
- Future tooling (web editor, admin panel, Claude sessions) can use SQL instead of a bespoke parser.
- Performance: indexed lookups by vnum are O(log n) rather than O(n) hash scan.

### Risks and Mitigations

| Risk | Mitigation |
|------|-----------|
| SQLite not available on all build hosts | Document dependency; SQLite is in every major Linux distro's default package set |
| Migration loses data (parsing edge cases) | Round-trip test: import all 45 files, export back, diff against originals |
| `reset.seq` ordering fragile if rows reordered | `seq` is immutable once set; INSERT order is preserved by `seq` column |
| 64-bit `act_flags` exceeds SQLite INTEGER range | SQLite INTEGER is 8 bytes (64-bit signed); max value 2^63-1. The `act` field in C is `unsigned long long` (64-bit). Values using bit 63 would overflow signed storage. Audit current areas for bit 63 usage before migration. |
| Increased build complexity | The `sqlite3` single-file amalgamation can be vendored into `src/` to eliminate the external dependency entirely |

### Alternative: Vendor `sqlite3` Amalgamation
To avoid the `-lsqlite3` external dependency, the `sqlite3.c` + `sqlite3.h` amalgamation (single ~250 KB C file) can be copied into `src/` and compiled directly. This makes the build fully self-contained. Given the project's existing self-contained posture, this is the recommended approach.

---

## 9. Future Work (Out of Scope for This Proposal)

- **Online Area Editor (OLC):** Admin commands to edit rooms, mobs, objects in-game, writing changes directly to the database without requiring a server restart.
- **Area versioning:** Using SQLite's WAL mode and a `changelog` table to record who changed what and when.
- **Area hot-reload:** Re-loading a single area from the database at runtime without a full hot-reboot.
- **Web-based area editor:** A REST API over the SQLite database for browser-based editing.

---

## 10. Implementation Checklist

For the implementing Claude session:

- [ ] Vendor `sqlite3` amalgamation into `src/sqlite3.c` and `src/sqlite3.h`
- [ ] Update `src/Makefile` to compile `sqlite3.c` and link it
- [ ] Write `area/schema.sql` with all `CREATE TABLE` statements from Section 4
- [ ] Write `src/tools/are_to_db.c`: import tool
- [ ] Write `src/tools/db_to_are.c`: export/round-trip tool
- [ ] Write `src/tests/test_db_roundtrip.c`: unit test
- [ ] Modify `src/db.c`: add SQLite load functions behind `#ifdef USE_DB_LOAD`
- [ ] Run `make unit-tests` with `USE_DB_LOAD` enabled
- [ ] Verify round-trip: `are_to_db` → `db_to_are` → diff against originals
- [ ] Move `.are` files to `area/legacy/`
- [ ] Add `area/areas.db` to `.gitignore`
- [ ] Remove `#ifdef USE_DB_LOAD` and old file-based load code
- [ ] Update `docs/area_db_spec.md` with finalized schema
