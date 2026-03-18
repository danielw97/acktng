# Area Plan: Shadowmere, the Blighted Realm (Full Redesign)

## Overview

**Area Name:** `@@dShadowmere, the @@pBlighted @@RCrownlands@@N` 
**File Name:** `shadowmere.are` 
**Keyword:** `shadowmere blight crownlands plague oathbound ruin` 
**Area Number:** `N 591` 
**Level Range:** `I 30 60` 
**Vnum Range (fixed):** **700-949** (`V 700 949`) 
**Room Envelope Used:** **591-690 (100/100 rooms used)** 
**Mobile Envelope Used:** **591-651 (61/61 mobs used)** 
**Object Envelope Used:** **700-949 (36/36 objects used)**

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
- Keep `V 700 949` unchanged.
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
5. **Living Ruin Politics:** Revenants, aristocratic remnants, binders, deserters, and plague-adapted fauna all claim legitimacy from different historical layers.

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
| I. Crown Gate March | 591-600 | Frontier entry, ruined authority | Orientation and first hostile patrols |
| II. Siege Courtworks | 601-610 | Keep gate and punitive infrastructure | Dense melee + trap pressure |
| III. Chapel of Broken Oaths | 611-620 | Ritual-legal corruption nexus | Caster/control encounters |
| IV. Catacomb Warline | 621-630 | Burial military logistics | Undead rank-and-file pressure |
| V. Crimson Household | 631-640 | Aristocratic court occupation | Social-predator elites |
| VI. Gallows and Toxins | 641-650 | Execution economy + poison industry | Attrition and debuff corridor |
| VII. Necropolis Engine | 651-660 | Mass interment and binding works | Sustain fights and summoners |
| VIII. Fen-Labyrinth Reach | 661-670 | Exterior blight ecology | Navigation and ambush gameplay |
| IX. Outer Settlements in Ruin | 671-680 | Collapsed civic outskirts | Mixed patrol + lore recovery |
| X. Servitor Spine and Final Watch | 681-690 | Hidden logistics + terminal judgments | End-cap elites and finale |

---

## Room-by-Room Reallocation (All 591-690)

### I. Crown Gate March (591-600)
591 Oath-Splintered Gate; 592 Courtyard of Broken Edicts; 593 Blight Rose Yard; 594 Charnel Standard Grove; 595 East March Wall; 596 Ruined Watchfoot; 597 Corpse-Tithe Road; 598 Windward Signal Crown; 599 Plague Sluice Trenches; 600 Black Keep Forecourt.

### II. Siege Courtworks (601-610)
601 Iron Intake Gatehouse; 602 Hall of Requisition Banners; 603 Tribunal Descent; 604 Upper Interrogation Cells; 605 Flooded Field Annex; 606 Quarantine Crypt Dock; 607 Mortuary Transit Hall; 608 Implements Chamber; 609 Blight Alchemy Annex; 610 Deep Remand Vault.

### III. Chapel of Broken Oaths (611-620)
611 Chapel Nave of the Last Witness; 612 Compact Antechamber; 613 Sanctum of the Crownless Lich; 614 Ossuary Register Vault; 615 Necromancer-Magistrate Study; 616 Whisper Ledger Gallery; 617 Mirror Testimony Corridor; 618 Black Archive Stacks; 619 Circle of Null Absolution; 620 The Red Pit of Appeals.

### IV. Catacomb Warline (621-630)
621 Catacomb Muster Gate; 622 Upper Burial Ranks; 623 Warren of Unclaimed Dead; 624 Deep War Crypts; 625 North Signal Tower; 626 Southern Wallwalk; 627 East Bastion Crown; 628 Undercroft of Oath-Iron; 629 Reliquary of Seized Names; 630 Bone Forge of the March.

### V. Crimson Household (631-640)
631 Hall of Night Petitions; 632 Deployment Arcade; 633 Antechamber of the Scarlet Court; 634 Countess Regent's Audience; 635 Sanguine Cellar; 636 Old Cask Vaults; 637 East Household Passage; 638 Gallery of Watching Portraits; 639 Arsenal of Silk and Steel; 640 Barracks of Bound Retainers.

