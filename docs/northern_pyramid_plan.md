# Area Plan: The Northern Pyramid

## Overview

**Area Name:** `@@yNorthern @@bPyramid@@N`
**File Name:** `northern_pyramid.are`
**Keyword:** `northernpyramid`
**Level Range:** 100–120 (I directive: `I 100 120`)
**Level Label:** `@@W(@@y100 120@@W)@@N`
**Vnum Range:** 10125–10324 (V directive: `V 10125 10324`)
**Vnum Count:** 200
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

## Narrative / Lore

The Northern Pyramid predates every living civilization. It rose from the desert sands long before the Northern Oasis was settled, and the traders who built the oasis did so in part to supply expeditions to the pyramid's depths. Countless fortune-seekers entered over the centuries; most never returned. The pyramid's architect — known only as the Sand Sovereign — bound the spirits of seven pharaoh-lords to guard the pyramid's seven floors for eternity, each one more powerful than the last. The Sand Sovereign itself rests at the apex, dreaming behind a gate that only the death of its six servants can unlock.

The pyramid's interior is a labyrinth of traps, false floors, and curse-sealed passages. One chamber in particular — the Stone Trap — can be reached from almost any floor through hidden pressure plates and disguised pit covers; it has no exit except a shaft plunging straight into the undercroft.

---

## Area Header Notes

- `Q 16` — mandatory version
- `K northernpyramid~` — single-word keyword, no spaces, no color codes
- `L @@W(@@y100 120@@W)@@N~` — white parens, yellow level numbers, white close paren, reset
- `N 75` — confirmed unused across all loaded area files
- `I 100 120` — area-gating and mob-level policy
- `V 10125 10324` — full 200-vnum envelope; all rooms, mobs, and objects stay within this range
- `F 20` — 20-minute reset frequency
- `U` — reset message as above; no double-newlines, terminated with `~`
- `T` — teleport flag line (rest ignored by loader)

---

## Level Structure

Seven levels with the trap room as a special single room:

| Level | Name | Vnums | Rooms | Boss Room |
|-------|------|-------|-------|-----------|
| Trap Room | The Stone Trap Chamber | 10125 | 1 | — (no_mob) |
| Sub-ground / Undercroft | The Pharaoh's Undercroft | 10126–10155 | 30 | 10155 |
| Ground Level | The Pyramid Threshold | 10156–10200 | 45 | 10200 |
| Floor 1 | The Hall of the Scarab | 10201–10235 | 35 | 10235 |
| Floor 2 | The Chamber of the High Priests | 10236–10265 | 30 | 10265 |
| Floor 3 | The Eternal Vault | 10266–10290 | 25 | 10290 |
| Floor 4 | The Drake's Ascent | 10291–10310 | 20 | 10310 |
| Floor 5 / Apex | The Summit Sanctum | 10311–10324 | 14 | 10324 |

**Total rooms: 200** (all vnums 10125–10324 used)

---

## Connection to the Northern Oasis

The Northern Pyramid connects bidirectionally to **room 8879** ("Golden Dust Walk") in `the_northern_oasis.are`. Room 8879 is in the northern section of the oasis grid and is currently missing its D1 (east) exit. The caravan road that winds through the oasis once continued east toward the ancient pyramid, a route now reclaimed by the desert.

| Direction | From | To | Notes |
|-----------|------|----|-------|
| east (D1) | 8879 (Golden Dust Walk, Northern Oasis) | 10156 (Before the Northern Pyramid) | Players leave the oasis heading east |
| west (D3) | 10156 (Before the Northern Pyramid) | 8879 (Golden Dust Walk, Northern Oasis) | Return path to the oasis |

### Required Edit to `the_northern_oasis.are`

Room 8879 currently has exits D0→8859 (north), D2→8899 (south), D3→8878 (west). Add a D1 (east) exit:

```
D1
The ancient road continues east through the dunes, where a @@bsandstone@@N peak rises against the sky.

~
^ancient road east~
0 -1 10156
```

The room description of 8879 must be updated to mention the ancient road and the pyramid silhouette on the eastern horizon so the named exit `^ancient road east` is discoverable in room text.

Room 10156 (the pyramid entry room) receives a D3 (west) exit back to 8879:

```
D3
The ancient road curves west through the dunes, back toward the cool shade of the Northern Oasis.

~
~
0 -1 8879
```

---

## Room Layout (200 rooms)

### Zone 0: The Stone Trap Chamber (1 room: 10125)

This special room exists outside the pyramid's normal vertical progression. Hidden pressure plates and disguised pit covers scattered throughout the pyramid all funnel into this one chamber. It has **no exit except D5 (down) to 10134** in the Undercroft. Players who fall in must navigate the Undercroft to escape.

| Vnum | Name | Sector | Flags |
|------|------|--------|-------|
| 10125 | The Stone Trap Chamber | inside | no_mob, indoors, no_recall, no_teleport |

**Exits:**
- D5 (down) → 10134 (The Sunken Trap-Fall in Undercroft); no keyword; no door

**One-way trap exits leading IN to 10125 from across the pyramid (these rooms have exits pointing to 10125; 10125 has no reciprocal exit back to them):**

| Source Room | Vnum | Direction | Notes |
|-------------|------|-----------|-------|
| The False Floor Gallery (Ground Level) | 10178 | D5 (down) | Hidden pit, no keyword |
| The Illusory Corridor (Floor 1) | 10220 | D4 (up) | Appears to lead higher but drops |
| The Marked Flagstones (Floor 2) | 10250 | D5 (down) | Pressure plate |
| The Treacherous Overhang (Floor 3) | 10277 | D4 (up) | False ceiling hatch |

Each source room description must describe the hazard (cracked floor, suspicious passage, etc.) without explicitly naming the trap destination, so players can discover it as a risk.

---

### Zone 1: The Pharaoh's Undercroft (30 rooms: 10126–10155)

The subterranean foundations of the pyramid: flooded burial chambers, root-tangled catacombs, collapsed archways. The oldest and most primitive construction in the structure. Sector type: `inside` (11) throughout. Most rooms flagged `dark`.

| Vnum | Room Name | Sector | Flags | Notes |
|------|-----------|--------|-------|-------|
| 10126 | The Undercroft Entry | inside | indoors | Entry via trapdoor from ground level (10177 ↓) |
| 10127 | A Flooded Antechamber | inside | indoors, dark | Ankle-deep black water |
| 10128 | The Root-Split Corridor | inside | indoors, dark | Ancient roots have fractured the stone |
| 10129 | A Collapsed Burial Niche | inside | indoors, dark | Rubble-filled alcove |
| 10130 | The Drain Tunnel | inside | indoors, dark | Carries overflow from flooded chambers |
| 10131 | A Crypt of Unnamed Dead | inside | indoors | Row of sealed sarcophagi |
| 10132 | The Embalming Chamber | inside | indoors | Ancient tools and canopic tables |
| 10133 | A Stone-Floored Passage | inside | indoors, dark | |
| 10134 | The Sunken Trap-Fall | inside | indoors, dark | Landing point for the Stone Trap (10125 ↓ arrives here) |
| 10135 | A Narrow Root Tunnel | inside | indoors, dark | |
| 10136 | The Ossuary | inside | indoors, dark | Bones stacked floor to ceiling |
| 10137 | A Sunken Gallery | inside | indoors | Faded reliefs on the walls |
| 10138 | The Underground Cistern | inside | indoors | Water still pools here |
| 10139 | A Crumbling Side Passage | inside | indoors, dark | |
| 10140 | The Fungal Grotto | inside | indoors, dark | Pale mushrooms glow faintly |
| 10141 | An Ancient Prayer Hall | inside | indoors | Defaced altar at center |
| 10142 | A Rotted Timber Walkway | inside | indoors, dark | Bridge over a flooded pit |
| 10143 | The Deep Vault | inside | indoors | Wide chamber, multiple exits |
| 10144 | A Hidden Side Chamber | inside | indoors, dark | |
| 10145 | The Sealed Crypt Wing | inside | indoors | Three burial niches sealed with glyphs |
| 10146 | A Stone Drainage Arch | inside | indoors, dark | |
| 10147 | The Putrid Pool | inside | indoors, dark | Stagnant black water, noxious fumes |
| 10148 | A Forgotten Workshop | inside | indoors | Abandoned tools of the pyramid builders |
| 10149 | The Underground Crossroads | inside | indoors | Four-way junction, central hub |
| 10150 | A Pit of Rubble | inside | indoors, dark | Partial collapse |
| 10151 | The Inner Undercroft Hall | inside | indoors | Grand chamber, cracked columns |
| 10152 | A Burial Antechamber | inside | indoors, dark | |
| 10153 | The Threshold of the Crypt Pharaoh | inside | indoors | Approach room; carvings warn of the guardian within |
| 10154 | The Crypt Pharaoh's Approach | inside | indoors | Immediately before boss room; carvings of the guardian's deeds |
| 10155 | The Pharaoh's Tomb Chamber | inside | no_mob, indoors | **BOSS ROOM** — The Crypt Pharaoh |

