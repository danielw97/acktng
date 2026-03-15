# Area Plan: Roc Road

## Overview
- **Area Name:** `@@bRoc Road @@Wand the @@GForest Path@@N`
- **File Name:** `rocroad.are`
- **Keyword:** `rocroad`
- **Intended Levels:** 10-120 (high-traffic overland corridor with scalable threats)
- **Vnum Range:** `1117-1215`
- **Vnum Count:** 99
- **Current Room Utilization:** 99/99 (all vnums in-range already assigned to rooms)
- **Reset Rate:** 10
- **Role in World:** Primary overland artery connecting the **Midgaard West Gate (east terminus)** to the **Kiess East Gate (west terminus)**, with multiple wilderness and legacy-route branch links.

## Lore Reference
See `rocroad_lore.md` for the full narrative canon extraction and expansion.

## Color Theme (multi-color, lore-aligned)
- **Roadstone and imperial milework:** `@@W` (white)
- **Waybanners, route authority, and watch sigils:** `@@B` (blue)
- **Canopy, verge growth, and druidic remnants:** `@@G` (green)
- **Merchant marks, coin-trade, and convoy lanterns:** `@@y` (yellow)
- **Danger cues (raids, blood, warning runes):** `@@R` (red)
- **River/sea mist, ferry marks, and ford glyphs:** `@@a` (light cyan)
- **Ancient/forgotten strata (ruins, old empire traces):** `@@p` (purple)
- **Always reset:** `@@N`

## #AREA Header Plan (spec-compliant)
- `Q 16`
- `K rocroad~`
- `L @@W(@@a10 120@@W)@@N~`
- `N 16` (retain existing area index unless reassigned globally)
- `I 10 120`
- `V 1117 1215`
- `X 0`
- `F 10`
- `U @@Wwagon wheels rattle and @@ytrade bells@@W ring along Roc Road@@N~`
- `T`
- `S`
- `M`
- `O Virant~` (required owner policy)

## Spec-First Constraints (from `area_file_spec.md`)

### Area Header
1. `Q 16` directive is mandatory.
2. Owner line must be `O Virant~` (area header owner policy). The existing file uses `O stephen~`; this must be updated.
3. `V 1117 1215` defines the vnum envelope; all authored rooms, mobiles, and objects must stay inside this range.
4. Remove `R stephen~` and `W stephen~` ACL directives from the legacy header; set ACLs at implementation time if needed.

### Vnum Allocation
5. Vnums for rooms, mobiles, and objects must be assigned in ascending sequential order with no gaps.
6. Fill existing gaps in the vnum range before assigning new higher vnums.
7. For rooms, attempt to use all room vnums in the assigned range (fill every available slot where practical). The existing file uses all 99 vnums but in non-sequential order; the rebuild must assign vnums `1117-1215` in ascending order.

### String and Description Rules
8. All text fields terminated by `~`. Strings must not contain back-to-back newlines (`\n\n`).
9. Vnums must never appear in any in-world description text (room descriptions, mob descriptions, object descriptions, extra descriptions, exit descriptions).
10. Mobile `long_descr` must be exactly one text line followed by a newline and a line containing only `~`. Multi-line `long_descr` is never valid.
11. Mobile `description` must end with exactly one newline immediately before the terminating `~`.
12. Object extra-description `<description>` must end with exactly one newline before `~`.
13. Room `<description>~` must end with exactly one trailing newline before `~`.

### Color Policy
14. Use intentional inline color coding with the defined Roc Road multi-color palette (see Color Theme section); avoid forbidden thematic black (`@@k`).
15. Do not use background color codes (`@@0`-`@@7`) or flashing (`@@f`) unless explicitly added by a human author.

### Room Constraints
16. Each room description must contain at least 3 sentences; important rooms (band terminus gates, crossroads, boss encounter rooms, major landmarks) must contain at least 5 sentences.
17. Each room description must be unique (no reuse of identical descriptions across rooms).
18. Exits must be bi-directional: if room A connects to room B via direction X, room B must provide the opposite-direction exit back to A (except in mazes). External links to other areas are exempt where the other area controls the return exit.
19. When a room uses a named exit (non-empty `<exit_keyword>`), that exit name must appear in the room's main description, an object that spawns in the room, or an `E` extra description.
20. Extra-description keywords must be discoverable: at least one keyword in any chain must appear in the room's main description.
21. Named exit keywords for door-style exits must be prefixed with `^` (e.g., `^iron gate`).
22. Any exit set to `closed` or `locked` via `#RESETS` command `D` must have `EX_ISDOOR` (`1`) set in its room `D<door>` definition.
23. If a door is reset to `locked` (state `2`), `<key_vnum>` must reference a valid key object, and that key must exist in `#OBJECTS`.
24. Rooms that are part of a maze must be flagged `ROOM_MAZE` (`524288`). Roc Road is not expected to contain mazes.
25. Boss mob rooms must be flagged `no_mob` (`4`).

### Mobile Constraints
26. All mobs must be flagged `stay_area` (`64`) so they do not wander out of Roc Road.
27. Within a sub-region (band), mobs must either be flagged `sentinel` or the sub-region must be enclosed by rooms flagged `no_mob`.
28. Boss mobs must be flagged `sentinel` (`2`) and `boss` (`67108864`) and placed only in rooms flagged `no_mob` (`4`).
29. Strong (non-boss) mobs must be flagged `solo` (`33554432`).
30. The `invasion` flag must never be set by builders; it is runtime-only.
31. The `aggressive` flag should be set only for mobs whose intended behavior is to initiate combat on sight (feral beasts, hostile bandits, revenants), not for neutral wayfolk, merchants, or non-hostile patrols.
32. Sentinel mobs (gate guards, bridge wardens, shrine keepers) should be flagged `sentinel` (`2`).

### Object Constraints
33. Every object must include `ITEM_TAKE` (`8388608`) in wear flags.
34. No object may include `ITEM_WEAR_CLAN_COLORS` (`16777216`).
35. Object `name`, `short_descr`, and `description` must be thematically consistent with the object's wear flags.
36. Object names must be unique within the area file (no duplicate item names).
37. Area-authored items must not define fixed stats; stats are generated at runtime.
38. Object weight encodes archetype: `1`-`5` caster, `6`-`10` melee, `11`-`15` tank.
39. `ITEM_GENERATED` (`1`) must not be set in area files; it is runtime-managed.
40. Items from boss mob loot tables must include `ITEM_BOSS` (`134217728`).
41. Items spawned from mob loot tables (`l`/`L` extension) must have `ITEM_LOOT` (`67108864`) set.
42. Weapons must have wear flags including both `hold` (`32768`) and `take` (`8388608`). Two-handed weapon names must include the `ITEM_TWO_HANDED` (`2147483648`) extra flag.
43. `value3 = 0` (`hit`) for weapons is only valid with the `ITEM_FIST` extra flag.

### Reset Constraints
44. Resets must reference valid vnums for the target type required by the command.
45. If a door is reset to `locked` via `D` state `2`, `<key_vnum>` must be set to a valid key object vnum (not `-1`), and that key object must exist in `#OBJECTS`.
46. `#RESETS` must not contain blank lines.

### Section Order
47. Follow canonical saver order: `#AREA`, `#ROOMS`, `#MOBILES`, `#OBJECTS`, `#SHOPS`, `#RESETS`, `#SPECIALS`, `#OBJFUNS`, `#$`.

