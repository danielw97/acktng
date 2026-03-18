# Area Plan: Sepulchur Pasture (Full Rebuild Within Existing Vnum Envelope)

## Overview

- **Area Name:** `@@WThe @@dSepulchur @@pPasture@@N`
- **Area File (existing):** `area/sepulcher_pasture.are`
- **Keyword:** `sepulchur sepulcher pasture gravebell funerary tithe`
- **Assigned Vnum Range (must remain unchanged):** **5550-5649**
- **Target Level Band:** **15-30** (preserved)
- **Reset Rate:** 10 (preserved)
- **Owner:** `Virant` (normalize header ownership casing)

This plan fully replaces the current repetitive lane-grid presentation with a coherent funerary-agricultural district tied directly to Midgaard’s civic burial bureaucracy, Lantern Reform logistics, and Violet Compact containment doctrine.

---

## Spec-First Constraints (from `area_file_spec.md`)

### Area Header
1. `Q 16` directive is mandatory.
2. Owner line must be `O Virant~` (area header owner policy — note the existing file uses lowercase `virant`; must normalize to `Virant`).
3. `V 5550 5649` defines the vnum envelope; all authored rooms, mobiles, and objects must stay inside this range.

### Vnum Allocation
4. Vnums for rooms, mobiles, and objects must be assigned in ascending sequential order with no gaps.
5. Fill existing gaps in the vnum range before assigning new higher vnums.
6. For rooms, attempt to use all room vnums in the assigned range (fill every available slot where practical).
7. Cross-type overlap is allowed: a room, mobile, and object may share the same numeric vnum.

### String and Description Rules
8. All text fields terminated by `~`. Strings must not contain back-to-back newlines (`\n\n`).
9. Vnums must never appear in any in-world description text (room descriptions, mob descriptions, object descriptions, extra descriptions, exit descriptions).
10. Mobile `long_descr` must be exactly one text line followed by a newline and a line containing only `~`. Multi-line `long_descr` is never valid.
11. Mobile `description` must end with exactly one newline immediately before the terminating `~`.
12. Object extra-description `<description>` must end with exactly one newline before `~`.
13. Room `<description>~` must end with exactly one trailing newline before `~`.

### Color Policy
14. Use intentional inline color coding with the defined Lantern Funeral Spectrum palette (see below); avoid forbidden thematic black (`@@k`).
15. Do not use background color codes (`@@0`–`@@7`) or flashing (`@@f`) unless explicitly added by a human author.

### Room Constraints
16. Each room description must contain at least 3 sentences; important rooms (zone anchors, boss rooms, crossroads) must contain at least 5 sentences.
17. Each room description must be unique; no identical descriptions across rooms.
18. Exits must be bi-directional: if room A connects to room B via direction X, room B must provide the opposite-direction exit back to A.
19. When a room uses a named exit (non-empty `<exit_keyword>`), the exit name must appear in the room's main description, an object that spawns in the room, or an `E` extra description.
20. Extra-description keyword chains must be anchored: at least one keyword in each chain must appear in the room's main description.
21. Named exit keywords for door-style exits must be prefixed with `^` (e.g., `^iron gate`).
22. Any exit set to `closed` or `locked` via `#RESETS` command `D` must have `EX_ISDOOR` (`1`) set in its room `D<door>` definition.
23. If a door is reset to `locked` (state `2`), `<key_vnum>` must reference a valid key object, and that key must exist in `#OBJECTS`.
24. Rooms that are part of a maze must be flagged `ROOM_MAZE` (`524288`). This area is not expected to contain mazes.
25. Boss mob rooms must be flagged `no_mob` (`4`).