**Undercroft connectivity:** Entry from ground level (10177 down, locked trapdoor). The key to that trapdoor spawns via `O` reset in room 10148 so trapped players can find it and escape. 10134 (trap-fall landing) connects into the undercroft network but has NO up exit back to 10125. The undercroft hub is 10149; boss approached via 10153→10154→10155.

---

### Zone 2: The Pyramid Threshold — Ground Level (45 rooms: 10156–10200)

The base of the pyramid and surrounding desert approach. Exterior rooms use `desert` sector; interior rooms use `inside` sector. This is the widest and most populated floor.

| Vnum | Room Name | Sector | Flags | Notes |
|------|-----------|--------|-------|-------|
| 10156 | Before the Northern Pyramid | desert | 0 | **Entry from Northern Oasis** (D3 west → 8879) |
| 10157 | The Ancient Road East | desert | 0 | Approach causeway |
| 10158 | The Outer Dune Ring | desert | 0 | Sand dunes surround the pyramid base |
| 10159 | The Southern Approach | desert | 0 | Direct path to main entrance |
| 10160 | The Pyramid's Western Face | desert | 0 | Sheer sandstone wall |
| 10161 | The Pyramid's Eastern Face | desert | 0 | Hieroglyphs carved into stone |
| 10162 | The Outer Courtyard | desert | 0 | Wide plaza before the entrance |
| 10163 | The Courtyard Garden Ruin | desert | 0 | Dried irrigation channels, dead palms |
| 10164 | The Obelisk Circle | desert | 0 | Ring of cracked obelisks |
| 10165 | The Guard Post Ruin | desert | 0 | Collapsed stone gatehouse |
| 10166 | The Pyramid's Main Gate | desert | 0 | Massive carved entrance arch; `^stone gate` (closed door, key -1) |
| 10167 | The Grand Entrance Hall | inside | indoors | First interior room; massive pillared hall |
| 10168 | East Entrance Corridor | inside | indoors | |
| 10169 | West Entrance Corridor | inside | indoors | |
| 10170 | The Pillar Forest | inside | indoors | Dense colonnade of cracked columns |
| 10171 | A Collapsed Side Wing | inside | indoors | Rubble blocks most of the room |
| 10172 | The Trophy Chamber | inside | indoors | Niches once held offerings |
| 10173 | The Inscription Gallery | inside | indoors | Floor-to-ceiling hieroglyphs |
| 10174 | A Guard Barracks Ruin | inside | indoors, dark | |
| 10175 | The Armory Vault | inside | indoors | Empty weapon racks |
| 10176 | The East Wing Hall | inside | indoors | |
| 10177 | The Lower Vault | inside | indoors | Contains trapdoor to undercroft (D5 down → 10126, locked) |
| 10178 | The False Floor Gallery | inside | indoors | Contains trap exit D5 → 10125 (hidden pit, one-way) |
| 10179 | A Side Alcove | inside | indoors, dark | |
| 10180 | The West Wing Hall | inside | indoors | |
| 10181 | The Stone Library | inside | indoors | Collapsed shelves, crumbled tablets |
| 10182 | The Map Chamber | inside | indoors | Floor mosaic of the surrounding desert |
| 10183 | The Central Hall | inside | indoors | Wide hub room, stairwells rise from here |
| 10184 | The East Stairwell Landing | inside | indoors | D4 (up) → Floor 1 entry (10201) |
| 10185 | The West Stairwell Landing | inside | indoors | Alternate D4 (up) → 10202 |
| 10186 | A Guard Chamber | inside | indoors, dark | |
| 10187 | The Offering Hall | inside | indoors | Ceremonial tables; empty basins |
| 10188 | The North Inner Wing | inside | indoors | |
| 10189 | The South Inner Wing | inside | indoors | |
| 10190 | A Ceremonial Passage | inside | indoors | |
| 10191 | The Root-Cracked Chamber | inside | indoors, dark | Ancient roots push through the floor |
| 10192 | The Throne Antechamber | inside | indoors | Approach to ground-floor boss zone |
| 10193 | The Lower Throne Room | inside | indoors | Grand dais, cracked royal chair |
| 10194 | The Sanctum Approach | inside | indoors | |
| 10195 | A Preparation Chamber | inside | indoors, dark | |
| 10196 | The Sentinel's Passage | inside | indoors | |
| 10197 | The Inner Sanctum Entrance | inside | indoors | `^ancient stone door` (closed, no lock) leads to 10198 |
| 10198 | The Waiting Hall | inside | indoors | |
| 10199 | The Final Approach | inside | indoors | |
| 10200 | The Pyramid's Heart Chamber | inside | no_mob, indoors | **BOSS ROOM** — The Desert Gate Guardian |

---

### Zone 3: The Hall of the Scarab — Floor 1 (35 rooms: 10201–10235)

First ascending floor. Slightly narrower than the ground level. Scarab imagery dominates the carvings.

| Vnum | Room Name | Sector | Flags | Notes |
|------|-----------|--------|-------|-------|
| 10201 | The First Ascent | inside | indoors | Entry from ground D4 (10184 ↑) |
| 10202 | The Upper Landing | inside | indoors | Entry from alternate ground D4 (10185 ↑) |
| 10203 | The Scarab Hall | inside | indoors | Giant scarab reliefs on every wall |
| 10204 | The Beetle Colonnade | inside | indoors | Stone scarabs atop every column |
| 10205 | An Alcove of Offerings | inside | indoors, dark | |
| 10206 | The East Passage | inside | indoors | |
| 10207 | The West Passage | inside | indoors | |
| 10208 | The Ritual Basin Room | inside | indoors | Dried ceremonial pools |
| 10209 | A Collapsed Side Chamber | inside | indoors, dark | |
| 10210 | The Cartouche Gallery | inside | indoors | Names of the ancient priests carved in stone |
| 10211 | The Scribes' Chamber | inside | indoors | Broken styluses and dried inkpots |
| 10212 | A Guard Room | inside | indoors, dark | |
| 10213 | The Mosaic Floor Room | inside | indoors | Floor is one enormous scarab mosaic |
| 10214 | The Inner Colonnade | inside | indoors | |
| 10215 | The East Stair Landing | inside | indoors | D4 (up) → Floor 2 (10236) |
| 10216 | The West Stair Landing | inside | indoors | D4 (up) → Floor 2 (10237) |
| 10217 | A Shrine Alcove | inside | indoors, dark | |
| 10218 | The Embalming Gallery | inside | indoors | Carved depictions of funerary rites |
| 10219 | The Priests' Corridor | inside | indoors | |
| 10220 | The Illusory Corridor | inside | indoors | Contains trap exit D4 → 10125 (appears to go higher, one-way) |
| 10221 | A Torch-Bracket Hall | inside | indoors, dark | |
| 10222 | The Scarab Shrine | inside | indoors | Central shrine, offerings intact |
| 10223 | A Guardians' Niche | inside | indoors, dark | |
| 10224 | The Ritual Cleansing Room | inside | indoors | |
| 10225 | The Inner Priest Chamber | inside | indoors | |
| 10226 | A Narrow Side Passage | inside | indoors, dark | |
| 10227 | The Hall of Prayers | inside | indoors | |
| 10228 | The Side Archive | inside | indoors, dark | |
| 10229 | A Ventilation Shaft Landing | inside | indoors | |
| 10230 | The Preparation Hall | inside | indoors | |
| 10231 | The Warden's Approach | inside | indoors | |
| 10232 | The Outer Guardian Chamber | inside | indoors | |
| 10233 | A Final Corridor | inside | indoors, dark | |
| 10234 | The Scarab Colossus Approach | inside | indoors | `^stone scarab door` (closed, no lock) leads to 10235 |
| 10235 | The Scarab Colossus Chamber | inside | no_mob, indoors | **BOSS ROOM** — The Scarab Colossus |

---

### Zone 4: The Chamber of the High Priests — Floor 2 (30 rooms: 10236–10265)

Second floor. Narrower than Floor 1. Sacred priestly quarters and ritual chambers.

