# Area Plan: The Eastern Desert (Full Redesign)

## Overview

**Area Name:** `@@yThe @@bEastern @@yDesert@@N`  
**File Name:** `eastern_desert.are`  
**Keyword:** `easterndesert`  
**Level Range:** 30-100 (`I 30 100`)  
**Vnum Range (fixed):** **8500-8599** (`V 8500 8599`)  
**Room Count Target:** 100/100 vnums used  
**Design Goal:** Replace the current corridor-heavy road desert with a coherent expedition sandbox: caravan artery, ruined observatory-cistern complex, cult encampments, storm dunes, and a buried royal necropolis.

This plan keeps the existing vnum envelope exactly as assigned and fully reworks room flow, encounter identity, objectization, and narrative progression.

---


## Lore
See `eastern_desert_lore.md` for extracted lore.

## `#AREA` Header Plan (spec-compliant)

All directives per `area_file_spec.md` section 3:

- `Q 16` — mandatory area version; no other value is valid
- `K easterndesert~` — single-word keyword, no spaces, no color codes, `~`-terminated
- `L @@W(@@y30 100@@W)@@N~` — white parens, yellow level numbers, white close paren, reset, `~`-terminated
- `N <allocate at implementation time>` — area number; must be confirmed unused across all loaded area files
- `I 30 100` — min/max level; controls mob-level gating and area matching
- `V 8500 8599` — full 100-vnum envelope; all rooms, mobs, and objects must stay within this range
- `F 15` — reset frequency in minutes
- `U @@yhot wind scours the Eastern Desert as sand shifts across the ancient road@@N~` — reset message; no double-newlines, `~`-terminated
- `O Virant~` — area owner must always be Virant (owner policy)
- `T` — teleport flag line (rest ignored by loader); presence enables teleport into the area
- `R`, `W` — ACL directives omitted; set at implementation time by the builder

### Global String and Description Requirements

Per `area_file_spec.md` section 2:

- All text fields must be `~`-terminated. Missing `~` causes parse failure.
- Strings must not contain back-to-back newlines (no `\n\n` blank lines).
- Vnums must never appear in any in-world description text (room descriptions, mob descriptions, object descriptions, extra descriptions, exit descriptions).
- Mobile `long_descr` must be exactly one text line followed by a newline and a `~`-only line. Multi-line `long_descr` is never valid.
- Mobile `description` must end with exactly one trailing newline before the terminating `~`.
- Room `<description>~` must end with exactly one trailing newline before `~`.
- Object extra-description `<description>~` must end with exactly one trailing newline before `~`.

---

## Narrative Structure (Act-Based)

### Act I - The Open Road and the False Safety (8500-8519)
Players begin on a visible caravan artery that appears controlled, but signs of recent violence and ration raids escalate quickly.

### Act II - Cisterns, Shrines, and Broken Administration (8520-8539)
Route branches into old civic infrastructure: dry basins, collapsed counting halls, and synod toll altars.

### Act III - Storm Belt and Mirage War-Zone (8540-8569)
Severe weather rooms, shifting pathing loops, ambush ridges, and elite cult warbands.

### Act IV - Buried Court of the Last Sand Regent (8570-8599)
Subterranean necropolis and eclipse observatory culminating in a boss ritual chamber that reframes the whole zone's history.

---

## Room Layout Redesign (All 100 Rooms)

> All room names/descriptions are fully replaced. Layout changes from mostly linear travel into a hub-and-spoke overworld with dungeon delves and branching paths.

### Room Description Requirements (spec compliance)

Per `area_file_spec.md` section 8:

- Every room `<description>~` must contain at least 3 sentences of text.
- Important rooms (zone entry points, boss rooms, major hub rooms, lore-critical rooms) must contain at least 5 sentences.
- Every room description must be unique to that room (no reused descriptions across rooms).
- Vnums must never appear in any in-world description text.
- Named exits (`<exit_keyword>`) must be discoverable in the room's `<description>~`, an object that spawns in the room, or an `E` extra description in the room.
- Extra-description keyword chains must be anchored: at least one keyword in each chain must appear in the room's main `<description>~`.
- Room descriptions must use the area's color theme intentionally and consistently.
- Each room's `<description>~` must end with exactly one trailing newline before `~`.

### Room Flags and Sector Types

Per `area_file_spec.md` sections 8.1 and 8.2:

