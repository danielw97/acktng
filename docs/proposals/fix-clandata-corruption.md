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

## Approach

### 1. Regenerate `clandata.dat` with correct 10-clan format

Since the current file is all zeros (no live game data to preserve), simply regenerate it with the correct dimensions: 10×10 diplomacy matrix, 10 treasury values, 10×10 end_current_state matrix.

### 2. Add a unit test

Write a test that validates `save_clan_table()` produces the correct number of values for the current `MAX_CLAN`, and that the load function can round-trip the data correctly.

## Affected Files

| File | Change |
|------|--------|
| `data/clandata.dat` | Regenerate with correct 10-clan format |
| `src/tests/test_clandata.c` | New unit test for save/load round-trip |
| `src/Makefile` | Add test target |

## Trade-offs

- **No migration logic**: Since all values are zero, we simply regenerate rather than writing a migration tool. If there were live diplomacy data, we'd need to parse the old format and expand the matrices.
- **No format versioning**: Adding a version header to `clandata.dat` would prevent this class of bug in the future, but that's a larger change beyond the current scope. The existing save/load format is inherited from the original codebase and changing it would require coordinating the save and load code.
