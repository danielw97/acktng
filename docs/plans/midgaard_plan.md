# Area Plan: Midgaard

## Overview
- **Area Name (current):** `@@WThe City of @@yMidgaard@@N`
- **File Name:** `midgaard.are`
- **Keyword:** `midgaard`
- **Current Vnum Range:** `917-1116`
- **Vnum Envelope Size:** 35
- **Current Room Utilization:** 35/35 rooms used (`917-1116`)
- **Current Mobile Utilization:** 56 mobiles in-range (up to `1077`)
- **Current Object Utilization:** 18 objects in-range (up to `1114`)
- **Layout Basis:** Preserve the existing implemented city footprint and connectivity, then expand by deepening district identity, quest/lore hooks, NPC roles, and room narrative differentiation (not by changing the core map geometry).

---


## Lore
See `midgaard_lore.md` for extracted lore.

## Spec-First Constraints (from `area_file_spec.md`)

### Area Header
1. `Q 16` directive is mandatory.
2. Owner line must be `O Virant~` (area header owner policy).
3. `V 917 1116` defines the vnum envelope; all authored rooms, mobiles, and objects must stay inside this range.

### Vnum Allocation
4. Vnums for rooms, mobiles, and objects must be assigned in ascending sequential order with no gaps.
5. Fill existing gaps in the vnum range before assigning new higher vnums.
6. For rooms, attempt to use all room vnums in the assigned range (fill every available slot where practical).

### String and Description Rules
7. All text fields terminated by `~`. Strings must not contain back-to-back newlines (`\n\n`).
8. Vnums must never appear in any in-world description text (room descriptions, mob descriptions, object descriptions, extra descriptions, exit descriptions).
9. Mobile `long_descr` must be exactly one text line followed by a newline and a line containing only `~`. Multi-line `long_descr` is never valid.
10. Mobile `description` must end with exactly one newline immediately before the terminating `~`.
11. Object extra-description `<description>` must end with exactly one newline before `~`.
12. Room `<description>~` must end with exactly one trailing newline before `~`.

### Color Policy
13. Use intentional inline color coding with the defined Lantern-Civic palette (see below); avoid forbidden thematic black (`@@k`).
14. Do not use background color codes (`@@0`–`@@7`) or flashing (`@@f`) unless explicitly added by a human author.

### Layout Preservation
15. Maintain the existing topological city layout; expansions are additive in detail and function, not a re-architecture.
16. Preserve all external world links already implemented from Midgaard:
  - `921 -> 5031` (Gloamvault link)
  - `930 -> 2294` (Dungeon link)
  - `941 -> 4681` (Ossuary/transport court link)
  - `946 -> 5106`
  - `957 -> 1146`
  - `976 -> 1832`
  - `1105 -> 5600`
  - `1106 -> 1434`

### Room Constraints
17. Each room description must contain at least 3 sentences; important rooms must contain at least 5 sentences.
18. Each room description must be unique (no reuse of identical descriptions across rooms).
19. Exits must be bi-directional: if room A connects to room B via direction X, room B must provide the opposite-direction exit back to A (except in mazes).
20. When a room uses a named exit (`<exit_keyword>`), that exit name must appear in the room's main description, an object that spawns in the room, or an `E` extra description.
21. Extra-description keywords must be discoverable: at least one keyword in any chain must appear in the room's main description.
22. Rooms that are part of a maze must be flagged `ROOM_MAZE` (`524288`).

### Mobile Constraints
23. All mobs must be flagged `stay_area` (`64`) so they do not wander out of their home area.
24. Boss mobs must be flagged `sentinel` (`2`) and `boss` (`67108864`) and placed only in rooms flagged `no_mob` (`4`).
25. Strong (non-boss) mobs must be flagged `solo` (`33554432`).
26. The `invasion` flag must never be set by builders; it is runtime-only.
27. The `aggressive` flag should be set only for mobs whose intended behavior is to initiate combat on sight (feral beasts, hostile undead, demonic sentries), not for townsfolk, merchants, quest givers, or guards.
28. Sentinel mobs (gate guards, shopkeepers) should be flagged `sentinel` (`2`).

