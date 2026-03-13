# Area Plan: The Southern Oasis (Spec-Aligned Rewrite)

## Overview

**Area Name:** `@@lThe Southern Oasis@@N`
**File Name:** `the_southern_oasis.are`
**Keyword:** `the southern oasis`
**Level Range:** 50-70 (`I 50 70`)
**Vnum Range (fixed):** **9800-9999** (`V 9800 9999`)
**Room Count Target:** 200/200 vnums used
**Design Goal:** Replace the current repetitive "future pyramid road" lane pattern and rebuild the area as an active civic-military basin linked to southern pyramid expeditions and regional trade. The Southern Oasis is the fortified hinge between oasis civil life and pyramid frontier risk — an armed staging ground, not a patrol template.

This plan keeps the existing vnum envelope exactly as assigned and fully reworks room flow, encounter identity, objectization, and narrative progression.

---

## Color Theme (Multi-Color, Lore-Driven)

- **Basin-blue (`@@l`)**: living water, sluice flow, bastion authority, warden insignia, cool shade.
- **Sun-gold (`@@y`)**: desert heat, caravan lanterns, trade gold, permit stamps, sunstruck masonry.
- **Earth-brown (`@@b`)**: dried mud walls, embalmer resin, worn sandstone, causeway gravel.
- **Cult-purple (`@@p`)**: Tribunal Splinter rites, funerary wards, death-claim rhetoric, curse-verdict glow.
- **Threat-red (`@@R`)**: boss foreshadowing, blood altars, military urgency, casualty wagons.
- **Reset (`@@N`)** after all colorized strings.

Theme rule compliance for implementation:
- No `@@k` thematic usage.
- No background codes (`@@0`-`@@7`) and no flashing (`@@f`).

---

## Design Pillars

1. **Living city-edge oasis, not a patrol template.** Districts need social, logistical, and ritual variance.
2. **Southward pressure from pyramid frontier.** The area should feel like a staging ground for dangerous deeper content.
3. **Water and law are contested institutions.** The conflict is political first, then violent.
4. **Readable traversal.** Main spine + lateral loops + optional hazard underroutes.

---

## Lore Alignment Targets

- **Great Oasis:** Southern Oasis is the militarized sister-node responsible for convoy hardening and ration enforcement. Legal-water precedent flows south from the Great Oasis Water Court, but Southern Oasis interprets those rules under emergency conditions.
- **Eastern Desert:** Receives caravans and legal traditions from eastern charter roads. Charter stone conventions, ration stamp vocabulary, and Keeper-era measurement motifs appear on approach routes and in administrative buildings.
- **Pyramids (stronger than Northern Oasis but still secondary):** Strong logistical connection to southern pyramid expeditions. The Southern Pyramid's Moon Sovereign theology bleeds into the Embalmer's Yard and Tribunal Splinter rhetoric. Expedition casualties return through the South Survey Gate bearing pyramid-origin curse residue. Avoid full tomb takeover of area identity.
- **Northern Oasis:** Provides intake verification and audited manifests that Southern authorities convert into convoy-ready logistics. The two oases form the twin administrative poles of the Oasis-Pyramid Corridor.
- **Midgaard:** Present via salvage contracts, scholars, and market demand for authenticated relics. Midgaard-funded relic appraisers operate in the Canopy Bazaar.

---

## Lore

See `southern_oasis_lore.md` for extracted lore.

---

## `#AREA` Header Plan (spec-compliant)

All directives per `area_file_spec.md` section 3:

- `Q 16` — mandatory area version; no other value is valid
- `K the southern oasis~` — keyword, `~`-terminated
- `L @@W(@@l50 70@@W)@@N~` — white parens, blue level numbers, white close paren, reset, `~`-terminated
- `N <allocate at implementation time>` — area number; must be confirmed unused across all loaded area files
- `I 50 70` — min/max level; controls mob-level gating and area matching
- `V 9800 9999` — full 200-vnum envelope; all rooms, mobs, and objects must stay within this range
- `F 15` — reset frequency in minutes
- `U @@lwater drums echo through the Southern Oasis as convoy lanterns flare along the causeway@@N~` — reset message; no double-newlines, `~`-terminated
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

