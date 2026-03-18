# Area Plan: Saltglass Reach

## Overview

**Area Name:** `@@yThe @@bSaltglass @@yReach@@N`
**File Name:** `saltglass_reach.are`
**Keyword:** `saltglass reach desert littoral cairn`
**Area Number:** `N 87`
**Level Range:** `I 55 80` (`55-80`)
**Vnum Range:** **6300-6699** (`V 6300 6699`)
**Room Count Target:** **400/400 rooms used**
**Primary Design Goal:** Build a massive jurisdictional transition corridor spanning four ecological bands — from the oasis caravan basin to the Mafdet coast — as a traversable frontier of overlapping legal authority, shattered survey infrastructure, and environmental hazard. The area is not a linear road; it is a web of routes, ruins, and contested waypoints where inland law fractures into maritime pragmatism.

**Western Connections:** Great Oasis (1932-2031), Northern Oasis (2032-2231), Southern Oasis (2539-2738) — the Reach's western approach connects to the oasis caravan basin.
**Eastern Connections:** Mafdet (future area) — the Reach's eastern terminus opens onto the Mafdet Strand and the port-city's harbor jurisdiction.

---

## Color Theme (Multi-Color, Lore-Matched)

- `@@y` **Sun Ledger Gold**: charter marks, survey law, toll-era inscriptions, Keeper obelisk remnants.
- `@@b` **Salt Brown**: dune clay, dried brine crust, cairn station masonry, cargo sled debris.
- `@@a` **Littoral Cyan**: tidal pools, brackish seep-lines, Tidemouth moisture, maritime conversion text.
- `@@l` **Glassfield Blue**: fused silica surfaces, translucent shards, Glasswind reflection.
- `@@p` **Synod Purple**: Jackal cult graffiti, Eclipsed Tongues rite residue, smuggler waymarks.
- `@@R` **Threat Red**: boss foreshadowing, thermal fault warnings, amber/violet glass danger zones.
- `@@g` **Calcified Gray-Green**: weathered cairn stone, old survey markers, lichen on ruins.
- `@@N` reset after each themed segment.

Theme restrictions (spec aligned):
- No `@@k` as thematic foreground.
- No flashing code `@@f`.
- No background codes `@@0`-`@@7`.

---

## Core Narrative Direction

The Saltglass Reach is a **jurisdictional gradient** — not empty desert but a corridor where the legal, economic, and cosmological systems of inland oasis civilization were adapted (often violently) to accommodate the reality of the sea. Every cairn, marker, and path implies a former purpose: toll collection, cargo certification, jurisdictional boundary. The glass and salt terrain is evidence of catastrophe (Iseth Reach drying, Ninth Meridian thermal pulse). Factions compete not for territory but for legal authority — the right to issue seals, collect tolls, and define valid passage.

Players traverse from oasis-connected caravan staging grounds through increasingly contested and environmentally hostile terrain, encountering the ruins of three parallel trade routes, the factions that now fight over their remnants, and the geological scars of the Ninth Meridian catastrophe. The area culminates in the Tidemouth approach to Mafdet, where inland law formally yields to maritime jurisdiction at the ancient Shoreward Seal.

---

## Cross-Area Lore Anchors

### Oases (West — Primary Connection)

- **Great Oasis** (1932-2031, L40-60): The institutional headwater for the Reach's legal systems. Charter witness formulas from the Great Oasis appear in adapted form on Reach cairn tablets. The Great Oasis historically functioned as the last major freshwater arbitration basin for caravans bound east to the coast — seaward convoys took certified water-right and charter attestations from the oasis, crossed the Reach, and converted claims into harbor contracts at Mafdet. The Great Oasis Measuring House's core samples from the Mirror Flats document forty seasonal layers of Iseth Reach flooding.
- **Northern Oasis** (2032-2231, L45-65): Ration-stamp conventions influenced the Reach's transit water tracking. Reach Wardens issue transit ration tokens modeled on Northern Oasis ceramic stamps. Mafdet's Ledger House clerks accept Northern Oasis witness seals at premium rates.
- **Southern Oasis** (2539-2738, L50-70): Casualty-processing protocols extend into the Reach — deaths on the Reach are recorded using Southern Oasis Casualty Ledger formats and forwarded for archival. The Southern Oasis Mortuary Circle maintains correspondence with Mafdet's Quarantine Ledger regarding relic hazard classifications. Southern Pyramid relics authenticated at the Southern Oasis carry premium provenance in Mafdet's maritime market.

### Mafdet (East — Future Connection)

- Mafdet is the principal coastal endpoint. The Reach's Tidemouth Dunes open onto Mafdet Strand, where the Shoreward Seal marks the formal boundary between Reach Warden and Harbor Warden jurisdiction.
- The Shrine of the First Claw's oath-enforcement tradition descends from the Risk Assumption Oath performed at the Shoreward Seal.
- Mafdet's Ledger Houses use notation descended from the Shoreward Tables — conversion ratio charts developed by Shoreward Assessors at Reach cairn stations.
- The Strandline Compact's "harbor first" principle governs the eastern boundary of Reach authority.
- The Red Sand Accounts operate Outriders inland through the Reach, purchasing distressed cargo and cairn station rights.

### Other Connections

- **Khar'Daan**: Funerary-bureaucratic tradition directly influenced the Reach's Sealed Route quarantine inspection protocols.
- **Arroyo**: Canyon-court legal tradition shares thematic DNA with cairn-court systems; "thirst oath" vocabulary appears on Reach cairn tablets.
- **Kowloon**: Jade Clerks fund expeditions to Measure's Rest seeking Littoral Branch source documents.
- **Midgaard**: Violet Compact registry protocols adapted by some Reach Wardens as models for relic transit documentation.
- **Scorched Wastes**: Both regions were physically transformed by the Ninth Meridian catastrophe — the Reach is the Scorched Wastes' eastern echo.

---

## `#AREA` Header Requirements (strict)

- `Q 16` mandatory.
- `K saltglass reach desert littoral cairn~`
- `L @@y{55 80}@@N~`
- `N 87`
- `I 55 80`
- `V 6300 6699` fixed.
- `F 15`
- `O Virant~`
- `U @@bSalt-crusted wind carries the scent of brine and old contracts across the @@ySaltglass Reach@@b.@@N~`

---

## Global Spec Compliance Rules

Implementation must follow `docs/area_file_spec.md` requirements, including:

- All strings `~`-terminated.
- No back-to-back newline runs in strings.
- No vnums in any in-world text.
- Room descriptions: minimum 3 sentences; major rooms minimum 5.
- Unique room descriptions throughout the area.
- Mobile `long_descr`: one text line only, then `~` line.
- Mobile `description`: exactly one trailing newline before `~`.
- Named exits must be text-anchored in room description, object text, or room extra description.
- Exits bi-directional except in maze-flagged rooms.
- All mobs flagged `stay_area`; bosses flagged `sentinel`, `boss`, placed in `no_mob` rooms.
- Strong non-boss mobs flagged `solo`.
- Sub-regions enclosed by `no_mob` boundary rooms or mobs flagged `sentinel`.
- Sequential vnum allocation, no gaps.
- All objects include `ITEM_TAKE`. Loot-table items include `ITEM_LOOT`. Boss drops include `ITEM_BOSS`.
- Object names unique within the area.

---

## Room Topology (All 400 Rooms Used)

The area is divided into **8 districts** of **50 rooms each**, arranged in four geographical bands with branching route networks. The layout is deliberately non-linear: the three historical trade routes (Swift Line, Heavy Road, Sealed Route) create parallel spines with cross-connections, dead-end ruins, hidden Glasswind paths, and vertical cairn station interiors.

### District Layout Overview

```
                    WEST (Oases)
                        |
            +-----------+-----------+
            |           |           |
    [A: Oasis          [B: Mirror        [C: Mirror
     Staging]           Flats North]      Flats South]
     6300-6349        6350-6399       6400-6449
            |           |           |
            +-----+-----+-----+----+
                  |           |
          [D: Glasswind    [E: Cairn Line
           Belts]           Ruins]
           6450-6499      6500-6549
                  |           |
            +-----+-----+----+
            |                 |
    [F: Sealed Route   [G: Tidemouth
     Remnants]          Approach]
     6550-6599        6600-6649
            |                 |
            +--------+--------+
                     |
              [H: Shoreward
               Threshold]
               6650-6699
                     |
                EAST (Mafdet)
```

The topology is not a grid. Each district has its own internal shape:

- **District A** fans out from a single oasis-road entry into a caravan staging hub with multiple departures.
- **Districts B and C** are the Mirror Flats — two parallel crossing routes (north/south) with maze-flagged disorientation zones in the center.
- **District D** is a labyrinthine Glasswind Belt with branching hazard paths and hidden Glasswalk Guide routes.
- **District E** is a ruined cairn station complex with vertical interiors (up/down), fortified halls, and the Measure's Rest library.
- **District F** follows the abandoned Sealed Route southward through quarantine ruins and Synod-infiltrated passages.
- **District G** is the Tidemouth Dunes — an open, wind-swept approach where desert gives way to brackish coast.
- **District H** is the Shoreward Threshold — the legal boundary zone where inland law formally yields to maritime jurisdiction, culminating at the Shoreward Seal itself.

---

## Room-by-Room Allocation (6300-6699)

### District A: Oasis Staging Grounds (6300-6349, 50 rooms)

**Theme:** Caravan marshaling area at the western mouth of the Reach, where oasis-certified convoys prepare for the crossing. Warden patrol headquarters, toll-assessment booths, and guide-hiring posts cluster around a dusty transit yard. This is the last place where oasis law operates without question.

**Internal Shape:** A central hub (the Staging Yard) with five spokes — north road from the Northern Oasis approach, south road from the Southern Oasis approach, west road from the Great Oasis approach, and two eastward departures (one toward the Mirror Flats North, one toward Mirror Flats South). Side branches lead to Warden barracks, a guide camp, a Red Sand counting house, and caravan repair stalls.

Key rooms:
- 6300 **Western Approach Road** — entry from oasis basin; 5+ sentences; connects west to oasis areas.
- 6301 **Dust-Choked Milestone** — first Reach marker, eroded Keeper survey stone.
- 6302 **Caravan Muster Ground** — open staging area, caravan traffic flavor.
- 6303 **Warden Toll Gate** — first toll checkpoint, manned by Reach Wardens.
- 6305 **Staging Yard** — central hub, 5+ sentences, multiple exits radiating outward.
- 6308 **Reach Warden Barracks** — patrol headquarters, sentinel guards.
- 6310 **Glasswalk Guide Camp** — neutral guide-hiring post, route song flavor.
- 6312 **Red Sand Counting House** — Outrider commercial predator den.
- 6315 **Northern Oasis Approach** — road connecting north toward Northern Oasis.
- 6318 **Southern Oasis Approach** — road connecting south toward Southern Oasis.
- 6320 **Caravan Repair Yard** — sled runners, harness mending, shade shelters.
- 6324 **Transit Seal Office** — where crossing permits are issued.
- 6328 **Drywell Rest Stop** — last functioning well before the Flats.
- 6330 **Heavy Road Departure** — eastward exit toward Mirror Flats South / central route.
- 6335 **Swift Line Departure** — eastward exit toward Mirror Flats North / fast route.
- 6340 **Provisioner's Row** — merchant stalls for crossing supplies.
- 6345 **Cairn Tablet Reading Room** — scholarly annex with extra descriptions for lore.
- 6349 **Eastern Boundary Marker** — last oasis-jurisdiction marker, transition text.

