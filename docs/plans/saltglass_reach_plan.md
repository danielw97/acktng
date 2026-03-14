# Area Plan: Saltglass Reach

## Overview

**Area Name:** `@@yThe @@bSaltglass @@yReach@@N`
**File Name:** `saltglass_reach.are`
**Keyword:** `saltglass reach desert littoral cairn`
**Area Number:** `N 311`
**Level Range:** `I 55 80` (`55-80`)
**Vnum Range:** **31100-31499** (`V 31100 31499`)
**Room Count Target:** **400/400 rooms used**
**Primary Design Goal:** Build a massive jurisdictional transition corridor spanning four ecological bands — from the oasis caravan basin to the Mafdet coast — as a traversable frontier of overlapping legal authority, shattered survey infrastructure, and environmental hazard. The area is not a linear road; it is a web of routes, ruins, and contested waypoints where inland law fractures into maritime pragmatism.

**Western Connections:** Great Oasis (8600-8699), Northern Oasis (8700-8899), Southern Oasis (9800-9999) — the Reach's western approach connects to the oasis caravan basin.
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

- **Great Oasis** (8600-8699, L40-60): The institutional headwater for the Reach's legal systems. Charter witness formulas from the Great Oasis appear in adapted form on Reach cairn tablets. The Great Oasis historically functioned as the last major freshwater arbitration basin for caravans bound east to the coast — seaward convoys took certified water-right and charter attestations from the oasis, crossed the Reach, and converted claims into harbor contracts at Mafdet. The Great Oasis Measuring House's core samples from the Mirror Flats document forty seasonal layers of Iseth Reach flooding.
- **Northern Oasis** (8700-8899, L45-65): Ration-stamp conventions influenced the Reach's transit water tracking. Reach Wardens issue transit ration tokens modeled on Northern Oasis ceramic stamps. Mafdet's Ledger House clerks accept Northern Oasis witness seals at premium rates.
- **Southern Oasis** (9800-9999, L50-70): Casualty-processing protocols extend into the Reach — deaths on the Reach are recorded using Southern Oasis Casualty Ledger formats and forwarded for archival. The Southern Oasis Mortuary Circle maintains correspondence with Mafdet's Quarantine Ledger regarding relic hazard classifications. Southern Pyramid relics authenticated at the Southern Oasis carry premium provenance in Mafdet's maritime market.

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
- `N 311`
- `I 55 80`
- `V 31100 31499` fixed.
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
     31100-31149        31150-31199       31200-31249
            |           |           |
            +-----+-----+-----+----+
                  |           |
          [D: Glasswind    [E: Cairn Line
           Belts]           Ruins]
           31250-31299      31300-31349
                  |           |
            +-----+-----+----+
            |                 |
    [F: Sealed Route   [G: Tidemouth
     Remnants]          Approach]
     31350-31399        31400-31449
            |                 |
            +--------+--------+
                     |
              [H: Shoreward
               Threshold]
               31450-31499
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

## Room-by-Room Allocation (31100-31499)

### District A: Oasis Staging Grounds (31100-31149, 50 rooms)

**Theme:** Caravan marshaling area at the western mouth of the Reach, where oasis-certified convoys prepare for the crossing. Warden patrol headquarters, toll-assessment booths, and guide-hiring posts cluster around a dusty transit yard. This is the last place where oasis law operates without question.

**Internal Shape:** A central hub (the Staging Yard) with five spokes — north road from the Northern Oasis approach, south road from the Southern Oasis approach, west road from the Great Oasis approach, and two eastward departures (one toward the Mirror Flats North, one toward Mirror Flats South). Side branches lead to Warden barracks, a guide camp, a Red Sand counting house, and caravan repair stalls.

