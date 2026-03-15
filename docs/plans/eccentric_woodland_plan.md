# Area Plan: Eccentric Woodland (400-Vnum Expansion)

## Overview
- **Area Name:** `@@WThe @@rEccentric @@GWoodland@@N`
- **File Name:** `wood.are`
- **Keyword:** `eccentricwoodland`
- **Intended Levels:** 18-92
- **Vnum Range (expanded):** `1432-1831`
- **Vnum Envelope Size:** 400
- **Room Utilization Goal:** 400/400 rooms used (`1432-1831`)
- **Lore Anchors:** `docs/lore/eccentric_woodland_lore.md`, `docs/lore/midgaard_lore.md`, `docs/lore/rakuen_lore.md`, `docs/lore/sepulchur_pasture_lore.md`

This plan rebuilds the woodland as Midgaard's unstable southern corridor and Rakuen's survival artery: a region of moving path logic, ritualized violence, and bureaucratic denial at the boundary.

---

## Spec-First Constraints (strictly from `/docs`)

### Area/Header Requirements
1. `Q 16` is mandatory (`#AREA`).
2. Owner must be `O Virant~`.
3. Envelope must be `V 1432 1831`; all rooms/mobs/objects remain inside.
4. Keep canonical section order: `#AREA`, `#ROOMS`, `#MOBILES`, `#OBJECTS`, `#SHOPS`, `#RESETS`, `#SPECIALS`, `#OBJFUNS`, `#$`.
5. Area files must not contain comments; comment syntax is not part of the accepted on-disk format (exception: `*` comment lines are allowed in `#MOBILES`, `#SPECIALS`, and `#OBJFUNS` sections only).

### String/Description Requirements
6. All strings terminate with `~`.
7. No blank lines inside area strings (`\n\n` disallowed).
8. No vnums in in-world description text (rooms, mobs, objects, extra descriptions, exit descriptions).
9. Mobile `long_descr` is exactly one text line then `~` line; no multi-line `long_descr` text.
10. Mobile `description` ends with exactly one trailing newline before `~`.
11. Room descriptions end with exactly one trailing newline before `~`.
12. Object extra-description text ends with exactly one trailing newline before `~`.

### Room/Topology Requirements
13. Every room in `1432-1831` exists and has unique description text.
14. Room descriptions must not be identical across rooms.
15. Each room's main `<description>~` must contain at least 3 sentences of text; important/landmark rooms must contain at least 5 sentences.
16. Room names and descriptions must not use placeholder or procedural naming patterns (e.g., `Corridor 12`, `Room 7`); names must be authored as in-world thematic content.
17. Exits are bidirectional except deliberate maze behavior; cross-area links also require the reverse exit in the connected area.
18. Maze rooms must use `ROOM_MAZE` (`524288`); every room in a maze vnum set must have this flag; only `ROOM_MAZE` rooms may use non-linear or looping exits.
19. Boss rooms must be `no_mob` (`4`).
20. Named exits (non-empty `<exit_keyword>`) must be discoverable in the room's main description, an object in the room, or an `E` extra description. Door-style named exits prefix the keyword with `^`.
21. If a door is reset to locked via `#RESETS D` state `2`, `<key_vnum>` must be a valid key object vnum (not `-1`) and that key object must exist in `#OBJECTS`.

