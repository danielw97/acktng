# Area Plan: The Northern Oasis (Spec-Aligned Rewrite)

## Overview

**Area Name:** `@@lThe Northern Oasis@@N`
**File Name:** `the_northern_oasis.are`
**Keyword:** `the northern oasis`
**Level Range:** 45-65 (`I 45 65`)
**Vnum Range (fixed):** **2250-2449** (`V 2250 2449`)
**Room Count Target:** 35/35 vnums used
**Design Goal:** Replace repetitive lane/patrol generation with a coherent story area: a functioning but politically strained oasis quarter where water adjudication, caravan protection, and relic trafficking intersect. The Northern Oasis is the measured frontier of the Great Oasis system — the place where water becomes policy.

This plan keeps the existing vnum envelope exactly as assigned and fully reworks room flow, encounter identity, objectization, and narrative progression.

---

## Color Theme (Multi-Color, Lore-Driven)

- **Intake-cyan (`@@a`)**: clean water, intake sluice flow, warden authority, cistern reflections.
- **Sun-gold (`@@y`)**: desert heat, caravan lanterns, charter stone carvings, permit stamps, cairn market brass.
- **Ledger-blue (`@@l`)**: adjudication authority, water-court proceedings, deep cistern water, measured calibration.
- **Earth-brown (`@@b`)**: dried mud, reed thatch, cracked clay, waystone sandstone, old cistern masonry.
- **Cult-purple (`@@p`)**: Salt-Jackal Offshoot rites, cistern cult activity, corrupted oath-language.
- **Threat-red (`@@R`)**: reaver aggression, boss foreshadowing, scarcity violence, blood.
- **Reset (`@@N`)** after all colorized strings.

Theme rule compliance for implementation:
- No `@@k` thematic usage.
- No background codes (`@@0`-`@@7`) and no flashing (`@@f`).

---

## Design Pillars

1. **Water is governance, not scenery.** Every district should communicate who controls flow, storage, and access.
2. **Northern Oasis is an administrative frontier.** It should feel less ceremonial than the Great Oasis and less militarized than deep desert cult zones.
3. **The area bridges living trade and dead empires.** Pyramid references are indirect (records, stonework, survey lines), not full tomb aesthetics.
4. **Room graph over corridor spam.** Use loops, optional subroutes, and role-based neighborhoods.

---

## Lore Alignment Targets

- **Great Oasis:** Northern Oasis functions as an upstream intake and arbitration station feeding the Great Oasis legal-water economy. The Water Court's charter system and Keeper-era measurement traditions are directly visible in the Reedline Intake Quarter and Cairn Market.
- **Eastern Desert:** Charter-stone, caravan law, and cistern engineering motifs continue from Eastern Desert waystations. The Keepers of Measure built the Northern Oasis's earliest infrastructure as a calibration waypoint on the road network.
- **Pyramids (lesser):** Presence appears through inherited measurement doctrine, salvage inscriptions, and old sovereign-era requisition marks. The Sand Sovereign's engineers requisitioned oasis water and labor during the Northern Pyramid's construction. Reused Keeper-era survey instruments appear in cistern walls.
- **Midgaard (lesser):** Midgaard interests appear through survey crews, buyers, and copied tablets rather than direct governance. Midgaard-funded authenticators operate in the Cairn Market.
- **Southern Oasis:** Northern Oasis intake verification and audited manifests feed into the Southern Oasis's convoy-hardening logistics. The two oases form the twin administrative poles of the Oasis-Pyramid Corridor.

---

## Lore

See `northern_oasis_lore.md` for extracted lore.

---

## `#AREA` Header Plan (spec-compliant)

All directives per `area_file_spec.md` section 3:

- `Q 16` — mandatory area version; no other value is valid
- `K the northern oasis~` — keyword, `~`-terminated
- `L @@W(@@l45 65@@W)@@N~` — white parens, blue level numbers, white close paren, reset, `~`-terminated
- `N <allocate at implementation time>` — area number; must be confirmed unused across all loaded area files
- `I 45 65` — min/max level; controls mob-level gating and area matching
- `V 2250 2449` — full 35-vnum envelope; all rooms, mobs, and objects must stay within this range
- `F 15` — reset frequency in minutes
- `U @@awater trickles through the intake sluices as the Northern Oasis measures another day@@N~` — reset message; no double-newlines, `~`-terminated
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