### Mobile Constraints
26. All mobs must have `stay_area` (`64`) set so they do not wander out of the area.
27. Stationary mobs (gate guards, clerks, named NPCs) must also have `sentinel` (`2`) set.
28. Boss mobs must be flagged `sentinel` (`2`) and `boss` (`67108864`) and placed only in rooms flagged `no_mob` (`4`).
29. Strong (non-boss) named mobs must be flagged `solo` (`33554432`).
30. The `invasion` flag must never be set by builders; it is runtime-only.
31. The `aggressive` flag should be set only for mobs whose intended behavior is to initiate combat on sight (hostile undead, feral creatures), not for townsfolk, clerks, or quest givers.

### Object Constraints
32. Every object must include `ITEM_TAKE` (`8388608`) in wear flags.
33. No object may include `ITEM_WEAR_CLAN_COLORS` (`16777216`).
34. Object `name`, `short_descr`, and `description` must be thematically consistent with wear flags.
35. Object names must be unique within the area file; no two objects may share the same `<name>~` value.
36. Do not define fixed stats in area files; stats are generated at runtime.
37. Object weight encodes archetype: `1`–`5` caster, `6`–`10` melee, `11`–`15` tank.
38. `ITEM_GENERATED` (`1`) must not be set in area files; it is runtime-managed.
39. Items from boss mob loot tables must include `ITEM_BOSS` (`134217728`).
40. Items spawned from mob loot tables (`l`/`L` extension) must have `ITEM_LOOT` (`67108864`) set.
41. Weapons must have wear flags including both `hold` (`32768`) and `take` (`8388608`). Two-handed weapon names must include the `ITEM_TWO_HANDED` (`2147483648`) extra flag.
42. `value3 = 0` (`hit`) for weapons is only valid with the `ITEM_FIST` extra flag.
43. `value3` (weapon attack type) must be thematically consistent with the weapon's name and description.

### Reset Constraints
44. Resets must reference valid vnums for the target type required by the command.
45. `#RESETS` must not contain blank lines.
46. If a door is reset to `locked` via `D` state `2`, `<key_vnum>` must be set to a valid key object vnum (not `-1`), and that key object must exist in `#OBJECTS`.

### Section Order
47. Follow canonical saver order: `#AREA`, `#ROOMS`, `#MOBILES`, `#OBJECTS`, `#SHOPS`, `#RESETS`, `#SPECIALS`, `#OBJFUNS`, `#$`.

### Specials Policy
48. Mob specials beginning with `spec_summon_` must never be set in area files (runtime-only).
49. Mob specials beginning with `spec_keep` may only be assigned manually by a human.

---

## Multi-Color Theme Bible: “Lantern Funeral Spectrum”

- `@@W` **Civic White**: Midgaard registry tablets, lawful process, carved decrees.
- `@@y` **Lantern Gold**: processional roads, waystones, public vigil lights.
- `@@d` **Dark Grey**: grave earth, weathered stone, ossuary walls.
- `@@p` **Purple**: oath-rites, Violet Compact seals, controlled necromancy.
- `@@a` **Light Cyan**: spirit vapor, memory wisps, sanctified ward-lines.
- `@@G` **Green**: remnant pastoral life, tithe fields, living memory of harvest.
- `@@N` **Reset**: always terminate colorized fragments.

### Theme Usage Rules

- Room titles: max two accent colors.
- Descriptions: color only on symbolic anchors (lanterns, seals, roots, tablets, bells).
- Midgaard civic references should favor `@@W` + `@@y`.
- Compact/occult pressure rooms should pivot to `@@p` + `@@a`.
- Pasture/field remnants should always include a `@@G` motif.

---

## Narrative Redirection

Sepulchur Pasture becomes Midgaard’s **outer funerary provisioning district**: historically a grain-and-cattle belt converted over generations into a burial-transit complex when wars, plague years, and dungeon attrition outpaced temple crypt capacity.

The rebuild centers on three civic functions:

1. **Receive** the dead from roads, gates, and caravans.
2. **Record** identity, debt, rank, and rite eligibility under Midgaard law.
3. **Inter or Hold** the dead in sanctified plots, warded vaults, or Compact quarantine pens.