### Specials Policy
48. Mob specials beginning with `spec_summon_` must never be set in area files (runtime-only).
49. Mob specials beginning with `spec_keep` may only be assigned manually by a human.

---

## External Link Inventory (existing connections to preserve)

The current `rocroad.are` has the following external exits that **must be preserved** in the rebuild (the room vnums they attach to will change as part of the sequential renumbering, but the connectivity must remain):

| Current Vnum | Direction | Destination Vnum | Destination Area | Notes |
|---|---|---|---|---|
| 1146 | east | 957 | Midgaard | **East terminus** — Midgaard West Gate link |
| 1150 | north | 3064 | Umbra Heartspire | Branch link north (close to Midgaard) |
| 1154 | down | 88 | External area | Vertical link (sub-road access) |
| 1156 | east | 2340 | *(removed — Forest of Confusion connects via Kiess north gate, not Roc Road)* | ~~Branch to Forest of Confusion~~ |
| 1158 | east | 3401 | External area | Branch link east |
| 1159 | north | 3434 | Kiess | **West terminus** — Kiess South Gate link |
| 1165 | north | 1206 | Roc Road (internal) | Stone path branch |
| 1166 | up | 7200 | External area | Vertical link (elevated access) |
| 1175 | north | 5083 | External sea area | Sea route link |
| 1207 | north | 4449 | External area | Building/structure link |

### New External Links (added in rebuild)

| New Room | Direction | Destination Vnum | Destination Area | Notes |
|---|---|---|---|---|
| *(Band A)* | north | 5181 | Cathedral of the Violet Eclipse | Branch link north (very close to Midgaard) |

**Rebuild rule:** Every preserved external link above must appear in the rebuilt area file, mapped to the new sequential vnum that replaces the original room. New links must also be implemented. If a connection requires a reciprocal exit in the destination area, coordinate with that area's file.

---

## Design Intent: Full Redo While Preserving Travel Function
The existing area already uses all room vnums and serves as a long transit chain. The rebuild keeps that macro function but completely replaces room identities, encounter ecology, objectization, and narrative flow so the route feels like a contested cultural frontier rather than generic wilderness.

### Core goals
1. **Keep all 99 room vnums active** and meaningful.
2. **Rebuild every room description and title** with stronger lore identity and route-stage progression.
3. **Introduce full mobile/object rosters** across the same vnum envelope.
4. **Tie route storytelling directly to Midgaard and Kiess history** (statecraft, trade law, military patrol doctrine, refugee memory, and frontier cult pressure).
5. **Maintain reliable traversal** for players who just need to travel, while adding optional side encounters, lore pockets, and mini-threat spikes.

## Spatial Model (99-room corridor with thematic bands)
Roc Road remains a longitudinal journey but is rebuilt as nine major bands plus connector micro-zones. All rooms use ascending sequential vnums `1117-1215` with no gaps.

### Band A — Midgaard Gate March (1117-1127, 11 rooms)
- **Sector types:** `city` (1) for the first 2 rooms nearest Midgaard gate; `field` (2) for the remaining road through settled farmland.
- **Room flags:** `0` for most; the east terminus room (1117) is an important room (5+ sentences).
- Stone-set roadway under Midgaard charter markers.
- Guard relays, tax cairns, and old civic inscriptions.
- Encounter emphasis: patrols, petitioners, minor bandit scouts, courier traffic.
- **External links:**
 - Room 1117 connects east to `957` (Midgaard West Gate). This is the canonical east terminus of Roc Road.
 - One room in this band connects north to `5181` (Cathedral of the Violet Eclipse). Very close to Midgaard — a shadow-liturgical cathedral visible from the road's first stretch.

### Band B — Banner Hills and Switchbacks (1128-1135, 8 rooms)
- **Sector types:** `hills` (4) for hill-climb rooms; `mountain` (5) for the high pass and switchback rooms.
- **Room flags:** `0` for most.
- Steeper terrain, signal poles, weather-exposed pass points.
- Visual language shifts from civic to frontier military.
- Encounter emphasis: rockfall hazards, hill raiders, watch outriders.

### Band C — River Ledger Crossing (1136-1143, 8 rooms)
- **Sector types:** `field` (2) for approach rooms; `water_swim` (6) for the ford room itself; `field` (2) for the far bank.
- **Room flags:** `0` for most.
- Ford/bridge complex with customs past and present layered together.
- Introduces merchant guild tension and smuggling subtext.
- Encounter emphasis: ferrymen, toll ghosts, river predators, contraband runners.

### Band D — Dustward Verge (1144-1150, 7 rooms)
- **Sector types:** `desert` (10) for arid stretches; `field` (2) for transitional rooms.
- **Room flags:** `0` for most.
- Drier transition where road maintenance degrades.
- Old milestone liturgy from pre-Kiess polities resurfaces.
- Encounter emphasis: scavengers, relic pickers, heat-maddened fauna.
- **External link:** Room at end of this band connects north to `3064` (Umbra Heartspire). The cathedral-fortress is visible from the road as a dark silhouette against the hills — close enough to Midgaard's jurisdiction that its original charter came from Midgaard's archive system.

### Band E — Crossroads Compact (1151-1159, 9 rooms)
- **Sector types:** `field` (2) for road rooms; `forest` (3) for rooms near the forest branch.
- **Room flags:** `0` for most; crossroads hub room is important (5+ sentences). Boss encounter room flagged `no_mob` (4).
- The major strategic intersection where branch routes diverge.
- Rebuilt as a political pressure node with evidence of overlapping jurisdiction.
- Encounter emphasis: diplomatic envoys, mercenary escorts, opportunist thieves.
- **External links preserved from this band:**
 - North to `3064` (Umbra Heartspire, from former 1150)
 - Down to `88` (vertical sub-road access, from former 1154)
 - East to `3401` (branch link east, from former 1158)
 - North to `3434` (Kiess South Gate, from former 1159) — this is the **canonical west terminus** of Roc Road.
- **Note:** The former `1156 east → 2340` (Forest of Confusion) link is **removed** in the rebuild. The Forest of Confusion connects via Kiess's north gate, not directly from Roc Road.

### Band F — Greenveil Spur (1160-1168, 9 rooms)
- **Sector types:** `forest` (3) for all rooms in the forest spur.
- **Room flags:** `0` for most; boss encounter room flagged `no_mob` (4).
- Forest-side path that once served as hidden troop and refugee movement line.
- Becomes a lore-rich optional path with druidic and exile traces.
- Encounter emphasis: foresters, displaced spirits, poachers, herbalists.
- **External links preserved from this band:**
 - North to internal stone path rooms (former 1206-1207)
 - Up to `7200` (elevated access, from former 1166)
 - North to `4449` (building/structure link, from former 1207)

### Band G — Inlet, Cave, and Sinkline (1169-1177, 9 rooms)
- **Sector types:** `water_swim` (6) for shallow water rooms; `water_noswim` (7) for deep water rooms; `desert` (10) for beach rooms; `inside` (11) for cave/sinkhole rooms.
- **Room flags:** `0` for water and beach; `dark` (1) for cave/sinkhole interiors. Boss encounter room flagged `no_mob` (4).
- Water-edge and cliff subzone with salvage economy and old smuggler pockets.
- Encounter emphasis: shoreline hunters, cave squatters, tidebound anomalies.
- **External links preserved from this band:**
 - North to `5083` (sea route link, from former 1175)
