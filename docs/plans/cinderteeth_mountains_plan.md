# Area Plan: The Cinderteeth Mountains

## Overview

**Area Name:** `@@RThe @@yCinderteeth @@rMountains@@N`
**File Name:** `cinderteeth_mountains.are`
**Keyword:** `cinderteeth mountains volcanic caldera ventspeaker`
**Area Number:** `N 88`
**Level Range:** `I 140 150` (player level gate; mobs 145–170)
**Vnum Range:** **6124-6723** (`V 6124 6723`)
**Room Count Target:** **600/600 rooms used**
**Primary Design Goal:** Build a massive volcanic arc spanning six distinct subregions — from the contested Blackstep Footholds through the resonance-fractured Bellspine, the obsidian Glasswash Fans, the sacred Western Fold, and up to the lethal Ember Crown caldera zone. The layout is deliberately non-linear and asymmetric, reflecting the mountain's actual geology: a discontinuous volcanic belt with saddle passes, deep structural folds, resonance caves, and open scree fields that resist any grid interpretation. Mob difficulty escalates sharply with altitude and depth. The mountain selects survivors.

**Connections (Planned — do not author until target areas exist):**
- **Forest of Confusion** (3 southern exits from Blackstep Footholds border): 6124 `south` → confusn 2519; 6153 `south` → confusn 2521; 6168 `south` → confusn 2524. All bidirectional.
- **Sunken Sanctum** (Western Fold deep descent): 6552 `down` → sunken_sanctum 2266; bidirectional (2266 `up` → 6552).
- **Great Northern Forest** (3 eastern exits from Ember Crown Approaches border): 6627 `east` → gnf 4179; 6633 `east` → gnf 4199; 6639 `east` → gnf 4219. All bidirectional.

---

## Color Theme (Multi-Color, Lore-Matched)

- `@@R` **Caldera Red**: active vent glow, lava seep warnings, boss chambers, lethal thermal zones.
- `@@y` **Ash Gold**: chalk-white saddle ash, sulfur crystal formations, ancient name-stones, Keeper survey remnants.
- `@@r` **Cinder Rust**: obsidian basalt, scorched rock surfaces, volcanic aggregate, Glasswash Fan coloring.
- `@@g` **Cinderpine Grey-Green**: pine groves, lower slopes, thermal moss, old fire-pit stone.
- `@@G` **Vent Green**: toxic gas seep indicators, sulfur moss, Yellow Garden crystal formations.
- `@@p` **Ash Cult Purple**: Ash Cult ritual text, speaking vent pronouncements, Ventspeaker name-stone carvings.
- `@@W` **Summit White**: chalk-ash drifts on saddle surfaces, pumice fields, high-altitude snow remnants.
- `@@l` **Crystal Blue**: Conclave resonance anchors, Bellspine crystal fragments, crystal conduit residue.
- `@@N` reset after each themed segment.

Theme restrictions (spec-aligned):
- No `@@k` as thematic foreground.
- No flashing code `@@f`.
- No background codes `@@0`-`@@7`.

---

## Core Narrative Direction

The Cinderteeth Mountains are a **civilizational stress-test given physical form** — a volcanic arc that has outlasted every institution that tried to name, govern, or exploit it. Players traverse six distinct subregions in a roughly ascending route from the foothill governance ruins to the lethal caldera crown. Each zone carries evidence of a different institutional failure: bandit occupation where caravan law collapsed, route house ruins where commerce could not sustain upkeep, resonance-haunted passes where the Spirebound Conclave's monitoring instruments still hum without operators, a sacred Fold where the Ash Cult tends vents that once belonged to the Ventspeakers, and a caldera summit where no institution has ever held authority longer than the mountain tolerated it.

The zone design principle: **the deeper or higher you go, the more the mountain has won**. The Blackstep Footholds still bear civic infrastructure remnants. The Chalkwind Saddles still have route houses, crumbling and contested. The Glasswash Fans still have extraction infrastructure — but the workers are gone and something has taken their place. The Bellspine resonance passes require silence and care. The Western Fold is held by the Ash Cult, who believe they have the mountain's sanction. The Ember Crown is held by nothing recognizable as civilization at all — only the volcanic intelligence of the mountain's native ecology at its most extreme.

**Player entry:** The area is entered from the Blackstep Threshold at 6124 (planned connection from Roc Road). Players working through the content move through zones of escalating difficulty as they gain altitude and depth. The area has no single "correct" route — there are parallel paths through the Chalkwind and the Glasswash Fans, and the Bellspine has three distinct hollow passes that can be threaded in different order.

---

## Cross-Area Lore Anchors

The Cinderteeth Mountains occupy the **far northwest corner** of the accessible continent. The world map lore is explicit: the range sits above the Forest of Confusion / Kiess axis, with the Great Northern Forest to the northeast and Kowloon's delta to the north. The mountain is connected to the rest of the world primarily through its effects — ash, volcanic exports, institutional refugees — not through close geographic proximity. The eastern desert corridor (Scorching Sands, Oases, Pyramids) is on the opposite end of the continent and has **no geographic connection** to the Cinderteeth.

### Roc Road (Trade Route Context — No Direct Room Connection)

- Roc Road runs east-west from Kiess to Midgaard — it does not physically reach the Cinderteeth. The lore references to "Roc Road caravans passing through the Blackstep Footholds" mean merchants who travel the Roc Road trade network made a secondary northward journey through the Forest of Confusion to reach the Cinderteeth and extract materials. The trade route is: Cinderteeth Blackstep zone → south through Forest of Confusion → Kiess → east along Roc Road → Midgaard.
- No direct room exit to Roc Road is planned or appropriate. The Ashfang Remnant's intelligence access to route-house scheduling records (suspected in the lore) traces through the Forest of Confusion connection, not a direct Roc Road link.
- Roc Road institutional references appear as NPC lore text throughout the area (route house records, trade schedules, warden commission documents).

### Sunken Sanctum (Western Fold Deep Link — Planned)

- The Sunken Sanctum lies beneath the Western Fold. The Cinderteeth lore explicitly places the Sanctum as a "deep fold in the western spur of the Cinderteeth Mountains." The Sanctum Scar surface collapse zone (Zone 6, room 6552) is the surface expression of the Sanctum's upper structural failure.
- Ash Cult cells in the Western Fold maintain doctrinal correspondence with factional remnants in the Sanctum's accessible peripheral chambers.
- **Planned connection** (bidirectional, one link):

| Cinderteeth | Direction | Sunken Sanctum | Notes |
|---|---|---|---|
| **6552** Sanctum Scar — Collapse Edge | `down` | **2266** The Collapsed Entrance | Surface descent to Sanctum upper approach |
| **2266** The Collapsed Entrance | `up` | **6552** Sanctum Scar — Collapse Edge | Reverse: ascent back to surface |

`sunken_sanctum_plan.md` room 2266 is already designated for this connection ("External room: TBD — connection to a Cinderteeth Mountains approach room"); the concrete destination is now **6552**. That plan must be updated to replace the TBD with `up` → 6552 before either exit is authored.

### Forest of Confusion (Primary Planned Connection — Southern Blackstep, 3 border links)

- The world map schematic places the Forest of Confusion directly south of the Cinderteeth, as the geographic intermediary between the mountain range and Kiess. This is the **primary connection to the outside world**: the only realistic path from the Cinderteeth to lowland civilization runs south through the Forest of Confusion's northern boundary.
- The Cinderteeth lore references "forests south of the Cinderteeth" as the location of the Prism Thesis faction's crystal-resonance field trials — those forests are the Forest of Confusion / Verdant Depths zone. The Forest of Confusion's northern boundary is already contested by the same institutional residue (Conclave crystal experiments) that defines the Western Fold.
- The Ashfang Remnant operates at this transition point precisely because Forest of Confusion northern territory is poorly governed by any lowland authority, providing cover for bandit operations.
- **Three planned border connections** (all bidirectional; both sides must be authored before exits can be live):

| Cinderteeth (south) | confusn (north) | Character |
|---|---|---|
| **6124** Threshold Post Road | **2519** (y=9, x=0) Northwestern Treeline Approach | Main road — primary player entry/exit |
| **6153** Old Caravan Ghost Road | **2521** (y=9, x=2) Northern Forest Caravan Track | Historic caravan route, predates route houses |
| **6168** Southern Foothills Descent | **2524** (y=9, x=5) Northern Scree Drainage Path | Rough terrain third path |

`confusn` rooms 2519, 2521, and 2524 are in the northern boundary row (y=9) of `confusn.are`. All three must be authored in `confusn.are` with `north` exits pointing back to the Cinderteeth rooms above. These rooms do not yet exist in `confusn.are`.

### Withered Depths (Lore Connection — via Forest of Confusion)

- The Prism Thesis faction's forest field trials — which produced the Withered Depths blight — used techniques developed in the Cinderteeth's Western Fold. Resonance crystal implantation in old-growth root junction nodes was tested in the forests south of the Cinderteeth (the Forest of Confusion interior) before the Conclave's collapse; the abandoned crystals, unmonitored for centuries, created the blight. The Cinderteeth is the institutional origin point of the Withered Depths catastrophe.
- Lore connection (no direct room exit planned): Withered Depths scholars appear as NPC reference in Bellspine zone text; the scholarly confusion over thermophilic fungi in the Western Fold vs. Withered Depths' corrupted networks is referenced in Zone 6 descriptions.

### Verdant Depths (Lore Connection — via Forest of Confusion)

- The Verdant Depths occupy the deep N/NW interior of the Forest of Confusion — the ancient living woodland whose distributed ecological intelligence predates all human cities. The Cinderteeth's volcanic chemistry has influenced the Verdant Depths through ash and sulfur deposition into northern forest water systems over geological time. No direct room connection is planned, but Verdant Depths scholars reference the Cinderteeth in NPC lore text.

### Great Northern Forest (Planned Connection — Northeastern High-Altitude Border, 3 links)

- The Great Northern Forest is northeast of the Cinderteeth; its northwestern high-latitude region (Northern Crown, GNF rooms 4179-4219 at x=0) directly borders the Cinderteeth's eastern high-altitude approaches (Zone 7, Ember Crown Approaches). The GNF's streams were poisoned by volcanic particulates during the Ashfall Monsoon — the Cinderteeth's influence on the GNF is primarily atmospheric and hydrological. The border zone is traversable at high altitude: exposed ridgeline traversals from the Ember Crown Approaches descend eastward into the GNF's Northwestern Crown.
- GNF ranger-writ expeditions from this border appear as NPC lore text throughout Zone 7.
- **Three planned border connections** (all bidirectional; both sides must be authored before exits can be live). GNF western-edge rooms use the `confusn`-style coordinate formula: vnum = 3739 + (y × 20) + x; x=0 is the westernmost column:

| Cinderteeth (east) | GNF (west) | Altitude |
|---|---|---|
| **6627** Northeastern Ridge Approach — Lower | **4179** (y=22, x=0) Northwestern Crown Ridgeline South | Lower altitude ridgeline |
| **6633** Northeastern Ridge Approach — Mid | **4199** (y=23, x=0) Northwestern Crown Ridgeline Mid | Mid-altitude ridgeline |
| **6639** Northeastern Ridge Approach — Upper | **4219** (y=24, x=0) Northwestern Crown — Westernmost Point | Highest altitude, northernmost link |

GNF rooms 4179, 4199, and 4219 exist in `great_northern_forest_plan.md`'s Northern Crown region (4219-4238). All three must be updated in `great_northern_forest_plan.md` to add `west` exits pointing back to the Cinderteeth rooms above.

### Kiess (Institutional Lore — via Roc Road and Forest of Confusion)

- Kiess was explicitly founded with an anti-Conclave institutional position. Kiess's Roc Road salvage economy drove the most organized pre-modern market for Cinderteeth materials. Kiess expedition permits into the Western Fold are referenced in NPC lore text. The Kiess Temple of Concord is the Ash Cult's primary institutional enemy. Kiess is accessible from the Cinderteeth via Roc Road (eastward from the foothills, westward from Kiess).

### Midgaard (Institutional Lore — via Roc Road)

- Midgaard's Violet Compact registry protocols required origin documentation for Cinderteeth materials. Salvage Assessment House remnants in the Blackstep zone reference Compact certification. Midgaard's Granite Arcade ranger-writ system issued Western Fold expedition permits.

---

## `#AREA` Header Requirements (strict)

```
#AREA
@@RThe @@yCinderteeth @@rMountains@@N~
Q 16
K cinderteeth mountains volcanic caldera ventspeaker~
L @@R{140 150}@@N~
N 88
I 140 150
V 6124 6723
F 12
O Virant~
U @@rThe mountain exhales ash and old judgment across the @@yCinderteeth@@r.@@N~
```

---

## Global Spec Compliance Rules

Implementation must follow `docs/area_file_spec.md` requirements, including:

- All strings `~`-terminated.
- No back-to-back newline runs in strings.
- No vnums in any in-world text.
- Room descriptions: minimum 3 sentences; key/major rooms minimum 5.
- Unique room descriptions throughout.
- Mobile `long_descr`: one text line only, then `~` line.
- Mobile `description`: exactly one trailing newline before `~`.
- Named exits must be text-anchored in room description or extra description.
- Exits bi-directional except in maze-flagged rooms (Bellspine).
- All mobs flagged `stay_area`; bosses flagged `sentinel`, `boss`, placed in `no_mob` rooms.
- Strong non-boss mobs (solo encounters) flagged `solo`.
- Sub-regions enclosed by `no_mob` boundary rooms or `sentinel` mobs.
- Sequential vnum allocation, no gaps.
- All objects include `ITEM_TAKE`. Loot-table items include `ITEM_LOOT`. Boss drops include `ITEM_BOSS`.
- Object names unique within the area.
- Boss mob stat line: `0 -100 10 10` through `0 -100 12 12` depending on tier.
- Normal mobs at level 145-155: `0 0 5 5` through `0 -50 7 7`.
- Normal mobs at level 156-170: `0 -50 7 7` through `0 -100 10 10`.

---

## Room Topology (All 600 Rooms Used)

The area is divided into **8 zones** arranged in a roughly ascending path from the lowland foothills to the summit caldera. The topology is deliberately asymmetric — the mountain's geology does not produce grids. Passes fork and merge. Subregions branch sideways. Vertical movement (up/down) is used throughout the upper zones. Certain routes are dead ends that reward exploration. The three main "trunk" routes (foothill road, saddle passes, glasswash fans) merge into a single convergence zone at the Bellspine before the path narrows toward the Western Fold and Ember Crown.

### Zone Overview Schematic

```
                   LOWLANDS (Roc Road / Scorching Sands)
                               |
                    [ZONE 1: Blackstep Threshold]
                        6124-6183  (60 rooms)
                     Foothills | Assessment House | Ashfang
                         /               \
          [ZONE 2: Chalkwind         [ZONE 3: Glasswash
           Saddles]                   Fans]
           6184-6263 (80r)            6264-6343 (80r)
           High/Blind/Long            Obsidian / Sulfur Fan /
           Saddle Routes              Dead Pools / Yellow Garden
               \                         /
                \                       /
              [ZONE 4: BELLSPINE]
               6344-6453 (110 rooms)
           Three Hollow Passes | Resonance Maze |
           Crystal Anchors | Null Cairn Field
                        |
              [ZONE 5: Western Fold Approach]
               6454-6533 (80 rooms)
           Seven Ash Arches | Red Sister Cisterns |
           Ventspeaker Sacred Sites | Ash Cult Approaches
                        |
              [ZONE 6: Western Fold / Sanctum Scar]
               6534-6593 (60 rooms)
           Speaking Vent | Tiered Fire-Pits |
           Sanctum Scar Surface | Ash Cult Stronghold
                        |
              [ZONE 7: Ember Crown Approaches]
               6594-6653 (60 rooms)
           Upper Scree Fields | Glasswalk Zones |
           Widow's Spire | Glow-Hawk Nesting Cliffs
                        |
              [ZONE 8: EMBER CROWN]
               6654-6723 (70 rooms)
           Firsttooth Caldera | Secondtooth Ridge |
           Oracle Vent | Summit — Final Boss
```

---

### Zone 1: Blackstep Threshold (6124-6183, 60 rooms)

**Theme:** The fractured lower foothills where lowland civic jurisdiction ends and mountain route-house authority begins — and where the gap between them has been exploited for generations by the Ashfang Remnant. Black basalt "foothold" step-formations define the terrain. Assessment House ruins, crumbling bandit clearance posts, and the Threshold Post administrative ruin establish that governance once existed here and failed. The Ashfang Remnant has occupied a fortified canyon stronghold. The zone fans out from the entry road into three main approach corridors (Chalkwind path, Glasswash path, Bellspine direct climb) and a side excursion into Ashfang territory.

**Sector types used:** hills (lower approaches), mountain (basalt ridges and passes), field (assessment ruins), inside (interiors).

**Internal Shape:** Entry road → Threshold Post hub (multi-exit) → three trunks (Chalkwind north, Glasswash east, central climb) plus one side branch (Ashfang Remnant territory to southwest). Ashfang branch terminates at a locked stronghold containing the zone mini-boss. Assessment House is an interior side-room cluster off the hub.

**Mob difficulty:** 145-148. Mini-boss: Ashfang Remnant Warlord (148).

