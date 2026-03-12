# Area Plan: Catacombs of Nightfall (Full Layout, Population, and Item Redesign)

## Overview

- **Area Name:** `@@dThe @@pCatacombs @@mof @@dNightfall@@N`
- **Area File (existing):** `area/nightfall_catacombs.are`
- **Keyword:** `nightfall catacombs reliquary ossuary blackchoir`
- **Assigned Vnum Range (must remain unchanged):** **21000-21199**
- **Target Level Band:** **10-25** (preserved from header intent)
- **Reset Rate:** 15 (keep)
- **Owner:** `Virant` (implementation must normalize casing from current `virant`)

This plan fully replaces the current repetitive topology and template-populated room/object/mobile distribution with a coherent undercity necropolis arc tied directly to Midgaard civic history and Great Northern Forest covenant memory.

---

## Spec-First Constraints (from `area_file_spec.md`)

### Area Header
1. `Q 16` directive is mandatory.
2. Owner line must be `O Virant~` (area header owner policy).
3. `V 21000 21199` defines the vnum envelope; all authored rooms, mobiles, and objects must stay inside this range.

### Vnum Allocation
4. Vnums for rooms, mobiles, and objects must be assigned in ascending sequential order with no gaps.
5. Fill existing gaps in the vnum range before assigning new higher vnums.
6. For rooms, attempt to use all room vnums in the assigned range (fill every available slot where practical).
7. Cross-type overlap is allowed: a room, mobile, and object may share the same numeric vnum as long as each remains unique within its own index type. Rooms, mobiles, and objects each have independent vnum namespaces and should each start from the lowest available vnum in the envelope.

### String and Description Rules
8. All text fields terminated by `~`. Strings must not contain back-to-back newlines (`\n\n`).
9. Vnums must never appear in any in-world description text (room descriptions, mob descriptions, object descriptions, extra descriptions, exit descriptions).
10. Mobile `long_descr` must be exactly one text line followed by a newline and a line containing only `~`. Multi-line `long_descr` is never valid.
11. Mobile `description` must end with exactly one newline immediately before the terminating `~`.
12. Object extra-description `<description>` must end with exactly one newline before `~`.
13. Room `<description>~` must end with exactly one trailing newline before `~`.

### Color Policy
14. Use intentional inline color coding with the defined Nightfall Chromatic Doctrine (see below); avoid forbidden thematic black (`@@k`).
15. Do not use background color codes (`@@0`-`@@7`) or flashing (`@@f`) unless explicitly added by a human author.

### Room Constraints
16. Each room description must contain at least 3 sentences; important rooms (landmarks, boss arenas, subzone entries) must contain at least 5 sentences.
17. Each room description must be unique (no reuse of identical descriptions across rooms).
18. Exits must be bi-directional: if room A connects to room B via direction X, room B must provide the opposite-direction exit back to A (except in mazes).
19. When a room uses a named exit (non-empty `<exit_keyword>`), that exit name must appear in the room's main description, an object that spawns in the room, or an `E` extra description.
20. Named exit keywords for door-style exits must be prefixed with `^` (e.g., `^iron gate`, `^bone hatch`).
21. Extra-description keywords must be discoverable: at least one keyword in any chain must appear in the room's main description. Chained `E` references are allowed as long as each chain is anchored.
22. Rooms that are part of a maze must be flagged `ROOM_MAZE` (`524288`). Every vnum in a maze set must have `ROOM_MAZE` set.
23. Repeated movement in the same direction must not enter a directional loop unless every room in the set has `ROOM_MAZE` set.
24. Boss mob rooms must be flagged `no_mob` (`4`).
25. Any exit set to `closed` or `locked` via `#RESETS` command `D` must have `EX_ISDOOR` (`1`) set in its room `D<door>` definition.
26. If a door is reset to `locked` (state `2`), `<key_vnum>` must reference a valid key object vnum (not `-1`), and that key object must exist in `#OBJECTS`.

### Mobile Constraints
27. All mobs must be flagged `stay_area` (`64`) so they do not wander out of their home area.
28. Boss mobs must be flagged `sentinel` (`2`) and `boss` (`67108864`) and placed only in rooms flagged `no_mob` (`4`).
29. Strong (non-boss) mobs must be flagged `solo` (`33554432`).
30. The `invasion` flag must never be set by builders; it is runtime-only.
31. The `aggressive` flag should be set for mobs whose intended behavior is to initiate combat on sight (hostile undead, feral creatures, cursed sentries), and left unset for neutral/civilian roles (lore NPCs, echo witnesses, non-hostile ghosts).
32. Sentinel mobs (gate guards, stationary wardens) should be flagged `sentinel` (`2`).

### Object Constraints
33. Every object must include `ITEM_TAKE` (`8388608`) in wear flags.
34. No object may include `ITEM_WEAR_CLAN_COLORS` (`16777216`).
35. Object `name`, `short_descr`, and `description` must be thematically consistent with the object's wear flags (e.g., a `head` item must read as headgear, `hold` as a held item).
36. Object names must be unique within the area file (no duplicate item names).
37. Area-authored items must not define fixed stats; stats are generated at runtime.
38. Object weight encodes archetype: `1`-`5` caster, `6`-`10` melee, `11`-`15` tank.
39. `ITEM_GENERATED` (`1`) must not be set in area files; it is runtime-managed.
40. Items from boss mob loot tables must include `ITEM_BOSS` (`134217728`).
41. Items spawned from mob loot tables (`l`/`L` extension) must have `ITEM_LOOT` (`67108864`) set.
42. Weapons must have `item_type = 5` (`ITEM_WEAPON`) and wear flags including both `hold` (`32768`) and `take` (`8388608`).
43. `value3 = 0` (`hit`) for weapons is only valid with the `ITEM_FIST` extra flag.
44. Weapon `value3` (attack type) must be thematically consistent with the weapon's concept and presentation.
45. Two-handed weapon names (e.g., "great axe") must include `ITEM_TWO_HANDED` (`2147483648`) extra flag.

