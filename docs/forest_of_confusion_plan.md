# Area Plan: Forest of Confusion

## Overview
- **Area Name:** `@@GForest @@Wof @@dConfusion@@N`
- **File Name:** `confusn.are`
- **Keyword:** `forestconfusion`
- **Intended Levels:** 10-60 (bridge zone between Kiess frontier traffic and deeper forest content)
- **Vnum Range:** `9600-9799`
- **Vnum Count:** 200 (all vnums assigned to rooms/objects/mobs; no unused room vnums)
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
- `V 9600 9799` (unchanged; required envelope)
- `X 32200` (retain map offset)
- `F 15`
- `U @@Wthe @@pmists @@Wshift and the @@Gforest @@Wrearranges itself...@@N~`
- `T` teleport flag retained
- `O Virant~` per area ownership policy

## Layout Rebuild Strategy (all rooms used)
Replace the current partial/legacy topology with a full 200-room design:

- **Grid model:** 20 x 10
- **Coordinate system:** `x=0..19` west->east, `y=0..9` north->south
- **Vnum mapping:** `vnum = 9600 + (y * 20) + x`
- **Coverage rule:** every vnum 9600-9799 is a valid room (no gaps)

## Zone Banding (room-level identity)

### Zone 1: South Verge of Kiess (9600-9639, rows y=0-1)
**Theme:** Kiess frontier infrastructure in slow retreat. Patrol roads, survey stakes, toll ruins, evac bells.
**Level range:** 10-20
**Sector:** primarily `forest`, `field` for cleared camps, `inside` for the survey station
**Room flags:** most rooms unflagged; camps and station get `no_mob`
**Mood:** organized anxiety — competent people losing ground to a forest that does not respect competence.

Row y=0 (vnums 9600-9619): The patrol road and its flanking positions.
- 9600: South Verge Gatehouse — a roofless stone archway where Roc Road ends and forest begins. The last Kiess waystone stands here. `inside` sector.
- 9601: Roc Road Terminus — the final stretch of maintained road, packed earth giving way to root-buckled stone. **Primary south connection: exit west to room 3240 (Roc Road).**
- 9602-9605: Patrol Road sections running east — increasingly overgrown, rope-line anchors visible, bell-posts with frayed signal cords.
- 9606: Bell-Post Clearing — an open break in the canopy where three bell-posts converge. The bells still ring when the mist shifts. `field` sector.
- 9607-9610: Patrol Road continuation east — survey stakes with faded Kiess markings, equipment caches half-buried by root growth.
- 9611: Waystone Junction — a crossroads where two patrol routes once intersected. The waystones bear Kiess compass-and-wall crests.
- 9612-9615: Patrol Road trailing east — the road narrows, the canopy closes, and the mist begins to pool at ground level.
- 9616: Southern Timber Cut — a cleared area where Kiess logging teams once worked. Stumps show saw marks; new growth has already reached head height. **Exit south to room 9052 (external connection, retained from legacy).**
- 9617-9619: Forest edge positions — the patrol road dissolves into game trails. Rope-line fragments lead north into thicker growth.

Row y=1 (vnums 9620-9639): Patrol camps, abandoned survey positions, and the forest's first confusion effects.
- 9620-9623: Abandoned Ranger Camp sequence — fire-scarred stone rings, collapsed lean-tos, dispatch tablets scattered in leaf litter. One room is the camp's former command post (`inside` sector, `no_mob`, `safe`).
- 9624-9627: Undergrowth Trail sections — the patrol road's northern spur, increasingly obscured by vine growth and root displacement.
- 9628-9631: Frayed Rope-Line Corridor — a once-maintained navigation line now hanging in loops. The mist here thickens noticeably. Directional confusion begins: exit arrangements become subtly asymmetric.
- 9632-9635: Warning Bell Perimeter — the outermost ring of Kiess bell-posts, most silent. One still sounds when mist pressure changes, providing an auditory landmark.
- 9636-9639: Southern Verge Fringe — the last positions with visible Kiess infrastructure. Ward-stones with corroded inscriptions. North exits lead into the Mistroute Thickets.

### Zone 2: Mistroute Thickets (9640-9679, rows y=2-3)
**Theme:** Directional confusion mechanics, mirrored groves, looping trails. The three-force overlap is first felt here.
**Level range:** 18-35
**Sector:** primarily `forest`, occasional `water_swim` for seep-pools
**Room flags:** most rooms unflagged; maze sub-region rooms get `ROOM_MAZE`
**Mood:** structural confusion — two competing logics (Verdant and Withered) producing unpredictable navigation.

Row y=2 (vnums 9640-9659): The Mirrorbark Switchmaze and its approaches.
- 9640-9643: Mistroute approaches — the canopy closes fully, light becomes diffuse, and the first mirrorbark trees appear. Their reflective bark surfaces create visual echoes of torchlight.
- **9644-9659: The Mirrorbark Switchmaze** (16 rooms, all `ROOM_MAZE`)
  - A dense thicket maze with repeated visual motifs and short looping connectors.
  - False-loop lattice with 2-3 true exits (south back to approaches, east to row y=2 continuation, north to row y=3).
  - Theme emphasis: `@@p` mist pressure + `@@G` living bark intelligence.
  - Clue language: bark reflections — left-reflection indicates south, overhead-reflection indicates true north.
  - Each room gets unique description despite shared visual motifs; anchors include specific bark patterns, root formations, mist density variations, and sound quality changes.
  - Dead-end pockets contain optional loot (druidic remnants, Kiess scout equipment).

