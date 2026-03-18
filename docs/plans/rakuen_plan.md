# Area Plan: Rakuen, the Failed Paradise

## Overview

- **Area Name:** `@@G@@WRakuen, @@gCity of the Last Promise@@N`
- **Area File:** `area/rakuen.are` (new area)
- **Vnum Envelope (35 total):** `V 4550 4749`
- **Allocation Model:** 100 rooms (`4239-4338`), 50 mobiles (`4339-4388`), 50 objects (`4389-4438`)
- **Target Level Band:** 45-95 (urban/woodland political hazard band)
- **Reset Rate:** 20
- **Reset Message:** `@@gwater climbs the old root-weave and Rakuen resets its queues@@N`
- **Owner:** `Virant`
- **Primary Access:** Eccentric Woodland southern route (room `1830`) connects north into Rakuen entry room `4239`

Rakuen is built as a vertically layered, politically fractured city that still performs paradise while living under ration law, flood risk, and corridor dependence. Layout, encounters, and objectization must center this contradiction.

---

## Strict Spec Compliance (from `/docs`)

### Area Header

1. Use `Q 16` and `O Virant~` in `#AREA`.
2. Keep all authored rooms/mobs/objects within `4550-4749`.
3. Keep prose free of explicit vnum mentions.

### String Formatting

4. All strings must be `~`-terminated; no missing terminators.
5. No blank lines (`\n\n`) inside any area string.
6. Mobile `long_descr` must be exactly one text line, then `~` on the next line — no multi-line format.
7. Mobile `description` must end with exactly one newline before `~`.
8. Room `description` must end with exactly one newline before `~` and contain at least 3 sentences; important rooms at least 5.
9. Extra-description `<description>` must end with exactly one newline before `~`.
10. No vnum values may appear in in-world description text (room, mob, object, or extra descriptions).

### Color Policy

11. Use intentional color coding with area theme palette only.
12. Do **not** use `@@k` (black foreground) in area-authored strings.
13. Do **not** use background color codes (`@@0`–`@@7`) or the flashing code (`@@f`) in area-authored strings.

### Structural Rules

14. Canonical section order: `#AREA`, `#ROOMS`, `#MOBILES`, `#OBJECTS`, `#SHOPS`, `#RESETS`, `#SPECIALS`, `#OBJFUNS`, `#$`.
15. `#ROOMS` must terminate with `#0`; `#MOBILES` must terminate with `#0`; `#OBJECTS` must terminate with `#0`.
16. `#RESETS` must not contain blank lines.
17. Vnums must be assigned in ascending sequential order within each content type; no gaps.

### Room Rules

18. Exits must be bi-directional (except rooms explicitly flagged `ROOM_MAZE` — not used in this area).
19. Boss rooms must have `no_mob` (`4`) flag; no other mobs load in those rooms.
20. Named exit keywords must appear in the room description, a spawned object, or an extra description.
21. All room names and descriptions must be unique, authored as in-world prose — no numbered-template naming (e.g., `Corridor 3`, `Room 7`).

### Mobile Rules

22. All mobs must include `stay_area` (`64`) in `act` flags.
23. Boss mobs must include `sentinel` (`2`) + `boss` (`67108864`) in `act` flags; placed only in `no_mob` rooms via resets.
24. Strong non-boss elites must include `solo` (`33554432`).
25. Civic/guard mobs that must not wander must include `sentinel` (`2`).
26. Do not set `invasion` flag in area files (runtime-only).
27. If a mob is intended to appear only during daytime, it must include the `day_only` act flag. If a mob is intended to appear only during nighttime, it must include the `night_only` act flag.
28. Do not assign `spec_summon_*` specials in area files. Do not assign `spec_keep_*` specials in area files.
29. Permitted ambient city specials: `spec_vendor`, `spec_laborer`, `spec_warden`, `spec_lamplighter`, `spec_janitor`.

### Object Rules