### Reset Constraints
46. Resets must reference valid vnums for the target type required by the command.
47. `#RESETS` must not contain blank lines.

### Section Order
48. Follow canonical saver order: `#AREA`, `#ROOMS`, `#MOBILES`, `#MOBPROGS`, `#OBJECTS`, `#SHOPS`, `#RESETS`, `#SPECIALS`, `#OBJFUNS`, `#$`.

### Specials Policy
49. Mob specials beginning with `spec_summon_` must never be set in area files (runtime-only).
50. Mob specials beginning with `spec_keep` may only be assigned manually by a human.

---

## Multi-Color Theme Bible (Nightfall Chromatic Doctrine)

- `@@d` **Dark Grey**: crypt stone, dust, old engineering, civic ruin.
- `@@p` **Purple**: vow-rites, necro-liturgy, cathedral authority.
- `@@m` **Magenta**: blood memory, oath violation, boss-state escalation.
- `@@a` **Light Cyan**: spirit echoes, forest memory, ward resonance.
- `@@G` **Green**: Midgaard/forest frontier heritage, old living pacts.
- `@@W` **White**: inscriptions, legal tablets, absolution markers.
- `@@N` **Reset**: mandatory closure of all colorized strings.

### Color Application Rules

- Room names: 1-2 accents max.
- Descriptions: color only on symbolic anchors (bells, sigils, roots, tablets, flames).
- Forest-memory nodes always include `@@a` + `@@G` pair.
- Midgaard civic remnants always include `@@W` or `@@d` with inscription motifs.
- Final sanctum encounters always include `@@p` and `@@m` cues.
- Every colorized string must terminate with `@@N`.

---

## Core Narrative Direction

The Catacombs of Nightfall are redesigned as the buried legal-necrotic archive beneath an abandoned Nightfall chapel district that once serviced Midgaard tribunals and funeral processions for frontier dead recovered from the Great Northern Forest marches.

The area is no longer just "undead corridors"; it becomes a machine of memory control built around three institutional acts:

1. **Record** (name, debt, oath, lineage)
2. **Inter** (body, relic, testimony)
3. **Reclaim** (raise or weaponize the dead when civic order fails)

This ties the area directly to:

- Midgaard's First Stone Accord, Lantern Reforms, Violet Compact, and Ash-and-Iron administrative eras.
- Great Northern Forest oath-stone traditions, Split-Pine Compact burial customs, and Cairn-Keeper ancestor rites.
- The Gloamvault's parallel institutional corruption (the Kel'Shadra order's Witness/Offer/Bind doctrine).
- The present-day collapse where sacred record-keeping has become predatory necromancy.

---

## Vnum Allocation Plan (Complete 200-Slot Envelope)

Per spec section 13.1, rooms, mobiles, and objects each have independent vnum namespaces. Cross-type overlap is allowed. Each type must be assigned in ascending sequential order starting from the lowest available vnum.

### Rooms (up to 200 rooms)
- `21000-21199`: full room network redesign. Fill every available slot where practical per spec requirement. Target 200 rooms across 20 subzones of 10 rooms each.

### Mobiles (up to 200 independent namespace)
- `21000-21054`: complete roster redesign (55 mobs: trash, elites, lore NPCs, bosses, echoes). Mobiles use their own independent vnum namespace starting at `21000`.

### Objects (up to 200 independent namespace)
- `21000-21044`: complete item/relic/key/quest-object redesign (45 objects). Objects use their own independent vnum namespace starting at `21000`.

This approach follows spec section 13.1: each content type starts from the lowest available vnum, assigned sequentially with no gaps. Cross-type overlap is explicitly allowed by the spec.

---

## Full Layout Redesign: 20 Subzones x 10 Rooms

The expanded 200-room layout divides into 20 thematic subzones of 10 rooms each. This doubles the original plan's room count to fill the entire vnum envelope per spec requirement.

### Upper Catacombs (Zones 1-5): Civic Ruin and Intake

### 1) Nightfall Gateworks (`21000-21009`)
**Theme:** collapsed chapel entrances, watch-house ruins, intake records.

- Entry from world remains at `21000`. External link to Midgaard room `3025` preserved (up exit from `21000`).
- Introduces dual heritage: Midgaard judicial stonework + Nightfall funerary iconography.
- Low-danger onboarding rooms with readable exits.
- Rooms should reference the Lantern Road's southern terminus and the Granite Arcade's administrative oversight of funeral processions.

**Key features:** intake ledger wall, cinder toll gate, cracked tribunal plinth.

**Room importance classification:**
- `21000` (entry): important room, minimum 5 sentences. Must reference the upward exit to Midgaard.
- Others: standard rooms, minimum 3 sentences each.

### 2) Processional of Lantern Ash (`21010-21019`)
**Theme:** formal funeral lane once used by Midgaard escort companies during the Lantern Reform era.

