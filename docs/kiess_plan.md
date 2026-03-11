# Area Plan: Kiess

## Overview
- **Area Name:** `@@W@@BKiess@@N`
- **File Name:** `kiess.are`
- **Keyword:** `kiess`
- **Intended Levels:** 1-170 (all-player-level city hub)
- **Vnum Range:** `13000-13099`
- **Vnum Count:** 100 (all vnums used)
- **Shape:** 10x10 rectangle (full grid used)
- **Recall Room:** `13055` (city center)
- **Reset Rate:** 15

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
- `L @@W(@@a1 170@@W)@@N~` — white parens, light cyan level numbers, white close paren, reset
- `N 77` — confirmed unused across all loaded area files
- `I 1 170` — all-level city hub; controls mob-level gating and area matching logic
- `V 13000 13099` — full 100-vnum envelope; all rooms, mobs, and objects must stay within this range
- `X 0` — map offset; no offset needed for this area
- `F 15` — reset frequency in minutes
- `U @@Wthe bells of @@BKiess@@W ring across the avenues@@N~` — reset message; white text with blue city name, reset; no double-newlines, terminated with `~`
- `T` — teleport flag line (rest ignored by loader); presence enables teleport into the area
- `O`, `R`, `W` — owner and ACL directives omitted; set at implementation time by the builder


## Lore

### The Fall of Evermeet and the Founding of Kiess

Evermeet did not fall in a single night. Over generations, its sprawling districts grew inward-looking, its outer wards neglected. Monster pressure from the surrounding wildlands intensified each season while civic attention turned to comfort rather than defense. Districts became isolated as bridges and crossings deteriorated. Supply houses burned during successive crises with no coordinated response. The wealthy core survived longest, but the working quarters that kept Evermeet fed, armed, and connected to the frontier slowly hollowed out. Eventually the last defenders withdrew, and Evermeet was abandoned to the wildlands — its gilded parlors left to rot, its gates standing open to whatever wandered in.

**Kiess** rises where Evermeet once stood, built on its foundations along **Roc Road**, the ancient trade artery linking the western territories to Midgaard. The merchants, soldiers, and temple orders who had fled Evermeet's collapse returned not to restore the old city but to replace it entirely. Where Evermeet grew by accident and indulgence, Kiess was built by design. The crown-chartered guilds and the temple compact laid it out as a fortified civic grid: a walled rectangle with predictable streets, centralized services, and controlled gates. Locals describe the difference simply: "Evermeet remembered, but finally made defensible."

### The Central Prism

The city was planned around a luminous civic keystone known as the **Central Prism** (room `13055`), a continuity anchor recovered from the ruins of Evermeet's civic hall. The Prism is both memorial and practical center: public law, messaging, provisioning, and magical logistics are all positioned within steps of it, so travelers can recover, orient, and re-enter the frontier quickly. The Executioner stationed at the Prism is the visible instrument of the compact's uncompromising enforcement of peace inside city limits — disputes are mediated in the square, but violent breach of city law is answered immediately and publicly.

### Three-Power Governance

Founding charters divide Kiess governance among three factions held in deliberate tension:

- **The Compact Temples** maintain sanctuary law, civic rites, and the Prism's memorial functions. The Temple of Concord (room `13035`) is their seat, housing the Shrine of the Compact Flame that symbolizes the original founding pact.
- **The Trade Syndics** maintain market continuity, caravan contracts, and commerce infrastructure. Their administrative annex (room `13031`) coordinates the flow of goods along Roc Road from Midgaard and the frontier.
- **Wall Command** maintains gate discipline, patrol rotation, and the city's four watchtowers. Their headquarters (room `13088`) oversees the sentries, scouts, and military supply lines that keep the walls staffed.

No single faction dominates. The Executioner post at the Central Prism is the legal mechanism that binds all three to a common peace code: each faction holds a seat on the tribunal, and each can invoke the Executioner's authority against the others if the compact is breached.

### Roc Road and the Southern Approach

Kiess's primary connection to the wider world runs south through the **South Gate** (room `13095`) onto Roc Road. This ancient highway connects to Midgaard, making Kiess the western terminus for caravans, adventurers, and military expeditions traveling between the civilized heartlands and the frontier. The north, west, and east gates currently face walled borderlands awaiting future development.

### The Forest of Confusion

The **Forest of Confusion** lies within reach of Kiess via Roc Road, and the city serves as a staging base for expeditions into its disorienting depths. Scouts report at debriefing posts inside the north ring, healers process the wounded in central wards, and quartermaster supply trains cycle between the depot and forest-edge staging trails. Herbal reagents, rare timbers, and relic fragments still flow through forest routes into Kiess's markets, while the deeper reaches — the ancient Verdant Depths and the blighted Withered Depths beyond — remain sources of both wealth and persistent danger. Kiess survives by organizing that risk rather than ignoring it.

### Architecture and Atmosphere

Kiess is identified by white civic stone, blue-and-gold banners, and prismatic glasswork set into canal bridges and plaza arches. The aesthetic is deliberate: bright enough to signal renewal and civic pride, ordered enough to signal discipline. Every district reads as lived-in and guarded rather than ornamental.

Names of Evermeet's lost outer districts are carved into waystones along the main avenues. Annual processions move from the north gate to the Prism to mark the first reconstruction oath. Monuments throughout the city serve double duty as navigational landmarks for returning expeditions and as memorial points honoring what was lost. The atmosphere is not nostalgic grief but forward-looking resolve — a city that chose preparedness over sentimentality, and keeps watch because it remembers what happens when a city stops watching.

## Rectangular Room Layout (10x10)

Coordinates: west->east `x=0..9`, north->south `y=0..9`.
`vnum = 13000 + (y * 10) + x`

