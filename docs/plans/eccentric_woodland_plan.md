# Area Plan: Eccentric Woodland (400-Vnum Expansion)

## Overview
- **Area Name:** `@@WThe @@rEccentric @@GWoodland@@N`
- **File Name:** `wood.are`
- **Keyword:** `eccentricwoodland`
- **Intended Levels:** 18-92
- **Vnum Range (expanded):** `7300-7699`
- **Vnum Envelope Size:** 400
- **Room Utilization Goal:** 400/400 rooms used (`7300-7699`)
- **Lore Anchors:** `docs/lore/eccentric_woodland_lore.md`, `docs/lore/midgaard_lore.md`, `docs/lore/rakuen_lore.md`, `docs/lore/sepulchur_pasture_lore.md`

This plan rebuilds the woodland as Midgaard's unstable southern corridor and Rakuen's survival artery: a region of moving path logic, ritualized violence, and bureaucratic denial at the boundary.

---

## Spec-First Constraints (strictly from `/docs`)

### Area/Header Requirements
1. `Q 16` is mandatory (`#AREA`).
2. Owner must be `O Virant~`.
3. Envelope must be `V 7300 7699`; all rooms/mobs/objects remain inside.
4. Keep canonical section order: `#AREA`, `#ROOMS`, `#MOBILES`, `#OBJECTS`, `#SHOPS`, `#RESETS`, `#SPECIALS`, `#OBJFUNS`, `#$`.

### String/Description Requirements
5. All strings terminate with `~`.
6. No blank lines inside area strings (`\n\n` disallowed).
7. No vnums in in-world description text.
8. Mobile `long_descr` is exactly one text line then `~` line.
9. Mobile `description` ends with exactly one trailing newline before `~`.
10. Room descriptions end with exactly one trailing newline before `~`.
11. Object extra-description text ends with exactly one trailing newline before `~`.

### Room/Topology Requirements
12. Every room in `7300-7699` exists and has unique description text.
13. Exits are bidirectional except deliberate maze behavior.
14. Maze rooms must use `ROOM_MAZE` (`524288`).
15. Boss rooms must be `no_mob` (`4`).
16. Exit keywords must be discoverable in room text/object/extra-description.

### Mob/Object/Reset Requirements
17. All mobs include `stay_area` (`64`).
18. Boss mobs include `sentinel` (`2`) + `boss` (`67108864`) and live only in `no_mob` rooms.
19. Strong non-boss elites include `solo` (`33554432`).
20. No builder-set `invasion` flag.
21. Every object includes `ITEM_TAKE` (`8388608`).
22. No object uses `ITEM_WEAR_CLAN_COLORS` (`16777216`).
23. Loot-table items use `ITEM_LOOT` (`67108864`); boss loot also uses `ITEM_BOSS` (`134217728`).
24. `#RESETS` has no blank lines; lock/key resets use valid key vnums.

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

#### Zone A — Northern Gate Verge (7300-7349, 50 rooms)
- Midgaard administrative bleed-through: pass checks, warning placards, escort kiosks.
- Includes existing historic nucleus rooms (duel room, binding room, circle stones) rewritten to fit modern canon.
- **Primary external link retained:** `7302 north -> 3190` (Midgaard Southern Wild Gate).
- Difficulty: 18-28.

#### Zone B — Bell-Fog Paths (7350-7399, 50 rooms)
- Older shepherd-priest route memory: bells, fog pockets, guide rods.
- Dynamic-feeling pathing through repeated motifs and short loops.
- Difficulty: 24-34.

#### Zone C — Escort Margin Braids (7400-7469, 70 rooms)
- “Managed denial” terrain: patrol endpoints, abandoned survey clearings, forged seal sites.
- Two switchback braids merge/split repeatedly (no square block layout).
- Difficulty: 30-44.

#### Zone D — Ritual House Quarter (7470-7529, 60 rooms)
- Black-marble insert architecture “where no quarry should exist.”
- Duel halls, binding chambers, oath courts, spectator galleries.
- Rival houses create local safe/unsafe pulses.
- Difficulty: 40-58.

#### Zone E — Predator Claims and Lapsed Compacts (7530-7599, 70 rooms)
- Ex-path-warden bands controlling territorial chokepoints.
- Contract boards, breach notices, stolen convoy manifests.
- First major boss rung starts here.
- Difficulty: 50-72.

#### Zone F — Ninth Milestone Tangles (7600-7649, 50 rooms)
- Shared historical seam with Sepulchur traditions.
- Half-buried stones, identity tags, ledgers of unnamed dead.
- Ritualized scavenger/warden conflicts.
- Difficulty: 62-82.

#### Zone G — Southern Dispatch Ruins (7650-7689, 40 rooms)
- Former Rakuen compact waystations and dispatch houses now fractured.
- Multiple route funnels converge before final causeway.
- Difficulty: 72-88.

#### Zone H — Rakuen Causeway Threshold (7690-7699, 10 rooms)
- Cleanest navigation in the area, but highest political tension.
- Gate records, compact remnants, emergency courier staging.
- **Primary south external link:** `7698 south -> 14600` (planned Rakuen North Dispatch Gate room).
- Difficulty: 84-92.

### Maze Sub-Regions (required flags)
1. **Driftbell Knot** (`7368-7383`, 16 rooms, `ROOM_MAZE`)  
   Clue language: bell pitch and echo delay.
2. **Marble Oath Labyrinth** (`7488-7507`, 20 rooms, `ROOM_MAZE`)  
   Clue language: engraved vow fragments indicating true turns.
3. **Breachwood Snare Maze** (`7564-7587`, 24 rooms, `ROOM_MAZE`)  
   Clue language: carved guide-rod notch patterns and broken wax-seal colors.