| Vnum | Room Name | Sector | Flags | Notes |
|------|-----------|--------|-------|-------|
| 10236 | The Second Ascent | inside | indoors | Entry from Floor 1 D4 (10215 ↑) |
| 10237 | The Priests' Landing | inside | indoors | Entry from alternate Floor 1 D4 (10216 ↑) |
| 10238 | The High Priests' Hall | inside | indoors | |
| 10239 | The Anointing Chamber | inside | indoors | |
| 10240 | A Ritual Side Room | inside | indoors, dark | |
| 10241 | The Sacred Scroll Room | inside | indoors | Crumbled papyrus fragments |
| 10242 | The Altar of the Four Winds | inside | indoors | Ceremonial altar; compass points carved in floor |
| 10243 | A Candlelit Niche | inside | indoors, dark | |
| 10244 | The Priests' Sleeping Quarters | inside | indoors | Stone cots along the walls |
| 10245 | The Inner Prayer Hall | inside | indoors | |
| 10246 | A Secret Passage | inside | indoors, dark | Hidden behind a loose block |
| 10247 | The Ritual Fire Pit | inside | indoors | Ash-filled depression in floor center |
| 10248 | The Vestment Chamber | inside | indoors | Empty robing alcoves |
| 10249 | A Corridor of Wards | inside | indoors | Protective glyphs cover every surface |
| 10250 | The Marked Flagstones | inside | indoors | Contains trap exit D5 → 10125 (pressure plate, one-way) |
| 10251 | The Chanting Hall | inside | indoors | Acoustic chamber; voices echo strangely |
| 10252 | A Side Shrine | inside | indoors, dark | |
| 10253 | The Vaulted Passage | inside | indoors | |
| 10254 | The East Ceremonial Wing | inside | indoors | |
| 10255 | The West Ceremonial Wing | inside | indoors | |
| 10256 | A Preparation Alcove | inside | indoors, dark | |
| 10257 | The Hall of the Sacred Asp | inside | indoors | Cobra carvings on every surface |
| 10258 | The Archpriest's Study | inside | indoors | |
| 10259 | A Collapsed Vault | inside | indoors, dark | |
| 10260 | The Upper Stair Landing | inside | indoors | D4 (up) → Floor 3 (10266), through `^iron-banded archway` (closed, no lock) |
| 10261 | The Inner Sanctum Antechamber | inside | indoors | |
| 10262 | The Outer Chamber | inside | indoors | |
| 10263 | A Final Hall | inside | indoors, dark | |
| 10264 | The High Priest's Approach | inside | indoors | `^carved stone threshold` (closed, no lock) leads to 10265 |
| 10265 | The High Priest's Sanctum | inside | no_mob, indoors | **BOSS ROOM** — The High Priest of the Ancient Sands |

---

### Zone 5: The Eternal Vault — Floor 3 (25 rooms: 10266–10290)

Third floor. Further narrowed. The eternal vault where powerful undead are interred.

| Vnum | Room Name | Sector | Flags | Notes |
|------|-----------|--------|-------|-------|
| 10266 | The Third Ascent | inside | indoors | Entry via `^iron-banded archway` from 10260 |
| 10267 | The Vault Antechamber | inside | indoors | |
| 10268 | A Sealed Side Chamber | inside | indoors, dark | |
| 10269 | The Hall of Eternal Rest | inside | indoors | Sealed sarcophagi in wall recesses |
| 10270 | The Embalmer's Gallery | inside | indoors | Illustrated burial rites |
| 10271 | The Soul Weighing Chamber | inside | indoors | Giant carved scale dominates the room |
| 10272 | A Dark Corridor | inside | indoors, dark | |
| 10273 | The Mummy Lords' Wing | inside | indoors | |
| 10274 | A Burial Alcove | inside | indoors, dark | |
| 10275 | The Preserved Archives | inside | indoors | Sealed jars of records; all unreadable |
| 10276 | The Ritual Unguent Room | inside | indoors | |
| 10277 | The Treacherous Overhang | inside | indoors, dark | Contains trap exit D4 → 10125 (false ceiling hatch, one-way) |
| 10278 | The East Vault Passage | inside | indoors | D4 (up) → Floor 4 entry (10291) |
| 10279 | The West Vault Passage | inside | indoors | |
| 10280 | The Canopic Shrine | inside | indoors | Four canopic jars on stone pedestals |
| 10281 | A Collapsed Crypt Wing | inside | indoors, dark | |
| 10282 | The Inner Vault Hall | inside | indoors | |
| 10283 | A Final Preparation Room | inside | indoors, dark | |
| 10284 | The Warded Threshold | inside | indoors | Glyphs of warning on every stone |
| 10285 | The Mummy Lord's Antechamber | inside | indoors | |
| 10286 | A Side Passage | inside | indoors, dark | |
| 10287 | The Guard Station | inside | indoors | |
| 10288 | The Inner Hall | inside | indoors | |
| 10289 | The Mummy Lord's Approach | inside | indoors | `^glowing stone arch` (closed, no lock) leads to 10290 |
| 10290 | The Eternal Vault Chamber | inside | no_mob, indoors | **BOSS ROOM** — The Eternal Mummy Lord |

---

### Zone 6: The Drake's Ascent — Floor 4 (20 rooms: 10291–10310)

Fourth floor. Narrow passages. The lair of a beast that has claimed the upper pyramid.

| Vnum | Room Name | Sector | Flags | Notes |
|------|-----------|--------|-------|-------|
| 10291 | The Fourth Ascent | inside | indoors | Entry from Floor 3 D4 (10278 ↑) |
| 10292 | The Drake's Outer Hall | inside | indoors | Scorched stone; heat lingers |
| 10293 | A Scorch-Marked Passage | inside | indoors, dark | |
| 10294 | The Ash-Filled Chamber | inside | indoors | Deep drifts of ash from ancient fires |
| 10295 | The Claw-Marked Corridor | inside | indoors, dark | Deep gouges in the stone walls |
| 10296 | The Bone-Strewn Hall | inside | indoors | |
| 10297 | The Char-Pit Room | inside | indoors, dark | Blackened floor pit at center |
| 10298 | The Trophy Hall | inside | indoors | Skulls mounted on iron spikes |
| 10299 | A Narrow Scorched Passage | inside | indoors, dark | |
| 10300 | The Upper Drake Hall | inside | indoors | |
| 10301 | A Heat-Shimmering Corridor | inside | indoors | Waves of heat emanate from cracks in the wall |
| 10302 | The Nest Antechamber | inside | indoors, dark | |
| 10303 | The Feeding Ground | inside | indoors | Gnawed bones carpet the floor |
| 10304 | An Ascending Shaft | inside | indoors | D4 (up) → Floor 5, through `^stone sentinel gate` (locked, key 10139) |
| 10305 | A Side Passage | inside | indoors, dark | |
| 10306 | The Inner Drake Hall | inside | indoors | |
| 10307 | A Scorched Archive | inside | indoors, dark | Burned shelves, ash-covered tablets |
| 10308 | The Pre-Lair Corridor | inside | indoors | |
| 10309 | The Drake's Approach | inside | indoors | `^bone-adorned gate` (closed, no lock) leads to 10310 |
| 10310 | The Pyramid Drake's Nest | inside | no_mob, indoors | **BOSS ROOM** — The Pyramid Drake |

---

### Zone 7: The Summit Sanctum — Floor 5 / Apex (14 rooms: 10311–10324)

The topmost and smallest floor. The apex of the pyramid. The final pharaoh-spirit waits here.

| Vnum | Room Name | Sector | Flags | Notes |
|------|-----------|--------|-------|-------|
| 10311 | The Summit Entry | inside | indoors | Entry via `^stone sentinel gate` from 10304 |
| 10312 | The Apex Corridor | inside | indoors | |
| 10313 | The Wind-Scoured Chamber | inside | indoors | Cracks in the ceiling let in hot desert air |
| 10314 | The Golden Hall | inside | indoors | Gold leaf lines every surface |
| 10315 | The Relic Chamber | inside | indoors | Empty pedestals where artifacts once rested |
| 10316 | A Side Niche | inside | indoors, dark | |
| 10317 | The Hall of Judgment | inside | indoors | Final carvings depict the afterlife |
| 10318 | The Pharaoh's Antechamber | inside | indoors | |
| 10319 | The Inner Apex Hall | inside | indoors | |
| 10320 | A Narrow Final Passage | inside | indoors, dark | |
| 10321 | The Throne Room Approach | inside | indoors | |
| 10322 | The Pre-Throne Hall | inside | indoors | |
| 10323 | The Sand Sovereign's Approach | inside | indoors | `^summit chamber door` (closed, no lock) leads to 10324 |
| 10324 | The Summit Throne Room | inside | no_mob, indoors | **BOSS ROOM** — The Sand Sovereign |

---

## Doors Summary