| y\\x | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 |
|---|---|---|---|---|---|---|---|---|---|---|
| 0 | 13000 | 13001 | 13002 | 13003 | 13004 | 13005 | 13006 | 13007 | 13008 | 13009 |
| 1 | 13010 | 13011 | 13012 | 13013 | 13014 | 13015 | 13016 | 13017 | 13018 | 13019 |
| 2 | 13020 | 13021 | 13022 | 13023 | 13024 | 13025 | 13026 | 13027 | 13028 | 13029 |
| 3 | 13030 | 13031 | 13032 | 13033 | 13034 | 13035 | 13036 | 13037 | 13038 | 13039 |
| 4 | 13040 | 13041 | 13042 | 13043 | 13044 | 13045 | 13046 | 13047 | 13048 | 13049 |
| 5 | 13050 | 13051 | 13052 | 13053 | 13054 | 13055 | 13056 | 13057 | 13058 | 13059 |
| 6 | 13060 | 13061 | 13062 | 13063 | 13064 | 13065 | 13066 | 13067 | 13068 | 13069 |
| 7 | 13070 | 13071 | 13072 | 13073 | 13074 | 13075 | 13076 | 13077 | 13078 | 13079 |
| 8 | 13080 | 13081 | 13082 | 13083 | 13084 | 13085 | 13086 | 13087 | 13088 | 13089 |
| 9 | 13090 | 13091 | 13092 | 13093 | 13094 | 13095 | 13096 | 13097 | 13098 | 13099 |

## District Layout (by rectangle)
- **North Gate & Caravan Ring (y=0-1):** arrivals, wagon spaces, and traveler services.
- **West Market Wards (x=0-2, y=2-7):** general stores, food, clothier, smithy, weapon shop.
- **East Arcane Wards (x=7-9, y=2-7):** magic shop, sage, healer, temple support, and research services.
- **Central Civic Cross (x=4-6, y=4-6):** recall square, justice, banking, communication.
- **South Military/Travel Ring (y=8-9):** quartermaster, training yards, ship/portal offices, outbound gates.

## Mandatory Service Rooms (exact placements)
- **13055: `@@WThe @@BCentral Prism@@N`** -- **Recall room**, city heart, **Executioner** stationed here.
- **13054: Postmaster of Kiess** -- mail services.
- **13056: Quartermaster's Depot** -- supplies and logistical gear.
- **13045: Grand Market Exchange** -- general shop.
- **13044: Kiess Bank of Ledgers** -- banker.
- **13046: Hall of Restoration** -- healer.
- **13043: Steel and Edge Atelier** -- weapon shop.
- **13053: Bastion Armory** -- armor shop.
- **13057: Argent Arcana** -- magic shop.
- **13064: Wayfarer's Inn** -- rent/innkeeper.
- **13066: Cartographer and Scribe** -- maps, utility items.
- **13035: Temple of Concord** -- donation/recall support.
- **13085: Guild Registrar Hall** -- class guidance/training hooks.

## NPC Level Targets
- **City authority and service NPCs:** level **150**, flagged **sentinel**
  - Executioner (`13055`): flagged **executioner**, **sentinel**, and **boss**; never allowed to move
  - Postmaster (`13054`)
  - Quartermaster (`13056`)
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

**Connectivity note:** With all 35 buildings as dead-ends, the outdoor street grid (64 rooms) carries all traversal. The north-south and east-west spines are interrupted by building footprints; players navigate via the perimeter wall avenues and connecting streets. Room 13036 was converted from inside (Judicial Archive) to city (Judicial Colonnade) to prevent rooms 13037 and 13047 from becoming an isolated pocket.

### Building Entrance Table (all 35 inside rooms)

| Vnum | Building Name | Exit Dir | To Room | Street/Room Name |
|------|--------------|----------|---------|-----------------|
| **Row 0** | | | | |
| 13003 | North Gate Customs Office | east | 13004 | North Gate Staging Yard |
| 13006 | North Gate Guard Post | west | 13005 | The North Gate of Kiess |
| **Row 1** | | | | |
| 13012 | Traveler's Registry | west | 13011 | Caravan Rest Yard |
| 13013 | Arrival Provisioner | east | 13014 | North Promenade, Upper |
| 13016 | Courier Dispatch Hall | west | 13015 | Central Arrival Plaza |
| 13018 | Scout Debriefing Post | west | 13017 | Wagonwright's Yard |
| **Row 2** | | | | |
| 13021 | Clothier's Row | east | 13022 | Foodmonger's Lane |
| 13027 | Sage's Alcove | west | 13026 | Apothecary Lane |
| **Row 3** | | | | |
| 13031 | Syndic Council Annex | west | 13030 | Western Wall Avenue, Mid-West |
| 13032 | Civic Registry Office | east | 13033 | West Cross Street, North |
| 13035 | Temple of Concord | west | 13034 | Temple Approach |
| 13038 | Arcane Studies Hall | west | 13037 | East Cross Street, North |
| **Row 4** | | | | |
| 13041 | Tinker's Workshop | east | 13042 | Market Arcade, West |
| 13043 | Steel and Edge Atelier | west | 13042 | Market Arcade, West |
| 13044 | Kiess Bank of Ledgers | north | 13034 | Temple Approach |
| 13045 | Grand Market Exchange | south | 13055 | The Central Prism |
| 13046 | Hall of Restoration | east | 13047 | East Arcane Walk |
| 13048 | Alchemist's Gallery | west | 13047 | East Arcane Walk |
| **Row 5** | | | | |
| 13053 | Bastion Armory | west | 13052 | Memorial Fountain Plaza |
| 13054 | Postmaster of Kiess | east | 13055 | The Central Prism |
| 13056 | Quartermaster's Depot | west | 13055 | The Central Prism |
| 13057 | Argent Arcana | east | 13058 | Boulevard of Syndics, East |
| **Row 6** | | | | |
| 13061 | Provision Warehouse | east | 13062 | Lamplighter's Lane |
| 13064 | Wayfarer's Inn | east | 13065 | South Promenade, Upper |
| 13066 | Cartographer and Scribe | west | 13065 | South Promenade, Upper |
| 13068 | Enchanter's Parlor | west | 13067 | East Commerce Lane |
| **Row 7** | | | | |
| 13071 | Patrol Barracks, West | east | 13072 | Drill Yard, West |
| 13078 | Patrol Barracks, East | west | 13077 | Signal Tower Base |
| **Row 8** | | | | |
| 13081 | Mercenary Board Hall | east | 13082 | Stablemaster's Compound |
| 13083 | Travel Administration Office | north | 13073 | Military Supply Road, West |
| 13085 | Guild Registrar Hall | north | 13075 | South Promenade, Mid-Lower |
| 13086 | Portal Warden's Office | north | 13076 | Military Supply Road, East |
| 13088 | Wall Command Headquarters | west | 13087 | Arena Observation Deck |
| **Row 9** | | | | |
| 13093 | South Gate Inspection Post | east | 13094 | South Gate Mustering Yard |
| 13096 | South Gate Quartermaster Annex | west | 13095 | The South Gate of Kiess |


