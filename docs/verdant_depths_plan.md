# Area Plan: The Verdant Depths

## Overview

**Area Name:** `@@rThe @@GVerdant @@gDepths@@N`
**File Name:** `verdant_depths.are`
**Keyword:** `verdantdepths`
**Level Range:** 75-95 (I directive: `I 75 95`)
**Level Label:** `@@W(@@r75 95@@W)@@N`
**Vnum Range:** 10000-10124 (V directive: `V 10000 10124`)
**Vnum Count:** 125
**Connection:** Links from the Forest of Confusion's deep cave network (room 9770 north exit currently leads to 9764; a new exit from an appropriate Forest of Confusion room will lead into The Verdant Depths entry room at vnum 10000)
**Reset Rate:** 15
**Reset Message:** `@@gthe ancient trees creak and groan as the forest shifts around you@@N`

### Color Theme

- **Primary:** `@@G` (green) -- canopy, foliage, living forest
- **Secondary:** `@@g` (gray) -- mist, ancient stone, decay
- **Accent:** `@@r` (light green) -- bioluminescent moss, glowing fungi
- **Danger accent:** `@@R` (red) -- blood, corruption, boss lairs
- **Text reset:** `@@N` after all color sequences

The theme evokes a primeval forest grown unnaturally deep and old, where the confusion of the parent forest has given way to something deliberate and predatory. Green dominates with gray undertones suggesting age and hidden threat.

---

## Area Header

Exact `#AREA` section content for `verdant_depths.are`:

```
#AREA
@@rThe @@GVerdant @@gDepths@@N~
Q 16
K verdantdepths~
L @@W(@@r75 95@@W)@@N~
N 74
I 75 95
V 10000 10124
F 15
U @@gthe ancient trees creak and groan as the forest shifts around you@@N~
T Teleport is allowed
```

**Directive notes:**

- `Q 16` -- mandatory version; any other value is out of spec
- `K verdantdepths~` -- single-word keyword, no spaces, no color codes
- `L @@W(@@r75 95@@W)@@N~` -- white parens, light-green level numbers, white close paren, reset; matches color conventions of peer areas
- `N 74` -- confirmed unused across all 57 loaded area files
- `I 75 95` -- controls mob-level gating and area matching logic
- `V 10000 10124` -- full 125-vnum envelope; all rooms, mobs, and objects must stay within this range
- `F 15` -- reset frequency in minutes, matching the Forest of Confusion parent area
- `U` -- reset message uses primary gray `@@g` with `@@N` reset; no double-newlines, terminated with `~`
- `T` -- rest of line is ignored by the loader; presence of the line enables teleport
- `O`, `R`, `W` -- owner and ACL directives omitted here; set at implementation time by the builder

---

## Narrative / Lore

The Verdant Depths is what lies beneath and beyond the Forest of Confusion -- an ancient, sentient woodland where the trees are older than civilization. The confusion above is merely a symptom of the forest's true nature: a living entity that lures creatures inward and transforms them. The deeper one ventures, the more the forest asserts its will. Three ancient Heartwood Guardians -- boss creatures -- protect the forest's core, each embodying a different aspect of the woodland's power: growth, decay, and predation.

---

## Room Layout (125 rooms, vnums 10000-10124)

### Zone 1: The Threshold (vnums 10000-10019, 20 rooms)

The transitional zone between the Forest of Confusion and the deeper woods. Trees grow impossibly tall, light fades, and the air thickens with spores.

| Vnum | Room Name | Sector | Flags | Notes |
|------|-----------|--------|-------|-------|
| 10000 | Entrance to the Verdant Depths | forest | 0 | Entry room; connects back to Forest of Confusion |
| 10001 | A Darkening Trail | forest | dark | Path south from entrance |
| 10002 | Beneath Towering Oaks | forest | 0 | |
| 10003 | A Moss-Covered Clearing | forest | 0 | Small clearing |
| 10004 | Among Gnarled Roots | forest | dark | Roots obstruct movement |
| 10005 | A Fungal Hollow | forest | 0 | Glowing mushrooms |
| 10006 | The Spore Thicket | forest | 0 | Dense spore clouds |
| 10007 | A Rotting Log Bridge | forest | 0 | Crosses a ravine |
| 10008 | Beneath a Canopy of Vines | forest | 0 | |
| 10009 | A Sunken Trail | forest | dark | Trail drops lower |
| 10010 | The Whispering Ferns | forest | 0 | |
| 10011 | A Thicket of Thorns | forest | 0 | |
| 10012 | Beside a Stagnant Pool | forest | 0 | |
| 10013 | A Tangle of Briars | forest | 0 | |
| 10014 | The Old Growth Border | forest | 0 | Transition to Zone 2 |
| 10015 | A Narrow Game Trail | forest | 0 | |
| 10016 | Among Fallen Trunks | forest | 0 | |
| 10017 | A Damp Hollow | forest | dark | |
| 10018 | Beneath Dripping Boughs | forest | 0 | |
| 10019 | The Edge of the Old Growth | forest | 0 | Gateway to Zone 2 |

**Connectivity:** Mostly linear with branches. Entry at 10000 (north connects back to Forest of Confusion). Paths branch east/west with the main trunk running roughly south through to 10019.

### Zone 2: The Old Growth (vnums 10020-10054, 35 rooms)

The heart of the explorable forest. Massive ancient trees, overgrown ruins, and increasingly hostile wildlife. This is the main trash mob hunting ground.

