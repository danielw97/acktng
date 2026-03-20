# Fix clandata.dat Corruption from MAX_CLAN Change

## Problem

`data/clandata.dat` was written when `MAX_CLAN` was 9 (8 clans + None) but the code now expects `MAX_CLAN` 11 (10 clans + None). The file format is a flat sequence of whitespace-delimited integers with no headers or version markers. The save/load loops iterate `for (x = 1; x < MAX_CLAN; x++)`, so:

| Section | Old (MAX_CLAN=9) | New (MAX_CLAN=11) |
|---------|-------------------|---------------------|
| diplomacy matrix | 8×8 = 64 values | 10×10 = 100 values |
| treasury array | 8 values | 10 values |
| end_current_state matrix | 8×8 = 64 values | 10×10 = 100 values |
| **Total** | **136 values** | **210 values** |

On boot, `fread_number()` reads sequentially. After reading 64 diplomacy values, the loader expects 36 more but instead reads into the treasury section, then the end_current_state section, corrupting everything. Eventually it runs out of data entirely, likely causing a crash or undefined behavior.

## Root Cause

Commit `4388aca` ("Add clan system: 10 lore-grounded clans with diplomacy framework") changed `MAX_CLAN` from 9 to 11 in `config.h` but did not update `data/clandata.dat`. The underlying design flaw is that the file format has no self-describing header — the loader blindly trusts that the file matches the current `MAX_CLAN`. Any future change to `MAX_CLAN` would cause the same silent corruption.

## Approach

### 1. Add a clan count header to the save/load format

Modify `save_clan_table()` in `act_clan.c` to write `MAX_CLAN` as the first value in `clandata.dat`. Modify the load code in `db.c` to read this header first and validate it matches the current `MAX_CLAN`. If it doesn't match, log a warning and skip loading (falling back to zero-initialized defaults) rather than silently corrupting memory.

**Save format (new):**
```
11
  <diplomacy matrix: (MAX_CLAN-1) × (MAX_CLAN-1) values>
  <treasury: (MAX_CLAN-1) values>
  <end_current_state matrix: (MAX_CLAN-1) × (MAX_CLAN-1) values>
```

**Load behavior:**
1. Read the first number as `file_max_clan`
2. If `file_max_clan != MAX_CLAN`, log a warning and skip loading (safe zero defaults)
3. Otherwise, load as before

### 2. Regenerate `clandata.dat` with correct format

Regenerate the file with the new header and correct 10-clan dimensions, all zeros.

### 3. Add a unit test

Write a test that validates:
- `save_clan_table()` writes the `MAX_CLAN` header
- The load function reads and validates the header
- Round-trip save/load preserves data correctly
- A mismatched header is detected and handled gracefully

## Affected Files

| File | Change |
|------|--------|
| `src/act_clan.c` | Write `MAX_CLAN` header in `save_clan_table()` |
| `src/db.c` | Read and validate `MAX_CLAN` header on load |
| `data/clandata.dat` | Regenerate with header + correct 10-clan format |
| `src/tests/test_clandata.c` | New unit test for save/load round-trip and header validation |
| `src/Makefile` | Add test target |

## Trade-offs

- **No migration logic**: Since all values are zero, we regenerate rather than writing a migration tool. If there were live diplomacy data, we'd need to parse the old format and expand the matrices.
- **Header is a single integer**: A more robust approach would be a named key-value format or versioned binary header, but a single integer is minimal, consistent with the existing `fread_number()` parsing, and sufficient to catch `MAX_CLAN` mismatches.