## Area Structure (Room Topology)

Target ~35 rooms in five connected bands, using all vnums 2250-2449:

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

- **Outdoor dune/approach rooms** (caravan roads, watch cairns, dune crests): `sector_type = desert` (`10`)
- **Oasis settlement rooms** (markets, courts, warden posts): `sector_type = city` (`1`)
- **Interior rooms** (cistern vaults, tally halls, intake sluice chambers, undercut tunnels): `sector_type = inside` (`11`), `room_flags` includes `indoors` (`8`)
- **Boss rooms**: `room_flags` includes `no_mob` (`4`) per boss placement policy
- **Dry Cistern Undercut rooms**: `room_flags` includes `dark` (`1`) for underground darkness
- **No rooms use `ROOM_MAZE`** (`524288`); this area does not contain mazes

### Vnum Allocation Policy

Per `area_file_spec.md` section 13.1:

- Room vnums must be assigned in ascending sequential order with no gaps.
- All 35 room vnums (2250-2449) must be used; fill every available room-vnum slot.

### Band 1: North Approach Dunes (2032-2071, 40 rooms)

Entry/combat zone. Caravan mile stones, wind-worn watch cairns, predatory fauna. Multiple ingress paths from eastern approaches.

- Sector: `desert` (`10`) throughout.
- Key rooms:
 - **2032** Eastern Desert Road Junction (important room, 5+ sentence description; external connector from Eastern Desert charter routes).
 - **2042** Charter Mile Marker Ridge (first branch hub; important room, 5+ sentence description with `E` extra descriptions for charter stone inscriptions).
 - **2052** Reaver Ambush Wash (combat hotspot; narrow terrain with high mob density).
 - **2062** Windbreak Cairn Circle (lore room; important room, 5+ sentence description with `E` extra descriptions for weathered survey glyphs).
 - **2071** Oasis Threshold (transition into settlement; important room, 5+ sentence description).

### Band 2: Reedline Intake Quarter (2072-2111, 40 rooms)

Social + utility zone. Wells, sluice gates, maintenance walkways, ration court. Neutral-to-guarded interactions.

- Sector: mix of `city` (`1`) for open-air wells and plazas and `inside` (`11`) for sluice chambers and ration court interiors.
- Indoor rooms use `room_flags` including `indoors` (`8`).
- Key rooms:
 - **2072** Intake Gate (important room, 5+ sentence description; named exit `^intake gate` discoverable in description).
 - **2080** Main Well Plaza (central hub; important room, 5+ sentence description).
 - **2087** Sluice Control Chamber (inside sector; important room, 5+ sentence description with `E` extra descriptions for sluice mechanisms and flow gauges).
 - **2097** Ration Court Hall (inside sector; important room, 5+ sentence description; civil arbitration for water quota disputes).
 - **2111** Reed Terrace Passage (connects to Cairn Market; important room).

### Band 3: Cairn Market and Ledger Ward (2112-2151, 40 rooms)

Trade/civil zone. Contract scribes, weighing courts, bonded caravans. Quest hooks tied to manifests, shortages, and forged seals.

- Sector: `city` (`1`) for outdoor market stalls and plazas; `inside` (`11`) for scribe offices and weighing court interiors.
- Key rooms:
 - **2112** Cairn Market Entrance (important room, 5+ sentence description).
 - **2122** Weighing Court (lore-critical arbitration room; inside sector; important room, 5+ sentence description with chained `E` extra descriptions for ledger entries, forged seals, and manifest disputes).
 - **2132** Bonded Caravan Yard (outdoor staging area; important room, 5+ sentence description).
 - **2140** Scribe's Archive Hall (inside sector; important room with `E` extra descriptions for charter copies, survey records, and pyramid-era requisition marks).
 - **2151** Ledger Ward Rear Gate (connects to Dry Cistern Undercut; important room).

### Band 4: Dry Cistern Undercut (2152-2191, 40 rooms)

Dungeon loop. Partial collapses, old Keeper tunnels, scavenger nests. Primary source of relic drops and mini-boss encounters.