| Vnum | Room Name | Sector | Flags | Notes |
|------|-----------|--------|-------|-------|
| 10020 | The Old Growth | forest | 0 | Zone 2 hub |
| 10021 | A Cathedral of Trees | forest | 0 | Massive canopy overhead |
| 10022 | Among Ancient Trunks | forest | 0 | |
| 10023 | A Root-Woven Path | forest | 0 | |
| 10024 | The Verdant Corridor | forest | 0 | |
| 10025 | A Grove of Sentinel Oaks | forest | 0 | |
| 10026 | Beneath a Hollow Giant | forest | dark | Inside a massive dead tree |
| 10027 | A Carpet of Dead Leaves | forest | 0 | |
| 10028 | The Emerald Glade | forest | 0 | |
| 10029 | A Vine-Choked Passage | forest | 0 | |
| 10030 | Among Luminous Fungi | forest | dark | Bioluminescent |
| 10031 | A Sheltered Dell | forest | 0 | |
| 10032 | The Heartwood Trail | forest | 0 | |
| 10033 | Beside a Trickling Stream | forest | 0 | |
| 10034 | A Fern-Covered Slope | forest | 0 | |
| 10035 | The Twisted Arch | forest | 0 | Natural tree arch |
| 10036 | A Clearing of Stumps | forest | 0 | Old logging site reclaimed |
| 10037 | Among Creeping Ivy | forest | 0 | |
| 10038 | The Briar Labyrinth Edge | forest | 0 | |
| 10039 | A Gloomy Thicket | forest | dark | |
| 10040 | Beneath the Great Willow | forest | 0 | |
| 10041 | A Path of Stepping Stones | forest | 0 | Across boggy ground |
| 10042 | The Overgrown Ruins | forest | 0 | Ancient stone structures |
| 10043 | Among Crumbling Walls | forest | 0 | |
| 10044 | A Ruined Courtyard | forest | 0 | |
| 10045 | The Broken Tower Base | forest | 0 | |
| 10046 | A Root-Split Foundation | forest | 0 | |
| 10047 | Beside a Moss-Covered Well | forest | 0 | |
| 10048 | The Sunken Garden | forest | 0 | |
| 10049 | A Colonnade of Oaks | forest | 0 | Trees grew in old columns |
| 10050 | The Dense Underbrush | forest | 0 | |
| 10051 | A Lichen-Stained Path | forest | 0 | |
| 10052 | Among Toppled Stones | forest | 0 | |
| 10053 | The Ancient Boundary | forest | 0 | Transition deeper |
| 10054 | A Passage Through the Canopy Wall | forest | 0 | Gateway to Zone 3 |

**Connectivity:** Web-like with multiple paths through. Main corridors branch into sub-loops around the ruins (10042-10049). Multiple exits lead south/down into Zone 3.

### Zone 3: The Deepwood (vnums 10055-10089, 35 rooms)

The forest grows alien and hostile. Trees move when unobserved. The three boss zones branch off from this area.

| Vnum | Room Name | Sector | Flags | Notes |
|------|-----------|--------|-------|-------|
| 10055 | The Deepwood | forest | 0 | Zone 3 hub |
| 10056 | A Living Corridor | forest | 0 | Trees form walls |
| 10057 | The Breathing Grove | forest | 0 | Trees visibly move |
| 10058 | Among Watchful Boughs | forest | dark | |
| 10059 | A Floor of Writhing Roots | forest | 0 | |
| 10060 | The Amber Sap Pools | forest | 0 | Pools of ancient sap |
| 10061 | Beneath Intertwined Canopy | forest | 0 | |
| 10062 | A Narrow Root Tunnel | forest | dark | Underground passage |
| 10063 | The Petrified Ring | forest | 0 | Circle of stone trees |
| 10064 | Among Thorned Sentinels | forest | 0 | |
| 10065 | A Bioluminescent Grotto | forest | dark | Glowing cavern |
| 10066 | The Weeping Hollow | forest | 0 | Sap drips constantly |
| 10067 | A Canopy Bridge | forest | 0 | High path between trees |
| 10068 | The Rotwood Stretch | forest | 0 | Decaying zone |
| 10069 | Among Bloated Toadstools | forest | 0 | |
| 10070 | A Clearing of Black Moss | forest | 0 | |
| 10071 | The Strangler Fig Passage | forest | 0 | |
| 10072 | Beneath a Web of Roots | forest | dark | |
| 10073 | The Verdant Maw | forest | 0 | Leads to boss 1 |
| 10074 | A Path of Bone-White Bark | forest | 0 | |
| 10075 | The Fungal Cathedral | forest | 0 | |
| 10076 | Among Pulsing Vines | forest | 0 | |
| 10077 | A Grove of Dead Saplings | forest | 0 | |
| 10078 | The Decaying Threshold | forest | 0 | Leads to boss 2 |
| 10079 | Beside a River of Sap | forest | 0 | |
| 10080 | A Carnivorous Garden | forest | 0 | |
| 10081 | The Predator's Trail | forest | 0 | |
| 10082 | Among Skeletal Branches | forest | dark | |
| 10083 | The Hunter's Blind | forest | 0 | Leads to boss 3 |
| 10084 | A Hollow of Whispering Leaves | forest | 0 | |
| 10085 | The Twisted Crossroads | forest | 0 | |
| 10086 | Beneath the Reaching Canopy | forest | 0 | |
| 10087 | A Sinkhole of Mulch | forest | 0 | |
| 10088 | The Groaning Path | forest | 0 | |
| 10089 | A Wall of Living Wood | forest | 0 | Dead-end feeling |

**Connectivity:** Branching layout. Three paths lead to the three boss sub-zones (Zone 4a/4b/4c). Multiple loops for hunting trash mobs.

### Zone 4a: The Rootmother's Sanctum (vnums 10090-10101, 12 rooms)

Growth-themed boss zone. Enter from 10073.

