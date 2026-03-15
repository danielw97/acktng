# Area Plan: The Northern Pyramid

## Overview

**Area Name:** `@@yNorthern @@bPyramid@@N`
**File Name:** `northern_pyramid.are`
**Keyword:** `northernpyramid`
**Level Range:** 100–120 (I directive: `I 100 120`)
**Level Label:** `@@W(@@y100 120@@W)@@N`
**Vnum Range:** 2864–3063 (V directive: `V 2864 3063`)
**Vnum Count:** 35
**Area Number:** `N 75`
**Reset Rate:** `F 20`
**Reset Message:** `@@ythe ancient pyramid groans as hot desert winds sweep its stone passages@@N`

### Color Theme

- **Primary:** `@@y` (yellow) — sun-baked sand, golden stone, ancient gold glyphs
- **Secondary:** `@@b` (brown) — weathered sandstone blocks, dried timber, ancient earth
- **Accent:** `@@c` (cyan) — lapis lazuli inlay, deep water cisterns, arcane inscriptions
- **Danger:** `@@R` (red) — ancient curse marks, blood, boss threat indicators
- **Text reset:** `@@N` after all color sequences

The theme evokes an ancient desert pyramid of immense scale: its sandstone blocks worn smooth by millennia of wind, its interior passages choked with collapsed ceilings and animated by the curses of the pharaohs entombed within. Each ascending floor grows more sacred and more deadly.

---


## Lore
See `northern_pyramid_lore.md` for extracted lore.

## Area Header Notes

- `Q 16` — mandatory version
- `K northernpyramid~` — single-word keyword, no spaces, no color codes
- `L @@W(@@y100 120@@W)@@N~` — white parens, yellow level numbers, white close paren, reset
- `N 75` — confirmed unused across all loaded area files
- `I 100 120` — area-gating and mob-level policy
- `V 2864 3063` — full 35-vnum envelope; all rooms, mobs, and objects stay within this range
- `F 20` — 20-minute reset frequency
- `U` — reset message as above; no double-newlines, terminated with `~`
- `O Virant~` — area owner must always be Virant
- `T` — teleport flag line (rest ignored by loader)

---

## Level Structure

Seven levels with the trap room as a special single room:

| Level | Name | Vnums | Rooms | Boss Room |
|-------|------|-------|-------|-----------|
| Trap Room | The Stone Trap Chamber | 2864 | 1 | — (no_mob) |
| Sub-ground / Undercroft | The Pharaoh's Undercroft | 2865–2894 | 30 | 2894 |
| Ground Level | The Pyramid Threshold | 2895–2939 | 45 | 2939 |
| Floor 1 | The Hall of the Scarab | 2940–2974 | 35 | 2974 |
| Floor 2 | The Chamber of the High Priests | 2975–3004 | 30 | 3004 |
| Floor 3 | The Eternal Vault | 3005–3029 | 25 | 3029 |
| Floor 4 | The Drake's Ascent | 3030–3049 | 20 | 3049 |
| Floor 5 / Apex | The Summit Sanctum | 3050–3063 | 14 | 3063 |

**Total rooms: 35** (all vnums 2864–3063 used)

---

## Connection to the Great Oasis (Northern Approach)

To align with current regional canon, the Northern Pyramid route is treated as a **Great Oasis frontier road** rather than a standalone "Northern Oasis" settlement route. The immediate gameplay connector remains the same vnum link, but its lore context is now the Great Oasis northern caravan approach and Measuring House survey network.

The Northern Pyramid connects bidirectionally to **room 2211** ("Golden Dust Walk") as the eastern caravan spur from the Great Oasis water-court domain. This keeps the established traversal path while aligning with Great Oasis lore that frames pyramid access as a chartered survey road inherited from Keeper-era astronomy and taxation infrastructure.

| Direction | From | To | Notes |
|-----------|------|----|-------|
| east (D1) | 2211 (Golden Dust Walk, Great Oasis northern approach) | 2895 (Before the Northern Pyramid) | Players leave the oasis road network heading east |
| west (D3) | 2895 (Before the Northern Pyramid) | 2211 (Golden Dust Walk, Great Oasis northern approach) | Return path to the Great Oasis approach charter road |

### Required Edit to oasis connector area

Room 2211 currently has exits D0→2191 (north), D2→2231 (south), D3→2210 (west). Add a D1 (east) exit:

```
D1
The charter road continues east through the dunes, where a @@bsandstone@@N pyramid shoulder rises against the horizon.

~
^charter road east~
0 -1 2895
```

The room description for 2211 should mention that this is a **Keeper-surveyed charter spur** from the Great Oasis basin and that Midgaard-sponsored scholars occasionally mark the stones with catalog glyphs before turning toward the pyramid.

Room 2895 (the pyramid entry room) receives a D3 (west) exit back to 2211:

```
D3
The charter road curves west through the dunes, returning toward the Great Oasis basin and its water-court lanterns.

~
~
0 -1 2211
```

---

## Lore Integration Requirements (Great Oasis / Eastern Desert / Midgaard)
These requirements expand implementation-facing lore guidance so builders can keep room text, mobiles, objects, and scripted flavor aligned with regional canon while preserving the existing mechanical layout.

### Canon Positioning

- Treat the Northern Pyramid as a **Sand Sovereign state monument** downstream from Great Oasis Keeper science and charter-law traditions.
- Emphasize that the route is a **charter survey spur** of the oasis basin network, not an isolated wilderness path.
- Preserve the historical throughline: integrated road/cistern governance → drought-era fracture → modern ruin expeditions.

### Great Oasis Alignment Rules

- Approach-room text should reference water-court legal language, Measuring House calibration marks, and legacy charter waystones.
- Environmental details on the western connector should imply maintained institutional memory (engraved distances, witness glyphs, toll notches), even when partially eroded.
- Avoid presenting the pyramid as culturally independent from oasis engineering lineages.

### Eastern Desert Alignment Rules

- Inscriptions and relic flavor should acknowledge shared vocabulary with Eastern Desert obelisk, cistern, and funerary traditions.
- When possible, link hazard design to governance collapse (sealed archives, ration tallies, redirected levy marks) rather than "random ancient curse" language.
- Maintain continuity with desert schism themes: official doctrine vs. cult reinterpretation vs. practical caravan survival.

### Midgaard Alignment Rules

- Include occasional signs of archive contact: catalog marks, copied rubbings, survey tags, or disputable translation plaques left by prior expeditions.
- Distinguish in-situ meaning from western reinterpretation: local inscriptions should feel authoritative, while imported labels may be incomplete or wrong.
- Keep Midgaard references atmospheric and diegetic, not expository lore dumps in every room.

### Tone and Content Guardrails

- Keep Northern Pyramid voice **administrative-celestial**: law, measure, procession, calibration, sanctioned ascent.
- Trap and ward language should imply procedural enforcement of doctrine (trespass correction, purification sequence, restricted witness access).
- Reserve overt apocalyptic rhetoric for high-tier or boss-adjacent spaces; lower tiers should feel bureaucratic before they feel mythic.

---

## Room Layout (35 rooms)

### Room Flag Policy

- **`no_teleport`**: Applied to **all 35 rooms**. Players and mobs cannot be teleported into any room in the pyramid.
- **`no_recall`**: Applied to **all rooms except** inter-level stairwell/transit rooms and the final boss room (3063). Stairwells and 3063 allow recall so players can escape between floors and exit after defeating the final boss.

Inter-level rooms that are **exempt from `no_recall`** (have only `no_mob, no_teleport, indoors`): 2865, 2916, 2923, 2924, 2940, 2941, 2954, 2955, 2975, 2976, 2999, 3005, 3017, 3030, 3043, 3050, 3063.

All other rooms (including boss rooms 2894, 2939, 2974, 3004, 3029, 3049) carry `no_recall`.

---

### Zone 0: The Stone Trap Chamber (1 room: 2864)

This special room exists outside the pyramid's normal vertical progression. Hidden pressure plates and disguised pit covers scattered throughout the pyramid all funnel into this one chamber. It has **no exit except D5 (down) to 2873** in the Undercroft. Players who fall in must navigate the Undercroft to escape.

| Vnum | Name | Sector | Flags |
|------|------|--------|-------|
| 2864 | The Stone Trap Chamber | inside | no_mob, indoors, no_recall, no_teleport |

**Exits:**
- D5 (down) → 2873 (The Sunken Trap-Fall in Undercroft); no keyword; no door

**One-way trap exits leading IN to 2864 from across the pyramid (these rooms have exits pointing to 2864; 2864 has no reciprocal exit back to them):**

| Source Room | Vnum | Direction | Notes |
|-------------|------|-----------|-------|
| The False Floor Gallery (Ground Level) | 2917 | D5 (down) | Hidden pit, no keyword |
| The Illusory Corridor (Floor 1) | 2959 | D4 (up) | Appears to lead higher but drops |
| The Marked Flagstones (Floor 2) | 2989 | D5 (down) | Pressure plate |
| The Treacherous Overhang (Floor 3) | 3016 | D4 (up) | False ceiling hatch |

Each source room description must describe the hazard (cracked floor, suspicious passage, etc.) without explicitly naming the trap destination, so players can discover it as a risk.

---

### Zone 1: The Pharaoh's Undercroft (30 rooms: 2865–2894)

The subterranean foundations of the pyramid: flooded burial chambers, root-tangled catacombs, collapsed archways. The oldest and most primitive construction in the structure. Sector type: `inside` (11) throughout. Most rooms flagged `dark`.

