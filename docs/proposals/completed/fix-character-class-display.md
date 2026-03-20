# Design Proposal: Fix Character Class Display During Character Creation

## Problem

The class selection menu shown during new-player character creation (`show_cmenu_to` in `src/login.c`) has two display bugs introduced when the Druid and Sentinel classes were added as mortal-tier classes:

1. **Wrong class count in descriptive text**: The menu reads "There are seven classes" but there are now **eight** mortal-tier classes: Mag, Cle, Cip, War, Psi, Pug, Dru, and Sen.

2. **Misaligned column header**: The header line is:
   ```
   Abr    Prime Atr    Name
   ---    ---------    ----
   ```
   But the data format string is `"%3.3s    %3.3s    %-10.10s\n\r"`, which produces a 3-character stat abbreviation column. This means:
   - The `Prime Atr` label is 9 characters wide for a 3-character data column.
   - The `Name` header appears at character position 20, but name data starts at position 14 — a 6-character misalignment.

   Actual output looks like:
   ```
   Abr    Prime Atr    Name
   ---    ---------    ----
   Mag    Int    Magi
   Cle    Wis    Cleric
   ...
   ```
   The `Name` column header sits 6 positions to the right of where the names actually appear.

## Approach

Fix both issues in `src/login.c`, inside `show_cmenu_to`:

1. Change `"There are seven classes."` → `"There are eight classes."`

2. Replace the misaligned header/separator lines with ones that match the 3-char stat column:
   ```
   Abr    Atr    Name
   ---    ---    ----------
   ```

No changes to the format string (`comm_format_class_menu_line`) are needed — the data format is correct; only the static header strings are wrong.

## Affected Files

- `src/login.c` — `show_cmenu_to` function (lines 315, 319–320)

## Trade-offs

- This is a pure text correction with no logic changes. No game mechanics are affected.
- The `---` separator under `Name` is widened to 10 dashes to match the `%-10.10s` data column width, making the separator visually consistent.
- No new unit tests are needed; the existing `test_comm_class_menu_formatter_*` tests cover the formatting function itself, and the static header strings are trivially verified visually.
