# Proposal: Move Area, Help, Shelp, Lore, Data, and Player Files to PostgreSQL

**Status:** Draft
**Branch:** `claude/database-schema-areas-vEeBw`

---

## 1. Problem Statement & Motivation

ACK!TNG currently stores all game content in flat text files:

- **45 `.are` files** — area data (rooms, mobs, objects, resets, shops, specials)
- **422 `help/` files** — in-game player help entries
- **401 `shelp/` files** — staff/spell/skill help entries
- **144 `lore/` files** — runtime world lore entries served to players in-game
- **9 `data/` files** — runtime server state: bans, clans, socials, corpses, brands, room marks, rulers, system data
- **`player/` directory** — one flat text file per player character

All six stores share the same friction points as the game world grows:

- **No referential integrity.** A reset can reference a mob vnum that doesn't exist. Cross-area exits can point nowhere. There is no enforcement layer.
- **No queryability.** Finding all mobs with a given act flag, all help entries with a given keyword, or all lore entries for a specific city requires `grep` over hundreds of files.
- **No atomic updates.** Editing any content requires rewriting an entire file and rebooting or reloading. There is no transactional safety.
- **No auditability.** Git diffs are human-readable but carry no structured metadata (who changed what, when, why).
- **Tooling friction.** Future Claude sessions, web-based editors, and admin tools must all re-implement the same bespoke parsers. A database provides a single source of truth with a standard access layer.
- **No remote access.** The database must be queryable from a machine other than the game host — flat files and embedded databases (SQLite) cannot satisfy this requirement.

The goal of this proposal is to define a PostgreSQL database schema and migration strategy that:

1. Captures 100% of the semantic content currently stored in `.are`, `help/`, `shelp/`, `lore/`, `data/`, and `player/` files.
2. Allows future Claude sessions (and any other tooling) to read and write content via standard SQL from any machine.
3. Keeps the C server's load path simple — replacing file I/O with `libpq` calls.
4. Is fully reversible: the database can regenerate the original flat files for inspection or rollback.

---

## 2. Scope

### In Scope
- All 45 `.are` files listed in `area/area.lst`
- All `.are` section types: `#AREA`, `#ROOMS`, `#MOBILES`, `#OBJECTS`, `#SHOPS`, `#RESETS`, `#SPECIALS`, `#OBJFUNS`
- All `.are` sub-records: room exits, extra descriptions, object affects, mobile loot tables, AI prompts
- All 422 `help/` files
- All 401 `shelp/` files
- All 144 `lore/` runtime files (the files the server reads at boot; `docs/lore/` source files are **not** touched)
- All `data/` runtime state files: `bans.lst`, `brands.lst`, `corpses.lst`, `roommarks.lst`, `rulers.lst`, `clandata.dat`, `socials.txt`, `sysdat.bln`, `system.dat`
- All player character files under `player/`
- A unified migration tool (`src/tools/import_to_db.c`) to import all six content stores
- An export tool (`src/tools/db_to_files.c`) to regenerate flat files from the database
- Updated load functions in `src/db.c` and `src/save/` to read from PostgreSQL
- Unit tests for the migration and export round-trip

### Out of Scope
- `docs/lore/` — the human-authored source documents; these remain in `docs/lore/` and in version control unchanged
- Online area editing commands (OLC) — a follow-on proposal
- `data/training/` and `data/knowledge/` — generated ML datasets, not server runtime state
- `data/chest/` — player-owned in-game chests (complex nested format; follow-on proposal)

---

## 3. Technology Choice: PostgreSQL

PostgreSQL is chosen because:

- **Network protocol.** PostgreSQL listens on TCP port 5432 and is accessible from any machine on the network, satisfying the remote-access requirement that ruled out SQLite.
- **ACID transactions.** Writes are atomic; a partial update cannot corrupt content.
- **Full SQL.** Views, CTEs, window functions, `LIKE`, `SIMILAR TO`, and full-text search are all available for content queries.
- **Mature C API.** `libpq` is the standard C client library, well-documented, available on every Linux distribution, and links cleanly with GCC.
- **Foreign keys enforced by default.** No pragma required; referential integrity is on from the start.
- **Schema introspection.** `\d tablename` and `information_schema` allow any tool to discover the schema without reading source code.
- **Standard tooling.** `psql`, pgAdmin, DBeaver, TablePlus — any SQL client works.

### Connection Configuration

The C server reads the connection string from `area/db.conf`, a single line in libpq connection-string format:

```
host=db.example.com port=5432 dbname=acktng user=ack password=secret sslmode=require
```

The standard PostgreSQL environment variables (`PGHOST`, `PGPORT`, `PGDATABASE`, `PGUSER`, `PGPASSWORD`) are also supported as a fallback (libpq reads them automatically when `db.conf` is absent). `area/db.conf` is gitignored.

---

## 4. Database Schema

All vnums and integer flags are `INTEGER` (32-bit) unless noted as `BIGINT` (64-bit). All text strings are `TEXT`. Auto-increment primary keys use `SERIAL`. Timestamps use `TIMESTAMP WITH TIME ZONE`.

### 4.1 `areas` Table

Corresponds to the `#AREA` section header.

```sql
CREATE TABLE areas (
    id          SERIAL  PRIMARY KEY,
    name        TEXT    NOT NULL,     -- display name
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
    owner       TEXT,                 -- O directive
    can_read    TEXT,                 -- R directive (ACL string)
    can_write   TEXT,                 -- W directive (ACL string)
    music       TEXT,                 -- C directive (mp3 filename)
    flags       INTEGER NOT NULL DEFAULT 0,  -- P/T/B/S/M boolean directives as bits
    UNIQUE(min_vnum),
    UNIQUE(max_vnum),
    CHECK(min_vnum <= max_vnum)
);
```

### 4.2 `rooms` Table

