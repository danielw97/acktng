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

## Color Theme (Multi-Color, Lore-Driven)

- **Sun-gold (`@@y`)**: daylight, imperial relics, caravan wealth, oath-markers.
- **Dune-brown (`@@b`)**: sandstone ruins, obelisks, cracked cistern masonry.
- **Sky-cyan (`@@a`)**: mirage-light, old astrolabe glass, moonwell water.
- **Ritual-purple (`@@p`)**: jackal cult rites, funerary wards, eclipse chambers.
- **Threat-red (`@@R`)**: blood altars, elite encounters, boss foreshadowing.
- **Reset (`@@N`)** after all colorized strings.

Theme rule compliance for implementation:
- No `@@k` thematic usage.
- No background codes (`@@0`-`@@7`) and no flashing (`@@f`).

---

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

## Existing Lore Anchors to Preserve and Elevate

Current area identity already implies:
1. A major east-west caravan road network.
2. Obelisk/jackal cult activity.
3. Broken shrines and dry cistern infrastructure.
4. Mirage-heavy dune traversal and reaver predation.

The redesign treats these not as disconnected motifs, but as pieces of one historical arc:

- The Eastern Desert was once the **astronomical frontier** of the old river kingdoms.
- Caravan houses financed waystations and cistern observatories aligned to solstice and moonrise.
- A dynastic collapse and prolonged drought fractured trade authority.
- The **Jackal Synod** (funerary priesthood turned militant cult) seized shrine-cistern nodes, weaponizing ritual access to water and tomb routes.
- Modern caravans still cross the old road, but only under armed charter.

---

## Cross-Area Lore Integration

This area should narratively bridge nearby desert and pyramid content, creating a cohesive world-historical arc:

### Connection to the Northern Pyramid (vnums 10125-10324, levels 100-120)

The Northern Pyramid was constructed by the **Sand Sovereign**, a dynastic heir of the Salt Ledger Kingdoms who elevated the old road-taxation apparatus into monumental funerary architecture. The Eastern Desert's Keepers of Measure and their obelisk-line survey system were the direct predecessors of the Northern Pyramid's astronomical alignment chambers. When players encounter the Twelve Obelisks in Zone D, they are standing in the same calibration tradition that the Sand Sovereign later scaled into pyramid-sized precision.

Specific connections:
- Inscription fragments in Zone B cistern archives should reference "the northern sovereign's commission" and orders to redirect water tax revenues toward pyramid construction.
- The astrolabe glass motif in Zone C (Blueglass Dune Crest) prefigures the Northern Pyramid's grand astrolabe chambers.
- The Synod's ritual vocabulary derives from the same liturgical tradition as the Northern Pyramid's curse-ward priesthood; players who learn Synod terminology in the Eastern Desert will recognize cognate phrases carved into pyramid walls.

### Connection to the Southern Pyramid (vnums 11450-11649, levels 120-140)

The Southern Pyramid was built by the **Moon Sovereign**, twin sibling and rival of the Sand Sovereign. Where the Northern Pyramid ascends, the Southern Pyramid descends into the earth through seven tiers consecrated to aspects of death: silence, decay, shadow, bone, venom, fire, and void. The Eastern Desert's failed Ninth Meridian eclipse rite was a last attempt to reconcile the two sovereigns' competing cosmologies — sun-ascendant versus underworld-descendant.

Specific connections:
- The Last Sand Regent's eclipse-preservation rite in Zone E was an attempt to merge the Sand Sovereign's solar doctrine with the Moon Sovereign's death-descent theology. The rite failed because the two systems were cosmologically incompatible, not because of insufficient precision.
- Salt mummies in Zone E use preservation techniques that parallel (and predate) the Southern Pyramid's Tier 1 Silent Crypts embalming methods.
- The Jackal Synod's "Eclipsed Tongues" faction (apocalyptic schism) draws its theology from Moon Sovereign texts smuggled out of the Southern Pyramid before it was sealed.