- **Outdoor desert rooms** (road, dunes, ridges, obelisk fields): `sector_type = desert` (`10`)
- **Interior rooms** (cistern vaults, tally halls, tomb chambers, observatory rooms): `sector_type = inside` (`11`), `room_flags` includes `indoors` (`8`)
- **Boss rooms** (8559, 8579, 8599): `room_flags` includes `no_mob` (`4`) per boss placement policy
- **Necropolis rooms** (Zone E, 8580-8599): `room_flags` includes `no_teleport` (`16384`) and `no_recall` (`8192`), except stairwell/transit rooms that allow recall for escape
- **Storm belt rooms** (Zone C, 8540-8559): `room_flags` includes `hot` (`32`) for desert heat hazard flavor
- **No rooms use `ROOM_MAZE`** (`524288`); this area does not contain mazes

### Vnum Allocation Policy

Per `area_file_spec.md` section 13.1:

- Room vnums must be assigned in ascending sequential order with no gaps.
- All 100 room vnums (8500-8599) must be used; fill every available room-vnum slot.

### Zone A: Caravan Spine (8500-8519, 20 rooms)
- Primary traversal axis and external area connectors.
- Sector: `desert` throughout.
- Includes road checkpoints, wreck fields, and charter stones.
- Key rooms:
  - **8500** New western entry gate from legacy route (important room, 5+ sentence description).
  - **8506** Charter Mile Marker (first branch hub; important room, 5+ sentence description).
  - **8512** Burned Convoy Basin (event hotspot).
  - **8519** Eastbound Split toward deep desert/pyramid approach (important room, 5+ sentence description).

### Zone B: Cistern Quarter (8520-8539, 20 rooms)
- Semi-ruined civic waterworks and shrine bureaucracy.
- Mix of `desert` (outdoor ruins) and `inside` (vaulted cistern interiors, tally halls) sectors.
- Indoor rooms use `room_flags` including `indoors` (`8`).
- Key rooms:
  - **8523** Moonwell Access Shaft (vertical connector via D4 up / D5 down exits).
  - **8528** The Ledger of Thirst (lore-rich archive room; `inside` sector; important room, 5+ sentence description with multiple `E` extra descriptions for inscriptions).
  - **8533** Synod Toll Court (elite patrol reset anchor).
  - **8539** Sealed Descent to buried observatory (important room, 5+ sentence description).

### Zone C: Sirocco Expanse (8540-8559, 20 rooms)
- High-variance weather belt with dune ridgelines, glass flats, and ambush washes.
- Sector: `desert` throughout.
- `room_flags` includes `hot` (`32`) for heat hazard flavor.
- Supports roaming predator/reaver ecology.
- Key rooms:
  - **8540** Stormfront Threshold (important room, 5+ sentence description).
  - **8547** Blueglass Dune Crest (visibility gimmick room; multiple `E` extra descriptions for what can be seen from the crest).
  - **8552** Carrion Banner Ridge (faction clash scene).
  - **8559** Eye of the Sirocco (mini-boss arena; `room_flags` includes `no_mob` per boss placement policy; important room, 5+ sentence description).

### Zone D: Obelisk Marches (8560-8579, 20 rooms)
- Dense ritual architecture zone: standing stones, ward pylons, anointing yards.
- Movement becomes deliberate with symbolic gate sequencing using named exits (`^` prefix keywords).
- Mix of `desert` (open obelisk fields) and `inside` (enclosed ritual chambers) sectors.
- Key rooms:
  - **8561** The Twelve Obelisks (important room, 5+ sentence description with `E` extra descriptions for individual obelisk inscriptions).
  - **8568** Jackal Anointing Yard.
  - **8572** Cracked Star Dial Plaza (important room, 5+ sentence description).
  - **8579** Regent's Processional Gate (mini-boss room; `room_flags` includes `no_mob`; important room, 5+ sentence description; named exit `^processional gate` discoverable in description).

