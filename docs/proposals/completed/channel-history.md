# Channel History Feature

**Date:** 2026-03-21
**Status:** Pending approval

---

## Problem

Players have no way to review recent channel traffic after connecting, tabbing away, or missing a message. Typing a channel name with no argument currently produces an unhelpful "Gossip what?" error. This wastes the error path and leaves players without context.

---

## Approach

### Summary

Store the last 15 messages per channel in a global in-memory circular buffer. When a player types a channel command with no argument, display that channel's history instead of the "X what?" error.

### Data Structure

Define a `CHAN_HISTORY_ENTRY` struct and per-channel ring buffers in `act_comm.c` (as file-static globals):

```c
#define CHAN_HIST_SIZE   15
#define CHAN_HIST_NUM    20

typedef struct {
    char speaker[64];
    char message[MAX_INPUT_LENGTH];
} CHAN_HISTORY_ENTRY;

static CHAN_HISTORY_ENTRY chan_history[CHAN_HIST_NUM][CHAN_HIST_SIZE];
static int chan_hist_pos[CHAN_HIST_NUM];   /* next write index (circular) */
static int chan_hist_count[CHAN_HIST_NUM]; /* total written, capped at CHAN_HIST_SIZE */
```

### Channel Index Mapping

A `channel_to_hist_idx()` helper maps CHANNEL_* bitmasks to the 0–19 array index. Channels not in the table return -1 (no history stored or shown).

Tracked channels and their indices:

| Index | Channel constant     | Command      |
|-------|----------------------|--------------|
| 0     | CHANNEL_AUCTION      | chat/auction |
| 1     | CHANNEL_GOSSIP       | gossip       |
| 2     | CHANNEL_MUSIC        | music        |
| 3     | CHANNEL_STAFFTALK    | stafftalk    |
| 4     | CHANNEL_NEWBIE       | newbie       |
| 5     | CHANNEL_QUESTION     | question     |
| 6     | CHANNEL_SHOUT        | shout        |
| 7     | CHANNEL_YELL         | yell         |
| 8     | CHANNEL_FLAME        | flame        |
| 9     | CHANNEL_ZZZ          | zzz          |
| 10    | CHANNEL_RACE         | race         |
| 11    | CHANNEL_CLAN         | clan         |
| 12    | CHANNEL_CREATOR      | creator      |
| 13    | CHANNEL_DIPLOMAT     | diplomat     |
| 14    | CHANNEL_CRUSADE      | crusade      |
| 15    | CHANNEL_REMORTTALK   | remorttalk   |
| 16    | CHANNEL_ADEPT        | adepttalk    |
| 17    | CHANNEL_OOC          | ooc          |
| 18    | CHANNEL_INVASION     | invasion     |
| 19    | CHANNEL_GAME         | game         |

### Recording Messages

At the top of `talk_channel()`, after all validation passes and the message is about to be broadcast, call:

```c
chan_history_add(channel, ch->name, argument);
```

This stores `speaker` + `message` at `chan_hist_pos[idx]`, advances the write cursor mod `CHAN_HIST_SIZE`, and increments `chan_hist_count[idx]` (capped at `CHAN_HIST_SIZE`).

### Displaying History

Replace the current empty-argument early return in `talk_channel()`:

```c
// Before:
if (argument[0] == '\0') {
    sprintf(buf, "%s what?\n\r", verb);
    buf[0] = UPPER(buf[0]);
    return;   // BUG: buf is never sent!
}

// After:
if (argument[0] == '\0') {
    chan_history_show(ch, channel, verb);
    return;
}
```

`chan_history_show()` iterates the circular buffer from oldest to newest and calls `send_to_char()` for each entry, formatted as:

```
[Gossip History]
  PlayerA gossips 'hello world'
  PlayerB gossips 'anyone around?'
  ...
No more history.
```

If the buffer is empty it prints: `"No {channel} history yet.\n\r"`

No access-control filtering is applied to history display — if a player can invoke the command (passes the per-command pre-checks), they see the stored messages. This mirrors how replay in chat clients works: you asked for history, you get history.

### Bug Fix

The existing code at line 488–492 has a bug: `buf` is written but never sent to `ch`. The `chan_history_show()` path fixes this by using `send_to_char()` explicitly.

---

## Affected Files

| File | Change |
|------|--------|
| `src/act_comm.c` | Add `CHAN_HISTORY_ENTRY` struct, static buffers, `channel_to_hist_idx()`, `chan_history_add()`, `chan_history_show()`, modify `talk_channel()` |
| `help/channels` | Document the new "no argument = history" behavior |
| `src/tests/test_chan_history.c` | Unit tests for `chan_history_add()` / `chan_history_show()` |

No header changes are needed — all new symbols are file-static or defined within `act_comm.c`.

---

## Trade-offs

**In-memory only:** History is lost on server reboot. This is intentional — channel history is ephemeral by design, consistent with the MUD's existing approach to runtime state.

**No access filtering on history:** A player who just connected sees the last 15 messages regardless of whether they were online when they were said. This is the expected UX; MUD clients often show backscroll exactly this way.

**CHAN_HIST_SIZE = 15:** Matches the stated requirement. Easy to increase later.

**File-static arrays:** No heap allocation, no risk of leaking, zero initialization on server start. The total memory cost is 20 × 15 × (64 + 640) ≈ 210 KB — negligible.

**Yell/Race/Clan scoping:** History stores all messages regardless of area/race/clan, but the command pre-checks (e.g. `do_clan` checks clan membership before calling `talk_channel`) still run, so a player with no clan cannot invoke `do_clan` and thus cannot see clan history. Scope is naturally enforced by the existing command guards.