Row y=3 (vnums 9660-9679): Post-maze thickets, catrat territory, and first broken-one encounters.
- 9660-9663: Switchmaze exits and recovery — the maze releases into slightly clearer thickets. Catrat family groups establish territory here.
- 9664-9667: Catrat Warren approaches — undergrowth dense with small-predator signs: gnawed bark, prey bones, territorial scratch-marks.
- 9668-9671: Broken-One Territory — the first evidence of Withered corruption on sentient beings. Degraded campsites, smashed equipment, and the shambling residents who once were people.
- 9672-9675: Neogi Fringe — concealed entrances to underground passages begin appearing. Signs of slave-taking: drag marks, abandoned restraints, fear-scent.
- 9676-9679: Seep-Pool Crossing — shallow water collects in root hollows, creating a wet barrier between the thickets and the channels below. `water_swim` sector for pool rooms.

### Zone 3: Shivergrove Channels (9680-9719, rows y=4-5)
**Theme:** Wet ravines, prism pools, root-bridges, partial verticality. The Prism-Fen Knot anchors this zone.
**Level range:** 25-45
**Sector:** mixed `forest`/`water_swim`/`inside` (for checkpoint hut)
**Room flags:** maze sub-region rooms get `ROOM_MAZE`; checkpoint hut gets `no_mob`
**Mood:** disorienting openness — the terrain appears navigable but folds back on itself.

Row y=4 (vnums 9680-9699): The Prism-Fen Knot, neogi warrens, and channel crossings.
- 9680-9683: Channel Approaches — the terrain drops into shallow ravines where water flows over root-bridges. The mist takes on a crystalline quality as Conclave fragments in the channel beds refract light.
- 9684-9689: Neogi Warrens — underground passages accessed through concealed entrances in the undergrowth. `inside` sector. The neogi hierarchy operates here: slave pens, guard posts, the master's chamber, and the ruler's throne. Rooms are tight, deliberately claustrophobic.
  - 9684: Neogi Tunnel Mouth — `inside` sector
  - 9685: Slave Pen — `inside` sector, `dark`
  - 9686: Neogi Guard Post — `inside` sector
  - 9687: Neogi Master's Chamber — `inside` sector
  - 9688: Neogi Ruler's Hall — `inside` sector, `no_mob`
  - 9689: Neogi Escape Tunnel — `inside` sector, connects back to surface
- 9690-9694: Root-Bridge Crossings — elevated root platforms spanning channels. The bridges shift as the root network redirects water flow.
- 9695: Dense Woodland Junction — a critical intersection point. **Exit east to room 10000 (Verdant Depths entry). Exit south to room 1000 (Withered Depths entry).** This room is the geographic pivot where the forest's two deeper regions become accessible. `no_mob`.
- 9696-9699: Deep Channel sections — the channels widen and deepen. Prism-scored fragments visible in the streambeds. Cockatrice territory begins.

Row y=5 (vnums 9700-9719): The Prism-Fen Knot, cockatrice nesting, and hippogriff canopy.
- **9700-9715: The Prism-Fen Knot** (16 rooms, all `ROOM_MAZE`)
  - Wetland maze using channels and root-bridges where pathing appears open but folds back.
  - Mixed `forest`/`water_swim` sectors.
  - One checkpoint hut at 9707: `inside` sector, `no_mob` — an old Kiess survey station providing re-orientation anchor.
  - Theme emphasis: `@@a` refracted moisture + `@@b` root/earth hazards.
  - Clue language: water-sound direction — flowing water on the left indicates east, flowing water ahead indicates north.
  - Prism-light distortions in the mist create rainbow false-paths; true paths are marked by absence of prismatic effect.
- 9706: Large Nesting Tree — an ancient old-growth tree massive enough to support hippogriff nests in its upper canopy. The tree is a Verdant-influenced landmark. Exit up to 9716.
- 9716: Hippogriff Nest — high canopy platform above the mist line. `forest` sector. The air is clear here; the confusion effects are weaker above the canopy.
- 9717-9719: Fen Exit and Glade Approach — the wetland gives way to drier ground. The canopy opens slightly. Cockatrice territory: cleared areas where the stone-gaze creatures display their crests.

### Zone 4: Conclave Scars (9720-9759, rows y=6-7)
**Theme:** Blight circles, crystal fractures, quarantine relics. Withered influence dominates.
**Level range:** 35-55
**Sector:** primarily `forest`, `inside` for cabin complex and caves
**Room flags:** maze sub-region rooms get `ROOM_MAZE`; boss rooms get `no_mob`
**Mood:** hostile and deliberate — the corruption is not random but follows the geometry of abandoned Conclave infrastructure.

