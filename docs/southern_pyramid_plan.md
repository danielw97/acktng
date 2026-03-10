# Area Plan: The Southern Pyramid

## Overview

**Area Name:** `@@ySouthern @@bPyramid@@N`
**File Name:** `southern_pyramid.are`
**Keyword:** `southernpyramid`
**Level Range:** 120–140 (I directive: `I 120 140`)
**Level Label:** `@@W(@@y120 140@@W)@@N`
**Vnum Range:** 11450–11649 (V directive: `V 11450 11649`)
**Vnum Count:** 200
**Area Number:** `N 76`
**Reset Rate:** `F 20`
**Reset Message:** `@@yancient sand cascades through the Southern Pyramid as sealed chambers exhale stale air@@N`

### Color Theme

- **Primary:** `@@y` (yellow) — sun-scorched sandstone, golden sarcophagi, amber torch-glow
- **Secondary:** `@@b` (brown) — cracked clay walls, petrified timber beams, dried mud mortar
- **Accent:** `@@e` (dark green) — jade inlay, verdigris-stained copper, poisonous lichen
- **Danger:** `@@R` (red) — cursed blood sigils, volcanic vents, boss threat indicators
- **Text reset:** `@@N` after all color sequences

The Southern Pyramid is older and deeper than its northern twin. Where the Northern Pyramid rises into the sky, the Southern Pyramid burrows downward — its visible capstone is merely the entrance to a vast inverted structure descending into the earth. The deeper one goes, the hotter the air grows, until the lowest chambers border on volcanic vents. The architect of this pyramid was the Moon Sovereign, rival and twin of the Sand Sovereign, who built downward to be closer to the underworld and the source of all death magic.

---

## Narrative / Lore

The Southern Pyramid was sealed a century before the Northern Pyramid. The Moon Sovereign — twin sibling and bitter rival of the Sand Sovereign — rejected the sky and the sun, choosing instead to dig toward the roots of the world. The pyramid's visible capstone rises only three stories above the desert floor, but beneath it plunge seven descending tiers carved from living bedrock, each tier older and stranger than the last.

The Moon Sovereign believed that death magic grew stronger the closer one drew to the underworld. Each descending tier was consecrated to a different aspect of death: silence, decay, shadow, bone, venom, fire, and void. Seven wardens were bound to these tiers, cursed to guard them until the world itself ended. The Moon Sovereign rests at the deepest point, entombed in a chamber of volcanic glass above a lake of molten stone.

Traders at the Southern Oasis whisper that the ground sometimes trembles, and that on windless nights a low moan rises from the dunes south of the settlement. Expedition parties have found the pyramid's capstone entrance, but few have returned from below the second tier.

---

## Area Header Notes

- `Q 16` — mandatory version
- `K southernpyramid~` — single-word keyword, no spaces, no color codes
- `L @@W(@@y120 140@@W)@@N~` — white parens, yellow level numbers, white close paren, reset
- `N 76` — confirmed unused across all loaded area files
- `I 120 140` — area-gating and mob-level policy
- `V 11450 11649` — full 200-vnum envelope; all rooms, mobs, and objects stay within this range
- `F 20` — 20-minute reset frequency
- `U` — reset message as above; no double-newlines, terminated with `~`
- `T` — teleport flag line (rest ignored by loader)

---

## Level Structure

Seven descending tiers plus a trap room. Unlike the Northern Pyramid which ascends, this pyramid descends — players go **down** through the tiers. The capstone (ground level) is the entrance; each subsequent tier is reached by going down.

| Level | Name | Vnums | Rooms | Boss Room |
|-------|------|-------|-------|-----------|
| Trap Room | The Crushing Hall | 11450 | 1 | — (no_mob) |
| Ground Level / Capstone | The Moon Gate | 11451–11495 | 45 | 11495 |
| Tier 1 (Silence) | The Silent Crypts | 11496–11530 | 35 | 11530 |
| Tier 2 (Decay) | The Rotting Galleries | 11531–11560 | 30 | 11560 |
| Tier 3 (Shadow) | The Shadow Nave | 11561–11585 | 25 | 11585 |
| Tier 4 (Bone) | The Ossuary Depths | 11586–11605 | 20 | 11605 |
| Tier 5 (Venom) | The Venomous Cisterns | 11606–11625 | 20 | 11625 |
| Tier 6 (Fire) | The Molten Sanctum | 11626–11639 | 14 | 11639 |
| Tier 7 / Nadir | The Void Throne | 11640–11649 | 10 | 11649 |

**Total rooms: 200** (all vnums 11450–11649 used)

---

## Connection to the Southern Oasis

The Southern Pyramid connects bidirectionally to **room 9800** ("Sunforged Causeway of Lantern") in `the_southern_oasis.are`. Room 9800 already describes "a precise triangular rise foreshadows a future pyramid road" to the south and has a D2 (south) exit to room 9810. The pyramid connection will use a **new D2 exit** from a room that currently lacks one, or add the pyramid as a branching path.

Since room 9800's D2 is already used (→ 9810), we will connect via room 9819 ("Reedbound Span of Lantern"), which also mentions the pyramid to the south. Room 9819 currently has exits D0→9809, D3→9818. Its D2 (south) is **free**.

| Direction | From | To | Notes |
|-----------|------|----|-------|
| south (D2) | 9819 (Reedbound Span of Lantern, Southern Oasis) | 11451 (Before the Southern Pyramid) | Players leave the oasis heading south |
| north (D0) | 11451 (Before the Southern Pyramid) | 9819 (Reedbound Span of Lantern, Southern Oasis) | Return path to the oasis |

### Required Edit to `the_southern_oasis.are`

Room 9819 currently has exits D0→9809, D3→9818. Add a D2 (south) exit:

```
D2
The ancient road continues south through the dunes, where a @@bsandstone@@N capstone rises against the sky.

~
^ancient road south~
0 -1 11451
```

The room description of 9819 already mentions the pyramid ("To the south, a precise triangular rise foreshadows a future pyramid road") so the named exit is discoverable.

Room 11451 (the pyramid entry room) receives a D0 (north) exit back to 9819:

```
D0
The ancient road curves north through the dunes, back toward the shade of the Southern Oasis.

~
~
0 -1 9819
```

---

## Room Layout (200 rooms)

### Room Flag Policy

- **`no_teleport`**: Applied to **all 200 rooms**. Players and mobs cannot be teleported into any room in the pyramid.
- **`no_recall`**: Applied to **all rooms except** inter-tier stairwell/transit rooms and the final boss room (11649). Stairwells and 11649 allow recall so players can escape between tiers and exit after defeating the final boss.

Inter-tier rooms that are **exempt from `no_recall`** (have only `no_mob, no_teleport, indoors`): 11451, 11472, 11479, 11496, 11497, 11510, 11531, 11532, 11549, 11561, 11562, 11576, 11586, 11587, 11601, 11606, 11620, 11626, 11640, 11649.

All other rooms (including boss rooms 11495, 11530, 11560, 11585, 11605, 11625, 11639) carry `no_recall`.

---

### Zone 0: The Crushing Hall (1 room: 11450)

This special room exists outside the pyramid's normal vertical progression. Hidden pressure plates, collapsing floors, and disguised pit covers scattered throughout the pyramid all funnel into this one chamber. It has **no exit except D5 (down) to 11509** in the Silent Crypts. Players who fall in must navigate the Silent Crypts to escape.

| Vnum | Name | Sector | Flags |
|------|------|--------|-------|
| 11450 | The Crushing Hall | inside | no_mob, indoors, no_recall, no_teleport |

**Exits:**
- D5 (down) → 11509 (The Trap-Fall Landing in Silent Crypts); no keyword; no door

**One-way trap exits leading IN to 11450 from across the pyramid (these rooms have exits pointing to 11450; 11450 has no reciprocal exit back to them):**

| Source Room | Vnum | Direction | Notes |
|-------------|------|-----------|-------|
| The Cracked Flagstone Hall (Ground Level) | 11473 | D5 (down) | Hidden pit, no keyword |
| The False Stairwell (Tier 1) | 11515 | D5 (down) | Appears to lead deeper but drops sideways |
| The Unstable Gallery (Tier 2) | 11545 | D5 (down) | Pressure plate collapse |
| The Treacherous Ledge (Tier 3) | 11572 | D5 (down) | False floor gives way |

Each source room description must describe the hazard (cracked floor, suspicious passage, etc.) without explicitly naming the trap destination, so players can discover it as a risk.

---

### Zone 1: The Moon Gate — Ground Level (45 rooms: 11451–11495)

The capstone structure visible above the desert and the first underground tier. Exterior rooms use `desert` sector; interior rooms use `inside` sector. This is the widest and most populated floor.

| Vnum | Room Name | Sector | Flags | Notes |
|------|-----------|--------|-------|-------|
| 11451 | Before the Southern Pyramid | desert | no_mob, no_teleport, indoors | **Entry from Southern Oasis** (D0 north → 9819) |
| 11452 | The Ancient Road South | desert | no_recall, no_teleport | Approach causeway |
| 11453 | The Outer Dune Ring | desert | no_recall, no_teleport | Sand dunes circle the capstone |
| 11454 | The Western Approach | desert | no_recall, no_teleport | Sheer weathered stone face |
| 11455 | The Eastern Approach | desert | no_recall, no_teleport | Hieroglyphs etched by wind |
| 11456 | The Capstone Forecourt | desert | no_recall, no_teleport | Wide plaza before the descending entrance |
| 11457 | The Obelisk Avenue | desert | no_recall, no_teleport | Broken obelisks line a sunken road |
| 11458 | The Sentinel Ruin | desert | no_recall, no_teleport | Collapsed guard station |
| 11459 | The Capstone Gate | desert | no_recall, no_teleport | Massive carved entrance arch; `^moon gate` (closed door, key -1) |
| 11460 | The Descending Vestibule | inside | no_recall, no_teleport, indoors | First interior room; stairs descend immediately |
| 11461 | The East Entry Corridor | inside | no_recall, no_teleport, indoors | |
| 11462 | The West Entry Corridor | inside | no_recall, no_teleport, indoors | |
| 11463 | The Pillar Hall | inside | no_recall, no_teleport, indoors | Thick columns support the capstone ceiling |
| 11464 | A Rubble-Choked Wing | inside | no_recall, no_teleport, indoors | Ceiling partially collapsed |
| 11465 | The Offering Alcove | inside | no_recall, no_teleport, indoors | Carved niches for tribute |
| 11466 | The Moon Inscription Gallery | inside | no_recall, no_teleport, indoors | Lunar cycle carved into every wall |
| 11467 | A Guard Barracks Ruin | inside | no_recall, no_teleport, indoors, dark | |
| 11468 | The Sealed Vault Wing | inside | no_recall, no_teleport, indoors | Empty storage chambers |
| 11469 | The East Wing Passage | inside | no_recall, no_teleport, indoors | |
| 11470 | The West Wing Passage | inside | no_recall, no_teleport, indoors | |
| 11471 | The Cartographer's Chamber | inside | no_recall, no_teleport, indoors | Floor mosaic shows all seven tiers |
| 11472 | The East Descent Landing | inside | no_mob, no_teleport, indoors | D5 (down) → Tier 1 entry (11496) |
| 11473 | The Cracked Flagstone Hall | inside | no_recall, no_teleport, indoors | Contains trap exit D5 → 11450 (hidden pit, one-way) |
| 11474 | A Side Alcove | inside | no_recall, no_teleport, indoors, dark | |
| 11475 | The Central Nave | inside | no_recall, no_teleport, indoors | Wide hub room, stairs descend from here |
| 11476 | The Moon Altar Chamber | inside | no_recall, no_teleport, indoors | Crescent-shaped obsidian altar |
| 11477 | A Collapsed Side Passage | inside | no_recall, no_teleport, indoors, dark | |
| 11478 | The Inner Vestibule | inside | no_recall, no_teleport, indoors | |
| 11479 | The West Descent Landing | inside | no_mob, no_teleport, indoors | Alternate D5 (down) → 11497 |
| 11480 | The Tomb Robber's Camp | inside | no_recall, no_teleport, indoors | Abandoned supplies, cold fire pit |
| 11481 | A Debris-Filled Corridor | inside | no_recall, no_teleport, indoors, dark | |
| 11482 | The North Inner Hall | inside | no_recall, no_teleport, indoors | |
| 11483 | The South Inner Hall | inside | no_recall, no_teleport, indoors | |
| 11484 | A Ceremonial Niche | inside | no_recall, no_teleport, indoors | |
| 11485 | The Root-Cracked Vault | inside | no_recall, no_teleport, indoors, dark | Ancient roots push through the ceiling |
| 11486 | The Lunar Throne Antechamber | inside | no_recall, no_teleport, indoors | Approach to ground-floor boss zone |
| 11487 | The Lower Throne Room | inside | no_recall, no_teleport, indoors | Grand dais, cracked obsidian chair |
| 11488 | The Sanctum Approach | inside | no_recall, no_teleport, indoors | |
| 11489 | A Preparation Chamber | inside | no_recall, no_teleport, indoors, dark | |
| 11490 | The Sentinel's Passage | inside | no_recall, no_teleport, indoors | |
| 11491 | The Inner Sanctum Entrance | inside | no_recall, no_teleport, indoors | `^obsidian door` (closed, no lock) leads to 11492 |
| 11492 | The Waiting Hall | inside | no_recall, no_teleport, indoors | |
| 11493 | The Final Approach | inside | no_recall, no_teleport, indoors | |
| 11494 | The Moon Guardian's Antechamber | inside | no_recall, no_teleport, indoors | |
| 11495 | The Moon Gate Chamber | inside | no_mob, no_recall, no_teleport, indoors | **BOSS ROOM** — The Moon Gate Guardian |