- Long north-south corridor with side chapels.
- Scripted auditory motif: bells, chain drags, distant recitations.
- First key fragment: **Seal of Procession**.
- Room descriptions should reference Midgaard's Bell Watch customs and the processional route's original civic function: escorting frontier dead from the Granite Arcade south through the chapel quarter.

**Key features:** ash-stained procession tiles, lantern bracket niches (empty, stolen or decayed), inscription plaques listing Lantern-era escort company names.

### 3) Register Catacombs (`21020-21029`)
**Theme:** sealed civic ledgers, debt tablets, name ossuaries.

- Medium-complex loops and alcoves for lore pickups.
- Mobs skew toward scribes, oath-collectors, ledger-wights.
- Introduces lock type: legal seals instead of iron keys. Any locked doors in this subzone must have corresponding key objects created in `#OBJECTS` and referenced by `<key_vnum>` in the exit definition.
- Room descriptions should reference Midgaard's Oath of Stone ("No judgment hidden, no sentence unrecorded") as inscriptions on vault walls, showing how the oath was both honored and perverted here.

**Key features:** vellum-shelf alcoves, ink-stained adjudication desks, debt tablet walls.

### 4) Processional Side Chapels (`21030-21039`)
**Theme:** mourning chapels where families were permitted final witness before interment.

- Side branches off the main processional spine.
- Introduces echo NPC encounters (non-hostile spectral testimony fragments).
- Room descriptions should reference the Nightfall Registry Rite's four marks: given name, witnessed deed, lineage/oath marker, final claimant seal.

**Key features:** witness alcoves, claimant benches, faded lament tapestries, broken family seal-presses.

### 5) Cinder Toll Gatehouse (`21040-21049`)
**Theme:** checkpoint between upper intake halls and deeper institutional corridors.

- Transition zone from low-danger to moderate-danger encounters.
- Mini-boss: **Nightfall Toll-Warden** (undead checkpoint officer still enforcing obsolete intake protocol).
- Locked gate requiring a key object. Per spec, if reset to locked via `D` state `2`, the `<key_vnum>` must point to a valid key in `#OBJECTS`.

**Key features:** toll ledger podium, confiscation cage, patrol muster point, burned roster board.

### Middle Catacombs (Zones 6-10): Institutional Core

### 6) Forest Dead Reliquary (`21050-21059`)
**Theme:** Great Northern Forest war-dead and ranger pyre relics.

- Roots break through vault ceilings.
- Distinct `@@a`/`@@G` ghost-light style in all room descriptions.
- First major lore bridge to forest clans and Lantern Road casualties.
- Room descriptions should reference the Split-Pine Compacts' requirement that forest dead be recorded without title erasure and cairn marks remain intact.
- Extra descriptions should include readable oath-stones with forest covenant language.

**Key features:** root-cracked vault ceilings, cairn-mark shelves, ranger pyre relic cases, green-stained oath stones alongside Midgaard legal tablets.

### 7) Bellcrypt Barracks (`21060-21069`)
**Theme:** militarized mortuary wing for reclaim operations.

- Tactical combat lanes, door control, patrol interlocks.
- Mini-boss: **Nightfall Bone-Captain** (strong non-boss, must be flagged `solo`).
- Loot profile: armor and martial relics.
- Multiple doors with `EX_ISDOOR` set; some reset to closed via `D` state `1`.
- Room descriptions should reference the four-bell cycle: First Bell (intake), Second Bell (adjudication), Third Bell (lament), Fourth Bell (reclamation summons -- forbidden under the Split-Pine Compacts).

**Key features:** weapon racks, patrol route chalk marks, bell-pull chains, barracks bunks, armory lock-cages.

### 8) Choir of Quiet Names (`21070-21079`)
**Theme:** echo choir that recites names of the interred.

- Controlled disorientation through mirrored aisles (no unfair maze spam). If any rooms use non-linear exits, they must all be flagged `ROOM_MAZE` (`524288`).
- Puzzle clue language references Midgaard district surnames (from the Lantern Registry system) and forest cairn marks.
- Second key fragment: **Veil Canticle Sigil**.
- Room descriptions should reference the Choir's original purpose: reading both civic names and lineage epithets, honoring the dead as citizens, allies, and kin.

**Key features:** carved name-walls, resonance alcoves, mirrored aisle tiles, broken canticle lecterns.

### 9) Violet Compact Vaults (`21080-21089`)
**Theme:** Midgaard containment-era legal/occult storage.

- Ward cages, confiscated relic evidence, sealed occult testimonies.
- Clerical and arcane mob mix.
- Door progression based on symbol order learned in prior zones.
- Room descriptions should reference the Violet Compact's three principles: registry primacy, containment over destruction, and civic oversight of thresholds. Inscriptions should echo the Compact's institutional language from Midgaard's archives.
- Extra descriptions on ward-cages and sealed chests should reference "sensitive incident" classifications from the Sealed Names doctrine.

**Key features:** violet-lacquered ward frames, confiscation manifest boards, sealed testimony chests, registry checkpoint sigils.

### 10) Register Annex and Legal Archive (`21090-21099`)
**Theme:** deeper institutional record-keeping, where civic ledgers become instruments of control.

- Introduces the transition from legitimate record-keeping to predatory bureaucracy.
- Room descriptions should show the Unindexed Years' traces: redacted plaques, replaced shelf labels, contradictory inventory marks.
- Safe-ish lore pocket with echo NPC encounter: **Archivist Nhal Sereth** (non-hostile exposition).

**Key features:** redacted name-walls, duplicate ledger shelves with different signatures, hidden true-name ledger alcove.