Remaining rooms (6304, 6306-6307, 6309, 6311, 6313-6314, 6316-6317, 6319, 6321-6323, 6325-6327, 6329, 6331-6334, 6336-6339, 6341-6344, 6346-6348): connecting passages, side alleys, storage rooms, guard posts, caravan animal pens, shade structures, and ambient transition corridors filling out the hub layout.

---

### District B: Mirror Flats North — Swift Line (6350-6399, 50 rooms)

**Theme:** The northern crossing of the Mirror Flats, following the old Swift Line — the fast route for light, high-value cargo. Hard salt crust blinds by day and reflects moonlight at night. Caravans cross at dawn and dusk. The Flats are a geological remnant of the Iseth Reach's terminal evaporation basin. Salt crickets swarm during fog events. This route is faster but more exposed.

**Internal Shape:** A sinuous east-west spine with short north-south deviations into salt-crystal formations, exposed survey rod stations, and a central maze-flagged blinding zone where the reflection is worst. The northern edge has a hidden path leading to a salt cricket breeding ground. The eastern end descends into the Glasswind Belt approach.

Key rooms:
- 6350 **Swift Line Marker One** — first cairn of the northern route, faded toll schedule.
- 6353 **Salt Mirror Crossing** — blinding white expanse, 5+ sentences, dawn/dusk crossing flavor.
- 6355 **Keeper Tide Rod Station** — ruined tide-measurement outpost with basalt stumps.
- 6358 **Crystal Ridge** — elevated salt formation, lookout point, cairn kite nests.
- 6360 **The Blinding Heart** — center of the Flats, maze-flagged zone begins (ROOM_MAZE).
- 6361-6367 **Maze: Reflected Expanse** — 7 maze rooms, disorienting salt-mirror terrain.
- 6370 **Salt Cricket Breeding Ground** — swarm encounter pocket, hidden north branch.
- 6373 **Shattered Evaporation Basin** — deep salt layers exposed, geological lore.
- 6375 **Swift Line Waypost** — ruined rest shelter, competed over by factions.
- 6378 **Mineral Seep** — rare moisture point, salt-encrusted spring.
- 6380 **Surveyor's Cairn Remnant** — old Keeper marker with Littoral Branch glyphs.
- 6385 **Salt Crust Collapse** — hazard room, thin crust over void.
- 6390 **Eastern Salt Shelf** — Flats thin out, first glass shards appear underfoot.
- 6395 **Swift Line Terminal** — last northern cairn before the Glasswind Belt.
- 6399 **Descent to the Glasswind** — transition to District D.

Remaining rooms: connecting salt-flat passages, wind-scoured channels, abandoned sled tracks, mineral deposit clusters, exposed survey benchmarks, and environmental traversal rooms.

---

### District C: Mirror Flats South — Heavy Road (6400-6449, 50 rooms)

**Theme:** The southern crossing of the Mirror Flats, following the old Heavy Road — the slow route for bulk commodities. Deeper salt deposits, wider passages carved by generations of laden cargo sleds. Every third cairn had a full assessment station. This route is safer but slower, with more infrastructure ruins and more faction competition.

**Internal Shape:** A broader east-west corridor with assessment station complexes branching off the main road at regular intervals. A southern spur leads to the abandoned Sealed Route junction. The eastern end connects to the Cairn Line Ruins district. A central fortified waystation serves as a mid-crossing anchor.

Key rooms:
- 6400 **Heavy Road Gate** — entry cairn, toll schedule still partially legible.
- 6403 **First Assessment Station** — ruined inspection hall, bronze scale remnants.
- 6406 **Cargo Sled Ruts** — deep-worn passage, generations of traffic visible.
- 6410 **Fortified Waystation** — mid-crossing rest point, 5+ sentences, hub room.
- 6412 **Waystation Cistern** — dry cistern with mineral stratification lore.
- 6415 **Second Assessment Station** — larger ruin, Shoreward Table fragments on walls.
- 6418 **Sealed Route Junction** — southern spur departure, quarantine markers.
- 6420 **Convoy Graveyard** — abandoned cargo, broken sled frames, scavenger activity.
- 6425 **Third Assessment Station** — best-preserved station, extra descriptions for conversion tables.
- 6428 **Heavy Road Overwatch** — elevated position, cairn kite observation.
- 6430 **Salt-Worn Foundation Row** — exposed building foundations from Ledger-Tariff era.
- 6435 **Brine Pool** — rare standing water, mineral-stained, fauna gathering point.
- 6440 **Eastern Heavy Road** — road narrows as salt gives way to glass-mixed terrain.
- 6445 **Heavy Road Terminal Cairn** — last Heavy Road marker, transition text.
- 6449 **Cairn Line Approach** — connects east to District E.

Remaining rooms: toll booths, guard posts, caravan shelter alcoves, salt-mining remnants, assessment clerk quarters, and transit corridor sections.

---

### District D: Glasswind Belts (6450-6499, 50 rooms)

**Theme:** Fields of wind-cut silica shards that abrade skin, cloth, and pack harness. The glass formed during the Ninth Meridian catastrophe's thermal pulse. Glass color indicates danger: milky (safe, pre-catastrophe), amber (thermal fault proximity), violet (eclipse-rite residue). Glasswalk Guides are essential for safe passage. Synod Whisper Cells use the Belts as concealment corridors. Glassworms burrow beneath the crust, creating collapse hazards.

**Internal Shape:** A tangled web of narrow glass-walled passages, hidden Glasswalk paths, Synod smuggler tunnels, and open glass fields. The district branches extensively — multiple paths lead east, but only certain routes are safe. A central area contains a Glasswalk Guide waystation (neutral ground). The southern portion connects to the Sealed Route remnants. Dead-end glass canyons and worm-collapse chambers provide exploration danger.

Key rooms:
- 6450 **Glasswind Threshold** — first glass underfoot, color-reading tutorial in description.
- 6453 **Milky Glass Corridor** — safe older glass, firm footing, Guide waymarker.
- 6455 **Amber Glass Warning** — thermal fault proximity, temperature shift described.
- 6458 **Glasswalk Guide Waystation** — neutral hub, 5+ sentences, guide services.
- 6460 **Shardstorm Narrows** — wind-cut passage, abrasion hazard flavor.
- 6463 **Violet Glass Field** — eclipse-rite residue zone, Synod theological significance.
- 6465 **Glassworm Sinkhole** — collapse hazard, hollow-ringing glass crust.
- 6468 **Synod Whisper Cache** — hidden smuggler stash, cult graffiti in Eclipsed Tongues script.
- 6470 **Thermal Fault Ridge** — elevated glass ridge, newly fused glass visible.
- 6473 **Freshly Vitrified Sand** — recent thermal event evidence, plot hook room.
- 6475 **Glass Canyon Dead End** — exploration trap, worm ambush.
- 6478 **Wind-Cut Gallery** — natural glass sculpture, environmental beauty amid hazard.
- 6480 **Glasswalk Secret Path** — hidden safe route known only to Guides.
- 6483 **Eclipsed Tongues Shrine** — Synod worship site, theological inscriptions.
- 6485 **Glass Scorpion Nest** — dense predator zone, glass-carapace fauna.
- 6488 **Subglass Worm Warren** — underground passage beneath glass crust.
- 6490 **Eastern Glass Shelf** — Belts thin, cairn ruins visible ahead.
- 6493 **Synod Courier Route** — unmarked whisper route bypassing cairn inspection.
- 6495 **Guide's Song Marker** — route song inscription, oral tradition waypoint.
- 6499 **Glasswind Eastern Exit** — transition to cairn district or Tidemouth approach.

Remaining rooms: glass-walled passages, wind-sheltered alcoves, partial glass caves, scorpion ambush corridors, worm tunnel intersections, shattered glass slopes, and Guide-marked safe passages.

---

### District E: Cairn Line Ruins — Measure's Rest (6500-6549, 50 rooms)

**Theme:** The ruined checkpoint chains where the three route systems converged into a major cairn station complex. This district contains **Measure's Rest** — the fortified Reach Warden library housing the most complete surviving record of the Reach's legal infrastructure. Cairn Scavengers strip bronze fittings and tablet fragments. Red Sand Outriders have purchased several stations outright. The district has significant vertical development — cairn station interiors go up into watchtowers and down into sealed tablet vaults.

**Internal Shape:** A cluster of interconnected cairn stations arranged in a rough crescent around Measure's Rest. Each station has surface-level ruins (patrol rooms, toll gates) and underground chambers (tablet vaults, assessment archives). Measure's Rest itself is a multi-level complex with a ground floor library, upper observation deck, and lower sealed archive. The layout resembles a fortified compound, not a road segment.

Key rooms:
- 6500 **Cairn Line Approach** — first visible cairn station, ruined but recognizable.
- 6502 **Station One: Bronze Gate** — stripped cairn station, scavenger activity.
- 6505 **Station Two: Toll Hall** — better preserved, competing toll signs from rival claimants.
- 6508 **Station Three: Assessment Court** — Shoreward Assessor hall with wind baffles.
- 6510 **Cairn Station Crossroads** — hub where three route lines converge, 5+ sentences.
- 6512 **Red Sand Purchased Station** — Outrider-controlled toll point, commercial predation.
- 6515 **Scavenger Market** — informal trade in stripped cairn materials.
- 6518 **Landward Tidewright Cell** — archaic scribes preserving full Measure Script.
- 6520 **Measure's Rest: Outer Court** — Warden fortress approach, 5+ sentences.
- 6522 **Measure's Rest: Library Hall** — cairn tablet transcription archive, lore-dense.
- 6524 **Measure's Rest: Upper Observation** — watchtower with views across the Reach.
- 6526 **Measure's Rest: Sealed Archive** — locked lower vault, key required, valuable tablets.
- 6528 **Measure's Rest: Warden Commander's Quarters** — faction leadership space.
- 6530 **Tablet Restoration Workshop** — scholarly repair and translation work.
- 6532 **Kowloon Scholar's Camp** — Jade Clerk-funded expedition camp, cross-area flavor.
- 6535 **Forgery Detection Room** — where fabricated tablets are identified.
- 6538 **Old Swift Line Junction** — ruined northern cairn, fast-route remnant.
- 6540 **Old Heavy Road Junction** — ruined central cairn, bulk-route remnant.
- 6542 **Old Sealed Route Junction** — abandoned southern cairn, quarantine markers.
- 6545 **Cairn Capstone Observatory** — shadow-rod mounting holes, astronomical survey.
- 6548 **Eastern Cairn Line** — last cairns before Tidemouth.
- 6549 **Tidemouth Descent** — transition to District G.

