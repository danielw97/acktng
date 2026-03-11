# Area Plan: Kowloon

## Overview
- **Area Name:** `@@R@@xKowloon@@N`
- **File Name:** `kowloon.are`
- **Keyword:** `kowloon`
- **Intended Levels:** 1-170 (all player levels)
- **Vnum Range:** `14000-14099`
- **Vnum Count:** 100 (all room vnums used)
- **Shape:** 10x10 rectangle (full grid)
- **Recall Room:** `14055` (`recall_set`), central city plaza
- **Central Law NPC:** Executioner in `14055`

## Lore
### Before the Walls: Delta of Lanterns
Before Kowloon had a name, the delta was called the **Shallow Thousand**—a drowned lacework of stilts, prayer poles, tide farms, and contraband docks. Clan skiffs moved by moon and bell, not by law. Some villages paid tribute to river spirits, some to mercenary captains, and some to whichever gang controlled the grain bridges that season.

For a time, this patchwork worked. Then came the **Ashfall Monsoon**. For three consecutive years the rain turned black with volcanic grit from distant mountains. Roofs collapsed under acidic sludge, canals choked, and fresh water went bitter. Compacts failed, river feuds became famine wars, and an entire generation learned to sleep above floodline with a blade in hand.

### The Neon Covenant
In the fourth year, six surviving houses convened on a basalt outcrop that never submerged, lit by storm lanterns and guarded by oathbound killers from rival bloodlines. Their pact became the **Neon Covenant**—a charter that traded clan autonomy for collective survival.

The Covenant demanded three hard promises:
1. **One wall, one law:** every district inside the walls answers to civic statute, not clan retaliation.
2. **One ledger, one ration code:** grain, medicine, and timber are tracked centrally in shortage years.
3. **One square, final judgment:** disputes unresolved by magistrates are settled under public witness at the covenant stone.

The new city was raised on terraces, drainage vaults, and reinforced underways. Old villages were dismantled and rebuilt into a rectangular plan that could be patrolled, supplied, and defended in any season.

### The Five Seats of Rule
Modern Kowloon is governed by five institutions, each occupying a seat in the compact tribunal:
- **Harbor Syndics:** regulate docks, tariffs, and long-haul contracts.
- **Jade Magistracy:** tax authority, civil law, and licensing.
- **Temple Circle:** healing houses, funerary rites, disaster sanctuaries.
- **Wardens of Iron:** walls, gate discipline, and armed response.
- **Courier Lantern Office:** registry archives, post routes, and recall administration.

No seat rules alone. Each can censure another, and all five are bound by tribunal record. The city believes balance is safer than trust.

### Colors of Power, Memory, and Street Life
Kowloon’s colors are not decoration—they are civic shorthand born from disaster:
- **Crimson (`@@R`)** marks emergency lanes, legal notices, and execution authority.
- **Jade (`@@G`)** marks permits, public granaries, and magistrate offices.
- **Gold (`@@y`)** marks licensed market corridors and bonded merchants.
- **Blue (`@@B`)** marks pumps, sluices, dock chains, and storm engineering.
- **Violet (`@@p`)** marks licensed night markets, occult exchanges, and private guild salons.
- **Light-cyan (`@@a`)** marks rainwater routes, glass beacons, and floodline indicators.

Citizens read color as quickly as text. Visitors who ignore it usually pay in coin, blood, or both.

### The Heart of the City: 14055 Execution Square
Room `14055`, the **Crimson-Jade Execution Square**, sits over the original covenant basalt and functions as recall anchor, legal stage, and political pressure valve. Every proclamation of war, ration, plague response, and debt amnesty has been spoken from its elevated stones.

The **Executioner** stationed there is a constitutional office, not a faction soldier. The post can be activated by tribunal seal when city peace is broken—assassination inside sanctuary wards, sabotage of gate machinery, theft from emergency reserves, or riot during flood alarm. The office’s public neutrality is terrifying by design: everyone fears it, so everyone obeys the square.

### District Memory and Daily Rhythm
Kowloon appears rigid on maps, but each district carries older identities beneath the plan:
- Northern lanes remember the village registrars and ferry clerks who became the modern post and administration spine.
- Western markets descended from tarp bazaars that once floated from pier to pier with tide levels.
- Eastern corridors house ritual specialists and arcane brokers who once sold weather charms to fishermen.
- Southern terraces hold barracks, warehouses, and caravan yards built on former flood refuges.

