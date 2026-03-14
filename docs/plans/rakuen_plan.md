# Area Plan: Rakuen, the Failed Paradise

## Overview

- **Area Name:** `@@G@@WRakuen, @@gCity of the Last Promise@@N`
- **Area File:** `area/rakuen.are` (new area)
- **Vnum Envelope (400 total):** `V 16000 16399`
- **Allocation Model:** 200 rooms (`16000-16199`), 100 mobiles (`16200-16299`), 100 objects (`16300-16399`)
- **Target Level Band:** 45-95 (urban/woodland political hazard band)
- **Reset Rate:** 20
- **Reset Message:** `@@gwater climbs the old root-weave and Rakuen resets its queues@@N`
- **Owner:** `Virant`
- **Primary Access:** Eccentric Woodland southern route into Rakuen's Dispatch House and Approach Terraces

Rakuen is built as a vertically layered, politically fractured city that still performs paradise while living under ration law, flood risk, and corridor dependence. Layout, encounters, and objectization must center this contradiction.

---

## Strict Spec Compliance (from `/docs`)

1. Use `Q 16` and `O Virant~` in `#AREA`.
2. Keep all authored rooms/mobs/objects within `16000-16399`.
3. Keep prose free of explicit vnum mentions.
4. Use valid `~`-terminated strings, no blank-line bodies.
5. Mobile `long_descr` must remain single-line format; mobile `description` must end with exactly one newline before `~`.
6. Use intentional ACK color coding only; do **not** use `@@k`, flashing, or background color codes.
7. Preserve canonical section structure and terminators (`#0`, `#$`).
8. Keep doors/locks/resets aligned with `room_spec` and `area_file_spec` conventions.

---

## Lore Anchors to Preserve

- **Identity:** failed paradise that refuses to call itself triage.
- **Vertical city logic:** Lower Terraces -> Causeway Quarter -> Upper Civic Ring, with Canopy Workshops on the eastern edge and Root Tunnels below.
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
- **Upper civic halo:** a partial ring around Bloom Pavilion and Promise Stone Plaza, with council shortcuts.
- **Eastern workshop comb:** lateral branchwork under dense canopy for industry and compact halls.
- **Subterranean siphon line:** narrow tunnel chain beneath terraces feeding flood cisterns.

Connectivity policy:
- Every zone has at least one loop and one vertical connector.
- Critical path remains readable; optional political/lore loops branch off it.
- Flooded detours and gateable causeways replace simple cardinal marching.

---

## Zone Plan (200 Rooms Total)

## Zone 1: Approach Terraces & Woodland Entry (`16000-16024`, 25 rooms)

**Role:** Transition from Eccentric Woodland uncertainty into Rakuen's managed-entry culture.

Key spaces:
- Outer guide-marker run
- Redwater crossing overlook
- Lower Dispatch House (entry)
- Route-board court
- First terrace stairs and flood warning drums

Mob focus (example range `16200-16214`):
- Dispatch registrar
- Canopy compact runner
- Woodland contract guide
- Exhausted caravan guard
- Opportunist route broker (low-tier hostile/social)

Object focus (example range `16300-16314`):
- Weathered route board slats
- stamped travel writ bundles
- compact rope harnesses
- flood-season marker lanterns

---

## Zone 2: Lower Terraces (`16025-16064`, 40 rooms)

**Role:** Flood-prone root-weave ruins, inhabited pockets, and rest gardens where paradise aesthetics persist under decay.

Subclusters:
- Stilted survivor lanes
- Root-weave house shells
- Rest garden shallows
- Collapsed promenade walk
- Ember Warden floodline stations

Mob focus (`16215-16234`):
- Lower Terrace compact stewards
- flood-watch wardens
- displaced gardeners
- root-rot scavengers
- drowned-root revenants (seasonal hazard set)

Object focus (`16315-16334`):
- salvage stilt braces
- moss-sealed grain jars
- promise-stone votive ribbons
- waterlogged ledger satchels

---

## Zone 3: Causeway Quarter (`16065-16124`, 60 rooms)

**Role:** Functional disaster-infrastructure core: ration towers, Central Market, Dispatch House, queue politics.

Required anchors:
- Seven ration tower approaches (non-identical)
- Central Market convergence decks
- Mid-city Dispatch House
- Ember Warden barracks stairs
- Queue lanes, wash channels, storehouse ribs

Mob focus (`16235-16264`):
- ration clerks
- tower guards (use city-appropriate specials as needed)
- market quarter laborers/vendors
- black-register smugglers
- petition agitators aligned with competing factions

Object focus (`16335-16364`):
- ration chit bundles
- sealed grain crates
- ash-scrub wash tools
- market contract tablets
- dispatch seal kits

---

## Zone 4: Upper Civic Ring (`16125-16164`, 40 rooms)

**Role:** Preserved civic layer where symbolism, governance, and archival power converge.

Required anchors:
- Bloom Pavilion council floor
- Garden Ledger Archive (public board + restricted stacks)
- Promise Stone Plaza + Founder's Hearth
- Upper Dispatch House
- steward residences and oath-walk galleries

Mob focus (`16265-16284`):
- Bloom Council stewards
- Garden Ledger clerks
- archive custodians
- Ember Warden liaison captains
- political envoys (Amendment/Preservation/Separatist voice lines)
- named leadership set (e.g., Senior Steward Colwen Dast, Commander Sallows)