| Vnum | Room Name | Sector | Flags | Notes |
|------|-----------|--------|-------|-------|
| 2865 | The Undercroft Entry | inside | no_mob, no_teleport, indoors | Entry via trapdoor from ground level (2916 ↓) |
| 2866 | A Flooded Antechamber | inside | no_recall, no_teleport, indoors, dark | Ankle-deep black water |
| 2867 | The Root-Split Corridor | inside | no_recall, no_teleport, indoors, dark | Ancient roots have fractured the stone |
| 2868 | A Collapsed Burial Niche | inside | no_recall, no_teleport, indoors, dark | Rubble-filled alcove |
| 2869 | The Drain Tunnel | inside | no_recall, no_teleport, indoors, dark | Carries overflow from flooded chambers |
| 2870 | A Crypt of Unnamed Dead | inside | no_recall, no_teleport, indoors | Row of sealed sarcophagi |
| 2871 | The Embalming Chamber | inside | no_recall, no_teleport, indoors | Ancient tools and canopic tables |
| 2872 | A Stone-Floored Passage | inside | no_recall, no_teleport, indoors, dark | |
| 2873 | The Sunken Trap-Fall | inside | no_recall, no_teleport, indoors, dark | Landing point for the Stone Trap (2864 ↓ arrives here) |
| 2874 | A Narrow Root Tunnel | inside | no_recall, no_teleport, indoors, dark | |
| 2875 | The Ossuary | inside | no_recall, no_teleport, indoors, dark | Bones stacked floor to ceiling |
| 2876 | A Sunken Gallery | inside | no_recall, no_teleport, indoors | Faded reliefs on the walls |
| 2877 | The Underground Cistern | inside | no_recall, no_teleport, indoors | Water still pools here |
| 2878 | A Crumbling Side Passage | inside | no_recall, no_teleport, indoors, dark | |
| 2879 | The Fungal Grotto | inside | no_recall, no_teleport, indoors, dark | Pale mushrooms glow faintly |
| 2880 | An Ancient Prayer Hall | inside | no_recall, no_teleport, indoors | Defaced altar at center |
| 2881 | A Rotted Timber Walkway | inside | no_recall, no_teleport, indoors, dark | Bridge over a flooded pit |
| 2882 | The Deep Vault | inside | no_recall, no_teleport, indoors | Wide chamber, multiple exits |
| 2883 | A Hidden Side Chamber | inside | no_recall, no_teleport, indoors, dark | |
| 2884 | The Sealed Crypt Wing | inside | no_recall, no_teleport, indoors | Three burial niches sealed with glyphs |
| 2885 | A Stone Drainage Arch | inside | no_recall, no_teleport, indoors, dark | |
| 2886 | The Putrid Pool | inside | no_recall, no_teleport, indoors, dark | Stagnant black water, noxious fumes |
| 2887 | A Forgotten Workshop | inside | no_recall, no_teleport, indoors | Abandoned tools of the pyramid builders |
| 2888 | The Underground Crossroads | inside | no_recall, no_teleport, indoors | Four-way junction, central hub |
| 2889 | A Pit of Rubble | inside | no_recall, no_teleport, indoors, dark | Partial collapse |
| 2890 | The Inner Undercroft Hall | inside | no_recall, no_teleport, indoors | Grand chamber, cracked columns |
| 2891 | A Burial Antechamber | inside | no_recall, no_teleport, indoors, dark | |
| 2892 | The Threshold of the Crypt Pharaoh | inside | no_recall, no_teleport, indoors | Approach room; carvings warn of the guardian within |
| 2893 | The Crypt Pharaoh's Approach | inside | no_recall, no_teleport, indoors | Immediately before boss room; carvings of the guardian's deeds |
| 2894 | The Pharaoh's Tomb Chamber | inside | no_mob, no_recall, no_teleport, indoors | **BOSS ROOM** — The Crypt Pharaoh |

**Undercroft connectivity:** Entry from ground level (2916 down, locked trapdoor). The key to that trapdoor spawns via `O` reset in room 2887 so trapped players can find it and escape. 2873 (trap-fall landing) connects into the undercroft network but has NO up exit back to 2864. The undercroft hub is 2888; boss approached via 2892→2893→2894.

---

### Zone 2: The Pyramid Threshold — Ground Level (45 rooms: 2895–2939)

The base of the pyramid and surrounding desert approach. Exterior rooms use `desert` sector; interior rooms use `inside` sector. This is the widest and most populated floor.

| Vnum | Room Name | Sector | Flags | Notes |
|------|-----------|--------|-------|-------|
| 2895 | Before the Northern Pyramid | desert | no_recall, no_teleport | **Entry from Great Oasis northern approach** (D3 west → 2211) |
| 2896 | The Ancient Road East | desert | no_recall, no_teleport | Approach causeway |
| 2897 | The Outer Dune Ring | desert | no_recall, no_teleport | Sand dunes surround the pyramid base |
| 2898 | The Southern Approach | desert | no_recall, no_teleport | Direct path to main entrance |
| 2899 | The Pyramid's Western Face | desert | no_recall, no_teleport | Sheer sandstone wall |
| 2900 | The Pyramid's Eastern Face | desert | no_recall, no_teleport | Hieroglyphs carved into stone |
| 2901 | The Outer Courtyard | desert | no_recall, no_teleport | Wide plaza before the entrance |
| 2902 | The Courtyard Garden Ruin | desert | no_recall, no_teleport | Dried irrigation channels, dead palms |
| 2903 | The Obelisk Circle | desert | no_recall, no_teleport | Ring of cracked obelisks |
| 2904 | The Guard Post Ruin | desert | no_recall, no_teleport | Collapsed stone gatehouse |
| 2905 | The Pyramid's Main Gate | desert | no_recall, no_teleport | Massive carved entrance arch; `^stone gate` (closed door, key -1) |
| 2906 | The Grand Entrance Hall | inside | no_recall, no_teleport, indoors | First interior room; massive pillared hall |
| 2907 | East Entrance Corridor | inside | no_recall, no_teleport, indoors | |
| 2908 | West Entrance Corridor | inside | no_recall, no_teleport, indoors | |
| 2909 | The Pillar Forest | inside | no_recall, no_teleport, indoors | Dense colonnade of cracked columns |
| 2910 | A Collapsed Side Wing | inside | no_recall, no_teleport, indoors | Rubble blocks most of the room |
| 2911 | The Trophy Chamber | inside | no_recall, no_teleport, indoors | Niches once held offerings |
| 2912 | The Inscription Gallery | inside | no_recall, no_teleport, indoors | Floor-to-ceiling hieroglyphs |
| 2913 | A Guard Barracks Ruin | inside | no_recall, no_teleport, indoors, dark | |
| 2914 | The Armory Vault | inside | no_recall, no_teleport, indoors | Empty weapon racks |
| 2915 | The East Wing Hall | inside | no_recall, no_teleport, indoors | |
| 2916 | The Lower Vault | inside | no_mob, no_teleport, indoors | Contains trapdoor to undercroft (D5 down → 2865, locked) |
| 2917 | The False Floor Gallery | inside | no_recall, no_teleport, indoors | Contains trap exit D5 → 2864 (hidden pit, one-way) |
| 2918 | A Side Alcove | inside | no_recall, no_teleport, indoors, dark | |
| 2919 | The West Wing Hall | inside | no_recall, no_teleport, indoors | |
| 2920 | The Stone Library | inside | no_recall, no_teleport, indoors | Collapsed shelves, crumbled tablets |
| 2921 | The Map Chamber | inside | no_recall, no_teleport, indoors | Floor mosaic of the surrounding desert |
| 2922 | The Central Hall | inside | no_recall, no_teleport, indoors | Wide hub room, stairwells rise from here |
| 2923 | The East Stairwell Landing | inside | no_mob, no_teleport, indoors | D4 (up) → Floor 1 entry (2940) |
| 2924 | The West Stairwell Landing | inside | no_mob, no_teleport, indoors | Alternate D4 (up) → 2941 |
| 2925 | A Guard Chamber | inside | no_recall, no_teleport, indoors, dark | |
| 2926 | The Offering Hall | inside | no_recall, no_teleport, indoors | Ceremonial tables; empty basins |
| 2927 | The North Inner Wing | inside | no_recall, no_teleport, indoors | |
| 2928 | The South Inner Wing | inside | no_recall, no_teleport, indoors | |
| 2929 | A Ceremonial Passage | inside | no_recall, no_teleport, indoors | |
| 2930 | The Root-Cracked Chamber | inside | no_recall, no_teleport, indoors, dark | Ancient roots push through the floor |
| 2931 | The Throne Antechamber | inside | no_recall, no_teleport, indoors | Approach to ground-floor boss zone |
| 2932 | The Lower Throne Room | inside | no_recall, no_teleport, indoors | Grand dais, cracked royal chair |
| 2933 | The Sanctum Approach | inside | no_recall, no_teleport, indoors | |
| 2934 | A Preparation Chamber | inside | no_recall, no_teleport, indoors, dark | |
| 2935 | The Sentinel's Passage | inside | no_recall, no_teleport, indoors | |
| 2936 | The Inner Sanctum Entrance | inside | no_recall, no_teleport, indoors | `^ancient stone door` (closed, no lock) leads to 2937 |
| 2937 | The Waiting Hall | inside | no_recall, no_teleport, indoors | |
| 2938 | The Final Approach | inside | no_recall, no_teleport, indoors | |
| 2939 | The Pyramid's Heart Chamber | inside | no_mob, no_recall, no_teleport, indoors | **BOSS ROOM** — The Desert Gate Guardian |

---

### Zone 3: The Hall of the Scarab — Floor 1 (35 rooms: 2940–2974)

First ascending floor. Slightly narrower than the ground level. Scarab imagery dominates the carvings.