At dawn, bells signal pump inspections and gate inventories. Midday belongs to contracts, provisioning, and tribunal filings. Dusk shifts toward tavern diplomacy, courier exchange, and quiet black-market whispers in violet-lit alleys.

### Why Adventurers Matter Here
Kowloon welcomes all levels because danger around the delta never truly ended. New adventurers escort supply trains, clear drainage tunnels, and guard shrine routes. Veteran companies negotiate high-risk salvage rights beyond the gate roads, hunt organized raider bands, and recover lost covenant relics from submerged ruins.

The city’s service geometry reflects this practical ethos: key shops are near traversal spines, interiors are single-access for auditability, and quartermaster logistics are positioned near the central recall nexus so recovering parties can rearm fast.

### Enemies, Tensions, and Rumors
Peace in Kowloon is durable, not gentle. Ongoing tensions include:
- **River Corsairs** testing gate convoys and warehouse patrol timings.
- **Salt Court Exiles** (deposed clan heirs) funding unrest through debt rings.
- **Ash Cult cells** that romanticize catastrophe and sabotage floodworks.
- **Ledger wars** between legal syndics and contraband financiers over shipping control.

Persistent rumors speak of a sealed undercity cistern beneath the central quadrants, containing pre-Covenant records that could invalidate noble titles—or prove the Covenant itself was founded on forged lineage claims.

### Gates and the Promise of Return
Kowloon’s four heavy gates symbolize a hard civic creed: **open to trade, closed to chaos**. They are reset secure by policy, inspected by shift, and opened by measured protocol rather than emotion. Inside, movement is fast and practical; outside, risk is assumed.

For battered travelers, recall to `14055` is more than convenience—it is the city’s oldest promise: if you can still stand, Kowloon will receive you, count you, provision you, judge you if needed, and send you back into the rain.

## Multi-Color Theme (spec-compliant)
- **Neon red arteries:** `@@R`
- **Jade market/civic accents:** `@@G`
- **Electric blue harbor-tech accents:** `@@B`
- **Lantern gold commerce accents:** `@@y`
- **Violet nightlife/mystic accents:** `@@p`
- **Light-cyan rain and glass highlights:** `@@a`
- **Always reset with:** `@@N`

> Theme policy compliance: no `@@k` black, no background codes (`@@0-@@7`), no flashing (`@@f`).

## #AREA Header Plan
- `Q 16`
- `K kowloon~`
- `L @@W(@@a1 170@@W)@@N~`
- `I 1 170`
- `V 14000 14099`
- `F 15`
- `U @@Rneon rain falls across @@GKowloon@@N~`
- `O Virant~`
- Optional builder directives (`N`, `X`, `R`, `W`, `T`) set during implementation.

## City Layout Concept (10x10 rectangle)
- Full rectangular footprint, all 100 vnums populated.
- Outer ring (`x=0`, `x=9`, `y=0`, `y=9`) is wall/gate boulevard.
- `14055` is exact center (row 5, col 5) as recall + civic nexus.
- Main spines:
  - **North/South Spine:** column 5 (vnums `14005, 14015, ... 14095`)
  - **West/East Spine:** row 5 (vnums `14050-14059`)
- Interior districts branch from spines: market, civic, docks, temples, barracks, inns.

## Shop + Service Requirements (all with one bi-directional exit only)
Every room below is an **inside-sector service room** with exactly one entry exit to one street room, and one return exit back (bi-directional pair only):

- `14021` Postmaster of Kowloon ↔ `14022`
- `14056` Quartermaster Depot ↔ `14055`
- `14042` General Goods ↔ `14052`
- `14043` Weapon Smith ↔ `14053`
- `14044` Armor Smith ↔ `14054`
- `14046` Temple Healer ↔ `14055`
- `14047` Mage Reagents ↔ `14057`
- `14061` Bank of Kowloon ↔ `14062`
- `14063` Pawn & Curios ↔ `14062`
- `14064` Provisioner (food/drink) ↔ `14065`
- `14066` Stablemaster ↔ `14065`
- `14067` Fletcher ↔ `14077`
- `14068` Jewel & Trinket ↔ `14078`
- `14081` Inn of Lantern Rain ↔ `14072`
- `14073` Training Hall ↔ `14072`
- `14074` Ship Chandler ↔ `14075`

