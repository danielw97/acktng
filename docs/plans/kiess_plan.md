# Area Plan: Kiess

## Overview
- **Area Name:** `@@W@@BKiess@@N`
- **File Name:** `kiess.are`
- **Keyword:** `kiess`
- **Intended Levels:** 1-5 (all-player-level city hub)
- **Vnum Range:** `3339-3438`
- **Vnum Count:** 100 (all vnums used)
- **Shape:** 10x10 rectangle (full grid used)
- **Recall Room:** `3394` (city center)
- **Reset Rate:** 15


## Lore
See `kiess_lore.md` for extracted lore.

## Color Theme (multi-color)
- **Primary civic stone:** `@@W` (white)
- **Canals and glasswork:** `@@a` (light cyan)
- **Markets and banners:** `@@y` (yellow)
- **Noble/admin district:** `@@B` (blue)
- **Gardens and commons:** `@@G` (green)
- **Danger/justice accents:** `@@R` (red)
- **Reset:** `@@N` after each colored phrase

## #AREA Header Plan

- `Q 16` — mandatory version; any other value is out of spec
- `K kiess~` — single-word keyword, no spaces, no color codes
- `L @@W(@@a1 5@@W)@@N~` — white parens, light cyan level numbers, white close paren, reset
- `N 77` — confirmed unused across all loaded area files
- `I 1 5` — all-level city hub; controls mob-level gating and area matching logic
- `V 3339 3438` — full 100-vnum envelope; all rooms, mobs, and objects must stay within this range
- `X 0` — map offset; no offset needed for this area
- `F 15` — reset frequency in minutes
- `U @@Wthe bells of @@BKiess@@W ring across the avenues@@N~` — reset message; white text with blue city name, reset; no double-newlines, terminated with `~`
- `T` — teleport flag line (rest ignored by loader); presence enables teleport into the area
- `O Virant~` — area owner must always be Virant
- `R`, `W` — ACL directives omitted; set at implementation time by the builder


## Rectangular Room Layout (10x10)

Coordinates: west->east `x=0..9`, north->south `y=0..9`.
`vnum = 3339 + (y * 10) + x`

| y\\x | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 |
|---|---|---|---|---|---|---|---|---|---|---|
| 0 | 3339 | 3340 | 3341 | 3342 | 3343 | 3344 | 3345 | 3346 | 3347 | 3348 |
| 1 | 3349 | 3350 | 3351 | 3352 | 3353 | 3354 | 3355 | 3356 | 3357 | 3358 |
| 2 | 3359 | 3360 | 3361 | 3362 | 3363 | 3364 | 3365 | 3366 | 3367 | 3368 |
| 3 | 3369 | 3370 | 3371 | 3372 | 3373 | 3374 | 3375 | 3376 | 3377 | 3378 |
| 4 | 3379 | 3380 | 3381 | 3382 | 3383 | 3384 | 3385 | 3386 | 3387 | 3388 |
| 5 | 3389 | 3390 | 3391 | 3392 | 3393 | 3394 | 3395 | 3396 | 3397 | 3398 |
| 6 | 3399 | 3400 | 3401 | 3402 | 3403 | 3404 | 3405 | 3406 | 3407 | 3408 |
| 7 | 3409 | 3410 | 3411 | 3412 | 3413 | 3414 | 3415 | 3416 | 3417 | 3418 |
| 8 | 3419 | 3420 | 3421 | 3422 | 3423 | 3424 | 3425 | 3426 | 3427 | 3428 |
| 9 | 3429 | 3430 | 3431 | 3432 | 3433 | 3434 | 3435 | 3436 | 3437 | 3438 |

## District Layout (by rectangle)
- **North Gate & Caravan Ring (y=0-1):** arrivals, wagon spaces, and traveler services.
- **West Market Wards (x=0-2, y=2-7):** general stores, food, clothier, smithy, weapon shop.
- **East Arcane Wards (x=7-9, y=2-7):** magic shop, sage, healer, temple support, and research services.
- **Central Civic Cross (x=4-6, y=4-6):** recall square, justice, banking, communication.
- **South Military/Travel Ring (y=8-9):** quartermaster, training yards, ship/portal offices, outbound gates.

## Mandatory Service Rooms (exact placements)
- **3394: `@@WThe @@BCentral Prism@@N`** -- **Recall room**, city heart, **Executioner** stationed here.
- **3393: Postmaster of Kiess** -- mail services.
- **3395: Quartermaster's Depot** -- supplies and logistical gear.
- **3384: Grand Market Exchange** -- general shop.
- **3383: Kiess Bank of Ledgers** -- banker.
- **3385: Hall of Restoration** -- healer.
- **3382: Steel and Edge Atelier** -- weapon shop.
- **3392: Bastion Armory** -- armor shop.
- **3396: Argent Arcana** -- magic shop.
- **3403: Wayfarer's Inn** -- rent/innkeeper.
- **3405: Cartographer and Scribe** -- maps, utility items.
- **3374: Temple of Concord** -- donation/recall support.
- **3424: Guild Registrar Hall** -- class guidance/training hooks.

## NPC Level Targets
- **City authority and service NPCs:** level **150**, flagged **sentinel**
 - Executioner (`3394`): flagged **executioner**, **sentinel**, and **boss**; never allowed to move
 - Postmaster (`3393`)
 - Quartermaster (`3395`)
 - All shopkeepers and service keepers (banker, healer, weapon/armor/magic/general, innkeeper, registrar, scribe), each flagged **sentinel**
- **Ambient city mobs (citizens/guards/patrols not tied to core services):** around level **40** (target range 38-45)

## Safe-Room Policy (buildings)
- All interior buildings are flagged **safe**.
- This includes all shop/service interiors and civic utility buildings, specifically:
 - Postmaster, Quartermaster, Bank, General Shop, Weapon Shop, Armor Shop, Magic Shop
 - Healer, Inn, Cartographer/Scribe, and Guild Registrar
 - Temple and other enclosed civic service spaces
- Exterior streets/plazas are not automatically safe unless explicitly designated later for gameplay flow.