- **Door preserved:** Iron door between beach and cave rooms (reset to closed, state 1, matching existing reset).

### Band H — Ruin Run and Weathered Causeway (1178-1190, 13 rooms)
- **Sector types:** `desert` (10) for arid wasteland rooms; `field` (2) for ruin and post-ruin rooms.
- **Room flags:** `0` for most; boss encounter room flagged `no_mob` (4).
- Ancient city fragments integrated into roadbed over centuries.
- Strongest "deep time" lore in the route.
- Encounter emphasis: ruin custodians, script-haunted echoes, relic caravans.

### Band I — Western Shore Reach (1191-1215, 25 rooms)
- **Sector types:** `field` (2) for coastal approach rooms; `desert` (10) for beach rooms; `water_swim` (6) for shallow surf; `hills` (4) for clifftop road segments.
- **Room flags:** `0` for most; the western terminus promontory room is important (5+ sentences).
- Windblown coast and terminal roadward points linked to Kiess-bound sea trade memory.
- Encounter emphasis: privateer remnants, coast sentries, carrion birds, stranded pilgrims.

## Room Rebuild Policy (all rooms)
- All room titles and descriptions are replaced.
- Each room receives a **route identity tag** in builder notes (not player text): civic, military, mercantile, druidic, ruin, littoral.
- Descriptions avoid vnum mentions and use sparse, intentional color coding only.
- Exits preserve macro connectivity (no accidental route breaks), but selected rooms add optional branch loops to reduce pure linear fatigue.
- Existing legacy external links are retained unless world owners request remap:
 - Midgaard West Gate linkage at the east terminus
 - Kiess East Gate linkage at the west terminus
 - Crossroad branch connectors
 - Sea and legacy side-area hooks

## Mobile Redesign (new full roster)
### Vnum strategy
- Use `1117-1215` as the mobile envelope (parallel namespace), with ~36 active mob templates and variant reset mixes.
- Vnums assigned in ascending sequential order with no gaps, starting at `1117`.

### Mob Spec Compliance and Placement Principles

**Required act flags for all mobs:**
- Every mob must have `stay_area` (`64`) set so it does not wander out of Roc Road.
- Stationary mobs (bridge wardens, shrine keepers, crossroads sentries) must also have `sentinel` (`2`) set.
- No mob should have `invasion` set; this is a runtime-only flag.

**Aggressive flag policy:**
- Feral predators (wolves, ridge cats, scavenger birds) and hostile humanoids (bandits, revenants, zealots) should be flagged `aggressive` (`32`).
- Neutral wayfolk (drovers, peddlers, ferrymen), Midgaard roadwardens, and Kiess convoy syndics must not be flagged aggressive.
- Guards who react when provoked may use `spec_guard`/`spec_policeman` specials rather than the aggressive flag.

**Boss and solo policy:**
- Boss mobs must be flagged `sentinel` (`2`) + `boss` (`67108864`) and placed only in rooms flagged `no_mob` (`4`).
- Strong (non-boss) named NPCs (e.g., patrol captains, convoy marshals) should be flagged `solo` (`33554432`).

**Mobile description format (strict):**
- `long_descr` must be exactly one text line, then a newline, then `~`. Multi-line `long_descr` is never valid.
- `description` must end with exactly one newline before `~`.
- No vnum may appear in any mob description text.

### Factions
1. **Midgaard Roadwardens** — disciplined patrol infantry and legal clerks.
2. **Kiess Convoy Syndics** — caravan marshals, quarter agents, route accountants.
3. **Independent Wayfolk** — drovers, peddlers, shrine-keepers, ferrymen.
4. **Frontier Predators** — wolves, ridge cats, river ambushers, scavenger birds.
5. **Ruin-Touched Hostiles** — script-blind revenants, cairn lurkers, relic zealots.
6. **Shadow Trade Cells** — smugglers, extortion crews, forged-seal couriers.

### Mobile Roster (planned ~36 templates)

#### Faction 1: Midgaard Roadwardens (non-aggressive, Bands A-C)

| Vnum | Name | Level | Act Flags | Band | Notes |
|------|------|-------|-----------|------|-------|
| 1117 | a Midgaard road warden | 30 | is_npc, sentinel, stay_area | A | Gate-adjacent patrol; `spec_guard` |
| 1118 | a Midgaard road clerk | 20 | is_npc, stay_area | A | Tax and census mark recorder |
| 1119 | a Midgaard road warden captain | 50 | is_npc, sentinel, stay_area, solo | A-B | Senior patrol officer |
| 1120 | a charter inscription keeper | 25 | is_npc, sentinel, stay_area | A | Maintains legal markers |

#### Faction 2: Kiess Convoy Syndics (non-aggressive, Bands D-E and road-wide)

| Vnum | Name | Level | Act Flags | Band | Notes |
|------|------|-------|-----------|------|-------|
| 1121 | a Kiess convoy marshal | 35 | is_npc, stay_area | E | Caravan leader |
| 1122 | a Kiess quarter agent | 25 | is_npc, stay_area | D-E | Supply and risk accountant |
| 1123 | a Kiess route ledger scribe | 20 | is_npc, sentinel, stay_area | E | Records debt and guard obligations |
| 1124 | a Kiess convoy guard | 40 | is_npc, stay_area | D-E | Armed escort |

#### Faction 3: Independent Wayfolk (non-aggressive, road-wide)

| Vnum | Name | Level | Act Flags | Band | Notes |
|------|------|-------|-----------|------|-------|
| 1125 | a road drover | 15 | is_npc, stay_area | A-D | Animal handler |
| 1126 | a peddler of the road | 18 | is_npc, stay_area | B-E | Traveling merchant |
| 1127 | a waystone shrine keeper | 22 | is_npc, sentinel, stay_area | C-D | Maintains roadside shrines |
| 1128 | a river ferryman | 20 | is_npc, sentinel, stay_area | C | River crossing operator |
| 1129 | a road-camp cook | 15 | is_npc, stay_area | B-D | Campsite laborer |

#### Faction 4: Frontier Predators (aggressive, band-appropriate levels)

| Vnum | Name | Level | Act Flags | Band | Notes |
|------|------|-------|-----------|------|-------|
| 1130 | a ridge wolf | 25 | is_npc, aggressive, stay_area | B | Hill predator |
| 1131 | a ridge cat | 35 | is_npc, aggressive, stay_area | B-C | Ambush predator |
| 1132 | a river ambusher snake | 20 | is_npc, aggressive, stay_area | C | Waterside threat |
| 1133 | a dust scavenger bird | 30 | is_npc, aggressive, stay_area | D-H | Carrion predator |
| 1134 | a coastal carrion gull | 35 | is_npc, aggressive, stay_area | I | Shore predator |
| 1135 | a plains stalker | 40 | is_npc, aggressive, stay_area | H-I | Open-ground hunter |

#### Faction 5: Ruin-Touched Hostiles (aggressive, Bands F-H)

| Vnum | Name | Level | Act Flags | Band | Notes |
|------|------|-------|-----------|------|-------|
| 1136 | a cairn lurker | 50 | is_npc, aggressive, stay_area | H | Haunts ancient ruins |
| 1137 | a script-blind revenant | 60 | is_npc, aggressive, stay_area, undead | H | Animated by old law magic |
| 1138 | a relic zealot | 45 | is_npc, aggressive, stay_area | H | Ideologue using antique script myths |
| 1139 | a ruin custodian shade | 55 | is_npc, aggressive, stay_area, undead | H | Guardian of old structures |
| 1140 | a displaced forest spirit | 40 | is_npc, aggressive, stay_area | F | Sanctuary violation specter |