### Lower Catacombs (Zones 11-15): Corruption and Resistance

### 11) Rootglass Ossuary (`21100-21109`)
**Theme:** forest spirit corruption and fractured ward convergence.

- Root-choked bone galleries with spectral environmental hazards.
- Hybrid mobs (undead + spirit remnants).
- Third key fragment: **Cairn Witness Stone**.
- Room descriptions should use `@@a`/`@@G` color pairs and describe rootglass formations: translucent root-sap fused with mortuary glass and ash, capturing fractured voices and oath fragments.

**Key features:** rootglass node clusters, bone-gallery arches, spectral ward fractures, captured-voice crystals.

### 12) Reclaim Processing Halls (`21110-21119`)
**Theme:** the institutional machinery of raising and weaponizing the dead.

- Room descriptions should expose the "civic remainder" doctrine: bodies unclaimed within prescribed intervals reclassified for reclaim-rite labor.
- Mobs here are reclaimed undead still performing obsolete tasks: hauling, guard service, excavation.
- The horror is procedural: intake forms, processing queues, labor assignment boards.

**Key features:** remainder intake desks, reclaim circle markings, labor assignment slates, body-transfer carts.

### 13) Covenant Fracture Galleries (`21120-21129`)
**Theme:** the physical evidence of the Split-Pine Compact's betrayal.

- Rooms where forest dead were promised return rites but were converted to reclaim stock.
- Spirit echoes in rootglass speak in forest oath formulas, not chapel liturgy.
- Room descriptions should reference the Closing of the Third Bell: forest delegates' protest that Sealed Names violated cairn law and spirit-rest obligations.

**Key features:** broken covenant tablets, defaced cairn-mark shelves, forced-conversion ritual circles, rootglass testimony nodes replaying protests.

### 14) Warden Crypt Armory (`21130-21139`)
**Theme:** the martial infrastructure of Nightfall's dreadguard cadres.

- Elite combat encounters with better-geared undead.
- Room descriptions should reference the Nightveil Matriarch's militarization of bellcrypt wardens into dreadguard cadres.
- Loot profile: mid-high armor, weapons, and martial relics.

**Key features:** dreadguard armor stands, oath-blade racks, patrol route maps, command seal displays.

### 15) Dissenter's Passage (`21140-21149`)
**Theme:** hidden corridors used by Nightfall registrars who secretly resisted.

- Rooms showing evidence of internal resistance: true-name copies hidden in wall cavities, smuggled relic tags, encoded restoration directions in funeral meter.
- Room descriptions should reference Archivist Nhal Sereth's hidden ledgers and wardens who smuggled relic tags to forest kin.
- Safe lore pocket with readable extra descriptions providing alternative testimony.

**Key features:** hollow-wall caches, hidden ledger niches, encoded funeral hymn tablets, smuggled forest relic tags.

### Deep Catacombs (Zones 16-20): Authority and Confrontation

### 16) Dread Procession Descent (`21150-21159`)
**Theme:** final filtration corridor to inner command sepulcher.

- Heavier elite density; fewer side branches.
- Ritual checkpoints require all 3 prior key fragments.
- Room descriptions should show increasing `@@p`/`@@m` color intensity as institutional authority concentrates.

**Key features:** ritual checkpoint pedestals (3 fragment slots), elite patrol corridors, doctrinal inscription walls.

### 17) Pallid Deathspeaker's Court (`21160-21169`)
**Theme:** the gatekeeper elite's personal domain.

- Penultimate encounter: **Pallid Deathspeaker of the Compact** (strong non-boss, flagged `solo`).
- Room descriptions should reference the Deathspeaker's role as doctrinal enforcer of the Violet Compact's sealed-channel protocols.
- Extra descriptions should include readable compact decrees and judicial verdicts.

**Key features:** judgment dais, compact decree tablets, sealed testimony vault doors, spectral audience galleries.

### 18) Matriarchal Antechamber (`21170-21179`)
**Theme:** the administrative threshold of the Nightveil Matriarch's inner authority.

- Room descriptions should reference the Matriarch's consolidated decrees: custody of all Sealed Name tablets, militarization of wardens, replacement of public lament rites, expansion of reclamation protocols.
- Transition from `@@p` purple authority into `@@m` magenta blood-memory tones.

**Key features:** Sealed Name tablet vault, consolidated decree wall, matriarchal office furnishings, handmaiden patrol stations.

### 19) Sepulcher of Nightfall Authority (`21180-21189`)
**Theme:** sovereign archive-throne where records become domination.

- Final boss arena plus ancillary memory chambers.
- Boss pair sequence:
  - **The Nightveil Matriarch, Keeper of Sealed Names** (phase one: authority). Must be flagged `sentinel` + `boss`, placed in room flagged `no_mob`.
  - **The Sepulcher Lich, Last Auditor of the Dead** (phase two: reclamation). Must be flagged `sentinel` + `boss`, placed in room flagged `no_mob`.
- Boss arena room must be flagged `no_mob` (`4`).
- Both bosses must have `stay_area` (`64`) set.
- Room descriptions should embody the chilling legal principle: "If the city survives, the dead consent retroactively."

**Important room classification:** All rooms in this subzone are important (minimum 5 sentences).

**Key features:** archive-throne dais, Sealed Name master registry, doctrinal command sigils, memory domination apparatus.

### 20) Exit Passages and Memorial (`21190-21199`)
**Theme:** post-boss resolution space and shortcut exit.