### Connection to the Northern/Southern Oasis

Surviving trade routes and waystone inscriptions in Zone A explicitly reference oasis provisioning guilds. The oases were originally cistern-relay stations in the Keeper-of-Measure road network — the last nodes that retained functional water arbitration after the central system collapsed.

Specific connections:
- Charter Mile Marker stones in Zone A bear guild seal impressions that match those found on oasis water-court buildings.
- Caravan quartermasters in Zone A carry route manifests sealed with oasis witness marks.
- The Iseth Reach (vanished river, Lore Bible section 7) once connected the eastern road network to the oasis water table; its drying severed the logistical link and isolated the Eastern Desert's cistern infrastructure.

### Connection to Midgaard (vnums 3001-3200)

Midgaard's city archives preserve late-era expedition charters and salvage law tablets from the Eastern Desert. The city's pragmatic trade culture treated the collapsed desert road network as a resource extraction opportunity: survey crews were sent to map cistern ruins for recoverable stonework, and academic expeditions cataloged obelisk inscriptions for the Midgaard historical society.

Specific connections:
- Object lore on salt ledger tablets (Zone B) should reference Midgaard archival catalog numbers, establishing that some desert artifacts have already been removed and are studied in the city.
- The Blind Oracle of the Ninth Meridian may reference "western scholars" who visit periodically to copy inscriptions.

### Connection to Kowloon (vnums 14000-14099)

Kowloon's Jade Magistracy maintains records of desert trade route law that descend from the same Keeper-of-Measure legal tradition. The Neon Covenant's insistence on "one ledger, one ration code" echoes the Salt Ledger Kingdoms' water-debt accounting system. This is not coincidence — Kowloon's founding House Jien (the Jade Clerks) employed scribal methods directly imported from the desert's collapsed ledger courts.

Specific connections:
- The Eastern Desert's "authorized thirst" toll vocabulary mirrors Kowloon's rationing-year emergency protocols.
- House Vao (the Grain Wardens) of Kowloon preserved diluted versions of the Eastern Desert's water arbitration law, which they adapted for grain and staple management.
- The Courier Lantern Office's registry methodology descends from the same census tradition that produced the desert's road-charter population counts.

### Connection to Kiess (vnums 13000-13099)

Kiess sits on Roc Road, the ancient western trade artery. While geographically distant from the Eastern Desert, Kiess's Trade Syndics maintain commercial intelligence about desert route conditions, and the Temple of Concord preserves religious texts that include fragments of Keeper-of-Measure survey prayers — repurposed as civic benedictions.

### Connection to the Great Northern Forest (vnums 15500-15999)

The connection is indirect but real. The Great Northern Forest's Passage Tithe system — caravans paying in grain, salt, lamp oil, and iron nails — descends from the same "levy in kind, not coin" economic philosophy that governed the Eastern Desert's water-right toll roads. The Ash Winters that devastated the Northern Forest were caused by the same Cinderteeth volcanic activity that contributed to the Eastern Desert's prolonged drought cycle. Both regions suffered ecological collapse from the same distant geological event, and both evolved similar privatized-militia responses (the forest's hired bow-clan escorts mirror the desert's Charter War mercenary crews).

### Unifying Thesis

The pyramid complexes farther north and south represent later ceremonial specializations of the same desert logistics state centered on roads, water ledgers, and celestial taxation. The Eastern Desert is the historical core from which this civilization radiated. Understanding the desert's administrative collapse is prerequisite to understanding why the pyramids were built, why the Synod exists, and why the Regent's final rite was both inevitable and doomed. The desert state's influence extends even further: its legal and economic innovations were carried by refugees, traders, and scribes to distant cities where they were adapted into new forms — Kowloon's covenant bureaucracy, Kiess's trade syndic protocols, and Midgaard's archival traditions all bear the fingerprints of Keeper-of-Measure law.

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

## Extensive Lore Bible (for Room + Mob + Object Writing)