```sql
CREATE TABLE rooms (
    vnum         INTEGER PRIMARY KEY,
    area_id      INTEGER NOT NULL REFERENCES areas(id),
    name         TEXT    NOT NULL,
    description  TEXT    NOT NULL,
    room_flags   INTEGER NOT NULL DEFAULT 0,
    sector_type  INTEGER NOT NULL DEFAULT 0
);
```

### 4.3 `room_exits` Table

```sql
CREATE TABLE room_exits (
    id          SERIAL  PRIMARY KEY,
    room_vnum   INTEGER NOT NULL REFERENCES rooms(vnum),
    direction   INTEGER NOT NULL,  -- 0=N 1=E 2=S 3=W 4=U 5=D
    dest_vnum   INTEGER,           -- NULL = no exit; not FK (cross-area exits)
    exit_flags  INTEGER NOT NULL DEFAULT 0,
    key_vnum    INTEGER,           -- NULL or 0 = no key
    keyword     TEXT,
    description TEXT,
    UNIQUE(room_vnum, direction)
);
```

### 4.4 `room_extra_descs` Table

```sql
CREATE TABLE room_extra_descs (
    id          SERIAL  PRIMARY KEY,
    room_vnum   INTEGER NOT NULL REFERENCES rooms(vnum),
    keyword     TEXT    NOT NULL,
    description TEXT    NOT NULL
);
```

### 4.5 `mobiles` Table