### Zone E: Buried Court Necropolis (8580-8599, 20 rooms)
- Tomb-complex finale: funerary galleries, eclipse machinery, royal crypt halls.
- Sector: `inside` throughout; `room_flags` includes `indoors` (`8`), `no_teleport` (`16384`), `no_recall` (`8192`).
- Exception: stairwell/transit rooms (8580, 8599) allow recall (omit `no_recall`) so players can escape.
- Includes final boss route and optional side-loot chambers.
- Key rooms:
  - **8580** Descending Funerary Ramp (zone entry; exempt from `no_recall`; important room, 5+ sentence description).
  - **8586** Chamber of Salt Oaths (important room, 5+ sentence description with `E` extra descriptions for oath inscriptions).
  - **8592** Eclipse Lens Gallery (arcane puzzle framing; important room with chained `E` extra descriptions for lens examination).
  - **8598** Throne of Dry Stars (final pre-boss; important room, 5+ sentence description).
  - **8599** The Regent Beneath the Ninth Sun (final boss room; `room_flags` includes `no_mob`; exempt from `no_recall`; important room, 5+ sentence description).

### Connectivity Principles

Per `area_file_spec.md` section 8 directional traversal constraints:

- **All exits must be bi-directional**: if room A connects to room B through direction X, room B must provide the opposite-direction exit back to room A. No one-way exits (this area is not a maze).
- **No directional loops**: repeated movement in the same direction must not return to a previously visited room. The spec prohibits room connection loops (e.g., `a -> b -> c -> a`) unless all rooms in the set are flagged `ROOM_MAZE`.
- **Branching paths replace loops**: the plan uses hub-and-spoke branching from 3 major hub rooms (Charter Mile Marker 8506, Stormfront Threshold 8540, Descending Funerary Ramp 8580) rather than circular loops. Players choose branches and backtrack through hubs.
- **Multiple traversal options between macro-zones**: at least 2 branching paths connect each pair of adjacent zones, providing route variety without circular loops.
- **External exits preserved** where legacy worldgraph depends on them, but contextualized via renamed rooms/exits with bi-directional counterparts.
- **Door usage** limited to ritual gates and vault shutters. Every door exit must have `EX_ISDOOR` (`1`) set in `<locks>`. Door initial state (open/closed/locked) is authored through `#RESETS` command `D`, not through room exit `<locks>` bits. Any locked door (`D` state `2`) must have `<key_vnum>` set to a valid key object vnum, and that key object must exist in `#OBJECTS`.
- **Named exits** use `^` prefix on keywords (e.g., `^ritual gate~`, `^vault shutter~`) so movement messaging treats them as standalone noun phrases. Each named exit keyword must appear in the room's `<description>~`, an object in the room, or an `E` extra description.

---

## Mob Redesign Plan

### Mob Vnum Strategy

Per `area_file_spec.md` section 13.1:

- Mob vnums must be assigned in ascending sequential order with no gaps, starting from 8500.
- Keep and fully rewrite existing vnums **8500-8513**.
- Expand roster with additional desert population templates through contiguous sequential slots **8514-8531** (staying within the 8500-8599 vnum envelope).
- Target composition: ~32 mob templates (vnums 8500-8531).

### Mobile Spec Compliance (all mobs)

Per `area_file_spec.md` sections 4 and 5.4:

- **All mobs** must have `act` flags including `is_npc` (`1`) and `stay_area` (`64`).
- **Sentinel mobs** (guards, shopkeepers, stationary NPCs): add `sentinel` (`2`) to `act` flags.
- **Boss mobs** (The Bannerless Reaver, High Anointer, The Last Sand Regent): must have `act` flags including `sentinel` (`2`) and `boss` (`67108864`), and must be placed only in rooms flagged `no_mob` (`4`).
- **Strong non-boss mobs** (elite patrol leaders, veteran reavers, apex predators): must have `act` flags including `solo` (`33554432`).
- **Aggressive mobs** (hostile cult members, reavers, undead, feral beasts): set `aggressive` (`32`) in `act` flags.
- **Undead mobs** (salt mummies, oathbound sentinels, eclipse wardens, the Last Sand Regent): set `undead` (`16384`) in `act` flags.
- **`invasion`** (`536870912`) must never be set in area files; it is runtime-only.
- Mobile `long_descr` must be exactly one text line followed by a newline and a `~`-only line. Multi-line `long_descr` is never valid under any circumstances.
- Mobile `description` must end with exactly one trailing newline before the terminating `~`.
- Mobile `player_name` (keyword), `short_descr`, `long_descr`, and `description` are all `~`-terminated.

### Mobile Extension Lines

Per `area_file_spec.md` sections 4.1-5.5:

- `!` extension: `<class> <clan> <race> <position> <skills> <cast> <def>` — used for combat-capable mobs. Skill bitvectors follow the level-gated floor policy tables (melee/hybrid/caster profiles).
- `|` extension: `<strong_magic> <weak_magic> <race_mods> <power_skills> <power_cast> <resist> <suscept>` — used for elemental affinities. Desert mobs should generally have `resist` including `fire` (`128`) and `suscept` including `water` (`64`).
- `+` extension: combat modifier tuning for significant encounters.
- `l`/`L` extensions: loot tables. `loot_amount` is percent-based (see spec). Items in loot tables must have `ITEM_LOOT` set in `extra_flags`.

### Factions and Ecology

1. **Charter Caravaneers** (neutral/defensive NPC ecosystem)
   - Outriders, drovers, quartermasters, water clerks.
   - `act`: `is_npc` + `stay_area` + `sentinel` (for stationary merchants/clerks). Non-aggressive.
   - Level range: 30-50.
   - Archetype: hybrid profile (light arms, some defensive casting).

2. **Jackal Synod** (hostile cult faction)
   - Initiates, toll-priests, embalmer-guards, dusk chanters, astromancer viziers.
   - `act`: `is_npc` + `stay_area` + `aggressive`. Toll-priests add `sentinel` (stationed at ritual nodes).
   - Level range: 45-80 (initiates low, viziers high).
   - Archetype: hybrid profile (ritual casting + melee). Dusk chanters use caster profile.
   - `affected_by`: toll-priests get `detect_hidden` (`32`); astromancers get `detect_magic` (`16`).

3. **Dune Reaver Confederates** (hostile opportunists)
   - Skirmishers, beast-handlers, banner captains, the Oathbreaker Captain.
   - `act`: `is_npc` + `stay_area` + `aggressive`. Banner captains add `no_flee` (`16`).
   - Level range: 40-70 (skirmishers low, banner captains high).
   - Archetype: melee profile. Beast-handlers use hybrid profile.

4. **Ancient Court Remnants** (undead/construct guardians)
   - Oathbound sentinels, salt mummies, eclipse wardens.
   - `act`: `is_npc` + `stay_area` + `aggressive` + `undead` (`16384`) + `sentinel`.
   - Level range: 65-95.
   - Archetype: melee profile (sentinels), hybrid profile (eclipse wardens with defensive casting).
   - `affected_by`: `infrared` (`512`); eclipse wardens add `detect_invis` (`8`).
   - `resist`: `fire` (`128`), `physical` (`2`). `suscept`: `water` (`64`), `holy` (`8`).

5. **Apex Predators** (desert fauna)
   - Glass scorpions, ash adders, carrion kites, dune stalkers.
   - `act`: `is_npc` + `stay_area` + `aggressive` + `no_mind` (`262144`).
   - Level range: 35-65.
   - Archetype: melee profile. Ash adders add poison spec (`spec_poison` in `#SPECIALS`).

### Boss Ladder

All boss mobs: `act` includes `is_npc` + `stay_area` + `sentinel` + `boss` + `aggressive` + `no_flee`. Placed only in rooms flagged `no_mob`.

- **Mini-boss 1 (8559, Zone C):** The Bannerless Reaver. Level 75. Melee profile. `act` adds `remember` (`8`). Items dropped must include `ITEM_BOSS` flag.
- **Mini-boss 2 (8579, Zone D):** High Anointer of the Synod. Level 85. Hybrid profile with defensive casting. Assigned `spec_cast_cleric` in `#SPECIALS`. Items dropped must include `ITEM_BOSS` flag.
- **Final Boss (8599, Zone E):** The Last Sand Regent. Level 100. Hybrid profile with strong offensive casting. `act` adds `undead` + `remember`. `affected_by`: `sanctuary` (`128`), `detect_invis` (`8`), `infrared` (`512`). Assigned `spec_cast_undead` in `#SPECIALS`. Items dropped must include `ITEM_BOSS` flag. Loot table (`l`/`L`) used for boss drops.

### Rewrite of Current Named Mobs

- Azure Jackal Vizier -> **Synod Astromancer Vizier** (ritual tactician; hybrid profile, level 75).
- Gilded Sand Reaver -> **Oathbreaker Captain of the Dune Host** (melee bruiser; melee profile, level 70; `solo` flag).
- Sapphire Sun Oracle -> **Blind Oracle of the Ninth Meridian** (non-hostile lore NPC; `sentinel`, non-aggressive; level 60).
- Tyrant of Endless Dunes -> replaced by true area-canon final boss: **The Last Sand Regent** (level 100, boss flags as above).