## City Wall and Perimeter Plan
- Kiess is fully enclosed by a continuous **city wall** around the outer rectangle (`x=0`, `x=9`, `y=0`, `y=9`).
- Wall-adjacent perimeter rooms represent battlements, gatehouses, and inner-wall avenues; they do not allow free exits to the wilderness except at designated gates.
- Only four controlled external links exist:
  - North Gate: `13005`
  - South Gate: `13095`
  - West Gate: `13050`
  - East Gate: `13059`
- Corner perimeter rooms (`13000`, `13009`, `13090`, `13099`) are fortified wall corners/towers with no direct wilderness traversal.
- Suggested room flags for wall/gatehouse rooms: defensive flavor flags as applicable, with gate doors reset closed for city security.

## Door Plan (explicit doors)
City streets remain open-grid except for controlled access points below. Building rooms are closed-off dead-ends (see Building Entrance Policy) and do not have doors — they simply have one exit.

1. **South Main Gate doors**
   - `13095 <-> 3243`
   - Door: south side, heavy gate, closed by reset, unlockable.

## Street Connectivity Rules
- All city-sector rooms connect cardinally to adjacent city-sector (or recall_set) rooms unless at the boundary edge.
- **All inside-sector rooms are dead-ends** (see Building Entrance Policy). They have exactly one exit and no other rooms connect into them except through that single entrance. Adjacent street rooms that border a building have no exit in the building's direction (building wall blocks passage).
- Boundary rooms are wall-line rooms: they connect inward, and connect outward only at the four designated gate exits.
- **Navigation pattern:** With 35 buildings as dead-ends, the outdoor street grid (64 rooms) forms an irregular network. The north-south and east-west spines are interrupted by building footprints in the central rows. Primary traversal uses:
  - **Perimeter wall avenues** (x=0 and x=9 columns): fully connected north-south corridors along both walls
  - **Row 7 band** (y=7, vnums 13070-13079): nearly complete east-west corridor through the military ring
  - **Row 2 band** (y=2, vnums 13020-13029): partial east-west corridor through the scholastic district
  - Buildings are accessed by stepping off these corridors into dead-end interiors
- Some street rooms become dead-end pockets (e.g., 13042 connects only south to 13052; 13017 connects only north to 13007). This is intentional — they serve as quiet alcoves adjacent to building entrances.

## Room Assignment Plan (all 100 rooms, detailed)

Sector types: `city` (1) for outdoor streets/plazas, `inside` (11) for enclosed buildings/interiors, `recall_set` (8) for the recall room only.

### Row 0: North Wall & Gate (y=0, vnums 13000-13009)

| Vnum | Room Name | Sector | Flags | Notes |
|------|-----------|--------|-------|-------|
| 13000 | Northwest Watchtower | city | 0 | Corner fortification; The Watcher of Storms statue; beacon brazier |
| 13001 | North Wall Walk, West End | city | 0 | Battlement walkway along north wall |
| 13002 | North Wall Walk, West Approach | city | 0 | Wall walk nearing the gate |
| 13003 | North Gate Customs Office | inside | safe | Screening station for arriving caravans; dead-end, entrance east from 13004 |
| 13004 | North Gate Staging Yard | city | 0 | Wagon marshalling area outside the gate proper |
| 13005 | The North Gate of Kiess | city | 0 | Primary north entry; Warden's Arch monument; external link north |
| 13006 | North Gate Guard Post | inside | safe | Guard office and patrol mustering room; dead-end, entrance west from 13005 |
| 13007 | North Wall Walk, East Approach | city | 0 | Wall walk east of the gate |
| 13008 | North Wall Walk, East End | city | 0 | Battlement walkway continuing east |
| 13009 | Northeast Watchtower | city | 0 | Corner fortification; The Watcher of Daybreak statue; beacon brazier |

### Row 1: Caravan Ring & Arrival Services (y=1, vnums 13010-13019)

