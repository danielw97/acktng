# Area File Specification (derived from `src/`)

This document specifies the on-disk area format currently accepted by the loader in `src/db.c` and written by `src/areasave.c`.

## 1) Top-level file structure

An area file is a sequence of named sections. Each section starts with `#<NAME>` and is parsed by `boot_db()`.

Recognized section names (saver order shown):

- `#AREA`
- `#ROOMS`
- `#MOBILES`
- `#OBJECTS`
- `#SHOPS`
- `#RESETS`
- `#SPECIALS`
- `#OBJFUNS`
- `#$` (end-of-file marker)

Notes:

- Section order is flexible for loading (the loader dispatches by section name).
- Some sections are optional.
- Section names are case-sensitive in area files as written.
- Area files must not contain comments of any kind. No section accepts comment lines. This applies to all sections including `#MOBILES`, `#OBJECTS`, `#RESETS`, `#SPECIALS`, and `#OBJFUNS`.

## 2) String encoding

Most text fields are read with `fread_string()`: they must be terminated by `~`.

Implications:

- Single-line and multi-line strings are allowed, but a terminating `~` is required.
- Strings must not contain back-to-back newlines (no blank lines represented by `\n\n`).
- A missing `~` causes parse failure.
- Mobile `long_descr` has a strict format: exactly one text line, then a newline, then a line containing only `~`. No other `long_descr` format is valid, and multi-line `long_descr` text is never allowed.
- Mobile `description` must end with exactly one newline immediately before the terminating `~` (i.e., the final line is just `~`, with no extra trailing blank lines).
- Vnums must never be mentioned in in-world description text. This includes room descriptions, mobile descriptions (`long_descr`/`description`), object descriptions, extra descriptions, and exit descriptions.

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
  - Owner policy: area headers must use `O Virant~`.
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

Mobile section details were extracted to [docs/mob_spec.md](mob_spec.md).

## 7) `#OBJECTS` section

Object section details were extracted to [docs/object_spec.md](object_spec.md).

## 8) `#ROOMS` section

Room section details were extracted to [docs/room_spec.md](room_spec.md).

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

Allowed `<spec_fun_name>` values (from `spec_lookup()` in `src/special.c`):

For full descriptions of each special's behavior, builder policies, and usage
guidance, see **`docs/mob_specials_spec.md`**.

Builder policy: mob specials beginning with `spec_summon_` must **never** be set in area files. These are assigned at runtime by summon systems only.
Builder policy: any mob special beginning with `spec_keep_` may only be assigned manually by a human (not by automation/tools).

**Breath weapon specials:**
- `spec_breath_any`
- `spec_breath_acid`
- `spec_breath_fire`
- `spec_breath_frost`
- `spec_breath_gas`
- `spec_breath_lightning`

**Combat caster specials:**
- `spec_cast_adept`
- `spec_cast_cleric`
- `spec_cast_judge`
- `spec_cast_mage`
- `spec_cast_undead`
- `spec_cast_cadaver`
- `spec_cast_bigtime`

**Law enforcement specials:**
- `spec_executioner`
- `spec_guard` (alias of `spec_policeman` in lookup return)
- `spec_policeman`
- `spec_mino_guard`

**NPC behavior specials:**
- `spec_fido`
- `spec_janitor`
- `spec_mayor`
- `spec_poison`
- `spec_thief`
- `spec_rewield`
- `spec_undead`
- `spec_stephen`
- `spec_sage`
- `spec_wizardofoz`
- `spec_tax_man`
- `spec_sylai_priest`
- `spec_keep_physical_captain` (human-assignment only)

**Ambient city specials:**
- `spec_lamplighter`
- `spec_warden`
- `spec_vendor`
- `spec_lay_sister`
- `spec_laborer`

**Scorching Sands ambient specials:**
- `spec_ss_cinder_broker`
- `spec_ss_manifest_warden`
- `spec_ss_kiln_overseer`

**Kiess ambient specials:**
- `spec_kiess_shopkeeper`
- `spec_kiess_innkeeper`
- `spec_kiess_scout`
- `spec_kiess_orator`
- `spec_kiess_wall_officer`