Remaining rooms: individual cairn interiors, scavenger tunnels, patrol corridors, tablet niche chambers, assessment clerk quarters, fortification walls, and connecting passages between stations.

---

### District F: Sealed Route Remnants (6550-6599, 50 rooms)

**Theme:** The abandoned southern cairn chain dedicated to funerary and relic consignments. When the funerary institutions fragmented during the Fracture Era, the Sealed Route was abandoned. Now the quarantine stations are infiltrated by Synod Whisper Cells, and the sealed containers left behind attract Cairn Scavengers and dangerous relic hazards. This is the most dangerous and least-traveled district.

**Internal Shape:** A linear southern spur branching off the Heavy Road, then curving east through quarantine stations, sealed relic vaults, and Synod-occupied passages. Dead ends and locked doors are common — this was designed for controlled passage, not free movement. The southern edge borders impassable terrain. A hidden Synod tunnel connects back to the Glasswind Belts.

Key rooms:
- 6550 **Sealed Route Marker One** — quarantine warning inscription, 5+ sentences.
- 6552 **First Quarantine Station** — inspection hall for relic consignments.
- 6555 **Sealed Container Yard** — abandoned cargo, some containers still sealed.
- 6558 **Quarantine Scribe Office** — Hazard Reclassification workspace.
- 6560 **Sealed Route Fortification** — armed escort staging area, arrow loops in walls.
- 6563 **Relic Examination Chamber** — provenance verification room, extra descriptions.
- 6565 **Witness Certification Hall** — where every cairn required witness certification.
- 6568 **Synod-Infiltrated Station** — Whisper Cell occupation, cult activity.
- 6570 **Eclipsed Tongues Ritual Room** — active Synod worship, theological inscriptions.
- 6573 **Sealed Relic Vault** — locked door, dangerous contents, key required.
- 6575 **Quarantine Breach** — broken containment, hazard released.
- 6578 **Funerary Consignment Depot** — Southern Pyramid provenance marks.
- 6580 **Synod Smuggler Tunnel** — hidden passage back to Glasswind Belts.
- 6583 **Abandoned Escort Barracks** — Fracture-Era military quarters.
- 6585 **Relic Cold-Storage Chamber** — unnaturally cold room, sealed crate that hums at dawn.
- 6588 **Route Terminus Ruin** — the point where the Sealed Route simply stopped.
- 6590 **Collapsed Quarantine Arch** — structural failure, rubble exploration.
- 6593 **Synod Command Post** — Whisper Cell leadership, intelligence cache.
- 6595 **Defaced Marker Row** — competing toll signs defacing each other.
- 6599 **Sealed Route Eastern Remnant** — connects to Tidemouth approach.

Remaining rooms: quarantine corridors, sealed niches, guard posts, escort route sections, abandoned relic inspection chambers, Synod hideouts, and structural collapse zones.

---

### District G: Tidemouth Approach (6600-6649, 50 rooms)

**Theme:** Low dunes and brackish channels marking approach to the coast. The desert's dry ecology gives way to salt marsh, tidal pools, and mineral-stained sand. Tidemouth crabs dig burrows that collapse under weight. The smell of sea-salt mixes with desert mineral dust. Maritime influence increases — anchor stones, hull-caulking resin dumps, sailcloth windbreaks. This is where the Reach's administrative logic historically terminated and Mafdet's harbor jurisdiction began.

**Internal Shape:** A broad, open area that narrows eastward toward the Shoreward Threshold. Multiple paths wind through dune ridges, tidal channels, and crab-burrowed flats. A northern branch follows the coast toward future Mafdet approach. A southern branch explores tide-pool caves. The central path leads to the Shoreward Seal district. The openness contrasts with the confined cairn stations — players can see the sea from elevated dunes.

Key rooms:
- 6600 **First Crab Burrow** — landmark: half a day from Mafdet, 5+ sentences.
- 6603 **Brackish Channel Crossing** — tidal water, desert-to-coast transition.
- 6605 **Dune Crest Overlook** — first sight of the sea on the eastern horizon.
- 6608 **Anchor Stone Field** — maritime debris begins, resin dumps, sailcloth scraps.
- 6610 **Tidemouth Flats** — open salt marsh, crab burrow hazards, 5+ sentences.
- 6613 **Tidal Pool Grotto** — coastal cave, amphibious fauna.
- 6615 **Sand-Sea Carters' Camp** — Guild caravan-to-ship transfer operation.
- 6618 **Crab Warren** — dense burrow network, collapse risk.
- 6620 **Mineral-Stained Dune** — vivid color from coastal mineral deposits.
- 6623 **Tide Obelisk Stump** — Keeper-era tidal measurement station, basalt remnant.
- 6625 **Saltmarsh Trail** — winding path through coastal vegetation.
- 6628 **Resin Dump Clearing** — hull-caulking waste, maritime industry evidence.
- 6630 **Wind-Torn Sailcloth Shelter** — improvised camp using ship materials.
- 6633 **Brackish Well** — last freshwater source, mineral-tainted but drinkable.
- 6635 **Tidemouth Warden Post** — contested jurisdiction checkpoint.
- 6638 **Harbor Warden Forward Camp** — Mafdet's jurisdiction extending inland.
- 6640 **Dual Jurisdiction Zone** — where Reach Wardens and Harbor Wardens overlap.
- 6643 **Coastal Fog Bank** — reduced visibility, smuggling cover during Fog Season.
- 6645 **Mafdet Strand Approach** — eastern exit toward future Mafdet, 5+ sentences.
- 6649 **Tidemouth Eastern Edge** — transition to District H.

Remaining rooms: tidal channels, crab-burrowed passages, dune ridges, coastal caves, maritime debris fields, fishing camps, and wind-scoured transition terrain.

---

### District H: Shoreward Threshold (6650-6699, 50 rooms)

**Theme:** The legal boundary zone where inland law formally yields to maritime jurisdiction. The Shoreward Seal — a large basalt slab carved with both inland charter notation and maritime loading symbols — marks the physical threshold. This district contains the ruins of the great Shoreward Assessment Halls, the Water-Debt Discharge basin, and the Dual Attestation Court. It is also the site of the area's major boss encounters, where the unresolved legal contradictions of the Reach manifest as hostile forces.

**Internal Shape:** A processional approach leading to the Shoreward Seal plaza, flanked by ruined Assessment Halls. Side chambers contain the specialized conversion facilities (Water-Debt Discharge basin, Risk Assumption Oath court, Relic Quarantine Transfer hall). Below the plaza, a sealed undercroft contains the most ancient jurisdictional machinery. The final rooms are boss encounter spaces where the Reach's contested authority takes physical form.

Key rooms:
- 6650 **Shoreward Approach** — processional road, assessment hall ruins visible ahead.
- 6652 **Assessment Hall of the Swift Line** — northern route's terminal conversion facility.
- 6655 **Assessment Hall of the Heavy Road** — central route's conversion facility, 5+ sentences.
- 6658 **Assessment Hall of the Sealed Route** — southern route's quarantine conversion.
- 6660 **Shoreward Assessors' Court** — specialist clerk training school ruins.
- 6662 **Shoreward Table Hall** — conversion ratio charts carved into walls, extra descriptions.
- 6665 **Water-Debt Discharge Basin** — basalt basin where water-debt was symbolically terminated.
- 6668 **Risk Assumption Oath Court** — where liability transferred from road to sea.
- 6670 **Relic Quarantine Transfer Hall** — hazard reclassification chamber.
- 6672 **Dual Attestation Chamber** — where both inland and maritime seals were applied.
- 6675 **The Shoreward Seal** — the great basalt slab itself, 5+ sentences, area landmark.
- 6678 **Dry Seal Archive** — inland witness mark storage.
- 6680 **Wet Seal Foundry** — maritime wax/bitumen seal preparation.
- 6682 **Jurisdictional Threshold** — the exact legal boundary, dual authority text.
- 6685 **Undercroft Descent** — stairs down into sealed sub-level.
- 6687 **Undercroft: Ancient Conversion Engine** — pre-Keeper jurisdictional machinery.
- 6690 **Undercroft: Fractured Authority Chamber** — where competing laws collide, boss prelude.
- 6692 **Lair of the Shoreward Revenant** — mini-boss room, no_mob flag.
- 6694 **Toll-Marshal of the Three Routes** — major boss room, no_mob flag, 5+ sentences.
- 6696 **Sanctum of Contested Jurisdiction** — second major boss, no_mob flag.
- 6698 **Heart of the Conversion** — final boss encounter, no_mob flag, 5+ sentences.
- 6699 **The Eastern Shore** — exit toward Mafdet, area terminus, 5+ sentences.

Remaining rooms: conversion clerk offices, seal storage rooms, undercroft passages, collapsed assessment wings, connecting corridors, and atmospheric transition rooms.

---

## New Mob Specials: Saltglass Reach Ambient Specials

The Saltglass Reach requires **8 new area-specific ambient specials** to bring its factions and institutions to life. These follow the established ambient-only pattern (8-way gate via `number_bits(3)`, action or speech, always return `FALSE`). **Do not assign to mobs outside the Saltglass Reach.**