29. Every object must include `ITEM_TAKE` (`8388608`) in `wear_flags`.
30. No object may include `ITEM_WEAR_CLAN_COLORS` (`16777216`).
31. Loot-table objects must have `ITEM_LOOT` (`67108864`) in `extra_flags`.
32. Boss-dropped objects must also have `ITEM_BOSS` (`134217728`).
33. Any object with `ITEM_LIFESTEALER` must also have `ITEM_ANTI_GOOD`.
34. Any object assigned `objfun_healing` must have `ITEM_ANTI_EVIL`.
35. Object `name` values must be unique within the area.
36. Weapons (`ITEM_WEAPON`) must have both `hold` and `take` in wear flags.
37. Two-handed weapons (great weapons by name/concept) must include `ITEM_TWO_HANDED` extra flag.
38. `value3 = 0` (`hit`) on weapons must not be used unless the weapon also has `ITEM_FIST`.
39. Do not set `ITEM_GENERATED` in area-authored objects.
40. Item stats (numeric stat values) must not be hand-authored; area files define identity/type/flags/weight/level only.

### Lock and Key Rules

41. Any exit with a locked reset (`D ... 2`) must have `EX_ISDOOR` (`door` = `1`) set in its room `D<door>` definition.
42. A locked-on-reset exit must have a valid `<key_vnum>` (not `-1`) pointing to a defined key object in `#OBJECTS`.

### Quest Rules

43. At least one quest must target the boss.
44. At least one cartography quest must be included.
45. The final quest in any quest chain must reward a piece of equipment.
46. Any quest targeting a boss must reward a piece of equipment.

---

## Color Theme

- `@@G` living plant matter, garden restoration effort, flourishing surfaces
- `@@g` aged/weathered green, root-weave decay, moss and residue
- `@@W` civic inscription, proclamations, promise stone carvings
- `@@c` water channels, cistern lines, flood markers
- `@@b` worked timber, earthen floors, lower terrace ground textures
- `@@p` political subtext, faction rhetoric, amendment-track annotations
- `@@N` reset

Policy: no `@@k`, no background codes (`@@0`–`@@7`), no flashing (`@@f`).

---

## Lore Anchors to Preserve

- **Identity:** failed paradise that refuses to call itself triage.
- **Vertical city logic:** Lower Terraces → Causeway Quarter → Upper Civic Ring, with Canopy Workshops on the eastern edge and Root Tunnels below.
- **Institutional conflict:** Bloom Council, Ember Wardens, Garden Ledger Clerks, Canopy Compacts.
- **Factional pressure:** Amendment Track, Quiet Separatists, Preservation Coalition.
- **Founding memory:** Verath Solen, Caitha Dunnmark, Orn Fallsbridge, Promise Stones.
- **Festival cadence:** Festival of Ever-Bloom, Ledger Vigil, Founder's Reckoning.
- **Corridor truth:** Rakuen survives through the Eccentric Woodland bottleneck and dispatch intelligence, not guaranteed roads.

---

## City Topology (Non-Grid / Organic)

No square-grid presentation. Use an **asymmetric braided graph** with stacked elevation loops:

- **Southwest ingress braid:** woodland switchbacks, lookout stumps, and dispatch checkpoints.
- **Lower flood fan:** semi-submerged crescents and root-bridge arcs that seasonally reroute travel.
- **Middle causeway spine:** elevated stone ribs linking ration towers and market lanes.
- **Upper civic halo:** partial ring around Bloom Pavilion and Promise Stone Plaza, with council shortcuts.
- **Eastern workshop comb:** lateral branchwork under dense canopy for industry and compact halls.
- **Subterranean siphon line:** narrow tunnel chain beneath terraces feeding flood cisterns.

Connectivity policy:
- Every zone has at least one loop and one vertical connector.
- Critical path remains readable; optional political/lore loops branch off it.
- Flooded detours and gateable causeways replace simple cardinal marching.

---

## External Connectivity

- **Eccentric Woodland link (north entry):** Room `4239` north exit connects to Eccentric Woodland room `1830` south. The Eccentric Woodland plan references this endpoint as the Rakuen north dispatch gate; the implementation assigns it to `4239`. Both sides must be wired symmetrically in the final build.
- Approach content in Zone 1 must preserve the "managed denial" and route-variance tone established in the Eccentric Woodland southern dispatch corridor.

---

## Zone Plan (100 Rooms Total)

### Zone 1: Approach Terraces & Woodland Entry (`4239–4251`, 13 rooms)