Key rooms:
- 6124 **Threshold Post Road** — entry from lowlands; 5+ sentences; the first named junction in the mountain approaches; **planned `south` exit to `confusn` 2519** (Northwestern Treeline Approach, primary connection point).
- 6125 **Threshold Post Ruins** — central hub, collapsed administrative building, 5+ sentences, 5+ exits radiating out; warden commission debris; `no_mob` boundary room.
- 6126 **Salvage Assessment House Forecourt** — ruined exterior of the Kiess-funded Assessment House; contested authority notices still nailed to a crumbling post.
- 6127 **Salvage Assessment House Interior** — inside sector; archive shelves with decomposed ledgers; extra descriptions for Compact certification documents.
- 6128 **Assessment House Vault Room** — inside sector; locked door; empty vault with old relic-classification plaques.
- 6129 **Bandit Clearance Post Alpha** — fortified shelter, crumbling; bloodstained walls, old warden rotation logs; sentient remains of patrol infrastructure.
- 6130 **Post Alpha Inner Room** — inside sector; weapon racks stripped bare; extra description: old warden commission charter fragment.
- 6131 **Blackstep Lower Road — North Fork** — transition junction; basalt footholds begin; connects north toward Chalkwind approach.
- 6132 **Blackstep Lower Road — East Fork** — transition junction; lower obsidian outcrops; connects east toward Glasswash approach.
- 6133 **Black Basalt Ridge** — volcanic basalt step-terrain; active Ashrunner warrens visible; sight lines across the lower foothills.
- 6134 **Cinderpine Copse** — dark-barked stunted conifers clustered along old lava flow boundary; sulfur-resin smell; extra description on resin harvest marks.
- 6135 **Ashrunner Warren Flats** — flat ash-deposit terrain honeycombed with Ashrunner burrow openings; unstable surface crust.
- 6136 **Scorched Pine Ridge** — charred cinderpine remnants from old lahar edge; heat-bleached white bark; overlooks Assessment House from above.
- 6137 **Thermal Seep Ground** — small warm-water seep staining the basalt orange with iron mineral deposit; sulfur moss indicator growth.
- 6138 **Volcanic Basalt Stairway** — natural "stair" of step-fractured basalt; primary climb route toward upper approaches.
- 6139 **Old Survey Marker Station** — collapsed Keeper survey post; a stone baseline marker still stands; extra description on condition-window notation.
- 6140 **Foothills Canyon Mouth** — canyon entrance guarded by Ashfang Remnant sentries; narrowing walls of black basalt.
- 6141 **Canyon West Wall** — sheer basalt wall, old carved footholds, climbing hazard text.
- 6142 **Canyon East Wall** — opposite face; Ashfang graffiti and gang markings cover the lower section.
- 6143 **Canyon Floor — Patrol Zone** — wide floor, active Ashfang patrol circuit; aggressive mob territory.
- 6144 **Ashfang Remnant Outer Camp** — tent encampment, cooking fires, supply caches; main bandit population density; aggressive mobs.
- 6145 **Ashfang Sentry Tower** — inside sector; elevated post at canyon bend; Ashfang snipers watch the entry approach.
- 6146 **Ashfang Tollbridge** — stone arch bridge over a dry lava channel; Ashfang mobs control passage; extra description on carved tollmark demands.
- 6147 **Ashfang Remnant Stronghold Gate** — `no_mob` room; heavy locked basalt door (key dropped by Ashfang Sentry Captain); forced entry narrative text.
- 6148 **Ashfang Stronghold Interior — Armory** — inside sector; `no_mob`; weapon stores, loot caches.
- 6149 **Ashfang Stronghold Interior — Barracks** — inside sector; `no_mob`; sleeping area turned war-room.
- 6150 **Ashfang Warlord's Command Chamber** — inside sector; `no_mob`; **MINI-BOSS ROOM** for Ashfang Remnant Warlord; carved territorial maps cover the walls.
- 6151 **Bandit Clearance Post Beta** — second ruined clearance post, this one partially Ashfang-occupied; evidence of dual use.
- 6152 **Post Beta Passage** — inside sector; connecting corridor.
- 6153 **Old Caravan Ghost Road** — ancient path worn into the basalt; faint wheel rut shadows; historically pre-dates the Keeper survey era; **planned `south` exit to `confusn` 2521** (Northern Forest Caravan Track, secondary connection).
- 6154 **Lower Obsidian Deposit** — natural obsidian outcrop, partially quarried; tool-quality and aggregate-quality fragments scattered.
- 6155 **Mountain Bear Scrublands** — `no_mob` boundary marker room; scraggly shrubs at volcanic soil edge; bear territory begins.
- 6156 **Bear Patrol Territory** — open slope, mountain bear encounters; no_mob boundary prevents mob bleed.
- 6157 **Warden Commission Post** — small stone shelter, warden rotation stop; old administrative notices; extra description: route house charter compact fragment.
- 6158 **Commission Post Archive Room** — inside sector; sealed records room, most records destroyed; one intact ledger with Ashfall-era entry survives.
- 6159 **Threshold Pass Narrows** — the choke point between foothill governance zone and the true mountain approaches; wind-scoured basalt walls.
- 6160 **North Foothill Slope** — wide slope ascending toward Chalkwind corridor; ashgrass patches, stable terrain.
- 6161 **East Foothill Slope** — sloping terrain toward Glasswash fan approach; obsidian fragment fields.
- 6162 **Central Ascent Path** — steep direct climb path toward Bellspine; used by those who know the mountain well.
- 6163 **Sulfurous Vent Pocket** — small vent gas seep zone; colorless gas, faint sweet-sulfur odor; extra description warning.
- 6168 **Southern Foothills Descent** — a rougher southwest-facing slope path; drainage channel cuts through exposed basalt; cinderpine scrub on the lower faces; **planned `south` exit to `confusn` 2524** (Northern Scree Drainage Path, third connection).
- 6164-6183 (remaining): Connecting passages, foothill traversal rooms, slope approaches, ashrunner territory, thermal spring runoff channels, abandoned caravan debris fields, cinderpine groves, and basalt stair segments filling out the zone. These rooms connect the five main clusters (Threshold hub, Assessment House, Ashfang branch, northern trunk, eastern trunk) into a coherent traversal network with no dead-end rooms that lack narrative purpose.

---

### Zone 2: Chalkwind Saddles (6184-6263, 80 rooms)

**Theme:** The main commercial transit zone of the Cinderteeth — three saddle passes through the mid-ridge, their route houses in varying states of ruin and occupation. Chalk-white ash accumulates in the saddle depressions. Mountain bears are active in low-game years. The route houses (Ironpost Lodge, Clearwater Camp, Muster Yard, Black Thorn Camp) each represent a different relationship to institutional survival: Ironpost has centuries of records, Clearwater's spring is ruined but the camp persists, the Muster Yard holds contested emergency-law documents, Black Thorn is the informal information exchange that outlived every official body.

**Sector types used:** mountain (saddles and passes), hills (lower saddle approaches), inside (route house interiors).

**Internal Shape:** A branching pass system with three parallel trunk routes (High Saddle, Blind Saddle, Long Saddle) connected by cross-links at their approaches and exits. The route houses cluster at approach junctions. The three routes converge at a single eastward exit toward the Bellspine / Glasswash junction. The layout is a loose Y-shape with three prongs to the west (each connecting back to Zone 1) and one trunk heading east toward Zone 3/4.

**Mob difficulty:** 148-152. Mini-boss: Ironpost Warden-Commander (152).

Key rooms:
- 6184 **Chalkwind Approach Road** — entry from Zone 1 north fork; chalk-white ash begins dusting the basalt; route markers visible.
- 6185 **Route House Junction — The Threefork** — hub room where the three saddle approaches diverge; old warden traffic-counting stone; 5+ sentences; 5+ exits.
- 6186 **Ironpost Lodge Exterior** — historic route house, largest in the Saddles; exterior shows multi-era construction; iron post signs still bear route schedules.
- 6187 **Ironpost Lodge Common Room** — inside sector; great hall with route maps pinned to the walls; active mob presence (route house survivors/occupants).
- 6188 **Ironpost Lodge Archive Room** — inside sector; `no_mob`; floor-to-ceiling route log shelves; extra descriptions for condition-window notation entries spanning four centuries; sealed archive door (key from Warden-Commander).
- 6189 **Ironpost Lodge Archive Sealed Wing** — inside sector; `no_mob`; most sensitive records; **MINI-BOSS ROOM** for Ironpost Warden-Commander (undead warden defending the archive).
- 6190 **Ironpost Lodge Upper Floor** — inside sector; sleeping quarters now occupied by mountain creatures.
- 6191 **High Saddle Departure** — the road heading toward the primary summer saddle; wide path, best sight lines.
- 6192 **High Saddle Lower Approach** — chalk-ash deepens; good visibility in clear weather; hazard: scree-shelf collapse risk.
- 6193 **High Saddle Midpoint** — widest chalk-ash accumulation zone; landmark: double-peaked ridgeline visible; extra description on condition-window dating markers.
- 6194 **High Saddle Summit** — highest point of the High Saddle route; ash-gray panorama; glow-hawk visible in upper thermals.
- 6195 **Clearwater Camp** — former summit waystation; "clearwater" spring now sulfur-contaminated; water supply hauled from downslope; sulfur moss indicator growth at spring; extra description on contamination history.
- 6196 **Clearwater Camp Interior** — inside sector; small shelter, camping evidence, old ration-law notices.
- 6197 **High Saddle Eastern Descent** — descending path toward Bellspine approach; chalk ash thins; obsidian fragments appear.
- 6198 **Chalkfall Gully** — narrow channel where chalk ash accumulates in seasonal drifts; partially blocked in heavy ash years.
- 6199 **Blind Saddle Departure** — turn toward the shelter-corridor winter route; ridgeline blocks sight lines almost immediately.
- 6200 **Blind Saddle Approach — Sheltered Corridor** — walls close in; astronomical navigation impossible; route-marker dependent.
- 6201 **Blind Saddle Midpoint** — deepest shelter section; low wind but heavy chalk ash; silent except for low mountain resonance.
- 6202 **Blind Saddle Eastern Opening** — saddle opens toward eastern approaches; sight lines restored; mountain echo from the Bellspine audible.
- 6203 **The Muster Yard** — former emergency requisition and supply depot; platform structures still standing; contested administrative authority.
- 6204 **Muster Yard Administrative Hall** — inside sector; emergency powers records in locked cabinet; extra description on Ashfall-era requisition authority dispute.
- 6205 **Muster Yard Storage Halls** — inside sector; stripped-bare supply depot; evidence of multiple occupation cycles.
- 6206 **Long Saddle Departure** — the oldest route, now partially undermined; wide path shows subsidence cracks.
- 6207 **Long Saddle Subsidence Zone Alpha** — cracked ground, hollow resonance underfoot; maze-flagged short section (ROOM_MAZE); subsidence warning text.
- 6208 **Long Saddle Midpoint** — stabilized section between subsidence zones; old road surface visible through ash layer.
- 6209 **Long Saddle Subsidence Zone Beta** — second hollow section; worst structural compromise on the route; narrow passable corridor.
- 6210 **Long Saddle Eastern Exit** — ancient road termination; the oldest survey markings visible; extra description on Keeper era baseline tablet.
- 6211 **Black Thorn Camp** — the informal information exchange; small unlicensed rest stop; bulletin board covered in handwritten convoy condition reports; no official administration.
- 6212 **Black Thorn Camp Interior** — inside sector; fire pit, sleeping pallets, contested route intelligence on walls.
- 6213 **Chalkwind Convergence Point** — where all three saddle routes rejoin on the eastern side; major landmark; chalk ash gives way to darker volcanic aggregate; 5+ sentences.
- 6214 **Mountain Bear Rock Ledge** — exposed ledge terrain, mountain bear territory; extra description on mineral-lick behavior.
- 6215 **Summit Roc Scree** — upper scree zone adjacent to saddle tops; glow-hawk seen on thermals above; unstable footing.
- 6216 **Chalk Ash Dune** — deep chalk-ash dune formed in saddle depression hollow; navigation requires care; partially maze-adjacent.
- 6217 **Saddle Wind Corridor** — exposed ridge section between High and Blind Saddle routes; cross-wind hazard text; panoramic views.
- 6218 **Old Tariff Post** — ruined toll station at saddle approach; carved tariff schedule still legible; warden commission connection.
- 6219 **Hushed Mile Pass** — a short section where the Ridgekin-origin toponym "Hushed Mile" survives in warden oral tradition; extra description on Bellspine acoustic character.
- 6220 **Chalkwind Northern Traverse** — pass connecting High Saddle approach to the Bellspine northern entry.
- 6221 **Chalkwind Southern Traverse** — pass connecting Long Saddle approach toward Glasswash Fan northern approach.
- 6222-6263: Connecting passages, saddle slope approaches, chalk-ash transition corridors, cross-links between the three saddle routes, mountain bear patrol zones, secondary route house ruins, scree-shelf hazard rooms, and warden patrol waypoints completing the full Chalkwind network.

---

### Zone 3: Glasswash Fans (6264-6343, 80 rooms)

**Theme:** The hardened obsidian-glass fan plains descending from the upper range toward drainage basins. Three fan zones of distinct character: North Fan (quarried, extraction infrastructure ruins), Middle Fan (primary transit surface, sink crust risk), Sulfur Fan with the Yellow Garden (crystal formations, Dead Pools, toxic chemistry). The Glasswash Fans are commercially the most important zone in the mountain but the infrastructure has been abandoned to creatures and hazard. The area is rich with loot potential and dangerous terrain. The "Dead Pools" are a genuine toxic hazard with narrative weight — the Ashblind Convoy Loss took place here.

**Sector types used:** mountain (upper approaches), field (fan surfaces), hills (fan edges), inside (extraction structure ruins).

**Internal Shape:** Entry from Zone 1 east fork leads to a North Fan / Middle Fan / Sulfur Fan tripartite structure. The North Fan is a quarry complex with interior rooms. The Middle Fan is the crossing route toward the Bellspine approach. The Sulfur Fan is a branching side excursion containing the Yellow Garden and Dead Pools sub-areas. The zone has one exit northeast toward the Bellspine convergence zone.

**Mob difficulty:** 150-155. Mini-boss: Sulfur Fan Colossus (156, in Yellow Garden).

Key rooms:
- 6264 **Glasswash Fan Approach** — entry from Zone 1 east fork; volcanic aggregate underfoot; older fan terrain begins; extra description on obsidian zone identification.
- 6265 **Fan Approach Junction** — where North Fan quarry track and Middle Fan crossing route diverge; old extraction crew signage still stands.
- 6266 **North Fan Quarry Road** — extraction infrastructure approach; heavy cart-rut impressions still visible in the fan surface.
- 6267 **North Fan Quarry Outer Works** — semi-permanent extraction staging area ruins; derrick footings, chain anchors, storage pad remnants.
- 6268 **North Fan Quarry Interior** — inside sector; active quarry face ruins; banded obsidian in three quality grades visible in cut walls.
- 6269 **North Fan Quarry Deep Cut** — inside sector; `no_mob`; the deepest extraction face; mirror-quality obsidian in exposed seam; extra description on banding patterns for origin-authentication.
- 6270 **North Fan Quarry Tool Cache** — inside sector; old tool storage built into fan edge; empty racks; extra description on extraction crew absence.
- 6271 **North Fan Surface — Northwest Approach** — hardened fan terrain; banded obsidian fragments underfoot; stable but sharp.
- 6272 **North Fan Surface — Central Expanse** — widest section; wind carries obsidian-glass particles; extra description on eye protection protocol.
- 6273 **North Fan East Edge** — fan surface meets basalt ridge; transition to Middle Fan crossing.
- 6274 **Middle Fan Approach** — transition from North Fan; fan surface color shifts from banded grey to rust-brown iron-content glass.
- 6275 **Middle Fan — Western Section** — stable fan surface in dry season; ashgrass coverage confirms stabilized ash substrate.
- 6276 **Middle Fan — Sink Crust Warning Zone** — section where thin hardened crust covers hollow gas pockets; extra description on detection by ashrunner behavior.
- 6277 **Middle Fan — Sink Crust Field** — dangerous crossing section; sparse ashgrass coverage signals substrate instability; `no_mob`.
- 6278 **Middle Fan — Central Expanse** — widest point; panoramic view of all three fan zones simultaneously; 5+ sentences.
- 6279 **Middle Fan — Eastern Approach** — fan surface stable again; ahead the basalt rises toward Bellspine foothills.
- 6280 **Middle Fan — Northeast Exit** — fan terminates against the lower Bellspine ridgeback; transition to Zone 4 Bellspine approach.
- 6281 **Sulfur Fan Junction** — branch point where Sulfur Fan track separates from the Middle Fan crossing; sulfur crystal fragments begin appearing.
- 6282 **Sulfur Fan — Lower Approach** — gas seep areas visible; yellow-white sulfur crystal formations cluster around vent sites; vent-crab territory begins.
- 6283 **Sulfur Fan — Crystal Field** — dense sulfur crystal formations; ongoing deposition creates new columns; "if crabs, don't drink" principle applies strongly here.
- 6284 **Dead Pools — Outer Approach** — the cluster of sulfur-seep pools in the southern Sulfur Fan; surface water looks clear, smells faintly wrong; extra description: Ashblind Convoy Loss memorial text on a carved stone marker.
- 6285 **Dead Pools — Main Pool Rim** — `no_mob`; largest seep pool; warning marker with double-water doctrine text; extra description on contamination history.
- 6286 **Dead Pools — Toxic Spring** — `no_mob`; the spring that fed the doomed convoy; sulfur moss grows continuously around the lip; no mobs can function here (gas saturation).
- 6287 **Dead Pools — Eastern Seep** — secondary seep pool; vent-crab colony at maximum density.
- 6288 **Yellow Garden Approach** — path through crystal formations; low continuous tone audible from crystal resonance; Ventspeaker name-stone visible at perimeter.
- 6289 **Yellow Garden — Outer Ring** — yellow, white, and amber crystal columns rising from hardened sulfur floor; visually spectacular; several harvested stumps show old collection.
- 6290 **Yellow Garden — Crystal Heart** — deepest concentration of formations; the resonance tone is strongest here; Ventspeaker name-stone at center (extra description: symbol vocabulary matching Bellspine anchors).
- 6291 **Yellow Garden — Southeast Approach** — `no_mob`; transition to Yellow Garden boss chamber.
- 6292 **Yellow Garden — Sulfur Colossus Chamber** — `no_mob`; **MINI-BOSS ROOM** for Sulfur Fan Colossus; enormous sulfur-encrusted entity that has grown from the fan surface itself; extra description on geological formation evidence.
- 6293 **Glasswash Fan Upper Reach** — where the fan surface rises toward the lower Ember Crown scree; obsidian quality improves with elevation.
- 6294 **Obsidian Mirror Ridge** — exposed obsidian face polished by lahar flow; reflective surface; extra description on Keeper survey reference to this site.
- 6295 **Fan Drainage Basin** — low point where fan surface drains; sulfur chemistry in the water; vent-crab population at its most diverse.
- 6296 **Sulfur Crystal Harvest Platform** — old extraction platform with crystal-shearing tools abandoned in mid-harvest.
- 6297 **Pumice Edge Zone** — fan surface transition to pumice aggregate; lighter material underfoot; filtration-grade pumice visible.
- 6298 **Old Extraction Camp Ruins** — semi-permanent camp where extraction crews once wintered; fire pits, tool shelters, partially collapsed bunk structure.
- 6299-6343: Connecting passages, fan surface traversal rooms, obsidian fragment fields, extraction road side-branches, sulfur seep monitoring stations, thermal spring runoff channels, pumice quarry approaches, and fan-edge transition corridors completing the Glasswash zone network.