| From Room | Direction | To Room | Keyword | Lock? | Key Vnum | Reset State |
|-----------|-----------|---------|---------|-------|----------|-------------|
| 8879 (N.Oasis) | D1 (east) | 10156 | `^ancient road east` | No | -1 | `D 8879 1 1` (closed) |
| 10156 | D3 (west) | 8879 | (none) | No | -1 | open |
| 10166 | D0 (north) | 10167 | `^stone gate` | No | -1 | `D 10166 0 1` (closed) |
| 10167 | D2 (south) | 10166 | `^stone gate` | No | -1 | `D 10167 2 1` (closed) |
| 10177 | D5 (down) | 10126 | `^heavy trapdoor` | Yes (locked) | 10138 | `D 10177 5 2` (locked) |
| 10126 | D4 (up) | 10177 | `^heavy trapdoor` | Yes (locked) | 10138 | `D 10126 4 2` (locked) |
| 10197 | D0 (north) | 10198 | `^ancient stone door` | No | -1 | `D 10197 0 1` (closed) |
| 10198 | D2 (south) | 10197 | `^ancient stone door` | No | -1 | `D 10198 2 1` (closed) |
| 10234 | D0 (north) | 10235 | `^stone scarab door` | No | -1 | `D 10234 0 1` (closed) |
| 10235 | D2 (south) | 10234 | `^stone scarab door` | No | -1 | `D 10235 2 1` (closed) |
| 10260 | D4 (up) | 10266 | `^iron-banded archway` | No | -1 | `D 10260 4 1` (closed) |
| 10266 | D5 (down) | 10260 | `^iron-banded archway` | No | -1 | `D 10266 5 1` (closed) |
| 10264 | D0 (north) | 10265 | `^carved stone threshold` | No | -1 | `D 10264 0 1` (closed) |
| 10265 | D2 (south) | 10264 | `^carved stone threshold` | No | -1 | `D 10265 2 1` (closed) |
| 10289 | D0 (north) | 10290 | `^glowing stone arch` | No | -1 | `D 10289 0 1` (closed) |
| 10290 | D2 (south) | 10289 | `^glowing stone arch` | No | -1 | `D 10290 2 1` (closed) |
| 10304 | D4 (up) | 10311 | `^stone sentinel gate` | Yes (locked) | 10139 | `D 10304 4 2` (locked) |
| 10311 | D5 (down) | 10304 | `^stone sentinel gate` | Yes (locked) | 10139 | `D 10311 5 2` (locked) |
| 10309 | D0 (north) | 10310 | `^bone-adorned gate` | No | -1 | `D 10309 0 1` (closed) |
| 10310 | D2 (south) | 10309 | `^bone-adorned gate` | No | -1 | `D 10310 2 1` (closed) |
| 10323 | D0 (north) | 10324 | `^summit chamber door` | No | -1 | `D 10323 0 1` (closed) |
| 10324 | D2 (south) | 10323 | `^summit chamber door` | No | -1 | `D 10324 2 1` (closed) |
| 10155 (boss) | D2 (south) | 10154 | `^root-sealed vault door` | No | -1 | `D 10155 2 1` (closed) |
| 10154 | D0 (north) | 10155 | `^root-sealed vault door` | No | -1 | `D 10154 0 1` (closed) |

All named doors must be mentioned in at least one of: the room's main description, a spawned object, or an `E` extra description.

Both locked doors require the corresponding key object to exist in `#OBJECTS`:
- `^heavy trapdoor` (rooms 10177/10126): key vnum **10138**
- `^stone sentinel gate` (rooms 10304/10311): key vnum **10139**

---

## Mobile Definitions

### Bosses (Level 130)

All bosses: `act` flags = `is_npc(1) | sentinel(2) | no_flee(16) | aggressive(32) | stay_area(64) | boss(67108864) | noassist(1073741824)` = **1140850879**

Boss skills (`!` line): `5_attack(8) | enhanced(32768) | counter(524288) | parry(8192) | dodge(4096) | nodisarm(32) | notrip(64)` = **569448**

All bosses placed only in `no_mob` flagged rooms.

| Mob Vnum | Name | Room | Act Flags | Theme | Spec |
|----------|------|------|-----------|-------|------|
| 10125 | the Crypt Pharaoh | 10155 | 1140850879 | Undead pharaoh-spirit of ancient death magic. Cast: suffocate, mind_flail, nerve_fire. Def: heal, iceshield. Strong: shadow, mental. Weak: holy, fire. | spec_cast_undead |
| 10126 | the Desert Gate Guardian | 10200 | 1140850879 | Massive animated stone construct, guardian of the ground floor. Cast: earthquake, flamestrike. Def: heal, shockshield. Strong: earth, physical. Weak: mental. 6_attack instead of 5_attack. | spec_cast_mage |
| 10127 | the Scarab Colossus | 10235 | 1140850879 | Enormous animated scarab construct. Cast: acid_blast, lava_burst. Def: fireshield, iceshield. Strong: earth, physical. Weak: mental, water. | spec_cast_mage |
| 10128 | the High Priest of the Ancient Sands | 10265 | 1140850879 | Ancient priestly undead spirit with potent offensive magic. Cast: chain_light, mindflame, bloody_tears. Def: heal, iceshield, fireshield. Strong: shadow, mental. Weak: holy. | spec_cast_bigtime |
| 10129 | the Eternal Mummy Lord | 10290 | 1140850879 | Mummified warlord animated by a millennium of curses. Cast: suffocate, mind_bolt, ego_whip. Def: heal, cure_critic. Strong: shadow. Weak: fire, holy. | spec_cast_undead |
| 10130 | the Pyramid Drake | 10310 | 1140850879 | Ancient desert dragon-kin that has claimed the upper floors. Cast: lava_burst, heat_armor, burn_hands. Def: fireshield. Strong: fire, physical. Weak: water, air. Spec: spec_breath_fire. | spec_breath_fire |
| 10131 | the Sand Sovereign | 10324 | 1140850879 | The immortal architect of the pyramid. Full spell set. Cast: nerve_fire, light_bolt, mindflame, suffocate, lava_burst. Def: heal, fireshield, iceshield, shockshield. Strong: earth, fire, mental. Weak: holy. | spec_cast_bigtime |

**Boss `+` extension:** All bosses receive moderate spellpower, crit, crit_mult, parry, dodge, and block modifiers appropriate for level 130 encounters. The Sand Sovereign (10131) receives maximum values.

---

### Strong (Solo) Mobs (Levels 118–122)

All strong mobs: `act` = `is_npc(1) | aggressive(32) | stay_area(64) | solo(33554432)` = **33554529**

| Mob Vnum | Name | Level | Zones | Notes |
|----------|------|-------|-------|-------|
| 10132 | a cursed pharaoh's shade | 120 | All | Undead spirit. Cast: mind_flail, suffocate. Def: heal. |
| 10133 | a desert stone titan | 122 | Ground, Floor 1 | Massive animated construct. Enhanced, 4_attack, nodisarm, notrip. |
| 10134 | a pyramid death knight | 121 | Floors 2–4 | Armored undead warrior. 4_attack, enhanced, parry, counter. |
| 10135 | a sand drake | 120 | Floors 3–5 | Lesser drake. 4_attack, enhanced, parry, dodge. Spec: spec_breath_fire. |
| 10136 | a golden sphinx | 122 | Floors 4–5 | Construct guardian. 3_attack, enhanced, parry, dodge. Cast: earthquake, light_bolt. |
| 10185 | a sandstorm elemental | 121 | Ground, Floor 1 | Air/sand elemental construct. no_mind. 3_attack, enhanced, dodge. Cast: col_spray, stun. Spec: spec_cast_mage. |
| 10186 | an ancient obsidian guardian | 122 | Floor 2, Floor 3 | Massive black-stone construct. no_mind, sentinel. 4_attack, enhanced, parry, nodisarm, notrip. |
| 10187 | a greater mummy overlord | 120 | Floor 3, Floor 4 | Towering ancient warlord-spirit. undead. 3_attack, enhanced, parry, counter. Cast: suffocate, ego_whip. Spec: spec_cast_undead. |

---

### Trash Mobs (Levels 105–120)

All trash: `act` = `is_npc(1) | aggressive(32) | stay_area(64)` = **97**

