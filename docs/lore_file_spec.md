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

**Source material:** `docs/lore/` contains expanded worldbuilding documentation.
`docs/lore/timeline.md` is the master chronology. These are authoritative
references for lore content.

### Database workflow (authoritative)

All lore content lives in the PostgreSQL database (`lore_topics` and `lore_entries`
tables). The flat files in `lore/` are legacy source material; the database is
authoritative.

**Step 1 — Insert the topic:**

```sql
INSERT INTO lore_topics (filename, keywords)
VALUES ('charter_wars', 'charter wars oasis conflict');
-- Note the returned id for use in the entries below.
```

**Step 2 — Insert the default entry (required, seq=1, flags=0):**

```sql
INSERT INTO lore_entries (topic_id, seq, flags, body)
VALUES (
    (SELECT id FROM lore_topics WHERE filename = 'charter_wars'),
    1, 0,
    'The Charter Wars were a series of conflicts...\n[10-15 lines of dense factual content]'
);
```

**Step 3 — Insert city entries (required, one per city, in order):**

City flag values (from `docs/lore_file_spec.md`): MIDGAARD=1, KIESS=2, KOWLOON=4, RAKUEN=8, MAFDET=16.

```sql
-- MIDGAARD entry (flags = 1)
INSERT INTO lore_entries (topic_id, seq, flags, body)
VALUES ((SELECT id FROM lore_topics WHERE filename = 'charter_wars'), 2, 1, 'Midgaard perspective...');

-- KIESS entry (flags = 2)
INSERT INTO lore_entries (topic_id, seq, flags, body)
VALUES ((SELECT id FROM lore_topics WHERE filename = 'charter_wars'), 3, 2, 'Kiess perspective...');

-- KOWLOON (4), RAKUEN (8), MAFDET (16) follow the same pattern at seq 4, 5, 6.
```

**Step 4 — Insert city+race entries (only where meaningfully distinct):**

Race flag values: HUMAN=32, KHENARI=64, KHEPHARI=128, ASHBORN=256, UMBRAL=512,
RIVENNID=1024, DELTARI=2048, USHABTI=4096, SERATHI=8192, KETHARI=16384.

Combine flags with `|`. A MIDGAARD+KHEPHARI entry has `flags = 1 | 128 = 129`.

```sql
INSERT INTO lore_entries (topic_id, seq, flags, body)
VALUES ((SELECT id FROM lore_topics WHERE filename = 'charter_wars'), 7, 129,
        'What a Khephari in Midgaard knows about the Charter Wars...');
```

**Step 5 — Validate:**

```sql
-- Confirm all required entries exist
SELECT seq, flags, LEFT(body, 60) FROM lore_entries
WHERE topic_id = (SELECT id FROM lore_topics WHERE filename = 'charter_wars')
ORDER BY seq;
```

Rules:
- Keep each entry to 10-15 lines of dense, factual content.
- Do not include `@@` color codes anywhere.
- The default entry (seq=1, flags=0) is required.
- All 5 city entries are required (flags 1, 2, 4, 8, 16 at minimum).
- City+race entries only where the race cognitive posture produces a meaningfully
  distinct perspective. See `docs/perspective.md`.
- Maximum 56 entries per topic (1 default + 5 city + up to 50 city+race).

### Adding a new lore source document

Source documents in `docs/lore/` (Markdown) are the worldbuilding reference that
lore entries are written from. They are not loaded by the server; they inform the
content that goes into the database.

1. Create `docs/lore/<topic>_lore.md` with full worldbuilding detail.
2. From the source document, write the lore entries into the database following
   the steps above.

### Editing an existing lore entry

```sql
UPDATE lore_entries SET body = 'Revised text...'
WHERE topic_id = (SELECT id FROM lore_topics WHERE filename = 'charter_wars')
  AND seq = 1;
```

### Removing a lore topic

```sql
DELETE FROM lore_entries
WHERE topic_id = (SELECT id FROM lore_topics WHERE filename = 'old_topic');
DELETE FROM lore_topics WHERE filename = 'old_topic';
```

### Querying lore

```sql
-- Find topics matching a keyword
SELECT filename, keywords FROM lore_topics WHERE keywords ILIKE '%oasis%';

-- Count entries per topic (check all have at least 6: default + 5 cities)
SELECT t.filename, COUNT(e.id) AS entry_count
FROM lore_topics t LEFT JOIN lore_entries e ON e.topic_id = t.id
GROUP BY t.filename ORDER BY entry_count;
```
