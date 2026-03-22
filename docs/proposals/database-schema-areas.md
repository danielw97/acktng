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
- All board files in `area/boards/` (`board.{vnum}` — one per board object)
- All player character files under `player/`
- A unified migration tool (`src/tools/import_to_db.c`) to import all six content stores
- An export tool (`src/tools/db_to_files.c`) to regenerate flat files from the database
- Updated load functions in `src/db.c` and `src/save/` to read from PostgreSQL
- Updated OLC save path in `src/save/save_area_files.c` to write to PostgreSQL instead of `.are` files
- Unit tests for the migration and export round-trip

### Out of Scope
- `docs/lore/` — the human-authored source documents; these remain in `docs/lore/` and in version control unchanged
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

The C server reads the connection string from `/data/db.conf`, a single line in libpq connection-string format:

```
host=db.example.com port=5432 dbname=acktng user=ack password=secret sslmode=require
```

The standard PostgreSQL environment variables (`PGHOST`, `PGPORT`, `PGDATABASE`, `PGUSER`, `PGPASSWORD`) are also supported as a fallback (libpq reads them automatically when `/data/db.conf` is absent). `/data/db.conf` is gitignored.

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

### 4.19 `boards` Table

Corresponds to the header section of each `area/boards/board.{vnum}` file. One row per board object — configuration only; messages are in `board_messages`.

```sql
CREATE TABLE boards (
    id            SERIAL  PRIMARY KEY,
    vnum          INTEGER NOT NULL UNIQUE,  -- matches OBJ value[3]
    expiry_days   INTEGER NOT NULL DEFAULT 10,
    min_read_lev  INTEGER NOT NULL DEFAULT 0,
    min_write_lev INTEGER NOT NULL DEFAULT 0,
    clan          INTEGER NOT NULL DEFAULT 0
);
```

`vnum` is the board's virtual number (stored in `OBJ_DATA->value[3]` for ITEM_BOARD objects). It is the stable identifier used when loading a board on demand.

### 4.20 `board_messages` Table

Corresponds to the `Messages` section of each `board.{vnum}` file.

```sql
CREATE TABLE board_messages (
    id         SERIAL      PRIMARY KEY,
    board_id   INTEGER     NOT NULL REFERENCES boards(id) ON DELETE CASCADE,
    posted_at  BIGINT      NOT NULL,   -- Unix timestamp (matches M<timestamp> line)
    author     TEXT        NOT NULL,
    title      TEXT        NOT NULL DEFAULT '',
    body       TEXT        NOT NULL DEFAULT '',
    seq        INTEGER     NOT NULL   -- original file order, for stable display order
);
CREATE INDEX board_messages_board_id_seq ON board_messages(board_id, seq);
```

`seq` preserves the original file ordering so display order matches what players are used to. `posted_at` is the raw Unix timestamp from the `M<timestamp>` line; no conversion is applied.

### 4.21 `clans` Table

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

### 4.22 `rulers` Table

Corresponds to `data/rulers.lst`. Stores named ruler entries (empty file = no rulers).

```sql
CREATE TABLE rulers (
    id    SERIAL PRIMARY KEY,
    name  TEXT   NOT NULL UNIQUE
);
```

### 4.23 `brands` Table

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

### 4.24 `room_marks` Table

Corresponds to `data/roommarks.lst`. Stores persistent room marks set by players or staff.

```sql
CREATE TABLE room_marks (
    id        SERIAL  PRIMARY KEY,
    room_vnum INTEGER NOT NULL,
    mark_text TEXT    NOT NULL
);
```

