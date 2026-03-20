# Design Proposal: Area-Specific Music — Midgaard and Kiess

**Date:** 2026-03-20
**Status:** Implemented

---

## 1. Problem

The web client currently plays `theme.mp3` immediately after the WebSocket handshake (implemented in `socket.c`). The request is to play area-specific music for two areas:

| Area | Music file |
|---|---|
| Midgaard City Center (`midgaard.are`, `midgaard_shops.are`) | `midgaard.mp3` |
| Kiess (`kiess.are`) | `kiess.mp3` |
| Mafdet (`mafdet.are`) | `mafdet.mp3` |

All other areas continue playing `theme.mp3`. The feature must:
- Only affect WebSocket connections (never plain telnet)
- Not re-send the music command on every room step — only on area transitions
- Revert to `theme.mp3` when the player leaves a themed area

**Note:** `kiess.mp3` and `mafdet.mp3` do not yet exist in `web/mp3/`. They must be placed there before the feature is live for those areas. All music files are served from `/web/mp3/`.

---

## 2. Approach

### 2.1 `char *music` Field on `AREA_DATA`

A new field is added to `struct area_data`:

```c
char *music; /* bare filename, e.g. "midgaard.mp3", or NULL for default theme */
```

`NULL` means the area uses the default `theme.mp3`. Areas with specific music store just the bare filename — the `/web/mp3/` prefix is prepended at send time, keeping the data free of URL scheme concerns.

### 2.2 Setting Music at Boot — `db.c`

After all areas have loaded, `db.c` sets `music` on the two themed areas by matching on `area->filename` (plain ASCII from `area.lst`, no color codes):

```c
for (pArea = first_area; pArea != NULL; pArea = pArea->next)
{
    if (!str_cmp(pArea->filename, "midgaard.are") ||
        !str_cmp(pArea->filename, "midgaard_shops.are"))
        pArea->music = str_dup("midgaard.mp3");
    else if (!str_cmp(pArea->filename, "kiess.are"))
        pArea->music = str_dup("kiess.mp3");
    /* else music remains NULL → theme */
}
```

Adding music to a new area in future requires only a new `else if` here, or no code change at all if the field is later made settable from the area file format.

### 2.3 Tracking State on the Descriptor

`DESCRIPTOR_DATA` gains one field:

```c
const char *websocket_current_music; /* mirrors area->music of the last track sent, or NULL */
```

Initialised to `NULL` in `init_descriptor()`. `NULL` represents the theme track (matching the handshake's unconditional `theme.mp3` send). Because this mirrors the stable `str_dup`'d pointer from `AREA_DATA` (which lives for the lifetime of the server), no allocation or `free_string` is needed on the descriptor.

Change detection uses pointer equality — same `area->music` pointer means same track, no command needed.

### 2.4 Music Dispatch — `send_area_music()` in `socket.c`

The existing inline music command in `handle_websocket_handshake()` is extracted into a static helper:

```c
static void send_music_play(DESCRIPTOR_DATA *d, const char *filename)
{
    char buf[256];
    snprintf(buf, sizeof(buf),
             "{\"type\":\"music\",\"action\":\"play\",\"url\":\"/web/mp3/%s\"}",
             filename);
    write_websocket_frame(d, 0x1, (const unsigned char *)buf, strlen(buf));
}
```

`handle_websocket_handshake()` is updated to call `send_music_play(d, "theme.mp3")` in place of the inline code (no behaviour change; `websocket_current_music` is already `NULL` at handshake time, matching the NULL-means-theme convention).

A new public function `void send_area_music(CHAR_DATA *ch)` is added:

1. Returns immediately if `ch` is an NPC, has no descriptor, or is not WebSocket-active.
2. Reads `track = ch->in_room->area->music` (may be `NULL`).
3. Compares against `ch->desc->websocket_current_music` by pointer.
4. If they differ, calls `send_music_play(d, track ? track : "theme.mp3")` and updates `websocket_current_music = track`.

### 2.5 Hook Point — `char_to_room()` in `handler.c`

`char_to_room()` is the single place where `ch->in_room` is updated, covering movement, teleports, spell effects, and mob resets. A call to `send_area_music(ch)` is added at the end of the function. The NPC guard in `send_area_music()` keeps it a no-op for mobs.

---

## 3. Affected Files

| File | Change |
|---|---|
| `src/headers/ack.h` | Add `char *music;` to `struct area_data`; add `const char *websocket_current_music;` to `struct descriptor_data` |
| `src/db.c` | After area load loop, set `music` on Midgaard and Kiess areas |
| `src/socket.c` | Extract `send_music_play()`; add `send_area_music()`; update handshake call |
| `src/headers/socket.h` | Declare `void send_area_music(CHAR_DATA *ch);` |
| `src/handler.c` | Call `send_area_music(ch)` at end of `char_to_room()` |
| `web/mp3/kiess.mp3` | Must be provided and placed here before the feature is live |

No changes to area files, help files, Makefile, or web templates are required. `midgaard.mp3` already exists; the web server already serves `/web/mp3/`.

---

## 4. Trade-offs and Constraints

**Why store just the filename and not the full URL in `area->music`?**
All music lives under `/web/mp3/`. Storing only the filename keeps the data clean and free of URL scheme assumptions; `send_music_play()` prepends the prefix at send time.

**Why `area->filename` for detection in `db.c` rather than `area->name`?**
The Kiess area name contains color codes (`@@W@@BKiess@@N`). Filenames from `area.lst` are always plain ASCII.

**Why pointer comparison for change detection?**
`area->music` is `str_dup`'d once at boot and never changed. The pointer is stable for the server's lifetime, so pointer equality reliably identifies the current track without string comparison.

**Both Midgaard area files share the same music?**
Yes. `midgaard_shops.are` is part of the Midgaard city experience.

**What plays on login if the player's saved room is in Midgaard or Kiess?**
The handshake sends `theme.mp3` unconditionally before the character is loaded. When `char_to_room()` restores the player's room, `send_area_music()` fires and switches to the area track. There is a brief moment of theme music during login — acceptable.

**Telnet safety.**
`send_area_music()` checks `d->websocket_active` before doing anything.