**Role:** Transition from Eccentric Woodland uncertainty into Rakuen's managed-entry culture.

Key spaces:
- Outer guide-marker run (3 rooms — entry approach from woodland, includes room `4239`)
- Redwater crossing overlook (1 room)
- Lower Dispatch House entry floor (3 rooms, `indoors`)
- Route-board court (2 rooms)
- First terrace stairs and flood warning drums (2 rooms)
- Terrace junction connecting to Zones 2 and 3 (2 rooms)

Mob focus (`4339–4346`, 8 mobs):
- Dispatch registrar (`sentinel` + `stay_area`)
- Canopy compact runner (`stay_area`)
- Woodland contract guide (`stay_area`)
- Exhausted caravan guard (`stay_area`)
- Opportunist route broker (hostile/social, `stay_area`)
- Flood drum attendant (`sentinel` + `stay_area`)
- Terrace checkpoint officer (`sentinel` + `stay_area`, `spec_guard` if law-enforcement)
- Approach guide stump warden (`stay_area`)

Object focus (`4389–4395`, 7 objects):
- Weathered route board slats (`board` type, `take`)
- Stamped travel writ bundle (`quest` type, `take`)
- Compact rope harness (`armor` type, `body` + `take`)
- Flood-season marker lantern (`light` type, `take`)
- Worn dispatch ledger folio (`treasure` type, `take`)
- Route advisory nail board (`board` type, `take`)
- Battered travel coffer (`container` type, `take`)

---

### Zone 2: Lower Terraces (`4252–4271`, 20 rooms)

**Role:** Flood-prone root-weave ruins and inhabited pockets where paradise aesthetics persist under decay.

Subclusters:
- Stilted survivor lanes (5 rooms)
- Root-weave house shells (5 rooms)
- Rest garden shallows (3 rooms)
- Collapsed promenade walk (4 rooms)
- Ember Warden floodline stations (3 rooms)

Mob focus (`4347–4357`, 11 mobs):
- Lower Terrace compact steward (`sentinel` + `stay_area`)
- Flood-watch warden (`sentinel` + `stay_area`)
- Displaced gardener (`stay_area`)
- Root-rot scavenger (`stay_area`)
- Drowned-root revenant (`aggressive` + `undead` + `stay_area`) — seasonal hazard type
- Rest-garden keeper (`sentinel` + `stay_area`)
- Stilted lane squatter (`stay_area`)
- Promenade ruin looter (`stay_area`)
- Ember Warden floodline station guard (`sentinel` + `stay_area`)
- Waterlogged root-leech (`aggressive` + `stay_area`)
- Lower terrace scavenger hound (`aggressive` + `stay_area`)

Object focus (`4396–4405`, 10 objects):
- Salvage stilt brace (`treasure` type, `take`)
- Moss-sealed grain jar (`food` type, `take`)
- Promise-stone votive ribbon (`treasure` type, `take`)
- Waterlogged ledger satchel (`container` type, `take`)
- Ember Warden floodline flag (`treasure` type, `take`)
- Root-weave splint bundle (`treasure` type, `take`)
- Cracked promenade tile shard (`treasure` type, `take`)
- Flood-season tide gauge (`treasure` type, `take`)
- Terrace-repair mortar crock (`treasure` type, `take`)
- Silt-caked gardening tool (`treasure` type, `take`)

---

### Zone 3: Causeway Quarter (`4272–4301`, 30 rooms)

**Role:** Functional disaster-infrastructure core — ration towers, Central Market, Dispatch House, queue politics.

Required anchors:
- Four ration tower approach clusters (non-identical, 8 rooms)
- Central Market convergence decks (6 rooms)
- Mid-city Dispatch House (4 rooms, `indoors`)
- Ember Warden barracks stairs (3 rooms)
- Queue lanes, wash channels, storehouse ribs (9 rooms)
- **Boss room:** one `no_mob` room for the Causeway Enforcement Marshal