### Object Constraints
29. Every object must include `ITEM_TAKE` (`8388608`) in wear flags.
30. No object may include `ITEM_WEAR_CLAN_COLORS` (`16777216`).
31. Object `name`, `short_descr`, and `description` must be thematically consistent with the object's wear flags.
32. Object names must be unique within the area file (no duplicate item names).
33. Area-authored items must not define fixed stats; stats are generated at runtime.
34. Object weight encodes archetype: `1`–`5` caster, `6`–`10` melee, `11`–`15` tank.
35. `ITEM_GENERATED` (`1`) must not be set in area files; it is runtime-managed.
36. Items from boss mob loot tables must include `ITEM_BOSS` (`134217728`).
37. Items spawned from mob loot tables (`l`/`L` extension) must have `ITEM_LOOT` (`67108864`) set.
38. Weapons must have wear flags including both `hold` and `take`. Two-handed weapon names must include the `ITEM_TWO_HANDED` extra flag.
39. `value3 = 0` (`hit`) for weapons is only valid with the `ITEM_FIST` extra flag.

### Reset Constraints
40. Resets must reference valid vnums for the target type required by the command.
41. If a door is reset to `locked` via `D` state `2`, `<key_vnum>` must be set to a valid key object vnum (not `-1`), and that key object must exist in `#OBJECTS`.
42. `#RESETS` must not contain blank lines.

### Section Order
43. Follow canonical saver order: `#AREA`, `#ROOMS`, `#MOBILES`, `#OBJECTS`, `#SHOPS`, `#RESETS`, `#SPECIALS`, `#OBJFUNS`, `#$`.

### Specials Policy
44. Mob specials beginning with `spec_summon_` must never be set in area files (runtime-only).
45. Mob specials beginning with `spec_keep` may only be assigned manually by a human.

---

## Existing Layout Interpretation (Preserve and Enrich)
Midgaard already reads as a **20x10 civic lattice** with repeated avenue naming motifs and several specialized civic/service pockets. The plan preserves this shape and formalizes it into district identities:

1. **Civic Spine District (central market-law corridor)**
  - Includes major named boulevards such as Cedar/Silver/Crimson/Auburn corridors and Lantern/Sunward/Granite/Saffron branches.
  - Function: trade, legal notices, guild traffic, cityguard visibility.

2. **Reliquary + Carceral Axis**
  - Anchors: `921` (**Violet Reliquary Arch**) and `930` (**Dungeon Intake Yard**).
  - Function: controlled transition to high-risk/occult and penal spaces.

3. **Sanctuary and Service Quarter**
  - Existing healer/postal/trade service rooms and temple-adjacent flow.
  - Function: practical city sustainment, religious legitimacy, traveler support.

4. **Newer Arcana-Requisition Strip (already present in high vnums)**
  - Anchors: `1038-1044`, `1077-1079` style rooms/NPCs.
  - Function: modernized outfitting, quest/quest brokerage, invasion logistics.

5. **Gateward Connectors**
  - Rooms with external exits become explicitly themed as managed boundaries between Midgaard and linked regions.

---

## Multi-Color Theme: “Lantern-Civic Spectrum”
A five-color civic palette that matches Midgaard’s existing naming and atmosphere:

- **Civic White (`@@W`)**: law, stonework, proclamations, institutions.
- **Lantern Gold (`@@y`)**: tradeflow, market warmth, wayfinding, public life.
- **Ward Cyan (`@@c`)**: sigils, registry marks, sanctioned arcana, records.
- **Reliquary Violet (`@@p`)**: old oaths, subterranean thresholds, contested memory.
- **Iron Red (`@@R`)**: militia, alarms, emergency mobilization.
- **Reset (`@@N`)** always used to prevent color bleed.

### Thematic Usage Rules
- Street names keep restrained color usage (`@@W` titles, selective accent words).
- Institutional rooms (post office, armories, broker halls) get stronger `@@c`/`@@y` accents for readability.
- Reliquary and dungeon transitions use controlled `@@p` + `@@c` interplay.
- Do not introduce `@@k`, flashing, or background color codes.