#### Faction 6: Shadow Trade Cells (aggressive, Bands C-G)

| Vnum | Name | Level | Act Flags | Band | Notes |
|------|------|-------|-----------|------|-------|
| 1141 | a tollbreak enforcer | 40 | is_npc, aggressive, stay_area | C-E | Violent extraction crew |
| 1142 | a contraband runner | 30 | is_npc, aggressive, stay_area | C-G | Smuggler |
| 1143 | a forged-seal courier | 25 | is_npc, aggressive, stay_area | E | Falsified legal documents |
| 1144 | a cave squatter | 35 | is_npc, aggressive, stay_area | G | Tide-cave holdout |

#### Boss/Elite Encounters (4 bosses, aggressive, sentinel, boss)

| Vnum | Name | Level | Act Flags | Band | Room | Notes |
|------|------|-------|-----------|------|------|-------|
| 1145 | the Toll-Reeve Without Crown | 90 | is_npc, sentinel, aggressive, stay_area, boss, no_flee | E | no_mob room | Crossroads legal revenant; undead; `spec_cast_undead` |
| 1146 | the Greenveil Antler-Warden | 85 | is_npc, sentinel, aggressive, stay_area, boss, no_flee | F | no_mob room | Forest spirit-warrior hybrid |
| 1147 | the Inlet Chainmaster | 80 | is_npc, sentinel, aggressive, stay_area, boss, no_flee | G | no_mob room | Smuggler captain in tide cave |
| 1148 | the Promontory Harrower | 95 | is_npc, sentinel, aggressive, stay_area, boss, no_flee | I | no_mob room | Coastal apex predator threat |

#### Ambient Flavor Mobs (non-aggressive)

| Vnum | Name | Level | Act Flags | Band | Notes |
|------|------|-------|-----------|------|-------|
| 1149 | a road wagon | 10 | is_npc, stay_area | A-E | Background traffic |
| 1150 | a mule train driver | 12 | is_npc, stay_area | B-D | Background traffic |
| 1151 | a road-weary pilgrim | 10 | is_npc, stay_area | D-I | Stranded traveler |
| 1152 | a ruin scavenger | 20 | is_npc, stay_area | H | Non-hostile relic picker |

**Mob combat extensions (`!` line):**
- Roadwarden and convoy mobs (level 20-50): melee profile, low-mid skills per floor tables.
- Predator and hostile mobs (level 25-60): melee profile, mid skills.
- Boss mobs (level 80-95): melee or hybrid profile, full skill floors guaranteed at their levels. Bosses with casting (Toll-Reeve) use hybrid profile with appropriate `cast` and `def` bitvectors.
- Ambient flavor mobs (level 10-20): minimal combat; 2 attacks and basic skills only.

**Mob elemental extensions (`|` line):**
- Ruin-touched hostiles: `strong_magic` shadow/fire; `resist` physical.
- River/coastal predators: `strong_magic` water; `suscept` fire.
- Forest spirits: `strong_magic` earth; `suscept` fire.
- Other factions: no elemental extensions (neutral wilderness environment).

## Object Redesign (new full roster)
### Vnum strategy
- Use `1117-1215` as the object envelope, target ~42 objects with progression tiers.
- Vnums assigned in ascending sequential order with no gaps, starting at `1117`.

### Object Spec Compliance

**Required for all objects:**
- Every object must include `ITEM_TAKE` (`8388608`) in wear flags.
- No object may include `ITEM_WEAR_CLAN_COLORS` (`16777216`).
- Object `name`, `short_descr`, and `description` must be thematically consistent with wear flags.
- Object names must be unique within the area file; no two objects may share the same `<name>~` value.
- Do not define fixed stats in area files; stats are generated at runtime.
- Do not set `ITEM_GENERATED` (`1`) in area files; it is runtime-managed.

**Weight archetype encoding:**
- `1`-`5`: caster item (wands, charms, scrolls, focus items)
- `6`-`10`: melee item (weapons, combat gear, medium armor)
- `11`-`15`: tank item (heavy armor, shields, reinforced gear)

**Weapon requirements:**
- Weapons must have `item_type = 5` (`ITEM_WEAPON`) and wear flags including both `hold` (`32768`) and `take` (`8388608`).
- `value3` (weapon attack type) must be thematically consistent with the weapon's name and description.
- `value3 = 0` (`hit`) is only valid with the `ITEM_FIST` extra flag.
- Two-handed weapon names (e.g., "great axe") must include `ITEM_TWO_HANDED` (`2147483648`) extra flag.

**Loot/boss item policy:**
- Items spawned from mob loot tables (`l`/`L` extension) must have `ITEM_LOOT` (`67108864`) set.
- Items that drop from boss mobs must include `ITEM_BOSS` (`134217728`).

**Extra description format:**
- `E` entry descriptions must end with exactly one trailing newline before `~`.
- No vnums may appear in any object description text.

### Item families

#### Roadwarden Kit (Bands A-C, levels 15-45)

| Vnum | Name | Item Type | Wear | Weight | Level | Notes |
|------|------|-----------|------|--------|-------|-------|
| 1117 | a roadwarden badgeplate | armor | body, take | 11 | 25 | Tank; patrol identification |
| 1118 | a signal cloak of the road watch | armor | about, take | 6 | 30 | Melee; wind-resistant travel cloak |
| 1119 | reinforced warden boots | armor | feet, take | 11 | 35 | Tank; road-rated footwear |
| 1120 | a toll rod of Midgaard charter | weapon | hold, take | 6 | 40 | Melee; value3=7 (pound); legal authority weapon |
| 1121 | a road warden patrol lantern | light | take | 3 | 20 | Caster; wayfinding light |
| 1122 | a charter inscription rubbing | trash | take | 1 | 15 | Caster; lore object with E extra desc |

#### Convoy Gear (Bands D-E, levels 25-55)

| Vnum | Name | Item Type | Wear | Weight | Level | Notes |
|------|------|-----------|------|--------|-------|-------|
| 1123 | a Kiess trade ledger | trash | take | 2 | 30 | Caster; lore object with E extra desc |
| 1124 | a convoy lockbox | container | take | 8 | 35 | Melee; trade container |
| 1125 | a mule tack harness | armor | waist, take | 7 | 25 | Melee; utility gear |
| 1126 | a lantern standard pole | weapon | hold, take | 8 | 45 | Melee; value3=7 (pound); improvised weapon |
| 1127 | a syndic quarter-agent seal | treasure | take | 2 | 40 | Caster; trade authority token |
| 1128 | a convoy guard shield | armor | hold, take | 13 | 50 | Tank; shield archetype |

#### Forest Relics (Band F, levels 30-55)

| Vnum | Name | Item Type | Wear | Weight | Level | Notes |
|------|------|-----------|------|--------|-------|-------|
| 1129 | a bark-charm of the old verge | treasure | take | 1 | 35 | Caster; druidic trace relic |
| 1130 | a branch fetish of the spur | treasure | take | 2 | 40 | Caster; forest spirit ward |
| 1131 | a herb satchel of the greenveil | container | take | 3 | 30 | Caster; medicinal herb pouch |
| 1132 | a vow-ribbon of the sanctuary | armor | neck, take | 1 | 45 | Caster; exile community token |
| 1133 | a moss-wrapped druid staff | weapon | hold, take | 4 | 55 | Caster; value3=8 (crush); two-handed (ITEM_TWO_HANDED) |