### 1) The Age of Measures
Before empires raised pyramids as royal vaults, the eastern waste was managed by surveyors known as the **Keepers of Measure**. They did not rule by armies but by calibration: road stones set to star angles, cistern basins graded by seasonal drift, and caravan levies assessed in water rights rather than coin. The desert was considered governable if measured correctly.

The Keepers built the first obelisk lines not as monuments but as instruments. At dawn and dusk, their shadows marked legal travel windows, ration allotment days, and safe convoy departure periods. To miss an alignment was not merely unlucky; it was unlawful, because mistimed crossings depleted shared wells and endangered every caravan behind you.

### 2) The Salt Ledger Kingdoms
As traffic expanded, merchant houses transformed survey law into hereditary authority. These dynasts became the **Salt Ledger Kingdoms**, so named for the crystalline tablets used to track water debt and caravan protection contracts. Nobility depended on logistics legitimacy: a ruler was accepted only if routes stayed open and cistern accounts balanced.

The future pyramid courts emerged from this economy. Royal tomb ideology fused with trade administration: kings promised eternal guardianship of roads in exchange for obedience in life. Court astronomy and desert taxation became one institution.

### 3) The Ninth Meridian Doctrine
Late in the dynasty, court astrologers proposed a doctrine of catastrophic precision: if the desert's "ninth sun" (a symbolic eclipse conjunction) could be ritually anchored, drought cycles might be suspended. Massive observatory-cistern complexes were retrofitted for this rite. The experiment failed. Instead of stabilizing rain patterns, it intensified aridity and shattered trust in the road charter system.

The regent line attempted to conceal the failure by militarizing funerary priesthoods, turning civil shrine staff into armed toll enforcers. These enforcers became the first Jackal Synod cadres.

### 4) Fall of the Open Road
Trade confederates withdrew protection when toll violence outpaced caravan profits. Outposts burned. Convoys moved only under private banners. The old east-west spine survived physically, but governance collapsed into pockets: reaver strongholds, cult toll courts, and abandoned cistern vaults with no neutral authority.

The final regent ordered a burial court built beneath the observatory strata, intending an eclipse-preservation rite that would restore legitimacy after death. The rite preserved neither dynasty nor climate. It created a bound remnant intelligence -- the Last Sand Regent -- who still issues decrees to a dead bureaucracy.

### 5) Modern Era Interpretation
Present-day scholars disagree whether the Synod serves the Regent willingly or has only inherited fragments of its command language. Caravan captains treat both possibilities as equally dangerous. Inscriptions in Zone B and Zone D should suggest this ambiguity: the cult is fanatical, but also administrative, obsessed with forms, quotas, and "authorized thirst." 

### 6) Links to Wider World History
- The pyramid complexes farther north and south represent later ceremonial specializations of the same desert state apparatus.
- Oasis guild charters preserve diluted versions of Keeper-of-Measure law (distance stones, water arbitration panels, witness seals).
- City archivists in major hubs prize eastern salt tablets because they document the transition from route commonwealth to necro-bureaucratic monarchy.

### 7) The River That Failed the East
Early tablets repeatedly name a vanished distributary called the **Iseth Reach**, once fed by spring floods descending from distant uplands. While modern travelers know only dry wadis and wind-cut channels, the oldest cistern basements were engineered as overflow governors rather than emergency reservoirs. This suggests the Eastern Desert was not always a pure deadland; it was a managed edge biome between river and dune.

The drying of the Iseth Reach destabilized everything. Water arbitration courts that once settled ordinary seasonal disagreements became war tribunals. Caravan houses stopped debating tariffs and began contesting survival rights. In room writing, this should appear through layered ruin evidence: flood-marks far above modern sand level, dock rings in rooms now miles from water, and legal stelae rewritten from trade law into emergency ration decrees.