---

## Object Redesign Plan

### Object Vnum Strategy

Per `area_file_spec.md` section 13.1:

- Object vnums must be assigned in ascending sequential order with no gaps, starting from 8500.
- Fully replace current object concepts beginning at **8500**.
- Expand through contiguous sequential slots (suggested through **8554**) while staying within the 8500-8599 vnum envelope.
- Target composition: ~55 object templates (vnums 8500-8554).
- Within the area, object `<name>~` values must be unique (no duplicate item names in the same area file).

### Object Spec Compliance (all objects)

Per `area_file_spec.md` section 7:

- **Every object must include `ITEM_TAKE`** (`8388608`) in `wear_flags`. No exceptions.
- **No object may include `ITEM_WEAR_CLAN_COLORS`** (`16777216`) in `wear_flags`.
- Object `name`, `short_descr`, and `description` must be thematically consistent with the object's non-`take` wear flags (e.g., a `head` item must read as headgear, `hold` as a held item).
- **Area-authored items must not define fixed stats** in the area file. Item stats are generated at runtime from item level and other runtime parameters. Author object identity/behavior data only (type/flags/wear/apply/value layout/weight/level), not handcrafted stat tuning.
- **`ITEM_GENERATED`** (`1`) must not be set in area-authored objects; it is runtime-managed.
- **`ITEM_LOOT`** (`67108864`): items spawned from a mobile's loot table (`l`/`L` extension) must have this set. Items loaded via `#RESETS` (`G`/`E`) must not have it unless explicitly intended as loot-only.
- **`ITEM_BOSS`** (`134217728`): any item that can drop from a boss mob must include this flag, regardless of delivery method (loot table or reset equipment).
- Object extra-description `<description>~` must end with exactly one trailing newline before `~`.

### Weight Ranges (archetype encoding)

Per `area_file_spec.md` section 7:

- `1`-`5`: caster item
- `6`-`10`: melee item
- `11`-`15`: tank item

### Weapon Constraints

Per `area_file_spec.md` sections 7.3 and 7.6:

- Weapons must have `item_type = ITEM_WEAPON` (`5`) and `wear_flags` including both `hold` (`32768`) and `take` (`8388608`).
- `value3` (attack type) must be thematically consistent with the weapon's concept and presentation (`name`, `short_descr`, `description`).
- `value3 = 0` (`hit`) must not be used unless the object also has `ITEM_FIST` extra flag.
- **Two-handed weapons**: if a weapon's identity clearly indicates a two-handed archetype (e.g., "great axe", "greatweapon"), it must include `ITEM_TWO_HANDED` (`2147483648`) in `extra_flags`.
- **Bucklers**: `item_type = ITEM_ARMOR`, `wear_flags` includes `hold`, `extra_flags` includes `ITEM_BUCKLER` (`268435456`).
- **Shields**: `item_type = ITEM_ARMOR`, `wear_flags` includes `hold`.

### Item Families

1. **Caravan Charter Set** (levels 30-50, traveler gear)
   - Veils (`head`, weight 1-5 caster), wraps (`about`, weight 1-5 caster), charter bucklers (`hold` + `ITEM_BUCKLER`, weight 11-15 tank), waterskin relics (`hold`, `ITEM_TREASURE`).
   - `L` level lines: 30-50 range.

2. **Synod Ritual Relics** (levels 50-75, cult-themed)
   - Jackal masks (`face`, weight 1-5 caster), anointing blades (`hold` + `ITEM_WEAPON`, `value3 = 1` slice, weight 6-10 melee), prayer censers (`hold`, `ITEM_TREASURE`, weight 1-5 caster), eclipse seals (`finger`, weight 1-5 caster).
   - `L` level lines: 50-75 range.

3. **Reaver Spoils** (levels 40-70, salvaged military)
   - Salvaged polearms (`hold` + `ITEM_WEAPON` + `ITEM_TWO_HANDED`, `value3 = 11` pierce, weight 6-10 melee), hooked blades (`hold` + `ITEM_WEAPON`, `value3 = 3` slash, weight 6-10 melee), storm cloaks (`about`, weight 11-15 tank).
   - `L` level lines: 40-70 range.