---

## District-by-District Content Expansion Plan (Using Existing Rooms)

## A. Lantern Market Belt (western/central commercial corridors)
**Goal:** convert currently repetitive avenue descriptions into sub-district personalities while preserving traffic flow.

- Differentiate contiguous streets into micro-themes:
 - weights/measures lane
 - food and staples lane
 - contracts and writ lane
 - durable goods lane
- Add recurring environmental markers every 3-4 rooms (notice poles, water troughs, militia chalk marks, shrine niches).
- Tie ambient text to time-of-day reset messaging (market dawn setup, dusk closure bells).

**Room update style:**
- Keep existing names where strong.
- Rewrite generic repeated description blocks so each corridor segment signals function and social class.

## B. Reliquary-Dungeon Threshold Arc (`921`, `930`, connected approaches)
**Goal:** make the city’s occult/carceral threshold narratively coherent.

- `921` remains the formal warded descent point; expand description to include registry sigils, oath plaques, and controlled access protocol.
- `930` becomes intake bureaucracy + moral pressure point (scribes, bonds, mercy petitions).
- Approach rooms gain visual escalation: brighter civic colors fading into violet-cyan ward language.
- Add rumor-bearing low-level NPCs (clerks, petitioners, exhausted guards).

## C. Temple and Civic Care Arc
**Goal:** unify healer, sexton, and sanctuary-adjacent rooms as a visible social safety network.

- Clarify distinction between charitable healing, paid treatment, and funerary duties.
- Introduce memorial boards and public rites references in room descriptions.
- Create light quest hooks around missing persons, burial records, and ward maintenance offerings.

## D. Arcana / Outfitters / Requisitions Expansion (`1038-1044`, `1077-1079` cluster)
**Goal:** formalize this as Midgaard’s “new bureaucracy of conflict readiness.”

- Themed as a post-crisis civic complex where adventurers process gear, contracts, and sanctioned risk.
- Distinct counters/offices:
 - quest adjudication
 - invasion reserve logistics
 - quest brokerage archives
 - armor specialization courts (cloth/melee/tank)
- Make each service room textually unique with queue behavior and document culture.

## E. Gateward Connections (all out-of-area exit rooms)
**Goal:** make each external link unmistakable in lore and wayfinding.

- Each link room receives a canonical “boundary identity” (signage, customs marks, patrol notes, travel advisories).
- Add consistent boundary objects: milestone plaques, city seal pillars, posted risk bulletins.
- NPC patrol dialogue references destination-specific threats/opportunities.

---

## Mobile/NPC Plan

### Preserve and Recontextualize Existing Core NPCs
- Keep iconic Midgaard roles: mayor, town crier, cityguards, postmaster, healer, bartender, artisan vendors, janitor, flavor civilians/animals.
- Refresh descriptions/dialogue to reflect district identity and current civic tensions.

### New NPC Additions (within free vnums `1078-1116`)
Proposed additions focus on lore delivery and district function:

1. **Registry Archivist of Lantern Hall** (lawful lore source, ledger quests)
2. **Ward-Sigil Inspector** (reliquary threshold gatekeeping)
3. **Mercy Petitioner** (dungeon intake empathy hook)
4. **Supply Sergeant of the Reserve** (requisition chain quests)
5. **Night Lamplighter-Captain** (patrol and city rumor delivery)
6. **Temple Memorist** (names of dead, continuity-vs-reckoning arc)
7. **Road Warden Liaison** (links to northern forest traffic; delivers Kiessward dispatch intelligence and Lantern Road status)
8. **Contract Notary of Granite Arcade** (trade/legal missions; custodian of the Granite Arcade trade ledger including the Conclave transfer entry)
9. **Desert Relic Appraiser** (eastern trade authentication; delivers oasis-pyramid corridor lore and Measure Script misinterpretation hooks)

### Mob Spec Compliance and Placement Principles