### 8) The Charter Wars and the Rise of Banner Law
When centralized ledger authority failed, major houses armed their survey crews and hired mercenary escorts. These conflicts are remembered as the **Charter Wars**, a generation-long struggle in which legal documents were enforced at spearpoint. Each faction claimed to be the true inheritor of Measure Law, but in practice control belonged to whichever banner could hold the next cistern gate.

This is the historical root of the modern Reaver Confederates. They were not initially "bandits" but privatized charter militias who outlived their employers. Over decades, their insignia shifted from account sigils to predatory totems, yet they retained procedural habits: seizure inventories, stamped claim-slates, and ritualized convoy inspections. Rooms in Zone C should show this contradiction -- violence operating in the shell of bureaucracy.

### 9) The Jackal Synod's Internal Schism
The Jackal Synod should not be portrayed as a monolith. Surviving liturgical fragments indicate at least three rival doctrinal schools:

- **Anointers of Continuance:** believe preserving any remnant of regnal command is sacred, even if the Regent is diminished or mad.
- **Keepers of Dry Mercy:** argue that tolls and rites exist to ration suffering, not maximize it; they are severe but not wantonly cruel.
- **Eclipsed Tongues:** apocalyptic faction that interprets drought as divine filtration and seeks total depopulation of the old roads.

Mob and room lore can leverage this fracture: conflicting graffiti in shrine corridors, mutually defaced iconography, and NPC barks that contradict one another about whether caravans are taxable subjects, sinners, or sacrificial fuel.

### 10) The Last Sand Regent as Tragic Antagonist
The Regent should read as more than a generic tyrant. In surviving pre-fall inscriptions, the regent figure appears as a mathematically devout administrator terrified of civil collapse. Their fatal flaw was certainty: the belief that enough precision, coercion, and ritual could force a broken climate back into compliance.

By the time players reach Zone E, environmental storytelling should reveal that the Regent's final rite was framed as a self-sacrifice for continuity. The horror is that the ritual succeeded only in preserving command intent without human judgment. What remains in 8599 is a policy engine with a royal memory -- still issuing decrees in the language of duty while annihilating the living world it claims to safeguard.

### 11) Material Culture Notes for Authoring Consistency
To keep descriptions cohesive, use recurring physical motifs tied to each historical era:

- **Measure Era motifs:** plumb-lines, shadow rods, angle marks, water gauges, chipped chalk numerals.
- **Ledger Kingdom motifs:** salt-crystal tablets, tally cords, bronze stamp seals, lacquered archive tubes.
- **Charter War motifs:** patched banner cloth, broken convoy locks, spear-racked gatehouses, inventory slates.
- **Synod motifs:** jackal-headed censers, bitumen-black unguents, ritual basins, lunar incision marks.
- **Regent Court motifs:** star-metal inlay, eclipse lenses, mirrored obsidian, cracked coronation reliefs.

These motifs should appear across rooms, object extra descriptions, and mob equipment text so the area feels authored as one civilizational ruin rather than disconnected desert set pieces.

### 12) Voice, Inscription, and Diegetic Text Style Guide
To strengthen immersion, plan on three distinct text voices:

1. **Civic-Legal voice** (roads/cisterns): terse, procedural, numerical.
2. **Liturgical voice** (Synod spaces): ceremonial, second-person imperative, references to thirst, purity, and witness.
3. **Court-Cosmic voice** (necropolis/ecliptic chambers): elevated, astronomical metaphors, destiny language.

Example inscription fragments for future use:
- "By sixth shadow and lawful witness, draw no more than one measure."
- "Thirst is the final auditor; concealment is impossible before the jackal stars."
- "When the ninth sun is seated, decree and dawn will be one."

### 13) Rumors, Expedition Hooks, and Survivor Testimony
Populate tavern-rumor and bulletin-board ecosystems in connected areas with short hooks that point back to Eastern Desert:

- A caravan auditor seeks escorts to recover "sealed drought ledgers" from the Cistern Quarter.
- A reaver defector claims the Synod is excavating a second regent chamber deeper beneath the necropolis.
- Oasis healers report unusual salt-burn curses on survivors who drank from certain moonwells.
- An old surveyor insists that one obelisk in Zone D still predicts storms accurately if read at dusk.

Include at least one recurring survivor NPC concept (in plan form) whose testimonies evolve with player progression, revealing that multiple factions are racing not for treasure but for control of water legitimacy narratives.

### 14) Why This Lore Supports the Redesign Layout
The five-zone structure maps directly to the historical collapse sequence:

- **Zone A** presents the surviving skeleton of public order.
- **Zone B** exposes administrative breakdown and contested utilities.
- **Zone C** demonstrates militarized privatization of movement.
- **Zone D** reveals ideological consolidation under ritual authority.
- **Zone E** confronts the preserved regnal command core that caused and perpetuates the crisis.

This alignment ensures progression is not only mechanical (harder mobs, deeper rooms) but epistemic: each zone teaches players what the previous zone could not fully explain.

### 15) The Obelisk Alphabet and Its Descendants

The Twelve Obelisks of Zone D are not merely decorative monuments. Each obelisk face carries a distinct glyph from the **Measure Script**, the oldest known writing system in the desert — predating even the Salt Ledger tablets. Measure Script was not alphabetic in the conventional sense. Each symbol encoded a compound meaning: a direction, a time of day, a water volume, and a legal status. A single glyph could mean "north-facing, morning, three measures, unrestricted" or "east-facing, dusk, half-measure, taxed."

This combinatorial writing system explains why the desert's bureaucratic culture was so obsessively precise: their language literally could not separate geography from time from resource allocation from law. To speak was to legislate. To inscribe was to adjudicate.

When the system collapsed, different successor cultures inherited fragments of Measure Script:
- The **Synod** preserved the liturgical-legal glyphs and abandoned the directional-temporal ones, producing a ceremonial script that sounds authoritative but has lost its navigational function.
- **Oasis water courts** preserved the volumetric-legal glyphs, retaining practical water arbitration capability but losing the astronomical timing layer.
- **Midgaard archivists** transliterated the script into their own alphabetic system, preserving the shapes but losing the combinatorial semantics. Their "translations" of obelisk inscriptions are technically accurate and functionally meaningless.
- **Kowloon's Jade Clerks** independently reconstructed parts of the volumetric-legal layer by comparing oasis documents with trade manifests, producing the closest modern approximation of functional Measure Script — though even they admit their version is a simplified dialect.

Room and object descriptions in Zone D should treat the obelisk inscriptions as partially legible: players can make out fragments, but the full compound meanings remain tantalizingly incomplete. This creates a natural motivation to explore both the Eastern Desert and connected areas for complementary inscription fragments.

### 16) Desert Ecology Before and After Collapse

The Eastern Desert is not a natural wasteland. It was engineered into aridity by the combined failures of the Iseth Reach diversion, the Ninth Meridian rite, and the subsequent abandonment of cistern maintenance. Understanding this unnatural ecology enriches room descriptions across all five zones.

**Pre-collapse ecology** (evidence visible in Zone B cistern ruins and Zone A charter stones):
- The desert supported seasonal grazing on its margins, with controlled burn-cycles managed by Keeper survey crews.
- Cistern overflow channels fed small garden plots around waystation nodes. Remnant root systems and dried irrigation channels should appear in cistern room descriptions.
- Migratory bird populations used the road obelisks as navigation waypoints; modern carrion kites in Zone C are the degenerate descendants of these flocks, their migration patterns collapsed into circular scavenging routes.

