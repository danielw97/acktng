# Area Plan: Shadowmere, the Blighted Realm (Full Redesign)

## Overview

**Area Name:** `@@dShadowmere, the @@pBlighted @@RCrownlands@@N` 
**File Name:** `shadowmere.are` 
**Keyword:** `shadowmere blight crownlands plague oathbound ruin` 
**Area Number:** `N 1400` 
**Level Range:** `I 30 60` 
**Vnum Range (fixed):** **1400-1600** (`V 1400 1600`) 
**Room Envelope Used:** **1400-1499 (100/100 rooms used)** 
**Mobile Envelope Used:** **1400-1460 (61/61 mobs used)** 
**Object Envelope Used:** **1400-1600 (201/201 objects used)**

**Primary Design Goal:** Rebuild Shadowmere into a coherent former Midgaard-aligned frontier march that collapsed during plague militarization, then transformed into a blight-soaked necro-political warzone where every room, mob, and object vnum is repurposed to support one connected narrative arc.

---

## Color Theme (Multi-Color, Lore-Locked)

- `@@W` **Pale White**: failed law, oaths, funerary record-stone.
- `@@p` **Violet**: Compact seals, warding doctrine, containment law.
- `@@R` **Blood Red**: active corruption, violence, sacrificial machinery.
- `@@a` **Sick Cyan**: plague runoff, alchemical seep, drowned channels.
- `@@d` **Ash Grey**: ruin, weathering, battlefield residue.
- `@@y` **Crown Gold**: old legitimacy, noble heraldry, broken authority.

Implementation constraints:
- Use `@@N` resets reliably.
- Do **not** use thematic `@@k`.
- Do not use background codes (`@@0`-`@@7`) or flashing (`@@f`).

---

## Area Header / Spec Compliance Checklist

Apply `docs/area_file_spec.md` exactly:

- `Q 16` required.
- `O Virant~` required (capitalization policy).
- Keep `V 1400 1600` unchanged.
- All strings `~` terminated.
- No blank-line doubles inside area strings.
- No vnums in in-world text.
- Mobile `long_descr` one-line format only.
- Mobile/room/object descriptions end with exactly one newline before `~`.

Proposed header strings:
- Name: `@@dShadowmere, the @@pBlighted @@RCrownlands@@N~`
- Level label: `@@p{@@W30@@p-@@W60@@p}@@N~`
- Reset msg: `@@dAsh drifts across broken oaths as @@pShadowmere@@d wakes to another watch of ruin.@@N~`

---

## Narrative Pillars

1. **Frontier of Law, Not Chaos:** Shadowmere began as a Crown-march enforcing Midgaard trade/security law at the edge of hostile wilds.
2. **The Plague Mandate:** During emergency years, civic and military powers fused, turning quarantine into permanent governance.
3. **The Oath-Fracture:** Competing authorities (crown, church, Compact, commanders) issued conflicting legal rites over life and death.
4. **Blight Hydrology:** Alchemical disposal and mass burial systems poisoned canals, crypts, and agricultural fringe.
5. **Living Ruin Politics:** Revenants, vampires, binders, deserters, and plague-adapted fauna all claim legitimacy from different historical layers.

---

## Midgaard Integration Requirements (Hard)

- Shadowmere must read as a **failed extension of Midgaard institutions**, not an isolated evil castle.
- Direct references in room/object/mob writing should echo:
 - Lantern-era registry habits (oath tablets, posted orders, watch rotations),
 - Violet Compact containment language,
 - Ash-and-Iron emergency requisition doctrine,
 - civic-memory disputes over who is owed burial, trial, or release.
- Expedition records in Shadowmere should mention Midgaard concerns: route safety, plague spillover, and archive legitimacy.

---

## Full Layout Redesign (100 Rooms)

Shadowmere is rebuilt into 10 districts of 10 rooms each.