- Exit shortcut unlock after completion for replay flow.
- Shortcut must maintain bi-directional exit requirements.
- Room descriptions should provide narrative closure: the area's systems are interrupted but not fully cleansed. Records remain. The forest's memory continues to resist.
- Final room should reference both Midgaard's civic complicity and the Great Northern Forest's ongoing claim.

**Key features:** unlocked shortcut passage, memorial chamber, rootglass testimony wall (final readable lore), exit corridor back to upper gateworks.

---

## Connectivity and Looping Rules

- Main spine: `21000 -> 21199` in readable progression through 20 subzones.
- Every subzone has at least one loopback to reduce corpse-run pain.
- At least 8 cross-links between non-adjacent subzones for routing variety (doubled from original plan to match doubled room count).
- Door-heavy tactical play in barracks/compact zones; open navigation in reliquary/root zones.
- No dead-end trap chains without alternate egress.
- All exits must be bi-directional (if A connects to B via direction X, B must have opposite-direction exit to A).
- Named exits must use `^` prefix for door-style keywords (e.g., `^bone gate`, `^iron hatch`).
- Named exit keywords must be discoverable in the room's main description, an object in the room, or an `E` extra description.
- External link preservation: room `21000` up exit to Midgaard room `3025` must remain unchanged.

---

## Mobile Redesign Plan (`21000-21054`)

Mobiles use their own independent vnum namespace starting at `21000`, assigned sequentially with no gaps.

### Required Act Flags for All Mobs
- Every mob must have `stay_area` (`64`) set.
- Boss mobs must have `sentinel` (`2`) + `boss` (`67108864`) set and be placed in `no_mob` rooms.
- Strong non-boss mobs (mini-bosses, named elites) must have `solo` (`33554432`) set.
- Stationary mobs (gate wardens, archive guardians) must have `sentinel` (`2`) set.
- The `invasion` flag must never be set.
- Undead mobs should have `undead` (`16384`) set where thematically appropriate.

### Aggressive Flag Policy
- Hostile undead (reclaimed laborers, dreadguard, feral dead): set `aggressive` (`32`).
- Neutral/lore NPCs (echo witnesses, Archivist Nhal Sereth): do not set `aggressive`.
- Wardens and sentries that respond to provocation only: do not set `aggressive`; use `spec_cast_undead` or `spec_undead` specials instead.

### Mobile Description Format (Strict)
- `long_descr` must be exactly one text line, then a newline, then `~`. Multi-line `long_descr` is never valid.
- `description` must end with exactly one newline before `~`.
- No vnum may appear in any mob description text.

## Population Tiers

### Tier 1 (entry/trash, levels 10-14)
- `21000` ash tunnel rat
- `21001` ledger mite
- `21002` candle scavenger
- `21003` chapel carrion bat
- Purpose: density and atmosphere, low complexity.
- Act flags: `is_npc` + `stay_area` + `aggressive` (feral creatures).
- Archetype: melee profile for skill floors.

### Tier 2 (core trash, levels 14-18)
- `21004` grave skitter acolyte
- `21005` processional ghoul
- `21006` chain penitent
- `21007` marrow porter
- `21008` crypt vermin swarm
- `21009` ashen chapel crawler
- Purpose: reinforce ritual-labor caste ecosystem.
- Act flags: `is_npc` + `stay_area` + `aggressive` + `undead`.

### Tier 3 (specialists, levels 18-22)
- `21010` bellcrypt warden
- `21011` oath registrar wight
- `21012` forest pyre bearer
- `21013` censer devotee
- `21014` reclaim circle acolyte
- `21015` covenant fracture specter
- `21016` dreadguard sentry
- `21017` sealed-name keeper
- Purpose: mixed mechanics and lore identity.
- Act flags: `is_npc` + `stay_area` + `aggressive` + `undead`. Wardens additionally `sentinel`.
- Archetype: hybrid profile for caster/melee mix.

### Tier 4 (elites, levels 22-25)
- `21018` ossuary champion
- `21019` violet compact inquisitor
- `21020` reliquary reaver
- `21021` grave oath knight
- `21022` dreadguard lieutenant
- `21023` matriarchal handmaiden
- Purpose: tactical pressure and subgroup identity.
- Act flags: `is_npc` + `stay_area` + `aggressive` + `undead` + `solo`.

### Named Lore/Boss NPCs
- `21024` **Archivist Nhal Sereth** (non-hostile echo NPC; exposition). Act flags: `is_npc` + `stay_area` + `sentinel`. NOT aggressive. Spec: none (non-combat).
- `21025` **Captain Thorne Vel** (fallen Midgaard reclaim captain; mini-boss memory duel). Act flags: `is_npc` + `stay_area` + `sentinel` + `aggressive` + `undead` + `solo`. Spec: `spec_cast_undead`.
- `21026` **Nightfall Toll-Warden** (checkpoint mini-boss). Act flags: `is_npc` + `stay_area` + `sentinel` + `aggressive` + `undead` + `solo`.
- `21027` **Nightfall Bone-Captain** (bellcrypt mini-boss). Act flags: `is_npc` + `stay_area` + `sentinel` + `aggressive` + `undead` + `solo`. Spec: `spec_rewield`.
- `21028` **Pallid Deathspeaker of the Compact** (gatekeeper elite). Act flags: `is_npc` + `stay_area` + `sentinel` + `aggressive` + `undead` + `solo`. Spec: `spec_cast_undead`.
- `21029` **Nightveil Matriarch, Keeper of Sealed Names** (phase one final boss). Act flags: `is_npc` + `stay_area` + `sentinel` + `aggressive` + `undead` + `boss`. Spec: `spec_cast_undead`. Must be placed in `no_mob` room.
- `21030` **Sepulcher Lich, Last Auditor of the Dead** (phase two final boss). Act flags: `is_npc` + `stay_area` + `sentinel` + `aggressive` + `undead` + `boss`. Spec: `spec_cast_undead`. Must be placed in `no_mob` room.

