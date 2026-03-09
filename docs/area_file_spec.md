# Area File Specification (derived from `src/`)

This document specifies the on-disk area format currently accepted by the loader in `src/db.c` and written by `src/areasave.c`.

## 1) Top-level file structure

An area file is a sequence of named sections. Each section starts with `#<NAME>` and is parsed by `boot_db()`.

Recognized section names:

- `#AREA`
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
- Strings must not contain back-to-back newlines (no blank lines represented by `\n\n`).
- A missing `~` causes parse failure.
- For mobile `long_descr` and mobile `description`, the string must end with exactly one newline immediately before the terminating `~` (i.e., the final line is just `~`, with no extra trailing blank lines).

### 2.1) In-string color codes (`colist`)

Area strings may include inline color escapes using `@@<code>`. The runtime/player `colist` command exposes this same palette (`do_colist()` over `ansi_table`).

Supported `colist` codes:

- `g` gray
- `R` red
- `G` green
- `b` brown
- `B` blue
- `m` magenta
- `c` cyan
- `k` black
- `y` yellow
- `W` white
- `N` normal (reset)
- `p` purple
- `d` dark_grey
- `l` light_blue
- `r` light_green
- `a` light_cyan
- `e` light_red
- `x` bold
- `f` flashing
- `i` inverse
- `2` back_red
- `3` back_green
- `4` back_yellow
- `1` back_blue
- `5` back_magenta
- `6` back_cyan
- `0` back_black
- `7` back_white

Color theme policy:

- Every area must have a defined color theme.
- Theme usage can be sparse or strict, but should be intentional and consistent with the area's identity.
- Black foreground (`@@k`) must not be used as a thematic color in area-authored strings.
- Keep control codes readable/maintainable: avoid noisy color-code churn that obscures base text.
- Automatic tools must not add background color codes (`back_*`, codes `0`-`7`) or the flashing code (`f`).
- Background/flashing codes are allowed only when explicitly added manually by a human author.


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

`V` defines the area's assigned vnum envelope. Area-owned entries must stay inside that envelope (see Structural constraints).

## 4) `#MOBILES` section

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

`<long_descr>` and `<description>` must each include exactly one trailing newline before the terminating `~`.

Optional extension blocks (detected by leading marker):

- `! <class> <clan> <race> <position> <skills> <cast> <def>`
- `| <strong_magic> <weak_magic> <race_mods> <power_skills> <power_cast> <resist> <suscept>`
- `+ <spellpower_mod> <crit_mod> <crit_mult_mod> <spell_crit_mod> <spell_mult_mod> <parry_mod> <dodge_mod> <block_mod> <pierce_mod>`
- `l <loot_amount> <loot[0..MAX_LOOT-1]>`
- `L <loot_chance[0..MAX_LOOT-1]>`

### 4.1) Mobile `skills` bitvector (`!` extension)

`skills` in the `!` extension line is a bitvector. Known flags (from builder lookup tables) are:

Note: this table intentionally uses the historical non-sequential bit assignments from `tab_mob_skill`/`MOB_*`; do not renumber by position.

- `2_attack` = `1`
- `3_attack` = `2`
- `4_attack` = `4`
- `punch` = `128`
- `headbutt` = `256`
- `knee` = `512`
- `disarm` = `1024`
- `trip` = `2048`
- `nodisarm` = `32`
- `notrip` = `64`
- `dodge` = `4096`
- `parry` = `8192`
- `martial` = `16384`
- `enhanced` = `32768`
- `dualwield` = `65536`
- `dirt` = `131072`
- `5_attack` = `8`
- `6_attack` = `16`
- `charge` = `262144`
- `counter` = `524288`
- `kick` = `1048576`

### 4.2) Mobile `cast` bitvector (`!` extension)

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

- `nada` = `1`
- `cure_light` = `2`
- `cure_serious` = `4`
- `cure_critic` = `8`
- `heal` = `16`
- `fireshield` = `32`
- `iceshield` = `64`
- `shockshield` = `128`

### 5.4) Mobile `act` flags bitvector

`act` on the main mobile data line (`<act> <affected_by> <alignment> S`) is a bitvector. Runtime flags are defined in `src/config.h` (builder keywords are mapped in `tab_mob_flags`):