| District | Vnums | Theme | Gameplay Role |
|---|---|---|---|
| I. Crown Gate March | 1400-1409 | Frontier entry, ruined authority | Orientation and first hostile patrols |
| II. Siege Courtworks | 1410-1419 | Keep gate and punitive infrastructure | Dense melee + trap pressure |
| III. Chapel of Broken Oaths | 1420-1429 | Ritual-legal corruption nexus | Caster/control encounters |
| IV. Catacomb Warline | 1430-1439 | Burial military logistics | Undead rank-and-file pressure |
| V. Crimson Household | 1440-1449 | Vampiric court occupation | Social-predator elites |
| VI. Gallows and Toxins | 1450-1459 | Execution economy + poison industry | Attrition and debuff corridor |
| VII. Necropolis Engine | 1460-1469 | Mass interment and binding works | Sustain fights and summoners |
| VIII. Fen-Labyrinth Reach | 1470-1479 | Exterior blight ecology | Navigation and ambush gameplay |
| IX. Outer Settlements in Ruin | 1480-1489 | Collapsed civic outskirts | Mixed patrol + lore recovery |
| X. Servitor Spine and Final Watch | 1490-1499 | Hidden logistics + terminal judgments | End-cap elites and finale |

---

## Room-by-Room Reallocation (All 1400-1499)

### I. Crown Gate March (1400-1409)
1400 Oath-Splintered Gate; 1401 Courtyard of Broken Edicts; 1402 Blight Rose Yard; 1403 Charnel Standard Grove; 1404 East March Wall; 1405 Ruined Watchfoot; 1406 Corpse-Tithe Road; 1407 Windward Signal Crown; 1408 Plague Sluice Trenches; 1409 Black Keep Forecourt.

### II. Siege Courtworks (1410-1419)
1410 Iron Intake Gatehouse; 1411 Hall of Requisition Banners; 1412 Tribunal Descent; 1413 Upper Interrogation Cells; 1414 Flooded Field Annex; 1415 Quarantine Crypt Dock; 1416 Mortuary Transit Hall; 1417 Implements Chamber; 1418 Blight Alchemy Annex; 1419 Deep Remand Vault.

### III. Chapel of Broken Oaths (1420-1429)
1420 Chapel Nave of the Last Witness; 1421 Compact Antechamber; 1422 Sanctum of the Crownless Lich; 1423 Ossuary Register Vault; 1424 Necromancer-Magistrate Study; 1425 Whisper Ledger Gallery; 1426 Mirror Testimony Corridor; 1427 Black Archive Stacks; 1428 Circle of Null Absolution; 1429 The Red Pit of Appeals.

### IV. Catacomb Warline (1430-1439)
1430 Catacomb Muster Gate; 1431 Upper Burial Ranks; 1432 Warren of Unclaimed Dead; 1433 Deep War Crypts; 1434 North Signal Tower; 1435 Southern Wallwalk; 1436 East Bastion Crown; 1437 Undercroft of Oath-Iron; 1438 Reliquary of Seized Names; 1439 Bone Forge of the March.

### V. Crimson Household (1440-1449)
1440 Hall of Night Petitions; 1441 Deployment Arcade; 1442 Antechamber of the Scarlet Court; 1443 Countess Regent's Audience; 1444 Sanguine Cellar; 1445 Old Cask Vaults; 1446 East Household Passage; 1447 Gallery of Watching Portraits; 1448 Arsenal of Silk and Steel; 1449 Barracks of Bound Retainers.

### VI. Gallows and Toxins (1450-1459)
1450 Poison Orchard Verge; 1451 Glasshouse of Viridian Rot; 1452 Venomwright Atelier; 1453 Gallows Square; 1454 Processing Hall of Sentence; 1455 Holding Pens of the Unheard; 1456 Necropolis Procession Gate; 1457 Avenue of Bellless Graves; 1458 Grand Mausoleum Threshold; 1459 Mausoleum Intake Court.

### VII. Necropolis Engine (1460-1469)
1460 Tomb of the First Marshal; 1461 Sunken Cemetery Rows; 1462 Crypt of Damned Claimants; 1463 Undying Watch Barracks; 1464 Binding Cells; 1465 Master Binder's Tribunal; 1466 Flooded Undercroft Works; 1467 Blackwater Reservoir; 1468 Shrine of Drowned Mercy; 1469 Ice Ledger Crypts.

