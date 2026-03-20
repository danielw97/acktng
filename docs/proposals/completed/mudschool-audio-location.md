# Design Proposal: Mudschool Area Music

## Problem

The Academy of Adventure (newschool.are, vnums 4900–4999) has no area-specific music
assigned. Web client players in the academy hear the default theme.mp3 instead of the
dedicated mudschool.mp3 track that already exists in `web/mp3/`.

## Approach

The music assignment system lives in `src/db.c` in a post-load loop that sets
`AREA_DATA.music` by matching filenames. Adding one `else if` clause for
`newschool.are` is sufficient.

No new data structures, no new functions, no protocol changes — the WebSocket
music dispatch path (`send_area_music()` in `src/socket.c`) already handles the
rest automatically when a player enters any room in the academy.

## Affected Files

| File | Change |
|------|--------|
| `src/db.c` | Add `else if (!str_cmp(pArea->filename, "newschool.are")) pArea->music = str_dup("mudschool.mp3");` in the area music assignment block (~line 795) |

No area file changes, no client changes, no new files.

## Trade-offs

- **Simple**: Follows the identical pattern used for midgaard, kiess, and mafdet.
- **No unit test needed**: The assignment is a trivial data setup step with no
  logic to test; the existing integration test (boot + login) will exercise the
  load path.

## Verification

After the change, a web client player who enters any newschool.are room will
receive `{"type":"music","action":"play","url":"/web/mp3/mudschool.mp3"}` and
the browser will play the track. Transitioning out of the academy will switch
back to the appropriate area music (or theme.mp3 if none is configured).