4. **Ninth Acre Root Spiral** (`7612-7627`, 16 rooms, `ROOM_MAZE`)  
   Clue language: funerary marker orientation and tallow scent.

---

## External Connectivity Plan (required narrative and play links)

### Midgaard North Connection
- **Hard link:** `7302 north -> 3190` (already canonical in `midgaard.are`).
- **Return link:** `3190 south -> 7302` retained.
- North edge rooms emphasize pass-check booths, variance postings, and liability language.

### Rakuen South Connection
- **Planned hard link:** `7698 south -> 14600` (Rakuen north dispatch gate; reserve in Rakuen plan/envelope).
- **Return link:** `14600 north -> 7698` required in Rakuen implementation.
- Southern edge text frames Rakuen dependence on corridor reliability (grain, medicine, mail, labor transit).

### Optional Future Side Connectors
- Potential Sepulchur-adjacent side spur (one controlled branch, no direct bypass of primary south corridor).
- No additional city-grade exits without charter lore justification.

---

## Room Program and Encounter Gradient
- **50 navigation-anchor rooms:** strong landmarks for player reorientation.
- **220 conflict rooms:** ambush sites, skirmish lanes, choke crossings.
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

### Boss Ladder (5 bosses)
1. **Sealbreaker Valt** (7542): forged escort syndicate chief.
2. **Abbess of the Black Vow** (7509): ritual house war-priestess.
3. **The Bell-Eater Stag** (7385): apex predator tied to path drift.
4. **Warden-Defector Hadrik** (7589): ex-compact commander turned corridor tyrant.
5. **Marrow Ledger of Ninth Acre** (7629): identity-stripping memory entity at funerary seam.

All bosses: `stay_area + sentinel + boss`, placed in `no_mob` rooms.

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

All loot-generated objects flagged `ITEM_LOOT`; boss drops include `ITEM_BOSS`.

---

## Quest Plan (includes cartography + boss contracts)

Assume static quest cap expanded to `64` (`PROP_MAX_STATIC_QUESTS`) so Eccentric Woodland can use IDs `48-57` (`49.prop-58.prop`).

### Core Exploration Chain
1. **Quest 48 — Southern Variance Recon (Midgaard)**  
   Type 1; targets three low-zone route predators.
2. **Quest 49 — Bell-Fog Cartography Survey (Midgaard Cartographers' Dispute tie-in)**  
   Type 2 collection quest; recover 5 survey rubbings from anchor rooms in Zones A/B/C.  
   **This is the required cartography quest.**
3. **Quest 50 — Forged Escort Seal Crackdown (Midgaard)**  
   Type 1; kill forged-escort officers and recover stamp blocks.

### Corridor Stabilization Chain (Rakuen logistics)
4. **Quest 51 — Dispatch Lane Reopening (Rakuen envoy)**  
   Type 3; kill-count against lapsed compact raiders.
5. **Quest 52 — Causeway Compact Proofs (Rakuen envoy)**  
   Type 2; collect compact tablets and breach notices.
6. **Quest 53 — Defector Command Severance (boss)**  
   Type 3; target **Warden-Defector Hadrik**.

### Ritual House Arc
7. **Quest 54 — Oath Court Schism Inquiry**  
   Type 1; eliminate rival oath enforcers.
8. **Quest 55 — Black Vow Decapitation (boss)**  
   Type 3; target **Abbess of the Black Vow**.

### High-End Boss Arc
9. **Quest 56 — Sealbreaker Execution Writ (boss)**  
   Type 3; target **Sealbreaker Valt**.
10. **Quest 57 — Ninth Acre Name-Recovery Rite (boss finale)**  
    Type 1; targets **Marrow Ledger of Ninth Acre** + elite attendants.

### Quest Offerers (planned)
- Midgaard Southern Affairs auditor (north gate cluster).
- Midgaard Lantern Registry cartographer (north edge mobile).
- Rakuen dispatch envoy (south threshold cluster).
- Neutral bell-keeper mediator (mid-zone refuge room).

---

## Reset/Spawn Rhythm
- Reset rate target: `F 15`.
- North zones: denser neutral + light hostile populations.
- Mid zones: mixed patrol and ritual spikes.
- South zones: higher elite concentration and boss gating.
- Boss resets: limit 1 each, fixed lairs, quest-friendly persistence.

---

## Implementation Sequence
1. Update `#AREA` header (`Q 16`, `O Virant~`, `V 7300 7699`, keyword/levels/reset msg).
2. Author 400-room braided topology, then validate reverse-exit integrity (except maze exceptions).
3. Add maze flags and clue-language consistency.
4. Add 96 mobiles with strict flag policy and boss-room placement.
5. Add 118 objects with wear/extra-flag compliance.
6. Build resets with valid keys/doors; remove blank lines.
7. Add quest files (`49.prop-58.prop`) and wire offerer vnums.
8. Validate north/south external links (Midgaard + Rakuen endpoint reservation).

---

## Acceptance Checklist
- [ ] `wood.are` header complies (`Q 16`, `O Virant~`, `V 7300 7699`).
- [ ] All 400 room vnums `7300-7699` are implemented.
- [ ] Layout is braided/looped (not square-grid only).
- [ ] Maze room sets flagged `ROOM_MAZE`.
- [ ] Midgaard north link retained (`7302 <-> 3190`).
- [ ] Rakuen south link planned (`7698 <-> 14600`) and documented for Rakuen build.
- [ ] All mobs include `stay_area`; bosses use `sentinel + boss` in `no_mob` rooms.
- [ ] Object flags comply (`ITEM_TAKE`, loot/boss rules, no clan colors).
- [ ] Quest suite includes cartography quest and multiple boss quests.
- [ ] Text/lore tone matches Eccentric Woodland contradiction pillars.
