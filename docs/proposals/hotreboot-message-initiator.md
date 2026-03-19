# Design Proposal: Hotreboot Message Initiator

## Problem

The current hotreboot broadcast message is hardcoded to read:

```
**** HOTreboot by An Immortal - Please remain ONLINE ****
```

This is uninformative. Players and staff cannot tell who triggered the reboot,
and the message is identical whether the reboot was triggered manually by an
immortal or automatically by the CI/CD deployment pipeline.

## Proposed Approach

`do_hotreboot(CHAR_DATA *ch, char *argument)` already receives `ch` as the
initiating character. When called from the automated deployment path in
`update.c`, it is called as `do_hotreboot(NULL, "")`.

The fix is to branch on `ch`:

- If `ch != NULL` → use `ch->name` in the message (manual immortal reboot).
- If `ch == NULL` → use a fixed string indicating an automated CI/CD reboot.

No new data structures, files, or global variables are needed.

## Affected Files

| File | Change |
|------|--------|
| `src/hotreboot.c` | Replace the hardcoded `buf` sprintf at lines 74–77 with a conditional that either names the immortal or says "Automated CI/CD Deployment". |

No other files need changes. `update.c` already passes `NULL` for automated
reboots; `interp.c` already passes the immortal's `CHAR_DATA *` for manual
reboots.

## Exact Message Change

**Before (both cases):**
```
**** HOTreboot by An Immortal - Please remain ONLINE ****
```

**After — manual reboot by immortal "Syrion":**
```
**** HOTreboot by Syrion - Please remain ONLINE ****
```

**After — automated CI/CD reboot:**
```
**** HOTreboot: Automated CI/CD Deployment - Please remain ONLINE ****
```

## Trade-offs

- **Minimal scope** — single-line conditional, no structural changes.
- **No unit test needed** — the message formatting is trivial string logic
  with no branching state; integration test covers the reboot path.
- The `ch == NULL` check is already the established convention in this
  codebase for automated/system calls (see `update.c` line 2054).

## Implementation Steps

1. In `src/hotreboot.c`, replace the `sprintf(buf, ...)` block (lines 74–77)
   with a conditional sprintf that uses `ch->name` when `ch` is non-NULL, or
   the CI/CD string when `ch` is NULL.
2. Run `cd src && make unit-tests` to confirm all tests pass.
3. Commit and push to `claude/hotreboot-message-initiator-SRRdi`.