- Sector: `inside` (`11`) throughout; `room_flags` includes `indoors` (`8`) and `dark` (`1`).
- Key rooms:
 - **2152** Cistern Access Shaft (zone entry; important room, 5+ sentence description; vertical connector via D4 up / D5 down exits).
 - **2162** Collapsed Keeper Tunnel (lore room with Keeper-era survey instruments embedded in walls; important room, 5+ sentence description).
 - **2170** Scavenger Nest Den (high mob density; combat hub).
 - **2177** Old Sluice Junction (old hydraulic infrastructure; important room with `E` extra descriptions for ancient flow marks and measurement glyphs).
 - **2184** Salt-Jackal Ritual Chamber (cult mini-boss room; `room_flags` includes `no_mob`; important room, 5+ sentence description).
 - **2191** Deep Cistern Passage (connects to High Adjudicator Terrace; important room).

### Band 5: High Adjudicator Terrace (2192-2231, 40 rooms)

Boss + resolution zone. Water-court chamber, shrine to oath-stones, final conflict. Outcome framing should support faction choices.

- Sector: mix of `city` (`1`) for outdoor terrace and shrine rooms and `inside` (`11`) for court chamber and archive interiors.
- Key rooms:
 - **2192** Terrace Approach Stair (important room, 5+ sentence description; transition from undercut to adjudication zone).
 - **2202** Oath-Stone Shrine (lore-critical room; important room, 5+ sentence description with chained `E` extra descriptions for oath-stone inscriptions tracing back to Keeper-era Water Court traditions).
 - **2212** High Adjudicator's Antechamber (pre-boss staging; important room, 5+ sentence description).
 - **2222** Water Court Chamber (primary boss room; `room_flags` includes `no_mob`; important room, 5+ sentence description; named exit `^court chamber` discoverable in description).
 - **2227** Archive of Disputed Quotas (lore room; inside sector; chained `E` extra descriptions for contradicting quota records).
 - **2231** Northern Road (external connector toward northern routes and Northern Pyramid approach; important room, 5+ sentence description).

### Connectivity Principles

Per `area_file_spec.md` section 8 directional traversal constraints:

- **All exits must be bi-directional**: if room A connects to room B through direction X, room B must provide the opposite-direction exit back to room A. No one-way exits (this area is not a maze).
- **No directional loops**: repeated movement in the same direction must not return to a previously visited room. The spec prohibits room connection loops (e.g., `a -> b -> c -> a`) unless all rooms in the set are flagged `ROOM_MAZE`.
- **Hub-and-spoke structure**: the plan uses hub rooms at band transitions (Oasis Threshold 2071, Main Well Plaza 2080, Cairn Market Entrance 2112, Cistern Access Shaft 2152, Terrace Approach Stair 2192) to provide route variety without circular loops.
- **Multiple traversal options between bands**: at least 2 branching paths connect each pair of adjacent bands, providing route variety without circular loops.
- **External exits preserved** where legacy worldgraph depends on them, but contextualized via renamed rooms/exits with bi-directional counterparts.
- **Door usage** limited to intake gates, cistern hatches, and court doors. Every door exit must have `EX_ISDOOR` (`1`) set in `<locks>`. Door initial state (open/closed/locked) is authored through `#RESETS` command `D`, not through room exit `<locks>` bits. Any locked door (`D` state `2`) must have `<key_vnum>` set to a valid key object vnum, and that key object must exist in `#OBJECTS`.
- **Named exits** use `^` prefix on keywords (e.g., `^intake gate~`, `^court chamber~`) so movement messaging treats them as standalone noun phrases. Each named exit keyword must appear in the room's `<description>~`, an object in the room, or an `E` extra description.

---

## Factions and Encounter Identity

### Mob Vnum Strategy

Per `area_file_spec.md` section 13.1:

- Mob vnums must be assigned in ascending sequential order with no gaps, starting from 2032.
- Target composition: ~45 mob templates (vnums 2032-2076).

### Mobile Spec Compliance (all mobs)

Per `area_file_spec.md` sections 4 and 5.4:

