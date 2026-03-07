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
- Area files must not contain comments; comment syntax is not part of the accepted on-disk format.

## 2) String encoding

Most text fields are read with `fread_string()`: they must be terminated by `~`.

Implications:

- Single-line and multi-line strings are allowed, but a terminating `~` is required.
- A missing `~` causes parse failure.
- For mobile `long_descr` and all extra-description text blocks (`E` entries in `#OBJECTS` and `#ROOMS`), the string must end with exactly one newline immediately before the terminating `~` (i.e., the final line is just `~`).

## 3) `#AREA` section

`#AREA` contains one required string, followed by directives. The `Q` directive is mandatory and must be set to version `16`:

```text
#AREA
<name>~
<directives...>
```

Supported directives:

- `Q 16`: required area revision/version (other values are out of spec)
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

`V` defines the area's assigned vnum envelope. Area-owned entries should stay inside that envelope (see Structural constraints).

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

`<long_descr>` must include exactly one trailing newline before the terminating `~`.

Optional extension blocks (detected by leading marker):

- `! <class> <clan> <race> <position> <skills> <cast> <def>`
- `| <strong_magic> <weak_magic> <race_mods> <power_skills> <power_cast> <resist> <suscept>`
- `+ <spellpower_mod> <crit_mod> <crit_mult_mod> <spell_crit_mod> <spell_mult_mod> <parry_mod> <dodge_mod> <block_mod> <pierce_mod>`
- `l <loot_amount> <loot[0..MAX_LOOT-1]>`
- `L <loot_chance[0..MAX_LOOT-1]>`

### 5.1) Mobile `skills` bitvector (`!` extension)

`skills` in the `!` extension line is a bitvector. Known flags (from builder lookup tables) are:

- `2_attack` = `1`
- `3_attack` = `2`
- `4_attack` = `4`
- `punch` = `8`
- `headbutt` = `16`
- `knee` = `32`
- `disarm` = `64`
- `trip` = `128`
- `nodisarm` = `256`
- `notrip` = `512`
- `dodge` = `1024`
- `parry` = `2048`
- `martial` = `4096`
- `enhanced` = `8192`
- `dualwield` = `16384`
- `dirt` = `32768`
- `5_attack` = `65536`
- `6_attack` = `131072`
- `charge` = `262144`
- `counter` = `524288`
- `kick` = `1048576`

### 5.2) Mobile `cast` bitvector (`!` extension)

`cast` in the `!` extension line is a bitvector of offensive spells the mobile may use:

- `nada` = `0`
- `placeholder` = `1`
- `mag_missile` = `2`
- `shock_grasp` = `4`
- `burn_hands` = `8`
- `col_spray` = `16`
- `fireball` = `32`
- `hellspawn` = `64`
- `acid_blast` = `128`
- `chain_light` = `256`
- `faerie_fire` = `512`
- `flare` = `1024`
- `flamestrike` = `2048`
- `earthquake` = `4096`
- `mind_flail` = `8192`
- `planergy` = `16384`
- `phobia` = `32768`
- `mind_bolt` = `65536`
- `static` = `131072`
- `ego_whip` = `262144`
- `bloody_tears` = `524288`
- `mindflame` = `1048576`
- `suffocate` = `2097152`
- `nerve_fire` = `4194304`
- `light_bolt` = `8388608`
- `heat_armor` = `16777216`
- `lava_burst` = `33554432`

### 5.3) Mobile `def` bitvector (`!` extension)

`def` in the `!` extension line is a bitvector of defensive spells/abilities:

- `nada` = `0`
- `cure_light` = `1`
- `cure_serious` = `2`
- `cure_critic` = `4`
- `heal` = `8`
- `fireshield` = `16`
- `iceshield` = `32`
- `shockshield` = `64`

### 5.4) Mobile `act` flags bitvector

`act` on the main mobile data line (`<act> <affected_by> <alignment> S`) is a bitvector with these known flags:

- `is_npc` = `1`
- `sentinel` = `2`
- `scavenger` = `4`
- `remember` = `8`
- `no_flee` = `16`
- `aggressive` = `32`
- `stay_area` = `64`
- `wimpy` = `128`
- `pet` = `256`
- `train` = `512`
- `practice` = `1024`
- `mercenary` = `2048`
- `heal` = `4096`
- `adapt` = `8192`
- `undead` = `16384`
- `bank` = `32768`
- `no_body` = `65536`
- `hunter` = `131072`
- `no_mind` = `262144`
- `postman` = `524288`
- `rewield` = `1048576`
- `reequip` = `2097152`
- `intelligent` = `BIT_23`
- `vampire` = `BIT_24`
- `no_hunt` = `BIT_25`
- `solo` = `BIT_26`
- `mount` = `BIT_28`
- `no_blood` = `BIT_29`
- `invasion` = `BIT_30`
- `noassist` = `BIT_31`