### 4.25 `corpses` Table

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
    value_4      INTEGER NOT NULL DEFAULT 0,
    value_5      INTEGER NOT NULL DEFAULT 0,
    value_6      INTEGER NOT NULL DEFAULT 0,
    value_7      INTEGER NOT NULL DEFAULT 0,
    value_8      INTEGER NOT NULL DEFAULT 0,
    value_9      INTEGER NOT NULL DEFAULT 0,
    parent_id    INTEGER REFERENCES corpses(id)  -- NULL = top-level corpse
);
```

Nested inventory objects (items inside a corpse container) are stored as rows with `parent_id` pointing to their containing corpse row.

### 4.26 `sysdata` Table

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

### 4.27 `players` Table

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

### 4.28 `keep_chests` Table

Corresponds to `data/chest/<vnum>` flat files. Each row represents one player-owned keep
chest. Chest contents are stored in `keep_chest_items` (§4.29).

```sql
CREATE TABLE keep_chests (
    id           SERIAL  PRIMARY KEY,
    vnum         INTEGER NOT NULL UNIQUE,  -- object prototype vnum (the chest container)
    owner_name   TEXT    NOT NULL,         -- owning player's name
    max_items    INTEGER NOT NULL DEFAULT 50,  -- current storage-tier capacity (value[3])
    created_at   TIMESTAMP WITH TIME ZONE NOT NULL DEFAULT NOW(),
    updated_at   TIMESTAMP WITH TIME ZONE NOT NULL DEFAULT NOW()
);
```

- `vnum` is the object prototype vnum allocated by `create_keep_chest_index()`. It falls
  within the keep area's vnum range.
- `owner_name` matches `ch->name` of the owning player. Case-sensitive; must match the
  player name exactly.
- `max_items` mirrors `chest->value[3]` — the current storage tier capacity. Starts at
  the result of `keep_chest_max_items()` (50) and increases by 5 per storage upgrade.
- `updated_at` is refreshed on every `save_chest()` call.

---

### 4.29 `keep_chest_items` Table

Each row is one object inside a keep chest (or nested inside a container within a chest).
The structure mirrors `fwrite_chest` output, extended to cover all ten value slots.

```sql
CREATE TABLE keep_chest_items (
    id          SERIAL  PRIMARY KEY,
    chest_id    INTEGER NOT NULL REFERENCES keep_chests (id) ON DELETE CASCADE,
    nest        INTEGER NOT NULL DEFAULT 0,  -- 0 = direct chest content; >0 = inside a container
    parent_id   INTEGER REFERENCES keep_chest_items (id),  -- NULL for top-level items
    name        TEXT    NOT NULL,
    short_descr TEXT    NOT NULL,
    description TEXT    NOT NULL,
    vnum        INTEGER NOT NULL DEFAULT 0,  -- object prototype vnum; 0 for temp/generated
    extra_flags BIGINT  NOT NULL DEFAULT 0,
    wear_flags  INTEGER NOT NULL DEFAULT 0,
    wear_loc    INTEGER NOT NULL DEFAULT -1,
    class_flags INTEGER NOT NULL DEFAULT 0,  -- item_apply bitvector
    item_type   INTEGER NOT NULL DEFAULT 0,
    weight      INTEGER NOT NULL DEFAULT 0,
    level       INTEGER NOT NULL DEFAULT 0,
    timer       INTEGER NOT NULL DEFAULT -1,
    cost        INTEGER NOT NULL DEFAULT 0,
    value_0     INTEGER NOT NULL DEFAULT 0,
    value_1     INTEGER NOT NULL DEFAULT 0,
    value_2     INTEGER NOT NULL DEFAULT 0,
    value_3     INTEGER NOT NULL DEFAULT 0,
    value_4     INTEGER NOT NULL DEFAULT 0,
    value_5     INTEGER NOT NULL DEFAULT 0,
    value_6     INTEGER NOT NULL DEFAULT 0,
    value_7     INTEGER NOT NULL DEFAULT 0,
    value_8     INTEGER NOT NULL DEFAULT 0,
    value_9     INTEGER NOT NULL DEFAULT 0,
    objfun      TEXT,                        -- objfun name string; NULL if none
    sort_order  INTEGER NOT NULL DEFAULT 0   -- preserves insertion order within a nest level
);
```

Notes:

- `nest = 0`: item is directly inside the keep chest container.
- `nest > 0`: item is inside a container that is itself inside the chest. `parent_id`
  points to the containing `keep_chest_items` row.
- `sort_order` preserves the order items appear in the chest so round-trip fidelity is
  maintained on export.
- `value_0–value_3` cover all fields that the legacy `fwrite_chest` `Values` line writes.
  `value_4–value_9` are added for consistency with the `objects` table; they are
  initialized to `0` on migration from legacy files and may be populated in future.
- The legacy format wrote only `value[0]–value[3]`; this table is a strict superset.

---

### 4.30 `schema_version` Table

```sql
CREATE TABLE schema_version (
    version    INTEGER                  NOT NULL,
    applied_at TIMESTAMP WITH TIME ZONE NOT NULL DEFAULT NOW()
);
INSERT INTO schema_version (version) VALUES (1);
```

---

## 4A. Keep Chest YAML Format

Keep chests support YAML import and export independently of area files. A chest YAML file
is a standalone file (not part of an area directory) that can be used to back up, restore,
or transfer a chest's contents.

### 4A.1. File Structure

Chest YAML files are placed in `data/chest/incoming/` for import and written to
`data/chest/export/` for export. A chest YAML file contains exactly one chest and all its
nested contents.

```yaml
chest:
  vnum:       1500
  owner:      "Aerindel"
  max_items:  55
  contents:
    - vnum:        1200
      name:        "blade black coral weapon"
      short_descr: "a blade of black coral"
      description: |
        The blade is carved from a single length of black coral.
      extra_flags:  [magic, loot, boss]
      wear_flags:   [hold, take]
      wear_loc:     none
      class_flags:  [nada]
      item_type:    weapon
      weight:       6
      level:        80
      timer:        -1
      cost:         0
      values:       [0, 0, 0, 3, 0, 0, 0, 0, 0, 0]
      contents:     []
    - vnum:        1201
      name:        "small pouch leather"
      short_descr: "a small leather pouch"
      description: |
        A simple leather pouch drawn closed with a cord.
      extra_flags:  []
      wear_flags:   [take]
      wear_loc:     none
      class_flags:  [nada]
      item_type:    container
      weight:       3
      level:        0
      timer:        -1
      cost:         0
      values:       [10, 1, -1, 0, 0, 0, 0, 0, 0, 0]
      contents:
        - vnum:       1210
          name:       "coin gold"
          short_descr: "a gold coin"
          description: |
            A standard gold coin bearing the city seal.
          extra_flags:  []
          wear_flags:   [take]
          wear_loc:     none
          class_flags:  [nada]
          item_type:    money
          weight:       1
          level:        0
          timer:        -1
          cost:         1
          values:       [100, 0, 0, 0, 0, 0, 0, 0, 0, 0]
          contents:     []
```

### 4A.2. YAML Field Definitions

**Top-level `chest` mapping:**

| Field | Required | Notes |
|-------|----------|-------|
| `vnum` | Yes | The chest's prototype vnum. Must exist in the database as an object with `ITEM_CONTAINER` and `CONT_KEEP_CHEST` set. |
| `owner` | Yes | Player name string. |
| `max_items` | Yes | Current storage capacity integer. |
| `contents` | Yes | Sequence of item mappings. May be empty `[]`. |

**Per-item mapping (recursive; same structure for nested items via `contents`):**

| Field | Required | Notes |
|-------|----------|-------|
| `vnum` | Yes | Object prototype vnum. `0` for runtime-generated objects. |
| `name` | Yes | Current name string. |
| `short_descr` | Yes | Current short description. |
| `description` | Yes | Block scalar. |
| `extra_flags` | Yes | List of extra-flag names from §III.16, or `[]`. |
| `wear_flags` | Yes | List of wear-flag names from §III.17, or `[]`. |
| `wear_loc` | Yes | Wear location name from §III.20, or `none` if not worn. |
| `class_flags` | Yes | List of item-apply names from §III.18. |
| `item_type` | Yes | Item type name from §III.15. |
| `weight` | Yes | Integer. |
| `level` | Yes | Integer. |
| `timer` | Yes | Integer. `-1` means no timer. |
| `cost` | Yes | Integer. |
| `values` | Yes | List of exactly 10 integers. |
| `contents` | Yes | Sequence of nested item mappings. `[]` if not a container or empty. |
| `objfun` | No | Objfun name string from §III.24, or omitted/null. |

### 4A.3. Import and Export Commands

**Export (wizard command):**
```
chest export <player-name>    — Write player's chest to data/chest/export/<name>.yaml
chest export all              — Export all chests to data/chest/export/
```

**Import (wizard command):**
```
chest import <player-name>    — Import data/chest/incoming/<name>.yaml into the database
                                 and hot-reload the chest in-world if the player is online
