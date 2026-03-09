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

**Mob combat extensions (`!` line):**
- Lower trash (80-86): 2-3 attacks, basic skills (punch, kick, dodge)
- Mid trash (87-90): 3-4 attacks, enhanced skills (headbutt, disarm, parry, dodge)
- Upper trash (91-95) and solo: 4-5 attacks, full combat skills (trip, counter, enhanced, parry, dodge)
- Caster mobs (blighted dryad, verdant wraith): cast bitvectors for nature-themed spells (flamestrike, earthquake, faerie_fire)
- Defensive mobs (moss-covered golem, petrified sentinel): def bitvectors (fireshield, iceshield)

**Mob elemental extensions (`|` line):**
- Plant-type mobs: strong earth, weak fire, resist poison, suscept fire
- Fungal mobs: strong poison, weak fire, resist earth, suscept fire
- Predator mobs: strong physical, resist physical
- Spirit mobs (wraith, dryad): strong mental, weak physical

### Boss Mobs (level 100)

All boss mobs: `act` flags include `is_npc|sentinel|stay_area|boss|no_flee`. Placed only in `no_mob` rooms.

| Vnum | Name | Level | Room | Theme |
|------|------|-------|------|-------|
| 10020 | the Rootmother | 100 | 10101 | Growth boss -- massive living tree entity |
| 10021 | the Blightwood Ancient | 100 | 10112 | Decay boss -- ancient rotting mega-treant |
| 10022 | Thornfang Alpha | 100 | 10124 | Predation boss -- apex forest predator beast |

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

**Trash gear affects (`A` lines):**
- Weapons: +hit and +damage appropriate for level range
- Armor: +ac, +hp, or +mana depending on archetype (caster=mana, melee=hit/dam, tank=ac/hp)

### Boss Gear (level 90, flagged `nodrop` + `boss`)

`extra_flags` for all boss gear: `ITEM_NODROP` (128) | `ITEM_BOSS` -- note: ITEM_BOSS does not exist as a named constant in the spec; use the `boss` bit if available, otherwise this needs to be verified against `extra_flags` definitions. Per the spec, `boss` is an `act` flag on mobs (67108864). For items, the user's intent is that boss drops are distinctly flagged. **Resolution:** Use `ITEM_UNIQUE` (33554432) combined with `ITEM_NODROP` (128) for boss items unless a dedicated ITEM_BOSS extra flag is confirmed. The user specified "flagged as boss" -- if `boss` maps to a valid item extra flag, use it; otherwise, clarify with the user.

**Confirmed from source:** `ITEM_BOSS` = `BIT_28` = `134217728` (defined in `src/config.h:1175`). `ITEM_NODROP` = `128`. Boss gear `extra_flags` = `ITEM_NODROP | ITEM_BOSS` = `134217856`.

| Vnum | Name | Type | Wear | Level | Weight | Archetype | Boss Source |
|------|------|------|------|-------|--------|-----------|-------------|
| 10020 | the Rootmother's living staff | weapon (two-handed) | hold, take | 90 | 1 | caster | Rootmother |
| 10021 | a crown of living thorns | armor | head, take | 90 | 1 | caster | Rootmother |
| 10022 | the Rootmother's heartwood buckler | armor (buckler) | hold, take | 90 | 11 | tank | Rootmother |
| 10023 | greaves of ancient bark | armor | legs, take | 90 | 11 | tank | Rootmother |
| 10024 | a rootweave tunic | armor | body, take | 90 | 6 | melee | Rootmother |
| 10025 | a blade of blighted wood | weapon | hold, take | 90 | 6 | melee | Blightwood Ancient |
| 10026 | a sporecrown helm | armor | head, take | 90 | 11 | tank | Blightwood Ancient |
| 10027 | a cloak of rotting leaves | armor | about, take | 90 | 1 | caster | Blightwood Ancient |
| 10028 | blightwood gauntlets | armor | hands, take | 90 | 6 | melee | Blightwood Ancient |
| 10029 | the Blightwood Ancient's fungal shield | armor | hold, take | 90 | 11 | tank | Blightwood Ancient |
| 10030 | Thornfang Alpha's great claw | weapon (two-handed) | hold, take | 90 | 6 | melee | Thornfang Alpha |
| 10031 | a collar of alpha fangs | armor | neck, take | 90 | 6 | melee | Thornfang Alpha |
| 10032 | a pelt of the apex predator | armor | body, take | 90 | 11 | tank | Thornfang Alpha |
| 10033 | the predator's eye pendant | armor | neck, take | 90 | 1 | caster | Thornfang Alpha |
| 10034 | thornfang hide boots | armor | feet, take | 90 | 6 | melee | Thornfang Alpha |