| Special | Intended mob roles | Flavor / setting | Sample lines |
|---|---|---|---|
| `spec_reach_warden` | Reach Warden patrol sentinels, toll gate guards, barracks officers | Desert patrol authority: scans the horizon, checks transit seal validity, marks patrol ledger, adjusts sun-hood, taps cairn marker with staff for soundness. References the Keeper survey inheritance, cairn tablet library at Measure's Rest, and the jurisdiction dispute with Mafdet's Harbor Wardens. | *"Transit seal current? Good. The Reach still has law, whatever the Harbor Wardens claim."* / *"Every cairn between here and the Seal carries our mark. That is not tradition — it is jurisdiction."* / *"Measure's Rest holds tablets older than Mafdet's harbor chain. We were here first. We remain."* |
| `spec_reach_guide` | Glasswalk Guide NPCs, guide camp attendants, route song keepers | Hereditary desert navigation: reads glass color underfoot, tests crust by tapping with chitin-soled staff, hums route song fragments, ties fresh cord markers, checks wind direction by tossing sand. References the neutrality tradition, glass-reading skill, and pre-Measure oral navigation. | *"Milky glass, firm step. Amber glass, watch your feet. Violet glass, turn back. The colors don't lie."* / *"We guided before the Keepers carved their first obelisk. The songs remember routes the stones forgot."* / *"I guide anyone who pays. Warden, Outrider, Synod — the glass does not care about your politics."* |
| `spec_reach_assessor` | Shoreward Assessor revenants, toll clerks, conversion table scribes, Landward Tidewrights | Jurisdictional assessment: weighs imaginary cargo, consults conversion tables, stamps invisible seals, mutters legal formulas, dips quill in dry inkwell. References the Shoreward Tables, the Water-Debt Discharge, dual attestation, and the three-route toll schedules. | *"Water-debt: forty-seven measures, offset against Great Oasis charter series nine. Shore conversion: pending at the Seal."* / *"The Tables do not lie. What the inland weighs in water, the sea weighs in risk. The conversion is exact."* / *"Dual attestation requires both marks — inland witness and maritime seal. One without the other is void."* |
| `spec_reach_outrider` | Red Sand debt collectors, Red Sand assessors, Red Sand enforcers | Commercial predation: examines cargo manifests with predatory interest, calculates interest on wax tablet, adjusts brass scales, whispers debt terms, marks distressed caravan identifiers. References Mafdet's Red Sand Accounts, exploitative lending, and the legal trap of shore conversion rates. | *"Distressed cargo at shore conversion rates? That interest compounds before you reach the dock."* / *"The Red Sand Accounts extend credit to anyone. Repayment, however, is not optional."* / *"Your manifest shows water-debt outstanding. We can consolidate that. The terms are... reasonable."* |
| `spec_reach_scavenger` | Cairn Scavenger crew members, relic hunters, scavenger lookouts | Ruin stripping: chips mortar from cairn joints, pries bronze fittings with a chisel, examines tablet fragments against lamplight, wraps salvage in oiled cloth, glances nervously for Warden patrols. References the salvage law gray zone, Mafdet buyer prices, and the tension between history and profit. | *"Bronze fittings fetch six measures at the Strand. A legible tablet? Ten times that. History pays."* / *"Salvage law is clear — abandoned structures are fair claim. The Wardens just disagree about what 'abandoned' means."* / *"Careful with that seal-stone. Cracked, it's gravel. Intact, a Kowloon scholar will pay a season's wages."* |
| `spec_reach_tidewright` | Landward Tidewright scholars, Measure Script preservationists | Archaic script preservation: traces glyphs on practice tablets, compares Littoral Branch notation against inland Measure Script, mutters tidal calculations, dips shadow-rod into sand to check angle, copies faded inscriptions. References the Tidewright Schism, the Seaward branch's departure to Mafdet, and Kowloon's Jade Clerk interest. | *"The Seaward Tidewrights simplified until the Script lost its soul. We preserve the full form. That is our vow."* / *"Tidal glyph: third-quarter rise, strong northerly, draft clearance marginal. The notation is precise to the hour."* / *"The Jade Clerks from Kowloon write with questions. We answer in Script. If they cannot read it, we are doing our work."* |
| `spec_reach_carter` | Sand-Sea Carters' Guild members, cargo transfer workers, carter camp cooks | Caravan-to-ship logistics: checks sled runner condition, retarps cargo against salt spray, sorts manifests into inland and maritime stacks, tests rope tension, signals to approaching wagons. References the Guild's transit monopoly, the Strandline Compact, and the seasonal cargo schedule. | *"Sled to dock, dock to hold. That is the Carter's road. Everything between the oasis and the ship passes through our hands."* / *"The Guild keeps the cargo moving. Without us, the manifests pile up and the ships wait empty."* / *"Season's almost over. Three more convoys, then Scour winds shut the Flats for a month."* |
| `spec_reach_smuggler` | Synod courier ambient (out-of-combat only), Whisper Cell lookouts | Clandestine operations: peers through glass-shard screen, wraps relic component in liturgical cloth, traces Eclipsed Tongues glyph in sand, checks dead-drop cache, listens for patrol footsteps. References the Ninth Meridian as divine judgment, the sacredness of fused glass, and the smuggling route network. Fires out of combat only; combat initiated by `aggressive` flag. | *"The fire that made this glass was not catastrophe. It was purification. The Ninth Meridian proved it."* / *"The whisper routes bypass the cairns. What the Wardens cannot inspect, the Wardens cannot confiscate."* / *"The Tongues speak of a second firing. When the glass glows again, the judgment resumes."* |

### Ambient Special Implementation Notes

- All 8 specials follow the standard ambient template: `!IS_AWAKE(ch) || is_fighting(ch)` guard, then `number_bits(3) != 0` gate (~12.5% per pulse), then action-or-say split. Always return `FALSE`.
- Each special needs 6 action lines and 6 speech lines for variety.
- Source files: `src/ai/spec_reach_*.c` (one file per special).
- Register in `spec_lookup()`, `rev_spec_lookup()`, and `print_spec_lookup()` in `src/special.c`.
- Add `ai/spec_reach_*.o` entries to the `AI_SPEC_OBJS` block in `src/Makefile`.
- Document in `docs/mob_specials_spec.md` under a new "Saltglass Reach ambient specials" section.

---

## Encounter and Faction Design

### Primary Factions

1. **Reach Wardens** (neutral-to-friendly patrol authority):
   - Self-appointed successors to Keeper survey authority. Patrol cairn lines, issue transit seals. Strongest in Mirror Flats, weakest in Tidemouth. Loose confederation of patrol bands.
   - **Hostile Wardens** appear as corrupted toll-enforcers in contested zones where authority has degraded into extortion.

2. **Red Sand Outriders** (hostile commercial predators):
   - Field agents of Mafdet's Red Sand Accounts. Use legal instruments as weapons. Identify distressed caravans, offer exploitative loans, collect at Mafdet's docks.
   - Operate throughout the Reach but concentrate at purchased cairn stations.

3. **Glasswalk Guides** (neutral):
   - Hereditary desert guides who know safe routes through the Glasswind Belts. Strictly neutral in Reach politics. Present as non-combat NPCs in guide camps and waypoints.
   - Route song fragments in room extra descriptions.

4. **Synod Whisper Cells** (hostile cult operatives):
   - Eclipsed Tongues faction. Smuggle relic components and cult communications. Use Glasswind Belts as concealment corridors. View fused glass as divine judgment evidence.
   - Concentrated in Sealed Route and Glasswind Belt districts.

5. **Cairn Scavengers** (hostile relic hunters):
   - Strip surviving cairn stations for saleable materials. Operate in legal gray zone between inland and maritime salvage law.
   - Found throughout cairn districts, often in competition with Red Sand Outriders.

6. **Tidemouth Hazard Fauna** (hostile wildlife):
   - Salt crickets (Mirror Flats swarms), glass scorpions (Glasswind Belts), tidemouth crabs (Tidemouth Dunes), cairn kites (throughout), glassworms (Glasswind subsurface).

7. **Jurisdictional Manifestations** (hostile — Shoreward Threshold only):
   - Revenants of Shoreward Assessors, animated toll-marshals, and conversion-engine constructs. The unresolved legal contradictions of the Reach given hostile physical form.

---

## Full Mob Roster (6300-6399, 100 mob templates)

Mob vnums are assigned sequentially. Each entry lists vnum, name, level, act flags, key combat traits, district placement, and special function assignment.

### District A Mobs: Oasis Staging Grounds (Friendly/Neutral NPCs + Light Hostiles)

| Vnum | Name | Lvl | Act Flags | Role | Special |
|---:|---|---:|---|---|---|
| 6300 | reach warden gate sentinel | 58 | `is_npc sentinel stay_area` | Toll gate guard, non-aggressive | `spec_reach_warden` |
| 6301 | reach warden patrol leader | 60 | `is_npc sentinel stay_area` | Barracks officer, quest offerer | `spec_reach_warden` |
| 6302 | reach warden cartographer | 58 | `is_npc sentinel stay_area` | Transit seal office, quest offerer | `spec_reach_warden` |
| 6303 | glasswalk guide recruiter | 56 | `is_npc sentinel stay_area` | Guide camp hiring post | `spec_reach_guide` |
| 6304 | red sand counting clerk | 60 | `is_npc sentinel stay_area` | Red Sand counting house, non-aggressive NPC | `spec_reach_outrider` |
| 6305 | caravan provisioner | 55 | `is_npc sentinel stay_area` | Provisioner's Row shopkeeper | `spec_reach_carter` |
| 6306 | sand-sea carter | 56 | `is_npc stay_area` | Caravan staging yard worker | `spec_reach_carter` |
| 6307 | reach transit clerk | 57 | `is_npc sentinel stay_area` | Transit seal office bureaucrat | `spec_reach_assessor` |
| 6308 | drywell attendant | 55 | `is_npc sentinel stay_area` | Well keeper, ambient | `spec_reach_carter` |
| 6309 | cairn tablet reader | 58 | `is_npc sentinel stay_area` | Scholarly annex, lore NPC | `spec_reach_tidewright` |
| 6310 | caravan drover | 55 | `is_npc stay_area` | Animal pen worker | — |
| 6311 | staging yard rat | 55 | `is_npc aggressive stay_area` | Light trash mob | `spec_fido` |

### District B Mobs: Mirror Flats North — Swift Line (Fauna + Warden Patrols)

| Vnum | Name | Lvl | Act Flags | Role | Special |
|---:|---|---:|---|---|---|
| 6312 | salt cricket swarm | 56 | `is_npc aggressive stay_area no_body` | Swarm mob, light damage, disorienting | — |
| 6313 | salt cricket swarm leader | 58 | `is_npc aggressive stay_area no_body` | Quest target, slightly harder swarm variant | — |
| 6314 | cairn kite | 57 | `is_npc aggressive stay_area` | Raptor predator, patrols salt flats | — |
| 6315 | cairn kite alpha | 60 | `is_npc aggressive stay_area solo` | Quest target, stronger raptor | — |
| 6316 | salt crust lurker | 58 | `is_npc aggressive stay_area` | Ambush predator hiding in salt deposits | `spec_poison` |
| 6317 | brine serpent | 59 | `is_npc aggressive stay_area` | Mineral seep predator | `spec_poison` |
| 6318 | corrupted warden toll-thief | 60 | `is_npc aggressive stay_area` | Degraded Warden extorting caravans, quest target | — |
| 6319 | swift line warden patrol | 58 | `is_npc stay_area` | Friendly patrol, non-aggressive | `spec_reach_warden` |
| 6320 | salt-blinded drifter | 56 | `is_npc aggressive stay_area` | Lost traveler gone mad from reflection | — |
| 6321 | mineral crust elemental | 62 | `is_npc aggressive stay_area no_body` | Salt-animated elemental, earth strong | — |

### District C Mobs: Mirror Flats South — Heavy Road (Faction + Fauna Mix)