Mob focus (`4358–4370`, 13 mobs):
- Ration clerk (`sentinel` + `stay_area`)
- Tower guard (hostile, `sentinel` + `stay_area`; `spec_guard` for law-enforcement role)
- Market quarter laborer (`stay_area`, `spec_laborer`)
- Market vendor (`sentinel` + `stay_area`, `spec_vendor`)
- Black-register smuggler (hostile, `stay_area`)
- Petition agitator (`stay_area`)
- Wash channel crew member (`stay_area`)
- Storehouse rib stevedore (`stay_area`)
- Queue lane enforcer (`sentinel` + `stay_area`)
- Dispatch House scrivener (`sentinel` + `stay_area`)
- Tower access gatekeeper (`sentinel` + `stay_area`)
- Causeway lamp lighter (`stay_area`, `spec_lamplighter`)
- **Boss: Causeway Enforcement Marshal** (`is_npc` + `sentinel` + `stay_area` + `boss`; placed in `no_mob` room; loot: `ITEM_BOSS` + `ITEM_LOOT`)

Object focus (`4406–4418`, 13 objects):
- Ration chit bundle (`quest` type, `take`)
- Sealed grain crate (`container` type, `take`)
- Ash-scrub wash tool set (`treasure` type, `take`)
- Market contract tablet (`quest` type, `take`)
- Dispatch seal kit (`treasure` type, `take`)
- Tower access medallion (`key` type, `take`)
- Queue lane warden baton (`weapon` type, `hold` + `take`; `value3` = `7` pound)
- Black-register smuggler's pouch (`container` type, `take`)
- Barracks posting board (`board` type, `take`)
- Storehouse tally ledger (`treasure` type, `take`)
- Causeway inspection slate (`treasure` type, `take`)
- Warden's dispatch lantern (`light` type, `take`)
- **Marshal's enforcement blade** (boss weapon; `weapon` type, `ITEM_BOSS` + `ITEM_LOOT`, `hold` + `take`; `value3` = `1` slice)

---

### Zone 4: Upper Civic Ring (`4302–4321`, 20 rooms)

**Role:** Preserved civic layer where symbolism, governance, and archival power converge.

Required anchors:
- Bloom Pavilion council floor (3 rooms, `indoors`)
- Garden Ledger Archive — public board and restricted stacks (4 rooms, `indoors`)
- Promise Stone Plaza and Founder's Hearth (3 rooms)
- Upper Dispatch House (2 rooms, `indoors`)
- Steward residences and oath-walk galleries (5 rooms, `indoors`)
- Connecting ring passage (3 rooms)
- **Boss room:** one `no_mob` room for Senior Steward Colwen Dast

Mob focus (`4371–4379`, 9 mobs):
- Bloom Council steward (`sentinel` + `stay_area`)
- Garden Ledger clerk (`sentinel` + `stay_area`)
- Archive custodian (`sentinel` + `stay_area`)
- Ember Warden liaison captain (`sentinel` + `stay_area`)
- Political envoy — Amendment Track (`stay_area`)
- Political envoy — Preservation Coalition (`stay_area`)
- Quiet Separatist courier (`stay_area`)
- Upper Dispatch House registrar (`sentinel` + `stay_area`)
- **Boss: Senior Steward Colwen Dast** (`is_npc` + `sentinel` + `stay_area` + `boss`; placed in `no_mob` room; loot: `ITEM_BOSS` + `ITEM_LOOT`)

Object focus (`4419–4428`, 10 objects):
- Edited thesis folio (`treasure` type, `take`)
- Archive challenge ledger (`quest` type, `take`)
- Council seal-box (`container` type, `take`)
- Promise Stone charcoal rubbing (`treasure` type, `take`)
- Hearth ember canister (`light` type, `take`)
- Unredacted transfer record (`quest` type, `take`)
- Oath-walk gallery placard (`treasure` type, `take`)
- Steward's correspondence bundle (`treasure` type, `take`)
- Archive shutter key (`key` type, `take`) — unlocks restricted-stacks door
- **Dast's authority sash** (boss armor; `armor` type, `ITEM_BOSS` + `ITEM_LOOT`, `about` + `take`)

---

### Zone 5: Canopy Workshops (`4322–4333`, 12 rooms)

**Role:** Eastern craft district governed by compact mutual-aid logic more than central ceremony.