## Building Entrance Policy (dead-end interiors)

**All `inside`-sector rooms are dead-end rooms** with exactly **one bi-directional entrance/exit** back to an adjacent city street room. Each building connects to the city grid through a single cardinal exit. No other rooms have exits leading into the building — building walls block passage from all other directions.

This means buildings do NOT participate in grid traversal. Adjacent street rooms that would normally connect to a building's grid position have no exit in that direction. Players enter a building from the street, use its services, and leave the same way.

**Connectivity note:** With all 31 buildings as dead-ends, the outdoor street grid (68 rooms) carries all traversal. The north-south and east-west spines are interrupted by building footprints; players navigate via the perimeter wall avenues and connecting streets. Room 3375 was converted from inside (Judicial Archive) to city (Judicial Colonnade) to prevent rooms 3376 and 3386 from becoming an isolated pocket. The full perimeter (rows 0 and 9, columns 0 and 9) is uninterrupted city-sector rooms, allowing a continuous wall walk around the entire city.

### Building Entrance Table (all 31 inside rooms)

| Vnum | Building Name | Exit Dir | To Room | Street/Room Name |
|------|--------------|----------|---------|-----------------|
| **Row 1** | | | | |
| 3351 | Traveler's Registry | west | 3350 | Caravan Rest Yard |
| 3352 | Arrival Provisioner | east | 3353 | North Promenade, Upper |
| 3355 | Courier Dispatch Hall | west | 3354 | Central Arrival Plaza |
| 3357 | Scout Debriefing Post | west | 3356 | Wagonwright's Yard |
| **Row 2** | | | | |
| 3360 | Clothier's Row | east | 3361 | Foodmonger's Lane |
| 3366 | Sage's Alcove | west | 3365 | Apothecary Lane |
| **Row 3** | | | | |
| 3370 | Syndic Council Annex | west | 3369 | Western Wall Avenue, Mid-West |
| 3371 | Civic Registry Office | east | 3372 | West Cross Street, North |
| 3374 | Temple of Concord | west | 3373 | Temple Approach |
| 3377 | Arcane Studies Hall | west | 3376 | East Cross Street, North |
| **Row 4** | | | | |
| 3380 | Tinker's Workshop | east | 3381 | Market Arcade, West |
| 3382 | Steel and Edge Atelier | west | 3381 | Market Arcade, West |
| 3383 | Kiess Bank of Ledgers | north | 3373 | Temple Approach |
| 3384 | Grand Market Exchange | south | 3394 | The Central Prism |
| 3385 | Hall of Restoration | east | 3386 | East Arcane Walk |
| 3387 | Alchemist's Gallery | west | 3386 | East Arcane Walk |
| **Row 5** | | | | |
| 3392 | Bastion Armory | west | 3391 | Memorial Fountain Plaza |
| 3393 | Postmaster of Kiess | east | 3394 | The Central Prism |
| 3395 | Quartermaster's Depot | west | 3394 | The Central Prism |
| 3396 | Argent Arcana | east | 3397 | Boulevard of Syndics, East |
| **Row 6** | | | | |
| 3400 | Provision Warehouse | east | 3401 | Lamplighter's Lane |
| 3403 | Wayfarer's Inn | east | 3404 | South Promenade, Upper |
| 3405 | Cartographer and Scribe | west | 3404 | South Promenade, Upper |
| 3407 | Enchanter's Parlor | west | 3406 | East Commerce Lane |
| **Row 7** | | | | |
| 3410 | Patrol Barracks, West | east | 3411 | Drill Yard, West |
| 3417 | Patrol Barracks, East | west | 3416 | Signal Tower Base |
| **Row 8** | | | | |
| 3420 | Mercenary Board Hall | east | 3421 | Stablemaster's Compound |
| 3422 | Travel Administration Office | north | 3412 | Military Supply Road, West |
| 3424 | Guild Registrar Hall | north | 3414 | South Promenade, Mid-Lower |
| 3425 | Portal Warden's Office | north | 3415 | Military Supply Road, East |
| 3427 | Wall Command Headquarters | west | 3426 | Arena Observation Deck |


## City Wall and Perimeter Plan
- Kiess is fully enclosed by a continuous **city wall** around the outer rectangle (`x=0`, `x=9`, `y=0`, `y=9`).
- Wall-adjacent perimeter rooms represent battlements, gatehouses, and inner-wall avenues; they do not allow free exits to the wilderness except at designated gates.
- Only four controlled external links exist:
 - North Gate: `3344`
 - South Gate: `3434`
 - West Gate: `3389`
 - East Gate: `3398`
- Corner perimeter rooms (`3339`, `3348`, `3429`, `3438`) are fortified wall corners/towers with no direct wilderness traversal.
- Suggested room flags for wall/gatehouse rooms: defensive flavor flags as applicable, with gate doors reset closed for city security.

## Door Plan (explicit doors)
City streets remain open-grid except for controlled access points below. Building rooms are closed-off dead-ends (see Building Entrance Policy) and do not have doors — they simply have one exit.

1. **South Main Gate doors**
  - `3434 <-> 1159`
  - Door: south side, heavy gate, closed by reset, unlockable.

## Street Connectivity Rules
- All city-sector rooms connect cardinally to adjacent city-sector (or recall_set) rooms unless at the boundary edge.
- **All inside-sector rooms are dead-ends** (see Building Entrance Policy). They have exactly one exit and no other rooms connect into them except through that single entrance. Adjacent street rooms that border a building have no exit in the building's direction (building wall blocks passage).
- Boundary rooms are wall-line rooms: they connect inward, and connect outward only at the four designated gate exits.
- **Navigation pattern:** With 35 buildings as dead-ends, the outdoor street grid (64 rooms) forms an irregular network. The north-south and east-west spines are interrupted by building footprints in the central rows. Primary traversal uses:
 - **Perimeter wall avenues** (x=0 and x=9 columns): fully connected north-south corridors along both walls
 - **Row 7 band** (y=7, vnums 3409-3418): nearly complete east-west corridor through the military ring
 - **Row 2 band** (y=2, vnums 3359-3368): partial east-west corridor through the scholastic district
 - Buildings are accessed by stepping off these corridors into dead-end interiors