Corruption enters where policy hardened into exploitation: tithe agents and gravewardens now prolong unrest to protect status, fees, and relic trafficking.

---

## External World Links (Preserved)

- `946 -> 5106` (Midgaard south-gate connector to The Withered Gate; return via `5106 up -> 946`)

This link is already implemented in both areas and must be preserved exactly.

---

## Vnum Allocation (Complete 75-Slot Envelope)

Cross-type overlap is allowed per spec §13: a room, mobile, and object may share the same numeric vnum as long as each remains unique within its own index type. Per §13.1, rooms should attempt to fill every available vnum slot.

- **Rooms:** `5550-5649` (75 rooms — fills the entire envelope)
- **Mobiles:** `5106-5127` (22 mobs — overlaps room vnums, which is valid)
- **Objects:** `5128-5145` (18 objects — overlaps room vnums, which is valid)

---

## Full Layout Redesign (75 Rooms)

### Zone 1: Gate-Tithe Forefield (`5106-5120`) — 15 rooms
Purpose: onboarding zone where Midgaard death-carts enter, are weighed, inspected, and taxed. Introduces the civic-funerary tone and the Last Burden Tithe process.

- **5106** The Withered Gate (external linkage to Midgaard `946` preserved; `up` returns to Midgaard)
- **5107** Cart Weighbridge of Last Burdens
- **5108** Lantern Docket Post
- **5109** Salt-Blessed Furrow Bend
- **5110** Mourners’ Queue Rail
- **5111** Gravebell Milestone (major crossroad; connects to Zone 2 and Zone 3)
- **5112** Toll of the Ninth Peal
- **5113** Tariff Clerk’s Alcove
- **5114** Salted Cart Yard
- **5115** Mourner’s Rest Bench
- **5116** Tithe-Bell Staging Lane
- **5117** Ash-Mortar Waystone
- **5118** Corpse-Cart Turnaround
- **5119** Pallbearer’s Crosswalk
- **5120** Ninth Acre Boundary Ditch

### Zone 2: Registry Furrows (`5121-5135`) — 15 rooms
Purpose: legal-religious administration over remains, inheritance marks, and claimant proceedings. Clerks, scribes, and ledger infrastructure dominate.

- **5121** Inkstone of the Unclaimed
- **5122** White Ledger Stakes
- **5123** Oath-Tablet Sluice
- **5124** Pilgrim’s Hollow (clerk/scribe junction; crossroad to Zone 3)
- **5125** Sealpress Arcade
- **5126** Tally of Ashen Sheaves
- **5127** Widow’s Furrows
- **5128** Claimant Queue Passage
- **5129** Deferred-Name Filing Hall
- **5130** Writ-Stamp Corridor
- **5131** Witness Affidavit Booth
- **5132** Ledger Binding Vault
- **5133** Ink-and-Salt Archive
- **5134** Registry Bell Niche
- **5135** Notary’s Overlook

### Zone 3: Reliquary Pasture (`5136-5153`) — 18 rooms
Purpose: old farm fields converted to cenotaph plots, spirit-break hedges, and reliquary gardens. The agricultural past bleeds through funerary repurposing.

- **5136** Barrowmouth Orchard Verge
- **5137** Nightsoil Channel (drainage/tithe conduit)
- **5138** Bleak Orchard of Nameposts
- **5139** Corpse-Lily Terraces
- **5140** Scarecrow Reliquary Rows
- **5141** Bellwind Irrigation Gate
- **5142** Ashen Tithe Path (subzone anchor; crossroad to Zone 2 and Zone 4)
- **5143** Cenotaph Hedgerow
- **5144** Furrow of Unnamed Markers
- **5145** Grain-Ghost Clearing
- **5146** Spirit-Break Fence Line
- **5147** Old Sheaf Altar
- **5148** Cairn-Marked Pasture Edge
- **5149** Rootbound Drainage Gully
- **5150** Bleached Milestone Row
- **5151** Broken Harvest Shrine
- **5152** Fog-Choked Furrow
- **5153** Reliquary Garden Gate