```sql
CREATE TABLE mobiles (
    vnum          INTEGER PRIMARY KEY,
    area_id       INTEGER NOT NULL REFERENCES areas(id),
    player_name   TEXT    NOT NULL,
    short_descr   TEXT    NOT NULL,
    long_descr    TEXT    NOT NULL,
    description   TEXT    NOT NULL,
    act_flags     BIGINT  NOT NULL DEFAULT 0,  -- 64-bit act bitvector
    affected_by   INTEGER NOT NULL DEFAULT 0,
    alignment     INTEGER NOT NULL DEFAULT 0,
    level         INTEGER NOT NULL DEFAULT 1,
    sex           INTEGER NOT NULL DEFAULT 0,
    hp_mod        INTEGER NOT NULL DEFAULT 0,
    ac_mod        INTEGER NOT NULL DEFAULT 0,
    hr_mod        INTEGER NOT NULL DEFAULT 0,
    dr_mod        INTEGER NOT NULL DEFAULT 0,
    -- '!' extension fields
    class         INTEGER NOT NULL DEFAULT 0,
    clan          INTEGER NOT NULL DEFAULT 0,
    race          INTEGER NOT NULL DEFAULT 0,
    position      INTEGER NOT NULL DEFAULT 0,
    skills        INTEGER NOT NULL DEFAULT 0,
    cast          INTEGER NOT NULL DEFAULT 0,
    def           INTEGER NOT NULL DEFAULT 0,
    -- '|' extension fields
    strong_magic  INTEGER NOT NULL DEFAULT 0,
    weak_magic    INTEGER NOT NULL DEFAULT 0,
    race_mods     INTEGER NOT NULL DEFAULT 0,
    power_skills  INTEGER NOT NULL DEFAULT 0,
    power_cast    INTEGER NOT NULL DEFAULT 0,
    resist        INTEGER NOT NULL DEFAULT 0,
    suscept       INTEGER NOT NULL DEFAULT 0,
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
    ai_knowledge  INTEGER NOT NULL DEFAULT 0,
    ai_prompt     TEXT,
    -- loot table (denormalized to match MAX_LOOT=9 fixed array in C)
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
- `act_flags` is `BIGINT` (64-bit signed). The C field is `unsigned long long`; values using bit 63 would be stored as negative. The load code must cast via `(unsigned long long)(int64_t)value` when reading.
- The loot table is denormalized (9 fixed columns each for vnums and chances) to match the fixed `MAX_LOOT=9` array in the C struct.

### 4.6 `objects` Table

```sql
CREATE TABLE objects (
    vnum         INTEGER PRIMARY KEY,
    area_id      INTEGER NOT NULL REFERENCES areas(id),
    name         TEXT    NOT NULL,
    short_descr  TEXT    NOT NULL,
    description  TEXT    NOT NULL,
    item_type    INTEGER NOT NULL,
    extra_flags  BIGINT  NOT NULL DEFAULT 0,  -- 64-bit
    wear_flags   INTEGER NOT NULL DEFAULT 0,
    item_apply   INTEGER NOT NULL DEFAULT 0,
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

### 4.7 `object_extra_descs` Table

```sql
CREATE TABLE object_extra_descs (
    id          SERIAL  PRIMARY KEY,
    obj_vnum    INTEGER NOT NULL REFERENCES objects(vnum),
    keyword     TEXT    NOT NULL,
    description TEXT    NOT NULL
);
```

### 4.8 `object_affects` Table

```sql
CREATE TABLE object_affects (
    id        SERIAL  PRIMARY KEY,
    obj_vnum  INTEGER NOT NULL REFERENCES objects(vnum),
    location  INTEGER NOT NULL,
    modifier  INTEGER NOT NULL
);
```

### 4.9 `shops` Table

```sql
CREATE TABLE shops (
    id           SERIAL  PRIMARY KEY,
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

### 4.10 `resets` Table

```sql
CREATE TABLE resets (
    id        SERIAL  PRIMARY KEY,
    area_id   INTEGER NOT NULL REFERENCES areas(id),
    seq       INTEGER NOT NULL,  -- 1-based; preserves file order
    command   CHAR(1) NOT NULL CHECK(command IN ('M','O','G','E','D','R','P','A')),
    ifflag    INTEGER NOT NULL DEFAULT 0,
    arg1      INTEGER NOT NULL DEFAULT 0,
    arg2      INTEGER NOT NULL DEFAULT 0,
    arg3      INTEGER NOT NULL DEFAULT 0,
    notes     TEXT,
    auto_msg  TEXT,
    UNIQUE(area_id, seq)
);
```

**Note:** `seq` is critical. Reset semantics are positional: `G` and `E` commands apply to the most recently loaded mob from a preceding `M` command. The server must load resets `ORDER BY seq`.

### 4.11 `mobile_specials` Table

```sql
CREATE TABLE mobile_specials (
    mob_vnum  INTEGER PRIMARY KEY REFERENCES mobiles(vnum),
    spec_name TEXT    NOT NULL
);
```

### 4.12 `object_functions` Table

```sql
CREATE TABLE object_functions (
    obj_vnum  INTEGER PRIMARY KEY REFERENCES objects(vnum),
    fun_name  TEXT    NOT NULL
);
```

### 4.13 `help_entries` Table

Corresponds to files in `help/`. One row per file.

```sql
CREATE TABLE help_entries (
    id       SERIAL  PRIMARY KEY,
    filename TEXT    NOT NULL UNIQUE,  -- source filename (e.g. "alias")
    level    INTEGER NOT NULL,         -- trust level required to see this help
    keywords TEXT    NOT NULL,         -- space-separated keyword list
    body     TEXT    NOT NULL          -- full help text (everything after "---")
);
```

### 4.14 `shelp_entries` Table

Corresponds to files in `shelp/`. One row per file.

```sql
CREATE TABLE shelp_entries (
    id       SERIAL  PRIMARY KEY,
    filename TEXT    NOT NULL UNIQUE,  -- e.g. "shelp_fireball"
    level    INTEGER NOT NULL,
    keywords TEXT    NOT NULL,
    body     TEXT    NOT NULL
);
```

### 4.15 `lore_topics` Table

Corresponds to individual runtime files in `lore/` (not `docs/lore/`). One row per file; holds the shared keyword list.

```sql
CREATE TABLE lore_topics (
    id       SERIAL PRIMARY KEY,
    filename TEXT   NOT NULL UNIQUE,  -- e.g. "cinderteeth"
    keywords TEXT   NOT NULL          -- space-separated keyword list
);
```

### 4.16 `lore_entries` Table

Each row is one flagged entry within a topic file. The default (unflagged) entry has `flags = 0`.

```sql
CREATE TABLE lore_entries (
    id        SERIAL  PRIMARY KEY,
    topic_id  INTEGER NOT NULL REFERENCES lore_topics(id),
    seq       INTEGER NOT NULL,         -- 1-based order within topic (1 = default)
    flags     BIGINT  NOT NULL DEFAULT 0,  -- lore flags bitvector (0 = default entry)
    body      TEXT    NOT NULL,
    UNIQUE(topic_id, seq)
);
```

**Notes:**
- `flags` encodes city and race bits as defined in `docs/lore_file_spec.md` (bits 0–4 = cities, bits 5–14 = races).
- The default entry always has `seq = 1` and `flags = 0`. The server selects the best match by flag specificity at runtime — this logic is unchanged; only the data source changes.

### 4.17 `bans` Table

Corresponds to `data/bans.lst`. One row per ban entry.

```sql
CREATE TABLE bans (
    id         SERIAL  PRIMARY KEY,
    ban_type   INTEGER NOT NULL DEFAULT 0,  -- 0=IP, 1=prefix, 2=all
    address    TEXT    NOT NULL,            -- IP or hostname pattern
    banned_by  TEXT    NOT NULL DEFAULT ''  -- immortal who set the ban
);
```

### 4.18 `socials` Table

Corresponds to `data/socials.txt`. One row per social command. The count line at the top of the file is derived from `COUNT(*)` and not stored.

```sql
CREATE TABLE socials (
    id            SERIAL PRIMARY KEY,
    name          TEXT   NOT NULL UNIQUE,
    char_no_arg   TEXT   NOT NULL DEFAULT '',
    others_no_arg TEXT   NOT NULL DEFAULT '',
    char_found    TEXT   NOT NULL DEFAULT '',
    others_found  TEXT   NOT NULL DEFAULT '',
    vict_found    TEXT   NOT NULL DEFAULT '',
    char_auto     TEXT   NOT NULL DEFAULT '',
    others_auto   TEXT   NOT NULL DEFAULT ''
);
```

Each tilde-terminated field from the file maps to one column. The nine fields correspond to the nine `~`-terminated lines in `socials.txt` order.

### 4.19 `clans` Table

Corresponds to `data/clandata.dat`. The file stores a fixed 11-clan array with per-clan war matrices and counters.

```sql
CREATE TABLE clans (
    id            INTEGER PRIMARY KEY,   -- 0-based clan index (matches C array)
    name          TEXT    NOT NULL DEFAULT '',
    war_count     INTEGER NOT NULL DEFAULT 0,
    win_count     INTEGER NOT NULL DEFAULT 0,
    loss_count    INTEGER NOT NULL DEFAULT 0,
    member_count  INTEGER NOT NULL DEFAULT 0,
    gold          INTEGER NOT NULL DEFAULT 0,
    -- war relationship matrix (11 slots × 11 slots stored as flat arrays)
    war_matrix    INTEGER[] NOT NULL DEFAULT '{}'  -- 11 elements
);
```

**Note:** `war_matrix` is a PostgreSQL `INTEGER[]` array of 11 elements (one slot per possible opponent clan). The C load code reads this as a fixed-size `int[MAX_CLAN][MAX_CLAN]` matrix.

### 4.20 `rulers` Table

Corresponds to `data/rulers.lst`. Stores named ruler entries (empty file = no rulers).

```sql
CREATE TABLE rulers (
    id    SERIAL PRIMARY KEY,
    name  TEXT   NOT NULL UNIQUE
);
```

### 4.21 `brands` Table

Corresponds to `data/brands.lst`. Each brand record documents an item that has been branded by an immortal.

```sql
CREATE TABLE brands (
    id          SERIAL PRIMARY KEY,
    branded_by  TEXT   NOT NULL,
    item_name   TEXT   NOT NULL,
    brand_date  TEXT   NOT NULL,
    description TEXT   NOT NULL DEFAULT ''
);
```

### 4.22 `room_marks` Table

Corresponds to `data/roommarks.lst`. Stores persistent room marks set by players or staff.

```sql
CREATE TABLE room_marks (
    id        SERIAL  PRIMARY KEY,
    room_vnum INTEGER NOT NULL,
    mark_text TEXT    NOT NULL
);
```

### 4.23 `corpses` Table

Corresponds to `data/corpses.lst`. Each row is one persisted corpse object with its nested inventory.

```sql
CREATE TABLE corpses (
    id           SERIAL  PRIMARY KEY,
    where_vnum   INTEGER NOT NULL,
    nest         INTEGER NOT NULL DEFAULT 0,  -- nesting depth (0 = top-level)
    name         TEXT    NOT NULL,
    short_descr  TEXT    NOT NULL,
    description  TEXT    NOT NULL,
    vnum         INTEGER NOT NULL DEFAULT 0,
    extra_flags  BIGINT  NOT NULL DEFAULT 0,
    wear_flags   INTEGER NOT NULL DEFAULT 0,
    wear_loc     INTEGER NOT NULL DEFAULT -1,
    class_flags  INTEGER NOT NULL DEFAULT 0,
    item_type    INTEGER NOT NULL DEFAULT 0,
    weight       INTEGER NOT NULL DEFAULT 0,
    level        INTEGER NOT NULL DEFAULT 0,
    timer        INTEGER NOT NULL DEFAULT 0,
    cost         INTEGER NOT NULL DEFAULT 0,
    value_0      INTEGER NOT NULL DEFAULT 0,
    value_1      INTEGER NOT NULL DEFAULT 0,
    value_2      INTEGER NOT NULL DEFAULT 0,
    value_3      INTEGER NOT NULL DEFAULT 0,
    parent_id    INTEGER REFERENCES corpses(id)  -- NULL = top-level corpse
);
```

Nested inventory objects (items inside a corpse container) are stored as rows with `parent_id` pointing to their containing corpse row.

### 4.24 `sysdata` Table

Corresponds to `data/sysdat.bln` and `data/system.dat`. Stores server-wide scalar configuration values as a single row.

```sql
CREATE TABLE sysdata (
    id             INTEGER PRIMARY KEY DEFAULT 1 CHECK(id = 1),  -- singleton
    -- system.dat fields (tilde-terminated strings)
    mud_name       TEXT    NOT NULL DEFAULT '',
    admin_email    TEXT    NOT NULL DEFAULT '',
    login_msg      TEXT    NOT NULL DEFAULT '',
    motd           TEXT    NOT NULL DEFAULT '',
    welcome        TEXT    NOT NULL DEFAULT '',
    news           TEXT    NOT NULL DEFAULT '',
    int_val_1      INTEGER NOT NULL DEFAULT 0,
    int_val_2      INTEGER NOT NULL DEFAULT 0,
    -- sysdat.bln fields (8 integers)
    bln_val_0      INTEGER NOT NULL DEFAULT 0,
    bln_val_1      INTEGER NOT NULL DEFAULT 0,
    bln_val_2      INTEGER NOT NULL DEFAULT 0,
    bln_val_3      INTEGER NOT NULL DEFAULT 0,
    bln_val_4      INTEGER NOT NULL DEFAULT 0,
    bln_val_5      INTEGER NOT NULL DEFAULT 0,
    bln_val_6      INTEGER NOT NULL DEFAULT 0,
    bln_val_7      INTEGER NOT NULL DEFAULT 0
);
INSERT INTO sysdata (id) VALUES (1);
```

The `CHECK(id = 1)` constraint enforces the singleton pattern — there is exactly one system data row.

### 4.25 `players` Table

Corresponds to flat files under `player/<initial>/<Name>`. One row per player character. Fields map 1:1 to the sections written by `fwrite_char()` in `src/save/save_players.c`.

```sql
CREATE TABLE players (
    id              SERIAL  PRIMARY KEY,
    name            TEXT    NOT NULL UNIQUE,
    pwd_hash        TEXT    NOT NULL,          -- crypt(3) hash
    title           TEXT    NOT NULL DEFAULT '',
    description     TEXT    NOT NULL DEFAULT '',
    race            INTEGER NOT NULL DEFAULT 0,
    sex             INTEGER NOT NULL DEFAULT 0,
    class           INTEGER NOT NULL DEFAULT 0,
    level           INTEGER NOT NULL DEFAULT 0,
    trust           INTEGER NOT NULL DEFAULT 0,
    played          INTEGER NOT NULL DEFAULT 0,  -- seconds of play time
    last_login      TIMESTAMP WITH TIME ZONE,
    hit             INTEGER NOT NULL DEFAULT 0,
    max_hit         INTEGER NOT NULL DEFAULT 0,
    mana            INTEGER NOT NULL DEFAULT 0,
    max_mana        INTEGER NOT NULL DEFAULT 0,
    move            INTEGER NOT NULL DEFAULT 0,
    max_move        INTEGER NOT NULL DEFAULT 0,
    gold            INTEGER NOT NULL DEFAULT 0,
    exp             INTEGER NOT NULL DEFAULT 0,
    act_flags       BIGINT  NOT NULL DEFAULT 0,
    affected_by     INTEGER NOT NULL DEFAULT 0,
    position        INTEGER NOT NULL DEFAULT 0,
    practice        INTEGER NOT NULL DEFAULT 0,
    quest_points    INTEGER NOT NULL DEFAULT 0,
    str             INTEGER NOT NULL DEFAULT 0,
    int_            INTEGER NOT NULL DEFAULT 0,  -- "int" is a reserved word
    wis             INTEGER NOT NULL DEFAULT 0,
    dex             INTEGER NOT NULL DEFAULT 0,
    con             INTEGER NOT NULL DEFAULT 0,
    str_mod         INTEGER NOT NULL DEFAULT 0,
    int_mod         INTEGER NOT NULL DEFAULT 0,
    wis_mod         INTEGER NOT NULL DEFAULT 0,
    dex_mod         INTEGER NOT NULL DEFAULT 0,
    con_mod         INTEGER NOT NULL DEFAULT 0,
    -- serialised blobs for variable-length sub-structures
    skills          JSONB   NOT NULL DEFAULT '{}',  -- learned[] array: {name: pct}
    affects         JSONB   NOT NULL DEFAULT '[]',  -- AFFECT_DATA list
    inventory       JSONB   NOT NULL DEFAULT '[]',  -- worn + carried objects (recursive)
    -- raw save file preserved for round-trip fidelity during transition
    raw_save        TEXT
);
```

**Notes:**
- `skills` is a JSONB object mapping skill name → learned percentage (e.g. `{"sword": 75, "dodge": 50}`).
- `affects` is a JSONB array of affect records: `[{"type": 3, "duration": 20, "location": 1, "modifier": 5, "bitvector": 0}]`.
- `inventory` is a JSONB array of object trees mirroring the C `OBJ_DATA` structure, with nested `contains` arrays for containers. The full object schema matches the fields in `corpses` (§4.23) plus wear location.
- `raw_save` stores the original flat-file text verbatim. During Phase 6 (transition), the server can fall back to re-parsing `raw_save` if any field is missing or malformed. It is set to NULL once the row has been fully round-tripped.

### 4.26 `schema_version` Table

```sql
CREATE TABLE schema_version (
    version    INTEGER                  NOT NULL,
    applied_at TIMESTAMP WITH TIME ZONE NOT NULL DEFAULT NOW()
);
INSERT INTO schema_version (version) VALUES (1);
```

---

## 5. Per-Area Views

Each area gets a set of named views generated automatically during migration. These provide logical isolation — working on Midgaard means querying `midgaard_rooms`, `midgaard_mobiles`, etc. — while the data physically lives in a single schema with full cross-area foreign key support.

Views are named `<keyword>_<entity>` where `keyword` is the area's `keyword` field (lowercased, spaces replaced with underscores). For areas without a keyword, the area `name` is slugified instead.

### 5.1 View Pattern

```sql
-- Example for the area with keyword "midgaard"
CREATE VIEW midgaard_rooms AS
    SELECT * FROM rooms
    WHERE area_id = (SELECT id FROM areas WHERE keyword = 'midgaard');

CREATE VIEW midgaard_mobiles AS
    SELECT * FROM mobiles
    WHERE area_id = (SELECT id FROM areas WHERE keyword = 'midgaard');

CREATE VIEW midgaard_objects AS
    SELECT * FROM objects
    WHERE area_id = (SELECT id FROM areas WHERE keyword = 'midgaard');

CREATE VIEW midgaard_resets AS
    SELECT * FROM resets
    WHERE area_id = (SELECT id FROM areas WHERE keyword = 'midgaard')
    ORDER BY seq;

CREATE VIEW midgaard_shops AS
    SELECT s.* FROM shops s
    JOIN mobiles m ON s.keeper_vnum = m.vnum
    WHERE m.area_id = (SELECT id FROM areas WHERE keyword = 'midgaard');

CREATE VIEW midgaard_room_exits AS
    SELECT re.* FROM room_exits re
    JOIN rooms r ON re.room_vnum = r.vnum
    WHERE r.area_id = (SELECT id FROM areas WHERE keyword = 'midgaard');

CREATE VIEW midgaard_room_extra_descs AS
    SELECT red.* FROM room_extra_descs red
    JOIN rooms r ON red.room_vnum = r.vnum
    WHERE r.area_id = (SELECT id FROM areas WHERE keyword = 'midgaard');

CREATE VIEW midgaard_object_affects AS
    SELECT oa.* FROM object_affects oa
    JOIN objects o ON oa.obj_vnum = o.vnum
    WHERE o.area_id = (SELECT id FROM areas WHERE keyword = 'midgaard');

CREATE VIEW midgaard_object_extra_descs AS
    SELECT oed.* FROM object_extra_descs oed
    JOIN objects o ON oed.obj_vnum = o.vnum
    WHERE o.area_id = (SELECT id FROM areas WHERE keyword = 'midgaard');

CREATE VIEW midgaard_mobile_specials AS
    SELECT ms.* FROM mobile_specials ms
    JOIN mobiles m ON ms.mob_vnum = m.vnum
    WHERE m.area_id = (SELECT id FROM areas WHERE keyword = 'midgaard');

CREATE VIEW midgaard_object_functions AS
    SELECT of.* FROM object_functions of
    JOIN objects o ON of.obj_vnum = o.vnum
    WHERE o.area_id = (SELECT id FROM areas WHERE keyword = 'midgaard');
```

### 5.2 View Generation

Views are created by `import_to_db.c` immediately after importing each area, and can be regenerated at any time:

```sh
./tools/db_to_files --views-only
```

### 5.3 Cross-Area Queries

```sql
-- All mobs named 'guard' across all areas
SELECT m.vnum, m.short_descr, a.name AS area
FROM mobiles m JOIN areas a ON m.area_id = a.id
WHERE m.player_name ILIKE '%guard%';

-- All cross-area exits
SELECT re.room_vnum, re.dest_vnum, a1.name AS from_area, a2.name AS to_area
FROM room_exits re
JOIN rooms r  ON re.room_vnum = r.vnum
JOIN areas a1 ON r.area_id = a1.id
LEFT JOIN rooms r2  ON re.dest_vnum = r2.vnum
LEFT JOIN areas a2  ON r2.area_id = a2.id
WHERE a1.id IS DISTINCT FROM a2.id;
```

---

## 6. Full Schema DDL

The complete `CREATE TABLE` and `CREATE VIEW` statements, in dependency order, are stored in `area/schema.sql`. This file is the authoritative schema definition. The server applies it on first connect if the `schema_version` table does not exist; otherwise it checks that the current version matches the compiled-in expected version and refuses to start if they differ.

The schema covers all six content stores: areas (§4.1–§4.12), help/shelp (§4.13–§4.14), lore (§4.15–§4.16), data/ runtime state (§4.17–§4.24), and player characters (§4.25).

---

## 7. Migration Strategy

### 7.1 Import Tool: `src/tools/import_to_db.c`

A standalone C program (not linked into the server binary) that imports all six content stores:

1. Reads `area/area.lst` and parses all `.are` files using the same logic as `db.c` loaders.
2. Reads all files from `help/` and parses the `level / keywords / --- / body` format.
3. Reads all files from `shelp/` with the same format.
4. Reads all files from `lore/` (the runtime files, **not** `docs/lore/`) and parses the multi-entry `keywords / [flags / ---] / body` format.
5. Reads all `data/` runtime state files: `bans.lst`, `brands.lst`, `corpses.lst`, `roommarks.lst`, `rulers.lst`, `clandata.dat`, `socials.txt`, `sysdat.bln`, `system.dat`.
6. Reads all player character files under `player/<initial>/<Name>` using the same field-by-field parsing as `fread_char()` in `src/save/save_players.c`; stores the raw file text in `players.raw_save` as a fallback.
7. Writes everything to PostgreSQL using `libpq` prepared statements inside a single transaction per content store.
8. Generates per-area views after all areas are inserted.
9. Reports validation errors (duplicate vnums, malformed files, unknown flags, unresolvable player skill names) to stderr.

Build target: `make tools/import_to_db`
Usage: `./tools/import_to_db "host=... dbname=acktng user=ack"`

### 7.2 Export Tool: `src/tools/db_to_files.c`

A standalone C program that reads from PostgreSQL and regenerates the original flat files:

- `.are` files → `area/export/` (same section order, tilde terminators, field layout)
- `help/` files → `help/export/`
- `shelp/` files → `shelp/export/`
- `lore/` files → `lore/export/` (multi-entry format with flags lines)
- `data/` files → `data/export/` (each file in its original format)
- `player/` files → `player/export/<initial>/<Name>` (same field layout as original save files)

The exported files are semantically identical to the originals. This serves as a round-trip regression test and a rollback path.

Build target: `make tools/db_to_files`
Flags: `--areas-only`, `--help-only`, `--lore-only`, `--data-only`, `--players-only`, `--views-only`

### 7.3 Server Load Path Changes

#### `src/db.c` — area, help, shelp, lore

The existing file-based loaders are replaced with PostgreSQL-backed equivalents:

```
db_load_areas_from_db()      -- replaces load_area() + area.lst enumeration
db_load_rooms_from_db()      -- replaces load_rooms()
db_load_mobiles_from_db()    -- replaces load_mobiles()
db_load_objects_from_db()    -- replaces load_objects()
db_load_resets_from_db()     -- replaces load_resets()  [ORDER BY seq]
db_load_shops_from_db()      -- replaces load_shops()
db_load_specials_from_db()   -- replaces load_specials()
db_load_objfuns_from_db()    -- replaces load_objfuns()
db_load_helps_from_db()      -- replaces file-based help/ loading
db_load_shelps_from_db()     -- replaces file-based shelp/ loading
db_load_lore_from_db()       -- replaces file-based lore/ loading
```

#### `src/save/` — data/ runtime state and player files

The existing `src/save/save_*.c` file-based functions are replaced with PostgreSQL-backed equivalents:

```
db_load_bans()               -- replaces load_bans()       in save_area_files.c
db_load_socials()            -- replaces load_socials()    in save_socials.c
db_load_clans()              -- replaces load_clans()      in save_area_files.c
db_load_rulers()             -- replaces load_rulers()     in save_rulers.c
db_load_brands()             -- replaces load_brands()     (data/brands.lst)
db_load_room_marks()         -- replaces load_room_marks() (data/roommarks.lst)
db_load_corpses()            -- replaces load_corpses()    (data/corpses.lst)
db_load_sysdata()            -- replaces load_sysdata()    in save_sysdata.c
db_load_char_obj()           -- replaces load_char_obj()   in save_players.c
db_save_char_obj()           -- replaces save_char_obj()   in save_players.c (live writes)
db_save_bans()               -- replaces save_bans()       (live writes)
db_save_socials()            -- replaces save_socials()    (live writes)
db_save_clans()              -- replaces save_clans()      (live writes)
db_save_corpses()            -- replaces save_corpses()    (live writes)
```

Unlike area/help/shelp/lore (which are read-only at boot), the data/ and player/ stores are written during normal play. The DB-backed save functions must update the database immediately on each write event — the existing file-write semantics (write to `.temp`, then rename) are replaced with a single `UPDATE` or `INSERT ... ON CONFLICT DO UPDATE` within a transaction.

#### Boot sequence

1. Open the connection using `area/db.conf` (falling back to PG environment variables).
2. Check `schema_version` — abort if it does not match `DB_SCHEMA_VERSION` compiled into the binary.
3. Run all `db_load_*` functions in dependency order: areas → rooms → mobiles → objects → resets → shops → specials → objfuns → helps → shelps → lore → bans → socials → clans → rulers → brands → room marks → corpses → sysdata.
4. Close the boot-time connection. Player loads (`db_load_char_obj`) and live saves (`db_save_*`) open short-lived connections for each operation. (A connection pool is out of scope for this proposal.)

### 7.4 Migration Phases

| Phase | Action | Reversible? |
|-------|--------|-------------|
| 1 | Provision PostgreSQL instance; create `acktng` database and `ack` role | Yes |
| 2 | Add `libpq` as a build dependency (`-lpq`) | Yes |
| 3 | Write `area/schema.sql` and apply it | Yes |
| 4 | Write and run `import_to_db` on all six content stores; verify row counts | Yes |
| 5 | Write `db_to_files`; verify round-trip diff against originals | Yes |
| 6 | Write new `db.c` and `save/` DB load/save functions behind `#ifdef USE_DB_LOAD` | Yes |
| 7 | Run integration tests with `USE_DB_LOAD` enabled | Yes |
| 8 | Remove `#ifdef` and old file-based load/save code | No (originals retained in `*/legacy/`) |

---

## 8. Affected Files

| File | Change |
|------|--------|
| `src/db.c` | Replace all file-based area/help/shelp/lore loaders with `libpq`-backed equivalents |
| `src/save/save_players.c` | Replace `load_char_obj`/`save_char_obj` with DB-backed equivalents |
| `src/save/save_socials.c` | Replace `load_socials`/`save_socials` with DB-backed equivalents |
| `src/save/save_rulers.c` | Replace `load_rulers`/`save_rulers` with DB-backed equivalents |
| `src/save/save_sysdata.c` | Replace `load_sysdata`/`save_sysdata` with DB-backed equivalents |
| `src/save/save_area_files.c` | Replace `load_bans`/`save_bans`, `load_clans`/`save_clans` with DB-backed equivalents |
| `src/Makefile` | Add `-lpq` to `LIBS`; add `tools/` build targets |
| `src/tools/import_to_db.c` | New: import all six content stores |
| `src/tools/db_to_files.c` | New: export all six content stores |
| `src/tests/test_db_roundtrip.c` | New: unit test for import→export round-trip |
| `area/schema.sql` | New: canonical schema DDL |
| `area/db.conf` | New: libpq connection string (gitignored) |
| `area/area.lst` | Retired (kept in `area/legacy/`) |
| `area/legacy/` | New: all original `.are` files |
| `help/legacy/` | New: all original `help/` files |
| `shelp/legacy/` | New: all original `shelp/` files |
| `lore/legacy/` | New: all original `lore/` runtime files |
| `data/legacy/` | New: all original `data/` runtime state files |
| `player/legacy/` | New: snapshot of all player files at migration time |
| `docs/area_db_spec.md` | New: promoted from this proposal after implementation |
| `docs/help_file_spec.md` | Update: add database authoring section |
| `docs/lore_file_spec.md` | Update: add database authoring section |
| `docs/player_file_spec.md` | New: document player save file format and DB schema |

**Not affected:** `docs/lore/` — source lore documents remain unchanged in version control.

`area/db.conf` is added to `.gitignore`. The canonical source of truth for schema structure is `area/schema.sql`; for content, the database itself is authoritative (the flat files in `*/legacy/` serve only as the migration input and rollback archive).

---

## 9. Integration Testing Strategy

The existing integration test (`integration-test.sh`) boots the server, walks the full new-player login flow over WebSocket, and monitors for crashes. It currently assumes the server can boot by reading flat files from `area/`. The DB migration breaks this assumption: once the flat-file load code is removed, the server needs a live PostgreSQL instance to boot at all.

The strategy is divided into two phases matching the migration phases in Section 7.4.

### 9.1 Phases 1–7: Flat-file fallback (transitional)

During migration Phases 1–7, the new DB load functions live behind `#ifdef USE_DB_LOAD` while the flat-file loaders remain in the binary. The server checks for `area/db.conf` at boot:

- If `area/db.conf` **exists** → open the PostgreSQL connection and use the DB loaders.
- If `area/db.conf` **is absent** → fall back to the flat-file loaders.

The integration test never places a `db.conf`, so it always takes the flat-file path. No changes to `integration-test.sh` or CI are required through Phase 7. The DB code path is exercised separately by the round-trip unit test (`test_db_roundtrip.c`) and by manual developer testing against a local PostgreSQL instance.

### 9.2 Phase 8+: Self-contained PostgreSQL in `integration-test.sh`

When Phase 8 is reached (flat-file load code removed, `#ifdef USE_DB_LOAD` stripped), the integration test must provide its own PostgreSQL instance. The test script gains a DB setup phase that runs before the server boots, and tears down cleanly on exit.

#### Changes to `integration-test.sh`

**New dependency check (near the top):**

```sh
# Require PostgreSQL tools for the DB-backed integration test.
if ! command -v initdb >/dev/null 2>&1 || ! command -v pg_ctl >/dev/null 2>&1; then
    echo "integration-test: FAILED - initdb/pg_ctl not found (install postgresql)"
    exit 1
fi
```

**New variables:**

```sh
PG_DATA="/tmp/mud-test-pgdata-$$"
PG_PORT=$(python3 -c \
    "import socket; s=socket.socket(); s.bind(('', 0)); print(s.getsockname()[1]); s.close()")
DB_CONF="$AREA_DIR/db.conf"
```

**Extended `cleanup` function:**

```sh
cleanup() {
    # ... existing MUD_PID kill ...
    if [ -n "$PG_DATA" ] && [ -d "$PG_DATA" ]; then
        pg_ctl stop -D "$PG_DATA" -m immediate -s 2>/dev/null || true
        rm -rf "$PG_DATA"
    fi
    rm -f "$DB_CONF"
    rm -f "$LOG_FILE"
}
```

**New Step 1a — spin up a temp PostgreSQL cluster:**

```sh
echo "integration-test: initializing temp PostgreSQL cluster..."
initdb -D "$PG_DATA" -U ack --no-instructions -q
cat >>"$PG_DATA/postgresql.conf" <<EOF
port = $PG_PORT
unix_socket_directories = '/tmp'
EOF
pg_ctl start -D "$PG_DATA" -l "$PG_DATA/pg.log" -s -w
createdb -h /tmp -p "$PG_PORT" -U ack acktng

echo "integration-test: applying schema and importing content..."
psql -h /tmp -p "$PG_PORT" -U ack -d acktng -f "$AREA_DIR/schema.sql" -q
"$SCRIPT_DIR/tools/import_to_db" "host=/tmp port=$PG_PORT dbname=acktng user=ack"

# Write db.conf so the server finds the cluster.
echo "host=/tmp port=$PG_PORT dbname=acktng user=ack" > "$DB_CONF"
```

The existing build, launch, wait-for-ready, WebSocket login, and crash-monitor steps are unchanged. The `cleanup` trap handles teardown whether the test passes or fails.

#### Why this approach

- **Self-contained.** No external service required. The test spins up and tears down its own PostgreSQL instance in `/tmp`. Works identically in CI and on any developer machine.
- **No CI service dependencies.** The GitHub Actions Ubuntu runner ships `postgresql` in its default tool cache (`postgresql-*` packages, `initdb`/`pg_ctl` on `PATH`). No workflow changes are needed beyond confirming `postgresql` is installed — which it is by default on `ubuntu-latest`.
- **Exercises the real code path.** The test runs the actual `import_to_db` tool and the actual `db_load_*` functions, not stubs.
- **Adds ~10–15 seconds** to test runtime (cluster init + import). Acceptable given the existing 90-second boot timeout budget.

#### Local developer setup

No special setup is required to run `make unit-tests` locally — the integration test manages its own cluster. The only prerequisite is that `postgresql` is installed:

```sh
# Debian/Ubuntu
sudo apt install postgresql

# macOS (Homebrew)
brew install postgresql
```

#### Validation query run at the end of import

After `import_to_db` completes, the tool prints a summary row count. The integration test checks the exit code of `import_to_db` and aborts if it is non-zero:

```sh
if ! "$SCRIPT_DIR/tools/import_to_db" \
        "host=/tmp port=$PG_PORT dbname=acktng user=ack"; then
    echo "integration-test: FAILED - import_to_db failed"
    exit 1
fi
```

---

## 10. Trade-offs and Risks

### Advantages
- Referential integrity enforced at the database layer.
- All content queryable via standard SQL from any machine.
- Transactional writes: partial updates cannot corrupt content.
- Future tooling (web editor, admin panel, Claude sessions) needs no bespoke parser.
- Help, lore, player, and area data are all queryable in joins.
- Player character data gains ACID durability — no more half-written `.temp` files.

### Risks and Mitigations

| Risk | Mitigation |
|------|-----------|
| PostgreSQL not available on build host | Available in every major Linux distro (`apt install postgresql libpq-dev`) |
| Network partition between game host and DB host at boot | Server aborts cleanly with a clear error message; keep DB on LAN or same host if desired |
| Network partition during live player save | `db_save_char_obj()` retries once; on second failure logs and falls back to writing `raw_save` text to a local emergency `.plr` file |
| Migration loses data (parsing edge cases) | Round-trip test: import all files, export back, diff against originals; `raw_save` column preserved during transition |
| `resets.seq` ordering fragile if rows are reordered | Load always uses `ORDER BY seq`; `seq` is immutable once set |
| 64-bit `act_flags` / `extra_flags` stored as signed `BIGINT` | C load code casts via `(unsigned long long)(int64_t)`; audit areas for bit-63 usage before migration |
| `db.conf` contains credentials | Gitignored; use `sslmode=require` and a restricted DB role with SELECT/INSERT/UPDATE/DELETE on all tables |
| Player inventory JSONB is opaque to SQL queries | A follow-on `player_items` normalisation table can be added without changing the server code path |
| `data/chest/` contains nested objects not covered by this proposal | `data/chest/` left as flat files; added to future work |

---

## 11. Future Work (Out of Scope)

- **Online Area Editor (OLC):** Admin commands to edit rooms/mobs/objects in-game, writing directly to the database.
- **Area hot-reload:** Re-loading a single area from the database without a full reboot.
- **Area versioning:** A `changelog` table recording who changed what and when.
- **Web-based editor:** A REST API over the database for browser-based area and lore editing.
- **Full-text search:** PostgreSQL `tsvector` indexes over room descriptions and lore bodies.

---

## 12. Implementation Checklist

For the implementing Claude session:

**Setup**
- [ ] Provision PostgreSQL; create `acktng` database and `ack` role with appropriate grants
- [ ] Add `area/db.conf` to `.gitignore`
- [ ] Add `-lpq` to `src/Makefile` `LIBS`
- [ ] Write `area/schema.sql` with all DDL from Section 4 (all 26 tables)
- [ ] Apply `schema.sql` to the database

**Migration tools**
- [ ] Write `src/tools/import_to_db.c`: import areas, help, shelp, `lore/` (runtime), `data/`, `player/`
- [ ] Write `src/tools/db_to_files.c`: export all six stores + `--views-only`
- [ ] Write `src/tests/test_db_roundtrip.c`: unit test
- [ ] Run import; verify row counts against file counts for all six stores
- [ ] Run export; diff against originals (no semantic differences expected)

**Server integration — area/help/shelp/lore**
- [ ] Write `db_load_*` functions in `src/db.c` behind `#ifdef USE_DB_LOAD`
- [ ] Run `make unit-tests` with `USE_DB_LOAD` enabled (flat-file fallback active)

**Server integration — data/ and player/**
- [ ] Write `db_load_*` / `db_save_*` functions in `src/save/` behind `#ifdef USE_DB_LOAD`
- [ ] Implement emergency fallback in `db_save_char_obj()` (write raw `.plr` on DB failure)
- [ ] Run `make unit-tests` with `USE_DB_LOAD` enabled

**Phase 8 cut-over**
- [ ] Remove `#ifdef USE_DB_LOAD` and old file-based load/save code
- [ ] Update `integration-test.sh` with PostgreSQL setup/teardown phase (Section 9.2)
- [ ] Verify `make unit-tests` still passes (integration test now spins up its own cluster)
- [ ] Move flat files to `*/legacy/` directories (`area/`, `help/`, `shelp/`, `lore/`, `data/`, `player/`)

**Documentation**
- [ ] Write `docs/player_file_spec.md` documenting player save format and DB schema
- [ ] Update `docs/area_db_spec.md` with finalized schema
- [ ] Update `docs/help_file_spec.md` with database authoring workflow
- [ ] Update `docs/lore_file_spec.md` with database authoring workflow (runtime `lore/` only; `docs/lore/` unchanged)
