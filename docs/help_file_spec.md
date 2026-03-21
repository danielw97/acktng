# Help File Specification

Help entries are stored as individual files in `/help` and `/shelp`.

## File location and naming

- Directories: `help/` for standard helps, `shelp/` for spell/skill help entries.
- One help entry per file.
- Filename should be the primary help command keyword (for example, `help/prompt`, `shelp/shelp_fireball`).
- Allowed filename characters: `a-z`, `A-Z`, `0-9`, `.`, `_`, `-`.
- Files ending in `~` are ignored.

## File format

Each help file must use this exact high-level structure:

```text
level <integer>
keywords <keyword list>
---
<help text>
```

Rules:

- `level` must be an integer.
- `keywords` must be non-empty and contain the keyword list used by `help` matching.
- `---` is a required separator line.
- All content after `---` is help text.
- Help text may be empty.

## Runtime behavior

- On boot, the server loads all help and shelp entries from the PostgreSQL database
  (`help_entries` and `shelp_entries` tables in `area/schema.sql`).
- `help` command lookup uses the loaded keyword list and trust-level check as before.
- The `help` command reads only from `help_entries`.
- The `shelp` command reads only from `shelp_entries`.

## Adding or editing a help entry (database workflow)

All help content lives in the PostgreSQL database. The flat files in `help/` and `shelp/`
are legacy source material retained for reference; the database is authoritative.

### Adding a new player help entry

Insert a row into `help_entries`:

```sql
INSERT INTO help_entries (filename, level, keywords, body)
VALUES (
    'mycommand',          -- unique filename key; lowercase, no extension
    0,                    -- trust level (0 = all players; -1 = immortal-only)
    'mycommand my_cmd',   -- space-separated keyword list; first keyword is primary
    'Syntax: mycommand <target>\n\nDescription of what the command does...'
);
```

### Adding a new staff/spell/skill help entry

Insert a row into `shelp_entries` using the same pattern. Filename must start with `shelp_`:

```sql
INSERT INTO shelp_entries (filename, level, keywords, body)
VALUES (
    'shelp_fireball',
    -1,                        -- staff-only
    'fireball fire ball',
    '*****...\n   Name: Fireball\n...'
);
```

### Editing an existing entry

```sql
UPDATE help_entries SET body = 'Updated text...' WHERE filename = 'mycommand';
UPDATE shelp_entries SET keywords = 'fireball fire' WHERE filename = 'shelp_fireball';
```

### Removing an entry

```sql
DELETE FROM help_entries WHERE filename = 'oldcommand';
DELETE FROM shelp_entries WHERE filename = 'shelp_oldspell';
```

### Querying

```sql
-- Find all help entries mentioning a keyword
SELECT filename, keywords FROM help_entries WHERE keywords ILIKE '%fireball%';

-- Find all shelp entries for a given level
SELECT filename FROM shelp_entries WHERE level = -1 ORDER BY filename;
```

### After making changes

Changes take effect on the next server boot (the server loads all help/shelp on startup
and does not hot-reload). To test locally, regenerate flat files and run the integration test:

```sh
./tools/db_to_files --help-only   # regenerates help/export/ and shelp/export/
cd src && make integration-test
```