| Mob Vnum | Name | Level | Zones | Combat Notes |
|----------|------|-------|-------|--------------|
| 10137 | a sand scarab swarm | 105 | Undercroft, Ground | no_body, no_mind. 2_attack. |
| 10138 (mob) | a skeleton warrior | 107 | Undercroft | undead. 2_attack, punch. |
| 10139 (mob) | a cursed mummy | 108 | Undercroft | undead. 2_attack, enhanced. |
| 10140 | a giant desert scorpion | 110 | Undercroft, Ground | Spec: spec_poison. 2_attack. |
| 10141 | a tomb robber's ghost | 106 | Ground | undead, no_body. Cast: flare, shock_grasp. |
| 10142 | a sand jackal | 112 | Ground | 2_attack, dodge, trip. |
| 10143 | a granite guardian statue | 115 | Ground, Floor 1 | no_mind. 3_attack, enhanced, nodisarm. |
| 10144 | a canopic horror | 109 | Undercroft, Ground | undead, no_mind. 2_attack, punch. |
| 10145 | a golden asp | 113 | Floor 1, Floor 2 | Spec: spec_poison. 2_attack. |
| 10146 | a desert hawk | 116 | Floor 1, Floor 2 | 2_attack, dodge. |
| 10147 | a sand golem | 110 | Floor 1 | no_mind. 3_attack, enhanced. |
| 10148 (mob) | a cursed acolyte | 118 | Floor 2 | Cast: faerie_fire, flare, col_spray. Def: cure_serious. |
| 10149 | a desert wight | 114 | Floor 2 | undead. 2_attack, enhanced, drain. |
| 10150 | a stone scarab guardian | 117 | Floor 2, Floor 3 | no_mind. 3_attack, enhanced, parry. |
| 10151 | a pyramid cobra | 111 | Floor 2 | Spec: spec_poison. 2_attack. |
| 10152 | a mummified archer | 119 | Floor 3 | undead. 2_attack, enhanced, dodge. |
| 10153 (mob) | a great desert vulture | 120 | Floor 3 | 2_attack, dodge, kick. |
| 10154 (mob) | an ancient face guardian | 116 | Floor 3 | no_mind, sentinel. 2_attack, parry, nodisarm. |
| 10155 (mob) | a desert shadow | 112 | Floor 3, Floor 4 | undead, no_body. Cast: mind_bolt, ego_whip. |
| 10156 (mob) | a desert shadow assassin | 118 | Floor 3, Floor 4 | 2_attack, enhanced, dodge, trip. |
| 10157 | a living pillar golem | 120 | Floor 4 | no_mind, sentinel. 4_attack, enhanced, nodisarm, notrip. |
| 10158 | a sacred golden cobra | 115 | Floor 4 | Spec: spec_poison. 2_attack. |
| 10159 | a pyramid cultist | 117 | Floor 4 | Cast: burn_hands, mag_missile. Def: cure_serious. |
| 10160 | a death hawk | 119 | Floor 4, Floor 5 | 2_attack, dodge, kick. |
| 10161 | a sand revenant | 113 | Floor 4 | undead. 2_attack, enhanced, parry. Def: cure_critic. |
| 10162 | a pyramid specter | 116 | Floor 5 | undead, no_body. Cast: mindflame, mind_bolt. Def: cure_critic. |
| 10163 | a tomb fly swarm | 105 | Undercroft | Clouds of carrion flies. no_body, no_mind. 2_attack. Spec: spec_disease. |
| 10164 | a desiccated desert rat | 105 | Undercroft, Ground | Dessicated undead vermin. no_mind. 2_attack, trip. Spec: spec_disease. |
| 10165 | a bone crawler | 107 | Undercroft | Animated skeletal mass. undead, no_mind. 3_attack, enhanced. |
| 10166 | a sand-choked ghoul | 109 | Undercroft, Ground | Ghoul packed with sand. undead. 2_attack, enhanced, drain. |
| 10167 | an animated obelisk shard | 112 | Ground | Fragment of a shattered obelisk come alive. no_mind, sentinel. 3_attack, enhanced, nodisarm. |
| 10168 | a cursed pyramid builder | 110 | Ground, Floor 1 | Ghost of a slave forced to build the pyramid. undead. 2_attack, enhanced. Cast: weaken, blindness. |
| 10169 | a desert sand adder | 108 | Ground | A fat, heat-seeking snake. 2_attack, dodge. Spec: spec_poison. |
| 10170 | an ancient sentinel construct | 114 | Ground, Floor 1 | Hulking guardian golem, still functional. no_mind. 3_attack, enhanced, parry, nodisarm. |
| 10171 | a hieroglyph guardian | 115 | Floor 1 | Living inscription carved into stone, peeled free. no_mind, sentinel. 3_attack, enhanced. |
| 10172 | an animated canopic jar | 112 | Floor 1, Floor 2 | A sealed burial jar walking on spindly carved legs. undead, no_body. 2_attack, punch. Cast: poison, col_spray. |
| 10173 | a scribal revenant | 116 | Floor 1 | The ghost of a scribe whose knowledge was sealed in the pyramid. undead. 2_attack. Cast: flare, mag_missile. Def: cure_light. |
| 10174 | a sand-wrapped mummy servant | 113 | Floor 1, Floor 2 | A lesser mummy bound in sand-crusted linen. undead. 2_attack, enhanced. |
| 10175 | a high priest's thrall | 118 | Floor 2 | Undead acolyte enslaved by the High Priest's will. undead. 2_attack, enhanced, parry. Cast: faerie_fire. |
| 10176 | a sand wraith | 117 | Floor 2, Floor 3 | A translucent desert spirit wreathed in swirling sand. undead, no_body. Cast: ego_whip, cold_grasp. Def: cure_serious. |
| 10177 | a curse-bound warrior | 119 | Floor 2, Floor 3 | An ancient soldier held together by curse alone. undead. 3_attack, enhanced, counter. |
| 10178 | an entombed warlord | 118 | Floor 3 | A general buried standing upright, still battle-ready. undead. 3_attack, enhanced, parry, dodge. |
| 10179 | a flesh golem | 120 | Floor 3, Floor 4 | A construct of stitched mummy flesh. no_mind. 4_attack, enhanced, nodisarm, notrip. |
| 10180 | a tomb stalker | 116 | Floor 3 | A beast that hunts intruders in the dark. 2_attack, dodge, trip. |
| 10181 | an ember lizard | 117 | Floor 4 | A long-tailed reptile that absorbs heat from the scorched stone. 2_attack, dodge. Resist: fire. |
| 10182 | a charred husk | 119 | Floor 4 | An undead burned to blackened bone and sinew. undead. 3_attack, enhanced. Strong: fire. Weak: water. |
| 10183 | an apex sentinel | 120 | Floor 5 | The last stone guardian before the throne. no_mind, sentinel. 3_attack, enhanced, parry, nodisarm. |
| 10184 | a gilded warrior shade | 118 | Floor 5 | The ghost of a royal bodyguard, still adorned in ceremonial gold. undead. 2_attack, enhanced. Cast: mind_bolt. |

**Note on vnum reuse:** Mob vnums 10137–10187 overlap numerically with object vnums in the same range. Cross-type vnum overlap is permitted by the spec; each is unique within its own type index.

---

## Object Definitions

### Piece Item Chain (13 objects: 10125–10137)

All seven boss fragment pieces are delivered via boss loot tables and carry `ITEM_LOOT`.

**Fragment extra_flags:** `ITEM_NODROP(128) | ITEM_BOSS(134217728) | ITEM_MAGIC(64) | ITEM_LOOT(67108864)` = **201326784**

**Assembly extra_flags (created by combining, not looted):** `ITEM_NODROP(128) | ITEM_BOSS(134217728) | ITEM_MAGIC(64)` = **134217920**

**Final Crown extra_flags:** `ITEM_RARE(1048576) | ITEM_NODROP(128) | ITEM_BOSS(134217728) | ITEM_MAGIC(64)` = **135266496**

All piece and assembly items: `item_type = 29` (ITEM_PIECE), `wear_flags = take(8388608)`.
Final Crown: `item_type = 9` (ITEM_ARMOR), `wear_flags = head(8) | take(8388608) = 8388616`.

| Obj Vnum | Name | Type | value0 | value1 | value2 | Source |
|----------|------|------|--------|--------|--------|--------|
| 10125 | a fragment of the pharaoh's crown (I) | piece | 0 | 10126 | 10132 | Crypt Pharaoh loot |
| 10126 | a fragment of the pharaoh's crown (II) | piece | 10125 | 0 | 10132 | Desert Gate Guardian loot |
| 10127 | a fragment of the pharaoh's crown (III) | piece | 10132 | 0 | 10133 | Scarab Colossus loot |
| 10128 | a fragment of the pharaoh's crown (IV) | piece | 10133 | 0 | 10134 | High Priest loot |
| 10129 | a fragment of the pharaoh's crown (V) | piece | 10134 | 0 | 10135 | Eternal Mummy Lord loot |
| 10130 | a fragment of the pharaoh's crown (VI) | piece | 10135 | 0 | 10136 | Pyramid Drake loot |
| 10131 | a fragment of the pharaoh's crown (VII) | piece | 10136 | 0 | 10137 | Sand Sovereign loot |
| 10132 | a partial crown of the pharaoh (I-II) | piece | 0 | 10127 | 10133 | Created from I+II |
| 10133 | a partial crown of the pharaoh (I-III) | piece | 0 | 10128 | 10134 | Created from (I-II)+III |
| 10134 | a partial crown of the pharaoh (I-IV) | piece | 0 | 10129 | 10135 | Created from (I-III)+IV |
| 10135 | a partial crown of the pharaoh (I-V) | piece | 0 | 10130 | 10136 | Created from (I-IV)+V |
| 10136 | a partial crown of the pharaoh (I-VI) | piece | 0 | 10131 | 10137 | Created from (I-V)+VI |
| 10137 | the Burial Crown of the Sand Sovereign | armor (head) | — | — | — | **Final combined item** |