| Vnum | Room Name | Sector | Flags | Notes |
|------|-----------|--------|-------|-------|
| 2940 | The First Ascent | inside | no_mob, no_teleport, indoors | Entry from ground D4 (2923 ↑) |
| 2941 | The Upper Landing | inside | no_mob, no_teleport, indoors | Entry from alternate ground D4 (2924 ↑) |
| 2942 | The Scarab Hall | inside | no_recall, no_teleport, indoors | Giant scarab reliefs on every wall |
| 2943 | The Beetle Colonnade | inside | no_recall, no_teleport, indoors | Stone scarabs atop every column |
| 2944 | An Alcove of Offerings | inside | no_recall, no_teleport, indoors, dark | |
| 2945 | The East Passage | inside | no_recall, no_teleport, indoors | |
| 2946 | The West Passage | inside | no_recall, no_teleport, indoors | |
| 2947 | The Ritual Basin Room | inside | no_recall, no_teleport, indoors | Dried ceremonial pools |
| 2948 | A Collapsed Side Chamber | inside | no_recall, no_teleport, indoors, dark | |
| 2949 | The Cartouche Gallery | inside | no_recall, no_teleport, indoors | Names of the ancient priests carved in stone |
| 2950 | The Scribes' Chamber | inside | no_recall, no_teleport, indoors | Broken styluses and dried inkpots |
| 2951 | A Guard Room | inside | no_recall, no_teleport, indoors, dark | |
| 2952 | The Mosaic Floor Room | inside | no_recall, no_teleport, indoors | Floor is one enormous scarab mosaic |
| 2953 | The Inner Colonnade | inside | no_recall, no_teleport, indoors | |
| 2954 | The East Stair Landing | inside | no_mob, no_teleport, indoors | D4 (up) → Floor 2 (2975) |
| 2955 | The West Stair Landing | inside | no_mob, no_teleport, indoors | D4 (up) → Floor 2 (2976) |
| 2956 | A Shrine Alcove | inside | no_recall, no_teleport, indoors, dark | |
| 2957 | The Embalming Gallery | inside | no_recall, no_teleport, indoors | Carved depictions of funerary rites |
| 2958 | The Priests' Corridor | inside | no_recall, no_teleport, indoors | |
| 2959 | The Illusory Corridor | inside | no_recall, no_teleport, indoors | Contains trap exit D4 → 2864 (appears to go higher, one-way) |
| 2960 | A Torch-Bracket Hall | inside | no_recall, no_teleport, indoors, dark | |
| 2961 | The Scarab Shrine | inside | no_recall, no_teleport, indoors | Central shrine, offerings intact |
| 2962 | A Guardians' Niche | inside | no_recall, no_teleport, indoors, dark | |
| 2963 | The Ritual Cleansing Room | inside | no_recall, no_teleport, indoors | |
| 2964 | The Inner Priest Chamber | inside | no_recall, no_teleport, indoors | |
| 2965 | A Narrow Side Passage | inside | no_recall, no_teleport, indoors, dark | |
| 2966 | The Hall of Prayers | inside | no_recall, no_teleport, indoors | |
| 2967 | The Side Archive | inside | no_recall, no_teleport, indoors, dark | |
| 2968 | A Ventilation Shaft Landing | inside | no_recall, no_teleport, indoors | |
| 2969 | The Preparation Hall | inside | no_recall, no_teleport, indoors | |
| 2970 | The Warden's Approach | inside | no_recall, no_teleport, indoors | |
| 2971 | The Outer Guardian Chamber | inside | no_recall, no_teleport, indoors | |
| 2972 | A Final Corridor | inside | no_recall, no_teleport, indoors, dark | |
| 2973 | The Scarab Colossus Approach | inside | no_recall, no_teleport, indoors | `^stone scarab door` (closed, no lock) leads to 2974 |
| 2974 | The Scarab Colossus Chamber | inside | no_mob, no_recall, no_teleport, indoors | **BOSS ROOM** — The Scarab Colossus |

---

### Zone 4: The Chamber of the High Priests — Floor 2 (30 rooms: 2975–3004)

Second floor. Narrower than Floor 1. Sacred priestly quarters and ritual chambers.

| Vnum | Room Name | Sector | Flags | Notes |
|------|-----------|--------|-------|-------|
| 2975 | The Second Ascent | inside | no_mob, no_teleport, indoors | Entry from Floor 1 D4 (2954 ↑) |
| 2976 | The Priests' Landing | inside | no_mob, no_teleport, indoors | Entry from alternate Floor 1 D4 (2955 ↑) |
| 2977 | The High Priests' Hall | inside | no_recall, no_teleport, indoors | |
| 2978 | The Anointing Chamber | inside | no_recall, no_teleport, indoors | |
| 2979 | A Ritual Side Room | inside | no_recall, no_teleport, indoors, dark | |
| 2980 | The Sacred Scroll Room | inside | no_recall, no_teleport, indoors | Crumbled papyrus fragments |
| 2981 | The Altar of the Four Winds | inside | no_recall, no_teleport, indoors | Ceremonial altar; compass points carved in floor |
| 2982 | A Candlelit Niche | inside | no_recall, no_teleport, indoors, dark | |
| 2983 | The Priests' Sleeping Quarters | inside | no_recall, no_teleport, indoors | Stone cots along the walls |
| 2984 | The Inner Prayer Hall | inside | no_recall, no_teleport, indoors | |
| 2985 | A Secret Passage | inside | no_recall, no_teleport, indoors, dark | Hidden behind a loose block |
| 2986 | The Ritual Fire Pit | inside | no_recall, no_teleport, indoors | Ash-filled depression in floor center |
| 2987 | The Vestment Chamber | inside | no_recall, no_teleport, indoors | Empty robing alcoves |
| 2988 | A Corridor of Wards | inside | no_recall, no_teleport, indoors | Protective glyphs cover every surface |
| 2989 | The Marked Flagstones | inside | no_recall, no_teleport, indoors | Contains trap exit D5 → 2864 (pressure plate, one-way) |
| 2990 | The Chanting Hall | inside | no_recall, no_teleport, indoors | Acoustic chamber; voices echo strangely |
| 2991 | A Side Shrine | inside | no_recall, no_teleport, indoors, dark | |
| 2992 | The Vaulted Passage | inside | no_recall, no_teleport, indoors | |
| 2993 | The East Ceremonial Wing | inside | no_recall, no_teleport, indoors | |
| 2994 | The West Ceremonial Wing | inside | no_recall, no_teleport, indoors | |
| 2995 | A Preparation Alcove | inside | no_recall, no_teleport, indoors, dark | |
| 2996 | The Hall of the Sacred Asp | inside | no_recall, no_teleport, indoors | Cobra carvings on every surface |
| 2997 | The Archpriest's Study | inside | no_recall, no_teleport, indoors | |
| 2998 | A Collapsed Vault | inside | no_recall, no_teleport, indoors, dark | |
| 2999 | The Upper Stair Landing | inside | no_mob, no_teleport, indoors | D4 (up) → Floor 3 (3005), through `^iron-banded archway` (closed, no lock) |
| 3000 | The Inner Sanctum Antechamber | inside | no_recall, no_teleport, indoors | |
| 3001 | The Outer Chamber | inside | no_recall, no_teleport, indoors | |
| 3002 | A Final Hall | inside | no_recall, no_teleport, indoors, dark | |
| 3003 | The High Priest's Approach | inside | no_recall, no_teleport, indoors | `^carved stone threshold` (closed, no lock) leads to 3004 |
| 3004 | The High Priest's Sanctum | inside | no_mob, no_recall, no_teleport, indoors | **BOSS ROOM** — The High Priest of the Ancient Sands |

---

### Zone 5: The Eternal Vault — Floor 3 (25 rooms: 3005–3029)

Third floor. Further narrowed. The eternal vault where powerful undead are interred.

| Vnum | Room Name | Sector | Flags | Notes |
|------|-----------|--------|-------|-------|
| 3005 | The Third Ascent | inside | no_mob, no_teleport, indoors | Entry via `^iron-banded archway` from 2999 |
| 3006 | The Vault Antechamber | inside | no_recall, no_teleport, indoors | |
| 3007 | A Sealed Side Chamber | inside | no_recall, no_teleport, indoors, dark | |
| 3008 | The Hall of Eternal Rest | inside | no_recall, no_teleport, indoors | Sealed sarcophagi in wall recesses |
| 3009 | The Embalmer's Gallery | inside | no_recall, no_teleport, indoors | Illustrated burial rites |
| 3010 | The Soul Weighing Chamber | inside | no_recall, no_teleport, indoors | Giant carved scale dominates the room |
| 3011 | A Dark Corridor | inside | no_recall, no_teleport, indoors, dark | |
| 3012 | The Mummy Lords' Wing | inside | no_recall, no_teleport, indoors | |
| 3013 | A Burial Alcove | inside | no_recall, no_teleport, indoors, dark | |
| 3014 | The Preserved Archives | inside | no_recall, no_teleport, indoors | Sealed jars of records; all unreadable |
| 3015 | The Ritual Unguent Room | inside | no_recall, no_teleport, indoors | |
| 3016 | The Treacherous Overhang | inside | no_recall, no_teleport, indoors, dark | Contains trap exit D4 → 2864 (false ceiling hatch, one-way) |
| 3017 | The East Vault Passage | inside | no_mob, no_teleport, indoors | D4 (up) → Floor 4 entry (3030) |
| 3018 | The West Vault Passage | inside | no_recall, no_teleport, indoors | |
| 3019 | The Canopic Shrine | inside | no_recall, no_teleport, indoors | Four canopic jars on stone pedestals |
| 3020 | A Collapsed Crypt Wing | inside | no_recall, no_teleport, indoors, dark | |
| 3021 | The Inner Vault Hall | inside | no_recall, no_teleport, indoors | |
| 3022 | A Final Preparation Room | inside | no_recall, no_teleport, indoors, dark | |
| 3023 | The Warded Threshold | inside | no_recall, no_teleport, indoors | Glyphs of warning on every stone |
| 3024 | The Mummy Lord's Antechamber | inside | no_recall, no_teleport, indoors | |
| 3025 | A Side Passage | inside | no_recall, no_teleport, indoors, dark | |
| 3026 | The Guard Station | inside | no_recall, no_teleport, indoors | |
| 3027 | The Inner Hall | inside | no_recall, no_teleport, indoors | |
| 3028 | The Mummy Lord's Approach | inside | no_recall, no_teleport, indoors | `^glowing stone arch` (closed, no lock) leads to 3029 |
| 3029 | The Eternal Vault Chamber | inside | no_mob, no_recall, no_teleport, indoors | **BOSS ROOM** — The Eternal Mummy Lord |

---

### Zone 6: The Drake's Ascent — Floor 4 (20 rooms: 3030–3049)

Fourth floor. Narrow passages. The lair of a beast that has claimed the upper pyramid.