| Vnum | Room Name | Sector | Flags | Notes |
|------|-----------|--------|-------|-------|
| 13010 | Western Wall Avenue, North | city | 0 | Inner avenue along west wall |
| 13011 | Caravan Rest Yard | city | 0 | Open-air resting area for arriving traders |
| 13012 | Traveler's Registry | inside | safe | Sign-in and public notices; dead-end, entrance west from 13011 |
| 13013 | Arrival Provisioner | inside | safe | Basic supplies for new arrivals; dead-end, entrance east from 13014 |
| 13014 | North Promenade, Upper | city | 0 | North-south spine road, north end |
| 13015 | Central Arrival Plaza | city | 0 | Wide plaza just south of the gate |
| 13016 | Courier Dispatch Hall | inside | safe | Message runners and dispatch coordination; dead-end, entrance west from 13015 |
| 13017 | Wagonwright's Yard | city | 0 | Wagon repair and livery stable |
| 13018 | Scout Debriefing Post | inside | safe | Forest scouts report here; dead-end, entrance west from 13017 |
| 13019 | Eastern Wall Avenue, North | city | 0 | Inner avenue along east wall |

### Row 2: North Commerce & Scholastic Streets (y=2, vnums 13020-13029)

| Vnum | Room Name | Sector | Flags | Notes |
|------|-----------|--------|-------|-------|
| 13020 | Western Wall Avenue, Upper Market | city | 0 | West wall avenue, market district begins |
| 13021 | Clothier's Row | inside | safe | Tailoring and garment services; dead-end, entrance east from 13022 |
| 13022 | Foodmonger's Lane | city | 0 | Open-air food stalls and bakeries |
| 13023 | Commerce Crossing, North | city | 0 | Intersection of market lanes |
| 13024 | North Promenade, Mid-Upper | city | 0 | North-south spine continuing south |
| 13025 | Civic Notice Square | city | 0 | Public announcement boards and waystones |
| 13026 | Apothecary Lane | city | 0 | Herbalists and potion-makers |
| 13027 | Sage's Alcove | inside | safe | Lore consultation and historical archives; dead-end, entrance west from 13026 |
| 13028 | Scholastic Colonnade | city | 0 | Covered walkway with memorial carvings |
| 13029 | Eastern Wall Avenue, Upper Scholastic | city | 0 | East wall avenue, scholastic district begins |

### Row 3: Temples & Civic Offices (y=3, vnums 13030-13039)

| Vnum | Room Name | Sector | Flags | Notes |
|------|-----------|--------|-------|-------|
| 13030 | Western Wall Avenue, Mid-West | city | 0 | West wall avenue continuing south |
| 13031 | Syndic Council Annex | inside | safe | Trade syndic administrative offices; dead-end, entrance west from 13030 |
| 13032 | Civic Registry Office | inside | safe | Birth, death, property records; dead-end, entrance east from 13033 |
| 13033 | West Cross Street, North | city | 0 | NW quadrant loop road |
| 13034 | Temple Approach | city | 0 | Processional avenue to the temple |
| 13035 | Temple of Concord | inside | safe | Compact Flame shrine; donation/recall support; dead-end, entrance west from 13034 |
| 13036 | Judicial Colonnade | city | 0 | Open-air covered walkway with arbitration tablets; connects 13037 to 13026 (prevents isolation) |
| 13037 | East Cross Street, North | city | 0 | NE quadrant loop road |
| 13038 | Arcane Studies Hall | inside | safe | Magical research and training; dead-end, entrance west from 13037 |
| 13039 | Eastern Wall Avenue, Mid-East | city | 0 | East wall avenue continuing south |

### Row 4: Core Trade Band (y=4, vnums 13040-13049)

| Vnum | Room Name | Sector | Flags | Notes |
|------|-----------|--------|-------|-------|
| 13040 | Western Wall Avenue, Trade Quarter | city | 0 | West wall avenue at trade band level |
| 13041 | Tinker's Workshop | inside | safe | Repair and crafting services; dead-end, entrance east from 13042 |
| 13042 | Market Arcade, West | city | 0 | Covered market walkway |
| 13043 | Steel and Edge Atelier | inside | safe | **Weapon shop**; dead-end, entrance west from 13042 |
| 13044 | Kiess Bank of Ledgers | inside | safe | **Banker**; dead-end, entrance north from 13034 |
| 13045 | Grand Market Exchange | inside | safe | **General shop**; dead-end, entrance south from 13055 |
| 13046 | Hall of Restoration | inside | safe | **Healer**; dead-end, entrance east from 13047 |
| 13047 | East Arcane Walk | city | 0 | Street connecting east cross street to arcane trade band |
| 13048 | Alchemist's Gallery | inside | safe | Potion identification and alchemical supplies; dead-end, entrance west from 13047 |
| 13049 | Eastern Wall Avenue, Trade Quarter | city | 0 | East wall avenue at trade band level |

### Row 5: Main Civic Boulevard (y=5, vnums 13050-13059)

| Vnum | Room Name | Sector | Flags | Notes |
|------|-----------|--------|-------|-------|
| 13050 | The West Gate of Kiess | city | 0 | West entry; Gatekeeper Sentinel statue; external link west |
| 13051 | Boulevard of Syndics, West | city | 0 | East-west spine road, west segment |
| 13052 | Memorial Fountain Plaza | city | 0 | Public fountain with carved Evermeet district names |
| 13053 | Bastion Armory | inside | safe | **Armor shop**; dead-end, entrance west from 13052 |
| 13054 | Postmaster of Kiess | inside | safe | **Postmaster**; mail services; dead-end, entrance east from 13055 |
| 13055 | The Central Prism | recall_set | safe | **Recall room**; Executioner stationed; Prism of Continuance |
| 13056 | Quartermaster's Depot | inside | safe | **Quartermaster**; supplies and logistical gear; dead-end, entrance west from 13055 |
| 13057 | Argent Arcana | inside | safe | **Magic shop**; dead-end, entrance east from 13058 |
| 13058 | Boulevard of Syndics, East | city | 0 | East-west spine road, east segment |
| 13059 | The East Gate of Kiess | city | 0 | East entry; Gatekeeper Sentinel statue; external link east |

### Row 6: Mid-South Commerce & Inn Corridor (y=6, vnums 13060-13069)