Subclusters:
- Dye platforms (2 rooms)
- Ropewalk spans (3 rooms)
- Woodcraft sheds (2 rooms, `indoors`)
- Medicine drying lofts (2 rooms, `indoors`)
- Compact mediation circle (3 rooms)

Mob focus (`4380–4385`, 6 mobs):
- Workshop foremaster (`sentinel` + `stay_area`)
- Cordage crew member (`stay_area`)
- Herbal preparer (`stay_area`)
- Compact mediator (`sentinel` + `stay_area`)
- Quiet separatist organizer (`stay_area`)
- Dye platform tender (`stay_area`)

Object focus (`4429–4434`, 6 objects):
- Trade-ready cordage coil (`treasure` type, `take`)
- Canopy resin jar (`treasure` type, `take`)
- Dye pigment bundle (`treasure` type, `take`)
- Emergency repair kit (`treasure` type, `take`)
- Compact mutual-aid ledger (`treasure` type, `take`)
- Medicine loft drying rack weights (`treasure` type, `take`)

---

### Zone 6: Root Tunnels (`4334–4338`, 5 rooms)

**Role:** Flooded undercroft beneath terraces; pressure-release corridors and hidden movement lines.

Spaces:
- Siphon vault (1 room, `indoors` + `dark`)
- Submerged root arch passage (1 room, `indoors`)
- Maintenance culvert fork (1 room, `indoors`)
- Hidden archive transfer niche (1 room, `indoors`)
- Emergency ascent shaft (1 room, `indoors`)

Mob focus (`4386–4388`, 3 mobs):
- Tunnel pump keeper (`sentinel` + `stay_area`)
- Undercroft scavenger (`stay_area`)
- Ash-slick root leech (`aggressive` + `stay_area`)

Object focus (`4435–4438`, 4 objects):
- Valve key (`key` type, `take`) — unlocks siphon vault access hatch
- Floodgate chain (`treasure` type, `take`)
- Sealed testimony tube (`quest` type, `take`)
- Contraband ration satchel (`container` type, `take`)

---

## Boss Roster

Two named bosses, each in a `no_mob`-flagged room, each dropping `ITEM_BOSS` loot:

### Boss 1: Causeway Enforcement Marshal (Zone 3)
- `act` flags: `is_npc` + `sentinel` + `stay_area` + `boss`
- Placed only via reset into a `no_mob`-flagged room within `4272–4301`
- Boss loot: the Marshal's enforcement blade (`ITEM_BOSS` + `ITEM_LOOT`)
- Quest target for Zone 3 boss quest

### Boss 2: Senior Steward Colwen Dast (Zone 4)
- `act` flags: `is_npc` + `sentinel` + `stay_area` + `boss`
- Placed only via reset into a `no_mob`-flagged room within `4302–4321`
- Boss loot: Dast's authority sash (`ITEM_BOSS` + `ITEM_LOOT`)
- Quest target for Zone 4 boss quest (final chain quest with equipment reward)

---

## Mobile Design Summary (50 total)

Allocation by zone:
- Zone 1 (dispatch/entry): 8 mobs (`4339–4346`)
- Zone 2 (lower terraces): 11 mobs (`4347–4357`)
- Zone 3 (causeway quarter): 13 mobs (`4358–4370`, includes 1 boss)
- Zone 4 (upper civic ring): 9 mobs (`4371–4379`, includes 1 boss)
- Zone 5 (canopy workshops): 6 mobs (`4380–4385`)
- Zone 6 (root tunnels): 3 mobs (`4386–4388`)

Behavior rules:
- All mobs must carry `stay_area`; civic/guard mobs that must not wander must also carry `sentinel`.
- Keep market turbulence local; no area-wide chaotic wanderers.
- Use ambient city specials where appropriate (`spec_vendor`, `spec_laborer`, `spec_warden`, `spec_lamplighter`).
- Never assign `spec_summon_*` specials; never assign `spec_keep_*` specials in area files.
- Sub-regions containing hostile mobs that lack `sentinel` must be enclosed by `no_mob`-flagged rooms so they cannot escape their sub-region.

---

## Object Design Summary (50 total)

