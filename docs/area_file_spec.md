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

*Sections II through VI follow: per-file schemas (§II), flag and enum reference tables (§III),
content validation rules (§IV), cross-section consistency rules (§V), and ingestion pipeline
mechanics (§VI).*
