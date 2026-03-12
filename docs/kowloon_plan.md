# Area Plan: Kowloon

## Overview
- **Area Name:** `@@R@@xKowloon@@N`
- **File Name:** `kowloon.are`
- **Keyword:** `kowloon`
- **Intended Levels:** 1-170 (all player levels)
- **Vnum Range:** `14000-14099`
- **Vnum Count:** 100 (all room vnums used)
- **Shape:** 10x10 rectangle (full grid)
- **Recall Room:** `14055` (`recall_set` sector), central city plaza
- **Central Law NPC:** Executioner in `14055` (assigned `spec_executioner`)


## Lore
See `kowloon_lore.md` for extracted lore.

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
Every room below is an **inside-sector** (`sector_type = 11`) service room with exactly one entry exit to one **cardinally adjacent** street room, and one return exit back (bi-directional pair only):

- `14021` Postmaster of Kowloon ↔ `14022` (west/east)
- `14056` Quartermaster Depot ↔ `14055` (west/east)
- `14042` General Goods ↔ `14052` (south/north)
- `14043` Weapon Smith ↔ `14053` (south/north)
- `14044` Armor Smith ↔ `14054` (south/north)
- `14046` Temple Healer ↔ `14045` (east/west)
- `14047` Mage Reagents ↔ `14057` (south/north)
- `14061` Bank of Kowloon ↔ `14062` (east/west)
- `14063` Pawn & Curios ↔ `14062` (west/east)
- `14064` Provisioner (food/drink) ↔ `14065` (east/west)
- `14066` Stablemaster ↔ `14065` (west/east)
- `14067` Fletcher ↔ `14077` (south/north)
- `14068` Jewel & Trinket ↔ `14078` (south/north)
- `14081` Inn of Lantern Rain ↔ `14071` (north/south)
- `14073` Training Hall ↔ `14072` (west/east)
- `14074` Ship Chandler ↔ `14075` (east/west)

**Rule:** no shop/service room has more than one connecting direction. All service-to-street connections are between cardinally adjacent grid cells.

## Central Recall + Justice Room
- **Room `14055`:** `@@RCrimson-Jade Execution Square@@N`
- **Sector:** `recall_set` (sector_type `8`)
- **Room flags:** `no_mob` (boss/law NPC room per spec policy)
- Required NPC reset: Executioner stationed here.
- Connected to four cardinal streets: north `14045`, west `14054`, east `14056`, south `14065`.
- Note: `14056` (Quartermaster) and `14046` (Temple Healer) are service dead-ends branching off adjacent street/hub rooms, not through-streets.

## NPC Spec Compliance Notes
All mobile definitions must follow these spec requirements:
- **All mobs** must have `stay_area` (`64`) set in `act` flags.
- **Executioner**: `act` flags include `is_npc` (`1`), `sentinel` (`2`), `stay_area` (`64`). Assigned `spec_executioner` in `#SPECIALS`.
- **Postmaster**: `act` flags include `is_npc` (`1`), `sentinel` (`2`), `stay_area` (`64`), `postman` (`524288`).
- **Shopkeepers**: `act` flags include `is_npc` (`1`), `sentinel` (`2`), `stay_area` (`64`). Listed in `#SHOPS` with appropriate `buy_type` entries.
- **Healer NPC**: `act` flags include `is_npc` (`1`), `sentinel` (`2`), `stay_area` (`64`), `heal` (`4096`).
- **Bank NPC**: `act` flags include `is_npc` (`1`), `sentinel` (`2`), `stay_area` (`64`), `bank` (`32768`).
- **Trainer NPC**: `act` flags include `is_npc` (`1`), `sentinel` (`2`), `stay_area` (`64`), `train` (`512`).
- **Practice NPC**: `act` flags include `is_npc` (`1`), `sentinel` (`2`), `stay_area` (`64`), `practice` (`1024`).
- **Boss mobs** (if any): must have `sentinel` + `boss` (`67108864`) and be placed only in rooms flagged `no_mob`.
- **Solo mobs** (strong non-boss): must have `solo` (`33554432`).
- Mobile `long_descr` must be exactly one text line followed by a newline and a `~`-only line. No multi-line `long_descr` is permitted.
- Mobile `description` must end with exactly one trailing newline before `~`.