| Vnum | Name | Lvl | Act Flags | Role | Special |
|---:|---|---:|---|---|---|
| 6322 | heavy road warden sentinel | 60 | `is_npc sentinel stay_area` | Assessment station guard | `spec_reach_warden` |
| 6323 | waystation cistern keeper | 58 | `is_npc sentinel stay_area` | Cistern lore NPC | `spec_reach_assessor` |
| 6324 | red sand outrider scout | 62 | `is_npc aggressive stay_area` | Outrider advance party | `spec_reach_outrider` |
| 6325 | convoy graveyard scavenger | 60 | `is_npc aggressive stay_area` | Scavenger picking through wrecked convoys | `spec_reach_scavenger` |
| 6326 | salt flat viper | 58 | `is_npc aggressive stay_area` | Venomous snake, salt-adapted | `spec_poison` |
| 6327 | brine pool predator | 60 | `is_npc aggressive stay_area` | Ambush hunter at water sources | — |
| 6328 | assessment station ghost | 62 | `is_npc aggressive stay_area undead no_body` | Spectral toll clerk | `spec_cast_undead` |
| 6329 | heavy road caravan guard | 58 | `is_npc stay_area` | Non-aggressive caravan escort NPC | — |
| 6330 | salt slab golem | 64 | `is_npc aggressive stay_area solo` | Construct defender of old assessment station | — |

### District D Mobs: Glasswind Belts (Glass Fauna + Synod + Guides)

| Vnum | Name | Lvl | Act Flags | Role | Special |
|---:|---|---:|---|---|---|
| 6331 | glass scorpion | 62 | `is_npc aggressive stay_area` | Glass-carapace predator, venomous | `spec_poison` |
| 6332 | glass scorpion matriarch | 65 | `is_npc aggressive stay_area solo` | Larger scorpion, quest target | `spec_poison` |
| 6333 | glassworm burrower | 63 | `is_npc aggressive stay_area` | Subsurface ambush worm | — |
| 6334 | glassworm tunnel sentinel | 66 | `is_npc aggressive stay_area solo` | Larger worm, guards tunnel network, quest target | `spec_breath_acid` |
| 6335 | glasswalk guide pathfinder | 60 | `is_npc sentinel stay_area` | Neutral guide at waystation | `spec_reach_guide` |
| 6336 | glasswalk guide elder | 63 | `is_npc sentinel stay_area` | Senior guide, quest offerer | `spec_reach_guide` |
| 6337 | synod whisper courier | 62 | `is_npc aggressive stay_area` | Smuggler carrying relic components | `spec_reach_smuggler` |
| 6338 | synod lookout | 60 | `is_npc aggressive stay_area` | Whisper Cell sentry, quest target | `spec_reach_smuggler` |
| 6339 | synod ritual keeper | 64 | `is_npc aggressive stay_area` | Cult caster, quest target | `spec_cast_cleric` |
| 6340 | thermal fault elemental | 66 | `is_npc aggressive stay_area no_body` | Fire/earth elemental at fault ridge, quest target | `spec_breath_fire` |
| 6341 | vitrified sand horror | 65 | `is_npc aggressive stay_area no_body` | Glass-animated construct, quest target | — |
| 6342 | molten glass sentinel | 67 | `is_npc aggressive stay_area solo no_body` | Fire-element guardian of thermal anomaly, quest target | `spec_breath_fire` |
| 6343 | shardstorm wind spirit | 63 | `is_npc aggressive stay_area no_body` | Air elemental carrying glass debris | — |
| 6344 | amber glass crawler | 61 | `is_npc aggressive stay_area` | Insect adapted to amber-tinted glass zones | — |
| 6345 | glassworm burrower queen | 70 | `is_npc aggressive sentinel stay_area boss no_flee` | **MINI-BOSS**: apex fauna predator of the Belts | `spec_breath_acid` |

### District E Mobs: Cairn Line Ruins — Measure's Rest (Multi-Faction + Scholars)

| Vnum | Name | Lvl | Act Flags | Role | Special |
|---:|---|---:|---|---|---|
| 6346 | cairn scavenger | 62 | `is_npc aggressive stay_area` | Generic relic hunter | `spec_reach_scavenger` |
| 6347 | cairn scavenger digger | 60 | `is_npc aggressive stay_area` | Laborer stripping bronze fittings | `spec_reach_scavenger` |
| 6348 | cairn scavenger lookout | 61 | `is_npc aggressive stay_area` | Sentry watching for Warden patrols | `spec_reach_scavenger` |
| 6349 | cairn scavenger warlord | 70 | `is_npc aggressive sentinel stay_area boss no_flee` | **MINI-BOSS**: relic hunter faction leader | — |
| 6350 | red sand debt collector | 64 | `is_npc aggressive stay_area` | Outrider field agent, quest target | `spec_reach_outrider` |
| 6351 | red sand assessor | 66 | `is_npc aggressive stay_area` | Outrider cargo evaluator, quest target | `spec_reach_outrider` |
| 6352 | red sand enforcer | 68 | `is_npc aggressive stay_area solo` | Outrider muscle, quest target | — |
| 6353 | measure's rest warden guard | 64 | `is_npc sentinel stay_area` | Fortress guard, non-aggressive | `spec_reach_warden` |
| 6354 | warden commander | 68 | `is_npc sentinel stay_area` | Faction leader, quest offerer | `spec_reach_warden` |
| 6355 | landward tidewright | 62 | `is_npc sentinel stay_area` | Archaic scribe preserving Measure Script | `spec_reach_tidewright` |
| 6356 | kowloon scholar | 60 | `is_npc sentinel stay_area` | Jade Clerk-funded researcher, quest offerer | `spec_reach_tidewright` |
| 6357 | tablet restoration apprentice | 58 | `is_npc sentinel stay_area` | Scholar's assistant | `spec_reach_tidewright` |
| 6358 | cairn station construct | 66 | `is_npc aggressive sentinel stay_area` | Ancient stone guardian of cairn ruins | — |
| 6359 | toll cairn specter | 64 | `is_npc aggressive stay_area undead no_body` | Ghost of a toll clerk, haunts ruined stations | `spec_cast_undead` |
| 6360 | forgery detection scribe | 60 | `is_npc sentinel stay_area` | Identifies fake tablets, ambient NPC | `spec_reach_assessor` |

### District F Mobs: Sealed Route Remnants (Synod + Quarantine Horrors)

| Vnum | Name | Lvl | Act Flags | Role | Special |
|---:|---|---:|---|---|---|
| 6361 | synod tunnel sentry | 65 | `is_npc aggressive stay_area` | Smuggler tunnel guard, quest target | `spec_reach_smuggler` |
| 6362 | synod relic handler | 66 | `is_npc aggressive stay_area` | Cult specialist moving contraband | `spec_reach_smuggler` |
| 6363 | synod eclipsed chanter | 68 | `is_npc aggressive stay_area` | Shadow/fire caster, ritual room priest | `spec_cast_bigtime` |
| 6364 | whisper cell commandant | 72 | `is_npc aggressive sentinel stay_area boss no_flee` | **MINI-BOSS**: Synod operative leader, quest target | `spec_cast_bigtime` |
| 6365 | quarantine breach horror | 68 | `is_npc aggressive stay_area no_body` | Relic hazard escaped containment, quest target | `spec_breath_gas` |
| 6366 | containment ward specter | 66 | `is_npc aggressive stay_area undead no_body` | Undead quarantine scribe | `spec_cast_undead` |
| 6367 | sealed crate aberration | 67 | `is_npc aggressive stay_area no_body` | Animated relic container, cold/shadow | — |
| 6368 | relic provenance ghost | 65 | `is_npc aggressive stay_area undead no_body` | Southern Pyramid provenance spirit | `spec_cast_undead` |
| 6369 | funerary consignment golem | 69 | `is_npc aggressive stay_area solo` | Construct guarding sealed cargo | — |
| 6370 | quarantine escort revenant | 67 | `is_npc aggressive stay_area undead` | Undead armed escort from the Fracture Era | — |
| 6371 | sealed route marker spirit | 64 | `is_npc aggressive stay_area undead no_body` | Ghost bound to abandoned route markers | `spec_cast_undead` |

### District G Mobs: Tidemouth Approach (Coastal Fauna + Jurisdiction NPCs)

| Vnum | Name | Lvl | Act Flags | Role | Special |
|---:|---|---:|---|---|---|
| 6372 | tidemouth crab | 60 | `is_npc aggressive stay_area` | Amphibious crustacean, burrow hazard | — |
| 6373 | tidemouth crab broodmother | 65 | `is_npc aggressive stay_area solo` | Larger crab, quest target | — |
| 6374 | tidal pool lurker | 62 | `is_npc aggressive stay_area` | Ambush predator in coastal pools | `spec_poison` |
| 6375 | brackish channel eel | 60 | `is_npc aggressive stay_area` | Aquatic predator in tidal channels | — |
| 6376 | saltmarsh serpent | 63 | `is_npc aggressive stay_area` | Venomous coastal snake | `spec_poison` |
| 6377 | tidemouth raider | 64 | `is_npc aggressive stay_area` | Bandit preying on coastal transition zone, quest target | `spec_thief` |
| 6378 | rogue outrider | 65 | `is_npc aggressive stay_area` | Red Sand agent gone independent, quest target | — |
| 6379 | jurisdiction specter | 66 | `is_npc aggressive stay_area undead no_body` | Ghost of a boundary dispute, quest target | `spec_cast_undead` |
| 6380 | sand-sea carter teamster | 58 | `is_npc sentinel stay_area` | Guild cargo transfer worker, non-aggressive | `spec_reach_carter` |
| 6381 | carter camp cook | 56 | `is_npc sentinel stay_area` | Camp services NPC | `spec_reach_carter` |
| 6382 | tidemouth warden post commander | 64 | `is_npc sentinel stay_area` | Contested jurisdiction officer, quest offerer | `spec_reach_warden` |
| 6383 | harbor warden forward scout | 62 | `is_npc sentinel stay_area` | Mafdet jurisdiction, non-aggressive | `spec_guard` |
| 6384 | coastal fog phantom | 63 | `is_npc aggressive stay_area night_only undead no_body` | Fog-season apparition, night only | `spec_cast_undead` |
| 6385 | dune scarab swarm | 59 | `is_npc aggressive stay_area no_body` | Coastal insect swarm | — |

### District H Mobs: Shoreward Threshold (Revenants + Bosses)

