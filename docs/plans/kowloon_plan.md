# Area Plan: Kowloon

## Overview
- **Area Name:** `@@R@@xKowloon@@N`
- **File Name:** `kowloon.are`
- **Keyword:** `kowloon`
- **Intended Levels:** 1-5 (all player levels)
- **Vnum Range:** `3439-3538`
- **Vnum Count:** 100 (all room vnums used)
- **Shape:** 10x10 rectangle (full grid)
- **Recall Room:** `3494` (`recall_set` sector), central city plaza
- **Central Law NPC:** Executioner in `3494` (assigned `spec_executioner`)


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
- `L @@W(@@a1 5@@W)@@N~`
- `I 1 5`
- `V 3439 3538`
- `F 15`
- `U @@Rneon rain falls across @@GKowloon@@N~`
- `O Virant~`
- Optional builder directives (`N`, `X`, `R`, `W`, `T`) set during implementation.

## City Layout Concept (10x10 rectangle)
- Full rectangular footprint, all 100 vnums populated.
- Outer ring (`x=0`, `x=9`, `y=0`, `y=9`) is wall/gate boulevard.
- `3494` is exact center (row 5, col 5) as recall + civic nexus.
- Main spines:
 - **North/South Spine:** column 5 (vnums `3444, 3454, ... 3534`)
 - **West/East Spine:** row 5 (vnums `3489-3498`)
- Interior districts branch from spines: market, civic, docks, temples, barracks, inns.

## Shop + Service Requirements (all with one bi-directional exit only)
Every room below is an **inside-sector** (`sector_type = 11`) service room with exactly one entry exit to one **cardinally adjacent** street room, and one return exit back (bi-directional pair only):

- `3460` Postmaster of Kowloon ↔ `3461` (west/east)
- `3495` Quartermaster Depot ↔ `3494` (west/east)
- `3481` General Goods ↔ `3491` (south/north)
- `3482` Weapon Smith ↔ `3492` (south/north)
- `3483` Armor Smith ↔ `3493` (south/north)
- `3485` Temple Healer ↔ `3484` (east/west)
- `3486` Mage Reagents ↔ `3496` (south/north)
- `3500` Bank of Kowloon ↔ `3501` (east/west)
- `3502` Pawn & Curios ↔ `3501` (west/east)
- `3503` Provisioner (food/drink) ↔ `3504` (east/west)
- `3505` Stablemaster ↔ `3504` (west/east)
- `3506` Fletcher ↔ `3516` (south/north)
- `3507` Jewel & Trinket ↔ `3517` (south/north)
- `3520` Inn of Lantern Rain ↔ `3510` (north/south)
- `3512` Training Hall ↔ `3511` (west/east)
- `3513` Ship Chandler ↔ `3514` (east/west)

**Rule:** no shop/service room has more than one connecting direction. All service-to-street connections are between cardinally adjacent grid cells.

## Central Recall + Justice Room
- **Room `3494`:** `@@RCrimson-Jade Execution Square@@N`
- **Sector:** `recall_set` (sector_type `8`)
- **Room flags:** `no_mob` (boss/law NPC room per spec policy)
- Required NPC reset: Executioner stationed here.
- Connected to four cardinal streets: north `3484`, west `3493`, east `3495`, south `3504`.
- Note: `3495` (Quartermaster) and `3485` (Temple Healer) are service dead-ends branching off adjacent street/hub rooms, not through-streets.