```

**Offline export tool:**
```
db_to_files --yaml --chest <player-name>     Export one chest by owner name
db_to_files --yaml --chest --vnum <N>        Export one chest by vnum
db_to_files --yaml --all-chests              Export all chests
```

Round-trip fidelity is required: `import(export(chest))` must produce an identical
database state.

---

## 4B. Keep Chest Migration Path

### 4B.1. Overview

Legacy chest data lives in `data/chest/<vnum>` flat files (one file per chest). The
migration moves these files into the database. The migration is performed once, during the
Phase 7 → Phase 8 cutover, and is non-destructive: the original flat files are renamed to
`data/chest.legacy/<vnum>` after a successful import.

### 4B.2. Migration Script

A standalone migration binary (`src/tools/migrate_chests.c`) is built alongside the server.

```
migrate_chests --dry-run          Scan data/chest/ and report what would be migrated
migrate_chests                    Migrate all chest files into the database
migrate_chests --vnum <N>         Migrate one chest file by vnum
```

The migration script:

1. Reads each `data/chest/<vnum>` file using the existing `fread_chest_item()` logic.
2. Resolves the chest prototype vnum to the owning player's name by scanning
   `data/chest/<vnum>` for a Nest=0 item and extracting its `ShortDescr` field (formatted
   as `<name>'s Keep Chest`). If the owner cannot be determined, the chest is skipped with
   a warning.
3. Inserts a `keep_chests` row for the chest.
4. Inserts `keep_chest_items` rows for each item, preserving nesting structure.
5. On success, renames `data/chest/<vnum>` to `data/chest.legacy/<vnum>`.
6. Logs each migrated chest to stdout.

Failures are non-fatal per-chest: if one chest file fails to parse, the migration logs the
error and continues with the next file.

### 4B.3. Server Boot Fallback

During the transition period (Phase 7), the server boot sequence is:

1. Try `db_load_chests()` — read chest data from the `keep_chests` /
   `keep_chest_items` tables.
2. For any chest vnum that is in the database but whose `data/chest/<vnum>` file still
   exists, skip the flat file (DB takes precedence).
3. For any chest vnum found only in `data/chest/` (not yet migrated), fall back to
   `load_chest()` (the existing flat-file loader). Log a warning for each flat-file
   fallback so operators can track migration progress.

Once all chests are migrated (no more fallbacks), `data/chest/` is empty (or contains
only `data/chest.legacy/`) and the fallback code can be removed in Phase 8.

### 4B.4. Save Path Cutover

After migration, `save_chest()` is replaced with a DB write:

```
save_chest(chest)   →   db_worker_enqueue_write(DB_WRITE_CHEST, chest)
```

The worker handler serializes the chest and its contents into a single transaction:
deletes all `keep_chest_items` rows for this chest's `chest_id`, then re-inserts the
current contents. This is safe because `keep_chest_items` has `ON DELETE CASCADE` on
`chest_id`.

The legacy `save_chest()` flat-file writer is removed once the DB write path is live and
has been verified in production.

### 4B.5. Load Path Cutover

After migration, `load_chest(vnum)` is replaced with a DB read that hydrates the
`OBJ_DATA` tree from `keep_chest_items` rows. The DB read is performed at server boot as
part of `db_load_chests()`, replacing the per-vnum `load_chest()` call in `db.c`.

### 4B.6. Checklist

- [ ] Add `keep_chests` and `keep_chest_items` tables to `schema.sql`
- [ ] Implement `migrate_chests` binary in `src/tools/`
- [ ] Run `migrate_chests --dry-run` in staging and verify all chests are found
- [ ] Run `migrate_chests` in production during a maintenance window
- [ ] Verify `db_load_chests()` in Phase 7 fallback mode loads all chests correctly
- [ ] Replace `save_chest()` with `db_worker_enqueue_write(DB_WRITE_CHEST, ...)`
- [ ] Add `DB_WRITE_CHEST`, `DB_DELETE_CHEST` to `DB_OP_TYPE` enum in `db_worker.h`
- [ ] Add `DB_WRITE_CHEST` handler to `db_worker.c`
- [ ] Replace `load_chest(vnum)` call in `db.c` with `db_load_chests()` at boot
- [ ] Implement `chest export` and `chest import` wizard commands
- [ ] Implement `db_to_files --yaml --chest` in the offline export tool
- [ ] Remove flat-file fallback code after Phase 8 cut-over is confirmed stable
- [ ] Add `docs/proposals/schema.sql` `keep_chests` and `keep_chest_items` table DDL

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

Unlike area/help/shelp/lore (which are read-only at boot), the data/ and player/ stores are written during normal play. These writes are **asynchronous** — the game thread never blocks on a database call. See §7.5 for the async writer design.

#### Boot sequence

1. Open a synchronous boot connection using `/data/db.conf` (falling back to PG environment variables).
2. Check `schema_version` — abort if it does not match `DB_SCHEMA_VERSION` compiled into the binary.
3. Run all `db_load_*` functions in dependency order: areas → rooms → mobiles → objects → resets → shops → specials → objfuns → helps → shelps → lore → bans → socials → boards → clans → rulers → brands → room marks → corpses → sysdata.
4. Close the boot connection.
5. Start the async writer thread (§7.5). All subsequent writes go through the writer queue.

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

### 7.5 Async DB Worker Architecture

The MUD runs a single-threaded game loop. A synchronous `libpq` call at any point during runtime — whether a player save or a player load at login — would block `select()` and cause visible lag for all connected players. All runtime database access (reads **and** writes) therefore goes through a dedicated **DB worker thread** that owns the `PGconn *`. The game thread never calls `libpq` after boot.

Boot-time loads (areas, rooms, mobiles, objects, lore, data/, etc.) remain **synchronous** — they execute before `game_loop()` starts, so there are no connected players to lag.

#### Components

**`src/db_worker.c` / `src/db_worker.h`** — a new module providing:

```c
/* Called at server startup, after all boot db_load_* calls complete. */
void db_worker_start(void);

/* Called at server shutdown — flushes the queue then joins the thread. */
void db_worker_stop(void);

/* Enqueue a write. Returns immediately; game thread never blocks.
 * data is deep-copied into the queue entry; caller retains ownership. */
void db_worker_enqueue_write(DB_OP_TYPE type, const void *data);

/* Enqueue an async player load for descriptor d.
 * Puts d->connected = CON_LOADING_FROM_DB.
 * Result is posted to the results queue; game_loop picks it up next tick. */
void db_worker_enqueue_load_player(DESCRIPTOR_DATA *d, const char *name);

/* Called each tick from game_loop to drain the results queue.
 * For each completed load result, advances the login state machine. */
void db_worker_poll_results(void);
```

