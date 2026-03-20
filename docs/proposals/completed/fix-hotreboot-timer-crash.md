# Design Proposal: Fix Hotreboot Virtual Timer Crash

## Problem

The game process is randomly killed during hotreboot recovery. The log shows:

```
Fri Mar 20 08:31:50 2026 :: ../lore/charter_wars successfully loaded.
Virtual timer expired startup: shutting down
```

The new process is being killed by `SIGVTALRM` (virtual CPU timer) before it can install its signal handler.

### Root Cause

1. During normal operation, `init_alarm_handler()` (`update.c:173`) installs a `SIGVTALRM` handler and starts an interval timer via `setitimer(ITIMER_VIRTUAL, ...)` with a 20-second CPU period (`ALARM_FREQUENCY = 20`).

2. When `do_hotreboot()` calls `execl()` (`hotreboot.c:168`) to replace itself with a fresh binary, **POSIX guarantees that interval timers survive `exec()`** — the countdown keeps running in the new process. Signal *handlers* are reset to `SIG_DFL` on exec; the *timer itself* is not.

3. In the new process, `main()` calls `boot_db()` first (`comm.c:184`), which loads all area files, help files, and lore files — a potentially multi-second operation. Only after `boot_db()` completes does `main()` call `init_alarm_handler()` (`comm.c:187`).

4. If the inherited timer fires during `boot_db()`, `SIGVTALRM` is delivered with no handler installed. The default disposition for `SIGVTALRM` is process termination.

The crash is random because the timer fires at any point in its 20-second cycle depending on when the previous process last reset the checkpoint.

## Approach

Stop the virtual interval timer in `do_hotreboot()` immediately before calling `execl()`. This ensures the new process inherits no running timer; `init_alarm_handler()` starts it fresh after `boot_db()` completes.

```c
/* Stop the virtual timer so the new process doesn't inherit a running
 * timer and get killed by SIGVTALRM before init_alarm_handler() can
 * install the handler (POSIX: interval timers survive exec). */
struct itimerval zero_timer;
memset(&zero_timer, 0, sizeof(zero_timer));
setitimer(ITIMER_VIRTUAL, &zero_timer, NULL);
```

This is placed immediately before the `execl()` call at `hotreboot.c:168`.

## Affected Files

- `src/hotreboot.c` — add the `setitimer` call before `execl()`

No other files require changes.

## Trade-offs

- **Alternative: `signal(SIGVTALRM, SIG_IGN)` before exec** — `SIG_IGN` disposition is preserved across `exec()`, so the new process would ignore `SIGVTALRM` until `init_alarm_handler()` overrides it. This also works, but clearing the timer is more direct: it eliminates the signal entirely rather than suppressing it.

- **Alternative: move `init_alarm_handler()` before `boot_db()`** — This would install the handler before `boot_db()` runs, catching the inherited timer with the proper handler. However, `boot_db()` can take more than `BOOT_DB_ABORT_THRESHOLD` CPU seconds on its own, which would then trigger a legitimate abort. The existing code deliberately starts the timer *after* `boot_db()` to avoid this.

- **Stopping the timer is the clean solution**: it eliminates the race at the source, adds two lines, and has no side effects.

## Testing

- `make unit-tests` (existing suite must pass)
- Manual hotreboot with players online, verifying recovery succeeds consistently
