# Design Proposal: Increase Alias Capacity and Fix Display

## Problem

The `alias` command currently displays all aliases, but players are capped at 6 aliases total (`MAX_ALIASES 6` in `config.h`). The load code in `save_players.c` also hardcodes individual `SKEY` entries for aliases 0–5, so adding more slots requires explicit entries there as well.

Raising the cap to 10 allows players meaningful alias headroom without structural complexity.

## Approach

1. **`src/headers/config.h`** — Change `MAX_ALIASES` from `6` to `10`.
2. **`src/save/save_players.c`** — Add `SKEY` entries for aliases 6–9 in the load block. (The save loop already uses `MAX_ALIASES`, so no change there.)
3. No changes needed in `do_alias` (display loop uses `MAX_ALIASES`), `interp.c` (lookup loop uses `MAX_ALIASES`), `scheck.c` (uses `MAX_ALIASES`), or `lists.c` (uses `MAX_ALIASES`).

## Affected Files

| File | Change |
|---|---|
| `src/headers/config.h` | `MAX_ALIASES 6` → `MAX_ALIASES 10` |
| `src/save/save_players.c` | Add 4 SKEY entries for aliases 6–9 |

## Trade-offs

- **Backward compatibility**: Player save files only contain entries for aliases that were set. New slots default to `<none>` on load, so old saves are safe.
- **Memory**: Each PC_DATA gains 8 pointers (4 × 2 new slots). Negligible.
- **No unit test needed**: The change is a constant bump and load-table extension with no logic to test.

## Error Message Fix

There is also a minor bug on line 4842 of `act_info.c`:
```c
sprintf(buf, "Valid alias numbers are 0 to %d.\n\r", MAX_ALIASES);
```
This should be `MAX_ALIASES - 1` since valid indices are 0-based. This will be fixed in the same PR.