### Zone 4: Compact Holding Grounds (`5154-5168`) — 15 rooms
Purpose: Violet Compact quarantine pens for cursed, plague-touched, or arcane-active dead. Warded perimeter, adjudication spaces, and containment trenches.

- **5154** Violet Stake Circle
- **5155** Wardwire Crossfield
- **5156** Censer Trench of Compliance
- **5157** Pen of Refused Rites
- **5158** Witness Lantern Walk
- **5159** Tribunal Cairn (crossroad to Zone 5)
- **5160** Quietus Chute
- **5161** Compact Patrol Track
- **5162** Quarantine Ditch
- **5163** Ward-Post Perimeter
- **5164** Adjudicator’s Bench
- **5165** Holding Pen Corridor
- **5166** Violet Seal Threshold
- **5167** Containment Censer Row
- **5168** Compact Gate of Final Review

### Zone 5: Sepulcher Core (`5169-5180`) — 12 rooms
Purpose: climax wing with command crypt, bell tower, boss encounters, and egress route. Contains the area’s named encounters and the broken institutional heart of the pasture.

- **5169** Chapel of the Last Acre
- **5170** Bone-Mill Antechamber
- **5171** Ossuary Lift House
- **5172** Bellmaster’s Gallery
- **5173** Reliquary of Deferred Names
- **5174** Heartcrypt of the Pasture (boss room; must be flagged `no_mob`)
- **5175** Lantern Return Gate (unlockable shortcut/egress)
- **5176** Steward’s Vestry
- **5177** Tariff-Seal Sanctum
- **5178** Bell Tower Stair
- **5179** Oath-Wraith Antechamber
- **5180** Pasture Overlook (final room; provides retreat path back to Zone 1)

### Connectivity Rules

- Main forward spine from `5106` through zones in sequence to `5174`.
- Cross-links at `5111` (Zone 1 → Zone 3), `5124` (Zone 2 ↔ Zone 3), `5142` (Zone 3 → Zone 2/Zone 4), and `5159` (Zone 4 → Zone 5) to provide lateral navigation.
- Compact wing (Zone 4) gates final core access (Zone 5) via three assembled legal tokens (ITEM_PIECE mechanic).
- At least one non-hostile retreat route from every high-danger cluster.
- All exits must be bi-directional (spec §8): if room A connects to room B via direction X, room B must provide the opposite-direction exit back to A.
- When a room uses a named exit (non-empty `<exit_keyword>`), that exit name must appear in the room’s main description, an object in the room, or an `E` extra description.
- `5175` Lantern Return Gate provides a shortcut egress back to `5111` Gravebell Milestone after core completion.
- `5180` Pasture Overlook provides a retreat path back to `5120` Ninth Acre Boundary Ditch.

---

## Mobile Redesign Plan (`5106-5127`)

### Population Tiers

#### Tier 1 (Lv 15-19: ambient/entry) — Zone 1 and Zone 2 population
- **5106** toll-bell acolyte (sentinel, non-aggressive; rings intake bells)
- **5107** cart-draft mourner (non-aggressive; ambient grief laborer)
- **5108** ditch lantern-keeper (sentinel, non-aggressive; maintains lane lights)
- **5109** tithe crow swarmling (aggressive; scavenger pest)

#### Tier 2 (Lv 19-23: core pasture pressure) — Zone 2 and Zone 3 population
- **5110** ledger-furrow clerk (sentinel, non-aggressive; registry bureaucrat)
- **5111** scaremask reaper (aggressive; corrupted field worker)
- **5112** corpse-lily groundsman (aggressive; garden-turned-grave tender)
- **5113** ash-sleeved pallbearer (aggressive; undead bearer still carrying)
- **5114** gravebell drudge (aggressive; bell-cursed laborer)