Target ~200 rooms in six district sets, using all vnums 9800-9999:

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

- **Outdoor causeway/bazaar rooms** (streets, markets, open-air plazas): `sector_type = city` (`1`)
- **Desert-edge approach rooms** (dune approaches, exterior fortifications): `sector_type = desert` (`10`)
- **Interior rooms** (sluice vaults, embalmer chambers, tribunal halls, sub-basin tunnels): `sector_type = inside` (`11`), `room_flags` includes `indoors` (`8`)
- **Boss rooms**: `room_flags` includes `no_mob` (`4`) per boss placement policy
- **Sub-Basin Tunnel rooms**: `room_flags` includes `dark` (`1`) for underground darkness
- **Sluice Bastion military rooms**: `room_flags` includes `no_mob` (`4`) where sentinel mobs are stationed to prevent wandering mobs from entering
- **No rooms use `ROOM_MAZE`** (`524288`); this area does not contain mazes

### Vnum Allocation Policy

Per `area_file_spec.md` section 13.1:

- Room vnums must be assigned in ascending sequential order with no gaps.
- All 200 room vnums (9800-9999) must be used; fill every available room-vnum slot.

### District 1: Lantern Causeway (9800-9834, 35 rooms)

Entry spine and external connectors. Convoy choke points, permit checks, caravan camp edges.

- Sector: `city` (`1`) for paved causeway rooms; `desert` (`10`) for outer approach dunes.
- Key rooms:
  - **9800** Western Causeway Gate (important room, 5+ sentence description; external connector to Great Oasis routes).
  - **9808** Lantern Inspection Post (convoy permit checkpoint; important room, 5+ sentence description).
  - **9815** Caravan Camp Margin (hub connecting to Bazaar and Bastion districts; important room, 5+ sentence description).
  - **9825** Eastern Approach Road (external connector toward Eastern Desert charter routes).
  - **9834** Southern Causeway Fork (transition toward South Survey Gate; important room, 5+ sentence description).

### District 2: Canopy Bazaar (9835-9869, 35 rooms)

Social and economic hub. Brokers, quartermasters, relic appraisers, dispute clerks.

- Sector: `city` (`1`) throughout.
- Key rooms:
  - **9835** Bazaar Entrance Arch (important room, 5+ sentence description).
  - **9842** Quartermaster's Exchange (trade hub; important room with `E` extra descriptions for posted manifests).
  - **9850** Relic Appraisal Court (lore room; important room, 5+ sentence description with chained `E` extra descriptions for examined relics).
  - **9858** Dispute Clerks' Gallery (civil arbitration hall; inside sector).
  - **9869** Bazaar Rear Gate (connects to Sluice Bastion).

### District 3: Sluice Bastion (9870-9899, 30 rooms)

Military-water infrastructure. Gatehouses, pump works, ration locks, defensive terraces.