**Combining sequence:**
1. Fragment I (10125) + Fragment II (10126) → Assembly I-II (10132)
2. Assembly I-II (10132) + Fragment III (10127) → Assembly I-III (10133)
3. Assembly I-III (10133) + Fragment IV (10128) → Assembly I-IV (10134)
4. Assembly I-IV (10134) + Fragment V (10129) → Assembly I-V (10135)
5. Assembly I-V (10135) + Fragment VI (10130) → Assembly I-VI (10136)
6. Assembly I-VI (10136) + Fragment VII (10131) → **the Burial Crown of the Sand Sovereign (10137)**

The `connect` command checks: `pieceA.value0 == pieceB.vnum` OR `pieceA.value1 == pieceB.vnum`. In each step above, one piece's `value1` equals the other piece's vnum, satisfying the check. On success, both are consumed and `pieceA.value2` is created.

---

### Key Objects (2 objects: 10138–10139)

Both keys are nodrop. Key 10138 spawns via `O` reset in room 10148 (Forgotten Workshop in Undercroft) so players who fall into the trap can eventually escape. Key 10139 drops from the Pyramid Drake (10130) via loot table.

| Obj Vnum | Name | item_type | extra_flags | wear_flags | Level | Notes |
|----------|------|-----------|-------------|------------|-------|-------|
| 10138 | a corroded bronze trapdoor key | key (18) | ITEM_NODROP (128) | take (8388608) | L 100 | Opens `^heavy trapdoor` (rooms 10177/10126) |
| 10139 | a carved stone sentinel key | key (18) | ITEM_NODROP(128) \| ITEM_LOOT(67108864) = 67108992 | take (8388608) | L 120 | Opens `^stone sentinel gate` (rooms 10304/10311). Boss loot from Pyramid Drake. |

---

### Boss Equipment (Level 120, extra_flags = ITEM_NODROP(128) | ITEM_BOSS(134217728) | ITEM_MAGIC(64) = 134217920)

Items delivered via boss loot tables also add ITEM_LOOT(67108864) for total: **201326784**.

Each boss drops its piece fragment plus 3 boss gear items via loot table. The Sand Sovereign drops 4 gear items.

Weight key: 1-5 = caster, 6-10 = melee, 11-15 = tank.

**Crypt Pharaoh (mob 10125) — gear vnums 10140–10142:**

| Obj Vnum | Name | item_type | wear_flags | weight | Archetype |
|----------|------|-----------|------------|--------|-----------|
| 10140 | the Crypt Pharaoh's death mask | armor (9) | face(16)\|take | 1 | caster |
| 10141 | a cloak of undying shadow | armor (9) | about(65536)\|take | 1 | caster |
| 10142 | the Crypt Pharaoh's burial wrappings | armor (9) | body(262144)\|take | 11 | tank |

**Desert Gate Guardian (mob 10126) — gear vnums 10143–10145:**

| Obj Vnum | Name | item_type | wear_flags | weight | Archetype |
|----------|------|-----------|------------|--------|-----------|
| 10143 | a gauntlet of the stone guardian | armor (9) | hands(4096)\|take | 11 | tank |
| 10144 | the guardian's sandstone maul | weapon (5) | hold(32768)\|take | 6 | melee; value3=7 (pound) |
| 10145 | a sash of the desert gate | armor (9) | waist(131072)\|take | 6 | melee |

**Scarab Colossus (mob 10127) — gear vnums 10146–10148:**

| Obj Vnum | Name | item_type | wear_flags | weight | Archetype |
|----------|------|-----------|------------|--------|-----------|
| 10146 | a chitinous scarab breastplate | armor (9) | body(262144)\|take | 11 | tank |
| 10147 | a scarab-shell buckler | armor (9) | hold(32768)\|take | 11 | tank; ITEM_BUCKLER extra flag added (extra_flags = 201326784 + 268435456 = 469762240 for loot version) |
| 10148 | a claw of the Scarab Colossus | weapon (5) | hold(32768)\|take | 6 | melee; ITEM_FIST flag; value3=0 (hit) |

Note for 10148: extra_flags for this loot weapon = ITEM_NODROP(128) | ITEM_BOSS(134217728) | ITEM_MAGIC(64) | ITEM_LOOT(67108864) | ITEM_FIST(1073741824) = 1275068608

**High Priest of the Ancient Sands (mob 10128) — gear vnums 10149–10151:**

| Obj Vnum | Name | item_type | wear_flags | weight | Archetype |
|----------|------|-----------|------------|--------|-----------|
| 10149 | the High Priest's golden circlet | armor (9) | head(8)\|take | 1 | caster |
| 10150 | a robe of the ancient rites | armor (9) | body(262144)\|take | 1 | caster |
| 10151 | the High Priest's sacrificial blade | weapon (5) | hold(32768)\|take | 6 | melee; value3=2 (stab) |

**Eternal Mummy Lord (mob 10129) — gear vnums 10152–10154:**

| Obj Vnum | Name | item_type | wear_flags | weight | Archetype |
|----------|------|-----------|------------|--------|-----------|
| 10152 | greaves of the eternal mummy | armor (9) | legs(1048576)\|take | 11 | tank |
| 10153 | the Mummy Lord's cursed flail | weapon (5) | hold(32768)\|take | 6 | melee; value3=7 (pound) |
| 10154 | wrappings of ancient preservation | armor (9) | arms(1024)\|take | 11 | tank |

**Pyramid Drake (mob 10130) — gear vnums 10155–10157:**

| Obj Vnum | Name | item_type | wear_flags | weight | Archetype |
|----------|------|-----------|------------|--------|-----------|
| 10155 | a scale-mail vest of the drake | armor (9) | body(262144)\|take | 11 | tank |
| 10156 | the drake's ember-fang | weapon (5) | hold(32768)\|take | 6 | melee; ITEM_FIST; value3=5 (claw) |
| 10157 | a crest of desert fire | armor (9) | horns(4)\|take | 6 | melee |

Note for 10156: extra_flags = 201326784 | ITEM_FIST(1073741824) = 1275068608

**Sand Sovereign (mob 10131) — gear vnums 10158–10161 (4 items, final boss):**

| Obj Vnum | Name | item_type | wear_flags | weight | Archetype |
|----------|------|-----------|------------|--------|-----------|
| 10158 | the Sand Sovereign's great scepter | weapon (5) | hold(32768)\|take | 1 | caster; ITEM_TWO_HANDED; value3=7 (pound) |
| 10159 | robes of the eternal sovereign | armor (9) | body(262144)\|take | 1 | caster |
| 10160 | the sovereign's amulet of dominion | armor (9) | neck(128)\|take | 1 | caster |
| 10161 | sandals of the desert throne | armor (9) | feet(2097152)\|take | 6 | melee |

Note for 10158: extra_flags = 201326784 | ITEM_TWO_HANDED(2147483648) = 2348810432

---

### Trash Equipment (Levels 105–110, ITEM_NODROP only unless loot-table)

Reset gear extra_flags: `ITEM_NODROP(128)` = **128**
Loot-table gear extra_flags: `ITEM_NODROP(128) | ITEM_LOOT(67108864)` = **67108992**

All items include `take(8388608)` in wear_flags.