#### Tier 3 (Lv 23-27: specialists/elites) — Zone 3 and Zone 4 population
- **5115** violet compact warden (aggressive; Compact quarantine enforcer)
- **5116** wardwire adjudicator (aggressive; legal-occult judge)
- **5117** oath-ink embalmer (aggressive; corrupted rite-worker)
- **5118** reliquary scytheknight (aggressive; armed reliquary guard; flagged `solo`)
- **5119** pilgrim-claim extortioner (aggressive; toll racketeer; flagged `solo`)

#### Named Lore NPCs / Bosses (Lv 27-30) — Zone 4 and Zone 5 population
- **5120** Marshal Edrin Vale, Gate of Final Tariffs (lawful mini-boss; flagged `sentinel`, `solo`; `spec_guard`)
- **5121** Sister Calvea of the Ninth Peal (ritual caster; flagged `sentinel`, `solo`; `spec_cast_cleric`)
- **5122** Archivist Mor Taland, Keeper of Deferred Names (record-warden caster; flagged `sentinel`, `solo`; `spec_cast_mage`)
- **5123** The Bell-Eaten Steward (mutated final custodian; flagged `sentinel`, `boss`; placed in `no_mob` room `5174`; `spec_cast_undead`)
- **5124** Pasture Heart Wraith (spirit boss tied to broken oaths; flagged `sentinel`, `boss`, `undead`, `no_body`; placed in `no_mob` room `5174`; `spec_cast_undead`)

#### Ambient/Flavor NPCs (non-combat)
- **5125** pasture mourner (sentinel, non-aggressive; weeping civilian flavor)
- **5126** tithe-cart driver (sentinel, non-aggressive; caravan worker)
- **5127** funerary bell-ringer (sentinel, non-aggressive; operates the gravebell)

### Mob Spec Compliance and Placement Rules

**Required act flags for all mobs:**
- Every mob must have `stay_area` (`64`) set.
- Stationary mobs (clerks, named NPCs, sentries) must also have `sentinel` (`2`).
- No mob should have `invasion` set; this is runtime-only.

**Aggressive flag policy:**
- Only hostile undead, feral scavengers, and corrupted workers should be flagged `aggressive` (`32`).
- Clerks, mourners, flavor NPCs, and civic quest givers must not be aggressive.

**Boss and solo policy:**
- Boss mobs (`5123`, `5124`) must have `sentinel` + `boss` flags and be placed in `no_mob`-flagged rooms.
- Strong non-boss named NPCs (`5118`, `5119`, `5120`, `5121`, `5122`) must be flagged `solo` (`33554432`).

**Mobile description format (strict):**
- `long_descr` must be exactly one text line, then a newline, then `~`. Multi-line `long_descr` is never valid.
- `description` must end with exactly one newline before `~`.
- No vnum may appear in any mob description text.

**Mob ecosystem rules:**
- Each zone has a dominant labor caste (porters, clerks, grounds, wardens, custodians).
- Mixed undead/human corruption, not pure undead spam.
- Midgaard institutional language appears in names/titles.
- Replace all generic repeated fillers with role-driven identities.

---

## Object Redesign Plan (`5128-5145`)

### Item Families

1. **Procession Implements** (lantern hooks, toll cords, mourner mantles) — wear locations: `hold`, `about`, `neck`
2. **Registry Instruments** (sealpress signets, ash ledgers, claimant tablets) — wear locations: `hold`, `finger`, `hands`
3. **Pasture Relics** (corpse-lily charms, scaremask talismans, furrow medals) — wear locations: `neck`, `head`, `waist`
4. **Compact Wards** (violet irons, witness censers, compliance bands) — wear locations: `hold`, `wrist`
5. **Boss Signatures** (bell-eaten crook, deferred-name reliquary shard) — wear locations: `hold`; must have `ITEM_BOSS` (`134217728`) if from boss loot tables