---

### Zone 2: The Silent Crypts — Tier 1 (35 rooms: 11496–11530)

First descending tier. Consecrated to silence — sounds are muffled, dust hangs motionless in the air. All rooms `inside` sector. Most rooms flagged `dark`.

| Vnum | Room Name | Sector | Flags | Notes |
|------|-----------|--------|-------|-------|
| 11496 | The First Descent | inside | no_mob, no_teleport, indoors | Entry from ground D5 (11472 ↓) |
| 11497 | The Lower Landing | inside | no_mob, no_teleport, indoors | Entry from alternate ground D5 (11479 ↓) |
| 11498 | The Silent Hall | inside | no_recall, no_teleport, indoors | Sound dies within paces |
| 11499 | The Dust-Choked Corridor | inside | no_recall, no_teleport, indoors, dark | Undisturbed dust inches thick |
| 11500 | A Sealed Burial Niche | inside | no_recall, no_teleport, indoors, dark | Stone slab seals a corpse-alcove |
| 11501 | The Whispering Gallery | inside | no_recall, no_teleport, indoors | Faintest sounds echo strangely |
| 11502 | A Collapsed Crypt Wing | inside | no_recall, no_teleport, indoors, dark | Rubble blocks the far end |
| 11503 | The Embalmer's Workshop | inside | no_recall, no_teleport, indoors | Canopic jars line stone shelves |
| 11504 | A Stone-Floored Passage | inside | no_recall, no_teleport, indoors, dark | |
| 11505 | The Sarcophagus Row | inside | no_recall, no_teleport, indoors | Sealed stone coffins, four abreast |
| 11506 | The Prayer Alcove | inside | no_recall, no_teleport, indoors | Defaced altar with lunar carvings |
| 11507 | A Narrow Root Tunnel | inside | no_recall, no_teleport, indoors, dark | |
| 11508 | The Ossuary Antechamber | inside | no_recall, no_teleport, indoors | Bones stacked in neat rows |
| 11509 | The Trap-Fall Landing | inside | no_recall, no_teleport, indoors, dark | Landing point from the Crushing Hall (11450 ↓ arrives here) |
| 11510 | The Crypt Crossroads | inside | no_mob, no_teleport, indoors | Central hub; four-way junction |
| 11511 | A Flooded Burial Chamber | inside | no_recall, no_teleport, indoors, dark | Ankle-deep stagnant water |
| 11512 | The Fungal Alcove | inside | no_recall, no_teleport, indoors, dark | Pale fungi glow faintly |
| 11513 | The Inner Crypt Hall | inside | no_recall, no_teleport, indoors | Grand chamber, cracked pillars |
| 11514 | A Burial Preparation Room | inside | no_recall, no_teleport, indoors | |
| 11515 | The False Stairwell | inside | no_recall, no_teleport, indoors | Contains trap exit D5 → 11450 (appears to descend, one-way) |
| 11516 | The Deep Alcove | inside | no_recall, no_teleport, indoors, dark | |
| 11517 | A Sealed Side Chamber | inside | no_recall, no_teleport, indoors, dark | |
| 11518 | The Inscription Corridor | inside | no_recall, no_teleport, indoors | Warnings carved in ancient script |
| 11519 | The Ventilation Shaft Landing | inside | no_recall, no_teleport, indoors | |
| 11520 | A Guard Post Ruin | inside | no_recall, no_teleport, indoors, dark | |
| 11521 | The Silent Shrine | inside | no_recall, no_teleport, indoors | Central shrine, untouched |
| 11522 | The East Crypt Passage | inside | no_recall, no_teleport, indoors | |
| 11523 | The West Crypt Passage | inside | no_recall, no_teleport, indoors | |
| 11524 | A Crumbling Side Passage | inside | no_recall, no_teleport, indoors, dark | |
| 11525 | The Inner Vault | inside | no_recall, no_teleport, indoors | |
| 11526 | The Warden's Approach | inside | no_recall, no_teleport, indoors | |
| 11527 | The Outer Warden Chamber | inside | no_recall, no_teleport, indoors | |
| 11528 | A Final Corridor | inside | no_recall, no_teleport, indoors, dark | |
| 11529 | The Silent Warden's Approach | inside | no_recall, no_teleport, indoors | `^glyph-sealed door` (closed, no lock) leads to 11530 |
| 11530 | The Silent Warden's Tomb | inside | no_mob, no_recall, no_teleport, indoors | **BOSS ROOM** — The Silent Warden |

---

### Zone 3: The Rotting Galleries — Tier 2 (30 rooms: 11531–11560)

Second descending tier. Consecrated to decay — walls seep moisture, fungal growth covers surfaces, the air is thick and fetid. Slightly narrower than Tier 1.

| Vnum | Room Name | Sector | Flags | Notes |
|------|-----------|--------|-------|-------|
| 11531 | The Second Descent | inside | no_mob, no_teleport, indoors | Entry from Tier 1 D5 (11510 ↓, through `^corroded iron hatch`) |
| 11532 | The Decay Landing | inside | no_mob, no_teleport, indoors | Alternate entry from Tier 1 D5 (11522 ↓) |
| 11533 | The Rotting Hall | inside | no_recall, no_teleport, indoors | Walls slick with moisture |
| 11534 | A Mold-Covered Passage | inside | no_recall, no_teleport, indoors, dark | |
| 11535 | The Fungal Garden | inside | no_recall, no_teleport, indoors | Mushrooms tall as a man |
| 11536 | The Seeping Vault | inside | no_recall, no_teleport, indoors | Water trickles from every crack |
| 11537 | A Collapsed Drain Tunnel | inside | no_recall, no_teleport, indoors, dark | |
| 11538 | The Refuse Pit | inside | no_recall, no_teleport, indoors | Generations of discarded offerings |
| 11539 | The Inner Rot Gallery | inside | no_recall, no_teleport, indoors | Frescoes dissolving under mold |
| 11540 | A Slime-Filled Alcove | inside | no_recall, no_teleport, indoors, dark | |
| 11541 | The Putrid Pool Chamber | inside | no_recall, no_teleport, indoors | Stagnant green-black water |
| 11542 | The Crumbling Archive | inside | no_recall, no_teleport, indoors | Scrolls rotted to paste |
| 11543 | A Corroded Side Passage | inside | no_recall, no_teleport, indoors, dark | |
| 11544 | The Decay Shrine | inside | no_recall, no_teleport, indoors | Altar covered in thick moss |
| 11545 | The Unstable Gallery | inside | no_recall, no_teleport, indoors | Contains trap exit D5 → 11450 (pressure plate collapse, one-way) |
| 11546 | The East Rot Wing | inside | no_recall, no_teleport, indoors | |
| 11547 | The West Rot Wing | inside | no_recall, no_teleport, indoors | |
| 11548 | A Mushroom-Choked Corridor | inside | no_recall, no_teleport, indoors, dark | |
| 11549 | The Descent Shaft Landing | inside | no_mob, no_teleport, indoors | D5 (down) → Tier 3 (11561), through `^rusted grate` (closed, no lock) |
| 11550 | The Inner Decay Hall | inside | no_recall, no_teleport, indoors | |
| 11551 | A Flooded Crypt Alcove | inside | no_recall, no_teleport, indoors, dark | |
| 11552 | The Compost Chamber | inside | no_recall, no_teleport, indoors | Ancient food stores, long rotted |
| 11553 | The Verdigris Passage | inside | no_recall, no_teleport, indoors | Green copper stains every surface |
| 11554 | A Crumbling Vault | inside | no_recall, no_teleport, indoors, dark | |
| 11555 | The East Gallery End | inside | no_recall, no_teleport, indoors | |
| 11556 | The Outer Warden Chamber | inside | no_recall, no_teleport, indoors | |
| 11557 | The Pre-Chamber Corridor | inside | no_recall, no_teleport, indoors, dark | |
| 11558 | The Rot Warden's Antechamber | inside | no_recall, no_teleport, indoors | |
| 11559 | The Rot Warden's Approach | inside | no_recall, no_teleport, indoors | `^moss-covered stone door` (closed, no lock) leads to 11560 |
| 11560 | The Rot Warden's Chamber | inside | no_mob, no_recall, no_teleport, indoors | **BOSS ROOM** — The Rot Warden |

---

### Zone 4: The Shadow Nave — Tier 3 (25 rooms: 11561–11585)

Third descending tier. Consecrated to shadow — natural darkness that resists all light sources. Torches gutter, magical light dims. Nearly all rooms flagged `dark`.

