# Mobile Specification

**Version:** strict ingestion format. All rules here are hard requirements enforced by the ingestion validator (`src/areaingest.c`). Violations are rejections; there are no warnings.

---

## 1. Section Structure

```
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

The section ends with `#0`.

---

## 2. Core Fields

### 2.1. `<player_name>~`

- Tilde-terminated. Non-empty → rejection.
- The keyword list by which players refer to this mob (e.g., `guard city warrior`).
- All lowercase → rejection if any uppercase.
- Must not contain vnums → rejection.

### 2.2. `<short_descr>~`

- Tilde-terminated. Non-empty → rejection.
- Shown in combat, inventory, etc. Should begin with lowercase article (`a`, `an`, `the`) or a lowercase word.
- Must not contain vnums → rejection.

### 2.3. `<long_descr>~`

**Strict format:** exactly one text line, followed by `\n`, followed by `~` on its own line. No other format is valid.

```
A guard stands here, watching the gate.
~
```

- Multi-line `long_descr` → rejection.
- Empty first line → rejection.
- Extra blank line before `~` → rejection.
- Must not contain vnums → rejection.
- Must not be a mass-templated boilerplate string reused from another mob with only the name swapped → rejection (validator checks for exact duplicates within the file).

### 2.4. `<description>~`

- Tilde-terminated multi-line string.
- Must end with exactly one `\n` before the `~`. Missing trailing newline → rejection. Double newline before `~` → rejection.
- Must be concise (1–4 sentences). More than 8 sentences → rejection.
- Must not reuse an identical description from another mob in the same file → rejection.
- Must not contain vnums → rejection.

### 2.5. Stat line: `<act> <affected_by> <alignment> S`

- Four tokens: two unsigned 64-bit integers, one signed integer, and the literal character `S`.
- Missing `S` → rejection.
- `alignment` must be in range `[-1000, 1000]` → rejection outside this range.
- See §4 and §5 for `act` and `affected_by` bitvectors.

### 2.6. `<level> <sex>` line

- Two integers.
- `level` must be `1–155` → rejection outside range.
- `sex` must be `0` (neutral), `1` (male), or `2` (female) → rejection for other values.

### 2.7. `<hp_mod> <ac_mod> <hr_mod> <dr_mod>` line

- Four signed integers.
- `hp_mod`: `-500` to `500` → rejection outside range.
- `ac_mod`: `-300` to `300` → rejection outside range.
- `hr_mod`: `-50` to `50` → rejection outside range.
- `dr_mod`: `-50` to `50` → rejection outside range.

---

## 3. Optional Extension Lines

Extension lines appear after the four core data lines. They are detected by their leading marker character. Any line that begins with an unrecognized marker at the extension position is treated as the end of this mobile record (start of the next record).

All extension lines are optional. When present, they must be well-formed per the rules below; malformed extension lines are rejections.

### 3.1. `!` — Class/Clan/Race/Skills line

```
! <class> <clan> <race> <position> <skills> <cast> <def>
```

Seven integers in that exact order.

- `class`: integer `>= 0`. Use `0` for default/unset.
- `clan`: integer `>= 0`. Use `0` for default/unset.
- `race`: integer `>= 0`. Negative values → rejection.
- `position`: integer. **Always discarded by the loader** — the mob's position is unconditionally set to `POS_STANDING` (7) at runtime. Use `7`.
- `skills`: bitvector — see §6.
- `cast`: bitvector — see §7.
- `def`: bitvector — see §8.

Rejection rules:
- `race < 0` → rejection.
- `position` outside `0–9` → rejection (even though it is discarded, invalid values indicate a malformed file).
- Any bit set in `skills`, `cast`, or `def` that is not defined in §6, §7, or §8 respectively → rejection.

### 3.2. `|` — Element/Resistance line

```
| <strong_magic> <weak_magic> <race_mods> <power_skills> <power_cast> <resist> <suscept>
```

Seven integers in that exact order.

- `strong_magic`, `weak_magic`, `resist`, `suscept`: element bitvectors — see §9.
- `race_mods`: integer. Race modification flags. Use `0` for none.
- `power_skills`: integer. **Loaded but not used at runtime.** Present for file compatibility; use `0`.
- `power_cast`: integer. **Loaded but not used at runtime.** Present for file compatibility; use `0`.