**Required act flags for all mobs:**
- Every mob must have `stay_area` (`64`) set so it does not wander out of Midgaard.
- Stationary mobs (gate guards, shopkeepers, service NPCs) must also have `sentinel` (`2`) set.
- No mob should have `invasion` set; this is a runtime-only flag.

**Aggressive flag policy:**
- Only feral animals or hostile intruders should be flagged `aggressive` (`32`). Cityguards, merchants, civic NPCs, and quest givers must not be aggressive.
- Guards who react when provoked should use `spec_guard`/`spec_policeman` specials rather than the aggressive flag.

**Boss and solo policy:**
- Midgaard is primarily a civic area, so boss mobs are unlikely. If any are added, they must have `sentinel` + `boss` flags and be placed in `no_mob`-flagged rooms.
- Strong (non-boss) named NPCs (e.g., the Mayor, Guard Captain) should be flagged `solo` (`33554432`).

**Mobile description format (strict):**
- `long_descr` must be exactly one text line, then a newline, then `~`. Multi-line `long_descr` is never valid.
- `description` must end with exactly one newline before `~`.
- No vnum may appear in any mob description text.

**General placement principles:**
- Keep high guard visibility near portals and civic hubs.
- Use non-combat civic NPCs as breadcrumb narrators for lore delivery.
- Place wildlife/flavor mobs in quieter wards only; avoid immersion breaks in formal civic interiors.
- Maintain strict role clarity for shopkeepers and service specialists.
- Shopkeeper mobs must have corresponding `#SHOPS` entries with valid `buy_type` and hours.

---

## Object Plan

### Preserve Existing Economy Objects
- Keep core shop stock identities (weapons, armor, provisions, arcana, consumables).

### Additive Object Themes (remaining free object vnums in-range)
1. **Civic documents:** stamped writs, tariff forms, sealed warrants, pardon requests.
2. **Wayfinding assets:** district maps, milestone slates, lantern registry tags.
3. **Ritual/ward items:** chalk sigils, ward tethers, reliquary tokens.
4. **Memory artifacts:** memorial ribbons, ledger fragments, old oath medallions, Watchers' Cairn eye-motif fragments (connect to pre-founding mythology and Verdant Depths/Sepulchur Pasture parallels).
5. **Boundary markers:** gate advisories tied to each external route.
6. **Cross-area lore objects (key items to embed in archive/temple/market rooms):**
  - Granite Arcade trade ledger page (the "forty iron bars and six condemned debtors" entry — Conclave smoking-gun document).
  - Measure Script transliteration folio (technically accurate, functionally meaningless pyramid-era rubbings).
  - Kiessward dispatch tablet (western frontier intelligence bulletin from Wall Command).
  - Desert relic authentication certificate (Midgaard-stamped provenance document for oasis-sourced artifacts).
  - Ilren Voss survey notes (Bent Mile Principle field observations from the legendary ranger).

### Object Spec Compliance

**Required for all objects:**
- Every object must include `ITEM_TAKE` (`8388608`) in wear flags.
- No object may include `ITEM_WEAR_CLAN_COLORS` (`16777216`).
- Object `name`, `short_descr`, and `description` must be thematically consistent with wear flags (e.g., a `head` item must read as headgear).
- Object names must be unique within the area file; no two objects may share the same `<name>~` value.
- Do not define fixed stats in area files; stats are generated at runtime.
- Do not set `ITEM_GENERATED` (`1`) in area files; it is runtime-managed.

**Weight archetype encoding:**
- `1`–`5`: caster item
- `6`–`10`: melee item
- `11`–`15`: tank item

**Weapon requirements:**
- Weapons must have `item_type = 5` (`ITEM_WEAPON`) and wear flags including both `hold` (`32768`) and `take` (`8388608`).
- `value3` (weapon attack type) must be thematically consistent with the weapon's name and description.
- `value3 = 0` (`hit`) is only valid with the `ITEM_FIST` extra flag.
- Two-handed weapon names (e.g., "great axe") must include `ITEM_TWO_HANDED` (`2147483648`) extra flag.