**Summon specials (runtime-only — do not set in area files):**
- `spec_summon_water`
- `spec_summon_fire`
- `spec_summon_earth`
- `spec_summon_undead`
- `spec_summon_holy`
- `spec_summon_shadow`
- `spec_summon_metal`
- `spec_summon_animate`
- `spec_summon_thought`

## 11) `#OBJFUNS` section

Terminated by `S`:

```text
#OBJFUNS
O <obj_vnum> <obj_fun_name>
...
S
```

Allowed `<obj_fun_name>` values (from `obj_fun_lookup()` in `src/obj_fun.c`):

Builder policy constraints for object-function/flag alignment:

- Any object with `ITEM_LIFESTEALER` in `extra_flags` must also include `ITEM_ANTI_GOOD`.
- Any object assigned `objfun_healing` in `#OBJFUNS` must include `ITEM_ANTI_EVIL` in `extra_flags`.

- `objfun_giggle`
- `objfun_cast_fight`
- `objfun_sword_aggro`
- `objfun_soul_moan`
- `objfun_infused_soul`
- `objfun_flaming`
- `objfun_healing`
- `objfun_dispeller`
- `objfun_regen`
- `objfun_clan`

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

### `E` command wear_loc values

`wear_loc` in the `E` reset command maps to `WEAR_*` constants from `src/headers/config.h`:

| Value | Constant | Slot |
|------:|----------|------|
| 0 | `WEAR_LIGHT` | light source |
| 1 | `WEAR_HALO` | halo |
| 2 | `WEAR_AURA` | aura |
| 3 | `WEAR_HORNS` | horns |
| 4 | `WEAR_HEAD` | head |
| 5 | `WEAR_FACE` | face |
| 6 | `WEAR_BEAK` | beak |
| 7 | `WEAR_EAR_L` | left ear |
| 8 | `WEAR_EAR_R` | right ear |
| 9 | `WEAR_NECK_1` | neck (first) |
| 10 | `WEAR_NECK_2` | neck (second) |
| 11 | `WEAR_WINGS` | wings |
| 12 | `WEAR_SHOULDERS` | shoulders |
| 13 | `WEAR_ARMS` | arms |
| 14 | `WEAR_WRIST_L` | left wrist |
| 15 | `WEAR_WRIST_R` | right wrist |
| 16 | `WEAR_HANDS` | hands |
| 17 | `WEAR_FINGER_L` | left finger |
| 18 | `WEAR_FINGER_R` | right finger |
| 19 | `WEAR_CLAWS` | claws |
| 20 | `WEAR_HOLD_HAND_L` | left hand hold |
| 21 | `WEAR_HOLD_HAND_R` | right hand hold (primary weapon/shield slot) |
| 22 | `WEAR_TWO_HANDED` | two-handed weapon |
| 23 | `WEAR_BUCKLER` | buckler |
| 24 | `WEAR_ABOUT` | about body (cloak) |
| 25 | `WEAR_WAIST` | waist |
| 26 | `WEAR_BODY` | body |
| 27 | `WEAR_TAIL` | tail |
| 28 | `WEAR_LEGS` | legs |
| 29 | `WEAR_FEET` | feet |
| 30 | `WEAR_HOOVES` | hooves |

The object's `wear_flags` must include the corresponding `ITEM_WEAR_*` bit for the chosen `wear_loc`, or the equip will silently fail at runtime. `WEAR_CLAN_COLORS` (31) and `WEAR_INVASION_EMBLEM` (32) are runtime-only and must not be used in `E` resets.

Builder editor mapping:

- `addreset put <obj-vnum> <container-vnum>` emits reset command `P` with `arg1=<obj-vnum>`, `arg2=0`, and `arg3=<container-vnum>`.

Shop inventory stocking pattern:

- Shop inventory is stocked by issuing `G` resets (give to previous mob) immediately after the `M` reset that spawns the shopkeeper mob.
- Each `G` reset gives one object type to that shopkeeper; the shopkeeper then sells those items.
- Use `G 0 <obj_vnum> <limit>` where limit is typically 5 to allow multiple simultaneous copies.
- Example sequence: `M 0 <keeper_vnum> 1 <room_vnum>` followed by one or more `G 0 <item_vnum> 5` lines.