4. **Regent Court Regalia** (levels 75-100, elite/boss drops)
   - Diadem fragments (`head`, weight 1-5 caster), sun-court mail (`body`, weight 11-15 tank), horizon-forged greatweapons (`hold` + `ITEM_WEAPON` + `ITEM_TWO_HANDED`, `value3 = 3` slash, weight 6-10 melee).
   - Boss drops: must include `ITEM_BOSS` in `extra_flags`.
   - Loot-table drops: must include `ITEM_LOOT` in `extra_flags`.
   - `L` level lines: 75-100 range.

5. **Lore Objects / Quest Hooks** (various levels)
   - Salt ledgers (`ITEM_TREASURE`, `hold`, weight 1-5), oath tablets (`ITEM_TREASURE`, `hold`, weight 6-10), cracked astrolabes (`ITEM_TREASURE`, `hold`, weight 1-5), cistern key-discs (`ITEM_KEY`, `hold`, weight 1-5).
   - Key objects: any cistern key-disc used as `<key_vnum>` for a locked door must exist in `#OBJECTS` with `item_type = ITEM_KEY` (`18`).

### Key Objects for Locked Doors

Per `area_file_spec.md` section 8.3: if any door is reset to locked (`D` state `2`), `<key_vnum>` must be set to a valid key object vnum, and that key object must exist in `#OBJECTS`. Plan the following key objects:

- **Cistern Vault Key** (for Zone B sealed descent door): `ITEM_KEY`, `hold` + `take`, weight 1.
- **Processional Gate Seal** (for Zone D ritual gate door): `ITEM_KEY`, `hold` + `take`, weight 1.
- **Regent's Eclipse Key** (for Zone E final boss chamber door): `ITEM_KEY`, `hold` + `take`, weight 1.

### Mechanical Tone

- Gear progression supports levels 30-100 with meaningful spikes at zone transitions (30-50 Zone A/B, 50-75 Zone C/D, 75-100 Zone E).
- Desert thematic identity expressed via object names, descriptions, and `item_apply` choices (e.g., `infra` for desert night vision, `det_hid` for sandstorm awareness), not via handcrafted stat values.
- Boss drops reflect eclipse/solar duality and visually match the multi-color theme in `short_descr` and `description` color code usage.

---

## Encounter and Progression Design

- **Early (8500-8524):** Navigation pressure, bandit skirmish encounters, readable faction telegraphing through room descriptions and mob `long_descr`. Charter Caravaneers and low-level Reaver skirmishers. Level 30-50 content.
- **Mid (8525-8569):** Branching objectives, elite Synod ritual nodes, hazard traversal through hot storm rooms. Jackal Synod mid-tier and Reaver banner captains. Level 50-75 content.
- **Late (8570-8599):** Boss approach with strong lore density, undead court remnants, relic payoff. Ancient Court Remnants and all three bosses. Level 75-100 content.

Optional implementation layer:
- Add rare lore-drop objects (with `ITEM_LOOT` flag) tied to codex-style help entries.
- Introduce one non-hostile "ledger ghost" NPC (the **Blind Oracle of the Ninth Meridian**) at a lore-critical room to deliver diegetic tutorial breadcrumbs. `act`: `is_npc` + `stay_area` + `sentinel`, non-aggressive.

---

## `#RESETS` Plan (spec-compliant)

Per `area_file_spec.md` section 12:

- `#RESETS` must not contain blank lines.
- Reset commands reference valid vnums within the area's vnum envelope.

### Mob Resets (`M` command)

Format: `M 0 <mob_vnum> <limit> <room_vnum> <notes>`

- **Zone A (Caravan Spine):** Reset Charter Caravaneers at road rooms (outriders at 8502, 8508; quartermaster at 8506; water clerk at 8510). Reset Reaver skirmishers at ambush rooms (8504, 8514, 8516). Limit 1-2 per mob.
- **Zone B (Cistern Quarter):** Reset Synod initiates and toll-priests at shrine/cistern rooms (8522, 8528, 8533). Reset Blind Oracle at lore room (8528 or 8535). Limit 1 per mob.
- **Zone C (Sirocco Expanse):** Reset apex predators across storm rooms (glass scorpions, ash adders, dune stalkers at 8542, 8545, 8548, 8553). Reset Reaver banner captain at 8552. Reset mini-boss The Bannerless Reaver at 8559 (limit 1).
- **Zone D (Obelisk Marches):** Reset Synod elites at ritual nodes (dusk chanters at 8563, 8567; astromancer vizier at 8572). Reset oathbound sentinels at obelisk rooms (8561, 8565). Reset mini-boss High Anointer at 8579 (limit 1).
- **Zone E (Buried Court):** Reset Ancient Court Remnants throughout (salt mummies at 8582, 8588; eclipse wardens at 8592, 8596). Reset final boss The Last Sand Regent at 8599 (limit 1).

