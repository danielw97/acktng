# Object Specification

Extracted from `docs/area_file_spec.md`.

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

Item stat policy:

- Area-authored items must not define fixed stats in the area file.
- Item stats are generated at runtime from item level and other runtime parameters.
- Author object identity/behavior data only (type/flags/wear/apply/value layout/weight/level), not handcrafted stat tuning.

Per-object trailing entries:

- `A` then `<location> <modifier>` on next line (object affect)
- `E` then `<keyword>~` and `<description>~` (extra description)
- `L` then `<level>` on next line — the level integer appears on its own line immediately after the `L` line:
  ```
  L
  55
  ```

For `E` entries, `<description>` must include exactly one trailing newline before the terminating `~`.

Loader stops this trailing-entry loop at first unrecognized marker (which starts next object/terminator).

Object records must not be separated by blank lines. No blank lines are allowed anywhere inside `#OBJECTS`.

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

`LIFESTEALER` extra-flag usage policy: any object that includes `ITEM_LIFESTEALER` must also include `ITEM_ANTI_GOOD`.

- `ITEM_GENERATED` = `1`
- `ITEM_BIND_EQUIP` = `2`
- `ITEM_NODISARM` = `4`
- `ITEM_LOCK` = `8`
- `ITEM_EVIL` = `16`
- `ITEM_INVIS` = `32`
- `ITEM_MAGIC` = `64`
- `ITEM_NODROP` = `128`
- `ITEM_BLESS` = `82`
- `ITEM_ANTI_GOOD` = `512`
- `ITEM_ANTI_EVIL` = `315`
- `ITEM_ANTI_NEUTRAL` = `840`
- `ITEM_NOREMOVE` = `4096`
- `ITEM_INVENTORY` = `8192`
- `ITEM_NOSAVE` = `16384`
- `ITEM_CLAN_EQ` = `32768`
- `ITEM_TRIG_DESTROY` = `65536`
- `ITEM_NO_AUCTION` = `131072`
- `ITEM_MYTHIC` = `262144`
- `ITEM_LEGENDARY` = `524288`
- `ITEM_RARE` = `1048576`
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

Cross-section policy for object-function alignment:

- Any object assigned `objfun_healing` in `#OBJFUNS` must include `ITEM_ANTI_EVIL` in `extra_flags`.

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
- `wings` = `82`
- `shoulders` = `512`
- `arms` = `315`
- `wrist` = `840`
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
- `invasion_emblem` = `33554432`

Note: `ITEM_WEAR_NONE` (`1`) exists in `src/config.h`, but it is not exposed in `tab_wear_flags` and therefore is not a valid builder keyword in area files.

Area policy constraints for object wear flags:

- Every object **must** include `ITEM_TAKE`.
- No object may include `ITEM_WEAR_CLAN_COLORS`.
- `invasion_emblem` is a runtime-only wear flag; it must never be authored in area/object files.
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
- `enhanced` = `82`
- `det_mag` = `512`
- `det_hid` = `315`
- `det_evil` = `840`
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