### Progression / Utility Objects (ITEM_PIECE System)

The area uses three legal tokens that combine via the `ITEM_PIECE` mechanic (spec §7.5) to form a fused writ. The fused writ is required to pass through the locked gate between Zone 4 and Zone 5.

- **5128** Token of Receipt (`item_type = 29`, ITEM_PIECE)
 - `value0`: `0` (no previous piece)
 - `value1`: `5129` (next piece: Token of Witness)
 - `value2`: `5131` (replacement: Fused Writ of Interment)
- **5129** Token of Witness (`item_type = 29`, ITEM_PIECE)
 - `value0`: `5128` (previous piece: Token of Receipt)
 - `value1`: `5130` (next piece: Token of Interment)
 - `value2`: `5131` (replacement: Fused Writ of Interment)
- **5130** Token of Interment (`item_type = 29`, ITEM_PIECE)
 - `value0`: `5129` (previous piece: Token of Witness)
 - `value1`: `0` (no next piece)
 - `value2`: `5131` (replacement: Fused Writ of Interment)
- **5131** Fused Writ of Interment (`item_type = 18`, ITEM_KEY)
 - Used as `<key_vnum>` for the locked door between `5168` and `5169`.

Token drop assignments:
- Token of Receipt drops from Marshal Edrin Vale (`5120`) or via reset in a Zone 2 room.
- Token of Witness drops from Sister Calvea (`5121`) or via reset in a Zone 3 room.
- Token of Interment drops from Archivist Mor Taland (`5122`) or via reset in a Zone 4 room.

### Equipment Objects

- **5132** mourner's lantern hook (hold; caster weight 1-5; `item_type = 1`, ITEM_LIGHT)
- **5133** toll-cord whip (hold; melee weight 6-10; `item_type = 5`, ITEM_WEAPON; `value3 = 4` whip)
- **5134** sealpress signet ring (finger; caster weight 1-5; `item_type = 8`, ITEM_TREASURE)
- **5135** ash-clerk's ledger tablet (hold; caster weight 1-5; `item_type = 8`, ITEM_TREASURE)
- **5136** corpse-lily charm necklace (neck; caster weight 1-5; `item_type = 8`, ITEM_TREASURE)
- **5137** scaremask helm (head; tank weight 11-15; `item_type = 9`, ITEM_ARMOR)
- **5138** pallbearer's iron mantle (about; tank weight 11-15; `item_type = 9`, ITEM_ARMOR)
- **5139** wardwire compliance band (wrist; melee weight 6-10; `item_type = 9`, ITEM_ARMOR)
- **5140** violet compact censer (hold; caster weight 1-5; `item_type = 8`, ITEM_TREASURE)
- **5141** reliquary scythe (hold; melee weight 6-10; `item_type = 5`, ITEM_WEAPON; `value3 = 3` slash; `ITEM_TWO_HANDED`)
- **5142** bell-eaten crook (hold; melee weight 6-10; `item_type = 5`, ITEM_WEAPON; `value3 = 7` pound; `ITEM_BOSS`, `ITEM_LOOT`)
- **5143** deferred-name reliquary shard (hold; caster weight 1-5; `item_type = 8`, ITEM_TREASURE; `ITEM_BOSS`, `ITEM_LOOT`)
- **5144** furrow warden's buckler (hold; tank weight 11-15; `item_type = 9`, ITEM_ARMOR; `ITEM_BUCKLER`)
- **5145** funerary bell key (hold; `item_type = 18`, ITEM_KEY; weight 1)

### Object Spec Compliance

**Required for all objects:**
- Every object must include `ITEM_TAKE` (`8388608`) in wear flags.
- No object may include `ITEM_WEAR_CLAN_COLORS` (`16777216`).
- Object `name`, `short_descr`, and `description` must be thematically consistent with wear flags.
- Object names must be unique within the area file.
- Do not define fixed stats; stats are generated at runtime.
- Do not set `ITEM_GENERATED` (`1`); it is runtime-managed.