### Additional Mobs (ambient/variant, `21031-21054`)
- `21031`-`21040`: variant trash and specialist mobs for population variety across 20 subzones.
- `21041`-`21050`: zone-specific ambient mobs (rootglass whisper, covenant ghost, reclaim laborer, etc.).
- `21051`-`21054`: additional named NPCs or variant elites as needed during implementation.
- All must follow the same flag and description format requirements above.

## Mob Ecosystem Rules

- Every subzone has one dominant caste (scribe, guard, clergy, reclaimed).
- Forest-linked rooms include at least one spirit-memory mob variant.
- Named mobs reference institutional role first, monstrosity second.
- Remove generic repeated filler names from current area and replace with narrative-ranked entities.
- All mobs with loot tables must have valid `l`/`L` extension data referencing objects that exist in `#OBJECTS` and have `ITEM_LOOT` set.
- Boss mob loot table items must also have `ITEM_BOSS` set.

---

## Object Redesign Plan (`21000-21044`)

Objects use their own independent vnum namespace starting at `21000`, assigned sequentially with no gaps. All object names must be unique within the area file.

### Required for All Objects
- Every object must include `ITEM_TAKE` (`8388608`) in wear flags.
- No object may include `ITEM_WEAR_CLAN_COLORS` (`16777216`).
- Do not set `ITEM_GENERATED` (`1`); it is runtime-managed.
- Do not define fixed stats; stats are generated at runtime.
- Object weight encodes archetype: `1`-`5` caster, `6`-`10` melee, `11`-`15` tank.
- Extra descriptions (`E` entries) must have `<description>` ending with exactly one trailing newline before `~`.
- No vnums in any object description text.

## Item Families

### 1. Processional Gear (entry-mid, levels 10-16)
- `21000` processional censer (hold, caster weight 3)
- `21001` ashen mourning mantle (about, caster weight 4)
- `21002` intake ledger chain (neck, melee weight 7)
- `21003` chapel candle lantern (hold, caster weight 2)
- Wear flags must include `take` + appropriate slot. Names must match wear slot identity.

### 2. Registry Relics (mid, levels 14-20)
- `21004` oath tablet shard (hold, caster weight 3)
- `21005` registrar's ink knife (hold+weapon, melee weight 8, `value3` = `2` stab)
- `21006` nightfall seal-ring (finger, caster weight 2)
- `21007` debt ledger fragment (hold, caster weight 1)
- `21008` adjudication quill (hold, caster weight 2)

### 3. Forest Memorial Relics (mid, levels 14-20)
- `21009` cairn witness token (hold, caster weight 3)
- `21010` rootglass charm (neck, caster weight 4)
- `21011` split-pine pyre badge (about, melee weight 6)
- `21012` ranger oath ribbon (wrist, caster weight 2)
- `21013` forest covenant tablet (hold, caster weight 5)

### 4. Compact Armaments (high, levels 20-25)
- `21014` decree blade (hold+weapon, melee weight 8, `value3` = `3` slash)
- `21015` witness shield (hold+armor, tank weight 13)
- `21016` dreadguard breastplate (body+armor, tank weight 15)
- `21017` bellcrypt warden helm (head+armor, tank weight 12)
- `21018` compact inquisitor's mace (hold+weapon, melee weight 10, `value3` = `7` pound)
- `21019` grave oath gauntlets (hands+armor, tank weight 11)
- `21020` reclaim circle staff (hold+weapon, caster weight 4, `value3` = `7` pound)

### 5. Boss/Signature Items (end, levels 23-25)
- `21021` matriarch's sealed veil (head+armor, caster weight 5). `ITEM_BOSS` + `ITEM_LOOT` required.
- `21022` auditor's decree staff (hold+weapon, caster weight 5, `value3` = `8` crush). `ITEM_BOSS` + `ITEM_LOOT` required.
- `21023` nightfall authority signet (finger, caster weight 2). `ITEM_BOSS` + `ITEM_LOOT` required.
- `21024` matriarch's blood-memory mantle (about, caster weight 4). `ITEM_BOSS` + `ITEM_LOOT` required.
- `21025` bone-captain's dread halberd (hold+weapon, melee weight 9, `value3` = `3` slash, `ITEM_TWO_HANDED`). `ITEM_BOSS` + `ITEM_LOOT` required. Two-handed weapon identity requires `ITEM_TWO_HANDED` extra flag.

### 6. Key and Progression Objects
- `21026` seal of procession (key, `item_type` = `18`, weight 1). First key fragment.
- `21027` veil canticle sigil (key, `item_type` = `18`, weight 1). Second key fragment.
- `21028` cairn witness stone (key, `item_type` = `18`, weight 1). Third key fragment.
- `21029` nightfall descent key (key, `item_type` = `18`, weight 1). Synthesis key for final descent.
- `21030` cinder toll gatehouse key (key, `item_type` = `18`, weight 1). For locked doors in zone 5.
- `21031` violet compact vault key (key, `item_type` = `18`, weight 1). For locked doors in zone 9.
- Per spec: any door reset to `locked` via `D` state `2` must have its `<key_vnum>` set to the matching key object's vnum, and that key must exist here.