| Vnum | Room Name | Sector | Flags | Notes |
|------|-----------|--------|-------|-------|
| 13060 | Western Wall Avenue, Lower Market | city | 0 | West wall avenue, south of trade band |
| 13061 | Provision Warehouse | inside | safe | Bulk goods storage; dead-end, entrance east from 13062 |
| 13062 | Lamplighter's Lane | city | 0 | Street maintenance and lamp services |
| 13063 | South Market Crossing | city | 0 | SW quadrant loop intersection |
| 13064 | Wayfarer's Inn | inside | safe | **Innkeeper**; rent services; dead-end, entrance east from 13065 |
| 13065 | South Promenade, Upper | city | 0 | North-south spine, south of center |
| 13066 | Cartographer and Scribe | inside | safe | **Maps/utility items**; dead-end, entrance west from 13065; Atlas Pedestal monument |
| 13067 | East Commerce Lane | city | 0 | SE quadrant loop road |
| 13068 | Enchanter's Parlor | inside | safe | Enchantment and identification services; dead-end, entrance west from 13067 |
| 13069 | Eastern Wall Avenue, Lower Scholastic | city | 0 | East wall avenue south of trade band |

### Row 7: Military Support Ring (y=7, vnums 13070-13079)

| Vnum | Room Name | Sector | Flags | Notes |
|------|-----------|--------|-------|-------|
| 13070 | Western Wall Avenue, Lower | city | 0 | West wall avenue nearing south ring |
| 13071 | Patrol Barracks, West | inside | safe | Guard quarters and armory; dead-end, entrance east from 13072 |
| 13072 | Drill Yard, West | city | 0 | Open training space |
| 13073 | Military Supply Road, West | city | 0 | SW quadrant loop road continuing |
| 13074 | Logistics Crossing | city | 0 | Intersection of supply routes |
| 13075 | South Promenade, Mid-Lower | city | 0 | North-south spine continuing south |
| 13076 | Military Supply Road, East | city | 0 | SE quadrant loop road continuing |
| 13077 | Signal Tower Base | city | 0 | Base of beacon communication tower |
| 13078 | Patrol Barracks, East | inside | safe | Eastern guard quarters; dead-end, entrance west from 13077 |
| 13079 | Eastern Wall Avenue, Lower | city | 0 | East wall avenue nearing south ring |

### Row 8: Southern Guild Annexes & Travel Admin (y=8, vnums 13080-13089)

| Vnum | Room Name | Sector | Flags | Notes |
|------|-----------|--------|-------|-------|
| 13080 | Western Wall Avenue, South | city | 0 | West wall avenue, south segment |
| 13081 | Mercenary Board Hall | inside | safe | Job postings and bounty contracts; dead-end, entrance east from 13082 |
| 13082 | Stablemaster's Compound | city | 0 | Mount stabling and travel prep |
| 13083 | Travel Administration Office | inside | safe | Permits, passes, and caravan scheduling; dead-end, entrance north from 13073 |
| 13084 | South Promenade, Lower | city | 0 | North-south spine, approaching south gate |
| 13085 | Guild Registrar Hall | inside | safe | **Class guidance/training hooks**; dead-end, entrance north from 13075 |
| 13086 | Portal Warden's Office | inside | safe | Teleportation and portal administration; dead-end, entrance north from 13076 |
| 13087 | Arena Observation Deck | city | 0 | Overlook for training yard matches |
| 13088 | Wall Command Headquarters | inside | safe | Military leadership offices; dead-end, entrance west from 13087 |
| 13089 | Eastern Wall Avenue, South | city | 0 | East wall avenue, south segment |

### Row 9: South Wall & Gate (y=9, vnums 13090-13099)

| Vnum | Room Name | Sector | Flags | Notes |
|------|-----------|--------|-------|-------|
| 13090 | Southwest Watchtower | city | 0 | Corner fortification; The Watcher of Storms statue |
| 13091 | South Wall Walk, West End | city | 0 | Battlement walkway along south wall |
| 13092 | South Wall Walk, West Approach | city | 0 | Wall walk nearing the south gate |
| 13093 | South Gate Inspection Post | inside | safe | Outbound cargo inspection; dead-end, entrance east from 13094 |
| 13094 | South Gate Mustering Yard | city | 0 | Assembly area for outbound expeditions |
| 13095 | The South Gate of Kiess | city | 0 | Primary south entry; Pillar of Returning Caravans; external link south |
| 13096 | South Gate Quartermaster Annex | inside | safe | Outbound supply distribution; dead-end, entrance west from 13095 |
| 13097 | South Wall Walk, East Approach | city | 0 | Wall walk east of the south gate |
| 13098 | South Wall Walk, East End | city | 0 | Battlement walkway continuing east |
| 13099 | Southeast Watchtower | city | 0 | Corner fortification; The Watcher of Dusk statue |

### Room Flag Summary

- **`recall_set` sector (8):** 13055 only (Central Prism); room_flags = `1024` (ROOM_SAFE)
- **`inside` sector (11), `safe` rooms (35 total):** All enclosed buildings — room_flags = `1024` (ROOM_SAFE). All are dead-end rooms with one exit (see Building Entrance Policy).
- **`city` sector (1), unflagged streets (64 total):** All exterior roads, plazas, wall walks, gate approaches — room_flags = `0`
- **No `dark` (1), `no_mob` (4), or `no_magic` (16) flags** are planned for any Kiess room (city hub should be fully accessible and lit)

**Area file format reference:** In the `#ROOMS` section, each room's header line is `<room_flags> <sector_type>`. For example, the Central Prism would be `1024 8` (safe + recall_set), an inside shop would be `1024 11` (safe + inside), and a city street would be `0 1` (no flags + city).


## Shop Inventory Plan (thematic items for sale)
All listed shop items are planned at levels **75-125**, and every level is a **multiple of 5**.

