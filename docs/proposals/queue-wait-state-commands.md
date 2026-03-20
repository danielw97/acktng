# Design Proposal: Queue Only Wait-State Commands

## Problem

Currently, the game loop in `socket.c` blocks **all** command processing for a character whenever `ch->wait > 0`. This means that while a player is recovering from a combat action (kick, spell cast, movement), every command they type — including completely non-combat commands like `look`, `score`, `say`, or `inventory` — is held in the input buffer and delayed until the wait expires.

This creates unnecessary friction: players cannot check their health, chat with allies, or look at their surroundings during the brief delay after a combat action. The wait was designed to throttle action-rate for combat and movement, not to suspend the character entirely.

## Desired Behavior

Only commands that **introduce a wait state** (i.e., call `WAIT_STATE(ch, ...)` as part of their normal execution) should be held in the queue while `ch->wait > 0`. All other commands should execute immediately regardless of the current wait value.

**Example scenario:**

1. Player kicks → `WAIT_STATE(ch, 16)` is set (2 pulses/second × 8 pulses/PULSE_VIOLENCE = 16 pulses)
2. Player types `look` during the wait → executes immediately (no wait)
3. Player types `score` during the wait → executes immediately
4. Player types `kick` again during the wait → stays queued until wait expires
5. Wait expires → queued `kick` executes

## Approach

### 1. New command flag: `CMD_FLAG_WAIT`

Add a new bitmask flag to `config.h`:

```c
#define CMD_FLAG_WAIT BIT_2   /* command introduces wait state; queue while ch->wait > 0 */
```

### 2. New macro in `interp.c`

Add a `CMD_WAIT` macro alongside the existing `CMD` and `CMD_NINJA` macros:

```c
#define CMD_WAIT(name, fun, pos, lvl, log, type, show) \
    { name, fun, pos, lvl, log, type, show, CMD_FLAG_WAIT }
#define CMD_NINJA_WAIT(name, fun, pos, lvl, log, type, show) \
    { name, fun, pos, lvl, log, type, show, CMD_FLAG_NINJA_OK | CMD_FLAG_WAIT }
```

### 3. Mark wait-state commands in the command table

Replace `CMD(...)` with `CMD_WAIT(...)` (or `CMD_NINJA_WAIT`) for every command that calls `WAIT_STATE(ch, ...)` in its implementation path.

**Commands to receive `CMD_FLAG_WAIT`:**

| Command(s) | Source | Reason |
|---|---|---|
| `north`, `south`, `east`, `west`, `up`, `down` | `act_move.c` | Always `WAIT_STATE(ch, 1)` |
| `cast` | `magic.c` | Always `WAIT_STATE(ch, beats)` |
| `kill` | `fight.c` | Always `WAIT_STATE(ch, PULSE_VIOLENCE)` |
| `murder` | `fight.c` | Always `WAIT_STATE(ch, PULSE_VIOLENCE)` |
| `backstab`, `bs` | `skills_poison.c` | `WAIT_STATE(ch, beats)` |
| `circle` | `skills_poison.c` | `WAIT_STATE(ch, beats)` |
| `stance` | `stance.c` | `WAIT_STATE(ch, 2*PULSE_VIOLENCE)` |
| `bond` | `weapon_bond.c` | `WAIT_STATE(ch, 24)` on bond creation |
| `brandish` | `act_obj.c` | `WAIT_STATE(ch, 2*PULSE_VIOLENCE)` |
| `recite` | `act_obj.c` | `WAIT_STATE(ch, 2*PULSE_VIOLENCE)` |
| `steal` | `act_obj.c` | `WAIT_STATE(ch, beats)` |
| `pick` | `skills/do_pick.c` | `WAIT_STATE(ch, beats)` |
| `kick`, `bash`, `punch`, `road sweep`, `checkpoint break`, `charge`, `fleche`, `holystrike` | `skills/` → `war_attack()` | `WAIT_STATE(ch, beats)` via `war_attack` |
| `palmstrike`, `aurabolt`, `pummel`, `spinfist` | `skills/` → `pug_attack()` | `WAIT_STATE(ch, beats)` via `pug_attack` |
| `dirt`, `ashfall strike`, `harbor dust`, `trip`, `berserk`, `disarm`, `frenzy`, `target`, `dunmar's call`, `riposte`, `anti magic shell`, `chi surge`, `breath of endurance`, `fist of the interior form`, `momentum chain`, `veteran's cadence`, `overwhelming assault`, `oathshield`, `sanctified strike`, `shadow reading`, `hex ward`, `reflex disruption`, `shieldblock`, `chiblock`, `chakra`, `flurry`, `reach silence`, `dissolution protocol`, `gap transit`, `asset flag`, `gap hold`, `seam snare`, `read intent`, `cleave`, `seven shade hold`, `charter challenge`, `rend`, `field patch`, `weapon mastery`, `grapple`, `choke hold`, `leg sweep`, `iron skin`, `roll with blow`, `applied understanding`, `feint`, `cistern discipline`, `verdict`, `read opponent`, `binding strike`, `ninth descent`, `condemn`, `seal testimony`, `rescue` | `skills/` | Direct `WAIT_STATE(ch, beats)` |