**Weight archetype encoding:**
- `1`–`5`: caster item
- `6`–`10`: melee item
- `11`–`15`: tank item

**Weapon requirements:**
- Weapons must have `item_type = 5` and wear flags including both `hold` (`32768`) and `take` (`8388608`).
- `value3` must be thematically consistent with the weapon's concept.
- `value3 = 0` (`hit`) only valid with `ITEM_FIST` extra flag.
- The reliquary scythe (`5141`) is a two-handed archetype and must include `ITEM_TWO_HANDED` (`2147483648`).

---

## Reset and Encounter Structure

### Zone Population Density
- **Zone 1 (Gate-Tithe Forefield):** Dense low-threat population with civic ambience. Mostly non-aggressive mourners, acolytes, and lantern-keepers. Tithe crow swarmlings provide light combat.
- **Zone 2 (Registry Furrows):** Mixed — non-aggressive clerks at stations, aggressive undead pallbearers and drudges patrolling between.
- **Zone 3 (Reliquary Pasture):** Medium density — aggressive scaremask reapers and corpse-lily groundsmen among the converted fields. Spirit disturbances in fog rooms.
- **Zone 4 (Compact Holding Grounds):** Controlled pulls — fewer but stronger violet compact wardens and adjudicators. Named elite encounters at key positions.
- **Zone 5 (Sepulcher Core):** Staged named encounters culminating in The Bell-Eaten Steward and Pasture Heart Wraith.

### Reset Commands

**Mobile resets (`M`):** Each mob is loaded into a specific room with limit `1` for named/boss mobs, limit `2-3` for common mobs.

**Object resets:**
- `O`: Token of Receipt placed in a Zone 2 room (reset limit `1`).
- `O`: Token of Witness placed in a Zone 3 room (reset limit `1`).
- `O`: Token of Interment placed in a Zone 4 room (reset limit `1`).
- `G`/`E`: Equipment objects given/equipped to appropriate mobs.
- Boss mobs (`5123`, `5124`) use loot tables (`l`/`L` extension) for boss signature drops (`5142`, `5143`). These items must have `ITEM_BOSS` and `ITEM_LOOT` set.

**Door resets (`D`):**
- `5168` → `5169` (Compact Gate of Final Review → Chapel of the Last Acre): locked door (`D` state `2`), keyed to Fused Writ of Interment (`5131`). The room exit at `5168` must have `EX_ISDOOR` (`1`) set and `<key_vnum>` = `5131`.
- `5175` (Lantern Return Gate): closed door (`D` state `1`), opened from the inside after core completion. Must have `EX_ISDOOR` set.

### Loot Table Policy
- Boss loot items must have `ITEM_BOSS` (`134217728`) and `ITEM_LOOT` (`67108864`) set in `extra_flags`.
- `loot_amount` for boss mobs set to `100` (one guaranteed drop).
- `loot_chance` values must sum to `<= 100`.

### #SHOPS Section
No shops are planned for this area. The area is a funerary district, not a commercial zone.

### #SPECIALS Section
- `M 5120 spec_guard` (Marshal Edrin Vale — lawful gate enforcement)
- `M 5121 spec_cast_cleric` (Sister Calvea — ritual healing/offense caster)
- `M 5122 spec_cast_mage` (Archivist Mor Taland — arcane record-warden)
- `M 5123 spec_cast_undead` (The Bell-Eaten Steward — undead custodian)
- `M 5124 spec_cast_undead` (Pasture Heart Wraith — spirit boss)

Note: `spec_summon_*` specials must not be used (runtime-only). `spec_keep*` specials must not be set by automation.

### #OBJFUNS Section
No object functions are planned for this area.