Key rooms:
- 31100 **Western Approach Road** — entry from oasis basin; 5+ sentences; connects west to oasis areas.
- 31101 **Dust-Choked Milestone** — first Reach marker, eroded Keeper survey stone.
- 31102 **Caravan Muster Ground** — open staging area, caravan traffic flavor.
- 31103 **Warden Toll Gate** — first toll checkpoint, manned by Reach Wardens.
- 31105 **Staging Yard** — central hub, 5+ sentences, multiple exits radiating outward.
- 31108 **Reach Warden Barracks** — patrol headquarters, sentinel guards.
- 31110 **Glasswalk Guide Camp** — neutral guide-hiring post, route song flavor.
- 31112 **Red Sand Counting House** — Outrider commercial predator den.
- 31115 **Northern Oasis Approach** — road connecting north toward Northern Oasis.
- 31118 **Southern Oasis Approach** — road connecting south toward Southern Oasis.
- 31120 **Caravan Repair Yard** — sled runners, harness mending, shade shelters.
- 31124 **Transit Seal Office** — where crossing permits are issued.
- 31128 **Drywell Rest Stop** — last functioning well before the Flats.
- 31130 **Heavy Road Departure** — eastward exit toward Mirror Flats South / central route.
- 31135 **Swift Line Departure** — eastward exit toward Mirror Flats North / fast route.
- 31140 **Provisioner's Row** — merchant stalls for crossing supplies.
- 31145 **Cairn Tablet Reading Room** — scholarly annex with extra descriptions for lore.
- 31149 **Eastern Boundary Marker** — last oasis-jurisdiction marker, transition text.

Remaining rooms (31104, 31106-31107, 31109, 31111, 31113-31114, 31116-31117, 31119, 31121-31123, 31125-31127, 31129, 31131-31134, 31136-31139, 31141-31144, 31146-31148): connecting passages, side alleys, storage rooms, guard posts, caravan animal pens, shade structures, and ambient transition corridors filling out the hub layout.

---

### District B: Mirror Flats North — Swift Line (31150-31199, 50 rooms)

**Theme:** The northern crossing of the Mirror Flats, following the old Swift Line — the fast route for light, high-value cargo. Hard salt crust blinds by day and reflects moonlight at night. Caravans cross at dawn and dusk. The Flats are a geological remnant of the Iseth Reach's terminal evaporation basin. Salt crickets swarm during fog events. This route is faster but more exposed.

**Internal Shape:** A sinuous east-west spine with short north-south deviations into salt-crystal formations, exposed survey rod stations, and a central maze-flagged blinding zone where the reflection is worst. The northern edge has a hidden path leading to a salt cricket breeding ground. The eastern end descends into the Glasswind Belt approach.

Key rooms:
- 31150 **Swift Line Marker One** — first cairn of the northern route, faded toll schedule.
- 31153 **Salt Mirror Crossing** — blinding white expanse, 5+ sentences, dawn/dusk crossing flavor.
- 31155 **Keeper Tide Rod Station** — ruined tide-measurement outpost with basalt stumps.
- 31158 **Crystal Ridge** — elevated salt formation, lookout point, cairn kite nests.
- 31160 **The Blinding Heart** — center of the Flats, maze-flagged zone begins (ROOM_MAZE).
- 31161-31167 **Maze: Reflected Expanse** — 7 maze rooms, disorienting salt-mirror terrain.
- 31170 **Salt Cricket Breeding Ground** — swarm encounter pocket, hidden north branch.
- 31173 **Shattered Evaporation Basin** — deep salt layers exposed, geological lore.
- 31175 **Swift Line Waypost** — ruined rest shelter, competed over by factions.
- 31178 **Mineral Seep** — rare moisture point, salt-encrusted spring.
- 31180 **Surveyor's Cairn Remnant** — old Keeper marker with Littoral Branch glyphs.
- 31185 **Salt Crust Collapse** — hazard room, thin crust over void.
- 31190 **Eastern Salt Shelf** — Flats thin out, first glass shards appear underfoot.
- 31195 **Swift Line Terminal** — last northern cairn before the Glasswind Belt.
- 31199 **Descent to the Glasswind** — transition to District D.

Remaining rooms: connecting salt-flat passages, wind-scoured channels, abandoned sled tracks, mineral deposit clusters, exposed survey benchmarks, and environmental traversal rooms.

---

### District C: Mirror Flats South — Heavy Road (31200-31249, 50 rooms)

**Theme:** The southern crossing of the Mirror Flats, following the old Heavy Road — the slow route for bulk commodities. Deeper salt deposits, wider passages carved by generations of laden cargo sleds. Every third cairn had a full assessment station. This route is safer but slower, with more infrastructure ruins and more faction competition.

**Internal Shape:** A broader east-west corridor with assessment station complexes branching off the main road at regular intervals. A southern spur leads to the abandoned Sealed Route junction. The eastern end connects to the Cairn Line Ruins district. A central fortified waystation serves as a mid-crossing anchor.