| Vnum | Room Name | Sector | Flags | Notes |
|------|-----------|--------|-------|-------|
| 10090 | The Rootmother's Approach | forest | 0 | |
| 10091 | Among Prehensile Roots | forest | 0 | |
| 10092 | A Tunnel of Living Wood | forest | dark | |
| 10093 | The Root Nexus | forest | 0 | |
| 10094 | Beneath the Mother Tree | forest | 0 | |
| 10095 | A Chamber of Coiled Roots | forest | 0 | |
| 10096 | The Flowering Heart | forest | 0 | |
| 10097 | Among Creeping Tendrils | forest | 0 | |
| 10098 | The Nourishing Dark | forest | dark | |
| 10099 | A Bower of Ancient Growth | forest | 0 | |
| 10100 | The Rootmother's Cradle | forest | 0 | |
| 10101 | The Rootmother's Sanctum | forest | no_mob | **BOSS ROOM** - The Rootmother spawns here |

### Zone 4b: The Blightwood Hollow (vnums 10102-10112, 11 rooms)

Decay-themed boss zone. Enter from 10078.

| Vnum | Room Name | Sector | Flags | Notes |
|------|-----------|--------|-------|-------|
| 10102 | The Blightwood Approach | forest | 0 | |
| 10103 | Among Rotting Trunks | forest | 0 | |
| 10104 | A Carpet of Fungal Growth | forest | dark | |
| 10105 | The Decomposing Glade | forest | 0 | |
| 10106 | Beneath Crumbling Bark | forest | 0 | |
| 10107 | A Miasma of Spores | forest | 0 | |
| 10108 | The Festering Pool | forest | 0 | |
| 10109 | Among Collapsing Canopy | forest | 0 | |
| 10110 | The Sporecrown Ring | forest | 0 | |
| 10111 | A Hollow of Black Mold | forest | dark | |
| 10112 | The Blightwood Throne | forest | no_mob | **BOSS ROOM** - The Blightwood Ancient spawns here |

### Zone 4c: The Predator's Den (vnums 10113-10124, 12 rooms)

Predation-themed boss zone. Enter from 10083.

| Vnum | Room Name | Sector | Flags | Notes |
|------|-----------|--------|-------|-------|
| 10113 | The Predator's Approach | forest | 0 | |
| 10114 | Among Trophy Bones | forest | 0 | |
| 10115 | A Trail of Claw Marks | forest | 0 | |
| 10116 | The Stalking Grounds | forest | 0 | |
| 10117 | Beneath a Canopy of Webs | forest | dark | |
| 10118 | A Den of Shed Scales | forest | 0 | |
| 10119 | The Feeding Ground | forest | 0 | |
| 10120 | Among Gnawed Remains | forest | 0 | |
| 10121 | The Broodmother's Passage | forest | 0 | |
| 10122 | A Nest of Matted Fur | forest | dark | |
| 10123 | The Kill Floor | forest | 0 | |
| 10124 | The Apex Predator's Lair | forest | no_mob | **BOSS ROOM** - Thornfang Alpha spawns here |

---

## Mobile Definitions

### Trash Mobs (levels 80-95)

All trash mobs: `act` flags include `is_npc|stay_area|aggressive`. Strong (non-boss) mobs include `solo`.