| Vnum | Room Name | Sector | Flags | Notes |
|------|-----------|--------|-------|-------|
| 3030 | The Fourth Ascent | inside | no_mob, no_teleport, indoors | Entry from Floor 3 D4 (3017 ↑) |
| 3031 | The Drake's Outer Hall | inside | no_recall, no_teleport, indoors | Scorched stone; heat lingers |
| 3032 | A Scorch-Marked Passage | inside | no_recall, no_teleport, indoors, dark | |
| 3033 | The Ash-Filled Chamber | inside | no_recall, no_teleport, indoors | Deep drifts of ash from ancient fires |
| 3034 | The Claw-Marked Corridor | inside | no_recall, no_teleport, indoors, dark | Deep gouges in the stone walls |
| 3035 | The Bone-Strewn Hall | inside | no_recall, no_teleport, indoors | |
| 3036 | The Char-Pit Room | inside | no_recall, no_teleport, indoors, dark | Blackened floor pit at center |
| 3037 | The Trophy Hall | inside | no_recall, no_teleport, indoors | Skulls mounted on iron spikes |
| 3038 | A Narrow Scorched Passage | inside | no_recall, no_teleport, indoors, dark | |
| 3039 | The Upper Drake Hall | inside | no_recall, no_teleport, indoors | |
| 3040 | A Heat-Shimmering Corridor | inside | no_recall, no_teleport, indoors | Waves of heat emanate from cracks in the wall |
| 3041 | The Nest Antechamber | inside | no_recall, no_teleport, indoors, dark | |
| 3042 | The Feeding Ground | inside | no_recall, no_teleport, indoors | Gnawed bones carpet the floor |
| 3043 | An Ascending Shaft | inside | no_mob, no_teleport, indoors | D4 (up) → Floor 5, through `^stone sentinel gate` (locked, key 2878) |
| 3044 | A Side Passage | inside | no_recall, no_teleport, indoors, dark | |
| 3045 | The Inner Drake Hall | inside | no_recall, no_teleport, indoors | |
| 3046 | A Scorched Archive | inside | no_recall, no_teleport, indoors, dark | Burned shelves, ash-covered tablets |
| 3047 | The Pre-Lair Corridor | inside | no_recall, no_teleport, indoors | |
| 3048 | The Drake's Approach | inside | no_recall, no_teleport, indoors | `^bone-adorned gate` (closed, no lock) leads to 3049 |
| 3049 | The Pyramid Drake's Nest | inside | no_mob, no_recall, no_teleport, indoors | **BOSS ROOM** — The Pyramid Drake |

---

### Zone 7: The Summit Sanctum — Floor 5 / Apex (14 rooms: 3050–3063)

The topmost and smallest floor. The apex of the pyramid. The final pharaoh-spirit waits here.

| Vnum | Room Name | Sector | Flags | Notes |
|------|-----------|--------|-------|-------|
| 3050 | The Summit Entry | inside | no_mob, no_teleport, indoors | Entry via `^stone sentinel gate` from 3043 |
| 3051 | The Apex Corridor | inside | no_recall, no_teleport, indoors | |
| 3052 | The Wind-Scoured Chamber | inside | no_recall, no_teleport, indoors | Cracks in the ceiling let in hot desert air |
| 3053 | The Golden Hall | inside | no_recall, no_teleport, indoors | Gold leaf lines every surface |
| 3054 | The Relic Chamber | inside | no_recall, no_teleport, indoors | Empty pedestals where artifacts once rested |
| 3055 | A Side Niche | inside | no_recall, no_teleport, indoors, dark | |
| 3056 | The Hall of Judgment | inside | no_recall, no_teleport, indoors | Final carvings depict the afterlife |
| 3057 | The Pharaoh's Antechamber | inside | no_recall, no_teleport, indoors | |
| 3058 | The Inner Apex Hall | inside | no_recall, no_teleport, indoors | |
| 3059 | A Narrow Final Passage | inside | no_recall, no_teleport, indoors, dark | |
| 3060 | The Throne Room Approach | inside | no_recall, no_teleport, indoors | |
| 3061 | The Pre-Throne Hall | inside | no_recall, no_teleport, indoors | |
| 3062 | The Sand Sovereign's Approach | inside | no_recall, no_teleport, indoors | `^summit chamber door` (closed, no lock) leads to 3063 |
| 3063 | The Summit Throne Room | inside | no_mob, no_teleport, indoors | **BOSS ROOM** — The Sand Sovereign |

---

## Doors Summary

| From Room | Direction | To Room | Keyword | Lock? | Key Vnum | Reset State |
|-----------|-----------|---------|---------|-------|----------|-------------|
| 2211 (N.Oasis) | D1 (east) | 2895 | `^ancient road east` | No | -1 | `D 2211 1 1` (closed) |
| 2895 | D3 (west) | 2211 | (none) | No | -1 | open |
| 2905 | D0 (north) | 2906 | `^stone gate` | No | -1 | `D 2905 0 1` (closed) |
| 2906 | D2 (south) | 2905 | `^stone gate` | No | -1 | `D 2906 2 1` (closed) |
| 2916 | D5 (down) | 2865 | `^heavy trapdoor` | Yes (locked) | 2877 | `D 2916 5 2` (locked) |
| 2865 | D4 (up) | 2916 | `^heavy trapdoor` | Yes (locked) | 2877 | `D 2865 4 2` (locked) |
| 2936 | D0 (north) | 2937 | `^ancient stone door` | No | -1 | `D 2936 0 1` (closed) |
| 2937 | D2 (south) | 2936 | `^ancient stone door` | No | -1 | `D 2937 2 1` (closed) |
| 2973 | D0 (north) | 2974 | `^stone scarab door` | No | -1 | `D 2973 0 1` (closed) |
| 2974 | D2 (south) | 2973 | `^stone scarab door` | No | -1 | `D 2974 2 1` (closed) |
| 2999 | D4 (up) | 3005 | `^iron-banded archway` | No | -1 | `D 2999 4 1` (closed) |
| 3005 | D5 (down) | 2999 | `^iron-banded archway` | No | -1 | `D 3005 5 1` (closed) |
| 3003 | D0 (north) | 3004 | `^carved stone threshold` | No | -1 | `D 3003 0 1` (closed) |
| 3004 | D2 (south) | 3003 | `^carved stone threshold` | No | -1 | `D 3004 2 1` (closed) |
| 3028 | D0 (north) | 3029 | `^glowing stone arch` | No | -1 | `D 3028 0 1` (closed) |
| 3029 | D2 (south) | 3028 | `^glowing stone arch` | No | -1 | `D 3029 2 1` (closed) |
| 3043 | D4 (up) | 3050 | `^stone sentinel gate` | Yes (locked) | 2878 | `D 3043 4 2` (locked) |
| 3050 | D5 (down) | 3043 | `^stone sentinel gate` | Yes (locked) | 2878 | `D 3050 5 2` (locked) |
| 3048 | D0 (north) | 3049 | `^bone-adorned gate` | No | -1 | `D 3048 0 1` (closed) |
| 3049 | D2 (south) | 3048 | `^bone-adorned gate` | No | -1 | `D 3049 2 1` (closed) |
| 3062 | D0 (north) | 3063 | `^summit chamber door` | No | -1 | `D 3062 0 1` (closed) |
| 3063 | D2 (south) | 3062 | `^summit chamber door` | No | -1 | `D 3063 2 1` (closed) |
| 2894 (boss) | D2 (south) | 2893 | `^root-sealed vault door` | No | -1 | `D 2894 2 1` (closed) |
| 2893 | D0 (north) | 2894 | `^root-sealed vault door` | No | -1 | `D 2893 0 1` (closed) |

All named doors must be mentioned in at least one of: the room's main description, a spawned object, or an `E` extra description.

Both locked doors require the corresponding key object to exist in `#OBJECTS`:
- `^heavy trapdoor` (rooms 2916/2865): key vnum **2877**
- `^stone sentinel gate` (rooms 3043/3050): key vnum **2878**

---

## Mobile Definitions

### Bosses (Level 130)

All bosses: `act` flags = `is_npc(1) | sentinel(2) | no_flee(16) | aggressive(32) | stay_area(64) | boss(67108864) | noassist(1073741824)` = **1140850879**

Boss skills (`!` line): `5_attack(8) | enhanced(32768) | counter(524288) | parry(8192) | dodge(4096) | nodisarm(32) | notrip(64)` = **569448**

All bosses placed only in `no_mob` flagged rooms.

| Mob Vnum | Name | Room | Act Flags | Theme | Spec |
|----------|------|------|-----------|-------|------|
| 2864 | the Crypt Pharaoh | 2894 | 1140850879 | Undead pharaoh-spirit of ancient death magic. Cast: suffocate, mind_flail, nerve_fire. Def: heal, iceshield. Strong: shadow, mental. Weak: holy, fire. | spec_cast_undead |
| 2865 | the Desert Gate Guardian | 2939 | 1140850879 | Massive animated stone construct, guardian of the ground floor. Cast: earthquake, flamestrike. Def: heal, shockshield. Strong: earth, physical. Weak: mental. 6_attack instead of 5_attack. | spec_cast_mage |
| 2866 | the Scarab Colossus | 2974 | 1140850879 | Enormous animated scarab construct. Cast: acid_blast, lava_burst. Def: fireshield, iceshield. Strong: earth, physical. Weak: mental, water. | spec_cast_mage |
| 2867 | the High Priest of the Ancient Sands | 3004 | 1140850879 | Ancient priestly undead spirit with potent offensive magic. Cast: chain_light, mindflame, bloody_tears. Def: heal, iceshield, fireshield. Strong: shadow, mental. Weak: holy. | spec_cast_bigtime |
| 2868 | the Eternal Mummy Lord | 3029 | 1140850879 | Mummified warlord animated by a millennium of curses. Cast: suffocate, mind_bolt, ego_whip. Def: heal, cure_critic. Strong: shadow. Weak: fire, holy. | spec_cast_undead |
| 2869 | the Pyramid Drake | 3049 | 1140850879 | Ancient desert dragon-kin that has claimed the upper floors. Cast: lava_burst, heat_armor, burn_hands. Def: fireshield. Strong: fire, physical. Weak: water, air. Spec: spec_breath_fire. | spec_breath_fire |
| 2870 | the Sand Sovereign | 3063 | 1140850879 | The immortal architect of the pyramid. Full spell set. Cast: nerve_fire, light_bolt, mindflame, suffocate, lava_burst. Def: heal, fireshield, iceshield, shockshield. Strong: earth, fire, mental. Weak: holy. | spec_cast_bigtime |

