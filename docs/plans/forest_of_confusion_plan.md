# Area Plan: Forest of Confusion

## Overview
- **Area Name:** `@@GForest @@Wof @@dConfusion@@N`
- **File Name:** `confusn.are`
- **Keyword:** `forestconfusion`
- **Intended Levels:** 10-60 (bridge zone between Kiess frontier traffic and deeper forest content)
- **Vnum Range:** `2339-2538`
- **Vnum Count:** 35 (all vnums assigned to rooms/objects/mobs; no unused room vnums)
- **Primary Geography:** transitional forest north of Kiess, far west of Midgaard
- **Reset Rate:** 15
- **Lore Reference:** `forest_of_confusion_lore.md`

## Multi-Color Theme
Use color with intent and sparseness (no `@@k`, no background colors, no flashing):

- `@@G` — living canopy, healthy growth, Verdant resurgence
- `@@d` — ash bark, deadfall, blight haze, Withered taint
- `@@p` — confusion-mist and liminal magical pressure
- `@@a` — prism dew, wet air, luminous spores, channel water
- `@@b` — trail earth, roots, old road timbers, survey posts
- `@@W` — Kiess-made markers, ward sigils, warning inscriptions
- `@@R` — danger cues: breach zones, blooded paths, failed wards
- `@@N` — explicit reset after colored phrases

## #AREA Header Plan
- `Q 16`
- `K forestconfusion~`
- `L @@W(@@r10 60@@W)@@N~`
- `N 13` (retain unless numbering audit requires reassignment)
- `I 10 60`
- `V 2339 2538` (unchanged; required envelope)
- `X 32200` (retain map offset)
- `F 15`
- `U @@Wthe @@pmists @@Wshift and the @@Gforest @@Wrearranges itself...@@N~`
- `T` teleport flag retained
- `O Virant~` per area ownership policy

## Layout Rebuild Strategy (all rooms used)
Replace the current partial/legacy topology with a full 35-room design:

- **Grid model:** 20 x 10
- **Coordinate system:** `x=0..19` west->east, `y=0..9` north->south
- **Vnum mapping:** `vnum = 2339 + (y * 20) + x`
- **Coverage rule:** every vnum 2339-2538 is a valid room (no gaps)

## Zone Banding (room-level identity)

### Zone 1: South Verge of Kiess (2339-2378, rows y=0-1)
**Theme:** Kiess frontier infrastructure in slow retreat. Patrol roads, survey stakes, toll ruins, evac bells.
**Level range:** 10-20
**Sector:** primarily `forest`, `field` for cleared camps, `inside` for the survey station
**Room flags:** most rooms unflagged; camps and station get `no_mob`
**Mood:** organized anxiety — competent people losing ground to a forest that does not respect competence.

Row y=0 (vnums 2339-2358): The patrol road and its flanking positions.
- 2339: South Verge Gatehouse — a roofless stone archway where Roc Road ends and forest begins. The last Kiess waystone stands here. `inside` sector.
- 2340: Roc Road Terminus — the final stretch of maintained road, packed earth giving way to root-buckled stone. **Primary south connection: exit west to room 1156 (Roc Road).**
- 2341-2344: Patrol Road sections running east — increasingly overgrown, rope-line anchors visible, bell-posts with frayed signal cords.
- 2345: Bell-Post Clearing — an open break in the canopy where three bell-posts converge. The bells still ring when the mist shifts. `field` sector.
- 2346-2349: Patrol Road continuation east — survey stakes with faded Kiess markings, equipment caches half-buried by root growth.
- 2350: Waystone Junction — a crossroads where two patrol routes once intersected. The waystones bear Kiess compass-and-wall crests.
- 2351-2354: Patrol Road trailing east — the road narrows, the canopy closes, and the mist begins to pool at ground level.
- 2355: Southern Timber Cut — a cleared area where Kiess logging teams once worked. Stumps show saw marks; new growth has already reached head height. **Exit south to room 9052 (external connection, retained from legacy).**
- 2356-2358: Forest edge positions — the patrol road dissolves into game trails. Rope-line fragments lead north into thicker growth.

Row y=1 (vnums 2359-2378): Patrol camps, abandoned survey positions, and the forest's first confusion effects.
- 2359-2362: Abandoned Ranger Camp sequence — fire-scarred stone rings, collapsed lean-tos, dispatch tablets scattered in leaf litter. One room is the camp's former command post (`inside` sector, `no_mob`, `safe`).
- 2363-2366: Undergrowth Trail sections — the patrol road's northern spur, increasingly obscured by vine growth and root displacement.
- 2367-2370: Frayed Rope-Line Corridor — a once-maintained navigation line now hanging in loops. The mist here thickens noticeably. Directional confusion begins: exit arrangements become subtly asymmetric.
- 2371-2374: Warning Bell Perimeter — the outermost ring of Kiess bell-posts, most silent. One still sounds when mist pressure changes, providing an auditory landmark.
- 2375-2378: Southern Verge Fringe — the last positions with visible Kiess infrastructure. Ward-stones with corroded inscriptions. North exits lead into the Mistroute Thickets.

### Zone 2: Mistroute Thickets (2379-2418, rows y=2-3)
**Theme:** Directional confusion mechanics, mirrored groves, looping trails. The three-force overlap is first felt here.
**Level range:** 18-35
**Sector:** primarily `forest`, occasional `water_swim` for seep-pools
**Room flags:** most rooms unflagged; maze sub-region rooms get `ROOM_MAZE`
**Mood:** structural confusion — two competing logics (Verdant and Withered) producing unpredictable navigation.

Row y=2 (vnums 2379-2398): The Mirrorbark Switchmaze and its approaches.
- 2379-2382: Mistroute approaches — the canopy closes fully, light becomes diffuse, and the first mirrorbark trees appear. Their reflective bark surfaces create visual echoes of torchlight.
- **2383-2398: The Mirrorbark Switchmaze** (16 rooms, all `ROOM_MAZE`)
 - A dense thicket maze with repeated visual motifs and short looping connectors.
 - False-loop lattice with 2-3 true exits (south back to approaches, east to row y=2 continuation, north to row y=3).
 - Theme emphasis: `@@p` mist pressure + `@@G` living bark intelligence.
 - Clue language: bark reflections — left-reflection indicates south, overhead-reflection indicates true north.
 - Each room gets unique description despite shared visual motifs; anchors include specific bark patterns, root formations, mist density variations, and sound quality changes.
 - Dead-end pockets contain optional loot (druidic remnants, Kiess scout equipment).