| Vnum | Name | Level | Act Flags | Notes |
|------|------|-------|-----------|-------|
| 10000 | a verdant stalker | 80 | is_npc, stay_area, aggressive | Camouflaged predator; Zones 1-2 |
| 10001 | a thornback briar beast | 82 | is_npc, stay_area, aggressive | Walking thorn bush; Zone 1-2 |
| 10002 | a canopy lurker | 84 | is_npc, stay_area, aggressive | Drops from above; Zone 2 |
| 10003 | an ancient treant | 86 | is_npc, stay_area, aggressive | Hostile tree creature; Zone 2 |
| 10004 | a sporecap shambler | 83 | is_npc, stay_area, aggressive | Fungal humanoid; Zone 2 |
| 10005 | a deepwood wolf | 85 | is_npc, stay_area, aggressive | Oversized wolf; Zone 2-3 |
| 10006 | a vine strangler | 87 | is_npc, stay_area, aggressive | Animated vine mass; Zone 2-3 |
| 10007 | a moss-covered golem | 88 | is_npc, stay_area, aggressive | Stone golem overgrown; Zone 2-3 |
| 10008 | a blighted dryad | 85 | is_npc, stay_area, aggressive | Corrupted forest spirit; Zone 3 |
| 10009 | a thornfang stalker | 90 | is_npc, stay_area, aggressive | Pack hunter; Zone 3 |
| 10010 | a rootweaver spider | 88 | is_npc, stay_area, aggressive | Giant spider; Zone 3 |
| 10011 | a petrified sentinel | 92 | is_npc, stay_area, aggressive, solo | Animated stone tree; Zone 3 |
| 10012 | a fungal abomination | 90 | is_npc, stay_area, aggressive | Massive mushroom creature; Zone 3 |
| 10013 | a heartwood guardian | 93 | is_npc, stay_area, aggressive, solo | Elite protector; Zone 4 approaches |
| 10014 | a rotwood shambler | 86 | is_npc, stay_area, aggressive | Decaying tree-creature; Zone 4b |
| 10015 | a sporecrown drone | 84 | is_npc, stay_area, aggressive | Fungal servant; Zone 4b |
| 10016 | a thornfang packmate | 89 | is_npc, stay_area, aggressive | Wolf creature; Zone 4c |
| 10017 | a root tendril mass | 87 | is_npc, stay_area, aggressive | Animated roots; Zone 4a |
| 10018 | a canopy weaver | 91 | is_npc, stay_area, aggressive, solo | Giant web-spinning beast; Zone 4c |
| 10019 | a verdant wraith | 95 | is_npc, stay_area, aggressive, solo | Forest ghost; rare spawn in Zone 3 |
| 10020 | a creeping shadow fox | 81 | is_npc, stay_area, aggressive | Dark-furred fox stalking the threshold; Zone 1 |
| 10021 | a tanglewood serpent | 83 | is_npc, stay_area, aggressive | Snake camouflaged in undergrowth; Zone 1-2 |
| 10022 | a fungal tick swarm | 80 | is_npc, stay_area, aggressive, no_body | Swarm of parasitic fungal ticks; Zone 1 |
| 10023 | a grasping bramble | 81 | is_npc, stay_area, aggressive | Animated bramble patch; Zone 1 |
| 10024 | a hollow tree lurker | 83 | is_npc, stay_area, aggressive | Creature nesting in dead trunks; Zone 1-2 |
| 10025 | a mosswort beetle | 80 | is_npc, stay_area, aggressive | Giant armored beetle; Zone 1 |
| 10026 | a deepwood bear | 88 | is_npc, stay_area, aggressive | Massive corrupted bear; Zone 2 |
| 10027 | a sap-blooded horror | 90 | is_npc, stay_area, aggressive | Ooze-like animated tree sap; Zone 2-3 |
| 10028 | a whispering shade | 86 | is_npc, stay_area, aggressive | Shadow creature between the trees; Zone 2 |
| 10029 | a bark-skinned ogre | 89 | is_npc, stay_area, aggressive | Forest ogre with bark-like hide; Zone 2-3 |
| 10030 | a thorn-crowned stag | 84 | is_npc, stay_area, aggressive | Corrupted great stag; Zone 2 |
| 10031 | a mycelium crawler | 82 | is_npc, stay_area, aggressive | Underground fungal burrower; Zone 3 |
| 10032 | a spore-maddened boar | 85 | is_npc, stay_area, aggressive | Infected wild boar; Zone 2-3 |
| 10033 | an elder vine serpent | 91 | is_npc, stay_area, aggressive, solo | Giant snake-like vine predator; Zone 3 |
| 10034 | a living deadfall | 87 | is_npc, stay_area, aggressive | Animated pile of fallen logs; Zone 3 |
| 10035 | a deepwood shaman | 93 | is_npc, stay_area, aggressive, solo | Corrupted humanoid caster; Zone 3 |
| 10036 | a rootwoven horror | 94 | is_npc, stay_area, aggressive, solo | Massive tangled root construct; Zone 3 |
| 10037 | a blightspore walker | 86 | is_npc, stay_area, aggressive | Fungal shambling zombie; Zone 4b |
| 10038 | a thornfang whelp | 82 | is_npc, stay_area, aggressive | Young predator beast; Zone 4c |
| 10039 | a rootbound guardian | 90 | is_npc, stay_area, aggressive | Living root sentinel; Zone 4a |
| 10040 | a canopy strangler | 88 | is_npc, stay_area, aggressive | Vine creature that drops from above; Zone 2-3 |
| 10041 | a withered dryad | 91 | is_npc, stay_area, aggressive, solo | Ancient decayed forest spirit; Zone 4b |
| 10042 | a mosswort queen | 85 | is_npc, stay_area, aggressive | Larger beetle matriarch; Zone 2 |
| 10043 | a verdant hulk | 92 | is_npc, stay_area, aggressive, solo | Massive plant-beast amalgamation; Zone 3 |
| 10044 | a rotting stag | 83 | is_npc, stay_area, aggressive, undead | Undead deer wreathed in fungus; Zone 2 |
| 10045 | a thorn wasp swarm | 81 | is_npc, stay_area, aggressive, no_body | Cloud of stinging thorn wasps; Zone 1-2 |
| 10046 | a barkskin wolf | 87 | is_npc, stay_area, aggressive | Wolf with bark-hardened hide; Zone 2-3 |
| 10047 | a deepwood centipede | 84 | is_npc, stay_area, aggressive | Giant venomous centipede; Zone 2 |
| 10048 | a bog troll | 89 | is_npc, stay_area, aggressive | Swamp troll covered in moss; Zone 2-3 |
| 10049 | a fungal broodmother | 93 | is_npc, stay_area, aggressive, solo | Giant fungal matriarch; Zone 4b |
| 10050 | an ancient moss elemental | 90 | is_npc, stay_area, aggressive | Elemental of living moss and earth; Zone 3 |
| 10051 | a twisted ent | 88 | is_npc, stay_area, aggressive | Corrupted tree creature; Zone 2-3 |
| 10052 | a thornveil assassin | 94 | is_npc, stay_area, aggressive, solo | Stealthy plant-like predator; Zone 3 |
| 10053 | a gloomwing moth | 82 | is_npc, stay_area, aggressive | Giant moth with toxic wing dust; Zone 1-2 |
| 10054 | a sap ooze | 85 | is_npc, stay_area, aggressive, no_body, no_mind | Animated blob of tree sap; Zone 2 |
| 10055 | a feral grove keeper | 91 | is_npc, stay_area, aggressive, solo | Corrupted nature guardian; Zone 3 |
| 10056 | a corpseflower lurker | 86 | is_npc, stay_area, aggressive | Carnivorous plant that feeds on the dead; Zone 4b |
| 10057 | a brambleback tortoise | 88 | is_npc, stay_area, aggressive | Huge tortoise with thorn-covered shell; Zone 2-3 |
| 10058 | a sporeveil phantom | 90 | is_npc, stay_area, aggressive | Ghostly shape in the spore clouds; Zone 3 |
| 10059 | a thornroot drake | 95 | is_npc, stay_area, aggressive, solo | Dragon-like plant creature; Zone 3 rare |
| 10060 | a deepwood prowler | 84 | is_npc, stay_area, aggressive | Large feline predator; Zone 2 |