- **All mobs** must have `act` flags including `is_npc` (`1`) and `stay_area` (`64`).
- **Sentinel mobs** (guards, shopkeepers, stationary NPCs): add `sentinel` (`2`) to `act` flags.
- **Boss mobs**: must have `act` flags including `sentinel` (`2`) and `boss` (`67108864`), and must be placed only in rooms flagged `no_mob` (`4`).
- **Strong non-boss mobs** (elite patrol leaders, veteran wardens): must have `act` flags including `solo` (`33554432`).
- **Aggressive mobs** (hostile reavers, cult members, scavenger fauna): set `aggressive` (`32`) in `act` flags.
- **`invasion`** (`536870912`) must never be set in area files; it is runtime-only.
- Mobile `long_descr` must be exactly one text line followed by a newline and a `~`-only line. Multi-line `long_descr` is never valid under any circumstances.
- Mobile `description` must end with exactly one trailing newline before the terminating `~`.
- Mobile `player_name` (keyword), `short_descr`, `long_descr`, and `description` are all `~`-terminated.
- Within a sub-region of the area, mobs must either be flagged `sentinel` or the sub-region must be enclosed by rooms flagged `no_mob`, so mobs cannot wander out of their sub-region.

### Mobile Extension Lines

Per `area_file_spec.md` sections 4.1-5.5:

- `!` extension: `<class> <clan> <race> <position> <skills> <cast> <def>` — used for combat-capable mobs. Skill bitvectors follow the level-gated floor policy tables (melee/hybrid/caster profiles).
- `|` extension: `<strong_magic> <weak_magic> <race_mods> <power_skills> <power_cast> <resist> <suscept>` — used for elemental affinities.
- `+` extension: combat modifier tuning for significant encounters.
- `l`/`L` extensions: loot tables. `loot_amount` is percent-based (see spec). Items in loot tables must have `ITEM_LOOT` set in `extra_flags`.

### Faction Roster

1. **Reedline Wardens** (order faction)
  - Intake guards, sluice engineers, quota enforcers, anti-poison monitors, reed maintenance crew.
  - `act`: `is_npc` + `stay_area` + `sentinel` (for stationary guards/engineers). Non-aggressive.
  - Level range: 45-55.
  - Archetype: melee profile (guards) and hybrid profile (engineers with light defensive casting).

2. **Cairn Brokers** (neutral-pragmatic)
  - Contract scribes, weighing clerks, caravan arbitrators, seal inspectors, trade authenticators.
  - `act`: `is_npc` + `stay_area` + `sentinel`. Non-aggressive.
  - Level range: 45-50.
  - Archetype: hybrid profile (light arms, some utility).

3. **Sunscar Reavers** (hostile)
  - Skirmishers, route cutters, wagon raiders, beast-handlers.
  - `act`: `is_npc` + `stay_area` + `aggressive`. Route cutters add `no_flee` (`16`).
  - Level range: 45-60 (skirmishers low, beast-handlers high).
  - Archetype: melee profile. Beast-handlers use hybrid profile.

4. **Salt-Jackal Offshoot** (limited cult presence, hostile)
  - Cistern rite-keepers, oath-twisting acolytes, brine ritualists.
  - `act`: `is_npc` + `stay_area` + `aggressive`. Rite-keepers add `sentinel` (stationed at ritual nodes).
  - Level range: 50-60.
  - Archetype: caster profile (ritual casting). Brine ritualists use hybrid profile.
  - `affected_by`: rite-keepers get `detect_hidden` (`32`).

5. **Approach Dune Fauna** (hostile)
  - Sand vipers, dune jackals, glass scorpions, carrion kites.
  - `act`: `is_npc` + `stay_area` + `aggressive` + `no_mind` (`262144`).
  - Level range: 45-55.
  - Archetype: melee profile. Sand vipers add poison spec (`spec_poison` in `#SPECIALS`).

### Boss Ladder

All boss mobs: `act` includes `is_npc` + `stay_area` + `sentinel` + `boss` + `aggressive` + `no_flee`. Placed only in rooms flagged `no_mob`.

- **Secondary Boss (2184, Band 4):** Cistern Scavenger-Matriarch. Level 58. Melee profile with bestial combat. `act` adds `remember` (`8`). `affected_by`: `infrared` (`512`). Items dropped must include `ITEM_BOSS` flag. Loot table (`l`/`L`) used for boss drops.
- **Primary Boss (2222, Band 5):** Warden-Commander (corrupted by scarcity politics — civil tragedy angle). Level 65. Hybrid profile with strong melee and defensive casting. `act` adds `remember` (`8`). Assigned `spec_cast_cleric` in `#SPECIALS`. `affected_by`: `detect_invis` (`8`), `infrared` (`512`). Items dropped must include `ITEM_BOSS` flag. Loot table (`l`/`L`) used for boss drops.