Row y=6 (vnums 9720-9739): Wemic territory, Ymmas's domain, centaur approaches.
- 9720-9722: Wemic Hunting Grounds — deep clearings where wemic family bands (male, female, cubs) establish territory. The clearings are Verdant-influenced pockets: the air is clearer, the growth is healthier, and the wemics patrol with territorial precision. `no_mob` flag on wemic den room.
  - **Exit north from 9722 to room 6600 (external connection, retained from legacy).**
- 9723-9725: Crystal Scar Approaches — the first visible evidence of Conclave crystal matrices. Root systems exposed by erosion show faintly luminous fragments. The air hums. Survey trench remnants cut narrow lines in the forest floor.
  - **Exit up from 9725 to room 166 (external connection, retained from legacy).**
- 9726-9732: Ymmas's Domain — a fortified cabin complex in the deep woodland.
  - 9726: Cabin Approach — the path narrows between sentinel trees. The Guardian Yugoloth patrols here. `no_mob`.
  - 9727: Strongroom — a sealed chamber within the cabin. `inside`, `no_mob`. Door: locked, requires key.
  - 9728: Cabin Main Room — `inside` sector. The anomaly: a human-made structure that has not been reclaimed by the forest.
  - 9729: Cabin Workshop — `inside` sector. Shelves of reagents, star charts, and bound texts.
  - 9730: Cabin Upper Room — `inside` sector. Ymmas's personal quarters.
  - 9731: Ymmas's Lair — `inside`, `no_mob`. The mage's sanctum. Ymmas (level 86 boss) resides here.
  - 9732: Throne Room — `inside`, `no_mob`. A formal audience chamber. The Staff of Summoning is kept here.
- 9733-9735: Deep Undergrowth — transitional rooms between Ymmas's domain and centaur territory. Dense, dark, and contested.
- 9736-9739: Centaur Glade Approaches — living-wood boundary markers appear. Woven bone-and-bark windchimes produce territorial tones. The forest shifts from corrupted to carefully maintained.

Row y=7 (vnums 9740-9759): Centaur settlement, the Ashen Lattice maze, and manscorpian approaches.
- 9740-9747: Centaur Settlement — a secluded glade of living-wood structures.
  - 9740: Glade Entrance — centaur boundary markers. The forest here is healthier than anywhere else in the area outside the Verdant-influenced pockets.
  - 9741-9743: Secluded Glade rooms — open spaces between trained trees. Centaur families move between these. `forest` sector.
  - 9744: Leader's Glade — `no_mob`. The centaur leader holds court here. Centaur oral tradition preserves accounts of the forest before the confusion mists.
  - 9745-9747: Centaur Huts — `inside` sector. Living-wood structures, doors of woven branches. Each hut has unique character.
- **9748-9759: The Ashen Lattice of Scars** (12 rooms, all `ROOM_MAZE`)
  - Blight-geometry maze built around Conclave scar lines and fungal lattice corridors.
  - Route feels hostile and deliberate: dead-end bait pockets, elite ambush corners, one reliable traversal lane.
  - Theme emphasis: `@@d` withered decay + `@@R` breach danger + restrained `@@p` distortion cues.
  - Clue language: fire-scar depth — the deepest, oldest fire scars mark the original Conclave survey routes, which remain navigable because the crystal resonance that created them also preserved them.
  - Fungal growth follows Conclave geometric patterns: hexagonal spore formations, lattice-structured mycelium.
  - 9755: Lattice Core — the densest concentration of crystal resonance at the surface. The air hums audibly. Boss-tier creature territory.

### Zone 5: Twin Thresholds (9760-9799, rows y=8-9)
**Theme:** The north arc forks into Verdant-biased and Withered-biased transition routes. Manscorpian caves. The Spider's web.
**Level range:** 45-60
**Sector:** mixed `forest`/`inside` (for caves)
**Room flags:** cave maze rooms get `ROOM_MAZE`; boss rooms get `no_mob`
**Mood:** threshold anticipation — the forest is about to become something else, and the two possible destinations are starkly different.

Row y=8 (vnums 9760-9779): Manscorpian cave maze, the spider's domain, and the Verdant approach.
- 9760-9762: Deep Woodland Transition — the forest reaches its maximum density. Two distinct environmental flavors begin: healthier growth to the east (Verdant influence), corrupted growth to the west (Withered influence).
- **9763-9778: Manscorpian Cave Maze** (16 rooms, all `ROOM_MAZE`)
  - The manscorpian king's domain. Caves accessed through concealed entrances.
  - `inside` sector throughout. `dark` flag on unlit chambers.
  - 9763: Cave Mouth — the entrance, guarded by manscorpian sentries.
  - 9764-9770: Maze of Caves — twisting passages navigated by the manscorpians via scent. Poison hazards.
  - 9770: Deep Cave Junction — **Exit south to room 10062 (Verdant Depths cave shortcut, retained from legacy).** `no_mob`.
  - 9771-9774: Inner Caves — the king's territory. More defended, better-lit by bioluminescent fungal growth from the deep root tunnels below.
  - 9775: King's Chamber — `inside`, `no_mob`. The manscorpian king (level 52, boss) and his aid.
  - 9776-9778: Cave exits and connecting passages back to surface.
- 9779: The Spider's Web — a massive web complex spanning multiple trees at the Verdant-Withered boundary. The spider (level 57, solo) hunts here. `no_mob`.