Key rooms:
- 31200 **Heavy Road Gate** — entry cairn, toll schedule still partially legible.
- 31203 **First Assessment Station** — ruined inspection hall, bronze scale remnants.
- 31206 **Cargo Sled Ruts** — deep-worn passage, generations of traffic visible.
- 31210 **Fortified Waystation** — mid-crossing rest point, 5+ sentences, hub room.
- 31212 **Waystation Cistern** — dry cistern with mineral stratification lore.
- 31215 **Second Assessment Station** — larger ruin, Shoreward Table fragments on walls.
- 31218 **Sealed Route Junction** — southern spur departure, quarantine markers.
- 31220 **Convoy Graveyard** — abandoned cargo, broken sled frames, scavenger activity.
- 31225 **Third Assessment Station** — best-preserved station, extra descriptions for conversion tables.
- 31228 **Heavy Road Overwatch** — elevated position, cairn kite observation.
- 31230 **Salt-Worn Foundation Row** — exposed building foundations from Ledger-Tariff era.
- 31235 **Brine Pool** — rare standing water, mineral-stained, fauna gathering point.
- 31240 **Eastern Heavy Road** — road narrows as salt gives way to glass-mixed terrain.
- 31245 **Heavy Road Terminal Cairn** — last Heavy Road marker, transition text.
- 31249 **Cairn Line Approach** — connects east to District E.

Remaining rooms: toll booths, guard posts, caravan shelter alcoves, salt-mining remnants, assessment clerk quarters, and transit corridor sections.

---

### District D: Glasswind Belts (31250-31299, 50 rooms)

**Theme:** Fields of wind-cut silica shards that abrade skin, cloth, and pack harness. The glass formed during the Ninth Meridian catastrophe's thermal pulse. Glass color indicates danger: milky (safe, pre-catastrophe), amber (thermal fault proximity), violet (eclipse-rite residue). Glasswalk Guides are essential for safe passage. Synod Whisper Cells use the Belts as concealment corridors. Glassworms burrow beneath the crust, creating collapse hazards.

**Internal Shape:** A tangled web of narrow glass-walled passages, hidden Glasswalk paths, Synod smuggler tunnels, and open glass fields. The district branches extensively — multiple paths lead east, but only certain routes are safe. A central area contains a Glasswalk Guide waystation (neutral ground). The southern portion connects to the Sealed Route remnants. Dead-end glass canyons and worm-collapse chambers provide exploration danger.

Key rooms:
- 31250 **Glasswind Threshold** — first glass underfoot, color-reading tutorial in description.
- 31253 **Milky Glass Corridor** — safe older glass, firm footing, Guide waymarker.
- 31255 **Amber Glass Warning** — thermal fault proximity, temperature shift described.
- 31258 **Glasswalk Guide Waystation** — neutral hub, 5+ sentences, guide services.
- 31260 **Shardstorm Narrows** — wind-cut passage, abrasion hazard flavor.
- 31263 **Violet Glass Field** — eclipse-rite residue zone, Synod theological significance.
- 31265 **Glassworm Sinkhole** — collapse hazard, hollow-ringing glass crust.
- 31268 **Synod Whisper Cache** — hidden smuggler stash, cult graffiti in Eclipsed Tongues script.
- 31270 **Thermal Fault Ridge** — elevated glass ridge, newly fused glass visible.
- 31273 **Freshly Vitrified Sand** — recent thermal event evidence, plot hook room.
- 31275 **Glass Canyon Dead End** — exploration trap, worm ambush.
- 31278 **Wind-Cut Gallery** — natural glass sculpture, environmental beauty amid hazard.
- 31280 **Glasswalk Secret Path** — hidden safe route known only to Guides.
- 31283 **Eclipsed Tongues Shrine** — Synod worship site, theological inscriptions.
- 31285 **Glass Scorpion Nest** — dense predator zone, glass-carapace fauna.
- 31288 **Subglass Worm Warren** — underground passage beneath glass crust.
- 31290 **Eastern Glass Shelf** — Belts thin, cairn ruins visible ahead.
- 31293 **Synod Courier Route** — unmarked whisper route bypassing cairn inspection.
- 31295 **Guide's Song Marker** — route song inscription, oral tradition waypoint.
- 31299 **Glasswind Eastern Exit** — transition to cairn district or Tidemouth approach.

Remaining rooms: glass-walled passages, wind-sheltered alcoves, partial glass caves, scorpion ambush corridors, worm tunnel intersections, shattered glass slopes, and Guide-marked safe passages.

---

### District E: Cairn Line Ruins — Measure's Rest (31300-31349, 50 rooms)

