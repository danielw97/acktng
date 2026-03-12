# Area Plan: The Sands of Akh'enet (Full Rebuild)

## Overview

- **Area Name:** `@@yThe Sands of Akh'enet@@N`
- **File:** `akhenet.are`
- **Assigned Vnum Range (fixed):** `2000-2149`
- **Level Range:** `70-90`
- **Room Utilization Target:** **150/150 rooms used** (all room vnums from 2000 through 2149)
- **Goal:** Fully replace current sparse/linear desert-ruin layout with a complete, lore-consistent oasis-pyramid frontier necropolis tied to the Oasis-Pyramid Corridor.

This plan keeps the exact current area envelope and rebuilds rooms, mobs, objects, resets, and progression to reflect Akh'enet as a living ruin-city where water law, burial theology, and caravan survival collide.

---

## Multi-Color Theme (Lore-Matched)

Akh'enet must read as heat + ritual + hidden water + moonlit undercrypt.

- `@@y` **Sun-gold:** open dunes, exposed roads, caravan seals, authority tablets.
- `@@b` **Stone-brown:** buried masonry, wind-scoured pylons, ancient roadwork.
- `@@l` **Aquifer-blue:** cistern channels, hidden wells, legal-water shrines.
- `@@p` **Funerary purple:** moon rites, descent halls, curse-lit glyph chambers.
- `@@R` **Hazard red:** collapse warnings, active curse zones, boss arenas.
- `@@W` **Chalk-white:** survey lines, keeper markings, salt-bleached memorials.
- Always close with `@@N`; do not use `@@k`, background colors (`@@0-@@7`), or flashing (`@@f`).

---

## Lore Anchors and Narrative Direction

Akh'enet is the **ruined relay-city between the Southern Oasis and Southern Pyramid routes**, originally founded as a Keeper-era measure station that became a Ledger Kingdom mortuary-port, then a Moon Sovereign doctrinal checkpoint. In the current era, Akh'enet is fought over by:

1. **Reed Ledger Wardens** (oasis-backed water-law custodians),
2. **Moon Sepulchrists** (descent cult revivalists),
3. **Salt Jackal scavenger circles** (curse-profiteers and tomb looters),
4. **Independent caravan freeholds** (trade survivalists trying to keep routes open).

The area should feel like a border institution under collapse, not a generic desert battlefield.

---

## Spec-Strict `#AREA` Header Plan

- `Q 16`
- `K sands akhenet desert ruins~`
- `L @@W(@@y70@@W-@@R90@@W)@@N~`
- `I 70 90`
- `V 2000 2149`
- `F 15`
- `U @@yblistering winds rake Akh'enet as buried bells ring beneath the dunes@@N~`
- `O Virant~` (owner must be exact-cased per spec)

String/format constraints to enforce during implementation:
- Every string `~`-terminated.
- No blank-line doubles in any string.
- No vnums in in-world text.
- Mob `long_descr` one-line only + newline + `~` line.
- Mob/room/extra descriptions end with exactly one newline before `~`.

---

## Full Room Topology (All 150 Rooms Used)

### District A — Sunbreak March (2000-2019, 20 rooms)
**Surface entry dunes and exposed caravan approach.**
- Gameplay: orientation, early hostiles, weather pressure.
- Features: shattered waystones, heat mirage spans, caravan wreck markers.
- Anchor rooms:
  - 2000 **Sunbreak Threshold** (entry hub)
  - 2008 **Survey Obelisk Stump**
  - 2015 **Half-Buried Toll Arch**
  - 2019 **Marchfall Descent Lip** (transition to buried roads)

### District B — Keeper Roadworks (2020-2044, 25 rooms)
**Submerged processional road, legal-marker remnants, drainage channels.**
- Gameplay: branched lanes, lore-heavy inscriptions, first locked route.
- Features: measure-script slabs, collapsed milestones, audit niches.
- Anchor rooms:
  - 2024 **Road Court Kiosk**
  - 2031 **Broken Distance Circle**
  - 2038 **Sluice Gate of Bearings** (keyed door)
  - 2044 **Deepshade Causeway Mouth**

### District C — Shuttered Bazaar of Reeds (2045-2069, 25 rooms)
**Buried commercial quarter once tied to Great/Southern Oasis water contracts.**
- Gameplay: lateral loops, faction negotiation pockets, ambush alleys.
- Features: reed-weigh halls, ration stamp offices, relic appraisal booths.
- Anchor rooms:
  - 2050 **Ledger Canopy Rotunda**
  - 2056 **Seal-Fire Notary**
  - 2063 **Caravan Bone Exchange**
  - 2069 **Moon Tax Stairwell**

### District D — Cistern Veins & Undersluices (2070-2094, 25 rooms)
**Hydraulic underworks linking Akh'enet to the Deepwell Confluence myth-cycle.**
- Gameplay: environmental hazard routing, water-control puzzles, ritual sabotage.
- Features: blue-lit seep halls, pressure doors, drowned record vaults.
- Anchor rooms:
  - 2072 **Cold Seep Gallery**
  - 2079 **Confluence Gauge Hall**
  - 2086 **Brine Sentence Vault**
  - 2094 **Nadir Pump Antechamber**