- Sector: mix of `city` (`1`) for outdoor fortification terraces and `inside` (`11`) for interior pump works and gatehouse rooms.
- Indoor rooms use `room_flags` including `indoors` (`8`).
- Key rooms:
  - **9870** Bastion Outer Gate (important room, 5+ sentence description; named exit `^bastion gate` discoverable in description).
  - **9878** Pump Works Central (lore-critical engineering room; inside sector; important room, 5+ sentence description with `E` extra descriptions for sluice mechanisms).
  - **9885** Ration Lock Chamber (important room; inside sector).
  - **9892** Warden Command Terrace (important room, 5+ sentence description; outdoor sector).
  - **9899** Bastion Rear Passage (connects to Embalmer's Yard and Sub-Basin Tunnels).

### District 4: Embalmer's Yard Annex (9900-9924, 25 rooms)

Former preparation quarter repurposed for expedition dead. Introduces pyramid funerary bleed-through without becoming a tomb zone.

- Sector: `inside` (`11`) throughout; `room_flags` includes `indoors` (`8`).
- Key rooms:
  - **9900** Embalmer's Yard Gate (important room, 5+ sentence description; named exit `^yard gate` discoverable in description).
  - **9906** Preparation Hall (lore room with embalming motifs from Southern Pyramid tradition; important room, 5+ sentence description).
  - **9912** Casualty Ledger Archive (chained `E` extra descriptions for expedition death records and pyramid-origin curse notations).
  - **9918** Mortuary Circle Sanctum (Tribunal Splinter secondary boss room; `room_flags` includes `no_mob`; important room, 5+ sentence description).
  - **9924** Sealed Descent (connects to Sub-Basin Tunnels via locked door).

### District 5: Sub-Basin Tunnels (9925-9959, 35 rooms)

Smuggler channels, sink chambers, hostile fauna/cult ambushes.

- Sector: `inside` (`11`) throughout; `room_flags` includes `indoors` (`8`) and `dark` (`1`).
- Key rooms:
  - **9925** Tunnel Access Shaft (important room, 5+ sentence description; vertical connector via D4 up / D5 down exits).
  - **9935** Smuggler's Cache (loot room with contraband relic fragments).
  - **9942** Sink Chamber (hazard room with water/collapse danger; important room, 5+ sentence description).
  - **9950** Cult Ambush Junction (Tribunal Splinter patrol hub).
  - **9959** Deep Basin Access (connects to South Survey Gate district).

### District 6: South Survey Gate (9960-9999, 40 rooms)

Forward command for pyramid-road claims and legal jurisdiction conflict. Boss zone and southern transition.

- Sector: mix of `city` (`1`) for military plaza rooms and `inside` (`11`) for command chambers.
- Key rooms:
  - **9960** Survey Gate Approach (important room, 5+ sentence description; transition from tunnels to military zone).
  - **9970** Forward Command Hall (important room, 5+ sentence description; inside sector; named exit `^command hall` discoverable).
  - **9980** Jurisdiction Court (lore-critical room where water-law and pyramid-claim doctrines collide; important room, 5+ sentence description with chained `E` extra descriptions).
  - **9990** Marshal's Redoubt (pre-boss staging; important room, 5+ sentence description).
  - **9995** South Survey Gate (primary boss room; `room_flags` includes `no_mob`; important room, 5+ sentence description).
  - **9999** Southern Road (external connector toward Southern Pyramid approach routes; important room, 5+ sentence description).

### Connectivity Principles

Per `area_file_spec.md` section 8 directional traversal constraints:

- **All exits must be bi-directional**: if room A connects to room B through direction X, room B must provide the opposite-direction exit back to room A. No one-way exits (this area is not a maze).
- **No directional loops**: repeated movement in the same direction must not return to a previously visited room. The spec prohibits room connection loops (e.g., `a -> b -> c -> a`) unless all rooms in the set are flagged `ROOM_MAZE`.
- **Hub-and-spoke structure**: the plan uses hub rooms at district transitions (Caravan Camp Margin 9815, Bastion Outer Gate 9870, Embalmer's Yard Gate 9900, Survey Gate Approach 9960) to provide route variety without circular loops.
- **Multiple traversal options between districts**: at least 2 branching paths connect each pair of adjacent districts, providing route variety without circular loops.
- **External exits preserved** where legacy worldgraph depends on them, but contextualized via renamed rooms/exits with bi-directional counterparts.
- **Door usage** limited to bastion gates, vault shutters, and embalmer seals. Every door exit must have `EX_ISDOOR` (`1`) set in `<locks>`. Door initial state (open/closed/locked) is authored through `#RESETS` command `D`, not through room exit `<locks>` bits. Any locked door (`D` state `2`) must have `<key_vnum>` set to a valid key object vnum, and that key object must exist in `#OBJECTS`.
- **Named exits** use `^` prefix on keywords (e.g., `^bastion gate~`, `^yard gate~`, `^command hall~`) so movement messaging treats them as standalone noun phrases. Each named exit keyword must appear in the room's `<description>~`, an object in the room, or an `E` extra description.

---

## Factions and Encounter Identity

### Mob Vnum Strategy

Per `area_file_spec.md` section 13.1:

- Mob vnums must be assigned in ascending sequential order with no gaps, starting from 9800.
- Target composition: ~50 mob templates (vnums 9800-9849).

### Mobile Spec Compliance (all mobs)

Per `area_file_spec.md` sections 4 and 5.4:

- **All mobs** must have `act` flags including `is_npc` (`1`) and `stay_area` (`64`).
- **Sentinel mobs** (guards, shopkeepers, stationary NPCs): add `sentinel` (`2`) to `act` flags.
- **Boss mobs**: must have `act` flags including `sentinel` (`2`) and `boss` (`67108864`), and must be placed only in rooms flagged `no_mob` (`4`).
- **Strong non-boss mobs** (elite patrol leaders, veteran wardens): must have `act` flags including `solo` (`33554432`).
- **Aggressive mobs** (hostile cult members, smugglers, tunnel fauna): set `aggressive` (`32`) in `act` flags.
- **`invasion`** (`536870912`) must never be set in area files; it is runtime-only.
- Mobile `long_descr` must be exactly one text line followed by a newline and a `~`-only line. Multi-line `long_descr` is never valid under any circumstances.
- Mobile `description` must end with exactly one trailing newline before the terminating `~`.
- Mobile `player_name` (keyword), `short_descr`, `long_descr`, and `description` are all `~`-terminated.
- Within a sub-region of the area, mobs must either be flagged `sentinel` or the sub-region must be enclosed by rooms flagged `no_mob`, so mobs cannot wander out of their sub-region.

### Mobile Extension Lines

Per `area_file_spec.md` sections 4.1-5.5:

- `!` extension: `<class> <clan> <race> <position> <skills> <cast> <def>` — used for combat-capable mobs. Skill bitvectors follow the level-gated floor policy tables (melee/hybrid/caster profiles).
- `|` extension: `<strong_magic> <weak_magic> <race_mods> <power_skills> <power_cast> <resist> <suscept>` — used for elemental affinities. Desert oasis mobs should generally have `resist` including `fire` (`128`) and `suscept` including `water` (`64`) where thematically appropriate (e.g., fire-resistant desert fauna, not water-using sluice workers).
- `+` extension: combat modifier tuning for significant encounters.
- `l`/`L` extensions: loot tables. `loot_amount` is percent-based (see spec). Items in loot tables must have `ITEM_LOOT` set in `extra_flags`.

### Faction Roster

1. **Bastion Wardens** (order/military faction)
   - Sluice guards, convoy marshals, ration enforcers, pump engineers, wall sentries.
   - `act`: `is_npc` + `stay_area` + `sentinel` (for stationary guards/engineers). Non-aggressive.
   - Level range: 50-65.
   - Archetype: melee profile (guards) and hybrid profile (marshals with light defensive casting).

2. **Bazaar Syndics** (neutral-pragmatic faction)
   - Permit clerks, relic appraisers, trade brokers, dispute mediators, quartermasters.
   - `act`: `is_npc` + `stay_area` + `sentinel`. Non-aggressive.
   - Level range: 50-55.
   - Archetype: hybrid profile (light arms, some utility casting).

3. **Expedition Mortuary Circle** (hostile-neutral faction)
   - Embalmers, corpse handlers, funerary ledger scribes, preservation ritualists.
   - `act`: `is_npc` + `stay_area`. Embalmers are `sentinel` and non-aggressive; preservation ritualists are `aggressive` in restricted sanctum areas.
   - Level range: 55-65.
   - Archetype: caster profile (ritual casting, preservation magic).
   - `affected_by`: `detect_hidden` (`32`) for preservation ritualists.

4. **Jackal Tribunal Splinter** (hostile cult faction)
   - Verdict-callers, oath-breaker hunters, death-claim advocates, curse scribes.
   - `act`: `is_npc` + `stay_area` + `aggressive`. Verdict-callers add `sentinel` (stationed at ritual nodes).
   - Level range: 55-70 (oath-breaker hunters low, curse scribes high).
   - Archetype: hybrid profile (legal-ritual casting + melee). Curse scribes use caster profile.
   - `affected_by`: death-claim advocates get `detect_hidden` (`32`); curse scribes get `detect_magic` (`16`).

5. **Sub-Basin Fauna and Smugglers** (hostile)
   - Tunnel scorpions, basin vipers, smuggler operatives, contraband runners.
   - `act`: `is_npc` + `stay_area` + `aggressive`. Fauna add `no_mind` (`262144`).
   - Level range: 50-60.
   - Archetype: melee profile (fauna), hybrid profile (smugglers).

### Boss Ladder

All boss mobs: `act` includes `is_npc` + `stay_area` + `sentinel` + `boss` + `aggressive` + `no_flee`. Placed only in rooms flagged `no_mob`.

- **Secondary Boss (9918, District 4):** Tribunal Splinter Adjudicator. Level 65. Caster profile with curse-verdict mechanics. Assigned `spec_cast_cleric` in `#SPECIALS`. `affected_by`: `detect_invis` (`8`), `detect_hidden` (`32`). Items dropped must include `ITEM_BOSS` flag. Loot table (`l`/`L`) used for boss drops.
- **Primary Boss (9995, District 6):** Survey Gate Marshal. Level 70. Hybrid profile with strong melee and defensive casting. `act` adds `remember` (`8`). Assigned `spec_cast_judge` in `#SPECIALS`. `affected_by`: `detect_invis` (`8`), `infrared` (`512`). Items dropped must include `ITEM_BOSS` flag. Loot table (`l`/`L`) used for boss drops.

---

## Object Redesign Plan

### Object Vnum Strategy

Per `area_file_spec.md` section 13.1:

- Object vnums must be assigned in ascending sequential order with no gaps, starting from 9800.
- Expand through contiguous sequential slots (suggested through **9870**) while staying within the 9800-9999 vnum envelope.
- Target composition: ~71 object templates (vnums 9800-9870).
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
- **Two-handed weapons**: if a weapon's identity clearly indicates a two-handed archetype (e.g., "great axe", "convoy pike"), it must include `ITEM_TWO_HANDED` (`2147483648`) in `extra_flags`.
- **Bucklers**: `item_type = ITEM_ARMOR`, `wear_flags` includes `hold`, `extra_flags` includes `ITEM_BUCKLER` (`268435456`).
- **Shields**: `item_type = ITEM_ARMOR`, `wear_flags` includes `hold`.

### Item Families

1. **Convoy Garrison Set** (levels 50-60, military gear)
   - Warden helms (`head`, weight 11-15 tank), convoy cloaks (`about`, weight 11-15 tank), sluice bucklers (`hold` + `ITEM_BUCKLER`, weight 11-15 tank), ration enforcement batons (`hold` + `ITEM_WEAPON`, `value3 = 7` pound, weight 6-10 melee).
   - `L` level lines: 50-60 range.

2. **Bazaar Trade Goods** (levels 50-55, utility/caster)
   - Appraiser's loupe (`hold`, `ITEM_TREASURE`, weight 1-5 caster), permit seal rings (`finger`, weight 1-5 caster), manifest scrolls (`hold`, `ITEM_TREASURE`, weight 1-5 caster), syndic veils (`face`, weight 1-5 caster).
   - `L` level lines: 50-55 range.

3. **Mortuary Circle Relics** (levels 55-65, cult-themed)
   - Embalmer's wrapping tools (`hold`, `ITEM_TREASURE`, weight 1-5 caster), funerary masks (`face`, weight 1-5 caster), preservation censers (`hold`, `ITEM_TREASURE`, weight 1-5 caster), corpse-handler's hooks (`hold` + `ITEM_WEAPON`, `value3 = 11` pierce, weight 6-10 melee).
   - `L` level lines: 55-65 range.

4. **Tribunal Splinter Arms** (levels 55-70, hostile-cult)
   - Verdict blades (`hold` + `ITEM_WEAPON`, `value3 = 3` slash, weight 6-10 melee), curse-inscribed bracers (`wrist`, weight 11-15 tank), oath-breaker's collar (`neck`, weight 1-5 caster), death-claim staves (`hold` + `ITEM_WEAPON` + `ITEM_TWO_HANDED`, `value3 = 7` pound, weight 1-5 caster).
   - `L` level lines: 55-70 range.

5. **Survey Gate Command Regalia** (levels 65-70, boss drops)
   - Marshal's authority seal (`finger`, weight 1-5 caster), survey gate mantle (`shoulders`, weight 11-15 tank), jurisdiction blade (`hold` + `ITEM_WEAPON`, `value3 = 3` slash, weight 6-10 melee), ration doctrine codex (`hold`, `ITEM_TREASURE`, weight 1-5 caster).
   - Boss drops: must include `ITEM_BOSS` in `extra_flags`.
   - Loot-table drops: must include `ITEM_LOOT` in `extra_flags`.
   - `L` level lines: 65-70 range.

6. **Lore Objects / Quest Hooks** (various levels)
   - Water-court writs (`ITEM_TREASURE`, `hold`, weight 1-5), convoy manifests (`ITEM_TREASURE`, `hold`, weight 1-5), embalmer's ledger fragments (`ITEM_TREASURE`, `hold`, weight 1-5), bastion keys (`ITEM_KEY`, `hold`, weight 1-5).
   - Key objects: any key used as `<key_vnum>` for a locked door must exist in `#OBJECTS` with `item_type = ITEM_KEY` (`18`).

### Key Objects for Locked Doors

Per `area_file_spec.md` section 8.3: if any door is reset to locked (`D` state `2`), `<key_vnum>` must be set to a valid key object vnum, and that key object must exist in `#OBJECTS`. Plan the following key objects:

- **Bastion Gate Key** (for Bastion Outer Gate 9870 door): `ITEM_KEY`, `hold` + `take`, weight 1.
- **Embalmer's Seal Key** (for Embalmer's Yard Gate 9900 door): `ITEM_KEY`, `hold` + `take`, weight 1.
- **Sealed Descent Key** (for District 4-to-5 transition 9924 door): `ITEM_KEY`, `hold` + `take`, weight 1.
- **Command Hall Key** (for Forward Command Hall 9970 door): `ITEM_KEY`, `hold` + `take`, weight 1.

---

## `#RESETS` Plan (spec-compliant)

Per `area_file_spec.md` section 12:

- `#RESETS` must not contain blank lines.
- Reset commands reference valid vnums within the area's vnum envelope.

### Mob Resets (`M` command)

Format: `M 0 <mob_vnum> <limit> <room_vnum> <notes>`

- **District 1 (Lantern Causeway):** Reset Bastion Warden sentries at causeway checkpoints (9800, 9808, 9825). Reset convoy marshals at Caravan Camp Margin (9815). Limit 1-2 per mob.
- **District 2 (Canopy Bazaar):** Reset Bazaar Syndic NPCs at trade rooms (9842, 9850, 9858). Reset relic appraisers at Appraisal Court (9850). Limit 1 per mob.
- **District 3 (Sluice Bastion):** Reset Bastion Warden guards at gate and terrace rooms (9870, 9878, 9892). Reset pump engineers at Pump Works Central (9878). Limit 1 per mob.
- **District 4 (Embalmer's Yard):** Reset Mortuary Circle embalmers at preparation rooms (9906, 9912). Reset secondary boss Tribunal Splinter Adjudicator at 9918 (limit 1).
- **District 5 (Sub-Basin Tunnels):** Reset tunnel fauna across tunnel rooms (tunnel scorpions, basin vipers at 9930, 9935, 9942). Reset smuggler operatives at 9935, 9950. Limit 1-2 per mob.
- **District 6 (South Survey Gate):** Reset Tribunal Splinter hostile mobs at approach rooms (9960, 9970). Reset Bastion Warden elite guards at Marshal's Redoubt (9990). Reset primary boss Survey Gate Marshal at 9995 (limit 1).

### Object Resets (`O` command)

Format: `O 0 <obj_vnum> <limit> <room_vnum> <notes>`

- Place lore objects (water-court writs, convoy manifests) in key lore rooms (9850, 9912, 9980) for atmospheric pickup.
- Place key objects near their corresponding doors or on gatekeeper mobs via `G`/`E` resets.

### Equipment Resets (`G`/`E` commands)

Format: `G 0 <obj_vnum> <limit>` (give to previous mob) / `E 0 <obj_vnum> <limit> <wear_loc>` (equip on previous mob)

- Equip Bastion Warden guards with warden helms and ration enforcement batons.
- Equip Tribunal Splinter mobs with verdict blades and curse-inscribed bracers.
- Equip boss mobs with thematic Survey Gate Command Regalia (items must include `ITEM_BOSS` flag).

### Door Resets (`D` command)

Format: `D 0 <room_vnum> <door_direction> <state>`

Per `area_file_spec.md` section 8.3: any exit set to closed or locked via `D` reset must have `EX_ISDOOR` (`1`) set in its room `D<door>` `<locks>` definition.

Planned door resets:

1. **Bastion Outer Gate (9870):** Direction appropriate to approach.
   - Room exit `<locks>`: `door` (`1`). `<key_vnum>`: vnum of Bastion Gate Key object.
   - `D` reset: `D 0 9870 <dir> 1` (closed, not locked — military gate, opened by guards).
   - Reverse exit must also have `door` bit set.

2. **Embalmer's Yard Gate (9900):** Direction appropriate to approach.
   - Room exit `<locks>`: `door` (`1`). `<key_vnum>`: vnum of Embalmer's Seal Key object.
   - `D` reset: `D 0 9900 <dir> 2` (locked).
   - Reverse exit must also have `door` bit set.
   - Key object (Embalmer's Seal Key) must exist in `#OBJECTS`.

3. **Sealed Descent (9924):** Direction down (D5) to Sub-Basin Tunnels.
   - Room exit `<locks>`: `door` (`1`). `<key_vnum>`: vnum of Sealed Descent Key object.
   - `D` reset: `D 0 9924 5 2` (locked).
   - Reverse exit on 9925 must also have `door` bit set.
   - Key object (Sealed Descent Key) must exist in `#OBJECTS`.

4. **Forward Command Hall (9970):** Direction into command hall.
   - Room exit `<locks>`: `door` (`1`). `<key_vnum>`: vnum of Command Hall Key object.
   - `D` reset: `D 0 9970 <dir> 2` (locked).
   - Reverse exit must also have `door` bit set.
   - Key object (Command Hall Key) must exist in `#OBJECTS`.

All locked doors (state `2`) have corresponding key objects in `#OBJECTS`. Keys can be placed on gatekeeper mobs via `G` resets or in adjacent rooms via `O` resets.

---

## `#SPECIALS` Plan

Per `area_file_spec.md` section 10:

- `spec_poison` assigned to basin viper mobs (tunnel venomous snake ecology).
- `spec_cast_judge` assigned to Survey Gate Marshal (primary boss; legal-military verdict casting).
- `spec_cast_cleric` assigned to Tribunal Splinter Adjudicator (secondary boss; curse-ritual defensive casting).
- `spec_cast_mage` assigned to Tribunal Splinter curse scribes (offensive arcane casting).
- No `spec_summon_*` specials (runtime-only, builder-prohibited).
- No `spec_keep*` specials (manual-assignment only).

---

## `#SHOPS` Plan

Per `area_file_spec.md` section 9:

- **Quartermaster's Exchange (9842):** Bazaar Syndic quartermaster mob sells general goods. `<keeper>` = quartermaster mob vnum, appropriate `<buy_type>` slots, reasonable profit margins, operating hours matching daytime trade.
- **Relic Appraisal Court (9850):** Relic appraiser mob buys/sells treasures. Similar shop configuration.

---

## Canonical Section Order

Per `area_file_spec.md` section 14, the final `.are` file should emit sections in this order:

1. `#AREA`
2. `#ROOMS`
3. `#MOBILES`
4. `#SCRIPTS` (if any inline scripts are used)
5. `#OBJECTS`
6. `#SHOPS`
7. `#RESETS`
8. `#SPECIALS`
9. `#OBJFUNS` (none planned for this area)
10. `#$` (end-of-file marker)

---

## Reset and Progression Plan

- **Early (Districts 1-2, 9800-9869):** Navigation and social exploration, caravan camp encounters, bazaar trade interactions, light combat with smugglers on periphery. Bastion Warden and Bazaar Syndic NPCs populate the scene. Level 50-55 content.
- **Mid (Districts 3-4, 9870-9924):** Military infrastructure exploration, Mortuary Circle encounters, Tribunal Splinter hostiles begin appearing. Locked doors gate progression. Level 55-65 content.
- **Late (Districts 5-6, 9925-9999):** Underground hazard traversal, cult ambushes, boss encounters, jurisdiction showdown. Tribunal Splinter and boss mobs. Level 65-70 content.

Loot profile:
- Water-law insignia, convoy gear, anti-heat kit, authenticated relic fragments.
- Controlled drop rate for pyramid-linked artifacts to avoid overshadowing dedicated pyramid areas.

---

## Implementation Checklist

### Rooms
- [ ] Populate all 200 room vnums (9800-9999) in ascending sequential order with no gaps.
- [ ] Write unique `<description>~` for every room (minimum 3 sentences; 5+ for important rooms).
- [ ] Assign correct `sector_type` (`city` for causeway/bazaar, `desert` for approach, `inside` for interiors) and `room_flags` per district requirements.
- [ ] Set `no_mob` on all boss rooms (9918, 9995).
- [ ] Set `dark` on Sub-Basin Tunnel rooms (9925-9959).
- [ ] Ensure all exits are bi-directional with no directional loops.
- [ ] Ensure all named exits use `^` prefix and are discoverable in room description, objects, or extra descriptions.
- [ ] Ensure all extra-description keyword chains are anchored in the room's main `<description>~`.
- [ ] Preserve required worldgraph external connectors while renaming and recontextualizing local rooms.

### Mobiles
- [ ] Write all ~50 mob templates (vnums 9800-9849) in ascending sequential order with no gaps.
- [ ] Ensure all mobs have `is_npc` + `stay_area` in `act` flags.
- [ ] Ensure boss mobs have `sentinel` + `boss` + `no_flee` and are placed only in `no_mob` rooms.
- [ ] Ensure strong non-boss mobs have `solo` flag.
- [ ] Ensure all `long_descr` are exactly one text line + newline + `~`-only line.
- [ ] Ensure all `description` end with exactly one trailing newline before `~`.
- [ ] Assign `!`/`|`/`+` extension lines for combat-capable mobs with appropriate skill profiles.
- [ ] Set `l`/`L` loot tables on boss mobs with valid object vnums; loot items have `ITEM_LOOT`.

### Objects
- [ ] Write all ~71 object templates (vnums 9800-9870) in ascending sequential order with no gaps.
- [ ] Ensure unique `<name>~` for every object in the area.
- [ ] Ensure all objects include `ITEM_TAKE` in `wear_flags`.
- [ ] Ensure no objects include `ITEM_WEAR_CLAN_COLORS`.
- [ ] Ensure no objects set `ITEM_GENERATED`.
- [ ] Ensure boss-drop items include `ITEM_BOSS` flag.
- [ ] Ensure loot-table items include `ITEM_LOOT` flag.
- [ ] Assign correct weight ranges (1-5 caster, 6-10 melee, 11-15 tank).
- [ ] Ensure weapon `value3` is thematically consistent; no `value3=0` without `ITEM_FIST`.
- [ ] Ensure two-handed weapons have `ITEM_TWO_HANDED` flag.
- [ ] Create all four key objects (Bastion Gate Key, Embalmer's Seal Key, Sealed Descent Key, Command Hall Key).

### Resets
- [ ] Write `M` resets for all planned mob placements with appropriate limits.
- [ ] Write `G`/`E` resets for mob equipment.
- [ ] Write `O` resets for atmospheric/lore objects.
- [ ] Write `D` resets for all four doors with appropriate states.
- [ ] Verify every locked door has `EX_ISDOOR` in room exit `<locks>` and valid `<key_vnum>`.

### Specials
- [ ] Assign `spec_poison` to basin viper mobs.
- [ ] Assign `spec_cast_judge` to Survey Gate Marshal.
- [ ] Assign `spec_cast_cleric` to Tribunal Splinter Adjudicator.
- [ ] Assign `spec_cast_mage` to Tribunal Splinter curse scribes.

### Color Theme
- [ ] Apply color theme consistently across all room, mob, and object authored strings.
- [ ] Verify no `@@k` thematic usage, no background codes (`@@0`-`@@7`), no flashing (`@@f`).
- [ ] Verify `@@N` reset after all colorized strings.

### Final Validation
- [ ] Validate complete area file format against `docs/area_file_spec.md`.
- [ ] Verify all vnums within `V 9800 9999` envelope.
- [ ] Verify `#ROOMS` and `#MOBILES` and `#OBJECTS` each terminate with `#0`.
- [ ] Verify `#RESETS`, `#SPECIALS` each terminate with `S`.
- [ ] Verify canonical section order.
- [ ] Run `make unit-tests` from `src/` to validate.
