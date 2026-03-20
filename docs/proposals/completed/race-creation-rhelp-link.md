# Design Proposal: Link Race Name to rhelp During Character Creation

## Problem

During character creation, when a player types a full race name (e.g., "Khenari") at the race selection prompt, the code calls `do_help(ch, argument)` (login.c:773). This invokes the general help system, which is unlikely to have a matching entry for individual race names. The intent is clearly to show race-specific information — which is precisely what `do_rhelp` does.

## Approach

Change the single call from `do_help` to `do_rhelp` at `src/login.c:773`.

`do_rhelp(ch, argument)` accepts a race name or abbreviation and displays:
- Base attributes
- Racial skills
- Race specials (flags)
- Magic realm affinities

No other changes are needed — the argument passed is already the full race title, which `do_rhelp` already handles via `find_race_index_by_name()`.

## Affected Files

| File | Change |
|------|--------|
| `src/login.c` | Line 773: `do_help` → `do_rhelp` |

## Trade-offs

- **Minimal risk**: one-character function name change, no logic changes.
- `do_rhelp` is already declared and accessible where `do_help` is used.
- The prompt text at line 279 already says "type the full race name for race info" — this change makes the behaviour match that description.

## No Design Concerns

No new structs, no new files, no new states. The prompt text in `show_rmenu_to()` does not need updating.
