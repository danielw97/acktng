# Area Plan: The Withered Depths

## Overview

**Area Name:** `@@pThe Withered @@GDepths@@N`
**Keyword:** `withereddepths`
**Vnum Range:** 291–440 (150 vnums)
**Target Level:** 80–100 (bosses at 110)
**Connection:** Bidirectional link with Forest of Confusion room **2434** (Dense Woodland)
**Sector Type:** `forest` (3) for most rooms; `water_swim` (6) for bog rooms; `inside` (11) for underground fungal caves
**Color Theme:** `@@p` (purple) and `@@G` (green) — purple represents creeping corruption/blight, green represents the underlying forest. `@@b` (brown) accent for earth/wood, `@@d` (dark grey) for decay.
**Reset Rate:** 15
**Reset Message:** `@@pA @@Gfoul wind @@pstirs through the twisted canopy...@@N`
**Area Number:** TBD (next available)

---


## Lore
See `withered_depths_lore.md` for full extracted lore.

### Lore Summary (for implementation reference)

The Withered Depths' corruption originates from abandoned **Spirebound Conclave** field trials (see `sunken_sanctum_lore.md`). The Conclave's **Prism Thesis** faction implanted resonance crystals in the forest's root systems, attempting to bind nature spirits as custodial anchors. When the Conclave collapsed, the crystals were never retrieved and mutated the surrounding ecology over centuries: root systems fused into subterranean networks, fungal colonies gained semi-sentient growth patterns, and the boundary between living and dead matter blurred.

The old druids created the **confusion mists** as a quarantine barrier. When the druids died out (their undead remnants are the withered druids in Zone 7), the quarantine failed and the corruption began spreading.

### Lore-to-Zone Mapping

Each zone corresponds to a layer of the corruption's expression:

| Zone | Corruption Layer | Lore Source |
|------|-----------------|-------------|
| 1: Rotting Threshold | Corruption's edge — visibly stressed but recognizable forest | Quarantine boundary failure |
| 2: Fungal Hollows | Underground crystal-mutated root networks; semi-sentient mycelium | Conclave crystal implantation |
| 3: Briar Maze | Druidic counter-magic vs. Conclave resonance interference zone | Competing magical fields |
| 4: Blightwood | Surface corruption at full strength — animate decay, arrested decomposition | Preservation magic without calibration |
| 5: Ancient Canopy | Vertical corruption — rapid growth-decay cycling in upper forest | Crystal resonance + sunlight interaction |
| 6: Miasma Bog | Liquid corruption — preservation medium in groundwater | Crystal-contaminated drainage |
| 7: Heartrot Grove | Corruption source — highest crystal concentration; Blightmother's nexus | Original Conclave implant sites |
| 8: Verdant Sanctum | Natural magical dead zone — pristine pocket resisting corruption | Forest's immune response |

### Boss Lore Context

| Boss | Lore Role | Conclave Parallel |
|------|-----------|-------------------|
| Fungal Colossus (291) | Crystal-organized fungal construct | Crystal conduit system expressed biologically |
| Briar Queen (292) | Interference-zone adaptation — incorporates both magical frequencies | Druidic/Conclave hybrid creature |
| Bog Hydra (293) | Preservation-medium apex predator | Creature sustained by contaminated waters |
| Canopy Warden (294) | Possibly an original Conclave-bound nature spirit, now corrupted | Conclave spirit-binding protocols |
| Blightmother (295) | Orphaned custodial entity — equivalent to the Conclave's Vaultkeeper | Prism Thesis binding without oversight |
| Verdant Guardian (296) | Forest's immune response — last defender of uncorrupted ground | Natural system opposing Conclave contamination |

### Cross-Area Lore Integration

Embed cross-area references through room descriptions, mob barks, object flavor text, and extra descriptions:

**Conclave signature motifs** (embed in Zones 2, 4, 7):
- Crystal fragments in root systems (faintly luminous blue-white)
- Geometric growth patterns in fungal matter (lattice structures, hexagonal spore formations)
- Fire scarring on the oldest trees near crystal implant sites
- Preservation runes absorbed into fungal surface growth

**Druidic remnant motifs** (embed in Zones 1, 3, 6):
- Stone circle fragments (most visible in Zone 6 druid-circle ruins, rooms 396–404)
- Weathered ward-signs on standing stones
- Seven-pace planting rings matching Everkeeper tradition (see `great_northern_forest_lore.md`)

**Institutional connection threads** (for NPC dialogue and object flavor):
- Midgaard's sealed warrants → Conclave subjects → crystal research → forest trials → blight (see `midgaard_lore.md`, section V)
- Kowloon's Black Ledger containing names of transferred condemned (see `kowloon_lore.md`)
- Kiess's obsidian disc with Conclave triune seal (see `kiess_lore.md`)
- Forest scouts reporting "anomalous decay patterns" to Midgaard (see `midgaard_lore.md`)
- Great Northern Forest's Rootbound cult seeking communion with the Blightmother (see `great_northern_forest_lore.md`)

**Ambient economy effects** (for connected-area NPC flavor):
- "Tainted northern timber" in Midgaard markets
- "Short herbalist shipments" affecting Midgaard and Kiess trade
- Kiess Wall Command posting blight-zone warnings on north-gate bulletin boards

### Material Culture Quick Reference

**Sounds:** dripping sap, spore releases (wet pop), root-system groaning, crystal resonance hum (deep zones only), metallic insect overtones, squelching ground.

**Smells:** sweet rot, methane, sulfur compounds, metallic crystal discharge tang, stagnant water, preservation-medium chemical sharpness.

**Visual textures:** blackened bark with luminous cracks, geometric fungal shelves, crystal-veined root-tunnel walls, purple-tinged ground mist, spiral-and-lattice vine growth, oily iridescent water.

## Zone Layout (8 Zones, 150 Rooms)

### Zone 1: The Rotting Threshold (Rooms 291–305, 15 rooms)
Entry zone connecting to the Forest of Confusion. The mist thickens and the trees grow darker. This is the transition from the lvl 10-60 Forest of Confusion into the high-level Withered Depths.