Row y=9 (vnums 9780-9799): The twin threshold approaches and final deep-forest positions.
- 9780-9784: Verdant Threshold — the eastern approach. The forest grows taller, the mist thins, and the root network's chemical signaling becomes a perceptible hum. Treants patrol these rooms as Verdant sentinels. The canopy opens briefly, providing glimpses of the ancient growth beyond.
- 9785-9789: Withered Threshold — the western approach. The forest darkens, bark blackens, and the preservation magic's chemical signature (sweet rot, metallic tang) becomes prominent. Broken ones and greater broken ones congregate. Failed quarantine circles — stone rings with flickering ward-signs that no longer seal anything — mark the positions where the druids made their last stand.
- 9790-9794: Deep Contested Band — the narrow strip where Verdant and Withered influences collide directly. Rooms alternate between eerie health and grinding corruption. The most dangerous non-boss encounters in the area occur here as creatures from both influence zones clash.
- 9795-9797: Northern Ridge — elevated positions where the forest canopy breaks enough to see the terrain ahead. The view north splits: verdant ancient growth to the east, blackened withered canopy to the west. These rooms provide narrative orientation — the player can see where both paths lead.
- 9798: Verdant Overlook — a high-canopy perch in a Verdant-influenced giant tree. The air is clear. The confusion effects lift entirely for the first time. The forest's intelligence is palpable as attention rather than disorientation. The view shows the Verdant Depths' ancient canopy stretching northward.
- 9799: Withered Overlook — a blighted ridge where the preservation magic's purple glow is visible in the root systems below. The confusion effects are replaced by a different kind of wrongness: everything is clear, but the clarity reveals a landscape of animate rot. The view shows the Withered Depths' blackened canopy extending northward.

## Maze Sub-Regions (mandatory thematic cores)

### 1. The Mirrorbark Switchmaze (9644-9659)
- 16 rooms, all `ROOM_MAZE`
- Dense thicket maze with repeated visual motifs and short looping connectors
- False-loop lattice with 2-3 true exits
- Theme emphasis: `@@p` mist pressure + `@@G` living bark intelligence
- Clue language: bark reflections (left = south, overhead = north)

### 2. The Prism-Fen Knot (9700-9715)
- 16 rooms, all `ROOM_MAZE`
- Wetland maze using channels and root-bridges
- Mixed `forest`/`water_swim` sectors; one `inside` checkpoint hut (9707) as re-orientation anchor
- Theme emphasis: `@@a` refracted moisture + `@@b` root/earth hazards
- Clue language: water-sound direction (left = east, ahead = north)

### 3. The Ashen Lattice of Scars (9748-9759)
- 12 rooms, all `ROOM_MAZE`
- Blight-geometry maze around Conclave scar lines and fungal lattice corridors
- Dead-end bait pockets, elite ambush corners, one reliable traversal lane
- Theme emphasis: `@@d` withered decay + `@@R` breach danger + `@@p` distortion
- Clue language: fire-scar depth (deepest scars mark the reliable traversal lane)

### 4. Manscorpian Cave Maze (9763-9778)
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
| 9601 | west | 3240 | Roc Road (primary Kiess approach) |
| 9616 | south | 9052 | External connection (legacy) |
| 9695 | east | 10000 | Verdant Depths entry (surface) |
| 9695 | south | 1000 | Withered Depths entry |
| 9722 | north | 6600 | External connection (legacy) |
| 9725 | up | 166 | External connection (legacy) |
| 9770 | south | 10062 | Verdant Depths entry (cave shortcut) |

### Internal Connectivity Rules
- Main progression spine runs roughly south-to-north through the grid, with east-west branching.
- Maze sub-regions have controlled entry/exit points; internal routing is non-linear.
- Side webs produce confusion without hard-locking progress.
- All non-maze rooms maintain bidirectional exit reciprocity.

### Door Policy
- Only man-made checkpoints and quarantine gates use door flags; natural forest transitions are open.
- Doors required:
  - Ymmas's strongroom (9727): locked door, requires key (emerald key, vnum 9619)
  - Ymmas's lair entrance: closed door
  - Centaur hut doors (9745-9747): closed doors (woven branch construction)
  - Neogi ruler's hall (9688): closed door
  - Manscorpian king's chamber (9775): closed door

## Mob/NPC Redesign Plan

### Faction 1: Kiess Expeditionary Presence (levels 12-25)
Sentinel or patrol mobs at camps and checkpoints in Zone 1. Non-aggressive.

| Vnum | Name | Short | Level | Profile | Placement |
|------|------|-------|-------|---------|-----------|
| 9600 | warden kiess patrol | a Kiess patrol warden | 20 | melee | 9606, 9611 (sentinel) |
| 9601 | rope-line team scout | a rope-line scout | 15 | melee | 9628-9631 |
| 9602 | medic kiess field | a Kiess field medic | 18 | hybrid | 9620-9623 (sentinel) |
| 9603 | signal keeper bell | a signal keeper | 12 | melee | 9632-9635 (sentinel) |

### Faction 2: Forest Wildlife — Baseline (levels 10-20)
Aggressive small predators in Zone 1 and Zone 2 fringe.