---

## Object Redesign Plan

### Object Vnum Strategy

Per `area_file_spec.md` section 13.1:

- Object vnums must be assigned in ascending sequential order with no gaps, starting from 2032.
- Expand through contiguous sequential slots (suggested through **2096**) while staying within the 2250-2449 vnum envelope.
- Target composition: ~65 object templates (vnums 2032-2096).
- Within the area, object `<name>~` values must be unique (no duplicate item names in the same area file).

### Object Spec Compliance (all objects)

Per `area_file_spec.md` section 7:

- **Every object must include `ITEM_TAKE`** (`8388608`) in `wear_flags`. No exceptions.
- **No object may include `ITEM_WEAR_CLAN_COLORS`** (`16777216`) in `wear_flags`.
- Object `name`, `short_descr`, and `description` must be thematically consistent with the object's non-`take` wear flags (e.g., a `head` item must read as headgear, `hold` as a held item).
- **Area-authored items must not define fixed stats** in the area file. Item stats are generated at runtime from item level and other runtime parameters.
- **`ITEM_GENERATED`** (`1`) must not be set in area-authored objects; it is runtime-managed.
- **`ITEM_LOOT`** (`67108864`): items spawned from a mobile's loot table (`l`/`L` extension) must have this set.
- **`ITEM_BOSS`** (`134217728`): any item that can drop from a boss mob must include this flag.
- Object extra-description `<description>~` must end with exactly one trailing newline before `~`.

### Weight Ranges (archetype encoding)

Per `area_file_spec.md` section 7:

- `1`-`5`: caster item
- `6`-`10`: melee item
- `11`-`15`: tank item

### Weapon Constraints

Per `area_file_spec.md` sections 7.3 and 7.6:

- Weapons must have `item_type = ITEM_WEAPON` (`5`) and `wear_flags` including both `hold` (`32768`) and `take` (`8388608`).
- `value3` (attack type) must be thematically consistent with the weapon's concept and presentation.
- `value3 = 0` (`hit`) must not be used unless the object also has `ITEM_FIST` extra flag.
- **Two-handed weapons**: must include `ITEM_TWO_HANDED` (`2147483648`) in `extra_flags`.
- **Bucklers**: `item_type = ITEM_ARMOR`, `wear_flags` includes `hold`, `extra_flags` includes `ITEM_BUCKLER` (`268435456`).
- **Shields**: `item_type = ITEM_ARMOR`, `wear_flags` includes `hold`.

### Item Families

1. **Desert Approach Set** (levels 45-55, traveler gear)
  - Wind-worn veils (`head`, weight 1-5 caster), caravan wraps (`about`, weight 11-15 tank), charter bucklers (`hold` + `ITEM_BUCKLER`, weight 11-15 tank), wayfinder rods (`hold` + `ITEM_WEAPON`, `value3 = 7` pound, weight 1-5 caster).
  - `L` level lines: 45-55 range.

2. **Warden Service Gear** (levels 45-60, military-utility)
  - Intake guard helms (`head`, weight 11-15 tank), sluice engineer's bracers (`wrist`, weight 11-15 tank), quota enforcement truncheons (`hold` + `ITEM_WEAPON`, `value3 = 8` crush, weight 6-10 melee), reed-maintenance sickles (`hold` + `ITEM_WEAPON`, `value3 = 3` slash, weight 6-10 melee).
  - `L` level lines: 45-60 range.

3. **Cairn Market Trade Goods** (levels 45-50, utility/caster)
  - Authenticator's loupe (`hold`, `ITEM_TREASURE`, weight 1-5 caster), contract seal rings (`finger`, weight 1-5 caster), weighing court tokens (`hold`, `ITEM_TREASURE`, weight 1-5 caster), bonded caravan pendants (`neck`, weight 1-5 caster).
  - `L` level lines: 45-50 range.

4. **Reaver Spoils** (levels 45-60, salvaged military)
  - Salvaged route-cutter blades (`hold` + `ITEM_WEAPON`, `value3 = 3` slash, weight 6-10 melee), raider's hooked spears (`hold` + `ITEM_WEAPON` + `ITEM_TWO_HANDED`, `value3 = 11` pierce, weight 6-10 melee), storm-patched cloaks (`about`, weight 11-15 tank).
  - `L` level lines: 45-60 range.