### 13045 - Grand Market Exchange (General Shop, 18 items)
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

### 13043 - Steel and Edge Atelier (Weapon Shop, 16 items)
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

### 13053 - Bastion Armory (Armor Shop, 16 items)
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

### 13057 - Argent Arcana (Magic Shop, 17 items)
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
- **13055 - Central Prism Plaza:**
  - Monument: **The Prism of Continuance** (city-defining relic monument tied to Evermeet's legacy)
  - Flavor objects: prismatic waystones, civic decree obelisk, memorial braziers
- **13005 - North Gate Approach:**
  - Monument: **Warden's Arch of Confusion Watch** (honors forest scouts and patrol losses)
  - Flavor objects: route marker stelae pointing toward the Forest of Confusion, supply manifests on iron lecterns
- **13095 - South Gate Approach:**
  - Monument: **Pillar of Returning Caravans** (marks safe return and trade continuity)
  - Flavor objects: carved caravan tally board, bronze wheel reliefs
- **13050 / 13059 - West/East Gatehouses:**
  - Statues: paired **Gatekeeper Sentinels** (stone guardians facing outward)
  - Flavor objects: gate chains, warning gongs, heraldic shields
- **13035 - Temple of Concord:**
  - Monument: **Shrine of the Compact Flame** (symbolizes guild-temple civic pact)
  - Flavor objects: votive racks, concord tablets, ceremonial font
- **13044 - Bank of Ledgers:**
  - Flavor objects: gilded ledger pedestals, oath-chest, seal press altar, iron strongbox display
- **13054 - Postmaster of Kiess:**
  - Flavor objects: dispatch board, courier satchel wall, route clock
- **13056 - Quartermaster's Depot:**
  - Flavor objects: campaign maps, requisition crates, stamped supply racks
- **13064 - Wayfarer's Inn:**
  - Flavor objects: painted city map mural, traveler memorial plaque, brass lantern cluster
- **13066 - Cartographer and Scribe:**
  - Monument object: **Atlas Pedestal of New Kiess** (updated map showing Evermeet-to-Kiess transition)
  - Flavor objects: survey tripods, rolled charts, inkstone cabinet
- **Perimeter wall towers (13000, 13009, 13090, 13099):**
  - Statues: **The Four Watchers** (named tower sentinels representing North/South/East/West vigilance)
  - Flavor objects: beacon braziers, weatherworn battle plaques

### Flavor Object Implementation Notes
- Place major monuments in high-traffic civic nodes to reinforce the Evermeet succession narrative.
- Keep monument names lore-facing and persistent; avoid novelty props that conflict with the fortified tone.
- Interior service-building props should remain non-blocking and compatible with safe-room policy.
- Gate and wall flavor objects should visually reinforce controlled entry through the four designated gates.

## NPC/Service Priority Notes
- Executioner is fixed at `13055`, level 150, flagged **executioner**, **sentinel**, and **boss**, and never allowed to move.
- Postmaster and Quartermaster are fixed-position level-150 utility NPCs (`13054`, `13056`) and flagged **sentinel**.
- Shopkeepers and all utility NPCs are non-wandering, level 150, and flagged **sentinel**; all core economy functions are within two steps of recall.
- Additional ambient mobs (citizens/watch) are set around level 40 for city life population.

---

## Mobile Definitions

### Service NPCs (level 150, all flagged `is_npc|sentinel`)

All service NPCs are level 150 and fixed in place. Sex assignments alternate for variety.

**Act flag reference values:** `is_npc`=1, `sentinel`=2, `boss`=67108864, `no_flee`=16, `bank`=32768, `heal`=4096, `postman`=524288, `practice`=1024, `stay_area`=64. Combine by addition (e.g., `is_npc|sentinel` = 3).

| Vnum | Name | Level | Room | Act Flags | Service Role | Notes |
|------|------|-------|------|-----------|-------------|-------|
| 13000 | the Executioner of Kiess | 150 | 13055 | is_npc, sentinel, boss, no_flee | Executioner | `spec_executioner`; city law enforcement |
| 13001 | the Postmaster of Kiess | 150 | 13054 | is_npc, sentinel, postman | Postmaster | Mail services |
| 13002 | the Quartermaster of Kiess | 150 | 13056 | is_npc, sentinel | Quartermaster | Supplies and gear |
| 13003 | a Grand Market merchant | 150 | 13045 | is_npc, sentinel | General shop | Shop: buys/sells general goods |
| 13004 | a Steel and Edge weaponsmith | 150 | 13043 | is_npc, sentinel | Weapon shop | Shop: buys/sells weapons |
| 13005 | a Bastion armorer | 150 | 13053 | is_npc, sentinel | Armor shop | Shop: buys/sells armor |
| 13006 | an Argent Arcana mage-merchant | 150 | 13057 | is_npc, sentinel | Magic shop | Shop: buys/sells magical items |
| 13007 | the Banker of Kiess | 150 | 13044 | is_npc, sentinel, bank | Banker | Banking services |
| 13008 | the Healer of Kiess | 150 | 13046 | is_npc, sentinel, heal | Healer | Healing services |
| 13009 | the Innkeeper of Kiess | 150 | 13064 | is_npc, sentinel | Innkeeper | Rent/save services |
| 13010 | a Cartographer of Kiess | 150 | 13066 | is_npc, sentinel | Scribe/map shop | Shop: maps and utility items |
| 13011 | the Guild Registrar of Kiess | 150 | 13085 | is_npc, sentinel, practice | Guild registrar | Class guidance and training |
| 13012 | a Temple Concord priest | 150 | 13035 | is_npc, sentinel | Temple keeper | Donation and recall support |

### Ambient City Mobs (level 38-45, all flagged `is_npc|stay_area`)

These populate the streets and give the city life. None are aggressive; they are background flavor. All use the melee profile for skill assignment (since they are non-hostile city NPCs at low level, they will have minimal combat skills).

| Vnum | Name | Level | Act Flags | Notes |
|------|------|-------|-----------|-------|
| 13013 | a Kiess citizen | 40 | is_npc, stay_area | Generic townsperson; roams streets |
| 13014 | a Kiess guardsman | 45 | is_npc, sentinel | Stationary patrol guard |
| 13015 | a Wall Command sentry | 42 | is_npc, sentinel | Wall and gate sentry |
| 13016 | a Kiess street sweeper | 38 | is_npc, stay_area | Civic janitor; `spec_janitor` |
| 13017 | a Syndic trade courier | 40 | is_npc, stay_area | Running messages between shops |
| 13018 | a Temple Concord acolyte | 39 | is_npc, stay_area | Junior temple attendant |
| 13019 | a caravan drover | 41 | is_npc, stay_area | Wagon handler in north ring |
| 13020 | a Kiess lamplighter | 38 | is_npc, stay_area | Maintains city lamps |
| 13021 | a Forest Confusion scout | 43 | is_npc, stay_area | Returning from forest patrol |
| 13022 | a Wall Command sergeant | 44 | is_npc, sentinel | Senior guard; stationed in military ring |
| 13023 | a Prism Square orator | 40 | is_npc, stay_area | Public speaker near recall |
| 13024 | a Kiess merchant's apprentice | 38 | is_npc, stay_area | Errand runner for shops |
| 13025 | a Kiess stablehand | 39 | is_npc, stay_area | Works at the stables |

**Mob combat extensions (`!` line):**
- Service NPCs (level 150): no combat extensions needed (non-combatant service roles); Executioner uses `spec_executioner` for combat behavior
- Ambient mobs (level 38-45): minimal combat; 2 attacks, punch, kick, dodge at most (per melee profile floors)

**Mob elemental extensions (`|` line):**
- No elemental extensions planned for city NPCs (neutral city environment, no elemental theme)

**Mob implementation notes (to be resolved during area file construction):**
- **Sex:** Alternate male/female across service NPCs for variety. Ambient mobs should also vary.
- **Alignment:** Service NPCs should be good-aligned (positive alignment, ~750). Ambient citizens neutral (~0), guards mildly good (~350).
- **Race/Class:** Use `human` race and `melee` class for all city NPCs unless a specific role warrants otherwise (e.g., Temple priest could be `cleric` class, Arcane Studies mobs could be `mage` class).
- **Executioner equipment:** The Executioner (mob 13000) engages in combat via `spec_executioner`. It should have equipment resets (E commands) for weapon and armor to be effective at level 150. Define appropriate weapon/armor objects within the 13068-13099 object vnum range (currently unallocated) and add E resets after the Executioner's M reset.
- **Short/long descriptions:** Each mob needs `short_descr` (used in combat/action messages, e.g., "the Executioner of Kiess") and `long_descr` (shown in room, e.g., "The Executioner of Kiess stands here, watching for lawbreakers."). These should be written during implementation per area file spec conventions.

---

## Shops Plan

| Keeper Vnum | Room | Buy Types | Profit Buy | Profit Sell | Open | Close | Notes |
|-------------|------|-----------|-----------|------------|------|-------|-------|
| 13003 | 13045 | 0 0 0 0 0 | 120 | 80 | 0 | 23 | General shop — buys anything (all buy_type slots 0) |
| 13004 | 13043 | 5 0 0 0 0 | 120 | 80 | 0 | 23 | Weapon shop — buys weapons (ITEM_WEAPON=5) |
| 13005 | 13053 | 9 0 0 0 0 | 120 | 80 | 0 | 23 | Armor shop — buys armor (ITEM_ARMOR=9) |
| 13006 | 13057 | 2 4 10 0 0 | 130 | 70 | 0 | 23 | Magic shop — buys scrolls (2), staves (4), potions (10) |
| 13010 | 13066 | 0 0 0 0 0 | 110 | 90 | 0 | 23 | Scribe/maps — buys anything (all buy_type slots 0) |

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
| M 0 | 13000 | 1 | 13055 | Executioner at Central Prism |
| M 0 | 13001 | 1 | 13054 | Postmaster |
| M 0 | 13002 | 1 | 13056 | Quartermaster |
| M 0 | 13003 | 1 | 13045 | General shop merchant |
| M 0 | 13004 | 1 | 13043 | Weapon shop |
| M 0 | 13005 | 1 | 13053 | Armor shop |
| M 0 | 13006 | 1 | 13057 | Magic shop |
| M 0 | 13007 | 1 | 13044 | Banker |
| M 0 | 13008 | 1 | 13046 | Healer |
| M 0 | 13009 | 1 | 13064 | Innkeeper |
| M 0 | 13010 | 1 | 13066 | Cartographer/Scribe |
| M 0 | 13011 | 1 | 13085 | Guild Registrar |
| M 0 | 13012 | 1 | 13035 | Temple priest |

**Ambient City Mobs (~25-30 total spawns across city streets):**

| Command | Mob Vnum | Limit | Room Vnum | Notes |
|---------|----------|-------|-----------|-------|
| M 0 | 13013 | 6 | 13015 | Citizen in arrival plaza |
| M 0 | 13013 | 6 | 13022 | Citizen in foodmonger's lane |
| M 0 | 13013 | 6 | 13052 | Citizen at memorial fountain |
| M 0 | 13013 | 6 | 13065 | Citizen on south promenade |
| M 0 | 13013 | 6 | 13042 | Citizen in market arcade |
| M 0 | 13013 | 6 | 13074 | Citizen at logistics crossing |
| M 0 | 13014 | 4 | 13005 | Guard at north gate |
| M 0 | 13014 | 4 | 13095 | Guard at south gate |
| M 0 | 13014 | 4 | 13055 | Guard at Central Prism |
| M 0 | 13014 | 4 | 13084 | Guard on south promenade |
| M 0 | 13015 | 3 | 13000 | Sentry at NW tower |
| M 0 | 13015 | 3 | 13009 | Sentry at NE tower |
| M 0 | 13015 | 3 | 13050 | Sentry at west gate |
| M 0 | 13015 | 3 | 13059 | Sentry at east gate |
| M 0 | 13015 | 3 | 13090 | Sentry at SW tower |
| M 0 | 13015 | 3 | 13099 | Sentry at SE tower |
| M 0 | 13016 | 2 | 13062 | Street sweeper on Lamplighter's Lane |
| M 0 | 13016 | 2 | 13025 | Street sweeper at notice square |
| M 0 | 13017 | 2 | 13051 | Trade courier on boulevard |
| M 0 | 13018 | 2 | 13034 | Acolyte near temple |
| M 0 | 13019 | 2 | 13011 | Drover in caravan rest yard |
| M 0 | 13020 | 2 | 13062 | Lamplighter on their lane |
| M 0 | 13021 | 2 | 13017 | Scout near debriefing post (Wagonwright's Yard) |
| M 0 | 13022 | 2 | 13075 | Sergeant on south promenade |
| M 0 | 13023 | 2 | 13055 | Orator at prism square |
| M 0 | 13024 | 2 | 13042 | Apprentice in market arcade |
| M 0 | 13025 | 2 | 13082 | Stablehand at compound |

### Door Resets (`D` commands)

| Command | Room Vnum | Door | State | Notes |
|---------|-----------|------|-------|-------|
| D 0 | 13095 | 2 | 1 | South Gate — closed on reset |

### Object Resets (`O` commands)

No standalone room object spawns planned initially. Shop inventory is defined through the `#SHOPS` section and shop object definitions. Flavor objects (monuments, braziers, etc.) will be implemented as room extra descriptions rather than spawned objects, keeping the area file simpler and avoiding unnecessary object management.

---

## Object Definitions (Non-Shop)

No non-shop objects are currently defined. Object vnum 13000 is available.

### Shop Object Vnum Assignments

Shop inventory items use vnums 13001-13067 (67 items total across 4 shops). Each shop's items are grouped sequentially:

- **13001-13018:** Grand Market Exchange items (18 general goods)
- **13019-13034:** Steel and Edge Atelier items (16 weapons)
- **13035-13050:** Bastion Armory items (16 armor pieces)
- **13051-13067:** Argent Arcana items (17 magical items)

Full item definitions (item_type, extra_flags, wear_flags, item_apply, value fields, weight) to be determined during implementation per the area file spec. All shop items follow the level and weight conventions specified in the Shop Inventory Plan above.

### Reserved Object Vnums

- **13000, 13068-13099:** Unallocated (33 vnums). Reserved for Executioner equipment (weapon, armor), quest items, or other non-shop objects added during implementation.

---

## Specials Plan

| Mob Vnum | Spec Function | Reason |
|----------|--------------|--------|
| 13000 | spec_executioner | Executioner — city law enforcement; attacks criminals |
| 13016 | spec_janitor | Street sweeper — picks up litter/dropped items |

Notes:
- Most service NPCs (shopkeepers, banker, healer, postmaster) do not need spec functions; their behavior is driven by their `act` flags (`bank`, `heal`, `postman`) and `#SHOPS` entries.
- Guards and sentries do not use `spec_guard`/`spec_policeman` in this plan to avoid unintended aggression toward players with criminal flags arriving at a recall hub. This may be revisited after playtesting.

---

## Connection Plan (External Links)

Only the south gate has an active external connection. The north, west, and east gates are internal city boundaries with no external exits defined at this time.

### South Gate (13095)
- **Direction:** south from 13095 → vnum `3243`; north from 3243 → vnum `13095`
- **Door:** `EX_ISDOOR`, reset closed (state 1), unlockable
- **Rationale:** Outbound military and trade expeditions depart south. Bidirectional link ensures players can return through the gate.

### Connection Implementation Notes
- The south gate uses `EX_ISDOOR` with a `D` reset to state `closed` (1), not `locked` (2), so players can freely open it.
- The reverse exit (north from `3243` back to `13095`) must be added to the area file that owns vnum `3243`.
- North (`13005`), west (`13050`), and east (`13059`) gates remain walled city exits with no external destination. They may be connected to future areas later.

---

## Implementation Checklist

1. [ ] Create `area/kiess.are` with `#AREA` header and `#$`
2. [ ] Write `#ROOMS` section (100 rooms, vnums 13000-13099, all used)
3. [ ] Write `#MOBILES` section (13 service NPCs + 13 ambient mobs = 26 mobs, vnums 13000-13025)
4. [ ] Write `#OBJECTS` section (67 shop items, vnums 13001-13067; plus Executioner equipment from reserved range)
5. [ ] Write `#SHOPS` section (5 shops: general, weapon, armor, magic, scribe)
6. [ ] Write `#RESETS` section (~13 service mob spawns + ~25 ambient mob spawns + 1 door reset)
7. [ ] Write `#SPECIALS` section (2 entries: executioner, janitor)
8. [ ] Add `kiess.are` to `area/area.lst`
9. [ ] Add north exit from vnum `3243` back to `13095` in `area/rocroad.are` (which owns vnum 3243, "Roc Road")
10. [ ] Run `cd src && make unit-tests` to validate
11. [ ] Verify all vnums within 13000-13099 range
12. [ ] Verify no vnum conflicts with existing areas (confirmed: minokeep uses 1300-1399, no overlap)