**Theme:** The ruined checkpoint chains where the three route systems converged into a major cairn station complex. This district contains **Measure's Rest** — the fortified Reach Warden library housing the most complete surviving record of the Reach's legal infrastructure. Cairn Scavengers strip bronze fittings and tablet fragments. Red Sand Outriders have purchased several stations outright. The district has significant vertical development — cairn station interiors go up into watchtowers and down into sealed tablet vaults.

**Internal Shape:** A cluster of interconnected cairn stations arranged in a rough crescent around Measure's Rest. Each station has surface-level ruins (patrol rooms, toll gates) and underground chambers (tablet vaults, assessment archives). Measure's Rest itself is a multi-level complex with a ground floor library, upper observation deck, and lower sealed archive. The layout resembles a fortified compound, not a road segment.

Key rooms:
- 31300 **Cairn Line Approach** — first visible cairn station, ruined but recognizable.
- 31302 **Station One: Bronze Gate** — stripped cairn station, scavenger activity.
- 31305 **Station Two: Toll Hall** — better preserved, competing toll signs from rival claimants.
- 31308 **Station Three: Assessment Court** — Shoreward Assessor hall with wind baffles.
- 31310 **Cairn Station Crossroads** — hub where three route lines converge, 5+ sentences.
- 31312 **Red Sand Purchased Station** — Outrider-controlled toll point, commercial predation.
- 31315 **Scavenger Market** — informal trade in stripped cairn materials.
- 31318 **Landward Tidewright Cell** — archaic scribes preserving full Measure Script.
- 31320 **Measure's Rest: Outer Court** — Warden fortress approach, 5+ sentences.
- 31322 **Measure's Rest: Library Hall** — cairn tablet transcription archive, lore-dense.
- 31324 **Measure's Rest: Upper Observation** — watchtower with views across the Reach.
- 31326 **Measure's Rest: Sealed Archive** — locked lower vault, key required, valuable tablets.
- 31328 **Measure's Rest: Warden Commander's Quarters** — faction leadership space.
- 31330 **Tablet Restoration Workshop** — scholarly repair and translation work.
- 31332 **Kowloon Scholar's Camp** — Jade Clerk-funded expedition camp, cross-area flavor.
- 31335 **Forgery Detection Room** — where fabricated tablets are identified.
- 31338 **Old Swift Line Junction** — ruined northern cairn, fast-route remnant.
- 31340 **Old Heavy Road Junction** — ruined central cairn, bulk-route remnant.
- 31342 **Old Sealed Route Junction** — abandoned southern cairn, quarantine markers.
- 31345 **Cairn Capstone Observatory** — shadow-rod mounting holes, astronomical survey.
- 31348 **Eastern Cairn Line** — last cairns before Tidemouth.
- 31349 **Tidemouth Descent** — transition to District G.

Remaining rooms: individual cairn interiors, scavenger tunnels, patrol corridors, tablet niche chambers, assessment clerk quarters, fortification walls, and connecting passages between stations.

---

### District F: Sealed Route Remnants (31350-31399, 50 rooms)

**Theme:** The abandoned southern cairn chain dedicated to funerary and relic consignments. When the funerary institutions fragmented during the Fracture Era, the Sealed Route was abandoned. Now the quarantine stations are infiltrated by Synod Whisper Cells, and the sealed containers left behind attract Cairn Scavengers and dangerous relic hazards. This is the most dangerous and least-traveled district.

**Internal Shape:** A linear southern spur branching off the Heavy Road, then curving east through quarantine stations, sealed relic vaults, and Synod-occupied passages. Dead ends and locked doors are common — this was designed for controlled passage, not free movement. The southern edge borders impassable terrain. A hidden Synod tunnel connects back to the Glasswind Belts.

Key rooms:
- 31350 **Sealed Route Marker One** — quarantine warning inscription, 5+ sentences.
- 31352 **First Quarantine Station** — inspection hall for relic consignments.
- 31355 **Sealed Container Yard** — abandoned cargo, some containers still sealed.
- 31358 **Quarantine Scribe Office** — Hazard Reclassification workspace.
- 31360 **Sealed Route Fortification** — armed escort staging area, arrow loops in walls.
- 31363 **Relic Examination Chamber** — provenance verification room, extra descriptions.
- 31365 **Witness Certification Hall** — where every cairn required witness certification.
- 31368 **Synod-Infiltrated Station** — Whisper Cell occupation, cult activity.
- 31370 **Eclipsed Tongues Ritual Room** — active Synod worship, theological inscriptions.
- 31373 **Sealed Relic Vault** — locked door, dangerous contents, key required.
- 31375 **Quarantine Breach** — broken containment, hazard released.
- 31378 **Funerary Consignment Depot** — Southern Pyramid provenance marks.
- 31380 **Synod Smuggler Tunnel** — hidden passage back to Glasswind Belts.
- 31383 **Abandoned Escort Barracks** — Fracture-Era military quarters.
- 31385 **Relic Cold-Storage Chamber** — unnaturally cold room, sealed crate that hums at dawn.
- 31388 **Route Terminus Ruin** — the point where the Sealed Route simply stopped.
- 31390 **Collapsed Quarantine Arch** — structural failure, rubble exploration.
- 31393 **Synod Command Post** — Whisper Cell leadership, intelligence cache.
- 31395 **Defaced Marker Row** — competing toll signs defacing each other.
- 31399 **Sealed Route Eastern Remnant** — connects to Tidemouth approach.