### District E — Necropolis Terraces (2095-2124, 30 rooms)
**Vertical funerary quarter where Moon Sovereign descent theology overwrote Keeper law.**
- Gameplay: elite cult packs, curse mechanics, high lore density.
- Features: tomb balconies, moonwells, jackal verdict stelae.
- Anchor rooms:
  - 2100 **Terrace of Quiet Debt**
  - 2108 **Jackal Balance Court**
  - 2115 **Moon Ladder Gallery**
  - 2124 **Sepulchral Wind Balcony**

### District F — Nadir Palace & Astral Well (2125-2149, 25 rooms)
**Final complex: command necropolis, ritual engine, and boss endcap.**
- Gameplay: gauntlet + court chambers + final confrontation.
- Features: mirrored moon discs, resonance shafts, legal-funerary throne hall.
- Anchor rooms:
  - 2130 **Hall of Unwatered Kings**
  - 2136 **Resonance Shaft of Sighs**
  - 2142 **Astral Well Rim**
  - 2148 **Court of the Eclipsed Ledger** (boss room)
  - 2149 **Sealed Return Gate** (post-boss egress)

---

## Traversal Rules

- Main spine runs 2000 -> 2149 in district order with optional side loops inside each district.
- All exits bidirectional.
- No non-maze directional loops.
- Named exits (`^...`) used for ritual doors, legal gates, and special passages.
- Door states controlled via resets (`D`), not by invalid lock bit usage.
- At least two inter-district route options between B/C, C/D, and D/E for replayability.

---

## Mob Rebuild Plan

### Vnum Allocation
- **Mob templates:** `2000-2049` (50 total; sequential, no gaps)
- **Boss/mini-boss concentration:** 2042-2049

### Faction Sets

1. **Reed Ledger Wardens** (neutral/defensive)
   - Patrol marshals, aquifer scribes, sluice veterans.
   - Emphasis: anti-curse support, defensive formations.

2. **Moon Sepulchrists** (hostile cult)
   - Quiet cantors, descent blades, silence adepts.
   - Emphasis: debuffs, silence, identity-erasure flavor.

3. **Salt Jackal Reclaimers** (hostile scavengers)
   - Tomb-cutters, bone appraisers, legal-forgery raiders.
   - Emphasis: burst melee, loot pressure.

4. **Caravan Freehold Survivors** (mixed)
   - Contract guards, medics, route-finders, relic brokers.
   - Emphasis: utility, occasional quest hooks.

5. **Undersluice Aberrants** (hostile environmental)
   - Brine shades, calcified eels, drowned adjudicator remnants.
   - Emphasis: resist variance by district.

### Key Encounters
- **Mini-boss A (2068):** `The Seal-Break Factor` (jackal forgery master).
- **Mini-boss B (2118):** `Silent Judge of the Ninth Descent` (cult adjudicator).
- **Final Boss (2148):** `Akhenet Nadir Regent` — fusion of court bureaucracy and moon-rite engine.

Boss rooms flagged `no_mob`; bosses flagged sentinel + boss; strong elites use solo as appropriate.

---

## Object Rebuild Plan

### Vnum Allocation
- **Object templates:** `2050-2149` (100 total; sequential, no gaps)

### Item Families
- **Water-law relics:** ration seals, tally rods, witness tablets.
- **Funerary regalia:** moon masks, jackal cords, descent vestments.
- **Cistern apparatus:** pressure keys, sluice crowns, gauge lenses.
- **Combat rewards:** tiered 70-90 gear with distinct archetype support.
- **Lore interactables:** readable tablets, inscription shards, court dockets.

### Required Keys / Doors
- Key items for:
  - Sluice Gate of Bearings (2038)
  - Brine Sentence Vault (2086)
  - Court of the Eclipsed Ledger (2148)

---

## Reset and Population Philosophy

- District identity enforced by reset clusters (surface patrols, market scavengers, underworks aberrants, terrace cult cells, palace court guard).
- Patrol mobs constrained by subregion or no_mob boundaries to prevent bleed.
- Shopkeepers and lore NPCs marked sentinel in secure rooms.
- Risk curve:
  - A/B low-mid pressure,
  - C/D sustained attrition,
  - E spike encounters,
  - F controlled endgame gauntlet.

---

## Expansion Goals vs Current Area

This rebuild explicitly replaces prior sparse desert repetition with:
- complete room utilization,
- stronger oasis/pyramid lore continuity,
- factional social logic (not just random hostiles),
- hydraulic + funerary thematic cohesion,
- high-value lore surfaces for long-term quest integration.

---

## Implementation Checklist

1. Rewrite `#ROOMS` to cover all vnums 2000-2149 (unique descriptions, 3+ sentences; major rooms 5+).
2. Rebuild `#MOBILES` sequentially 2000-2049 with full extension-line compliance.
3. Rebuild `#OBJECTS` sequentially 2050-2149 with loot/flags/key integrity.
4. Re-author `#RESETS` for district populations, doors, shops, and boss gating.
5. Validate header and all strings against `docs/area_file_spec.md`.
6. Run lint/validation pass for exits, vnum envelope, and string termination.