`DB_OP_TYPE` covers all operations the worker thread executes:

```c
typedef enum {
    /* Writes */
    DB_WRITE_PLAYER,      /* save_char_obj   */
    DB_WRITE_CLANS,       /* save_clans      */
    DB_WRITE_BANS,        /* save_bans       */
    DB_WRITE_SOCIALS,     /* save_socials    */
    DB_WRITE_CORPSES,     /* save_corpses    */
    DB_WRITE_SYSDATA,     /* save_sysdata    */
    DB_WRITE_RULERS,      /* save_rulers     */
    DB_WRITE_BRANDS,      /* save_brands     */
    DB_WRITE_ROOM_MARKS,  /* save_room_marks */
    /* Reads */
    DB_READ_PLAYER,       /* load_char_obj at login */
    /* Control */
    DB_OP_SHUTDOWN,
} DB_OP_TYPE;
```

#### Request queue (game thread → worker thread)

A singly-linked list protected by a `pthread_mutex_t` with a `pthread_cond_t` for wakeup. Read requests (`DB_READ_PLAYER`) are inserted at the **head** of the queue; write requests are appended at the **tail**. This gives reads priority over pending writes so login latency is not delayed by a backlog of saves.

**Last-write-wins coalescing for player saves:** before appending a `DB_WRITE_PLAYER` entry, the queue is scanned for an existing pending write for the same character name. If found, the data is replaced in-place. This caps queue depth at one pending write per player name.

#### Results queue (worker thread → game thread)

A second lock-protected list for completed reads. Each entry holds the descriptor pointer and the loaded `CHAR_DATA *` (or an error flag if the row was not found or the query failed). The worker posts entries here after each `DB_READ_PLAYER` completes; `db_worker_poll_results()` (called once per tick) drains it on the game thread.

#### `CON_LOADING_FROM_DB` descriptor state

A new `connected` state added to the login state machine:

```
CON_GET_NAME
  → player sends name
  → db_worker_enqueue_load_player(d, name)
  → d->connected = CON_LOADING_FROM_DB
  → game loop continues processing other descriptors

(next tick or whenever result arrives)
  → db_worker_poll_results() finds result for d
  → if found:    d->character = loaded_char; d->connected = CON_GET_OLD_PASSWORD
  → if not found: d->connected = CON_GET_NEW_PASSWORD  (new character flow)
  → if DB error:  send "Try again later"; close descriptor
```

Players in `CON_LOADING_FROM_DB` receive no input processing — they are parked until the result arrives. From their perspective the delay is identical to the previous synchronous implementation (a few milliseconds), but the game loop is never stalled.

#### Flow diagram

```
Game thread                           DB worker thread
────────────────────────────────────  ────────────────────────────────────
-- Login --
player sends name
  → enqueue_load_player(d, "Virant")  (sleeping on cond)
  → d->connected = CON_LOADING_FROM_DB
  → return to game loop

-- Tick --
db_worker_poll_results()              wake on signal
  (nothing yet, continue)             lock; dequeue DB_READ_PLAYER; unlock
                                      SELECT * FROM players WHERE name=$1
                                      lock results queue; post result; unlock
-- Next tick --
db_worker_poll_results()
  finds result for d
  d->character = ch
  d->connected = CON_GET_OLD_PASSWORD

-- Save --
save_char_obj(ch)
  → serialise ch                      (sleeping on cond)
  → enqueue_write(DB_WRITE_PLAYER, …)
  → coalesce if pending entry exists
  → signal cond; return to game loop
                                      wake on signal
                                      lock; dequeue; unlock
                                      PQexecPrepared(INSERT … ON CONFLICT)
                                      on error: log + emergency .plr
```

The worker thread holds a **persistent** `PGconn *` open for the life of the server, shared across reads and writes. Since the worker is the only caller of `libpq`, no locking on the connection is needed.

#### Shutdown flush

`db_worker_stop()` enqueues `DB_OP_SHUTDOWN` at the tail of the request queue, then `pthread_join()`s. The worker drains all pending entries before processing the sentinel and exiting — no saves are lost on clean shutdown. Any in-progress player loads at shutdown time are cancelled; those descriptors are being closed anyway.

#### Emergency fallback

If the worker encounters a connection error it cannot recover from (three consecutive `PQexec` failures), it:

1. Logs the error to `log/db_worker_error.log`.
2. Drains all pending write entries to emergency flat files using the original `fwrite_char` / `save_*.c` logic (compiled in behind `#ifdef DB_WORKER_FALLBACK`).
3. Sets the global `db_worker_failed` flag; the game thread switches subsequent saves to call flat-file functions directly.
4. For any pending `DB_READ_PLAYER` entries: posts an error result so `db_worker_poll_results()` can close the affected descriptors gracefully with a "try again" message.

This keeps the server running through a DB outage at the cost of reverting to flat-file saves temporarily.

---

## 8. OLC Write Path

The in-game builder system (`src/build.c`, `src/buildare.c`, `src/save/save_area_files.c`) currently saves areas by rewriting entire `.are` files to disk. Once the database is the authoritative source for area content, this write path must be replaced with direct `libpq` updates.

### 8.1 Current Save Path

When a builder edits a room, mob, or object in-game:

1. Any `build_set*()` function calls `area_modified(pArea)`, setting `pArea->modified = 1`.
2. `build_save()` is called every game loop tick (from `update.c`). When `saving_area` is set, it opens `<area>.new`, calls `build_save_area()` → `build_save_mobs()` → `build_save_objects()` → `build_save_rooms()` → `build_save_shops()` → `build_save_resets()` → `build_save_end()`, which renames `<area>.new` over `<area>.are`.
3. `build_save_flush()` is called every `PULSE_AREA` tick. It iterates all areas with `modified == 1` and queues each for the above file-write cycle.

### 8.2 Replacement: Per-Entity DB Writes

After Phase 8 cut-over, the OLC save path is replaced as follows:

**`build_save_rooms()`** → per-room `UPDATE rooms SET ...` (or `INSERT ... ON CONFLICT UPDATE`).

**`build_save_mobs()`** → per-mob `UPDATE mobiles SET ...`.

**`build_save_objects()`** → per-object `UPDATE objects SET ...`.

**`build_save_resets()`** → `DELETE FROM resets WHERE area_id = $1` followed by bulk `INSERT INTO resets ...` for the current reset list (resets have no stable primary key — the whole list is the value).

**`build_save_shops()`** → per-shop `UPDATE shops SET ...`.