## NPC Spec Compliance Notes
All mobile definitions must follow these spec requirements:
- **All mobs** must have `stay_area` (`64`) set in `act` flags.
- **Executioner**: `act` flags include `is_npc` (`1`), `sentinel` (`2`), `stay_area` (`64`). Assigned `spec_executioner` in `#SPECIALS`.
- **Postmaster**: `act` flags include `is_npc` (`1`), `sentinel` (`2`), `stay_area` (`64`), `postman` (`524288`).
- **Shopkeepers**: `act` flags include `is_npc` (`1`), `sentinel` (`2`), `stay_area` (`64`). Listed in `#SHOPS` with appropriate `buy_type` entries.
- **Healer NPC**: `act` flags include `is_npc` (`1`), `sentinel` (`2`), `stay_area` (`64`), `heal` (`4096`).
- **Bank NPC**: `act` flags include `is_npc` (`1`), `sentinel` (`2`), `stay_area` (`64`), `bank` (`32768`).
- **Trainer NPC**: `act` flags include `is_npc` (`1`), `sentinel` (`2`), `stay_area` (`64`), `train` (`512`).
- **Practice NPC**: `act` flags include `is_npc` (`1`), `sentinel` (`2`), `stay_area` (`64`), `practice` (`315`).
- **Boss mobs** (if any): must have `sentinel` + `boss` (`67108864`) and be placed only in rooms flagged `no_mob`.
- **Solo mobs** (strong non-boss): must have `solo` (`33554432`).
- Mobile `long_descr` must be exactly one text line followed by a newline and a `~`-only line. No multi-line `long_descr` is permitted.
- Mobile `description` must end with exactly one trailing newline before `~`.

## Door Plan (explicit, spec-compliant)
Only controlled perimeter gates use doors. All four gate doors follow the same pattern:

1. **Jade Gate (North):** room `3444` — direction north to wilderness link.
  - Room `D0` (north) exit line: `<locks>` includes `door` bit (`1`). `<key_vnum>` = `-1` (no key).
  - `#RESETS` command `D`: `D 0 3444 0 1` (door north, state `closed`).
  - Reverse exit on the wilderness-side room must also have `door` bit set.

2. **Iron Gate (South):** room `3534` — direction south to caravan road/connector.
  - Room `D2` (south) exit line: `<locks>` includes `door` bit (`1`). `<key_vnum>` = `-1` (no key).
  - `#RESETS` command `D`: `D 0 3534 2 1` (door south, state `closed`).
  - Reverse exit on the connector-side room must also have `door` bit set.

3. **Lantern Gate (West):** room `3489` — direction west to wilderness link.
  - Room `D3` (west) exit line: `<locks>` includes `door` bit (`1`). `<key_vnum>` = `-1` (no key).
  - `#RESETS` command `D`: `D 0 3489 3 1` (door west, state `closed`).
  - Reverse exit on the wilderness-side room must also have `door` bit set.

4. **Tide Gate (East):** room `3498` — direction east to harbor road/connector.
  - Room `D1` (east) exit line: `<locks>` includes `door` bit (`1`). `<key_vnum>` = `-1` (no key).
  - `#RESETS` command `D`: `D 0 3498 1 1` (door east, state `closed`).
  - Reverse exit on the connector-side room must also have `door` bit set.

All four gates reset to **closed** (state `1`), not locked. Since they are not locked on reset, no key objects are required (spec: key objects are only mandatory when `D` state = `2` locked). `<key_vnum>` is set to `-1` per spec for exits that cannot be locked/unlocked with a key.

No doors on interior shop entrances (single-step service access, no door bits).

## 10x10 Room Grid (room layout)
Coordinates: row `y=0..9` top-to-bottom, column `x=0..9` left-to-right.

