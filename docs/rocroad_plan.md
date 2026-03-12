# Area Plan: Roc Road

## Overview
- **Area Name:** `@@bRoc Road @@Wand the @@GForest Path@@N`
- **File Name:** `rocroad.are`
- **Keyword:** `rocroad`
- **Intended Levels:** 10-120 (high-traffic overland corridor with scalable threats)
- **Vnum Range:** `3201-3299`
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
- `V 3201 3299`
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
3. `V 3201 3299` defines the vnum envelope; all authored rooms, mobiles, and objects must stay inside this range.
4. Remove `R stephen~` and `W stephen~` ACL directives from the legacy header; set ACLs at implementation time if needed.

### Vnum Allocation
5. Vnums for rooms, mobiles, and objects must be assigned in ascending sequential order with no gaps.
6. Fill existing gaps in the vnum range before assigning new higher vnums.
7. For rooms, attempt to use all room vnums in the assigned range (fill every available slot where practical). The existing file uses all 99 vnums but in non-sequential order; the rebuild must assign vnums `3201-3299` in ascending order.

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
47. Follow canonical saver order: `#AREA`, `#ROOMS`, `#MOBILES`, `#MOBPROGS`, `#OBJECTS`, `#SHOPS`, `#RESETS`, `#SPECIALS`, `#OBJFUNS`, `#$`.

### Specials Policy
48. Mob specials beginning with `spec_summon_` must never be set in area files (runtime-only).
49. Mob specials beginning with `spec_keep` may only be assigned manually by a human.

---

## External Link Inventory (existing connections to preserve)

The current `rocroad.are` has the following external exits that **must be preserved** in the rebuild (the room vnums they attach to will change as part of the sequential renumbering, but the connectivity must remain):

| Current Vnum | Direction | Destination Vnum | Destination Area | Notes |
|---|---|---|---|---|
| 3230 | east | 3041 | Midgaard | **East terminus** — Midgaard West Gate link |
| 3234 | north | 11200 | Umbra Heartspire | Branch link north (close to Midgaard) |
| 3238 | down | 610 | External area | Vertical link (sub-road access) |
| 3240 | east | 9601 | *(removed — Forest of Confusion connects via Kiess north gate, not Roc Road)* | ~~Branch to Forest of Confusion~~ |
| 3242 | east | 3401 | External area | Branch link east |
| 3243 | north | 13095 | Kiess | **West terminus** — Kiess South Gate link |
| 3249 | north | 3290 | Roc Road (internal) | Stone path branch |
| 3250 | up | 7200 | External area | Vertical link (elevated access) |
| 3259 | north | 5083 | External sea area | Sea route link |
| 3291 | north | 4449 | External area | Building/structure link |

### New External Links (added in rebuild)

| New Room | Direction | Destination Vnum | Destination Area | Notes |
|---|---|---|---|---|
| *(Band A)* | north | 30250 | Cathedral of the Violet Eclipse | Branch link north (very close to Midgaard) |

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
Roc Road remains a longitudinal journey but is rebuilt as nine major bands plus connector micro-zones. All rooms use ascending sequential vnums `3201-3299` with no gaps.

### Band A — Midgaard Gate March (3201-3211, 11 rooms)
- **Sector types:** `city` (1) for the first 2 rooms nearest Midgaard gate; `field` (2) for the remaining road through settled farmland.
- **Room flags:** `0` for most; the east terminus room (3201) is an important room (5+ sentences).
- Stone-set roadway under Midgaard charter markers.
- Guard relays, tax cairns, and old civic inscriptions.
- Encounter emphasis: patrols, petitioners, minor bandit scouts, courier traffic.
- **External links:**
  - Room 3201 connects east to `3041` (Midgaard West Gate). This is the canonical east terminus of Roc Road.
  - One room in this band connects north to `30250` (Cathedral of the Violet Eclipse). Very close to Midgaard — a shadow-liturgical cathedral visible from the road's first stretch.

### Band B — Banner Hills and Switchbacks (3212-3219, 8 rooms)
- **Sector types:** `hills` (4) for hill-climb rooms; `mountain` (5) for the high pass and switchback rooms.
- **Room flags:** `0` for most.
- Steeper terrain, signal poles, weather-exposed pass points.
- Visual language shifts from civic to frontier military.
- Encounter emphasis: rockfall hazards, hill raiders, watch outriders.