**Boss `+` extension:** All bosses receive moderate spellpower, crit, crit_mult, parry, dodge, and block modifiers appropriate for level 130 encounters. The Sand Sovereign (2870) receives maximum values.

---

### Strong (Solo) Mobs (Levels 118–122)

All strong mobs: `act` = `is_npc(1) | aggressive(32) | stay_area(64) | solo(33554432)` = **33554529**

| Mob Vnum | Name | Level | Zones | Notes |
|----------|------|-------|-------|-------|
| 2871 | a cursed pharaoh's shade | 120 | All | Undead spirit. Cast: mind_flail, suffocate. Def: heal. |
| 2872 | a desert stone titan | 122 | Ground, Floor 1 | Massive animated construct. Enhanced, 4_attack, nodisarm, notrip. |
| 2873 | a pyramid death knight | 121 | Floors 2–4 | Armored undead warrior. 4_attack, enhanced, parry, counter. |
| 2874 | a sand drake | 120 | Floors 3–5 | Lesser drake. 4_attack, enhanced, parry, dodge. Spec: spec_breath_fire. |
| 2875 | a golden sphinx | 122 | Floors 4–5 | Construct guardian. 3_attack, enhanced, parry, dodge. Cast: earthquake, light_bolt. |
| 2924 | a sandstorm elemental | 121 | Ground, Floor 1 | Air/sand elemental construct. no_mind. 3_attack, enhanced, dodge. Cast: col_spray, stun. Spec: spec_cast_mage. |
| 2925 | an ancient obsidian guardian | 122 | Floor 2, Floor 3 | Massive black-stone construct. no_mind, sentinel. 4_attack, enhanced, parry, nodisarm, notrip. |
| 2926 | a greater mummy overlord | 120 | Floor 3, Floor 4 | Towering ancient warlord-spirit. undead. 3_attack, enhanced, parry, counter. Cast: suffocate, ego_whip. Spec: spec_cast_undead. |

---

### Trash Mobs (Levels 105–120)

All trash: `act` = `is_npc(1) | aggressive(32) | stay_area(64)` = **97**

| Mob Vnum | Name | Level | Zones | Combat Notes |
|----------|------|-------|-------|--------------|
| 2876 | a sand scarab swarm | 105 | Undercroft, Ground | no_body, no_mind. 2_attack. |
| 2877 (mob) | a skeleton warrior | 107 | Undercroft | undead. 2_attack, punch. |
| 2878 (mob) | a cursed mummy | 108 | Undercroft | undead. 2_attack, enhanced. |
| 2879 | a giant desert scorpion | 110 | Undercroft, Ground | Spec: spec_poison. 2_attack. |
| 2880 | a tomb robber's ghost | 106 | Ground | undead, no_body. Cast: flare, shock_grasp. |
| 2881 | a sand jackal | 112 | Ground | 2_attack, dodge, trip. |
| 2882 | a granite guardian statue | 115 | Ground, Floor 1 | no_mind. 3_attack, enhanced, nodisarm. |
| 2883 | a canopic horror | 109 | Undercroft, Ground | undead, no_mind. 2_attack, punch. |
| 2884 | a golden asp | 113 | Floor 1, Floor 2 | Spec: spec_poison. 2_attack. |
| 2885 | a desert hawk | 116 | Floor 1, Floor 2 | 2_attack, dodge. |
| 2886 | a sand golem | 110 | Floor 1 | no_mind. 3_attack, enhanced. |
| 2887 (mob) | a cursed acolyte | 118 | Floor 2 | Cast: faerie_fire, flare, col_spray. Def: cure_serious. |
| 2888 | a desert wight | 114 | Floor 2 | undead. 2_attack, enhanced, drain. |
| 2889 | a stone scarab guardian | 117 | Floor 2, Floor 3 | no_mind. 3_attack, enhanced, parry. |
| 2890 | a pyramid cobra | 111 | Floor 2 | Spec: spec_poison. 2_attack. |
| 2891 | a mummified archer | 119 | Floor 3 | undead. 2_attack, enhanced, dodge. Cast: chill_touch. Spec: spec_cast_undead. |
| 2892 (mob) | a great desert vulture | 120 | Floor 3 | 2_attack, dodge, kick. |
| 2893 (mob) | an ancient face guardian | 116 | Floor 3 | no_mind, sentinel. 2_attack, parry, nodisarm. |
| 2894 (mob) | a desert shadow | 112 | Floor 3, Floor 4 | undead, no_body. Cast: mind_bolt, ego_whip. |
| 2895 (mob) | a desert shadow assassin | 118 | Floor 3, Floor 4 | 2_attack, enhanced, dodge, trip. |
| 2896 | a living pillar golem | 120 | Floor 4 | no_mind, sentinel. 4_attack, enhanced, nodisarm, notrip. |
| 2897 | a sacred golden cobra | 115 | Floor 4 | Spec: spec_poison. 2_attack. |
| 2898 | a pyramid cultist | 117 | Floor 4 | Cast: burn_hands, mag_missile. Def: cure_serious. |
| 2899 | a death hawk | 119 | Floor 4, Floor 5 | 2_attack, dodge, kick. |
| 2900 | a sand revenant | 113 | Floor 4 | undead. 2_attack, enhanced, parry. Def: cure_critic. |
| 2901 | a pyramid specter | 116 | Floor 5 | undead, no_body. Cast: mindflame, mind_bolt. Def: cure_critic. |
| 2902 | a tomb fly swarm | 105 | Undercroft | Clouds of carrion flies. no_body, no_mind. 2_attack. Spec: spec_poison. |
| 2903 | a desiccated desert rat | 105 | Undercroft, Ground | Dessicated undead vermin. no_mind. 2_attack, trip. Spec: spec_poison. |
| 2904 | a bone crawler | 107 | Undercroft | Animated skeletal mass. undead, no_mind. 3_attack, enhanced. |
| 2905 | a sand-choked ghoul | 109 | Undercroft, Ground | Ghoul packed with sand. undead. 2_attack, enhanced, drain. |
| 2906 | an animated obelisk shard | 112 | Ground | Fragment of a shattered obelisk come alive. no_mind, sentinel. 3_attack, enhanced, nodisarm. |
| 2907 | a cursed pyramid builder | 110 | Ground, Floor 1 | Ghost of a slave forced to build the pyramid. undead. 2_attack, enhanced. Cast: weaken, blindness. |
| 2908 | a desert sand adder | 108 | Ground | A fat, heat-seeking snake. 2_attack, dodge. Spec: spec_poison. |
| 2909 | an ancient sentinel construct | 114 | Ground, Floor 1 | Hulking guardian golem, still functional. no_mind. 3_attack, enhanced, parry, nodisarm. |
| 2910 | a hieroglyph guardian | 115 | Floor 1 | Living inscription carved into stone, peeled free. no_mind, sentinel. 3_attack, enhanced. |
| 2911 | an animated canopic jar | 112 | Floor 1, Floor 2 | A sealed burial jar walking on spindly carved legs. undead, no_body. 2_attack, punch. Cast: poison, col_spray. |
| 2912 | a scribal revenant | 116 | Floor 1 | The ghost of a scribe whose knowledge was sealed in the pyramid. undead. 2_attack. Cast: flare, mag_missile. Def: cure_light. |
| 2913 | a sand-wrapped mummy servant | 113 | Floor 1, Floor 2 | A lesser mummy bound in sand-crusted linen. undead. 2_attack, enhanced. |
| 2914 | a high priest's thrall | 118 | Floor 2 | Undead acolyte enslaved by the High Priest's will. undead. 2_attack, enhanced, parry. Cast: faerie_fire. |
| 2915 | a sand wraith | 117 | Floor 2, Floor 3 | A translucent desert spirit wreathed in swirling sand. undead, no_body. Cast: ego_whip, cold_grasp. Def: cure_serious. |
| 2916 | a curse-bound warrior | 119 | Floor 2, Floor 3 | An ancient soldier held together by curse alone. undead. 3_attack, enhanced, counter. |
| 2917 | an entombed warlord | 118 | Floor 3 | A general buried standing upright, still battle-ready. undead. 3_attack, enhanced, parry, dodge. |
| 2918 | a flesh golem | 120 | Floor 3, Floor 4 | A construct of stitched mummy flesh. no_mind. 4_attack, enhanced, nodisarm, notrip. |
| 2919 | a tomb stalker | 116 | Floor 3 | A beast that hunts intruders in the dark. 2_attack, dodge, trip. |
| 2920 | an ember lizard | 117 | Floor 4 | A long-tailed reptile that absorbs heat from the scorched stone. 2_attack, dodge. Resist: fire. |
| 2921 | a charred husk | 119 | Floor 4 | An undead burned to blackened bone and sinew. undead. 3_attack, enhanced. Strong: fire. Weak: water. |
| 2922 | an apex sentinel | 120 | Floor 5 | The last stone guardian before the throne. no_mind, sentinel. 3_attack, enhanced, parry, nodisarm. |
| 2923 | a gilded warrior shade | 118 | Floor 5 | The ghost of a royal bodyguard, still adorned in ceremonial gold. undead. 2_attack, enhanced. Cast: mind_bolt. |

**Note on vnum reuse:** Mob vnums 2876–2926 overlap numerically with object vnums in the same range. Cross-type vnum overlap is permitted by the spec; each is unique within its own type index.

---

## Object Definitions

### Piece Item Chain (13 objects: 2864–2876)

All seven boss fragment pieces are delivered via boss loot tables and carry `ITEM_LOOT`.

**Fragment extra_flags:** `ITEM_NODROP(128) | ITEM_BOSS(134217728) | ITEM_MAGIC(64) | ITEM_LOOT(67108864)` = **201326784**