Row y=3 (vnums 2399-2418): Post-maze thickets, catrat territory, and first broken-one encounters.
- 2399-2402: Switchmaze exits and recovery — the maze releases into slightly clearer thickets. Catrat family groups establish territory here.
- 2403-2406: Catrat Warren approaches — undergrowth dense with small-predator signs: gnawed bark, prey bones, territorial scratch-marks.
- 2407-2410: Broken-One Territory — the first evidence of Withered corruption on sentient beings. Degraded campsites, smashed equipment, and the shambling residents who once were people.
- 2411-2414: Neogi Fringe — concealed entrances to underground passages begin appearing. Signs of slave-taking: drag marks, abandoned restraints, fear-scent.
- 2415-2418: Seep-Pool Crossing — shallow water collects in root hollows, creating a wet barrier between the thickets and the channels below. `water_swim` sector for pool rooms.

### Zone 3: Shivergrove Channels (2419-2458, rows y=4-5)
**Theme:** Wet ravines, prism pools, root-bridges, partial verticality. The Prism-Fen Knot anchors this zone.
**Level range:** 25-45
**Sector:** mixed `forest`/`water_swim`/`inside` (for checkpoint hut)
**Room flags:** maze sub-region rooms get `ROOM_MAZE`; checkpoint hut gets `no_mob`
**Mood:** disorienting openness — the terrain appears navigable but folds back on itself.

Row y=4 (vnums 2419-2438): The Prism-Fen Knot, neogi warrens, and channel crossings.
- 2419-2422: Channel Approaches — the terrain drops into shallow ravines where water flows over root-bridges. The mist takes on a crystalline quality as Conclave fragments in the channel beds refract light.
- 2423-2428: Neogi Warrens — underground passages accessed through concealed entrances in the undergrowth. `inside` sector. The neogi hierarchy operates here: slave pens, guard posts, the master's chamber, and the ruler's throne. Rooms are tight, deliberately claustrophobic.
 - 2423: Neogi Tunnel Mouth — `inside` sector
 - 2424: Slave Pen — `inside` sector, `dark`
 - 2425: Neogi Guard Post — `inside` sector
 - 2426: Neogi Master's Chamber — `inside` sector
 - 2427: Neogi Ruler's Hall — `inside` sector, `no_mob`
 - 2428: Neogi Escape Tunnel — `inside` sector, connects back to surface
- 2429-2433: Root-Bridge Crossings — elevated root platforms spanning channels. The bridges shift as the root network redirects water flow.
- 2434: Dense Woodland Junction — a critical intersection point. **Exit east to room 2739 (Verdant Depths entry). Exit south to room 291 (Withered Depths entry).** This room is the geographic pivot where the forest's two deeper regions become accessible. `no_mob`.
- 2435-2438: Deep Channel sections — the channels widen and deepen. Prism-scored fragments visible in the streambeds. Cockatrice territory begins.

Row y=5 (vnums 2439-2458): The Prism-Fen Knot, cockatrice nesting, and hippogriff canopy.
- **2439-2454: The Prism-Fen Knot** (16 rooms, all `ROOM_MAZE`)
 - Wetland maze using channels and root-bridges where pathing appears open but folds back.
 - Mixed `forest`/`water_swim` sectors.
 - One checkpoint hut at 2446: `inside` sector, `no_mob` — an old Kiess survey station providing re-orientation anchor.
 - Theme emphasis: `@@a` refracted moisture + `@@b` root/earth hazards.
 - Clue language: water-sound direction — flowing water on the left indicates east, flowing water ahead indicates north.
 - Prism-light distortions in the mist create rainbow false-paths; true paths are marked by absence of prismatic effect.
- 2445: Large Nesting Tree — an ancient old-growth tree massive enough to support hippogriff nests in its upper canopy. The tree is a Verdant-influenced landmark. Exit up to 2455.
- 2455: Hippogriff Nest — high canopy platform above the mist line. `forest` sector. The air is clear here; the confusion effects are weaker above the canopy.
- 2456-2458: Fen Exit and Glade Approach — the wetland gives way to drier ground. The canopy opens slightly. Cockatrice territory: cleared areas where the stone-gaze creatures display their crests.

### Zone 4: Conclave Scars (2459-2498, rows y=6-7)
**Theme:** Blight circles, crystal fractures, quarantine relics. Withered influence dominates.
**Level range:** 35-55
**Sector:** primarily `forest`, `inside` for cabin complex and caves
**Room flags:** maze sub-region rooms get `ROOM_MAZE`; boss rooms get `no_mob`
**Mood:** hostile and deliberate — the corruption is not random but follows the geometry of abandoned Conclave infrastructure.

Row y=6 (vnums 2459-2478): Wemic territory, Ymmas's domain, centaur approaches.
- 2459-2461: Wemic Hunting Grounds — deep clearings where wemic family bands (male, female, cubs) establish territory. The clearings are Verdant-influenced pockets: the air is clearer, the growth is healthier, and the wemics patrol with territorial precision. `no_mob` flag on wemic den room.
 - **Exit north from 2461 to room 6600 (external connection, retained from legacy).**
- 2462-2464: Crystal Scar Approaches — the first visible evidence of Conclave crystal matrices. Root systems exposed by erosion show faintly luminous fragments. The air hums. Survey trench remnants cut narrow lines in the forest floor.
 - **Exit up from 2464 to room 1 (external connection, retained from legacy).**
- 2465-2471: Ymmas's Domain — a fortified cabin complex in the deep woodland.
 - 2465: Cabin Approach — the path narrows between sentinel trees. The Guardian Yugoloth patrols here. `no_mob`.
 - 2466: Strongroom — a sealed chamber within the cabin. `inside`, `no_mob`. Door: locked, requires key.
 - 2467: Cabin Main Room — `inside` sector. The anomaly: a human-made structure that has not been reclaimed by the forest.
 - 2468: Cabin Workshop — `inside` sector. Shelves of reagents, star charts, and bound texts.
 - 2469: Cabin Upper Room — `inside` sector. Ymmas's personal quarters.
 - 2470: Ymmas's Lair — `inside`, `no_mob`. The mage's sanctum. Ymmas (level 86 boss) resides here.
 - 2471: Throne Room — `inside`, `no_mob`. A formal audience chamber. The Staff of Summoning is kept here.
