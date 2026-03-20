# Design Proposal: Area-Specific Music — Midgaard

**Date:** 2026-03-20
**Status:** Pending approval

---

## 1. Problem

The web client currently plays `theme.mp3` immediately after the WebSocket handshake (implemented in `socket.c`). The request is to additionally play `midgaard.mp3` whenever the player is in the **Midgaard City Center** area, and revert to `theme.mp3` when they leave.

The feature must:
- Only affect WebSocket connections (never plain telnet)
- Not re-send the music command on every room step — only on area transitions
- Switch back to `theme.mp3` when leaving Midgaard

---

## 2. Approach

### 2.1 Tracking State

A single boolean field `websocket_in_midgaard` is added to `DESCRIPTOR_DATA`. It is initialised to `FALSE` in `init_descriptor()`. On each room change for a PC WebSocket client, the field is compared against the destination area; a music command is sent only when the value changes (i.e. on the transition, not every step).

Using a boolean (rather than a `char *current_music_url`) keeps the change minimal and avoids string allocation bookkeeping.

### 2.2 Midgaard Detection

The Midgaard area is identified by comparing `ch->in_room->area->name` against the string `"Midgaard City Center"` (the name loaded from the `#AREA` block of `midgaard.are`). This is preferred over a vnum range check because the area pointer is already set on `ROOM_INDEX_DATA` and the name is the canonical human-readable identifier.

### 2.3 Music Dispatch — `send_music_play()` in `socket.c`

The existing inline music command in `handle_websocket_handshake()` is extracted into a static helper:

```c
static void send_music_play(DESCRIPTOR_DATA *d, const char *url)
{
    char buf[256];
    snprintf(buf, sizeof(buf),
             "{\"type\":\"music\",\"action\":\"play\",\"url\":\"%s\"}", url);
    write_websocket_frame(d, 0x1, (const unsigned char *)buf, strlen(buf));
}
```

`handle_websocket_handshake()` is updated to call `send_music_play(d, "/web/mp3/theme.mp3")` in place of the inline code (no behaviour change there).

A new public function `void send_area_music(CHAR_DATA *ch)` is added to `socket.c` and declared in `socket.h`. It:

1. Returns immediately if `ch` is an NPC or has no descriptor or is not WebSocket-active.
2. Determines whether the player is now in Midgaard: `in_midgaard = !str_cmp(ch->in_room->area->name, "Midgaard City Center")`.
3. Compares against `ch->desc->websocket_in_midgaard`.
4. If the value changed, calls `send_music_play()` with the appropriate URL and updates the flag.

### 2.4 Hook Point — `char_to_room()` in `handler.c`

`char_to_room()` is the single place where `ch->in_room` is updated. At the end of the function (after the room assignment and all existing side-effects), a call to `send_area_music(ch)` is added. Because `char_to_room()` is called for both PCs and NPCs, the NPC guard in `send_area_music()` keeps it a no-op for mobs.

---

## 3. Affected Files

| File | Change |
|---|---|
| `src/headers/ack.h` | Add `bool websocket_in_midgaard;` to `struct descriptor_data` |
| `src/socket.c` | Extract `send_music_play()` helper; add `send_area_music()` function |
| `src/headers/socket.h` | Declare `void send_area_music(CHAR_DATA *ch);` |
| `src/handler.c` | Call `send_area_music(ch)` at the end of `char_to_room()` |

No changes to area files, help files, Makefile, or web templates are required. The `midgaard.mp3` file already exists at `web/mp3/midgaard.mp3` and the web server already serves `/web/mp3/` files.

---

## 4. Trade-offs and Constraints

**Why `char_to_room()` and not movement commands in `act_move.c`?**
`char_to_room()` is the single canonical point where `ch->in_room` changes, covering teleports, spell effects, mob resets, and normal movement. Hooking movement commands would miss many transition paths.

**Why a boolean and not the URL string?**
There are only two states: Midgaard music or theme music. A boolean avoids string allocation and `free_string` bookkeeping in `DESCRIPTOR_DATA`. If more areas are added later, the boolean can be replaced with an enum or `char *` at that point.

**What plays if the player logs out and back in while in Midgaard?**
The flag is initialised to `FALSE` on each new descriptor. The handshake unconditionally sends `theme.mp3`. Once the player enters the game and their room is restored, `char_to_room()` is called, `send_area_music()` fires, and `midgaard.mp3` starts — correct behaviour, slight delay.

**No stop command sent on disconnect.**
The browser drops the WebSocket on disconnect, which stops audio naturally. No explicit stop message is needed.

**Telnet safety.**
`send_area_music()` checks `d->websocket_active` before doing anything, so plain telnet connections are unaffected.