| Vnum | Name | Lvl | Act Flags | Role | Special |
|---:|---|---:|---|---|---|
| 6386 | assessment hall guardian | 70 | `is_npc aggressive sentinel stay_area` | Stone construct defending conversion halls | — |
| 6387 | shoreward clerk revenant | 68 | `is_npc aggressive stay_area undead` | Undead assessor, conversion table caster | `spec_cast_cleric` |
| 6388 | toll-gate wraith | 70 | `is_npc aggressive stay_area undead no_body` | Incorporeal toll enforcer | `spec_cast_undead` |
| 6389 | water-debt phantom | 69 | `is_npc aggressive stay_area undead no_body` | Ghost bound to the Discharge Basin | `spec_cast_undead` |
| 6390 | risk assumption shade | 71 | `is_npc aggressive stay_area undead no_body` | Spirit of the Oath Court | `spec_cast_cleric` |
| 6391 | conversion engine construct | 72 | `is_npc aggressive stay_area solo` | Ancient jurisdictional machinery given life | — |
| 6392 | dual-seal sentinel | 73 | `is_npc aggressive sentinel stay_area solo` | Construct guarding the Dual Attestation Chamber | — |
| 6393 | undercroft watcher | 71 | `is_npc aggressive stay_area undead` | Revenant guarding the sealed sub-level | `spec_cast_undead` |
| 6394 | fractured authority elemental | 74 | `is_npc aggressive stay_area solo no_body` | Manifestation of competing laws, earth/water | `spec_breath_any` |
| 6395 | shoreward revenant | 75 | `is_npc aggressive sentinel stay_area boss no_flee undead` | **BOSS**: Dead Assessor, caster/melee hybrid | `spec_cast_bigtime` |
| 6396 | toll-marshal of the three routes | 78 | `is_npc aggressive sentinel stay_area boss no_flee` | **BOSS**: Animated enforcer of all three routes | `spec_cast_judge` |
| 6397 | arbiter of the conversion | 80 | `is_npc aggressive sentinel stay_area boss no_flee no_body` | **FINAL BOSS**: The Shoreward Conversion given hostile sentience | `spec_cast_bigtime` |
| 6398 | reach warden officer | 66 | `is_npc sentinel stay_area` | Warden at Sealed Route approach, quest offerer | `spec_reach_warden` |
| 6399 | shoreward seal guardian | 72 | `is_npc aggressive sentinel stay_area solo` | Elite construct at the Seal itself | — |

### Boss Cadence Summary

| Boss | Vnum | Room | Level | Type | Special |
|---|---:|---|---:|---|---|
| Glassworm Burrower Queen | 6345 | ~6475 (District D) | 70 | Mini-boss | `spec_breath_acid` |
| Cairn Scavenger Warlord | 6349 | ~6515 (District E) | 70 | Mini-boss | — |
| Whisper Cell Commandant | 6364 | ~6593 (District F) | 72 | Mini-boss | `spec_cast_bigtime` |
| Shoreward Revenant | 6395 | 6692 (District H) | 75 | Boss | `spec_cast_bigtime` |
| Toll-Marshal of the Three Routes | 6396 | 6694 (District H) | 78 | Boss | `spec_cast_judge` |
| Arbiter of the Conversion | 6397 | 6698 (District H) | 80 | Final Boss | `spec_cast_bigtime` |

### Specials Assignment Summary

| Special | Assigned To |
|---|---|
| `spec_reach_warden` | 6300, 6301, 6302, 6319, 6322, 6353, 6354, 6382, 6398 |
| `spec_reach_guide` | 6303, 6335, 6336 |
| `spec_reach_assessor` | 6307, 6323, 6360 |
| `spec_reach_outrider` | 6304, 6324, 6350, 6351 |
| `spec_reach_scavenger` | 6346, 6347, 6348 |
| `spec_reach_tidewright` | 6309, 6355, 6356, 6357 |
| `spec_reach_carter` | 6305, 6306, 6308, 6380, 6381 |
| `spec_reach_smuggler` | 6337, 6338, 6361, 6362 |
| `spec_poison` | 6316, 6317, 6326, 6331, 6332, 6374, 6376 |
| `spec_cast_undead` | 6328, 6359, 6366, 6368, 6371, 6379, 6384, 6388, 6389, 6393 |
| `spec_cast_cleric` | 6339, 6387, 6390 |
| `spec_cast_bigtime` | 6363, 6364, 6395, 6397 |
| `spec_cast_judge` | 6396 |
| `spec_breath_fire` | 6340, 6342 |
| `spec_breath_acid` | 6334, 6345 |
| `spec_breath_gas` | 6365 |
| `spec_breath_any` | 6394 |
| `spec_fido` | 6311 |
| `spec_thief` | 6377 |
| `spec_guard` | 6383 |

---

## Full Object Roster (6300-6424, 125 objects)

Object vnums are assigned sequentially. Each entry lists vnum, name, item type, wear flags, extra flags, weight range, and thematic notes.

### Keys (6300-6304)

| Vnum | Name | Type | Wear | Extra | Wt | Notes |
|---:|---|---|---|---|---:|---|
| 6300 | warden archive key | key | take | — | 1 | Unlocks Measure's Rest Sealed Archive (6526) |
| 6301 | quarantine seal key | key | take | — | 1 | Unlocks Sealed Relic Vault (6573) |
| 6302 | shoreward threshold key | key | take | — | 1 | Unlocks Undercroft Descent (6685) |
| 6303 | cairn station master key | key | take | — | 1 | Unlocks internal cairn station doors |
| 6304 | synod cipher key | key | take | — | 1 | Unlocks Synod smuggler tunnel hidden door |

### Weapons (6305-6324)

| Vnum | Name | Type | Wear | Extra | Wt | value3 | Notes |
|---:|---|---|---|---|---:|---:|---|
| 6305 | salt-crusted bronze khopesh | weapon | hold take | — | 8 | 3 (slash) | Mirror Flats melee drop, sickle-sword with brine patina |
| 6306 | glasswalk guide's chitin staff | weapon | hold take | two-handed | 4 | 7 (pound) | Guide weapon, chitin-and-resin construction |
| 6307 | cairn scavenger's pry bar | weapon | hold take | — | 9 | 8 (crush) | Heavy iron tool repurposed as weapon |
| 6308 | red sand assessor's stylus blade | weapon | hold take | — | 3 | 2 (stab) | Disguised weapon, sharp bronze writing implement |
| 6309 | synod eclipse dagger | weapon | hold take | — | 3 | 11 (pierce) | Ritual knife with Eclipsed Tongues glyphs |
| 6310 | warden patrol spear | weapon | hold take | two-handed | 7 | 11 (pierce) | Standard Reach Warden patrol weapon |
| 6311 | glass-shard flensing knife | weapon | hold take | — | 4 | 1 (slice) | Glassworm-tooth blade, caster weight |
| 6312 | toll-marshal's triple-headed flail | weapon | hold take | two-handed boss | 10 | 7 (pound) | Boss drop: Toll-Marshal. Three route-branded heads |
| 6313 | arbiter's gavel of conversion | weapon | hold take | boss | 5 | 8 (crush) | Boss drop: Arbiter. Basalt judicial hammer, inscribed with dual notation |
| 6314 | brine-forged scimitar | weapon | hold take | — | 8 | 3 (slash) | Tidemouth melee drop, salt-crystal edge |
| 6315 | shoreward revenant's quill-sword | weapon | hold take | boss | 6 | 2 (stab) | Boss drop: Shoreward Revenant. Elongated stylus-blade |
| 6316 | glassworm stinger lance | weapon | hold take | two-handed | 7 | 11 (pierce) | Crafted from glassworm tail barb |
| 6317 | cairn kite talon dagger | weapon | hold take | — | 4 | 5 (claw) | Raptor-talon blade, light caster weapon |
| 6318 | assessment scale mace | weapon | hold take | — | 9 | 7 (pound) | Bronze balance arm repurposed as bludgeon |
| 6319 | tidemouth crab-claw gauntlet | weapon | hold take | fist | 10 | 0 (hit) | Fist weapon: chitin claw mounted on leather glove |
| 6320 | thermal fault obsidian blade | weapon | hold take | — | 6 | 3 (slash) | Volcanic glass sword from thermal anomaly zone |
| 6321 | sand-sea carter's cargo hook | weapon | hold take | — | 8 | 9 (grep) | Dock-loading hook, heavy melee |
| 6322 | landward tidewright's inscribing rod | weapon | hold take | wand | 2 | 7 (pound) | Wand: Measure Script focus implement |
| 6323 | quarantine ward staff | weapon | hold take | two-handed | 3 | 7 (pound) | Sealed Route containment focus, caster two-hander |
| 6324 | glassworm queen's fang | weapon | hold take | boss | 7 | 11 (pierce) | Boss drop: Glassworm Queen. Acid-etched crystalline fang |

### Armor — Head/Face/Neck (6325-6334)

| Vnum | Name | Type | Wear | Extra | Wt | Notes |
|---:|---|---|---|---|---:|---|
| 6325 | salt-bleached sun hood | armor | head take | — | 4 | Mirror Flats head protection, linen and salt-stiffened leather |
| 6326 | glasswalk guide's tinted goggles | armor | face take | — | 2 | Glass-reading eye protection, smoked-glass lenses |
| 6327 | reach warden's patrol helm | armor | head take | — | 9 | Bronze helm with survey-mark crest |
| 6328 | cairn kite feather gorget | armor | neck take | — | 4 | Raptor-feather collar, light neck protection |
| 6329 | synod eclipse mask | armor | face take | — | 3 | Eclipsed Tongues ritual mask, dark glass and leather |
| 6330 | shoreward assessor's circlet | armor | head take | boss | 5 | Boss drop: Shoreward Revenant. Bronze circlet with dual-notation inscription |
| 6331 | red sand factor's silk veil | armor | face take | — | 2 | Desert sun protection and identity concealment |
| 6332 | toll-cairn sentinel's basalt helm | armor | head take | — | 14 | Heavy stone-reinforced helm, tank weight |
| 6333 | tidemouth salt-crust collar | armor | neck take | — | 8 | Mineral-encrusted neck guard, maritime motif |
| 6334 | arbiter's jurisdictional crown | armor | head take | boss | 6 | Boss drop: Arbiter. Basalt crown with inland and maritime law inscriptions |

### Armor — Body/About/Shoulders (6335-6344)

| Vnum | Name | Type | Wear | Extra | Wt | Notes |
|---:|---|---|---|---|---:|---|
| 6335 | glasswalk guide's leather jerkin | armor | body take | — | 7 | Chitin-reinforced desert leather, melee weight |
| 6336 | warden patrol brigandine | armor | body take | — | 12 | Bronze-studded patrol armor, tank weight |
| 6337 | synod whisper cloak | armor | about take | — | 3 | Dark cloth cloak with cult sigils woven in |
| 6338 | cairn scavenger's oiled duster | armor | about take | — | 7 | Weather-resistant salvage coat |
| 6339 | red sand outrider's silk surcoat | armor | body take | — | 4 | Fine commercial attire doubling as light armor |
| 6340 | shoreward table vestment | armor | about take | — | 5 | Assessor's ceremonial robe with conversion tables embroidered |
| 6341 | toll-marshal's triple-route tabard | armor | body take | boss | 11 | Boss drop: Toll-Marshal. Tabard bearing all three route insignia |
| 6342 | salt-crusted mantle | armor | shoulders take | — | 8 | Mineral-encrusted shoulder piece, Mirror Flats origin |
| 6343 | glassworm-hide pauldrons | armor | shoulders take | — | 10 | Translucent chitin shoulder guards |
| 6344 | tidemouth waxed storm cloak | armor | about take | — | 7 | Maritime-influenced weather cloak, resin-sealed |

### Armor — Arms/Wrist/Hands (6345-6354)

