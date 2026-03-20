# Design Proposal: Area Theme Songs as Area File Directives

**Date:** 2026-03-20
**Status:** Pending approval

---

## 1. Problem

Area-specific music tracks are currently hardcoded in `src/db.c` after boot:

```c
if (!str_cmp(pArea->filename, "midgaard.are") || ...)
    pArea->music = str_dup("midgaard.mp3");
else if (!str_cmp(pArea->filename, "kiess.are"))
    pArea->music = str_dup("kiess.mp3");
/* ... */
```

This is fragile, hard to extend, and puts content configuration in C source. Additionally:

- `whisperingforestpreserve.mp3` already exists on disk but has no mechanism to be associated with the Whispering Forest Preserve area.
- Areas without a configured track silently fall back to `theme.mp3`. There is no way to explicitly stop music when entering an area.

The request is to:

1. Move the music configuration into the `.are` file format as a loadable/saveable directive.
2. Associate `whisperingforestpreserve.mp3` with the Whispering Forest Preserve area.
3. When entering an area with no music configured, fade out and stop playback (rather than defaulting to `theme.mp3`).

---

## 2. Approach

### 2.1 New Area File Directive: `C`

Add a `C` (Choon) directive to the `#AREA` header section. The value is a tilde-terminated string containing the bare mp3 filename served from `/web/mp3/`.

```
C whisperingforestpreserve.mp3~
```

- If present: `pArea->music` is set to the filename string on load.
- If absent: `pArea->music` remains `NULL`, which now explicitly means "stop music".

The letter `C` is not currently used by any other directive (existing: F, O, Q, U, R, W, P, M, X, V, N, T, B, S, K, L, I).

### 2.2 Behavior Change: NULL Music → Stop

Currently `send_area_music()` in `socket.c` falls back to `theme.mp3` when `pArea->music` is NULL. After this change:

- `NULL` → send `{"type":"music","action":"stop"}` to the client (fade out and stop).
- Non-NULL → send `{"type":"music","action":"play","url":"/web/mp3/<filename>"}` as today.

The `websocket_current_music` pointer on the descriptor distinguishes "playing" from "stopped" to avoid redundant stop messages (using a sentinel value).

### 2.3 Client-Side Stop Handler

The web client's `handleMusicCommand` function currently only handles `action === 'play'`. A new branch handles `action === 'stop'`: fade the current track out over `FADE_DURATION` (2 s) and then pause and clear `audio.src`.

---

## 3. Affected Files

| File | Change |
|---|---|
| `src/save/save_area_files.c` | `load_area()`: add `case 'C'` to parse the directive. `build_save_area()`: write `C <music>~` when `CurSaveArea->music` is non-NULL. |
| `src/db.c` | Remove the hardcoded post-boot music assignment block (lines 784–800). |
| `src/socket.c` | `send_area_music()`: replace `theme.mp3` fallback with a stop message; track stopped state with a sentinel. |
| `web/templates/mud_client.html` | Add `action === 'stop'` branch in `handleMusicCommand` with fade-out. |
| `src/tests/test_area_format.c` | Add `'C'` to the set of valid `#AREA` directives; add `'C'` to the tilde-string directive set alongside K, L, O, U. |
| `docs/area_file_spec.md` | Document the `C` directive. |
| `area/forest_preserve.are` | Add `C whisperingforestpreserve.mp3~`. |
| `area/midgaard.are` | Add `C midgaard.mp3~`. |
| `area/midgaard_shops.are` | Add `C midgaard.mp3~`. |
| `area/kiess.are` | Add `C kiess.mp3~`. |
| `area/mafdet.are` | Add `C mafdet.mp3~`. |
| `area/newschool.are` | Add `C mudschool.mp3~`. |

All other areas will have no `C` directive and will cause music to stop on entry.

---

## 4. Sentinel Value for "Stopped" State

`d->websocket_current_music` is a `const char *` that tracks what is currently playing. It is `NULL` at connection start (meaning "nothing sent yet"). We need to distinguish three states:

| State | `websocket_current_music` value |
|---|---|
| Nothing sent yet (fresh connection) | `NULL` |
| Stop message sent | `(const char *) 1` — a non-NULL sentinel |
| Track playing | pointer to the track string |

On entry to a room, `send_area_music()` checks:
- If `area->music == NULL` and `current == sentinel` → no-op (already stopped).
- If `area->music == NULL` and `current != sentinel` → send stop, set `current = sentinel`.
- If `area->music != NULL` and `current == area->music` → no-op (same track).
- Otherwise → send play, set `current = area->music`.

---

## 5. Trade-offs

**Behavior change for areas without music:** All areas that previously played `theme.mp3` by default will now go silent. Any area that should continue playing theme music needs an explicit `C theme.mp3~` directive. Based on the hardcoded list, exactly five areas had explicit music; the request implies the rest should go silent. If `theme.mp3` should remain the default for unlabeled areas, that is a one-line change in `send_area_music()` — but the request says "stop playing music with a fade out" for areas with no theme song.

**No new unit test for `load_area`:** The `load_area` function reads from `FILE *` directly and is tested via `test_area_format.c` (which validates all live area files). Adding the `C` directive to `forest_preserve.are` and updating the validator to accept `C` provides integration-level coverage. A dedicated unit test for `load_area` parsing of `C` would require a test fixture file; this is out of scope for this task but could be added later.

---

## 6. Summary of Visible Changes

- Entering Whispering Forest Preserve → client plays `whisperingforestpreserve.mp3`.
- Entering Midgaard / Midgaard Shops → client plays `midgaard.mp3` (same as before).
- Entering Kiess → `kiess.mp3`, Mafdet → `mafdet.mp3`, Newschool → `mudschool.mp3` (same as before).
- Entering any other area → music fades out and stops.
- Area music is now set in the `.are` file and persists through `do_savearea`.