### Band C — River Ledger Crossing (3220-3227, 8 rooms)
- **Sector types:** `field` (2) for approach rooms; `water_swim` (6) for the ford room itself; `field` (2) for the far bank.
- **Room flags:** `0` for most.
- Ford/bridge complex with customs past and present layered together.
- Introduces merchant guild tension and smuggling subtext.
- Encounter emphasis: ferrymen, toll ghosts, river predators, contraband runners.

### Band D — Dustward Verge (3228-3234, 7 rooms)
- **Sector types:** `desert` (10) for arid stretches; `field` (2) for transitional rooms.
- **Room flags:** `0` for most.
- Drier transition where road maintenance degrades.
- Old milestone liturgy from pre-Kiess polities resurfaces.
- Encounter emphasis: scavengers, relic pickers, heat-maddened fauna.
- **External link:** Room at end of this band connects north to `11200` (Umbra Heartspire). The cathedral-fortress is visible from the road as a dark silhouette against the hills — close enough to Midgaard's jurisdiction that its original charter came from Midgaard's archive system.

### Band E — Crossroads Compact (3235-3243, 9 rooms)
- **Sector types:** `field` (2) for road rooms; `forest` (3) for rooms near the forest branch.
- **Room flags:** `0` for most; crossroads hub room is important (5+ sentences). Boss encounter room flagged `no_mob` (4).
- The major strategic intersection where branch routes diverge.
- Rebuilt as a political pressure node with evidence of overlapping jurisdiction.
- Encounter emphasis: diplomatic envoys, mercenary escorts, opportunist thieves.
- **External links preserved from this band:**
  - North to `11200` (Umbra Heartspire, from former 3234)
  - Down to `610` (vertical sub-road access, from former 3238)
  - East to `3401` (branch link east, from former 3242)
  - North to `13095` (Kiess South Gate, from former 3243) — this is the **canonical west terminus** of Roc Road.
- **Note:** The former `3240 east → 9601` (Forest of Confusion) link is **removed** in the rebuild. The Forest of Confusion connects via Kiess's north gate, not directly from Roc Road.

### Band F — Greenveil Spur (3244-3252, 9 rooms)
- **Sector types:** `forest` (3) for all rooms in the forest spur.
- **Room flags:** `0` for most; boss encounter room flagged `no_mob` (4).
- Forest-side path that once served as hidden troop and refugee movement line.
- Becomes a lore-rich optional path with druidic and exile traces.
- Encounter emphasis: foresters, displaced spirits, poachers, herbalists.
- **External links preserved from this band:**
  - North to internal stone path rooms (former 3290-3291)
  - Up to `7200` (elevated access, from former 3250)
  - North to `4449` (building/structure link, from former 3291)

### Band G — Inlet, Cave, and Sinkline (3253-3261, 9 rooms)
- **Sector types:** `water_swim` (6) for shallow water rooms; `water_noswim` (7) for deep water rooms; `desert` (10) for beach rooms; `inside` (11) for cave/sinkhole rooms.
- **Room flags:** `0` for water and beach; `dark` (1) for cave/sinkhole interiors. Boss encounter room flagged `no_mob` (4).
- Water-edge and cliff subzone with salvage economy and old smuggler pockets.
- Encounter emphasis: shoreline hunters, cave squatters, tidebound anomalies.
- **External links preserved from this band:**
  - North to `5083` (sea route link, from former 3259)
- **Door preserved:** Iron door between beach and cave rooms (reset to closed, state 1, matching existing reset).

### Band H — Ruin Run and Weathered Causeway (3262-3274, 13 rooms)
- **Sector types:** `desert` (10) for arid wasteland rooms; `field` (2) for ruin and post-ruin rooms.
- **Room flags:** `0` for most; boss encounter room flagged `no_mob` (4).
- Ancient city fragments integrated into roadbed over centuries.
- Strongest "deep time" lore in the route.
- Encounter emphasis: ruin custodians, script-haunted echoes, relic caravans.