| Vnum | Room Name | Sector | Flags | Notes |
|------|-----------|--------|-------|-------|
| 11561 | The Third Descent | inside | no_mob, no_teleport, indoors | Entry via `^rusted grate` from 11549 |
| 11562 | The Shadow Landing | inside | no_mob, no_teleport, indoors | Alternate entry from Tier 2 D5 (11546 ↓) |
| 11563 | The Shadow Hall | inside | no_recall, no_teleport, indoors, dark | Darkness presses like a weight |
| 11564 | A Lightless Corridor | inside | no_recall, no_teleport, indoors, dark | |
| 11565 | The Umbral Shrine | inside | no_recall, no_teleport, indoors, dark | Black altar absorbs all light |
| 11566 | The Eclipse Chamber | inside | no_recall, no_teleport, indoors, dark | Carved depictions of a sunless sky |
| 11567 | A Dark Side Passage | inside | no_recall, no_teleport, indoors, dark | |
| 11568 | The Shadowmancer's Study | inside | no_recall, no_teleport, indoors, dark | Crumbled tablets and spilled ink |
| 11569 | The Inner Shadow Hall | inside | no_recall, no_teleport, indoors, dark | |
| 11570 | A Pitch-Black Alcove | inside | no_recall, no_teleport, indoors, dark | |
| 11571 | The Blind Gallery | inside | no_recall, no_teleport, indoors, dark | Reliefs meant to be read by touch |
| 11572 | The Treacherous Ledge | inside | no_recall, no_teleport, indoors, dark | Contains trap exit D5 → 11450 (false floor, one-way) |
| 11573 | The East Shadow Wing | inside | no_recall, no_teleport, indoors, dark | |
| 11574 | The West Shadow Wing | inside | no_recall, no_teleport, indoors, dark | |
| 11575 | The Obsidian Colonnade | inside | no_recall, no_teleport, indoors, dark | Polished black stone columns |
| 11576 | The Descent Shaft | inside | no_mob, no_teleport, indoors | D5 (down) → Tier 4 (11586) |
| 11577 | A Shadow-Flooded Crypt | inside | no_recall, no_teleport, indoors, dark | |
| 11578 | The Darkened Archive | inside | no_recall, no_teleport, indoors, dark | |
| 11579 | The Warden's Outer Guard | inside | no_recall, no_teleport, indoors, dark | |
| 11580 | A Narrow Dark Passage | inside | no_recall, no_teleport, indoors, dark | |
| 11581 | The Pre-Chamber Hall | inside | no_recall, no_teleport, indoors, dark | |
| 11582 | The Inner Nave | inside | no_recall, no_teleport, indoors, dark | |
| 11583 | The Shadow Warden's Antechamber | inside | no_recall, no_teleport, indoors, dark | |
| 11584 | The Shadow Warden's Approach | inside | no_recall, no_teleport, indoors, dark | `^shadow-wreathed portal` (closed, no lock) leads to 11585 |
| 11585 | The Shadow Warden's Sanctum | inside | no_mob, no_recall, no_teleport, indoors | **BOSS ROOM** — The Shadow Warden |

---

### Zone 5: The Ossuary Depths — Tier 4 (20 rooms: 11586–11605)

Fourth descending tier. Consecrated to bone — walls built from interlocking skeletal remains, floors paved with compressed bone-meal. Narrow passages.

| Vnum | Room Name | Sector | Flags | Notes |
|------|-----------|--------|-------|-------|
| 11586 | The Fourth Descent | inside | no_mob, no_teleport, indoors | Entry from Tier 3 D5 (11576 ↓) |
| 11587 | The Bone Landing | inside | no_mob, no_teleport, indoors | Alternate entry from Tier 3 (11573 ↓) |
| 11588 | The Ossuary Hall | inside | no_recall, no_teleport, indoors | Walls of interlocking skulls |
| 11589 | A Bone-Paved Corridor | inside | no_recall, no_teleport, indoors, dark | |
| 11590 | The Skull Colonnade | inside | no_recall, no_teleport, indoors | Pillars of stacked skulls |
| 11591 | The Bone Throne Room | inside | no_recall, no_teleport, indoors | A throne of welded femurs |
| 11592 | A Rib-Vaulted Chamber | inside | no_recall, no_teleport, indoors, dark | Ceiling arches of giant ribs |
| 11593 | The Marrow Pool | inside | no_recall, no_teleport, indoors | Shallow pool of liquefied marrow |
| 11594 | The Inner Ossuary | inside | no_recall, no_teleport, indoors | |
| 11595 | A Bone-Dust Alcove | inside | no_recall, no_teleport, indoors, dark | |
| 11596 | The Calcification Chamber | inside | no_recall, no_teleport, indoors | Dripping mineral water encrusts everything |
| 11597 | The East Bone Wing | inside | no_recall, no_teleport, indoors | |
| 11598 | The West Bone Wing | inside | no_recall, no_teleport, indoors, dark | |
| 11599 | The Reliquary | inside | no_recall, no_teleport, indoors | Sacred bone fragments on stone shelves |
| 11600 | A Narrow Bone Tunnel | inside | no_recall, no_teleport, indoors, dark | |
| 11601 | The Deep Shaft Landing | inside | no_mob, no_teleport, indoors | D5 (down) → Tier 5 (11606), through `^bone-barred gate` (locked, key 11514) |
| 11602 | The Bone Warden's Outer Hall | inside | no_recall, no_teleport, indoors | |
| 11603 | The Bone Warden's Antechamber | inside | no_recall, no_teleport, indoors | |
| 11604 | The Bone Warden's Approach | inside | no_recall, no_teleport, indoors | `^skull-arch threshold` (closed, no lock) leads to 11605 |
| 11605 | The Bone Warden's Crypt | inside | no_mob, no_recall, no_teleport, indoors | **BOSS ROOM** — The Bone Warden |

---

### Zone 6: The Venomous Cisterns — Tier 5 (20 rooms: 11606–11625)

Fifth descending tier. Consecrated to venom — flooded with toxic pools, venomous creatures, and poisonous vapors. Green-tinged light from bioluminescent growths.

| Vnum | Room Name | Sector | Flags | Notes |
|------|-----------|--------|-------|-------|
| 11606 | The Fifth Descent | inside | no_mob, no_teleport, indoors | Entry via `^bone-barred gate` from 11601 |
| 11607 | The Cistern Landing | inside | no_recall, no_teleport, indoors | Toxic vapor rises from below |
| 11608 | The Venom Hall | inside | no_recall, no_teleport, indoors | Green-tinged walls, acrid air |
| 11609 | A Dripping Toxic Corridor | inside | no_recall, no_teleport, indoors, dark | |
| 11610 | The Greater Cistern | inside | no_recall, no_teleport, indoors | Vast pool of luminous green liquid |
| 11611 | The Serpent Shrine | inside | no_recall, no_teleport, indoors | Coiled-serpent carvings |
| 11612 | A Toxic Alcove | inside | no_recall, no_teleport, indoors, dark | |
| 11613 | The Alchemy Chamber | inside | no_recall, no_teleport, indoors | Cracked vials and corroded apparatus |
| 11614 | The Inner Cistern Passage | inside | no_recall, no_teleport, indoors | |
| 11615 | A Flooded Side Tunnel | inside | no_recall, no_teleport, indoors, dark | Knee-deep in toxic water |
| 11616 | The Fume Vent Chamber | inside | no_recall, no_teleport, indoors | Hot toxic gas seeps from cracks |
| 11617 | The East Cistern Wing | inside | no_recall, no_teleport, indoors | |
| 11618 | The West Cistern Wing | inside | no_recall, no_teleport, indoors, dark | |
| 11619 | The Venom Altar | inside | no_recall, no_teleport, indoors | Carved serpent coils around an obsidian basin |
| 11620 | The Descent Shaft | inside | no_mob, no_teleport, indoors | D5 (down) → Tier 6 (11626) |
| 11621 | The Venom Warden's Outer Hall | inside | no_recall, no_teleport, indoors | |
| 11622 | A Noxious Side Passage | inside | no_recall, no_teleport, indoors, dark | |
| 11623 | The Venom Warden's Antechamber | inside | no_recall, no_teleport, indoors | |
| 11624 | The Venom Warden's Approach | inside | no_recall, no_teleport, indoors | `^venom-slick gate` (closed, no lock) leads to 11625 |
| 11625 | The Venom Warden's Cistern | inside | no_mob, no_recall, no_teleport, indoors | **BOSS ROOM** — The Venom Warden |

---

### Zone 7: The Molten Sanctum — Tier 6 (14 rooms: 11626–11639)

Sixth descending tier. Consecrated to fire — volcanic vents heat the stone, lava seeps through cracks. The air shimmers with heat. Sector type `inside` throughout.

| Vnum | Room Name | Sector | Flags | Notes |
|------|-----------|--------|-------|-------|
| 11626 | The Sixth Descent | inside | no_mob, no_teleport, indoors | Entry from Tier 5 D5 (11620 ↓) |
| 11627 | The Scorched Corridor | inside | no_recall, no_teleport, indoors, hot | Walls radiate intense heat |
| 11628 | The Lava Seep Chamber | inside | no_recall, no_teleport, indoors, hot | Molten stone oozes from cracks |
| 11629 | A Basalt Passage | inside | no_recall, no_teleport, indoors, hot, dark | |
| 11630 | The Magma Pool Room | inside | no_recall, no_teleport, indoors, hot | Pool of glowing magma at center |
| 11631 | The Obsidian Gallery | inside | no_recall, no_teleport, indoors, hot | Walls of natural volcanic glass |
| 11632 | A Heat-Shimmering Alcove | inside | no_recall, no_teleport, indoors, hot, dark | |
| 11633 | The Forge of the Dead | inside | no_recall, no_teleport, indoors, hot | Ancient forge powered by volcanic vent |
| 11634 | The Inner Sanctum Passage | inside | no_recall, no_teleport, indoors, hot | |
| 11635 | The Ember Throne Room | inside | no_recall, no_teleport, indoors, hot | Charred obsidian throne |
| 11636 | The Fire Warden's Outer Hall | inside | no_recall, no_teleport, indoors, hot | |
| 11637 | The Fire Warden's Antechamber | inside | no_recall, no_teleport, indoors, hot | |
| 11638 | The Fire Warden's Approach | inside | no_recall, no_teleport, indoors, hot | `^molten-iron gate` (closed, no lock) leads to 11639 |
| 11639 | The Fire Warden's Crucible | inside | no_mob, no_recall, no_teleport, indoors, hot | **BOSS ROOM** — The Fire Warden |

---

### Zone 8: The Void Throne — Tier 7 / Nadir (10 rooms: 11640–11649)

The deepest and smallest tier. The nadir of the pyramid. The Moon Sovereign's final resting place. A chamber of volcanic glass suspended above a lake of molten stone. Sector type `inside`.

| Vnum | Room Name | Sector | Flags | Notes |
|------|-----------|--------|-------|-------|
| 11640 | The Final Descent | inside | no_mob, no_teleport, indoors | Entry from Tier 6 D5 (11634 ↓), through `^obsidian sentinel gate` (locked, key 11515) |
| 11641 | The Void Corridor | inside | no_recall, no_teleport, indoors, dark | Absolute silence, no heat despite proximity to magma |
| 11642 | The Glass Antechamber | inside | no_recall, no_teleport, indoors, dark | Walls of polished volcanic glass |
| 11643 | The Reflection Hall | inside | no_recall, no_teleport, indoors, dark | Infinite reflections in obsidian mirrors |
| 11644 | A Void-Touched Alcove | inside | no_recall, no_teleport, indoors, dark | |
| 11645 | The Inner Void Passage | inside | no_recall, no_teleport, indoors, dark | |
| 11646 | The Moon Sovereign's Antechamber | inside | no_recall, no_teleport, indoors, dark | Carved depictions of the Moon Sovereign's conquest |
| 11647 | The Pre-Throne Hall | inside | no_recall, no_teleport, indoors, dark | |
| 11648 | The Moon Sovereign's Approach | inside | no_recall, no_teleport, indoors, dark | `^void chamber door` (closed, no lock) leads to 11649 |
| 11649 | The Void Throne Room | inside | no_mob, no_teleport, indoors | **BOSS ROOM** — The Moon Sovereign |

**Tier 7 connectivity:** Entry from Tier 6 via locked `^obsidian sentinel gate` (rooms 11634/11640). The key (11515) drops from the Fire Warden via loot table. The Void Throne Room (11649) allows recall, letting players exit after defeating the final boss.

---

## Doors Summary