**Rule:** no shop/service room has more than one connecting direction.

## Central Recall + Justice Room
- **Room `14055`:** `@@RCrimson-Jade Execution Square@@N`
- Flags/Sector: `recall_set`, city sector behavior for movement flavor.
- Required NPC reset: Executioner stationed here.
- Connected to four cardinal streets (`14045`, `14054`, `14056`, `14065`) plus service stubs where specified.

## Door Plan (explicit)
Only controlled perimeter gates use doors:
1. **North Gate:** `14005` north to wilderness link — closed door reset, unlockable.
2. **South Gate:** `14095` south to Roc Road/connector — closed door reset, unlockable.
3. **West Gate:** `14050` west to wilderness link — closed door reset, unlockable.
4. **East Gate:** `14059` east to harbor road/connector — closed door reset, unlockable.

No doors on interior shop entrances (to keep one-step service access clean).

## 10x10 Room Grid (room layout)
Coordinates: row `y=0..9` top-to-bottom, column `x=0..9` left-to-right.

| y\\x | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 |
|---|---|---|---|---|---|---|---|---|---|---|
| 0 | 14000 | 14001 | 14002 | 14003 | 14004 | 14005 | 14006 | 14007 | 14008 | 14009 |
| 1 | 14010 | 14011 | 14012 | 14013 | 14014 | 14015 | 14016 | 14017 | 14018 | 14019 |
| 2 | 14020 | 14021 | 14022 | 14023 | 14024 | 14025 | 14026 | 14027 | 14028 | 14029 |
| 3 | 14030 | 14031 | 14032 | 14033 | 14034 | 14035 | 14036 | 14037 | 14038 | 14039 |
| 4 | 14040 | 14041 | 14042 | 14043 | 14044 | 14045 | 14046 | 14047 | 14048 | 14049 |
| 5 | 14050 | 14051 | 14052 | 14053 | 14054 | 14055 | 14056 | 14057 | 14058 | 14059 |
| 6 | 14060 | 14061 | 14062 | 14063 | 14064 | 14065 | 14066 | 14067 | 14068 | 14069 |
| 7 | 14070 | 14071 | 14072 | 14073 | 14074 | 14075 | 14076 | 14077 | 14078 | 14079 |
| 8 | 14080 | 14081 | 14082 | 14083 | 14084 | 14085 | 14086 | 14087 | 14088 | 14089 |
| 9 | 14090 | 14091 | 14092 | 14093 | 14094 | 14095 | 14096 | 14097 | 14098 | 14099 |

## District/Room Assignment Plan
- **Perimeter Boulevard & Wall Ring:**
  - `14000-14009`, `14010`, `14019`, `14020`, `14029`, `14030`, `14039`, `14040`, `14049`, `14050`, `14059`, `14060`, `14069`, `14070`, `14079`, `14080`, `14089`, `14090-14099`
- **Civic + Recall Core:**
  - `14045`, `14054`, `14055`, `14056`, `14065`
- **Market Spine (west-central):**
  - Streets: `14041`, `14051`, `14052`, `14053`
  - Shops: `14042`, `14043`, `14044`
- **Temple/Arcane Spine (east-central):**
  - Streets: `14057`, `14058`
  - Services: `14046`, `14047`
- **Logistics + Finance (south-central):**
  - Streets: `14062`, `14075`, `14076`
  - Services: `14061`, `14063`, `14064`, `14066`, `14067`, `14068`, `14074`
- **Residential + Traveler Support:**
  - Streets: `14071`, `14072`
  - Services: `14073`, inn `14081`
- **Post + Admin (north-central):**
  - Streets: `14022`, `14023`, `14024`, `14025`
  - Service: `14021` postmaster
- **Remaining interior vnums not listed above:** standard city streets, plazas, alleys, patrol points, and flavor interiors while preserving rectangular connectivity.

## Connectivity Rules Summary
- Outdoor/city rooms follow rectangular cardinal adjacency unless blocked by a designated building wall.
- Shop/service interiors are strict one-exit dead-ends with exactly one bi-directional connection.
- Central recall room (`14055`) is the dominant hub and always reachable from all four quadrants through the two main spines.
- Gate rooms provide the only external area links and the only required doors.
