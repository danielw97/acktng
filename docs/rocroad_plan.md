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

## Design Intent: Full Redo While Preserving Travel Function
The existing area already uses all room vnums and serves as a long transit chain. The rebuild keeps that macro function but completely replaces room identities, encounter ecology, objectization, and narrative flow so the route feels like a contested cultural frontier rather than generic wilderness.

### Core goals
1. **Keep all 99 room vnums active** and meaningful.
2. **Rebuild every room description and title** with stronger lore identity and route-stage progression.
3. **Introduce full mobile/object rosters** across the same vnum envelope.
4. **Tie route storytelling directly to Midgaard and Kiess history** (statecraft, trade law, military patrol doctrine, refugee memory, and frontier cult pressure).
5. **Maintain reliable traversal** for players who just need to travel, while adding optional side encounters, lore pockets, and mini-threat spikes.

## Spatial Model (99-room corridor with thematic bands)
Roc Road remains a longitudinal journey but is rebuilt as nine major bands plus connector micro-zones.

### Band A — Midgaard Gate March (3201-3211)
- Stone-set roadway under Midgaard charter markers.
- Guard relays, tax cairns, and old civic inscriptions.
- Encounter emphasis: patrols, petitioners, minor bandit scouts, courier traffic.

### Band B — Banner Hills and Switchbacks (3212-3219)
- Steeper terrain, signal poles, weather-exposed pass points.
- Visual language shifts from civic to frontier military.
- Encounter emphasis: rockfall hazards, hill raiders, watch outriders.

### Band C — River Ledger Crossing (3220-3227)
- Ford/bridge complex with customs past and present layered together.
- Introduces merchant guild tension and smuggling subtext.
- Encounter emphasis: ferrymen, toll ghosts, river predators, contraband runners.

### Band D — Dustward Verge (3228-3234)
- Drier transition where road maintenance degrades.
- Old milestone liturgy from pre-Kiess polities resurfaces.
- Encounter emphasis: scavengers, relic pickers, heat-maddened fauna.

### Band E — Crossroads Compact (3235-3243)
- The major strategic intersection where branch routes diverge.
- Rebuilt as a political pressure node with evidence of overlapping jurisdiction.
- Encounter emphasis: diplomatic envoys, mercenary escorts, opportunist thieves.

### Band F — Greenveil Spur (3244-3252)
- Forest-side path that once served as hidden troop and refugee movement line.
- Becomes a lore-rich optional path with druidic and exile traces.
- Encounter emphasis: foresters, displaced spirits, poachers, herbalists.

### Band G — Inlet, Cave, and Sinkline (3253-3261)
- Water-edge and cliff subzone with salvage economy and old smuggler pockets.
- Encounter emphasis: shoreline hunters, cave squatters, tidebound anomalies.

### Band H — Ruin Run and Weathered Causeway (3262-3274)
- Ancient city fragments integrated into roadbed over centuries.
- Strongest "deep time" lore in the route.
- Encounter emphasis: ruin custodians, script-haunted echoes, relic caravans.

### Band I — Western Shore Reach (3275-3299)
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
  - Crossroad/forest branch connectors
  - Sea and legacy side-area hooks

## Mobile Redesign (new full roster)
### Vnum strategy
- Use `3201-3299` as the mobile envelope (parallel namespace), with ~36 active mob templates and variant reset mixes.

### Factions
1. **Midgaard Roadwardens** — disciplined patrol infantry and legal clerks.
2. **Kiess Convoy Syndics** — caravan marshals, quarter agents, route accountants.
3. **Independent Wayfolk** — drovers, peddlers, shrine-keepers, ferrymen.
4. **Frontier Predators** — wolves, ridge cats, river ambushers, scavenger birds.
5. **Ruin-Touched Hostiles** — script-blind revenants, cairn lurkers, relic zealots.
6. **Shadow Trade Cells** — smugglers, extortion crews, forged-seal couriers.

### Boss/elite encounters (optional path pressure)
- **The Toll-Reeve Without Crown** (crossroads legal revenant)
- **Greenveil Antler-Warden** (forest spirit-warrior hybrid)
- **The Inlet Chainmaster** (smuggler captain, tide-cave holdout)
- **Rocroad Promontory Harrower** (coastal apex predator/human hybrid threat)

## Object Redesign (new full roster)
### Vnum strategy
- Use `3201-3299` as the object envelope, target ~42 objects with progression tiers.

### Item families
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

## Deliverables for Build Phase
1. Rewrite `#ROOMS` entries for all vnums `3201-3299`.
2. Add complete `#MOBILES` roster with faction tagging and level bands.
3. Add complete `#OBJECTS` roster aligned to encounter ecology.
4. Replace `#RESETS` with full population and loot cadence.
5. Add `#SPECIALS` only where behavior materially improves role identity.
6. Validate parse and policy compliance from `docs/area_file_spec.md`.