**Commands intentionally NOT flagged** (they may sometimes call `WAIT_STATE` but should execute immediately):

- `flee`, `recall`, `/` — escape commands; must execute without queuing
- `shout` — communication command; the wait is an anti-spam throttle, not an action delay
- `password` — the wait only fires on wrong password entry (security lockout)
- `smash`, `stun`, `morale`, `leadership`, `interior discipline` — do not call `WAIT_STATE` in their implementation
- `assist`, `revenant`, `disguise`, `reach remedy` — do not call `WAIT_STATE`

### 4. New helper function in `interp.c`

Add a function (declared in `interp.h` or via forward declaration in `socket.c`):

```c
/*
 * Returns TRUE if the command in 'argument' has CMD_FLAG_WAIT set,
 * meaning it should be held in the queue while ch->wait > 0.
 */
bool command_has_wait_flag(CHAR_DATA *ch, const char *argument);
```

This function parses the first word of `argument`, looks it up in `cmd_table`, and checks for `CMD_FLAG_WAIT`.

### 5. Restructure the game loop in `socket.c`

**Current flow (lines 737–759):**

```c
if (d->character != NULL && d->character->wait > 0)
{
    --d->character->wait;
    continue;   /* skip read_from_buffer and interpret */
}

read_from_buffer(d);
if (d->incomm[0] != '\0')
{
    d->fcommand = TRUE;
    ...
    interpret(d->character, d->incomm);
    d->incomm[0] = '\0';
}
```

**New flow:**

```c
/* Read the next pending command from the input buffer.
 * read_from_buffer is a no-op if incomm already has a command waiting. */
read_from_buffer(d);

if (d->character != NULL && d->character->wait > 0)
{
    --d->character->wait;

    /* If the pending command introduces a wait state, keep it queued.
     * (incomm is left non-empty; read_from_buffer will not overwrite it.) */
    if (d->incomm[0] != '\0'
        && d->connected == CON_PLAYING
        && command_has_wait_flag(d->character, d->incomm))
    {
        continue;
    }
    /* Otherwise fall through: non-wait-state commands execute immediately. */
}

if (d->incomm[0] != '\0')
{
    d->fcommand = TRUE;
    stop_idling(d->character);
    d->timeout = current_time + 180;

    if (d->connected == CON_PLAYING)
        if (d->showstr_point)
            show_string(d, d->incomm);
        else
            interpret(d->character, d->incomm);
    else
        nanny(d, d->incomm);

    d->incomm[0] = '\0';
}
```

**Key properties of the new flow:**

- `read_from_buffer` is called unconditionally, so a command is always read into `incomm` when one is available.
- `ch->wait` is still decremented every pulse regardless of command type, so timing is unchanged.
- If `incomm` holds a wait-state command and `wait > 0`, we `continue` without clearing `incomm`. On the next pulse, `read_from_buffer` sees `incomm` is non-empty and returns immediately, so the command stays queued.
- If `incomm` holds a non-wait command, we fall through and execute it immediately, then clear `incomm`.
- Non-playing descriptors (login flow, paging) are not affected by the wait-state check.

## Affected Files

| File | Change |
|---|---|
| `src/headers/config.h` | Add `CMD_FLAG_WAIT BIT_2` |
| `src/interp.c` | Add `CMD_WAIT` / `CMD_NINJA_WAIT` macros; update command table; add `command_has_wait_flag()` |
| `src/socket.c` | Restructure input processing loop |
| `src/tests/test_command_wait_flag.c` | New unit test |

## Trade-offs

**Pro:**
- Players can freely use informational and social commands during combat waits
- No change to combat timing — wait still ticks down every pulse
- Clear, auditable list of which commands are gated vs. immediate

**Con:**
- Commands must be manually marked with `CMD_FLAG_WAIT`; new commands that introduce wait state must be flagged at the time of addition (can be enforced by convention / CLAUDE.md)
- A wait-state command already in `incomm` will delay the next command from `inbuf` even if that next command is non-wait — but this only matters when two commands are typed in rapid succession, which is uncommon