**Mob combat extensions (`!` line):**
- Lower trash (80-86): 2-3 attacks, basic skills (punch, kick, dodge)
- Mid trash (87-90): 3-4 attacks, enhanced skills (headbutt, disarm, parry, dodge)
- Upper trash (91-95) and solo: 4-5 attacks, full combat skills (trip, counter, enhanced, parry, dodge)
- Caster mobs (blighted dryad, verdant wraith, deepwood shaman, withered dryad, feral grove keeper): cast bitvectors for nature-themed spells (flamestrike, earthquake, faerie_fire)
- Defensive mobs (moss-covered golem, petrified sentinel, rootbound guardian, brambleback tortoise, ancient moss elemental): def bitvectors (fireshield, iceshield)
- Poison mobs (rootweaver spider, fungal abomination, blightspore walker, mycelium crawler, deepwood centipede, gloomwing moth, corpseflower lurker): spec_poison

**Mob elemental extensions (`|` line):**
- Plant-type mobs (treant, vine strangler, grasping bramble, living deadfall, rootwoven horror, rootbound guardian, twisted ent, verdant hulk, thornveil assassin, corpseflower lurker): strong earth, weak fire, resist poison, suscept fire
- Fungal mobs (sporecap shambler, fungal abomination, mycelium crawler, blightspore walker, sporecrown drone, fungal broodmother, fungal tick swarm): strong poison, weak fire, resist earth, suscept fire
- Predator mobs (deepwood wolf, thornfang stalker/packmate/whelp, deepwood bear, spore-maddened boar, elder vine serpent, barkskin wolf, deepwood prowler, thornroot drake, creeping shadow fox, tanglewood serpent): strong physical, resist physical
- Spirit mobs (verdant wraith, blighted dryad, whispering shade, withered dryad, sporeveil phantom): strong mental, weak physical
- Insect mobs (mosswort beetle, mosswort queen, rootweaver spider, canopy weaver, thorn wasp swarm, deepwood centipede, gloomwing moth): strong earth, resist poison
- Construct mobs (moss-covered golem, petrified sentinel): strong earth, strong physical, resist physical, resist earth
- Ooze mobs (sap ooze, sap-blooded horror): strong poison, resist physical, suscept fire, no_mind
- Troll/Ogre mobs (bog troll, bark-skinned ogre): strong physical, resist earth

### Boss Mobs (level 100)

All boss mobs: `act` flags include `is_npc|sentinel|stay_area|boss|no_flee`. Placed only in `no_mob` rooms.

| Vnum | Name | Level | Room | Theme |
|------|------|-------|------|-------|
| 10061 | the Rootmother | 100 | 10101 | Growth boss -- massive living tree entity |
| 10062 | the Blightwood Ancient | 100 | 10112 | Decay boss -- ancient rotting mega-treant |
| 10063 | Thornfang Alpha | 100 | 10124 | Predation boss -- apex forest predator beast |

**Boss combat extensions:**
- All bosses: 5-6 attacks, enhanced, counter, parry, dodge, nodisarm, notrip
- Rootmother: cast earthquake + flamestrike; def heal + fireshield; strong earth, resist physical, suscept fire
- Blightwood Ancient: cast acid_blast + chain_light + faerie_fire; def heal + iceshield; strong poison, resist earth, suscept fire
- Thornfang Alpha: pure melee focused; 6 attacks, enhanced, counter, parry, dodge, kick, headbutt, martial; strong physical, resist physical, suscept mental

**Boss `+` extension mods:**
- All bosses get moderate spellpower, crit, parry, dodge, and block modifiers appropriate for level 100 encounters

---

## Object Definitions

### Trash Gear (levels 80-85, flagged `nodrop`)

`extra_flags` for all trash gear: `ITEM_NODROP` (128). All items include `take` (8388608) in wear_flags.

