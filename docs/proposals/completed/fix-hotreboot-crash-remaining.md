# Design Proposal: Fix Remaining Hotreboot Crash Issues

## Problem

After the initial SIGVTALRM timer-race fix (`docs/proposals/completed/fix-hotreboot-timer-crash.md`),
crashes during and after hotreboot recovery continue to occur. A GDB backtrace from a production
core dump shows:

```
#0  __GI___libc_write (fd=4, buf=..., nbytes=31)
#2  write_to_descriptor (desc=4, txt="\n\rCopyover recovery complete.\n\r") at socket.c:1569
#3  copyover_recover () at hotreboot.c:260
#4  main (argc=5, argv=...) at comm.c:195
```

The duplicate `__libc_write` frames are the signature of a signal delivered while inside the
`write()` syscall.  The crash is process termination by `SIGVTALRM` with `SIG_DFL` disposition —
meaning it happened before `init_alarm_handler()` had a chance to install the real handler.

Additionally, two NULL/out-of-bounds bugs were found in code paths active on the first game-loop
iteration after recovery, and the logs currently contain too little detail to pinpoint which
function causes a crash.

---

## Root Causes

### 1. SIGVTALRM race still possible after existing setitimer(zero) fix

The existing fix stops the virtual timer with `setitimer(ITIMER_VIRTUAL, &zero_timer, NULL)` just
before `execl()`.  However, POSIX does not guarantee that `setitimer` discards a signal that is
already *pending* in the kernel's signal queue.  If the virtual CPU timer fires in the tiny window
between the last `reset_itimer()` call (inside `alarm_handler`) and the `setitimer(zero)` call,
`SIGVTALRM` becomes pending.  After `execl()`, signal dispositions are reset to `SIG_DFL` — but
the pending signal remains.  When the kernel delivers it to the new process (which may be deep in
`boot_db()` or `copyover_recover()`) it terminates the process with a core dump.

**Fix**: Set `signal(SIGVTALRM, SIG_IGN)` immediately before `setitimer(zero)`.  `SIG_IGN` is
preserved across `execl()`.  When the new process receives any pending `SIGVTALRM`, it is silently
discarded.  `init_alarm_handler()` then overwrites `SIG_IGN` with the real handler as normal.

### 2. NULL dereference in `npc_dialogue_deliver()` — `npc_dialogue.c`

The guard condition for stale NPC dialogue responses:

```c
if (resp->npc == NULL || resp->npc->is_free || resp->player == NULL || resp->player->is_free)
{
    resp->npc->dlg_pending = FALSE;   /* BUG: resp->npc may be NULL here */
    free(resp);
    continue;
}
```

When `resp->npc == NULL`, the very next line dereferences it.  `npc_dialogue_deliver()` is called
at the top of every `update_handler()` invocation, including the first one after hotreboot recovery.

**Fix**: Guard the `dlg_pending` reset:

```c
if (resp->npc == NULL || resp->npc->is_free || resp->player == NULL || resp->player->is_free)
{
    if (resp->npc != NULL && !resp->npc->is_free)
        resp->npc->dlg_pending = FALSE;
    free(resp);
    continue;
}
```

### 3. Out-of-bounds array access in `can_use_pub_society_skill()` — `act_pub_society.c`

The function checks `ch->pcdata->pub_society <= PUB_SOCIETY_NONE` to guard against invalid low
values, but never validates the upper bound.  If a corrupted or newly added player has
`pub_society >= MAX_PUB_SOCIETY`, it reads `pub_society_table[soc]` out of bounds.

**Fix**: Add an upper-bound check immediately after the existing lower-bound check:

```c
int soc = ch->pcdata->pub_society;
if (soc <= PUB_SOCIETY_NONE)
    return FALSE;
if (soc >= MAX_PUB_SOCIETY)
    return FALSE;
```

### 4. `last_checkpoint` stale after `copyover_recover()` — `comm.c`

`init_alarm_handler()` records `last_checkpoint = get_user_seconds()` right after `boot_db()`
completes.  If `copyover_recover()` then consumes another 20+ CPU seconds (loading many players),
the alarm handler can fire *after* `disable_timer_abort` is set back to `FALSE` and find that
`usage_now - last_checkpoint > BOOT_DB_ABORT_THRESHOLD`.  This triggers an `abort()` that looks
like a spurious hang.

**Fix**: Reset `last_checkpoint` to the current CPU time immediately after `copyover_recover()`
returns, before clearing `disable_timer_abort`:

```c
if (fCopyOver) {
    disable_timer_abort = TRUE;
    copyover_recover();
    last_checkpoint = get_user_seconds();   /* keep watchdog alive */
    disable_timer_abort = FALSE;
}
```

### 5. Insufficient logging — `update.c`, `hotreboot.c`

When a crash occurs in `violence_update()`, `round_update()`, `char_update()`, or related
functions, the log contains no information about which function was executing.  Adding `log_f()`
calls at the entry point of each major update phase and at key milestones in `copyover_recover()`
allows the log to pinpoint the crash location.

---

## Affected Files

| File | Change |
|------|--------|
| `src/hotreboot.c` | Add `signal(SIGVTALRM, SIG_IGN)` before `setitimer(zero)` in `do_hotreboot()` |
| `src/npc_dialogue.c` | Guard `resp->npc->dlg_pending` reset against NULL |
| `src/act_pub_society.c` | Add upper-bound check for `pub_society` in `can_use_pub_society_skill()` |
| `src/comm.c` | Reset `last_checkpoint` after `copyover_recover()` |
| `src/update.c` | Add `log_f()` calls before each major update function |
| `src/hotreboot.c` | Add `log_f()` milestone logging in `copyover_recover()` |

---

## Trade-offs

- The `SIG_IGN` approach for the SIGVTALRM race was discussed in the original proposal as an
  alternative to `setitimer(zero)`.  Both fixes together (stop the timer *and* ignore pending
  signals) provide defense in depth.

- Logging adds minor overhead per game tick but only to `stderr`/logfile, not to players.  Each
  `log_f()` is a single `fprintf` call with negligible cost.

- The `last_checkpoint` reset is purely defensive: it only matters in the unusual case where
  `copyover_recover()` consumes an unexpectedly large amount of CPU.

---

## Testing

- `make unit-tests` (existing suite must pass)
- Manual hotreboot with a player online, verifying recovery succeeds without crash
- Verify "Used N CPU seconds" still appears correctly after hotreboot