Door reset both-sides requirement:

- When a door is reset to `closed` (state 1) or `locked` (state 2) via `D`, both sides of the door must be reset with matching `D` commands.
- For a door between rooms A and B: issue `D 0 <roomA> <dir_to_B> <state>` and `D 0 <roomB> <dir_to_A> <state>`.
- Both exits must also have the `door` (`EX_ISDOOR`, bit `1`) flag set in their room `D<door>` entries.

Reset vnum validity rule:

- Resets must reference valid vnums for the target type required by the command (room/mobile/object as applicable).

Cross-area object dependency warning:

- When rebuilding an area and changing its object vnum layout, other area files may contain `G`, `E`, or `P` resets that reference the old object vnums. Such references become invalid and cause `test_db` to fail with "reset references undefined object vnum". Before finalising a vnum reassignment, search all `.are` files for `G`, `E`, and `P` lines that reference any vnum in the rebuilt area's envelope, and update or remove those references. The `area/area_index.md` and `docs/world_links.md` documents can help identify which areas are likely to have cross-area dependencies.

## 13) Structural constraints enforced by tests

From `src/test_area_format.c`, `src/test_wood_area.c`, and `src/test_db.c`:

### area.lst ordering constraint

The `area/area.lst` file must list areas in non-decreasing order by `V` minimum vnum. Ties (same minimum) are further resolved by non-decreasing `V` maximum vnum. `test_area_format.c` enforces this at every test run and will fail with a diagnostic if the order is violated.

Practical rule: when inserting a new area or expanding an existing area's `V` range, verify that its position in `area.lst` remains consistent with its new minimum vnum. See `docs/area_index.md` for the current ordered list.

Note: `V` envelopes may overlap between areas (e.g., two areas can both declare ranges that share an integer). The ordering check is on minimum vnums only, not on range exclusivity. What must remain globally unique are the individual room, mobile, and object vnums actually defined inside each area's `#ROOMS`, `#MOBILES`, and `#OBJECTS` sections.

- `#ROOMS` must exist.
- `#ROOMS` must end with `#0` before the next section header.
- Inside `#ROOMS`, each room body may contain only `D`, `E`, or `S` entries.
- `D` entries must include two `~` strings and a destination line with exactly three integers.
- If `#MOBILES` exists, it must terminate with `#0` before `#OBJECTS`.
- If `#OBJECTS` exists, it must terminate with `#0` before `#RESETS`.
- `#OBJECTS` must not contain blank lines between object records.
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
- `area/area.lst` must list area files in strictly ascending order by their `V <min>` vnum. Inserting an area out of vnum order causes the `unit-test-area-format` check to fail.

## 13.1) Vnum allocation policy

For all content types (`#MOBILES`, `#ROOMS`, and `#OBJECTS`), vnums must be assigned in ascending sequential order.

- Use lower available vnums before higher ones (i.e., use `1` before `2`, `2` before `3`, and so on).
- Do not leave gaps in vnum sequences.
- When gaps exist in an area's assigned vnum range, fill those gaps wherever possible before assigning new higher vnums.
- For `#ROOMS`, attempt to use all room vnums in the area's assigned range (i.e., fill every available room-vnum slot where practical).

## 13.2) Area quest design constraints

Area quest design (when authoring quest sets for an area) must satisfy all of the following:

- Every area must include at least one quest whose target is a boss.
- New areas must include at least one cartography quest, unless explicitly specified otherwise.
- The final quest in any quest chain must reward a piece of equipment.
- Any quest that targets a boss must reward a piece of equipment.

## 14) Canonical section order emitted by saver

`areasave.c` writes sections in this order when present:

1. `#AREA`
2. `#ROOMS`
3. `#MOBILES`
4. `#OBJECTS`
5. `#SHOPS`
6. `#RESETS`
7. `#SPECIALS`
8. `#OBJFUNS`
9. `#$`

Using this order is recommended for consistency, even though the loader dispatch is name-based.