- Some street rooms become dead-end pockets (e.g., 3381 connects only south to 3391; 3356 connects only north to 3346). This is intentional — they serve as quiet alcoves adjacent to building entrances.

## Room Assignment Plan (all 100 rooms, detailed)

Sector types: `city` (1) for outdoor streets/plazas, `inside` (11) for enclosed buildings/interiors, `recall_set` (8) for the recall room only.

### Row 0: North Wall & Gate (y=0, vnums 3339-3348)

| Vnum | Room Name | Sector | Flags | Notes |
|------|-----------|--------|-------|-------|
| 3339 | Northwest Watchtower | city | 0 | Corner fortification; The Watcher of Storms statue; beacon brazier |
| 3340 | North Wall Walk, West End | city | 0 | Battlement walkway along north wall |
| 3341 | North Wall Walk, West Approach | city | 0 | Wall walk nearing the gate |
| 3342 | North Gate Customs Yard | city | 0 | Open-air screening area for arriving caravans; part of continuous wall walk |
| 3343 | North Gate Staging Yard | city | 0 | Wagon marshalling area outside the gate proper |
| 3344 | The North Gate of Kiess | city | 0 | Primary north entry; Warden's Arch monument; external link north |
| 3345 | North Gate Guard Platform | city | 0 | Open guard platform and patrol mustering area; part of continuous wall walk |
| 3346 | North Wall Walk, East Approach | city | 0 | Wall walk east of the gate |
| 3347 | North Wall Walk, East End | city | 0 | Battlement walkway continuing east |
| 3348 | Northeast Watchtower | city | 0 | Corner fortification; The Watcher of Daybreak statue; beacon brazier |

### Row 1: Caravan Ring & Arrival Services (y=1, vnums 3349-3358)

| Vnum | Room Name | Sector | Flags | Notes |
|------|-----------|--------|-------|-------|
| 3349 | Western Wall Avenue, North | city | 0 | Inner avenue along west wall |
| 3350 | Caravan Rest Yard | city | 0 | Open-air resting area for arriving traders |
| 3351 | Traveler's Registry | inside | safe | Sign-in and public notices; dead-end, entrance west from 3350 |
| 3352 | Arrival Provisioner | inside | safe | Basic supplies for new arrivals; dead-end, entrance east from 3353 |
| 3353 | North Promenade, Upper | city | 0 | North-south spine road, north end |
| 3354 | Central Arrival Plaza | city | 0 | Wide plaza just south of the gate |
| 3355 | Courier Dispatch Hall | inside | safe | Message runners and dispatch coordination; dead-end, entrance west from 3354 |
| 3356 | Wagonwright's Yard | city | 0 | Wagon repair and livery stable |
| 3357 | Scout Debriefing Post | inside | safe | Forest scouts report here; dead-end, entrance west from 3356 |
| 3358 | Eastern Wall Avenue, North | city | 0 | Inner avenue along east wall |

### Row 2: North Commerce & Scholastic Streets (y=2, vnums 3359-3368)

| Vnum | Room Name | Sector | Flags | Notes |
|------|-----------|--------|-------|-------|
| 3359 | Western Wall Avenue, Upper Market | city | 0 | West wall avenue, market district begins |
| 3360 | Clothier's Row | inside | safe | Tailoring and garment services; dead-end, entrance east from 3361 |
| 3361 | Foodmonger's Lane | city | 0 | Open-air food stalls and bakeries |
| 3362 | Commerce Crossing, North | city | 0 | Intersection of market lanes |
| 3363 | North Promenade, Mid-Upper | city | 0 | North-south spine continuing south |
| 3364 | Civic Notice Square | city | 0 | Public announcement boards and waystones |
| 3365 | Apothecary Lane | city | 0 | Herbalists and potion-makers |
| 3366 | Sage's Alcove | inside | safe | Lore consultation and historical archives; dead-end, entrance west from 3365 |
| 3367 | Scholastic Colonnade | city | 0 | Covered walkway with memorial carvings |
| 3368 | Eastern Wall Avenue, Upper Scholastic | city | 0 | East wall avenue, scholastic district begins |

### Row 3: Temples & Civic Offices (y=3, vnums 3369-3378)

| Vnum | Room Name | Sector | Flags | Notes |
|------|-----------|--------|-------|-------|
| 3369 | Western Wall Avenue, Mid-West | city | 0 | West wall avenue continuing south |
| 3370 | Syndic Council Annex | inside | safe | Trade syndic administrative offices; dead-end, entrance west from 3369 |
| 3371 | Civic Registry Office | inside | safe | Birth, death, property records; dead-end, entrance east from 3372 |
| 3372 | West Cross Street, North | city | 0 | NW quadrant loop road |
| 3373 | Temple Approach | city | 0 | Processional avenue to the temple |
| 3374 | Temple of Concord | inside | safe | Compact Flame shrine; donation/recall support; dead-end, entrance west from 3373 |
| 3375 | Judicial Colonnade | city | 0 | Open-air covered walkway with arbitration tablets; connects 3376 to 3365 (prevents isolation) |
| 3376 | East Cross Street, North | city | 0 | NE quadrant loop road |
| 3377 | Arcane Studies Hall | inside | safe | Magical research and training; dead-end, entrance west from 3376 |
| 3378 | Eastern Wall Avenue, Mid-East | city | 0 | East wall avenue continuing south |

### Row 4: Core Trade Band (y=4, vnums 3379-3388)