5. **Cistern Cult Relics** (levels 50-60, cult-themed)
  - Salt-jackal masks (`face`, weight 1-5 caster), brine-ritual censers (`hold`, `ITEM_TREASURE`, weight 1-5 caster), cistern oath tokens (`finger`, weight 1-5 caster), corroded ritual daggers (`hold` + `ITEM_WEAPON`, `value3 = 2` stab, weight 6-10 melee).
  - `L` level lines: 50-60 range.

6. **Adjudicator Court Regalia** (levels 58-65, boss drops)
  - Warden-Commander's signet (`finger`, weight 1-5 caster), adjudicator's mantle (`shoulders`, weight 11-15 tank), water court gavel (`hold` + `ITEM_WEAPON`, `value3 = 8` crush, weight 6-10 melee), quota doctrine codex (`hold`, `ITEM_TREASURE`, weight 1-5 caster).
  - Boss drops: must include `ITEM_BOSS` in `extra_flags`.
  - Loot-table drops: must include `ITEM_LOOT` in `extra_flags`.
  - `L` level lines: 58-65 range.

7. **Lore Objects / Quest Hooks** (various levels)
  - Flow ledger fragments (`ITEM_TREASURE`, `hold`, weight 1-5), charter stone rubbings (`ITEM_TREASURE`, `hold`, weight 1-5), quota dispute writs (`ITEM_TREASURE`, `hold`, weight 1-5), cistern vault keys (`ITEM_KEY`, `hold`, weight 1-5).
  - Key objects: any key used as `<key_vnum>` for a locked door must exist in `#OBJECTS` with `item_type = ITEM_KEY` (`18`).

### Key Objects for Locked Doors

Per `area_file_spec.md` section 8.3: if any door is reset to locked (`D` state `2`), `<key_vnum>` must be set to a valid key object vnum, and that key object must exist in `#OBJECTS`. Plan the following key objects:

- **Intake Gate Key** (for Intake Gate 2072 door): `ITEM_KEY`, `hold` + `take`, weight 1.
- **Cistern Hatch Key** (for Cistern Access Shaft 2152 door): `ITEM_KEY`, `hold` + `take`, weight 1.
- **Court Chamber Key** (for Water Court Chamber 2222 door): `ITEM_KEY`, `hold` + `take`, weight 1.

---

## `#RESETS` Plan (spec-compliant)

Per `area_file_spec.md` section 12:

- `#RESETS` must not contain blank lines.
- Reset commands reference valid vnums within the area's vnum envelope.

### Mob Resets (`M` command)

Format: `M 0 <mob_vnum> <limit> <room_vnum> <notes>`

- **Band 1 (North Approach Dunes):** Reset dune fauna across approach rooms (sand vipers, dune jackals, glass scorpions at 2037, 2044, 2052, 2057). Reset Sunscar Reaver skirmishers at ambush rooms (2052, 2060). Limit 1-2 per mob.
- **Band 2 (Reedline Intake Quarter):** Reset Reedline Warden guards at intake rooms (2072, 2080, 2087). Reset sluice engineers at Control Chamber (2087). Limit 1 per mob.
- **Band 3 (Cairn Market and Ledger Ward):** Reset Cairn Broker NPCs at market rooms (2112, 2122, 2132, 2140). Reset Sunscar Reaver scouts at market periphery (2117). Limit 1 per mob.
- **Band 4 (Dry Cistern Undercut):** Reset cistern scavenger fauna (2157, 2162, 2170). Reset Salt-Jackal Offshoot cult mobs at ritual nodes (2177, 2184). Reset secondary boss Cistern Scavenger-Matriarch at 2184 (limit 1).
- **Band 5 (High Adjudicator Terrace):** Reset Reedline Warden elite guards at terrace rooms (2192, 2202, 2212). Reset primary boss Warden-Commander at 2222 (limit 1).

### Object Resets (`O` command)

Format: `O 0 <obj_vnum> <limit> <room_vnum> <notes>`

- Place lore objects (flow ledger fragments, charter stone rubbings) in key lore rooms (2062, 2140, 2202) for atmospheric pickup.
- Place key objects near their corresponding doors or on gatekeeper mobs via `G`/`E` resets.

### Equipment Resets (`G`/`E` commands)