| y\\x | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 |
|---|---|---|---|---|---|---|---|---|---|---|
| 0 | 3439 | 3440 | 3441 | 3442 | 3443 | 3444 | 3445 | 3446 | 3447 | 3448 |
| 1 | 3449 | 3450 | 3451 | 3452 | 3453 | 3454 | 3455 | 3456 | 3457 | 3458 |
| 2 | 3459 | 3460 | 3461 | 3462 | 3463 | 3464 | 3465 | 3466 | 3467 | 3468 |
| 3 | 3469 | 3470 | 3471 | 3472 | 3473 | 3474 | 3475 | 3476 | 3477 | 3478 |
| 4 | 3479 | 3480 | 3481 | 3482 | 3483 | 3484 | 3485 | 3486 | 3487 | 3488 |
| 5 | 3489 | 3490 | 3491 | 3492 | 3493 | 3494 | 3495 | 3496 | 3497 | 3498 |
| 6 | 3499 | 3500 | 3501 | 3502 | 3503 | 3504 | 3505 | 3506 | 3507 | 3508 |
| 7 | 3509 | 3510 | 3511 | 3512 | 3513 | 3514 | 3515 | 3516 | 3517 | 3518 |
| 8 | 3519 | 3520 | 3521 | 3522 | 3523 | 3524 | 3525 | 3526 | 3527 | 3528 |
| 9 | 3529 | 3530 | 3531 | 3532 | 3533 | 3534 | 3535 | 3536 | 3537 | 3538 |

## District/Room Assignment Plan
- **Perimeter Boulevard & Wall Ring:**
 - `3439-3448`, `3449`, `3458`, `3459`, `3468`, `3469`, `3478`, `3479`, `3488`, `3489`, `3498`, `3499`, `3508`, `3509`, `3518`, `3519`, `3528`, `3529-3538`
- **Civic + Recall Core:**
 - `3484`, `3493`, `3494`, `3495`, `3504`
- **Market Spine (west-central):**
 - Streets: `3480`, `3490`, `3491`, `3492`
 - Shops: `3481`, `3482`, `3483`
- **Temple/Arcane Spine (east-central):**
 - Streets: `3496`, `3497`
 - Services: `3485`, `3486`
- **Logistics + Finance (south-central):**
 - Streets: `3501`, `3514`, `3515`
 - Services: `3500`, `3502`, `3503`, `3505`, `3506`, `3507`, `3513`
- **Residential + Traveler Support:**
 - Streets: `3510`, `3511`
 - Services: `3512`, inn `3520`
- **Post + Admin (north-central):**
 - Streets: `3461`, `3462`, `3463`, `3464`
 - Service: `3460` postmaster
- **Remaining interior vnums not listed above:** standard city streets, plazas, alleys, patrol points, and flavor interiors while preserving rectangular connectivity.

## Connectivity Rules Summary
- Outdoor/city rooms follow rectangular cardinal adjacency unless blocked by a designated building wall.
- Shop/service interiors are strict one-exit dead-ends with exactly one bi-directional connection to a cardinally adjacent street room.
- Central recall room (`3494`) is the dominant hub and always reachable from all four quadrants through the two main spines.
- Gate rooms provide the only external area links and the only required doors.
- All exits are bi-directional (no one-way connections); this is not a maze area and no rooms use `ROOM_MAZE`.
- Directional loops are prohibited per spec (no repeated same-direction movement returning to a visited room).

## Safe-Room + Building Policy
- All indoor service rooms listed in **Shop + Service Requirements** are flagged `safe` and remain single-exit dead-ends.
- Outdoor roads, plazas, and gate approaches remain non-safe by default to preserve ambient city danger and law-enforcement relevance.
- `3494` (recall square) is **not** flagged `safe`; safety is enforced socially and by law NPC presence rather than room-level immunity.
- No interior room is used as a transit shortcut; each interior is entered for purpose (trade, healing, banking, lodging, training) and exited back to the same street node.

## NPC Level Targets (service vs ambient)
- **Service/legal NPCs:** level `150` baseline so city services are resilient and do not die to stray pulls.
- **Executioner (`3494`):** level `150` to match service-level parity while retaining legal enforcement authority.
- **Ambient civilians/workers:** level band `32-55` (`stay_area`, non-sentinel unless stationary worker).
- **District guards/patrols:** level band `85-120`, with stronger gate captains at `130-145`.
- **No boss-class city mobs** in initial pass; if later added, they must obey `boss` + `no_mob` placement policy in spec notes.

## Building Entrance Table (all current inside rooms)