**Extra description format:**
- `E` entry descriptions must end with exactly one trailing newline before `~`.
- No vnums may appear in any object description text.

### Flavor + Quest Utility
- Ensure a subset of new objects are read/examined for lore progression.
- Use document chains to connect market, temple, reliquary, and requisition districts.
- Document objects (writs, warrants, ledger fragments) should use `E` extra descriptions to provide readable text when examined, with keywords discoverable from the object's `short_descr`.

---

## Room Rewrite and Differentiation Strategy
Because all 35 room vnums are already occupied, expansion is delivered through **qualitative room differentiation**:

1. **Tier 1 (landmarks):** major hubs receive bespoke multi-paragraph descriptions and lore anchors. These are "important rooms" under the spec and must contain at least 5 sentences.
2. **Tier 2 (corridors):** repeated base templates replaced with district-specific variants (materials, sounds, professions, signage). Must contain at least 3 sentences.
3. **Tier 3 (connectors):** external-link rooms gain directional identity and travel-state cues. Must contain at least 3 sentences.
4. **Tier 4 (service interiors):** each shop/service room gains explicit operational flavor and social context. Must contain at least 3 sentences.

This preserves existing mechanics while making traversal feel like meaningful urban progression rather than repeated text blocks.

### Room Spec Compliance Checklist
- Every room description must contain at least 3 sentences (5 for important/landmark rooms).
- Every room description must be unique; no identical descriptions across rooms.
- Room descriptions must end with exactly one trailing newline before `~`.
- No vnums may appear in room descriptions, exit descriptions, or extra descriptions.
- Exits must be bi-directional: if room A connects to room B via direction X, room B must provide the opposite-direction exit back to A. (External links to other areas are exempt where the other area controls the return exit.)
- When a room uses a named exit (non-empty `<exit_keyword>`), the exit name must appear in the room's main description, an object that spawns in the room, or an `E` extra description.
- Extra-description keyword chains must be anchored: at least one keyword in each chain must appear in the room's main description.
- Named exit keywords for door-style exits must be prefixed with `^` (e.g., `^iron gate`).
- Any exit set to `closed` or `locked` via `#RESETS` command `D` must have `EX_ISDOOR` (`1`) set in its room `D<door>` definition.
- If a door is reset to `locked` (state `2`), `<key_vnum>` must reference a valid key object, and that key must exist in `#OBJECTS`.
- Rooms that are part of a maze must be flagged `ROOM_MAZE` (`524288`). Midgaard is not expected to contain mazes.
- Boss mob rooms must be flagged `no_mob` (`4`).

---

## Implementation Phases (Plan Only)

### Phase 1: Canon Audit and Theming Pass
- Inventory all Midgaard room texts and classify by district.
- Assign Lantern-Civic palette usage per district.
- Mark existing repetitive descriptions for rewrite priority.

### Phase 2: Landmark Rewrite
- Rewrite key anchor rooms first (`921`, `930`, `965`, temple/post office/arcana hubs, external connectors).
- Align lore vocabulary across civic, temple, and requisition hubs.

### Phase 3: NPC + Dialogue Expansion
- Update existing NPC descriptions/dialogue.
- Add new civic/lore NPCs in free vnum space (`1078-1116`).
- Wire dialogue cues for cross-area hooks.

### Phase 4: Object + Reset Enrichment
- Add lore documents and boundary markers in remaining object vnums.
- Update resets to populate new ambient civic life without overcrowding.

### Phase 5: Cohesion and Spec Validation
- Verify vnum containment (`917-1116` only).
- Verify color policy compliance and readability.
- Verify external links unchanged and clearly signposted.
- Verify room/mob/object text avoids vnum mentions.

---

## Success Criteria
1. Midgaard remains mechanically recognizable and fully compatible with existing world routes.
2. Traversal now communicates clear district identity and historical depth.
3. Lore links Midgaard naturally to the wider campaign geography and institutional history.
4. Service/NPC ecosystem feels civic, lived-in, and narratively purposeful.
5. All planned additions remain within current Midgaard vnum envelope and area specification constraints.