Format: `G 0 <obj_vnum> <limit>` (give to previous mob) / `E 0 <obj_vnum> <limit> <wear_loc>` (equip on previous mob)

- Equip Reedline Warden guards with intake guard helms and quota enforcement truncheons.
- Equip Sunscar Reavers with salvaged route-cutter blades and storm-patched cloaks.
- Equip boss mobs with thematic Adjudicator Court Regalia (items must include `ITEM_BOSS` flag).

### Door Resets (`D` command)

Format: `D 0 <room_vnum> <door_direction> <state>`

Per `area_file_spec.md` section 8.3: any exit set to closed or locked via `D` reset must have `EX_ISDOOR` (`1`) set in its room `D<door>` `<locks>` definition.

Planned door resets:

1. **Intake Gate (2072):** Direction appropriate to approach from dunes.
  - Room exit `<locks>`: `door` (`1`). `<key_vnum>`: vnum of Intake Gate Key object.
  - `D` reset: `D 0 2072 <dir> 1` (closed, not locked — intake gate, opened by wardens).
  - Reverse exit must also have `door` bit set.

2. **Cistern Access Shaft (2152):** Direction down (D5) to Dry Cistern Undercut.
  - Room exit `<locks>`: `door` (`1`). `<key_vnum>`: vnum of Cistern Hatch Key object.
  - `D` reset: `D 0 2152 5 2` (locked).
  - Reverse exit on 2153 (or appropriate vnum) must also have `door` bit set.
  - Key object (Cistern Hatch Key) must exist in `#OBJECTS`.

3. **Water Court Chamber (2222):** Direction into boss room.
  - Room exit `<locks>`: `door` (`1`). `<key_vnum>`: vnum of Court Chamber Key object.
  - `D` reset: `D 0 2222 <dir> 2` (locked).
  - Reverse exit must also have `door` bit set.
  - Key object (Court Chamber Key) must exist in `#OBJECTS`.

All locked doors (state `2`) have corresponding key objects in `#OBJECTS`. Keys can be placed on gatekeeper mobs via `G` resets or in adjacent rooms via `O` resets.

---

## `#SPECIALS` Plan

Per `area_file_spec.md` section 10:

- `spec_poison` assigned to sand viper mobs (desert venomous snake ecology).
- `spec_cast_cleric` assigned to Warden-Commander (primary boss; defensive-ritual casting).
- `spec_cast_undead` assigned to Salt-Jackal brine ritualists (cult offensive casting with death-rite theme).
- No `spec_summon_*` specials (runtime-only, builder-prohibited).
- No `spec_keep*` specials (manual-assignment only).

---

## `#SHOPS` Plan

Per `area_file_spec.md` section 9:

- **Main Well Plaza (2080):** Reedline Warden water vendor mob sells provisions. `<keeper>` = vendor mob vnum, appropriate `<buy_type>` slots, reasonable profit margins, daytime operating hours.
- **Cairn Market Entrance (2112):** Cairn Broker trade goods vendor. Similar shop configuration.
- **Bonded Caravan Yard (2132):** Caravan outfitter selling travel gear. Similar shop configuration.

---

## Canonical Section Order

Per `area_file_spec.md` section 14, the final `.are` file should emit sections in this order:

1. `#AREA`
2. `#ROOMS`
3. `#MOBILES`

5. `#OBJECTS`
6. `#SHOPS`
7. `#RESETS`
8. `#SPECIALS`
9. `#OBJFUNS` (none planned for this area)
10. `#$` (end-of-file marker)

---

## Reset and Economy Plan

- **Early (Bands 1-2, 2032-2111):** Desert approach navigation, fauna encounters, intake quarter exploration, social NPC interactions. Reedline Warden and dune fauna populate the scene. Level 45-55 content.
- **Mid (Band 3, 2112-2151):** Market trade, arbitration disputes, manifest quests, peripheral reaver encounters. Cairn Brokers and quest hooks. Level 50-55 content.
- **Late (Bands 4-5, 2152-2231):** Cistern dungeon delve, cult encounters, boss showdowns, water-court resolution. Salt-Jackal cult and both bosses. Level 55-65 content.

Loot profile:
- Distinct reset clusters by district (approach, market, undercut, terrace).
- Loot emphasizes practical desert gear, contract seals, water tools, and limited relic fragments.
- Rare-table items should reference legal-water and survey traditions, not full pyramid regalia.