Remaining rooms: quarantine corridors, sealed niches, guard posts, escort route sections, abandoned relic inspection chambers, Synod hideouts, and structural collapse zones.

---

### District G: Tidemouth Approach (31400-31449, 50 rooms)

**Theme:** Low dunes and brackish channels marking approach to the coast. The desert's dry ecology gives way to salt marsh, tidal pools, and mineral-stained sand. Tidemouth crabs dig burrows that collapse under weight. The smell of sea-salt mixes with desert mineral dust. Maritime influence increases — anchor stones, hull-caulking resin dumps, sailcloth windbreaks. This is where the Reach's administrative logic historically terminated and Mafdet's harbor jurisdiction began.

**Internal Shape:** A broad, open area that narrows eastward toward the Shoreward Threshold. Multiple paths wind through dune ridges, tidal channels, and crab-burrowed flats. A northern branch follows the coast toward future Mafdet approach. A southern branch explores tide-pool caves. The central path leads to the Shoreward Seal district. The openness contrasts with the confined cairn stations — players can see the sea from elevated dunes.

Key rooms:
- 31400 **First Crab Burrow** — landmark: half a day from Mafdet, 5+ sentences.
- 31403 **Brackish Channel Crossing** — tidal water, desert-to-coast transition.
- 31405 **Dune Crest Overlook** — first sight of the sea on the eastern horizon.
- 31408 **Anchor Stone Field** — maritime debris begins, resin dumps, sailcloth scraps.
- 31410 **Tidemouth Flats** — open salt marsh, crab burrow hazards, 5+ sentences.
- 31413 **Tidal Pool Grotto** — coastal cave, amphibious fauna.
- 31415 **Sand-Sea Carters' Camp** — Guild caravan-to-ship transfer operation.
- 31418 **Crab Warren** — dense burrow network, collapse risk.
- 31420 **Mineral-Stained Dune** — vivid color from coastal mineral deposits.
- 31423 **Tide Obelisk Stump** — Keeper-era tidal measurement station, basalt remnant.
- 31425 **Saltmarsh Trail** — winding path through coastal vegetation.
- 31428 **Resin Dump Clearing** — hull-caulking waste, maritime industry evidence.
- 31430 **Wind-Torn Sailcloth Shelter** — improvised camp using ship materials.
- 31433 **Brackish Well** — last freshwater source, mineral-tainted but drinkable.
- 31435 **Tidemouth Warden Post** — contested jurisdiction checkpoint.
- 31438 **Harbor Warden Forward Camp** — Mafdet's jurisdiction extending inland.
- 31440 **Dual Jurisdiction Zone** — where Reach Wardens and Harbor Wardens overlap.
- 31443 **Coastal Fog Bank** — reduced visibility, smuggling cover during Fog Season.
- 31445 **Mafdet Strand Approach** — eastern exit toward future Mafdet, 5+ sentences.
- 31449 **Tidemouth Eastern Edge** — transition to District H.

Remaining rooms: tidal channels, crab-burrowed passages, dune ridges, coastal caves, maritime debris fields, fishing camps, and wind-scoured transition terrain.

---

### District H: Shoreward Threshold (31450-31499, 50 rooms)

**Theme:** The legal boundary zone where inland law formally yields to maritime jurisdiction. The Shoreward Seal — a large basalt slab carved with both inland charter notation and maritime loading symbols — marks the physical threshold. This district contains the ruins of the great Shoreward Assessment Halls, the Water-Debt Discharge basin, and the Dual Attestation Court. It is also the site of the area's major boss encounters, where the unresolved legal contradictions of the Reach manifest as hostile forces.