### 7. Lore and Utility Objects
- `21032` memorial name ribbon (treasure, `item_type` = `8`, weight 1). Readable `E` extra description with interred names.
- `21033` redacted registry page (treasure, `item_type` = `8`, weight 1). Readable `E` with contradictory records.
- `21034` forest covenant fragment (treasure, `item_type` = `8`, weight 1). Readable `E` with Split-Pine Compact text.
- `21035` rootglass voice shard (treasure, `item_type` = `8`, weight 1). Readable `E` with captured testimony.
- `21036` nightfall toll ledger (treasure, `item_type` = `8`, weight 1). Readable `E` with intake records.
- `21037` sealed name tablet (treasure, `item_type` = `8`, weight 1). Readable `E` with hidden true-name.
- `21038` dissenter's encoded hymnal (treasure, `item_type` = `8`, weight 1). Readable `E` with resistance-encoded text.
- `21039`-`21044`: additional lore objects, variant equipment, or quest items as needed during implementation.

## Stat/Identity Rules

- Item names and extra descriptions align with color-theme semantics.
- Maintain level progression 10-25 with meaningful weapon/armor variety.
- Avoid clutter-only drops; each high-tier object signals a role origin (guard, clergy, registrar, reclaimer).
- Weapons must have `item_type = 5`, wear flags `hold` + `take`, and `value3` thematically consistent with the weapon's concept.
- Two-handed weapons (halberds, great axes, staves described as two-handed) must include `ITEM_TWO_HANDED` extra flag.
- `value3 = 0` (hit) only valid with `ITEM_FIST` extra flag.
- All loot-table items must have `ITEM_LOOT`. Boss loot-table items must additionally have `ITEM_BOSS`.

---

## Reset and Encounter Philosophy

- Keep reset cadence brisk but not swarm-heavy at entry.
- Use set-piece clusters in zones 7, 9, 14, 16, 17, and 19.
- Place at least one safe-ish lore pocket in zones 4, 6, 10, and 15.
- Boss resets positioned to avoid accidental chain aggro from doorways.
- Boss mobs reset in `no_mob`-flagged rooms only.
- `#RESETS` must not contain blank lines.
- All reset vnums must reference valid existing entries (rooms, mobs, objects).
- Door resets (`D` command) must only target exits with `EX_ISDOOR` set.
- Locked door resets (`D` state `2`) must target exits with valid `<key_vnum>` pointing to existing key objects.
- `G` and `E` resets for equipment on mobs: place after the corresponding `M` reset.
- Loot table items (`l`/`L` mob extension) are independent of reset-placed equipment.

---

## Midgaard Integration Targets

### Chronological Era References
- **First Stone Accord era:** Tablets in zone 3 describe the original impetus for Nightfall: stabilizing chaotic frontier burial practices after caravan and patrol company losses. Reference the militia pay-chest disputes and kin-claim conflicts that the Nightfall Registry Rite was designed to resolve.
- **Lantern Reform era:** Processional halls (zones 2, 4) show the formalization of intake gates, processional route segmentation by status and cause of death, and the indexed reliquary system. Reference Midgaard's standardized paving marks and posted notice systems that Nightfall adopted.
- **Violet Compact era:** Vaults in zone 9 explicitly connect to the Compact's three principles: registry primacy, containment over destruction, civic oversight of thresholds. Reference the Violet Reliquary Arch at Midgaard room 3005 as the surface-side checkpoint that fed material into Nightfall's underground channels.
- **Ash-and-Iron era:** Deeper zones (12-14) show emergency escalation: requisition language, sealed warrants, and the expansion of reclaim rites under crisis authorization. Reference the same sealed warrants that authorized prisoner transfers to the Spirebound Conclave.
- **Modern Tension era:** Exit/memorial zones (20) should reference the Continuity-vs-Reckoning political split in present-day Midgaard. Reformists who demand transparent burial review versus security blocs who defend sealed channels.

### Specific Cross-References
- Tablets describe old Midgaard tribunal contracts for identifying battlefield dead.
- Names in the Choir of Quiet Names include district-lineage references recognizable from Midgaard's Lantern Registry system.
- Violet Compact vault records explicitly connect to Midgaard's containment bureaucracy.
- Nightfall is framed as a once-legitimate civic institution that became autonomous and predatory.
- Reference the Oath of Stone ("No judgment hidden, no sentence unrecorded") as both aspiration and irony: Nightfall's registrars carved the oath on their walls while hiding Sealed Names.
- Reference the Temple of the Resounding Heart's position that the city has accrued "a debt to the unnamed" -- Nightfall is where most of those unnamed ended up.
- Reference Midgaard's Violet Silence festival (one evening of quiet observance for names removed from official rolls) as connected to Nightfall's sealed registries.

---

## Great Northern Forest Integration Targets

### Split-Pine Compact References
- Reliquary wing (zone 6) stores recovered ranger tokens, cairn ribbons, and split-pine oath markers.
- Room descriptions reference the Split-Pine Compacts' four requirements: record without title erasure, preserve cairn marks, protect pyre relics, prohibit spirit-disturbing rites below the third bell.
- Reference Ranger-Matron Ilse Thornwake as the forest signatory to the burial covenants.

### Forest Peoples' Legacy
- Cairn-Keeper ancestor communion traditions referenced in rootglass ossuary (zone 11): the belief that the honored dead face north toward the Cold Throne.
- Mire-Speaker herbal knowledge referenced in preservation practices within the reliquary.
- Everkeeper planting-ring symbolism referenced in green-stained oath stones (seven-pace circles).