| Vnum | Room Name | Sector | Flags | Notes |
|------|-----------|--------|-------|-------|
| 3379 | Western Wall Avenue, Trade Quarter | city | 0 | West wall avenue at trade band level |
| 3380 | Tinker's Workshop | inside | safe | Repair and crafting services; dead-end, entrance east from 3381 |
| 3381 | Market Arcade, West | city | 0 | Covered market walkway |
| 3382 | Steel and Edge Atelier | inside | safe | **Weapon shop**; dead-end, entrance west from 3381 |
| 3383 | Kiess Bank of Ledgers | inside | safe | **Banker**; dead-end, entrance north from 3373 |
| 3384 | Grand Market Exchange | inside | safe | **General shop**; dead-end, entrance south from 3394 |
| 3385 | Hall of Restoration | inside | safe | **Healer**; dead-end, entrance east from 3386 |
| 3386 | East Arcane Walk | city | 0 | Street connecting east cross street to arcane trade band |
| 3387 | Alchemist's Gallery | inside | safe | Potion identification and alchemical supplies; dead-end, entrance west from 3386 |
| 3388 | Eastern Wall Avenue, Trade Quarter | city | 0 | East wall avenue at trade band level |

### Row 5: Main Civic Boulevard (y=5, vnums 3389-3398)

| Vnum | Room Name | Sector | Flags | Notes |
|------|-----------|--------|-------|-------|
| 3389 | The West Gate of Kiess | city | 0 | West entry; Gatekeeper Sentinel statue; external link west |
| 3390 | Boulevard of Syndics, West | city | 0 | East-west spine road, west segment |
| 3391 | Memorial Fountain Plaza | city | 0 | Public fountain with carved Evermeet district names |
| 3392 | Bastion Armory | inside | safe | **Armor shop**; dead-end, entrance west from 3391 |
| 3393 | Postmaster of Kiess | inside | safe | **Postmaster**; mail services; dead-end, entrance east from 3394 |
| 3394 | The Central Prism | recall_set | safe | **Recall room**; Executioner stationed; Prism of Continuance |
| 3395 | Quartermaster's Depot | inside | safe | **Quartermaster**; supplies and logistical gear; dead-end, entrance west from 3394 |
| 3396 | Argent Arcana | inside | safe | **Magic shop**; dead-end, entrance east from 3397 |
| 3397 | Boulevard of Syndics, East | city | 0 | East-west spine road, east segment |
| 3398 | The East Gate of Kiess | city | 0 | East entry; Gatekeeper Sentinel statue; external link east |

### Row 6: Mid-South Commerce & Inn Corridor (y=6, vnums 3399-3408)

| Vnum | Room Name | Sector | Flags | Notes |
|------|-----------|--------|-------|-------|
| 3399 | Western Wall Avenue, Lower Market | city | 0 | West wall avenue, south of trade band |
| 3400 | Provision Warehouse | inside | safe | Bulk goods storage; dead-end, entrance east from 3401 |
| 3401 | Lamplighter's Lane | city | 0 | Street maintenance and lamp services |
| 3402 | South Market Crossing | city | 0 | SW quadrant loop intersection |
| 3403 | Wayfarer's Inn | inside | safe | **Innkeeper**; rent services; dead-end, entrance east from 3404 |
| 3404 | South Promenade, Upper | city | 0 | North-south spine, south of center |
| 3405 | Cartographer and Scribe | inside | safe | **Maps/utility items**; dead-end, entrance west from 3404; Atlas Pedestal monument |
| 3406 | East Commerce Lane | city | 0 | SE quadrant loop road |
| 3407 | Enchanter's Parlor | inside | safe | Enchantment and identification services; dead-end, entrance west from 3406 |
| 3408 | Eastern Wall Avenue, Lower Scholastic | city | 0 | East wall avenue south of trade band |

### Row 7: Military Support Ring (y=7, vnums 3409-3418)

| Vnum | Room Name | Sector | Flags | Notes |
|------|-----------|--------|-------|-------|
| 3409 | Western Wall Avenue, Lower | city | 0 | West wall avenue nearing south ring |
| 3410 | Patrol Barracks, West | inside | safe | Guard quarters and armory; dead-end, entrance east from 3411 |
| 3411 | Drill Yard, West | city | 0 | Open training space |
| 3412 | Military Supply Road, West | city | 0 | SW quadrant loop road continuing |
| 3413 | Logistics Crossing | city | 0 | Intersection of supply routes |
| 3414 | South Promenade, Mid-Lower | city | 0 | North-south spine continuing south |
| 3415 | Military Supply Road, East | city | 0 | SE quadrant loop road continuing |
| 3416 | Signal Tower Base | city | 0 | Base of beacon communication tower |
| 3417 | Patrol Barracks, East | inside | safe | Eastern guard quarters; dead-end, entrance west from 3416 |
| 3418 | Eastern Wall Avenue, Lower | city | 0 | East wall avenue nearing south ring |

### Row 8: Southern Guild Annexes & Travel Admin (y=8, vnums 3419-3428)

| Vnum | Room Name | Sector | Flags | Notes |
|------|-----------|--------|-------|-------|
| 3419 | Western Wall Avenue, South | city | 0 | West wall avenue, south segment |
| 3420 | Mercenary Board Hall | inside | safe | Job postings and bounty contracts; dead-end, entrance east from 3421 |
| 3421 | Stablemaster's Compound | city | 0 | Mount stabling and travel prep |
| 3422 | Travel Administration Office | inside | safe | Permits, passes, and caravan scheduling; dead-end, entrance north from 3412 |
| 3423 | South Promenade, Lower | city | 0 | North-south spine, approaching south gate |
| 3424 | Guild Registrar Hall | inside | safe | **Class guidance/training hooks**; dead-end, entrance north from 3414 |
| 3425 | Portal Warden's Office | inside | safe | Teleportation and portal administration; dead-end, entrance north from 3415 |
| 3426 | Arena Observation Deck | city | 0 | Overlook for training yard matches |
| 3427 | Wall Command Headquarters | inside | safe | Military leadership offices; dead-end, entrance west from 3426 |
| 3428 | Eastern Wall Avenue, South | city | 0 | East wall avenue, south segment |

### Row 9: South Wall & Gate (y=9, vnums 3429-3438)