- 2472-2474: Deep Undergrowth — transitional rooms between Ymmas's domain and centaur territory. Dense, dark, and contested.
- 2475-2478: Centaur Glade Approaches — living-wood boundary markers appear. Woven bone-and-bark windchimes produce territorial tones. The forest shifts from corrupted to carefully maintained.

Row y=7 (vnums 2479-2498): Centaur settlement, the Ashen Lattice maze, and manscorpian approaches.
- 2479-2486: Centaur Settlement — a secluded glade of living-wood structures.
 - 2479: Glade Entrance — centaur boundary markers. The forest here is healthier than anywhere else in the area outside the Verdant-influenced pockets.
 - 2480-2482: Secluded Glade rooms — open spaces between trained trees. Centaur families move between these. `forest` sector.
 - 2483: Leader's Glade — `no_mob`. The centaur leader holds court here. Centaur oral tradition preserves accounts of the forest before the confusion mists.
 - 2484-2486: Centaur Huts — `inside` sector. Living-wood structures, doors of woven branches. Each hut has unique character.
- **2487-2498: The Ashen Lattice of Scars** (12 rooms, all `ROOM_MAZE`)
 - Blight-geometry maze built around Conclave scar lines and fungal lattice corridors.
 - Route feels hostile and deliberate: dead-end bait pockets, elite ambush corners, one reliable traversal lane.
 - Theme emphasis: `@@d` withered decay + `@@R` breach danger + restrained `@@p` distortion cues.
 - Clue language: fire-scar depth — the deepest, oldest fire scars mark the original Conclave survey routes, which remain navigable because the crystal resonance that created them also preserved them.
 - Fungal growth follows Conclave geometric patterns: hexagonal spore formations, lattice-structured mycelium.
 - 2494: Lattice Core — the densest concentration of crystal resonance at the surface. The air hums audibly. Boss-tier creature territory.

### Zone 5: Twin Thresholds (2499-2538, rows y=8-9)
**Theme:** The north arc forks into Verdant-biased and Withered-biased transition routes. Manscorpian caves. The Spider's web.
**Level range:** 45-60
**Sector:** mixed `forest`/`inside` (for caves)
**Room flags:** cave maze rooms get `ROOM_MAZE`; boss rooms get `no_mob`
**Mood:** threshold anticipation — the forest is about to become something else, and the two possible destinations are starkly different.

Row y=8 (vnums 2499-2518): Manscorpian cave maze, the spider's domain, and the Verdant approach.
- 2499-2501: Deep Woodland Transition — the forest reaches its maximum density. Two distinct environmental flavors begin: healthier growth to the east (Verdant influence), corrupted growth to the west (Withered influence).
- **2502-2517: Manscorpian Cave Maze** (16 rooms, all `ROOM_MAZE`)
 - The manscorpian king's domain. Caves accessed through concealed entrances.
 - `inside` sector throughout. `dark` flag on unlit chambers.
 - 2502: Cave Mouth — the entrance, guarded by manscorpian sentries.
 - 2503-2509: Maze of Caves — twisting passages navigated by the manscorpians via scent. Poison hazards.
 - 2509: Deep Cave Junction — **Exit south to room 2801 (Verdant Depths cave shortcut, retained from legacy).** `no_mob`.
 - 2510-2513: Inner Caves — the king's territory. More defended, better-lit by bioluminescent fungal growth from the deep root tunnels below.
 - 2514: King's Chamber — `inside`, `no_mob`. The manscorpian king (level 52, boss) and his aid.
 - 2515-2517: Cave exits and connecting passages back to surface.
- 2518: The Spider's Web — a massive web complex spanning multiple trees at the Verdant-Withered boundary. The spider (level 57, solo) hunts here. `no_mob`.

Row y=9 (vnums 2519-2538): The twin threshold approaches and final deep-forest positions.
- 2519-2523: Verdant Threshold — the eastern approach. The forest grows taller, the mist thins, and the root network's chemical signaling becomes a perceptible hum. Treants patrol these rooms as Verdant sentinels. The canopy opens briefly, providing glimpses of the ancient growth beyond.
- 2524-2528: Withered Threshold — the western approach. The forest darkens, bark blackens, and the preservation magic's chemical signature (sweet rot, metallic tang) becomes prominent. Broken ones and greater broken ones congregate. Failed quarantine circles — stone rings with flickering ward-signs that no longer seal anything — mark the positions where the druids made their last stand.
- 2529-2533: Deep Contested Band — the narrow strip where Verdant and Withered influences collide directly. Rooms alternate between eerie health and grinding corruption. The most dangerous non-boss encounters in the area occur here as creatures from both influence zones clash.
- 2534-2536: Northern Ridge — elevated positions where the forest canopy breaks enough to see the terrain ahead. The view north splits: verdant ancient growth to the east, blackened withered canopy to the west. These rooms provide narrative orientation — the player can see where both paths lead.
- 2537: Verdant Overlook — a high-canopy perch in a Verdant-influenced giant tree. The air is clear. The confusion effects lift entirely for the first time. The forest's intelligence is palpable as attention rather than disorientation. The view shows the Verdant Depths' ancient canopy stretching northward.
- 2538: Withered Overlook — a blighted ridge where the preservation magic's purple glow is visible in the root systems below. The confusion effects are replaced by a different kind of wrongness: everything is clear, but the clarity reveals a landscape of animate rot. The view shows the Withered Depths' blackened canopy extending northward.

## Maze Sub-Regions (mandatory thematic cores)

### 1. The Mirrorbark Switchmaze (2383-2398)
- 16 rooms, all `ROOM_MAZE`
- Dense thicket maze with repeated visual motifs and short looping connectors
- False-loop lattice with 2-3 true exits
- Theme emphasis: `@@p` mist pressure + `@@G` living bark intelligence
- Clue language: bark reflections (left = south, overhead = north)