| Vnum | Name | Short | Level | Profile | Placement |
|------|------|-------|-------|---------|-----------|
| 9604 | catrat young | a young catrat | 12 | melee | 9636-9639 |
| 9605 | catrat female | a female catrat | 10 | melee | 9664-9667 |
| 9606 | catrat male | a male catrat | 14 | melee | 9664-9667 |

### Faction 3: Verdant-Touched Fauna (levels 20-50)
Ambush/territorial behavior in healthy pockets. Aggressive.

| Vnum | Name | Short | Level | Profile | Placement |
|------|------|-------|-------|---------|-----------|
| 9607 | mirror stag verdant | a mirror stag | 22 | melee | 9644-9659 (switchmaze) |
| 9608 | rootcat verdant | a rootcat | 28 | melee | 9690-9694 |
| 9609 | canopy drake verdant | a canopy drake | 35 | melee | 9716, 9780-9784 |
| 9610 | whisper-owl verdant | a whisper-owl | 20 | caster | 9640-9643 |
| 9611 | cockatrice male cock | a male cockatrice | 30 | melee | 9717-9719 |
| 9612 | cockatrice female cock | a female cockatrice | 32 | melee | 9696-9699 |
| 9613 | hippogriff young hippo | a young hippogriff | 25 | melee | 9716 |
| 9614 | hippogriff hippo | a hippogriff | 35 | melee | 9706, 9716 |
| 9615 | treant tree | a treant | 33 | melee | 9780-9784, 9790-9794 |
| 9616 | treant elder tree | an elder treant | 42 | melee | 9798 (sentinel) |

### Faction 4: Withered-Blight Creatures (levels 25-55)
Stronger in Conclave Scar bands and Withered threshold. Aggressive.

| Vnum | Name | Short | Level | Profile | Placement |
|------|------|-------|-------|---------|-----------|
| 9617 | broken one | a broken one | 27 | melee | 9668-9671 |
| 9618 | broken one | a broken one | 30 | melee | 9785-9789 |
| 9619 | greater broken one | a greater broken one | 39 | melee | 9785-9789, 9790-9794 |
| 9620 | spore husk withered | a spore husk | 35 | melee | 9748-9759 (lattice) |
| 9621 | mist-wolf withered | a mist-wolf | 32 | melee | 9748-9759, 9785-9789 |
| 9622 | bark revenant withered | a bark revenant | 45 | melee | 9790-9794 |
| 9623 | prism-leech swarm | a prism-leech swarm | 40 | caster | 9723-9725, 9755 |

### Faction 5: Neogi Slavers (levels 15-35)
Sentinel in warrens. Aggressive.

| Vnum | Name | Short | Level | Profile | Placement |
|------|------|-------|-------|---------|-----------|
| 9624 | neogi slave | a Neogi slave | 15 | melee | 9685 |
| 9625 | neogi slave | a Neogi slave | 20 | melee | 9684, 9689 |
| 9626 | neogi | a Neogi | 22 | melee | 9686 |
| 9627 | neogi master | a Neogi master | 28 | hybrid | 9687 |
| 9628 | neogi ruler | the Neogi ruler | 35 | caster | 9688 (sentinel) |

### Faction 6: Centaur Society (levels 22-50)
Mixed aggressive/non-aggressive in glade settlement.

| Vnum | Name | Short | Level | Profile | Placement |
|------|------|-------|-------|---------|-----------|
| 9629 | centaur male | a male centaur | 49 | melee | 9741-9743 |
| 9630 | centaur female | a female centaur | 42 | melee | 9741-9743 |
| 9631 | centaur baby | a baby centaur | 22 | melee | 9745-9747 |
| 9632 | centaur leader | the centaur leader | 50 | hybrid | 9744 (sentinel) |

### Faction 7: Wemic Bands (levels 34-50)
Territorial aggressive in deep clearings.

| Vnum | Name | Short | Level | Profile | Placement |
|------|------|-------|-------|---------|-----------|
| 9633 | wemic male | a male wemic | 50 | melee | 9720 |
| 9634 | wemic female | a female wemic | 48 | melee | 9721 |
| 9635 | wemic cub | a wemic cub | 34 | melee | 9720-9722 |

### Faction 8: Manscorpian Colony (levels 43-52)
Territorial aggressive in cave maze.

| Vnum | Name | Short | Level | Profile | Placement |
|------|------|-------|-------|---------|-----------|
| 9636 | manscorpian man scorpian | a manscorpian | 43 | melee | 9763-9770 |
| 9637 | manscorpian man scorpian | a manscorpian | 51 | melee | 9771-9774 |
| 9638 | manscorpian king man scorpian | the manscorpian king | 52 | melee | 9775 (sentinel, boss) |
| 9639 | manscorpian aid | the king's aid | 46 | caster | 9775 (sentinel) |
| 9640 | manscorpian man scorpian | a manscorpian | 51 | hybrid | 9771-9774 |

### Faction 9: Named Encounters (levels 55-86)
Boss and solo mobs. Sentinel in specific rooms.