Allocation by zone:
- Zone 1 (approach): 7 objects (`4389–4395`)
- Zone 2 (lower terraces): 10 objects (`4396–4405`)
- Zone 3 (causeway): 13 objects (`4406–4418`, includes 1 boss drop)
- Zone 4 (upper ring): 10 objects (`4419–4428`, includes 1 boss drop)
- Zone 5 (workshops): 6 objects (`4429–4434`)
- Zone 6 (root tunnels): 4 objects (`4435–4438`)

Thematic families:
- **Administrative material culture** (ledgers, seals, writ cases, board notices): ~12 objects
- **Survival infrastructure** (flood tools, ration implements, stilt/brace kits): ~10 objects
- **Founding relics** (Promise Stone rubbings, Compact copies): ~4 objects
- **Workshop trade goods** (rope, dye, medicine): ~6 objects
- **Political evidence objects** (petition drafts, redacted records): ~6 objects
- **Equipment (wearable/holdable)** (armor, weapons, jewelry): ~6 objects
- **Keys and specialized containers**: ~6 objects

Distribution rule:
- Each zone must include at least one object thread that reveals the same event from competing institutional perspectives.
- All loot-table items must have `ITEM_LOOT`; boss drops must also have `ITEM_BOSS`.

---

## Quest Plan

Quest file numbers are to be allocated against the global quest index during implementation (requires expanding `PROP_MAX_STATIC_QUESTS` beyond `48` if the global cap is still at that value).

### Quest 1 — Rakuen Cartography: Dispatch Approach to Upper Ring *(required cartography quest)*

- **Type:** `1` (kill-variety)
- **Targets:** three patrol/guard types spread across Zones 1–3 (dispatch registrar family, tower guard family, queue lane enforcer)
- **Offerer:** dispatch registrar (Zone 1, `4339–4346` range)
- **Reward:** 6500 gold, 6 qp
- **Level:** 45–75
- **Prereq:** `-1`

### Quest 2 — Causeway Queue Disorder