### 5.5) Mobile strong/weak/resist/suscept element flags (`|` extension)

The `|` extension fields `strong_magic`, `weak_magic`, `resist`, and `suscept` are all bitvectors over the element definitions in `src/magic.h` (`ELE_*` / `ELEMENT_*`):

- `none` = `BIT_0` (`ELE_NONE`)
- `physical` = `BIT_1` (`ELE_PHYSICAL`)
- `mental` = `BIT_2` (`ELE_MENTAL`)
- `holy` = `BIT_3` (`ELE_HOLY`)
- `air` = `BIT_4` (`ELE_AIR`)
- `earth` = `BIT_5` (`ELE_EARTH`)
- `water` = `BIT_6` (`ELE_WATER`)
- `fire` = `BIT_7` (`ELE_FIRE`)
- `shadow` = `BIT_8` (`ELE_SHADOW`)
- `poison` = `BIT_9` (`ELE_POISON`)

### 5.6) Mobile program trigger names

For inline mobile program blocks (`> <prog_type_name> ...`), the loader accepts these trigger type names:

- `in_file_prog`
- `act_prog`
- `speech_prog`
- `rand_prog`
- `fight_prog`
- `hitprcnt_prog`
- `death_prog`
- `entry_prog`
- `greet_prog`
- `all_greet_prog`
- `give_prog`
- `bribe_prog`

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

Object weight ranges encode item archetype in area content policy:

- `1`-`5`: caster item
- `6`-`10`: melee item
- `11`-`15`: tank item

Per-object trailing entries:

- `A` then `<location> <modifier>` on next line (object affect)
- `E` then `<keyword>~` and `<description>~` (extra description)
- `L` then `<level>` on next line

For `E` entries, `<description>` must include exactly one trailing newline before the terminating `~`.

Loader stops this trailing-entry loop at first unrecognized marker (which starts next object/terminator).

### 7.1) Item `extra_flags` bitvector

`extra_flags` is a bitvector field in the object header line and supports the following bits from `src/config.h`:

- `ITEM_GENERATED` = `1`
- `ITEM_BIND_EQUIP` = `2`
- `ITEM_NODISARM` = `4`
- `ITEM_LOCK` = `8`
- `ITEM_EVIL` = `16`
- `ITEM_INVIS` = `32`
- `ITEM_MAGIC` = `64`
- `ITEM_NODROP` = `128`
- `ITEM_BLESS` = `256`
- `ITEM_ANTI_GOOD` = `512`
- `ITEM_ANTI_EVIL` = `1024`
- `ITEM_ANTI_NEUTRAL` = `2048`
- `ITEM_NOREMOVE` = `4096`
- `ITEM_INVENTORY` = `8192`
- `ITEM_NOSAVE` = `16384`
- `ITEM_CLAN_EQ` = `32768`
- `ITEM_TRIG_DESTROY` = `65536`
- `ITEM_NO_AUCTION` = `131072`
- `ITEM_MYTHIC` = `BIT_19`
- `ITEM_LEGENDARY` = `BIT_20`
- `ITEM_RARE` = `BIT_21`
- `ITEM_VAMP` = `BIT_22`
- `ITEM_NOLOOT` = `BIT_23`
- `ITEM_NOSAC` = `BIT_24`
- `ITEM_UNIQUE` = `BIT_25`
- `ITEM_LIFESTEALER` = `BIT_26`
- `ITEM_LOOT` = `BIT_27`
- `ITEM_BUCKLER` = `BIT_29`
- `ITEM_EXTRA_WAND` = `BIT_30`
- `ITEM_FIST` = `BIT_31`
- `ITEM_TWO_HANDED` = `BIT_32`

### 7.2) Item `wear_flags` bitvector

`wear_flags` is a bitvector field in the object header line. Builder-facing names and values are defined by `tab_wear_flags` in `src/buildtab.c`:

- `halo` = `1`
- `aura` = `2`
- `horns` = `4`
- `head` = `8`
- `face` = `16`
- `beak` = `32`
- `ear` = `64`
- `neck` = `128`
- `wings` = `256`
- `shoulders` = `512`
- `arms` = `1024`
- `wrist` = `2048`
- `hands` = `4096`
- `finger` = `8192`
- `claws` = `16384`
- `hold` = `32768`
- `about` = `65536`
- `waist` = `131072`
- `body` = `262144`
- `tail` = `524288`
- `legs` = `1048576`
- `feet` = `2097152`
- `hooves` = `4194304`
- `take` = `8388608`
- `clan_colors` = `16777216`

Note: `ITEM_WEAR_NONE` (`BIT_0`) exists in `src/config.h`, but it is not exposed in `tab_wear_flags` and therefore is not a valid builder keyword in area files.