**`build_save_area()`** → `UPDATE areas SET ... WHERE id = $1`.

All of these writes go through the async DB worker (`db_worker_enqueue_write()`), exactly as player saves do. The game thread never blocks.

The `build_save_end()` file-rename logic and the `SaveFile` / `saving_area` / `SaveQ[]` queue mechanism in `save_area_files.c` are removed entirely. `area_modified()` becomes a no-op stub (or is removed).

### 8.3 OLC Creates (New Rooms/Mobs/Objects)

When a builder uses `addroom`, `addmob`, or `addobject` to create a new entity:

- The entity is allocated in memory as normal (existing `build_addroom()` etc. logic unchanged).
- A `db_worker_enqueue_write(DB_WRITE_ROOM, ...)` / `DB_WRITE_MOB` / `DB_WRITE_OBJ` call is made immediately after the entity is linked into the world.
- The worker does `INSERT INTO rooms/mobiles/objects ... ON CONFLICT DO NOTHING`.

### 8.4 OLC Deletes

When a builder deletes a room, mob, or object, a `DB_DELETE_ROOM` / `DB_DELETE_MOB` / `DB_DELETE_OBJ` operation type is enqueued. The worker issues `DELETE FROM rooms/mobiles/objects WHERE vnum = $1`.

### 8.5 Social Editor (`sedit`)

`sedit` currently calls `save_social_table()` synchronously after every edit — it rewrites the entire `data/socials.txt` file in-place. After cut-over, `save_social_table()` is replaced:

- `sedit new <name>` → `db_worker_enqueue_write(DB_WRITE_SOCIAL, ...)` using `INSERT INTO socials ...`
- `sedit delete <name>` → `db_worker_enqueue_write(DB_DELETE_SOCIAL, ...)` using `DELETE FROM socials WHERE name = $1`
- Any `sedit <field>` edit → `db_worker_enqueue_write(DB_WRITE_SOCIAL, ...)` using `UPDATE socials SET <field> = $1 WHERE name = $2`

The in-memory `social_table` array continues to be updated immediately (as it is today) so the change takes effect at once. The DB write is asynchronous and does not block the game loop.

### 8.6 Board Writes

Boards are currently written by `save_board()` in `src/board.c`, which opens and overwrites `area/boards/board.{vnum}` synchronously after any post, delete, or edit. After cut-over:

- **New post** (`finished_editing()`) → `db_worker_enqueue_write(DB_WRITE_BOARD_MESSAGE, ...)` using `INSERT INTO board_messages ...`
- **Delete message** (`do_delete()`) → `db_worker_enqueue_write(DB_DELETE_BOARD_MESSAGE, ...)` using `DELETE FROM board_messages WHERE id = $1`
- **Board config change** (if board metadata is edited) → `db_worker_enqueue_write(DB_WRITE_BOARD, ...)` using `UPDATE boards SET ... WHERE vnum = $1`

`save_board()` is removed. The in-memory `BOARD_DATA` / `MESSAGE_DATA` linked lists continue to be updated immediately before the enqueue, so players see changes at once without waiting for the DB write.

**Boot-time loading:** Unlike the current on-demand file read, boards are loaded at boot from the `boards` and `board_messages` tables (as part of the standard boot sequence in §7.3). This eliminates the edge case where the first player to touch a board triggers a file read mid-session, and allows expiry filtering to be done in SQL (`WHERE posted_at > $1`) rather than in C.

### 8.8 No `areasave` Command After Cut-Over

The `do_areasave` command in `src/act_wiz.c` becomes a no-op after Phase 8. Edits are persisted to the database immediately (via the worker queue) — there is no separate "save" step. The command can be removed or repurposed to display DB write queue depth as a diagnostic.

---

## 9. Affected Files

| File | Change |
|------|--------|
| `src/db.c` | Replace all file-based area/help/shelp/lore loaders with `libpq`-backed equivalents |
| `src/db_worker.c` | New: async DB worker thread, request/results queues, coalescing, emergency fallback |
| `src/db_worker.h` | New: `db_worker_start/stop/enqueue_write/enqueue_load_player/poll_results` API |
| `src/comm.c` | Add `db_worker_poll_results()` call in `game_loop()`; add `CON_LOADING_FROM_DB` handling |
| `src/typedefs.h` | Add `CON_LOADING_FROM_DB` to the `connected` enum |
| `src/save/save_players.c` | Replace `save_char_obj` with `db_worker_enqueue_write(DB_WRITE_PLAYER, ...)`; replace `load_char_obj` with `db_worker_enqueue_load_player()` |
| `src/save/save_socials.c` | Replace `save_social_table()` with per-operation worker enqueue (`DB_WRITE_SOCIAL`, `DB_DELETE_SOCIAL`) |
| `src/board.c` | Replace `save_board()` with `db_worker_enqueue_write(DB_WRITE_BOARD_MESSAGE / DB_DELETE_BOARD_MESSAGE)`; move board loading to boot-time `db_load_boards()` |
| `src/save/save_rulers.c` | Replace `save_rulers` with worker enqueue |
| `src/save/save_sysdata.c` | Replace `save_sysdata` with worker enqueue |
| `src/save/save_area_files.c` | Replace `save_bans`, `save_clans` with worker enqueue; replace `build_save_*` file-write functions with per-entity DB worker enqueue calls; remove `SaveFile`/`saving_area`/`SaveQ[]` queue mechanism |
| `src/build.c` | Remove `do_areasave` (or make no-op); call `db_worker_enqueue_write` from `build_addroom/mob/obj` and OLC delete paths |
| `src/act_wiz.c` | Remove or stub `do_areasave` after Phase 8 cut-over |
| `src/Makefile` | Add `-lpq -lpthread` to `LIBS`; add `tools/` and `db_worker.o` build targets |
| `src/tools/import_to_db.c` | New: import all six content stores |
| `src/tools/db_to_files.c` | New: export all six content stores |
| `src/tests/test_db_roundtrip.c` | New: unit test for import→export round-trip |
| `area/schema.sql` | New: canonical schema DDL |
| `/data/db.conf` | New: libpq connection string (gitignored) |
| `area/area.lst` | Retired (kept in `area/legacy/`) |
| `area/legacy/` | New: all original `.are` files |
| `help/legacy/` | New: all original `help/` files |
| `shelp/legacy/` | New: all original `shelp/` files |
| `lore/legacy/` | New: all original `lore/` runtime files |
| `data/legacy/` | New: all original `data/` runtime state files |
| `area/boards/legacy/` | New: all original `board.{vnum}` files |
| `player/legacy/` | New: snapshot of all player files at migration time |
| `docs/area_db_spec.md` | New: promoted from this proposal after implementation |
| `docs/help_file_spec.md` | Update: add database authoring section |
| `docs/lore_file_spec.md` | Update: add database authoring section |
| `docs/player_file_spec.md` | New: document player save file format and DB schema |