### 2. The Prism-Fen Knot (2439-2454)
- 16 rooms, all `ROOM_MAZE`
- Wetland maze using channels and root-bridges
- Mixed `forest`/`water_swim` sectors; one `inside` checkpoint hut (2446) as re-orientation anchor
- Theme emphasis: `@@a` refracted moisture + `@@b` root/earth hazards
- Clue language: water-sound direction (left = east, ahead = north)

### 3. The Ashen Lattice of Scars (2487-2498)
- 12 rooms, all `ROOM_MAZE`
- Blight-geometry maze around Conclave scar lines and fungal lattice corridors
- Dead-end bait pockets, elite ambush corners, one reliable traversal lane
- Theme emphasis: `@@d` withered decay + `@@R` breach danger + `@@p` distortion
- Clue language: fire-scar depth (deepest scars mark the reliable traversal lane)

### 4. Manscorpian Cave Maze (2502-2517)
- 16 rooms, all `ROOM_MAZE`
- `inside` sector, `dark` flag on unlit chambers
- Scent-navigated by manscorpians; disorienting to human visitors
- Theme emphasis: `@@d` darkness + `@@a` bioluminescent fungal light in deeper chambers

**Maze Design Rules**
- Every maze sub-region must be internally coherent and traversable without soft-locking players.
- Each maze has one clear "readable" clue language (bark reflections, water sound, fire-scar depth, scent trails).
- Main progression spine must bypass the deepest dead-end maze pockets, while optional rewards live inside them.
- Maze rooms still require unique descriptions; no generic duplicated text blocks.
- All rooms in a maze vnum set must have `ROOM_MAZE` flag.

## Connectivity and External Links

### Primary Connections (retained from legacy)
| Room | Direction | Destination | Notes |
|------|-----------|-------------|-------|
| 2340 | west | 1156 | Roc Road (primary Kiess approach) |
| 2355 | south | 9052 | External connection (legacy) |
| 2434 | east | 2739 | Verdant Depths entry (surface) |
| 2434 | south | 291 | Withered Depths entry |
| 2461 | north | 6600 | External connection (legacy) |
| 2464 | up | 1 | External connection (legacy) |
| 2509 | south | 2801 | Verdant Depths entry (cave shortcut) |

### Internal Connectivity Rules
- Main progression spine runs roughly south-to-north through the grid, with east-west branching.
- Maze sub-regions have controlled entry/exit points; internal routing is non-linear.
- Side webs produce confusion without hard-locking progress.
- All non-maze rooms maintain bidirectional exit reciprocity.

### Door Policy
- Only man-made checkpoints and quarantine gates use door flags; natural forest transitions are open.
- Doors required:
 - Ymmas's strongroom (2466): locked door, requires key (emerald key, vnum 2358)
 - Ymmas's lair entrance: closed door
 - Centaur hut doors (2484-2486): closed doors (woven branch construction)
 - Neogi ruler's hall (2427): closed door
 - Manscorpian king's chamber (2514): closed door

## Mob/NPC Redesign Plan

### Faction 1: Kiess Expeditionary Presence (levels 12-25)
Sentinel or patrol mobs at camps and checkpoints in Zone 1. Non-aggressive.

| Vnum | Name | Short | Level | Profile | Placement |
|------|------|-------|-------|---------|-----------|
| 2339 | warden kiess patrol | a Kiess patrol warden | 20 | melee | 2345, 2350 (sentinel) |
| 2340 | rope-line team scout | a rope-line scout | 15 | melee | 2367-2370 |
| 2341 | medic kiess field | a Kiess field medic | 18 | hybrid | 2359-2362 (sentinel) |
| 2342 | signal keeper bell | a signal keeper | 12 | melee | 2371-2374 (sentinel) |

### Faction 2: Forest Wildlife — Baseline (levels 10-20)
Aggressive small predators in Zone 1 and Zone 2 fringe.

| Vnum | Name | Short | Level | Profile | Placement |
|------|------|-------|-------|---------|-----------|
| 2343 | catrat young | a young catrat | 12 | melee | 2375-2378 |
| 2344 | catrat female | a female catrat | 10 | melee | 2403-2406 |
| 2345 | catrat male | a male catrat | 14 | melee | 2403-2406 |

### Faction 3: Verdant-Touched Fauna (levels 20-50)
Ambush/territorial behavior in healthy pockets. Aggressive.

| Vnum | Name | Short | Level | Profile | Placement |
|------|------|-------|-------|---------|-----------|
| 2346 | mirror stag verdant | a mirror stag | 22 | melee | 2383-2398 (switchmaze) |
| 2347 | rootcat verdant | a rootcat | 28 | melee | 2429-2433 |
| 2348 | canopy drake verdant | a canopy drake | 35 | melee | 2455, 2519-2523 |
| 2349 | whisper-owl verdant | a whisper-owl | 20 | caster | 2379-2382 |
| 2350 | cockatrice male cock | a male cockatrice | 30 | melee | 2456-2458 |
| 2351 | cockatrice female cock | a female cockatrice | 32 | melee | 2435-2438 |
| 2352 | hippogriff young hippo | a young hippogriff | 25 | melee | 2455 |
| 2353 | hippogriff hippo | a hippogriff | 35 | melee | 2445, 2455 |
| 2354 | treant tree | a treant | 33 | melee | 2519-2523, 2529-2533 |
| 2355 | treant elder tree | an elder treant | 42 | melee | 2537 (sentinel) |

### Faction 4: Withered-Blight Creatures (levels 25-55)
Stronger in Conclave Scar bands and Withered threshold. Aggressive.

| Vnum | Name | Short | Level | Profile | Placement |
|------|------|-------|-------|---------|-----------|
| 2356 | broken one | a broken one | 27 | melee | 2407-2410 |
| 2357 | broken one | a broken one | 30 | melee | 2524-2528 |
| 2358 | greater broken one | a greater broken one | 39 | melee | 2524-2528, 2529-2533 |
| 2359 | spore husk withered | a spore husk | 35 | melee | 2487-2498 (lattice) |
| 2360 | mist-wolf withered | a mist-wolf | 32 | melee | 2487-2498, 2524-2528 |
| 2361 | bark revenant withered | a bark revenant | 45 | melee | 2529-2533 |
| 2362 | prism-leech swarm | a prism-leech swarm | 40 | caster | 2462-2464, 2494 |

### Faction 5: Neogi Slavers (levels 15-35)
Sentinel in warrens. Aggressive.