**Assembly extra_flags (created by combining, not looted):** `ITEM_NODROP(128) | ITEM_BOSS(134217728) | ITEM_MAGIC(64)` = **134217920**

**Final Crown extra_flags:** `ITEM_RARE(1048576) | ITEM_NODROP(128) | ITEM_BOSS(134217728) | ITEM_MAGIC(64) | ITEM_LOOT(67108864)` = **202375360**

All piece and assembly items: `item_type = 29` (ITEM_PIECE), `wear_flags = take(8388608)`.
Final Crown: `item_type = 9` (ITEM_ARMOR), `wear_flags = head(8) | take(8388608) = 8388616`.

| Obj Vnum | Name | Type | value0 | value1 | value2 | Source |
|----------|------|------|--------|--------|--------|--------|
| 2864 | a fragment of the pharaoh's crown (I) | piece | 0 | 2865 | 2871 | Crypt Pharaoh loot |
| 2865 | a fragment of the pharaoh's crown (II) | piece | 2864 | 0 | 2871 | Desert Gate Guardian loot |
| 2866 | a fragment of the pharaoh's crown (III) | piece | 2871 | 0 | 2872 | Scarab Colossus loot |
| 2867 | a fragment of the pharaoh's crown (IV) | piece | 2872 | 0 | 2873 | High Priest loot |
| 2868 | a fragment of the pharaoh's crown (V) | piece | 2873 | 0 | 2874 | Eternal Mummy Lord loot |
| 2869 | a fragment of the pharaoh's crown (VI) | piece | 2874 | 0 | 2875 | Pyramid Drake loot |
| 2870 | a fragment of the pharaoh's crown (VII) | piece | 2875 | 0 | 2876 | Sand Sovereign loot |
| 2871 | a partial crown of the pharaoh (I-II) | piece | 0 | 2866 | 2872 | Created from I+II |
| 2872 | a partial crown of the pharaoh (I-III) | piece | 0 | 2867 | 2873 | Created from (I-II)+III |
| 2873 | a partial crown of the pharaoh (I-IV) | piece | 0 | 2868 | 2874 | Created from (I-III)+IV |
| 2874 | a partial crown of the pharaoh (I-V) | piece | 0 | 2869 | 2875 | Created from (I-IV)+V |
| 2875 | a partial crown of the pharaoh (I-VI) | piece | 0 | 2870 | 2876 | Created from (I-V)+VI |
| 2876 | the Burial Crown of the Sand Sovereign | armor (head) | — | — | — | **Final combined item** |

**Combining sequence:**
1. Fragment I (2864) + Fragment II (2865) → Assembly I-II (2871)
2. Assembly I-II (2871) + Fragment III (2866) → Assembly I-III (2872)
3. Assembly I-III (2872) + Fragment IV (2867) → Assembly I-IV (2873)
4. Assembly I-IV (2873) + Fragment V (2868) → Assembly I-V (2874)
5. Assembly I-V (2874) + Fragment VI (2869) → Assembly I-VI (2875)
6. Assembly I-VI (2875) + Fragment VII (2870) → **the Burial Crown of the Sand Sovereign (2876)**

The `connect` command checks: `pieceA.value0 == pieceB.vnum` OR `pieceA.value1 == pieceB.vnum`. In each step above, one piece's `value1` equals the other piece's vnum, satisfying the check. On success, both are consumed and `pieceA.value2` is created.

---

### Key Objects (2 objects: 2877–2878)

Both keys are nodrop. Key 2877 spawns via `O` reset in room 2887 (Forgotten Workshop in Undercroft) so players who fall into the trap can eventually escape. Key 2878 drops from the Pyramid Drake (2869) via loot table.

| Obj Vnum | Name | item_type | extra_flags | wear_flags | Level | Notes |
|----------|------|-----------|-------------|------------|-------|-------|
| 2877 | a corroded bronze trapdoor key | key (18) | ITEM_NODROP (128) | take (8388608) | L 100 | Opens `^heavy trapdoor` (rooms 2916/2865) |
| 2878 | a carved stone sentinel key | key (18) | ITEM_NODROP(128) \| ITEM_LOOT(67108864) = 67108992 | take (8388608) | L 120 | Opens `^stone sentinel gate` (rooms 3043/3050). Boss loot from Pyramid Drake. |

---

### Boss Equipment (Level 120, extra_flags = ITEM_NODROP(128) | ITEM_BOSS(134217728) | ITEM_MAGIC(64) = 134217920)

Items delivered via boss loot tables also add ITEM_LOOT(67108864) for total: **201326784**.

Each boss drops its piece fragment plus 3 boss gear items via loot table. The Sand Sovereign drops 4 gear items.

Weight key: 1-5 = caster, 6-10 = melee, 11-15 = tank.

**Crypt Pharaoh (mob 2864) — gear vnums 2879–2881:**

| Obj Vnum | Name | item_type | wear_flags | weight | Archetype |
|----------|------|-----------|------------|--------|-----------|
| 2879 | the Crypt Pharaoh's death mask | armor (9) | face(16)\|take | 1 | caster |
| 2880 | a cloak of undying shadow | armor (9) | about(65536)\|take | 1 | caster |
| 2881 | the Crypt Pharaoh's burial wrappings | armor (9) | body(262144)\|take | 11 | tank |

**Desert Gate Guardian (mob 2865) — gear vnums 2882–2884:**

| Obj Vnum | Name | item_type | wear_flags | weight | Archetype |
|----------|------|-----------|------------|--------|-----------|
| 2882 | a gauntlet of the stone guardian | armor (9) | hands(4096)\|take | 11 | tank |
| 2883 | the guardian's sandstone maul | weapon (5) | hold(32768)\|take | 6 | melee; value3=7 (pound) |
| 2884 | a sash of the desert gate | armor (9) | waist(131072)\|take | 6 | melee |

**Scarab Colossus (mob 2866) — gear vnums 2885–2887:**

| Obj Vnum | Name | item_type | wear_flags | weight | Archetype |
|----------|------|-----------|------------|--------|-----------|
| 2885 | a chitinous scarab breastplate | armor (9) | body(262144)\|take | 11 | tank |
| 2886 | a scarab-shell buckler | armor (9) | hold(32768)\|take | 11 | tank; ITEM_BUCKLER extra flag added (extra_flags = 201326784 + 268435456 = 469762240 for loot version) |
| 2887 | a claw of the Scarab Colossus | weapon (5) | hold(32768)\|take | 6 | melee; ITEM_FIST flag; value3=0 (hit) |

Note for 2887: extra_flags for this loot weapon = ITEM_NODROP(128) | ITEM_BOSS(134217728) | ITEM_MAGIC(64) | ITEM_LOOT(67108864) | ITEM_FIST(1073741824) = 1275068608

**High Priest of the Ancient Sands (mob 2867) — gear vnums 2888–2890:**

| Obj Vnum | Name | item_type | wear_flags | weight | Archetype |
|----------|------|-----------|------------|--------|-----------|
| 2888 | the High Priest's golden circlet | armor (9) | head(8)\|take | 1 | caster |
| 2889 | a robe of the ancient rites | armor (9) | body(262144)\|take | 1 | caster |
| 2890 | the High Priest's sacrificial blade | weapon (5) | hold(32768)\|take | 6 | melee; value3=2 (stab) |

**Eternal Mummy Lord (mob 2868) — gear vnums 2891–2893:**

| Obj Vnum | Name | item_type | wear_flags | weight | Archetype |
|----------|------|-----------|------------|--------|-----------|
| 2891 | greaves of the eternal mummy | armor (9) | legs(1048576)\|take | 11 | tank |
| 2892 | the Mummy Lord's cursed flail | weapon (5) | hold(32768)\|take | 6 | melee; value3=7 (pound) |
| 2893 | wrappings of ancient preservation | armor (9) | arms(315)\|take | 11 | tank |

**Pyramid Drake (mob 2869) — gear vnums 2894–2896:**

| Obj Vnum | Name | item_type | wear_flags | weight | Archetype |
|----------|------|-----------|------------|--------|-----------|
| 2894 | a scale-mail vest of the drake | armor (9) | body(262144)\|take | 11 | tank |
| 2895 | the drake's ember-fang | weapon (5) | hold(32768)\|take | 6 | melee; ITEM_FIST; value3=5 (claw) |
| 2896 | a crest of desert fire | armor (9) | horns(4)\|take | 6 | melee |

Note for 2895: extra_flags = 201326784 | ITEM_FIST(1073741824) = 1275068608

**Sand Sovereign (mob 2870) — gear vnums 2897–2900 (4 items, final boss):**

| Obj Vnum | Name | item_type | wear_flags | weight | Archetype |
|----------|------|-----------|------------|--------|-----------|
| 2897 | the Sand Sovereign's great scepter | weapon (5) | hold(32768)\|take | 1 | caster; ITEM_TWO_HANDED; value3=7 (pound) |
| 2898 | robes of the eternal sovereign | armor (9) | body(262144)\|take | 1 | caster |
| 2899 | the sovereign's amulet of dominion | armor (9) | neck(128)\|take | 1 | caster |
| 2900 | sandals of the desert throne | armor (9) | feet(2097152)\|take | 6 | melee |

Note for 2897: extra_flags = 201326784 | ITEM_TWO_HANDED(2147483648) = 2348810432

---

### Trash Equipment (Levels 105–110, ITEM_NODROP only unless loot-table)

Reset gear extra_flags: `ITEM_NODROP(128)` = **128**
Loot-table gear extra_flags: `ITEM_NODROP(128) | ITEM_LOOT(67108864)` = **67108992**

All items include `take(8388608)` in wear_flags.