### Band I — Western Shore Reach (3275-3299, 25 rooms)
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
- Use `3201-3299` as the mobile envelope (parallel namespace), with ~36 active mob templates and variant reset mixes.
- Vnums assigned in ascending sequential order with no gaps, starting at `3201`.

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
| 3201 | a Midgaard road warden | 30 | is_npc, sentinel, stay_area | A | Gate-adjacent patrol; `spec_guard` |
| 3202 | a Midgaard road clerk | 20 | is_npc, stay_area | A | Tax and census mark recorder |
| 3203 | a Midgaard road warden captain | 50 | is_npc, sentinel, stay_area, solo | A-B | Senior patrol officer |
| 3204 | a charter inscription keeper | 25 | is_npc, sentinel, stay_area | A | Maintains legal markers |

#### Faction 2: Kiess Convoy Syndics (non-aggressive, Bands D-E and road-wide)

| Vnum | Name | Level | Act Flags | Band | Notes |
|------|------|-------|-----------|------|-------|
| 3205 | a Kiess convoy marshal | 35 | is_npc, stay_area | E | Caravan leader |
| 3206 | a Kiess quarter agent | 25 | is_npc, stay_area | D-E | Supply and risk accountant |
| 3207 | a Kiess route ledger scribe | 20 | is_npc, sentinel, stay_area | E | Records debt and guard obligations |
| 3208 | a Kiess convoy guard | 40 | is_npc, stay_area | D-E | Armed escort |

#### Faction 3: Independent Wayfolk (non-aggressive, road-wide)

| Vnum | Name | Level | Act Flags | Band | Notes |
|------|------|-------|-----------|------|-------|
| 3209 | a road drover | 15 | is_npc, stay_area | A-D | Animal handler |
| 3210 | a peddler of the road | 18 | is_npc, stay_area | B-E | Traveling merchant |
| 3211 | a waystone shrine keeper | 22 | is_npc, sentinel, stay_area | C-D | Maintains roadside shrines |
| 3212 | a river ferryman | 20 | is_npc, sentinel, stay_area | C | River crossing operator |
| 3213 | a road-camp cook | 15 | is_npc, stay_area | B-D | Campsite laborer |

#### Faction 4: Frontier Predators (aggressive, band-appropriate levels)

| Vnum | Name | Level | Act Flags | Band | Notes |
|------|------|-------|-----------|------|-------|
| 3214 | a ridge wolf | 25 | is_npc, aggressive, stay_area | B | Hill predator |
| 3215 | a ridge cat | 35 | is_npc, aggressive, stay_area | B-C | Ambush predator |
| 3216 | a river ambusher snake | 20 | is_npc, aggressive, stay_area | C | Waterside threat |
| 3217 | a dust scavenger bird | 30 | is_npc, aggressive, stay_area | D-H | Carrion predator |
| 3218 | a coastal carrion gull | 35 | is_npc, aggressive, stay_area | I | Shore predator |
| 3219 | a plains stalker | 40 | is_npc, aggressive, stay_area | H-I | Open-ground hunter |

#### Faction 5: Ruin-Touched Hostiles (aggressive, Bands F-H)

| Vnum | Name | Level | Act Flags | Band | Notes |
|------|------|-------|-----------|------|-------|
| 3220 | a cairn lurker | 50 | is_npc, aggressive, stay_area | H | Haunts ancient ruins |
| 3221 | a script-blind revenant | 60 | is_npc, aggressive, stay_area, undead | H | Animated by old law magic |
| 3222 | a relic zealot | 45 | is_npc, aggressive, stay_area | H | Ideologue using antique script myths |
| 3223 | a ruin custodian shade | 55 | is_npc, aggressive, stay_area, undead | H | Guardian of old structures |
| 3224 | a displaced forest spirit | 40 | is_npc, aggressive, stay_area | F | Sanctuary violation specter |

#### Faction 6: Shadow Trade Cells (aggressive, Bands C-G)

