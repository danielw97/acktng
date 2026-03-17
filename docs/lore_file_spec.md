# Lore File Specification

Lore entries are stored as files in `/lore`. They provide in-game world knowledge
that players access via the `lore` command. Each file may contain multiple entries,
selected at runtime based on NPC storyteller flags.

## File location and naming

- Directory: `lore/`
- One or more entries per file.
- Filename should be the primary lore topic (for example, `lore/cinderteeth`, `lore/midgaard`).
- Allowed filename characters: `a-z`, `A-Z`, `0-9`, `.`, `_`, `-`.
- Files ending in `~` are ignored.

## File format

Each entry uses this structure:

```text
keywords <keyword list>
[flags <FLAG1> [FLAG2 ...]]
---
<lore text>
```

Multiple entries are concatenated in the same file with no separator between them --
the start of a new `keywords` line marks the beginning of the next entry:

```text
keywords cinderteeth mountains volcanic
---
Default lore text about the Cinderteeth...

keywords cinderteeth mountains volcanic
flags MIDGAARD
---
Midgaard-perspective lore about the Cinderteeth...

keywords cinderteeth mountains volcanic
flags KIESS
---
Kiess-perspective lore about the Cinderteeth...
```

### Rules

- **No level line.** Unlike help/shelp files, lore entries do not have a `level` field.
  All lore is accessible to all players.
- `keywords` must be non-empty and is the keyword list used by `lore` matching.
- `flags` is optional. When present, it contains space-separated flag names.
- `---` is a required separator between the header and the text body.
- All content after `---` until the next `keywords` line (or EOF) is lore text.
- **No color codes.** Lore text must not contain `@@` color codes (e.g., `@@r`, `@@N`).
  The test suite enforces this restriction.

## Flags

Available flags (defined in `config.h`):

| Flag       | Constant             |
|------------|----------------------|
| MIDGAARD   | `LORE_FLAG_MIDGAARD` |
| KIESS      | `LORE_FLAG_KIESS`    |
| KOWLOON    | `LORE_FLAG_KOWLOON`  |
| RAKUEN     | `LORE_FLAG_RAKUEN`   |
| MAFDET     | `LORE_FLAG_MAFDET`   |

Flag names are case-insensitive during parsing.

## Runtime behavior

- On boot, the server loads all files from `lore/` (sorted by filename).
- The `lore` command searches the loaded entries by keyword (exact match, then prefix match).
- When multiple entries share the same keywords, the system selects the best match
  based on NPC storyteller flags:
  1. Collect `lore_flags` from all NPCs in the player's room (OR'd together).
  2. For each matching entry, its flags must be a **subset** of the NPC flags.
  3. The entry with the **most matching flag bits** wins (most specific match).
  4. An entry with no flags (the default) is the lowest-priority fallback.
- If no NPC with `lore_flags` is in the room, only unflagged (default) entries match.

## NPC lore flags

NPCs carry `lore_flags` on their mob template (`MOB_INDEX_DATA`) and instance
(`CHAR_DATA`). These are set via the builder command:

```
set mob <vnum> lore <FLAG>    -- toggle a flag
set mob <vnum> lore clear     -- clear all flags
```

Lore flags are saved in area files using the `^` marker.

## Adding new lore

1. Create a file in `lore/` named after the topic.
2. Write the default (unflagged) entry first.
3. Add city-perspective entries with appropriate `flags` lines.
4. Do not include `@@` color codes anywhere in the file.
5. Run `make unit-tests` from `src/` to validate the file format.