### VI. Gallows and Toxins (641-650)
641 Poison Orchard Verge; 642 Glasshouse of Viridian Rot; 643 Venomwright Atelier; 644 Gallows Square; 645 Processing Hall of Sentence; 646 Holding Pens of the Unheard; 647 Necropolis Procession Gate; 648 Avenue of Bellless Graves; 649 Grand Mausoleum Threshold; 650 Mausoleum Intake Court.

### VII. Necropolis Engine (651-660)
651 Tomb of the First Marshal; 652 Sunken Cemetery Rows; 653 Crypt of Damned Claimants; 654 Undying Watch Barracks; 655 Binding Cells; 656 Master Binder's Tribunal; 657 Flooded Undercroft Works; 658 Blackwater Reservoir; 659 Shrine of Drowned Mercy; 660 Ice Ledger Crypts.

### VIII. Fen-Labyrinth Reach (661-670)
661 Frost Forge Relay; 662 Labyrinth Gate of Stakes; 663 Crooked Labyrinth Lanes; 664 Heart of the Hedge-Maze; 665 Shadowfen Crossing; 666 Witch-Light Mere; 667 Fen Shrine of Lost Lanterns; 668 Screaming Stone Spire; 669 Lower Howling Gallery; 670 Upper Wail Chamber.

### IX. Outer Settlements in Ruin (671-680)
671 Forsaken Commons; 672 Mill of Ash Grain; 673 Mill-Below Sump; 674 Dark South Road; 675 Collapsed Signal Tower; 676 Rubble Depth Shaft; 677 Chapel of Corroded Grace; 678 Shrine of Red Corruption; 679 East Outer Rampart; 680 Gatehouse of Last Muster.

### X. Servitor Spine and Final Watch (681-690)
681 West Outer Rampart; 682 Servitors' Crawlways; 683 Servitor Dormitory Ruin; 684 Hidden Warrant Vault; 685 Observatory Stair; 686 Ruined Observatory Crown; 687 Execution Grounds; 688 Bell Tower of Final Count; 689 Outer Gravefield; 690 Northern Gravefield Verge.

---

## Mobile Redesign Plan (591-651)

Rebuild all 61 mob vnums as coherent faction ecology:

- **591-603:** March sentries, plague levy soldiers, oath clerks, condemned labor.
- **604-615:** gaol keepers, alchemical orderlies, tribunal adepts, compact wardens.
- **616-627:** ossuary priests, annalist wraiths, catacomb pikemen, bone-smiths.
- **628-639:** Crimson Household officers, enthralled retinue, duelists, blood stewards.
- **640-649:** poisoners, execution crews, necropolis processional captains.
- **650-651 (boss pair):**
 - 650 **The Countess-Regent of Ashwine** (political apex of Crimson Household)
 - 651 **Marshal-Lector Veyr, Oath of the Last Bell** (final authority claim)

Design rules:
- Enemy composition shifts from martial order -> ritual bureaucracy -> predatory aristocracy -> death-machine command.
- At least one signature elite per district with unique dialogue/prog cues.
- Boss speech should reference Midgaard law, failed Compact doctrine, and disputed legitimacy.

---

## Object Redesign Plan (700-949)

Rebuild all 36 object vnums into progression bands:

- **591-630:** march gear, registry tablets, ward-badges, low-tier martial kit.
- **631-670:** chapel/catacomb relics, anti-undead and anti-curse utility items.
- **671-720:** Crimson Household court equipment, lifesteal/risk-reward itemization.
- **721-760:** poison and execution line gear, resist/cleanse tradeoffs.
- **761-785:** necropolis command relics, high-tier oath and control artifacts.
- **786-791:** capstone named pieces from Countess/Marshal/Observatory finale loop.

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

- [ ] Keep `V 700 949` unchanged.
- [ ] Use all room vnums `591-690`.
- [ ] Use all mobile vnums `591-651`.
- [ ] Use all object vnums `700-949`.
- [ ] Keep level band 30-60.
- [ ] Apply defined multi-color theme with `@@N` resets.
- [ ] No thematic `@@k` usage.
- [ ] Area owner set to `O Virant~`.
- [ ] No in-world vnum mentions in strings.
- [ ] Preserve explicit Midgaard institutional ties throughout lore-facing text.