| Vnum | Name | Level | Act Flags | Band | Notes |
|------|------|-------|-----------|------|-------|
| 3225 | a tollbreak enforcer | 40 | is_npc, aggressive, stay_area | C-E | Violent extraction crew |
| 3226 | a contraband runner | 30 | is_npc, aggressive, stay_area | C-G | Smuggler |
| 3227 | a forged-seal courier | 25 | is_npc, aggressive, stay_area | E | Falsified legal documents |
| 3228 | a cave squatter | 35 | is_npc, aggressive, stay_area | G | Tide-cave holdout |

#### Boss/Elite Encounters (4 bosses, aggressive, sentinel, boss)

| Vnum | Name | Level | Act Flags | Band | Room | Notes |
|------|------|-------|-----------|------|------|-------|
| 3229 | the Toll-Reeve Without Crown | 90 | is_npc, sentinel, aggressive, stay_area, boss, no_flee | E | no_mob room | Crossroads legal revenant; undead; `spec_cast_undead` |
| 3230 | the Greenveil Antler-Warden | 85 | is_npc, sentinel, aggressive, stay_area, boss, no_flee | F | no_mob room | Forest spirit-warrior hybrid |
| 3231 | the Inlet Chainmaster | 80 | is_npc, sentinel, aggressive, stay_area, boss, no_flee | G | no_mob room | Smuggler captain in tide cave |
| 3232 | the Promontory Harrower | 95 | is_npc, sentinel, aggressive, stay_area, boss, no_flee | I | no_mob room | Coastal apex predator threat |

#### Ambient Flavor Mobs (non-aggressive)

| Vnum | Name | Level | Act Flags | Band | Notes |
|------|------|-------|-----------|------|-------|
| 3233 | a road wagon | 10 | is_npc, stay_area | A-E | Background traffic |
| 3234 | a mule train driver | 12 | is_npc, stay_area | B-D | Background traffic |
| 3235 | a road-weary pilgrim | 10 | is_npc, stay_area | D-I | Stranded traveler |
| 3236 | a ruin scavenger | 20 | is_npc, stay_area | H | Non-hostile relic picker |

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
- Use `3201-3299` as the object envelope, target ~42 objects with progression tiers.
- Vnums assigned in ascending sequential order with no gaps, starting at `3201`.

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
| 3201 | a roadwarden badgeplate | armor | body, take | 11 | 25 | Tank; patrol identification |
| 3202 | a signal cloak of the road watch | armor | about, take | 6 | 30 | Melee; wind-resistant travel cloak |
| 3203 | reinforced warden boots | armor | feet, take | 11 | 35 | Tank; road-rated footwear |
| 3204 | a toll rod of Midgaard charter | weapon | hold, take | 6 | 40 | Melee; value3=7 (pound); legal authority weapon |
| 3205 | a road warden patrol lantern | light | take | 3 | 20 | Caster; wayfinding light |
| 3206 | a charter inscription rubbing | trash | take | 1 | 15 | Caster; lore object with E extra desc |

#### Convoy Gear (Bands D-E, levels 25-55)

| Vnum | Name | Item Type | Wear | Weight | Level | Notes |
|------|------|-----------|------|--------|-------|-------|
| 3207 | a Kiess trade ledger | trash | take | 2 | 30 | Caster; lore object with E extra desc |
| 3208 | a convoy lockbox | container | take | 8 | 35 | Melee; trade container |
| 3209 | a mule tack harness | armor | waist, take | 7 | 25 | Melee; utility gear |
| 3210 | a lantern standard pole | weapon | hold, take | 8 | 45 | Melee; value3=7 (pound); improvised weapon |
| 3211 | a syndic quarter-agent seal | treasure | take | 2 | 40 | Caster; trade authority token |
| 3212 | a convoy guard shield | armor | hold, take | 13 | 50 | Tank; shield archetype |

#### Forest Relics (Band F, levels 30-55)

| Vnum | Name | Item Type | Wear | Weight | Level | Notes |
|------|------|-----------|------|--------|-------|-------|
| 3213 | a bark-charm of the old verge | treasure | take | 1 | 35 | Caster; druidic trace relic |
| 3214 | a branch fetish of the spur | treasure | take | 2 | 40 | Caster; forest spirit ward |
| 3215 | a herb satchel of the greenveil | container | take | 3 | 30 | Caster; medicinal herb pouch |
| 3216 | a vow-ribbon of the sanctuary | armor | neck, take | 1 | 45 | Caster; exile community token |
| 3217 | a moss-wrapped druid staff | weapon | hold, take | 4 | 55 | Caster; value3=8 (crush); two-handed (ITEM_TWO_HANDED) |