| From Room | Direction | To Room | Keyword | Lock? | Key Vnum | Reset State |
|-----------|-----------|---------|---------|-------|----------|-------------|
| 9819 (S.Oasis) | D2 (south) | 11451 | `^ancient road south` | No | -1 | `D 9819 2 1` (closed) |
| 11451 | D0 (north) | 9819 | (none) | No | -1 | open |
| 11459 | D2 (south) | 11460 | `^moon gate` | No | -1 | `D 11459 2 1` (closed) |
| 11460 | D0 (north) | 11459 | `^moon gate` | No | -1 | `D 11460 0 1` (closed) |
| 11491 | D2 (south) | 11492 | `^obsidian door` | No | -1 | `D 11491 2 1` (closed) |
| 11492 | D0 (north) | 11491 | `^obsidian door` | No | -1 | `D 11492 0 1` (closed) |
| 11472 | D5 (down) | 11496 | (none) | No | -1 | open |
| 11496 | D4 (up) | 11472 | (none) | No | -1 | open |
| 11479 | D5 (down) | 11497 | (none) | No | -1 | open |
| 11497 | D4 (up) | 11479 | (none) | No | -1 | open |
| 11529 | D2 (south) | 11530 | `^glyph-sealed door` | No | -1 | `D 11529 2 1` (closed) |
| 11530 | D0 (north) | 11529 | `^glyph-sealed door` | No | -1 | `D 11530 0 1` (closed) |
| 11510 | D5 (down) | 11531 | `^corroded iron hatch` | No | -1 | `D 11510 5 1` (closed) |
| 11531 | D4 (up) | 11510 | `^corroded iron hatch` | No | -1 | `D 11531 4 1` (closed) |
| 11522 | D5 (down) | 11532 | (none) | No | -1 | open |
| 11532 | D4 (up) | 11522 | (none) | No | -1 | open |
| 11559 | D2 (south) | 11560 | `^moss-covered stone door` | No | -1 | `D 11559 2 1` (closed) |
| 11560 | D0 (north) | 11559 | `^moss-covered stone door` | No | -1 | `D 11560 0 1` (closed) |
| 11549 | D5 (down) | 11561 | `^rusted grate` | No | -1 | `D 11549 5 1` (closed) |
| 11561 | D4 (up) | 11549 | `^rusted grate` | No | -1 | `D 11561 4 1` (closed) |
| 11546 | D5 (down) | 11562 | (none) | No | -1 | open |
| 11562 | D4 (up) | 11546 | (none) | No | -1 | open |
| 11584 | D2 (south) | 11585 | `^shadow-wreathed portal` | No | -1 | `D 11584 2 1` (closed) |
| 11585 | D0 (north) | 11584 | `^shadow-wreathed portal` | No | -1 | `D 11585 0 1` (closed) |
| 11576 | D5 (down) | 11586 | (none) | No | -1 | open |
| 11586 | D4 (up) | 11576 | (none) | No | -1 | open |
| 11573 | D5 (down) | 11587 | (none) | No | -1 | open |
| 11587 | D4 (up) | 11573 | (none) | No | -1 | open |
| 11604 | D2 (south) | 11605 | `^skull-arch threshold` | No | -1 | `D 11604 2 1` (closed) |
| 11605 | D0 (north) | 11604 | `^skull-arch threshold` | No | -1 | `D 11605 0 1` (closed) |
| 11601 | D5 (down) | 11606 | `^bone-barred gate` | Yes (locked) | 11514 | `D 11601 5 2` (locked) |
| 11606 | D4 (up) | 11601 | `^bone-barred gate` | Yes (locked) | 11514 | `D 11606 4 2` (locked) |
| 11624 | D2 (south) | 11625 | `^venom-slick gate` | No | -1 | `D 11624 2 1` (closed) |
| 11625 | D0 (north) | 11624 | `^venom-slick gate` | No | -1 | `D 11625 0 1` (closed) |
| 11620 | D5 (down) | 11626 | (none) | No | -1 | open |
| 11626 | D4 (up) | 11620 | (none) | No | -1 | open |
| 11638 | D2 (south) | 11639 | `^molten-iron gate` | No | -1 | `D 11638 2 1` (closed) |
| 11639 | D0 (north) | 11638 | `^molten-iron gate` | No | -1 | `D 11639 0 1` (closed) |
| 11634 | D5 (down) | 11640 | `^obsidian sentinel gate` | Yes (locked) | 11515 | `D 11634 5 2` (locked) |
| 11640 | D4 (up) | 11634 | `^obsidian sentinel gate` | Yes (locked) | 11515 | `D 11640 4 2` (locked) |
| 11648 | D2 (south) | 11649 | `^void chamber door` | No | -1 | `D 11648 2 1` (closed) |
| 11649 | D0 (north) | 11648 | `^void chamber door` | No | -1 | `D 11649 0 1` (closed) |
| 11495 (boss) | D0 (north) | 11494 | `^lunar vault door` | No | -1 | `D 11495 0 1` (closed) |
| 11494 | D2 (south) | 11495 | `^lunar vault door` | No | -1 | `D 11494 2 1` (closed) |

All named doors must be mentioned in at least one of: the room's main description, a spawned object, or an `E` extra description.

Both locked doors require the corresponding key object to exist in `#OBJECTS`:
- `^bone-barred gate` (rooms 11601/11606): key vnum **11463**
- `^obsidian sentinel gate` (rooms 11634/11640): key vnum **11464**

---

## Mobile Definitions

### Bosses (Level 150)

All bosses: `act` flags = `is_npc(1) | sentinel(2) | no_flee(16) | aggressive(32) | stay_area(64) | boss(67108864) | noassist(1073741824)` = **1140850879**

Boss skills (`!` line): `5_attack(8) | enhanced(32768) | counter(524288) | parry(8192) | dodge(4096) | nodisarm(32) | notrip(64)` = **569448**

All bosses placed only in `no_mob` flagged rooms.

| Mob Vnum | Name | Room | Act Flags | Theme | Spec |
|----------|------|------|-----------|-------|------|
| 11450 | the Moon Gate Guardian | 11495 | 1140850879 | Massive obsidian construct, guardian of the capstone entrance. Cast: earthquake, flamestrike. Def: heal, shockshield. Strong: earth, physical. Weak: mental. 6_attack instead of 5_attack. | spec_cast_mage |
| 11451 | the Silent Warden | 11530 | 1140850879 | Spectral guardian bound in eternal silence. Cast: suffocate, mind_flail, nerve_fire. Def: heal, iceshield. Strong: shadow, mental. Weak: holy, fire. | spec_cast_undead |
| 11452 | the Rot Warden | 11560 | 1140850879 | Titanic fungal horror animated by decay magic. Cast: acid_blast, suffocate, poison. Def: heal, cure_critic. Strong: poison, earth. Weak: fire, holy. | spec_cast_mage |
| 11453 | the Shadow Warden | 11585 | 1140850879 | Living shadow given form by death magic. Cast: mindflame, bloody_tears, ego_whip. Def: heal, iceshield, fireshield. Strong: shadow, mental. Weak: holy, light. | spec_cast_bigtime |
| 11454 | the Bone Warden | 11605 | 1140850879 | Colossus of welded skeletal remains. Cast: chain_light, mind_bolt, earthquake. Def: heal, shockshield. Strong: physical, earth. Weak: holy, fire. | spec_cast_mage |
| 11455 | the Venom Warden | 11625 | 1140850879 | Giant serpent-construct dripping with toxin. Cast: acid_blast, suffocate, lava_burst. Def: heal, cure_critic. Strong: poison, water. Weak: fire, air. Spec: spec_poison. | spec_poison |
| 11456 | the Fire Warden | 11639 | 1140850879 | Elemental of living magma and volcanic stone. Cast: lava_burst, heat_armor, flamestrike. Def: fireshield. Strong: fire, physical. Weak: water, air. Spec: spec_breath_fire. | spec_breath_fire |
| 11457 | the Moon Sovereign | 11649 | 1140850879 | The immortal architect of the inverted pyramid. Full spell set. Cast: nerve_fire, light_bolt, mindflame, suffocate, lava_burst. Def: heal, fireshield, iceshield, shockshield. Strong: shadow, death, mental. Weak: holy. | spec_cast_bigtime |

**Boss `+` extension:** All bosses receive moderate spellpower, crit, crit_mult, parry, dodge, and block modifiers appropriate for level 150 encounters. The Moon Sovereign (11457) receives maximum values.

---

### Strong (Solo) Mobs (Levels 135–142)

All strong mobs: `act` = `is_npc(1) | aggressive(32) | stay_area(64) | solo(33554432)` = **33554529**

| Mob Vnum | Name | Level | Zones | Notes |
|----------|------|-------|-------|-------|
| 11458 | a cursed moon shade | 138 | All | Undead spirit. Cast: mind_flail, suffocate. Def: heal. |
| 11459 | an obsidian colossus | 142 | Ground, Tier 1 | Massive animated construct. Enhanced, 4_attack, nodisarm, notrip. |
| 11460 | a death-sworn knight | 140 | Tiers 2–4 | Armored undead warrior. 4_attack, enhanced, parry, counter. |
| 11461 | a magma drake | 140 | Tiers 5–6 | Lesser drake of molten stone. 4_attack, enhanced, parry, dodge. Spec: spec_breath_fire. |
| 11462 | a jade sphinx | 142 | Tiers 4–5 | Construct guardian. 3_attack, enhanced, parry, dodge. Cast: earthquake, light_bolt. |
| 11510 | a toxic sand elemental | 139 | Ground, Tier 1 | Poisonous sand elemental. no_mind. 3_attack, enhanced, dodge. Cast: acid_blast, col_spray. Spec: spec_cast_mage. |
| 11511 | an ancient basalt guardian | 142 | Tier 2, Tier 3 | Massive black-stone construct. no_mind, sentinel. 4_attack, enhanced, parry, nodisarm, notrip. |
| 11512 | a greater bone overlord | 140 | Tier 3, Tier 4 | Towering skeletal warlord-spirit. undead. 3_attack, enhanced, parry, counter. Cast: suffocate, ego_whip. Spec: spec_cast_undead. |
| 11627 (mob) | a void-touched juggernaut | 142 | Tier 6, Tier 7 | Massive construct infused with void energy. no_mind. 4_attack, enhanced, parry, counter, nodisarm, notrip. |
| 11628 (mob) | a cistern matriarch | 140 | Tier 5 | Giant serpentine predator ruling the toxic pools. 4_attack, enhanced, parry, dodge. Spec: spec_poison. |
| 11629 (mob) | a petrified desert titan | 142 | Ground, Tier 1 | Enormous fossilized humanoid animated by pyramid magic. no_mind. 4_attack, enhanced, parry, nodisarm, notrip. Spec: spec_cast_mage. |
| 11630 (mob) | a fungal colossus | 138 | Tier 2 | Towering mushroom-creature trailing toxic spores. 3_attack, enhanced, parry. Spec: spec_disease. |
| 11631 (mob) | a nightmare stalker | 140 | Tier 3, Tier 4 | Apex predator of the shadow tier. 4_attack, enhanced, dodge, trip, counter. |

---

### Trash Mobs (Levels 125–140)

All trash: `act` = `is_npc(1) | aggressive(32) | stay_area(64)` = **97**