**Internal Shape:** A processional approach leading to the Shoreward Seal plaza, flanked by ruined Assessment Halls. Side chambers contain the specialized conversion facilities (Water-Debt Discharge basin, Risk Assumption Oath court, Relic Quarantine Transfer hall). Below the plaza, a sealed undercroft contains the most ancient jurisdictional machinery. The final rooms are boss encounter spaces where the Reach's contested authority takes physical form.

Key rooms:
- 31450 **Shoreward Approach** — processional road, assessment hall ruins visible ahead.
- 31452 **Assessment Hall of the Swift Line** — northern route's terminal conversion facility.
- 31455 **Assessment Hall of the Heavy Road** — central route's conversion facility, 5+ sentences.
- 31458 **Assessment Hall of the Sealed Route** — southern route's quarantine conversion.
- 31460 **Shoreward Assessors' Court** — specialist clerk training school ruins.
- 31462 **Shoreward Table Hall** — conversion ratio charts carved into walls, extra descriptions.
- 31465 **Water-Debt Discharge Basin** — basalt basin where water-debt was symbolically terminated.
- 31468 **Risk Assumption Oath Court** — where liability transferred from road to sea.
- 31470 **Relic Quarantine Transfer Hall** — hazard reclassification chamber.
- 31472 **Dual Attestation Chamber** — where both inland and maritime seals were applied.
- 31475 **The Shoreward Seal** — the great basalt slab itself, 5+ sentences, area landmark.
- 31478 **Dry Seal Archive** — inland witness mark storage.
- 31480 **Wet Seal Foundry** — maritime wax/bitumen seal preparation.
- 31482 **Jurisdictional Threshold** — the exact legal boundary, dual authority text.
- 31485 **Undercroft Descent** — stairs down into sealed sub-level.
- 31487 **Undercroft: Ancient Conversion Engine** — pre-Keeper jurisdictional machinery.
- 31490 **Undercroft: Fractured Authority Chamber** — where competing laws collide, boss prelude.
- 31492 **Lair of the Shoreward Revenant** — mini-boss room, no_mob flag.
- 31494 **Toll-Marshal of the Three Routes** — major boss room, no_mob flag, 5+ sentences.
- 31496 **Sanctum of Contested Jurisdiction** — second major boss, no_mob flag.
- 31498 **Heart of the Conversion** — final boss encounter, no_mob flag, 5+ sentences.
- 31499 **The Eastern Shore** — exit toward Mafdet, area terminus, 5+ sentences.

Remaining rooms: conversion clerk offices, seal storage rooms, undercroft passages, collapsed assessment wings, connecting corridors, and atmospheric transition rooms.

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

### Mob Vnum Plan

Use mob vnums sequentially within `31100-31499`. Target **65-80 mob templates**:

| Category | Count | Level Range | Notes |
|---|---:|---|---|
| Reach Warden patrols | 6-8 | 55-65 | Mix of friendly sentinels and corrupted hostiles |
| Red Sand Outriders | 5-7 | 60-72 | Commercial predators, solo-flagged elites |
| Synod Whisper Cells | 6-8 | 58-70 | Cult operatives, casters with shadow/fire |
| Cairn Scavengers | 5-7 | 55-65 | Relic hunters, melee focus |
| Salt Flat fauna | 5-6 | 55-62 | Crickets, kites, salt-adapted creatures |
| Glasswind fauna | 6-8 | 60-70 | Glass scorpions, glassworms, glass-adapted predators |
| Tidemouth fauna | 4-6 | 58-65 | Crabs, amphibious creatures |
| Cairn station defenders | 4-6 | 62-72 | Remnant guards, construct sentinels |
| Sealed Route horrors | 5-7 | 65-75 | Quarantine breach manifestations, relic hazards |
| Shoreward revenants | 6-8 | 70-78 | Jurisdictional undead, conversion-engine constructs |
| Mini-bosses | 3-4 | 68-75 | Solo flagged; see boss cadence below |
| Major bosses | 3-4 | 75-80 | Boss flagged, sentinel, no_flee; in no_mob rooms |

### Boss Cadence

- **Mini-boss: Glassworm Burrower Queen** (District D, ~31275) — fauna apex predator of the Belts.
- **Mini-boss: Whisper Cell Commandant** (District F, ~31393) — Synod operative leader.
- **Mini-boss: Cairn Scavenger Warlord** (District E, ~31315) — relic hunter faction leader.
- **Boss: Shoreward Revenant** (31492) — undead Shoreward Assessor, caster/melee hybrid. Level 75.
- **Boss: Toll-Marshal of the Three Routes** (31494) — animated enforcer of all three route systems simultaneously, martial elite. Level 78.
- **Boss: Arbiter of the Conversion** (31498) — the Shoreward Conversion itself given hostile sentience, final encounter. Level 80. Casts from both inland (earth, fire) and maritime (water, air) traditions.