#### Ruin Salvage (Band H, levels 40-70)

| Vnum | Name | Item Type | Wear | Weight | Level | Notes |
|------|------|-----------|------|--------|-------|-------|
| 3218 | an etched stone tablet | trash | take | 3 | 45 | Caster; lore object with E extra desc (old tax records) |
| 3219 | a weathered bronze coin | treasure | take | 1 | 40 | Caster; pre-charter currency |
| 3220 | a fractured sealstone | treasure | take | 2 | 50 | Caster; broken authority seal |
| 3221 | a ruin-script amulet | armor | neck, take | 3 | 60 | Caster; ancient script pendant |
| 3222 | a causeway flagstone shard | weapon | hold, take | 9 | 55 | Melee; value3=8 (crush); improvised weapon |
| 3223 | a script-etched bone knife | weapon | hold, take | 5 | 65 | Caster; value3=2 (stab); relic blade |

#### Coastal Finds (Band I, levels 35-65)

| Vnum | Name | Item Type | Wear | Weight | Level | Notes |
|------|------|-----------|------|--------|-------|-------|
| 3224 | a salt-cured leather vest | armor | body, take | 7 | 40 | Melee; coastal wear |
| 3225 | a barnacled cutlass | weapon | hold, take | 8 | 50 | Melee; value3=3 (slash); corroded naval blade |
| 3226 | a ropework climbing harness | armor | waist, take | 7 | 35 | Melee; cliff-side utility |
| 3227 | a sea-glass focus shard | treasure | hold, take | 2 | 55 | Caster; magical focus |
| 3228 | a driftwood buckler | armor | hold, take | 12 | 45 | Tank; buckler (ITEM_BUCKLER); salvaged shield |
| 3229 | a privateer signal horn | treasure | take | 4 | 60 | Caster; old maritime call |

#### Boss Signature Drops (ITEM_LOOT + ITEM_BOSS, one per elite)

| Vnum | Name | Item Type | Wear | Weight | Level | Extra Flags | Notes |
|------|------|-----------|------|--------|-------|-------------|-------|
| 3230 | the Toll-Reeve's unsealed warrant | armor | hold, take | 3 | 90 | ITEM_LOOT, ITEM_BOSS | Caster; legal revenant's authority document; shield archetype |
| 3231 | the Antler-Warden's crown of thorns | armor | head, take | 5 | 85 | ITEM_LOOT, ITEM_BOSS | Caster; forest spirit headpiece |
| 3232 | the Chainmaster's tide-iron shackle | armor | wrist, take | 9 | 80 | ITEM_LOOT, ITEM_BOSS | Melee; smuggler captain's symbol |
| 3233 | the Harrower's storm-bone talisman | armor | neck, take | 4 | 95 | ITEM_LOOT, ITEM_BOSS | Caster; apex predator trophy |

#### Utility and Key Items

| Vnum | Name | Item Type | Wear | Weight | Level | Notes |
|------|------|-----------|------|--------|-------|-------|
| 3234 | a rusted iron cave key | key | take | 1 | 1 | Key for iron door in Band G cave (if door is set to locked) |

#### Reserved Object Vnums
- **3235-3299:** Unallocated (65 vnums). Reserved for additional encounter drops, quest items, shop stock (if a waystation shop is added), and per-band flavor objects added during implementation.

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

**Band A (3201-3211): Midgaard Gate March**
- 2x road warden (3201), limit 3, in gate-adjacent rooms (sentinel)
- 1x road clerk (3202), limit 2
- 1x road warden captain (3203), limit 1 (solo)
- 1x charter keeper (3204), limit 1 (sentinel)
- 2x road drover (3209), limit 3
- 1x road wagon (3233), limit 2

**Band B (3212-3219): Banner Hills**
- 2x ridge wolf (3214), limit 3
- 1x ridge cat (3215), limit 2
- 1x peddler (3210), limit 2
- 1x road-camp cook (3213), limit 2