### Mob/Object/Reset Requirements
22. All mobs must include `is_npc` (`1`) + `stay_area` (`64`).
23. Boss mobs must include `sentinel` (`2`) + `boss` (`67108864`) and live only in `no_mob` rooms.
24. Strong non-boss elites must include `solo` (`33554432`).
25. Within each sub-region, non-sentinel mobs must be confined by `sentinel` flag or `no_mob` room walls so they cannot wander out of their sub-region.
26. No builder-set `invasion` flag (`536870912`); this is a runtime-only flag.
27. Mobs intended to appear only during daytime must include `day_only` (`2147483648`) in `act`; mobs intended to appear only during nighttime must include `night_only` (`4294967296`). Do not set both simultaneously (runtime treats both-set as unrestricted). Time-gated mobs in Eccentric Woodland (e.g., nighttime ambush bands, daytime patrol wardens) must use these flags rather than relying on description text alone.
28. Every object must include `take` (`8388608`) in `wear_flags`.
29. No object may include `clan_colors` (`16777216`) in `wear_flags`.
30. Loot-table items must have `ITEM_LOOT` (`67108864`) in `extra_flags`; any item that can drop from a boss must also have `ITEM_BOSS` (`134217728`).
31. Area-authored objects must not set `ITEM_GENERATED` (`1`) in `extra_flags`; that flag is runtime-managed.
32. Item stats are generated at runtime; area files must not define handcrafted stat values. Author identity/behavior data only (type, flags, wear, apply, value layout, weight, level).
33. Object `name~` values must be unique within the area (no duplicate item names in the same area file).
34. Object weight encodes archetype: `1-5` = caster, `6-10` = melee, `11-15` = tank.
35. `ITEM_WEAPON` objects must include both `hold` and `take` in `wear_flags`.
36. Weapon `value3` must be thematically consistent with the weapon's concept; `value3 = 0` (`hit`) may only be used if the object also has `ITEM_FIST` in `extra_flags`.
37. Two-handed weapons whose name/description clearly indicates a two-handed archetype must include `ITEM_TWO_HANDED` (`2147483648`) in `extra_flags`.
38. Any object assigned `objfun_healing` in `#OBJFUNS` must include `ITEM_ANTI_EVIL` (`315`) in `extra_flags`.
39. Any object with `ITEM_LIFESTEALER` (`33554432`) must also include `ITEM_ANTI_GOOD` (`512`) in `extra_flags`.
40. `#RESETS` must not contain blank lines; lock/key resets must use valid key vnums within the area envelope.
41. Mobile loot tables: `loot_amount` must be greater than `0` for loot drops to be attempted; the sum of all `loot_chance[x]` values on an `L` line must be `<= 100`.
42. Bitvector hygiene: do not persist undefined or unknown bits in any bitvector-backed field.

### Quest Design Requirements
43. Every area must include at least one quest whose target is a boss.
44. Every area must include at least one cartography quest.
45. The final quest in every quest chain must reward a piece of equipment.
46. Any quest that targets a boss must reward a piece of equipment.

---

## Canon-Driven Identity and Play Pillars
1. **Boundary governance fails inward** (safe at gate, unstable inside).
2. **Authority is theatrical and local** (ritual houses, fake escort seals, path claims).
3. **South-corridor continuity** (Midgaard -> Eccentric Woodland -> Rakuen).
4. **Contradiction as environment** (pastoral clearings beside ambush halls).
5. **Route truth is contested** (maps, bells, guide rods, forged advisories).

---

## Multi-Color Theme: “Variance Canopy Palette”
- `@@G` living canopy, refuge pockets, non-hostile remnants
- `@@d` ash/char, failed wards, old dueling bloodstone
- `@@p` ritual pressure, oath geometry, uncanny path drift
- `@@a` fog channels, wet rootways, lantern haze
- `@@b` trodden earth, guide stakes, patrol traces
- `@@W` Midgaard notices, seal fragments, civic warning boards
- `@@R` ambush cues, breach markers, predator claims
- `@@N` reset

Policy: no `@@k`, no background color codes (`@@0`-`@@7`), no flashing (`@@f`).

---

## Topology Plan (non-grid, 400 rooms)

Use a **braided-corridor topology** instead of a square grid:
- One north/south **spine** from Midgaard gate approach to Rakuen causeway.
- Two long **counter-rotating loop systems** (“Bell Loop” and “Marble Loop”).
- Three **knot mazes** where route logic intentionally breaks.
- Four **crosslinks** allowing partial bypasses for skilled navigators.
- Several one-way pressure links used only in ambush subpaths and ritual gauntlets.

### Vnum Banding by Macro-Zone

#### Zone A — Northern Gate Verge (1432-1481, 50 rooms)
- Midgaard administrative bleed-through: pass checks, warning placards, escort kiosks.
- Includes existing historic nucleus rooms (duel room, binding room, circle stones) rewritten to fit modern canon.
- **Primary external link retained:** `1434 north -> 1106` (Midgaard Southern Wild Gate).
- Difficulty: 18-28.