| Mob Vnum | Name | Level | Zones | Combat Notes |
|----------|------|-------|-------|--------------|
| 11463 (mob) | a desert tomb scarab swarm | 125 | Ground, Tier 1 | no_body, no_mind. 2_attack. |
| 11464 (mob) | a moon-cursed skeleton | 127 | Ground, Tier 1 | undead. 2_attack, punch. |
| 11465 | a desiccated mummy | 128 | Ground, Tier 1 | undead. 2_attack, enhanced. |
| 11466 | a venomous desert scorpion | 130 | Ground, Tier 1 | Spec: spec_poison. 2_attack. |
| 11467 | a grave robber's specter | 126 | Ground | undead, no_body. Cast: flare, shock_grasp. |
| 11468 | a sand hyena | 132 | Ground | 2_attack, dodge, trip. |
| 11469 | a basalt guardian statue | 135 | Ground, Tier 1 | no_mind. 3_attack, enhanced, nodisarm. |
| 11470 | a canopic abomination | 129 | Ground, Tier 1 | undead, no_mind. 2_attack, punch. |
| 11471 | a jade asp | 133 | Tier 1, Tier 2 | Spec: spec_poison. 2_attack. |
| 11472 (mob) | a desert vulture | 136 | Tier 1, Tier 2 | 2_attack, dodge. |
| 11473 (mob) | an obsidian golem | 130 | Tier 1 | no_mind. 3_attack, enhanced. |
| 11474 | a decay acolyte | 138 | Tier 2 | Cast: faerie_fire, flare, col_spray. Def: cure_serious. |
| 11475 | a rot wight | 134 | Tier 2 | undead. 2_attack, enhanced, drain. |
| 11476 | a stone scarab sentinel | 137 | Tier 2, Tier 3 | no_mind. 3_attack, enhanced, parry. |
| 11477 | a tomb cobra | 131 | Tier 2 | Spec: spec_poison. 2_attack. |
| 11478 | a shadow archer | 139 | Tier 3 | undead. 2_attack, enhanced, dodge. |
| 11479 (mob) | a great shadow vulture | 140 | Tier 3 | 2_attack, dodge, kick. |
| 11480 | a shadow face guardian | 136 | Tier 3 | no_mind, sentinel. 2_attack, parry, nodisarm. |
| 11481 | a creeping shadow | 132 | Tier 3, Tier 4 | undead, no_body. Cast: mind_bolt, ego_whip. |
| 11482 | a shadow assassin | 138 | Tier 3, Tier 4 | 2_attack, enhanced, dodge, trip. |
| 11483 | an animated bone pillar | 140 | Tier 4 | no_mind, sentinel. 4_attack, enhanced, nodisarm, notrip. |
| 11484 | a venomous golden cobra | 135 | Tier 4, Tier 5 | Spec: spec_poison. 2_attack. |
| 11485 | a bone cultist | 137 | Tier 4 | Cast: burn_hands, mag_missile. Def: cure_serious. |
| 11486 | a death hawk | 139 | Tier 4, Tier 5 | 2_attack, dodge, kick. |
| 11487 | a bone revenant | 133 | Tier 4 | undead. 2_attack, enhanced, parry. Def: cure_critic. |
| 11488 | a void specter | 136 | Tier 5, Tier 6 | undead, no_body. Cast: mindflame, mind_bolt. Def: cure_critic. |
| 11489 | a carrion fly swarm | 125 | Ground, Tier 1 | Clouds of carrion flies. no_body, no_mind. 2_attack. Spec: spec_disease. |
| 11490 | a skeletal desert rat | 125 | Ground, Tier 1 | Skeletal undead vermin. no_mind. 2_attack, trip. Spec: spec_disease. |
| 11491 | a bone crawler | 127 | Ground, Tier 1 | Animated skeletal mass. undead, no_mind. 3_attack, enhanced. |
| 11492 | a sand-choked revenant | 129 | Ground, Tier 1 | Revenant packed with sand. undead. 2_attack, enhanced, drain. |
| 11493 | an animated obelisk shard | 132 | Ground | Fragment of a shattered obelisk. no_mind, sentinel. 3_attack, enhanced, nodisarm. |
| 11494 | a cursed pyramid laborer | 130 | Ground, Tier 1 | Ghost of a slave. undead. 2_attack, enhanced. Cast: weaken, blindness. |
| 11495 (mob) | a desert pit viper | 128 | Ground | A coiled heat-seeking viper. 2_attack, dodge. Spec: spec_poison. |
| 11496 (mob) | an obsidian sentinel construct | 134 | Ground, Tier 1 | Hulking guardian golem. no_mind. 3_attack, enhanced, parry, nodisarm. |
| 11497 (mob) | a moon glyph guardian | 135 | Tier 1 | Living inscription in stone. no_mind, sentinel. 3_attack, enhanced. |
| 11498 | an animated burial jar | 132 | Tier 1, Tier 2 | A walking canopic jar. undead, no_body. 2_attack, punch. Cast: poison, col_spray. |
| 11499 | a scribal phantom | 136 | Tier 1 | Ghost of a scribe. undead. 2_attack. Cast: flare, mag_missile. Def: cure_light. |
| 11500 | a sand-bound mummy thrall | 133 | Tier 1, Tier 2 | Lesser mummy. undead. 2_attack, enhanced. |
| 11501 | a rot priest's thrall | 138 | Tier 2 | Undead acolyte. undead. 2_attack, enhanced, parry. Cast: faerie_fire. |
| 11502 | a fungal wraith | 137 | Tier 2, Tier 3 | Translucent fungal spirit. undead, no_body. Cast: ego_whip, suffocate. Def: cure_serious. |
| 11503 | a curse-bound sentinel | 139 | Tier 2, Tier 3 | Ancient soldier held by curse. undead. 3_attack, enhanced, counter. |
| 11504 | an entombed general | 138 | Tier 3 | Buried standing, still armed. undead. 3_attack, enhanced, parry, dodge. |
| 11505 | a bone golem | 140 | Tier 3, Tier 4 | Construct of fused skeletal parts. no_mind. 4_attack, enhanced, nodisarm, notrip. |
| 11506 | a crypt stalker | 136 | Tier 3 | A beast that hunts in the dark. 2_attack, dodge, trip. |
| 11507 | a magma lizard | 137 | Tier 5, Tier 6 | Long-tailed reptile absorbing volcanic heat. 2_attack, dodge. Resist: fire. |
| 11508 | a charred bone husk | 139 | Tier 5, Tier 6 | Undead burned to blackened bone. undead. 3_attack, enhanced. Strong: fire. Weak: water. |
| 11509 | a void sentinel | 140 | Tier 7 | The last stone guardian before the throne. no_mind, sentinel. 3_attack, enhanced, parry, nodisarm. |
| 11513 | a gilded moon shade | 138 | Tier 7 | Ghost of a royal bodyguard in ceremonial obsidian. undead. 2_attack, enhanced. Cast: mind_bolt. |
| 11597 (mob) | a petrified sand wurm | 134 | Ground, Tier 1 | Burrowing worm fossilized mid-strike. no_mind. 3_attack, enhanced, notrip. |
| 11598 (mob) | a tomb guardian jackal | 130 | Ground | Undead jackal bound to patrol. undead. 2_attack, dodge, trip. |
| 11599 (mob) | an embalmed crocodile | 133 | Ground, Tier 1 | Preserved reptile animated by curse. undead, no_mind. 3_attack, enhanced. |
| 11600 (mob) | a dust devil spirit | 127 | Ground | Whirling sand elemental fragment. no_body, no_mind. 2_attack, dodge. |
| 11601 (mob) | a whispering wraith | 131 | Tier 1 | Translucent shade that mouths silent curses. undead, no_body. Cast: weaken, blindness, chill_touch. |
| 11602 (mob) | a calcified serpent | 129 | Tier 1, Tier 2 | Mineralized snake, still strikes. no_mind. 2_attack, dodge. Spec: spec_poison. |
| 11603 (mob) | a tomb beetle colossus | 136 | Tier 1, Tier 2 | Man-sized scarab with iron mandibles. no_mind. 3_attack, enhanced, parry. |
| 11604 (mob) | a mold-ridden ghoul | 135 | Tier 2 | Fungal growth erupts from rotting flesh. undead. 2_attack, enhanced, drain. Spec: spec_disease. |
| 11605 (mob) | a bloated corpse walker | 132 | Tier 2 | Swollen cadaver leaking toxic gas. undead. 2_attack, enhanced. |
| 11606 (mob) | a decay spider | 128 | Tier 2 | Eight-legged creature woven from rotting sinew. 2_attack, dodge, trip. Spec: spec_poison. |
| 11607 (mob) | a mildew phantom | 134 | Tier 2, Tier 3 | Ghostly outline dripping condensation. undead, no_body. Cast: chill_touch, weaken. Def: cure_light. |
| 11608 (mob) | a shadow hound | 137 | Tier 3 | Pack hunter from the dark. 2_attack, enhanced, dodge, trip. |
| 11609 (mob) | a nightmare sentinel | 139 | Tier 3 | Animated suit of black iron armor. no_mind. 3_attack, enhanced, parry, nodisarm. |
| 11610 (mob) | a shade dancer | 133 | Tier 3 | Flickering silhouette that attacks from blind spots. 2_attack, dodge, trip. |
| 11611 (mob) | a darkfire wisp | 126 | Tier 3, Tier 4 | Small sphere of black flame. no_body, no_mind. 2_attack. Cast: burn_hands, flare. |
| 11612 (mob) | an ossuary spider | 131 | Tier 4 | Spider constructed from finger bones. no_mind. 2_attack, dodge, trip. Spec: spec_poison. |
| 11613 (mob) | a marrow elemental | 138 | Tier 4, Tier 5 | Oozing mass of liquefied bone marrow. no_mind. 3_attack, enhanced. |
| 11614 (mob) | a skeletal war drummer | 134 | Tier 4 | Undead percussionist that rallies nearby dead. undead. 2_attack, enhanced. Cast: haste, armor. |
| 11615 (mob) | a calcite horror | 140 | Tier 4, Tier 5 | Crystallized bone growth animate with malice. no_mind. 3_attack, enhanced, parry, notrip. |
| 11616 (mob) | a venom puddle ooze | 127 | Tier 5 | Corrosive toxic slime. no_body, no_mind. 2_attack. Spec: spec_poison. |
| 11617 (mob) | a cistern lurker | 136 | Tier 5 | Amphibious predator that hides in toxic pools. 2_attack, enhanced, dodge, trip. |
| 11618 (mob) | a poison dart frog construct | 130 | Tier 5 | Jade-and-gold mechanical frog. no_mind. 2_attack, dodge. Spec: spec_poison. |
| 11619 (mob) | a toxic gas wraith | 135 | Tier 5, Tier 6 | Ghostly cloud of concentrated fumes. undead, no_body. Cast: acid_blast, suffocate. Def: cure_serious. |
| 11620 (mob) | a lava crawler | 137 | Tier 6 | Salamander-like beast that swims through molten stone. 2_attack, enhanced, dodge. Resist: fire. |
| 11621 (mob) | an ember revenant | 139 | Tier 6 | Charred undead warrior wreathed in dying flames. undead. 3_attack, enhanced, parry. Cast: heat_armor, flamestrike. |
| 11622 (mob) | a slag golem | 140 | Tier 6 | Construct of cooled volcanic slag. no_mind. 4_attack, enhanced, nodisarm, notrip. |
| 11623 (mob) | an obsidian gargoyle | 138 | Tier 6, Tier 7 | Winged figure carved from volcanic glass. no_mind. 3_attack, enhanced, dodge, parry. |
| 11624 (mob) | a void acolyte | 137 | Tier 7 | Living cultist sustained by void magic across millennia. 2_attack, enhanced, parry. Cast: mindflame, ego_whip. Def: heal. |
| 11625 (mob) | a sovereign's tomb guardian | 140 | Tier 7 | Elite construct clad in obsidian plate. no_mind, sentinel. 4_attack, enhanced, parry, counter, nodisarm. |
| 11626 (mob) | a moon-touched scorpion | 135 | Tier 5, Tier 6 | Oversized scorpion glowing faintly silver. 2_attack, dodge. Spec: spec_poison. |

**Note on vnum reuse:** Mob vnums 11463–11513 overlap numerically with object vnums in the same range. Cross-type vnum overlap is permitted by the spec; each is unique within its own type index.

---

## Object Definitions

### Piece Item Chain (15 objects: 11450–11464)

All eight boss fragment pieces are delivered via boss loot tables and carry `ITEM_LOOT`.

**Fragment extra_flags:** `ITEM_NODROP(128) | ITEM_BOSS(134217728) | ITEM_MAGIC(64) | ITEM_LOOT(67108864)` = **201326784**

**Assembly extra_flags (created by combining, not looted):** `ITEM_NODROP(128) | ITEM_BOSS(134217728) | ITEM_MAGIC(64)` = **134217920**

**Final Circlet extra_flags:** `ITEM_RARE(1048576) | ITEM_NODROP(128) | ITEM_BOSS(134217728) | ITEM_MAGIC(64) | ITEM_LOOT(67108864)` = **202375360**

