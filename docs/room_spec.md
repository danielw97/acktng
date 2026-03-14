# Room Specification

Extracted from `docs/area_file_spec.md`.

## 8) `#ROOMS` section

A list of room records terminated by `#0`:

```text
#ROOMS
#<vnum>
<name>~
<description>~
<room_flags> <sector_type>
[zero or more room entries]
S
...
#0
```

Room entries are one of:

- Exit:
  ```text
  D<door>
  <exit_description>~
  <exit_keyword>~
  <locks> <key_vnum> <to_room_vnum>
  ```
  - `door` must be 0..5.
  - destination line must be exactly 3 integers.
  - Named exits are allowed by setting a non-empty `<exit_keyword>` when it improves room flavor/clarity.
  - For door-style named exits, prefix the keyword with `^` (for example, `^iron gate`).
  - `<key_vnum>` must be the vnum of a key object that unlocks this exit. Use `-1` when no key exists for the exit.
- Extra description:
  ```text
  E
  <keyword>~
  <description>~
  ```
  - Extra-description `<keyword>` names must be discoverable in-room. Chained `E` references are allowed (for example, extra 1 keyword appears in the room description, extra 1 text mentions extra 2 keyword, extra 2 text mentions extra 3 keyword), but each chain must be anchored: at least one keyword in the chain must appear in the room's main `<description>~`.
  - `<description>` must include exactly one trailing newline before the terminating `~`.
- End of room: `S`

Any other token in a room body is invalid.

Room description content requirements:

- Each room's main `<description>~` must end with exactly one trailing newline immediately before the terminating `~`.
- Each room's main `<description>~` must contain at least 3 sentences of text.
- Important rooms must contain at least 5 sentences of text in the main `<description>~`.
- Each room's main `<description>~` must be unique to that room (do not reuse identical room descriptions across rooms).
- Room names and descriptions must not use placeholder/procedural naming patterns (for example, `Staging Corridor 12`, `Room 7`, `Passage 3`) or any equivalent numbered-template text. Names and prose must be authored as in-world, thematic content rather than indexed placeholders.
- When a room uses a named exit (non-empty `<exit_keyword>`), that exit name must appear in at least one of:
  - the room's main `<description>~`,
  - an object that spawns in the room, or
  - an `E` extra description in the room.
- Puzzle guidance: you may chain details across multiple `E` extra descriptions; this is acceptable as long as each extra-description chain is anchored in the room's main `<description>~` and any named exit remains discoverable through the room/object/extra-description text.

Directional traversal constraints:

- Except in mazes, exits are expected to be bi-directional: if room `A` connects to room `B` through direction `X`, room `B` should provide the opposite-direction exit back to room `A`.
- This bi-directional expectation also applies to planned cross-area/world links (for example, Area A -> Area B): if an authored connection is intended as a traversable world route, the reverse link in the connected area must be added as part of the same content change.
- Mazes are explicitly exempt from the bi-directional expectation and may use one-way, asymmetric, or otherwise non-reciprocal exits.
- Room connections must not loop back in non-linear patterns (for example, `a -> b -> c -> d -> e -> a -> b`) unless the involved vnum set is a maze and every room in that set has `ROOM_MAZE` set.
- Repeated movement in the same direction must not enter a directional loop unless the involved vnum set is a maze and every room in that set has `ROOM_MAZE` set.
- Example (disallowed outside `ROOM_MAZE`-flagged maze vnum sets): repeatedly taking `east` yields `a -> b -> c -> a`.
- Rooms that are part of a maze must be flagged `ROOM_MAZE`.
- Every vnum in a maze vnum set must be flagged `ROOM_MAZE`.

### 8.1) `room_flags` bitvector

`room_flags` is a bitvector field in room headers (`<room_flags> <sector_type>`). Builder-facing names and values are defined by `tab_room_flags` in `src/buildtab.c`:

- `nada` = `0`
- `dark` = `1`
- `regen` = `2`
- `no_mob` = `4`
- `indoors` = `8`
- `no_magic` = `16`
- `hot` = `32`
- `cold` = `64`
- `pk` = `128`
- `quiet` = `256`
- `private` = `512`
- `safe` = `1024`
- `solitary` = `2048`
- `pet_shop` = `4096`
- `no_recall` = `8192`
- `no_teleport` = `16384`
- `hunt_hunt` = `32768`
- `no_bloodwalk` = `65536`
- `no_portal` = `131072`
- `no_repop` = `262144`
- `maze` = `524288` (`ROOM_MAZE`)

Maze flag requirements:

- Rooms that are part of a maze must have `ROOM_MAZE` set.
- Every vnum in a maze vnum set must have `ROOM_MAZE` set.
- Only `ROOM_MAZE` rooms may use non-linear or looping exits.

### 8.2) `sector_type` values

`sector_type` is a numeric enum in room headers. Builder-facing names and values are defined by `tab_sector_types` in `src/buildtab.c`:

- `nada` = `0`
- `city` = `1`
- `field` = `2`
- `forest` = `3`
- `hills` = `4`
- `mountain` = `5`
- `water_swim` = `6`
- `water_noswim` = `7`
- `recall_set` = `8`
- `air` = `9`
- `desert` = `10`
- `inside` = `11`

### 8.3) Exit `locks` and reset door state values

Door indices in `D<door>` lines map to movement directions (from `compass_name` in `src/act_move.c`):

- `0` = north
- `1` = east
- `2` = south
- `3` = west
- `4` = up
- `5` = down

In room `D<door>` entries, the destination line field `<locks>` is a bitvector over `tab_door_types` in `src/buildtab.c`:

- `door` = `1`
- `closed` = `2`
- `locked` = `4`
- `climb` = `8`
- `immortal` = `16`
- `pickproof` = `32`
- `smashproof` = `64`
- `passproof` = `128`
- `nodetect` = `256`

Practical door behavior in area files/runtime:

- Set `<locks>` bit `door` (`EX_ISDOOR`) when the exit must behave like an actual door/gate that can be opened/closed/locked.
- Exit keywords and doors are independent:
  - A named exit can exist without a door by setting `<exit_keyword>` and leaving `door` (`EX_ISDOOR`) unset.
  - A door can be unnamed by leaving `<exit_keyword>` empty.
  - When a named door is used, start the keyword with `^` (for example, `^stone hatch`) so movement messaging treats it as a standalone noun phrase.
- `closed`/`locked` are runtime state bits (`EX_CLOSED`/`EX_LOCKED`). On save, the area writer strips these two bits from `<locks>`, so persistent initial door state must be authored through `#RESETS` command `D`, not by relying on `closed`/`locked` in the room exit line.
- Any exit that is set to `closed` or `locked` on area reset via `#RESETS` command `D` MUST also have `<locks>` bit `door` (`EX_ISDOOR`) set in its room `D<door>` definition.
- During gameplay, opening/closing/locking/unlocking an exit updates the reverse side too when the reverse exit exists and points back to the source room.
- Locking rules depend on both `<locks>` and `<key_vnum>`:
  - If the exit is not marked as a door (`EX_ISDOOR` unset), lock/unlock/open/close door commands do not apply.
  - If `<key_vnum>` is `< 0`, players will be told the exit cannot be locked/unlocked with a key.
  - If `<key_vnum>` is set, it should match the intended key object's vnum exactly; otherwise unlock/lock attempts with the thematic key will fail.
  - If a door is reset to `locked` via `#RESETS` command `D` state `2`, `<key_vnum>` MUST be set to a valid key object vnum (not `-1`).
  - For every such locked-on-reset door, an object record for that exact key vnum MUST exist in `#OBJECTS` (create the key item if it does not already exist).

In `#RESETS`, command `D` uses door state enum values from `tab_door_states` in `src/buildtab.c`:

- `open` = `0`
- `closed` = `1`
- `locked` = `2`