#### Ruin Salvage (Band H, levels 40-70)

| Vnum | Name | Item Type | Wear | Weight | Level | Notes |
|------|------|-----------|------|--------|-------|-------|
| 1134 | an etched stone tablet | trash | take | 3 | 45 | Caster; lore object with E extra desc (old tax records) |
| 1135 | a weathered bronze coin | treasure | take | 1 | 40 | Caster; pre-charter currency |
| 1136 | a fractured sealstone | treasure | take | 2 | 50 | Caster; broken authority seal |
| 1137 | a ruin-script amulet | armor | neck, take | 3 | 60 | Caster; ancient script pendant |
| 1138 | a causeway flagstone shard | weapon | hold, take | 9 | 55 | Melee; value3=8 (crush); improvised weapon |
| 1139 | a script-etched bone knife | weapon | hold, take | 5 | 65 | Caster; value3=2 (stab); relic blade |

#### Coastal Finds (Band I, levels 35-65)

| Vnum | Name | Item Type | Wear | Weight | Level | Notes |
|------|------|-----------|------|--------|-------|-------|
| 1140 | a salt-cured leather vest | armor | body, take | 7 | 40 | Melee; coastal wear |
| 1141 | a barnacled cutlass | weapon | hold, take | 8 | 50 | Melee; value3=3 (slash); corroded naval blade |
| 1142 | a ropework climbing harness | armor | waist, take | 7 | 35 | Melee; cliff-side utility |
| 1143 | a sea-glass focus shard | treasure | hold, take | 2 | 55 | Caster; magical focus |
| 1144 | a driftwood buckler | armor | hold, take | 12 | 45 | Tank; buckler (ITEM_BUCKLER); salvaged shield |
| 1145 | a privateer signal horn | treasure | take | 4 | 60 | Caster; old maritime call |

#### Boss Signature Drops (ITEM_LOOT + ITEM_BOSS, one per elite)

| Vnum | Name | Item Type | Wear | Weight | Level | Extra Flags | Notes |
|------|------|-----------|------|--------|-------|-------------|-------|
| 1146 | the Toll-Reeve's unsealed warrant | armor | hold, take | 3 | 90 | ITEM_LOOT, ITEM_BOSS | Caster; legal revenant's authority document; shield archetype |
| 1147 | the Antler-Warden's crown of thorns | armor | head, take | 5 | 85 | ITEM_LOOT, ITEM_BOSS | Caster; forest spirit headpiece |
| 1148 | the Chainmaster's tide-iron shackle | armor | wrist, take | 9 | 80 | ITEM_LOOT, ITEM_BOSS | Melee; smuggler captain's symbol |
| 1149 | the Harrower's storm-bone talisman | armor | neck, take | 4 | 95 | ITEM_LOOT, ITEM_BOSS | Caster; apex predator trophy |

#### Utility and Key Items

| Vnum | Name | Item Type | Wear | Weight | Level | Notes |
|------|------|-----------|------|--------|-------|-------|
| 1150 | a rusted iron cave key | key | take | 1 | 1 | Key for iron door in Band G cave (if door is set to locked) |

#### Reserved Object Vnums
- **1151-1215:** Unallocated (65 vnums). Reserved for additional encounter drops, quest items, shop stock (if a waystation shop is added), and per-band flavor objects added during implementation.

### Item families summary
- **Roadwarden kit:** badgeplates, signal cloaks, reinforced boots, toll rods.
- **Convoy gear:** trade ledgers, lockboxes, mule tack, lantern standards.
- **Forest relics:** bark-charms, branch fetishes, herb satchels, vow-ribbons.
- **Ruin salvage:** etched tablets, weathered coinage, fractured sealstones.
- **Coastal finds:** salt-cured leathers, barnacled blades, ropework harnesses.
- **Named signatures:** each elite encounter drops one identity object tied to lore arcs.

## Reset Strategy
- Replace the two-door-only reset block with a full ecosystem reset table.
- Ensure each thematic band has:
 - 1 stable baseline population reset
 - 1 roaming pressure reset
 - 1 occasional rare/event reset
- Protect transit reliability by preventing hard-lock hostile overpopulation at mandatory choke rooms.
- Keep any lock/key mechanics optional or side-route only (no required key gates on the main through-road).
- `#RESETS` must not contain blank lines.
- All resets must reference valid vnums for their target type.

### Mob Resets (`M` commands) — planned distribution

**Band A (1117-1127): Midgaard Gate March**
- 2x road warden (1117), limit 3, in gate-adjacent rooms (sentinel)
- 1x road clerk (1118), limit 2
- 1x road warden captain (1119), limit 1 (solo)
- 1x charter keeper (1120), limit 1 (sentinel)
- 2x road drover (1125), limit 3
- 1x road wagon (1149), limit 2

**Band B (1128-1135): Banner Hills**
- 2x ridge wolf (1130), limit 3
- 1x ridge cat (1131), limit 2
- 1x peddler (1126), limit 2
- 1x road-camp cook (1129), limit 2

**Band C (1136-1143): River Crossing**
- 1x ferryman (1128), limit 1 (sentinel)
- 1x shrine keeper (1127), limit 1 (sentinel)
- 1x river ambusher snake (1132), limit 2
- 1x tollbreak enforcer (1141), limit 2
- 1x contraband runner (1142), limit 2

**Band D (1144-1150): Dustward Verge**
- 1x Kiess quarter agent (1122), limit 2
- 1x dust scavenger bird (1133), limit 3
- 1x road-weary pilgrim (1151), limit 2
- 1x mule train driver (1150), limit 2

**Band E (1151-1159): Crossroads Compact**
- 1x Kiess convoy marshal (1121), limit 1
- 1x Kiess route ledger scribe (1123), limit 1 (sentinel)
- 1x Kiess convoy guard (1124), limit 2
- 1x forged-seal courier (1143), limit 2
- 1x Toll-Reeve Without Crown (1145), limit 1 (boss, in no_mob room)

**Band F (1160-1168): Greenveil Spur**
- 1x displaced forest spirit (1140), limit 2
- 1x contraband runner (1142), limit 1
- 1x Greenveil Antler-Warden (1146), limit 1 (boss, in no_mob room)

**Band G (1169-1177): Inlet and Cave**
- 1x cave squatter (1144), limit 2
- 1x coastal carrion gull (1134), limit 2
- 1x Inlet Chainmaster (1147), limit 1 (boss, in no_mob room)

**Band H (1178-1190): Ruin Run**
- 2x cairn lurker (1136), limit 3
- 1x script-blind revenant (1137), limit 2
- 1x relic zealot (1138), limit 2
- 1x ruin custodian shade (1139), limit 2
- 1x ruin scavenger (1152), limit 2

**Band I (1191-1215): Western Shore**
- 2x coastal carrion gull (1134), limit 3
- 1x plains stalker (1135), limit 2
- 1x road-weary pilgrim (1151), limit 2
- 1x Promontory Harrower (1148), limit 1 (boss, in no_mob room)

### Equipment Resets (`E`/`G` commands)
- Boss mobs receive equipment resets for their signature items (weapon + armor appropriate to their level and theme).
- Roadwarden and convoy mobs receive level-appropriate equipment through `E` resets.
- Equipment objects placed via E/G resets are normal reset equipment and do NOT have ITEM_LOOT set.