| Obj Vnum | Name | item_type | wear_flags | weight | Level | Delivery | Source |
|----------|------|-----------|------------|--------|-------|----------|--------|
| 2901 | a skeleton warrior's blade | weapon (5) | hold\|take | 6 | L 107 | Reset (E) | skeleton warrior |
| 2902 | a mummy's wrapping sash | armor (9) | waist\|take | 11 | L 108 | Loot | cursed mummy |
| 2903 | a scorpion-claw gauntlet | armor (9) | hands\|take | 11 | L 110 | Loot | giant desert scorpion |
| 2904 | a ghostly tomb shroud | armor (9) | about\|take | 1 | L 106 | Loot | tomb robber's ghost |
| 2905 | a jackal-hide belt | armor (9) | waist\|take | 6 | L 112 | Loot | sand jackal |
| 2906 | granite guardian fist | weapon (5) | hold\|take | 11 | L 115 | Reset (E) | granite guardian statue; ITEM_FIST; value3=0 |
| 2907 | canopic horror mandible | weapon (5) | hold\|take | 6 | L 109 | Loot | canopic horror; value3=10 (bite) |
| 2908 | an asp-fang earring | armor (9) | ear\|take | 1 | L 113 | Loot | golden asp |
| 2909 | desert hawk wing-cloak | armor (9) | about\|take | 1 | L 116 | Loot | desert hawk |
| 2910 | a sand golem's core shard | armor (9) | hold\|take | 11 | L 110 | Loot | sand golem; shield |
| 2911 | acolyte's cursed headwrap | armor (9) | head\|take | 1 | L 118 | Reset (E) | cursed acolyte |
| 2912 | desert wight's bone ring | armor (9) | finger\|take | 1 | L 114 | Loot | desert wight |
| 2913 | scarab guardian shell plate | armor (9) | body\|take | 11 | L 117 | Reset (E) | stone scarab guardian |
| 2914 | a cobra-scale bracer | armor (9) | wrist\|take | 11 | L 111 | Loot | pyramid cobra |
| 2915 | a mummified archer's bow wrap | armor (9) | arms\|take | 6 | L 119 | Loot | mummified archer |
| 2916 | vulture-feather mantle | armor (9) | shoulders\|take | 6 | L 120 | Loot | great desert vulture |
| 2917 | face guardian stone buckler | armor (9) | hold\|take | 11 | L 116 | Reset (E) | ancient face guardian; ITEM_BUCKLER |
| 2918 | desert shadow cowl | armor (9) | head\|take | 1 | L 112 | Loot | desert shadow |
| 2919 | an assassin's sand-silk hood | armor (9) | head\|take | 1 | L 118 | Loot | desert shadow assassin |
| 2920 | a pillar golem's knuckle-spike | weapon (5) | hold\|take | 11 | L 120 | Reset (E) | living pillar golem; ITEM_FIST; value3=0 |
| 2921 | sacred cobra fang pendant | armor (9) | neck\|take | 1 | L 115 | Loot | sacred golden cobra |
| 2922 | cultist's obsidian dagger | weapon (5) | hold\|take | 6 | L 117 | Loot | pyramid cultist; value3=2 (stab) |
| 2923 | death hawk talons | weapon (5) | hold\|take | 6 | L 119 | Loot | death hawk; value3=5 (claw) |
| 2924 | sand revenant's burial ring | armor (9) | finger\|take | 1 | L 113 | Loot | sand revenant |
| 2925 | spectre-touched robes | armor (9) | body\|take | 1 | L 116 | Reset (E) | pyramid specter |
| 2926 | cursed pharaoh shade's halo | armor (9) | halo\|take | 1 | L 120 | Reset (E) | cursed pharaoh's shade |
| 2927 | desert titan's stone bracer | armor (9) | wrist\|take | 11 | L 122 | Reset (E) | desert stone titan |
| 2928 | a death knight's greatsword | weapon (5) | hold\|take | 6 | L 121 | Reset (E) | pyramid death knight; ITEM_TWO_HANDED; value3=3 (slash) |
| 2929 | sand drake scale boots | armor (9) | feet\|take | 11 | L 120 | Reset (E) | sand drake |
| 2930 | a sphinx-carved shoulder plate | armor (9) | shoulders\|take | 11 | L 122 | Reset (E) | golden sphinx |
| 2931 | scarab swarm chitin ring | armor (9) | finger\|take | 1 | L 105 | Loot | sand scarab swarm |
| 2932 | a tomb fly carapace brooch | armor (9) | finger\|take | 1 | L 105 | Loot | tomb fly swarm |
| 2933 | a desiccated rat-tail lash | weapon (5) | hold\|take | 6 | L 105 | Loot | desiccated desert rat; value3=1 (whip) |
| 2934 | bone crawler chitin pauldrons | armor (9) | shoulders\|take | 11 | L 107 | Loot | bone crawler |
| 2935 | a ghoul's tattered girdle | armor (9) | waist\|take | 11 | L 109 | Loot | sand-choked ghoul |
| 2936 | an animated obelisk shard buckler | armor (9) | hold\|take | 11 | L 112 | Loot | animated obelisk shard; ITEM_BUCKLER |
| 2937 | a cursed builder's stone trowel | weapon (5) | hold\|take | 6 | L 110 | Loot | cursed pyramid builder; value3=7 (pound) |
| 2938 | a desert sand adder fang | armor (9) | ear\|take | 1 | L 108 | Loot | desert sand adder |
| 2939 | an ancient sentinel's faceplate | armor (9) | face\|take | 11 | L 114 | Reset (E) | ancient sentinel construct |
| 2940 | a hieroglyph guardian stone slab | armor (9) | hold\|take | 11 | L 115 | Loot | hieroglyph guardian; ITEM_BUCKLER |
| 2941 | an animated canopic jar lid | armor (9) | head\|take | 1 | L 112 | Loot | animated canopic jar |
| 2942 | a scribe's cursed inkpot | armor (9) | hold\|take | 1 | L 116 | Loot | scribal revenant |
| 2943 | linen wraps of the mummy servant | armor (9) | wrist\|take | 11 | L 113 | Loot | sand-wrapped mummy servant |
| 2944 | a high priest thrall's robe | armor (9) | body\|take | 1 | L 118 | Reset (E) | high priest's thrall |
| 2945 | a sand wraith's wisp veil | armor (9) | about\|take | 1 | L 117 | Loot | sand wraith |
| 2946 | a curse-bound warrior's iron pauldron | armor (9) | shoulders\|take | 11 | L 119 | Loot | curse-bound warrior |
| 2947 | an entombed warlord's war crown | armor (9) | head\|take | 11 | L 118 | Reset (E) | entombed warlord |
| 2948 | a flesh golem's riveted knuckle | weapon (5) | hold\|take | 11 | L 120 | Loot | flesh golem; ITEM_FIST; value3=0; extra_flags = NODROP+LOOT+FIST = 1140850816 |
| 2949 | a tomb stalker's shadow wrapping | armor (9) | about\|take | 1 | L 116 | Loot | tomb stalker |
| 2950 | ember lizard scale vest | armor (9) | body\|take | 11 | L 117 | Loot | ember lizard |
| 2951 | charred husk smoldering bracers | armor (9) | wrist\|take | 11 | L 119 | Loot | charred husk |
| 2952 | an apex sentinel's stone gauntlets | armor (9) | hands\|take | 11 | L 120 | Reset (E) | apex sentinel |
| 2953 | a gilded warrior shade's helm | armor (9) | head\|take | 1 | L 118 | Loot | gilded warrior shade |
| 2954 | a sandstorm elemental's howling band | armor (9) | wrist\|take | 1 | L 121 | Reset (E) | sandstorm elemental |
| 2955 | an obsidian guardian's plate arm | armor (9) | arms\|take | 11 | L 122 | Reset (E) | ancient obsidian guardian |
| 2956 | a mummy overlord's burial circlet | armor (9) | head\|take | 11 | L 120 | Reset (E) | greater mummy overlord |

**Note:** Object vnums 2901–2956 overlap with mob vnums in the same range. Cross-type overlap is permitted; each is unique within its type index.

---

## Resets Plan

### Boss Mob Resets

```
M 0 2864 1 2894 Crypt Pharaoh in Pharaoh's Tomb Chamber
M 0 2865 1 2939 Desert Gate Guardian in Pyramid's Heart Chamber
M 0 2866 1 2974 Scarab Colossus in Scarab Colossus Chamber
M 0 2867 1 3004 High Priest in High Priest's Sanctum
M 0 2868 1 3029 Eternal Mummy Lord in Eternal Vault Chamber
M 0 2869 1 3049 Pyramid Drake in Pyramid Drake's Nest
M 0 2870 1 3063 Sand Sovereign in Summit Throne Room
```

### Boss Loot Tables (l/L extensions on boss mobs)

Each boss carries its piece fragment plus 3 gear items (Sand Sovereign: piece + 4 gear).
`loot_amount`: Set based on guaranteed drops. Each boss uses a dedicated loot slot per item.

Example for Crypt Pharaoh (mob 2864):
```
l 400 2864 2879 2880 2881 0 0 0 0 0 0
L 25 25 25 25 0 0 0 0 0
```
This guarantees 4 drops: the piece fragment (2864 object), death mask (2879), shadow cloak (2880), burial wrappings (2881), each with equal 25% selection weight. The Sand Sovereign is structured similarly with 5 slots.

### Strong Mob Resets (limit 2–3 each)

```
M 0 2871 3 2931 cursed pharaoh's shade (multiple rooms)
M 0 2871 3 2912 cursed pharaoh's shade (another spawn)
M 0 2872 2 2909 desert stone titan
M 0 2872 2 2943 desert stone titan
M 0 2873 2 2983 pyramid death knight
M 0 2873 2 3001 pyramid death knight
M 0 2874 2 3021 sand drake
M 0 2874 2 3032 sand drake
M 0 2875 2 3052 golden sphinx
M 0 2875 2 3058 golden sphinx
M 0 2924 2 2909 sandstorm elemental
M 0 2924 2 2942 sandstorm elemental
M 0 2925 2 2988 ancient obsidian guardian
M 0 2925 2 3009 ancient obsidian guardian
M 0 2926 2 3021 greater mummy overlord
M 0 2926 2 3035 greater mummy overlord
```

### Trash Mob Resets (representative sample, ~60–80 total resets)