**Not affected:** `docs/lore/` — source lore documents remain unchanged in version control.

`/data/db.conf` is added to `.gitignore`. The canonical source of truth for schema structure is `area/schema.sql`; for content, the database itself is authoritative (the flat files in `*/legacy/` serve only as the migration input and rollback archive).

---

## 10. Area Ingestion Pipeline

Once the database is the authoritative source for area content, a mechanism is needed to add new areas without a full server reboot and without manually running SQL. The ingestion pipeline provides a file-drop interface: a `.are` file placed in a staging directory is validated, imported into the database, loaded into the live server, and deleted — atomically, from the server's perspective.

### 10.1 Staging Directory

New area files are placed in **`area/incoming/`**.

This co-locates the staging area with the existing `area/` directory (where the server runs from) and keeps all area-related files together. Two subdirectories are created by the server at boot if they do not exist:

```
area/incoming/          ← author drops .are files here
area/incoming/failed/   ← failed ingestions moved here, with an error log alongside
```

`area/incoming/` and `area/incoming/failed/` are both gitignored.

### 10.2 Detection: Polling in the Update Loop

The server polls `area/incoming/` on every `PULSE_AREA` tick (the same timer that drives area resets). The polling function `area_incoming_poll()` is called from `area_update()` in `update.c`.

`opendir()` / `readdir()` is used to scan for `*.are` files. Processing is limited to **one file per tick** to bound the work done in a single game loop iteration. If multiple files are present, each subsequent file is handled on the next tick.

inotify is explicitly not used. It would require either a thread (complicating the single-threaded game loop) or integration into the `select()` fd set in `comm.c`. Polling on `PULSE_AREA` is already the model for periodic file-system work in this codebase and avoids that complexity entirely.

### 10.3 Strict Format Requirements

Current `.are` file loading crashes the server on parse errors (`kill(getpid(), SIGQUIT)` or `exit(1)`). The ingestion pipeline cannot tolerate this — a malformed drop file must be rejected cleanly, not crash the live server.

The ingestion validator therefore enforces a **strict superset** of the format checked by `test_area_format.c`. Every rule below is a hard rejection (no warnings, no partial loads):

#### Required `#AREA` directives

All of the following must be present and well-formed:

| Directive | Requirement |
|-----------|-------------|
| `Q 16` | Exact string. Any other value is rejected. |
| `V <min> <max>` | Both integers present; `min < max`; `max - min >= 9` (minimum viable range). |
| `K <keyword>~` | Non-empty, lowercase, no spaces, unique across all loaded areas. |
| `L <label>~` | Non-empty. |
| `N <number>` | Integer. |
| `I <min_level> <max_level>` | Both integers; `0 <= min_level <= max_level <= 105`. |
| `O Virant~` | Exact string. Any other owner is rejected. |
| `F <rate>` | Integer `>= 1`. |

#### Vnum envelope

- The `V <min> <max>` range must **not overlap** any existing area already in the database. Overlap is tested as `new_min <= existing_max AND new_max >= existing_min`.
- Every room, mobile, and object vnum in the file must fall within `[V_min, V_max]` inclusive.
- Vnum ranges are not pre-provisioned — the submitter is responsible for choosing a non-conflicting range. The error message on conflict names the existing area whose range it overlaps.

#### Section requirements

- `#ROOMS` must be present and contain at least one room.
- All string fields must be tilde-terminated with no embedded bare tildes.
- No blank lines within `#RESETS` (blank lines are valid between sections, not within them).
- Mobile `long_descr` must follow the exact format: one line of text, then a newline, then `~` on its own line.
- No double newlines (`\n\n`) within any tilde-delimited string.
- Exit destination vnums must reference either: (a) a room in the same file, or (b) a room vnum already present in the `rooms` table. Forward references within the same file are allowed; references to non-existent rooms in external areas are rejected.
- Reset `M` and `O` commands must reference vnums defined in this file or already in the database.
- `#SPECIALS` entries must name a valid `spec_*` function (validated against the same table `test_area_format.c` uses).

#### Encoding

- All text is UTF-8. Null bytes are rejected.
- Lines must end with `\n` (Unix line endings). `\r\n` is stripped silently; bare `\r` is rejected.

### 10.4 Validation and Ingestion Flow

Processing a file in `area/incoming/foo.are` proceeds in three phases. Any failure in any phase aborts and moves the file to `area/incoming/failed/`.

**Phase 1 — Pre-parse validation (no DB contact)**

1. Open the file. If it cannot be opened, abort.
2. Read the entire file into memory (max 4 MB; larger files are rejected).
3. Run the strict format validator (§10.3) on the in-memory buffer. This catches all structural and encoding errors without touching the database or the live world.
4. If any error is found, write `area/incoming/failed/foo.are.error` with the line number and error message, move `foo.are` to `area/incoming/failed/foo.are`, log to the server log, and return.

**Phase 2 — DB ingestion (inside a transaction)**

1. Open a synchronous `libpq` connection (same pattern as the boot connection in §7.3 — this is a rare, staff-triggered operation and blocking is acceptable).
2. `BEGIN`.
3. Call the same import logic used by `import_to_db` for the area, mob, room, object, reset, shop, specials, and objfuns tables. All inserts go into the transaction.
4. If any insert fails (constraint violation, duplicate vnum, etc.), `ROLLBACK`, write the error file, move the `.are` to `failed/`, and return.
5. `COMMIT`.

**Phase 3 — Hot-load into memory**

1. Call the existing `db_load_*` functions for the newly inserted area (same functions used at boot), selecting `WHERE area_id = <new_id>`. This populates the in-memory room, mob, and object hash tables.
2. Link the new `AREA_DATA` into the `area_list` linked list in the correct sorted position.
3. Log success: `"area_incoming: loaded area '<name>' vnums %d-%d"`.
4. Delete `area/incoming/foo.are`.