Builder note: set the `aggressive` flag for mobs whose intended behavior is to initiate combat on sight (for example, feral beasts in dangerous wilds, hostile undead in cursed ruins, or demonic sentries in enemy strongholds), and leave it unset for neutral/civilian roles (for example, townsfolk, merchants, quest givers, and guards meant to react only when provoked).

Builder note: mobs that should remain in place and not wander (for example, gate guards) should be flagged with `act sentinel`.

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
- `vampire` = `8388608`
- `no_hunt` = `16777216`
- `solo` = `33554432`
- `boss` = `67108864`
- `no_blood` = `268435456`
- `invasion` = `536870912` (runtime-only flag; do not set in area files)
- `noassist` = `1073741824`

Legacy keyword note from `tab_mob_flags` in `src/buildtab.c`:

- `intelligent` and `mount` appear in the builder lookup table as legacy keywords, but they do not have active `ACT_*` definitions in `src/config.h` in this codebase, so they must not be used in area files.

Builder policy for special difficulty mobs:

- All mobs must be flagged `stay_area` so they do not wander out of their home area.
- Boss mobs must be flagged `sentinel` and `boss` and must be placed only in rooms flagged `no_mob`.
- `invasion` must never be set by builders in area files; it is set/cleared by runtime invasion systems only.
- Strong (non-boss) mobs must be flagged `solo`.

### 5.5) Mobile strong/weak/resist/suscept element flags (`|` extension)

The `|` extension fields `strong_magic`, `weak_magic`, `resist`, and `suscept` are all bitvectors over the element definitions in `src/magic.h` (`ELE_*` / `ELEMENT_*`):

- `none` = `1` (`ELE_NONE`)
- `physical` = `2` (`ELE_PHYSICAL`)
- `mental` = `4` (`ELE_MENTAL`)
- `holy` = `8` (`ELE_HOLY`)
- `air` = `16` (`ELE_AIR`)
- `earth` = `32` (`ELE_EARTH`)
- `water` = `64` (`ELE_WATER`)
- `fire` = `128` (`ELE_FIRE`)
- `shadow` = `256` (`ELE_SHADOW`)
- `poison` = `512` (`ELE_POISON`)

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

Within an area, object `<name>~` values must be unique (no duplicate item names in the same area file).

### 7.1) Item `item_type` enum

`item_type` is the first numeric field on the object header line. Builder-facing names and values are defined by `tab_item_types` in `src/buildtab.c`.

- `light` = `1` (`ITEM_LIGHT`)
- `scroll` = `2` (`ITEM_SCROLL`)
- `null` = `3` (legacy placeholder; no `ITEM_*` define)
- `staff` = `4` (`ITEM_STAFF`)
- `weapon` = `5` (`ITEM_WEAPON`)
- `beacon` = `6` (`ITEM_BEACON`)
- `portal` = `7` (`ITEM_PORTAL`)
- `treasure` = `8` (`ITEM_TREASURE`)
- `armor` = `9` (`ITEM_ARMOR`)
- `potion` = `10` (`ITEM_POTION`)
- `clutch` = `11` (`ITEM_CLUTCH`)
- `furniture` = `12` (`ITEM_FURNITURE`)
- `trash` = `13` (`ITEM_TRASH`)
- `trigger` = `14` (`ITEM_TRIGGER`)
- `container` = `15` (`ITEM_CONTAINER`)
- `quest` = `16` (`ITEM_QUEST`)
- `drink_con` = `17` (`ITEM_DRINK_CON`)
- `key` = `18` (`ITEM_KEY`)
- `food` = `19` (`ITEM_FOOD`)
- `money` = `20` (`ITEM_MONEY`)
- `stake` = `21` (`ITEM_STAKE`)
- `boat` = `22` (`ITEM_BOAT`)
- `corpse_npc` = `23` (`ITEM_CORPSE_NPC`)
- `corpse_pc` = `24` (`ITEM_CORPSE_PC`)
- `fountain` = `25` (`ITEM_FOUNTAIN`)
- `pill` = `26` (`ITEM_PILL`)
- `board` = `27` (`ITEM_BOARD`)
- `soul` = `28` (`ITEM_SOUL`)
- `piece` = `29` (`ITEM_PIECE`)
- `matrix` = `30` (`ITEM_SPELL_MATRIX`)
- `enchantment` = `31` (`ITEM_ENCHANTMENT`)
- `present` = `32` (`ITEM_WRAPPED_PRESENT`)
- `stash` = `33` (`ITEM_STASH`)