Distributed across zones by level:
- **Undercroft (2865–2893):** Skeleton warriors, cursed mummies, sand scarab swarms, giant scorpions, tomb robber's ghosts, canopic horrors, tomb fly swarms, desiccated desert rats, bone crawlers, sand-choked ghouls (~18 resets)
- **Ground Level (2895–2938):** Sand jackals, tomb robber's ghosts, granite guardian statues, canopic horrors, animated obelisk shards, cursed pyramid builders, desert sand adders, ancient sentinel constructs, sand-choked ghouls (~28 resets)
- **Floor 1 (2940–2973):** Golden asps, desert hawks, sand golems, granite statues, cursed pyramid builders, ancient sentinel constructs, hieroglyph guardians, animated canopic jars, scribal revenants, sand-wrapped mummy servants (~20 resets)
- **Floor 2 (2975–3003):** Cursed acolytes, desert wights, stone scarab guardians, pyramid cobras, animated canopic jars, sand-wrapped mummy servants, high priest thralls, sand wraiths, curse-bound warriors (~16 resets)
- **Floor 3 (3005–3028):** Mummified archers, great desert vultures, ancient face guardians, desert shadows, shadow assassins, sand wraiths, curse-bound warriors, entombed warlords, flesh golems, tomb stalkers (~14 resets)
- **Floor 4 (3030–3048):** Living pillar golems, sacred golden cobras, pyramid cultists, death hawks, sand revenants, flesh golems, ember lizards, charred husks (~12 resets)
- **Floor 5 (3050–3062):** Pyramid specters, apex sentinels, gilded warrior shades, death knights (strong), cursed pharaoh shades (strong) (~8 resets)

### Equipment Resets (E/G after mob M)

Each trash mob is followed by one `E` reset for their associated reset gear (items delivered via reset do not need ITEM_LOOT). Boss mobs have their gear delivered through loot tables only (no E resets on bosses — loot tables handle all boss drops).

### Object Resets (O commands, items in rooms)

```
O 0 2877 1 2887 Trapdoor key in Forgotten Workshop (Undercroft)
```

### Door Resets (D commands)

Per the Doors Summary table above:
```
D 0 2211 1 1  charter road east (Great Oasis side, closed)
D 0 2905 0 1  stone gate (closed)
D 0 2906 2 1  stone gate reverse (closed)
D 0 2916 5 2  heavy trapdoor (locked)
D 0 2865 4 2  heavy trapdoor reverse (locked)
D 0 2936 0 1  ancient stone door (closed)
D 0 2937 2 1  ancient stone door reverse (closed)
D 0 2973 0 1  stone scarab door (closed)
D 0 2974 2 1  stone scarab door reverse (closed)
D 0 2999 4 1  iron-banded archway (closed)
D 0 3005 5 1  iron-banded archway reverse (closed)
D 0 3003 0 1  carved stone threshold (closed)
D 0 3004 2 1  carved stone threshold reverse (closed)
D 0 3028 0 1  glowing stone arch (closed)
D 0 3029 2 1  glowing stone arch reverse (closed)
D 0 3043 4 2  stone sentinel gate (locked)
D 0 3050 5 2  stone sentinel gate reverse (locked)
D 0 3048 0 1  bone-adorned gate (closed)
D 0 3049 2 1  bone-adorned gate reverse (closed)
D 0 3062 0 1  summit chamber door (closed)
D 0 3063 2 1  summit chamber door reverse (closed)
D 0 2893 0 1  root-sealed vault door (closed)
D 0 2894 2 1  root-sealed vault door reverse (closed)
```

---

## Specials

```
#SPECIALS
M 2864 spec_cast_undead  Crypt Pharaoh
M 2865 spec_cast_mage   Desert Gate Guardian
M 2866 spec_cast_mage   Scarab Colossus
M 2867 spec_cast_bigtime  High Priest
M 2868 spec_cast_undead  Eternal Mummy Lord
M 2869 spec_breath_fire  Pyramid Drake
M 2870 spec_cast_bigtime  Sand Sovereign
M 2879 spec_poison     giant desert scorpion
M 2880 spec_cast_undead  tomb robber's ghost (cast flare/shock_grasp)
M 2884 spec_poison     golden asp
M 2887 spec_cast_mage   cursed acolyte
M 2888 spec_cast_undead  desert wight
M 2890 spec_poison     pyramid cobra
M 2891 spec_cast_undead  mummified archer (undead caster)
M 2894 spec_cast_undead  desert shadow
M 2897 spec_poison     sacred golden cobra
M 2898 spec_cast_mage   pyramid cultist
M 2901 spec_cast_undead  pyramid specter
M 2902 spec_poison     tomb fly swarm
M 2903 spec_poison     desiccated desert rat
M 2908 spec_poison     desert sand adder
M 2911 spec_cast_undead  animated canopic jar
M 2912 spec_cast_mage   scribal revenant
M 2914 spec_cast_mage   high priest's thrall
M 2915 spec_cast_undead  sand wraith
M 2924 spec_cast_mage   sandstorm elemental
M 2926 spec_cast_undead  greater mummy overlord
S
```

**Spec compliance note:** `spec_disease` does not appear in the allowed `spec_fun_name` list in `area_file_spec.md` (section 10). Tomb fly swarm (2902) and desiccated desert rat (2903) were originally assigned `spec_disease` but have been corrected to `spec_poison`, which is the closest valid behavior for disease-vector mobs. The poison spec delivers a similar debilitation effect through the game's existing poison mechanics.

---

## Item Flag Reference

| Category | Item flag composition | Numeric value |
|----------|-----------------------|---------------|
| Boss piece fragments (loot) | NODROP+BOSS+MAGIC+LOOT | 201326784 |
| Assembly pieces (created) | NODROP+BOSS+MAGIC | 134217920 |
| Final Crown | RARE+NODROP+BOSS+MAGIC+LOOT | 202375360 |
| Boss gear via loot table | NODROP+BOSS+MAGIC+LOOT | 201326784 |
| Boss gear via reset (E) | NODROP+BOSS+MAGIC | 134217920 |
| Trash gear via loot table | NODROP+LOOT | 67108992 |
| Trash gear via reset (E) | NODROP | 128 |
| Keys (room spawn or boss loot) | See per-key notes above | 128 or 67108992 |
| Scarab buckler boss loot | NODROP+BOSS+MAGIC+LOOT+BUCKLER | 469762240 |
| Fist weapon boss loot | NODROP+BOSS+MAGIC+LOOT+FIST | 1275068608 |
| Two-handed weapon boss loot | NODROP+BOSS+MAGIC+LOOT+TWO_HANDED | 2348810432 |
| Stone face guardian buckler (trash reset) | NODROP+BUCKLER | 268435584 |
| Pillar golem fist (trash reset) | NODROP+FIST | 1073741952 |
| Death knight greatsword (trash reset) | NODROP+TWO_HANDED | 2147483776 |
| Animated obelisk buckler (trash loot) | NODROP+LOOT+BUCKLER | 335544320 |
| Hieroglyph guardian buckler (trash loot) | NODROP+LOOT+BUCKLER | 335544320 |
| Flesh golem knuckle (trash loot) | NODROP+LOOT+FIST | 1140850816 |

---

## Room Connectivity Notes

- **Trap Room (2864):** One-way entrance only from rooms 2917, 2959, 2989, 3016. Single D5 (down) exit to 2873. No exit back to any source room.
- **Undercroft to Ground:** D4 (up) from 2865 to 2916 (locked trapdoor, bidirectional, both sides locked via resets).
- **Ground to Floor 1:** Two parallel D4 (up) exits: 2923→2940 and 2924→2941. No door on either.
- **Floor 1 to Floor 2:** Two parallel D4 (up) exits: 2954→2975 and 2955→2976. No door.
- **Floor 2 to Floor 3:** Single D4 (up) 2999→3005, through `^iron-banded archway` (closed door).
- **Floor 3 to Floor 4:** Single D4 (up) 3017→3030. No door.
- **Floor 4 to Floor 5:** Single D4 (up) 3043→3050, through `^stone sentinel gate` (locked door, key from Pyramid Drake loot).
- No directional loops exist outside ROOM_MAZE-flagged rooms. All paths branch and terminate; none cycle back in the same direction.

---

## Implementation Checklist

1. [ ] Create branch `claude/northern-pyramid-area-design-TnN2N`
2. [ ] Create `area/northern_pyramid.are` with `#AREA` header and `#$`
3. [ ] Write `#MOBILES` section — 7 bosses (2864–2870) + 8 strong mobs (2871–2875, 2924–2926) + 48 trash (2876–2923)
4. [ ] Write `#OBJECTS` section — 13 piece chain (2864–2876) + 2 keys (2877–2878) + 22 boss gear (2879–2900) + 56 trash gear (2901–2956)
5. [ ] Write `#ROOMS` section — 35 rooms (2864–3063), all vnums used, all descriptions unique with 3+ sentences (5+ for important rooms), named exits mentioned in room text
6. [ ] Write `#RESETS` section — boss M resets, loot table structure, strong/trash M resets, E equipment resets, O object reset (key in 2887), D door resets
7. [ ] Write `#SPECIALS` section
8. [ ] Write `#OBJFUNS` section if any item special functions are desired
9. [ ] Add `northern_pyramid.are` to `area/area.lst`
10. [ ] Edit `the_northern_oasis.are` room 2211 — add D1 (east) exit to 2895 and update room description to mention the pyramid on the eastern horizon
11. [ ] Edit `the_northern_oasis.are` room 2211 — add `D 2211 1 1` to `#RESETS` to set the east door to closed state
12. [ ] Verify all vnums are within V 2864 3063 range
13. [ ] Verify no vnum conflicts with existing areas (2864–3063 is confirmed free)
14. [ ] Verify all boss rooms have `no_mob` flag
15. [ ] Verify trap room (2864) has `no_mob`, `no_recall`, `no_teleport`
16. [ ] Verify all trap source rooms (2917, 2959, 2989, 3016) have one-way exits to 2864 with no reciprocal exit
17. [ ] Verify piece item chain `value0/value1/value2` correctness before and after each combine step
18. [ ] Verify locked doors each have corresponding key object in `#OBJECTS`
19. [ ] Verify sum of all `L` loot_chance values per mob is ≤ 100
20. [ ] Run `cd src && make unit-tests` to validate