**Band C (3220-3227): River Crossing**
- 1x ferryman (3212), limit 1 (sentinel)
- 1x shrine keeper (3211), limit 1 (sentinel)
- 1x river ambusher snake (3216), limit 2
- 1x tollbreak enforcer (3225), limit 2
- 1x contraband runner (3226), limit 2

**Band D (3228-3234): Dustward Verge**
- 1x Kiess quarter agent (3206), limit 2
- 1x dust scavenger bird (3217), limit 3
- 1x road-weary pilgrim (3235), limit 2
- 1x mule train driver (3234), limit 2

**Band E (3235-3243): Crossroads Compact**
- 1x Kiess convoy marshal (3205), limit 1
- 1x Kiess route ledger scribe (3207), limit 1 (sentinel)
- 1x Kiess convoy guard (3208), limit 2
- 1x forged-seal courier (3227), limit 2
- 1x Toll-Reeve Without Crown (3229), limit 1 (boss, in no_mob room)

**Band F (3244-3252): Greenveil Spur**
- 1x displaced forest spirit (3224), limit 2
- 1x contraband runner (3226), limit 1
- 1x Greenveil Antler-Warden (3230), limit 1 (boss, in no_mob room)

**Band G (3253-3261): Inlet and Cave**
- 1x cave squatter (3228), limit 2
- 1x coastal carrion gull (3218), limit 2
- 1x Inlet Chainmaster (3231), limit 1 (boss, in no_mob room)

**Band H (3262-3274): Ruin Run**
- 2x cairn lurker (3220), limit 3
- 1x script-blind revenant (3221), limit 2
- 1x relic zealot (3222), limit 2
- 1x ruin custodian shade (3223), limit 2
- 1x ruin scavenger (3236), limit 2

**Band I (3275-3299): Western Shore**
- 2x coastal carrion gull (3218), limit 3
- 1x plains stalker (3219), limit 2
- 1x road-weary pilgrim (3235), limit 2
- 1x Promontory Harrower (3232), limit 1 (boss, in no_mob room)

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
- **3201-3220:** lower pressure, high traffic, social/travel readability.
- **3221-3250:** moderate pressure, mixed social/hostile density.
- **3251-3275:** elevated pressure with environmental storytelling.
- **3276-3299:** high volatility optional coast-end challenge, but still traversable with caution.

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
| 3201 | spec_guard | Road warden — reacts to criminal-flagged players |
| 3229 | spec_cast_undead | Toll-Reeve Without Crown — boss undead caster |

Notes:
- Most mobs do not need spec functions; their behavior is driven by `act` flags (aggressive, sentinel, etc.).
- Predators and bandits use the `aggressive` flag for combat initiation rather than spec functions.
- The Greenveil Antler-Warden, Inlet Chainmaster, and Promontory Harrower are melee-focused bosses and do not require casting specs.
- `spec_summon_*` functions must never be set in area files (runtime-only).
- `spec_keep*` functions may only be assigned manually by a human.

---

## Deliverables for Build Phase
1. Rewrite `#ROOMS` entries for all vnums `3201-3299` in ascending sequential order.
2. Add complete `#MOBILES` roster with faction tagging and level bands (vnums `3201-3236`).
3. Add complete `#OBJECTS` roster aligned to encounter ecology (vnums `3201-3234`+).
4. Replace `#RESETS` with full population, equipment, loot, and door cadence.
5. Add `#SPECIALS` for road warden (`spec_guard`) and Toll-Reeve (`spec_cast_undead`).
6. No `#SHOPS` section (wilderness corridor, no vendors).
7. No `#OBJFUNS` section planned unless playtesting warrants.
8. Terminate with `#$`.
9. Follow canonical section order: `#AREA`, `#ROOMS`, `#MOBILES`, `#OBJECTS`, `#RESETS`, `#SPECIALS`, `#$`.
10. Validate parse and policy compliance from `docs/area_file_spec.md`.
11. Verify all vnums within `3201-3299` range.
12. Verify all external links preserved (see External Link Inventory).
13. Verify no vnum mentions in any description text.
14. Run `cd src && make unit-tests` to validate.