| Obj Vnum | Name | item_type | wear_flags | weight | Level | Delivery | Source |
|----------|------|-----------|------------|--------|-------|----------|--------|
| 10162 | a skeleton warrior's blade | weapon (5) | hold\|take | 6 | L 107 | Reset (E) | skeleton warrior |
| 10163 | a mummy's wrapping sash | armor (9) | waist\|take | 11 | L 108 | Loot | cursed mummy |
| 10164 | a scorpion-claw gauntlet | armor (9) | hands\|take | 11 | L 110 | Loot | giant desert scorpion |
| 10165 | a ghostly tomb shroud | armor (9) | about\|take | 1 | L 106 | Loot | tomb robber's ghost |
| 10166 | a jackal-hide belt | armor (9) | waist\|take | 6 | L 112 | Loot | sand jackal |
| 10167 | granite guardian fist | weapon (5) | hold\|take | 11 | L 115 | Reset (E) | granite guardian statue; ITEM_FIST; value3=0 |
| 10168 | canopic horror mandible | weapon (5) | hold\|take | 6 | L 109 | Loot | canopic horror; value3=10 (bite) |
| 10169 | an asp-fang earring | armor (9) | ear\|take | 1 | L 113 | Loot | golden asp |
| 10170 | desert hawk wing-cloak | armor (9) | about\|take | 1 | L 116 | Loot | desert hawk |
| 10171 | a sand golem's core shard | armor (9) | hold\|take | 11 | L 110 | Loot | sand golem; shield |
| 10172 | acolyte's cursed headwrap | armor (9) | head\|take | 1 | L 118 | Reset (E) | cursed acolyte |
| 10173 | desert wight's bone ring | armor (9) | finger\|take | 1 | L 114 | Loot | desert wight |
| 10174 | scarab guardian shell plate | armor (9) | body\|take | 11 | L 117 | Reset (E) | stone scarab guardian |
| 10175 | a cobra-scale bracer | armor (9) | wrist\|take | 11 | L 111 | Loot | pyramid cobra |
| 10176 | a mummified archer's bow wrap | armor (9) | arms\|take | 6 | L 119 | Loot | mummified archer |
| 10177 | vulture-feather mantle | armor (9) | shoulders\|take | 6 | L 120 | Loot | great desert vulture |
| 10178 | face guardian stone buckler | armor (9) | hold\|take | 11 | L 116 | Reset (E) | ancient face guardian; ITEM_BUCKLER |
| 10179 | desert shadow cowl | armor (9) | head\|take | 1 | L 112 | Loot | desert shadow |
| 10180 | an assassin's sand-silk hood | armor (9) | head\|take | 1 | L 118 | Loot | desert shadow assassin |
| 10181 | a pillar golem's knuckle-spike | weapon (5) | hold\|take | 11 | L 120 | Reset (E) | living pillar golem; ITEM_FIST; value3=0 |
| 10182 | sacred cobra fang pendant | armor (9) | neck\|take | 1 | L 115 | Loot | sacred golden cobra |
| 10183 | cultist's obsidian dagger | weapon (5) | hold\|take | 6 | L 117 | Loot | pyramid cultist; value3=2 (stab) |
| 10184 | death hawk talons | weapon (5) | hold\|take | 6 | L 119 | Loot | death hawk; value3=5 (claw) |
| 10185 | sand revenant's burial ring | armor (9) | finger\|take | 1 | L 113 | Loot | sand revenant |
| 10186 | spectre-touched robes | armor (9) | body\|take | 1 | L 116 | Reset (E) | pyramid specter |
| 10187 | cursed pharaoh shade's halo | armor (9) | halo\|take | 1 | L 120 | Reset (E) | cursed pharaoh's shade |
| 10188 | desert titan's stone bracer | armor (9) | wrist\|take | 11 | L 122 | Reset (E) | desert stone titan |
| 10189 | a death knight's greatsword | weapon (5) | hold\|take | 6 | L 121 | Reset (E) | pyramid death knight; ITEM_TWO_HANDED; value3=3 (slash) |
| 10190 | sand drake scale boots | armor (9) | feet\|take | 11 | L 120 | Reset (E) | sand drake |
| 10191 | a sphinx-carved shoulder plate | armor (9) | shoulders\|take | 11 | L 122 | Reset (E) | golden sphinx |
| 10192 | scarab swarm chitin ring | armor (9) | finger\|take | 1 | L 105 | Loot | sand scarab swarm |
| 10193 | a tomb fly carapace brooch | armor (9) | finger\|take | 1 | L 105 | Loot | tomb fly swarm |
| 10194 | a desiccated rat-tail lash | weapon (5) | hold\|take | 6 | L 105 | Loot | desiccated desert rat; value3=1 (whip) |
| 10195 | bone crawler chitin pauldrons | armor (9) | shoulders\|take | 11 | L 107 | Loot | bone crawler |
| 10196 | a ghoul's tattered girdle | armor (9) | waist\|take | 11 | L 109 | Loot | sand-choked ghoul |
| 10197 | an animated obelisk shard buckler | armor (9) | hold\|take | 11 | L 112 | Loot | animated obelisk shard; ITEM_BUCKLER |
| 10198 | a cursed builder's stone trowel | weapon (5) | hold\|take | 6 | L 110 | Loot | cursed pyramid builder; value3=7 (pound) |
| 10199 | a desert sand adder fang | armor (9) | ear\|take | 1 | L 108 | Loot | desert sand adder |
| 10200 | an ancient sentinel's faceplate | armor (9) | face\|take | 11 | L 114 | Reset (E) | ancient sentinel construct |
| 10201 | a hieroglyph guardian stone slab | armor (9) | hold\|take | 11 | L 115 | Loot | hieroglyph guardian; ITEM_BUCKLER |
| 10202 | an animated canopic jar lid | armor (9) | head\|take | 1 | L 112 | Loot | animated canopic jar |
| 10203 | a scribe's cursed inkpot | armor (9) | hold\|take | 1 | L 116 | Loot | scribal revenant |
| 10204 | linen wraps of the mummy servant | armor (9) | wrist\|take | 11 | L 113 | Loot | sand-wrapped mummy servant |
| 10205 | a high priest thrall's robe | armor (9) | body\|take | 1 | L 118 | Reset (E) | high priest's thrall |
| 10206 | a sand wraith's wisp veil | armor (9) | about\|take | 1 | L 117 | Loot | sand wraith |
| 10207 | a curse-bound warrior's iron pauldron | armor (9) | shoulders\|take | 11 | L 119 | Loot | curse-bound warrior |
| 10208 | an entombed warlord's war crown | armor (9) | head\|take | 11 | L 118 | Reset (E) | entombed warlord |
| 10209 | a flesh golem's riveted knuckle | weapon (5) | hold\|take | 11 | L 120 | Loot | flesh golem; ITEM_FIST; value3=0; extra_flags = NODROP+LOOT+FIST = 1140850816 |
| 10210 | a tomb stalker's shadow wrapping | armor (9) | about\|take | 1 | L 116 | Loot | tomb stalker |
| 10211 | ember lizard scale vest | armor (9) | body\|take | 11 | L 117 | Loot | ember lizard |
| 10212 | charred husk smoldering bracers | armor (9) | wrist\|take | 11 | L 119 | Loot | charred husk |
| 10213 | an apex sentinel's stone gauntlets | armor (9) | hands\|take | 11 | L 120 | Reset (E) | apex sentinel |
| 10214 | a gilded warrior shade's helm | armor (9) | head\|take | 1 | L 118 | Loot | gilded warrior shade |
| 10215 | a sandstorm elemental's howling band | armor (9) | wrist\|take | 1 | L 121 | Reset (E) | sandstorm elemental |
| 10216 | an obsidian guardian's plate arm | armor (9) | arms\|take | 11 | L 122 | Reset (E) | ancient obsidian guardian |
| 10217 | a mummy overlord's burial circlet | armor (9) | head\|take | 11 | L 120 | Reset (E) | greater mummy overlord |

**Note:** Object vnums 10162–10217 overlap with mob vnums in the same range. Cross-type overlap is permitted; each is unique within its type index.

---

## Resets Plan

### Boss Mob Resets

```
M 0 10125 1 10155  Crypt Pharaoh in Pharaoh's Tomb Chamber
M 0 10126 1 10200  Desert Gate Guardian in Pyramid's Heart Chamber
M 0 10127 1 10235  Scarab Colossus in Scarab Colossus Chamber
M 0 10128 1 10265  High Priest in High Priest's Sanctum
M 0 10129 1 10290  Eternal Mummy Lord in Eternal Vault Chamber
M 0 10130 1 10310  Pyramid Drake in Pyramid Drake's Nest
M 0 10131 1 10324  Sand Sovereign in Summit Throne Room
```

### Boss Loot Tables (l/L extensions on boss mobs)

Each boss carries its piece fragment plus 3 gear items (Sand Sovereign: piece + 4 gear).
`loot_amount`: Set based on guaranteed drops. Each boss uses a dedicated loot slot per item.

Example for Crypt Pharaoh (mob 10125):
```
l 400 10125 10140 10141 10142 0 0 0 0 0 0
L 25 25 25 25 0 0 0 0 0
```
This guarantees 4 drops: the piece fragment (10125 object), death mask (10140), shadow cloak (10141), burial wrappings (10142), each with equal 25% selection weight. The Sand Sovereign is structured similarly with 5 slots.

### Strong Mob Resets (limit 2–3 each)

```
M 0 10132 3 10192  cursed pharaoh's shade (multiple rooms)
M 0 10132 3 10173  cursed pharaoh's shade (another spawn)
M 0 10133 2 10170  desert stone titan
M 0 10133 2 10204  desert stone titan
M 0 10134 2 10244  pyramid death knight
M 0 10134 2 10262  pyramid death knight
M 0 10135 2 10282  sand drake
M 0 10135 2 10293  sand drake
M 0 10136 2 10313  golden sphinx
M 0 10136 2 10319  golden sphinx
M 0 10185 2 10170  sandstorm elemental
M 0 10185 2 10203  sandstorm elemental
M 0 10186 2 10249  ancient obsidian guardian
M 0 10186 2 10270  ancient obsidian guardian
M 0 10187 2 10282  greater mummy overlord
M 0 10187 2 10296  greater mummy overlord
```