---

### Zone 4: Bellspine (6344-6453, 110 rooms)

**Theme:** The acoustic heart of the Cinderteeth — a twenty-mile fracture-scarred mid-ridge where subsurface resonance surfaces as felt vibration and heard bell-tone. The three Hollow Passes (Upper, Middle, Lower) offer different risk profiles. The Hushed Switchbacks on the Middle Hollow's western face encode the original Silent Bell Protocol history. The Null Cairn Field at the Lower Hollow's exit is the mountain's most symbolically complex site — a field of deliberately erased name-stones marking an acknowledged legal failure. Conclave crystal resonance anchors embedded in seventeen fault intersections still hum with residual function. The Bellspine requires careful navigation — resonance accumulation causes disorientation (implemented via maze-flagged sections in the echo corridors), and false echo corridors can misdirect players.

**Sector types used:** mountain (passes and ridges), inside (hollow passes, cave sections), hills (lower approaches).

**Internal Shape:** Three parallel pass routes (Upper, Middle, Lower Hollow) running east-west across the Bellspine. Each has a western approach from the Chalkwind/Glasswash convergence and an eastern exit toward the Western Fold Approach. Cross-links between the passes exist at midpoints. The Null Cairn Field is a dead-end side area off the Lower Hollow eastern exit. Crystal anchor sites are scattered throughout. A resonance labyrinth section (maze-flagged) exists in the central Bellspine where false echo corridors operate. The Boss room is the Resonance Anchor Construct's sealed vault, off the central anchor concentration.

**Mob difficulty:** 153-160. Boss: Bellspine Resonance Anchor Construct (160).

Key rooms:
- 6344 **Bellspine Approach — Western Gate** — convergence point from Chalkwind (Zone 2) and Glasswash (Zone 3); the Bellspine begins here; subsurface vibration first felt through boot soles; 5+ sentences.
- 6345 **Upper Hollow Departure** — path toward the highest, shortest, most exposed pass; suitable in dry seasons.
- 6346 **Upper Hollow — Exposed Ridge** — shortest distance but highest wind exposure; echo from the ridge amplifies; disorientation risk noted.
- 6347 **Upper Hollow — Echo Corridor Alpha** — maze-flagged section; multi-directional echo makes sound sources unidentifiable.
- 6348 **Upper Hollow — Echo Corridor Beta** — maze-flagged; resonance accumulation hazard; extra description on inner-ear disruption physiology.
- 6349 **Upper Hollow — Echo Corridor Gamma** — maze-flagged; disorientation peak; extra description on Silent Bell Protocol response instructions.
- 6350 **Upper Hollow — Ice-Season Close** — the section impassable in winter; iced ledges, drift obstructions; passable now but narrowed.
- 6351 **Upper Hollow — Eastern Descent** — descending from the ridge crest; resonance diminishes; ahead the Bellspine opens.
- 6352 **Middle Hollow Departure** — the primary commercial route; the Hushed Switchbacks begin here on the western face.
- 6353 **Hushed Switchback One** — first turn of seventeen; "no-call movement" protocol signs carved into the basalt wall.
- 6354 **Hushed Switchback Three** — third turn; resonance audible as a sustained tone; extra description on convoy coordination history.
- 6355 **Hushed Switchback Seven** — halfway point; panoramic view down the western approach; `no_mob` boundary.
- 6356 **Hushed Switchback Twelve** — three-quarters; resonance strongest on this face; extra description on Warden Halvek's founding protocol decision.
- 6357 **Hushed Switchback Seventeen** — final turn; the switchback sequence ends; the hollow opens inward.
- 6358 **Middle Hollow — Interior Passage** — the main hollow corridor; inside sector; resonance tone carried in the rock; Conclave anchor sites visible in the walls.
- 6359 **Middle Hollow — Anchor Site Alpha** — inside sector; first Conclave crystal anchor embedded in the fault face; still faintly luminous; `no_mob`; extra description on residual function debate.
- 6360 **Middle Hollow — Resonance Chamber** — inside sector; widened hollow where subsurface frequencies amplify; the bell-tone is audible without concentration; sustained exposure causes disorientation.
- 6361 **Middle Hollow — Anchor Site Beta** — inside sector; second anchor cluster; crystal fragments scattered from an anchor that partially fractured; extra description on Conclave technical failure note.
- 6362 **Middle Hollow — Eastern Exit** — hollow narrows back to pass width; resonance diminishes; eastern side of the Bellspine.
- 6363 **Lower Hollow Departure** — longest route, lowest elevation, used in extreme conditions; the approach is through a forested lower ridge section.
- 6364 **Lower Hollow — Forested Lower Approach** — cinderpine grove section; terrain more sheltered; resonance less intense than upper passes.
- 6365 **Lower Hollow — Base Route** — the oldest mapped passage through the Bellspine; Ridgekin-era cairn fragments at intervals.
- 6366 **Lower Hollow — Null Cairn Field Approach** — path to the famous memorial site; extra description foreshadowing the memorial's significance.
- 6367 **Null Cairn Field** — `no_mob`; field of deliberately erased name-stones; blank stones from the original erasure; stones marked "unknown" in three script traditions; 5+ sentences with full historical weight; extra descriptions for blank stone, erased stone, trilingual "unknown" stone.
- 6368 **Lower Hollow — Eastern Exit** — hollow opens onto the eastern Bellspine; the Null Cairn Field is the last memorable site before the Western Fold approach.
- 6369 **Bellspine Central — Northern Cross-Link** — passage connecting Upper Hollow midpoint to Middle Hollow midpoint; inside sector; resonance crosses through.
- 6370 **Bellspine Central — Southern Cross-Link** — passage connecting Middle Hollow midpoint to Lower Hollow approach; inside sector.
- 6371 **Bellspine Central — Anchor Concentration Zone** — inside sector; where multiple anchor fault intersections cluster; the combined resonance here is intense; `no_mob`; extra description on anchor placement logic.
- 6372 **Anchor Construct Sealed Vault Approach** — `no_mob`; the passage narrows toward a sealed door; rumbling resonance from within is not natural.
- 6373 **Anchor Construct Sealed Vault** — `no_mob`; **BOSS ROOM** for Bellspine Resonance Anchor Construct; a Conclave construct reanimated by centuries of resonance accumulation; crystal anchor array surrounds its form; extra description on Conclave origin notes.
- 6374 **Bellspine Eastern Gate** — convergence point where all three hollow passes rejoin on the eastern side; the Western Fold Approach begins; 5+ sentences.
- 6375 **Tremor-Snap Ledge Zone** — exposed section of the eastern Bellspine where thermal cycling has fractured rock into overlapping plates; hazard description.
- 6376 **Resonance Serpent Territory** — section where bellserpent activity indicates current resonance conditions; active in high-frequency periods; extra description on serpent behavior as monitoring instrument.
- 6377 **Old Crystal Conduit Trench** — excavated trench where Conclave teams installed underground crystal conduit between anchors; trench exposed by a collapse; extra description on conduit fragments still faintly active.
- 6378 **Bellspine False Echo Chamber** — inside sector; maze-flagged; a cave system where echo produces multi-directional false source signals; extremely disorienting.
- 6379-6453: Connecting passages, resonance corridor rooms, cross-links between the three hollow passes, additional anchor sites, secondary cave systems, Bellspine mid-ridge traversal rooms, scree approaches to anchor positions, and fault-face observation points completing the full Bellspine network.

---

### Zone 5: Western Fold Approach (6454-6533, 80 rooms)

**Theme:** The approach corridor to the Western Fold — the most historically contested section of the Cinderteeth. The Seven Ash Arches transit cut, rebuilt six or seven times, serves as an orientation point for understanding geological time. The Red Sister Cisterns (two parallel filtration structures, Ventspeaker-origin, Conclave-upgraded) demonstrate sophisticated pre-modern water engineering. Ventspeaker sacred sites — vent platforms, name-stones, tiered fire-pit remnants — are increasingly dense. Ash Cult cells have begun claiming this corridor, and their presence creates combat encounters framed as religious territorial defense.

**Sector types used:** mountain (ridge approach), inside (arch corridor, cistern interiors), field (Fold approach), hills (lower approach from Bellspine).

**Internal Shape:** Entry from Bellspine eastern gate (Zone 4). Linear corridor approach through the Seven Ash Arches (inside sector series). Red Sister Cisterns as a side cluster off the arches. Ventspeaker sacred sites as scattered rooms throughout. Ash Cult patrol territories growing denser toward the Fold itself. One boss room in the deepest Ventspeaker site.

**Mob difficulty:** 158-163. Boss: Ghost of the Last Ventspeaker (163).

Key rooms:
- 6454 **Western Fold Approach Road** — entry from Zone 4; the mountain's western structural depression is ahead; extra description on geological downthrown fault block character.
- 6455 **Seven Ash Arches — Approach** — the transit cut through the ridge spur comes into view; each arch a different generation of construction; 5+ sentences describing the palimpsest quality.
- 6456 **Seven Ash Arches — First Arch** — the oldest arch; heaviest stone, simplest geometry; material from the first collapse incorporated into the foundations.
- 6457 **Seven Ash Arches — Second and Third Arches** — two arches from the same rebuilding era; slightly different geometry shows different master's hand.
- 6458 **Seven Ash Arches — Fourth Arch** — cracked but standing; the crack is old and stabilized; extra description on structural assessment techniques taught here.
- 6459 **Seven Ash Arches — Fifth and Sixth Arches** — most recent rebuilt pair; cleanest stonework; material sourced from the fourth generation collapse.
- 6460 **Seven Ash Arches — Seventh Arch** — final arch; the cut opens beyond it into the Western Fold approach zone; extra description on the seventh arch as marker of transition.
- 6461 **Red Sister Cistern — Outer Approach** — just inside the arch corridor; two parallel cistern structures visible side by side.
- 6462 **Red Sister Cistern Alpha** — inside sector; `no_mob`; the first cistern; multi-stage filtration beds of pumice and calcite; extra description on filtration chemistry and Ventspeaker-origin water management.
- 6463 **Red Sister Cistern Beta** — inside sector; `no_mob`; the second cistern; shared filtration system originally connecting both; extra description on Great Oasis cistern parallel development reference.
- 6464 **Red Sister Cistern Filtration Chamber** — inside sector; `no_mob`; the deep filtration level; spring water input from the geothermal source; iron-rich and sulfur-processed.
- 6465 **Red Sister Cistern Maintenance Passage** — inside sector; narrow access corridor between both cisterns.
- 6466 **Vent Platform Site Alpha** — first Ventspeaker vent platform encountered; flat stone construction around an active vent; elevated oracle stage; extra description on construction methodology (thermally stable substrate selection).
- 6467 **Vent Platform Site Beta** — second platform; layered construction showing multigenerational rebuilding at the same site; Conclave equipment staging area remnants on top of the oracle stage.
- 6468 **Name-Stone Array — First Cluster** — polished volcanic rock tablets with carved symbols at a vent lip; three and seven placement patterns; extra description on symbol vocabulary and Bellspine anchor connection.
- 6469 **Tiered Fire-Pit Site Alpha** — concentric stone rings for sustained ritual combustion; ash-layer visible; use ended when Conclave occupied the Fold; extra description on stratigraphic dating.
- 6470 **Tiered Fire-Pit Site Beta** — second fire-pit cluster; smaller rings for preparatory ritual; Ash Cult has re-lit one pit in recent memory (fresh ash layer visible).
- 6471 **Ash Cult Forward Camp** — Ash Cult cell maintaining watch over the approach corridor; doctrinal graffiti on the walls; aggressive cult members.
- 6472 **Ash Cult Patrol Path Alpha** — patrol route between forward camp and inner territory.
- 6473 **Western Fold Outer Perimeter** — `no_mob` boundary room; transition from approach corridor to the Fold proper.
- 6474 **Conclave Survey Equipment Cache** — ruined instrument cache; melted crystal apparatus, fractured thermal rods; extra description on the surveyor team loss narrative.
- 6475 **Old Whisper Route Entry** — a narrow informal path used to bypass formal access checkpoints; extra description on whisper route history.
- 6476 **Ghost of the Ventspeaker Approach** — `no_mob`; the path narrows toward a sacred vent site where a presence makes itself known.
- 6477 **Last Ventspeaker's Oracle Ground** — `no_mob`; **BOSS ROOM** for Ghost of the Last Ventspeaker (Cairn-Mother Retha An); the final custodian of Ventspeaker knowledge, preserved as a ghost bound to her most sacred site; extra description on her identity as "mountain-tongue born."
- 6478-6533: Connecting passages, Western Fold approach traversal rooms, Ash Cult territory expansion zones, additional vent platform sites, name-stone arrays, cistern exterior approach roads, arch corridor side alcoves, and transition corridors to Zone 6.

---

### Zone 6: Western Fold / Sanctum Scar (6534-6593, 60 rooms)

**Theme:** The deepest structurally accessible zone below the Ember Crown — the Western Fold itself, geologically anomalous (downthrown fault block, geothermal amplified, gas-trapping depression). The Speaking Vent at the southern end still pulses with periodic gas releases interpreted by the Ash Cult as pronouncements. The tiered fire-pit central complex (the most important Ventspeaker site, now re-occupied by Ash Cult leadership). The Sanctum Scar surface — the circular collapse depression above the Sunken Sanctum's upper structures — is accessible as a side area with a sealed downward passage (planned future link to Sunken Sanctum). Thermophilic fungal networks carpet the walls. The Grand Vent-Oracle of the Ash Cult occupies the Speaking Vent chamber.

**Sector types used:** mountain (Fold approaches), field (Fold floor), inside (tiered fire-pit interior, Scar descent chambers).

**Internal Shape:** Entry from Zone 5. A central Fold floor area branching to three key sub-sites: Speaking Vent (boss), Sanctum Scar surface (planned deep exit), and the Tiered Fire-Pit complex (Ash Cult stronghold). The zone layout reflects the Fold's enclosed character — surrounded by flanking ridges, accessible only through the arch corridor approach.

**Mob difficulty:** 163-166. Boss: Grand Vent-Oracle of the Ash Cult (166).

