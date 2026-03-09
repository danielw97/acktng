# Area Plan: The Withered Depths

## Overview

**Area Name:** `@@pThe Withered @@GDepths@@N`
**Keyword:** `withereddepths`
**Vnum Range:** 1000–1149 (150 vnums)
**Target Level:** 80–100 (bosses at 110)
**Connection:** Bidirectional link with Forest of Confusion room **9695** (Dense Woodland)
**Sector Type:** `forest` (3) for most rooms; `water_swim` (6) for bog rooms; `inside` (11) for underground fungal caves
**Color Theme:** `@@p` (purple) and `@@G` (green) — purple represents creeping corruption/blight, green represents the underlying forest. `@@b` (brown) accent for earth/wood, `@@d` (dark grey) for decay.
**Reset Rate:** 15
**Reset Message:** `@@pA @@Gfoul wind @@pstirs through the twisted canopy...@@N`
**Area Number:** TBD (next available)

---

## Lore

Deep within the Forest of Confusion, past the disorienting mists, lies a region the old druids called The Withered Depths. Here the corruption that birthed the confusion mists has its source — an ancient blight that seeps from a primordial wound in the earth. The trees grow impossibly tall and twisted, their bark blackened with fungal rot. The mist is thicker here, carrying the stench of decay, and the creatures within have been warped by centuries of exposure into something far more dangerous than the beasts of the outer forest.

---

## Zone Layout (8 Zones, 150 Rooms)

### Zone 1: The Rotting Threshold (Rooms 1000–1014, 15 rooms)
Entry zone connecting to the Forest of Confusion. The mist thickens and the trees grow darker. This is the transition from the lvl 10-60 Forest of Confusion into the high-level Withered Depths.

- **Room 1000:** The Darkening Path — entry room, exit west to Forest of Confusion room 9695 (Dense Woodland), exit east deeper in.
- **Rooms 1001–1008:** Winding trail through increasingly twisted forest. Linear path with some branches.
- **Rooms 1009–1012:** A clearing where corrupted animals prowl. Wider area with multiple connections.
- **Room 1013:** A collapsed stone archway — named exit `^crumbling arch` (door, closed, locked) leading down to Zone 2 (Fungal Hollows). Key required (key object at vnum 1000).
- **Room 1014:** A watchpost ruin overlooking the canopy, exit up to Zone 6 (Ancient Canopy).

**Connectivity:**
- West from 1000 → Forest of Confusion room 9695 (Dense Woodland)
- East/south progression through 1001–1012
- Down from 1013 → Zone 2 (room 1015) via locked door
- Up from 1014 → Zone 6 (room 1080)

### Zone 2: The Fungal Hollows (Rooms 1015–1034, 20 rooms)
Underground caverns beneath massive root systems, filled with bioluminescent fungi. Sector type: `inside` (11).

- **Rooms 1015–1024:** Twisting root-tunnels, some narrow, some opening into larger chambers. Various fungal descriptions.
- **Rooms 1025–1029:** The Spore Caverns — wider rooms with poisonous spore clouds. Some rooms flagged `dark`.
- **Room 1030:** The Mycelium Throne — **BOSS ROOM** (flagged `no_mob`). The Fungal Colossus spawns here.
- **Rooms 1031–1034:** Exit tunnels leading back up to Zone 4 (Blightwood) and connecting to Zone 3 (Briar Maze).

**Connectivity:**
- Down from Zone 1 room 1013 → room 1015
- Room 1034 east → Zone 3 (room 1035)
- Room 1031 up → Zone 4 (room 1055)

### Zone 3: The Briar Maze (Rooms 1035–1054, 20 rooms)
A tangled labyrinth of thorny vines and twisted brambles. ALL rooms in this zone flagged `ROOM_MAZE` (524288). Exits loop and misdirect. Sector type: `forest` (3).

- **Rooms 1035–1049:** Maze rooms with disorienting exits. Rooms connect in non-linear patterns (allowed because all are ROOM_MAZE). Descriptions emphasize thorns, tangled vines, and being lost.
- **Room 1050:** The Thorn Nexus — **BOSS ROOM** (flagged `no_mob | maze`). The Briar Queen spawns here. Central maze room.
- **Rooms 1051–1054:** Maze exit paths leading to Zone 4 (Blightwood) and Zone 5 (Miasma Bog).

**Connectivity:**
- West from room 1035 → Zone 2 (room 1034)
- Maze-internal looping exits throughout 1035–1054
- Room 1054 south → Zone 5 (room 1075)
- Room 1051 north → Zone 4 (room 1055)

### Zone 4: The Blightwood (Rooms 1055–1079, 25 rooms)
The corrupted heart of the forest at ground level. Massive blackened trees with oozing bark. The canopy blocks nearly all light. Sector type: `forest` (3). Many rooms flagged `dark`.

- **Rooms 1055–1064:** Dense corrupted forest with multiple branching paths. Room descriptions feature rotting bark, blackened leaves, strange sounds.
- **Rooms 1065–1074:** Deeper blightwood, where the trees seem almost alive and hostile. Wider area with loops (non-maze, so no directional loops — paths branch and reconverge but never loop in same direction).
- **Room 1075:** A named exit `^gnarled root gate` (door, closed) leading south to Zone 5. No lock, just a closeable barrier.
- **Rooms 1076–1079:** Transition area connecting to Zone 7 (Heartrot Grove) via east exits.

**Connectivity:**
- Up from Zone 2 room 1031 → room 1055
- North from Zone 3 room 1051 → room 1055
- Room 1075 south (door) → Zone 5 (room 1095)
- Room 1079 east → Zone 7 (room 1110)
- Room 1064 up → Zone 6 (room 1080)

### Zone 5: The Miasma Bog (Rooms 1080–1104, 25 rooms)
**Note: Renumbered — rooms 1080–1094 are Ancient Canopy, 1095–1114 are Miasma Bog. See corrected layout below.**