| Vnum | Name | Type | Wear | Level | Weight | Archetype | Notes |
|------|------|------|------|-------|--------|-----------|-------|
| 10000 | a thornback claw | weapon | hold, take | 80 | 6 | melee | slash; from thornback briar beasts |
| 10001 | a canopy lurker's fang | weapon | hold, take | 82 | 6 | melee | pierce; from canopy lurkers |
| 10002 | a treant bark shield | armor | hold, take | 83 | 11 | tank | shield; from ancient treants |
| 10003 | a sporecap cowl | armor | head, take | 80 | 1 | caster | head slot; from sporecap shamblers |
| 10004 | a wolf-hide mantle | armor | shoulders, take | 81 | 6 | melee | shoulders; from deepwood wolves |
| 10005 | a vine-woven tunic | armor | body, take | 84 | 11 | tank | body; from vine stranglers |
| 10006 | a moss golem's fist | weapon | hold, take, fist | 85 | 6 | melee | hit (fist weapon); from moss golems |
| 10007 | a blighted branch wand | weapon | hold, take, wand | 82 | 1 | caster | wand; from blighted dryads |
| 10008 | a thornfang pelt | armor | about, take | 83 | 6 | melee | about body; from thornfang stalkers |
| 10009 | a web-silk sash | armor | waist, take | 80 | 1 | caster | waist; from rootweaver spiders |
| 10010 | a petrified bark bracer | armor | wrist, take | 85 | 11 | tank | wrist; from petrified sentinels |
| 10011 | a fungal tendril whip | weapon | hold, take | 84 | 1 | caster | whip; from fungal abominations |
| 10012 | a heartwood splint | armor | legs, take | 85 | 11 | tank | legs; from heartwood guardians |
| 10013 | a rotwood club | weapon | hold, take | 81 | 6 | melee | pound; from rotwood shamblers |
| 10014 | a sporecrown circlet | armor | head, take | 82 | 1 | caster | head; from sporecrown drones |
| 10015 | a thornfang tooth necklace | armor | neck, take | 83 | 6 | melee | neck; from thornfang packmates |
| 10016 | a root tendril armband | armor | arms, take | 80 | 11 | tank | arms; from root tendril masses |
| 10017 | a canopy weaver's silk gloves | armor | hands, take | 84 | 1 | caster | hands; from canopy weavers |
| 10018 | a verdant wraith's shroud | armor | about, take | 85 | 1 | caster | about; from verdant wraiths |
| 10019 | a deepwood wolf's ear | treasure | take | 80 | 6 | melee | trophy/quest item |
| 10020 | a shadow fox pelt cloak | armor | about, take | 81 | 6 | melee | about; from creeping shadow foxes |
| 10021 | a tanglewood fang blade | weapon | hold, take | 83 | 6 | melee | stab; from tanglewood serpents |
| 10022 | a tick-chitin armguard | armor | wrist, take | 80 | 11 | tank | wrist; from fungal tick swarms |
| 10023 | a verdant hulk's knuckle | weapon | hold, take, fist | 85 | 11 | tank | hit (fist); from verdant hulks |
| 10024 | a rotting stag antler | weapon | hold, take | 82 | 6 | melee | stab; from rotting stags |
| 10025 | a thorn wasp wing mantle | armor | shoulders, take | 81 | 1 | caster | shoulders; from thorn wasp swarms |
| 10026 | a barkskin wolf pelt | armor | about, take | 84 | 6 | melee | about; from barkskin wolves |
| 10027 | a centipede fang earring | armor | ear, take | 82 | 1 | caster | ear; from deepwood centipedes |
| 10028 | a bog troll's hide belt | armor | waist, take | 85 | 11 | tank | waist; from bog trolls |
| 10029 | a broodmother's spore sac | armor | hold, take | 85 | 1 | caster | held; from fungal broodmothers |
| 10030 | a moss elemental's core | treasure | take | 84 | 11 | tank | trophy; from ancient moss elementals |
| 10031 | a twisted ent branch | weapon | hold, take | 84 | 6 | melee | pound; from twisted ents |
| 10032 | a thornveil assassin's cowl | armor | head, take | 85 | 1 | caster | head; from thornveil assassins |
| 10033 | a gloomwing dust pouch | armor | hold, take | 80 | 1 | caster | held; from gloomwing moths |
| 10034 | a brambleback shell plate | armor | body, take | 85 | 11 | tank | body; from brambleback tortoises |
| 10035 | a bramble-thorn dagger | weapon | hold, take | 81 | 6 | melee | stab; from grasping brambles |
| 10036 | a beetle-shell pauldron | armor | shoulders, take | 80 | 11 | tank | shoulders; from mosswort beetles |
| 10037 | a deepwood bear paw | weapon | hold, take, fist | 83 | 6 | melee | hit (fist); from deepwood bears |
| 10038 | a shade-touched ring | armor | finger, take | 82 | 1 | caster | finger; from whispering shades |
| 10039 | a bark-skinned armguard | armor | wrist, take | 84 | 11 | tank | wrist; from bark-skinned ogres |
| 10040 | a crown of twisted antlers | armor | horns, take | 83 | 6 | melee | horns; from thorn-crowned stags |
| 10041 | a mycelium-threaded robe | armor | body, take | 82 | 1 | caster | body; from mycelium crawlers |
| 10042 | a boar-tusk earring | armor | ear, take | 80 | 6 | melee | ear; from spore-maddened boars |
| 10043 | an elder vine lash | weapon | hold, take | 85 | 1 | caster | whip; from elder vine serpents |
| 10044 | a deadfall bark vest | armor | body, take | 84 | 11 | tank | body; from living deadfalls |
| 10045 | a shaman's gnarled staff | weapon (two-handed) | hold, take | 85 | 1 | caster | pound; from deepwood shamans |
| 10046 | rootwoven greaves | armor | legs, take | 85 | 6 | melee | legs; from rootwoven horrors |
| 10047 | a blightspore mask | armor | face, take | 81 | 1 | caster | face; from blightspore walkers |
| 10048 | a thornfang whelp's collar | armor | neck, take | 80 | 6 | melee | neck; from thornfang whelps |
| 10049 | a rootbound gauntlet | armor | hands, take | 84 | 11 | tank | hands; from rootbound guardians |
| 10050 | a sap-crusted hauberk | armor | body, take | 85 | 11 | tank | body; from sap-blooded horrors |
| 10051 | a strangler vine belt | armor | waist, take | 82 | 6 | melee | waist; from canopy stranglers |
| 10052 | a withered dryad's circlet | armor | head, take | 84 | 1 | caster | head; from withered dryads |
| 10053 | a mosswort queen's mandible | weapon | hold, take | 83 | 6 | melee | slash; from mosswort queens |
| 10054 | a hollow lurker's hide boots | armor | feet, take | 81 | 6 | melee | feet; from hollow tree lurkers |
| 10055 | a stalker's vine garrote | weapon | hold, take | 80 | 1 | caster | crush; from verdant stalkers |
| 10056 | a beetle-shell buckler | armor (buckler) | hold, take | 83 | 11 | tank | buckler; from mosswort beetles |
| 10057 | an ogre-hewn great club | weapon (two-handed) | hold, take | 85 | 6 | melee | crush; from bark-skinned ogres |
| 10058 | a sap-hardened ring | armor | finger, take | 84 | 11 | tank | finger; from sap-blooded horrors |
| 10059 | a bear-claw amulet | armor | neck, take | 82 | 6 | melee | neck; from deepwood bears |

**Trash gear affects (`A` lines):**
- Weapons: +hit and +damage appropriate for level range
- Armor: +ac, +hp, or +mana depending on archetype (caster=mana, melee=hit/dam, tank=ac/hp)

### Boss Gear (level 90, flagged `nodrop` + `boss`)