Key rooms:
- 6534 **Western Fold Entry** — the downthrown fault block becomes evident; terrain drops lower than surrounding ridges; heat and gas concentration noticeably higher; 5+ sentences.
- 6535 **Western Fold Floor — Northern Reach** — the Fold floor opens out; geothermal warmth rises through the basalt; thermophilic fungal networks visible on rock surfaces.
- 6536 **Western Fold Floor — Central Expanse** — widest point of the Fold; iron-rich spring drainage visible; multiple approach corridors visible ahead; Ash Cult markers everywhere.
- 6537 **Thermophilic Fungal Growth Zone** — dense fungal network on warm rock; visually similar to Withered Depths networks but biologically distinct; extra description on the scholarly confusion warning.
- 6538 **Geothermal Furnace Site** — where the Conclave installed their metallurgical furnaces; stone infrastructure still standing; extra description on why the Conclave chose this site (four practical reasons enumerated in Conclave records).
- 6539 **Iron-Rich Spring Basin** — mineral spring drainage; iron-moss (rust-orange mineral accretion) covers nearby rock; extra description on ritual pool chemistry reference.
- 6540 **Ash Cult Inner Territory** — deep Ash Cult occupation zone; senior cult members; aggressive and fanatical.
- 6541 **Ash Cult Doctrine Hall** — inside sector; cult ritual space with inscribed doctrinal pronouncements; extra descriptions for key Ash Cult theological claims.
- 6542 **Ash Cult Archivist's Chamber** — inside sector; cult records; extra description referencing "ridge elders" and claimed contact with Ventspeaker lineage survivors.
- 6543 **Tiered Fire-Pit Complex — Outer Ring** — the most important Ventspeaker sacred site, now Ash Cult operational center; concentric stone rings; multiple ongoing fires.
- 6544 **Tiered Fire-Pit Complex — Middle Ring** — inside the ritual complex; cult members performing interpretive rites.
- 6545 **Tiered Fire-Pit Complex — Inner Sanctum** — inside sector; `no_mob`; the innermost fire-pit ring; extra description on the abrupt end of Ventspeaker use layer coinciding with Conclave arrival.
- 6546 **Speaking Vent Approach** — the path toward the southern vent; periodic pulse rhythm audible well before arrival; gas seep concentration increases.
- 6547 **Speaking Vent Chamber** — the most sacred Ventspeaker site and now the Grand Vent-Oracle's seat; the vent produces periodic gas pulses with variable intervals; `no_mob`; 5+ sentences.
- 6548 **Grand Vent-Oracle's Throne** — `no_mob`; **BOSS ROOM** for Grand Vent-Oracle of the Ash Cult; a cultist who has breathed the Speaking Vent's gases for decades and developed genuine geological intuition warped through theological conviction; extra description distinguishing Ventspeaker empiricism from Ash Cult doctrine.
- 6549 **Sanctum Scar — Surface Approach** — the ground changes; fractured and subsided terrain; circular depression ahead.
- 6550 **Sanctum Scar — Outer Ring** — the collapse depression's rim; ground fissures, intermittent gas seepage; extra description on seasonal character variation.
- 6551 **Sanctum Scar — Inner Zone** — inside the circular depression; occasional subsidence events; extra description on the scale and history of the collapse.
- 6552 **Sanctum Scar — Collapse Edge** — `no_mob`; the deepest accessible surface point; a shaft descends into darkness below (planned link down to Sunken Sanctum; exit sealed/impassable until that area is built; described as blocked in room text).
- 6553-6593: Connecting passages, Fold floor traversal rooms, Ash Cult patrol territories, thermophilic zone exploration branches, iron-spring drainage channels, geothermal vent pockets, Conclave instrument ruin sites, and Sanctum Scar perimeter rooms completing the Western Fold network.

---

### Zone 7: Ember Crown Approaches (6594-6653, 60 rooms)

**Theme:** The transition zone between the habitable mountain and the lethal caldera — upper scree fields, glasswalk zones of Ember Crown obsidian (harder, shinier, greener-tinted than Fan obsidian), the Widow's Spire (stable summit, best observation point), and glow-hawk nesting cliffs. The terrain becomes genuinely dangerous: vent-gas inversion pockets, glassfall hazard, scree-shelf collapse. Lightning-grounded ash columns may develop. Mobs here are large, heat-adapted, and highly territorial. The approach routes to the three principal peaks of the Ember Crown separate here.

**Sector types used:** mountain (upper approaches), hills (lower upper scree), inside (observation post, scree shelter interiors).

**Internal Shape:** Entry from Zone 6 via an upper corridor. The zone branches into three approach paths: Firsttooth Approach (leads to Zone 8 boss content), Secondtooth Approach (flanking path, heavier mob density), and Widow's Spire (observatory peak, lookout, no active volcano but high-altitude challenge). Cross-links between the three approaches. The Widow's Spire is the highest stable terrain in the area and offers the best narrative vantage.

**Mob difficulty:** 165-168. No boss in this zone (boss content begins in Zone 8).

Key rooms:
- 6594 **Upper Mountain Gate** — the threshold between the mid-range and the upper Crown approaches; visible caldera glow at night; 5+ sentences.
- 6595 **Ember Crown Scree Approach** — upper scree terrain; basalt fragments from thermal cycling fracture underfoot; glassfall debris on older surfaces.
- 6596 **Glasswalk Zone — Entry** — Ember Crown obsidian begins; black with greenish tint in cross-section; distinctly higher-quality than Fan obsidian; licensed extraction shelters visible.
- 6597 **Glasswalk Zone — Harvest Plateau** — extraction crew emergency retreat protocol signs still bolted to stakes; extra description on licensed extraction rules.
- 6598 **Glow-Hawk Nesting Cliff** — sheer basalt cliff face; enormous raptor nests at heights inaccessible to ground predators; glow-hawk behavioral warning about eruption precursor included.
- 6599 **Vent-Gas Inversion Pocket Alpha** — `no_mob`; colorless gas in caldera depression; faint sweet-sulfur odor at approach; hazard description; extra description on detection protocol.
- 6600 **Vent-Gas Inversion Pocket Beta** — second inversion depression; extra description on the death cases that formalized the protocol.
- 6601 **Widow's Spire Approach** — path to the secondary collapsed peak; most stable terrain on the Crown; best sightlines.
- 6602 **Widow's Spire Base** — the collapsed peak's lower structure; stable ancient basalt; rest point before the exposed climb.
- 6603 **Widow's Spire — Mid Climb** — exposed climbing route up the stable peak; wind-scoured; breath visible in the thin air.
- 6604 **Widow's Spire — Summit** — `no_mob`; highest stable observation point in the range; panoramic view of the full Crown, all three principal peaks, and the Western Fold depression; 5+ sentences; extra description on sightline catalog.
- 6605 **Firsttooth Approach Path** — the primary route toward the active caldera's tallest peak; caldera glow visible ahead.
- 6606 **Firsttooth Lower Scree** — unstable scree from thermal cycling; loaded footsteps trigger small slides.
- 6607 **Firsttooth Mid-Approach** — hazard zone; scree-shelf collapse risk increases; no-traverse markers left by extraction crews.
- 6608 **Secondtooth Flanking Path** — path around to the connected secondary caldera peak; heavier creature density (territory of the most aggressive Crown fauna).
- 6609 **Secondtooth Lower Approach** — the shared magma chamber coupling is evident in synchronized tremors; "double-shudder" tremor signature described.
- 6610 **Secondtooth Mid-Approach** — thermal cycling scree; heat-adapted fauna at peak density.
- 6611 **Crown Scree — Lightning Hazard Zone** — large ash column visible above; static discharge grounding to nearby metal described; extra description on lightning-grounded ash columns.
- 6612 **Crown Scree — Glassfall Zone** — glassfall debris litters the ground; old hail of fused volcanic glass particles; extra description on glassfall characteristics and danger.
- 6613 **Upper Crown Junction** — where Firsttooth and Secondtooth approach paths converge before the final approach to Zone 8; last stable rest point.
- 6627 **Northeastern Ridge Approach — Lower** — eastward-facing basalt ridge; forested lowland canopy visible far below and to the northeast; the GNF's Northwestern Crown treeline is distantly visible on clear days; **planned `east` exit to GNF 4179** (Northwestern Crown Ridgeline South).
- 6633 **Northeastern Ridge Approach — Mid** — exposed mid-altitude ridgeline; wind-stripped rock; ash deposits from the Ashfall Monsoon still coat the eastern face; **planned `east` exit to GNF 4199** (Northwestern Crown Ridgeline Mid).
- 6639 **Northeastern Ridge Approach — Upper** — highest stable traverse point on the northeastern face; the GNF crown is a vast dark-green expanse to the northeast; ranger-writ expedition markers are driven into the rock here; `no_mob`; **planned `east` exit to GNF 4219** (Northwestern Crown — Westernmost Point); 5+ sentences.
- 6614-6653 (remaining): Connecting passages, upper scree traversal rooms, vent-gas pocket approach corridors, glow-hawk territory, glasswalk harvest zone side branches, extraction shelter ruins, summit approach paths, and ridge-line traversal rooms completing the Crown approach network.

---

### Zone 8: Ember Crown (6654-6723, 70 rooms)

**Theme:** The caldera crown itself — the most dangerous and least accountable zone in the world. Three principal peaks: the Firsttooth (tallest, active caldera, persistent glow-vent activity), the Secondtooth (magnetically coupled to Firsttooth through shared magma chamber, "double-shudder" tremor zone), and the Widow's Spire upper structure (already accessed from Zone 7). The Oracle Vent on the Firsttooth's southern slope (partially collapsed but still active, most important Ventspeaker sacred site, ongoing Ash Cult pilgrimages). The summit caldera ring of the Firsttooth contains the Ember Throne — a basalt formation that has accumulated volcanic minerals over geological time into a shape that the Ash Cult interprets as a judge's seat. The Ember Throne Patriarch, a being that has merged with the caldera's geological intelligence, is the final boss.

**Sector types used:** mountain (upper caldera), inside (vent chambers, caldera rim interiors).

**Internal Shape:** Entry from Zone 7. Firsttooth summit approach (primary route, boss content). Secondtooth ridge (flanking route, connects to Firsttooth via shared magma chamber passage — a deep inside-sector traverse). Oracle Vent site (side excursion from Firsttooth southern slope, significant lore content). Ember Throne summit (final boss). The layout ascends and circles the caldera rim before penetrating the interior.

**Mob difficulty:** 168-170. Boss (secondary): Secondtooth Caldera Watcher (168). Final Boss: The Ember Throne Patriarch (170).

Key rooms:
- 6654 **Ember Crown Entry** — the caldera zone proper begins; ground radiates heat through boots; persistent glow-vent illumination at night; 5+ sentences.
- 6655 **Firsttooth Lower Caldera Face** — the active caldera's lowest approach; magma proximity elevated; scree unstable.
- 6656 **Firsttooth Mid-Caldera Face** — halfway up the caldera structure; vent emissions visible above; extra description on ferric scoria formation.
- 6657 **Firsttooth Upper Caldera Approach** — the caldera ring is close; glow-vent illumination from below; heat haze distorts vision.
- 6658 **Firsttooth Caldera Rim — West** — on the caldera rim; panoramic view into the active magma-lit interior below; `no_mob`; 5+ sentences.
- 6659 **Firsttooth Caldera Rim — North** — rim traversal; the glow intensifies; temperature at maximum sustainable.
- 6660 **Firsttooth Caldera Rim — East** — rim traversal; Oracle Vent approach visible to the south; extra description on glow-hawk behavior changes visible from rim height.
- 6661 **Firsttooth Caldera Rim — South / Oracle Vent Overlook** — the partially collapsed Oracle Vent opening visible on the southern slope below.
- 6662 **Oracle Vent Approach** — descent toward the southern slope; the vent's periodic pulse rhythm becomes detectable.
- 6663 **Oracle Vent Site** — `no_mob`; the widest-mouthed vent opening, partially collapsed, most elaborately carved name-stone cluster in the range; Ash Cult pilgrimage offerings at the lip; 5+ sentences; extra descriptions for the name-stone carvings, offering accumulation, and gas-release pattern record.
- 6664 **Secondtooth Approach Ridge** — the ridge connecting Firsttooth and Secondtooth; subsurface magma coupling produces the "double-shudder" underfoot.
- 6665 **Secondtooth Lower Caldera** — the secondary peak's lower caldera face; not as tall as Firsttooth but tremor-coupled to it.
- 6666 **Secondtooth Mid-Approach** — most aggressive creature territory on the Crown; maximum mob density.
- 6667 **Secondtooth Summit Approach** — `no_mob` boundary; transition to boss approach.
- 6668 **Secondtooth Caldera Watcher's Ground** — `no_mob`; **BOSS ROOM** for Secondtooth Caldera Watcher; an enormous heat-adapted apex predator that has established permanent territory at the coupled caldera; not a construct but a genuine creature of extreme scale.
- 6669 **Shared Magma Chamber Passage** — inside sector; deep passage through the shared subsurface magma chamber connecting Firsttooth and Secondtooth; extreme heat; extra description on the geological coupling mechanism.
- 6670 **Ember Throne Approach** — the final approach to the Firsttooth's summit interior; the caldera rim gives way to the interior caldera chamber.
- 6671 **Ember Throne Inner Caldera Chamber** — inside sector; the caldera interior; the Ember Throne is visible — a basalt formation that has accumulated volcanic minerals into a cathedral form; `no_mob`; 5+ sentences.
- 6672 **Ember Throne Antechamber** — inside sector; `no_mob`; the space immediately before the Throne; volcanic intelligence presses against the walls; extra description on how different civilizations have interpreted this formation.
- 6673 **The Ember Throne** — inside sector; `no_mob`; **FINAL BOSS ROOM** for the Ember Throne Patriarch; a geological entity that has been accumulating consciousness from the mountain's cycles for an indeterminate number of eruption-dormancy cycles; the Ash Cult worships it as the judge who delivers verdicts; extra description on the difference between this and mere construction; 5+ sentences.
- 6674 **Caldera Observation Ledge** — natural ledge on the caldera interior wall; view of the entire magma chamber below; `no_mob`; lore-rich room for players who survived the encounter.
- 6675 **Summit Pumice Field** — upper-layer pumice from the most recent eruptive cycle; vesicular structure superior for filtration applications; extra description on quality differentiation.
- 6676 **High Sulfur Bloom Deposit** — largest sulfur concentration in the range; freshly deposited pale yellow crystals; alchemical premium quality.
- 6677 **Mirror-Quality Obsidian Seam** — the highest-grade obsidian in the Cinderteeth; distinctive greenish cross-section tint; extra description on origin-authentication by color and bubble structure.
- 6678-6723: Connecting passages, caldera face traversal rooms, vent-gas inversion pockets at summit level, name-stone sites accessible only at summit altitude, scree approach corridors, caldera rim traverse sections, and glow-vent observation alcoves completing the Ember Crown network.

---


## Full Mob Roster (6124-6243, 120 mobs)

Mob vnums are assigned sequentially from 6124. Each mob entry lists: vnum, name, level, act flags, combat profile, stats line, special, and zone. All mobs include `is_npc` and `stay_area`. All mobs in open traversal sub-regions include `no_hunt` to prevent bleed. Bosses include `sentinel`, `boss`, `no_flee`, `no_hunt`. Elite solos include `solo`. All standard mobs receive melee profile at this tier.

**Boss stat convention:**
- Mini-boss (tier 1, 148-156): `0 -100 10 10`
- Boss (tier 2, 160-166): `0 -100 11 11`
- Final Boss (tier 3, 170): `0 -100 12 12`

**Standard mob stat convention by level band:**
- 145-149: `0 0 5 5`
- 150-154: `0 -25 6 6`
- 155-159: `0 -50 7 7`
- 160-164: `0 -75 8 8`
- 165-170: `0 -100 9 9`

---

### Zone 1 Mobs — Blackstep Threshold (6124-6143, 20 mobs)

| Vnum | Name | Level | Act Flags | Stats | Special | Notes |
|---:|---|---:|---|---|---|---|
| 6124 | ashfang remnant scout | 145 | `is_npc aggressive stay_area no_hunt` | `0 0 5 5` | — | Patrol mob, Zone 1 entry |
| 6125 | ashfang remnant raider | 146 | `is_npc aggressive stay_area no_hunt` | `0 0 5 5` | — | Standard bandit, outer territory |
| 6126 | ashfang remnant enforcer | 147 | `is_npc aggressive stay_area no_hunt` | `0 -25 6 6` | — | Tougher bandit; tollbridge area |
| 6127 | ashfang sentry captain | 147 | `is_npc aggressive sentinel stay_area` | `0 -25 6 6` | — | Drops stronghold key; Sentry Tower |
| 6128 | ashfang remnant saboteur | 146 | `is_npc aggressive stay_area no_hunt` | `0 0 5 5` | `spec_poison` | Poisoned blade melee; outer camp |
| 6129 | ashfang remnant shaman | 146 | `is_npc aggressive stay_area no_hunt` | `0 0 5 5` | `spec_cast_cleric` | Caster support; outer camp |
| 6130 | ashfang remnant warlord | 148 | `is_npc aggressive sentinel stay_area boss no_flee no_hunt` | `0 -100 10 10` | `spec_cinderteeth_warlord` | **MINI-BOSS**; Command Chamber; boss/no_mob room |
| 6131 | cinderteeth ashrunner | 145 | `is_npc aggressive stay_area no_hunt` | `0 0 5 5` | — | Fast rodent; Warren Flats |
| 6132 | cinderteeth mountain bear | 147 | `is_npc aggressive stay_area no_hunt` | `0 -25 6 6` | — | Large omnivore; Bear Territory |
| 6133 | cinderteeth mountain bear elder | 148 | `is_npc aggressive stay_area no_hunt solo` | `0 -50 7 7` | — | Elite bear; mineral-lick zones |
| 6134 | blackstep basalt sentinel | 146 | `is_npc aggressive stay_area no_hunt` | `0 0 5 5` | — | Volcanic construct; basalt ridge |
| 6135 | blackstep vent-crab swarm | 145 | `is_npc aggressive stay_area no_hunt` | `0 0 5 5` | `spec_poison` | Thermal spring areas; sulfur mob |
| 6136 | cinderpine resin-hawk | 145 | `is_npc aggressive stay_area no_hunt` | `0 0 5 5` | — | Flying creature; Pine Copse |
| 6137 | foothills scavenger vulture | 145 | `is_npc aggressive stay_area no_hunt` | `0 0 5 5` | `spec_fido` | Ash-feeder; patrol zone cleanup |
| 6138 | old warden commission ghost | 146 | `is_npc aggressive stay_area no_hunt undead` | `0 0 5 5` | `spec_cast_undead` | Undead patrol; Commission Post |
| 6139 | ashfang remnant quartermaster | 147 | `is_npc sentinel stay_area no_hunt` | `0 -25 6 6` | — | Non-aggressive supply keeper; Stronghold Armory |
| 6140 | thermal spring basilisk | 147 | `is_npc aggressive stay_area no_hunt solo` | `0 -50 7 7` | `spec_breath_fire` | Elite creature; Thermal Seep Ground |
| 6141 | keeper survey shade | 145 | `is_npc aggressive stay_area no_hunt undead` | `0 0 5 5` | `spec_cast_undead` | Ghost of old surveyor; Survey Marker Station |
| 6142 | blackstep obsidian golem | 148 | `is_npc aggressive stay_area no_hunt solo` | `0 -50 7 7` | — | Elite construct; Obsidian Deposit |
| 6143 | ashfang remnant tracker | 146 | `is_npc aggressive stay_area no_hunt hunter` | `0 0 5 5` | — | Hunter mob; pursues fleeing players |