*Corrected zone assignments for sequential vnum usage:*

### Zone 5 (Corrected): The Ancient Canopy (Rooms 1080–1094, 15 rooms)
Elevated treetop paths high above the forest floor. Rickety bridges of woven vines and massive branches. Sector type: `forest` (3). Some rooms flagged `dark`.

- **Rooms 1080–1088:** Vine-bridge walkways and platform rooms in the canopy. Precarious paths with beautiful but eerie views of the blighted forest below.
- **Room 1089:** The Warden's Perch — **BOSS ROOM** (flagged `no_mob`). The Canopy Warden spawns here.
- **Rooms 1090–1094:** Descending paths from the canopy, connecting back to Zone 4 and forward to Zone 7.

**Connectivity:**
- Up from Zone 1 room 1014 → room 1080
- Up from Zone 4 room 1064 → room 1082
- Room 1094 down → Zone 7 (room 1110)

### Zone 6 (Corrected): The Miasma Bog (Rooms 1095–1119, 25 rooms)
A fetid swamp where the corruption has liquefied the ground. Poisonous gases hang low. Sector type: `water_swim` (6) for most rooms. Some rooms flagged `dark`.

- **Rooms 1095–1104:** Shallow bog rooms with treacherous footing. Descriptions feature bubbling muck, rotting vegetation, will-o-wisps.
- **Rooms 1105–1113:** Deeper swamp, partially submerged ruins of an old druid circle. Some rooms have extra descriptions for examining ruins.
- **Room 1114:** The Drowning Hollow — **BOSS ROOM** (flagged `no_mob`). The Bog Hydra spawns here.
- **Rooms 1115–1119:** Bog edges transitioning to Zone 7 (Heartrot Grove).

**Connectivity:**
- South from Zone 3 room 1054 → room 1095
- South from Zone 4 room 1075 (via door) → room 1095
- Room 1119 east → Zone 7 (room 1120)

### Zone 7: The Heartrot Grove (Rooms 1120–1139, 20 rooms)
The deepest corruption. Trees here are barely alive, pulsing with a sickly purple glow. The air is thick and oppressive. Sector type: `forest` (3). All rooms `dark`.

- **Rooms 1120–1129:** The outer grove, where the corruption is palpable. Twisted amalgamations of tree and fungus.
- **Rooms 1130–1134:** The inner grove, approaching the source. Ground becomes spongy and organic.
- **Room 1135:** The Blightmother's Sanctum — **BOSS ROOM** (flagged `no_mob | no_recall | no_teleport`). The Blightmother spawns here. Named exit `^pulsing membrane` (door, closed, locked) from room 1134. Key required (key object at vnum 1001).
- **Rooms 1136–1139:** Post-boss rooms / escape route. A hidden path that leads to Zone 8.

**Connectivity:**
- East from Zone 4 room 1079 → room 1120
- Down from Zone 5 room 1094 → room 1110 (overlap with zone, adjust: room 1094 down → room 1120)
- Room 1119 east → room 1120
- Room 1134 north (locked door) → room 1135
- Room 1139 south → Zone 8 (room 1140)

### Zone 8: The Verdant Sanctum (Rooms 1140–1149, 10 rooms)
A hidden, pristine pocket of the forest untouched by corruption — yet guarded fiercely by an ancient protector who trusts no one. Sector type: `forest` (3). Rooms flagged `regen` (2).