`extra_flags` for all boss gear: `ITEM_NODROP` (128) | `ITEM_BOSS` -- note: ITEM_BOSS does not exist as a named constant in the spec; use the `boss` bit if available, otherwise this needs to be verified against `extra_flags` definitions. Per the spec, `boss` is an `act` flag on mobs (67108864). For items, the user's intent is that boss drops are distinctly flagged. **Resolution:** Use `ITEM_UNIQUE` (33554432) combined with `ITEM_NODROP` (128) for boss items unless a dedicated ITEM_BOSS extra flag is confirmed. The user specified "flagged as boss" -- if `boss` maps to a valid item extra flag, use it; otherwise, clarify with the user.

**Confirmed from source:** `ITEM_BOSS` = `BIT_28` = `134217728` (defined in `src/config.h:1175`). `ITEM_NODROP` = `128`. Boss gear `extra_flags` = `ITEM_NODROP | ITEM_BOSS` = `134217856`.

| Vnum | Name | Type | Wear | Level | Weight | Archetype | Boss Source |
|------|------|------|------|-------|--------|-----------|-------------|
| 10060 | the Rootmother's living staff | weapon (two-handed) | hold, take | 90 | 1 | caster | Rootmother |
| 10061 | a crown of living thorns | armor | head, take | 90 | 1 | caster | Rootmother |
| 10062 | the Rootmother's heartwood buckler | armor (buckler) | hold, take | 90 | 11 | tank | Rootmother |
| 10063 | greaves of ancient bark | armor | legs, take | 90 | 11 | tank | Rootmother |
| 10064 | a rootweave tunic | armor | body, take | 90 | 6 | melee | Rootmother |
| 10065 | the Rootmother's seed | armor | hold, take | 90 | 1 | caster | Rootmother; held orb-like item |
| 10066 | living bark vambraces | armor | wrist, take | 90 | 11 | tank | Rootmother |
| 10067 | a blade of blighted wood | weapon | hold, take | 90 | 6 | melee | Blightwood Ancient |
| 10068 | a sporecrown helm | armor | head, take | 90 | 11 | tank | Blightwood Ancient |
| 10069 | a cloak of rotting leaves | armor | about, take | 90 | 1 | caster | Blightwood Ancient |
| 10070 | blightwood gauntlets | armor | hands, take | 90 | 6 | melee | Blightwood Ancient |
| 10071 | the Blightwood Ancient's fungal shield | armor | hold, take | 90 | 11 | tank | Blightwood Ancient |
| 10072 | a blightwood ring of decay | armor | finger, take | 90 | 1 | caster | Blightwood Ancient |
| 10073 | a sporewoven sash | armor | waist, take | 90 | 6 | melee | Blightwood Ancient |
| 10074 | Thornfang Alpha's great claw | weapon (two-handed) | hold, take | 90 | 6 | melee | Thornfang Alpha |
| 10075 | a collar of alpha fangs | armor | neck, take | 90 | 6 | melee | Thornfang Alpha |
| 10076 | a pelt of the apex predator | armor | body, take | 90 | 11 | tank | Thornfang Alpha |
| 10077 | the predator's eye pendant | armor | neck, take | 90 | 1 | caster | Thornfang Alpha |
| 10078 | thornfang hide boots | armor | feet, take | 90 | 6 | melee | Thornfang Alpha |
| 10079 | the predator's maw wand | weapon (wand) | hold, take | 90 | 1 | caster | Thornfang Alpha; wand |
| 10080 | thornfang alpha's shoulder hide | armor | shoulders, take | 90 | 11 | tank | Thornfang Alpha |

**Boss gear affects (`A` lines):**
- Boss weapons: stronger +hit and +damage than trash equivalents
- Boss armor: stronger +ac, +hp, or +mana; some pieces with item_apply effects (e.g., enhanced, det_invis)
- Rootmother drops 7 items (vnums 10060-10066): 2 caster, 3 tank, 2 melee
- Blightwood Ancient drops 7 items (vnums 10067-10073): 2 caster, 2 tank, 3 melee
- Thornfang Alpha drops 7 items (vnums 10074-10080): 2 caster, 2 tank, 3 melee

### Delivery Method

- **Trash gear:** Equipped on mobs via `E` resets or placed in mob inventory via `G` resets. Some can also use loot tables (`l`/`L` extensions) with `ITEM_LOOT` flag added.
- **Boss gear:** Equipped on boss mobs via `E` resets and/or in boss loot tables. All boss items carry `ITEM_BOSS` extra flag per spec policy.

---

## Resets Plan

### Mob Resets (`M` commands)

**Zone 1 (10000-10019):** ~12-15 trash mob spawns
- verdant stalkers, thornback briar beasts, grasping brambles, mosswort beetles, hollow tree lurkers
- Lower-level mobs (80-84) for zone entry
- Limit of 1-2 per spawn point

**Zone 2 (10020-10054):** ~25-30 trash mob spawns
- Full range of mid-level trash: canopy lurkers, ancient treants, sporecap shamblers, deepwood wolves, vine stranglers, deepwood bears, thorn-crowned stags, bark-skinned ogres, whispering shades, mosswort queens, spore-maddened boars, canopy stranglers
- Heavier density; this is the primary farming zone
- Limits of 1-3 per spawn point

**Zone 3 (10055-10089):** ~20-25 trash mob spawns
- Higher-level trash: thornfang stalkers, rootweaver spiders, petrified sentinels, fungal abominations, sap-blooded horrors, elder vine serpents, living deadfalls, deepwood shamans, rootwoven horrors
- Solo-flagged mobs more common here
- Include verdant wraiths as rare encounters

**Zone 4a (10090-10101):** ~6-8 trash mob spawns + 1 boss
- root tendril masses, rootbound guardians, heartwood guardians as guards
- M 0 10061 1 10101 -- Rootmother boss (limit 1, in no_mob room)