### Object Resets (`O` command)

Format: `O 0 <obj_vnum> <limit> <room_vnum> <notes>`

- Place lore objects (salt ledgers, oath tablets) in key lore rooms (8528, 8561, 8586) for atmospheric pickup.
- Place key objects near their corresponding doors or on gatekeeper mobs via `G`/`E` resets.

### Equipment Resets (`G`/`E` commands)

Format: `G 0 <obj_vnum> <limit>` (give to previous mob) / `E 0 <obj_vnum> <limit> <wear_loc>` (equip on previous mob)

- Equip Synod toll-priests with anointing blades and jackal masks.
- Equip Reaver banner captains with salvaged polearms and storm cloaks.
- Equip boss mobs with thematic Regent Court Regalia (items must include `ITEM_BOSS` flag).

### Door Resets (`D` command)

Format: `D 0 <room_vnum> <door_direction> <state>`

Per `area_file_spec.md` section 8.3: any exit set to closed or locked via `D` reset must have `EX_ISDOOR` (`1`) set in its room `D<door>` `<locks>` definition.

Planned door resets:

1. **Zone B Sealed Descent (8539):** Direction down (D5) to Zone E entry.
   - Room exit `<locks>`: `door` (`1`). `<key_vnum>`: vnum of Cistern Vault Key object.
   - `D` reset: `D 0 8539 5 2` (locked).
   - Reverse exit on 8580 must also have `door` bit set.
   - Key object (Cistern Vault Key) must exist in `#OBJECTS`.

2. **Zone D Processional Gate (8579):** Direction south (D2) or appropriate direction to Zone E.
   - Room exit `<locks>`: `door` (`1`). `<key_vnum>`: vnum of Processional Gate Seal object.
   - `D` reset: `D 0 8579 2 2` (locked).
   - Reverse exit must also have `door` bit set.
   - Key object (Processional Gate Seal) must exist in `#OBJECTS`.