- **Room 291:** The Darkening Path — entry room, exit west to Forest of Confusion room 2434 (Dense Woodland), exit east deeper in.
- **Rooms 292–299:** Winding trail through increasingly twisted forest. Linear path with some branches.
- **Rooms 300–303:** A clearing where corrupted animals prowl. Wider area with multiple connections.
- **Room 304:** A collapsed stone archway — named exit `^crumbling arch` (door, closed, locked) leading down to Zone 2 (Fungal Hollows). Key required (key object at vnum 291).
- **Room 305:** A watchpost ruin overlooking the canopy, exit up to Zone 6 (Ancient Canopy).

**Connectivity:**
- West from 291 → Forest of Confusion room 2434 (Dense Woodland)
- East/south progression through 292–303
- Down from 304 → Zone 2 (room 306) via locked door
- Up from 305 → Zone 6 (room 371)

### Zone 2: The Fungal Hollows (Rooms 306–325, 20 rooms)
Underground caverns beneath massive root systems, filled with bioluminescent fungi. Sector type: `inside` (11).

- **Rooms 306–315:** Twisting root-tunnels, some narrow, some opening into larger chambers. Various fungal descriptions.
- **Rooms 316–320:** The Spore Caverns — wider rooms with poisonous spore clouds. Some rooms flagged `dark`.
- **Room 321:** The Mycelium Throne — **BOSS ROOM** (flagged `no_mob`). The Fungal Colossus spawns here.
- **Rooms 322–325:** Exit tunnels leading back up to Zone 4 (Blightwood) and connecting to Zone 3 (Briar Maze).

**Connectivity:**
- Down from Zone 1 room 304 → room 306
- Room 325 east → Zone 3 (room 326)
- Room 322 up → Zone 4 (room 346)

### Zone 3: The Briar Maze (Rooms 326–345, 20 rooms)
A tangled labyrinth of thorny vines and twisted brambles. ALL rooms in this zone flagged `ROOM_MAZE` (524288). Exits loop and misdirect. Sector type: `forest` (3).

- **Rooms 326–340:** Maze rooms with disorienting exits. Rooms connect in non-linear patterns (allowed because all are ROOM_MAZE). Descriptions emphasize thorns, tangled vines, and being lost.
- **Room 341:** The Thorn Nexus — **BOSS ROOM** (flagged `no_mob | maze`). The Briar Queen spawns here. Central maze room.
- **Rooms 342–345:** Maze exit paths leading to Zone 4 (Blightwood) and Zone 5 (Miasma Bog).

**Connectivity:**
- West from room 326 → Zone 2 (room 325)
- Maze-internal looping exits throughout 326–345
- Room 345 south → Zone 5 (room 366)
- Room 342 north → Zone 4 (room 346)

### Zone 4: The Blightwood (Rooms 346–370, 25 rooms)
The corrupted heart of the forest at ground level. Massive blackened trees with oozing bark. The canopy blocks nearly all light. Sector type: `forest` (3). Many rooms flagged `dark`.

- **Rooms 346–355:** Dense corrupted forest with multiple branching paths. Room descriptions feature rotting bark, blackened leaves, strange sounds.
- **Rooms 356–365:** Deeper blightwood, where the trees seem almost alive and hostile. Wider area with loops (non-maze, so no directional loops — paths branch and reconverge but never loop in same direction).
- **Room 366:** A named exit `^gnarled root gate` (door, closed) leading south to Zone 5. No lock, just a closeable barrier.
- **Rooms 367–370:** Transition area connecting to Zone 7 (Heartrot Grove) via east exits.

**Connectivity:**
- Up from Zone 2 room 322 → room 346
- North from Zone 3 room 342 → room 346
- Room 366 south (door) → Zone 5 (room 386)
- Room 370 east → Zone 7 (room 401)
- Room 355 up → Zone 6 (room 371)

### Zone 5: The Miasma Bog (Rooms 371–395, 25 rooms)
**Note: Renumbered — rooms 371–385 are Ancient Canopy, 386–405 are Miasma Bog. See corrected layout below.**

*Corrected zone assignments for sequential vnum usage:*

### Zone 5 (Corrected): The Ancient Canopy (Rooms 371–385, 15 rooms)
Elevated treetop paths high above the forest floor. Rickety bridges of woven vines and massive branches. Sector type: `forest` (3). Some rooms flagged `dark`.

- **Rooms 371–379:** Vine-bridge walkways and platform rooms in the canopy. Precarious paths with beautiful but eerie views of the blighted forest below.
- **Room 380:** The Warden's Perch — **BOSS ROOM** (flagged `no_mob`). The Canopy Warden spawns here.
- **Rooms 381–385:** Descending paths from the canopy, connecting back to Zone 4 and forward to Zone 7.

**Connectivity:**
- Up from Zone 1 room 305 → room 371
- Up from Zone 4 room 355 → room 373
- Room 385 down → Zone 7 (room 401)

### Zone 6 (Corrected): The Miasma Bog (Rooms 386–410, 25 rooms)
A fetid swamp where the corruption has liquefied the ground. Poisonous gases hang low. Sector type: `water_swim` (6) for most rooms. Some rooms flagged `dark`.

- **Rooms 386–395:** Shallow bog rooms with treacherous footing. Descriptions feature bubbling muck, rotting vegetation, will-o-wisps.
- **Rooms 396–404:** Deeper swamp, partially submerged ruins of an old druid circle. Some rooms have extra descriptions for examining ruins.
- **Room 405:** The Drowning Hollow — **BOSS ROOM** (flagged `no_mob`). The Bog Hydra spawns here.
- **Rooms 406–410:** Bog edges transitioning to Zone 7 (Heartrot Grove).

**Connectivity:**
- South from Zone 3 room 345 → room 386
- South from Zone 4 room 366 (via door) → room 386
- Room 410 east → Zone 7 (room 411)

### Zone 7: The Heartrot Grove (Rooms 411–430, 20 rooms)
The deepest corruption. Trees here are barely alive, pulsing with a sickly purple glow. The air is thick and oppressive. Sector type: `forest` (3). All rooms `dark`.

- **Rooms 411–420:** The outer grove, where the corruption is palpable. Twisted amalgamations of tree and fungus.
- **Rooms 421–425:** The inner grove, approaching the source. Ground becomes spongy and organic.
- **Room 426:** The Blightmother's Sanctum — **BOSS ROOM** (flagged `no_mob | no_recall | no_teleport`). The Blightmother spawns here. Named exit `^pulsing membrane` (door, closed, locked) from room 425. Key required (key object at vnum 292).
- **Rooms 427–430:** Post-boss rooms / escape route. A hidden path that leads to Zone 8.