Area policy constraints for object wear flags:

- Every object **must** include `ITEM_TAKE`.
- No object may include `ITEM_WEAR_CLAN_COLORS`.
- Object `name`, `short_descr`, and `description` fields should be thematically consistent with the object's non-`take` wear flags (e.g., a `head` item should read as headgear, `wrist` as wristwear, `hold` as a held item).

Builder conventions for held equipment archetypes:

- A **shield** is an object with `item_type = ITEM_ARMOR` and wear flags including `hold`.
- A **weapon** is an object with `item_type = ITEM_WEAPON` and wear flags including `hold`.
- A **buckler** is an object with `item_type = ITEM_ARMOR`, wear flags including `hold`, and extra flags including `buckler`.
- A **wand** is an object with `item_type = ITEM_WEAPON`, wear flags including `hold`, and extra flags including `wand`.
- A **fist weapon** is an object with `item_type = ITEM_WEAPON`, wear flags including `hold`, and extra flags including `fist`.
- A **two-handed weapon** is an object with `item_type = ITEM_WEAPON`, wear flags including `hold`, and extra flags including `two-handed`.

### 7.3) `ITEM_PIECE` object values

For objects with `item_type = 29` (`ITEM_PIECE`), the first three value slots define how piece-combining works (`connect` command in `src/act_obj.c`):

- `value0`: previous-piece vnum in the connection chain
- `value1`: next-piece vnum in the connection chain
- `value2`: replacement object vnum created when two matching pieces are connected

At runtime, two piece objects connect successfully when either `pieceA.value0 == pieceB.vnum` or `pieceA.value1 == pieceB.vnum`. On success, both pieces are consumed and a new object with vnum `pieceA.value2` is created.

### 7.4) `ITEM_WEAPON` object values

For objects with `item_type = 5` (`ITEM_WEAPON`), `value3` stores the weapon attack type (`Weapon Type` in builder displays).

Allowed `value3` values (from `tab_weapon_types` in `src/buildtab.c`):

Area policy constraint: for `ITEM_WEAPON`, `value3` should be thematically consistent with the weapon's concept and presentation (`name`, `short_descr`, and `description`) so combat messaging matches builder intent.

- `0`: `hit`
- `1`: `slice`
- `2`: `stab`
- `3`: `slash`
- `4`: `whip`
- `5`: `claw`
- `6`: `blast`
- `7`: `pound`
- `8`: `crush`
- `9`: `grep`
- `10`: `bite`
- `11`: `pierce`
- `12`: `suction`

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
  - `<description>` must include exactly one trailing newline before the terminating `~`.
- End of room: `S`

Any other token in a room body is invalid.

Room description content requirements:

- Each room's main `<description>~` must contain at least 3 lines of text.
- Each room's main `<description>~` must be unique to that room (do not reuse identical room descriptions across rooms).

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

From `src/test_area_format.c`, `src/test_wood_area.c`, and `src/test_db.c`:

- `#ROOMS` must exist.
- `#ROOMS` must end with `#0` before the next section header.
- Inside `#ROOMS`, each room body may contain only `D`, `E`, or `S` entries.
- `D` entries must include two `~` strings and a destination line with exactly three integers.
- If `#MOBILES` exists, it must terminate with `#0` before `#OBJECTS`.
- If `#OBJECTS` exists, it must terminate with `#0` before `#RESETS`.
- `#ROOMS` must contain at least one room in listed area files.
- Area-owned vnums must stay inside the `#AREA` `V <min> <max>` range:
  - `#ROOMS`, `#MOBILES`, and `#OBJECTS` entry headers (`#<vnum>`)
  - `#SHOPS` shopkeeper vnums (first integer on each shop line)
  - `#SPECIALS` targets for `M` and `O` lines
- Duplicate index vnums are invalid for `#ROOMS`, `#MOBILES`, and `#OBJECTS`:
  - A given room vnum may appear only once across all loaded area files.
  - A given mobile vnum may appear only once across all loaded area files.
  - A given object vnum may appear only once across all loaded area files.
  - Cross-type overlap is allowed: a room, mobile, and object may share the same numeric vnum as long as each remains unique within its own index type.

## 13.1) Vnum allocation policy

For all content types (`#MOBILES`, `#ROOMS`, and `#OBJECTS`), vnums should be assigned in ascending sequential order.

- Use lower available vnums before higher ones (i.e., use `1` before `2`, `2` before `3`, and so on).
- Do not leave gaps in vnum sequences.
- When gaps exist in an area's assigned vnum range, fill those gaps wherever possible before assigning new higher vnums.
- For `#ROOMS`, attempt to use all room vnums in the area's assigned range (i.e., fill every available room-vnum slot where practical).

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
