# Mobile Specification

Extracted from `docs/area_file_spec.md`.

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
[optional inline script blocks]
...
#0
```

`<description>` must include exactly one trailing newline before the terminating `~`.
`<long_descr>` is stricter: it must be exactly one text line, followed by a newline, followed by a line containing only `~`.
There is no valid multi-line alternative for mob `<long_descr>`; under no circumstances may `<long_descr>` text span multiple lines.

Optional extension blocks (detected by leading marker):

- `! <class> <clan> <race> <position> <skills> <cast> <def>`
- `| <strong_magic> <weak_magic> <race_mods> <power_skills> <power_cast> <resist> <suscept>`
- `+ <spellpower_mod> <crit_mod> <crit_mult_mod> <spell_crit_mod> <spell_mult_mod> <parry_mod> <dodge_mod> <block_mod> <pierce_mod>`
- `l <loot_amount> <loot[0..MAX_LOOT-1]>`
- `L <loot_chance[0..MAX_LOOT-1]>`

Loot table semantics:
- `loot_amount` must be greater than 0 for loot drops to be attempted.
- `loot_amount` is percent-based with overflow in 100-point chunks: `50` = 50% chance to drop one item, `100` = exactly one item, `150` = one guaranteed + 50% chance for a second, `300` = exactly three items.
- `loot[x]` contains the object vnum for each loot slot.
- `loot_chance[x]` contains each slot's weight in integer percent points for selection.
- The sum of all `loot_chance[x]` values on an `L` line must be `<= 100`.
- Items referenced by loot tables should have `ITEM_LOOT` set in `extra_flags`.

### 4.1) Mobile `skills` bitvector (`!` extension)

`skills` in the `!` extension line is a bitvector. Known flags (from builder lookup tables) are:

### 4.1.a) Mobile skill level policy (floors and ceilings)

For generated mobs (AI-generated skill loadouts), combat-skill assignment uses two level gates per skill:

- **chance floor**: minimum level where the skill can start rolling randomly.
- **guaranteed floor**: level where the skill is always set.

The floor policy now depends on AI archetype:

- **Melee profile** (`generate_phys` only): earlier physical-skill access and stronger guaranteed combat baseline.
- **Hybrid profile** (`generate_phys` + casting): mid/late physical-skill access, preserving mixed-role identity.
- **Caster profile** (physical policy defined for completeness): very late physical-skill access, prioritizing spell identity.

Current policy (mobs can scale to level 170):

#### Melee profile

| Skill | Chance floor | Guaranteed floor |
|---|---:|---:|
| `2_attack` | 20 | 70 |
| `3_attack` | 40 | 85 |
| `4_attack` | 80 | 105 |
| `5_attack` | 110 | 130 |
| `6_attack` | 150 | 155 |
| `nodisarm` | 35 | 85 |
| `notrip` | 35 | 85 |
| `punch` | 15 | 60 |
| `headbutt` | 20 | 70 |
| `knee` | 35 | 90 |
| `disarm` | 45 | 100 |
| `trip` | 45 | 100 |
| `dodge` | 30 | 80 |
| `parry` | 55 | 110 |
| `martial` | 65 | 120 |
| `enhanced` | 40 | 95 |
| `dualwield` | 75 | 125 |
| `dirt` | 50 | 105 |
| `counter` | 60 | 115 |
| `kick` | 25 | 75 |
| `charge` | 80 | 135 |

#### Hybrid profile

| Skill | Chance floor | Guaranteed floor |
|---|---:|---:|
| `2_attack` | 35 | 85 |
| `3_attack` | 45 | 95 |
| `4_attack` | 90 | 120 |
| `5_attack` | 120 | 145 |
| `6_attack` | 160 | 170 |
| `nodisarm` | 45 | 95 |
| `notrip` | 45 | 95 |
| `punch` | 25 | 70 |
| `headbutt` | 30 | 80 |
| `knee` | 45 | 100 |
| `disarm` | 55 | 110 |
| `trip` | 55 | 110 |
| `dodge` | 40 | 90 |
| `parry` | 70 | 125 |
| `martial` | 80 | 135 |
| `enhanced` | 55 | 105 |
| `dualwield` | 95 | 145 |
| `dirt` | 65 | 115 |
| `counter` | 80 | 130 |
| `kick` | 35 | 85 |
| `charge` | 100 | 150 |

#### Caster profile

| Skill | Chance floor | Guaranteed floor |
|---|---:|---:|
| `2_attack` | 45 | 95 |
| `3_attack` | 55 | 105 |
| `4_attack` | 100 | 130 |
| `5_attack` | 130 | 155 |
| `6_attack` | 165 | 170 |
| `nodisarm` | 55 | 105 |
| `notrip` | 55 | 105 |
| `punch` | 35 | 80 |
| `headbutt` | 40 | 90 |
| `knee` | 55 | 110 |
| `disarm` | 65 | 120 |
| `trip` | 65 | 120 |
| `dodge` | 50 | 100 |
| `parry` | 80 | 135 |
| `martial` | 90 | 145 |
| `enhanced` | 65 | 115 |
| `dualwield` | 105 | 155 |
| `dirt` | 75 | 125 |
| `counter` | 90 | 140 |
| `kick` | 45 | 95 |
| `charge` | 110 | 160 |

This prevents low-level mobs from rolling late-game skills and guarantees baseline skill sets for high-level mobs.

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
- Within a sub-region of an area, mobs must either be flagged `sentinel` or the sub-region must be enclosed by rooms flagged `no_mob`, so mobs cannot wander out of their sub-region.
- Boss mobs must be flagged `sentinel` and `boss` and must be placed only in rooms flagged `no_mob`.
- `invasion` must never be set by builders in area files; it is set/cleared by runtime invasion systems only.
- Strong (non-boss) mobs must be flagged `solo`.

### 5.4a) Mobile `affected_by` bitvector

`affected_by` on the main mobile data line (`<act> <affected_by> <alignment> S`) is a bitvector. Flags are defined in `src/config.h` as `AFF_*`:

- `blind` = `1` (`AFF_BLIND`)
- `invisible` = `2` (`AFF_INVISIBLE`)
- `detect_evil` = `4` (`AFF_DETECT_EVIL`)
- `detect_invis` = `8` (`AFF_DETECT_INVIS`)
- `detect_magic` = `16` (`AFF_DETECT_MAGIC`)
- `detect_hidden` = `32` (`AFF_DETECT_HIDDEN`)
- `cloak_reflection` = `64` (`AFF_CLOAK_REFLECTION`)
- `sanctuary` = `128` (`AFF_SANCTUARY`)
- `faerie_fire` = `256` (`AFF_FAERIE_FIRE`)
- `infrared` = `512` (`AFF_INFRARED`)
- `curse` = `1024` (`AFF_CURSE`)
- `cloak_flaming` = `2048` (`AFF_CLOAK_FLAMING`)
- `poison` = `4096` (`AFF_POISON`)
- `protect` = `8192` (`AFF_PROTECT`)
- `cloak_absorption` = `16384` (`AFF_CLOAK_ABSORPTION`)
- `sneak` = `32768` (`AFF_SNEAK`)
- `hide` = `65536` (`AFF_HIDE`)
- `sleep` = `131072` (`AFF_SLEEP`)
- `charm` = `262144` (`AFF_CHARM`)
- `flying` = `524288` (`AFF_FLYING`)
- `pass_door` = `1048576` (`AFF_PASS_DOOR`)
- `anti_magic` = `2097152` (`AFF_ANTI_MAGIC`)
- `blasted` = `4194304` (`AFF_BLASTED`)
- `remort_curse` = `8388608` (`AFF_REMORT_CURSE`)
- `confused` = `16777216` (`AFF_CONFUSED`)
- `vamp_healing` = `33554432` (`AFF_VAMP_HEALING`) — **do not set in area files**
- `hold` = `67108864` (`AFF_HOLD`)
- `paralysis` = `134217728` (`AFF_PARALYSIS`)
- `cloak_adept` = `268435456` (`AFF_CLOAK_ADEPT`)

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

### 5.5a) Mobile special functions

A mobile may be assigned a special function (`spec_fun`) in the `#SPECIALS` section of the area file. Special functions run periodically to give the mob autonomous behavior — ambient atmosphere, law enforcement, breath weapons, NPC scripts, and more.

See **`docs/mob_specials_spec.md`** for the complete list of available special functions with descriptions of their behavior, builder policies (including the rule that `spec_summon_*` must never be set in area files), and sample output.

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

Optional inline scripts inside a mobile entry:

```text
> <prog_type_name> <arglist>~
<comlist>~
...
|
```