All piece and assembly items: `item_type = 29` (ITEM_PIECE), `wear_flags = take(8388608)`.
Final Circlet: `item_type = 9` (ITEM_ARMOR), `wear_flags = head(8) | take(8388608) = 8388616`.

| Obj Vnum | Name | Type | value0 | value1 | value2 | Source |
|----------|------|------|--------|--------|--------|--------|
| 11450 | a shard of the moon sovereign's circlet (I) | piece | 0 | 11451 | 11458 | Moon Gate Guardian loot |
| 11451 | a shard of the moon sovereign's circlet (II) | piece | 11450 | 0 | 11458 | Silent Warden loot |
| 11452 | a shard of the moon sovereign's circlet (III) | piece | 11458 | 0 | 11459 | Rot Warden loot |
| 11453 | a shard of the moon sovereign's circlet (IV) | piece | 11459 | 0 | 11460 | Shadow Warden loot |
| 11454 | a shard of the moon sovereign's circlet (V) | piece | 11460 | 0 | 11461 | Bone Warden loot |
| 11455 | a shard of the moon sovereign's circlet (VI) | piece | 11461 | 0 | 11462 | Venom Warden loot |
| 11456 | a shard of the moon sovereign's circlet (VII) | piece | 11462 | 0 | 11463 (obj) | Fire Warden loot |
| 11457 | a shard of the moon sovereign's circlet (VIII) | piece | 11463 (obj) | 0 | 11464 (obj) | Moon Sovereign loot |
| 11458 | a partial circlet of the moon sovereign (I-II) | piece | 0 | 11452 | 11459 | Created from I+II |
| 11459 | a partial circlet of the moon sovereign (I-III) | piece | 0 | 11453 | 11460 | Created from (I-II)+III |
| 11460 | a partial circlet of the moon sovereign (I-IV) | piece | 0 | 11454 | 11461 | Created from (I-III)+IV |
| 11461 | a partial circlet of the moon sovereign (I-V) | piece | 0 | 11455 | 11462 | Created from (I-IV)+V |
| 11462 | a partial circlet of the moon sovereign (I-VI) | piece | 0 | 11456 | 11463 (obj) | Created from (I-V)+VI |
| 11463 (obj) | a partial circlet of the moon sovereign (I-VII) | piece | 0 | 11457 | 11464 (obj) | Created from (I-VI)+VII |
| 11464 (obj) | the Void Circlet of the Moon Sovereign | armor (head) | — | — | — | **Final combined item** |

**Combining sequence:**
1. Shard I (11450) + Shard II (11451) → Assembly I-II (11458)
2. Assembly I-II (11458) + Shard III (11452) → Assembly I-III (11459)
3. Assembly I-III (11459) + Shard IV (11453) → Assembly I-IV (11460)
4. Assembly I-IV (11460) + Shard V (11454) → Assembly I-V (11461)
5. Assembly I-V (11461) + Shard VI (11455) → Assembly I-VI (11462)
6. Assembly I-VI (11462) + Shard VII (11456) → Assembly I-VII (11463 obj)
7. Assembly I-VII (11463 obj) + Shard VIII (11457) → **the Void Circlet of the Moon Sovereign (11464 obj)**

The `connect` command checks: `pieceA.value0 == pieceB.vnum` OR `pieceA.value1 == pieceB.vnum`. In each step above, one piece's `value1` equals the other piece's vnum, satisfying the check. On success, both are consumed and `pieceA.value2` is created.

---

### Key Objects (2 objects: 11463, 11464 — note: these share vnums with piece-chain assemblies)

Since the piece-chain assembly objects 11463 and 11464 are created by the `connect` command and never loaded from area files, the key objects can safely share those vnums. However, to avoid any confusion, we will use **separate vnums** for the keys: **11514** and **11515**.

| Obj Vnum | Name | item_type | extra_flags | wear_flags | Level | Notes |
|----------|------|-----------|-------------|------------|-------|-------|
| 11514 | a corroded bone-barred key | key (18) | ITEM_NODROP (128) | take (8388608) | L 120 | Opens `^bone-barred gate` (rooms 11601/11606). Spawns via `O` reset in room 11599 (The Reliquary in Ossuary Depths). |
| 11515 | a volcanic glass sentinel key | key (18) | ITEM_NODROP(128) \| ITEM_LOOT(67108864) = 67108992 | take (8388608) | L 140 | Opens `^obsidian sentinel gate` (rooms 11634/11640). Boss loot from Fire Warden. |

**Updated door key references:**
- `^bone-barred gate` (rooms 11601/11606): key vnum **11514**
- `^obsidian sentinel gate` (rooms 11634/11640): key vnum **11515**

---

### Boss Equipment (Level 140, extra_flags = ITEM_NODROP(128) | ITEM_BOSS(134217728) | ITEM_MAGIC(64) = 134217920)

Items delivered via boss loot tables also add ITEM_LOOT(67108864) for total: **201326784**.

Each boss drops its piece shard plus 3 boss gear items via loot table. The Moon Sovereign drops 4 gear items.

Weight key: 1-5 = caster, 6-10 = melee, 11-15 = tank.

**Moon Gate Guardian (mob 11450) — gear vnums 11516–11518:**

| Obj Vnum | Name | item_type | wear_flags | weight | Archetype |
|----------|------|-----------|------------|--------|-----------|
| 11516 | a gauntlet of the obsidian guardian | armor (9) | hands(4096)\|take | 11 | tank |
| 11517 | the guardian's basalt maul | weapon (5) | hold(32768)\|take | 6 | melee; value3=7 (pound) |
| 11518 | a sash of the moon gate | armor (9) | waist(131072)\|take | 6 | melee |

**Silent Warden (mob 11451) — gear vnums 11519–11521:**

| Obj Vnum | Name | item_type | wear_flags | weight | Archetype |
|----------|------|-----------|------------|--------|-----------|
| 11519 | the Silent Warden's death mask | armor (9) | face(16)\|take | 1 | caster |
| 11520 | a shroud of eternal silence | armor (9) | about(65536)\|take | 1 | caster |
| 11521 | the Silent Warden's muffled wrappings | armor (9) | body(262144)\|take | 11 | tank |

**Rot Warden (mob 11452) — gear vnums 11522–11524:**

| Obj Vnum | Name | item_type | wear_flags | weight | Archetype |
|----------|------|-----------|------------|--------|-----------|
| 11522 | a moss-grown breastplate of decay | armor (9) | body(262144)\|take | 11 | tank |
| 11523 | the Rot Warden's fungal staff | weapon (5) | hold(32768)\|take | 1 | caster; ITEM_TWO_HANDED; value3=7 (pound) |
| 11524 | a ring of creeping rot | armor (9) | finger(8192)\|take | 1 | caster |

Note for 11523: extra_flags = 201326784 | ITEM_TWO_HANDED(2147483648) = 2348810432

**Shadow Warden (mob 11453) — gear vnums 11525–11527:**

| Obj Vnum | Name | item_type | wear_flags | weight | Archetype |
|----------|------|-----------|------------|--------|-----------|
| 11525 | the Shadow Warden's eclipse circlet | armor (9) | head(8)\|take | 1 | caster |
| 11526 | robes of living shadow | armor (9) | body(262144)\|take | 1 | caster |
| 11527 | the Shadow Warden's umbral blade | weapon (5) | hold(32768)\|take | 6 | melee; value3=2 (stab) |

**Bone Warden (mob 11454) — gear vnums 11528–11530:**

| Obj Vnum | Name | item_type | wear_flags | weight | Archetype |
|----------|------|-----------|------------|--------|-----------|
| 11528 | greaves of the bone warden | armor (9) | legs(1048576)\|take | 11 | tank |
| 11529 | the Bone Warden's skull flail | weapon (5) | hold(32768)\|take | 6 | melee; value3=7 (pound) |
| 11530 | pauldrons of fused bone | armor (9) | shoulders(512)\|take | 11 | tank |

**Venom Warden (mob 11455) — gear vnums 11531–11533:**

| Obj Vnum | Name | item_type | wear_flags | weight | Archetype |
|----------|------|-----------|------------|--------|-----------|
| 11531 | a scale-mail hauberk of venom | armor (9) | body(262144)\|take | 11 | tank |
| 11532 | the Venom Warden's fang-blade | weapon (5) | hold(32768)\|take | 6 | melee; ITEM_FIST; value3=5 (claw) |
| 11533 | a serpent-coil armlet | armor (9) | wrist(2048)\|take | 1 | caster |

Note for 11532: extra_flags = 201326784 | ITEM_FIST(1073741824) = 1275068608

**Fire Warden (mob 11456) — gear vnums 11534–11536:**

| Obj Vnum | Name | item_type | wear_flags | weight | Archetype |
|----------|------|-----------|------------|--------|-----------|
| 11534 | a magma-forged breastplate | armor (9) | body(262144)\|take | 11 | tank |
| 11535 | the Fire Warden's ember-claw | weapon (5) | hold(32768)\|take | 6 | melee; ITEM_FIST; value3=5 (claw) |
| 11536 | a crest of volcanic stone | armor (9) | horns(4)\|take | 6 | melee |

Note for 11535: extra_flags = 201326784 | ITEM_FIST(1073741824) = 1275068608

**Moon Sovereign (mob 11457) — gear vnums 11537–11540 (4 items, final boss):**

| Obj Vnum | Name | item_type | wear_flags | weight | Archetype |
|----------|------|-----------|------------|--------|-----------|
| 11537 | the Moon Sovereign's void scepter | weapon (5) | hold(32768)\|take | 1 | caster; ITEM_TWO_HANDED; value3=7 (pound) |
| 11538 | robes of the eternal moon | armor (9) | body(262144)\|take | 1 | caster |
| 11539 | the sovereign's amulet of the void | armor (9) | neck(128)\|take | 1 | caster |
| 11540 | boots of the underworld throne | armor (9) | feet(2097152)\|take | 6 | melee |

Note for 11537: extra_flags = 201326784 | ITEM_TWO_HANDED(2147483648) = 2348810432

---

### Trash Equipment (Levels 125–130, ITEM_NODROP only unless loot-table)

Reset gear extra_flags: `ITEM_NODROP(128)` = **128**
Loot-table gear extra_flags: `ITEM_NODROP(128) | ITEM_LOOT(67108864)` = **67108992**

All items include `take(8388608)` in wear_flags.