#### Zone B — Bell-Fog Paths (1482-1531, 50 rooms)
- Older shepherd-priest route memory: bells, fog pockets, guide rods.
- Dynamic-feeling pathing through repeated motifs and short loops.
- Difficulty: 24-34.

#### Zone C — Escort Margin Braids (1532-1601, 70 rooms)
- “Managed denial” terrain: patrol endpoints, abandoned survey clearings, forged seal sites.
- Two switchback braids merge/split repeatedly (no square block layout).
- Difficulty: 30-44.

#### Zone D — Ritual House Quarter (1602-1661, 60 rooms)
- Black-marble insert architecture “where no quarry should exist.”
- Duel halls, binding chambers, oath courts, spectator galleries.
- Rival houses create local safe/unsafe pulses.
- Difficulty: 40-58.

#### Zone E — Predator Claims and Lapsed Compacts (1662-1731, 70 rooms)
- Ex-path-warden bands controlling territorial chokepoints.
- Contract boards, breach notices, stolen convoy manifests.
- First major boss rung starts here.
- Difficulty: 50-72.

#### Zone F — Ninth Milestone Tangles (1732-1781, 50 rooms)
- Shared historical seam with Sepulchur traditions.
- Half-buried stones, identity tags, ledgers of unnamed dead.
- Ritualized scavenger/warden conflicts.
- Difficulty: 62-82.

#### Zone G — Southern Dispatch Ruins (1782-1821, 40 rooms)
- Former Rakuen compact waystations and dispatch houses now fractured.
- Multiple route funnels converge before final causeway.
- Difficulty: 72-88.

#### Zone H — Rakuen Causeway Threshold (1822-1831, 10 rooms)
- Cleanest navigation in the area, but highest political tension.
- Gate records, compact remnants, emergency courier staging.
- **Primary south external link:** `1830 south -> 14600` (planned Rakuen North Dispatch Gate room).
- Difficulty: 84-92.

### Maze Sub-Regions (required flags)
1. **Driftbell Knot** (`1500-1515`, 16 rooms, `ROOM_MAZE`)  
   Clue language: bell pitch and echo delay.
2. **Marble Oath Labyrinth** (`1620-1639`, 20 rooms, `ROOM_MAZE`)  
   Clue language: engraved vow fragments indicating true turns.
3. **Breachwood Snare Maze** (`1696-1719`, 24 rooms, `ROOM_MAZE`)  
   Clue language: carved guide-rod notch patterns and broken wax-seal colors.
4. **Ninth Acre Root Spiral** (`1744-1759`, 16 rooms, `ROOM_MAZE`)  
   Clue language: funerary marker orientation and tallow scent.

---

## External Connectivity Plan (required narrative and play links)

### Midgaard North Connection
- **Hard link:** `1434 north -> 1106` (already canonical in `midgaard.are`).
- **Return link:** `1106 south -> 1434` retained.
- North edge rooms emphasize pass-check booths, variance postings, and liability language.

### Rakuen South Connection
- **Planned hard link:** `1830 south -> 14600` (Rakuen north dispatch gate; reserve in Rakuen plan/envelope).
- **Return link:** `14600 north -> 1830` required in Rakuen implementation.
- Southern edge text frames Rakuen dependence on corridor reliability (grain, medicine, mail, labor transit).

### Optional Future Side Connectors
- Potential Sepulchur-adjacent side spur (one controlled branch, no direct bypass of primary south corridor).
- No additional city-grade exits without charter lore justification.

---

## Room Program and Encounter Gradient
- **50 navigation-anchor rooms:** strong landmarks for player reorientation.
- **55 conflict rooms:** ambush sites, skirmish lanes, choke crossings.
- **60 ritual/inside rooms:** oath houses, duel halls, archives, binding chambers.
- **40 refuge/barter rooms:** survivor burrows, neutral fire pits, guide markets.
- **30 threshold/quest rooms:** boss arenas, quest hand-in micro-hubs, map survey points.

Traversal target: average full clear 70-95 minutes at-level; efficient route 40-55 minutes with map knowledge.

---

## Mobile Plan (target: 96 mobiles)