---

### Zone 2 Mobs — Chalkwind Saddles (6144-6163, 20 mobs)

| Vnum | Name | Level | Act Flags | Stats | Special | Notes |
|---:|---|---:|---|---|---|---|
| 6144 | chalkwind patrol sentinel | 148 | `is_npc aggressive stay_area no_hunt` | `0 -25 6 6` | — | Route house survivor patrol; Saddles entry |
| 6145 | chalkwind mountain bear | 149 | `is_npc aggressive stay_area no_hunt` | `0 -25 6 6` | — | Larger bear; mineral-lick saddle zones |
| 6146 | chalkwind mountain bear alpha | 150 | `is_npc aggressive stay_area no_hunt solo` | `0 -50 7 7` | — | Elite bear; mineral-lick rock ledge |
| 6147 | chalkwind saddle wraith | 149 | `is_npc aggressive stay_area no_hunt undead` | `0 -25 6 6` | `spec_cast_undead` | Undead caravan ghost; Long Saddle subsidence area |
| 6148 | chalk-ash elemental | 150 | `is_npc aggressive stay_area no_hunt` | `0 -25 6 6` | — | Chalk-ash given animate form; saddle depressions |
| 6149 | chalkwind resonance wyrm | 150 | `is_npc aggressive stay_area no_hunt solo` | `0 -50 7 7` | `spec_breath_any` | Elite serpentine creature; Saddle Wind Corridor |
| 6150 | ironpost route house revenant | 151 | `is_npc aggressive stay_area no_hunt undead` | `0 -50 7 7` | `spec_cast_cleric` | Old route house defender; Ironpost upper floor |
| 6151 | ironpost warden-commander | 152 | `is_npc aggressive sentinel stay_area boss no_flee no_hunt undead` | `0 -100 10 10` | `spec_cinderteeth_warden_cmd` | **MINI-BOSS**; Archive Sealed Wing; drops archive key |
| 6152 | saddle pass scavenger vulture | 148 | `is_npc stay_area no_hunt` | `0 0 5 5` | `spec_fido` | Non-aggressive cleanup; passive presence |
| 6153 | chalkwind ice-shelf shade | 149 | `is_npc aggressive stay_area no_hunt undead` | `0 -25 6 6` | `spec_cast_undead` | Ghost of a winter-close casualty; Upper Hollow area |
| 6154 | chalk-ash drift crawler | 149 | `is_npc aggressive stay_area no_hunt` | `0 -25 6 6` | — | Ash-deposit creature; Ash Dune zone |
| 6155 | saddle route hound | 148 | `is_npc aggressive stay_area no_hunt hunter` | `0 0 5 5` | — | Feral hunting dog, remnant of convoy; Long Saddle |
| 6156 | muster yard requisitions ghost | 150 | `is_npc aggressive stay_area no_hunt undead` | `0 -25 6 6` | `spec_cast_undead` | Emergency-powers bureaucrat undead; Muster Yard |
| 6157 | chalkwind ledge crawler | 149 | `is_npc aggressive stay_area no_hunt` | `0 -25 6 6` | — | Rock-face creature; scree-shelf zone |
| 6158 | chalkwind summit raptor | 150 | `is_npc aggressive stay_area no_hunt` | `0 -25 6 6` | — | Large bird of prey; Summit Roc Scree |
| 6159 | blind saddle echo-lurker | 150 | `is_npc aggressive stay_area no_hunt no_mind` | `0 -25 6 6` | — | Disorienting creature; Blind Saddle; no_mind = immune to mental |
| 6160 | chalkwind ashfall survivor | 151 | `is_npc aggressive stay_area no_hunt` | `0 -50 7 7` | — | Maddened Ashfall-era survivor; Black Thorn Camp area |
| 6161 | chalkwind chalk-golem | 151 | `is_npc aggressive stay_area no_hunt solo` | `0 -50 7 7` | — | Chalk-ash construct; saddle summit area |
| 6162 | old tariff enforcer ghost | 149 | `is_npc aggressive stay_area no_hunt undead` | `0 -25 6 6` | `spec_cast_undead` | Ghost tollman; Old Tariff Post |
| 6163 | chalkwind glow-hawk | 151 | `is_npc aggressive stay_area no_hunt` | `0 -50 7 7` | — | Large heat-adapted raptor; High Saddle thermals |

---

### Zone 3 Mobs — Glasswash Fans (6164-6183, 20 mobs)

| Vnum | Name | Level | Act Flags | Stats | Special | Notes |
|---:|---|---:|---|---|---|---|
| 6164 | glasswash fan crawler | 150 | `is_npc aggressive stay_area no_hunt` | `0 -25 6 6` | — | Fan-surface creature; Middle Fan |
| 6165 | glasswash obsidian shard golem | 151 | `is_npc aggressive stay_area no_hunt` | `0 -50 7 7` | — | Animated obsidian fragments; North Fan |
| 6166 | glasswash mirror-blind hunter | 151 | `is_npc aggressive stay_area no_hunt` | `0 -50 7 7` | — | Light-adapted predator; obsidian mirror zones |
| 6167 | glasswash ashrunner pack-leader | 152 | `is_npc aggressive stay_area no_hunt` | `0 -50 7 7` | — | Elite ashrunner; Fan surface |
| 6168 | glasswash vent-crab broodmother | 152 | `is_npc aggressive stay_area no_hunt solo` | `0 -50 7 7` | `spec_poison` | Elite vent-crab; Sulfur Fan crystal field |
| 6169 | glasswash acid-seep leech | 150 | `is_npc aggressive stay_area no_hunt` | `0 -25 6 6` | `spec_poison` | Sulfur-tolerant creature; Dead Pools approach |
| 6170 | glasswash sulfur elemental | 152 | `is_npc aggressive stay_area no_hunt` | `0 -50 7 7` | `spec_breath_gas` | Sulfur-infused elemental; Sulfur Fan |
| 6171 | glasswash crystal resonance sprite | 152 | `is_npc aggressive stay_area no_hunt no_body` | `0 -50 7 7` | `spec_cast_mage` | Crystal-attuned spirit; Yellow Garden |
| 6172 | glasswash sulfur fan colossus | 156 | `is_npc aggressive sentinel stay_area boss no_flee no_hunt` | `0 -100 10 10` | `spec_cinderteeth_sulfur_colossus` | **MINI-BOSS**; Yellow Garden Colossus Chamber; sulfur-encrusted giant |
| 6173 | glasswash pumice drifter | 150 | `is_npc aggressive stay_area no_hunt` | `0 -25 6 6` | — | Pumice-aggregate creature; Pumice Edge Zone |
| 6174 | glasswash extraction site haunt | 151 | `is_npc aggressive stay_area no_hunt undead` | `0 -50 7 7` | `spec_cast_undead` | Ghost of extraction worker; North Fan |
| 6175 | glasswash mirror-shard swarm | 151 | `is_npc aggressive stay_area no_hunt no_body` | `0 -50 7 7` | — | Animated glass particle cloud; North Fan central expanse |
| 6176 | glasswash iron-moss elemental | 153 | `is_npc aggressive stay_area no_hunt` | `0 -50 7 7` | — | Iron-mineral creature; Fan drainage basin |
| 6177 | glasswash lava tube lurker | 153 | `is_npc aggressive stay_area no_hunt solo` | `0 -75 8 8` | `spec_breath_fire` | Elite creature; Long Saddle subsidence echo, appears here |
| 6178 | glasswash ashfall remnant | 152 | `is_npc aggressive stay_area no_hunt undead` | `0 -50 7 7` | `spec_cast_undead` | Convoy ghost from Ashblind event; Dead Pools area |
| 6179 | glasswash thermal vent wyrm | 154 | `is_npc aggressive stay_area no_hunt solo` | `0 -75 8 8` | `spec_breath_fire` | Elite creature; upper fan reach |
| 6180 | glasswash scavenger vulture elder | 152 | `is_npc aggressive stay_area no_hunt` | `0 -50 7 7` | — | Elder ash-feeder; fan area |
| 6181 | glasswash crystal golem | 154 | `is_npc aggressive stay_area no_hunt solo` | `0 -75 8 8` | — | Obsidian construct; Mirror Ridge |
| 6182 | glasswash sulfur harvester ghost | 151 | `is_npc aggressive stay_area no_hunt undead` | `0 -50 7 7` | `spec_cast_undead` | Ghost of sulfur harvester; harvest platform |
| 6183 | glasswash fan patrol hawk | 151 | `is_npc aggressive stay_area no_hunt` | `0 -50 7 7` | — | Glow-hawk variant; upper fan reach |

---

### Zone 4 Mobs — Bellspine (6184-6203, 20 mobs)

| Vnum | Name | Level | Act Flags | Stats | Special | Notes |
|---:|---|---:|---|---|---|---|
| 6184 | bellspine resonance serpent | 153 | `is_npc aggressive stay_area no_hunt` | `0 -50 7 7` | — | Bellserpent; resonance-active zones |
| 6185 | bellspine resonance serpent elder | 155 | `is_npc aggressive stay_area no_hunt solo` | `0 -75 8 8` | — | Elite bellserpent; echo chamber areas |
| 6186 | bellspine false-echo hunter | 154 | `is_npc aggressive stay_area no_hunt no_mind` | `0 -50 7 7` | — | Creature that navigates by resonance; echo corridors |
| 6187 | bellspine crystal anchor shard | 154 | `is_npc aggressive stay_area no_hunt no_body` | `0 -50 7 7` | `spec_cast_mage` | Animated Conclave crystal fragment; anchor sites |
| 6188 | bellspine resonance wraith | 155 | `is_npc aggressive stay_area no_hunt undead no_body` | `0 -75 8 8` | `spec_cast_undead` | Resonance-sustained ghost; Resonance Chamber |
| 6189 | bellspine null cairn shade | 155 | `is_npc aggressive stay_area no_hunt undead` | `0 -75 8 8` | `spec_cast_undead` | Ghost of the erased dead; Null Cairn Field approaches |
| 6190 | bellspine acoustic predator | 155 | `is_npc aggressive stay_area no_hunt solo` | `0 -75 8 8` | — | Elite creature using resonance for hunting |
| 6191 | bellspine conduit warden | 156 | `is_npc aggressive stay_area no_hunt undead` | `0 -75 8 8` | `spec_cast_cleric` | Undead Conclave warden maintaining conduit; trench area |
| 6192 | bellspine ledge-snap golem | 156 | `is_npc aggressive stay_area no_hunt` | `0 -75 8 8` | — | Rock-plate animate; tremor-snap ledge zone |
| 6193 | bellspine mountain bear | 154 | `is_npc aggressive stay_area no_hunt` | `0 -50 7 7` | — | Bear penetrating into Bellspine from lower approach |
| 6194 | bellspine resonance anchor construct | 160 | `is_npc aggressive sentinel stay_area boss no_flee no_hunt` | `0 -100 11 11` | `spec_cinderteeth_anchor` | **BOSS**; Anchor Construct Sealed Vault; reanimated Conclave construct |
| 6195 | bellspine hollow pass guardian | 157 | `is_npc aggressive stay_area no_hunt solo` | `0 -75 8 8` | — | Elite guardian creature; Upper Hollow summit |
| 6196 | bellspine tremor elemental | 157 | `is_npc aggressive stay_area no_hunt` | `0 -75 8 8` | `spec_breath_lightning` | Seismic elemental; fault-face areas |
| 6197 | bellspine ice-season phantom | 155 | `is_npc aggressive stay_area no_hunt undead no_body` | `0 -75 8 8` | `spec_cast_undead` | Winter-close casualty ghost; Upper Hollow |
| 6198 | bellspine resonance locust swarm | 154 | `is_npc aggressive stay_area no_hunt no_body` | `0 -50 7 7` | — | Resonance-attuned insect entity; Lower Hollow |
| 6199 | bellspine acoustic bat colony | 153 | `is_npc aggressive stay_area no_hunt no_body` | `0 -50 7 7` | — | Resonance-adapted bats; cave sections |
| 6200 | bellspine switchback revenant | 156 | `is_npc aggressive stay_area no_hunt undead` | `0 -75 8 8` | `spec_cast_undead` | Ghost of convoy loss; Hushed Switchbacks |
| 6201 | bellspine crystal anchor cluster | 158 | `is_npc aggressive stay_area no_hunt no_body` | `0 -75 8 8` | `spec_cast_mage` | Cluster of active anchors given hostile resonance-form |
| 6202 | bellspine fault-crack wyrm | 158 | `is_npc aggressive stay_area no_hunt solo` | `0 -100 9 9` | `spec_breath_any` | Elite wyrm inhabiting fault cracks |
| 6203 | bellspine seismic sentinel | 159 | `is_npc aggressive stay_area no_hunt solo` | `0 -100 9 9` | — | Elite stone-guardian at Bellspine Eastern Gate |

---

### Zone 5 Mobs — Western Fold Approach (6204-6213, 10 mobs)

| Vnum | Name | Level | Act Flags | Stats | Special | Notes |
|---:|---|---:|---|---|---|---|
| 6204 | ash cult forward zealot | 158 | `is_npc aggressive stay_area no_hunt` | `0 -75 8 8` | — | Standard Ash Cult cultist; forward camp |
| 6205 | ash cult interpreter-priest | 159 | `is_npc aggressive stay_area no_hunt` | `0 -75 8 8` | `spec_cast_cleric` | Caster cultist; forward camp and patrol |
| 6206 | ash cult ash-walker | 159 | `is_npc aggressive stay_area no_hunt` | `0 -75 8 8` | — | Melee cultist hardened by vent exposure |
| 6207 | western fold approach warden ghost | 160 | `is_npc aggressive stay_area no_hunt undead` | `0 -100 9 9` | `spec_cast_undead` | Undead expedition warden; approach corridor |
| 6208 | ventspeaker vent platform spirit | 160 | `is_npc aggressive stay_area no_hunt undead no_body` | `0 -100 9 9` | `spec_cast_undead` | Spirit bound to a vent platform; platform sites |
| 6209 | ash cult fire-keeper | 161 | `is_npc aggressive stay_area no_hunt` | `0 -100 9 9` | `spec_breath_fire` | Elite cultist tending the fire-pits |
| 6210 | ash cult doctrinal enforcer | 162 | `is_npc aggressive stay_area no_hunt solo` | `0 -100 9 9` | `spec_cast_bigtime` | Elite senior cultist; inner territory approach |
| 6211 | conclave survey instrument shade | 160 | `is_npc aggressive stay_area no_hunt undead no_body` | `0 -100 9 9` | `spec_cast_mage` | Ghost of a Conclave survey instrument consciousness |
| 6212 | ghost of the last ventspeaker | 163 | `is_npc aggressive sentinel stay_area boss no_flee no_hunt undead no_body` | `0 -100 11 11` | `spec_cinderteeth_ventspeaker` | **BOSS**; Last Ventspeaker's Oracle Ground; Cairn-Mother Retha An's ghost |
| 6213 | western fold approach basilisk | 162 | `is_npc aggressive stay_area no_hunt solo` | `0 -100 9 9` | `spec_breath_fire` | Elite creature inhabiting the approach road |

---

### Zone 6 Mobs — Western Fold / Sanctum Scar (6214-6223, 10 mobs)

| Vnum | Name | Level | Act Flags | Stats | Special | Notes |
|---:|---|---:|---|---|---|---|
| 6214 | ash cult senior zealot | 163 | `is_npc aggressive stay_area no_hunt` | `0 -100 9 9` | — | High-rank cult member; Fold inner territory |
| 6215 | ash cult doctrine-speaker | 164 | `is_npc aggressive stay_area no_hunt` | `0 -100 9 9` | `spec_cast_cleric` | Caster; Doctrine Hall |
| 6216 | ash cult archivist | 164 | `is_npc sentinel stay_area no_hunt` | `0 -100 9 9` | `spec_cast_undead` | Semi-passive; Archivist Chamber; drops cult archive key |
| 6217 | western fold thermophilic titan | 164 | `is_npc aggressive stay_area no_hunt solo` | `0 -100 9 9` | — | Massive fungal-encrusted creature; fungal growth zone |
| 6218 | western fold iron-spring elemental | 163 | `is_npc aggressive stay_area no_hunt` | `0 -100 9 9` | — | Iron-mineral elemental; spring basin |
| 6219 | ash cult tiered fire-pit ritualist | 165 | `is_npc aggressive stay_area no_hunt` | `0 -100 9 9` | `spec_cast_cleric` | Elite cultist; Fire-Pit Complex |
| 6220 | ash cult speaking vent herald | 165 | `is_npc aggressive stay_area no_hunt solo` | `0 -100 9 9` | `spec_breath_gas` | Elite herald; Speaking Vent approach |
| 6221 | grand vent-oracle of the ash cult | 166 | `is_npc aggressive sentinel stay_area boss no_flee no_hunt` | `0 -100 11 11` | `spec_cinderteeth_oracle` | **BOSS**; Grand Vent-Oracle's Throne; genuine geological intuition corrupted by theology |
| 6222 | sanctum scar collapse elemental | 163 | `is_npc aggressive stay_area no_hunt` | `0 -100 9 9` | — | Scar-formed entity; Scar perimeter |
| 6223 | western fold geothermal drake | 165 | `is_npc aggressive stay_area no_hunt solo` | `0 -100 9 9` | `spec_breath_fire` | Elite creature; Fold floor perimeter |