---

## Object Design Plan

### Object Vnum Plan

Use object vnums sequentially within `31100-31499`. Target **80-100 objects**:

| Category | Count | Notes |
|---|---:|---|
| Charter relics | 8-10 | Survey stones, oath tablets, measure rods, transit seals |
| Toll-era equipment | 8-10 | Bronze scales, assessor tools, conversion table fragments |
| Guide craft | 6-8 | Glasswalk shoes, route-song scrolls, glass-reading lenses |
| Synod contraband | 6-8 | Cult ritual materials, smuggled relic components |
| Environmental drops | 8-10 | Salt crystals, glass shards, crab chitin, scorpion stingers |
| Cairn salvage | 6-8 | Bronze fittings, tablet fragments, survey instruments |
| Faction gear | 10-12 | Warden patrol equipment, Outrider ledgers, Scavenger tools |
| Keys | 4-6 | For locked doors in Districts E, F, H |
| Boss signature drops | 6-8 | Unique boss loot with `ITEM_BOSS` flag |
| Weapons | 10-12 | Themed melee and caster weapons across level range |
| Armor | 10-12 | Themed protective gear across wear slots |
| Consumables | 4-6 | Food, potions, light sources for crossing |

### Itemization Intent

- Blend oasis-administrative motifs (charter stones, water measures, witness seals) with maritime conversion motifs (wax stamps, draft tables, storm charts).
- Environmental gear reflects terrain: salt-crusted boots for Mirror Flats, chitin-soled sandals for Glasswind, waxed cloaks for Tidemouth.
- Boss drops should visibly bridge the inland-maritime divide: weapons inscribed with both desert glyph and tide notation, armor with salt-crust and barnacle motifs.
- Extra descriptions on key objects should contain inscription text in the three voice styles (Survey-Administrative, Legal-Conversion, Guide-Oral).
- Keyed doors in Districts E (Measure's Rest sealed archive), F (sealed relic vault), and H (undercroft descent) each have explicit key objects.

---

## Progression and Difficulty Curve

- **31100-31149** (District A): onboarding, 55-58. Safe hub with friendly/neutral NPCs and light patrols.
- **31150-31199** (District B): Mirror Flats crossing, 55-62. Environmental hazard + fauna.
- **31200-31249** (District C): Heavy Road crossing, 57-64. Moderate fauna + faction skirmishes.
- **31250-31299** (District D): Glasswind Belts, 60-70. Exploration hazard + Synod + glass fauna + mini-boss.
- **31300-31349** (District E): Cairn Line Ruins, 62-72. Faction density + vertical exploration + mini-boss.
- **31350-31399** (District F): Sealed Route, 65-75. Dangerous relic hazards + Synod operations + mini-boss.
- **31400-31449** (District G): Tidemouth Approach, 60-68. Open terrain + coastal fauna + jurisdiction conflict.
- **31450-31499** (District H): Shoreward Threshold, 70-80. Elite + boss gauntlet.

---

## Quest Plan (Quest Files 81-95)

All quests use offerer vnums from within the area (`31100-31499`) unless specified as external quest givers. Quest target vnums reference mobs within the area.

### Cartography Quest Chain (81-82)

| File | Static ID | Title | Prereq | Type | Description |
|---|---:|---|---:|---:|---|
| `81.prop` | 80 | Saltglass Reach cartography survey: Mirror Flats | -1 | 1 (kill-variety) | Survey the Mirror Flats by eliminating threats at key waypoints. Kill one of each: salt cricket swarm leader, cairn kite alpha, corrupted Warden toll-thief. Offerer: Reach Warden cartographer in District A. Reward: 4000 gold, 4 qp. Level 55-70. |
| `82.prop` | 81 | Saltglass Reach cartography survey: Glasswind to Tidemouth | 80 | 1 (kill-variety) | Complete the Reach survey by clearing observation points from the Glasswind Belts to the Tidemouth Dunes. Kill one of each: glass scorpion matriarch, glassworm tunnel sentinel, tidemouth crab broodmother. Offerer: same cartographer. Reward: 6000 gold, 5 qp. Level 60-75. |

### Faction Conflict Quests (83-87)

| File | Static ID | Title | Prereq | Type | Description |
|---|---:|---|---:|---:|---|
| `83.prop` | 82 | Reach Warden toll restoration | -1 | 3 (kill-count) | Eliminate corrupted Warden toll-enforcers extorting caravans along the Swift Line. Kill 8 corrupted Warden toll-thieves. Offerer: Warden Commander at Measure's Rest. Reward: 3500 gold, 4 qp. Level 55-70. |
| `84.prop` | 83 | Red Sand Outrider interdiction | -1 | 1 (kill-variety) | Disrupt Red Sand Accounts operations by eliminating their field agents. Kill one of each: Red Sand debt collector, Red Sand assessor, Red Sand enforcer. Offerer: Reach Warden patrol leader. Reward: 4500 gold, 5 qp. Level 60-75. |
| `85.prop` | 84 | Synod whisper cell disruption | -1 | 1 (kill-variety) | Intercept Synod smuggling operations in the Glasswind Belts. Kill one of each: Synod courier, Synod ritual keeper, Synod lookout. Offerer: Reach Warden at Glasswind approach. Reward: 4000 gold, 4 qp. Level 58-72. |
| `86.prop` | 85 | Cairn scavenger expulsion | -1 | 3 (kill-count) | Clear Cairn Scavengers stripping Measure's Rest of irreplaceable tablets. Kill 10 cairn scavengers. Offerer: Warden Commander. Reward: 3800 gold, 4 qp. Level 60-72. |
| `87.prop` | 86 | Sealed Route quarantine enforcement | 84 | 1 (kill-variety) | Re-establish quarantine along the abandoned Sealed Route by eliminating Synod occupation forces. Kill one of each: Synod Whisper Cell commandant, quarantine breach horror, Synod tunnel sentry. Offerer: Reach Warden officer. Reward: 5500 gold, 5 qp. Level 65-78. |

### Boss Quests (88-92)

| File | Static ID | Title | Prereq | Type | Description |
|---|---:|---|---:|---:|---|
| `88.prop` | 87 | Glassworm burrower queen extermination | -1 | 3 (kill-count) | The glassworm burrower queen threatens route stability across the Belts. Destroy her. Kill 1 glassworm burrower queen. Offerer: Glasswalk Guide elder. Reward: 5000 gold, 5 qp. Level 62-75. |
| `89.prop` | 88 | Whisper Cell commandant elimination | 84 | 3 (kill-count) | Track and eliminate the Synod Whisper Cell commandant directing smuggling operations from the Sealed Route. Kill 1 Whisper Cell commandant. Offerer: Warden Commander. Reward: 6000 gold, 6 qp. Level 68-78. |
| `90.prop` | 89 | Shoreward Revenant banishment | -1 | 3 (kill-count) | The Shoreward Revenant — a dead Assessor who refuses to yield jurisdiction — haunts the Shoreward Threshold. Destroy it. Kill 1 Shoreward Revenant. Offerer: Reach Warden at Tidemouth. Reward: 7000 gold, 6 qp. Level 70-80. |
| `91.prop` | 90 | Toll-Marshal of the Three Routes: writ of dissolution | 89 | 3 (kill-count) | The Toll-Marshal enforces all three routes simultaneously, blocking the Shoreward approach. Execute the writ of dissolution. Kill 1 Toll-Marshal of the Three Routes. Offerer: same. Reward: 8500 gold, 7 qp. Level 75-80. |
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
| Reach Warden Cartographer | ~31124 | District A: Transit Seal Office |
| Warden Commander | ~31328 | District E: Measure's Rest |
| Reach Warden Patrol Leader | ~31108 | District A: Warden Barracks |
| Glasswalk Guide Elder | ~31258 | District D: Guide Waystation |
| Kowloon Scholar | ~31332 | District E: Kowloon Scholar's Camp |
| Tidemouth Warden Post Commander | ~31435 | District G: Tidemouth Warden Post |
| Reach Warden Officer (Sealed Route) | ~31350 | District F: Sealed Route approach |

---

## Reset and Door Plan

### Door Plan

| Location | Door Description | Key Object | Reset State |
|---|---|---|---|
| Measure's Rest: Sealed Archive (31326) | Heavy bronze door with Keeper-era seal | Warden archive key | Locked (state 2) |
| Sealed Relic Vault (31373) | Iron-bound quarantine door with containment wards | Quarantine seal key | Locked (state 2) |
| Undercroft Descent (31485) | Basalt hatch with dual-jurisdiction inscription | Shoreward threshold key | Locked (state 2) |

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