### Post-Clear Egress
- Post-clear reset opens the Lantern Return Gate (`5175`), providing a shortcut back to Gravebell Milestone (`5111`) to reduce repetitive backtracking.
- Pasture Overlook (`5180`) provides an alternative retreat path back to Zone 1.

---

## Midgaard Lore Integration Requirements

1. Explicit references to the **Lantern Reforms** in signage, route markers, and patrol habits.
2. Explicit references to **Violet Compact** containment law in compact pens and ward devices.
3. Historical tie to Midgaard’s burden as crossroads city: caravan dead, dungeon dead, militia dead.
4. Tension between Temple mercy doctrine and civic tariff bureaucracy.
5. Lore objects that mention records disputes, missing names, and forged interment claims.

---

## Implementation Checklist

### Phase 1: Room Construction
- [ ] Write all 75 room names and descriptions with unique identity and themed anchors.
- [ ] Ensure each room description has at least 3 sentences (5 for important/landmark rooms).
- [ ] Ensure all room descriptions are unique — no identical descriptions across rooms.
- [ ] Define all exits as bi-directional.
- [ ] Add named exits with discoverability (in room description, object, or `E` extra).
- [ ] Prefix named door-style exit keywords with `^`.
- [ ] Add `E` extra descriptions with anchored keyword chains.
- [ ] Flag boss rooms (`5174`) with `no_mob` (`4`).
- [ ] Set appropriate `room_flags` and `sector_type` per zone.

### Phase 2: Mobile Construction
- [ ] Build all 22 mobs with proper act flags (`stay_area` on all; `sentinel` where appropriate).
- [ ] Flag boss mobs with `sentinel` + `boss`; solo mobs with `solo`.
- [ ] Set `aggressive` only on hostile mobs, not civic NPCs.
- [ ] Write single-line `long_descr` and properly formatted `description` for each mob.
- [ ] Configure `!` extension lines with appropriate class/skills/cast/def per tier.
- [ ] Configure `|` extension lines with element strengths/weaknesses.
- [ ] Set up loot tables (`l`/`L`) for boss mobs.

### Phase 3: Object Construction
- [ ] Build all 18 objects with `ITEM_TAKE` in wear flags.
- [ ] Set up ITEM_PIECE tokens (`5128`-`5130`) with correct `value0`/`value1`/`value2`.
- [ ] Create Fused Writ key object (`5131`) as ITEM_KEY.
- [ ] Ensure unique object names within the area.
- [ ] Apply correct weight archetype encoding (caster/melee/tank).
- [ ] Set `ITEM_BOSS` + `ITEM_LOOT` on boss signature items.
- [ ] Set `ITEM_TWO_HANDED` on the reliquary scythe.
- [ ] Set `ITEM_BUCKLER` on the furrow warden's buckler.
- [ ] Verify weapon `value3` thematic consistency.

### Phase 4: Resets and Specials
- [ ] Write `M` resets for all mob placements with appropriate limits.
- [ ] Write `O` resets for token placements.
- [ ] Write `G`/`E` resets for mob equipment.
- [ ] Write `D` resets for locked gate (`5168→5169`, state `2`, key `5131`) and closed gate (`5175`, state `1`).
- [ ] Write `#SPECIALS` entries for named mobs.
- [ ] Verify all reset vnums reference valid targets.

### Phase 5: Validation
- [ ] Verify vnum containment (`5550-5649` only).
- [ ] Verify all text fields terminated with `~`, no `\n\n`, no vnum mentions.
- [ ] Verify color-code policy compliance (no `@@k`, no background/flashing codes).
- [ ] Verify `EX_ISDOOR` set on all exits reset to closed/locked via `D`.
- [ ] Verify key object exists for every locked door.
- [ ] Verify area header: `Q 16`, `O Virant~`, `V 5550 5649`.
- [ ] Verify canonical section order.
- [ ] Run `make unit-tests` from `src/`.
