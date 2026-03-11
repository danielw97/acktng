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
- `Q 16`
- `K kiess~`
- `L @@W(@@a1 170@@W)@@N~`
- `I 1 170`
- `V 13000 13099`
- `F 15`
- `U @@Wthe bells of @@BKiess@@W ring across the avenues@@N~`
- `T`


## Lore
Kiess rises where **Evermeet once stood**, built after the old city fell to repeated planar stress, trade-route collapse, and monster pressure spilling in from surrounding wildlands. Rather than rebuild Evermeet in its former fragmented pattern, the crown-chartered guilds and the temple compact founded Kiess as a fortified civic grid designed for resilience, rapid response, and uninterrupted commerce.

The city was planned around a luminous civic keystone known as the **Central Prism** (room `13055`), a memorial and continuity anchor carried from the final days of Evermeet. Public law, messaging, provisioning, and magical logistics were all positioned around this center so travelers can recover, orient, and re-enter the frontier quickly. The Executioner at the Prism symbolizes the compact's uncompromising enforcement of peace inside city limits.

Kiess keeps **close operational ties to the Forest of Confusion**: scouts, wardens, and quartermaster routes run daily between the city gates and forest-edge staging trails. Caravans arriving from the forest are screened at the north approach, healers and scribes process reports in the central wards, and supply trains cycle from the quartermaster ring back to expedition parties. This makes Kiess the primary stabilizing hub between civilization and the shifting deep-forest frontier.

Architecturally, Kiess is identified by white civic stone, blue-and-gold banners, and prismatic glasswork over canals and plazas. The mood is disciplined rather than decadent: every district is intended to read as lived-in, guarded, and connected to frontier support. In-world, locals describe Kiess as "Evermeet remembered, but finally made defensible."

The fall of Evermeet did not happen in a single night. Records describe a long unraveling: civic districts isolated by unstable crossings, supply houses burned during successive crises, and outer wards abandoned as wilderness pressure increased each season. Kiess was commissioned specifically to end that cycle by replacing improvised growth with deliberate urban geometry, allowing defenders, couriers, and quartermaster crews to predict movement and recover from attacks without paralyzing the whole city.

Founding charters divide Kiess governance among three powers held in tension: the Compact Temples, the Trade Syndics, and the Wall Command. The temples maintain sanctuary law and civic rites, the syndics maintain market continuity and caravan contracts, and Wall Command maintains gate discipline and patrol rotation. The Executioner post at the Central Prism is the legal instrument that binds those factions to a common peace code: disputes are mediated in the square, but violent breach of city law is answered immediately and publicly.

Kiess also inherited Evermeet's memory culture. Names of the lost districts are carved into waystones along the main avenues, and annual processions move from the north gate to the Prism to mark the first reconstruction oath. Monuments across the city are not decorative excess; they are navigational memory points for residents and returning expeditions, reminding players that Kiess is both a functioning fortress-city and a living memorial.

Beyond the walls, the Forest of Confusion remains both threat and necessity. Herbal reagents, rare timbers, and relic fragments still flow through forest routes, and Kiess survives by organizing that risk rather than pretending it is gone. This is why gatehouses are strict, quartermaster halls are central, and messenger lines are overbuilt: the city is designed as a permanent interface between civilized order and an unpredictable frontier.

In present-day play, Kiess should feel like a city that has chosen preparedness over nostalgia. Its bright colors and civic glasswork signal renewal, while statues, decrees, and tower beacons signal vigilance. The intended atmosphere is "rebuilt, resolute, and always watching."

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
City streets remain open-grid except for controlled access points below.

1. **North Main Gate doors**
   - `13005 <-> external north-link`
   - Door: north side, heavy gate, closed by reset, unlockable.
2. **South Main Gate doors**
   - `13095 <-> external south-link`
   - Door: south side, heavy gate, closed by reset, unlockable.
3. **West Trade Gate doors**
   - `13050 <-> external west-link`
   - Door: west side, closed/open cycle by guards.
4. **East Scholar Gate doors**
   - `13059 <-> external east-link`
   - Door: east side, closed by night reset, unlockable.
5. **Bank Vault antechamber**
   - `13044 east -> 13047`
   - Door: locked iron door (bank staff access/event use).
6. **Quartermaster stockroom**
   - `13056 south -> 13066`
   - Door: reinforced, closed, unlockable by quartermaster key.
7. **Execution dais lockout**
   - `13055 up -> 13055 (platform sub-flag/virtual internal)`
   - Door/exit behavior: staff-only blocked climb except by scripted event.

## Street Connectivity Rules
- Every interior grid room connects cardinally to valid neighbors (N/E/S/W) unless boundary edge.
- Boundary rooms are wall-line rooms: they connect inward, and connect outward only at the four designated gate exits.
- Central avenues are full straight lines:
  - North-south spine: `13005 -> 13095`
  - West-east spine: `13050 -> 13059`
- Four quadrant loops ensure navigation without dead ends:
  - NW loop around 13033/13034/13043/13044
  - NE loop around 13036/13037/13046/13047
  - SW loop around 13063/13064/13073/13074
  - SE loop around 13066/13067/13076/13077

## Room Assignment Plan (all 100 rooms used)
- **13000-13009:** North wall, customs, gate walk, arrival street.
- **13010-13019:** Caravan lodgings, starter services, public notices.
- **13020-13029:** West-north commerce and east-north scholastic streets.
- **13030-13039:** Temples, civic offices, training entrances.
- **13040-13049:** Core trade band (bank, market, healer, weapon lines).
- **13050-13059:** Main civic boulevard (includes recall center, postmaster, quartermaster).
- **13060-13069:** Mid-south commerce and inn/provision corridors.
- **13070-13079:** Military support ring and logistics corridors.
- **13080-13089:** Southern guild annexes and travel administration.
- **13090-13099:** South wall, outbound security, grand gate approach.


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
  - Flavor objects: gilded ledger pedestals, oath-chest, seal press altar
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