**Zone 4b (10102-10112):** ~6-8 trash mob spawns + 1 boss
- rotwood shamblers, sporecrown drones, blightspore walkers, withered dryads, fungal broodmothers, corpseflower lurkers
- M 0 10062 1 10112 -- Blightwood Ancient boss (limit 1, in no_mob room)

**Zone 4c (10113-10124):** ~6-8 trash mob spawns + 1 boss
- thornfang packmates, thornfang whelps, canopy weavers
- M 0 10063 1 10124 -- Thornfang Alpha boss (limit 1, in no_mob room)

### Equipment Resets (`E`/`G` commands)

- Each trash mob type has 1-2 pieces of thematically matched equipment via `E` resets
- Each boss has 4-5 pieces of equipment via `E` resets (weapons wielded/held, armor equipped)
- Remaining boss items (2-3 per boss) via loot table (`l`/`L` mob extensions) with `ITEM_LOOT` added to those items' extra_flags

### Object Resets (`O` commands)

- No standalone room object spawns planned (all gear delivered through mobs)

---

## Connection to Forest of Confusion

Two bidirectional links connect the areas, giving players a surface entrance and an underground shortcut.

### Link 1: Surface Entrance (Primary)

**Room 9695** ("Dense Woodland") is a dead-end in the forest's dense woodland zone with only a D3 (west) exit to 9694. This is the natural "deeper into the forest" transition point.

- **confusn.are edit:** Add D1 (east) exit to room 9695, destination vnum 10000
- **verdant_depths.are:** Room 10000 (Entrance to the Verdant Depths) has D3 (west) exit back to 9695

| Direction | From | To | Description |
|-----------|------|----|-------------|
| east | 9695 (Dense Woodland) | 10000 (Entrance to the Verdant Depths) | Into the new area |
| west | 10000 (Entrance to the Verdant Depths) | 9695 (Dense Woodland) | Back to Forest of Confusion |

### Link 2: Underground Passage (Secondary)

**Room 9770** ("In a Maze of Caves") is the deepest dead-end in the cave maze with only a D0 (north) exit to 9764. This connects to the Deepwood zone underground, providing an alternate route for players who explored the caves.

- **confusn.are edit:** Add D2 (south) exit to room 9770, destination vnum 10062
- **verdant_depths.are:** Room 10062 (A Narrow Root Tunnel) has D0 (north) exit back to 9770

| Direction | From | To | Description |
|-----------|------|----|-------------|
| south | 9770 (In a Maze of Caves) | 10062 (A Narrow Root Tunnel) | Underground shortcut to Zone 3 |
| north | 10062 (A Narrow Root Tunnel) | 9770 (In a Maze of Caves) | Back to cave maze |

### Why These Rooms

- **9695** is a surface forest dead-end deep in the woodland -- thematically perfect for "the forest gets darker and deeper." Players following the main overland path naturally discover the primary entrance.
- **9770** is the deepest cave dead-end in the entire maze network -- a hidden underground passage rewards exploration of the caves with a shortcut directly into Zone 3 (The Deepwood), bypassing Zones 1-2.
- **9769** (A Small Cavern) was considered but rejected: it has zero exits and is completely unreachable in the current area. It appears to be a stranded/orphaned room.

### Required Edits to confusn.are

1. Room 9695: Add `D1` exit block (east to 10000)
2. Room 9770: Add `D2` exit block (south to 10062)

---

## Specials Plan

| Mob Vnum | Spec Function | Reason |
|----------|--------------|--------|
| 10008 | spec_cast_mage | Blighted dryad -- nature magic caster |
| 10019 | spec_cast_mage | Verdant wraith -- ghostly caster |
| 10035 | spec_cast_mage | Deepwood shaman -- corrupted caster |
| 10041 | spec_cast_mage | Withered dryad -- decay magic caster |
| 10055 | spec_cast_mage | Feral grove keeper -- corrupted guardian caster |
| 10061 | spec_cast_cleric | Rootmother boss -- healing/nature |
| 10062 | spec_cast_mage | Blightwood Ancient boss -- decay magic |
| 10010 | spec_poison | Rootweaver spider -- venomous |
| 10012 | spec_poison | Fungal abomination -- toxic |
| 10031 | spec_poison | Mycelium crawler -- toxic spores |
| 10037 | spec_poison | Blightspore walker -- fungal toxin |
| 10047 | spec_poison | Deepwood centipede -- venomous bite |
| 10053 | spec_poison | Gloomwing moth -- toxic wing dust |
| 10056 | spec_poison | Corpseflower lurker -- poisonous pollen |

---

## Implementation Checklist

1. [ ] Create `area/verdant_depths.are` with all sections in canonical order
3. [ ] Write `#AREA` header with color-themed name and directives
4. [ ] Write `#MOBILES` section (61 trash mobs + 3 bosses = 64 mobs, vnums 10000-10063)
5. [ ] Write `#OBJECTS` section (60 trash items + 21 boss items = 81 objects, vnums 10000-10080)
6. [ ] Write `#ROOMS` section (125 rooms, vnums 10000-10124, all used)
7. [ ] Write `#RESETS` section (~80-100 mob spawns with equipment)
8. [ ] Write `#SPECIALS` section
9. [ ] Add `D1` (east) exit to room 9695 in `confusn.are` pointing to 10000
10. [ ] Add `D2` (south) exit to room 9770 in `confusn.are` pointing to 10062
11. [ ] Add `verdant_depths.are` to `area/area.lst`
11. [ ] Run `cd src && make unit-tests` to validate
12. [ ] Verify all vnums within 10000-10124 range
13. [ ] Verify no vnum conflicts with existing areas