| Vnum | Room Name | Sector | Flags | Notes |
|------|-----------|--------|-------|-------|
| 3429 | Southwest Watchtower | city | 0 | Corner fortification; The Watcher of Storms statue |
| 3430 | South Wall Walk, West End | city | 0 | Battlement walkway along south wall |
| 3431 | South Wall Walk, West Approach | city | 0 | Wall walk nearing the south gate |
| 3432 | South Gate Inspection Yard | city | 0 | Open-air outbound cargo inspection area; part of continuous wall walk |
| 3433 | South Gate Mustering Yard | city | 0 | Assembly area for outbound expeditions |
| 3434 | The South Gate of Kiess | city | 0 | Primary south entry; Pillar of Returning Caravans; external link south |
| 3435 | South Gate Supply Yard | city | 0 | Open-air outbound supply distribution area; part of continuous wall walk |
| 3436 | South Wall Walk, East Approach | city | 0 | Wall walk east of the south gate |
| 3437 | South Wall Walk, East End | city | 0 | Battlement walkway continuing east |
| 3438 | Southeast Watchtower | city | 0 | Corner fortification; The Watcher of Dusk statue |

### Room Flag Summary

- **`recall_set` sector (8):** 3394 only (Central Prism); room_flags = `315` (ROOM_SAFE)
- **`inside` sector (11), `safe` rooms (31 total):** All enclosed buildings — room_flags = `315` (ROOM_SAFE). All are dead-end rooms with one exit (see Building Entrance Policy).
- **`city` sector (1), unflagged streets (68 total):** All exterior roads, plazas, wall walks, gate approaches — room_flags = `0`. The full perimeter (rows 0/9, columns 0/9) is uninterrupted city-sector rooms.
- **No `dark` (1), `no_mob` (4), or `no_magic` (16) flags** are planned for any Kiess room (city hub should be fully accessible and lit)

**Area file format reference:** In the `#ROOMS` section, each room's header line is `<room_flags> <sector_type>`. For example, the Central Prism would be `315 8` (safe + recall_set), an inside shop would be `315 11` (safe + inside), and a city street would be `0 1` (no flags + city).


## Shop Inventory Plan (thematic items for sale)
All listed shop items are planned at levels **75-125**, and every level is a **multiple of 5**.

### 3384 - Grand Market Exchange (General Shop, 18 items)
1. Prism-Waxed Travel Pack (lvl 75)
2. Evermeet Memorial Bedroll (lvl 80)
3. Kiess Route Lantern (lvl 85)
4. Compact Iron Rations (lvl 75)
5. Surveyor's Chalk Bundle (lvl 90)
6. Reinforced Waterskin of the Ringroad (lvl 80)
7. Tower-Knot Rope Coil (lvl 85)
8. Courier's Seal Satchel (lvl 95)
9. Glass-Capped Oil Flask (lvl 90)
10. Gatewarden's Tinder Kit (lvl 75)
11. Wallrunner Climbing Hook (lvl 100)
12. Prismatic Signal Whistle (lvl 95)
13. Frontier Trail Spikes (lvl 85)
14. Ledgerbound Storage Pouch (lvl 105)
15. Citywatch Field Blanket (lvl 80)
16. Brass Compass of Kiess (lvl 110)
17. Forest Edge Insect Veil (lvl 100)
18. Prism-Crest Travel Cloak (lvl 115)

### 3382 - Steel and Edge Atelier (Weapon Shop, 16 items)
1. Ringroad Militia Shortsword (lvl 75)
2. Confusion-Ward Hatchet (lvl 80)
3. Prism-Tempered Longsword (lvl 95)
4. Gatehouse Pike (lvl 90)
5. Syndic Guard Mace (lvl 85)
6. Watchtower Warhammer (lvl 100)
7. Forest Marshal Spear (lvl 90)
8. Beaconsteel Falchion (lvl 105)
9. Compact Tribunal Dagger (lvl 80)
10. Kiess Recurve Longbow (lvl 95)
11. Wall Command Halberd (lvl 110)
12. Cartographer's Defense Knife (lvl 75)
13. Prismline Scimitar (lvl 100)
14. Frontier Interdiction Axe (lvl 115)
15. Concord Judicator Blade (lvl 120)
16. Evermeet Legacy Greatsword (lvl 125)

### 3392 - Bastion Armory (Armor Shop, 16 items)
1. Ringroad Padded Vest (lvl 75)
2. Gatewarden Chain Shirt (lvl 85)
3. Prism-Riveted Breastplate (lvl 95)
4. Wallline Iron Greaves (lvl 90)
5. Concord Bracer Set (lvl 80)
6. Syndic Scale Coat (lvl 100)
7. Beacon Tower Helm (lvl 105)
8. Forest Patrol Leather Boots (lvl 85)
9. Memorial Guard Cloak (lvl 95)
10. Quartermaster Buckler (lvl 90)
11. Kiess Plate Gauntlets (lvl 110)
12. Arcane Lattice Mantle (lvl 115)
13. Prismstone Gorget (lvl 100)
14. Watch Captain Tower Shield (lvl 120)
15. Compact Oathplate Cuirass (lvl 125)
16. Stalwart Wallmail Skirt (lvl 80)

### 3396 - Argent Arcana (Magic Shop, 17 items)
1. Prismatic Focus Wand (lvl 75)
2. Forest-Edge Ward Scroll (lvl 80)
3. Concord Sigil Charm (lvl 85)
4. Lattice Mana Ring (lvl 90)
5. Prism Veil Talisman (lvl 95)
6. Cartographer's Scrying Lens (lvl 100)
7. Gate Beacon Rune Stone (lvl 105)
8. Evermeet Echo Orb (lvl 110)
9. Compact Aegis Amulet (lvl 115)
10. Quartermaster Recall Token (lvl 90)
11. Towerlight Spell Candle (lvl 80)
12. Syndic Binding Thread (lvl 85)
13. Confusion-Breaker Charm (lvl 120)
14. Prismline Ritual Ink (lvl 95)
15. Warden's Thought Crystal (lvl 100)
16. Triune Compact Reliquary (lvl 125)
17. Civic Nullfire Vial (lvl 105)