| Vnum | Name | Short | Level | Profile | Placement |
|------|------|-------|-------|---------|-----------|
| 2363 | neogi slave | a Neogi slave | 15 | melee | 2424 |
| 2364 | neogi slave | a Neogi slave | 20 | melee | 2423, 2428 |
| 2365 | neogi | a Neogi | 22 | melee | 2425 |
| 2366 | neogi master | a Neogi master | 28 | hybrid | 2426 |
| 2367 | neogi ruler | the Neogi ruler | 35 | caster | 2427 (sentinel) |

### Faction 6: Centaur Society (levels 22-50)
Mixed aggressive/non-aggressive in glade settlement.

| Vnum | Name | Short | Level | Profile | Placement |
|------|------|-------|-------|---------|-----------|
| 2368 | centaur male | a male centaur | 49 | melee | 2480-2482 |
| 2369 | centaur female | a female centaur | 42 | melee | 2480-2482 |
| 2370 | centaur baby | a baby centaur | 22 | melee | 2484-2486 |
| 2371 | centaur leader | the centaur leader | 50 | hybrid | 2483 (sentinel) |

### Faction 7: Wemic Bands (levels 34-50)
Territorial aggressive in deep clearings.

| Vnum | Name | Short | Level | Profile | Placement |
|------|------|-------|-------|---------|-----------|
| 2372 | wemic male | a male wemic | 50 | melee | 2459 |
| 2373 | wemic female | a female wemic | 48 | melee | 2460 |
| 2374 | wemic cub | a wemic cub | 34 | melee | 2459-2461 |

### Faction 8: Manscorpian Colony (levels 43-52)
Territorial aggressive in cave maze.

| Vnum | Name | Short | Level | Profile | Placement |
|------|------|-------|-------|---------|-----------|
| 2375 | manscorpian man scorpian | a manscorpian | 43 | melee | 2502-2509 |
| 2376 | manscorpian man scorpian | a manscorpian | 51 | melee | 2510-2513 |
| 2377 | manscorpian king man scorpian | the manscorpian king | 52 | melee | 2514 (sentinel, boss) |
| 2378 | manscorpian aid | the king's aid | 46 | caster | 2514 (sentinel) |
| 2379 | manscorpian man scorpian | a manscorpian | 51 | hybrid | 2510-2513 |

### Faction 9: Named Encounters (levels 55-86)
Boss and solo mobs. Sentinel in specific rooms.

| Vnum | Name | Short | Level | Profile | Placement |
|------|------|-------|-------|---------|-----------|
| 2380 | yugoloth guardian | a Guardian Yugoloth | 65 | hybrid | 2465 (sentinel, solo) |
| 2381 | ymmas | the evil Ymmas | 60 | caster | 2470 (sentinel, boss) |
| 2382 | spider | a giant spider | 57 | melee | 2518 (sentinel, solo) |
| 2383 | mist cartographer | the Mist Cartographer | 55 | hybrid | 2434 or roaming Zone 3 (solo) — a neutral mini-boss event mob; a former Kiess survey captain who learned the forest's logic and refuses to return |

### Mob act flags policy
- All mobs: `stay_area` (mandatory per spec)
- Kiess expeditionary (2339-2342): `sentinel`, non-aggressive
- Wildlife and fauna: `aggressive`, `stay_area`
- Boss mobs (2377 manscorpian king, 2381 Ymmas): `sentinel`, `boss`, placed in `no_mob` rooms
- Solo mobs (2380 yugoloth, 2382 spider, 2383 cartographer): `solo`, `sentinel`
- Sub-region containment: mobs without `sentinel` must be in regions enclosed by `no_mob` rooms