- **Rooms 1140–1144:** A serene glade with clear water, healthy trees, and soft moss. Stark contrast to the rest of the area.
- **Room 1145:** The Heart of the Forest — **BOSS ROOM** (flagged `no_mob | no_recall | no_teleport`). The Verdant Guardian spawns here (optional 6th boss, the forest's last protector, driven mad by the surrounding corruption).
- **Rooms 1146–1149:** The sanctum's deeper reaches. Room 1149 has an exit back to Zone 1 (room 1000 area) providing a loop back, but NOT a directional loop (different direction than entry).

**Connectivity:**
- South from Zone 7 room 1139 → room 1140
- Room 1149 west → Zone 1 room 1000 (shortcut back to entrance)

---

## Doors Summary

| From Room | Direction | To Room | Door Keyword | Lock? | Key Vnum | Reset State |
|-----------|-----------|---------|--------------|-------|----------|-------------|
| 1013 | down | 1015 | `^crumbling arch` | Yes (locked) | 1000 | `D 1013 5 2` (locked) |
| 1015 | up | 1013 | `^crumbling arch` | Yes (locked) | 1000 | `D 1015 4 2` (locked) |
| 1075 | south | 1095 | `^gnarled root gate` | No | -1 | `D 1075 2 1` (closed) |
| 1095 | north | 1075 | `^gnarled root gate` | No | -1 | `D 1095 0 1` (closed) |
| 1134 | north | 1135 | `^pulsing membrane` | Yes (locked) | 1001 | `D 1134 0 2` (locked) |
| 1135 | south | 1134 | `^pulsing membrane` | Yes (locked) | 1001 | `D 1135 2 2` (locked) |

All doors are bidirectional (both sides defined). Locked doors have corresponding key objects in `#OBJECTS`.

---

## Mobiles

### Bosses (Level 110, flagged `boss | sentinel | stay_area`)

All bosses spawn in `no_mob` rooms only.

| Vnum | Name | short_descr | Level | Zone | Room | Notes |
|------|------|-------------|-------|------|------|-------|
| 1000 | The Fungal Colossus | `@@pthe Fungal Colossus@@N` | 110 | 2 | 1030 | Massive fungal golem. `act`: is_npc\|sentinel\|no_flee\|aggressive\|stay_area\|boss. Strong: earth, poison. Weak: fire. Cast: acid_blast, suffocate. Def: heal, iceshield. Spec: `spec_cast_mage`. Loot table: boss items. |
| 1001 | The Briar Queen | `@@pthe Briar Queen@@N` | 110 | 3 | 1050 | Thorny fey queen. `act`: is_npc\|sentinel\|no_flee\|aggressive\|stay_area\|boss. Strong: earth, physical. Weak: fire. Cast: chain_light, flamestrike. Def: heal, fireshield. Spec: `spec_cast_cleric`. Loot table: boss items. |
| 1002 | The Bog Hydra | `@@pthe Bog Hydra@@N` | 110 | 6 | 1114 | Multi-headed swamp beast. `act`: is_npc\|sentinel\|no_flee\|aggressive\|stay_area\|no_mind\|boss. Strong: water, poison. Weak: fire, holy. Cast: acid_blast, earthquake. Def: heal. Spec: `spec_breath_acid`. Skills: 4_attack, enhanced, nodisarm, notrip. |
| 1003 | The Canopy Warden | `@@pthe Canopy Warden@@N` | 110 | 5 | 1089 | Ancient corrupted treant. `act`: is_npc\|sentinel\|no_flee\|aggressive\|stay_area\|boss. Strong: earth, physical. Weak: fire, shadow. Cast: earthquake, flamestrike. Def: heal, shockshield. Spec: `spec_cast_cleric`. |
| 1004 | The Blightmother | `@@pthe Blightmother@@N` | 110 | 7 | 1135 | Source of the corruption, final boss. `act`: is_npc\|sentinel\|no_flee\|aggressive\|stay_area\|boss. Strong: poison, shadow, earth. Weak: holy, fire. Cast: suffocate, mindflame, nerve_fire. Def: heal, fireshield, iceshield. Spec: `spec_cast_bigtime`. Skills: 5_attack, enhanced, dodge, parry, counter. |
| 1005 | The Verdant Guardian | `@@Gthe Verdant Guardian@@N` | 110 | 8 | 1145 | Maddened forest protector, optional boss. `act`: is_npc\|sentinel\|no_flee\|aggressive\|stay_area\|boss. Strong: holy, earth. Weak: shadow, poison. Cast: flamestrike, light_bolt. Def: heal, fireshield, shockshield. Spec: `spec_cast_cleric`. |

### Strong Mobs (flagged `solo | stay_area`, levels 97–100)

| Vnum | Name | Level | Zone | Notes |
|------|------|-------|------|-------|
| 1006 | A rotwood sentinel | 100 | 4 | Corrupted tree guardian. aggressive, solo, stay_area. Skills: 3_attack, enhanced, parry. |
| 1007 | A miasma wraith | 97 | 6 | Gaseous undead. aggressive, solo, stay_area, undead, no_body. Cast: mind_flail, suffocate. |
| 1008 | A thornweaver matriarch | 98 | 3 | Giant spider queen. aggressive, solo, stay_area. Skills: 3_attack, dodge, counter. |
| 1030 | A blightwood revenant | 100 | 7 | Undead treant horror. aggressive, solo, stay_area, undead. Skills: 4_attack, enhanced, parry, nodisarm. Cast: earthquake. Def: heal. |
| 1031 | A fungal titan | 99 | 2 | Towering mushroom construct. aggressive, solo, stay_area, no_mind. Skills: 3_attack, enhanced, punch, notrip. |
| 1032 | A corrupted dryad elder | 98 | 4 | Ancient fey twisted by blight. aggressive, solo, stay_area. Cast: chain_light, flamestrike, faerie_fire. Def: heal, fireshield. Spec: `spec_cast_cleric`. |
| 1033 | A bog behemoth | 99 | 6 | Massive swamp reptile. aggressive, solo, stay_area. Skills: 4_attack, enhanced, headbutt, nodisarm. |
| 1034 | A canopy horror | 97 | 5 | Winged predator nesting in the treetops. aggressive, solo, stay_area. Skills: 3_attack, enhanced, dodge, kick. |
| 1035 | A heartrot abomination | 100 | 7 | Pulsating mass of corrupted flesh and bark. aggressive, solo, stay_area, no_mind. Skills: 4_attack, enhanced, counter. Def: heal. |
| 1036 | A briar lord | 98 | 3 | Massive thorned guardian of the maze. aggressive, solo, stay_area. Skills: 3_attack, enhanced, parry, disarm. |

### Trash Mobs (flagged `stay_area | aggressive`, levels 85–96)

| Vnum | Name | Level | Zone(s) | Notes |
|------|------|-------|---------|-------|
| 1009 | A blighted wolf | 85 | 1, 4 | Corrupted predator. Skills: 2_attack, enhanced. |
| 1010 | A twisted dryad | 90 | 1, 4 | Corrupted fey. Cast: faerie_fire, flare. Def: cure_serious. |
| 1011 | A fungal crawler | 88 | 2 | Insectoid fungal creature. Skills: 2_attack, punch. |
| 1012 | A venomous viper | 92 | 1, 5, 6 | Giant poisonous snake. Spec: `spec_poison`. |
| 1013 | A corrupted treant | 95 | 4 | Blighted tree creature. Skills: 3_attack, enhanced. |
| 1014 | A bog lurker | 90 | 6 | Swamp ambush predator. Skills: 2_attack, trip. |
| 1015 | A thornweaver spider | 93 | 3 | Giant thorny spider. Skills: 2_attack, dodge. |
| 1016 | A swamp ghoul | 91 | 6 | Undead, undead flag. Skills: 2_attack, punch. |
| 1017 | A blighted bear | 88 | 1, 4 | Corrupted bear. Skills: 2_attack, enhanced. |
| 1018 | A canopy stalker | 95 | 5 | Predatory bird-lizard. Skills: 3_attack, dodge. |
| 1019 | A spore shambler | 86 | 2 | Shambling fungal mass. no_mind. Skills: 2_attack. |
| 1020 | A briar hound | 89 | 3, 4 | Thorn-covered beast. Skills: 2_attack, enhanced. |
| 1021 | A moss horror | 94 | 7 | Creeping plant horror. Skills: 3_attack, enhanced. |
| 1022 | A withered druid | 96 | 7 | Undead druid. undead. Cast: flamestrike, earthquake. Def: heal. |
| 1023 | A vine strangler | 87 | 1, 4 | Animated vine mass. no_mind. Skills: 2_attack, trip. |
| 1024 | A bog wisp | 85 | 6 | Will-o-wisp. no_body. Cast: shock_grasp, flare. |
| 1025 | A gloomwing moth | 90 | 5 | Giant corrupted moth. Skills: 2_attack, dodge. |
| 1026 | A heartrot spawn | 96 | 7, 8 | Blight creature. Skills: 3_attack, enhanced, punch. |
| 1027 | A corrupted stag | 92 | 4 | Warped forest animal. Skills: 2_attack, charge, enhanced. |
| 1028 | A fungal spitter | 89 | 2 | Ranged fungal mob. Cast: acid_blast. |
| 1037 | A rotting wolf alpha | 90 | 1, 4 | Pack leader of the blighted wolves. Skills: 3_attack, enhanced, dodge. |
| 1038 | A mire crocodile | 93 | 6 | Massive swamp predator. Skills: 2_attack, enhanced, trip. |
| 1039 | A blighted owl | 86 | 5 | Corrupted raptor with glowing eyes. Skills: 2_attack, dodge. |
| 1040 | A thorn elemental | 94 | 3 | Animated mass of thorny vines. no_mind. Skills: 3_attack, enhanced. |
| 1041 | A spore cloud | 85 | 2 | Floating toxic fungal mass. no_body, no_mind. Cast: suffocate. Spec: `spec_poison`. |
| 1042 | A withered ent | 95 | 4, 7 | Dying tree creature lashing out. Skills: 3_attack, enhanced, punch. |
| 1043 | A bog rat swarm | 86 | 6 | Swarm of corrupted rats. Skills: 2_attack, punch. |
| 1044 | A bramble golem | 93 | 3 | Construct of tangled thorny branches. no_mind. Skills: 2_attack, enhanced, parry. |
| 1045 | A shadow fox | 88 | 1, 4 | Dark furred predator that hunts by stealth. Skills: 2_attack, dodge, trip. |
| 1046 | A canopy viper | 91 | 5 | Arboreal serpent with virulent toxin. Spec: `spec_poison`. Skills: 2_attack. |
| 1047 | A corpse flower | 87 | 6, 7 | Carnivorous plant animated by corruption. no_mind. Skills: 2_attack, trip. |
| 1048 | A mycelium creeper | 90 | 2 | Network of fungal tendrils that ambushes prey. no_mind. Skills: 2_attack, enhanced, trip. |
| 1049 | A blighted hawk | 89 | 5 | Corrupted bird of prey circling the canopy. Skills: 2_attack, enhanced, dodge. |
| 1050 | A fungal zombie | 88 | 2 | Reanimated corpse covered in parasitic mushrooms. undead. Skills: 2_attack, punch. |
| 1051 | A thorn dart frog | 85 | 3, 6 | Tiny but deadly poisonous amphibian. Spec: `spec_poison`. |
| 1052 | A withered wolf spider | 91 | 3 | Large wolf spider with rotting exoskeleton. Skills: 2_attack, dodge. |
| 1053 | A peat wraith | 93 | 6 | Ghostly bog spirit. undead, no_body. Cast: mind_flail, ego_whip. |
| 1054 | A blight beetle | 86 | 2, 4 | Oversized beetle infested with fungal growth. Skills: 2_attack, enhanced. |
| 1055 | A corrupted boar | 90 | 1, 4 | Massive warped boar with fungal tusks. Skills: 2_attack, enhanced, charge. |
| 1056 | A verdant wisp | 87 | 8 | Faded forest spirit clinging to old magic. no_body. Cast: flare, light_bolt. Def: cure_serious. Non-aggressive. |
| 1057 | A canopy weaver | 92 | 5 | Spider that builds webs between the treetops. Skills: 2_attack, dodge, trip. |
| 1058 | A rotwood beetle swarm | 88 | 4, 7 | Cloud of wood-boring insects. Skills: 2_attack, enhanced. |
| 1059 | A mire leech | 85 | 6 | Giant parasitic swamp leech. Skills: 2_attack. |
| 1060 | A fungal bat colony | 87 | 2 | Cluster of bats coated in luminous spores. Skills: 2_attack, dodge. |
| 1061 | A blighted centipede | 89 | 2, 4 | Enormous segmented crawler oozing poison. Spec: `spec_poison`. Skills: 2_attack. |
| 1062 | A thornback tortoise | 94 | 3 | Slow but heavily armored reptile covered in thorns. Skills: 2_attack, parry, nodisarm. |
| 1063 | A grove phantom | 96 | 7 | Spectral remnant of a druid who fell to the blight. undead, no_body. Cast: mindflame, mind_bolt. Def: cure_critic. |
| 1064 | A swamp adder | 88 | 6 | Thick-bodied venomous water snake. Spec: `spec_poison`. Skills: 2_attack. |
| 1065 | A blighted raven | 85 | 1, 5 | Corrupted scavenger bird with eerie intelligence. Skills: 2_attack, dodge. |
| 1066 | A heartrot tendril | 91 | 7 | Animated root-tentacle of the Blightmother. no_mind. Skills: 2_attack, trip. |
| 1067 | A corrupted satyr | 93 | 4 | Warped fey creature driven mad by blight. Skills: 2_attack, enhanced, kick. Cast: faerie_fire. |
| 1068 | A mold hulk | 95 | 2 | Shambling heap of rotting vegetation held together by mold. no_mind. Skills: 3_attack, enhanced. |

### Non-Combat / Utility Mobs

| Vnum | Name | Level | Notes |
|------|------|-------|-------|
| 1029 | A trapped explorer | 50 | Non-aggressive, sentinel, stay_area. Found in Zone 1. Flavor NPC. |
| 1069 | A dying forest sprite | 40 | Non-aggressive, sentinel, stay_area. Found in Zone 8. Flavor NPC, whispers lore about the Blightmother. |
| 1070 | A lost herbalist | 55 | Non-aggressive, sentinel, stay_area. Found in Zone 4. Flavor NPC seeking rare fungi. |
| 1071 | A feral cat | 30 | Non-aggressive, stay_area. Wanders Zone 1. Ambient wildlife. |

---

## Objects

### Keys

| Vnum | Name | short_descr | item_type | extra_flags | wear_flags | Level | Notes |
|------|------|-------------|-----------|-------------|------------|-------|-------|
| 1000 | a corroded iron key | `@@da corroded iron key@@N` | key (18) | ITEM_NODROP (128) | take (8388608) | L 90 | Opens crumbling arch (rooms 1013/1015). |
| 1001 | a pulsing blight key | `@@pa pulsing blight key@@N` | key (18) | ITEM_NODROP (128) | take (8388608) | L 100 | Opens pulsing membrane (rooms 1134/1135). Dropped by a withered druid (vnum 1022) as loot. |

### Boss Equipment (Level 100, flagged `ITEM_NODROP | ITEM_BOSS | ITEM_MAGIC`)

extra_flags base = ITEM_NODROP(128) + ITEM_BOSS(134217728) + ITEM_MAGIC(64) = 134217920

Items delivered via boss loot tables (`l`/`L` extensions). All boss loot items additionally get `ITEM_LOOT` (67108864), making total extra_flags = 201326784.

| Vnum | Name | short_descr | item_type | extra_flags | wear_flags | weight | Level | Source Boss |
|------|------|-------------|-----------|-------------|------------|--------|-------|-------------|
| 1002 | a spore-encrusted helm | `@@pa spore-encrusted helm@@N` | armor (9) | 201326784 | head\|take | 11 | L 100 | Fungal Colossus (loot) |
| 1003 | the Colossus Core | `@@pthe Colossus Core@@N` | armor (9) | 201326784 | hold\|take | 11 | L 100 | Fungal Colossus (loot). Shield. |
| 1004 | a crown of living thorns | `@@pa crown of living thorns@@N` | armor (9) | 201326784 | head\|take | 1 | L 100 | Briar Queen (loot) |
| 1005 | the Briar Queen's scepter | `@@pthe Briar Queen's scepter@@N` | weapon (5) | 201326784 | hold\|take | 1 | L 100 | Briar Queen (loot). value3=7 (pound). |
| 1006 | a hydra-scale hauberk | `@@pa hydra-scale hauberk@@N` | armor (9) | 201326784 | body\|take | 11 | L 100 | Bog Hydra (loot) |
| 1007 | a fang of the hydra | `@@pa fang of the hydra@@N` | weapon (5) | 201326784 | hold\|take | 6 | L 100 | Bog Hydra (loot). value3=2 (stab). |
| 1008 | gauntlets of living bark | `@@Ggauntlets of living bark@@N` | armor (9) | 201326784 | hands\|take | 6 | L 100 | Canopy Warden (loot) |
| 1009 | the Warden's gnarled staff | `@@Gthe Warden's gnarled staff@@N` | weapon (5) | 201326784 + ITEM_TWO_HANDED(2147483648) = 2348810432 | hold\|take | 6 | L 100 | Canopy Warden (loot). Two-handed. value3=7 (pound). |
| 1010 | a carapace of the Blightmother | `@@pa carapace of the Blightmother@@N` | armor (9) | 201326784 | body\|take | 11 | L 100 | Blightmother (loot) |
| 1011 | the Blightmother's claw | `@@pthe Blightmother's claw@@N` | weapon (5) | 201326784 + ITEM_FIST(1073741824) = 1275068608 | hold\|take | 6 | L 100 | Blightmother (loot). Fist weapon. value3=0 (hit). |
| 1012 | a mantle of verdant wrath | `@@Ga mantle of verdant wrath@@N` | armor (9) | 201326784 | shoulders\|take | 6 | L 100 | Verdant Guardian (loot) |
| 1013 | the Guardian's living blade | `@@Gthe Guardian's living blade@@N` | weapon (5) | 201326784 | hold\|take | 1 | L 100 | Verdant Guardian (loot). value3=3 (slash). |
| 1014 | a ring of fungal resilience | `@@pa ring of fungal resilience@@N` | armor (9) | 201326784 | finger\|take | 1 | L 100 | Fungal Colossus (loot) |
| 1015 | the Briar Queen's thorned cloak | `@@pthe Briar Queen's thorned cloak@@N` | armor (9) | 201326784 | about\|take | 1 | L 100 | Briar Queen (loot) |
| 1016 | a hydra-fang necklace | `@@pa hydra-fang necklace@@N` | armor (9) | 201326784 | neck\|take | 6 | L 100 | Bog Hydra (loot) |
| 1017 | the Blightmother's eye | `@@pthe Blightmother's eye@@N` | armor (9) | 201326784 | halo\|take | 1 | L 100 | Blightmother (loot) |

### Trash Equipment (Levels 90–100, flagged `ITEM_NODROP`)

Items spawned via resets (`E`/`G` commands) do NOT get ITEM_LOOT. Items spawned via loot tables DO get ITEM_LOOT.

extra_flags for reset gear: ITEM_NODROP(128) = 128
extra_flags for loot-table gear: ITEM_NODROP(128) + ITEM_LOOT(67108864) = 67108992

| Vnum | Name | short_descr | item_type | extra_flags | wear_flags | weight | Level | Delivery |
|------|------|-------------|-----------|-------------|------------|--------|-------|----------|
| 1018 | a rotwood bark shield | `@@pa rotwood bark shield@@N` | armor (9) | 128 | hold\|take | 11 | L 95 | Reset (E) on rotwood sentinel |
| 1019 | a blighted wolf pelt | `@@pa blighted wolf pelt@@N` | armor (9) | 67108992 | about\|take | 6 | L 90 | Loot from blighted wolf |
| 1020 | a twisted dryad's circlet | `@@pa twisted dryad's circlet@@N` | armor (9) | 67108992 | head\|take | 1 | L 92 | Loot from twisted dryad |
| 1021 | a fungal crawler carapace | `@@pa fungal crawler carapace@@N` | armor (9) | 67108992 | body\|take | 11 | L 90 | Loot from fungal crawler |
| 1022 | a viper-fang dagger | `@@pa viper-fang dagger@@N` | weapon (5) | 67108992 | hold\|take | 6 | L 93 | Loot from venomous viper. value3=2 (stab). |
| 1023 | a treant-bark bracer | `@@pa treant-bark bracer@@N` | armor (9) | 67108992 | wrist\|take | 11 | L 95 | Loot from corrupted treant |
| 1024 | a bog lurker's hide boots | `@@pa bog lurker's hide boots@@N` | armor (9) | 67108992 | feet\|take | 6 | L 92 | Loot from bog lurker |
| 1025 | a thornweaver silk wrap | `@@pa thornweaver silk wrap@@N` | armor (9) | 67108992 | waist\|take | 1 | L 93 | Loot from thornweaver spider |
| 1026 | a ghoul-bone ring | `@@pa ghoul-bone ring@@N` | armor (9) | 67108992 | finger\|take | 1 | L 91 | Loot from swamp ghoul |
| 1027 | a moss-covered longsword | `@@Ga moss-covered longsword@@N` | weapon (5) | 128 | hold\|take | 6 | L 95 | Reset (E) on corrupted treant. value3=3 (slash). |
| 1028 | a canopy stalker's talon | `@@Ga canopy stalker's talon@@N` | weapon (5) | 67108992 | hold\|take | 6 | L 95 | Loot from canopy stalker. value3=5 (claw). |
| 1029 | a spore-dusted cowl | `@@pa spore-dusted cowl@@N` | armor (9) | 67108992 | head\|take | 1 | L 90 | Loot from spore shambler |
| 1030 | a briar hound collar | `@@pa briar hound collar@@N` | armor (9) | 67108992 | neck\|take | 6 | L 91 | Loot from briar hound |
| 1031 | a moss horror's tendril whip | `@@pa moss horror's tendril whip@@N` | weapon (5) | 67108992 | hold\|take | 6 | L 94 | Loot from moss horror. value3=4 (whip). |
| 1032 | a withered druid's staff | `@@pa withered druid's staff@@N` | weapon (5) | 128 | hold\|take | 1 | L 96 | Reset (E) on withered druid. value3=7 (pound). |
| 1033 | a vine strangler's cord belt | `@@pa vine strangler's cord belt@@N` | armor (9) | 67108992 | waist\|take | 1 | L 90 | Loot from vine strangler |
| 1034 | a gloomwing moth wing-cloak | `@@pa gloomwing moth wing-cloak@@N` | armor (9) | 67108992 | about\|take | 1 | L 92 | Loot from gloomwing moth |
| 1035 | a heartrot spawn's fang | `@@pa heartrot spawn's fang@@N` | weapon (5) | 67108992 | hold\|take | 6 | L 96 | Loot from heartrot spawn. value3=11 (pierce). |
| 1036 | a corrupted stag's antler mace | `@@pa corrupted stag's antler mace@@N` | weapon (5) | 67108992 | hold\|take | 6 | L 93 | Loot from corrupted stag. value3=8 (crush). |
| 1037 | a miasma wraith's shroud | `@@pa miasma wraith's shroud@@N` | armor (9) | 128 | about\|take | 1 | L 97 | Reset (E) on miasma wraith |
| 1038 | a thornweaver matriarch's legs | `@@pa thornweaver matriarch's legs@@N` | armor (9) | 128 | legs\|take | 6 | L 98 | Reset (E) on thornweaver matriarch |
| 1039 | a rotwood sentinel's core | `@@pa rotwood sentinel's core@@N` | armor (9) | 67108992 | hold\|take | 11 | L 100 | Loot from rotwood sentinel. Shield. |
| 1040 | a fungal spitter's acid gland | `@@pa fungal spitter's acid gland@@N` | armor (9) | 67108992 | hold\|take | 1 | L 91 | Loot from fungal spitter. |
| 1041 | a pair of blighted bear claws | `@@pa pair of blighted bear claws@@N` | weapon (5) | 67108992 + ITEM_FIST(1073741824) = 1140850816 | hold\|take | 6 | L 90 | Loot from blighted bear. Fist. value3=0 (hit). |
| 1045 | a mire crocodile tooth blade | `@@pa mire crocodile tooth blade@@N` | weapon (5) | 67108992 | hold\|take | 6 | L 93 | Loot from mire crocodile. value3=3 (slash). |
| 1046 | a thorn elemental's barbed arm | `@@pa thorn elemental's barbed arm@@N` | weapon (5) | 67108992 | hold\|take | 6 | L 94 | Loot from thorn elemental. value3=11 (pierce). |
| 1047 | a withered ent's bark vest | `@@pa withered ent's bark vest@@N` | armor (9) | 67108992 | body\|take | 11 | L 95 | Loot from withered ent |
| 1048 | a bramble golem's plating | `@@pa bramble golem's plating@@N` | armor (9) | 67108992 | arms\|take | 11 | L 93 | Loot from bramble golem |
| 1049 | a shadow fox pelt hood | `@@pa shadow fox pelt hood@@N` | armor (9) | 67108992 | head\|take | 1 | L 90 | Loot from shadow fox |
| 1050 | a canopy viper fang earring | `@@pa canopy viper fang earring@@N` | armor (9) | 67108992 | ear\|take | 1 | L 91 | Loot from canopy viper |
| 1051 | a corpse flower petal sash | `@@pa corpse flower petal sash@@N` | armor (9) | 67108992 | waist\|take | 1 | L 90 | Loot from corpse flower |
| 1052 | a mycelium creeper's cord | `@@pa mycelium creeper's cord@@N` | armor (9) | 67108992 | wrist\|take | 1 | L 92 | Loot from mycelium creeper |
| 1053 | a fungal zombie's jaw club | `@@pa fungal zombie's jaw club@@N` | weapon (5) | 67108992 | hold\|take | 6 | L 90 | Loot from fungal zombie. value3=8 (crush). |
| 1054 | a peat wraith's ghostly cowl | `@@pa peat wraith's ghostly cowl@@N` | armor (9) | 67108992 | head\|take | 1 | L 93 | Loot from peat wraith |
| 1055 | a corrupted boar tusk dagger | `@@pa corrupted boar tusk dagger@@N` | weapon (5) | 67108992 | hold\|take | 6 | L 92 | Loot from corrupted boar. value3=2 (stab). |
| 1056 | a thornback tortoise shell buckler | `@@pa thornback tortoise shell buckler@@N` | armor (9) | 67108992 + ITEM_BUCKLER(268435456) = 335544320 | hold\|take | 11 | L 94 | Loot from thornback tortoise. Buckler. |
| 1057 | a grove phantom's spectral robe | `@@pa grove phantom's spectral robe@@N` | armor (9) | 67108992 | body\|take | 1 | L 96 | Loot from grove phantom |
| 1058 | a corrupted satyr's horn | `@@pa corrupted satyr's horn@@N` | armor (9) | 67108992 | horns\|take | 6 | L 93 | Loot from corrupted satyr |
| 1059 | a mold hulk's dense mantle | `@@pa mold hulk's dense mantle@@N` | armor (9) | 67108992 | shoulders\|take | 11 | L 95 | Loot from mold hulk |
| 1060 | a blightwood revenant's greatsword | `@@pa blightwood revenant's greatsword@@N` | weapon (5) | 128 + ITEM_TWO_HANDED(2147483648) = 2147483776 | hold\|take | 6 | L 100 | Reset (E) on blightwood revenant. Two-handed. value3=3 (slash). |
| 1061 | a fungal titan's core shard | `@@pa fungal titan's core shard@@N` | armor (9) | 128 | hold\|take | 11 | L 99 | Reset (E) on fungal titan. Shield. |
| 1062 | a corrupted dryad elder's circlet | `@@pa corrupted dryad elder's circlet@@N` | armor (9) | 128 | head\|take | 1 | L 98 | Reset (E) on corrupted dryad elder |
| 1063 | a bog behemoth's hide greaves | `@@pa bog behemoth's hide greaves@@N` | armor (9) | 128 | legs\|take | 11 | L 99 | Reset (E) on bog behemoth |
| 1064 | a canopy horror's talon gauntlets | `@@pa canopy horror's talon gauntlets@@N` | armor (9) | 128 | hands\|take | 6 | L 97 | Reset (E) on canopy horror |
| 1065 | a heartrot abomination's pulsing eye | `@@pa heartrot abomination's pulsing eye@@N` | armor (9) | 128 | halo\|take | 1 | L 100 | Reset (E) on heartrot abomination |
| 1066 | a briar lord's thorned crown | `@@pa briar lord's thorned crown@@N` | armor (9) | 128 | head\|take | 6 | L 98 | Reset (E) on briar lord |
| 1067 | a rotting wolf alpha's fang necklace | `@@pa rotting wolf alpha's fang necklace@@N` | armor (9) | 67108992 | neck\|take | 6 | L 92 | Loot from rotting wolf alpha |
| 1068 | a blight beetle's chitin bracer | `@@pa blight beetle's chitin bracer@@N` | armor (9) | 67108992 | wrist\|take | 6 | L 90 | Loot from blight beetle |
| 1069 | a heartrot tendril's lash | `@@pa heartrot tendril's lash@@N` | weapon (5) | 67108992 | hold\|take | 6 | L 93 | Loot from heartrot tendril. value3=4 (whip). |

### Consumables / Misc (in rooms via `O` reset)

| Vnum | Name | item_type | extra_flags | wear_flags | Level | Notes |
|------|------|-----------|-------------|------------|-------|-------|
| 1070 | a potion of blight resistance | potion (10) | 128 | take | L 90 | Spawns in select rooms. |
| 1071 | a glowing mushroom | food (19) | 128 | take | L 85 | Spawns in Fungal Hollows rooms. |
| 1072 | a vial of purified sap | potion (10) | 128 | take | L 95 | Spawns in Verdant Sanctum rooms. |

---

## Resets Plan

### Mobile Resets (`M` commands)

Each boss spawns once (limit 1) in its designated `no_mob` room:
- `M 0 1000 1 1030` — Fungal Colossus in Mycelium Throne
- `M 0 1001 1 1050` — Briar Queen in Thorn Nexus
- `M 0 1002 1 1114` — Bog Hydra in Drowning Hollow
- `M 0 1003 1 1089` — Canopy Warden in Warden's Perch
- `M 0 1004 1 1135` — Blightmother in Blightmother's Sanctum
- `M 0 1005 1 1145` — Verdant Guardian in Heart of the Forest

Strong mobs spawn 2–3 times each in their zones:
- `M 0 1006 3 <room>` — Rotwood sentinels (3x in Zone 4)
- `M 0 1007 2 <room>` — Miasma wraiths (2x in Zone 6)
- `M 0 1008 2 <room>` — Thornweaver matriarchs (2x in Zone 3)
- `M 0 1030 2 <room>` — Blightwood revenants (2x in Zone 7)
- `M 0 1031 2 <room>` — Fungal titans (2x in Zone 2)
- `M 0 1032 2 <room>` — Corrupted dryad elders (2x in Zone 4)
- `M 0 1033 2 <room>` — Bog behemoths (2x in Zone 6)
- `M 0 1034 2 <room>` — Canopy horrors (2x in Zone 5)
- `M 0 1035 2 <room>` — Heartrot abominations (2x in Zone 7)
- `M 0 1036 2 <room>` — Briar lords (2x in Zone 3)

Trash mobs spawn 2–4 times each, distributed across their zones. Total approximately 120–140 mob resets to populate 150 rooms.

### Equipment Resets (`E` commands, after corresponding `M`)

Reset equipment (non-loot, flagged ITEM_NODROP only):
- `E 0 1018 1 <hold_loc>` — Rotwood bark shield on rotwood sentinel
- `E 0 1027 1 <hold_loc>` — Moss-covered longsword on corrupted treant
- `E 0 1032 1 <hold_loc>` — Withered druid's staff on withered druid
- `E 0 1037 1 <about_loc>` — Miasma wraith's shroud on miasma wraith
- `E 0 1038 1 <legs_loc>` — Thornweaver matriarch's legs on thornweaver matriarch
- `E 0 1060 1 <hold_loc>` — Blightwood revenant's greatsword on blightwood revenant
- `E 0 1061 1 <hold_loc>` — Fungal titan's core shard (shield) on fungal titan
- `E 0 1062 1 <head_loc>` — Corrupted dryad elder's circlet on corrupted dryad elder
- `E 0 1063 1 <legs_loc>` — Bog behemoth's hide greaves on bog behemoth
- `E 0 1064 1 <hands_loc>` — Canopy horror's talon gauntlets on canopy horror
- `E 0 1065 1 <halo_loc>` — Heartrot abomination's pulsing eye on heartrot abomination
- `E 0 1066 1 <head_loc>` — Briar lord's thorned crown on briar lord

### Object Resets (`O` commands, items in rooms)

- `O 0 1070 1 <room>` — Potions in select rooms (3-4 placements)
- `O 0 1071 1 <room>` — Mushrooms in Fungal Hollows (3-4 placements)
- `O 0 1072 1 <room>` — Purified sap in Verdant Sanctum (2 placements)

### Door Resets (`D` commands)

- `D 0 1013 5 2` — Crumbling arch (down) locked
- `D 0 1015 4 2` — Crumbling arch (up) locked
- `D 0 1075 2 1` — Gnarled root gate (south) closed
- `D 0 1095 0 1` — Gnarled root gate (north) closed
- `D 0 1134 0 2` — Pulsing membrane (north) locked
- `D 0 1135 2 2` — Pulsing membrane (south) locked

### Loot Tables (on boss mobs, `l`/`L` extensions)

Each boss has 2–3 items in its loot table with drop chances (e.g., 30–50% per slot). All loot-table items have `ITEM_LOOT` flag.

---

## Specials

```
M 1000 spec_cast_mage
M 1001 spec_cast_cleric
M 1002 spec_breath_acid
M 1003 spec_cast_cleric
M 1004 spec_cast_bigtime
M 1005 spec_cast_cleric
M 1007 spec_cast_undead
M 1010 spec_cast_cleric
M 1012 spec_poison
M 1022 spec_cast_undead
M 1028 spec_cast_mage
M 1032 spec_cast_cleric
M 1041 spec_poison
M 1046 spec_poison
M 1051 spec_poison
M 1053 spec_cast_undead
M 1061 spec_poison
M 1063 spec_cast_undead
M 1064 spec_poison
```

---

## Room Allocation Summary

| Zone | Name | Vnums | Count | Sector | Key Flags |
|------|------|-------|-------|--------|-----------|
| 1 | The Rotting Threshold | 1000–1014 | 15 | forest | no_teleport on some |
| 2 | The Fungal Hollows | 1015–1034 | 20 | inside | dark on some |
| 3 | The Briar Maze | 1035–1054 | 20 | forest | maze on ALL |
| 4 | The Blightwood | 1055–1079 | 25 | forest | dark on most |
| 5 | The Ancient Canopy | 1080–1094 | 15 | forest | — |
| 6 | The Miasma Bog | 1095–1119 | 25 | water_swim | dark on some |
| 7 | The Heartrot Grove | 1120–1139 | 20 | forest | dark on all |
| 8 | The Verdant Sanctum | 1140–1149 | 10 | forest | regen |
| **Total** | | **1000–1149** | **150** | | |

All 150 vnums (1000–1149) are used as rooms.

---

## Connection to Forest of Confusion

**Bidirectional link between room 1000 (The Darkening Path) and room 9695 (Dense Woodland) in `confusn.are`.**

- **In `withered_depths.are`:** Room 1000 has exit **west (D3)** → room 9695.
- **In `confusn.are`:** Room 9695 gains a new exit **east (D1)** → room 1000.

Room 9695 is a dead-end deep in the Forest of Confusion's dense woodland interior. It currently has only a single west exit (to 9694). Adding an east exit to room 1000 creates a natural transition: players traveling east from the withered depths of the confusion forest pass through increasingly corrupted woodland into The Withered Depths. The room's existing description — "very dense woodland, full of withered, stunted trees" — thematically bridges the two areas without modification.

---

## Implementation Checklist

1. Create `area/withered_depths.are` with all sections in canonical order
2. Add `withered_depths.are` to `area/area.lst`
3. Add reciprocal exit in `confusn.are` pointing to room 1000
4. Write all 150 room descriptions (unique, 3+ sentences each, 5+ for important rooms)
5. Write all mobile descriptions (long_descr and description fields)
6. Write all object descriptions
7. Write extra descriptions for rooms with named exits and points of interest
8. Ensure all maze rooms (1035–1054) have `ROOM_MAZE` flag
9. Ensure all boss rooms have `no_mob` flag
10. Ensure vnum sequential ordering (no gaps)
11. Run `make unit-tests` to validate
