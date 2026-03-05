# Area File Specification (derived from `src/`)

This document specifies the on-disk area format currently accepted by the loader in `src/db.c` and written by `src/areasave.c`.

## 1) Top-level file structure

An area file is a sequence of named sections. Each section starts with `#<NAME>` and is parsed by `boot_db()`.

Recognized section names:

- `#AREA`
- `#HELPS`
- `#MOBILES`
- `#MOBPROGS`
- `#OBJECTS`
- `#ROOMS`
- `#SHOPS`
- `#SPECIALS`
- `#OBJFUNS`
- `#$` (end-of-file marker)

Notes:

- Section order is flexible for loading (the loader dispatches by section name).
- Some sections are optional.
- Section names are case-sensitive in area files as written.

## 2) String encoding

Most text fields are read with `fread_string()`: they must be terminated by `~`.

Implications:

- Single-line and multi-line strings are allowed, but a terminating `~` is required.
- A missing `~` causes parse failure.

## 3) `#AREA` section

`#AREA` contains one required string, followed by zero or more single-letter directives:

```text
#AREA
<name>~
<directives...>
```

Supported directives:

- `Q <int>`: area revision/version
- `K <string>~`: keyword
- `L <string>~`: level label
- `N <int>`: area number
- `I <int> <int>`: min/max level
- `V <int> <int>`: min/max vnum
- `X <int>`: map offset
- `F <int>`: reset rate
- `U <string>~`: reset message
- `O <string>~`: owner
- `R <string>~`: can_read ACL
- `W <string>~`: can_write ACL
- `P ...`: pay-area flag (rest of line ignored)
- `T ...`: teleport flag (rest of line ignored)
- `B ...`: building flag (rest of line ignored)
- `S ...`: no-show flag (rest of line ignored)
- `M ...`: no-room-affects flag (rest of line ignored)

Parsing stops when the next `#` section header is encountered.

## 4) `#HELPS` section

A list of help entries terminated by `0 $~`:

```text
#HELPS
<level> <keyword>~
<text>~
...
0 $~
```

- `<level>` is an integer (signed values accepted by parser).
- If help text starts with whitespace when saved, saver prefixes `.` to preserve formatting.

## 5) `#MOBILES` section

A list of mobile records, each introduced by `#<vnum>`, terminated by `#0`:

```text
#MOBILES
#<vnum>
<player_name>~
<short_descr>~
<long_descr>~
<description>~
<act> <affected_by> <alignment> S
<level> <sex>
<hp_mod> <ac_mod> <hr_mod> <dr_mod>
[optional extension lines]
[optional inline mobprog blocks]
...
#0
```

Optional extension blocks (detected by leading marker):

- `! <class> <clan> <race> <position> <skills> <cast> <def>`
- `| <strong_magic> <weak_magic> <race_mods> <power_skills> <power_cast> <resist> <suscept>`
- `+ <spellpower_mod> <crit_mod> <crit_mult_mod> <spell_crit_mod> <spell_mult_mod> <parry_mod> <dodge_mod> <block_mod> <pierce_mod>`
- `l <loot_amount> <loot[0..MAX_LOOT-1]>`
- `L <loot_chance[0..MAX_LOOT-1]>`

Optional inline mobprogs inside a mobile entry:

```text
> <prog_type_name> <arglist>~
<comlist>~
...
|
```

## 6) `#MOBPROGS` section

External mobprog file bindings, terminated by `S`:

```text
#MOBPROGS
M <mob_vnum> <filename>
...
S
```

Comment lines beginning with `*` are accepted.

## 7) `#OBJECTS` section

A list of object records terminated by `#0`:

```text
#OBJECTS
#<vnum>
<name>~
<short_descr>~
<description>~
<item_type> <extra_flags> <wear_flags> <item_apply>
<value0> <value1> <value2> <value3> <value4> <value5> <value6> <value7> <value8> <value9>
<weight>
[zero or more A/E/L entries]
...
#0
```

Per-object trailing entries:

- `A` then `<location> <modifier>` on next line (object affect)
- `E` then `<keyword>~` and `<description>~` (extra description)
- `L` then `<level>` on next line

Loader stops this trailing-entry loop at first unrecognized marker (which starts next object/terminator).

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
- Extra description:
  ```text
  E
  <keyword>~
  <description>~
  ```
- End of room: `S`

Any other token in a room body is invalid.

## 9) `#SHOPS` section

Terminated by keeper `0`:

```text
#SHOPS
<keeper> <buy_type_0> ... <buy_type_(MAX_TRADE-1)> <profit_buy> <profit_sell> <open_hour> <close_hour>
...
0
```

## 10) `#SPECIALS` section

Terminated by `S`:

```text
#SPECIALS
M <mob_vnum> <spec_fun_name>
...
S
```

`*` comment lines are accepted.

## 11) `#OBJFUNS` section

Terminated by `S`:

```text
#OBJFUNS
O <obj_vnum> <obj_fun_name>
...
S
```

`*` comment lines are accepted.

## 12) `#RESETS` section

Terminated by `S`:

```text
#RESETS
<command> <ifflag> <arg1> <arg2> [arg3] <notes...>
...
S
```

Encoding details:

- For commands `G` and `R`, there is **no** `arg3` in file; parser sets it to `0`.
- For other commands, `arg3` is required.
- Remaining text on the line is stored as `notes`.

Commands used by validation/editor paths:

- `M`: load mob (`arg1` mob vnum, `arg2` limit, `arg3` room vnum)
- `O`: load object into room (`arg1` obj vnum, `arg2` limit, `arg3` room vnum)
- `G`: give object to previous mob (`arg1` obj vnum, `arg2` limit)
- `E`: equip object on previous mob (`arg1` obj vnum, `arg2` limit, `arg3` wear_loc)
- `D`: set door state (`arg1` room vnum, `arg2` door 0..5, `arg3` state 0..2)
- `R`: randomize exits (`arg1` room vnum, `arg2` max door)
- `A`/`P`: obsolete; loader/checker still recognize them

## 13) Structural constraints enforced by tests

From `src/test_area_format.c` and `src/test_wood_area.c`:

- `#ROOMS` must exist.
- `#ROOMS` must end with `#0` before the next section header.
- Inside `#ROOMS`, each room body may contain only `D`, `E`, or `S` entries.
- `D` entries must include two `~` strings and a destination line with exactly three integers.
- If `#MOBILES` exists, it must terminate with `#0` before `#OBJECTS`.
- If `#OBJECTS` exists, it must terminate with `#0` before `#RESETS`.
- `#ROOMS` must contain at least one room in listed area files.

## 14) Canonical section order emitted by saver

`areasave.c` writes sections in this order when present:

1. `#AREA`
2. `#HELPS`
3. `#MOBILES`
4. `#MOBPROGS`
5. `#OBJECTS`
6. `#ROOMS`
7. `#SHOPS`
8. `#SPECIALS`
9. `#OBJFUNS`
10. `#RESETS`
11. `#$`

Using this order is recommended for consistency, even though the loader dispatch is name-based.