### VIII. Fen-Labyrinth Reach (1470-1479)
1470 Frost Forge Relay; 1471 Labyrinth Gate of Stakes; 1472 Crooked Labyrinth Lanes; 1473 Heart of the Hedge-Maze; 1474 Shadowfen Crossing; 1475 Witch-Light Mere; 1476 Fen Shrine of Lost Lanterns; 1477 Screaming Stone Spire; 1478 Lower Howling Gallery; 1479 Upper Wail Chamber.

### IX. Outer Settlements in Ruin (1480-1489)
1480 Forsaken Commons; 1481 Mill of Ash Grain; 1482 Mill-Below Sump; 1483 Dark South Road; 1484 Collapsed Signal Tower; 1485 Rubble Depth Shaft; 1486 Chapel of Corroded Grace; 1487 Shrine of Red Corruption; 1488 East Outer Rampart; 1489 Gatehouse of Last Muster.

### X. Servitor Spine and Final Watch (1490-1499)
1490 West Outer Rampart; 1491 Servitors' Crawlways; 1492 Servitor Dormitory Ruin; 1493 Hidden Warrant Vault; 1494 Observatory Stair; 1495 Ruined Observatory Crown; 1496 Execution Grounds; 1497 Bell Tower of Final Count; 1498 Outer Gravefield; 1499 Northern Gravefield Verge.

---

## Mobile Redesign Plan (1400-1460)

Rebuild all 61 mob vnums as coherent faction ecology:

- **1400-1412:** March sentries, plague levy soldiers, oath clerks, condemned labor.
- **1413-1424:** gaol keepers, alchemical orderlies, tribunal adepts, compact wardens.
- **1425-1436:** ossuary priests, annalist wraiths, catacomb pikemen, bone-smiths.
- **1437-1448:** vampire household officers, enthralled retinue, duelists, blood stewards.
- **1449-1458:** poisoners, execution crews, necropolis processional captains.
- **1459-1460 (boss pair):**
 - 1459 **The Countess-Regent of Ashwine** (political apex of Crimson Household)
 - 1460 **Marshal-Lector Veyr, Oath of the Last Bell** (final authority claim)

Design rules:
- Enemy composition shifts from martial order -> ritual bureaucracy -> predatory aristocracy -> death-machine command.
- At least one signature elite per district with unique dialogue/prog cues.
- Boss speech should reference Midgaard law, failed Compact doctrine, and disputed legitimacy.

---

## Object Redesign Plan (1400-1600)

Rebuild all 201 object vnums into progression bands:

- **1400-1439:** march gear, registry tablets, ward-badges, low-tier martial kit.
- **1440-1479:** chapel/catacomb relics, anti-undead and anti-curse utility items.
- **1480-1529:** vampire court equipment, lifesteal/risk-reward itemization.
- **1530-1569:** poison and execution line gear, resist/cleanse tradeoffs.
- **1570-1594:** necropolis command relics, high-tier oath and control artifacts.
- **1595-1600:** capstone named pieces from Countess/Marshal/Observatory finale loop.

Itemization principles:
- Preserve archetype coverage (martial/rogue/caster/hybrid).
- Keep provenance text tied to Midgaard frontier governance and Shadowmere collapse.
- Ensure high-value relic text encodes faction dispute over rightful rule.

---

## Reset, Progression, and Encounter Cadence

- Preserve deterministic progression through districts I -> X.
- Provide loopback shortcuts only after district clears to reduce corpse-runs.
- Each district should include:
 - ambient patrol reset,
 - one elite anchor encounter,
 - one transition gate encounter.
- Final cadence:
 - defeat Countess-Regent line influence,
 - breach Servitor Spine command layer,
 - confront Marshal-Lector at Final Count axis.

---

## Build Checklist (Strict)

- [ ] Keep `V 1400 1600` unchanged.
- [ ] Use all room vnums `1400-1499`.
- [ ] Use all mobile vnums `1400-1460`.
- [ ] Use all object vnums `1400-1600`.
- [ ] Keep level band 30-60.
- [ ] Apply defined multi-color theme with `@@N` resets.
- [ ] No thematic `@@k` usage.
- [ ] Area owner set to `O Virant~`.
- [ ] No in-world vnum mentions in strings.
- [ ] Preserve explicit Midgaard institutional ties throughout lore-facing text.