### Faction Buckets
1. **Path Wardens (18 mobs):** guides, scouts, seal-checkers, route brokers.
2. **Ritual Houses (22 mobs):** duelists, oath speakers, marble adepts.
3. **Predator Bands (28 mobs):** lapsed compact raiders, false escorts, ambushers.
4. **Survivor Clusters (14 mobs):** refugees, barterkeepers, bell keepers.
5. **Midgaard Interface NPCs (8 mobs):** auditors, gate liaisons, variance clerks.
6. **Rakuen Dispatch NPCs (6 mobs):** couriers, supply runners, compact envoys.

### Mob Act Flag Policy
- All mobs: `is_npc` (`1`) + `stay_area` (`64`) are mandatory.
- Hostile mobs that initiate combat on sight: add `aggressive` (`32`).
- Stationary mobs (guards, questgivers, vendors): add `sentinel` (`2`).
- Strong non-boss elites: add `solo` (`33554432`).
- Do not set `invasion` (`536870912`) — runtime-only.
- Time-gated mobs (nighttime ambush bands, daytime patrol wardens): set `day_only` (`2147483648`) or `night_only` (`4294967296`) as appropriate. Do not set both flags simultaneously.
- Sub-region containment: every mob in a sub-region must be either flagged `sentinel` or confined by `no_mob`-flagged room walls, so mobs cannot wander outside their designated sub-zone.

### Boss Ladder (5 bosses)
1. **Sealbreaker Valt** (1674): forged escort syndicate chief.
2. **Abbess of the Black Vow** (1641): ritual house war-priestess.
3. **The Bell-Eater Stag** (1517): apex predator tied to path drift.
4. **Warden-Defector Hadrik** (1721): ex-compact commander turned corridor tyrant.
5. **Marrow Ledger of Ninth Acre** (1761): identity-stripping memory entity at funerary seam.

All bosses: `is_npc` + `stay_area` + `sentinel` + `boss`, placed exclusively in `no_mob` rooms. Boss loot objects carry both `ITEM_LOOT` and `ITEM_BOSS` in `extra_flags`.

---

## Object Plan (target: 118 objects)

### Category Split
- **Weapons (26):** dueling blades, stake spears, compact hatchets.
- **Armor/Jewelry (40):** escort coats, oath-sign rings, barkscale wraps.
- **Utility/Quest Objects (30):** route tags, map slates, wax seals, bell clappers.
- **Boss Loot (22):** signature pieces tied to the five bosses.

### Signature Object Sets
- **Midgaard Administrative Set:** stamped advisory boards, pass ledgers, seal kits.
- **Ritual House Set:** marble oath medallions, vow cords, judgment gavels.
- **Predator Counterfeit Set:** forged escort badges, fake dispatch writs.
- **Rakuen Dispatch Set:** ration markers, emergency courier satchels, compact tablets.

### Object Authoring Policy
- All loot-generated objects: `ITEM_LOOT` (`67108864`) in `extra_flags`; boss drops also include `ITEM_BOSS` (`134217728`).
- Every object must include `take` (`8388608`) in `wear_flags`; no object may include `clan_colors` (`16777216`).
- Do not set `ITEM_GENERATED` (`1`) in `extra_flags`; that flag is runtime-managed.
- Item stats are generated at runtime from level and runtime parameters. Author only identity/behavior data (type, flags, wear, apply selector, value layout, weight, level via `L` entry). Do not hand-tune stat values.
- Object weight encodes archetype: `1-5` caster, `6-10` melee, `11-15` tank.
- `ITEM_WEAPON` objects must include both `hold` and `take` in `wear_flags`.
- Weapon `value3` must match weapon concept; `value3 = 0` (`hit`) only if `ITEM_FIST` is also set.
- Two-handed weapons with a clearly two-handed name/description must include `ITEM_TWO_HANDED` (`2147483648`) in `extra_flags`.
- Object `name~` values must be unique within the area; no duplicate item names in the same area file.
- Object `name`, `short_descr`, and `description` must be thematically consistent with non-`take` wear flags (head items read as headgear, wrist as wristwear, held items as held objects, etc.).

---

## Quest Plan (includes cartography + boss contracts)

### Quest File Range
The current static quest load cap is `PROP_MAX_STATIC_QUESTS = 48`, meaning only `1.prop`–`48.prop` are loaded at startup. Eccentric Woodland quests require **expanding the cap to at least `58`** in `src/` before the quest files below will be active. This is a required code change, not an area-file change. Until the cap is raised, quest files `49.prop`–`58.prop` will be silently ignored at boot. Quest file numbering below uses static IDs `48-57` (files `49.prop-58.prop`).