## Door Plan (explicit, spec-compliant)
Only controlled perimeter gates use doors. All four gate doors follow the same pattern:

1. **Jade Gate (North):** room `14005` — direction north to wilderness link.
   - Room `D0` (north) exit line: `<locks>` includes `door` bit (`1`). `<key_vnum>` = `-1` (no key).
   - `#RESETS` command `D`: `D 0 14005 0 1` (door north, state `closed`).
   - Reverse exit on the wilderness-side room must also have `door` bit set.

2. **Iron Gate (South):** room `14095` — direction south to caravan road/connector.
   - Room `D2` (south) exit line: `<locks>` includes `door` bit (`1`). `<key_vnum>` = `-1` (no key).
   - `#RESETS` command `D`: `D 0 14095 2 1` (door south, state `closed`).
   - Reverse exit on the connector-side room must also have `door` bit set.

3. **Lantern Gate (West):** room `14050` — direction west to wilderness link.
   - Room `D3` (west) exit line: `<locks>` includes `door` bit (`1`). `<key_vnum>` = `-1` (no key).
   - `#RESETS` command `D`: `D 0 14050 3 1` (door west, state `closed`).
   - Reverse exit on the wilderness-side room must also have `door` bit set.

4. **Tide Gate (East):** room `14059` — direction east to harbor road/connector.
   - Room `D1` (east) exit line: `<locks>` includes `door` bit (`1`). `<key_vnum>` = `-1` (no key).
   - `#RESETS` command `D`: `D 0 14059 1 1` (door east, state `closed`).
   - Reverse exit on the connector-side room must also have `door` bit set.

All four gates reset to **closed** (state `1`), not locked. Since they are not locked on reset, no key objects are required (spec: key objects are only mandatory when `D` state = `2` locked). `<key_vnum>` is set to `-1` per spec for exits that cannot be locked/unlocked with a key.

No doors on interior shop entrances (single-step service access, no door bits).

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
- Shop/service interiors are strict one-exit dead-ends with exactly one bi-directional connection to a cardinally adjacent street room.
- Central recall room (`14055`) is the dominant hub and always reachable from all four quadrants through the two main spines.
- Gate rooms provide the only external area links and the only required doors.
- All exits are bi-directional (no one-way connections); this is not a maze area and no rooms use `ROOM_MAZE`.
- Directional loops are prohibited per spec (no repeated same-direction movement returning to a visited room).

## Safe-Room + Building Policy
- All indoor service rooms listed in **Shop + Service Requirements** are flagged `safe` and remain single-exit dead-ends.
- Outdoor roads, plazas, and gate approaches remain non-safe by default to preserve ambient city danger and law-enforcement relevance.
- `14055` (recall square) is **not** flagged `safe`; safety is enforced socially and by law NPC presence rather than room-level immunity.
- No interior room is used as a transit shortcut; each interior is entered for purpose (trade, healing, banking, lodging, training) and exited back to the same street node.

## NPC Level Targets (service vs ambient)
- **Service/legal NPCs:** level `150` baseline so city services are resilient and do not die to stray pulls.
- **Executioner (`14055`):** level `150` to match service-level parity while retaining legal enforcement authority.
- **Ambient civilians/workers:** level band `32-55` (`stay_area`, non-sentinel unless stationary worker).
- **District guards/patrols:** level band `85-120`, with stronger gate captains at `130-145`.
- **No boss-class city mobs** in initial pass; if later added, they must obey `boss` + `no_mob` placement policy in spec notes.

## Building Entrance Table (all current inside rooms)