### Trash Mob Resets (representative sample, ~60–80 total resets)

Distributed across zones by level:
- **Undercroft (10126–10154):** Skeleton warriors, cursed mummies, sand scarab swarms, giant scorpions, tomb robber's ghosts, canopic horrors, tomb fly swarms, desiccated desert rats, bone crawlers, sand-choked ghouls (~18 resets)
- **Ground Level (10156–10199):** Sand jackals, tomb robber's ghosts, granite guardian statues, canopic horrors, animated obelisk shards, cursed pyramid builders, desert sand adders, ancient sentinel constructs, sand-choked ghouls (~28 resets)
- **Floor 1 (10201–10234):** Golden asps, desert hawks, sand golems, granite statues, cursed pyramid builders, ancient sentinel constructs, hieroglyph guardians, animated canopic jars, scribal revenants, sand-wrapped mummy servants (~20 resets)
- **Floor 2 (10236–10264):** Cursed acolytes, desert wights, stone scarab guardians, pyramid cobras, animated canopic jars, sand-wrapped mummy servants, high priest thralls, sand wraiths, curse-bound warriors (~16 resets)
- **Floor 3 (10266–10289):** Mummified archers, great desert vultures, ancient face guardians, desert shadows, shadow assassins, sand wraiths, curse-bound warriors, entombed warlords, flesh golems, tomb stalkers (~14 resets)
- **Floor 4 (10291–10309):** Living pillar golems, sacred golden cobras, pyramid cultists, death hawks, sand revenants, flesh golems, ember lizards, charred husks (~12 resets)
- **Floor 5 (10311–10323):** Pyramid specters, apex sentinels, gilded warrior shades, death knights (strong), cursed pharaoh shades (strong) (~8 resets)

### Equipment Resets (E/G after mob M)

Each trash mob is followed by one `E` reset for their associated reset gear (items delivered via reset do not need ITEM_LOOT). Boss mobs have their gear delivered through loot tables only (no E resets on bosses — loot tables handle all boss drops).

### Object Resets (O commands, items in rooms)

```
O 0 10138 1 10148  Trapdoor key in Forgotten Workshop (Undercroft)
```

### Door Resets (D commands)

Per the Doors Summary table above:
```
D 0 8879  1 1   ancient road east (Northern Oasis side, closed)
D 0 10166 0 1   stone gate (closed)
D 0 10167 2 1   stone gate reverse (closed)
D 0 10177 5 2   heavy trapdoor (locked)
D 0 10126 4 2   heavy trapdoor reverse (locked)
D 0 10197 0 1   ancient stone door (closed)
D 0 10198 2 1   ancient stone door reverse (closed)
D 0 10234 0 1   stone scarab door (closed)
D 0 10235 2 1   stone scarab door reverse (closed)
D 0 10260 4 1   iron-banded archway (closed)
D 0 10266 5 1   iron-banded archway reverse (closed)
D 0 10264 0 1   carved stone threshold (closed)
D 0 10265 2 1   carved stone threshold reverse (closed)
D 0 10289 0 1   glowing stone arch (closed)
D 0 10290 2 1   glowing stone arch reverse (closed)
D 0 10304 4 2   stone sentinel gate (locked)
D 0 10311 5 2   stone sentinel gate reverse (locked)
D 0 10309 0 1   bone-adorned gate (closed)
D 0 10310 2 1   bone-adorned gate reverse (closed)
D 0 10323 0 1   summit chamber door (closed)
D 0 10324 2 1   summit chamber door reverse (closed)
D 0 10154 0 1   root-sealed vault door (closed)
D 0 10155 2 1   root-sealed vault door reverse (closed)
```

---

## Specials

```
#SPECIALS
M 10125 spec_cast_undead    Crypt Pharaoh
M 10126 spec_cast_mage      Desert Gate Guardian
M 10127 spec_cast_mage      Scarab Colossus
M 10128 spec_cast_bigtime   High Priest
M 10129 spec_cast_undead    Eternal Mummy Lord
M 10130 spec_breath_fire    Pyramid Drake
M 10131 spec_cast_bigtime   Sand Sovereign
M 10140 spec_poison         giant desert scorpion
M 10141 spec_cast_undead    tomb robber's ghost (cast flare/shock_grasp)
M 10145 spec_poison         golden asp
M 10148 spec_cast_mage      cursed acolyte
M 10149 spec_cast_undead    desert wight
M 10151 spec_poison         pyramid cobra
M 10152 spec_cast_undead    mummified archer (undead caster)
M 10155 spec_cast_undead    desert shadow
M 10158 spec_poison         sacred golden cobra
M 10159 spec_cast_mage      pyramid cultist
M 10162 spec_cast_undead    pyramid specter
M 10163 spec_disease        tomb fly swarm
M 10164 spec_disease        desiccated desert rat
M 10169 spec_poison         desert sand adder
M 10172 spec_cast_undead    animated canopic jar
M 10173 spec_cast_mage      scribal revenant
M 10175 spec_cast_mage      high priest's thrall
M 10176 spec_cast_undead    sand wraith
M 10185 spec_cast_mage      sandstorm elemental
M 10187 spec_cast_undead    greater mummy overlord
S
```

---

## Item Flag Reference

| Category | Item flag composition | Numeric value |
|----------|-----------------------|---------------|
| Boss piece fragments (loot) | NODROP+BOSS+MAGIC+LOOT | 201326784 |
| Assembly pieces (created) | NODROP+BOSS+MAGIC | 134217920 |
| Final Crown | RARE+NODROP+BOSS+MAGIC | 135266496 |
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

- **Trap Room (10125):** One-way entrance only from rooms 10178, 10220, 10250, 10277. Single D5 (down) exit to 10134. No exit back to any source room.
- **Undercroft to Ground:** D4 (up) from 10126 to 10177 (locked trapdoor, bidirectional, both sides locked via resets).
- **Ground to Floor 1:** Two parallel D4 (up) exits: 10184→10201 and 10185→10202. No door on either.
- **Floor 1 to Floor 2:** Two parallel D4 (up) exits: 10215→10236 and 10216→10237. No door.
- **Floor 2 to Floor 3:** Single D4 (up) 10260→10266, through `^iron-banded archway` (closed door).
- **Floor 3 to Floor 4:** Single D4 (up) 10278→10291. No door.
- **Floor 4 to Floor 5:** Single D4 (up) 10304→10311, through `^stone sentinel gate` (locked door, key from Pyramid Drake loot).
- No directional loops exist outside ROOM_MAZE-flagged rooms. All paths branch and terminate; none cycle back in the same direction.

---

## Implementation Checklist

1. [ ] Create branch `claude/northern-pyramid-area-design-TnN2N`
2. [ ] Create `area/northern_pyramid.are` with `#AREA` header and `#$`
3. [ ] Write `#MOBILES` section — 7 bosses (10125–10131) + 8 strong mobs (10132–10136, 10185–10187) + 48 trash (10137–10184)
4. [ ] Write `#OBJECTS` section — 13 piece chain (10125–10137) + 2 keys (10138–10139) + 22 boss gear (10140–10161) + 56 trash gear (10162–10217)
5. [ ] Write `#ROOMS` section — 200 rooms (10125–10324), all vnums used, all descriptions unique with 3+ sentences (5+ for important rooms), named exits mentioned in room text
6. [ ] Write `#RESETS` section — boss M resets, loot table structure, strong/trash M resets, E equipment resets, O object reset (key in 10148), D door resets
7. [ ] Write `#SPECIALS` section
8. [ ] Write `#OBJFUNS` section if any item special functions are desired
9. [ ] Add `northern_pyramid.are` to `area/area.lst`
10. [ ] Edit `the_northern_oasis.are` room 8879 — add D1 (east) exit to 10156 and update room description to mention the pyramid on the eastern horizon
11. [ ] Edit `the_northern_oasis.are` room 8879 — add `D 8879 1 1` to `#RESETS` to set the east door to closed state
12. [ ] Verify all vnums are within V 10125 10324 range
13. [ ] Verify no vnum conflicts with existing areas (10125–10324 is confirmed free)
14. [ ] Verify all boss rooms have `no_mob` flag
15. [ ] Verify trap room (10125) has `no_mob`, `no_recall`, `no_teleport`
16. [ ] Verify all trap source rooms (10178, 10220, 10250, 10277) have one-way exits to 10125 with no reciprocal exit
17. [ ] Verify piece item chain `value0/value1/value2` correctness before and after each combine step
18. [ ] Verify locked doors each have corresponding key object in `#OBJECTS`
19. [ ] Verify sum of all `L` loot_chance values per mob is ≤ 100
20. [ ] Run `cd src && make unit-tests` to validate