| Vnum | Name | Type | Wear | Extra | Wt | Notes |
|---:|---|---|---|---|---:|---|
| 6345 | reach warden's bronze vambraces | armor | arms take | — | 10 | Standard Warden arm protection |
| 6346 | glasswalk guide's chitin bracers | armor | wrist take | — | 3 | Glass-resistant wrist guards |
| 6347 | cairn scavenger's work gloves | armor | hands take | — | 7 | Reinforced leather digging gloves |
| 6348 | synod courier's wrist wraps | armor | wrist take | — | 2 | Dyed cloth bindings with hidden pockets |
| 6349 | red sand enforcer's gauntlets | armor | hands take | — | 10 | Brass-knuckled gloves for debt collection |
| 6350 | shoreward revenant's seal-stamp gloves | armor | hands take | boss | 4 | Boss drop: Shoreward Revenant. Ink-stained gloves that still press ghost-seals |
| 6351 | assessment hall stone bracers | armor | wrist take | — | 13 | Heavy construct-origin arm guards |
| 6352 | tidemouth crab-shell gauntlets | armor | hands take | — | 11 | Chitin-plated coastal fighting gloves |
| 6353 | thermal fault obsidian bracers | armor | wrist take | — | 6 | Volcanic glass arm guards from anomaly zone |
| 6354 | conversion engine gear-mesh sleeves | armor | arms take | — | 12 | Mechanical arm coverings from the undercroft |

### Armor — Waist/Legs/Feet (6355-6364)

| Vnum | Name | Type | Wear | Extra | Wt | Notes |
|---:|---|---|---|---|---:|---|
| 6355 | glasswalk chitin-soled sandals | armor | feet take | — | 5 | The famous glasswalk shoes: heavy leather with resin soles |
| 6356 | salt-crusted desert boots | armor | feet take | — | 8 | Mirror Flats footwear, mineral-stiffened leather |
| 6357 | reach warden's patrol belt | armor | waist take | — | 7 | Utility belt with seal pouch and ration hooks |
| 6358 | cairn scavenger's tool belt | armor | waist take | — | 8 | Chisel loops, tablet wrapping, bronze-fitting pouch |
| 6359 | synod eclipse leggings | armor | legs take | — | 4 | Dark-dyed ritual legwear |
| 6360 | tidemouth brackish waders | armor | legs take | — | 9 | Waterproofed coastal leg protection |
| 6361 | arbiter's basalt greaves | armor | legs take | boss | 13 | Boss drop: Arbiter. Stone-and-bronze leg armor |
| 6362 | heavy road caravan boots | armor | feet take | — | 10 | Thick-soled boots for loaded sled following |
| 6363 | toll-marshal's route-branded belt | armor | waist take | boss | 9 | Boss drop: Toll-Marshal. Belt buckle with three route symbols |
| 6364 | shoreward seal stone greaves | armor | legs take | — | 14 | Heavy basalt-reinforced leg armor, tank weight |

### Shields and Held Items (6365-6374)

| Vnum | Name | Type | Wear | Extra | Wt | Notes |
|---:|---|---|---|---|---:|---|
| 6365 | warden's transit seal buckler | armor | hold take | buckler | 8 | Small shield stamped with Reach survey marks |
| 6366 | cairn capstone shield | armor | hold take | — | 13 | Heavy shield carved from cairn stone |
| 6367 | glassworm-shell roundshield | armor | hold take | — | 10 | Translucent chitin shield, melee weight |
| 6368 | shoreward table fragment shield | armor | hold take | boss | 11 | Boss drop: conversion ratio chart on bronze backing |
| 6369 | red sand ledger tome | treasure | hold take | — | 3 | Held item: Outrider's debt calculation book |
| 6370 | glasswalk guide's route-song scroll | treasure | hold take | — | 2 | Held item: encoded navigation chant |
| 6371 | landward tidewright's glyph tablet | treasure | hold take | — | 4 | Held item: Littoral Branch practice tablet |
| 6372 | synod relic component pouch | treasure | hold take | — | 3 | Held item: smuggled ritual materials |
| 6373 | shoreward seal wax stamp | treasure | hold take | — | 2 | Held item: maritime seal press |
| 6374 | toll-marshal's jurisdiction scepter | treasure | hold take | boss | 6 | Boss drop: Toll-Marshal. Triple-headed authority rod |

### Armor — Miscellaneous Slots (6375-6384)

| Vnum | Name | Type | Wear | Extra | Wt | Notes |
|---:|---|---|---|---|---:|---|
| 6375 | cairn kite feather earring | armor | ear take | — | 1 | Raptor-feather ear adornment |
| 6376 | salt crystal pendant | armor | neck take | — | 2 | Mirror Flats mineral necklace |
| 6377 | synod eclipse signet | armor | finger take | — | 1 | Eclipsed Tongues cult ring |
| 6378 | warden survey compass ring | armor | finger take | — | 2 | Keeper-era directional aid |
| 6379 | glassworm tooth amulet | armor | neck take | — | 3 | Trophy pendant from Belts |
| 6380 | red sand bronze chain | armor | neck take | — | 5 | Outrider status chain, heavy links |
| 6381 | tidemouth barnacle-crusted bangle | armor | wrist take | — | 4 | Coastal wrist decoration with maritime motif |
| 6382 | shoreward dual-notation pendant | armor | neck take | boss | 3 | Boss drop: Arbiter. Pendant inscribed both desert and tide glyphs |
| 6383 | cairn station guardian's halo-sigil | armor | halo take | — | 1 | Floating glyph above ancient construct |
| 6384 | glasswalk aura ward | armor | aura take | — | 1 | Guide's protective aura, chitin-and-glass charm |

### Environmental Drops and Loot-Table Items (6385-6404)

All items in this section have `ITEM_LOOT` set in extra flags. Boss-source items also have `ITEM_BOSS`.

| Vnum | Name | Type | Wear | Extra | Wt | Notes |
|---:|---|---|---|---|---:|---|
| 6385 | salt cricket carapace | treasure | take | loot | 2 | Mirror Flats fauna drop |
| 6386 | glass scorpion stinger | treasure | take | loot | 3 | Glasswind Belt fauna drop, venomous |
| 6387 | glassworm segment | treasure | take | loot | 4 | Subsurface fauna drop, translucent chitin |
| 6388 | tidemouth crab shell | treasure | take | loot | 5 | Coastal fauna drop |
| 6389 | cairn kite tail feather | treasure | take | loot | 1 | Raptor drop, used in fletching |
| 6390 | amber glass shard | treasure | take | loot | 2 | Glasswind material, thermal fault proximity indicator |
| 6391 | violet glass fragment | treasure | take | loot | 2 | Eclipse-rite residue glass, Synod sacred material |
| 6392 | milky glass chip | treasure | take | loot | 2 | Pre-catastrophe glass, stable and safe |
| 6393 | salt crystal cluster | treasure | take | loot | 3 | Mirror Flats mineral specimen |
| 6394 | bronze cairn fitting | treasure | take | loot | 4 | Stripped from cairn stations by scavengers |
| 6395 | shoreward table fragment | treasure | take | loot | 3 | Conversion ratio chart on stone chip |
| 6396 | synod liturgical scroll | treasure | take | loot | 1 | Eclipsed Tongues religious text |
| 6397 | red sand debt token | treasure | take | loot | 1 | Outrider loan instrument, brass disc |
| 6398 | warden transit seal blank | treasure | take | loot | 2 | Unstamped seal disc, valuable to forgers |
| 6399 | tidal glyph rubbing | treasure | take | loot | 1 | Littoral Branch inscription copy |
| 6400 | quarantine ward fragment | treasure | take | loot | 3 | Sealed Route containment marker piece |
| 6401 | glassworm queen acid gland | treasure | take | loot boss | 4 | Boss drop: Glassworm Queen |
| 6402 | scavenger warlord's trophy pouch | treasure | take | loot boss | 3 | Boss drop: Cairn Scavenger Warlord |
| 6403 | commandant's cipher codex | treasure | take | loot boss | 2 | Boss drop: Whisper Cell Commandant |
| 6404 | arbiter's fractured seal-stone | treasure | take | loot boss | 5 | Boss drop: Arbiter of the Conversion |

### Quest Items (6405-6407)

| Vnum | Name | Type | Wear | Extra | Wt | Notes |
|---:|---|---|---|---|---:|---|
| 6405 | forged cairn tablet | quest | take | — | 3 | Quest 93 collect target: fabricated historical precedent |
| 6406 | reach survey waypoint marker | quest | take | — | 2 | Cartography quest series tracking item |
| 6407 | southern pyramid relic crate seal | quest | take | — | 1 | Provenance tracking for Sealed Route quests |

### Consumables and Utility (6408-6418)

| Vnum | Name | Type | Wear | Extra | Wt | Notes |
|---:|---|---|---|---|---:|---|
| 6408 | reach warden's ration brick | food | take | — | 3 | Compressed travel food, salt-preserved |
| 6409 | salt-preserved caravan jerky | food | take | — | 2 | Standard crossing provisions |
| 6410 | brackish well water flask | drink_con | take | — | 4 | Mineral-tainted but drinkable water |
| 6411 | glasswalk route-lamp | light | take | — | 3 | Shielded lantern for Belt navigation |
| 6412 | cairn station oil lamp | light | take | — | 4 | Repurposed survey lamp from cairn ruins |
| 6413 | salt-crystal torch | light | take | — | 3 | Mineral-fueled light source, Mirror Flats |
| 6414 | antivenom poultice | potion | take | — | 1 | Counter for glass scorpion / serpent venom |
| 6415 | shoreward assessment stamp | treasure | take | — | 1 | Decorative/trade item: old conversion seal |
| 6416 | transit ration token | treasure | take | — | 1 | Ceramic allotment stamp modeled on Northern Oasis design |
| 6417 | glasswalk shoe repair kit | treasure | take | — | 3 | Chitin and resin sole patches, trade goods |
| 6418 | cairn tablet transcription | treasure | take | — | 2 | Scholar's copy of historical toll schedules |

### Containers (6419-6421)

| Vnum | Name | Type | Wear | Extra | Wt | Notes |
|---:|---|---|---|---|---:|---|
| 6419 | warden dispatch satchel | container | take | — | 3 | Patrol communication bag |
| 6420 | cairn scavenger's salvage sack | container | take | — | 4 | Large carry bag for stripped materials |
| 6421 | synod courier's sealed pouch | container | take | — | 2 | Contraband transport container |

### Furniture (6422-6424)

| Vnum | Name | Type | Wear | Extra | Wt | Notes |
|---:|---|---|---|---|---:|---|
| 6422 | warden barracks cot | furniture | — | — | 15 | Rest furniture in District A barracks |
| 6423 | glasswalk guide's desert bedroll | furniture | — | — | 8 | Rest furniture in guide camps |
| 6424 | cairn station stone bench | furniture | — | — | 15 | Rest furniture in cairn interiors |

### Itemization Intent