### Loot Table Assignments (`l`/`L` mob extensions)
- Each boss mob carries a loot table pointing to its signature drop object.
- Boss signature drops use `ITEM_LOOT` + `ITEM_BOSS` in extra_flags.
- `loot_amount` for bosses: `100` (guaranteed one drop).
- `loot_chance` for single-item tables: `100` (100% weight on the single slot).

### Door Resets (`D` commands)
- Iron door in Band G (cave entrance): `D 0 <cave_beach_vnum> <door_dir> 1` (closed, not locked). Matches existing reset behavior.
- Iron door reverse side: `D 0 <cave_interior_vnum> <door_dir> 1` (closed). Matches existing reset.

### Object Resets (`O` commands)
- No standalone room object spawns planned initially. Lore objects and flavor items will be implemented as room extra descriptions rather than spawned objects, keeping the reset table focused on population management.

## Gameplay Flow and Difficulty Curve
- **1117-1136:** lower pressure, high traffic, social/travel readability.
- **1137-1166:** moderate pressure, mixed social/hostile density.
- **1167-1191:** elevated pressure with environmental storytelling.
- **1192-1215:** high volatility optional coast-end challenge, but still traversable with caution.

## Terminus Gate Connectivity (required)
- **East end of Roc Road must connect to Midgaard West Gate.**
- **West end of Roc Road must connect to Kiess East Gate.**
- Terminus room text, exit labels, and reset populations should reinforce these gate identities as the canonical endpoints of the corridor.

## Kiess/Midgaard Integration Requirements
- Midgaard influence appears as road law, census marks, disciplined engineering, and civic inscriptions.
- Kiess influence appears as convoy organization, layered commerce, adaptive frontier pragmatism, and multilingual route culture.
- Crossroads rooms explicitly show **jurisdictional overlap** rather than a single sovereign narrative.
- NPC barks and object texts reference shared dependence: neither city can fully control the road, yet both require it.

## Shops Plan

No shops are planned for Roc Road. The area is a wilderness travel corridor, not a settlement hub. Players requiring merchant services should visit Midgaard (east terminus) or Kiess (west terminus). If playtesting reveals a need for a mid-route waystation vendor, a single shop could be added to the Crossroads Compact band (Band E) using reserved mob and object vnums.

---

## Specials Plan

| Mob Vnum | Spec Function | Reason |
|----------|--------------|--------|
| 1117 | spec_guard | Road warden — reacts to criminal-flagged players |
| 1145 | spec_cast_undead | Toll-Reeve Without Crown — boss undead caster |

Notes:
- Most mobs do not need spec functions; their behavior is driven by `act` flags (aggressive, sentinel, etc.).
- Predators and bandits use the `aggressive` flag for combat initiation rather than spec functions.
- The Greenveil Antler-Warden, Inlet Chainmaster, and Promontory Harrower are melee-focused bosses and do not require casting specs.
- `spec_summon_*` functions must never be set in area files (runtime-only).
- `spec_keep*` functions may only be assigned manually by a human.

---

## Quests Plan

Roc Road quests are static quests offered by the postmasters of the three connected cities. They reflect the road's role as a contested corridor: Midgaard issues law-enforcement and infrastructure-protection tasks; Kiess issues commerce-protection and frontier-intelligence tasks; Kowloon issues supply-chain and diplomatic tasks that depend on the road remaining open.

**Postman vnums:**
- Midgaard: `931`
- Kiess: `3340`
- Kowloon: TBD (area not yet implemented; mob vnum will be in the `3479-3508` service range per `kowloon_plan.md`). Quests below use placeholder `0` for the offerer vnum until the Kowloon postmaster mob is created.

**File location:** `/quests/N.prop` (N = quest ID)

**Existing quests:** IDs 1-5 are already allocated. Roc Road quests use IDs **6-14**.

**Static quest file format (5 lines):**
1. `<title>`
2. `<prerequisite_static_id> <prop_type> <num_targets> <kill_needed> <min_level> <offerer_vnum> <reward_gold> <reward_qp> <reward_item_vnum> <reward_item_count>`
3. `<target_vnum_1> [target_vnum_2] ...`
4. `<accept_message>`
5. `<completion_message>`

**Prop types:** `1` = kill variety (kill N different mob types), `2` = collect items (collect N different items), `3` = kill count (kill N of one mob type)

---

### Quest 6: Road Predator Cull (Midgaard, non-chain)

**File:** `6.prop`

| Field | Value |
|---|---|
| Title | Road predator cull: Banner Hills to Dustward |
| Prerequisite | `-1` (none) |
| Type | `1` (kill variety) |
| Num targets | `3` |
| Kill needed | `0` |
| Min level | `15` |
| Offerer vnum | `931` (Midgaard postman) |
| Reward gold | `278` |
| Reward QP | `2` |
| Reward item vnum | `0` |
| Reward item count | `0` |
| Target vnums | `1130` (ridge wolf), `1131` (ridge cat), `1133` (dust scavenger bird) |

**Line 2:** `-1 1 3 0 15 931 278 2 0 0`
**Line 3:** `1130 1131 1133`

**Accept text:** Road warden command has documented predator incursions along the Banner Hills and Dustward Verge that are delaying courier rotations and forcing convoy escorts to double their watch. Cull one of each primary threat species — ridge wolves, ridge cats, and dust scavenger birds — and report your engagement positions so patrol routes can be adjusted. The postmaster needs confirmation before the next warden shift rotation can be authorized.

**Completion text:** Patrol command has updated the threat map using your engagement data. Warden shift rotations are proceeding on revised schedules, and convoy escorts report the predator pressure has dropped enough to resume single-watch departures through the hills. The postmaster has logged your cull as a standing reference for the next quarterly road-safety assessment.

---

### Quest 7: Tollbreak Crew Interdiction (Midgaard, chain start)

**File:** `7.prop`

| Field | Value |
|---|---|
| Title | Tollbreak crew interdiction: River Crossing |
| Prerequisite | `-1` (none) |
| Type | `3` (kill count) |
| Num targets | `1` |
| Kill needed | `6` |
| Min level | `30` |
| Offerer vnum | `931` (Midgaard postman) |
| Reward gold | `792` |
| Reward QP | `3` |
| Reward item vnum | `0` |
| Reward item count | `0` |
| Target vnums | `1141` (tollbreak enforcer) |

**Line 2:** `-1 3 1 6 30 931 792 3 0 0`
**Line 3:** `1141`

**Accept text:** Tollbreak crews have seized the River Ledger Crossing approaches and are extracting violent tolls from merchants who lack armed escort. Midgaard's charter authority over the crossing is being openly defied. Eliminate at least six tollbreak enforcers to break their grip on the ford and restore safe passage for bonded traffic. The postmaster requires a body count sufficient to justify redeploying wardens from the Banner Hills garrison.

**Completion text:** The tollbreak presence at the River Crossing has collapsed. Surviving crew members have scattered into the downstream brush, and bonded merchants are crossing without extortion for the first time in weeks. The postmaster has authorized warden redeployment from the Banner Hills garrison to establish a permanent checkpoint at the ford approach. Your interdiction report has been forwarded to the Magistrate Ledgerhouse as evidence for the next road-security appropriation hearing.

---

### Quest 8: Shadow Trade Disruption (Midgaard, chains from 7)

**File:** `8.prop`