3. **Zone E Boss Chamber (8598 -> 8599):** Direction into boss room.
   - Room exit `<locks>`: `door` (`1`). `<key_vnum>`: vnum of Regent's Eclipse Key object.
   - `D` reset: `D 0 8598 <dir> 2` (locked).
   - Reverse exit on 8599 must also have `door` bit set.
   - Key object (Regent's Eclipse Key) must exist in `#OBJECTS`.

All three doors reset to **locked** (state `2`). Each has a corresponding key object in `#OBJECTS`. Keys can be placed on gatekeeper mobs via `G` resets or in adjacent rooms via `O` resets.

### Spawn Philosophy

- Keep ambient life active on caravan spine (Zone A) so the zone never feels empty. Use higher limits (2-3) for common road NPCs.
- Concentrate Synod elites at ritual chokepoints (Zone B/D) rather than random saturation. Use limit 1 for elite mobs.
- Use higher mob density in storm rooms (Zone C) to create traversal urgency.
- Gate final boss chamber (8599) with locked door requiring Regent's Eclipse Key, providing visible progression gating.

---

## `#SPECIALS` Plan

Per `area_file_spec.md` section 10:

- `spec_poison` assigned to ash adder mobs (desert venomous snake ecology).
- `spec_cast_undead` assigned to The Last Sand Regent (final boss; undead offensive casting).
- `spec_cast_cleric` assigned to High Anointer of the Synod (mini-boss; ritual defensive casting).
- `spec_cast_mage` assigned to Synod Astromancer Vizier (offensive arcane casting).
- No `spec_summon_*` specials (runtime-only, builder-prohibited).
- No `spec_keep*` specials (manual-assignment only).

---

## Canonical Section Order

Per `area_file_spec.md` section 14, the final `.are` file should emit sections in this order:

1. `#AREA`
2. `#ROOMS`
3. `#MOBILES`
4. `#MOBPROGS` (if any inline mobprogs are used)
5. `#OBJECTS`
6. `#SHOPS` (none planned for this area; no merchant NPCs)
7. `#RESETS`
8. `#SPECIALS`
9. `#OBJFUNS` (none planned for this area)
10. `#$` (end-of-file marker)

---

## Implementation Checklist

### Rooms
- [ ] Populate all 100 room vnums (8500-8599) in ascending sequential order with no gaps.
- [ ] Write unique `<description>~` for every room (minimum 3 sentences; 5+ for important rooms).
- [ ] Assign correct `sector_type` (`desert` for outdoor, `inside` for interior) and `room_flags` per zone requirements.
- [ ] Set `no_mob` on all boss rooms (8559, 8579, 8599).
- [ ] Set `no_teleport` + `no_recall` on Zone E rooms (except transit rooms 8580, 8599).
- [ ] Set `hot` on Zone C storm rooms (8540-8559).
- [ ] Ensure all exits are bi-directional with no directional loops.
- [ ] Ensure all named exits use `^` prefix and are discoverable in room description, objects, or extra descriptions.
- [ ] Ensure all extra-description keyword chains are anchored in the room's main `<description>~`.
- [ ] Preserve required worldgraph external connectors while renaming and recontextualizing local rooms.

### Mobiles
- [ ] Write all ~32 mob templates (vnums 8500-8531) in ascending sequential order with no gaps.
- [ ] Ensure all mobs have `is_npc` + `stay_area` in `act` flags.
- [ ] Ensure boss mobs have `sentinel` + `boss` + `no_flee` and are placed only in `no_mob` rooms.
- [ ] Ensure strong non-boss mobs have `solo` flag.
- [ ] Ensure all `long_descr` are exactly one text line + newline + `~`-only line.
- [ ] Ensure all `description` end with exactly one trailing newline before `~`.
- [ ] Assign `!`/`|`/`+` extension lines for combat-capable mobs with appropriate skill profiles.
- [ ] Set `l`/`L` loot tables on boss mobs with valid object vnums; loot items have `ITEM_LOOT`.

### Objects
- [ ] Write all ~55 object templates (vnums 8500-8554) in ascending sequential order with no gaps.
- [ ] Ensure unique `<name>~` for every object in the area.
- [ ] Ensure all objects include `ITEM_TAKE` in `wear_flags`.
- [ ] Ensure no objects include `ITEM_WEAR_CLAN_COLORS`.
- [ ] Ensure no objects set `ITEM_GENERATED`.
- [ ] Ensure boss-drop items include `ITEM_BOSS` flag.
- [ ] Ensure loot-table items include `ITEM_LOOT` flag.
- [ ] Assign correct weight ranges (1-5 caster, 6-10 melee, 11-15 tank).
- [ ] Ensure weapon `value3` is thematically consistent; no `value3=0` without `ITEM_FIST`.
- [ ] Ensure two-handed weapons have `ITEM_TWO_HANDED` flag.
- [ ] Create all three key objects (Cistern Vault Key, Processional Gate Seal, Regent's Eclipse Key).

### Resets
- [ ] Write `M` resets for all planned mob placements with appropriate limits.
- [ ] Write `G`/`E` resets for mob equipment.
- [ ] Write `O` resets for atmospheric/lore objects.
- [ ] Write `D` resets for all three locked doors with state `2`.
- [ ] Verify every locked door has `EX_ISDOOR` in room exit `<locks>` and valid `<key_vnum>`.

### Specials
- [ ] Assign `spec_poison` to ash adder mobs.
- [ ] Assign `spec_cast_undead` to The Last Sand Regent.
- [ ] Assign `spec_cast_cleric` to High Anointer of the Synod.
- [ ] Assign `spec_cast_mage` to Synod Astromancer Vizier.

### Color Theme
- [ ] Apply color theme consistently across all room, mob, and object authored strings.
- [ ] Verify no `@@k` thematic usage, no background codes (`@@0`-`@@7`), no flashing (`@@f`).
- [ ] Verify `@@N` reset after all colorized strings.

### Final Validation
- [ ] Validate complete area file format against `docs/area_file_spec.md`.
- [ ] Verify all vnums within `V 8500 8599` envelope.
- [ ] Verify `#ROOMS` and `#MOBILES` and `#OBJECTS` each terminate with `#0`.
- [ ] Verify `#RESETS`, `#SPECIALS` each terminate with `S`.
- [ ] Verify canonical section order.
- [ ] Run `make unit-tests` from `src/` to validate.