- Blend oasis-administrative motifs (charter stones, water measures, witness seals) with maritime conversion motifs (wax stamps, draft tables, storm charts).
- Environmental gear reflects terrain: salt-crusted boots for Mirror Flats, chitin-soled sandals for Glasswind, waxed cloaks for Tidemouth.
- Boss drops should visibly bridge the inland-maritime divide: weapons inscribed with both desert glyph and tide notation, armor with salt-crust and barnacle motifs.
- Extra descriptions on key objects should contain inscription text in the three voice styles (Survey-Administrative, Legal-Conversion, Guide-Oral).
- Keyed doors in Districts E (Measure's Rest sealed archive), F (sealed relic vault), and H (undercroft descent) each have explicit key objects.
- All loot-table items have `ITEM_LOOT`. All boss-source items have both `ITEM_LOOT` and `ITEM_BOSS`.

---

## Progression and Difficulty Curve

- **6300-6349** (District A): onboarding, 55-58. Safe hub with friendly/neutral NPCs and light patrols.
- **6350-6399** (District B): Mirror Flats crossing, 55-62. Environmental hazard + fauna.
- **6400-6449** (District C): Heavy Road crossing, 57-64. Moderate fauna + faction skirmishes.
- **6450-6499** (District D): Glasswind Belts, 60-70. Exploration hazard + Synod + glass fauna + mini-boss.
- **6500-6549** (District E): Cairn Line Ruins, 62-72. Faction density + vertical exploration + mini-boss.
- **6550-6599** (District F): Sealed Route, 65-75. Dangerous relic hazards + Synod operations + mini-boss.
- **6600-6649** (District G): Tidemouth Approach, 60-68. Open terrain + coastal fauna + jurisdiction conflict.
- **6650-6699** (District H): Shoreward Threshold, 70-80. Elite + boss gauntlet.

---

## Quest Plan (Quest Files 81-95)

All quests use offerer vnums from within the area (`6300-6699`) unless specified as external quest givers. Quest target vnums reference mobs within the area.

### Cartography Quest Chain (81-82)

| File | Static ID | Title | Prereq | Type | Description |
|---|---:|---|---:|---:|---|
| `81.prop` | 80 | Saltglass Reach cartography survey: Mirror Flats | -1 | 1 (kill-variety) | Survey the Mirror Flats by eliminating threats at key waypoints. Kill one of each: salt cricket swarm leader, cairn kite alpha, corrupted Warden toll-thief. Offerer: Reach Warden cartographer in District A. Reward: 4000 gold, 4 qp. Level 55-70. |
| `82.prop` | 81 | Saltglass Reach cartography survey: Glasswind to Tidemouth | 80 | 1 (kill-variety) | Complete the Reach survey by clearing observation points from the Glasswind Belts to the Tidemouth Dunes. Kill one of each: glass scorpion matriarch, glassworm tunnel sentinel, tidemouth crab broodmother. Offerer: same cartographer. Reward: 6576 gold, 5 qp. Level 60-75. |

### Faction Conflict Quests (83-87)

| File | Static ID | Title | Prereq | Type | Description |
|---|---:|---|---:|---:|---|
| `83.prop` | 82 | Reach Warden toll restoration | -1 | 3 (kill-count) | Eliminate corrupted Warden toll-enforcers extorting caravans along the Swift Line. Kill 8 corrupted Warden toll-thieves. Offerer: Warden Commander at Measure's Rest. Reward: 1266 gold, 4 qp. Level 55-70. |
| `84.prop` | 83 | Red Sand Outrider interdiction | -1 | 1 (kill-variety) | Disrupt Red Sand Accounts operations by eliminating their field agents. Kill one of each: Red Sand debt collector, Red Sand assessor, Red Sand enforcer. Offerer: Reach Warden patrol leader. Reward: 4500 gold, 5 qp. Level 60-75. |
| `85.prop` | 84 | Synod whisper cell disruption | -1 | 1 (kill-variety) | Intercept Synod smuggling operations in the Glasswind Belts. Kill one of each: Synod courier, Synod ritual keeper, Synod lookout. Offerer: Reach Warden at Glasswind approach. Reward: 4000 gold, 4 qp. Level 58-72. |
| `86.prop` | 85 | Cairn scavenger expulsion | -1 | 3 (kill-count) | Clear Cairn Scavengers stripping Measure's Rest of irreplaceable tablets. Kill 10 cairn scavengers. Offerer: Warden Commander. Reward: 3800 gold, 4 qp. Level 60-72. |
| `87.prop` | 86 | Sealed Route quarantine enforcement | 84 | 1 (kill-variety) | Re-establish quarantine along the abandoned Sealed Route by eliminating Synod occupation forces. Kill one of each: Synod Whisper Cell commandant, quarantine breach horror, Synod tunnel sentry. Offerer: Reach Warden officer. Reward: 5500 gold, 5 qp. Level 65-78. |

### Boss Quests (88-92)

| File | Static ID | Title | Prereq | Type | Description |
|---|---:|---|---:|---:|---|
| `88.prop` | 87 | Glassworm burrower queen extermination | -1 | 3 (kill-count) | The glassworm burrower queen threatens route stability across the Belts. Destroy her. Kill 1 glassworm burrower queen. Offerer: Glasswalk Guide elder. Reward: 5000 gold, 5 qp. Level 62-75. |
| `89.prop` | 88 | Whisper Cell commandant elimination | 84 | 3 (kill-count) | Track and eliminate the Synod Whisper Cell commandant directing smuggling operations from the Sealed Route. Kill 1 Whisper Cell commandant. Offerer: Warden Commander. Reward: 6576 gold, 6 qp. Level 68-78. |
| `90.prop` | 89 | Shoreward Revenant banishment | -1 | 3 (kill-count) | The Shoreward Revenant — a dead Assessor who refuses to yield jurisdiction — haunts the Shoreward Threshold. Destroy it. Kill 1 Shoreward Revenant. Offerer: Reach Warden at Tidemouth. Reward: 7000 gold, 6 qp. Level 70-80. |
| `91.prop` | 90 | Toll-Marshal of the Three Routes: writ of dissolution | 89 | 3 (kill-count) | The Toll-Marshal enforces all three routes simultaneously, blocking the Shoreward approach. Execute the writ of dissolution. Kill 1 Toll-Marshal of the Three Routes. Offerer: same. Reward: 1832 gold, 7 qp. Level 75-80. |
| `92.prop` | 91 | Arbiter of the Conversion: final jurisdictional reckoning | 90 | 3 (kill-count) | The Conversion itself has become hostile sentience. End the Arbiter before the legal contradiction consumes the Threshold. Kill 1 Arbiter of the Conversion. Offerer: Warden Commander. Reward: 11000 gold, 9 qp. Level 78-80. |

### Exploration and Lore Quests (93-95)

| File | Static ID | Title | Prereq | Type | Description |
|---|---:|---|---:|---:|---|
| `93.prop` | 92 | Cairn tablet recovery: forged precedents | -1 | 2 (collect-items) | A Mafdet Ledger House clerk identified forged cairn tablets in Measure's Rest. Recover the forgeries for analysis. Collect forged cairn tablet (quest item). Offerer: Kowloon scholar at District E. Reward: 4500 gold, 5 qp. Level 60-75. |
| `94.prop` | 93 | Glass-field thermal anomaly investigation | -1 | 1 (kill-variety) | A Glasswalk Guide family reports freshly fused glass in a previously stable section. Investigate the thermal anomaly and eliminate its source. Kill one of each: thermal fault elemental, vitrified sand horror, molten glass sentinel. Offerer: Glasswalk Guide elder. Reward: 5500 gold, 5 qp. Level 65-78. |
| `95.prop` | 94 | Tidemouth jurisdiction enforcement sweep | -1 | 1 (kill-variety) | Clear hostiles from the contested Tidemouth zone where Reach and Harbor Warden jurisdictions overlap. Kill one of each: tidemouth raider, rogue outrider, jurisdiction specter. Offerer: Tidemouth Warden post commander. Reward: 5000 gold, 5 qp. Level 60-72. |

### Quest Offerer Mob Summary

| Offerer Role | Approximate Vnum | Location |
|---|---|---|
| Reach Warden Cartographer | ~6324 | District A: Transit Seal Office |
| Warden Commander | ~6528 | District E: Measure's Rest |
| Reach Warden Patrol Leader | ~6308 | District A: Warden Barracks |
| Glasswalk Guide Elder | ~6458 | District D: Guide Waystation |
| Kowloon Scholar | ~6532 | District E: Kowloon Scholar's Camp |
| Tidemouth Warden Post Commander | ~6635 | District G: Tidemouth Warden Post |
| Reach Warden Officer (Sealed Route) | ~6550 | District F: Sealed Route approach |

---

## Reset and Door Plan

### Door Plan

| Location | Door Description | Key Object | Reset State |
|---|---|---|---|
| Measure's Rest: Sealed Archive (6526) | Heavy bronze door with Keeper-era seal | Warden archive key | Locked (state 2) |
| Sealed Relic Vault (6573) | Iron-bound quarantine door with containment wards | Quarantine seal key | Locked (state 2) |
| Undercroft Descent (6685) | Basalt hatch with dual-jurisdiction inscription | Shoreward threshold key | Locked (state 2) |

All locked doors require:
- `EX_ISDOOR` set on both sides.
- Valid key vnum object in `#OBJECTS`.
- `#RESETS` command `D` with state `2`.

### Reset Density

- **District A:** Moderate — Warden sentinels, guide NPCs, ambient caravan traffic.
- **Districts B/C:** Light to moderate — fauna clusters at waypoints, sparse patrols.
- **District D:** Moderate to heavy — glass fauna, Synod operatives, Guide waystation NPCs.
- **District E:** Heavy — Scavengers, Wardens, scholars, cairn station defenders.
- **District F:** Moderate — Synod occupation, quarantine horrors, scattered scavengers.
- **District G:** Light — coastal fauna, sparse faction presence, transitional.
- **District H:** Heavy — elite revenants, boss encounters, assessment hall defenders.

---

## Implementation Checklist

1. Create all 400 room entries with unique descriptions, proper topology, and bi-directional exits.
2. Create all mob templates with faction-appropriate flags, skills, and loot tables.
3. Create all objects with thematic consistency, proper wear/extra flags, and unique names.
4. Set up all resets: mob placement, object spawns, door states.
5. Configure specials: combat casters for Synod mobs, breath weapons for fauna bosses, ambient specials for NPCs.
6. Write quest files `81.prop` through `95.prop` with proper format.
7. Validate all strings, exits, door flags, key existence, and no-vnum-in-text policy.
8. Run area parser/load checks and in-game traversal sanity pass.
9. Verify western connections to oasis areas and eastern placeholder for future Mafdet connection.

---

## Deliverables

- `docs/lore/saltglass_reach_lore.md` (already exists — canon lore bible).
- `docs/plans/saltglass_reach_plan.md` (this document).
- Future: `area/saltglass_reach.are` (implementation pass aligning with this plan and spec constraints).
- Future: `quests/81.prop` through `quests/95.prop` (quest files).
