# Design Proposal: Area-Specific Music — Midgaard and Kiess

**Date:** 2026-03-20
**Status:** Pending approval

---

## 1. Problem

The web client currently plays `theme.mp3` immediately after the WebSocket handshake (implemented in `socket.c`). The request is to play area-specific music for two areas:

| Area | Music |
|---|---|
| Midgaard City Center (`midgaard.are`, `midgaard_shops.are`) | `midgaard.mp3` |
| Kiess (`kiess.are`) | `kiess.mp3` |

All other areas continue playing `theme.mp3`. The feature must:
- Only affect WebSocket connections (never plain telnet)
- Not re-send the music command on every room step — only on area transitions
- Revert to `theme.mp3` when the player leaves a themed area

**Note:** `kiess.mp3` does not yet exist in `web/mp3/`. It must be placed there before the feature is live.

---

## 2. Approach

### 2.1 Area Detection via Filename

Area names in the file format may include color codes (e.g. Kiess is stored as `@@W@@BKiess@@N`), making name-string comparison fragile. `AREA_DATA->filename` is set from `area.lst` and is always a plain filename with no color codes, making it the reliable identifier:

- Midgaard: `area->filename` is `"midgaard.are"` or `"midgaard_shops.are"`
- Kiess: `area->filename` is `"kiess.are"`

### 2.2 Music URL Constants

Three static string constants are defined in `socket.c`:

```c
static const char MUSIC_THEME[]    = "/web/mp3/theme.mp3";
static const char MUSIC_MIDGAARD[] = "/web/mp3/midgaard.mp3";
static const char MUSIC_KIESS[]    = "/web/mp3/kiess.mp3";
```

A helper `music_url_for_area()` maps an `AREA_DATA *` to the appropriate constant pointer:

```c
static const char *music_url_for_area(AREA_DATA *area)
{
    if (area == NULL)
        return MUSIC_THEME;
    if (!str_cmp(area->filename, "midgaard.are") ||
        !str_cmp(area->filename, "midgaard_shops.are"))
        return MUSIC_MIDGAARD;
    if (!str_cmp(area->filename, "kiess.are"))
        return MUSIC_KIESS;
    return MUSIC_THEME;
}
```

### 2.3 Tracking State

`DESCRIPTOR_DATA` gains one field:

```c
const char *websocket_current_music; /* points to a MUSIC_* constant, or NULL */
```

Initialised to `NULL` in `init_descriptor()`. Because this always points to a static constant (never a heap string), no `free_string` bookkeeping is needed.

Change detection uses pointer equality — same constant pointer means same track, no command needed.

### 2.4 Music Dispatch — `send_area_music()` in `socket.c`

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

`handle_websocket_handshake()` is updated to call `send_music_play(d, MUSIC_THEME)` and set `d->websocket_current_music = MUSIC_THEME` in place of the inline code.

A new public function `void send_area_music(CHAR_DATA *ch)` is added to `socket.c` and declared in `socket.h`. It:

1. Returns immediately if `ch` is an NPC, has no descriptor, or is not WebSocket-active.
2. Calls `music_url_for_area(ch->in_room->area)` to determine the correct track.
3. Compares the result against `ch->desc->websocket_current_music` by pointer.
4. If they differ, calls `send_music_play()` with the new URL and updates `websocket_current_music`.

### 2.5 Hook Point — `char_to_room()` in `handler.c`

`char_to_room()` is the single place where `ch->in_room` is updated, covering movement, teleports, spell effects, and mob resets. A call to `send_area_music(ch)` is added at the end of the function. The NPC guard in `send_area_music()` keeps it a no-op for mobs.

---

## 3. Affected Files

| File | Change |
|---|---|
| `src/headers/ack.h` | Add `const char *websocket_current_music;` to `struct descriptor_data` |
| `src/socket.c` | Add `MUSIC_*` constants; extract `send_music_play()`; add `music_url_for_area()`; add `send_area_music()`; update handshake to use them |
| `src/headers/socket.h` | Declare `void send_area_music(CHAR_DATA *ch);` |
| `src/handler.c` | Call `send_area_music(ch)` at end of `char_to_room()` |
| `web/mp3/kiess.mp3` | Must be provided and placed here before the feature is live |

No changes to area files, help files, Makefile, or web templates are required. `midgaard.mp3` already exists; the web server already serves `/web/mp3/`.

---

## 4. Trade-offs and Constraints

**Why `area->filename` and not `area->name`?**
The Kiess area name contains color codes (`@@W@@BKiess@@N`). Filenames are always plain ASCII from `area.lst` — no stripping required.

**Why a `const char *` pointer and not a boolean or enum?**
Two area-specific tracks plus the default makes three states. A pointer to a static constant handles any number of future areas without adding new fields or extending an enum. Pointer comparison is O(1) and allocation-free.

**Both Midgaard area files play the same music?**
Yes. `midgaard_shops.are` is part of the Midgaard city experience and should share its music.

**What plays on login if the player's saved room is in Midgaard or Kiess?**
The handshake sends `theme.mp3` unconditionally (before the character is loaded). When `char_to_room()` is called to restore the player's room, `send_area_music()` fires immediately and switches to the area track. There is a brief moment of theme music during login — acceptable.

**No stop command on disconnect.**
The browser drops the WebSocket on disconnect, which pauses audio naturally.

**Telnet safety.**
`send_area_music()` checks `d->websocket_active` before doing anything.