---

## Delivery Notes

Implementation should prioritize:
1. Room rewrite first,
2. Faction mobs and scripts second,
3. Objectization and loot balance third,
4. Exits/resets polish last.

---

## Implementation Checklist

### Rooms
- [ ] Populate all 35 room vnums (2250-2449) in ascending sequential order with no gaps.
- [ ] Write unique `<description>~` for every room (minimum 3 sentences; 5+ for important rooms).
- [ ] Assign correct `sector_type` (`desert` for approach, `city` for settlement, `inside` for interiors) and `room_flags` per band requirements.
- [ ] Set `no_mob` on all boss rooms (2184, 2222).
- [ ] Set `dark` on Dry Cistern Undercut rooms (2152-2191).
- [ ] Ensure all exits are bi-directional with no directional loops.
- [ ] Ensure all named exits use `^` prefix and are discoverable in room description, objects, or extra descriptions.
- [ ] Ensure all extra-description keyword chains are anchored in the room's main `<description>~`.
- [ ] Preserve required worldgraph external connectors while renaming and recontextualizing local rooms.

### Mobiles
- [ ] Write all ~45 mob templates (vnums 2032-2076) in ascending sequential order with no gaps.
- [ ] Ensure all mobs have `is_npc` + `stay_area` in `act` flags.
- [ ] Ensure boss mobs have `sentinel` + `boss` + `no_flee` and are placed only in `no_mob` rooms.
- [ ] Ensure strong non-boss mobs have `solo` flag.
- [ ] Ensure all `long_descr` are exactly one text line + newline + `~`-only line.
- [ ] Ensure all `description` end with exactly one trailing newline before `~`.
- [ ] Assign `!`/`|`/`+` extension lines for combat-capable mobs with appropriate skill profiles.
- [ ] Set `l`/`L` loot tables on boss mobs with valid object vnums; loot items have `ITEM_LOOT`.

### Objects
- [ ] Write all ~65 object templates (vnums 2032-2096) in ascending sequential order with no gaps.
- [ ] Ensure unique `<name>~` for every object in the area.
- [ ] Ensure all objects include `ITEM_TAKE` in `wear_flags`.
- [ ] Ensure no objects include `ITEM_WEAR_CLAN_COLORS`.
- [ ] Ensure no objects set `ITEM_GENERATED`.
- [ ] Ensure boss-drop items include `ITEM_BOSS` flag.
- [ ] Ensure loot-table items include `ITEM_LOOT` flag.
- [ ] Assign correct weight ranges (1-5 caster, 6-10 melee, 11-15 tank).
- [ ] Ensure weapon `value3` is thematically consistent; no `value3=0` without `ITEM_FIST`.
- [ ] Ensure two-handed weapons have `ITEM_TWO_HANDED` flag.
- [ ] Create all three key objects (Intake Gate Key, Cistern Hatch Key, Court Chamber Key).

### Resets
- [ ] Write `M` resets for all planned mob placements with appropriate limits.
- [ ] Write `G`/`E` resets for mob equipment.
- [ ] Write `O` resets for atmospheric/lore objects.
- [ ] Write `D` resets for all three doors with appropriate states.
- [ ] Verify every locked door has `EX_ISDOOR` in room exit `<locks>` and valid `<key_vnum>`.

### Specials
- [ ] Assign `spec_poison` to sand viper mobs.
- [ ] Assign `spec_cast_cleric` to Warden-Commander.
- [ ] Assign `spec_cast_undead` to Salt-Jackal brine ritualists.

### Color Theme
- [ ] Apply color theme consistently across all room, mob, and object authored strings.
- [ ] Verify no `@@k` thematic usage, no background codes (`@@0`-`@@7`), no flashing (`@@f`).
- [ ] Verify `@@N` reset after all colorized strings.

### Final Validation
- [ ] Validate complete area file format against `docs/area_file_spec.md`.
- [ ] Verify all vnums within `V 2250 2449` envelope.
- [ ] Verify `#ROOMS` and `#MOBILES` and `#OBJECTS` each terminate with `#0`.
- [ ] Verify `#RESETS`, `#SPECIALS` each terminate with `S`.
- [ ] Verify canonical section order.
- [ ] Run `make unit-tests` from `src/` to validate.