---

### Zone 7 Mobs — Ember Crown Approaches (6224-6233, 10 mobs)

| Vnum | Name | Level | Act Flags | Stats | Special | Notes |
|---:|---|---:|---|---|---|---|
| 6224 | ember crown scree titan | 165 | `is_npc aggressive stay_area no_hunt` | `0 -100 9 9` | — | Massive creature; upper scree fields |
| 6225 | ember crown glow-hawk elder | 166 | `is_npc aggressive stay_area no_hunt solo` | `0 -100 9 9` | — | Enormous heat-adapted raptor; nesting cliff |
| 6226 | ember crown glasswalk guardian | 166 | `is_npc aggressive stay_area no_hunt` | `0 -100 9 9` | — | Glasswalk zone creature; obsidian harvest area |
| 6227 | ember crown glassfall elemental | 167 | `is_npc aggressive stay_area no_hunt no_body` | `0 -100 9 9` | — | Animate glassfall cloud; glassfall zone |
| 6228 | ember crown vent-gas wraith | 167 | `is_npc aggressive stay_area no_hunt undead no_body` | `0 -100 9 9` | `spec_breath_gas` | Suffocation hazard; vent-gas inversion pockets |
| 6229 | ember crown lava serpent | 167 | `is_npc aggressive stay_area no_hunt solo` | `0 -100 9 9` | `spec_breath_fire` | Elite heat-adapted serpent; Firsttooth approach |
| 6230 | ember crown secondtooth prowler | 167 | `is_npc aggressive stay_area no_hunt` | `0 -100 9 9` | — | Large predator; Secondtooth flanking path |
| 6231 | ember crown obsidian golem | 168 | `is_npc aggressive stay_area no_hunt solo` | `0 -100 9 9` | — | Elite obsidian construct; Glasswalk Zone Entry |
| 6232 | ember crown lightning-trapped shade | 167 | `is_npc aggressive stay_area no_hunt undead no_body` | `0 -100 9 9` | `spec_cast_mage` | Ghost killed by lightning strike; lightning hazard zone |
| 6233 | ember crown ridge sentinel | 168 | `is_npc aggressive stay_area no_hunt solo` | `0 -100 9 9` | — | Elite guardian; Upper Crown Junction |

---

### Zone 8 Mobs — Ember Crown (6234-6243, 10 mobs)

| Vnum | Name | Level | Act Flags | Stats | Special | Notes |
|---:|---|---:|---|---|---|---|
| 6234 | firsttooth caldera spawn | 168 | `is_npc aggressive stay_area no_hunt` | `0 -100 9 9` | `spec_breath_fire` | Magma-formed creature; lower caldera face |
| 6235 | firsttooth scree-shelf titan | 169 | `is_npc aggressive stay_area no_hunt` | `0 -100 9 9` | — | Massive scree creature; mid-caldera |
| 6236 | firsttooth vent elemental | 169 | `is_npc aggressive stay_area no_hunt no_body` | `0 -100 9 9` | `spec_breath_fire` | Vent-gas elemental; caldera rim |
| 6237 | oracle vent pilgrim | 168 | `is_npc aggressive stay_area no_hunt` | `0 -100 9 9` | `spec_cast_cleric` | Ash Cult pilgrim at Oracle Vent; caldera south slope |
| 6238 | secondtooth caldera watcher | 168 | `is_npc aggressive sentinel stay_area boss no_flee no_hunt` | `0 -100 11 11` | `spec_cinderteeth_caldera_watcher` | **BOSS**; Secondtooth Caldera Watcher's Ground; heat-adapted apex predator |
| 6239 | caldera rim guardian | 169 | `is_npc aggressive stay_area no_hunt solo` | `0 -100 9 9` | `spec_breath_fire` | Elite guardian; caldera rim traversal |
| 6240 | ember throne servant | 169 | `is_npc aggressive stay_area no_hunt undead` | `0 -100 9 9` | `spec_cast_undead` | Undead servant of the Patriarch; inner caldera approach |
| 6241 | ember throne geological titan | 169 | `is_npc aggressive stay_area no_hunt solo` | `0 -100 9 9` | — | Elite geological construct; Antechamber |
| 6242 | the ember throne patriarch | 170 | `is_npc aggressive sentinel stay_area boss no_flee no_hunt no_body` | `0 -100 12 12` | `spec_cinderteeth_patriarch` | **FINAL BOSS**; The Ember Throne room; geological intelligence entity |
| 6243 | caldera rim lava spider | 168 | `is_npc aggressive stay_area no_hunt` | `0 -100 9 9` | `spec_poison` | Heat-adapted arthropod; caldera rim |

---

### Boss Cadence Summary

| Boss | Vnum | Room | Level | Type | Special |
|---|---:|---|---:|---|---|
| Ashfang Remnant Warlord | 6130 | 6150 | 148 | Mini-boss | `spec_cinderteeth_warlord` |
| Ironpost Warden-Commander | 6151 | 6189 | 152 | Mini-boss | `spec_cinderteeth_warden_cmd` |
| Glasswash Sulfur Fan Colossus | 6172 | 6292 | 156 | Mini-boss | `spec_cinderteeth_sulfur_colossus` |
| Bellspine Resonance Anchor Construct | 6194 | 6373 | 160 | Boss (tier 2) | `spec_cinderteeth_anchor` |
| Ghost of the Last Ventspeaker | 6212 | 6477 | 163 | Boss (tier 2) | `spec_cinderteeth_ventspeaker` |
| Grand Vent-Oracle of the Ash Cult | 6221 | 6548 | 166 | Boss (tier 2) | `spec_cinderteeth_oracle` |
| Secondtooth Caldera Watcher | 6238 | 6668 | 168 | Boss (tier 3) | `spec_cinderteeth_caldera_watcher` |
| The Ember Throne Patriarch | 6242 | 6673 | 170 | Final Boss | `spec_cinderteeth_patriarch` |

---

### Specials Assignment Summary

| Special | Assigned Mobs |
|---|---|
| `spec_cinderteeth_warlord` | 6130 (mini-boss) |
| `spec_cinderteeth_warden_cmd` | 6151 (mini-boss) |
| `spec_cinderteeth_sulfur_colossus` | 6172 (mini-boss) |
| `spec_cinderteeth_anchor` | 6194 (boss) |
| `spec_cinderteeth_ventspeaker` | 6212 (boss) |
| `spec_cinderteeth_oracle` | 6221 (boss) |
| `spec_cinderteeth_caldera_watcher` | 6238 (boss) |
| `spec_cinderteeth_patriarch` | 6242 (final boss) |
| `spec_cast_cleric` | 6129, 6150, 6205, 6215, 6219, 6237 |
| `spec_cast_mage` | 6171, 6187, 6201, 6211, 6232 |
| `spec_cast_undead` | 6128, 6138, 6141, 6147, 6153, 6156, 6162, 6174, 6178, 6182, 6188, 6189, 6191, 6197, 6200, 6207, 6208, 6216, 6228, 6232, 6240 |
| `spec_breath_fire` | 6140, 6177, 6179, 6209, 6213, 6223, 6229, 6234, 6236, 6239 |
| `spec_breath_gas` | 6170, 6172 (mini-boss), 6220, 6228 |
| `spec_breath_any` | 6149, 6202, 6238 (boss) |
| `spec_breath_lightning` | 6196 |
| `spec_poison` | 6128, 6135, 6168, 6169, 6243 |
| `spec_fido` | 6137, 6152 |

---

## Full Object Roster (6124-6253, 130 objects)

Object vnums are assigned sequentially from 6124, overlapping the room and mob vnum ranges (cross-type overlap is legal per spec). Each entry lists: vnum, name, item type, wear flags, extra flags, weight, value3 (weapons only), and notes.

**Weight conventions per spec:**
- 1-5: caster item
- 6-10: melee item
- 11-15: tank item

**Item stat policy:** No fixed stats in area file. Stats generated at runtime from item level. Use `L` entry to specify level. All objects include `take` in wear flags.

**Boss drop policy:** All items droppable by a boss mob must include `ITEM_BOSS` in extra flags.
**Loot table policy:** Items on mob loot tables must include `ITEM_LOOT` in extra flags.
**Object names must be unique within the area.**

---

### Keys (6124-6128)

| Vnum | Name | Type | Wear | Extra | Wt | Notes |
|---:|---|---|---|---|---:|---|
| 6124 | ashfang stronghold iron key | key | take | — | 1 | Dropped by Ashfang Sentry Captain; unlocks Stronghold Gate (6147) |
| 6125 | ironpost archive sealed key | key | take | — | 1 | Dropped by Ironpost Warden-Commander boss; unlocks Archive Sealed Wing (6188) |
| 6126 | bellspine vault resonance key | key | take | — | 1 | Dropped by Bellspine Resonance Anchor Construct boss; unlocks further inner Conclave vaults |
| 6127 | ash cult doctrine archive key | key | take | — | 1 | Dropped by Ash Cult Archivist; unlocks Doctrine Archive cabinet (6541) |
| 6128 | ember throne inner sanctum key | key | take | — | 1 | Dropped by Secondtooth Caldera Watcher boss; unlocks Ember Throne Antechamber (6672) |

---

### Weapons (6129-6158)

| Vnum | Name | Type | Wear | Extra | Wt | v3 | Notes |
|---:|---|---|---|---|---:|---:|---|
| 6129 | ashfang remnant iron claw blade | weapon | hold take | ITEM_LOOT | 8 | 5 (claw) | Ashfang melee drop; notched iron blade shaped for raking |
| 6130 | ashfang warlord's judgment axe | weapon | hold take | ITEM_LOOT ITEM_BOSS | 10 | 3 (slash) | Boss drop: Ashfang Warlord; territorial execution tool |
| 6131 | blackstep basalt spike | weapon | hold take | ITEM_LOOT | 9 | 11 (pierce) | Ashrunner territory drop; volcanic basalt point |
| 6132 | chalkwind saddle pick | weapon | hold take | ITEM_LOOT | 8 | 11 (pierce) | Saddle creature drop; saddle ice-breaking tool used as weapon |
| 6133 | ironpost warden's patrol halberd | weapon | hold take | ITEM_LOOT ITEM_BOSS two-handed | 9 | 11 (pierce) | Boss drop: Ironpost Warden-Commander; historic route house patrol weapon |
| 6134 | chalk-ash encrusted mace | weapon | hold take | ITEM_LOOT | 9 | 7 (pound) | Chalk-ash elemental drop; mineral-encrusted bludgeon |
| 6135 | chalkwind resonance tuning fork | weapon | hold take | ITEM_LOOT wand | 3 | 7 (pound) | Resonance wyrm drop; wand; Conclave-era resonance instrument |
| 6136 | glasswash obsidian cleaver | weapon | hold take | ITEM_LOOT | 8 | 3 (slash) | Fan creature drop; banded obsidian blade, North Fan quality |
| 6137 | sulfur fan crystalline staff | weapon | hold take | ITEM_LOOT two-handed | 4 | 7 (pound) | Sulfur zone drop; wand; crystal-growth staff |
| 6138 | sulfur fan colossus spine | weapon | hold take | ITEM_LOOT ITEM_BOSS two-handed | 10 | 7 (pound) | Boss drop: Sulfur Fan Colossus; massive encrusted spine extracted from the colossus form |
| 6139 | bellspine resonance blade | weapon | hold take | ITEM_LOOT | 7 | 3 (slash) | Bellspine creature drop; blade that hums at certain frequencies |
| 6140 | conclave crystal anchor shard blade | weapon | hold take | ITEM_LOOT | 4 | 1 (slice) | Crystal anchor shard drop; caster weapon; still faintly resonant |
| 6141 | bellspine anchor construct arm | weapon | hold take | ITEM_LOOT ITEM_BOSS two-handed | 9 | 7 (pound) | Boss drop: Resonance Anchor Construct; a severed manipulator limb from the construct |
| 6142 | ash cult ritual ignition rod | weapon | hold take | ITEM_LOOT wand | 3 | 7 (pound) | Ash Cult caster drop; fire-starting wand with doctrinal inscriptions |
| 6143 | ash cult tiered-fire ceremonial blade | weapon | hold take | ITEM_LOOT | 7 | 2 (stab) | Ash Cult melee drop; ritual blade from fire-pit rite |
| 6144 | ventspeaker name-stone shard blade | weapon | hold take | ITEM_LOOT ITEM_BOSS | 5 | 1 (slice) | Boss drop: Ghost of the Last Ventspeaker; a name-stone fragment shaped by unknown means into a weapon |
| 6145 | western fold geothermal lance | weapon | hold take | ITEM_LOOT two-handed | 9 | 11 (pierce) | Fold creature drop; heat-forged iron lance |
| 6146 | grand vent-oracle's pronouncement staff | weapon | hold take | ITEM_LOOT ITEM_BOSS wand | 4 | 7 (pound) | Boss drop: Grand Vent-Oracle; wand; staff that channels speaking vent gas into targeted eruptions |
| 6147 | ember crown scree-shard whip | weapon | hold take | ITEM_LOOT | 7 | 4 (whip) | Crown approach drop; obsidian-tipped leather lash |
| 6148 | glow-hawk talon sword | weapon | hold take | ITEM_LOOT | 7 | 5 (claw) | Glow-hawk elder drop; talons bound to a heat-resistant haft |
| 6149 | firsttooth caldera iron sword | weapon | hold take | ITEM_LOOT | 8 | 3 (slash) | Caldera spawn drop; iron blade forge-quenched in caldera water |
| 6150 | ember crown lava serpent fang | weapon | hold take | ITEM_LOOT | 7 | 10 (bite) | Lava serpent drop; venomous caldera fang mounted on a hilt |
| 6151 | secondtooth watcher claw | weapon | hold take | ITEM_LOOT ITEM_BOSS fist | 10 | 0 (hit) | Boss drop: Secondtooth Caldera Watcher; fist weapon; enormous heat-hardened claw |
| 6152 | ember throne patriarch shard | weapon | hold take | ITEM_LOOT ITEM_BOSS | 5 | 6 (blast) | Final Boss drop: Ember Throne Patriarch; weapon; obsidian fragment infused with geological intelligence |
| 6153 | bellspine null cairn pick | weapon | hold take | ITEM_LOOT | 8 | 11 (pierce) | Bellspine null cairn shade drop; old warden excavation pick |
| 6154 | chalkwind saddle frost-blade | weapon | hold take | ITEM_LOOT | 7 | 1 (slice) | Chalkwind ice-shelf shade drop; blade kept cold by old winter exposure |
| 6155 | ash cult glass-blade | weapon | hold take | ITEM_LOOT | 6 | 1 (slice) | Ash Cult ash-walker drop; obsidian glass knife |
| 6156 | glasswash iron-moss mace | weapon | hold take | ITEM_LOOT | 9 | 7 (pound) | Iron-moss elemental drop; mineral-encrusted bludgeon |
| 6157 | bellspine fault-crack wyrm fang | weapon | hold take | ITEM_LOOT | 8 | 10 (bite) | Fault-crack wyrm drop; magma-coated fang |
| 6158 | cinderteeth caravan ghost spear | weapon | hold take | ITEM_LOOT undead | 8 | 11 (pierce) | Keeper survey shade / ghost drop; ghostly spear with historical weight |

---

### Armor — Head/Face/Neck (6159-6168)

| Vnum | Name | Type | Wear | Extra | Wt | Notes |
|---:|---|---|---|---|---:|---|
| 6159 | ashfang remnant bone mask | armor | face take | ITEM_LOOT | 3 | Ashfang facial armor; carved bone from mountain prey |
| 6160 | chalkwind chalk-crusted helm | armor | head take | ITEM_LOOT | 12 | Heavy saddle-region helm; chalk-white mineral crust deposit |
| 6161 | ironpost warden's command circlet | armor | head take | ITEM_LOOT ITEM_BOSS | 6 | Boss drop: Ironpost Warden-Commander; authority circlet with route log insignia |
| 6162 | glasswash obsidian shard crown | armor | head take | ITEM_LOOT | 5 | Caster; obsidian-fragment head piece; sharp-edged with visual menace |
| 6163 | sulfur fan colossus crystal collar | armor | neck take | ITEM_LOOT ITEM_BOSS | 8 | Boss drop: Sulfur Fan Colossus; sulfur crystal neck piece |
| 6164 | bellspine resonance anchor headband | armor | head take | ITEM_LOOT | 3 | Crystal anchor shard drop; crystal-studded band that filters resonance |
| 6165 | ash cult tiered fire hood | armor | head take | ITEM_LOOT | 4 | Ash Cult caster head covering; fire-ashen cloth |
| 6166 | last ventspeaker's name-stone headpiece | armor | head take | ITEM_LOOT ITEM_BOSS | 5 | Boss drop: Ghost of the Last Ventspeaker; carved name-stone fragments bound to temple leather |
| 6167 | grand vent-oracle's volcanic glass mask | armor | face take | ITEM_LOOT ITEM_BOSS | 4 | Boss drop: Grand Vent-Oracle; obsidian gas-filter mask from decades of vent breathing |
| 6168 | ember throne patriarch crown shard | armor | head take | ITEM_LOOT ITEM_BOSS | 6 | Final Boss drop: shard of the Ember Throne's mineral crown formation |

---

### Armor — Body/About/Shoulders (6169-6178)