| Obj Vnum | Name | item_type | wear_flags | weight | Level | Delivery | Source |
|----------|------|-----------|------------|--------|-------|----------|--------|
| 11541 | a moon skeleton's blade | weapon (5) | hold\|take | 6 | L 127 | Reset (E) | moon-cursed skeleton |
| 11542 | a mummy's decay sash | armor (9) | waist\|take | 11 | L 128 | Loot | desiccated mummy |
| 11543 | a scorpion-sting gauntlet | armor (9) | hands\|take | 11 | L 130 | Loot | venomous desert scorpion |
| 11544 | a ghostly grave shroud | armor (9) | about\|take | 1 | L 126 | Loot | grave robber's specter |
| 11545 | a hyena-hide belt | armor (9) | waist\|take | 6 | L 132 | Loot | sand hyena |
| 11546 | basalt guardian fist | weapon (5) | hold\|take | 11 | L 135 | Reset (E) | basalt guardian statue; ITEM_FIST; value3=0 |
| 11547 | canopic abomination mandible | weapon (5) | hold\|take | 6 | L 129 | Loot | canopic abomination; value3=10 (bite) |
| 11548 | a jade asp-fang earring | armor (9) | ear\|take | 1 | L 133 | Loot | jade asp |
| 11549 | desert vulture wing-cloak | armor (9) | about\|take | 1 | L 136 | Loot | desert vulture |
| 11550 | an obsidian golem's core shard | armor (9) | hold\|take | 11 | L 130 | Loot | obsidian golem; shield |
| 11551 | decay acolyte's cursed headwrap | armor (9) | head\|take | 1 | L 138 | Reset (E) | decay acolyte |
| 11552 | rot wight's bone ring | armor (9) | finger\|take | 1 | L 134 | Loot | rot wight |
| 11553 | stone scarab sentinel shell plate | armor (9) | body\|take | 11 | L 137 | Reset (E) | stone scarab sentinel |
| 11554 | a cobra-scale bracer | armor (9) | wrist\|take | 11 | L 131 | Loot | tomb cobra |
| 11555 | a shadow archer's bow wrap | armor (9) | arms\|take | 6 | L 139 | Loot | shadow archer |
| 11556 | shadow vulture-feather mantle | armor (9) | shoulders\|take | 6 | L 140 | Loot | great shadow vulture |
| 11557 | shadow face guardian stone buckler | armor (9) | hold\|take | 11 | L 136 | Reset (E) | shadow face guardian; ITEM_BUCKLER |
| 11558 | creeping shadow cowl | armor (9) | head\|take | 1 | L 132 | Loot | creeping shadow |
| 11559 | a shadow assassin's sand-silk hood | armor (9) | head\|take | 1 | L 138 | Loot | shadow assassin |
| 11560 | an animated bone pillar's knuckle-spike | weapon (5) | hold\|take | 11 | L 140 | Reset (E) | animated bone pillar; ITEM_FIST; value3=0 |
| 11561 | venomous cobra fang pendant | armor (9) | neck\|take | 1 | L 135 | Loot | venomous golden cobra |
| 11562 | bone cultist's obsidian dagger | weapon (5) | hold\|take | 6 | L 137 | Loot | bone cultist; value3=2 (stab) |
| 11563 | death hawk talons | weapon (5) | hold\|take | 6 | L 139 | Loot | death hawk; value3=5 (claw) |
| 11564 | bone revenant's burial ring | armor (9) | finger\|take | 1 | L 133 | Loot | bone revenant |
| 11565 | void specter-touched robes | armor (9) | body\|take | 1 | L 136 | Reset (E) | void specter |
| 11566 | cursed moon shade's halo | armor (9) | halo\|take | 1 | L 138 | Reset (E) | cursed moon shade |
| 11567 | obsidian colossus's stone bracer | armor (9) | wrist\|take | 11 | L 142 | Reset (E) | obsidian colossus |
| 11568 | a death-sworn knight's greatsword | weapon (5) | hold\|take | 6 | L 140 | Reset (E) | death-sworn knight; ITEM_TWO_HANDED; value3=3 (slash) |
| 11569 | magma drake scale boots | armor (9) | feet\|take | 11 | L 140 | Reset (E) | magma drake |
| 11570 | a jade sphinx shoulder plate | armor (9) | shoulders\|take | 11 | L 142 | Reset (E) | jade sphinx |
| 11571 | scarab swarm chitin ring | armor (9) | finger\|take | 1 | L 125 | Loot | desert tomb scarab swarm |
| 11572 | a carrion fly carapace brooch | armor (9) | finger\|take | 1 | L 125 | Loot | carrion fly swarm |
| 11573 | a skeletal rat-tail lash | weapon (5) | hold\|take | 6 | L 125 | Loot | skeletal desert rat; value3=1 (whip) |
| 11574 | bone crawler chitin pauldrons | armor (9) | shoulders\|take | 11 | L 127 | Loot | bone crawler |
| 11575 | a revenant's tattered girdle | armor (9) | waist\|take | 11 | L 129 | Loot | sand-choked revenant |
| 11576 | an animated obelisk buckler | armor (9) | hold\|take | 11 | L 132 | Loot | animated obelisk shard; ITEM_BUCKLER |
| 11577 | a cursed laborer's stone trowel | weapon (5) | hold\|take | 6 | L 130 | Loot | cursed pyramid laborer; value3=7 (pound) |
| 11578 | a desert pit viper fang | armor (9) | ear\|take | 1 | L 128 | Loot | desert pit viper |
| 11579 | an obsidian sentinel's faceplate | armor (9) | face\|take | 11 | L 134 | Reset (E) | obsidian sentinel construct |
| 11580 | a moon glyph guardian stone slab | armor (9) | hold\|take | 11 | L 135 | Loot | moon glyph guardian; ITEM_BUCKLER |
| 11581 | an animated burial jar lid | armor (9) | head\|take | 1 | L 132 | Loot | animated burial jar |
| 11582 | a scribal phantom's cursed inkpot | armor (9) | hold\|take | 1 | L 136 | Loot | scribal phantom |
| 11583 | linen wraps of the mummy thrall | armor (9) | wrist\|take | 11 | L 133 | Loot | sand-bound mummy thrall |
| 11584 | a rot priest thrall's robe | armor (9) | body\|take | 1 | L 138 | Reset (E) | rot priest's thrall |
| 11585 | a fungal wraith's wisp veil | armor (9) | about\|take | 1 | L 137 | Loot | fungal wraith |
| 11586 | a curse-bound sentinel's iron pauldron | armor (9) | shoulders\|take | 11 | L 139 | Loot | curse-bound sentinel |
| 11587 | an entombed general's war crown | armor (9) | head\|take | 11 | L 138 | Reset (E) | entombed general |
| 11588 | a bone golem's riveted knuckle | weapon (5) | hold\|take | 11 | L 140 | Loot | bone golem; ITEM_FIST; value3=0; extra_flags = NODROP+LOOT+FIST = 1140850816 |
| 11589 | a crypt stalker's shadow wrapping | armor (9) | about\|take | 1 | L 136 | Loot | crypt stalker |
| 11590 | magma lizard scale vest | armor (9) | body\|take | 11 | L 137 | Loot | magma lizard |
| 11591 | charred bone husk smoldering bracers | armor (9) | wrist\|take | 11 | L 139 | Loot | charred bone husk |
| 11592 | a void sentinel's stone gauntlets | armor (9) | hands\|take | 11 | L 140 | Reset (E) | void sentinel |
| 11593 | a gilded moon shade's helm | armor (9) | head\|take | 1 | L 138 | Loot | gilded moon shade |
| 11594 | a toxic sand elemental's howling band | armor (9) | wrist\|take | 1 | L 139 | Reset (E) | toxic sand elemental |
| 11595 | an ancient basalt guardian's plate arm | armor (9) | arms\|take | 11 | L 142 | Reset (E) | ancient basalt guardian |
| 11596 | a bone overlord's burial circlet | armor (9) | head\|take | 11 | L 140 | Reset (E) | greater bone overlord |
| 11597 | a petrified sand wurm's calcified fang | weapon (5) | hold\|take | 6 | L 134 | Loot | petrified sand wurm; value3=10 (bite) |
| 11598 | a tomb jackal's obsidian collar | armor (9) | neck\|take | 11 | L 130 | Loot | tomb guardian jackal |
| 11599 | embalmed crocodile scale greaves | armor (9) | legs\|take | 11 | L 133 | Loot | embalmed crocodile |
| 11600 | a dust devil's swirling band | armor (9) | finger\|take | 1 | L 127 | Loot | dust devil spirit |
| 11601 (obj) | a whispering wraith's torn veil | armor (9) | face\|take | 1 | L 131 | Loot | whispering wraith |
| 11602 (obj) | calcified serpent scale pauldrons | armor (9) | shoulders\|take | 11 | L 129 | Loot | calcified serpent |
| 11603 (obj) | tomb beetle mandible gauntlets | armor (9) | hands\|take | 11 | L 136 | Loot | tomb beetle colossus |
| 11604 (obj) | a mold-ridden ghoul's festering girdle | armor (9) | waist\|take | 1 | L 135 | Loot | mold-ridden ghoul |
| 11605 (obj) | a bloated corpse walker's burst vest | armor (9) | body\|take | 11 | L 132 | Loot | bloated corpse walker |
| 11606 (obj) | decay spider silk wraps | armor (9) | wrist\|take | 1 | L 128 | Loot | decay spider |
| 11607 (obj) | a mildew phantom's dripping cowl | armor (9) | head\|take | 1 | L 134 | Loot | mildew phantom |
| 11608 (obj) | shadow hound fang necklace | armor (9) | neck\|take | 6 | L 137 | Loot | shadow hound |
| 11609 (obj) | a nightmare sentinel's black iron helm | armor (9) | head\|take | 11 | L 139 | Reset (E) | nightmare sentinel |
| 11610 (obj) | shade dancer's flickering sash | armor (9) | waist\|take | 1 | L 133 | Loot | shade dancer |
| 11611 (obj) | a darkfire wisp's ember cinder | armor (9) | hold\|take | 1 | L 126 | Loot | darkfire wisp; light source |
| 11612 (obj) | an ossuary spider's bone-silk bracer | armor (9) | wrist\|take | 1 | L 131 | Loot | ossuary spider |
| 11613 (obj) | a marrow elemental's slick boots | armor (9) | feet\|take | 11 | L 138 | Loot | marrow elemental |
| 11614 (obj) | a skeletal war drummer's bone drumstick | weapon (5) | hold\|take | 6 | L 134 | Loot | skeletal war drummer; value3=7 (pound) |
| 11615 (obj) | calcite horror shard plate | armor (9) | body\|take | 11 | L 140 | Loot | calcite horror |
| 11616 (obj) | a venom puddle ooze's corroded ring | armor (9) | finger\|take | 1 | L 127 | Loot | venom puddle ooze |
| 11617 (obj) | cistern lurker scale leggings | armor (9) | legs\|take | 11 | L 136 | Loot | cistern lurker |
| 11618 (obj) | a jade dart frog's golden eye | armor (9) | ear\|take | 1 | L 130 | Loot | poison dart frog construct |
| 11619 (obj) | toxic gas wraith's acrid shroud | armor (9) | about\|take | 1 | L 135 | Loot | toxic gas wraith |
| 11620 (obj) | lava crawler's igneous hide vest | armor (9) | body\|take | 11 | L 137 | Loot | lava crawler |
| 11621 (obj) | an ember revenant's smoldering blade | weapon (5) | hold\|take | 6 | L 139 | Loot | ember revenant; value3=3 (slash) |
| 11622 (obj) | a slag golem's cooled-iron fist | weapon (5) | hold\|take | 11 | L 140 | Reset (E) | slag golem; ITEM_FIST; value3=0 |
| 11623 (obj) | obsidian gargoyle wing-plate pauldrons | armor (9) | shoulders\|take | 11 | L 138 | Loot | obsidian gargoyle |
| 11624 (obj) | a void acolyte's dark prayer beads | armor (9) | neck\|take | 1 | L 137 | Loot | void acolyte |
| 11625 (obj) | sovereign tomb guardian's obsidian greaves | armor (9) | legs\|take | 11 | L 140 | Reset (E) | sovereign's tomb guardian |
| 11626 (obj) | moon-touched scorpion tail barb | weapon (5) | hold\|take | 6 | L 135 | Loot | moon-touched scorpion; value3=2 (stab); spec_poison |
| 11627 (obj) | void-touched juggernaut's core plate | armor (9) | body\|take | 11 | L 142 | Reset (E) | void-touched juggernaut |
| 11628 (obj) | cistern matriarch's venom-drip crown | armor (9) | head\|take | 1 | L 140 | Reset (E) | cistern matriarch |
| 11629 (obj) | petrified desert titan's stone fist | weapon (5) | hold\|take | 11 | L 142 | Reset (E) | petrified desert titan; ITEM_FIST; value3=0 |
| 11630 (obj) | fungal colossus spore-cap helm | armor (9) | head\|take | 1 | L 138 | Reset (E) | fungal colossus |
| 11631 (obj) | nightmare stalker's shadowed claws | weapon (5) | hold\|take | 6 | L 140 | Reset (E) | nightmare stalker; value3=5 (claw) |

**Note:** Object vnums 11541–11631 overlap with mob vnums in the same range. Cross-type overlap is permitted; each is unique within its type index.

---

## Resets Plan

### Boss Mob Resets