| Field | Value |
|---|---|
| Title | Shadow trade disruption: Crossroads to Greenveil |
| Prerequisite | `7` (requires Prop 7 — Tollbreak Crew Interdiction) |
| Type | `1` (kill variety) |
| Num targets | `3` |
| Kill needed | `0` |
| Min level | `40` |
| Offerer vnum | `931` (Midgaard postman) |
| Reward gold | `1266` |
| Reward QP | `4` |
| Reward item vnum | `0` |
| Reward item count | `0` |
| Target vnums | `1142` (contraband runner), `1143` (forged-seal courier), `1144` (cave squatter) |

**Line 2:** `7 1 3 0 40 931 1266 4 0 0`
**Line 3:** `1142 1143 1144`

**Accept text:** With the tollbreak crews broken at the crossing, Reckoning clerks have intercepted forged-seal documents suggesting a deeper shadow network operating between the Crossroads and the inlet caves. Contraband runners, forged-seal couriers, and cave squatters form an interconnected supply chain moving falsified legal instruments and untaxed goods. Disrupt each node of this network before they can reroute through the Greenveil Spur. The postmaster wants evidence that all three cell types have been hit.

**Completion text:** The shadow trade network between the Crossroads and the caves has been fragmented. Reckoning clerks recovered forged-seal dies and falsified convoy manifests from the bodies, confirming that the network was moving documents far more dangerous than contraband. The postmaster has forwarded your intelligence to the Magistrate Ledgerhouse, where it will be used to build the case for extending warden jurisdiction into the inlet subzone — a jurisdictional expansion that Kiess's Trade Syndics have resisted for years.

---

### Quest 9: Convoy Route Security Assessment (Kiess, non-chain)

**File:** `9.prop`

| Field | Value |
|---|---|
| Title | Convoy route security: Dustward predator sweep |
| Prerequisite | `-1` (none) |
| Type | `3` (kill count) |
| Num targets | `1` |
| Kill needed | `8` |
| Min level | `20` |
| Offerer vnum | `3340` (Kiess postman) |
| Reward gold | `291` |
| Reward QP | `2` |
| Reward item vnum | `0` |
| Reward item count | `0` |
| Target vnums | `1133` (dust scavenger bird) |

**Line 2:** `-1 3 1 8 20 3340 291 2 0 0`
**Line 3:** `1133`

**Accept text:** Trade Syndic quartermasters report that dust scavenger birds are harassing mule trains and tearing into unsecured cargo along the Dustward Verge. Convoy insurance claims have spiked, and two scheduled departures have been postponed pending threat reassessment. Thin the scavenger bird population until convoy marshals can certify the route as insurable again. The postmaster needs your kill tally to file with the Syndic Council's risk-adjustment office.

**Completion text:** Scavenger bird pressure along the Dustward Verge has dropped below the threshold that triggers convoy insurance surcharges. Trade Syndic quartermasters have cleared the postponed departures, and mule train drivers report intact cargo for the first time in weeks. The postmaster has filed your tally with the Syndic Council and noted that your work saved the convoy compact more in avoided claims than the posted reward cost.

---

### Quest 10: Greenveil Spur Reconnaissance (Kiess, chain start)

**File:** `10.prop`

| Field | Value |
|---|---|
| Title | Greenveil Spur reconnaissance: spirit clearance |
| Prerequisite | `-1` (none) |
| Type | `1` (kill variety) |
| Num targets | `2` |
| Kill needed | `0` |
| Min level | `35` |
| Offerer vnum | `3340` (Kiess postman) |
| Reward gold | `1800` |
| Reward QP | `3` |
| Reward item vnum | `0` |
| Reward item count | `0` |
| Target vnums | `1140` (displaced forest spirit), `1142` (contraband runner) |

**Line 2:** `-1 1 2 0 35 3340 1800 3 0 0`
**Line 3:** `1140 1142`

**Accept text:** Wall Command scouts report that the Greenveil Spur — the convoy reroute used when the main road is compromised — has become impassable. Displaced forest spirits are attacking anything that enters the canopy, and contraband runners have established camps along the path to intercept diverted traffic. Clear both threats so the spur can be recertified as an emergency bypass. The postmaster needs confirmation before convoy marshals can add the spur back to their contingency routing tables.

**Completion text:** The Greenveil Spur has been cleared of hostile spirits and contraband camps. Convoy marshals have recertified the path as an emergency bypass route, and Wall Command scouts have resumed periodic patrols. The postmaster notes that this is the first time in two seasons that Kiess has had a functional reroute option — your clearance work restores a critical piece of the convoy compact's redundancy planning.

---

### Quest 11: Inlet Smuggler Crackdown (Kiess, chains from 10)

**File:** `11.prop`

| Field | Value |
|---|---|
| Title | Inlet smuggler crackdown: cave network purge |
| Prerequisite | `10` (requires Prop 10 — Greenveil Spur Reconnaissance) |
| Type | `3` (kill count) |
| Num targets | `1` |
| Kill needed | `10` |
| Min level | `50` |
| Offerer vnum | `3340` (Kiess postman) |
| Reward gold | `4000` |
| Reward QP | `5` |
| Reward item vnum | `0` |
| Reward item count | `0` |
| Target vnums | `1144` (cave squatter) |

**Line 2:** `10 3 1 10 50 3340 4000 5 0 0`
**Line 3:** `1144`

**Accept text:** With the Greenveil Spur secured, Wall Command has turned attention to the inlet cave network where the Chainmaster's crew operates a private toll system that undercuts legitimate convoy compact fees. The cave squatters are former laborers and deserters — dangerous not because of individual skill but because they know the road's blind spots from years of service. Purge the cave network of at least ten squatters to collapse the Chainmaster's labor base. The postmaster wants this done before the next convoy scheduling cycle so the Syndic Council can close the insurance loophole that smuggler routes create.

**Completion text:** The cave squatter workforce has been gutted. Without laborers to maintain chains, watch positions, and cargo transfers, the Chainmaster's private toll operation is crippled. Wall Command scouts report that the cave network is effectively abandoned, and the Syndic Council has closed the insurance loophole that allowed shippers to claim smuggler-route losses against legitimate convoy compact pools. The postmaster has noted that this cleanup may force the Chainmaster himself out of his stronghold — a problem for another day, but a day that is now closer.

---

### Quest 12: Ruin Custodian Suppression (Midgaard, non-chain)

**File:** `12.prop`

| Field | Value |
|---|---|
| Title | Ruin custodian suppression: Weathered Causeway |
| Prerequisite | `-1` (none) |
| Type | `1` (kill variety) |
| Num targets | `4` |
| Kill needed | `0` |
| Min level | `50` |
| Offerer vnum | `931` (Midgaard postman) |
| Reward gold | `4500` |
| Reward QP | `5` |
| Reward item vnum | `0` |
| Reward item count | `0` |
| Target vnums | `1136` (cairn lurker), `1137` (script-blind revenant), `1138` (relic zealot), `1139` (ruin custodian shade) |

**Line 2:** `-1 1 4 0 50 931 4500 5 0 0`
**Line 3:** `1136 1137 1138 1139`

**Accept text:** Reckoning archivists need the Ruin Run corridor cleared for a documentation expedition. The ruin-touched hostiles — cairn lurkers, script-blind revenants, relic zealots, and ruin custodian shades — have made the causeway impassable to unarmed scholars. Eliminate one of each threat type to create a safe transit window. The postmaster emphasizes that this is a civic-academic priority: the tablets in those ruins may contain evidence relevant to ongoing Continuity-Reckoning legal proceedings.