### Equipment Reward Policy
Per spec section 13.2:
- Any quest that targets a boss **must** reward a piece of equipment.
- The final quest in every chain **must** reward a piece of equipment.
- Equipment rewards are authored as a custom reward-object block in the `.prop` file; the block specifies short description, keywords, long description, wear flags, extra flags, weight, and item-apply selector.

### Core Exploration Chain
1. **Quest 48 (49.prop) — Southern Variance Recon (Midgaard)**
   Type 1; targets three low-zone route predators. Rewards gold + QP.
2. **Quest 49 (50.prop) — Bell-Fog Cartography Survey (Midgaard Cartographers' Dispute tie-in)**
   Type 2 collection quest; recover 5 survey rubbings from anchor rooms in Zones A/B/C.
   **This is the required cartography quest.** Rewards gold + QP; this is a chain-internal quest and need not supply equipment.
3. **Quest 50 (51.prop) — Forged Escort Seal Crackdown (Midgaard)**
   Type 1; kill forged-escort officers and recover stamp blocks. Final quest of Core Exploration Chain — **must reward a piece of equipment.**

### Corridor Stabilization Chain (Rakuen logistics)
4. **Quest 51 (52.prop) — Dispatch Lane Reopening (Rakuen envoy)**
   Type 3; kill-count against lapsed compact raiders. Rewards gold + QP.
5. **Quest 52 (53.prop) — Causeway Compact Proofs (Rakuen envoy)**
   Type 2; collect compact tablets and breach notices. Rewards gold + QP.
6. **Quest 53 (54.prop) — Defector Command Severance (boss)**
   Type 3; target **Warden-Defector Hadrik**. Final quest of Corridor Stabilization Chain and boss target — **must reward a piece of equipment.**

### Ritual House Arc
7. **Quest 54 (55.prop) — Oath Court Schism Inquiry**
   Type 1; eliminate rival oath enforcers. Rewards gold + QP.
8. **Quest 55 (56.prop) — Black Vow Decapitation (boss)**
   Type 3; target **Abbess of the Black Vow**. Final quest of Ritual House Arc and boss target — **must reward a piece of equipment.**

### High-End Boss Arc
9. **Quest 56 (57.prop) — Sealbreaker Execution Writ (boss)**
   Type 3; target **Sealbreaker Valt**. Boss target — **must reward a piece of equipment.**
10. **Quest 57 (58.prop) — Ninth Acre Name-Recovery Rite (boss finale)**
    Type 1; targets **Marrow Ledger of Ninth Acre** + elite attendants. Final quest of High-End Boss Arc and boss target — **must reward a piece of equipment.**

### Bell-Eater Stag
The Bell-Eater Stag (1517) is an apex boss without a dedicated standalone quest chain above. Either add it as a target to an existing boss quest or create an additional quest; do not leave a boss without at least one quest targeting it.

### Quest Offerers (planned)
- Midgaard Southern Affairs auditor (north gate cluster, vnum in Zone A).
- Midgaard Lantern Registry cartographer (north edge mobile, vnum in Zone A/B).
- Rakuen dispatch envoy (south threshold cluster, vnum in Zone H).
- Neutral bell-keeper mediator (mid-zone refuge room, vnum in Zone B/C).

---

## Reset/Spawn Rhythm
- Reset rate target: `F 15`.
- North zones: denser neutral + light hostile populations.
- Mid zones: mixed patrol and ritual spikes.
- South zones: higher elite concentration and boss gating.
- Boss resets: limit 1 each, fixed lairs, quest-friendly persistence.

---

## Implementation Sequence
1. Update `#AREA` header (`Q 16`, `O Virant~`, `V 1432 1831`, keyword/levels/reset msg).
2. Author 400-room braided topology, then validate reverse-exit integrity (except maze exceptions).
3. Verify room descriptions: each at least 3 sentences; landmark/boss-approach rooms at least 5 sentences; all descriptions unique; no placeholder names.
4. Add maze flags and clue-language consistency; confirm all maze vnum sets are fully flagged `ROOM_MAZE`.
5. Add 96 mobiles with strict flag policy (`is_npc + stay_area` mandatory; boss flags; solo for elites; sub-region containment via sentinel or no_mob walls).
6. Add 118 objects with wear/extra-flag compliance (unique names, no handcrafted stats, weight archetype policy, weapon value3 consistency, two-handed flag where required).
7. Build resets with valid keys/doors; remove blank lines; verify loot table `loot_amount > 0` and `L` line sums `<= 100`.
8. Expand `PROP_MAX_STATIC_QUESTS` to at least `58` in `src/` (required code change before quest files go live).
9. Add quest files (`49.prop-58.prop`), wire offerer vnums, add equipment reward blocks to all boss-targeting and chain-final quests.
10. Validate north/south external links (Midgaard + Rakuen endpoint reservation).

---

## Acceptance Checklist

### Header and Structure
- [ ] `wood.are` header complies (`Q 16`, `O Virant~`, `V 1432 1831`).
- [ ] Section order: `#AREA`, `#ROOMS`, `#MOBILES`, `#OBJECTS`, `#SHOPS`, `#RESETS`, `#SPECIALS`, `#OBJFUNS`, `#$`.
- [ ] No comment lines outside the permitted sections (`#MOBILES`, `#SPECIALS`, `#OBJFUNS`).

### Rooms and Topology
- [ ] All 400 room vnums `1432-1831` are implemented; no gaps in vnum sequence.
- [ ] Layout is braided/looped (not square-grid only).
- [ ] All maze room sets fully flagged `ROOM_MAZE`; only those rooms use looping/non-linear exits.
- [ ] Every room description is unique, contains at least 3 sentences, and contains no vnum references.
- [ ] Landmark and boss-approach rooms contain at least 5 sentences.
- [ ] No placeholder or procedural room names.
- [ ] Named exits are discoverable in room description, object, or extra description.
- [ ] All locked-on-reset doors have `<key_vnum>` pointing to a valid key object in `#OBJECTS`.

### External Links
- [ ] Midgaard north link retained (`1434 <-> 1106`).
- [ ] Rakuen south link planned (`1830 <-> 14600`) and documented for Rakuen build.

### Mobiles
- [ ] All mobs include `is_npc` + `stay_area`; no `invasion` flag set.
- [ ] Bosses include `sentinel` + `boss`, placed only in `no_mob` rooms.
- [ ] Strong non-boss elites include `solo`.
- [ ] Sub-region mobs are sentinel-flagged or enclosed by `no_mob` walls.
- [ ] Time-gated mobs use `day_only` or `night_only` act flags; neither flag sets both simultaneously.
### Objects
- [ ] Every object includes `take` in `wear_flags`; no object includes `clan_colors`.
- [ ] No object sets `ITEM_GENERATED` in `extra_flags`.
- [ ] Loot-table objects have `ITEM_LOOT`; boss drops also have `ITEM_BOSS`.
- [ ] No handcrafted stat values; item level set via `L` entry only.
- [ ] All object `name~` values are unique within the area.
- [ ] Object weight matches archetype (1-5 caster, 6-10 melee, 11-15 tank).
- [ ] Weapons include both `hold` and `take` in `wear_flags`.
- [ ] Weapon `value3 = 0` only if `ITEM_FIST` is set; two-handed weapons with two-handed identity carry `ITEM_TWO_HANDED`.

### Resets
- [ ] `#RESETS` contains no blank lines.
- [ ] Mob loot tables have `loot_amount > 0`; `L` line sums are `<= 100`.

### Quests
- [ ] `PROP_MAX_STATIC_QUESTS` raised to at least `58` in `src/` before quest files are active.
- [ ] Quest suite includes at least one cartography quest (Quest 49).
- [ ] All five bosses have at least one quest targeting them.
- [ ] All boss-targeting quests include equipment reward blocks.
- [ ] All chain-final quests include equipment reward blocks.

### Lore and Tone
- [ ] Text/lore tone matches Eccentric Woodland contradiction pillars.
- [ ] Color theme uses Variance Canopy Palette; no `@@k`, no background codes, no flashing.
- [ ] No bitvector fields contain undefined or unknown bits.