The entire ingestion — including Phase 2 and Phase 3 — runs on the game thread during a single `PULSE_AREA` tick. Phase 2 is synchronous on the boot connection pattern; the game loop stalls for the duration of the DB round-trip. This is acceptable because ingestion is rare (staff-only, infrequent) and the window is bounded by a single area's worth of inserts (hundreds of rows, not millions). A log message is emitted before and after so operators can see the stall in the log.

### 10.5 Failure Handling

On any failure:

- The `.are` file is moved to `area/incoming/failed/<filename>`.
- A companion `area/incoming/failed/<filename>.error` is written with:
  - The phase that failed (pre-parse / DB ingestion / hot-load)
  - The exact error message and line number (for pre-parse failures)
  - The PostgreSQL error string (for DB failures)
  - A timestamp
- A `log_f()` message is emitted to the server log at `LOGLEV_WARN` level.
- The database transaction is rolled back (Phase 2 failures). No partial data is left.
- The in-memory world is unaffected (Phase 3 failures leave the DB updated but the in-memory load incomplete — this is logged as a critical error and the area's DB rows are deleted to keep DB and memory in sync).

### 10.6 Schema Addition: `area_incoming_log` Table

A lightweight audit log for all ingestion attempts:

```sql
CREATE TABLE area_incoming_log (
    id          SERIAL                   PRIMARY KEY,
    filename    TEXT                     NOT NULL,
    attempted_at TIMESTAMP WITH TIME ZONE NOT NULL DEFAULT NOW(),
    success     BOOLEAN                  NOT NULL,
    area_id     INTEGER                  REFERENCES areas(id) ON DELETE SET NULL,
    error_phase TEXT,                    -- 'preparse', 'db', 'hotload', or NULL on success
    error_msg   TEXT                     -- full error text, or NULL on success
);
```

This lets operators query what was ingested and when, and diagnose past failures, without parsing log files.

### 10.7 Staff Command: `areaingest`

A new wizard command `do_areaingest` provides manual control:

```
areaingest list          — list files currently in area/incoming/
areaingest run <file>    — immediately process a specific file (bypass the poll timer)
areaingest failures      — list files in area/incoming/failed/ with error summaries
areaingest retry <file>  — move a failed file back to area/incoming/ for re-processing
areaingest purge         — delete all files in area/incoming/failed/
```

`areaingest run` is the primary workflow for interactive use: a builder drops a file into `area/incoming/`, then types `areaingest run foo.are` to ingest it immediately without waiting for the next `PULSE_AREA` tick. The poll-based auto-ingest is a background convenience for automated pipelines.

### 10.8 Affected Files (Ingestion Pipeline)

| File | Change |
|------|--------|
| `src/update.c` | Add `area_incoming_poll()` call in `area_update()` |
| `src/areaingest.c` | New: ingestion validator, DB import, hot-load, failure handling |
| `src/areaingest.h` | New: `area_incoming_poll()`, `do_areaingest()` declarations |
| `src/act_wiz.c` | Register `do_areaingest` command |
| `src/db.c` | Expose `db_load_area_by_id()` for hot-load use by ingestion |
| `area/schema.sql` | Add `area_incoming_log` table |
| `.gitignore` | Add `area/incoming/` and `area/incoming/failed/` |

### 10.9 Checklist (Ingestion Pipeline)

- [ ] Create `area/incoming/` and `area/incoming/failed/` directories; add both to `.gitignore`
- [ ] Add `area_incoming_log` table to `area/schema.sql`
- [ ] Write `src/areaingest.c`: Phase 1 validator (strict rules from §10.3), Phase 2 DB transaction, Phase 3 hot-load, failure handler
- [ ] Write `src/areaingest.h`
- [ ] Add `area_incoming_poll()` call to `area_update()` in `src/update.c`
- [ ] Implement `do_areaingest` with `list`, `run`, `failures`, `retry`, `purge` subcommands in `src/areaingest.c`
- [ ] Register `do_areaingest` in `src/act_wiz.c`
- [ ] Expose `db_load_area_by_id(PGconn *conn, int area_id)` in `src/db.c`
- [ ] Write `src/tests/test_areaingest.c`: unit tests for the Phase 1 validator (good file, each rejection case)
- [ ] Add `areaingest` help file to `shelp/`

---

## 11. Integration Testing Strategy

The existing integration test (`integration-test.sh`) boots the server, walks the full new-player login flow over WebSocket, and monitors for crashes. It currently assumes the server can boot by reading flat files from `area/`. The DB migration breaks this assumption: once the flat-file load code is removed, the server needs a live PostgreSQL instance to boot at all.

The strategy is divided into two phases matching the migration phases in Section 7.4.

### 9.1 Phases 1–7: Flat-file fallback (transitional)

During migration Phases 1–7, the new DB load functions live behind `#ifdef USE_DB_LOAD` while the flat-file loaders remain in the binary. The server checks for `/data/db.conf` at boot:

- If `/data/db.conf` **exists** → open the PostgreSQL connection and use the DB loaders.
- If `/data/db.conf` **is absent** → fall back to the flat-file loaders.

The integration test never places a `/data/db.conf`, so it always takes the flat-file path. No changes to `integration-test.sh` or CI are required through Phase 7. The DB code path is exercised separately by the round-trip unit test (`test_db_roundtrip.c`) and by manual developer testing against a local PostgreSQL instance.

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
DB_CONF="/data/db.conf"
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

# Write /data/db.conf so the server finds the cluster.
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

## 12. Trade-offs and Risks

### Advantages
- Referential integrity enforced at the database layer.
- All content queryable via standard SQL from any machine.
- Transactional writes: partial updates cannot corrupt content.
- Future tooling (web editor, admin panel, Claude sessions) needs no bespoke parser.
- Help, lore, player, and area data are all queryable in joins.
- Player character data gains ACID durability — no more half-written `.temp` files.
- No database call ever blocks the game thread at runtime — saves and player loads are both async via the DB worker (§7.5).

### Risks and Mitigations

| Risk | Mitigation |
|------|-----------|
| PostgreSQL not available on build host | Available in every major Linux distro (`apt install postgresql libpq-dev`) |
| Network partition between game host and DB host at boot | Server aborts cleanly with a clear error message; keep DB on LAN or same host if desired |
| Network partition during live saves | DB worker detects failure after three retries; falls back to flat-file saves via `DB_WORKER_FALLBACK` and sets `db_worker_failed` flag; server continues running |
| Queued saves lost on crash (not clean shutdown) | Writer queue is in-memory only; a hard crash can lose the last few seconds of saves. Mitigated by tick saves (saves happen on every pulse for online players) keeping the queue shallow |
| Queue growth during tick saves with many players | Last-write-wins coalescing (§7.5) ensures at most one pending entry per player name at any time |
| Migration loses data (parsing edge cases) | Round-trip test: import all files, export back, diff against originals; `raw_save` column preserved during transition |
| `resets.seq` ordering fragile if rows are reordered | Load always uses `ORDER BY seq`; `seq` is immutable once set |
| 64-bit `act_flags` / `extra_flags` stored as signed `BIGINT` | C load code casts via `(unsigned long long)(int64_t)`; audit areas for bit-63 usage before migration |
| `/data/db.conf` contains credentials | Gitignored; use `sslmode=require` and a restricted DB role with SELECT/INSERT/UPDATE/DELETE on all tables |
| Player inventory JSONB is opaque to SQL queries | A follow-on `player_items` normalisation table can be added without changing the server code path |
| `data/chest/` contains nested objects not covered by this proposal | `data/chest/` left as flat files; added to future work |

---

## 13. Future Work (Out of Scope)

- **Area hot-reload:** Re-loading a single area from the database without a full reboot.
- **Area versioning:** A `changelog` table recording who changed what and when.
- **Web-based editor:** A REST API over the database for browser-based area and lore editing.
- **Full-text search:** PostgreSQL `tsvector` indexes over room descriptions and lore bodies.

---

## 14. Implementation Checklist

For the implementing Claude session:

**Setup**
- [ ] Provision PostgreSQL; create `acktng` database and `ack` role with appropriate grants
- [ ] Add `/data/db.conf` to `.gitignore`
- [ ] Add `-lpq` to `src/Makefile` `LIBS`
- [ ] Write `area/schema.sql` with all DDL from Section 4 (all 26 tables)
- [ ] Apply `schema.sql` to the database

**Migration tools**
- [ ] Write `src/tools/import_to_db.c`: import areas, help, shelp, `lore/` (runtime), `data/`, `player/`, `area/boards/`
- [ ] Write `src/tools/db_to_files.c`: export all six stores + `--views-only`
- [ ] Write `src/tests/test_db_roundtrip.c`: unit test
- [ ] Run import; verify row counts against file counts for all six stores
- [ ] Run export; diff against originals (no semantic differences expected)

**Server integration — area/help/shelp/lore**
- [ ] Write `db_load_*` functions in `src/db.c` behind `#ifdef USE_DB_LOAD`
- [ ] Run `make unit-tests` with `USE_DB_LOAD` enabled (flat-file fallback active)

**Async DB worker**
- [ ] Write `src/db_worker.c` and `src/db_worker.h`: request queue, results queue, coalescing, shutdown flush
- [ ] Add `CON_LOADING_FROM_DB` to `connected` enum in `src/typedefs.h`
- [ ] Add `db_worker_poll_results()` call in `game_loop()` in `src/comm.c`
- [ ] Handle `CON_LOADING_FROM_DB` in `nanny()` / descriptor processing (block input while loading)
- [ ] Implement emergency flat-file fallback behind `DB_WORKER_FALLBACK` in `db_worker.c`
- [ ] Add `db_worker_start()` call to boot sequence (after all `db_load_*` complete)
- [ ] Add `db_worker_stop()` call to `do_shutdown` / signal handler
- [ ] Add `-lpthread` to `src/Makefile` `LIBS`

**Server integration — data/ and player/**
- [ ] Write `db_load_*` functions in `src/save/` behind `#ifdef USE_DB_LOAD`
- [ ] Replace all `save_*` call sites with `db_worker_enqueue_write(...)` calls
- [ ] Replace `load_char_obj()` call site with `db_worker_enqueue_load_player()`
- [ ] Run `make unit-tests` with `USE_DB_LOAD` enabled

**OLC write path (Phase 8)**
- [ ] Replace `build_save_rooms/mobs/objects/resets/shops/area()` with per-entity `db_worker_enqueue_write()` calls
- [ ] Add `DB_WRITE_ROOM`, `DB_WRITE_MOB`, `DB_WRITE_OBJ`, `DB_WRITE_RESET_LIST`, `DB_WRITE_SHOP`, `DB_DELETE_ROOM`, `DB_DELETE_MOB`, `DB_DELETE_OBJ` to `DB_OP_TYPE` enum
- [ ] Implement worker handlers for each new op type in `db_worker.c`
- [ ] Add `db_worker_enqueue_write()` calls to `build_addroom()`, `build_addmob()`, `build_addobj()` in `build.c`
- [ ] Add `db_worker_enqueue_write(DB_DELETE_*)` calls to OLC delete paths in `build.c`
- [ ] Remove `SaveFile`, `saving_area`, `SaveQ[]` queue mechanism from `save_area_files.c`
- [ ] Remove or stub `do_areasave` in `act_wiz.c`

**Socials and boards write path (Phase 8)**
- [ ] Add `DB_WRITE_SOCIAL`, `DB_DELETE_SOCIAL`, `DB_WRITE_BOARD_MESSAGE`, `DB_DELETE_BOARD_MESSAGE`, `DB_WRITE_BOARD` to `DB_OP_TYPE` enum
- [ ] Implement worker handlers for each new op type in `db_worker.c`
- [ ] Replace `save_social_table()` in `save_socials.c` with per-operation worker enqueue calls
- [ ] Replace `save_board()` in `board.c` with per-message worker enqueue calls
- [ ] Move board loading from on-demand `load_board()` to boot-time `db_load_boards()` in `db.c`; boards and messages loaded at boot, expiry filter applied in SQL

**Phase 8 cut-over**
- [ ] Remove `#ifdef USE_DB_LOAD` and old file-based load/save code
- [ ] Update `integration-test.sh` with PostgreSQL setup/teardown phase (Section 10.2)
- [ ] Verify `make unit-tests` still passes (integration test now spins up its own cluster)
- [ ] Move flat files to `*/legacy/` directories (`area/`, `help/`, `shelp/`, `lore/`, `data/`, `player/`, `area/boards/`)

**Documentation**
- [ ] Write `docs/player_file_spec.md` documenting player save format and DB schema
- [ ] Update `docs/area_db_spec.md` with finalized schema
- [ ] Update `docs/help_file_spec.md` with database authoring workflow
- [ ] Update `docs/lore_file_spec.md` with database authoring workflow (runtime `lore/` only; `docs/lore/` unchanged)