| Vnum | Name | Type | Wear | Extra | Wt | Notes |
|---:|---|---|---|---|---:|---|
| 6169 | ashfang remnant scavenged brigandine | armor | body take | ITEM_LOOT | 11 | Ashfang standard body armor; mismatched salvage plates |
| 6170 | chalkwind saddle ash-coat | armor | about take | ITEM_LOOT | 7 | Chalk-ash resistant travel coat; saddle route gear |
| 6171 | ironpost warden's archive tabard | armor | body take | ITEM_LOOT ITEM_BOSS | 9 | Boss drop: Ironpost Warden-Commander; tabard with route schedule inscribed |
| 6172 | glasswash obsidian-plate cuirass | armor | body take | ITEM_LOOT | 13 | Fan creature drop; tank weight; obsidian-reinforced chest plate |
| 6173 | ash cult doctrine cloak | armor | about take | ITEM_LOOT | 4 | Ash Cult drop; dark cloak with doctrinal symbols woven throughout |
| 6174 | bellspine resonance dampening mantle | armor | shoulders take | ITEM_LOOT | 7 | Resonance zone drop; shoulder piece that partially insulates from sonic effects |
| 6175 | western fold thermophilic hide coat | armor | about take | ITEM_LOOT | 8 | Western Fold creature drop; heat-resistant fungal-fiber coat |
| 6176 | grand vent-oracle's speaking vent vestment | armor | body take | ITEM_LOOT ITEM_BOSS | 5 | Boss drop: Grand Vent-Oracle; ceremonial caster vestment from decades of vent ritual |
| 6177 | ember crown glasswalk pauldrons | armor | shoulders take | ITEM_LOOT | 10 | Crown approach drop; volcanic glass shoulder guards |
| 6178 | ember throne patriarch stone mantle | armor | about take | ITEM_LOOT ITEM_BOSS | 12 | Final Boss drop: mineral-encrusted stone mantle from the Throne itself |

---

### Armor — Arms/Wrist/Hands (6179-6188)

| Vnum | Name | Type | Wear | Extra | Wt | Notes |
|---:|---|---|---|---|---:|---|
| 6179 | ashfang remnant barbed vambraces | armor | arms take | ITEM_LOOT | 9 | Ashfang drop; barbed iron arm protection for close fighting |
| 6180 | chalkwind saddle chalk-wrapped bracers | armor | wrist take | ITEM_LOOT | 7 | Saddle zone drop; chalk-mineral hardened leather bracers |
| 6181 | ironpost route log gauntlets | armor | hands take | ITEM_LOOT ITEM_BOSS | 9 | Boss drop: Ironpost Warden-Commander; gauntlets stamped with route archive seal |
| 6182 | glasswash fan obsidian gloves | armor | hands take | ITEM_LOOT | 6 | Fan zone drop; obsidian-plate knuckle reinforcement; melee weight |
| 6183 | bellspine crystal anchor bracers | armor | wrist take | ITEM_LOOT | 3 | Crystal anchor drop; caster; wrist fragments from resonance anchor |
| 6184 | ash cult fire-keeper's ashen gloves | armor | hands take | ITEM_LOOT | 7 | Ash Cult Fire-Keeper drop; ash-insulated gloves for tending hot vents |
| 6185 | last ventspeaker's observation bracers | armor | wrist take | ITEM_LOOT ITEM_BOSS | 4 | Boss drop: Ghost of the Last Ventspeaker; carved bracers with vent-reading symbols |
| 6186 | ember crown scree-grip gauntlets | armor | hands take | ITEM_LOOT | 13 | Crown approach drop; tank weight; heavy stone-grip gauntlets for scree climbing |
| 6187 | ember throne patriarch basalt gauntlets | armor | hands take | ITEM_LOOT ITEM_BOSS | 12 | Final Boss drop: mineral-accretion gauntlets |
| 6188 | secondtooth watcher hide bracers | armor | wrist take | ITEM_LOOT ITEM_BOSS | 10 | Boss drop: Secondtooth Caldera Watcher; heat-hardened apex creature hide |

---

### Armor — Waist/Legs/Feet (6189-6198)

| Vnum | Name | Type | Wear | Extra | Wt | Notes |
|---:|---|---|---|---|---:|---|
| 6189 | ashfang remnant iron-shod boots | armor | feet take | ITEM_LOOT | 10 | Ashfang footwear; metal-reinforced for foothills terrain |
| 6190 | chalkwind chalk-ash greaves | armor | legs take | ITEM_LOOT | 11 | Saddle zone drop; chalk-hardened leg protection |
| 6191 | glasswash fan glass-sole sandals | armor | feet take | ITEM_LOOT | 6 | Fan zone drop; melee; glass-walking footwear with obsidian-resistant soles |
| 6192 | bellspine resonance dampening boots | armor | feet take | ITEM_LOOT | 8 | Resonance zone drop; vibration-insulated boot soles |
| 6193 | ash cult ritual foot wraps | armor | feet take | ITEM_LOOT | 2 | Ash Cult drop; caster; ash-soaked ceremonial foot bindings |
| 6194 | ash cult speaking vent belt | armor | waist take | ITEM_LOOT | 5 | Ash Cult Herald drop; gas-collection belt pouches |
| 6195 | western fold iron-moss leg guards | armor | legs take | ITEM_LOOT | 11 | Fold creature drop; iron mineral accretion leg protection |
| 6196 | grand vent-oracle's geology belt | armor | waist take | ITEM_LOOT ITEM_BOSS | 6 | Boss drop: Grand Vent-Oracle; geological sample-pouch belt |
| 6197 | ember crown caldera-stride boots | armor | feet take | ITEM_LOOT | 8 | Crown approach drop; heat-insulated caldera approach footwear |
| 6198 | ember throne patriarch stone greaves | armor | legs take | ITEM_LOOT ITEM_BOSS | 14 | Final Boss drop: stone-mineral leg armor |

---

### Shields and Held Items (6199-6208)

| Vnum | Name | Type | Wear | Extra | Wt | Notes |
|---:|---|---|---|---|---:|---|
| 6199 | ashfang remnant iron buckler | armor | hold take | ITEM_LOOT buckler | 9 | Ashfang drop; small iron buckler |
| 6200 | chalkwind saddle stone shield | armor | hold take | ITEM_LOOT | 13 | Saddle zone drop; carved chalk-stone shield |
| 6201 | glasswash obsidian tower shield | armor | hold take | ITEM_LOOT | 13 | Fan zone drop; obsidian-faced tower shield |
| 6202 | bellspine crystal anchor shield | armor | hold take | ITEM_LOOT | 7 | Bellspine drop; melee shield reinforced with crystal anchor fragments |
| 6203 | ironpost archive seal | armor | hold take | ITEM_LOOT ITEM_BOSS | 4 | Boss drop: Ironpost Warden-Commander; held ceremonial seal of archive authority |
| 6204 | ash cult doctrine board | armor | hold take | ITEM_LOOT | 3 | Ash Cult caster drop; held; doctrinal scripture on carved basalt |
| 6205 | ash cult grand vent focus crystal | armor | hold take | ITEM_LOOT ITEM_BOSS | 3 | Boss drop: Grand Vent-Oracle; caster held; vent-gas condenser crystal |
| 6206 | ember crown volcanic glass kite shield | armor | hold take | ITEM_LOOT | 9 | Crown approach drop; Ember Crown obsidian-faced shield |
| 6207 | secondtooth caldera volcanic buckler | armor | hold take | ITEM_LOOT ITEM_BOSS buckler | 8 | Boss drop: Secondtooth Caldera Watcher; caldera-stone buckler |
| 6208 | ember throne patriarch geological orb | armor | hold take | ITEM_LOOT ITEM_BOSS | 4 | Final Boss drop: caster held; geological intelligence condenser |

---

### Finger/Ear/Neck/Wrist Jewelry (6209-6218)

| Vnum | Name | Type | Wear | Extra | Wt | Notes |
|---:|---|---|---|---|---:|---|
| 6209 | ashfang remnant iron band | armor | finger take | ITEM_LOOT | 2 | Ashfang drop; territorial gang ring |
| 6210 | chalkwind chalk-crystal ring | armor | finger take | ITEM_LOOT | 1 | Saddle zone drop; chalk-crystal set in copper |
| 6211 | glasswash obsidian signet ring | armor | finger take | ITEM_LOOT | 2 | Fan zone drop; banded obsidian signet |
| 6212 | bellspine resonance crystal earring | armor | ear take | ITEM_LOOT | 1 | Bellspine drop; crystal anchor fragment as ear ornament |
| 6213 | ash cult fire-brand necklace | armor | neck take | ITEM_LOOT | 2 | Ash Cult drop; ember-stone pendant on a sinew cord |
| 6214 | ironpost warden's route record necklace | armor | neck take | ITEM_LOOT ITEM_BOSS | 3 | Boss drop: Ironpost Warden-Commander; carved route schedule pendant |
| 6215 | ventspeaker's judgment-stone ring | armor | finger take | ITEM_LOOT ITEM_BOSS | 1 | Boss drop: Ghost of the Last Ventspeaker; carved name-stone ring with vent-reading symbol |
| 6216 | ember crown caldera glow ring | armor | finger take | ITEM_LOOT | 1 | Crown approach drop; iron ring with embedded glow-mineral |
| 6217 | secondtooth watcher heat-gem necklace | armor | neck take | ITEM_LOOT ITEM_BOSS | 3 | Boss drop: Secondtooth Caldera Watcher; heat-gem pendant |
| 6218 | ember throne patriarch obsidian sigil ring | armor | finger take | ITEM_LOOT ITEM_BOSS | 1 | Final Boss drop: obsidian ring inscribed with geological judgment symbol |

---

### Aura/Halo/Wings/Horn (6219-6223)

| Vnum | Name | Type | Wear | Extra | Wt | Notes |
|---:|---|---|---|---|---:|---|
| 6219 | ashfall monsoon ash halo | armor | halo take | ITEM_LOOT | 1 | Chalkwind zone drop; pale chalk-ash ring |
| 6220 | conclave resonance crystal aura | armor | aura take | ITEM_LOOT | 2 | Bellspine drop; faint crystal luminescence aura |
| 6221 | ash cult speaking vent aura | armor | aura take | ITEM_LOOT ITEM_BOSS | 2 | Boss drop: Grand Vent-Oracle; volcanic gas-and-glow aura |
| 6222 | glow-hawk feather wings | armor | wings take | ITEM_LOOT | 3 | Glow-hawk elder drop; wing-span feathers |
| 6223 | ember throne patriarch caldera halo | armor | halo take | ITEM_LOOT ITEM_BOSS | 2 | Final Boss drop: magma-light crown effect |

---

### Light Sources (6224-6226)

| Vnum | Name | Type | Wear | Extra | Wt | Notes |
|---:|---|---|---|---|---:|---|
| 6224 | cinderteeth glow-vent lantern | light | take | — | 3 | Light source; glow-vent glass lantern; sold/dropped |
| 6225 | conclave resonance glow-crystal | light | take | ITEM_LOOT | 2 | Bellspine crystal anchor drop; light source from Conclave era |
| 6226 | ember crown caldera torch | light | take | — | 2 | Light source; caldera-oil torch; standard summit gear |

---

### Potions and Consumables (6227-6232)

| Vnum | Name | Type | Wear | Extra | Wt | Notes |
|---:|---|---|---|---|---:|---|
| 6227 | ventspeaker's sulfur-filtered water | potion | take | — | 2 | Healing; safe water filtered at Red Sister Cisterns |
| 6228 | ash cult fire-rite potion | potion | take | ITEM_LOOT | 1 | Ash Cult drop; combat potion |
| 6229 | conclave crystal resonance tincture | potion | take | ITEM_LOOT | 1 | Bellspine crystal drop; restoration potion |
| 6230 | cinderteeth sulfur bloom alchemical | potion | take | — | 1 | General area potion; alchemical benefit |
| 6231 | glasswash mirror-obsidian extract | potion | take | — | 1 | General area potion |
| 6232 | ember crown thermal spring draught | potion | take | — | 1 | High-elevation healing draught |

---

### Quest Items (6233-6237)

| Vnum | Name | Type | Wear | Extra | Wt | Notes |
|---:|---|---|---|---|---:|---|
| 6233 | ironpost archive route log fragment | quest | take | ITEM_NOSAC ITEM_NOLOOT | 1 | Quest: Ironpost archive quest target; recovered route log |
| 6234 | ventspeaker name-stone rubbing | quest | take | ITEM_NOSAC ITEM_NOLOOT | 1 | Quest: cartography / lore recovery quest target |
| 6235 | conclave crystal anchor fragment | quest | take | ITEM_NOSAC ITEM_NOLOOT | 1 | Quest: Bellspine anchor investigation quest target |
| 6236 | ash cult doctrine transcription | quest | take | ITEM_NOSAC ITEM_NOLOOT | 1 | Quest: cult intelligence quest target |
| 6237 | sulfur bloom alchemical sample | quest | take | ITEM_NOSAC ITEM_NOLOOT | 1 | Quest: Yellow Garden material recovery quest target |

---

### Treasure / Misc (6238-6243)

| Vnum | Name | Type | Wear | Extra | Wt | Notes |
|---:|---|---|---|---|---:|---|
| 6238 | cinderteeth mirror-quality obsidian nodule | treasure | take | — | 3 | Ember Crown treasure; high-value vendor item |
| 6239 | cinderteeth sulfur crystal cluster | treasure | take | — | 2 | Yellow Garden treasure; alchemical trade goods |
| 6240 | conclave crystal conduit shard | treasure | take | — | 1 | Bellspine treasure; collector value |
| 6241 | ventspeaker name-stone fragment | treasure | take | — | 2 | Sacred site treasure; scholarly collector value |
| 6242 | ashfall monsoon ash-core sample | treasure | take | — | 1 | Glasswash Fans treasure; geological record |
| 6243 | ironpost route log volume | treasure | take | — | 3 | Ironpost archive treasure; historical record |

---

### Boss Gear Level Summary (`L` entries)

All equipment items receive `L` entries matching the zone difficulty of their drop source:
- Zone 1 drops (Ashfang): `L 148`
- Zone 2 drops (Chalkwind/Ironpost boss): `L 150`–`L 152`
- Zone 3 drops (Glasswash/Sulfur boss): `L 154`–`L 156`
- Zone 4 drops (Bellspine/Anchor boss): `L 158`–`L 160`
- Zone 5 drops (Fold Approach/Ventspeaker boss): `L 161`–`L 163`
- Zone 6 drops (Fold/Vent-Oracle boss): `L 163`–`L 166`
- Zone 7 drops (Crown Approaches): `L 165`–`L 168`
- Zone 8 drops (Crown/Patriarch boss): `L 168`–`L 170`

---

## Quest Design

Per `docs/area_file_spec.md` §13.2: every area must include at least one quest targeting a boss; at least one cartography quest; the final quest in any chain must reward equipment; any quest targeting a boss must reward equipment.

### Quest Set: The Cinderteeth Mountains (25 quests, files 81–105, IDs 80–104)

Quest givers: Kiess postmaster (3340), Kowloon postmaster (3440), Midgaard postmaster (3015), Mafdet postmaster (3539), Rakuen dispatch registrar (4339). All 8 bosses have dedicated kill quests with equipment rewards. One standalone cartography quest (type 4). Seven chains (2–3 quests each). Five standalone non-chain quests.

---

#### Chain B — Kiess Ashfang Removal (files 81–82, IDs 80–81)

**ID 80 (file 81): "Cinderteeth foothills raid suppression"**
- Prereq: none | Type: 1 (kill variety) | Offerer: 3340 (Kiess)
- Targets: 6125 (ashfang remnant raider), 6126 (ashfang remnant enforcer), 6143 (ashfang remnant tracker)
- Level: 120–155 | Gold: 7000 | QP: 6 | Exp: 3200000
- Notes: Zone 1 chain start. Confirms tactical withdrawal is real or a Blood Oath hold-pattern.

**ID 81 (file 82): "Warlord's blood oath, cancelled" [BOSS 6130, EQUIP]**
- Prereq: 80 | Type: 1 | Offerer: 3340 (Kiess)
- Targets: 6130 (ashfang remnant warlord)
- Level: 120–155 | Gold: 9500 | QP: 8 | Exp: 4200000
- Equip: iron-studded bracer stamped with an Ashfang blood-oath seal (WEAR_WRIST=2048)

---

#### Chain C — Kiess Chalkwind Archive (files 83–85, IDs 82–84)

**ID 82 (file 83): "Chalkwind archive undead threat survey"**
- Prereq: none | Type: 1 | Offerer: 3340 (Kiess)
- Targets: 6147 (chalkwind saddle wraith), 6153 (chalkwind ice-shelf shade), 6162 (old tariff enforcer ghost)
- Level: 125–160 | Gold: 7500 | QP: 6 | Exp: 3400000

**ID 83 (file 84): "Ironpost archive corridor: revenant suppression"**
- Prereq: 82 | Type: 1 | Offerer: 3340 (Kiess)
- Targets: 6150 (ironpost route house revenant), 6156 (muster yard requisitions ghost), 6154 (chalk-ash drift crawler)
- Level: 125–160 | Gold: 8000 | QP: 7 | Exp: 3700000

**ID 84 (file 85): "Warden-commander interdiction: Ironpost Archive" [BOSS 6151, EQUIP]**
- Prereq: 83 | Type: 1 | Offerer: 3340 (Kiess)
- Targets: 6151 (ironpost warden-commander)
- Level: 125–160 | Gold: 11000 | QP: 9 | Exp: 4800000
- Equip: tarnished ironpost archive seal pendant (WEAR_NECK=128)

---

#### Chain D — Kowloon Glasswash Research (files 86–87, IDs 85–86)