**Boss gear affects (`A` lines):**
- Boss weapons: stronger +hit and +damage than trash equivalents
- Boss armor: stronger +ac, +hp, or +mana; some pieces with item_apply effects (e.g., enhanced, det_invis)
- Each boss drops ~5 items distributed across archetypes (caster/melee/tank)

### Delivery Method

- **Trash gear:** Equipped on mobs via `E` resets or placed in mob inventory via `G` resets. Some can also use loot tables (`l`/`L` extensions) with `ITEM_LOOT` flag added.
- **Boss gear:** Equipped on boss mobs via `E` resets and/or in boss loot tables. All boss items carry `ITEM_BOSS` extra flag per spec policy.

---

## Resets Plan

### Mob Resets (`M` commands)

**Zone 1 (10000-10019):** ~8-10 trash mob spawns
- verdant stalkers, thornback briar beasts scattered through the threshold rooms
- Limit of 1-2 per spawn point

**Zone 2 (10020-10054):** ~20-25 trash mob spawns
- Full range of mid-level trash: canopy lurkers, ancient treants, sporecap shamblers, deepwood wolves, vine stranglers
- Heavier density; this is the primary farming zone
- Limits of 1-3 per spawn point

**Zone 3 (10055-10089):** ~15-20 trash mob spawns
- Higher-level trash: thornfang stalkers, rootweaver spiders, petrified sentinels, fungal abominations
- Solo-flagged mobs more common here
- Include verdant wraiths as rare encounters

**Zone 4a (10090-10101):** ~4-5 trash mob spawns + 1 boss
- root tendril masses, heartwood guardians as guards
- M 0 10020 1 10101 -- Rootmother boss (limit 1, in no_mob room)

**Zone 4b (10102-10112):** ~4-5 trash mob spawns + 1 boss
- rotwood shamblers, sporecrown drones
- M 0 10021 1 10112 -- Blightwood Ancient boss (limit 1, in no_mob room)

**Zone 4c (10113-10124):** ~4-5 trash mob spawns + 1 boss
- thornfang packmates, canopy weavers
- M 0 10022 1 10124 -- Thornfang Alpha boss (limit 1, in no_mob room)

### Equipment Resets (`E`/`G` commands)

- Each trash mob type has 1-2 pieces of equipment via `E` resets
- Each boss has 3-5 pieces of equipment via `E` resets (weapons wielded/held, armor equipped)
- Remaining boss items via loot table (`l`/`L` mob extensions)

### Object Resets (`O` commands)

- No standalone room object spawns planned (all gear delivered through mobs)

---

## Connection to Forest of Confusion

The new area connects via a one-way-in/one-way-out path:

1. **From Forest of Confusion:** Add a new exit from room **9769** (A Small Cavern -- currently a dead-end with no exits) going **south** to vnum **10000** (Entrance to the Verdant Depths)
2. **Return path:** Room **10000** has a **north** exit back to room **9769**

This places the entrance deep within the Forest of Confusion's cave network, requiring players to navigate the existing area before reaching the higher-level content. The small cavern (9769) serves as a natural transition point -- emerging from caves into the deeper forest.

**Note:** This requires a minor edit to `confusn.are` to add a `D2` (south) exit to room 9769 pointing to vnum 10000.

---

## Specials Plan

| Mob Vnum | Spec Function | Reason |
|----------|--------------|--------|
| 10008 | spec_cast_mage | Blighted dryad -- nature magic caster |
| 10019 | spec_cast_mage | Verdant wraith -- ghostly caster |
| 10020 | spec_cast_cleric | Rootmother boss -- healing/nature |
| 10021 | spec_cast_mage | Blightwood Ancient boss -- decay magic |
| 10010 | spec_poison | Rootweaver spider -- venomous |
| 10012 | spec_poison | Fungal abomination -- toxic |

---

## Implementation Checklist

1. [ ] Create `area/verdant_depths.are` with all sections in canonical order
3. [ ] Write `#AREA` header with color-themed name and directives
4. [ ] Write `#MOBILES` section (20 trash mobs + 3 bosses = 23 mobs, vnums 10000-10022)
5. [ ] Write `#OBJECTS` section (20 trash items + 15 boss items = 35 objects, vnums 10000-10034)
6. [ ] Write `#ROOMS` section (125 rooms, vnums 10000-10124, all used)
7. [ ] Write `#RESETS` section (~60-70 mob spawns with equipment)
8. [ ] Write `#SPECIALS` section
9. [ ] Add `D2` exit to room 9769 in `confusn.are` pointing to 10000
10. [ ] Add `verdant_depths.are` to `area/area.lst`
11. [ ] Run `cd src && make unit-tests` to validate
12. [ ] Verify all vnums within 10000-10124 range
13. [ ] Verify no vnum conflicts with existing areas