**Post-collapse ecology** (visible across all zones):
- Glass scorpions evolved their reflective carapaces in response to intensified solar radiation after the canopy of managed scrubland was destroyed. Their glass-like shells are not decorative; they are a survival adaptation to an artificially harsh environment.
- Ash adders colonized abandoned cistern pipes, exploiting the thermal gradient between hot surface sand and cooler underground masonry. Their presence in Zone B rooms indicates active cistern remnants — if the pipes were fully dead, the adders would not be there.
- Dune stalkers (large pack predators) occupy territory that was once controlled grazing land. Their aggressive territorial behavior reflects compressed hunting ranges, not natural savagery. In ecological terms, they are starving.
- The sand itself is drier and finer than it was before the Iseth Reach dried up. Older rock layers in Zone D and E show coarser, moisture-retaining sediment. The sirocco storms of Zone C are more violent than they would be in a naturally arid desert because the engineering failure removed the moisture-retention layer that once dampened wind energy.

### 17) The Astrolabe Tradition and Eclipse Science

The Eastern Desert's observatory-cistern complexes were not superstitious temples. They were precision instruments. The Keepers of Measure developed a remarkably accurate understanding of solar and lunar cycles, eclipse prediction, and seasonal drift — all driven by practical necessity: water allocation depended on knowing exactly when seasonal floods would arrive (or fail to arrive).

Key concepts for room and object descriptions:
- **Shadow rods** (found in Zone A charter stones): simple gnomon devices that tracked solar position for legal travel-window calculations. The shadows cast by obelisks were not metaphors; they were measurement tools.
- **Astrolabe glass** (found in Zone C and Zone D): polished obsidian lenses used to observe stellar positions without sun-blindness. The "blueglass" of Zone C's dune crest refers to ancient astrolabe fragments weathered into the sand.
- **Eclipse calendars** (found in Zone D obelisks and Zone E galleries): interlocking stone discs that predicted eclipse events decades in advance. The accuracy of these calendars was the basis for the Ninth Meridian Doctrine's fatal overconfidence — the astronomers correctly predicted the eclipse but incorrectly believed they could harness it.
- **The Ninth Sun**: not literally a ninth sun, but the astronomical term for the specific eclipse conjunction that occurs once every 847 years when nine celestial bodies (sun, moon, and seven bright stars the Keepers tracked) align along the eastern meridian. The Regent's court astrologers believed this alignment could be "anchored" by ritual, suspending drought cycles. The experiment failed catastrophically.

The Eclipse Lens Gallery (room 8592) should be described as a working instrument chamber, not a mystical shrine. The horror of Zone E is not that the Regent's court practiced magic — it is that they practiced science with insufficient understanding and refused to accept negative results.

### 18) Salt as Currency, Sacrament, and Weapon

Salt appears throughout the Eastern Desert in three distinct symbolic registers, and room/object/mob descriptions should maintain these distinctions:

1. **Salt as currency**: the Salt Ledger Kingdoms derived their name from the literal salt-crystal tablets used as both accounting media and commodity money. Salt had intrinsic value in the desert (preservation, water purification, wound treatment) and served as the unit of account for water-debt calculations. Finding a salt tablet in Zone B is like finding a banknote — it has a face value, a declared authority, and an expiration clause.

2. **Salt as sacrament**: the Jackal Synod adopted salt as a purification medium in their funerary rites. "Salt oaths" (Zone E, Chamber of Salt Oaths) are binding commitments sealed by consuming measured salt — the idea being that the oath literally enters the body and cannot be expelled. Synod toll-priests carry consecrated salt that they consider spiritually different from commercial salt, though chemically identical.

3. **Salt as weapon**: the Regent's final rite involved saturating the necropolis with magically concentrated brine, intending to preserve the court in a state of suspended animation. The rite succeeded in preserving the bodies but destroyed the life-supporting moisture in the surrounding rock. The "salt-burn curses" reported by oasis healers are not magical afflictions — they are chemical injuries from exposure to hyper-concentrated brine that still seeps from Zone E into the water table.

### 19) The Caravan Charter System in Detail

Zone A rooms should reflect a specific and detailed charter system, not generic "trade route" flavor. The charter system worked as follows:

- **Charter stones** (physical markers at regular intervals along the road) bore three types of inscription: distance to next waystation, current toll rate (updated seasonally by Keeper survey crews), and legal witness seals of the last three caravans to pass.
- **Convoy formation rules**: caravans were required to maintain minimum spacing (to prevent well-depletion at waystations) and maximum group size (to prevent any single convoy from monopolizing cistern capacity). Violators were fined in water-rights, the most severe penalty short of exile.
- **Ration stamps**: each caravan member carried a ceramic token stamped with their daily water allotment. Tokens were non-transferable. Lost tokens could not be replaced until the next waystation, where a Keeper clerk would issue a replacement after identity verification.
- **Night-travel prohibition**: the charter system prohibited road travel between dusk and dawn. This was not superstition — the obelisk shadow-measurement system that regulated traffic was useless at night, and unregulated night travel caused accidents, well-contamination, and toll evasion. The prohibition collapsed when the Synod began enforcing its own toll schedule, which operated on a ritual calendar rather than a solar one.

Modern caravan remnants in Zone A should show the bones of this system: damaged charter stones with unreadable toll rates, discarded ration tokens half-buried in sand, and convoy spacing markers that no one observes anymore.

### 20) The Regent's Court: Personnel and Protocol

The Last Sand Regent was not a solitary tyrant but the apex of a fully staffed court that continued functioning long after it should have dissolved. Understanding the court's personnel explains the mob ecology of Zone E:

- **Oathbound sentinels**: the Regent's personal guard, sworn to protect the court until released by the Regent's own voice. Since the Regent never died (only transformed), the sentinels were never released. They have stood at their posts for centuries, their bodies preserved by the same brine saturation that destroyed the surrounding ecology. They do not attack out of malice but out of protocol compliance — they are executing standing orders that have never been countermanded.

- **Salt mummies**: court bureaucrats (clerks, archivists, tax assessors) who were preserved by the eclipse rite alongside the Regent. Unlike the sentinels, who retained their martial training, the mummies retained their administrative programming. They may attempt to "process" intruders as if they were caravan operators arriving for toll assessment — demanding documents, inspecting goods, and issuing citations before attacking when the proper paperwork is not produced.

- **Eclipse wardens**: the court's astrologer-priests, who operated the observatory instruments and conducted the Ninth Meridian rite. They are the most dangerous non-boss enemies in Zone E because they retain both martial capability and casting ability. Their spells are not random offensive magic but targeted applications of the same eclipse-manipulation techniques that destroyed the desert — compressed drought, accelerated desiccation, and localized solar intensification.

- **The Last Sand Regent**: the policy engine at the center. The Regent does not "attack" in the conventional sense. It issues decrees. Each decree translates into a combat effect: "All unauthorized water consumption is punishable by desiccation" manifests as a drain spell. "The ninth sun requires tribute" manifests as a solar damage aura. The Regent believes it is governing, not fighting. This distinction should be reflected in combat messaging and mob bark text.

### 21) Seasonal and Diurnal Variation (Implementation Notes)

While the area engine does not natively support time-of-day room description variants, the plan should note thematic intentions for future enhancement:

- **Dawn/dusk rooms**: Zone A charter stones and Zone D obelisks are most legible during the shadow-measurement hours (dawn and dusk). Room descriptions should reference this, encouraging players to imagine the space at different times.
- **Noon rooms**: Zone C storm rooms are most dangerous at midday when heat shimmer peaks. The "mirage war-zone" concept means that visibility and navigation are worst during high sun.
- **Night rooms**: Zone E's eclipse galleries were designed to operate at night when stellar observations were possible. The necropolis is thematically a "night place" even though the game engine treats it as constant.
- **Seasonal markers**: inscription fragments should reference the three seasons of the old desert calendar — **Flood** (when the Iseth Reach ran), **Measure** (the dry productive season when surveys and construction occurred), and **Ash** (the dormant season when all but emergency travel ceased). These seasons no longer correspond to actual climate variation, but the Synod still observes them ritually.

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