### Spirit and Memory Resistance
- Spirit echoes in rootglass chambers speak in forest oath formulas, not chapel liturgy.
- Conflict established: forest dead were promised return rites, but Nightfall converted them into reclaim stock.
- Root incursion into lower crypts symbolizes the forest's memory refusing bureaucratic erasure.
- The Closing of the Third Bell is explicitly referenced as the moment when consent-based burial ended and coercive posthumous use began.

### Lantern Road Connection
- Nightfall received many of its dead from the Lantern Road: caravan casualties, patrol company losses, winter ambush victims, creature incursion dead.
- The Passage Tithe system's toll ledgers appear as readable objects in zone 3 and zone 5.
- Reference Ilren Voss's Bent Mile Principle and the negotiated road as context for why forest dead were brought to Nightfall at all: the road made Midgaard's civic burial system accessible to frontier casualties.

---

## Gloamvault and Broader World Integration

### Parallel Institutional Corruption
- The Gloamvault's Kel'Shadra order and Nightfall share a structural pattern: both began as institutions that bound memory to stone for civic benefit, and both devolved into engines of coercive control.
- Room descriptions in zones 9-10 may reference the Kel'Shadra order's "Witness/Offer/Bind" doctrine as a parallel tradition known to Nightfall's administrators.
- The Night of Unsounded Bells (Gloamvault's internal purge) parallels the Closing of the Third Bell (Nightfall's betrayal of forest covenant).

### Spirebound Conclave Shadow
- Deeper zones (12, 14) may reference sealed warrants that Midgaard issued for prisoner transfers to both Nightfall (for reclaim labor) and the Spirebound Conclave (for experimentation). The administrative apparatus was the same: the Violet Compact's registry system processed human beings as "arcane material."
- This connects Nightfall's institutional horror to the broader world's institutional horror: the Sunken Sanctum's Conclave, Midgaard's sealed warrants, Kowloon's Black Ledger.

---

## Lore Delivery Strategy (Rooms, Mobs, Objects)

- **Rooms:** architecture + inscriptions communicate era and purpose. Use extra descriptions (`E` entries) for readable inscriptions, tablets, and environmental details. Anchor all extra-description keywords in room main descriptions.
- **Mobs:** titles indicate institutional function (warden, registrar, prelate, reclaimer). `long_descr` provides one-line scene placement. `description` provides fuller institutional identity.
- **Objects:** relic provenance ties each drop to civic/forest history. Use `E` extra descriptions for readable lore text on treasure/document items.
- **Echo NPCs:** short, repeating testimony fragments provide layered truth without text walls in combat spaces. Non-aggressive, sentinel-flagged.

---

## Implementation Checklist (Builder-Facing)

1. Replace all room titles/descriptions in `21000-21199` with subzone-specific writing (20 zones, 10 rooms each).
2. Verify every room description has minimum 3 sentences (5 for important rooms: entry, boss arenas, subzone anchors).
3. Verify every room description is unique.
4. Rebuild exits/doors/loops per 20-zone structure. Verify all exits are bi-directional.
5. Verify all named exits use `^` prefix and are discoverable in room description, object, or `E` entry.
6. Verify all extra-description chains are anchored in room main descriptions.
7. Replace all mobiles with new roster and rank identities. Verify sequential vnum assignment starting at `21000`.
8. Verify all mobs have `stay_area` set. Verify bosses have `sentinel` + `boss`. Verify strong non-bosses have `solo`.
9. Verify all mob `long_descr` is exactly one text line. Verify no vnums in mob text.
10. Replace all objects with item-family structure. Verify sequential vnum assignment starting at `21000`.
11. Verify all objects have `ITEM_TAKE` in wear flags. Verify no `ITEM_WEAR_CLAN_COLORS`. Verify no `ITEM_GENERATED`.
12. Verify object weight encodes archetype correctly (1-5 caster, 6-10 melee, 11-15 tank).
13. Verify all weapons have `hold` + `take` wear flags and thematically consistent `value3`.
14. Verify boss loot items have `ITEM_BOSS` + `ITEM_LOOT`.
15. Verify all object names are unique within the area.
16. Rewrite resets to match new encounter ecology. Verify no blank lines in `#RESETS`.
17. Verify all locked door resets have valid key objects in `#OBJECTS`.
18. Verify boss mob rooms are flagged `no_mob`.
19. Verify section order: `#AREA`, `#ROOMS`, `#MOBILES`, `#MOBPROGS`, `#OBJECTS`, `#SHOPS`, `#RESETS`, `#SPECIALS`, `#OBJFUNS`, `#$`.
20. Validate all authored strings for color/spec compliance. Verify `@@N` terminates all colorized strings.
21. Validate no in-world description contains numeric vnums.
22. Validate level spread, spawn density, and boss gating for 10-25 flow.
23. Verify external link preservation: room `21000` up exit to Midgaard room `3025`.

---

## Success Criteria

- Area feels like a coherent institution, not a repeated corridor generator.
- Players can infer history from environment without external docs.
- Midgaard and Great Northern Forest links are explicit, accurate, and playable.
- Gloamvault and Sunken Sanctum parallels are visible to attentive players.
- Vnum envelope, color policy, and area-format constraints remain fully compliant.
- All 200 room vnums utilized across 20 subzones.
- All spec constraints from `area_file_spec.md` are satisfied.