**ID 85 (file 86): "Glasswash hostile element assessment"**
- Prereq: none | Type: 1 | Offerer: 3440 (Kowloon)
- Targets: 6164 (glasswash fan crawler), 6166 (glasswash mirror-blind hunter), 6171 (glasswash crystal resonance sprite)
- Level: 130–160 | Gold: 7500 | QP: 6 | Exp: 3500000

**ID 86 (file 87): "Glasswash colossus elimination" [BOSS 6172, EQUIP]**
- Prereq: 85 | Type: 1 | Offerer: 3440 (Kowloon)
- Targets: 6172 (glasswash sulfur fan colossus)
- Level: 130–160 | Gold: 11500 | QP: 9 | Exp: 5000000
- Equip: Glasswash obsidian ring threaded with volcanic sulfur crystal (WEAR_FINGER=8192)

---

#### Chain E — Kowloon Bellspine Resonance (files 88–89, IDs 87–88)

**ID 87 (file 88): "Bellspine resonance entity clearance"**
- Prereq: none | Type: 1 | Offerer: 3440 (Kowloon)
- Targets: 6184 (bellspine resonance serpent), 6187 (bellspine crystal anchor shard), 6188 (bellspine resonance wraith)
- Level: 135–165 | Gold: 8000 | QP: 6 | Exp: 3800000

**ID 88 (file 89): "Anchor construct decommission" [BOSS 6194, EQUIP]**
- Prereq: 87 | Type: 1 | Offerer: 3440 (Kowloon)
- Targets: 6194 (bellspine resonance anchor construct)
- Level: 135–165 | Gold: 12000 | QP: 10 | Exp: 5200000
- Equip: arc-etched Bellspine resonance crystal bracer (WEAR_WRIST=2048)

---

#### Chain A — Midgaard Ash Cult Campaign (files 90–91, IDs 89–90)

**ID 89 (file 90): "Western Fold doctrine apparatus: institutional assessment"**
- Prereq: none | Type: 1 | Offerer: 3015 (Midgaard)
- Targets: 6214 (ash cult senior zealot), 6215 (ash cult doctrine-speaker), 6216 (ash cult archivist)
- Level: 140–170 | Gold: 8500 | QP: 7 | Exp: 4200000

**ID 90 (file 91): "Vent-Oracle silencing" [BOSS 6221, EQUIP]**
- Prereq: 89 | Type: 1 | Offerer: 3015 (Midgaard)
- Targets: 6221 (grand vent-oracle of the ash cult)
- Level: 140–170 | Gold: 13000 | QP: 10 | Exp: 5500000
- Equip: oracle's volcanic glass speaking mask (WEAR_FACE=16)

---

#### Chain F — Mafdet Caldera Watcher Campaign (files 92–94, IDs 91–93)

**ID 91 (file 92): "Ember Crown approach: transit threat survey"**
- Prereq: none | Type: 1 | Offerer: 3539 (Mafdet)
- Targets: 6224 (ember crown scree titan), 6227 (ember crown glassfall elemental), 6228 (ember crown vent-gas wraith)
- Level: 145–170 | Gold: 8000 | QP: 6 | Exp: 4000000

**ID 92 (file 93): "Ember Crown lava serpent cull"**
- Prereq: 91 | Type: 3 (kill count, 3 kills) | Offerer: 3539 (Mafdet)
- Target: 6229 (ember crown lava serpent)
- Level: 145–170 | Gold: 8500 | QP: 7 | Exp: 4200000

**ID 93 (file 94): "Secondtooth caldera watcher: territorial claim broken" [BOSS 6238, EQUIP]**
- Prereq: 92 | Type: 1 | Offerer: 3539 (Mafdet)
- Targets: 6238 (secondtooth caldera watcher)
- Level: 148–170 | Gold: 14000 | QP: 11 | Exp: 5800000
- Equip: secondtooth caldera heat-scaled pelt mantle (WEAR_SHOULDERS=512)

---

#### Chain G — Rakuen Ventspeaker Return (files 95–96, IDs 94–95)

**ID 94 (file 95): "Western Fold approach: bound spirit clearance"**
- Prereq: none | Type: 1 | Offerer: 4339 (Rakuen)
- Targets: 6207 (western fold approach warden ghost), 6208 (ventspeaker vent platform spirit), 6211 (conclave survey instrument shade)
- Level: 135–165 | Gold: 7500 | QP: 6 | Exp: 3900000

**ID 95 (file 96): "Oracle ground reckoning: Fold approach enforcement" [CHAIN END EQUIP]**
- Prereq: 94 | Type: 1 | Offerer: 4339 (Rakuen)
- Targets: 6209 (ash cult fire-keeper), 6210 (ash cult doctrinal enforcer), 6213 (western fold approach basilisk)
- Level: 135–165 | Gold: 9000 | QP: 8 | Exp: 4500000
- Equip: worn ventspeaker cairn-name token in Old Cindertongue (WEAR_FINGER=8192)

---

#### Standalone Quests (files 97–105, IDs 96–104)

**ID 96 (file 97): "Cinderteeth Mountains cartography commission" [CARTOGRAPHY, Rakuen]**
- Prereq: none | Type: 4 (cartography) | Offerer: 4339 (Rakuen)
- Target: 6124 (first room — full area survey)
- Level: 125–170 | Gold: 22000 | QP: 30 | Exp: 4500000
- Notes: Cannot be part of a chain. Maps all 8 zones of the range for Ventspeaker refugee route planning.

**ID 97 (file 98): "Ash Cult forward position: zealot reduction" [Midgaard]**
- Prereq: none | Type: 3 (kill count, 6 kills) | Offerer: 3015 (Midgaard)
- Target: 6204 (ash cult forward zealot)
- Level: 140–170 | Gold: 8000 | QP: 6 | Exp: 4100000

**ID 98 (file 99): "Ember Throne Patriarch: geological verdict appealed" [BOSS 6242, EQUIP, Midgaard]**
- Prereq: none | Type: 1 | Offerer: 3015 (Midgaard)
- Targets: 6242 (the ember throne patriarch)
- Level: 150–170 | Gold: 16000 | QP: 12 | Exp: 6000000
- Equip: ember throne patriarch obsidian sigil ring (WEAR_FINGER=8192)

**ID 99 (file 100): "Foothills wildlife verification: Cinderteeth entry zone" [Kiess]**
- Prereq: none | Type: 1 | Offerer: 3340 (Kiess)
- Targets: 6131 (cinderteeth ashrunner), 6132 (cinderteeth mountain bear), 6140 (thermal spring basilisk)
- Level: 120–155 | Gold: 6500 | QP: 5 | Exp: 3000000

**ID 100 (file 101): "Glasswash extraction site: labor ghost suppression" [Kowloon]**
- Prereq: none | Type: 3 (kill count, 5 kills) | Offerer: 3440 (Kowloon)
- Target: 6174 (glasswash extraction site haunt)
- Level: 130–160 | Gold: 7000 | QP: 6 | Exp: 3600000

**ID 101 (file 102): "Mafdet mineral route: sulfur element threat reduction" [Mafdet]**
- Prereq: none | Type: 1 | Offerer: 3539 (Mafdet)
- Targets: 6169 (glasswash acid-seep leech), 6170 (glasswash sulfur elemental), 6173 (glasswash pumice drifter)
- Level: 135–165 | Gold: 7500 | QP: 6 | Exp: 3800000

**ID 102 (file 103): "Ghost of the last Ventspeaker: oracle ground cleared" [BOSS 6212, EQUIP, Mafdet]**
- Prereq: none | Type: 1 | Offerer: 3539 (Mafdet)
- Targets: 6212 (ghost of the last ventspeaker)
- Level: 140–170 | Gold: 13000 | QP: 10 | Exp: 5500000
- Equip: last ventspeaker's name-stone arm binding (WEAR_ARMS=1024)

**ID 103 (file 104): "Bellspine resonance locust suppression" [Rakuen]**
- Prereq: none | Type: 3 (kill count, 6 kills) | Offerer: 4339 (Rakuen)
- Target: 6198 (bellspine resonance locust swarm)
- Level: 135–165 | Gold: 7000 | QP: 6 | Exp: 3700000

**ID 104 (file 105): "Cinderteeth entry zone: volcanic creature survey" [Rakuen]**
- Prereq: none | Type: 1 | Offerer: 4339 (Rakuen)
- Targets: 6134 (blackstep basalt sentinel), 6135 (blackstep vent-crab swarm), 6136 (cinderpine resin-hawk)
- Level: 120–155 | Gold: 6500 | QP: 5 | Exp: 3000000

---

## Reset Philosophy

### Population Density
The area uses a **descending density** model: the Blackstep Threshold and Chalkwind Saddles are the most densely populated zones. The Glasswash Fans have moderate mob density with wide spacing on fan surfaces. The Bellspine's resonance passes have concentrated but carefully positioned mobs (sentinel-heavy in sub-regions). The Western Fold zones are sparse on the approach and dense inside the Ash Cult stronghold. The Ember Crown approaches are moderately dense; the Crown itself is sparse with elite encounters only.

### Reset Cadence
`F 12` (12-minute reset cycle). High-level area; players are expected to take longer per encounter. 12 minutes allows partial respawn without punishing deliberate clearing.

### Reset Notes by Zone

**Zone 1 (Blackstep):** Ashfang mobs reset densely in the canyon and outer camp rooms. Mountain bears reset in the Bear Territory sub-region only. Ashrunners reset on Warren Flats. Boss (6130) resets in Command Chamber (6150) only, limit 1.

**Zone 2 (Chalkwind):** Mountain bears reset on Rock Ledge and scree zone rooms. Route house revenants reset in Ironpost upper floor and Muster Yard rooms. Saddle wraiths reset in Long Saddle subsidence areas. Boss (6151) resets in Archive Sealed Wing (6189) only, limit 1.

**Zone 3 (Glasswash):** Crystal resonance sprites reset in Yellow Garden rooms. Glasswash fan creatures reset across fan surface rooms. Extraction site haunts reset in quarry interior rooms. Boss (6172) resets in Sulfur Colossus Chamber (6292) only, limit 1.

**Zone 4 (Bellspine):** Resonance serpents reset in pass traversal rooms. Crystal anchor shards reset at anchor site rooms only. Resonance wraiths reset in hollow pass interior rooms. Boss (6194) resets in Anchor Construct Sealed Vault (6373) only, limit 1. Null Cairn Field (6367) must remain mob-free.

**Zone 5 (Western Fold Approach):** Ash Cult forward zealots and ash-walkers reset in forward camp and patrol path rooms. Ventspeaker spirits reset at vent platform rooms only. Boss (6212) resets in Last Ventspeaker's Oracle Ground (6477) only, limit 1. Red Sister Cisterns (6462-6465) must remain mob-free.

**Zone 6 (Western Fold):** Ash Cult senior zealots and doctrine speakers reset in inner territory and Doctrine Hall rooms. Thermophilic titan resets in fungal growth zone. Speaking vent herald resets in Speaking Vent approach room. Boss (6221) resets in Grand Vent-Oracle's Throne (6548) only, limit 1. Sanctum Scar (6549-6552) no boss; perimeter rooms have collapse elementals.

**Zone 7 (Crown Approaches):** Scree titans reset across scree approach rooms. Glow-hawk elder resets at nesting cliff room only. Crown glassfall elementals reset in glassfall zone rooms. No boss in this zone.

**Zone 8 (Ember Crown):** Caldera spawn reset on lower caldera face rooms. Caldera rim guardians and lava serpents reset in approach and mid-caldera rooms. Boss (6238) resets in Secondtooth Caldera Watcher's Ground (6668) only, limit 1. Final boss (6242) resets in The Ember Throne (6673) only, limit 1. Oracle Vent Site (6663) must remain mob-free. Caldera Observation Ledge (6674) must remain mob-free.

### Loot Table Resets
Loot-bearing mobs use `l`/`L` extension lines. Standard mobs carry 1 item at 100% from a 2-3 item pool. Boss mobs carry 1 guaranteed drop (100%) plus 1 roll from a secondary pool (50%):

Example boss reset pattern:
```
l 150 6130 6171 0 0 0 0 0 0 0
L 100 50 0 0 0 0 0 0 0
```
(guaranteed primary boss drop + 50% chance for secondary zone armor piece)

### Door Resets
The following doors require both-sides `D` resets:
- Ashfang Stronghold Gate (6147) — requires lock state `2` (locked), key 6124. Both sides: D from the approach room and D from inside the Armory (6148).
- Ironpost Archive Sealed Wing (6188→6189) — locked, key 6125. Both sides.
- Anchor Construct Sealed Vault (6372→6373) — locked, key 6126. Both sides.
- Ember Throne Antechamber (6671→6672) — locked, key 6128. Both sides.

---

## Implementation Checklist

### Spec Compliance
- [ ] `Q 16` present in `#AREA` section
- [ ] `O Virant~` present
- [ ] All mob vnums sequential 6124-6243, no gaps
- [ ] All object vnums sequential 6124-6243, no gaps
- [ ] All room vnums sequential 6124-6723, no gaps
- [ ] No vnum mentioned in any in-world text
- [ ] No back-to-back newline runs in any string
- [ ] All strings `~`-terminated

### Room Quality
- [ ] All 600 rooms have unique descriptions
- [ ] All rooms have at least 3 sentences in description
- [ ] All `no_mob` boss rooms have at least 5 sentences
- [ ] Named exits anchored in room description or `E` extra description
- [ ] All exits bidirectional except maze-flagged rooms
- [ ] Maze-flagged rooms: Upper Hollow Echo Corridors (6347-6349), Long Saddle Subsidence Zone Alpha (6207), Bellspine False Echo Chamber (6378)
- [ ] Sub-regions enclosed by `no_mob` boundary rooms or `sentinel` mobs
- [ ] Boss rooms all flagged `no_mob`
- [ ] Planned cross-area exits omitted until target areas exist; described as directions or sealed passages in room text (see connections section for exact room pairs)

### Mobile Quality
- [ ] All 120 mobs include `is_npc` and `stay_area`
- [ ] All 8 boss mobs include `sentinel`, `boss`, `no_flee`, `no_hunt`
- [ ] All solo elite mobs include `solo`
- [ ] Boss stat lines: tier 1 `0 -100 10 10`, tier 2 `0 -100 11 11`, tier 3 `0 -100 12 12`
- [ ] `long_descr` format: exactly one text line, then `~` line (no multi-line)
- [ ] `description` format: exactly one trailing newline before `~`
- [ ] No mass-template boilerplate mob descriptions
- [ ] `spec_summon_*` never assigned in area file
- [ ] `spec_keep_*` not used (no human-assignment-only specials)
- [ ] `invasion` act flag never set in area file
- [ ] Loot tables: `l` line has exactly 10 fields; `L` line has exactly 9 fields; L values sum ≤ 100

### Object Quality
- [ ] All 130 objects include `take` in wear flags
- [ ] All loot-table items include `ITEM_LOOT`
- [ ] All boss-source items include `ITEM_BOSS`
- [ ] `ITEM_GENERATED` not set in area file
- [ ] Object names unique within area (all 130 names distinct)
- [ ] Weapon `value3 = 0` only on fist-weapon objects (6151 has fist + hit)
- [ ] Two-handed weapons have `two-handed` in extra flags
- [ ] Wands have `wand` in extra flags
- [ ] Fist weapons have `fist` in extra flags
- [ ] Bucklers have `buckler` in extra flags
- [ ] Object weight 1-5 for caster items, 6-10 for melee, 11-15 for tank
- [ ] `L` level entries present for all equipment objects matching zone difficulty
- [ ] `ITEM_LIFESTEALER` not used (would require `ITEM_ANTI_GOOD` pairing)
- [ ] `objfun_healing` not used (would require `ITEM_ANTI_EVIL`)

### Resets
- [ ] `#RESETS` has no blank lines
- [ ] `G` and `R` commands have no `arg3`
- [ ] All `E` reset `wear_loc` values match object `wear_flags`
- [ ] Door resets issued for both sides of all locked doors
- [ ] Boss mobs have limit 1 in `M` reset
- [ ] All reset vnums reference valid mobs/objects/rooms within area envelope
- [ ] No `A` (obsolete) reset commands used

### Quest Design
- [ ] At least one quest targets a boss ✓ (Quest 2: Grand Vent-Oracle, Quest 3: Ember Throne Patriarch)
- [ ] At least one cartography quest ✓ (Quest 1: Witness Freshness Survey)
- [ ] Final quest in chain rewards equipment ✓ (Quest 3: obsidian sigil ring)
- [ ] All boss-targeting quests reward equipment ✓ (Quest 2, Quest 3)

### World Links
- [ ] `docs/world_links.md` updated with all 7 planned connections (status: Planned):
  - 3× Forest of Confusion (6124↔confusn 2519, 6153↔confusn 2521, 6168↔confusn 2524)
  - 1× Sunken Sanctum (6552↔sunken_sanctum 2266)
  - 3× Great Northern Forest (6627↔gnf 4179, 6633↔gnf 4199, 6639↔gnf 4219)
- [ ] `sunken_sanctum_plan.md` room 2266 updated: replace TBD with `up` → 6552
- [ ] `great_northern_forest_plan.md` rooms 4179, 4199, 4219 updated: add `west` → respective Cinderteeth rooms
- [ ] `confusn.are` / `confusn` plan: rooms 2519, 2521, 2524 must be authored with `north` exits to 6124, 6153, 6168 respectively
- [ ] `docs/area_index.md` updated with new area entry (6124-6723, I 140 150)
- [ ] `area/area.lst` updated with `cinderteeth_mountains.are` inserted after `saltglass_reach.are` (vnum ordering: 5724 < 6124, correct position at end of active list before `ceiling.are`)

---
