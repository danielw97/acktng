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

1. **Default entry** (no flags) — general-purpose baseline lore shown when no
   flags match. Should describe only what is directly relevant to this topic;
   information about other topics belongs in those topics' files.
2. **City-only entries** (5, required) — one per city flag: MIDGAARD, KIESS,
   KOWLOON, RAKUEN, MAFDET. Describes the topic from that city's institutional
   perspective, vocabulary, and founding-trauma lens. See `docs/perspective.md`
   for authoritative city voice and framing guidance.
3. **City + Race entries** (up to 50, only when needed) — one for each
   city-race combination where the race's cognitive posture produces a
   meaningfully different perspective than the city entry alone. If the
   city+race perspective would be essentially the same as the city-only entry,
   omit it. See `docs/perspective.md` for race cognitive profiles.
   Order: for each city, all 10 races in order.

**Maximum 56 entries per file** (1 default + 5 city + up to 50 city+race).
Omit city+race entries when the race perspective does not add distinct
information beyond the city perspective for this specific topic.

Example structure:

```text
keywords cinderteeth mountains volcanic
---
Default lore about the Cinderteeth...

flags MIDGAARD
---
Midgaard perspective on the Cinderteeth...

flags MIDGAARD KHEPHARI
---
What a Khephari in Midgaard knows about the Cinderteeth...
(Only included because Khephari have specific seismic expertise relevant here.)

flags KIESS
---
Kiess perspective on the Cinderteeth...
(No KIESS HUMAN entry needed if the human perspective matches the city entry.)
```

### Perspective guide

`docs/perspective.md` is authoritative for city and race voice. Each city has
a founding-trauma lens, institutional vocabulary, and rhetorical shape. Each
race has a cognitive posture, observational mode, and characteristic tell.

- **City layer** governs surface delivery: words chosen, metaphors, institutional
  reflex. Midgaard frames in procedure; Kowloon in covenant and color; Mafdet in
  oath and attestation; Kiess in transparency and disclosure; Rakuen in communal
  stewardship.
- **Race layer** governs deep patterning: how a speaker processes information.
  Khenari need procedural closure on records; Khephari start from physical
  terrain; Umbral note what is absent before what is present; Kethari operate on
  geological timescale; Ushabti distinguish claim/evidence/verdict; Deltari cite
  measured patterns; etc.

Include a city+race entry only when the race's cognitive posture produces
observations or framings that the city perspective alone would not generate.

### Rules

- **No level line.** Unlike help/shelp files, lore entries have no `level` field.
- `keywords` appears exactly once, at the top of the file. Must be non-empty.
- `flags` is required for every entry after the first. Contains space-separated flag names.
- `---` is a required separator between header and text body.
- **No color codes.** Lore text must not contain `@@` codes. Tests enforce this.
- **No repeated keywords.** Do not repeat the `keywords` line for each entry.
- **10-15 lines per entry.** Dense, factual, AI-optimized. No prose essays.
- **Topic-specific content only.** Each entry covers only what is relevant to
  this lore topic. Cross-topic relationships belong in those topics' own entries.

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
- **PCs always receive the default (unflagged) entry**, regardless of which NPCs
  are in the room. Room NPC flags are ignored for player characters.
- When an NPC uses `lore`, the best match is selected by flag specificity:
  1. Collect `lore_flags` from all NPCs in the caller's room (OR'd together).
  2. Entry flags must be a **subset** of the collected NPC flags.
  3. The entry with the **most matching flag bits** wins (most specific).
  4. An unflagged entry is the lowest-priority fallback.
- If no flagged NPC is present (or the caller is a PC), only the default
  (unflagged) entry matches.

### Match examples

- PC in any room → always gets the unflagged default entry.
- NPC with `MIDGAARD HUMAN` in room → best match is `flags MIDGAARD HUMAN`
  (2 bits), then `flags MIDGAARD` (1 bit), then unflagged (0 bits).
- NPC with `KIESS` in room → matches `flags KIESS` but not `flags KIESS HUMAN`
  (HUMAN is not in NPC flags).

## NPC lore flags

NPCs carry `lore_flags` on their mob template and instance. Set via builder:

```
set mob <vnum> lore <FLAG>    -- toggle a flag
set mob <vnum> lore clear     -- clear all flags
```

Lore flags are saved in area files using the `^` marker.

## Adding new lore

New lore topics should be added for historically notable figures, major events,
significant locations, institutions, or other subjects that players or NPCs
would plausibly ask about. When in doubt about whether a topic warrants its own
entry, check `docs/lore/timeline.md` for significance and whether existing lore
files already cover the topic adequately.

1. Create a file in `lore/` named after the topic (lowercase, underscores for
   spaces; e.g., `lore/caleph_dunmar`, `lore/charter_wars`).
2. Start with a single `keywords` line listing all search keywords.
3. Write the default entry (required) — general-purpose overview of the topic.
4. Write all 5 city entries (required, in order: MIDGAARD, KIESS, KOWLOON,
   RAKUEN, MAFDET). Apply city voice from `docs/perspective.md`.
5. Write city+race entries only where the race's cognitive posture produces a
   meaningfully distinct perspective. Apply race profiles from
   `docs/perspective.md`. Omit entries that would merely restate the city entry.
6. Keep each entry to 10-15 lines of dense, factual content.
7. Do not include `@@` color codes anywhere.
8. Run `make unit-tests` from `src/` to validate format.

**Source material:** `docs/lore/` contains expanded worldbuilding documentation.
`docs/lore/timeline.md` is the master chronology. These are authoritative
references for lore content. `/lore/` files are generated from this source
material; they should accurately reflect it.
