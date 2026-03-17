# Lore File Specification

Lore entries are stored as files in `/lore`. They provide world knowledge for
AI context injection, accessed via the `lore` command. Each file contains
multiple entries selected at runtime based on NPC storyteller flags.

## Purpose

Lore text is designed for **AI consumption**, not human reading. Entries should
be dense, structured reference material — short declarative sentences packing
maximum information into minimum lines. Target **10-15 lines per entry**.

## File location and naming

- Directory: `lore/`
- One or more entries per file.
- Filename should be the primary lore topic (e.g., `lore/cinderteeth`).
- Allowed filename characters: `a-z`, `A-Z`, `0-9`, `.`, `_`, `-`.
- Files ending in `~` are ignored.

## File format

Each file begins with a single `keywords` line, followed by one or more entries.
The first entry is the default (no flags). Subsequent entries each start with a
`flags` line. All entries in the file share the same keywords.

```text
keywords <keyword list>
[flags <FLAG1> [FLAG2 ...]]
---
<lore text>
[flags <FLAG1> [FLAG2 ...]]
---
<lore text>
...
```

The `keywords` line appears **only once**, at the top of the file. Each entry
after the first begins with a `flags` line (no repeated keywords).

### Required entries per file

Every lore file must contain these entries in order:

1. **Default entry** (no flags) — baseline lore shown when no flags match.
2. **City-only entries** (5) — one per city flag: MIDGAARD, KIESS, KOWLOON,
   RAKUEN, MAFDET. Shown when an NPC has only the matching city flag.
3. **City + Race entries** (50) — one for each city-race combination.
   Shown when an NPC has both the city flag and the race flag set.
   Order: for each city, all 10 races in order.

Total: **56 entries per file**.

Example structure:

```text
keywords cinderteeth mountains volcanic
---
Default lore about the Cinderteeth...

flags MIDGAARD
---
Midgaard perspective on the Cinderteeth...

flags MIDGAARD HUMAN
---
What a human in Midgaard knows about the Cinderteeth...

flags MIDGAARD KHENARI
---
What a Khenari in Midgaard knows about the Cinderteeth...
```

### Rules

- **No level line.** Unlike help/shelp files, lore entries have no `level` field.
- `keywords` appears exactly once, at the top of the file. Must be non-empty.
- `flags` is required for every entry after the first. Contains space-separated flag names.
- `---` is a required separator between header and text body.
- **No color codes.** Lore text must not contain `@@` codes. Tests enforce this.
- **No repeated keywords.** Do not repeat the `keywords` line for each entry.
- **10-15 lines per entry.** Dense, factual, AI-optimized. No prose essays.

## Flags

### City flags

| Flag       | Constant             | Bit  |
|------------|----------------------|------|
| MIDGAARD   | `LORE_FLAG_MIDGAARD` | 1<<0 |
| KIESS      | `LORE_FLAG_KIESS`    | 1<<1 |
| KOWLOON    | `LORE_FLAG_KOWLOON`  | 1<<2 |
| RAKUEN     | `LORE_FLAG_RAKUEN`   | 1<<3 |
| MAFDET     | `LORE_FLAG_MAFDET`   | 1<<4 |

### Race flags

| Flag       | Constant              | Bit   |
|------------|-----------------------|-------|
| HUMAN      | `LORE_FLAG_HUMAN`     | 1<<5  |
| KHENARI    | `LORE_FLAG_KHENARI`   | 1<<6  |
| KHEPHARI   | `LORE_FLAG_KHEPHARI`  | 1<<7  |
| ASHBORN    | `LORE_FLAG_ASHBORN`   | 1<<8  |
| UMBRAL     | `LORE_FLAG_UMBRAL`    | 1<<9  |
| RIVENNID   | `LORE_FLAG_RIVENNID`  | 1<<10 |
| DELTARI    | `LORE_FLAG_DELTARI`   | 1<<11 |
| USHABTI    | `LORE_FLAG_USHABTI`   | 1<<12 |
| SERATHI    | `LORE_FLAG_SERATHI`   | 1<<13 |
| KETHARI    | `LORE_FLAG_KETHARI`   | 1<<14 |

Flag names are case-insensitive during parsing.

## Runtime behavior

- On boot, the server loads all files from `lore/` (sorted by filename).
- The `lore` command searches entries by keyword (exact match, then prefix match).
- When multiple entries share keywords, the best match is selected by flag specificity:
  1. Collect `lore_flags` from all NPCs in the player's room (OR'd together).
  2. Entry flags must be a **subset** of the NPC flags.
  3. The entry with the **most matching flag bits** wins (most specific).
  4. An unflagged entry is the lowest-priority fallback.
- If no flagged NPC is present, only the default (unflagged) entry matches.

### Match examples

- NPC has `MIDGAARD HUMAN` → best match is `flags MIDGAARD HUMAN` (2 bits),
  then `flags MIDGAARD` (1 bit), then unflagged (0 bits).
- NPC has `KIESS` → matches `flags KIESS` but not `flags KIESS HUMAN`
  (HUMAN is not in NPC flags).

## NPC lore flags

NPCs carry `lore_flags` on their mob template and instance. Set via builder:

```
set mob <vnum> lore <FLAG>    -- toggle a flag
set mob <vnum> lore clear     -- clear all flags
```

Lore flags are saved in area files using the `^` marker.

## Adding new lore

1. Create a file in `lore/` named after the topic.
2. Start with a single `keywords` line listing all search keywords.
3. Write all 56 entries: 1 default + 5 city + 50 city-race.
   - The first (default) entry follows immediately after the keywords line.
   - Each subsequent entry starts with a `flags` line (no repeated keywords).
4. Keep each entry to 10-15 lines of dense, factual content.
5. Do not include `@@` color codes anywhere.
6. Run `make unit-tests` from `src/` to validate format.