| Inside Room | Name (working) | Entrance Street | Direction from street |
|---|---|---|---|
| 14021 | Kowloon Postmaster Hall | 14022 | west |
| 14042 | Green Banner General Goods | 14052 | south |
| 14043 | Ironhands Weapon Forge | 14053 | south |
| 14044 | Red Ledger Armory | 14054 | south |
| 14046 | Temple of the Unbroken Lamp | 14045 | east |
| 14047 | Azure Reagent Archive | 14057 | south |
| 14056 | Quartermaster Depot | 14055 | west |
| 14061 | Bank of Kowloon | 14062 | east |
| 14063 | Pawn and Curio Counter | 14062 | west |
| 14064 | Salt-Rice Provisioner | 14065 | east |
| 14066 | Iron Bit Stable Office | 14065 | west |
| 14067 | Rainline Fletcher | 14077 | south |
| 14068 | Pearl and Wire Jeweler | 14078 | south |
| 14073 | Ohn-Sek Training Hall | 14072 | west |
| 14074 | Harbor Chandlery Desk | 14075 | east |
| 14081 | Inn of Lantern Rain | 14071 | north |

## District Layout (by rectangle)
- **North Wall + Arrival Ring (`y=0..1`)**: gate processing, customs traffic, first-impression civic control.
- **Northern Market Belt (`y=2..4`, `x=1..8`)**: dense commerce, supply chain, legal-adjacent storefronts.
- **Civic Core (`x=4..6`, `y=4..6`)**: recall square, magistracy frontage, quartermaster, oath and punishment spaces.
- **Harbor/Logistics Side (`x=6..9`, `y=5..8`)**: chandlery, freight routing, dock administration, shipbound commerce.
- **Residential + Guild Support (`x=1..4`, `y=6..8`)**: inn, training hall, bank/pawn pairing, trades support.
- **South Wall Caravan Ring (`y=9`)**: outbound road staging and controlled gate egress.

## Shop Inventory Plan (thematic stock targets)
- **14042 General Goods:** torches, lantern oil, rope, waterskins, trail rations, bedrolls, lockpicks, writing kits.
- **14043 Weapon Forge:** blades, polearms, cudgels, compact sidearms favored by alley patrols.
- **14044 Armory:** layered coats, scale vests, shield variants, reinforced travel wear.
- **14047 Mage Reagents:** low-mid spell components, runic inks, ritual chalks, charm bases.
- **14064 Provisioner:** prepared meals, dry staples, tea, low-alcohol street drink.
- **14067 Fletcher:** arrows/bolts bundles and maintenance supplies.
- **14068 Jeweler:** trinkets, charms, signet rings, lightweight enchanted accessories.

All shop objects must remain within area vnum envelope policy for owned entries, use spec-compliant string termination, and avoid embedding vnums in description text.


### 14043 - Ironhands Weapon Forge (level 75-125 stock, 16 items)
- Riverguard Falchion (Lv 75)
- Lanternhook Cutlass (Lv 80)
- Covenant Spear (Lv 80)
- Brass-Knuckle Baton (Lv 85)
- Ohn-Sek War Mace (Lv 85)
- Tidepiercer Trident (Lv 90)
- Magistrate Execution Axe (Lv 90)
- Stormbell Halberd (Lv 95)
- Smuggler's Splitblade (Lv 100)
- Black-Rain Greatsword (Lv 100)
- Harbor Chain Whip (Lv 105)
- Six-House Glaive (Lv 110)
- Dragon-Mask Reaver (Lv 115)
- Coppersalt Pike (Lv 120)
- Covenant Oathblade (Lv 125)
- Neon Verdict Poleaxe (Lv 125)

### 14044 - Red Ledger Armory (level 75-125 stock, 16 items)
- Tar-Sealed Leather Coat (Lv 75)
- Riveted Street Brigandine (Lv 80)
- Lanternglass Buckler (Lv 80)
- Syndic Scale Vest (Lv 85)
- Floodline Greaves (Lv 85)
- Harborwatch Helm (Lv 90)
- Rainwarden Vambraces (Lv 90)
- Jade-Marked Kite Shield (Lv 95)
- Ohn-Sek Plate Skirt (Lv 100)
- Gatehouse Half-Plate (Lv 100)
- Coppersalt Tower Shield (Lv 105)
- Ashfall Proof Cloak (Lv 110)
- Magistracy Full Cuirass (Lv 115)
- Dragon-Ridge Greatshield (Lv 120)
- Covenant Guard Panoply (Lv 125)
- Execution Square Bulwark (Lv 125)