### Mob specials
| Vnum | Special |
|------|---------|
| 2381 (Ymmas) | `spec_cast_mage` |
| 2371 (Centaur Leader) | `spec_cast_cleric` |
| 2379 (Manscorpian hybrid) | `spec_cast_mage` |
| 2378 (King's Aid) | `spec_cast_cleric` |
| 2376 (Manscorpian lv51) | `spec_poison` |
| 2375 (Manscorpian lv43) | `spec_poison` |
| 2377 (Manscorpian King) | `spec_poison` |
| 2382 (Spider) | `spec_poison` |
| 2367 (Neogi Ruler) | `spec_cast_mage` |

## Object Redesign Plan

Replace legacy generic drops with forest-specific equipment and utility objects. All objects must include `ITEM_TAKE` in wear_flags. Object names must be unique within the area.

### Equipment Objects (mob drops and reset equipment)

| Vnum | Name | Short | Type | Wear | Weight | Level | Notes |
|------|------|-------|------|------|--------|-------|-------|
| 2339 | neogi whip barbed | a barbed Neogi whip | weapon | hold take | 6 | 22 | value3=4 (whip) |
| 2340 | jewelled dagger neogi | a jewelled Neogi dagger | weapon | hold take | 3 | 28 | value3=2 (stab) |
| 2341 | fur coat catrat | a catrat-fur coat | armor | body take | 8 | 12 | |
| 2342 | rootbound talisman verdant | a rootbound talisman | armor | neck take | 2 | 30 | |
| 2343 | mirrorbark buckler | a mirrorbark buckler | armor | hold take | 12 | 25 | extra: buckler |
| 2344 | stone shield centaur | a centaur stone shield | armor | hold take | 14 | 42 | |
| 2345 | stone hammer centaur | a centaur stone hammer | weapon | hold take | 10 | 42 | value3=7 (pound) |
| 2346 | emerald sword | an emerald sword | weapon | hold take | 8 | 49 | value3=3 (slash) |
| 2347 | emerald breastplate | an emerald breastplate | armor | body take | 14 | 49 | |
| 2348 | emerald helm | an emerald helm | armor | head take | 10 | 49 | |
| 2349 | emerald boots | a pair of emerald boots | armor | feet take | 8 | 49 | |
| 2350 | emerald leggings | emerald leggings | armor | legs take | 10 | 49 | |
| 2351 | oaken club centaur | an oaken club | weapon | hold take | 9 | 35 | value3=8 (crush) |
| 2352 | lance centaur | a centaur lance | weapon | hold take | 10 | 45 | extra: two-handed, value3=11 (pierce) |
| 2353 | red battle axe manscorpian | a red battle axe | weapon | hold take | 9 | 48 | value3=3 (slash) |
| 2354 | red javelin manscorpian | a red javelin | weapon | hold take | 7 | 43 | value3=11 (pierce) |
| 2355 | red bracer manscorpian | a red bracer | armor | wrist take | 8 | 48 | |
| 2356 | leather belt scout | a scout's leather belt | armor | waist take | 6 | 15 | |
| 2357 | anti-spore wrap kiess | an anti-spore wrap | armor | face take | 3 | 18 | |
| 2358 | emerald key | an emerald key | key | take | 1 | 49 | unlocks 2466 |
| 2359 | oak shield centaur | an oak shield | armor | hold take | 12 | 40 | |
| 2360 | gnarled staff ymmas | a gnarled staff | weapon | hold take | 4 | 60 | extra: wand, value3=7 (pound) |
| 2361 | bark claw cockatrice | a large yellow claw | weapon | hold take | 6 | 30 | value3=5 (claw) |
| 2362 | primitive club broken | a primitive club | weapon | hold take | 8 | 27 | value3=8 (crush) |
| 2363 | branch treant | a treant branch | weapon | hold take | 10 | 33 | value3=7 (pound) |
| 2364 | wemic fang necklace | a wemic fang necklace | armor | neck take | 7 | 48 | |

### Utility and Lore Objects

| Vnum | Name | Short | Type | Wear | Weight | Level | Notes |
|------|------|-------|------|------|--------|-------|-------|
| 2365 | food pack kiess | a Kiess food pack | food | take | 3 | 1 | placed in camp rooms |
| 2366 | furry pouch catrat | a small furry pouch | container | take | 2 | 10 | |
| 2367 | yellow stone light | a yellow stone | light | take | 1 | 15 | |
| 2368 | gold pile | a pile of gold | money | take | 1 | 1 | |
| 2369 | blue scroll magic | a blue scroll | scroll | take | 1 | 25 | |
| 2370 | magic potion vial | a small vial of magic potion | potion | take | 1 | 20 | |
| 2371 | dispatch tablet kiess | a Kiess dispatch tablet | treasure | take | 3 | 1 | lore object — Kiess route notation on flat slate |
| 2372 | conclave survey slate | a damaged Conclave survey slate | treasure | take | 4 | 1 | lore object — geometric notation, crystal-implant diagrams |
| 2373 | druid quarantine marker | a druid quarantine marker | treasure | take | 5 | 1 | lore object — weathered ward-sign on river-washed stone |
| 2374 | prism shard crystal | a prism shard | treasure | take | 1 | 30 | lore object — faintly luminous Conclave crystal fragment |
| 2375 | staff summoning | the Staff of Summoning | staff | hold take | 4 | 60 | Ymmas's artifact, placed in room 2471 |

### Boss Loot Objects (ITEM_LOOT + ITEM_BOSS)

| Vnum | Name | Short | Type | Wear | Weight | Level | Notes |
|------|------|-------|------|------|--------|-------|-------|
| 2376 | ymmas grimoire | Ymmas's grimoire | armor | hold take | 2 | 60 | boss loot |
| 2377 | manscorpian crown chitin | a chitin crown | armor | head take | 8 | 52 | boss loot |
| 2378 | cartographer compass mist | the Mist Cartographer's compass | armor | hold take | 3 | 55 | solo loot |

## Reset and Encounter Rhythm

### Zone 1 (South Verge): Patrol-and-wildlife mix
- Kiess wardens sentinel at bell-post clearing and waystone junction
- Rope-line scouts wandering in corridor rooms
- Field medics sentinel at camp
- Catrats at zone fringe (2-3 per reset)
- Lore objects (dispatch tablets, food packs) placed in camp rooms

### Zone 2 (Mistroute Thickets): Navigation pressure and skirmish frequency
- Mirror stags in switchmaze (3-4 per reset)
- Whisper-owls at maze approaches
- Catrats in warren territory
- Broken ones in corruption fringe (2-3 per reset)
- Neogi in undergrowth fringe (1-2 scouts)

### Zone 3 (Shivergrove Channels): Mixed terrain encounters
- Neogi full hierarchy in warrens (slaves, guards, master, ruler)
- Rootcats on root-bridges
- Cockatrices in nesting territory (2-3 per reset)
- Hippogriffs in canopy rooms (2-3 per reset)
- Mist Cartographer at junction (solo event mob)

### Zone 4 (Conclave Scars): Elite clusters
- Wemic family band in clearings (male, female, 1-2 cubs)
- Guardian Yugoloth at cabin approach
- Ymmas in lair (boss)
- Centaur settlement populated (leader, males, females, babies)
- Spore husks and mist-wolves in Ashen Lattice (4-6 per reset)
- Prism-leech swarms at crystal scar approaches

### Zone 5 (Twin Thresholds): Branching objective routes
- Manscorpian full hierarchy in cave maze (king, aid, guards, scouts)
- Spider in web room (solo)
- Elder treant at Verdant overlook
- Greater broken ones and bark revenants at Withered threshold
- Treants patrolling Verdant approach
- No boss/solo mobs blocking mandatory travel corridors

## Room Content Redesign Rules
- Rewrite all room names/descriptions to eliminate generic repetition.
- Every room gets at least one unique anchor (sound, smell, landmark, hazard, or lore cue).
- Split sectors intentionally:
 - `forest` dominant
 - `water_swim` for channels/bogs
 - `field`/`hills` only where terrain supports it
 - `inside` only for camps, ruins, warrens, huts, or cabins
- Preserve readability: avoid color-code spam; emphasize semantic color use.
- Minimum 3 sentences per room description; important rooms (bosses, junctions, entries) get 5+.
- No duplicate room descriptions across rooms.
- Named exits must be discoverable in room descriptions or extra descriptions.
- Extra descriptions chain from room description anchors (no orphan keywords).

## Quests

Static quests for Forest of Confusion content using IDs `25-34` (files `26.prop` through `35.prop`). These quests are distributed across Midgaard, Kiess, and Kowloon postmasters and align with courier, customs, and containment priorities tied to the forest's confusion-mist frontier.

Postmaster vnums:
- **Midgaard:** `931`
- **Kiess:** `3340`
- **Kowloon:** `3460`

### Non-Chain Quests

#### Quest 25 — Bell-Post Line Reopening (Kiess)

| Field | Value |
|---|---|
| **File** | `26.prop` |
| **Static ID** | 25 |
| **Title** | Bell-Post Line Reopening |
| **prerequisite_static_id** | -1 |
| **type** | 3 (KILL_COUNT) |
| **num_targets** | 1 |
| **kill_needed** | 8 |
| **min_level** | 18 |
| **max_level** | 38 |
| **offerer_vnum** | 3340 |
| **reward_gold** | 1700 |
| **reward_qp** | 2 |
| **reward_item_vnum** | 0 |
| **reward_item_count** | 0 |
| **Target vnums** | `2343` (young catrat) |
| **Accept message** | Kiess signal keepers cannot keep the southern bell-post line active while catrat packs keep denning under the posts and attacking runners at rope anchors. Thin the young packs around the fringe so repair crews can re-string the warning cords before the next mist surge. |
| **Completion message** | The bell-post crews report stable footing and fewer attacks while working the line. Kiess dispatch has resumed timed bell checks on the southern verge, restoring the first layer of early warning for patrol routes entering the forest. |

#### Quest 26 — Mirrorbark Predator Census (Midgaard)

| Field | Value |
|---|---|
| **File** | `27.prop` |
| **Static ID** | 26 |
| **Title** | Mirrorbark Predator Census |
| **prerequisite_static_id** | -1 |
| **type** | 1 (KILL_VARIETY) |
| **num_targets** | 3 |
| **kill_needed** | 0 |
| **min_level** | 28 |
| **max_level** | 49 |
| **offerer_vnum** | 931 |
| **reward_gold** | 2400 |
| **reward_qp** | 3 |
| **reward_item_vnum** | 0 |
| **reward_item_count** | 0 |
| **Target vnums** | `2346 2347 2349` (mirror stag, rootcat, whisper-owl) |
| **Accept message** | Midgaard's caravan insurers need current hazard records from the Mistroute Thickets after conflicting reports about predator migration under the confusion mists. Confirm contact with each major threat type so the Granite Arcade can update courier indemnity rates and route advisories. |
| **Completion message** | Your field confirmations let Midgaard revise its hazard tables with current data instead of old speculation. Courier contracts through Kiess can now price risk by actual predator distribution rather than blanket high-danger penalties. |

#### Quest 27 — Conclave Survey Slate Recovery (Kowloon)

| Field | Value |
|---|---|
| **File** | `28.prop` |
| **Static ID** | 27 |
| **Title** | Conclave Survey Slate Recovery |
| **prerequisite_static_id** | -1 |
| **type** | 2 (ACQUIRE_OBJECT) |
| **num_targets** | 1 |
| **kill_needed** | 0 |
| **min_level** | 34 |
| **max_level** | 58 |
| **offerer_vnum** | 3460 |
| **reward_gold** | 2800 |
| **reward_qp** | 3 |
| **reward_item_vnum** | 0 |
| **reward_item_count** | 0 |
| **Target vnums** | `2372` (damaged Conclave survey slate) |
| **Accept message** | Kowloon's magistrate-couriers are assembling a legal archive of abandoned Conclave claims that still affect northern customs rulings. Retrieve a readable survey slate from the Forest of Confusion so the city can challenge lingering paper rights tied to dead field stations. |
| **Completion message** | The recovered slate has been transferred to Kowloon's archive clerks for transcription and seal verification. Magistrate aides confirm it contains route geometry that supports nullifying several obsolete Conclave-era toll assertions. |

#### Quest 28 — Neogi Warrant Service (Kiess)

| Field | Value |
|---|---|
| **File** | `29.prop` |
| **Static ID** | 28 |
| **Title** | Neogi Warrant Service |
| **prerequisite_static_id** | -1 |
| **type** | 3 (KILL_COUNT) |
| **num_targets** | 1 |
| **kill_needed** | 1 |
| **min_level** | 35 |
| **max_level** | 60 |
| **offerer_vnum** | 3340 |
| **reward_gold** | 1116 |
| **reward_qp** | 4 |
| **reward_item_vnum** | 0 |
| **reward_item_count** | 0 |
| **Target vnums** | `2367` (Neogi ruler) |
| **Accept message** | Kiess has authorized a direct warrant against the Neogi ruler coordinating kidnappings from rope-line detachments and salvage crews. Serve the warrant in the warrens and end the command node directing those raids. |
| **Completion message** | With the Neogi ruler eliminated, Kiess scouts report fragmented slave-taking bands rather than organized raids. Missing-person alerts from the southern camps have already dropped, and warren pressure on channel crossings is easing. |

#### Quest 29 — Ashen Lattice Containment Sweep (Midgaard)

| Field | Value |
|---|---|
| **File** | `30.prop` |
| **Static ID** | 29 |
| **Title** | Ashen Lattice Containment Sweep |
| **prerequisite_static_id** | -1 |
| **type** | 1 (KILL_VARIETY) |
| **num_targets** | 3 |
| **kill_needed** | 0 |
| **min_level** | 42 |
| **max_level** | 68 |
| **offerer_vnum** | 931 |
| **reward_gold** | 3900 |
| **reward_qp** | 4 |
| **reward_item_vnum** | 0 |
| **reward_item_count** | 0 |
| **Target vnums** | `2359 2360 2362` (spore husk, mist-wolf, prism-leech swarm) |
| **Accept message** | Midgaard's herbal and timber consignments are being rejected as contaminated after passing near the Ashen Lattice scars. Clear representative blight carriers in the scar band so inspectors can certify outbound cargo again. |
| **Completion message** | Quarantine clerks confirm lower active spore density along inspected routes through the scar band. Midgaard's receiving houses have resumed normal intake for stamped Kiess manifests instead of automatic rejection. |

### Chain Quests

#### Kiess Wardline Restoration Chain (30-32)

##### Quest 30 — Wardline Reconsolidation

| Field | Value |
|---|---|
| **File** | `31.prop` |
| **Static ID** | 30 |
| **Title** | Wardline Reconsolidation |
| **prerequisite_static_id** | -1 |
| **type** | 1 (KILL_VARIETY) |
| **num_targets** | 2 |
| **kill_needed** | 0 |
| **min_level** | 48 |
| **max_level** | 74 |
| **offerer_vnum** | 3340 |
| **reward_gold** | 4300 |
| **reward_qp** | 4 |
| **reward_item_vnum** | 0 |
| **reward_item_count** | 0 |
| **Target vnums** | `2361 2358` (bark revenant, greater broken one) |
| **Accept message** | Kiess ward engineers cannot reset the old druidic marker ring while revenants and greater broken ones keep overrunning the scar perimeter at dusk. Break both threat groups so a repair team can hold position long enough to re-ink the ward lattice. |
| **Completion message** | Engineers held the perimeter through a full mist cycle and re-established the first wardline interval. The forest still shifts, but the repaired markers now give patrol captains a reliable baseline for route triangulation. |

##### Quest 31 — Yugoloth Contract Severance

| Field | Value |
|---|---|
| **File** | `32.prop` |
| **Static ID** | 31 |
| **Title** | Yugoloth Contract Severance |
| **prerequisite_static_id** | 30 |
| **type** | 3 (KILL_COUNT) |
| **num_targets** | 1 |
| **kill_needed** | 1 |
| **min_level** | 56 |
| **max_level** | 82 |
| **offerer_vnum** | 3340 |
| **reward_gold** | 5200 |
| **reward_qp** | 5 |
| **reward_item_vnum** | 0 |
| **reward_item_count** | 0 |
| **Target vnums** | `2380` (guardian yugoloth) |
| **Accept message** | Intercepted tablets indicate Ymmas hired extraplanar muscle to hold his cabin approaches while he continues experiments in the inner rooms. Eliminate the guardian yugoloth and collapse that contract line before Kiess commits a full strike team. |
| **Completion message** | The yugoloth's presence has ended, and the cabin perimeter no longer shows coordinated outsider tactics. Kiess command has authorized the final assault window on Ymmas's sanctum while his defenses are still destabilized. |

##### Quest 32 — Ymmas Lair Strike

| Field | Value |
|---|---|
| **File** | `33.prop` |
| **Static ID** | 32 |
| **Title** | Ymmas Lair Strike |
| **prerequisite_static_id** | 31 |
| **type** | 3 (KILL_COUNT) |
| **num_targets** | 1 |
| **kill_needed** | 1 |
| **min_level** | 60 |
| **max_level** | 90 |
| **offerer_vnum** | 3340 |
| **reward_gold** | 7000 |
| **reward_qp** | 6 |
| **reward_item_vnum** | 0 |
| **reward_item_count** | 0 |
| **Target vnums** | `2381` (the evil Ymmas) |
| **Accept message** | With the outer contract broken, Kiess issues a direct kill order on Ymmas, whose Conclave salvage rites are amplifying confusion-mist surges across active patrol bands. Enter the sanctum and end his operation before another bell-line collapses. |
| **Completion message** | Ymmas is dead, and mist-pressure readings around the central cabin complex have dropped below emergency thresholds. Kiess dispatch has reopened suspended patrol loops and logged the strike as a major stabilization event for the entire southern frontier. |

#### Kowloon Deep-Courier Security Chain (33-34)

##### Quest 33 — Cave Route Pacification

| Field | Value |
|---|---|
| **File** | `34.prop` |
| **Static ID** | 33 |
| **Title** | Cave Route Pacification |
| **prerequisite_static_id** | -1 |
| **type** | 1 (KILL_VARIETY) |
| **num_targets** | 2 |
| **kill_needed** | 0 |
| **min_level** | 44 |
| **max_level** | 70 |
| **offerer_vnum** | 3460 |
| **reward_gold** | 4200 |
| **reward_qp** | 4 |
| **reward_item_vnum** | 0 |
| **reward_item_count** | 0 |
| **Target vnums** | `2375 2379` (manscorpian scout, manscorpian hybrid) |
| **Accept message** | Kowloon courier lantern crews are testing a cave-linked relay route to bypass surface fog banks, but manscorpian patrols are collapsing marker lines before runners can map safe intervals. Remove both common patrol castes so survey crews can complete their relay chart. |
| **Completion message** | The cave relay team completed a full marker pass with only routine resistance. Kowloon has approved provisional use of the subterranean courier segment for high-priority winter dispatches. |

##### Quest 34 — Chitin Crown Decapitation

| Field | Value |
|---|---|
| **File** | `35.prop` |
| **Static ID** | 34 |
| **Title** | Chitin Crown Decapitation |
| **prerequisite_static_id** | 33 |
| **type** | 3 (KILL_COUNT) |
| **num_targets** | 1 |
| **kill_needed** | 1 |
| **min_level** | 52 |
| **max_level** | 80 |
| **offerer_vnum** | 3460 |
| **reward_gold** | 6100 |
| **reward_qp** | 5 |
| **reward_item_vnum** | 0 |
| **reward_item_count** | 0 |
| **Target vnums** | `2377` (manscorpian king) |
| **Accept message** | Initial pacification proved the cave route viable, but the manscorpian king still commands concentrated strikes from the deepest chamber and can rebuild losses faster than patrols can clear them. Eliminate the king to collapse centralized resistance and secure the relay corridor for Kowloon's long-haul post. |
| **Completion message** | With the manscorpian king removed, coordinated cave ambushes have stopped and remaining packs are splintering into disorganized scavenger cells. Kowloon's courier office has formally designated the route as a protected contingency lane during heavy fog seasons. |

## Implementation Checklist
1. Rebuild `#ROOMS` so vnums 2339-2538 are fully populated and connected per zone plan.
2. Replace room text set with unique, lore-consistent descriptions following the color theme.
3. Rebuild `#MOBILES` per faction tables above with correct act flags, profiles, and extensions.
4. Rebuild `#OBJECTS` per equipment and utility tables above with correct types, wear flags, and weight ranges.
5. Update `#RESETS` for coherent spawn pacing and checkpoint logic per encounter rhythm plan.
6. Write `#SPECIALS` per mob specials table.
7. Validate all external links (Kiess/Roc Road, Verdant Depths, Withered Depths, legacy connections).
8. Verify all maze rooms have `ROOM_MAZE` flag and all non-maze rooms have bidirectional exits.
9. Verify door policy compliance (locked doors have keys, `D` resets match `EX_ISDOOR` exits).
10. Verify color policy and owner policy compliance.
11. Run area load validation and traversal sanity checks.