**Connectivity:**
- East from Zone 4 room 370 → room 411
- Down from Zone 5 room 385 → room 401 (overlap with zone, adjust: room 385 down → room 411)
- Room 410 east → room 411
- Room 425 north (locked door) → room 426
- Room 430 south → Zone 8 (room 431)

### Zone 8: The Verdant Sanctum (Rooms 431–440, 10 rooms)
A hidden, pristine pocket of the forest untouched by corruption — yet guarded fiercely by an ancient protector who trusts no one. Sector type: `forest` (3). Rooms flagged `regen` (2).

- **Rooms 431–435:** A serene glade with clear water, healthy trees, and soft moss. Stark contrast to the rest of the area.
- **Room 436:** The Heart of the Forest — **BOSS ROOM** (flagged `no_mob | no_recall | no_teleport`). The Verdant Guardian spawns here (optional 6th boss, the forest's last protector, driven mad by the surrounding corruption).
- **Rooms 437–440:** The sanctum's deeper reaches. Room 440 has an exit back to Zone 1 (room 291 area) providing a loop back, but NOT a directional loop (different direction than entry).

**Connectivity:**
- South from Zone 7 room 430 → room 431
- Room 440 west → Zone 1 room 291 (shortcut back to entrance)

---

## Doors Summary

| From Room | Direction | To Room | Door Keyword | Lock? | Key Vnum | Reset State |
|-----------|-----------|---------|--------------|-------|----------|-------------|
| 304 | down | 306 | `^crumbling arch` | Yes (locked) | 291 | `D 304 5 2` (locked) |
| 306 | up | 304 | `^crumbling arch` | Yes (locked) | 291 | `D 306 4 2` (locked) |
| 366 | south | 386 | `^gnarled root gate` | No | -1 | `D 366 2 1` (closed) |
| 386 | north | 366 | `^gnarled root gate` | No | -1 | `D 386 0 1` (closed) |
| 425 | north | 426 | `^pulsing membrane` | Yes (locked) | 292 | `D 425 0 2` (locked) |
| 426 | south | 425 | `^pulsing membrane` | Yes (locked) | 292 | `D 426 2 2` (locked) |

All doors are bidirectional (both sides defined). Locked doors have corresponding key objects in `#OBJECTS`.

---

## Mobiles

### Bosses (Level 110, flagged `boss | sentinel | stay_area`)

All bosses spawn in `no_mob` rooms only.

| Vnum | Name | short_descr | Level | Zone | Room | Notes |
|------|------|-------------|-------|------|------|-------|
| 291 | The Fungal Colossus | `@@pthe Fungal Colossus@@N` | 110 | 2 | 321 | Massive fungal golem. `act`: is_npc\|sentinel\|no_flee\|aggressive\|stay_area\|boss. Strong: earth, poison. Weak: fire. Cast: acid_blast, suffocate. Def: heal, iceshield. Spec: `spec_cast_mage`. Loot table: boss items. |
| 292 | The Briar Queen | `@@pthe Briar Queen@@N` | 110 | 3 | 341 | Thorny fey queen. `act`: is_npc\|sentinel\|no_flee\|aggressive\|stay_area\|boss. Strong: earth, physical. Weak: fire. Cast: chain_light, flamestrike. Def: heal, fireshield. Spec: `spec_cast_cleric`. Loot table: boss items. |
| 293 | The Bog Hydra | `@@pthe Bog Hydra@@N` | 110 | 6 | 405 | Multi-headed swamp beast. `act`: is_npc\|sentinel\|no_flee\|aggressive\|stay_area\|no_mind\|boss. Strong: water, poison. Weak: fire, holy. Cast: acid_blast, earthquake. Def: heal. Spec: `spec_breath_acid`. Skills: 4_attack, enhanced, nodisarm, notrip. |
| 294 | The Canopy Warden | `@@pthe Canopy Warden@@N` | 110 | 5 | 380 | Ancient corrupted treant. `act`: is_npc\|sentinel\|no_flee\|aggressive\|stay_area\|boss. Strong: earth, physical. Weak: fire, shadow. Cast: earthquake, flamestrike. Def: heal, shockshield. Spec: `spec_cast_cleric`. |
| 295 | The Blightmother | `@@pthe Blightmother@@N` | 110 | 7 | 426 | Source of the corruption, final boss. `act`: is_npc\|sentinel\|no_flee\|aggressive\|stay_area\|boss. Strong: poison, shadow, earth. Weak: holy, fire. Cast: suffocate, mindflame, nerve_fire. Def: heal, fireshield, iceshield. Spec: `spec_cast_bigtime`. Skills: 5_attack, enhanced, dodge, parry, counter. |
| 296 | The Verdant Guardian | `@@Gthe Verdant Guardian@@N` | 110 | 8 | 436 | Maddened forest protector, optional boss. `act`: is_npc\|sentinel\|no_flee\|aggressive\|stay_area\|boss. Strong: holy, earth. Weak: shadow, poison. Cast: flamestrike, light_bolt. Def: heal, fireshield, shockshield. Spec: `spec_cast_cleric`. |

### Strong Mobs (flagged `solo | stay_area`, levels 97–100)

| Vnum | Name | Level | Zone | Notes |
|------|------|-------|------|-------|
| 297 | A rotwood sentinel | 100 | 4 | Corrupted tree guardian. aggressive, solo, stay_area. Skills: 3_attack, enhanced, parry. |
| 298 | A miasma wraith | 97 | 6 | Gaseous undead. aggressive, solo, stay_area, undead, no_body. Cast: mind_flail, suffocate. |
| 299 | A thornweaver matriarch | 98 | 3 | Giant spider queen. aggressive, solo, stay_area. Skills: 3_attack, dodge, counter. |
| 321 | A blightwood revenant | 100 | 7 | Undead treant horror. aggressive, solo, stay_area, undead. Skills: 4_attack, enhanced, parry, nodisarm. Cast: earthquake. Def: heal. |
| 322 | A fungal titan | 99 | 2 | Towering mushroom construct. aggressive, solo, stay_area, no_mind. Skills: 3_attack, enhanced, punch, notrip. |
| 323 | A corrupted dryad elder | 98 | 4 | Ancient fey twisted by blight. aggressive, solo, stay_area. Cast: chain_light, flamestrike, faerie_fire. Def: heal, fireshield. Spec: `spec_cast_cleric`. |
| 324 | A bog behemoth | 99 | 6 | Massive swamp reptile. aggressive, solo, stay_area. Skills: 4_attack, enhanced, headbutt, nodisarm. |
| 325 | A canopy horror | 97 | 5 | Winged predator nesting in the treetops. aggressive, solo, stay_area. Skills: 3_attack, enhanced, dodge, kick. |
| 326 | A heartrot abomination | 100 | 7 | Pulsating mass of corrupted flesh and bark. aggressive, solo, stay_area, no_mind. Skills: 4_attack, enhanced, counter. Def: heal. |
| 327 | A briar lord | 98 | 3 | Massive thorned guardian of the maze. aggressive, solo, stay_area. Skills: 3_attack, enhanced, parry, disarm. |

### Trash Mobs (flagged `stay_area | aggressive`, levels 85–96)

| Vnum | Name | Level | Zone(s) | Notes |
|------|------|-------|---------|-------|
| 300 | A blighted wolf | 85 | 1, 4 | Corrupted predator. Skills: 2_attack, enhanced. |
| 301 | A twisted dryad | 90 | 1, 4 | Corrupted fey. Cast: faerie_fire, flare. Def: cure_serious. |
| 302 | A fungal crawler | 88 | 2 | Insectoid fungal creature. Skills: 2_attack, punch. |
| 303 | A venomous viper | 92 | 1, 5, 6 | Giant poisonous snake. Spec: `spec_poison`. |
| 304 | A corrupted treant | 95 | 4 | Blighted tree creature. Skills: 3_attack, enhanced. |
| 305 | A bog lurker | 90 | 6 | Swamp ambush predator. Skills: 2_attack, trip. |
| 306 | A thornweaver spider | 93 | 3 | Giant thorny spider. Skills: 2_attack, dodge. |
| 307 | A swamp ghoul | 91 | 6 | Undead, undead flag. Skills: 2_attack, punch. |
| 308 | A blighted bear | 88 | 1, 4 | Corrupted bear. Skills: 2_attack, enhanced. |
| 309 | A canopy stalker | 95 | 5 | Predatory bird-lizard. Skills: 3_attack, dodge. |
| 310 | A spore shambler | 86 | 2 | Shambling fungal mass. no_mind. Skills: 2_attack. |
| 311 | A briar hound | 89 | 3, 4 | Thorn-covered beast. Skills: 2_attack, enhanced. |
| 312 | A moss horror | 94 | 7 | Creeping plant horror. Skills: 3_attack, enhanced. |
| 313 | A withered druid | 96 | 7 | Undead druid. undead. Cast: flamestrike, earthquake. Def: heal. |
| 314 | A vine strangler | 87 | 1, 4 | Animated vine mass. no_mind. Skills: 2_attack, trip. |
| 315 | A bog wisp | 85 | 6 | Will-o-wisp. no_body. Cast: shock_grasp, flare. |
| 316 | A gloomwing moth | 90 | 5 | Giant corrupted moth. Skills: 2_attack, dodge. |
| 317 | A heartrot spawn | 96 | 7, 8 | Blight creature. Skills: 3_attack, enhanced, punch. |
| 318 | A corrupted stag | 92 | 4 | Warped forest animal. Skills: 2_attack, charge, enhanced. |
| 319 | A fungal spitter | 89 | 2 | Ranged fungal mob. Cast: acid_blast. |
| 328 | A rotting wolf alpha | 90 | 1, 4 | Pack leader of the blighted wolves. Skills: 3_attack, enhanced, dodge. |
| 329 | A mire crocodile | 93 | 6 | Massive swamp predator. Skills: 2_attack, enhanced, trip. |
| 330 | A blighted owl | 86 | 5 | Corrupted raptor with glowing eyes. Skills: 2_attack, dodge. |
| 331 | A thorn elemental | 94 | 3 | Animated mass of thorny vines. no_mind. Skills: 3_attack, enhanced. |
| 332 | A spore cloud | 85 | 2 | Floating toxic fungal mass. no_body, no_mind. Cast: suffocate. Spec: `spec_poison`. |
| 333 | A withered ent | 95 | 4, 7 | Dying tree creature lashing out. Skills: 3_attack, enhanced, punch. |
| 334 | A bog rat swarm | 86 | 6 | Swarm of corrupted rats. Skills: 2_attack, punch. |
| 335 | A bramble golem | 93 | 3 | Construct of tangled thorny branches. no_mind. Skills: 2_attack, enhanced, parry. |
| 336 | A shadow fox | 88 | 1, 4 | Dark furred predator that hunts by stealth. Skills: 2_attack, dodge, trip. |
| 337 | A canopy viper | 91 | 5 | Arboreal serpent with virulent toxin. Spec: `spec_poison`. Skills: 2_attack. |
| 338 | A corpse flower | 87 | 6, 7 | Carnivorous plant animated by corruption. no_mind. Skills: 2_attack, trip. |
| 339 | A mycelium creeper | 90 | 2 | Network of fungal tendrils that ambushes prey. no_mind. Skills: 2_attack, enhanced, trip. |
| 340 | A blighted hawk | 89 | 5 | Corrupted bird of prey circling the canopy. Skills: 2_attack, enhanced, dodge. |
| 341 | A fungal zombie | 88 | 2 | Reanimated corpse covered in parasitic mushrooms. undead. Skills: 2_attack, punch. |
| 342 | A thorn dart frog | 85 | 3, 6 | Tiny but deadly poisonous amphibian. Spec: `spec_poison`. |
| 343 | A withered wolf spider | 91 | 3 | Large wolf spider with rotting exoskeleton. Skills: 2_attack, dodge. |
| 344 | A peat wraith | 93 | 6 | Ghostly bog spirit. undead, no_body. Cast: mind_flail, ego_whip. |
| 345 | A blight beetle | 86 | 2, 4 | Oversized beetle infested with fungal growth. Skills: 2_attack, enhanced. |
| 346 | A corrupted boar | 90 | 1, 4 | Massive warped boar with fungal tusks. Skills: 2_attack, enhanced, charge. |
| 347 | A verdant wisp | 87 | 8 | Faded forest spirit clinging to old magic. no_body. Cast: flare, light_bolt. Def: cure_serious. Non-aggressive. |
| 348 | A canopy weaver | 92 | 5 | Spider that builds webs between the treetops. Skills: 2_attack, dodge, trip. |
| 349 | A rotwood beetle swarm | 88 | 4, 7 | Cloud of wood-boring insects. Skills: 2_attack, enhanced. |
| 350 | A mire leech | 85 | 6 | Giant parasitic swamp leech. Skills: 2_attack. |
| 351 | A fungal bat colony | 87 | 2 | Cluster of bats coated in luminous spores. Skills: 2_attack, dodge. |
| 352 | A blighted centipede | 89 | 2, 4 | Enormous segmented crawler oozing poison. Spec: `spec_poison`. Skills: 2_attack. |
| 353 | A thornback tortoise | 94 | 3 | Slow but heavily armored reptile covered in thorns. Skills: 2_attack, parry, nodisarm. |
| 354 | A grove phantom | 96 | 7 | Spectral remnant of a druid who fell to the blight. undead, no_body. Cast: mindflame, mind_bolt. Def: cure_critic. |
| 355 | A swamp adder | 88 | 6 | Thick-bodied venomous water snake. Spec: `spec_poison`. Skills: 2_attack. |
| 356 | A blighted raven | 85 | 1, 5 | Corrupted scavenger bird with eerie intelligence. Skills: 2_attack, dodge. |
| 357 | A heartrot tendril | 91 | 7 | Animated root-tentacle of the Blightmother. no_mind. Skills: 2_attack, trip. |
| 358 | A corrupted satyr | 93 | 4 | Warped fey creature driven mad by blight. Skills: 2_attack, enhanced, kick. Cast: faerie_fire. |
| 359 | A mold hulk | 95 | 2 | Shambling heap of rotting vegetation held together by mold. no_mind. Skills: 3_attack, enhanced. |

### Non-Combat / Utility Mobs

| Vnum | Name | Level | Notes |
|------|------|-------|-------|
| 320 | A trapped explorer | 50 | Non-aggressive, sentinel, stay_area. Found in Zone 1. Flavor NPC. |
| 360 | A dying forest sprite | 40 | Non-aggressive, sentinel, stay_area. Found in Zone 8. Flavor NPC, whispers lore about the Blightmother. |
| 361 | A lost herbalist | 55 | Non-aggressive, sentinel, stay_area. Found in Zone 4. Flavor NPC seeking rare fungi. |
| 362 | A feral cat | 30 | Non-aggressive, stay_area. Wanders Zone 1. Ambient wildlife. |

---

## Objects

### Keys

| Vnum | Name | short_descr | item_type | extra_flags | wear_flags | Level | Notes |
|------|------|-------------|-----------|-------------|------------|-------|-------|
| 291 | a corroded iron key | `@@da corroded iron key@@N` | key (18) | ITEM_NODROP (128) | take (8388608) | L 90 | Opens crumbling arch (rooms 304/306). |
| 292 | a pulsing blight key | `@@pa pulsing blight key@@N` | key (18) | ITEM_NODROP (128) | take (8388608) | L 100 | Opens pulsing membrane (rooms 425/426). Dropped by a withered druid (vnum 313) as loot. |

### Boss Equipment (Level 100, flagged `ITEM_NODROP | ITEM_BOSS | ITEM_MAGIC`)

extra_flags base = ITEM_NODROP(128) + ITEM_BOSS(134217728) + ITEM_MAGIC(64) = 134217920

Items delivered via boss loot tables (`l`/`L` extensions). All boss loot items additionally get `ITEM_LOOT` (67108864), making total extra_flags = 201326784.

| Vnum | Name | short_descr | item_type | extra_flags | wear_flags | weight | Level | Source Boss |
|------|------|-------------|-----------|-------------|------------|--------|-------|-------------|
| 293 | a spore-encrusted helm | `@@pa spore-encrusted helm@@N` | armor (9) | 201326784 | head\|take | 11 | L 100 | Fungal Colossus (loot) |
| 294 | the Colossus Core | `@@pthe Colossus Core@@N` | armor (9) | 201326784 | hold\|take | 11 | L 100 | Fungal Colossus (loot). Shield. |
| 295 | a crown of living thorns | `@@pa crown of living thorns@@N` | armor (9) | 201326784 | head\|take | 1 | L 100 | Briar Queen (loot) |
| 296 | the Briar Queen's scepter | `@@pthe Briar Queen's scepter@@N` | weapon (5) | 201326784 | hold\|take | 1 | L 100 | Briar Queen (loot). value3=7 (pound). |
| 297 | a hydra-scale hauberk | `@@pa hydra-scale hauberk@@N` | armor (9) | 201326784 | body\|take | 11 | L 100 | Bog Hydra (loot) |
| 298 | a fang of the hydra | `@@pa fang of the hydra@@N` | weapon (5) | 201326784 | hold\|take | 6 | L 100 | Bog Hydra (loot). value3=2 (stab). |
| 299 | gauntlets of living bark | `@@Ggauntlets of living bark@@N` | armor (9) | 201326784 | hands\|take | 6 | L 100 | Canopy Warden (loot) |
| 300 | the Warden's gnarled staff | `@@Gthe Warden's gnarled staff@@N` | weapon (5) | 201326784 + ITEM_TWO_HANDED(2147483648) = 2348810432 | hold\|take | 6 | L 100 | Canopy Warden (loot). Two-handed. value3=7 (pound). |
| 301 | a carapace of the Blightmother | `@@pa carapace of the Blightmother@@N` | armor (9) | 201326784 | body\|take | 11 | L 100 | Blightmother (loot) |
| 302 | the Blightmother's claw | `@@pthe Blightmother's claw@@N` | weapon (5) | 201326784 + ITEM_FIST(1073741824) = 1275068608 | hold\|take | 6 | L 100 | Blightmother (loot). Fist weapon. value3=0 (hit). |
| 303 | a mantle of verdant wrath | `@@Ga mantle of verdant wrath@@N` | armor (9) | 201326784 | shoulders\|take | 6 | L 100 | Verdant Guardian (loot) |
| 304 | the Guardian's living blade | `@@Gthe Guardian's living blade@@N` | weapon (5) | 201326784 | hold\|take | 1 | L 100 | Verdant Guardian (loot). value3=3 (slash). |
| 305 | a ring of fungal resilience | `@@pa ring of fungal resilience@@N` | armor (9) | 201326784 | finger\|take | 1 | L 100 | Fungal Colossus (loot) |
| 306 | the Briar Queen's thorned cloak | `@@pthe Briar Queen's thorned cloak@@N` | armor (9) | 201326784 | about\|take | 1 | L 100 | Briar Queen (loot) |
| 307 | a hydra-fang necklace | `@@pa hydra-fang necklace@@N` | armor (9) | 201326784 | neck\|take | 6 | L 100 | Bog Hydra (loot) |
| 308 | the Blightmother's eye | `@@pthe Blightmother's eye@@N` | armor (9) | 201326784 | halo\|take | 1 | L 100 | Blightmother (loot) |

### Trash Equipment (Levels 90–100, flagged `ITEM_NODROP`)

Items spawned via resets (`E`/`G` commands) do NOT get ITEM_LOOT. Items spawned via loot tables DO get ITEM_LOOT.

extra_flags for reset gear: ITEM_NODROP(128) = 128
extra_flags for loot-table gear: ITEM_NODROP(128) + ITEM_LOOT(67108864) = 67108992

| Vnum | Name | short_descr | item_type | extra_flags | wear_flags | weight | Level | Delivery |
|------|------|-------------|-----------|-------------|------------|--------|-------|----------|
| 309 | a rotwood bark shield | `@@pa rotwood bark shield@@N` | armor (9) | 128 | hold\|take | 11 | L 95 | Reset (E) on rotwood sentinel |
| 310 | a blighted wolf pelt | `@@pa blighted wolf pelt@@N` | armor (9) | 67108992 | about\|take | 6 | L 90 | Loot from blighted wolf |
| 311 | a twisted dryad's circlet | `@@pa twisted dryad's circlet@@N` | armor (9) | 67108992 | head\|take | 1 | L 92 | Loot from twisted dryad |
| 312 | a fungal crawler carapace | `@@pa fungal crawler carapace@@N` | armor (9) | 67108992 | body\|take | 11 | L 90 | Loot from fungal crawler |
| 313 | a viper-fang dagger | `@@pa viper-fang dagger@@N` | weapon (5) | 67108992 | hold\|take | 6 | L 93 | Loot from venomous viper. value3=2 (stab). |
| 314 | a treant-bark bracer | `@@pa treant-bark bracer@@N` | armor (9) | 67108992 | wrist\|take | 11 | L 95 | Loot from corrupted treant |
| 315 | a bog lurker's hide boots | `@@pa bog lurker's hide boots@@N` | armor (9) | 67108992 | feet\|take | 6 | L 92 | Loot from bog lurker |
| 316 | a thornweaver silk wrap | `@@pa thornweaver silk wrap@@N` | armor (9) | 67108992 | waist\|take | 1 | L 93 | Loot from thornweaver spider |
| 317 | a ghoul-bone ring | `@@pa ghoul-bone ring@@N` | armor (9) | 67108992 | finger\|take | 1 | L 91 | Loot from swamp ghoul |
| 318 | a moss-covered longsword | `@@Ga moss-covered longsword@@N` | weapon (5) | 128 | hold\|take | 6 | L 95 | Reset (E) on corrupted treant. value3=3 (slash). |
| 319 | a canopy stalker's talon | `@@Ga canopy stalker's talon@@N` | weapon (5) | 67108992 | hold\|take | 6 | L 95 | Loot from canopy stalker. value3=5 (claw). |
| 320 | a spore-dusted cowl | `@@pa spore-dusted cowl@@N` | armor (9) | 67108992 | head\|take | 1 | L 90 | Loot from spore shambler |
| 321 | a briar hound collar | `@@pa briar hound collar@@N` | armor (9) | 67108992 | neck\|take | 6 | L 91 | Loot from briar hound |
| 322 | a moss horror's tendril whip | `@@pa moss horror's tendril whip@@N` | weapon (5) | 67108992 | hold\|take | 6 | L 94 | Loot from moss horror. value3=4 (whip). |
| 323 | a withered druid's staff | `@@pa withered druid's staff@@N` | weapon (5) | 128 | hold\|take | 1 | L 96 | Reset (E) on withered druid. value3=7 (pound). |
| 324 | a vine strangler's cord belt | `@@pa vine strangler's cord belt@@N` | armor (9) | 67108992 | waist\|take | 1 | L 90 | Loot from vine strangler |
| 325 | a gloomwing moth wing-cloak | `@@pa gloomwing moth wing-cloak@@N` | armor (9) | 67108992 | about\|take | 1 | L 92 | Loot from gloomwing moth |
| 326 | a heartrot spawn's fang | `@@pa heartrot spawn's fang@@N` | weapon (5) | 67108992 | hold\|take | 6 | L 96 | Loot from heartrot spawn. value3=11 (pierce). |
| 327 | a corrupted stag's antler mace | `@@pa corrupted stag's antler mace@@N` | weapon (5) | 67108992 | hold\|take | 6 | L 93 | Loot from corrupted stag. value3=8 (crush). |
| 328 | a miasma wraith's shroud | `@@pa miasma wraith's shroud@@N` | armor (9) | 128 | about\|take | 1 | L 97 | Reset (E) on miasma wraith |
| 329 | a thornweaver matriarch's legs | `@@pa thornweaver matriarch's legs@@N` | armor (9) | 128 | legs\|take | 6 | L 98 | Reset (E) on thornweaver matriarch |
| 330 | a rotwood sentinel's core | `@@pa rotwood sentinel's core@@N` | armor (9) | 67108992 | hold\|take | 11 | L 100 | Loot from rotwood sentinel. Shield. |
| 331 | a fungal spitter's acid gland | `@@pa fungal spitter's acid gland@@N` | armor (9) | 67108992 | hold\|take | 1 | L 91 | Loot from fungal spitter. |
| 332 | a pair of blighted bear claws | `@@pa pair of blighted bear claws@@N` | weapon (5) | 67108992 + ITEM_FIST(1073741824) = 1140850816 | hold\|take | 6 | L 90 | Loot from blighted bear. Fist. value3=0 (hit). |
| 336 | a mire crocodile tooth blade | `@@pa mire crocodile tooth blade@@N` | weapon (5) | 67108992 | hold\|take | 6 | L 93 | Loot from mire crocodile. value3=3 (slash). |
| 337 | a thorn elemental's barbed arm | `@@pa thorn elemental's barbed arm@@N` | weapon (5) | 67108992 | hold\|take | 6 | L 94 | Loot from thorn elemental. value3=11 (pierce). |
| 338 | a withered ent's bark vest | `@@pa withered ent's bark vest@@N` | armor (9) | 67108992 | body\|take | 11 | L 95 | Loot from withered ent |
| 339 | a bramble golem's plating | `@@pa bramble golem's plating@@N` | armor (9) | 67108992 | arms\|take | 11 | L 93 | Loot from bramble golem |
| 340 | a shadow fox pelt hood | `@@pa shadow fox pelt hood@@N` | armor (9) | 67108992 | head\|take | 1 | L 90 | Loot from shadow fox |
| 341 | a canopy viper fang earring | `@@pa canopy viper fang earring@@N` | armor (9) | 67108992 | ear\|take | 1 | L 91 | Loot from canopy viper |
| 342 | a corpse flower petal sash | `@@pa corpse flower petal sash@@N` | armor (9) | 67108992 | waist\|take | 1 | L 90 | Loot from corpse flower |
| 343 | a mycelium creeper's cord | `@@pa mycelium creeper's cord@@N` | armor (9) | 67108992 | wrist\|take | 1 | L 92 | Loot from mycelium creeper |
| 344 | a fungal zombie's jaw club | `@@pa fungal zombie's jaw club@@N` | weapon (5) | 67108992 | hold\|take | 6 | L 90 | Loot from fungal zombie. value3=8 (crush). |
| 345 | a peat wraith's ghostly cowl | `@@pa peat wraith's ghostly cowl@@N` | armor (9) | 67108992 | head\|take | 1 | L 93 | Loot from peat wraith |
| 346 | a corrupted boar tusk dagger | `@@pa corrupted boar tusk dagger@@N` | weapon (5) | 67108992 | hold\|take | 6 | L 92 | Loot from corrupted boar. value3=2 (stab). |
| 347 | a thornback tortoise shell buckler | `@@pa thornback tortoise shell buckler@@N` | armor (9) | 67108992 + ITEM_BUCKLER(268435456) = 335544320 | hold\|take | 11 | L 94 | Loot from thornback tortoise. Buckler. |
| 348 | a grove phantom's spectral robe | `@@pa grove phantom's spectral robe@@N` | armor (9) | 67108992 | body\|take | 1 | L 96 | Loot from grove phantom |
| 349 | a corrupted satyr's horn | `@@pa corrupted satyr's horn@@N` | armor (9) | 67108992 | horns\|take | 6 | L 93 | Loot from corrupted satyr |
| 350 | a mold hulk's dense mantle | `@@pa mold hulk's dense mantle@@N` | armor (9) | 67108992 | shoulders\|take | 11 | L 95 | Loot from mold hulk |
| 351 | a blightwood revenant's greatsword | `@@pa blightwood revenant's greatsword@@N` | weapon (5) | 128 + ITEM_TWO_HANDED(2147483648) = 2147483776 | hold\|take | 6 | L 100 | Reset (E) on blightwood revenant. Two-handed. value3=3 (slash). |
| 352 | a fungal titan's core shard | `@@pa fungal titan's core shard@@N` | armor (9) | 128 | hold\|take | 11 | L 99 | Reset (E) on fungal titan. Shield. |
| 353 | a corrupted dryad elder's circlet | `@@pa corrupted dryad elder's circlet@@N` | armor (9) | 128 | head\|take | 1 | L 98 | Reset (E) on corrupted dryad elder |
| 354 | a bog behemoth's hide greaves | `@@pa bog behemoth's hide greaves@@N` | armor (9) | 128 | legs\|take | 11 | L 99 | Reset (E) on bog behemoth |
| 355 | a canopy horror's talon gauntlets | `@@pa canopy horror's talon gauntlets@@N` | armor (9) | 128 | hands\|take | 6 | L 97 | Reset (E) on canopy horror |
| 356 | a heartrot abomination's pulsing eye | `@@pa heartrot abomination's pulsing eye@@N` | armor (9) | 128 | halo\|take | 1 | L 100 | Reset (E) on heartrot abomination |
| 357 | a briar lord's thorned crown | `@@pa briar lord's thorned crown@@N` | armor (9) | 128 | head\|take | 6 | L 98 | Reset (E) on briar lord |
| 358 | a rotting wolf alpha's fang necklace | `@@pa rotting wolf alpha's fang necklace@@N` | armor (9) | 67108992 | neck\|take | 6 | L 92 | Loot from rotting wolf alpha |
| 359 | a blight beetle's chitin bracer | `@@pa blight beetle's chitin bracer@@N` | armor (9) | 67108992 | wrist\|take | 6 | L 90 | Loot from blight beetle |
| 360 | a heartrot tendril's lash | `@@pa heartrot tendril's lash@@N` | weapon (5) | 67108992 | hold\|take | 6 | L 93 | Loot from heartrot tendril. value3=4 (whip). |

### Consumables / Misc (in rooms via `O` reset)

| Vnum | Name | item_type | extra_flags | wear_flags | Level | Notes |
|------|------|-----------|-------------|------------|-------|-------|
| 361 | a potion of blight resistance | potion (10) | 128 | take | L 90 | Spawns in select rooms. |
| 362 | a glowing mushroom | food (19) | 128 | take | L 85 | Spawns in Fungal Hollows rooms. |
| 363 | a vial of purified sap | potion (10) | 128 | take | L 95 | Spawns in Verdant Sanctum rooms. |

---

## Resets Plan

### Mobile Resets (`M` commands)

Each boss spawns once (limit 1) in its designated `no_mob` room:
- `M 0 291 1 321` — Fungal Colossus in Mycelium Throne
- `M 0 292 1 341` — Briar Queen in Thorn Nexus
- `M 0 293 1 405` — Bog Hydra in Drowning Hollow
- `M 0 294 1 380` — Canopy Warden in Warden's Perch
- `M 0 295 1 426` — Blightmother in Blightmother's Sanctum
- `M 0 296 1 436` — Verdant Guardian in Heart of the Forest

Strong mobs spawn 2–3 times each in their zones:
- `M 0 297 3 <room>` — Rotwood sentinels (3x in Zone 4)
- `M 0 298 2 <room>` — Miasma wraiths (2x in Zone 6)
- `M 0 299 2 <room>` — Thornweaver matriarchs (2x in Zone 3)
- `M 0 321 2 <room>` — Blightwood revenants (2x in Zone 7)
- `M 0 322 2 <room>` — Fungal titans (2x in Zone 2)
- `M 0 323 2 <room>` — Corrupted dryad elders (2x in Zone 4)
- `M 0 324 2 <room>` — Bog behemoths (2x in Zone 6)
- `M 0 325 2 <room>` — Canopy horrors (2x in Zone 5)
- `M 0 326 2 <room>` — Heartrot abominations (2x in Zone 7)
- `M 0 327 2 <room>` — Briar lords (2x in Zone 3)

Trash mobs spawn 2–4 times each, distributed across their zones. Total approximately 120–140 mob resets to populate 150 rooms.

### Equipment Resets (`E` commands, after corresponding `M`)

Reset equipment (non-loot, flagged ITEM_NODROP only):
- `E 0 309 1 <hold_loc>` — Rotwood bark shield on rotwood sentinel
- `E 0 318 1 <hold_loc>` — Moss-covered longsword on corrupted treant
- `E 0 323 1 <hold_loc>` — Withered druid's staff on withered druid
- `E 0 328 1 <about_loc>` — Miasma wraith's shroud on miasma wraith
- `E 0 329 1 <legs_loc>` — Thornweaver matriarch's legs on thornweaver matriarch
- `E 0 351 1 <hold_loc>` — Blightwood revenant's greatsword on blightwood revenant
- `E 0 352 1 <hold_loc>` — Fungal titan's core shard (shield) on fungal titan
- `E 0 353 1 <head_loc>` — Corrupted dryad elder's circlet on corrupted dryad elder
- `E 0 354 1 <legs_loc>` — Bog behemoth's hide greaves on bog behemoth
- `E 0 355 1 <hands_loc>` — Canopy horror's talon gauntlets on canopy horror
- `E 0 356 1 <halo_loc>` — Heartrot abomination's pulsing eye on heartrot abomination
- `E 0 357 1 <head_loc>` — Briar lord's thorned crown on briar lord

### Object Resets (`O` commands, items in rooms)

- `O 0 361 1 <room>` — Potions in select rooms (3-4 placements)
- `O 0 362 1 <room>` — Mushrooms in Fungal Hollows (3-4 placements)
- `O 0 363 1 <room>` — Purified sap in Verdant Sanctum (2 placements)

### Door Resets (`D` commands)

- `D 0 304 5 2` — Crumbling arch (down) locked
- `D 0 306 4 2` — Crumbling arch (up) locked
- `D 0 366 2 1` — Gnarled root gate (south) closed
- `D 0 386 0 1` — Gnarled root gate (north) closed
- `D 0 425 0 2` — Pulsing membrane (north) locked
- `D 0 426 2 2` — Pulsing membrane (south) locked

### Loot Tables (on boss mobs, `l`/`L` extensions)

Each boss has 2–3 items in its loot table with drop chances (e.g., 30–50% per slot). All loot-table items have `ITEM_LOOT` flag.

---

## Specials

```
M 291 spec_cast_mage
M 292 spec_cast_cleric
M 293 spec_breath_acid
M 294 spec_cast_cleric
M 295 spec_cast_bigtime
M 296 spec_cast_cleric
M 298 spec_cast_undead
M 301 spec_cast_cleric
M 303 spec_poison
M 313 spec_cast_undead
M 319 spec_cast_mage
M 323 spec_cast_cleric
M 332 spec_poison
M 337 spec_poison
M 342 spec_poison
M 344 spec_cast_undead
M 352 spec_poison
M 354 spec_cast_undead
M 355 spec_poison
```

---

## Room Allocation Summary

| Zone | Name | Vnums | Count | Sector | Key Flags |
|------|------|-------|-------|--------|-----------|
| 1 | The Rotting Threshold | 291–305 | 15 | forest | no_teleport on some |
| 2 | The Fungal Hollows | 306–325 | 20 | inside | dark on some |
| 3 | The Briar Maze | 326–345 | 20 | forest | maze on ALL |
| 4 | The Blightwood | 346–370 | 25 | forest | dark on most |
| 5 | The Ancient Canopy | 371–385 | 15 | forest | — |
| 6 | The Miasma Bog | 386–410 | 25 | water_swim | dark on some |
| 7 | The Heartrot Grove | 411–430 | 20 | forest | dark on all |
| 8 | The Verdant Sanctum | 431–440 | 10 | forest | regen |
| **Total** | | **291–440** | **150** | | |

All 150 vnums (291–440) are used as rooms.

---

## Connection to Forest of Confusion

**Bidirectional link between room 291 (The Darkening Path) and room 2434 (Dense Woodland) in `confusn.are`.**

- **In `withered_depths.are`:** Room 291 has exit **west (D3)** → room 2434.
- **In `confusn.are`:** Room 2434 gains a new exit **east (D1)** → room 291.

Room 2434 is a dead-end deep in the Forest of Confusion's dense woodland interior. It currently has only a single west exit (to 2433). Adding an east exit to room 291 creates a natural transition: players traveling east from the withered depths of the confusion forest pass through increasingly corrupted woodland into The Withered Depths. The room's existing description — "very dense woodland, full of withered, stunted trees" — thematically bridges the two areas without modification.

---

## Implementation Checklist

1. Create `area/withered_depths.are` with all sections in canonical order
2. Add `withered_depths.are` to `area/area.lst`
3. Add reciprocal exit in `confusn.are` pointing to room 291
4. Write all 150 room descriptions (unique, 3+ sentences each, 5+ for important rooms)
5. Write all mobile descriptions (long_descr and description fields)
6. Write all object descriptions
7. Write extra descriptions for rooms with named exits and points of interest
8. Ensure all maze rooms (326–345) have `ROOM_MAZE` flag
9. Ensure all boss rooms have `no_mob` flag
10. Ensure vnum sequential ordering (no gaps)
11. Run `make unit-tests` to validate