**Completion text:** The Ruin Run corridor is clear enough for the documentation expedition to proceed under warden escort. Reckoning archivists have already dispatched a survey team to photograph and transcribe the exposed tablets before weather or further hostile activity degrades them. The postmaster has logged this as a joint security-academic operation — the first of its kind on the western road — and forwarded your engagement report to both the Magistrate Ledgerhouse and the Granite Arcade archive office.

---

### Quest 13: Coastal Hazard Survey (Kowloon, non-chain)

**File:** `13.prop`

| Field | Value |
|---|---|
| Title | Coastal hazard survey: Western Shore predators |
| Prerequisite | `-1` (none) |
| Type | `1` (kill variety) |
| Num targets | `3` |
| Kill needed | `0` |
| Min level | `40` |
| Offerer vnum | `0` (Kowloon postman — TBD, see note above) |
| Reward gold | `2500` |
| Reward QP | `3` |
| Reward item vnum | `0` |
| Reward item count | `0` |
| Target vnums | `1134` (coastal carrion gull), `1135` (plains stalker), `1132` (river ambusher snake) |

**Line 2:** `-1 1 3 0 40 0 2500 3 0 0`
**Line 3:** `1134 1135 1132`

**Accept text:** Kowloon's shipping agents report that the western shore terminus of Roc Road has become too dangerous for the small-boat relay crews who transfer sealed diplomatic pouches between overland couriers and coastal vessels. Carrion gulls harass the landing sites, plains stalkers hunt the approach roads, and river ambushers threaten the waterside transfer points. Survey the full threat spectrum — engage one of each — and report conditions so Kowloon's courier office can determine whether to reroute diplomatic traffic through Kiess's harbor instead.

**Completion text:** Your threat survey confirms that the western shore terminus remains viable for small-boat relay operations if escort protocols are tightened. Kowloon's courier office has decided to maintain the overland-to-coastal transfer at the current terminus rather than rerouting through Kiess — a decision that keeps Kowloon's diplomatic pouch schedule independent of Kiess harbor fees. The postmaster has forwarded your survey data to the covenant registrar for inclusion in the next quarterly route-risk assessment.

---

### Quest 14: Transcontinental Corridor Assessment (Kowloon, chains from 13)

**File:** `14.prop`

| Field | Value |
|---|---|
| Title | Transcontinental corridor assessment: full-road interdiction |
| Prerequisite | `13` (requires Prop 13 — Coastal Hazard Survey) |
| Type | `1` (kill variety) |
| Num targets | `5` |
| Kill needed | `0` |
| Min level | `60` |
| Offerer vnum | `0` (Kowloon postman — TBD, see note above) |
| Reward gold | `6000` |
| Reward QP | `6` |
| Reward item vnum | `0` |
| Reward item count | `0` |
| Target vnums | `1130` (ridge wolf), `1141` (tollbreak enforcer), `1136` (cairn lurker), `1144` (cave squatter), `1135` (plains stalker) |

**Line 2:** `13 1 5 0 60 0 6000 6 0 0`
**Line 3:** `1130 1141 1136 1144 1135`

**Accept text:** Kowloon's covenant registrar has commissioned a full corridor threat assessment spanning the entire length of Roc Road. Diplomatic and commercial traffic flowing from Kowloon through Midgaard and westward to Kiess depends on every segment of this road remaining traversable. Engage one representative threat from each major zone — hills predators, river tollbreak crews, ruin hostiles, cave smugglers, and coastal stalkers — and report the engagement conditions. This is the most comprehensive road survey Kowloon has commissioned since the post-Evermeet reconstruction, and the registrar wants field data from every band.

**Completion text:** The full-road assessment is complete and Kowloon's covenant registrar now has field-verified threat data for every major segment of Roc Road. This is the first end-to-end corridor survey since the Ash-and-Iron Decades, and it confirms that the road is traversable but requires sustained investment in escort infrastructure. The registrar has forwarded copies to both Midgaard's Magistrate Ledgerhouse and Kiess's Syndic Council, recommending a three-city maintenance compact — the first formal acknowledgment that Kowloon considers Roc Road part of its own supply chain rather than merely a Midgaard-Kiess bilateral concern.

---

### Quest Summary Table

| ID | Title | Type | Offerer | Prereq | Min Lvl | Chain |
|----|-------|------|---------|--------|---------|-------|
| 6 | Road predator cull | Kill variety (3) | Midgaard (931) | None | 15 | — |
| 7 | Tollbreak crew interdiction | Kill count (6) | Midgaard (931) | None | 30 | Chain start |
| 8 | Shadow trade disruption | Kill variety (3) | Midgaard (931) | Prop 7 | 40 | Chains from 7 |
| 9 | Convoy route security | Kill count (8) | Kiess (3340) | None | 20 | — |
| 10 | Greenveil Spur reconnaissance | Kill variety (2) | Kiess (3340) | None | 35 | Chain start |
| 11 | Inlet smuggler crackdown | Kill count (10) | Kiess (3340) | Prop 10 | 50 | Chains from 10 |
| 12 | Ruin custodian suppression | Kill variety (4) | Midgaard (931) | None | 50 | — |
| 13 | Coastal hazard survey | Kill variety (3) | Kowloon (TBD) | None | 40 | Chain start |
| 14 | Transcontinental corridor assessment | Kill variety (5) | Kowloon (TBD) | Prop 13 | 60 | Chains from 13 |

**Chain structure:**
- **Midgaard chain:** 7 (Tollbreak Crew) → 8 (Shadow Trade Disruption)
- **Kiess chain:** 10 (Greenveil Recon) → 11 (Inlet Crackdown)
- **Kowloon chain:** 13 (Coastal Survey) → 14 (Full-Road Assessment)
- **Standalone:** 6 (Predator Cull), 9 (Convoy Route Security), 12 (Ruin Suppression)

**Thematic distribution:**
- Midgaard quests focus on law enforcement, infrastructure protection, and Reckoning-faction academic access
- Kiess quests focus on convoy commerce, route redundancy, and syndic insurance logistics
- Kowloon quests focus on diplomatic courier routes, full-corridor visibility, and transcontinental supply chain integrity

---

## Deliverables for Build Phase
1. Rewrite `#ROOMS` entries for all vnums `1117-1215` in ascending sequential order.
2. Add complete `#MOBILES` roster with faction tagging and level bands (vnums `1117-1152`).
3. Add complete `#OBJECTS` roster aligned to encounter ecology (vnums `1117-1150`+).
4. Replace `#RESETS` with full population, equipment, loot, and door cadence.
5. Add `#SPECIALS` for road warden (`spec_guard`) and Toll-Reeve (`spec_cast_undead`).
6. No `#SHOPS` section (wilderness corridor, no vendors).
7. No `#OBJFUNS` section planned unless playtesting warrants.
8. Terminate with `#$`.
9. Follow canonical section order: `#AREA`, `#ROOMS`, `#MOBILES`, `#OBJECTS`, `#RESETS`, `#SPECIALS`, `#$`.
10. Validate parse and policy compliance from `docs/area_file_spec.md`.
11. Verify all vnums within `1117-1215` range.
12. Verify all external links preserved (see External Link Inventory).
13. Verify no vnum mentions in any description text.
14. Create static quest files `6.prop` through `14.prop` in `quests/` directory per the Quests Plan.
15. Run `cd src && make unit-tests` to validate.