| Inside Room | Name (working) | Entrance Street | Direction from street |
|---|---|---|---|
| 3460 | Kowloon Postmaster Hall | 3461 | west |
| 3481 | Green Banner General Goods | 3491 | south |
| 3482 | Ironhands Weapon Forge | 3492 | south |
| 3483 | Red Ledger Armory | 3493 | south |
| 3485 | Temple of the Unbroken Lamp | 3484 | east |
| 3486 | Azure Reagent Archive | 3496 | south |
| 3495 | Quartermaster Depot | 3494 | west |
| 3500 | Bank of Kowloon | 3501 | east |
| 3502 | Pawn and Curio Counter | 3501 | west |
| 3503 | Salt-Rice Provisioner | 3504 | east |
| 3505 | Iron Bit Stable Office | 3504 | west |
| 3506 | Rainline Fletcher | 3516 | south |
| 3507 | Pearl and Wire Jeweler | 3517 | south |
| 3512 | Ohn-Sek Training Hall | 3511 | west |
| 3513 | Harbor Chandlery Desk | 3514 | east |
| 3520 | Inn of Lantern Rain | 3510 | north |

## District Layout (by rectangle)
- **North Wall + Arrival Ring (`y=0..1`)**: gate processing, customs traffic, first-impression civic control.
- **Northern Market Belt (`y=2..4`, `x=1..8`)**: dense commerce, supply chain, legal-adjacent storefronts.
- **Civic Core (`x=4..6`, `y=4..6`)**: recall square, magistracy frontage, quartermaster, oath and punishment spaces.
- **Harbor/Logistics Side (`x=6..9`, `y=5..8`)**: chandlery, freight routing, dock administration, shipbound commerce.
- **Residential + Guild Support (`x=1..4`, `y=6..8`)**: inn, training hall, bank/pawn pairing, trades support.
- **South Wall Caravan Ring (`y=9`)**: outbound road staging and controlled gate egress.

## Shop Inventory Plan (thematic stock targets)
- **3481 General Goods:** torches, lantern oil, rope, waterskins, trail rations, bedrolls, lockpicks, writing kits.
- **3482 Weapon Forge:** blades, polearms, cudgels, compact sidearms favored by alley patrols.
- **3483 Armory:** layered coats, scale vests, shield variants, reinforced travel wear.
- **3486 Mage Reagents:** low-mid spell components, runic inks, ritual chalks, charm bases.
- **3503 Provisioner:** prepared meals, dry staples, tea, low-alcohol street drink.
- **3506 Fletcher:** arrows/bolts bundles and maintenance supplies.
- **3507 Jeweler:** trinkets, charms, signet rings, lightweight enchanted accessories.

All shop objects must remain within area vnum envelope policy for owned entries, use spec-compliant string termination, and avoid embedding vnums in description text.


### 3482 - Ironhands Weapon Forge (level 75-125 stock, 16 items)
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

### 3483 - Red Ledger Armory (level 75-125 stock, 16 items)
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

### 3507 - Pearl and Wire Jeweler (level 75-125 stock, 16 items)
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
- `3439-3478` mostly civic/arrival/law ambient mobs.
- `3479-3508` core service population and magistracy-facing law presence.
- `3509-3538` logistics, traveler, and gateward populations.

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
- `spec_executioner` assigned to central law NPC in `3494`.
- Optional future specs (only if needed after playtest): gate shout/call specs for captains, dock-crier behavior for harbor announcer.
- No combat gimmick specs on shop/service NPCs; keep services stable and predictable.

## Connection Plan (external links)
- **North (3444):** into northern overland/wilderness approach.
- **South (3534):** into caravan/trade road approach.
- **West (3489):** into inland district connector.
- **East (3498):** into harbor/coastal connector.

Connection constraints:
- Each gate is the **only** outward link on its wall segment.
- Reverse links must exist and must carry matching door semantics.
- Corner towers (`3439`, `3448`, `3529`, `3538`) remain internal fortification nodes with no wilderness bypass exits.

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