| Vnum | Name | Short | Level | Profile | Placement |
|------|------|-------|-------|---------|-----------|
| 9641 | yugoloth guardian | a Guardian Yugoloth | 65 | hybrid | 9726 (sentinel, solo) |
| 9642 | ymmas | the evil Ymmas | 60 | caster | 9731 (sentinel, boss) |
| 9643 | spider | a giant spider | 57 | melee | 9779 (sentinel, solo) |
| 9644 | mist cartographer | the Mist Cartographer | 55 | hybrid | 9695 or roaming Zone 3 (solo) — a neutral mini-boss event mob; a former Kiess survey captain who learned the forest's logic and refuses to return |

### Mob act flags policy
- All mobs: `stay_area` (mandatory per spec)
- Kiess expeditionary (9600-9603): `sentinel`, non-aggressive
- Wildlife and fauna: `aggressive`, `stay_area`
- Boss mobs (9638 manscorpian king, 9642 Ymmas): `sentinel`, `boss`, placed in `no_mob` rooms
- Solo mobs (9641 yugoloth, 9643 spider, 9644 cartographer): `solo`, `sentinel`
- Sub-region containment: mobs without `sentinel` must be in regions enclosed by `no_mob` rooms

### Mob specials
| Vnum | Special |
|------|---------|
| 9642 (Ymmas) | `spec_cast_mage` |
| 9632 (Centaur Leader) | `spec_cast_cleric` |
| 9640 (Manscorpian hybrid) | `spec_cast_mage` |
| 9639 (King's Aid) | `spec_cast_cleric` |
| 9637 (Manscorpian lv51) | `spec_poison` |
| 9636 (Manscorpian lv43) | `spec_poison` |
| 9638 (Manscorpian King) | `spec_poison` |
| 9643 (Spider) | `spec_poison` |
| 9628 (Neogi Ruler) | `spec_cast_mage` |

## Object Redesign Plan

Replace legacy generic drops with forest-specific equipment and utility objects. All objects must include `ITEM_TAKE` in wear_flags. Object names must be unique within the area.

### Equipment Objects (mob drops and reset equipment)

| Vnum | Name | Short | Type | Wear | Weight | Level | Notes |
|------|------|-------|------|------|--------|-------|-------|
| 9600 | neogi whip barbed | a barbed Neogi whip | weapon | hold take | 6 | 22 | value3=4 (whip) |
| 9601 | jewelled dagger neogi | a jewelled Neogi dagger | weapon | hold take | 3 | 28 | value3=2 (stab) |
| 9602 | fur coat catrat | a catrat-fur coat | armor | body take | 8 | 12 | |
| 9603 | rootbound talisman verdant | a rootbound talisman | armor | neck take | 2 | 30 | |
| 9604 | mirrorbark buckler | a mirrorbark buckler | armor | hold take | 12 | 25 | extra: buckler |
| 9605 | stone shield centaur | a centaur stone shield | armor | hold take | 14 | 42 | |
| 9606 | stone hammer centaur | a centaur stone hammer | weapon | hold take | 10 | 42 | value3=7 (pound) |
| 9607 | emerald sword | an emerald sword | weapon | hold take | 8 | 49 | value3=3 (slash) |
| 9608 | emerald breastplate | an emerald breastplate | armor | body take | 14 | 49 | |
| 9609 | emerald helm | an emerald helm | armor | head take | 10 | 49 | |
| 9610 | emerald boots | a pair of emerald boots | armor | feet take | 8 | 49 | |
| 9611 | emerald leggings | emerald leggings | armor | legs take | 10 | 49 | |
| 9612 | oaken club centaur | an oaken club | weapon | hold take | 9 | 35 | value3=8 (crush) |
| 9613 | lance centaur | a centaur lance | weapon | hold take | 10 | 45 | extra: two-handed, value3=11 (pierce) |
| 9614 | red battle axe manscorpian | a red battle axe | weapon | hold take | 9 | 48 | value3=3 (slash) |
| 9615 | red javelin manscorpian | a red javelin | weapon | hold take | 7 | 43 | value3=11 (pierce) |
| 9616 | red bracer manscorpian | a red bracer | armor | wrist take | 8 | 48 | |
| 9617 | leather belt scout | a scout's leather belt | armor | waist take | 6 | 15 | |
| 9618 | anti-spore wrap kiess | an anti-spore wrap | armor | face take | 3 | 18 | |
| 9619 | emerald key | an emerald key | key | take | 1 | 49 | unlocks 9727 |
| 9620 | oak shield centaur | an oak shield | armor | hold take | 12 | 40 | |
| 9621 | gnarled staff ymmas | a gnarled staff | weapon | hold take | 4 | 60 | extra: wand, value3=7 (pound) |
| 9622 | bark claw cockatrice | a large yellow claw | weapon | hold take | 6 | 30 | value3=5 (claw) |
| 9623 | primitive club broken | a primitive club | weapon | hold take | 8 | 27 | value3=8 (crush) |
| 9624 | branch treant | a treant branch | weapon | hold take | 10 | 33 | value3=7 (pound) |
| 9625 | wemic fang necklace | a wemic fang necklace | armor | neck take | 7 | 48 | |

### Utility and Lore Objects

| Vnum | Name | Short | Type | Wear | Weight | Level | Notes |
|------|------|-------|------|------|--------|-------|-------|
| 9626 | food pack kiess | a Kiess food pack | food | take | 3 | 1 | placed in camp rooms |
| 9627 | furry pouch catrat | a small furry pouch | container | take | 2 | 10 | |
| 9628 | yellow stone light | a yellow stone | light | take | 1 | 15 | |
| 9629 | gold pile | a pile of gold | money | take | 1 | 1 | |
| 9630 | blue scroll magic | a blue scroll | scroll | take | 1 | 25 | |
| 9631 | magic potion vial | a small vial of magic potion | potion | take | 1 | 20 | |
| 9632 | dispatch tablet kiess | a Kiess dispatch tablet | treasure | take | 3 | 1 | lore object — Kiess route notation on flat slate |
| 9633 | conclave survey slate | a damaged Conclave survey slate | treasure | take | 4 | 1 | lore object — geometric notation, crystal-implant diagrams |
| 9634 | druid quarantine marker | a druid quarantine marker | treasure | take | 5 | 1 | lore object — weathered ward-sign on river-washed stone |
| 9635 | prism shard crystal | a prism shard | treasure | take | 1 | 30 | lore object — faintly luminous Conclave crystal fragment |
| 9636 | staff summoning | the Staff of Summoning | staff | hold take | 4 | 60 | Ymmas's artifact, placed in room 9732 |

### Boss Loot Objects (ITEM_LOOT + ITEM_BOSS)

| Vnum | Name | Short | Type | Wear | Weight | Level | Notes |
|------|------|-------|------|------|--------|-------|-------|
| 9637 | ymmas grimoire | Ymmas's grimoire | armor | hold take | 2 | 60 | boss loot |
| 9638 | manscorpian crown chitin | a chitin crown | armor | head take | 8 | 52 | boss loot |
| 9639 | cartographer compass mist | the Mist Cartographer's compass | armor | hold take | 3 | 55 | solo loot |

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

## Propositions

Static propositions for Forest of Confusion content using IDs `25-34` (files `26.prop` through `35.prop`). These propositions are distributed across Midgaard, Kiess, and Kowloon postmasters and align with courier, customs, and containment priorities tied to the forest's confusion-mist frontier.

Postmaster vnums:
- **Midgaard:** `3015`
- **Kiess:** `13001`
- **Kowloon:** `14021`

### Non-Chain Propositions

#### Proposition 25 — Bell-Post Line Reopening (Kiess)

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
| **offerer_vnum** | 13001 |
| **reward_gold** | 1700 |
| **reward_qp** | 2 |
| **reward_item_vnum** | 0 |
| **reward_item_count** | 0 |
| **Target vnums** | `9604` (young catrat) |
| **Accept message** | Kiess signal keepers cannot keep the southern bell-post line active while catrat packs keep denning under the posts and attacking runners at rope anchors. Thin the young packs around the fringe so repair crews can re-string the warning cords before the next mist surge. |
| **Completion message** | The bell-post crews report stable footing and fewer attacks while working the line. Kiess dispatch has resumed timed bell checks on the southern verge, restoring the first layer of early warning for patrol routes entering the forest. |

#### Proposition 26 — Mirrorbark Predator Census (Midgaard)

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
| **offerer_vnum** | 3015 |
| **reward_gold** | 2400 |
| **reward_qp** | 3 |
| **reward_item_vnum** | 0 |
| **reward_item_count** | 0 |
| **Target vnums** | `9607 9608 9610` (mirror stag, rootcat, whisper-owl) |
| **Accept message** | Midgaard's caravan insurers need current hazard records from the Mistroute Thickets after conflicting reports about predator migration under the confusion mists. Confirm contact with each major threat type so the Granite Arcade can update courier indemnity rates and route advisories. |
| **Completion message** | Your field confirmations let Midgaard revise its hazard tables with current data instead of old speculation. Courier contracts through Kiess can now price risk by actual predator distribution rather than blanket high-danger penalties. |

#### Proposition 27 — Conclave Survey Slate Recovery (Kowloon)

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
| **offerer_vnum** | 14021 |
| **reward_gold** | 2800 |
| **reward_qp** | 3 |
| **reward_item_vnum** | 0 |
| **reward_item_count** | 0 |
| **Target vnums** | `9633` (damaged Conclave survey slate) |
| **Accept message** | Kowloon's magistrate-couriers are assembling a legal archive of abandoned Conclave claims that still affect northern customs rulings. Retrieve a readable survey slate from the Forest of Confusion so the city can challenge lingering paper rights tied to dead field stations. |
| **Completion message** | The recovered slate has been transferred to Kowloon's archive clerks for transcription and seal verification. Magistrate aides confirm it contains route geometry that supports nullifying several obsolete Conclave-era toll assertions. |

#### Proposition 28 — Neogi Warrant Service (Kiess)

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
| **offerer_vnum** | 13001 |
| **reward_gold** | 3200 |
| **reward_qp** | 4 |
| **reward_item_vnum** | 0 |
| **reward_item_count** | 0 |
| **Target vnums** | `9628` (Neogi ruler) |
| **Accept message** | Kiess has authorized a direct warrant against the Neogi ruler coordinating kidnappings from rope-line detachments and salvage crews. Serve the warrant in the warrens and end the command node directing those raids. |
| **Completion message** | With the Neogi ruler eliminated, Kiess scouts report fragmented slave-taking bands rather than organized raids. Missing-person alerts from the southern camps have already dropped, and warren pressure on channel crossings is easing. |

#### Proposition 29 — Ashen Lattice Containment Sweep (Midgaard)

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
| **offerer_vnum** | 3015 |
| **reward_gold** | 3900 |
| **reward_qp** | 4 |
| **reward_item_vnum** | 0 |
| **reward_item_count** | 0 |
| **Target vnums** | `9620 9621 9623` (spore husk, mist-wolf, prism-leech swarm) |
| **Accept message** | Midgaard's herbal and timber consignments are being rejected as contaminated after passing near the Ashen Lattice scars. Clear representative blight carriers in the scar band so inspectors can certify outbound cargo again. |
| **Completion message** | Quarantine clerks confirm lower active spore density along inspected routes through the scar band. Midgaard's receiving houses have resumed normal intake for stamped Kiess manifests instead of automatic rejection. |

### Chain Propositions

#### Kiess Wardline Restoration Chain (30-32)

##### Proposition 30 — Wardline Reconsolidation

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
| **offerer_vnum** | 13001 |
| **reward_gold** | 4300 |
| **reward_qp** | 4 |
| **reward_item_vnum** | 0 |
| **reward_item_count** | 0 |
| **Target vnums** | `9622 9619` (bark revenant, greater broken one) |
| **Accept message** | Kiess ward engineers cannot reset the old druidic marker ring while revenants and greater broken ones keep overrunning the scar perimeter at dusk. Break both threat groups so a repair team can hold position long enough to re-ink the ward lattice. |
| **Completion message** | Engineers held the perimeter through a full mist cycle and re-established the first wardline interval. The forest still shifts, but the repaired markers now give patrol captains a reliable baseline for route triangulation. |

##### Proposition 31 — Yugoloth Contract Severance

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
| **offerer_vnum** | 13001 |
| **reward_gold** | 5200 |
| **reward_qp** | 5 |
| **reward_item_vnum** | 0 |
| **reward_item_count** | 0 |
| **Target vnums** | `9641` (guardian yugoloth) |
| **Accept message** | Intercepted tablets indicate Ymmas hired extraplanar muscle to hold his cabin approaches while he continues experiments in the inner rooms. Eliminate the guardian yugoloth and collapse that contract line before Kiess commits a full strike team. |
| **Completion message** | The yugoloth's presence has ended, and the cabin perimeter no longer shows coordinated outsider tactics. Kiess command has authorized the final assault window on Ymmas's sanctum while his defenses are still destabilized. |

##### Proposition 32 — Ymmas Lair Strike

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
| **offerer_vnum** | 13001 |
| **reward_gold** | 7000 |
| **reward_qp** | 6 |
| **reward_item_vnum** | 0 |
| **reward_item_count** | 0 |
| **Target vnums** | `9642` (the evil Ymmas) |
| **Accept message** | With the outer contract broken, Kiess issues a direct kill order on Ymmas, whose Conclave salvage rites are amplifying confusion-mist surges across active patrol bands. Enter the sanctum and end his operation before another bell-line collapses. |
| **Completion message** | Ymmas is dead, and mist-pressure readings around the central cabin complex have dropped below emergency thresholds. Kiess dispatch has reopened suspended patrol loops and logged the strike as a major stabilization event for the entire southern frontier. |

#### Kowloon Deep-Courier Security Chain (33-34)

##### Proposition 33 — Cave Route Pacification

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
| **offerer_vnum** | 14021 |
| **reward_gold** | 4200 |
| **reward_qp** | 4 |
| **reward_item_vnum** | 0 |
| **reward_item_count** | 0 |
| **Target vnums** | `9636 9640` (manscorpian scout, manscorpian hybrid) |
| **Accept message** | Kowloon courier lantern crews are testing a cave-linked relay route to bypass surface fog banks, but manscorpian patrols are collapsing marker lines before runners can map safe intervals. Remove both common patrol castes so survey crews can complete their relay chart. |
| **Completion message** | The cave relay team completed a full marker pass with only routine resistance. Kowloon has approved provisional use of the subterranean courier segment for high-priority winter dispatches. |

##### Proposition 34 — Chitin Crown Decapitation

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
| **offerer_vnum** | 14021 |
| **reward_gold** | 6100 |
| **reward_qp** | 5 |
| **reward_item_vnum** | 0 |
| **reward_item_count** | 0 |
| **Target vnums** | `9638` (manscorpian king) |
| **Accept message** | Initial pacification proved the cave route viable, but the manscorpian king still commands concentrated strikes from the deepest chamber and can rebuild losses faster than patrols can clear them. Eliminate the king to collapse centralized resistance and secure the relay corridor for Kowloon's long-haul post. |
| **Completion message** | With the manscorpian king removed, coordinated cave ambushes have stopped and remaining packs are splintering into disorganized scavenger cells. Kowloon's courier office has formally designated the route as a protected contingency lane during heavy fog seasons. |

## Implementation Checklist
1. Rebuild `#ROOMS` so vnums 9600-9799 are fully populated and connected per zone plan.
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