- **Type:** `3` (kill-count)
- **Target:** Causeway Enforcement Marshal (Zone 3 boss)
- **Kill needed:** 1
- **Offerer:** dispatch clerk (Zone 3, `4358–4370` range)
- **Reward:** 7200 gold, 6 qp + equipment piece (the Marshal's enforcement blade)
- **Level:** 50–80
- **Prereq:** `-1`

### Quest 3 — Archive Record Seizure

- **Type:** `2` (collect-items)
- **Targets:** collect 3 archive challenge ledger objects (Zone 4)
- **Offerer:** Garden Ledger clerk (Zone 4, `4371–4379` range)
- **Reward:** 6000 gold, 5 qp
- **Level:** 55–80
- **Prereq:** `-1`

### Quest 4 — Steward Colwen Dast Removal Writ *(final quest in chain; equipment reward)*

- **Type:** `3` (kill-count)
- **Target:** Senior Steward Colwen Dast (Zone 4 boss)
- **Kill needed:** 1
- **Offerer:** political envoy (Zone 4, `4371–4379` range)
- **Reward:** 1832 gold, 8 qp + equipment piece (Dast's authority sash)
- **Level:** 60–90
- **Prereq:** Quest 3 (static ID of archive seizure quest)

### Quest Constraint Compliance

- Cartography quest included (Quest 1). ✓
- Boss quest (Quest 2) rewards equipment. ✓
- Boss quest (Quest 4) rewards equipment. ✓
- Final quest in chain (Quest 4) rewards equipment. ✓

---

## Lock and Key Plan

Diegetic locked exits only — no arbitrary dungeon locks:

1. **Archive restricted stacks door** (Zone 4, Upper Civic Ring)
   - Exit: `EX_ISDOOR` set; reset state: locked (`D ... 2`)
   - Key: archive shutter key (object `4427`)
   - Key must be placed on a guarded mob or in a discoverable container via `#RESETS`

2. **Siphon vault access hatch** (Zone 6, Root Tunnels)
   - Exit: `EX_ISDOOR` set; reset state: locked (`D ... 2`)
   - Key: valve key (object `4435`)
   - Key must be placed in Zone 5 or Zone 6 on an appropriate mob/room reset

---

## Political + Festival Implementation Layer

- **Festival of Ever-Bloom:** temporary decoration resets in Upper Ring and Lower Terraces (performative abundance over damaged stone and flooding).
- **Ledger Vigil:** archive board and plaza messaging; challenge ledgers and testimony objects become foreground interactables.
- **Founder's Reckoning:** protest inscriptions and petition rhetoric intensify near Promise Stone Plaza.

Factions are represented via dialogue, patrol overlaps, and object text rather than hard citywide phase states.

---

## Reset and Flow Strategy

1. Seed entry and dispatch rhythms first — Zone 1 sets arrival readability.
2. Layer causeway queue pressure via market and tower population resets — Zone 3 densest spawn.
3. Keep upper ring socially higher-stakes with named NPC concentration — Zone 4.
4. Root tunnel resets are low-frequency hazard and lore shortcut — Zone 6.
5. Boss resets: limit 1 each, fixed lairs in `no_mob` rooms, quest-friendly persistence.
6. No blank lines anywhere in `#RESETS`.

---

## Cross-Area Integration

- **Eccentric Woodland:** entry room `4239` north connects to EW room `1830` south. Approach rooms preserve "managed denial" and route-variance tone.
- **Midgaard:** documents and NPC dialogue reflect subsidy and liability dispute language.
- **Kiess/Kowloon/Mafdet hooks:** references to amendment review, grain compacts, and maritime fallback networks.
- **Kel'Shadra linkage:** archive-loss and testimony-disappearance lore appears as record gaps and contested annotations.

---

## Implementation Sequence

1. Write `#AREA` header with strict compliance fields (`Q 16`, `V 4550 4749`, `O Virant~`, keyword, `I 45 95`, `F 20`, reset message).
2. Build 100-room organic graph across six zones with vertical connectors; wire north entry (`4239`) to EW room `1830` south.
3. Author 50-mobile roster (`4339–4388`) by institution and district function; assign `stay_area` to all; `sentinel` to civic/guard mobs; boss flags to the two named bosses.
4. Author 50-object roster (`4389–4438`) with correct `item_type`, wear flags (all include `take`), and extra flags (`ITEM_LOOT`, `ITEM_BOSS` as required).
5. Write `#RESETS` — civic rhythm, boss room placements (limit 1 each in `no_mob` rooms), door resets for two keyed exits; no blank lines.
6. Validate exits — all bi-directional; locked doors have valid key vnums and `EX_ISDOOR` set.
7. Write `#SPECIALS` for applicable ambient city mobs; write `#OBJFUNS` if any object functions are assigned.
8. Add quest prop files and wire offerer vnums.
9. Run spec audit: string formatting, color policy, vnum envelope compliance, section terminators, bitvector hygiene.

---

## Acceptance Checklist

- [ ] Vnum envelope `V 4550 4749` (35 vnums total).
- [ ] `Q 16` and `O Virant~` present in `#AREA` header.
- [ ] Room count exactly 100 with zone split `13/20/30/20/12/5`.
- [ ] All rooms within `4239–4338`; all mobs within `4339–4388`; all objects within `4389–4438`.
- [ ] Layout is vertical/organic and clearly non-grid.
- [ ] Major anchors implemented: Dispatch Houses, Central Market, Bloom Pavilion, Archive, Promise Stone Plaza, Canopy Workshops, Root Tunnels.
- [ ] Two named bosses in `no_mob` rooms with `sentinel` + `boss` flags.
- [ ] Political factions and festivals visible in text and encounter ecology.
- [ ] Eccentric Woodland north entry link (`4239 ↔ 1830`) documented and implemented bidirectionally.
- [ ] Cartography quest included.
- [ ] Boss quests (both) reward equipment.
- [ ] Final quest in chain rewards equipment.
- [ ] All mobs include `stay_area`; civic/guard mobs include `sentinel`.
- [ ] All objects include `ITEM_TAKE`; no object includes `ITEM_WEAR_CLAN_COLORS`.
- [ ] Loot-table objects have `ITEM_LOOT`; boss drops have `ITEM_BOSS`.
- [ ] No blank lines in `#RESETS`; locked doors have valid key vnums and `EX_ISDOOR` set.
- [ ] Color theme used intentionally; no `@@k`, no background codes, no flashing.
- [ ] All area-spec constraints from `/docs` satisfied.