### 14068 - Pearl and Wire Jeweler (level 75-125 stock, 16 items)
- Wirebound River Pearl Ring (Lv 75)
- Lampkeeper Copper Torque (Lv 80)
- Harbor Seal Signet (Lv 80)
- Rainchime Silver Pendant (Lv 85)
- Jade Ledger Charm (Lv 85)
- Temple Ember Brooch (Lv 90)
- Lanternrunner Wrist Chain (Lv 90)
- Coppersalt Moon Earrings (Lv 95)
- Covenant Knot Necklace (Lv 100)
- Stormglass Focus Ring (Lv 100)
- Magistrate Seal Locket (Lv 105)
- Tidewire Sapphire Circlet (Lv 110)
- Dragon Pearl Choker (Lv 115)
- Ashfall Memory Torque (Lv 120)
- Five-Seats Diadem (Lv 125)
- Neon Covenant Crownlet (Lv 125)

## Mobile Definitions (planning set)
- **Service core (sentinel):** executioner, postmaster, healer, banker, trainer, practice master, and all shopkeepers.
- **Law layer:** gate sergeants (4), wall patrol pairs (8), plaza inspectors (2).
- **Ambient layer:** dockhands, lantern-runners, scribes, food vendors, shrine keepers, caravan callers.
- **Hostile/pressure layer (light):** pickpocket crews and smuggler lookouts in non-safe alleys to keep city traversal active.

Planned implementation split:
- `14000-14039` mostly civic/arrival/law ambient mobs.
- `14040-14069` core service population and magistracy-facing law presence.
- `14070-14099` logistics, traveler, and gateward populations.

## Resets Plan
### Mob resets (`M`)
- One persistent reset for every service NPC room.
- Gate rooms get at least one guard reset each; major gates get captain + guard pairing.
- Ambient mobs distributed with low saturation to prevent crowding in narrow streets.

### Door resets (`D`)
- Exactly four door resets, one for each perimeter gate as specified in **Door Plan**.
- All reset state values set to `1` (closed), never `2` (locked), consistent with no-key gate policy.

### Object resets (`O`/`G`/`E`)
- Place civic flavor objects in plaza/wall rooms (`O`).
- Equip sentinel guards with thematic but controlled gear (`G`/`E`) without exceeding intended level power.
- Reserve object resets for atmosphere first; avoid cluttering service interiors where NPC utility is primary.

## Specials Plan
- `spec_executioner` assigned to central law NPC in `14055`.
- Optional future specs (only if needed after playtest): gate shout/call specs for captains, dock-crier behavior for harbor announcer.
- No combat gimmick specs on shop/service NPCs; keep services stable and predictable.

## Connection Plan (external links)
- **North (14005):** into northern overland/wilderness approach.
- **South (14095):** into caravan/trade road approach.
- **West (14050):** into inland district connector.
- **East (14059):** into harbor/coastal connector.

Connection constraints:
- Each gate is the **only** outward link on its wall segment.
- Reverse links must exist and must carry matching door semantics.
- Corner towers (`14000`, `14009`, `14090`, `14099`) remain internal fortification nodes with no wilderness bypass exits.

## Implementation Checklist
- Finalize all 100 room names + unique descriptions (3+ sentences each, 5+ for key hubs).
- Build exits with strict rectangular logic and dead-end interiors.
- Create service mobiles with required `act` bits (`is_npc`, `stay_area`, role-specific flags).
- Author shop inventories and assign buy types per shop role.
- Add four gate door resets and validate close-state behavior.
- Validate area header directives (`Q 16`, `O Virant~`, vnum bounds, level band).
- Run format lint pass for string terminators and mobile description constraints.

## Room Description Requirements (spec compliance)
- Every room `<description>~` must contain at least 3 sentences.
- Important rooms (recall square, gate rooms, major service hubs) must contain at least 5 sentences.
- Every room description must be unique (no reused descriptions across rooms).
- Vnums must never appear in any in-world description text (room descriptions, mob descriptions, object descriptions, extra descriptions, exit descriptions).
- Named exits (`<exit_keyword>`) must be discoverable in the room's `<description>~`, an object in the room, or an `E` extra description.
- Extra-description keyword chains must be anchored: at least one keyword in each chain must appear in the room's main `<description>~`.
- Room descriptions must use the area's color theme intentionally and consistently.