### 7.2) Item `extra_flags` bitvector

`extra_flags` is a bitvector field in the object header line and supports the following bits from `src/config.h`:

Area-authored object definitions must **not** set `ITEM_GENERATED`; that flag is runtime-managed by item generation systems.

`ITEM_LOOT` usage policy: items intended to be spawned from a mobile's loot table (`#MOBILES` `l`/`L` extension data) must have `ITEM_LOOT` set in `extra_flags`. Items loaded onto mobiles via `#RESETS` (`G`/`E`) are normal reset equipment/inventory and must not be treated as loot-only objects unless explicitly intended.

`BOSS` extra-flag usage policy: any item that can drop from a boss mob must include the `BOSS` item extra flag, regardless of how it is delivered (loot-table drop via `l`/`L` or normal equipment/inventory placed through resets).

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
- `ITEM_MYTHIC` = `262144`
- `ITEM_LEGENDARY` = `524288`
- `ITEM_RARE` = `1048576`
- `ITEM_VAMP` = `2097152`
- `ITEM_NOLOOT` = `4194304`
- `ITEM_NOSAC` = `8388608`
- `ITEM_UNIQUE` = `16777216`
- `ITEM_LIFESTEALER` = `33554432`
- `ITEM_LOOT` = `67108864`
- `ITEM_BOSS` = `134217728`
- `ITEM_BUCKLER` = `268435456`
- `ITEM_EXTRA_WAND` = `536870912`
- `ITEM_FIST` = `1073741824`
- `ITEM_TWO_HANDED` = `2147483648`

### 7.3) Item `wear_flags` bitvector

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

Note: `ITEM_WEAR_NONE` (`1`) exists in `src/config.h`, but it is not exposed in `tab_wear_flags` and therefore is not a valid builder keyword in area files.

Area policy constraints for object wear flags:

- Every object **must** include `ITEM_TAKE`.
- No object may include `ITEM_WEAR_CLAN_COLORS`.
- Object `name`, `short_descr`, and `description` fields must be thematically consistent with the object's non-`take` wear flags (e.g., a `head` item must read as headgear, `wrist` as wristwear, `hold` as a held item).

Builder conventions for held equipment archetypes:

- A **shield** is an object with `item_type = ITEM_ARMOR` and wear flags including `hold`.
- A **weapon** is an object with `item_type = ITEM_WEAPON` and wear flags including both `hold` and `take`.
- A **buckler** is an object with `item_type = ITEM_ARMOR`, wear flags including `hold`, and extra flags including `buckler`.
- A **wand** is an object with `item_type = ITEM_WEAPON`, wear flags including `hold`, and extra flags including `wand`.
- A **fist weapon** is an object with `item_type = ITEM_WEAPON`, wear flags including `hold`, and extra flags including `fist`.
- A **two-handed weapon** is an object with `item_type = ITEM_WEAPON`, wear flags including `hold`, and extra flags including `two-handed`.

Area policy constraint for naming consistency: if a weapon's presented identity (`name`, `short_descr`, or `description`) clearly indicates a two-handed archetype (for example, a "great axe"), it must include the `two-handed` extra flag.

Additional constraint: an item may only be set to `item_type = ITEM_WEAPON` if its wear flags include both `hold` and `take`.

### 7.4) Item `item_apply` bitvector

`item_apply` is a bitvector field in the object header line. Builder-facing names and values are defined by `tab_item_apply` in `src/buildtab.c`:

- `nada` = `1`
- `infra` = `2`
- `invis` = `4`
- `det_invis` = `8`
- `sanc` = `16`
- `sneak` = `32`
- `hide` = `64`
- `prot` = `128`
- `enhanced` = `256`
- `det_mag` = `512`
- `det_hid` = `1024`
- `det_evil` = `2048`
- `pass_door` = `4096`
- `det_poison` = `8192`
- `fly` = `16384`
- `know_align` = `32768`
- `detect_undead` = `65536`
- `heated` = `131072`

Notes:

- `nada` (`ITEM_APPLY_NONE`) is the baseline/default bit and is set on newly created objects.
- This field is additive, so multiple `item_apply` effects may be combined with bitwise OR.

### 7.5) `ITEM_PIECE` object values

