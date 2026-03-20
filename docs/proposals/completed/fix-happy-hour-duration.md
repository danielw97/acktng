# Design Proposal: Fix Auto-Triggered Happy Hour Duration

## Problem

Automatically triggered happy hours (fired when a new concurrent-player record is set) end
way too fast. The trigger in `login.c` sets `happy_hour_ticks_remaining = 3`, which at one
tick per in-game hour (= 60 real seconds each) means the entire happy hour expires in **3
real-world minutes**.

The info message that accompanies the trigger reads *"Happy hour triggered for three
in-game hours!"*, so the intent is clearly stated — but the unit mismatch makes it
meaningless in practice.

## Background: Timing Model

| Constant | Value | Real time |
|---|---|---|
| `PULSE_PER_SECOND` | 8 pulses | — |
| `PULSE_TICK` | `60 × 8 = 480` pulses | **60 seconds** |
| One in-game hour | 1 tick | **1 real minute** |
| One in-game day | 24 ticks | **24 real minutes** |

`weather_update()` — the function that decrements `happy_hour_ticks_remaining` — is called
once per `PULSE_TICK`, so every decrement equals one in-game hour / one real minute.

## Root Cause

`login.c` line 989:
```c
happy_hour_ticks_remaining = 3;   // 3 ticks = 3 real minutes
```

The value `3` was likely intended to mean "three hours of wall-clock time" but was set in
tick units, producing a duration 60× shorter than intended.

For comparison, wizard-triggered happy hours (via `trigger_happy_hour()` in `act_wiz.c`)
do **not** set `happy_hour_ticks_remaining`, leaving it at 0.  The decrement logic in
`update.c` only runs when `ticks_remaining > 0`, so wizard-triggered happy hours run
indefinitely until the midnight reset — potentially up to 24 real minutes.  Auto-triggered
happy hours therefore end far sooner than a manually started one.

## Proposed Fix

### 1. Add a named constant to `src/headers/config.h`

```c
/* Duration of an automatically triggered happy hour, in ticks (1 tick = 1 in-game hour
 * = 60 real seconds).  24 ticks = one full in-game day ≈ 24 real minutes. */
#define HAPPY_HOUR_AUTO_DURATION 24
```

A full in-game day (24 ticks, 24 real minutes) is a natural, player-perceivable unit that
gives everyone online a reasonable window to benefit.

### 2. Update `src/login.c`

Replace the magic literal:
```c
// before
happy_hour_ticks_remaining = 3;
info("New player record reached! Happy hour triggered for three in-game hours!", 1);

// after
happy_hour_ticks_remaining = HAPPY_HOUR_AUTO_DURATION;
info("New player record reached! Happy hour triggered for one full in-game day!", 1);
```

### 3. No other source files need changes

`update.c` already handles the countdown and expiry correctly; only the initial value is
wrong.

## Affected Files

| File | Change |
|---|---|
| `src/headers/config.h` | Add `HAPPY_HOUR_AUTO_DURATION` constant |
| `src/login.c` | Use constant; update info message |

## Trade-offs

| Option | Duration | Notes |
|---|---|---|
| Keep `3` but fix units (180 ticks) | ~3 real hours | Matches original message literally; long for a MUD session |
| One in-game day (`24` ticks) | ~24 real minutes | Natural game unit; matches wizard-triggered upper bound |
| Half in-game day (`12` ticks) | ~12 real minutes | Conservative; still 4× longer than current |

The **24-tick** option is recommended: it aligns auto-triggered duration with the natural
maximum of a wizard-triggered happy hour, and 24 real minutes is long enough to feel
meaningful without being excessive.

## Unit Tests

No new unit test is strictly required (the change is a single integer constant), but the
existing integration test covers the boot path and will confirm the build is not broken.

## Roll-out

1. Add constant to `config.h`
2. Update `login.c`
3. Run `make unit-tests` to verify clean build + integration test
4. Commit and push to `claude/fix-happy-hour-duration-wXs8a`