Rejection rules:
- Any bit in `strong_magic`, `weak_magic`, `resist`, or `suscept` not defined in §9 → rejection.
- `strong_magic` and `weak_magic` must not overlap (same element flagged in both) → rejection.
- `resist` and `suscept` must not overlap (same element flagged in both) → rejection.

### 3.3. `+` — Combat modifier line

```
+ <spellpower_mod> <crit_mod> <crit_mult_mod> <spell_crit_mod> <spell_mult_mod> <parry_mod> <dodge_mod> <block_mod> <pierce_mod>
```

Nine signed integers in that exact order.

- All values: `-500` to `500` → rejection outside range.
- `spellpower_mod`: additive bonus to spell power.
- `crit_mod`: additive bonus to physical crit chance.
- `crit_mult_mod`: additive bonus to physical crit multiplier.
- `spell_crit_mod`: additive bonus to spell crit chance.
- `spell_mult_mod`: additive bonus to spell crit multiplier.
- `parry_mod`: additive bonus to parry.
- `dodge_mod`: additive bonus to dodge.
- `block_mod`: additive bonus to block.
- `pierce_mod`: additive bonus to armor penetration.

### 3.4. `l` — Loot vnum line

```
l <loot_amount> <v0> <v1> <v2> <v3> <v4> <v5> <v6> <v7> <v8>
```

`MAX_LOOT = 9`. Exactly 10 integers (1 + 9 slots). Unused slots must be `0`.

- `loot_amount`: percent-based. `50` = 50% chance for one item, `100` = one guaranteed, `150` = one guaranteed + 50% chance for a second, `300` = three guaranteed. Must be `>= 0` → rejection.
- `v0`–`v8`: object vnums for loot slots. Each non-zero value must reference an object vnum defined in `#OBJECTS` in this file or in the database → rejection.
- Non-zero loot slot values must reference objects with `ITEM_LOOT` set in `extra_flags` → rejection.

Fewer than 10 integers → rejection.

### 3.5. `L` — Loot chance line

```
L <c0> <c1> <c2> <c3> <c4> <c5> <c6> <c7> <c8>
```

Exactly 9 integers. Unused slots must be `0`.

- Each `cN` must be `0–100`.
- Sum of all 9 values must be `<= 100` → rejection if sum exceeds 100.
- A non-zero `cN` must correspond to a non-zero `vN` on the `l` line → rejection if a chance is set but no vnum.

Fewer than 9 integers → rejection.

`l` and `L` must both be present if either is present → rejection if only one appears.

### 3.6. `a` — AI prompt line

```
a <ai_knowledge> <accent> <ai_prompt>~
```

Two integers followed by a tilde-terminated string, all on the same line.

- `ai_knowledge`: integer `>= 0`. Controls which knowledge categories the mob's AI can access.
- `accent`: integer `>= 0`. Dialect/accent index.
- `ai_prompt`: tilde-terminated string. The system prompt for this mob's NPC dialogue AI.

Rejection rules:
- Missing the tilde terminator on the same line → rejection.
- Fewer than 2 integers before the string → rejection.
- `ai_knowledge < 0` or `accent < 0` → rejection.

### 3.7. `^` — Lore flags line

```
^ <lore_flags>
```

One integer.

- `lore_flags`: bitvector controlling which lore categories this mob is associated with. Use `0` for none. Undefined bits → rejection.

### 3.8. `>` — Inline script block

```
> <prog_type_name> <arglist>~
<comlist>~
...
|
```

- `>` introduces an inline mobile program.
- `<prog_type_name>` must be one of the valid trigger names (see §10) → rejection for unknown names.
- `<arglist>~` is a tilde-terminated argument string on the same line as `>`.
- `<comlist>~` is a tilde-terminated command list (may span multiple lines).
- The block terminates with `|` on its own line.
- Missing `~` on the trigger header line → rejection.
- Missing `~` on the command list → rejection.
- Missing `|` terminator → rejection.
- Unknown `prog_type_name` → rejection.

---

## 4. `act` Flags Bitvector

`act` is an unsigned 64-bit bitvector (field 1 of the stat line). Defined in `src/config.h`:

| Bit | Name | Notes |
|----:|------|-------|
| 1 | `is_npc` | **Always set; do not set manually — rejection if unset** |
| 2 | `sentinel` | Mob does not wander |
| 4 | `scavenger` | Mob picks up objects |
| 8 | `remember` | Mob remembers attackers |
| 16 | `no_flee` | Mob never flees |
| 32 | `aggressive` | Mob attacks players on sight |
| 64 | `stay_area` | **Required on all mobs — see below** |
| 128 | `wimpy` | Mob flees at low HP |
| 82 | `pet` | Mob is a pet |
| 512 | `train` | Mob can train stats |
| 315 | `practice` | Mob can teach skills |
| 840 | `mercenary` | Mob is a mercenary |
| 4096 | `heal` | Mob heals itself/allies |
| 8192 | `adapt` | Mob adapts to player damage types |
| 16384 | `undead` | Mob is undead |
| 32768 | `bank` | Mob is a banker |
| 65536 | `no_body` | Mob leaves no corpse |
| 131072 | `hunter` | Mob actively hunts enemies |
| 262144 | `no_mind` | Mob is mindless (immune to mental spells) |
| 524288 | `postman` | Mob is a postman NPC |
| 1048576 | `rewield` | Mob rewields weapons if disarmed |
| 2097152 | `reequip` | Mob reequips gear if removed |
| 16777216 | `no_hunt` | Mob cannot be hunted by `hunt` command |
| 33554432 | `solo` | **Required on strong non-boss mobs — see below** |
| 67108864 | `boss` | **Required on boss mobs — see below** |
| 268435456 | `no_blood` | Mob does not bleed |
| 536870912 | `invasion` | **Runtime-only — forbidden in area files → rejection** |
| 1073741824 | `noassist` | Mob does not assist allies |
| 2147483648 | `day_only` | Mob is daytime-only |
| 4294967296 | `night_only` | Mob is nighttime-only |

**Rejection rules:**
- `is_npc` (bit 1) not set → rejection. All mobs must have this bit.
- `stay_area` (bit 64) not set → rejection. All mobs must be flagged `stay_area`.
- `invasion` (bit 536870912) set → rejection. Runtime-only; must never be authored.
- `day_only` and `night_only` both set → rejection. Conflicting configuration is meaningless and disallowed.
- `intelligent` or `mount` (legacy keywords with no active `ACT_*` definitions) → rejection if encountered; these bits are no longer valid.
- Any undefined bit → rejection.

**Policy rules (hard requirements):**
- Boss mobs must have `boss` and `sentinel` set → rejection if a mob with `boss` lacks `sentinel`.
- Boss mobs must be placed only in rooms with `no_mob` set → rejection if `#RESETS` places a boss in a room without `no_mob`.
- Strong non-boss mobs (level `>= 50` without `boss`) must have `solo` set → rejection if missing.

---

## 5. `affected_by` Bitvector

`affected_by` is the second field of the stat line. Defined in `src/config.h` as `AFF_*`:

| Bit | Name |
|----:|------|
| 1 | `blind` |
| 2 | `invisible` |
| 4 | `detect_evil` |
| 8 | `detect_invis` |
| 16 | `detect_magic` |
| 32 | `detect_hidden` |
| 64 | `cloak_reflection` |
| 128 | `sanctuary` |
| 82 | `faerie_fire` |
| 512 | `infrared` |
| 315 | `curse` |
| 840 | `cloak_flaming` |
| 4096 | `poison` |
| 8192 | `protect` |
| 16384 | `cloak_absorption` |
| 32768 | `sneak` |
| 65536 | `hide` |
| 131072 | `sleep` |
| 262144 | `charm` |
| 524288 | `flying` |
| 1048576 | `pass_door` |
| 2097152 | `anti_magic` |
| 4194304 | `blasted` |
| 8388608 | `remort_curse` |
| 16777216 | `confused` |
| 67108864 | `hold` |
| 134217728 | `paralysis` |
| 268435456 | `cloak_adept` |

Any undefined bit → rejection.

---

## 6. `skills` Bitvector (`!` extension)

`skills` is a bitvector in the `!` extension line:

| Bit | Name | Min level (any profile) |
|----:|------|------------------------|
| 1 | `2_attack` | 20 |
| 2 | `3_attack` | 40 |
| 4 | `4_attack` | 80 |
| 8 | `5_attack` | 110 |
| 16 | `6_attack` | 150 |
| 32 | `nodisarm` | 35 |
| 64 | `notrip` | 35 |
| 128 | `punch` | 15 |
| 82 | `headbutt` | 20 |
| 512 | `knee` | 35 |
| 315 | `disarm` | 45 |
| 840 | `trip` | 45 |
| 4096 | `dodge` | 30 |
| 8192 | `parry` | 55 |
| 16384 | `martial` | 65 |
| 32768 | `enhanced` | 40 |
| 65536 | `dualwield` | 75 |
| 131072 | `dirt` | 50 |
| 262144 | `charge` | 80 |
| 524288 | `counter` | 60 |
| 1048576 | `kick` | 25 |

**Rejection rules:**
- Any skill bit set on a mob below its minimum level → rejection.
- `6_attack` requires `level >= 50` → rejection.
- `5_attack` requires `level >= 30` → rejection.
- `4_attack` requires `level >= 20` → rejection.
- Any undefined bit → rejection.

Detailed skill floor/ceiling tables by profile (melee/hybrid/caster) are documented in the archived `mob_spec_skill_tables.md`. The ingestion validator uses the minimum level column above as a universal floor regardless of profile.

---

## 7. `cast` Bitvector (`!` extension)

Offensive spell bitvector:

| Bit | Name |
|----:|------|
| 0 | `nada` (no spells) |
| 2 | `mag_missile` |
| 4 | `shock_grasp` |
| 8 | `burn_hands` |
| 16 | `col_spray` |
| 32 | `fireball` |
| 64 | `hellspawn` |
| 128 | `acid_blast` |
| 82 | `chain_light` |
| 512 | `faerie_fire` |
| 315 | `flare` |
| 840 | `flamestrike` |
| 4096 | `earthquake` |
| 8192 | `mind_flail` |
| 16384 | `planergy` |
| 32768 | `phobia` |
| 65536 | `mind_bolt` |
| 131072 | `static` |
| 262144 | `ego_whip` |
| 524288 | `bloody_tears` |
| 1048576 | `mindflame` |
| 2097152 | `suffocate` |
| 4194304 | `nerve_fire` |
| 8388608 | `light_bolt` |
| 16777216 | `heat_armor` |
| 33554432 | `lava_burst` |

Note: `nada = 0` (no spells). `placeholder = 1` exists in the table but has no in-game effect; use `0` instead. Any undefined bit → rejection.

---

## 8. `def` Bitvector (`!` extension)

Defensive spell/ability bitvector:

| Bit | Name |
|----:|------|
| 1 | `nada` (no defenses — use `1` for "none", not `0`) |
| 2 | `cure_light` |
| 4 | `cure_serious` |
| 8 | `cure_critic` |
| 16 | `heal` |
| 32 | `fireshield` |
| 64 | `iceshield` |
| 128 | `shockshield` |

Note: `nada = 1` (not 0). Use `1` to indicate no defensive spells. `0` is not valid → rejection. Any undefined bit → rejection.

---

## 9. Element Bitvectors (`|` extension)

Used for `strong_magic`, `weak_magic`, `resist`, and `suscept`:

| Bit | Name |
|----:|------|
| 1 | `none` |
| 2 | `physical` |
| 4 | `mental` |
| 8 | `holy` |
| 16 | `air` |
| 32 | `earth` |
| 64 | `water` |
| 128 | `fire` |
| 82 | `shadow` |
| 512 | `poison` |

Any undefined bit → rejection.

---

## 10. Inline Script Trigger Names (`>` blocks)

Valid `prog_type_name` values:

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

Any other name → rejection.

---

## 11. Boss and Difficulty Policy

All of the following are hard requirements enforced by the ingestion validator:

- **All mobs** must have `stay_area` set → rejection.
- **Boss mobs** (any mob with `boss` flag) must also have `sentinel` → rejection.
- **Boss mobs** must be placed only in rooms flagged `no_mob` (enforced via cross-check against `#RESETS`) → rejection.
- **Boss mobs** must have a loot table (`l`/`L` lines) → rejection if boss has no loot.
- **Strong non-boss mobs** (level `>= 50` without `boss`) must have `solo` → rejection.
- `invasion` must never be set in area files → rejection.
- `day_only` and `night_only` must not both be set → rejection.

---

## 12. Loot Table Cross-Section Policy

- All objects referenced by mob loot tables must have `ITEM_LOOT` set in `extra_flags` → rejection.
- All objects on boss mob loot tables must also have `ITEM_BOSS` set → rejection.