For objects with `item_type = 29` (`ITEM_PIECE`), the first three value slots define how piece-combining works (`connect` command in `src/act_obj.c`):

- `value0`: previous-piece vnum in the connection chain
- `value1`: next-piece vnum in the connection chain
- `value2`: replacement object vnum created when two matching pieces are connected

At runtime, two piece objects connect successfully when either `pieceA.value0 == pieceB.vnum` or `pieceA.value1 == pieceB.vnum`. On success, both pieces are consumed and a new object with vnum `pieceA.value2` is created.

### 7.6) `ITEM_WEAPON` object values

For objects with `item_type = 5` (`ITEM_WEAPON`), `value3` stores the weapon attack type (`Weapon Type` in builder displays).

Allowed `value3` values (from `tab_weapon_types` in `src/buildtab.c`):

Area policy constraints for `ITEM_WEAPON`:

- `value3` must be thematically consistent with the weapon's concept and presentation (`name`, `short_descr`, and `description`) so combat messaging matches builder intent.
- `value3 = 0` (`hit`) must not be used unless the object also has the `ITEM_FIST` extra flag.

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
  - Named exits are allowed by setting a non-empty `<exit_keyword>` when it improves room flavor/clarity.
  - For door-style named exits, prefix the keyword with `^` (for example, `^iron gate`).
  - `<key_vnum>` must be the vnum of a key object that unlocks this exit. Use `-1` when no key exists for the exit.
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

- Each room's main `<description>~` must end with exactly one trailing newline immediately before the terminating `~`.
- Each room's main `<description>~` must contain at least 3 lines of text.
- Important rooms must contain at least 5 lines of text in the main `<description>~`.
- Each room's main `<description>~` must be unique to that room (do not reuse identical room descriptions across rooms).

Directional traversal constraints:

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
- During gameplay, opening/closing/locking/unlocking an exit updates the reverse side too when the reverse exit exists and points back to the source room.
- Locking rules depend on both `<locks>` and `<key_vnum>`:
  - If the exit is not marked as a door (`EX_ISDOOR` unset), lock/unlock/open/close door commands do not apply.
  - If `<key_vnum>` is `< 0`, players will be told the exit cannot be locked/unlocked with a key.
  - If `<key_vnum>` is set, it should match the intended key object's vnum exactly; otherwise unlock/lock attempts with the thematic key will fail.

In `#RESETS`, command `D` uses door state enum values from `tab_door_states` in `src/buildtab.c`:

- `open` = `0`
- `closed` = `1`
- `locked` = `2`

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

- `#RESETS` must not contain blank lines.
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
- `P`: put object into previous room-reset object/container (`arg1` object vnum, `arg2` limit, `arg3` container object vnum)
- `A`: obsolete; loader/checker still recognize it

Builder editor mapping:

- `addreset put <obj-vnum> <container-vnum>` emits reset command `P` with `arg1=<obj-vnum>`, `arg2=0`, and `arg3=<container-vnum>`.

Reset vnum validity rule:

- Resets must reference valid vnums for the target type required by the command (room/mobile/object as applicable).

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
- Bitvector hygiene: undefined bits must be removed from serialized fields when detected.
  - Do not persist undefined/unknown bits in any bitvector-backed area field.
  - Keep only bits explicitly defined for that field (or documented runtime-only exceptions where applicable).

## 13.1) Vnum allocation policy

For all content types (`#MOBILES`, `#ROOMS`, and `#OBJECTS`), vnums must be assigned in ascending sequential order.

- Use lower available vnums before higher ones (i.e., use `1` before `2`, `2` before `3`, and so on).
- Do not leave gaps in vnum sequences.
- When gaps exist in an area's assigned vnum range, fill those gaps wherever possible before assigning new higher vnums.
- For `#ROOMS`, attempt to use all room vnums in the area's assigned range (i.e., fill every available room-vnum slot where practical).

## 14) Canonical section order emitted by saver

`areasave.c` writes sections in this order when present:

1. `#AREA`
2. `#MOBILES`
3. `#MOBPROGS`
4. `#OBJECTS`
5. `#ROOMS`
6. `#SHOPS`
7. `#SPECIALS`
8. `#OBJFUNS`
9. `#RESETS`
10. `#$`

Using this order is recommended for consistency, even though the loader dispatch is name-based.