Object focus (`16365-16384`):
- edited/unredacted thesis folios
- archive challenge ledgers
- council seal-boxes
- Promise Stone charcoal rubbings
- hearth ember canisters

---

## Zone 5: Canopy Workshops (`16165-16189`, 25 rooms)

**Role:** Economically vital eastern craft district governed by compact mutual-aid logic more than central ceremony.

Subclusters:
- Dye platforms
- Ropewalk spans
- Woodcraft sheds
- Medicine drying lofts
- Compact mediation circle

Mob focus (`16285-16294`):
- workshop foremasters
- cordage crews
- herbal preparers
- compact mediators
- quiet separatist organizers

Object focus (`16385-16394`):
- trade-ready cordage coils
- canopy resin jars
- dye vats and pigment bundles
- emergency repair kits

---

## Zone 6: Root Tunnels (`16190-16199`, 10 rooms)

**Role:** Flooded undercroft beneath terraces; pressure-release corridors and hidden movement lines.

Spaces:
- siphon vault
- submerged root arches
- maintenance culvert forks
- hidden archive transfer niche
- emergency ascent shaft

Mob focus (`16295-16299`):
- tunnel pump keepers
- undercroft scavengers
- ash-slick eels / root leeches
- one elite hidden witness-keeper encounter

Object focus (`16395-16399`):
- valve keys
- floodgate chains
- sealed testimony tube
- contraband ration satchel

---

## Political + Festival Implementation Layer

- **Festival of Ever-Bloom:** temporary decoration resets in Upper Ring + Lower Terraces (flowers over damaged stone, performative abundance).
- **Ledger Vigil:** archive board and plaza messaging updates; challenge ledgers and testimony objects become foreground interactables.
- **Founder's Reckoning:** protest inscriptions and petition rhetoric intensify near Promise Stone Plaza and Southmark Wall analogue spaces.

Factions are represented via dialogue, patrol overlaps, and object text rather than hard citywide phase states.

---

## Mobile Design Bible (100 total)

Families:
1. **Dispatch & Route Economy** (entry/causeway)
2. **Floodline Survival Castes** (lower terraces/root tunnels)
3. **Civic Ceremony Offices** (upper ring)
4. **Compact Labor Networks** (workshops)
5. **Political Agitators & Opportunists** (market/plaza)
6. **Named Institutional Figures** (limited-count anchors)

Behavior rules:
- Most civic mobs should be `sentinel` or short-patrol constrained.
- Keep market turbulence local; no area-wide chaotic wanderers.
- Use ambient city specials where appropriate (`spec_vendor`, `spec_laborer`, `spec_warden`, `spec_lamplighter`), avoiding prohibited summon/keep automation assignments.

---

## Object Design Bible (100 total)

Families:
- **Administrative material culture:** ledgers, seals, writ cases, board notices.
- **Survival infrastructure:** flood tools, ration implements, stilt/brace kits.
- **Founding relics:** Promise Stone rubbings, Compact copies, Dunnmark correspondence excerpts.
- **Workshop trade goods:** rope, dye, medicine, treated timber.
- **Political evidence objects:** discrepancy ledgers, petition drafts, redacted transfer records.

Distribution rule:
- Each zone gets at least one object thread that reveals the same event from competing institutional perspectives.

---

## Reset + Flow Strategy

1. Seed entry and dispatch rhythms first (arrival readability).
2. Layer causeway queue pressure via timed market/tower population resets.
3. Keep upper ring less dense but socially higher-stakes (named NPC concentration).
4. Use root tunnel resets as a low-frequency hazard + lore shortcut system.
5. Ensure all lock/key chains are diegetic (valves, archive shutters, tower gates), not arbitrary dungeon locks.

---

## Cross-Area Integration Requirements

- **Eccentric Woodland:** southern approach rooms must preserve "managed denial" and route-variance tone.
- **Midgaard:** documents and NPC dialogue should reflect subsidy/liability dispute language.
- **Kiess/Kowloon/Mafdet hooks:** include references to amendment review, grain compacts, and maritime fallback networks.
- **Kel'Shadra linkage:** archive-loss/testimony disappearance lore appears as record gaps and contested annotations.

---

## Implementation Sequence

1. Write `#AREA` header with strict compliance fields (`Q 16`, `V 16000 16399`, `O Virant~`).
2. Build 200-room organic graph by six zones and vertical connectors.
3. Author 100-mobile roster by institution and district function.
4. Author 100-object roster by governance/survival/trade/political-evidence families.
5. Implement resets for civic rhythm (queues, patrols, festival overlays).
6. Validate exits, door states, and all two-way dependencies.
7. Run spec audit for string formatting, color policy, vnum envelope, and section terminators.

---

## Acceptance Checklist

- [ ] Exactly 400 authored vnums used inside `16000-16399`.
- [ ] Room count exactly 200 with zone split `25/40/60/40/25/10`.
- [ ] Layout is vertical/organic and clearly non-grid.
- [ ] Major anchors implemented: Dispatch Houses, Central Market, Bloom Pavilion, Archive, Promise Stone Plaza, Workshops, Root Tunnels.
- [ ] Political factions and festivals are visible in text and encounter ecology.
- [ ] Cross-area corridor pressure from Eccentric Woodland is present in approach and dispatch content.
- [ ] All area-spec constraints from `/docs` satisfied.