## Monuments, Statues, and Flavor Objects Plan
- **3394 - Central Prism Plaza:**
 - Monument: **The Prism of Continuance** (city-defining relic monument tied to Evermeet's legacy)
 - Flavor objects: prismatic waystones, civic decree obelisk, memorial braziers
- **3344 - North Gate Approach:**
 - Monument: **Warden's Arch of Confusion Watch** (honors forest scouts and patrol losses)
 - Flavor objects: route marker stelae pointing toward the Forest of Confusion, supply manifests on iron lecterns
- **3434 - South Gate Approach:**
 - Monument: **Pillar of Returning Caravans** (marks safe return and trade continuity)
 - Flavor objects: carved caravan tally board, bronze wheel reliefs
- **3389 / 3398 - West/East Gatehouses:**
 - Statues: paired **Gatekeeper Sentinels** (stone guardians facing outward)
 - Flavor objects: gate chains, warning gongs, heraldic shields
- **3374 - Temple of Concord:**
 - Monument: **Shrine of the Compact Flame** (symbolizes guild-temple civic pact)
 - Flavor objects: votive racks, concord tablets, ceremonial font
- **3383 - Bank of Ledgers:**
 - Flavor objects: gilded ledger pedestals, oath-chest, seal press altar, iron strongbox display
- **3393 - Postmaster of Kiess:**
 - Flavor objects: dispatch board, courier satchel wall, route clock
- **3395 - Quartermaster's Depot:**
 - Flavor objects: campaign maps, requisition crates, stamped supply racks
- **3403 - Wayfarer's Inn:**
 - Flavor objects: painted city map mural, traveler memorial plaque, brass lantern cluster
- **3405 - Cartographer and Scribe:**
 - Monument object: **Atlas Pedestal of New Kiess** (updated map showing Evermeet-to-Kiess transition)
 - Flavor objects: survey tripods, rolled charts, inkstone cabinet
- **Perimeter wall towers (3339, 3348, 3429, 3438):**
 - Statues: **The Four Watchers** (named tower sentinels representing North/South/East/West vigilance)
 - Flavor objects: beacon braziers, weatherworn battle plaques

### Flavor Object Implementation Notes
- Place major monuments in high-traffic civic nodes to reinforce the Evermeet succession narrative.
- Keep monument names lore-facing and persistent; avoid novelty props that conflict with the fortified tone.
- Interior service-building props should remain non-blocking and compatible with safe-room policy.
- Gate and wall flavor objects should visually reinforce controlled entry through the four designated gates.

## NPC/Service Priority Notes
- Executioner is fixed at `3394`, level 150, flagged **executioner**, **sentinel**, and **boss**, and never allowed to move.
- Postmaster and Quartermaster are fixed-position level-150 utility NPCs (`3393`, `3395`) and flagged **sentinel**.
- Shopkeepers and all utility NPCs are non-wandering, level 150, and flagged **sentinel**; all core economy functions are within two steps of recall.
- Additional ambient mobs (citizens/watch) are set around level 40 for city life population.

---

## Mobile Definitions

### Service NPCs (level 150, all flagged `is_npc|sentinel`)

All service NPCs are level 150 and fixed in place. Sex assignments alternate for variety.

**Act flag reference values:** `is_npc`=1, `sentinel`=2, `boss`=67108864, `no_flee`=16, `bank`=32768, `heal`=4096, `postman`=524288, `practice`=315, `stay_area`=64. Combine by addition (e.g., `is_npc|sentinel` = 3).

| Vnum | Name | Level | Room | Act Flags | Service Role | Notes |
|------|------|-------|------|-----------|-------------|-------|
| 3339 | the Executioner of Kiess | 150 | 3394 | is_npc, sentinel, boss, no_flee | Executioner | `spec_executioner`; city law enforcement |
| 3340 | the Postmaster of Kiess | 150 | 3393 | is_npc, sentinel, postman | Postmaster | Mail services |
| 3341 | the Quartermaster of Kiess | 150 | 3395 | is_npc, sentinel | Quartermaster | Supplies and gear |
| 3342 | a Grand Market merchant | 150 | 3384 | is_npc, sentinel | General shop | Shop: buys/sells general goods |
| 3343 | a Steel and Edge weaponsmith | 150 | 3382 | is_npc, sentinel | Weapon shop | Shop: buys/sells weapons |
| 3344 | a Bastion armorer | 150 | 3392 | is_npc, sentinel | Armor shop | Shop: buys/sells armor |
| 3345 | an Argent Arcana mage-merchant | 150 | 3396 | is_npc, sentinel | Magic shop | Shop: buys/sells magical items |
| 3346 | the Banker of Kiess | 150 | 3383 | is_npc, sentinel, bank | Banker | Banking services |
| 3347 | the Healer of Kiess | 150 | 3385 | is_npc, sentinel, heal | Healer | Healing services |
| 3348 | the Innkeeper of Kiess | 150 | 3403 | is_npc, sentinel | Innkeeper | Rent/save services |
| 3349 | a Cartographer of Kiess | 150 | 3405 | is_npc, sentinel | Scribe/map shop | Shop: maps and utility items |
| 3350 | the Guild Registrar of Kiess | 150 | 3424 | is_npc, sentinel, practice | Guild registrar | Class guidance and training |
| 3351 | a Temple Concord priest | 150 | 3374 | is_npc, sentinel | Temple keeper | Donation and recall support |

### Ambient City Mobs (level 38-45, all flagged `is_npc|stay_area`)

These populate the streets and give the city life. None are aggressive; they are background flavor. All use the melee profile for skill assignment (since they are non-hostile city NPCs at low level, they will have minimal combat skills).

| Vnum | Name | Level | Act Flags | Notes |
|------|------|-------|-----------|-------|
| 3352 | a Kiess citizen | 40 | is_npc, stay_area | Generic townsperson; roams streets |
| 3353 | a Kiess guardsman | 45 | is_npc, sentinel | Stationary patrol guard |
| 3354 | a Wall Command sentry | 42 | is_npc, sentinel | Wall and gate sentry |
| 3355 | a Kiess street sweeper | 38 | is_npc, stay_area | Civic janitor; `spec_janitor` |
| 3356 | a Syndic trade courier | 40 | is_npc, stay_area | Running messages between shops |
| 3357 | a Temple Concord acolyte | 39 | is_npc, stay_area | Junior temple attendant |
| 3358 | a caravan drover | 41 | is_npc, stay_area | Wagon handler in north ring |
| 3359 | a Kiess lamplighter | 38 | is_npc, stay_area | Maintains city lamps |
| 3360 | a Forest Confusion scout | 43 | is_npc, stay_area | Returning from forest patrol |
| 3361 | a Wall Command sergeant | 44 | is_npc, sentinel | Senior guard; stationed in military ring |
| 3362 | a Prism Square orator | 40 | is_npc, stay_area | Public speaker near recall |
| 3363 | a Kiess merchant's apprentice | 38 | is_npc, stay_area | Errand runner for shops |
| 3364 | a Kiess stablehand | 39 | is_npc, stay_area | Works at the stables |

**Mob combat extensions (`!` line):**
- Service NPCs (level 150): no combat extensions needed (non-combatant service roles); Executioner uses `spec_executioner` for combat behavior
- Ambient mobs (level 38-45): minimal combat; 2 attacks, punch, kick, dodge at most (per melee profile floors)

**Mob elemental extensions (`|` line):**
- No elemental extensions planned for city NPCs (neutral city environment, no elemental theme)

**Mob implementation notes (to be resolved during area file construction):**
- **Sex:** Alternate male/female across service NPCs for variety. Ambient mobs should also vary.
- **Alignment:** Service NPCs should be good-aligned (positive alignment, ~228). Ambient citizens neutral (~0), guards mildly good (~350).
- **Race/Class:** Use `human` race and `melee` class for all city NPCs unless a specific role warrants otherwise (e.g., Temple priest could be `cleric` class, Arcane Studies mobs could be `mage` class).
- **Executioner equipment:** The Executioner (mob 3339) engages in combat via `spec_executioner`. It should have equipment resets (E commands) for weapon and armor to be effective at level 150. Define appropriate weapon/armor objects within the 3407-3438 object vnum range (currently unallocated) and add E resets after the Executioner's M reset.
- **Short/long descriptions:** Each mob needs `short_descr` (used in combat/action messages, e.g., "the Executioner of Kiess") and `long_descr` (shown in room, e.g., "The Executioner of Kiess stands here, watching for lawbreakers."). These should be written during implementation per area file spec conventions.

---

## Shops Plan

| Keeper Vnum | Room | Buy Types | Profit Buy | Profit Sell | Open | Close | Notes |
|-------------|------|-----------|-----------|------------|------|-------|-------|
| 3342 | 3384 | 0 0 0 0 0 | 120 | 80 | 0 | 23 | General shop — buys anything (all buy_type slots 0) |
| 3343 | 3382 | 5 0 0 0 0 | 120 | 80 | 0 | 23 | Weapon shop — buys weapons (ITEM_WEAPON=5) |
| 3344 | 3392 | 9 0 0 0 0 | 120 | 80 | 0 | 23 | Armor shop — buys armor (ITEM_ARMOR=9) |
| 3345 | 3396 | 2 4 10 0 0 | 130 | 70 | 0 | 23 | Magic shop — buys scrolls (2), staves (4), potions (10) |
| 3349 | 3405 | 0 0 0 0 0 | 110 | 90 | 0 | 23 | Scribe/maps — buys anything (all buy_type slots 0) |

Note: The "Room" column is for reference only — it is not part of the `#SHOPS` file format. The shop format line is: `<keeper_vnum> <buy_type[0..4]> <profit_buy> <profit_sell> <open_hour> <close_hour>`. The keeper mob is placed in the room via `M` resets in `#RESETS`.

Notes:
- All shops open 24 hours (hour 0 to hour 23)
- Profit buy/sell values are percentage-based: 120/80 means 120% markup on purchases, 80% payout on sales (standard city rates)
- Magic shop uses slightly higher buy markup (130%) and lower sell payout (70%) reflecting arcane rarity

---

## Resets Plan

### Mob Resets (`M` commands)

**Service NPCs (one per room, limit 1):**

| Command | Mob Vnum | Limit | Room Vnum | Notes |
|---------|----------|-------|-----------|-------|
| M 0 | 3339 | 1 | 3394 | Executioner at Central Prism |
| M 0 | 3340 | 1 | 3393 | Postmaster |
| M 0 | 3341 | 1 | 3395 | Quartermaster |
| M 0 | 3342 | 1 | 3384 | General shop merchant |
| M 0 | 3343 | 1 | 3382 | Weapon shop |
| M 0 | 3344 | 1 | 3392 | Armor shop |
| M 0 | 3345 | 1 | 3396 | Magic shop |
| M 0 | 3346 | 1 | 3383 | Banker |
| M 0 | 3347 | 1 | 3385 | Healer |
| M 0 | 3348 | 1 | 3403 | Innkeeper |
| M 0 | 3349 | 1 | 3405 | Cartographer/Scribe |
| M 0 | 3350 | 1 | 3424 | Guild Registrar |
| M 0 | 3351 | 1 | 3374 | Temple priest |

**Ambient City Mobs (~25-30 total spawns across city streets):**

| Command | Mob Vnum | Limit | Room Vnum | Notes |
|---------|----------|-------|-----------|-------|
| M 0 | 3352 | 6 | 3354 | Citizen in arrival plaza |
| M 0 | 3352 | 6 | 3361 | Citizen in foodmonger's lane |
| M 0 | 3352 | 6 | 3391 | Citizen at memorial fountain |
| M 0 | 3352 | 6 | 3404 | Citizen on south promenade |
| M 0 | 3352 | 6 | 3381 | Citizen in market arcade |
| M 0 | 3352 | 6 | 3413 | Citizen at logistics crossing |
| M 0 | 3353 | 4 | 3344 | Guard at north gate |
| M 0 | 3353 | 4 | 3434 | Guard at south gate |
| M 0 | 3353 | 4 | 3394 | Guard at Central Prism |
| M 0 | 3353 | 4 | 3423 | Guard on south promenade |
| M 0 | 3354 | 3 | 3339 | Sentry at NW tower |
| M 0 | 3354 | 3 | 3348 | Sentry at NE tower |
| M 0 | 3354 | 3 | 3389 | Sentry at west gate |
| M 0 | 3354 | 3 | 3398 | Sentry at east gate |
| M 0 | 3354 | 3 | 3429 | Sentry at SW tower |
| M 0 | 3354 | 3 | 3438 | Sentry at SE tower |
| M 0 | 3355 | 2 | 3401 | Street sweeper on Lamplighter's Lane |
| M 0 | 3355 | 2 | 3364 | Street sweeper at notice square |
| M 0 | 3356 | 2 | 3390 | Trade courier on boulevard |
| M 0 | 3357 | 2 | 3373 | Acolyte near temple |
| M 0 | 3358 | 2 | 3350 | Drover in caravan rest yard |
| M 0 | 3359 | 2 | 3401 | Lamplighter on their lane |
| M 0 | 3360 | 2 | 3356 | Scout near debriefing post (Wagonwright's Yard) |
| M 0 | 3361 | 2 | 3414 | Sergeant on south promenade |
| M 0 | 3362 | 2 | 3394 | Orator at prism square |
| M 0 | 3363 | 2 | 3381 | Apprentice in market arcade |
| M 0 | 3364 | 2 | 3421 | Stablehand at compound |

### Door Resets (`D` commands)

| Command | Room Vnum | Door | State | Notes |
|---------|-----------|------|-------|-------|
| D 0 | 3434 | 2 | 1 | South Gate — closed on reset |

### Object Resets (`O` commands)

No standalone room object spawns planned initially. Shop inventory is defined through the `#SHOPS` section and shop object definitions. Flavor objects (monuments, braziers, etc.) will be implemented as room extra descriptions rather than spawned objects, keeping the area file simpler and avoiding unnecessary object management.

---

## Object Definitions (Non-Shop)

No non-shop objects are currently defined. Object vnum 3339 is available.

### Shop Object Vnum Assignments

Shop inventory items use vnums 3340-3406 (67 items total across 4 shops). Each shop's items are grouped sequentially:

- **3340-3357:** Grand Market Exchange items (18 general goods)
- **3358-3373:** Steel and Edge Atelier items (16 weapons)
- **3374-3389:** Bastion Armory items (16 armor pieces)
- **3390-3406:** Argent Arcana items (17 magical items)

Full item definitions (item_type, extra_flags, wear_flags, item_apply, value fields, weight) to be determined during implementation per the area file spec. All shop items follow the level and weight conventions specified in the Shop Inventory Plan above.

### Reserved Object Vnums

- **3339, 3407-3438:** Unallocated (33 vnums). Reserved for Executioner equipment (weapon, armor), quest items, or other non-shop objects added during implementation.

---

## Specials Plan

| Mob Vnum | Spec Function | Reason |
|----------|--------------|--------|
| 3339 | spec_executioner | Executioner — city law enforcement; attacks criminals |
| 3355 | spec_janitor | Street sweeper — picks up litter/dropped items |

Notes:
- Most service NPCs (shopkeepers, banker, healer, postmaster) do not need spec functions; their behavior is driven by their `act` flags (`bank`, `heal`, `postman`) and `#SHOPS` entries.
- Guards and sentries do not use `spec_guard`/`spec_policeman` in this plan to avoid unintended aggression toward players with criminal flags arriving at a recall hub. This may be revisited after playtesting.

---

## Connection Plan (External Links)

Only the east gate has an active external connection (Roc Road to Midgaard). The north, west, and south gates are internal city boundaries with no external exits defined at this time.

### East Gate (3434)
- **Direction:** south from 3434 → vnum `1159`; north from 1159 → vnum `3434`
- **Door:** `EX_ISDOOR`, reset closed (state 1), unlockable
- **Rationale:** Outbound military and trade expeditions depart east toward Midgaard via Roc Road. Bidirectional link ensures players can return through the gate. Note: the in-game exit direction (south/north) is a legacy of the area's grid layout and does not change the canonical lore direction (east toward Midgaard).
- **Lore direction:** Kiess is west of Midgaard. Roc Road runs east from Kiess to Midgaard. The gate facing the Roc Road is canonically the "East Gate" in lore, regardless of the in-game exit direction used for the room connection.

### Connection Implementation Notes
- The east gate uses `EX_ISDOOR` with a `D` reset to state `closed` (1), not `locked` (2), so players can freely open it.
- The reverse exit (north from `1159` back to `3434`) must be added to the area file that owns vnum `1159`.
- North (`3344`), west (`3389`), and south (`3398`) gates remain walled city exits with no external destination. They may be connected to future areas later.

---

## Implementation Checklist

1. [ ] Create `area/kiess.are` with `#AREA` header and `#$`
2. [ ] Write `#ROOMS` section (100 rooms, vnums 3339-3438, all used)
3. [ ] Write `#MOBILES` section (13 service NPCs + 13 ambient mobs = 26 mobs, vnums 3339-3364)
4. [ ] Write `#OBJECTS` section (67 shop items, vnums 3340-3406; plus Executioner equipment from reserved range)
5. [ ] Write `#SHOPS` section (5 shops: general, weapon, armor, magic, scribe)
6. [ ] Write `#RESETS` section (~13 service mob spawns + ~25 ambient mob spawns + 1 door reset)
7. [ ] Write `#SPECIALS` section (2 entries: executioner, janitor)
8. [ ] Add `kiess.are` to `area/area.lst`
9. [ ] Add north exit from vnum `1159` back to `3434` in `area/rocroad.are` (which owns vnum 1159, "Roc Road")
10. [ ] Run `cd src && make unit-tests` to validate
11. [ ] Verify all vnums within 3339-3438 range
12. [ ] Verify no vnum conflicts with existing areas (confirmed: minokeep uses 1300-1399, no overlap)