```
M 0 11450 1 11495  Moon Gate Guardian in Moon Gate Chamber
M 0 11451 1 11530  Silent Warden in Silent Warden's Tomb
M 0 11452 1 11560  Rot Warden in Rot Warden's Chamber
M 0 11453 1 11585  Shadow Warden in Shadow Warden's Sanctum
M 0 11454 1 11605  Bone Warden in Bone Warden's Crypt
M 0 11455 1 11625  Venom Warden in Venom Warden's Cistern
M 0 11456 1 11639  Fire Warden in Fire Warden's Crucible
M 0 11457 1 11649  Moon Sovereign in Void Throne Room
```

### Boss Loot Tables (l/L extensions on boss mobs)

Each boss carries its piece shard plus 3 gear items (Moon Sovereign: shard + 4 gear).
`loot_amount`: Set based on guaranteed drops. Each boss uses a dedicated loot slot per item.

Example for Moon Gate Guardian (mob 11450):
```
l 400 11450 11516 11517 11518 0 0 0 0 0 0
L 25 25 25 25 0 0 0 0 0
```
This guarantees 4 drops: the piece shard (11450 object), guardian gauntlet (11516), basalt maul (11517), moon gate sash (11518), each with equal 25% selection weight. The Moon Sovereign is structured similarly with 5 slots.

### Strong Mob Resets (limit 2–3 each)

```
M 0 11458 3 11487  cursed moon shade (multiple rooms)
M 0 11458 3 11466  cursed moon shade (another spawn)
M 0 11459 2 11463  obsidian colossus
M 0 11459 2 11498  obsidian colossus
M 0 11460 2 11544  death-sworn knight
M 0 11460 2 11556  death-sworn knight
M 0 11461 2 11617  magma drake
M 0 11461 2 11628  magma drake
M 0 11462 2 11597  jade sphinx
M 0 11462 2 11608  jade sphinx
M 0 11510 2 11463  toxic sand elemental
M 0 11510 2 11498  toxic sand elemental
M 0 11511 2 11546  ancient basalt guardian
M 0 11511 2 11563  ancient basalt guardian
M 0 11512 2 11569  greater bone overlord
M 0 11512 2 11588  greater bone overlord
M 0 11627 2 11635  void-touched juggernaut
M 0 11627 2 11641  void-touched juggernaut
M 0 11628 2 11610  cistern matriarch
M 0 11628 2 11616  cistern matriarch
M 0 11629 2 11457  petrified desert titan
M 0 11629 2 11475  petrified desert titan
M 0 11630 2 11535  fungal colossus
M 0 11630 2 11541  fungal colossus
M 0 11631 2 11571  nightmare stalker
M 0 11631 2 11582  nightmare stalker
```

### Trash Mob Resets (representative sample, ~60–80 total resets)

Distributed across zones by level:
- **Ground Level (11451–11494):** Desert tomb scarab swarms, moon-cursed skeletons, desiccated mummies, venomous scorpions, grave robber's specters, sand hyenas, basalt guardian statues, canopic abominations, animated obelisk shards, cursed pyramid laborers, desert pit vipers, obsidian sentinel constructs, carrion fly swarms, skeletal desert rats, bone crawlers, sand-choked revenants, tomb guardian jackals, dust devil spirits, embalmed crocodiles, petrified sand wurms (~36 resets)
- **Tier 1 — Silent Crypts (11496–11529):** Jade asps, desert vultures, obsidian golems, basalt statues, cursed laborers, sentinel constructs, moon glyph guardians, animated burial jars, scribal phantoms, sand-bound mummy thralls, scarab swarms, desiccated mummies, whispering wraiths, calcified serpents, tomb beetle colossi, petrified sand wurms, embalmed crocodiles (~26 resets)
- **Tier 2 — Rotting Galleries (11531–11559):** Decay acolytes, rot wights, stone scarab sentinels, tomb cobras, animated burial jars, mummy thralls, rot priest thralls, fungal wraiths, curse-bound sentinels, mold-ridden ghouls, bloated corpse walkers, decay spiders, mildew phantoms, calcified serpents, tomb beetle colossi (~22 resets)
- **Tier 3 — Shadow Nave (11561–11584):** Shadow archers, great shadow vultures, shadow face guardians, creeping shadows, shadow assassins, fungal wraiths, curse-bound sentinels, entombed generals, bone golems, crypt stalkers, shadow hounds, nightmare sentinels, shade dancers, darkfire wisps, mildew phantoms (~20 resets)
- **Tier 4 — Ossuary Depths (11586–11604):** Animated bone pillars, venomous golden cobras, bone cultists, death hawks, bone revenants, bone golems, ossuary spiders, marrow elementals, skeletal war drummers, calcite horrors, darkfire wisps, shadow assassins, creeping shadows (~18 resets)
- **Tier 5 — Venomous Cisterns (11606–11624):** Void specters, venomous golden cobras, death hawks, magma lizards, charred bone husks, venom puddle oozes, cistern lurkers, poison dart frog constructs, toxic gas wraiths, moon-touched scorpions, marrow elementals, calcite horrors (~14 resets)
- **Tier 6 — Molten Sanctum (11626–11638):** Magma lizards, charred bone husks, void specters, lava crawlers, ember revenants, slag golems, obsidian gargoyles, moon-touched scorpions, toxic gas wraiths (~12 resets)
- **Tier 7 — Void Throne (11640–11648):** Void sentinels, gilded moon shades, void acolytes, sovereign's tomb guardians, obsidian gargoyles (~8 resets)

### Equipment Resets (E/G after mob M)

Each trash mob is followed by one `E` reset for their associated reset gear (items delivered via reset do not need ITEM_LOOT). Boss mobs have their gear delivered through loot tables only (no E resets on bosses — loot tables handle all boss drops).

### Object Resets (O commands, items in rooms)

```
O 0 11514 1 11599  Bone-barred key in The Reliquary (Ossuary Depths)
```

### Door Resets (D commands)

Per the Doors Summary table above:
```
D 0 9819  2 1   ancient road south (Southern Oasis side, closed)
D 0 11459 2 1   moon gate (closed)
D 0 11460 0 1   moon gate reverse (closed)
D 0 11491 2 1   obsidian door (closed)
D 0 11492 0 1   obsidian door reverse (closed)
D 0 11494 2 1   lunar vault door (closed)
D 0 11495 0 1   lunar vault door reverse (closed)
D 0 11529 2 1   glyph-sealed door (closed)
D 0 11530 0 1   glyph-sealed door reverse (closed)
D 0 11510 5 1   corroded iron hatch (closed)
D 0 11531 4 1   corroded iron hatch reverse (closed)
D 0 11559 2 1   moss-covered stone door (closed)
D 0 11560 0 1   moss-covered stone door reverse (closed)
D 0 11549 5 1   rusted grate (closed)
D 0 11561 4 1   rusted grate reverse (closed)
D 0 11584 2 1   shadow-wreathed portal (closed)
D 0 11585 0 1   shadow-wreathed portal reverse (closed)
D 0 11604 2 1   skull-arch threshold (closed)
D 0 11605 0 1   skull-arch threshold reverse (closed)
D 0 11601 5 2   bone-barred gate (locked)
D 0 11606 4 2   bone-barred gate reverse (locked)
D 0 11624 2 1   venom-slick gate (closed)
D 0 11625 0 1   venom-slick gate reverse (closed)
D 0 11638 2 1   molten-iron gate (closed)
D 0 11639 0 1   molten-iron gate reverse (closed)
D 0 11634 5 2   obsidian sentinel gate (locked)
D 0 11640 4 2   obsidian sentinel gate reverse (locked)
D 0 11648 2 1   void chamber door (closed)
D 0 11649 0 1   void chamber door reverse (closed)
```

---

## Specials

```
#SPECIALS
M 11450 spec_cast_mage      Moon Gate Guardian
M 11451 spec_cast_undead     Silent Warden
M 11452 spec_cast_mage       Rot Warden
M 11453 spec_cast_bigtime    Shadow Warden
M 11454 spec_cast_mage       Bone Warden
M 11455 spec_poison          Venom Warden
M 11456 spec_breath_fire     Fire Warden
M 11457 spec_cast_bigtime    Moon Sovereign
M 11461 spec_breath_fire     magma drake
M 11466 spec_poison          venomous desert scorpion
M 11467 spec_cast_undead     grave robber's specter
M 11471 spec_poison          jade asp
M 11474 spec_cast_mage       decay acolyte
M 11475 spec_cast_undead     rot wight
M 11477 spec_poison          tomb cobra
M 11481 spec_cast_undead     creeping shadow
M 11484 spec_poison          venomous golden cobra
M 11485 spec_cast_mage       bone cultist
M 11488 spec_cast_undead     void specter
M 11489 spec_disease         carrion fly swarm
M 11490 spec_disease         skeletal desert rat
M 11492 spec_cast_undead     sand-choked revenant
M 11494 spec_cast_undead     cursed pyramid laborer
M 11495 spec_poison          desert pit viper
M 11498 spec_cast_undead     animated burial jar
M 11499 spec_cast_mage       scribal phantom
M 11501 spec_cast_mage       rot priest's thrall
M 11502 spec_cast_undead     fungal wraith
M 11510 spec_cast_mage       toxic sand elemental
M 11512 spec_cast_undead     greater bone overlord
M 11601 spec_cast_undead     whispering wraith
M 11602 spec_poison          calcified serpent
M 11604 spec_disease         mold-ridden ghoul
M 11606 spec_poison          decay spider
M 11607 spec_cast_undead     mildew phantom
M 11611 spec_cast_mage       darkfire wisp
M 11612 spec_poison          ossuary spider
M 11614 spec_cast_mage       skeletal war drummer
M 11616 spec_poison          venom puddle ooze
M 11618 spec_poison          poison dart frog construct
M 11619 spec_cast_undead     toxic gas wraith
M 11621 spec_cast_mage       ember revenant
M 11624 spec_cast_bigtime    void acolyte
M 11626 spec_poison          moon-touched scorpion
M 11628 spec_poison          cistern matriarch
M 11629 spec_cast_mage       petrified desert titan
M 11630 spec_disease         fungal colossus
S
```

---

## Item Flag Reference

| Category | Item flag composition | Numeric value |
|----------|-----------------------|---------------|
| Boss piece shards (loot) | NODROP+BOSS+MAGIC+LOOT | 201326784 |
| Assembly pieces (created) | NODROP+BOSS+MAGIC | 134217920 |
| Final Circlet | RARE+NODROP+BOSS+MAGIC+LOOT | 202375360 |
| Boss gear via loot table | NODROP+BOSS+MAGIC+LOOT | 201326784 |
| Boss gear via reset (E) | NODROP+BOSS+MAGIC | 134217920 |
| Trash gear via loot table | NODROP+LOOT | 67108992 |
| Trash gear via reset (E) | NODROP | 128 |
| Keys (room spawn) | NODROP | 128 |
| Keys (boss loot) | NODROP+LOOT | 67108992 |
| Fist weapon boss loot | NODROP+BOSS+MAGIC+LOOT+FIST | 1275068608 |
| Two-handed weapon boss loot | NODROP+BOSS+MAGIC+LOOT+TWO_HANDED | 2348810432 |
| Shadow face guardian buckler (trash reset) | NODROP+BUCKLER | 268435584 |
| Animated bone pillar fist (trash reset) | NODROP+FIST | 1073741952 |
| Death-sworn knight greatsword (trash reset) | NODROP+TWO_HANDED | 2147483776 |
| Basalt guardian fist (trash reset) | NODROP+FIST | 1073741952 |
| Animated obelisk buckler (trash loot) | NODROP+LOOT+BUCKLER | 335544320 |
| Moon glyph guardian buckler (trash loot) | NODROP+LOOT+BUCKLER | 335544320 |
| Bone golem knuckle (trash loot) | NODROP+LOOT+FIST | 1140850816 |
