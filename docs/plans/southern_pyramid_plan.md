# Area Plan: The Southern Pyramid

## Overview

**Area Name:** `@@ySouthern @@bPyramid@@N`
**File Name:** `southern_pyramid.are`
**Keyword:** `southernpyramid`
**Level Range:** 120–140 (I directive: `I 120 140`)
**Level Label:** `@@W(@@y120 140@@W)@@N`
**Vnum Range:** 3139–3338 (V directive: `V 3139 3338`)
**Vnum Count:** 35
**Area Number:** `N 76`
**Reset Rate:** `F 20`
**Reset Message:** `@@yancient sand cascades through the Southern Pyramid as sealed chambers exhale stale air@@N`

### Color Theme

- **Primary:** `@@y` (yellow) — sun-scorched sandstone, golden sarcophagi, amber torch-glow
- **Secondary:** `@@b` (brown) — cracked clay walls, petrified timber beams, dried mud mortar
- **Accent:** `@@G` (green) — jade inlay, verdigris-stained copper, poisonous lichen
- **Danger:** `@@R` (red) — cursed blood sigils, volcanic vents, boss threat indicators
- **Text reset:** `@@N` after all color sequences

The Southern Pyramid is older and deeper than its northern twin. Where the Northern Pyramid rises into the sky, the Southern Pyramid burrows downward — its visible capstone is merely the entrance to a vast inverted structure descending into the earth. The deeper one goes, the hotter the air grows, until the lowest chambers border on volcanic vents. The architect of this pyramid was the Moon Sovereign, rival and twin of the Sand Sovereign, who built downward to be closer to the underworld and the source of all death magic.

---


## Lore
See `southern_pyramid_lore.md` for extracted lore.

## Area Header Notes

- `Q 16` — mandatory version
- `K southernpyramid~` — single-word keyword, no spaces, no color codes
- `L @@W(@@y120 140@@W)@@N~` — white parens, yellow level numbers, white close paren, reset
- `N 76` — confirmed unused across all loaded area files
- `I 120 140` — area-gating and mob-level policy
- `V 3139 3338` — full 35-vnum envelope; all rooms, mobs, and objects stay within this range
- `F 20` — 20-minute reset frequency
- `U` — reset message as above; no double-newlines, terminated with `~`
- `O Virant~` — area owner must always be Virant
- `T` — teleport flag line (rest ignored by loader)

---

## Level Structure

Seven descending tiers plus a trap room. Unlike the Northern Pyramid which ascends, this pyramid descends — players go **down** through the tiers. The capstone (ground level) is the entrance; each subsequent tier is reached by going down.

| Level | Name | Vnums | Rooms | Boss Room |
|-------|------|-------|-------|-----------|
| Trap Room | The Crushing Hall | 3139 | 1 | — (no_mob) |
| Ground Level / Capstone | The Moon Gate | 3140–3184 | 45 | 3184 |
| Tier 1 (Silence) | The Silent Crypts | 3185–3219 | 35 | 3219 |
| Tier 2 (Decay) | The Rotting Galleries | 3220–3249 | 30 | 3249 |
| Tier 3 (Shadow) | The Shadow Nave | 3250–3274 | 25 | 3274 |
| Tier 4 (Bone) | The Ossuary Depths | 3275–3294 | 20 | 3294 |
| Tier 5 (Venom) | The Venomous Cisterns | 3295–3314 | 20 | 3314 |
| Tier 6 (Fire) | The Molten Sanctum | 3315–3328 | 14 | 3328 |
| Tier 7 / Nadir | The Void Throne | 3329–3338 | 10 | 3338 |

**Total rooms: 35** (all vnums 3139–3338 used)

---

## Connection to the Great Oasis (Southern Basin Route)

To match current setting canon, the Southern Pyramid is reached through the **Great Oasis southern basin corridor** (historically associated with the Twin Basin Compact), not as isolated "Southern Oasis" lore. The practical room linkage remains unchanged while terminology and narrative framing are updated.

The Southern Pyramid connects bidirectionally via **room 2558** ("Reedbound Span of Lantern"), the southbound charter road emerging from the Great Oasis-linked basin settlements. This preserves existing movement flow while aligning with Great Oasis/Eastern Desert continuity.

| Direction | From | To | Notes |
|-----------|------|----|-------|
| south (D2) | 2558 (Reedbound Span of Lantern, Great Oasis southern basin route) | 3140 (Before the Southern Pyramid) | Players leave the oasis corridor heading south |
| north (D0) | 3140 (Before the Southern Pyramid) | 2558 (Reedbound Span of Lantern, Great Oasis southern basin route) | Return path to the basin road network |

### Required Edit to oasis connector area

Room 2558 currently has exits D0→2548, D3→2557. Add a D2 (south) exit:

```
D2
The charter road continues south through the dunes, where a @@bsandstone@@N capstone rises above the heat-haze.

~
^charter road south~
0 -1 3140
```

Room 2558's description should explicitly reference basin traffic from the Great Oasis and occasional Midgaard archive expeditions hiring local escorts before descending toward the Moon Sovereign's domain.

Room 3140 (the pyramid entry room) receives a D0 (north) exit back to 2558:

```
D0
The charter road bends north through the dunes, returning toward the Great Oasis basin lights and water courts.

~
~
0 -1 2558
```

---

## Lore Integration Requirements (Great Oasis / Eastern Desert / Midgaard)
These requirements expand implementation-facing lore guidance so builders can keep the Southern Pyramid consistent with regional canon while preserving existing mechanical and vnum structure.

### Canon Positioning

- Treat the Southern Pyramid as the **Moon Sovereign counter-monument** to northern ascendant doctrine.
- Preserve the Great Oasis basin linkage through charter-road language and Twin Basin legacy references on approach routes.
- Frame descent as jurisdictional progression (tiered death functions), not merely depth-based danger scaling.

### Great Oasis Basin Alignment Rules

- Surface and connector text should reference basin-era permits, witness phrases, ration contracts, and caravan legal rituals.
- Route flavor should suggest that expeditions are still organized through inherited oasis logistics, even in partial institutional collapse.
- Keep naming and signage coherent with charter-road terminology used in connector exits and door/reset labels.

### Eastern Desert Alignment Rules

- Reinforce continuity with eclipse-era failures, synod schisms, and repurposed funerary rites.
- Tier text should acknowledge that many southern ritual forms were later copied (often incorrectly) by desert factions.
- Use contradiction intentionally: inscriptions may conflict because they represent doctrinal compromise attempts rather than unified theology.

### Midgaard Alignment Rules

- Include evidence of archive extraction culture: incomplete translation notes, catalog stamps, missing fragment sockets, expedition graffiti in scholarly shorthand.
- Preserve uncertainty: Midgaard records can be useful but should not flatten tier-specific meanings into generic summaries.
- Use western scholarship as tension, not truth-source supremacy.

### Tone and Content Guardrails

- Keep Southern Pyramid voice **subterranean-jurisdictional**: sealed procedure, enforced silence, ritualized penalties, controlled descent.
- Emphasize that hazards are institutional tools (containment, correction, elimination), not random environmental accidents.
- Escalate from procedural dread in upper tiers to metaphysical sovereignty at the nadir.

---

## Room Layout (35 rooms)

### Room Flag Policy

- **`no_teleport`**: Applied to **all 35 rooms**. Players and mobs cannot be teleported into any room in the pyramid.
- **`no_recall`**: Applied to **all rooms except** inter-tier stairwell/transit rooms and the final boss room (3338). Stairwells and 3338 allow recall so players can escape between tiers and exit after defeating the final boss.

Inter-tier rooms that are **exempt from `no_recall`** (have only `no_mob, no_teleport, indoors`): 3140, 3161, 3168, 3185, 3186, 3199, 3220, 3221, 3238, 3250, 3251, 3265, 3275, 3276, 3290, 3295, 3309, 3315, 3329, 3338.

All other rooms (including boss rooms 3184, 3219, 3249, 3274, 3294, 3314, 3328) carry `no_recall`.

---

### Zone 0: The Crushing Hall (1 room: 3139)

This special room exists outside the pyramid's normal vertical progression. Hidden pressure plates, collapsing floors, and disguised pit covers scattered throughout the pyramid all funnel into this one chamber. It has **no exit except D4 (up) to 3198** in the Silent Crypts. Players who fall in must navigate the Silent Crypts to escape.

| Vnum | Name | Sector | Flags |
|------|------|--------|-------|
| 3139 | The Crushing Hall | inside | no_mob, indoors, no_recall, no_teleport |

**Exits:**
- D4 (up) → 3198 (The Trap-Fall Landing in Silent Crypts); no keyword; no door

**One-way trap exits leading IN to 3139 from across the pyramid (these rooms have exits pointing to 3139; 3139 has no reciprocal exit back to them):**

| Source Room | Vnum | Direction | Notes |
|-------------|------|-----------|-------|
| The Cracked Flagstone Hall (Ground Level) | 3162 | D5 (down) | Hidden pit, no keyword |
| The False Stairwell (Tier 1) | 3204 | D5 (down) | Appears to lead deeper but drops sideways |
| The Unstable Gallery (Tier 2) | 3234 | D5 (down) | Pressure plate collapse |
| The Treacherous Ledge (Tier 3) | 3261 | D5 (down) | False floor gives way |

Each source room description must describe the hazard (cracked floor, suspicious passage, etc.) without explicitly naming the trap destination, so players can discover it as a risk.

---

### Zone 1: The Moon Gate — Ground Level (45 rooms: 3140–3184)

The capstone structure visible above the desert and the first underground tier. Exterior rooms use `desert` sector; interior rooms use `inside` sector. This is the widest and most populated floor.

| Vnum | Room Name | Sector | Flags | Notes |
|------|-----------|--------|-------|-------|
| 3140 | Before the Southern Pyramid | desert | no_mob, no_teleport, indoors | **Entry from Great Oasis southern basin route** (D0 north → 2558) |
| 3141 | The Ancient Road South | desert | no_recall, no_teleport | Approach causeway |
| 3142 | The Outer Dune Ring | desert | no_recall, no_teleport | Sand dunes circle the capstone |
| 3143 | The Western Approach | desert | no_recall, no_teleport | Sheer weathered stone face |
| 3144 | The Eastern Approach | desert | no_recall, no_teleport | Hieroglyphs etched by wind |
| 3145 | The Capstone Forecourt | desert | no_recall, no_teleport | Wide plaza before the descending entrance |
| 3146 | The Obelisk Avenue | desert | no_recall, no_teleport | Broken obelisks line a sunken road |
| 3147 | The Sentinel Ruin | desert | no_recall, no_teleport | Collapsed guard station |
| 3148 | The Capstone Gate | desert | no_recall, no_teleport | Massive carved entrance arch; `^moon gate` (closed door, key -1) |
| 3149 | The Descending Vestibule | inside | no_recall, no_teleport, indoors | First interior room; stairs descend immediately |
| 3150 | The East Entry Corridor | inside | no_recall, no_teleport, indoors | |
| 3151 | The West Entry Corridor | inside | no_recall, no_teleport, indoors | |
| 3152 | The Pillar Hall | inside | no_recall, no_teleport, indoors | Thick columns support the capstone ceiling |
| 3153 | A Rubble-Choked Wing | inside | no_recall, no_teleport, indoors | Ceiling partially collapsed |
| 3154 | The Offering Alcove | inside | no_recall, no_teleport, indoors | Carved niches for tribute |
| 3155 | The Moon Inscription Gallery | inside | no_recall, no_teleport, indoors | Lunar cycle carved into every wall |
| 3156 | A Guard Barracks Ruin | inside | no_recall, no_teleport, indoors, dark | |
| 3157 | The Sealed Vault Wing | inside | no_recall, no_teleport, indoors | Empty storage chambers |
| 3158 | The East Wing Passage | inside | no_recall, no_teleport, indoors | |
| 3159 | The West Wing Passage | inside | no_recall, no_teleport, indoors | |
| 3160 | The Cartographer's Chamber | inside | no_recall, no_teleport, indoors | Floor mosaic shows all seven tiers |
| 3161 | The East Descent Landing | inside | no_mob, no_teleport, indoors | D5 (down) → Tier 1 entry (3185) |
| 3162 | The Cracked Flagstone Hall | inside | no_recall, no_teleport, indoors | Contains trap exit D5 → 3139 (hidden pit, one-way) |
| 3163 | A Side Alcove | inside | no_recall, no_teleport, indoors, dark | |
| 3164 | The Central Nave | inside | no_recall, no_teleport, indoors | Wide hub room, stairs descend from here |
| 3165 | The Moon Altar Chamber | inside | no_recall, no_teleport, indoors | Crescent-shaped obsidian altar |
| 3166 | A Collapsed Side Passage | inside | no_recall, no_teleport, indoors, dark | |
| 3167 | The Inner Vestibule | inside | no_recall, no_teleport, indoors | |
| 3168 | The West Descent Landing | inside | no_mob, no_teleport, indoors | Alternate D5 (down) → 3186 |
| 3169 | The Tomb Robber's Camp | inside | no_recall, no_teleport, indoors | Abandoned supplies, cold fire pit |
| 3170 | A Debris-Filled Corridor | inside | no_recall, no_teleport, indoors, dark | |
| 3171 | The North Inner Hall | inside | no_recall, no_teleport, indoors | |
| 3172 | The South Inner Hall | inside | no_recall, no_teleport, indoors | |
| 3173 | A Ceremonial Niche | inside | no_recall, no_teleport, indoors | |
| 3174 | The Root-Cracked Vault | inside | no_recall, no_teleport, indoors, dark | Ancient roots push through the ceiling |
| 3175 | The Lunar Throne Antechamber | inside | no_recall, no_teleport, indoors | Approach to ground-floor boss zone |
| 3176 | The Lower Throne Room | inside | no_recall, no_teleport, indoors | Grand dais, cracked obsidian chair |
| 3177 | The Sanctum Approach | inside | no_recall, no_teleport, indoors | |
| 3178 | A Preparation Chamber | inside | no_recall, no_teleport, indoors, dark | |
| 3179 | The Sentinel's Passage | inside | no_recall, no_teleport, indoors | |
| 3180 | The Inner Sanctum Entrance | inside | no_recall, no_teleport, indoors | `^obsidian door` (closed, no lock) leads to 3181 |
| 3181 | The Waiting Hall | inside | no_recall, no_teleport, indoors | |
| 3182 | The Final Approach | inside | no_recall, no_teleport, indoors | |
| 3183 | The Moon Guardian's Antechamber | inside | no_recall, no_teleport, indoors | |
| 3184 | The Moon Gate Chamber | inside | no_mob, no_recall, no_teleport, indoors | **BOSS ROOM** — The Moon Gate Guardian |

---

### Zone 2: The Silent Crypts — Tier 1 (35 rooms: 3185–3219)

First descending tier. Consecrated to silence — sounds are muffled, dust hangs motionless in the air. All rooms `inside` sector. Most rooms flagged `dark`.

| Vnum | Room Name | Sector | Flags | Notes |
|------|-----------|--------|-------|-------|
| 3185 | The First Descent | inside | no_mob, no_teleport, indoors | Entry from ground D5 (3161 ↓) |
| 3186 | The Lower Landing | inside | no_mob, no_teleport, indoors | Entry from alternate ground D5 (3168 ↓) |
| 3187 | The Silent Hall | inside | no_recall, no_teleport, indoors | Sound dies within paces |
| 3188 | The Dust-Choked Corridor | inside | no_recall, no_teleport, indoors, dark | Undisturbed dust inches thick |
| 3189 | A Sealed Burial Niche | inside | no_recall, no_teleport, indoors, dark | Stone slab seals a corpse-alcove |
| 3190 | The Whispering Gallery | inside | no_recall, no_teleport, indoors | Faintest sounds echo strangely |
| 3191 | A Collapsed Crypt Wing | inside | no_recall, no_teleport, indoors, dark | Rubble blocks the far end |
| 3192 | The Embalmer's Workshop | inside | no_recall, no_teleport, indoors | Canopic jars line stone shelves |
| 3193 | A Stone-Floored Passage | inside | no_recall, no_teleport, indoors, dark | |
| 3194 | The Sarcophagus Row | inside | no_recall, no_teleport, indoors | Sealed stone coffins, four abreast |
| 3195 | The Prayer Alcove | inside | no_recall, no_teleport, indoors | Defaced altar with lunar carvings |
| 3196 | A Narrow Root Tunnel | inside | no_recall, no_teleport, indoors, dark | |
| 3197 | The Ossuary Antechamber | inside | no_recall, no_teleport, indoors | Bones stacked in neat rows |
| 3198 | The Trap-Fall Landing | inside | no_recall, no_teleport, indoors, dark | Landing point from the Crushing Hall (3139 ↓ arrives here) |
| 3199 | The Crypt Crossroads | inside | no_mob, no_teleport, indoors | Central hub; four-way junction |
| 3200 | A Flooded Burial Chamber | inside | no_recall, no_teleport, indoors, dark | Ankle-deep stagnant water |
| 3201 | The Fungal Alcove | inside | no_recall, no_teleport, indoors, dark | Pale fungi glow faintly |
| 3202 | The Inner Crypt Hall | inside | no_recall, no_teleport, indoors | Grand chamber, cracked pillars |
| 3203 | A Burial Preparation Room | inside | no_recall, no_teleport, indoors | |
| 3204 | The False Stairwell | inside | no_recall, no_teleport, indoors | Contains trap exit D5 → 3139 (appears to descend, one-way) |
| 3205 | The Deep Alcove | inside | no_recall, no_teleport, indoors, dark | |
| 3206 | A Sealed Side Chamber | inside | no_recall, no_teleport, indoors, dark | |
| 3207 | The Inscription Corridor | inside | no_recall, no_teleport, indoors | Warnings carved in ancient script |
| 3208 | The Ventilation Shaft Landing | inside | no_recall, no_teleport, indoors | |
| 3209 | A Guard Post Ruin | inside | no_recall, no_teleport, indoors, dark | |
| 3210 | The Silent Shrine | inside | no_recall, no_teleport, indoors | Central shrine, untouched |
| 3211 | The East Crypt Passage | inside | no_recall, no_teleport, indoors | |
| 3212 | The West Crypt Passage | inside | no_recall, no_teleport, indoors | |
| 3213 | A Crumbling Side Passage | inside | no_recall, no_teleport, indoors, dark | |
| 3214 | The Inner Vault | inside | no_recall, no_teleport, indoors | |
| 3215 | The Warden's Approach | inside | no_recall, no_teleport, indoors | |
| 3216 | The Outer Warden Chamber | inside | no_recall, no_teleport, indoors | |
| 3217 | A Final Corridor | inside | no_recall, no_teleport, indoors, dark | |
| 3218 | The Silent Warden's Approach | inside | no_recall, no_teleport, indoors | `^glyph-sealed door` (closed, no lock) leads to 3219 |
| 3219 | The Silent Warden's Tomb | inside | no_mob, no_recall, no_teleport, indoors | **BOSS ROOM** — The Silent Warden |

---

### Zone 3: The Rotting Galleries — Tier 2 (30 rooms: 3220–3249)

Second descending tier. Consecrated to decay — walls seep moisture, fungal growth covers surfaces, the air is thick and fetid. Slightly narrower than Tier 1.

| Vnum | Room Name | Sector | Flags | Notes |
|------|-----------|--------|-------|-------|
| 3220 | The Second Descent | inside | no_mob, no_teleport, indoors | Entry from Tier 1 D5 (3199 ↓, through `^corroded iron hatch`) |
| 3221 | The Decay Landing | inside | no_mob, no_teleport, indoors | Alternate entry from Tier 1 D5 (3211 ↓) |
| 3222 | The Rotting Hall | inside | no_recall, no_teleport, indoors | Walls slick with moisture |
| 3223 | A Mold-Covered Passage | inside | no_recall, no_teleport, indoors, dark | |
| 3224 | The Fungal Garden | inside | no_recall, no_teleport, indoors | Mushrooms tall as a man |
| 3225 | The Seeping Vault | inside | no_recall, no_teleport, indoors | Water trickles from every crack |
| 3226 | A Collapsed Drain Tunnel | inside | no_recall, no_teleport, indoors, dark | |
| 3227 | The Refuse Pit | inside | no_recall, no_teleport, indoors | Generations of discarded offerings |
| 3228 | The Inner Rot Gallery | inside | no_recall, no_teleport, indoors | Frescoes dissolving under mold |
| 3229 | A Slime-Filled Alcove | inside | no_recall, no_teleport, indoors, dark | |
| 3230 | The Putrid Pool Chamber | inside | no_recall, no_teleport, indoors | Stagnant green-black water |
| 3231 | The Crumbling Archive | inside | no_recall, no_teleport, indoors | Scrolls rotted to paste |
| 3232 | A Corroded Side Passage | inside | no_recall, no_teleport, indoors, dark | |
| 3233 | The Decay Shrine | inside | no_recall, no_teleport, indoors | Altar covered in thick moss |
| 3234 | The Unstable Gallery | inside | no_recall, no_teleport, indoors | Contains trap exit D5 → 3139 (pressure plate collapse, one-way) |
| 3235 | The East Rot Wing | inside | no_recall, no_teleport, indoors | |
| 3236 | The West Rot Wing | inside | no_recall, no_teleport, indoors | |
| 3237 | A Mushroom-Choked Corridor | inside | no_recall, no_teleport, indoors, dark | |
| 3238 | The Descent Shaft Landing | inside | no_mob, no_teleport, indoors | D5 (down) → Tier 3 (3250), through `^rusted grate` (closed, no lock) |
| 3239 | The Inner Decay Hall | inside | no_recall, no_teleport, indoors | |
| 3240 | A Flooded Crypt Alcove | inside | no_recall, no_teleport, indoors, dark | |
| 3241 | The Compost Chamber | inside | no_recall, no_teleport, indoors | Ancient food stores, long rotted |
| 3242 | The Verdigris Passage | inside | no_recall, no_teleport, indoors | Green copper stains every surface |
| 3243 | A Crumbling Vault | inside | no_recall, no_teleport, indoors, dark | |
| 3244 | The East Gallery End | inside | no_recall, no_teleport, indoors | |
| 3245 | The Outer Warden Chamber | inside | no_recall, no_teleport, indoors | |
| 3246 | The Pre-Chamber Corridor | inside | no_recall, no_teleport, indoors, dark | |
| 3247 | The Rot Warden's Antechamber | inside | no_recall, no_teleport, indoors | |
| 3248 | The Rot Warden's Approach | inside | no_recall, no_teleport, indoors | `^moss-covered stone door` (closed, no lock) leads to 3249 |
| 3249 | The Rot Warden's Chamber | inside | no_mob, no_recall, no_teleport, indoors | **BOSS ROOM** — The Rot Warden |

---

### Zone 4: The Shadow Nave — Tier 3 (25 rooms: 3250–3274)

Third descending tier. Consecrated to shadow — natural darkness that resists all light sources. Torches gutter, magical light dims. Nearly all rooms flagged `dark`.

| Vnum | Room Name | Sector | Flags | Notes |
|------|-----------|--------|-------|-------|
| 3250 | The Third Descent | inside | no_mob, no_teleport, indoors | Entry via `^rusted grate` from 3238 |
| 3251 | The Shadow Landing | inside | no_mob, no_teleport, indoors | Alternate entry from Tier 2 D5 (3235 ↓) |
| 3252 | The Shadow Hall | inside | no_recall, no_teleport, indoors, dark | Darkness presses like a weight |
| 3253 | A Lightless Corridor | inside | no_recall, no_teleport, indoors, dark | |
| 3254 | The Umbral Shrine | inside | no_recall, no_teleport, indoors, dark | Black altar absorbs all light |
| 3255 | The Eclipse Chamber | inside | no_recall, no_teleport, indoors, dark | Carved depictions of a sunless sky |
| 3256 | A Dark Side Passage | inside | no_recall, no_teleport, indoors, dark | |
| 3257 | The Shadowmancer's Study | inside | no_recall, no_teleport, indoors, dark | Crumbled tablets and spilled ink |
| 3258 | The Inner Shadow Hall | inside | no_recall, no_teleport, indoors, dark | |
| 3259 | A Pitch-Black Alcove | inside | no_recall, no_teleport, indoors, dark | |
| 3260 | The Blind Gallery | inside | no_recall, no_teleport, indoors, dark | Reliefs meant to be read by touch |
| 3261 | The Treacherous Ledge | inside | no_recall, no_teleport, indoors, dark | Contains trap exit D5 → 3139 (false floor, one-way) |
| 3262 | The East Shadow Wing | inside | no_recall, no_teleport, indoors, dark | |
| 3263 | The West Shadow Wing | inside | no_recall, no_teleport, indoors, dark | |
| 3264 | The Obsidian Colonnade | inside | no_recall, no_teleport, indoors, dark | Polished black stone columns |
| 3265 | The Descent Shaft | inside | no_mob, no_teleport, indoors | D5 (down) → Tier 4 (3275) |
| 3266 | A Shadow-Flooded Crypt | inside | no_recall, no_teleport, indoors, dark | |
| 3267 | The Darkened Archive | inside | no_recall, no_teleport, indoors, dark | |
| 3268 | The Warden's Outer Guard | inside | no_recall, no_teleport, indoors, dark | |
| 3269 | A Narrow Dark Passage | inside | no_recall, no_teleport, indoors, dark | |
| 3270 | The Pre-Chamber Hall | inside | no_recall, no_teleport, indoors, dark | |
| 3271 | The Inner Nave | inside | no_recall, no_teleport, indoors, dark | |
| 3272 | The Shadow Warden's Antechamber | inside | no_recall, no_teleport, indoors, dark | |
| 3273 | The Shadow Warden's Approach | inside | no_recall, no_teleport, indoors, dark | `^shadow-wreathed portal` (closed, no lock) leads to 3274 |
| 3274 | The Shadow Warden's Sanctum | inside | no_mob, no_recall, no_teleport, indoors | **BOSS ROOM** — The Shadow Warden |

---

### Zone 5: The Ossuary Depths — Tier 4 (20 rooms: 3275–3294)

Fourth descending tier. Consecrated to bone — walls built from interlocking skeletal remains, floors paved with compressed bone-meal. Narrow passages.

| Vnum | Room Name | Sector | Flags | Notes |
|------|-----------|--------|-------|-------|
| 3275 | The Fourth Descent | inside | no_mob, no_teleport, indoors | Entry from Tier 3 D5 (3265 ↓) |
| 3276 | The Bone Landing | inside | no_mob, no_teleport, indoors | Alternate entry from Tier 3 (3262 ↓) |
| 3277 | The Ossuary Hall | inside | no_recall, no_teleport, indoors | Walls of interlocking skulls |
| 3278 | A Bone-Paved Corridor | inside | no_recall, no_teleport, indoors, dark | |
| 3279 | The Skull Colonnade | inside | no_recall, no_teleport, indoors | Pillars of stacked skulls |
| 3280 | The Bone Throne Room | inside | no_recall, no_teleport, indoors | A throne of welded femurs |
| 3281 | A Rib-Vaulted Chamber | inside | no_recall, no_teleport, indoors, dark | Ceiling arches of giant ribs |
| 3282 | The Marrow Pool | inside | no_recall, no_teleport, indoors | Shallow pool of liquefied marrow |
| 3283 | The Inner Ossuary | inside | no_recall, no_teleport, indoors | |
| 3284 | A Bone-Dust Alcove | inside | no_recall, no_teleport, indoors, dark | |
| 3285 | The Calcification Chamber | inside | no_recall, no_teleport, indoors | Dripping mineral water encrusts everything |
| 3286 | The East Bone Wing | inside | no_recall, no_teleport, indoors | |
| 3287 | The West Bone Wing | inside | no_recall, no_teleport, indoors, dark | |
| 3288 | The Reliquary | inside | no_recall, no_teleport, indoors | Sacred bone fragments on stone shelves |
| 3289 | A Narrow Bone Tunnel | inside | no_recall, no_teleport, indoors, dark | |
| 3290 | The Deep Shaft Landing | inside | no_mob, no_teleport, indoors | D5 (down) → Tier 5 (3295), through `^bone-barred gate` (locked, key 3203) |
| 3291 | The Bone Warden's Outer Hall | inside | no_recall, no_teleport, indoors | |
| 3292 | The Bone Warden's Antechamber | inside | no_recall, no_teleport, indoors | |
| 3293 | The Bone Warden's Approach | inside | no_recall, no_teleport, indoors | `^skull-arch threshold` (closed, no lock) leads to 3294 |
| 3294 | The Bone Warden's Crypt | inside | no_mob, no_recall, no_teleport, indoors | **BOSS ROOM** — The Bone Warden |

---

### Zone 6: The Venomous Cisterns — Tier 5 (20 rooms: 3295–3314)

Fifth descending tier. Consecrated to venom — flooded with toxic pools, venomous creatures, and poisonous vapors. Green-tinged light from bioluminescent growths.

| Vnum | Room Name | Sector | Flags | Notes |
|------|-----------|--------|-------|-------|
| 3295 | The Fifth Descent | inside | no_mob, no_teleport, indoors | Entry via `^bone-barred gate` from 3290 |
| 3296 | The Cistern Landing | inside | no_recall, no_teleport, indoors | Toxic vapor rises from below |
| 3297 | The Venom Hall | inside | no_recall, no_teleport, indoors | Green-tinged walls, acrid air |
| 3298 | A Dripping Toxic Corridor | inside | no_recall, no_teleport, indoors, dark | |
| 3299 | The Greater Cistern | inside | no_recall, no_teleport, indoors | Vast pool of luminous green liquid |
| 3300 | The Serpent Shrine | inside | no_recall, no_teleport, indoors | Coiled-serpent carvings |
| 3301 | A Toxic Alcove | inside | no_recall, no_teleport, indoors, dark | |
| 3302 | The Alchemy Chamber | inside | no_recall, no_teleport, indoors | Cracked vials and corroded apparatus |
| 3303 | The Inner Cistern Passage | inside | no_recall, no_teleport, indoors | |
| 3304 | A Flooded Side Tunnel | inside | no_recall, no_teleport, indoors, dark | Knee-deep in toxic water |
| 3305 | The Fume Vent Chamber | inside | no_recall, no_teleport, indoors | Hot toxic gas seeps from cracks |
| 3306 | The East Cistern Wing | inside | no_recall, no_teleport, indoors | |
| 3307 | The West Cistern Wing | inside | no_recall, no_teleport, indoors, dark | |
| 3308 | The Venom Altar | inside | no_recall, no_teleport, indoors | Carved serpent coils around an obsidian basin |
| 3309 | The Descent Shaft | inside | no_mob, no_teleport, indoors | D5 (down) → Tier 6 (3315) |
| 3310 | The Venom Warden's Outer Hall | inside | no_recall, no_teleport, indoors | |
| 3311 | A Noxious Side Passage | inside | no_recall, no_teleport, indoors, dark | |
| 3312 | The Venom Warden's Antechamber | inside | no_recall, no_teleport, indoors | |
| 3313 | The Venom Warden's Approach | inside | no_recall, no_teleport, indoors | `^venom-slick gate` (closed, no lock) leads to 3314 |
| 3314 | The Venom Warden's Cistern | inside | no_mob, no_recall, no_teleport, indoors | **BOSS ROOM** — The Venom Warden |

---

### Zone 7: The Molten Sanctum — Tier 6 (14 rooms: 3315–3328)

Sixth descending tier. Consecrated to fire — volcanic vents heat the stone, lava seeps through cracks. The air shimmers with heat. Sector type `inside` throughout.

| Vnum | Room Name | Sector | Flags | Notes |
|------|-----------|--------|-------|-------|
| 3315 | The Sixth Descent | inside | no_mob, no_teleport, indoors | Entry from Tier 5 D5 (3309 ↓) |
| 3316 | The Scorched Corridor | inside | no_recall, no_teleport, indoors, hot | Walls radiate intense heat |
| 3317 | The Lava Seep Chamber | inside | no_recall, no_teleport, indoors, hot | Molten stone oozes from cracks |
| 3318 | A Basalt Passage | inside | no_recall, no_teleport, indoors, hot, dark | |
| 3319 | The Magma Pool Room | inside | no_recall, no_teleport, indoors, hot | Pool of glowing magma at center |
| 3320 | The Obsidian Gallery | inside | no_recall, no_teleport, indoors, hot | Walls of natural volcanic glass |
| 3321 | A Heat-Shimmering Alcove | inside | no_recall, no_teleport, indoors, hot, dark | |
| 3322 | The Forge of the Dead | inside | no_recall, no_teleport, indoors, hot | Ancient forge powered by volcanic vent |
| 3323 | The Inner Sanctum Passage | inside | no_recall, no_teleport, indoors, hot | |
| 3324 | The Ember Throne Room | inside | no_recall, no_teleport, indoors, hot | Charred obsidian throne |
| 3325 | The Fire Warden's Outer Hall | inside | no_recall, no_teleport, indoors, hot | |
| 3326 | The Fire Warden's Antechamber | inside | no_recall, no_teleport, indoors, hot | |
| 3327 | The Fire Warden's Approach | inside | no_recall, no_teleport, indoors, hot | `^molten-iron gate` (closed, no lock) leads to 3328 |
| 3328 | The Fire Warden's Crucible | inside | no_mob, no_recall, no_teleport, indoors, hot | **BOSS ROOM** — The Fire Warden |

---

### Zone 8: The Void Throne — Tier 7 / Nadir (10 rooms: 3329–3338)

The deepest and smallest tier. The nadir of the pyramid. The Moon Sovereign's final resting place. A chamber of volcanic glass suspended above a lake of molten stone. Sector type `inside`.

| Vnum | Room Name | Sector | Flags | Notes |
|------|-----------|--------|-------|-------|
| 3329 | The Final Descent | inside | no_mob, no_teleport, indoors | Entry from Tier 6 D5 (3323 ↓), through `^obsidian sentinel gate` (locked, key 3204) |
| 3330 | The Void Corridor | inside | no_recall, no_teleport, indoors, dark | Absolute silence, no heat despite proximity to magma |
| 3331 | The Glass Antechamber | inside | no_recall, no_teleport, indoors, dark | Walls of polished volcanic glass |
| 3332 | The Reflection Hall | inside | no_recall, no_teleport, indoors, dark | Infinite reflections in obsidian mirrors |
| 3333 | A Void-Touched Alcove | inside | no_recall, no_teleport, indoors, dark | |
| 3334 | The Inner Void Passage | inside | no_recall, no_teleport, indoors, dark | |
| 3335 | The Moon Sovereign's Antechamber | inside | no_recall, no_teleport, indoors, dark | Carved depictions of the Moon Sovereign's conquest |
| 3336 | The Pre-Throne Hall | inside | no_recall, no_teleport, indoors, dark | |
| 3337 | The Moon Sovereign's Approach | inside | no_recall, no_teleport, indoors, dark | `^void chamber door` (closed, no lock) leads to 3338 |
| 3338 | The Void Throne Room | inside | no_mob, no_teleport, indoors | **BOSS ROOM** — The Moon Sovereign |

**Tier 7 connectivity:** Entry from Tier 6 via locked `^obsidian sentinel gate` (rooms 3323/3329). The key (3204) drops from the Fire Warden via loot table. The Void Throne Room (3338) allows recall, letting players exit after defeating the final boss.

---

## Doors Summary

| From Room | Direction | To Room | Keyword | Lock? | Key Vnum | Reset State |
|-----------|-----------|---------|---------|-------|----------|-------------|
| 2558 (S.Oasis) | D2 (south) | 3140 | `^ancient road south` | No | -1 | `D 2558 2 1` (closed) |
| 3140 | D0 (north) | 2558 | (none) | No | -1 | open |
| 3148 | D2 (south) | 3149 | `^moon gate` | No | -1 | `D 3148 2 1` (closed) |
| 3149 | D0 (north) | 3148 | `^moon gate` | No | -1 | `D 3149 0 1` (closed) |
| 3180 | D2 (south) | 3181 | `^obsidian door` | No | -1 | `D 3180 2 1` (closed) |
| 3181 | D0 (north) | 3180 | `^obsidian door` | No | -1 | `D 3181 0 1` (closed) |
| 3161 | D5 (down) | 3185 | (none) | No | -1 | open |
| 3185 | D4 (up) | 3161 | (none) | No | -1 | open |
| 3168 | D5 (down) | 3186 | (none) | No | -1 | open |
| 3186 | D4 (up) | 3168 | (none) | No | -1 | open |
| 3218 | D2 (south) | 3219 | `^glyph-sealed door` | No | -1 | `D 3218 2 1` (closed) |
| 3219 | D0 (north) | 3218 | `^glyph-sealed door` | No | -1 | `D 3219 0 1` (closed) |
| 3199 | D5 (down) | 3220 | `^corroded iron hatch` | No | -1 | `D 3199 5 1` (closed) |
| 3220 | D4 (up) | 3199 | `^corroded iron hatch` | No | -1 | `D 3220 4 1` (closed) |
| 3211 | D5 (down) | 3221 | (none) | No | -1 | open |
| 3221 | D4 (up) | 3211 | (none) | No | -1 | open |
| 3248 | D2 (south) | 3249 | `^moss-covered stone door` | No | -1 | `D 3248 2 1` (closed) |
| 3249 | D0 (north) | 3248 | `^moss-covered stone door` | No | -1 | `D 3249 0 1` (closed) |
| 3238 | D5 (down) | 3250 | `^rusted grate` | No | -1 | `D 3238 5 1` (closed) |
| 3250 | D4 (up) | 3238 | `^rusted grate` | No | -1 | `D 3250 4 1` (closed) |
| 3235 | D5 (down) | 3251 | (none) | No | -1 | open |
| 3251 | D4 (up) | 3235 | (none) | No | -1 | open |
| 3273 | D2 (south) | 3274 | `^shadow-wreathed portal` | No | -1 | `D 3273 2 1` (closed) |
| 3274 | D0 (north) | 3273 | `^shadow-wreathed portal` | No | -1 | `D 3274 0 1` (closed) |
| 3265 | D5 (down) | 3275 | (none) | No | -1 | open |
| 3275 | D4 (up) | 3265 | (none) | No | -1 | open |
| 3262 | D5 (down) | 3276 | (none) | No | -1 | open |
| 3276 | D4 (up) | 3262 | (none) | No | -1 | open |
| 3293 | D2 (south) | 3294 | `^skull-arch threshold` | No | -1 | `D 3293 2 1` (closed) |
| 3294 | D0 (north) | 3293 | `^skull-arch threshold` | No | -1 | `D 3294 0 1` (closed) |
| 3290 | D5 (down) | 3295 | `^bone-barred gate` | Yes (locked) | 3203 | `D 3290 5 2` (locked) |
| 3295 | D4 (up) | 3290 | `^bone-barred gate` | Yes (locked) | 3203 | `D 3295 4 2` (locked) |
| 3313 | D2 (south) | 3314 | `^venom-slick gate` | No | -1 | `D 3313 2 1` (closed) |
| 3314 | D0 (north) | 3313 | `^venom-slick gate` | No | -1 | `D 3314 0 1` (closed) |
| 3309 | D5 (down) | 3315 | (none) | No | -1 | open |
| 3315 | D4 (up) | 3309 | (none) | No | -1 | open |
| 3327 | D2 (south) | 3328 | `^molten-iron gate` | No | -1 | `D 3327 2 1` (closed) |
| 3328 | D0 (north) | 3327 | `^molten-iron gate` | No | -1 | `D 3328 0 1` (closed) |
| 3323 | D5 (down) | 3329 | `^obsidian sentinel gate` | Yes (locked) | 3204 | `D 3323 5 2` (locked) |
| 3329 | D4 (up) | 3323 | `^obsidian sentinel gate` | Yes (locked) | 3204 | `D 3329 4 2` (locked) |
| 3337 | D2 (south) | 3338 | `^void chamber door` | No | -1 | `D 3337 2 1` (closed) |
| 3338 | D0 (north) | 3337 | `^void chamber door` | No | -1 | `D 3338 0 1` (closed) |
| 3184 (boss) | D0 (north) | 3183 | `^lunar vault door` | No | -1 | `D 3184 0 1` (closed) |
| 3183 | D2 (south) | 3184 | `^lunar vault door` | No | -1 | `D 3183 2 1` (closed) |

All named doors must be mentioned in at least one of: the room's main description, a spawned object, or an `E` extra description.

Both locked doors require the corresponding key object to exist in `#OBJECTS`:
- `^bone-barred gate` (rooms 3290/3295): key vnum **3152**
- `^obsidian sentinel gate` (rooms 3323/3329): key vnum **3153**

---

## Mobile Definitions

### Bosses (Level 150)

All bosses: `act` flags = `is_npc(1) | sentinel(2) | no_flee(16) | aggressive(32) | stay_area(64) | boss(67108864) | noassist(1073741824)` = **1140850879**

Boss skills (`!` line): `5_attack(8) | enhanced(32768) | counter(524288) | parry(8192) | dodge(4096) | nodisarm(32) | notrip(64)` = **569448**

All bosses placed only in `no_mob` flagged rooms.

| Mob Vnum | Name | Room | Act Flags | Theme | Spec |
|----------|------|------|-----------|-------|------|
| 3139 | the Moon Gate Guardian | 3184 | 1140850879 | Massive obsidian construct, guardian of the capstone entrance. Cast: earthquake, flamestrike. Def: heal, shockshield. Strong: earth, physical. Weak: mental. 6_attack instead of 5_attack. | spec_cast_mage |
| 3140 | the Silent Warden | 3219 | 1140850879 | Spectral guardian bound in eternal silence. Cast: suffocate, mind_flail, nerve_fire. Def: heal, iceshield. Strong: shadow, mental. Weak: holy, fire. | spec_cast_undead |
| 3141 | the Rot Warden | 3249 | 1140850879 | Titanic fungal horror animated by decay magic. Cast: acid_blast, suffocate, poison. Def: heal, cure_critic. Strong: poison, earth. Weak: fire, holy. | spec_cast_mage |
| 3142 | the Shadow Warden | 3274 | 1140850879 | Living shadow given form by death magic. Cast: mindflame, bloody_tears, ego_whip. Def: heal, iceshield, fireshield. Strong: shadow, mental. Weak: holy, light. | spec_cast_bigtime |
| 3143 | the Bone Warden | 3294 | 1140850879 | Colossus of welded skeletal remains. Cast: chain_light, mind_bolt, earthquake. Def: heal, shockshield. Strong: physical, earth. Weak: holy, fire. | spec_cast_mage |
| 3144 | the Venom Warden | 3314 | 1140850879 | Giant serpent-construct dripping with toxin. Cast: acid_blast, suffocate, lava_burst. Def: heal, cure_critic. Strong: poison, water. Weak: fire, air. Spec: spec_poison. | spec_poison |
| 3145 | the Fire Warden | 3328 | 1140850879 | Elemental of living magma and volcanic stone. Cast: lava_burst, heat_armor, flamestrike. Def: fireshield. Strong: fire, physical. Weak: water, air. Spec: spec_breath_fire. | spec_breath_fire |
| 3146 | the Moon Sovereign | 3338 | 1140850879 | The immortal architect of the inverted pyramid. Full spell set. Cast: nerve_fire, light_bolt, mindflame, suffocate, lava_burst. Def: heal, fireshield, iceshield, shockshield. Strong: shadow, death, mental. Weak: holy. | spec_cast_bigtime |

**Boss `+` extension:** All bosses receive moderate spellpower, crit, crit_mult, parry, dodge, and block modifiers appropriate for level 150 encounters. The Moon Sovereign (3146) receives maximum values.

---

### Strong (Solo) Mobs (Levels 135–142)

All strong mobs: `act` = `is_npc(1) | aggressive(32) | stay_area(64) | solo(33554432)` = **33554529**

| Mob Vnum | Name | Level | Zones | Notes |
|----------|------|-------|-------|-------|
| 3147 | a cursed moon shade | 138 | All | Undead spirit. Cast: mind_flail, suffocate. Def: heal. |
| 3148 | an obsidian colossus | 142 | Ground, Tier 1 | Massive animated construct. Enhanced, 4_attack, nodisarm, notrip. |
| 3149 | a death-sworn knight | 140 | Tiers 2–4 | Armored undead warrior. 4_attack, enhanced, parry, counter. |
| 3150 | a magma drake | 140 | Tiers 5–6 | Lesser drake of molten stone. 4_attack, enhanced, parry, dodge. Spec: spec_breath_fire. |
| 3151 | a jade sphinx | 142 | Tiers 4–5 | Construct guardian. 3_attack, enhanced, parry, dodge. Cast: earthquake, light_bolt. |
| 3199 | a toxic sand elemental | 139 | Ground, Tier 1 | Poisonous sand elemental. no_mind. 3_attack, enhanced, dodge. Cast: acid_blast, col_spray. Spec: spec_cast_mage. |
| 3200 | an ancient basalt guardian | 142 | Tier 2, Tier 3 | Massive black-stone construct. no_mind, sentinel. 4_attack, enhanced, parry, nodisarm, notrip. |
| 3201 | a greater bone overlord | 140 | Tier 3, Tier 4 | Towering skeletal warlord-spirit. undead. 3_attack, enhanced, parry, counter. Cast: suffocate, ego_whip. Spec: spec_cast_undead. |
| 3316 (mob) | a void-touched juggernaut | 142 | Tier 6, Tier 7 | Massive construct infused with void energy. no_mind. 4_attack, enhanced, parry, counter, nodisarm, notrip. |
| 3317 (mob) | a cistern matriarch | 140 | Tier 5 | Giant serpentine predator ruling the toxic pools. 4_attack, enhanced, parry, dodge. Spec: spec_poison. |
| 3318 (mob) | a petrified desert titan | 142 | Ground, Tier 1 | Enormous fossilized humanoid animated by pyramid magic. no_mind. 4_attack, enhanced, parry, nodisarm, notrip. Spec: spec_cast_mage. |
| 3319 (mob) | a fungal colossus | 138 | Tier 2 | Towering mushroom-creature trailing toxic spores. 3_attack, enhanced, parry. Spec: spec_poison. |
| 3320 (mob) | a nightmare stalker | 140 | Tier 3, Tier 4 | Apex predator of the shadow tier. 4_attack, enhanced, dodge, trip, counter. |

---

### Trash Mobs (Levels 125–140)

All trash: `act` = `is_npc(1) | aggressive(32) | stay_area(64)` = **97**

| Mob Vnum | Name | Level | Zones | Combat Notes |
|----------|------|-------|-------|--------------|
| 3152 (mob) | a desert tomb scarab swarm | 125 | Ground, Tier 1 | no_body, no_mind. 2_attack. |
| 3153 (mob) | a moon-cursed skeleton | 127 | Ground, Tier 1 | undead. 2_attack, punch. |
| 3154 | a desiccated mummy | 128 | Ground, Tier 1 | undead. 2_attack, enhanced. |
| 3155 | a venomous desert scorpion | 130 | Ground, Tier 1 | Spec: spec_poison. 2_attack. |
| 3156 | a grave robber's specter | 126 | Ground | undead, no_body. Cast: flare, shock_grasp. |
| 3157 | a sand hyena | 132 | Ground | 2_attack, dodge, trip. |
| 3158 | a basalt guardian statue | 135 | Ground, Tier 1 | no_mind. 3_attack, enhanced, nodisarm. |
| 3159 | a canopic abomination | 129 | Ground, Tier 1 | undead, no_mind. 2_attack, punch. |
| 3160 | a jade asp | 133 | Tier 1, Tier 2 | Spec: spec_poison. 2_attack. |
| 3161 (mob) | a desert vulture | 136 | Tier 1, Tier 2 | 2_attack, dodge. |
| 3162 (mob) | an obsidian golem | 130 | Tier 1 | no_mind. 3_attack, enhanced. |
| 3163 | a decay acolyte | 138 | Tier 2 | Cast: faerie_fire, flare, col_spray. Def: cure_serious. |
| 3164 | a rot wight | 134 | Tier 2 | undead. 2_attack, enhanced, drain. |
| 3165 | a stone scarab sentinel | 137 | Tier 2, Tier 3 | no_mind. 3_attack, enhanced, parry. |
| 3166 | a tomb cobra | 131 | Tier 2 | Spec: spec_poison. 2_attack. |
| 3167 | a shadow archer | 139 | Tier 3 | undead. 2_attack, enhanced, dodge. |
| 3168 (mob) | a great shadow vulture | 140 | Tier 3 | 2_attack, dodge, kick. |
| 3169 | a shadow face guardian | 136 | Tier 3 | no_mind, sentinel. 2_attack, parry, nodisarm. |
| 3170 | a creeping shadow | 132 | Tier 3, Tier 4 | undead, no_body. Cast: mind_bolt, ego_whip. |
| 3171 | a shadow assassin | 138 | Tier 3, Tier 4 | 2_attack, enhanced, dodge, trip. |
| 3172 | an animated bone pillar | 140 | Tier 4 | no_mind, sentinel. 4_attack, enhanced, nodisarm, notrip. |
| 3173 | a venomous golden cobra | 135 | Tier 4, Tier 5 | Spec: spec_poison. 2_attack. |
| 3174 | a bone cultist | 137 | Tier 4 | Cast: burn_hands, mag_missile. Def: cure_serious. |
| 3175 | a death hawk | 139 | Tier 4, Tier 5 | 2_attack, dodge, kick. |
| 3176 | a bone revenant | 133 | Tier 4 | undead. 2_attack, enhanced, parry. Def: cure_critic. |
| 3177 | a void specter | 136 | Tier 5, Tier 6 | undead, no_body. Cast: mindflame, mind_bolt. Def: cure_critic. |
| 3178 | a carrion fly swarm | 125 | Ground, Tier 1 | Clouds of carrion flies. no_body, no_mind. 2_attack. Spec: spec_poison. |
| 3179 | a skeletal desert rat | 125 | Ground, Tier 1 | Skeletal undead vermin. no_mind. 2_attack, trip. Spec: spec_poison. |
| 3180 | a bone crawler | 127 | Ground, Tier 1 | Animated skeletal mass. undead, no_mind. 3_attack, enhanced. |
| 3181 | a sand-choked revenant | 129 | Ground, Tier 1 | Revenant packed with sand. undead. 2_attack, enhanced, drain. |
| 3182 | an animated obelisk shard | 132 | Ground | Fragment of a shattered obelisk. no_mind, sentinel. 3_attack, enhanced, nodisarm. |
| 3183 | a cursed pyramid laborer | 130 | Ground, Tier 1 | Ghost of a slave. undead. 2_attack, enhanced. Cast: weaken, blindness. |
| 3184 (mob) | a desert pit viper | 128 | Ground | A coiled heat-seeking viper. 2_attack, dodge. Spec: spec_poison. |
| 3185 (mob) | an obsidian sentinel construct | 134 | Ground, Tier 1 | Hulking guardian golem. no_mind. 3_attack, enhanced, parry, nodisarm. |
| 3186 (mob) | a moon glyph guardian | 135 | Tier 1 | Living inscription in stone. no_mind, sentinel. 3_attack, enhanced. |
| 3187 | an animated burial jar | 132 | Tier 1, Tier 2 | A walking canopic jar. undead, no_body. 2_attack, punch. Cast: poison, col_spray. |
| 3188 | a scribal phantom | 136 | Tier 1 | Ghost of a scribe. undead. 2_attack. Cast: flare, mag_missile. Def: cure_light. |
| 3189 | a sand-bound mummy thrall | 133 | Tier 1, Tier 2 | Lesser mummy. undead. 2_attack, enhanced. |
| 3190 | a rot priest's thrall | 138 | Tier 2 | Undead acolyte. undead. 2_attack, enhanced, parry. Cast: faerie_fire. |
| 3191 | a fungal wraith | 137 | Tier 2, Tier 3 | Translucent fungal spirit. undead, no_body. Cast: ego_whip, suffocate. Def: cure_serious. |
| 3192 | a curse-bound sentinel | 139 | Tier 2, Tier 3 | Ancient soldier held by curse. undead. 3_attack, enhanced, counter. |
| 3193 | an entombed general | 138 | Tier 3 | Buried standing, still armed. undead. 3_attack, enhanced, parry, dodge. |
| 3194 | a bone golem | 140 | Tier 3, Tier 4 | Construct of fused skeletal parts. no_mind. 4_attack, enhanced, nodisarm, notrip. |
| 3195 | a crypt stalker | 136 | Tier 3 | A beast that hunts in the dark. 2_attack, dodge, trip. |
| 3196 | a magma lizard | 137 | Tier 5, Tier 6 | Long-tailed reptile absorbing volcanic heat. 2_attack, dodge. Resist: fire. |
| 3197 | a charred bone husk | 139 | Tier 5, Tier 6 | Undead burned to blackened bone. undead. 3_attack, enhanced. Strong: fire. Weak: water. |
| 3198 | a void sentinel | 140 | Tier 7 | The last stone guardian before the throne. no_mind, sentinel. 3_attack, enhanced, parry, nodisarm. |
| 3202 | a gilded moon shade | 138 | Tier 7 | Ghost of a royal bodyguard in ceremonial obsidian. undead. 2_attack, enhanced. Cast: mind_bolt. |
| 3286 (mob) | a petrified sand wurm | 134 | Ground, Tier 1 | Burrowing worm fossilized mid-strike. no_mind. 3_attack, enhanced, notrip. |
| 3287 (mob) | a tomb guardian jackal | 130 | Ground | Undead jackal bound to patrol. undead. 2_attack, dodge, trip. |
| 3288 (mob) | an embalmed crocodile | 133 | Ground, Tier 1 | Preserved reptile animated by curse. undead, no_mind. 3_attack, enhanced. |
| 3289 (mob) | a dust devil spirit | 127 | Ground | Whirling sand elemental fragment. no_body, no_mind. 2_attack, dodge. |
| 3290 (mob) | a whispering wraith | 131 | Tier 1 | Translucent shade that mouths silent curses. undead, no_body. Cast: weaken, blindness, chill_touch. |
| 3291 (mob) | a calcified serpent | 129 | Tier 1, Tier 2 | Mineralized snake, still strikes. no_mind. 2_attack, dodge. Spec: spec_poison. |
| 3292 (mob) | a tomb beetle colossus | 136 | Tier 1, Tier 2 | Man-sized scarab with iron mandibles. no_mind. 3_attack, enhanced, parry. |
| 3293 (mob) | a mold-ridden ghoul | 135 | Tier 2 | Fungal growth erupts from rotting flesh. undead. 2_attack, enhanced, drain. Spec: spec_poison. |
| 3294 (mob) | a bloated corpse walker | 132 | Tier 2 | Swollen cadaver leaking toxic gas. undead. 2_attack, enhanced. |
| 3295 (mob) | a decay spider | 128 | Tier 2 | Eight-legged creature woven from rotting sinew. 2_attack, dodge, trip. Spec: spec_poison. |
| 3296 (mob) | a mildew phantom | 134 | Tier 2, Tier 3 | Ghostly outline dripping condensation. undead, no_body. Cast: chill_touch, weaken. Def: cure_light. |
| 3297 (mob) | a shadow hound | 137 | Tier 3 | Pack hunter from the dark. 2_attack, enhanced, dodge, trip. |
| 3298 (mob) | a nightmare sentinel | 139 | Tier 3 | Animated suit of black iron armor. no_mind. 3_attack, enhanced, parry, nodisarm. |
| 3299 (mob) | a shade dancer | 133 | Tier 3 | Flickering silhouette that attacks from blind spots. 2_attack, dodge, trip. |
| 3300 (mob) | a darkfire wisp | 126 | Tier 3, Tier 4 | Small sphere of black flame. no_body, no_mind. 2_attack. Cast: burn_hands, flare. |
| 3301 (mob) | an ossuary spider | 131 | Tier 4 | Spider constructed from finger bones. no_mind. 2_attack, dodge, trip. Spec: spec_poison. |
| 3302 (mob) | a marrow elemental | 138 | Tier 4, Tier 5 | Oozing mass of liquefied bone marrow. no_mind. 3_attack, enhanced. |
| 3303 (mob) | a skeletal war drummer | 134 | Tier 4 | Undead percussionist that rallies nearby dead. undead. 2_attack, enhanced. Cast: haste, armor. |
| 3304 (mob) | a calcite horror | 140 | Tier 4, Tier 5 | Crystallized bone growth animate with malice. no_mind. 3_attack, enhanced, parry, notrip. |
| 3305 (mob) | a venom puddle ooze | 127 | Tier 5 | Corrosive toxic slime. no_body, no_mind. 2_attack. Spec: spec_poison. |
| 3306 (mob) | a cistern lurker | 136 | Tier 5 | Amphibious predator that hides in toxic pools. 2_attack, enhanced, dodge, trip. |
| 3307 (mob) | a poison dart frog construct | 130 | Tier 5 | Jade-and-gold mechanical frog. no_mind. 2_attack, dodge. Spec: spec_poison. |
| 3308 (mob) | a toxic gas wraith | 135 | Tier 5, Tier 6 | Ghostly cloud of concentrated fumes. undead, no_body. Cast: acid_blast, suffocate. Def: cure_serious. |
| 3309 (mob) | a lava crawler | 137 | Tier 6 | Salamander-like beast that swims through molten stone. 2_attack, enhanced, dodge. Resist: fire. |
| 3310 (mob) | an ember revenant | 139 | Tier 6 | Charred undead warrior wreathed in dying flames. undead. 3_attack, enhanced, parry. Cast: heat_armor, flamestrike. |
| 3311 (mob) | a slag golem | 140 | Tier 6 | Construct of cooled volcanic slag. no_mind. 4_attack, enhanced, nodisarm, notrip. |
| 3312 (mob) | an obsidian gargoyle | 138 | Tier 6, Tier 7 | Winged figure carved from volcanic glass. no_mind. 3_attack, enhanced, dodge, parry. |
| 3313 (mob) | a void acolyte | 137 | Tier 7 | Living cultist sustained by void magic across millennia. 2_attack, enhanced, parry. Cast: mindflame, ego_whip. Def: heal. |
| 3314 (mob) | a sovereign's tomb guardian | 140 | Tier 7 | Elite construct clad in obsidian plate. no_mind, sentinel. 4_attack, enhanced, parry, counter, nodisarm. |
| 3315 (mob) | a moon-touched scorpion | 135 | Tier 5, Tier 6 | Oversized scorpion glowing faintly silver. 2_attack, dodge. Spec: spec_poison. |

**Note on vnum reuse:** Mob vnums 3152–3202 overlap numerically with object vnums in the same range. Cross-type vnum overlap is permitted by the spec; each is unique within its own type index.

---

## Object Definitions

### Piece Item Chain (15 objects: 3139–3153)

All eight boss fragment pieces are delivered via boss loot tables and carry `ITEM_LOOT`.

**Fragment extra_flags:** `ITEM_NODROP(128) | ITEM_BOSS(134217728) | ITEM_MAGIC(64) | ITEM_LOOT(67108864)` = **201326784**

**Assembly extra_flags (created by combining, not looted):** `ITEM_NODROP(128) | ITEM_BOSS(134217728) | ITEM_MAGIC(64)` = **134217920**

**Final Lantern extra_flags:** `ITEM_RARE(1048576) | ITEM_NODROP(128) | ITEM_BOSS(134217728) | ITEM_MAGIC(64) | ITEM_LOOT(67108864)` = **202375360**

All piece and assembly items: `item_type = 29` (ITEM_PIECE), `wear_flags = take(8388608)`.
Final Lantern: `item_type = 1` (ITEM_LIGHT), `wear_flags = hold(32768) | take(8388608) = 8421376`, `value[2] = -1` (infinite light).

| Obj Vnum | Name | Type | value0 | value1 | value2 | Source |
|----------|------|------|--------|--------|--------|--------|
| 3139 | a shard of the moon sovereign's lantern (I) | piece | 0 | 3140 | 3147 | Moon Gate Guardian loot |
| 3140 | a shard of the moon sovereign's lantern (II) | piece | 3139 | 0 | 3147 | Silent Warden loot |
| 3141 | a shard of the moon sovereign's lantern (III) | piece | 3147 | 0 | 3148 | Rot Warden loot |
| 3142 | a shard of the moon sovereign's lantern (IV) | piece | 3148 | 0 | 3149 | Shadow Warden loot |
| 3143 | a shard of the moon sovereign's lantern (V) | piece | 3149 | 0 | 3150 | Bone Warden loot |
| 3144 | a shard of the moon sovereign's lantern (VI) | piece | 3150 | 0 | 3151 | Venom Warden loot |
| 3145 | a shard of the moon sovereign's lantern (VII) | piece | 3151 | 0 | 3152 (obj) | Fire Warden loot |
| 3146 | a shard of the moon sovereign's lantern (VIII) | piece | 3152 (obj) | 0 | 3153 (obj) | Moon Sovereign loot |
| 3147 | a partial lantern of the moon sovereign (I-II) | piece | 0 | 3141 | 3148 | Created from I+II |
| 3148 | a partial lantern of the moon sovereign (I-III) | piece | 0 | 3142 | 3149 | Created from (I-II)+III |
| 3149 | a partial lantern of the moon sovereign (I-IV) | piece | 0 | 3143 | 3150 | Created from (I-III)+IV |
| 3150 | a partial lantern of the moon sovereign (I-V) | piece | 0 | 3144 | 3151 | Created from (I-IV)+V |
| 3151 | a partial lantern of the moon sovereign (I-VI) | piece | 0 | 3145 | 3152 (obj) | Created from (I-V)+VI |
| 3152 (obj) | a partial lantern of the moon sovereign (I-VII) | piece | 0 | 3146 | 3153 (obj) | Created from (I-VI)+VII |
| 3153 (obj) | the Void Lantern of the Moon Sovereign | light (1) | 0 | 0 | -1 | **Final combined item** |

**Combining sequence:**
1. Shard I (3139) + Shard II (3140) → Assembly I-II (3147)
2. Assembly I-II (3147) + Shard III (3141) → Assembly I-III (3148)
3. Assembly I-III (3148) + Shard IV (3142) → Assembly I-IV (3149)
4. Assembly I-IV (3149) + Shard V (3143) → Assembly I-V (3150)
5. Assembly I-V (3150) + Shard VI (3144) → Assembly I-VI (3151)
6. Assembly I-VI (3151) + Shard VII (3145) → Assembly I-VII (3152 obj)
7. Assembly I-VII (3152 obj) + Shard VIII (3146) → **the Void Lantern of the Moon Sovereign (3153 obj)**

The `connect` command checks: `pieceA.value0 == pieceB.vnum` OR `pieceA.value1 == pieceB.vnum`. In each step above, one piece's `value1` equals the other piece's vnum, satisfying the check. On success, both are consumed and `pieceA.value2` is created.

---

### Key Objects (2 objects: 3152, 3153 — note: these share vnums with piece-chain assemblies)

Since the piece-chain assembly objects 3152 and 3153 are created by the `connect` command and never loaded from area files, the key objects can safely share those vnums. However, to avoid any confusion, we will use **separate vnums** for the keys: **3203** and **3204**.

| Obj Vnum | Name | item_type | extra_flags | wear_flags | Level | Notes |
|----------|------|-----------|-------------|------------|-------|-------|
| 3203 | a corroded bone-barred key | key (18) | ITEM_NODROP (128) | take (8388608) | L 120 | Opens `^bone-barred gate` (rooms 3290/3295). Spawns via `O` reset in room 3288 (The Reliquary in Ossuary Depths). |
| 3204 | a volcanic glass sentinel key | key (18) | ITEM_NODROP(128) \| ITEM_LOOT(67108864) = 67108992 | take (8388608) | L 140 | Opens `^obsidian sentinel gate` (rooms 3323/3329). Boss loot from Fire Warden. |

**Updated door key references:**
- `^bone-barred gate` (rooms 3290/3295): key vnum **3203**
- `^obsidian sentinel gate` (rooms 3323/3329): key vnum **3204**

---

### Boss Equipment (Level 140, extra_flags = ITEM_NODROP(128) | ITEM_BOSS(134217728) | ITEM_MAGIC(64) = 134217920)

Items delivered via boss loot tables also add ITEM_LOOT(67108864) for total: **201326784**.

Each boss drops its piece shard plus 3 boss gear items via loot table. The Moon Sovereign drops 4 gear items.

Weight key: 1-5 = caster, 6-10 = melee, 11-15 = tank.

**Moon Gate Guardian (mob 3139) — gear vnums 3205–3207:**

| Obj Vnum | Name | item_type | wear_flags | weight | Archetype |
|----------|------|-----------|------------|--------|-----------|
| 3205 | a gauntlet of the obsidian guardian | armor (9) | hands(4096)\|take | 13 | tank |
| 3206 | the guardian's basalt maul | weapon (5) | hold(32768)\|take | 8 | melee; value3=7 (pound) |
| 3207 | a sash of the moon gate | armor (9) | waist(131072)\|take | 7 | melee |

**Silent Warden (mob 3140) — gear vnums 3208–3210:**

| Obj Vnum | Name | item_type | wear_flags | weight | Archetype |
|----------|------|-----------|------------|--------|-----------|
| 3208 | the Silent Warden's death mask | armor (9) | face(16)\|take | 2 | caster |
| 3209 | a shroud of eternal silence | armor (9) | about(65536)\|take | 3 | caster |
| 3210 | the Silent Warden's muffled wrappings | armor (9) | body(262144)\|take | 14 | tank |

**Rot Warden (mob 3141) — gear vnums 3211–3213:**

| Obj Vnum | Name | item_type | wear_flags | weight | Archetype |
|----------|------|-----------|------------|--------|-----------|
| 3211 | a moss-grown breastplate of decay | armor (9) | body(262144)\|take | 15 | tank |
| 3212 | the Rot Warden's fungal staff | weapon (5) | hold(32768)\|take | 4 | caster; ITEM_TWO_HANDED; value3=7 (pound) |
| 3213 | a ring of creeping rot | armor (9) | finger(8192)\|take | 1 | caster |

Note for 3212: extra_flags = 201326784 | ITEM_TWO_HANDED(2147483648) = 2348810432

**Shadow Warden (mob 3142) — gear vnums 3214–3216:**

| Obj Vnum | Name | item_type | wear_flags | weight | Archetype |
|----------|------|-----------|------------|--------|-----------|
| 3214 | the Shadow Warden's eclipse circlet | armor (9) | head(8)\|take | 5 | caster |
| 3215 | robes of living shadow | armor (9) | body(262144)\|take | 2 | caster |
| 3216 | the Shadow Warden's umbral blade | weapon (5) | hold(32768)\|take | 9 | melee; value3=2 (stab) |

**Bone Warden (mob 3143) — gear vnums 3217–3219:**

| Obj Vnum | Name | item_type | wear_flags | weight | Archetype |
|----------|------|-----------|------------|--------|-----------|
| 3217 | greaves of the bone warden | armor (9) | legs(1048576)\|take | 11 | tank |
| 3218 | the Bone Warden's skull flail | weapon (5) | hold(32768)\|take | 10 | melee; value3=7 (pound) |
| 3219 | pauldrons of fused bone | armor (9) | shoulders(512)\|take | 12 | tank |

**Venom Warden (mob 3144) — gear vnums 3220–3222:**

| Obj Vnum | Name | item_type | wear_flags | weight | Archetype |
|----------|------|-----------|------------|--------|-----------|
| 3220 | a scale-mail hauberk of venom | armor (9) | body(262144)\|take | 13 | tank |
| 3221 | the Venom Warden's fang-blade | weapon (5) | hold(32768)\|take | 6 | melee; ITEM_FIST; value3=5 (claw) |
| 3222 | a serpent-coil armlet | armor (9) | wrist(840)\|take | 3 | caster |

Note for 3221: extra_flags = 201326784 | ITEM_FIST(1073741824) = 1275068608

**Fire Warden (mob 3145) — gear vnums 3223–3225:**

| Obj Vnum | Name | item_type | wear_flags | weight | Archetype |
|----------|------|-----------|------------|--------|-----------|
| 3223 | a magma-forged breastplate | armor (9) | body(262144)\|take | 14 | tank |
| 3224 | the Fire Warden's ember-claw | weapon (5) | hold(32768)\|take | 7 | melee; ITEM_FIST; value3=5 (claw) |
| 3225 | a crest of volcanic stone | armor (9) | horns(4)\|take | 8 | melee |

Note for 3224: extra_flags = 201326784 | ITEM_FIST(1073741824) = 1275068608

**Moon Sovereign (mob 3146) — gear vnums 3226–3229 (4 items, final boss):**

| Obj Vnum | Name | item_type | wear_flags | weight | Archetype |
|----------|------|-----------|------------|--------|-----------|
| 3226 | the Moon Sovereign's void scepter | weapon (5) | hold(32768)\|take | 4 | caster; ITEM_TWO_HANDED; value3=7 (pound) |
| 3227 | robes of the eternal moon | armor (9) | body(262144)\|take | 5 | caster |
| 3228 | the sovereign's amulet of the void | armor (9) | neck(128)\|take | 1 | caster |
| 3229 | boots of the underworld throne | armor (9) | feet(2097152)\|take | 9 | melee |

Note for 3226: extra_flags = 201326784 | ITEM_TWO_HANDED(2147483648) = 2348810432

---

### Trash Equipment (Levels 125–130, ITEM_NODROP only unless loot-table)

Reset gear extra_flags: `ITEM_NODROP(128)` = **128**
Loot-table gear extra_flags: `ITEM_NODROP(128) | ITEM_LOOT(67108864)` = **67108992**

All items include `take(8388608)` in wear_flags.

| Obj Vnum | Name | item_type | wear_flags | weight | Level | Delivery | Source |
|----------|------|-----------|------------|--------|-------|----------|--------|
| 3230 | a moon skeleton's blade | weapon (5) | hold\|take | 6 | L 127 | Reset (E) | moon-cursed skeleton |
| 3231 | a mummy's decay sash | armor (9) | waist\|take | 12 | L 128 | Loot | desiccated mummy |
| 3232 | a scorpion-sting gauntlet | armor (9) | hands\|take | 13 | L 130 | Loot | venomous desert scorpion |
| 3233 | a ghostly grave shroud | armor (9) | about\|take | 2 | L 126 | Loot | grave robber's specter |
| 3234 | a hyena-hide belt | armor (9) | waist\|take | 7 | L 132 | Loot | sand hyena |
| 3235 | basalt guardian fist | weapon (5) | hold\|take | 14 | L 135 | Reset (E) | basalt guardian statue; ITEM_FIST; value3=0 |
| 3236 | canopic abomination mandible | weapon (5) | hold\|take | 8 | L 129 | Loot | canopic abomination; value3=10 (bite) |
| 3237 | a jade asp-fang earring | armor (9) | ear\|take | 3 | L 133 | Loot | jade asp |
| 3238 | desert vulture wing-cloak | armor (9) | about\|take | 4 | L 136 | Loot | desert vulture |
| 3239 | an obsidian golem's core shard | armor (9) | hold\|take | 15 | L 130 | Loot | obsidian golem; shield |
| 3240 | decay acolyte's cursed headwrap | armor (9) | head\|take | 5 | L 138 | Reset (E) | decay acolyte |
| 3241 | rot wight's bone ring | armor (9) | finger\|take | 1 | L 134 | Loot | rot wight |
| 3242 | stone scarab sentinel shell plate | armor (9) | body\|take | 11 | L 137 | Reset (E) | stone scarab sentinel |
| 3243 | a cobra-scale bracer | armor (9) | wrist\|take | 12 | L 131 | Loot | tomb cobra |
| 3244 | a shadow archer's bow wrap | armor (9) | arms\|take | 9 | L 139 | Loot | shadow archer |
| 3245 | shadow vulture-feather mantle | armor (9) | shoulders\|take | 10 | L 140 | Loot | great shadow vulture |
| 3246 | shadow face guardian stone buckler | armor (9) | hold\|take | 13 | L 136 | Reset (E) | shadow face guardian; ITEM_BUCKLER |
| 3247 | creeping shadow cowl | armor (9) | head\|take | 2 | L 132 | Loot | creeping shadow |
| 3248 | a shadow assassin's sand-silk hood | armor (9) | head\|take | 3 | L 138 | Loot | shadow assassin |
| 3249 | an animated bone pillar's knuckle-spike | weapon (5) | hold\|take | 14 | L 140 | Reset (E) | animated bone pillar; ITEM_FIST; value3=0 |
| 3250 | venomous cobra fang pendant | armor (9) | neck\|take | 4 | L 135 | Loot | venomous golden cobra |
| 3251 | bone cultist's obsidian dagger | weapon (5) | hold\|take | 6 | L 137 | Loot | bone cultist; value3=2 (stab) |
| 3252 | death hawk talons | weapon (5) | hold\|take | 7 | L 139 | Loot | death hawk; value3=5 (claw) |
| 3253 | bone revenant's burial ring | armor (9) | finger\|take | 5 | L 133 | Loot | bone revenant |
| 3254 | void specter-touched robes | armor (9) | body\|take | 1 | L 136 | Reset (E) | void specter |
| 3255 | cursed moon shade's halo | armor (9) | halo\|take | 2 | L 138 | Reset (E) | cursed moon shade |
| 3256 | obsidian colossus's stone bracer | armor (9) | wrist\|take | 15 | L 142 | Reset (E) | obsidian colossus |
| 3257 | a death-sworn knight's greatsword | weapon (5) | hold\|take | 8 | L 140 | Reset (E) | death-sworn knight; ITEM_TWO_HANDED; value3=3 (slash) |
| 3258 | magma drake scale boots | armor (9) | feet\|take | 11 | L 140 | Reset (E) | magma drake |
| 3259 | a jade sphinx shoulder plate | armor (9) | shoulders\|take | 12 | L 142 | Reset (E) | jade sphinx |
| 3260 | scarab swarm chitin ring | armor (9) | finger\|take | 3 | L 125 | Loot | desert tomb scarab swarm |
| 3261 | a carrion fly carapace brooch | armor (9) | finger\|take | 4 | L 125 | Loot | carrion fly swarm |
| 3262 | a skeletal rat-tail lash | weapon (5) | hold\|take | 9 | L 125 | Loot | skeletal desert rat; value3=1 (whip) |
| 3263 | bone crawler chitin pauldrons | armor (9) | shoulders\|take | 13 | L 127 | Loot | bone crawler |
| 3264 | a revenant's tattered girdle | armor (9) | waist\|take | 14 | L 129 | Loot | sand-choked revenant |
| 3265 | an animated obelisk buckler | armor (9) | hold\|take | 15 | L 132 | Loot | animated obelisk shard; ITEM_BUCKLER |
| 3266 | a cursed laborer's stone trowel | weapon (5) | hold\|take | 10 | L 130 | Loot | cursed pyramid laborer; value3=7 (pound) |
| 3267 | a desert pit viper fang | armor (9) | ear\|take | 5 | L 128 | Loot | desert pit viper |
| 3268 | an obsidian sentinel's faceplate | armor (9) | face\|take | 11 | L 134 | Reset (E) | obsidian sentinel construct |
| 3269 | a moon glyph guardian stone slab | armor (9) | hold\|take | 12 | L 135 | Loot | moon glyph guardian; ITEM_BUCKLER |
| 3270 | an animated burial jar lid | armor (9) | head\|take | 1 | L 132 | Loot | animated burial jar |
| 3271 | a scribal phantom's cursed inkpot | armor (9) | hold\|take | 2 | L 136 | Loot | scribal phantom |
| 3272 | linen wraps of the mummy thrall | armor (9) | wrist\|take | 13 | L 133 | Loot | sand-bound mummy thrall |
| 3273 | a rot priest thrall's robe | armor (9) | body\|take | 3 | L 138 | Reset (E) | rot priest's thrall |
| 3274 | a fungal wraith's wisp veil | armor (9) | about\|take | 4 | L 137 | Loot | fungal wraith |
| 3275 | a curse-bound sentinel's iron pauldron | armor (9) | shoulders\|take | 14 | L 139 | Loot | curse-bound sentinel |
| 3276 | an entombed general's war crown | armor (9) | head\|take | 15 | L 138 | Reset (E) | entombed general |
| 3277 | a bone golem's riveted knuckle | weapon (5) | hold\|take | 11 | L 140 | Loot | bone golem; ITEM_FIST; value3=0; extra_flags = NODROP+LOOT+FIST = 1140850816 |
| 3278 | a crypt stalker's shadow wrapping | armor (9) | about\|take | 5 | L 136 | Loot | crypt stalker |
| 3279 | magma lizard scale vest | armor (9) | body\|take | 12 | L 137 | Loot | magma lizard |
| 3280 | charred bone husk smoldering bracers | armor (9) | wrist\|take | 13 | L 139 | Loot | charred bone husk |
| 3281 | a void sentinel's stone gauntlets | armor (9) | hands\|take | 14 | L 140 | Reset (E) | void sentinel |
| 3282 | a gilded moon shade's helm | armor (9) | head\|take | 1 | L 138 | Loot | gilded moon shade |
| 3283 | a toxic sand elemental's howling band | armor (9) | wrist\|take | 2 | L 139 | Reset (E) | toxic sand elemental |
| 3284 | an ancient basalt guardian's plate arm | armor (9) | arms\|take | 15 | L 142 | Reset (E) | ancient basalt guardian |
| 3285 | a bone overlord's burial circlet | armor (9) | head\|take | 11 | L 140 | Reset (E) | greater bone overlord |
| 3286 | a petrified sand wurm's calcified fang | weapon (5) | hold\|take | 6 | L 134 | Loot | petrified sand wurm; value3=10 (bite) |
| 3287 | a tomb jackal's obsidian collar | armor (9) | neck\|take | 12 | L 130 | Loot | tomb guardian jackal |
| 3288 | embalmed crocodile scale greaves | armor (9) | legs\|take | 13 | L 133 | Loot | embalmed crocodile |
| 3289 | a dust devil's swirling band | armor (9) | finger\|take | 3 | L 127 | Loot | dust devil spirit |
| 3290 (obj) | a whispering wraith's torn veil | armor (9) | face\|take | 4 | L 131 | Loot | whispering wraith |
| 3291 (obj) | calcified serpent scale pauldrons | armor (9) | shoulders\|take | 14 | L 129 | Loot | calcified serpent |
| 3292 (obj) | tomb beetle mandible gauntlets | armor (9) | hands\|take | 15 | L 136 | Loot | tomb beetle colossus |
| 3293 (obj) | a mold-ridden ghoul's festering girdle | armor (9) | waist\|take | 5 | L 135 | Loot | mold-ridden ghoul |
| 3294 (obj) | a bloated corpse walker's burst vest | armor (9) | body\|take | 11 | L 132 | Loot | bloated corpse walker |
| 3295 (obj) | decay spider silk wraps | armor (9) | wrist\|take | 1 | L 128 | Loot | decay spider |
| 3296 (obj) | a mildew phantom's dripping cowl | armor (9) | head\|take | 2 | L 134 | Loot | mildew phantom |
| 3297 (obj) | shadow hound fang necklace | armor (9) | neck\|take | 7 | L 137 | Loot | shadow hound |
| 3298 (obj) | a nightmare sentinel's black iron helm | armor (9) | head\|take | 12 | L 139 | Reset (E) | nightmare sentinel |
| 3299 (obj) | shade dancer's flickering sash | armor (9) | waist\|take | 3 | L 133 | Loot | shade dancer |
| 3300 (obj) | a darkfire wisp's ember cinder | armor (9) | hold\|take | 4 | L 126 | Loot | darkfire wisp; light source |
| 3301 (obj) | an ossuary spider's bone-silk bracer | armor (9) | wrist\|take | 5 | L 131 | Loot | ossuary spider |
| 3302 (obj) | a marrow elemental's slick boots | armor (9) | feet\|take | 13 | L 138 | Loot | marrow elemental |
| 3303 (obj) | a skeletal war drummer's bone drumstick | weapon (5) | hold\|take | 8 | L 134 | Loot | skeletal war drummer; value3=7 (pound) |
| 3304 (obj) | calcite horror shard plate | armor (9) | body\|take | 14 | L 140 | Loot | calcite horror |
| 3305 (obj) | a venom puddle ooze's corroded ring | armor (9) | finger\|take | 1 | L 127 | Loot | venom puddle ooze |
| 3306 (obj) | cistern lurker scale leggings | armor (9) | legs\|take | 15 | L 136 | Loot | cistern lurker |
| 3307 (obj) | a jade dart frog's golden eye | armor (9) | ear\|take | 2 | L 130 | Loot | poison dart frog construct |
| 3308 (obj) | toxic gas wraith's acrid shroud | armor (9) | about\|take | 3 | L 135 | Loot | toxic gas wraith |
| 3309 (obj) | lava crawler's igneous hide vest | armor (9) | body\|take | 11 | L 137 | Loot | lava crawler |
| 3310 (obj) | an ember revenant's smoldering blade | weapon (5) | hold\|take | 9 | L 139 | Loot | ember revenant; value3=3 (slash) |
| 3311 (obj) | a slag golem's cooled-iron fist | weapon (5) | hold\|take | 12 | L 140 | Reset (E) | slag golem; ITEM_FIST; value3=0 |
| 3312 (obj) | obsidian gargoyle wing-plate pauldrons | armor (9) | shoulders\|take | 13 | L 138 | Loot | obsidian gargoyle |
| 3313 (obj) | a void acolyte's dark prayer beads | armor (9) | neck\|take | 4 | L 137 | Loot | void acolyte |
| 3314 (obj) | sovereign tomb guardian's obsidian greaves | armor (9) | legs\|take | 14 | L 140 | Reset (E) | sovereign's tomb guardian |
| 3315 (obj) | moon-touched scorpion tail barb | weapon (5) | hold\|take | 10 | L 135 | Loot | moon-touched scorpion; value3=2 (stab); spec_poison |
| 3316 (obj) | void-touched juggernaut's core plate | armor (9) | body\|take | 15 | L 142 | Reset (E) | void-touched juggernaut |
| 3317 (obj) | cistern matriarch's venom-drip crown | armor (9) | head\|take | 5 | L 140 | Reset (E) | cistern matriarch |
| 3318 (obj) | petrified desert titan's stone fist | weapon (5) | hold\|take | 11 | L 142 | Reset (E) | petrified desert titan; ITEM_FIST; value3=0 |
| 3319 (obj) | fungal colossus spore-cap helm | armor (9) | head\|take | 1 | L 138 | Reset (E) | fungal colossus |
| 3320 (obj) | nightmare stalker's shadowed claws | weapon (5) | hold\|take | 6 | L 140 | Reset (E) | nightmare stalker; value3=5 (claw) |

**Note:** Object vnums 3230–3320 overlap with mob vnums in the same range. Cross-type overlap is permitted; each is unique within its type index.

---

## Resets Plan

### Boss Mob Resets

```
M 0 3139 1 3184 Moon Gate Guardian in Moon Gate Chamber
M 0 3140 1 3219 Silent Warden in Silent Warden's Tomb
M 0 3141 1 3249 Rot Warden in Rot Warden's Chamber
M 0 3142 1 3274 Shadow Warden in Shadow Warden's Sanctum
M 0 3143 1 3294 Bone Warden in Bone Warden's Crypt
M 0 3144 1 3314 Venom Warden in Venom Warden's Cistern
M 0 3145 1 3328 Fire Warden in Fire Warden's Crucible
M 0 3146 1 3338 Moon Sovereign in Void Throne Room
```

### Boss Loot Tables (l/L extensions on boss mobs)

Each boss carries its piece shard plus 3 gear items (Moon Sovereign: shard + 4 gear).
`loot_amount`: Set based on guaranteed drops. Each boss uses a dedicated loot slot per item.

Example for Moon Gate Guardian (mob 3139):
```
l 400 3139 3205 3206 3207 0 0 0 0 0 0
L 25 25 25 25 0 0 0 0 0
```
This guarantees 4 drops: the piece shard (3139 object), guardian gauntlet (3205), basalt maul (3206), moon gate sash (3207), each with equal 25% selection weight. The Moon Sovereign is structured similarly with 5 slots.

### Strong Mob Resets (limit 2–3 each)

```
M 0 3147 3 3176 cursed moon shade (multiple rooms)
M 0 3147 3 3155 cursed moon shade (another spawn)
M 0 3148 2 3152 obsidian colossus
M 0 3148 2 3187 obsidian colossus
M 0 3149 2 3233 death-sworn knight
M 0 3149 2 3245 death-sworn knight
M 0 3150 2 3306 magma drake
M 0 3150 2 3317 magma drake
M 0 3151 2 3286 jade sphinx
M 0 3151 2 3297 jade sphinx
M 0 3199 2 3152 toxic sand elemental
M 0 3199 2 3187 toxic sand elemental
M 0 3200 2 3235 ancient basalt guardian
M 0 3200 2 3252 ancient basalt guardian
M 0 3201 2 3258 greater bone overlord
M 0 3201 2 3277 greater bone overlord
M 0 3316 2 3324 void-touched juggernaut
M 0 3316 2 3330 void-touched juggernaut
M 0 3317 2 3299 cistern matriarch
M 0 3317 2 3305 cistern matriarch
M 0 3318 2 3146 petrified desert titan
M 0 3318 2 3164 petrified desert titan
M 0 3319 2 3224 fungal colossus
M 0 3319 2 3230 fungal colossus
M 0 3320 2 3260 nightmare stalker
M 0 3320 2 3271 nightmare stalker
```

### Trash Mob Resets (representative sample, ~60–80 total resets)

Distributed across zones by level:
- **Ground Level (3140–3183):** Desert tomb scarab swarms, moon-cursed skeletons, desiccated mummies, venomous scorpions, grave robber's specters, sand hyenas, basalt guardian statues, canopic abominations, animated obelisk shards, cursed pyramid laborers, desert pit vipers, obsidian sentinel constructs, carrion fly swarms, skeletal desert rats, bone crawlers, sand-choked revenants, tomb guardian jackals, dust devil spirits, embalmed crocodiles, petrified sand wurms (~36 resets)
- **Tier 1 — Silent Crypts (3185–3218):** Jade asps, desert vultures, obsidian golems, basalt statues, cursed laborers, sentinel constructs, moon glyph guardians, animated burial jars, scribal phantoms, sand-bound mummy thralls, scarab swarms, desiccated mummies, whispering wraiths, calcified serpents, tomb beetle colossi, petrified sand wurms, embalmed crocodiles (~26 resets)
- **Tier 2 — Rotting Galleries (3220–3248):** Decay acolytes, rot wights, stone scarab sentinels, tomb cobras, animated burial jars, mummy thralls, rot priest thralls, fungal wraiths, curse-bound sentinels, mold-ridden ghouls, bloated corpse walkers, decay spiders, mildew phantoms, calcified serpents, tomb beetle colossi (~22 resets)
- **Tier 3 — Shadow Nave (3250–3273):** Shadow archers, great shadow vultures, shadow face guardians, creeping shadows, shadow assassins, fungal wraiths, curse-bound sentinels, entombed generals, bone golems, crypt stalkers, shadow hounds, nightmare sentinels, shade dancers, darkfire wisps, mildew phantoms (~20 resets)
- **Tier 4 — Ossuary Depths (3275–3293):** Animated bone pillars, venomous golden cobras, bone cultists, death hawks, bone revenants, bone golems, ossuary spiders, marrow elementals, skeletal war drummers, calcite horrors, darkfire wisps, shadow assassins, creeping shadows (~18 resets)
- **Tier 5 — Venomous Cisterns (3295–3313):** Void specters, venomous golden cobras, death hawks, magma lizards, charred bone husks, venom puddle oozes, cistern lurkers, poison dart frog constructs, toxic gas wraiths, moon-touched scorpions, marrow elementals, calcite horrors (~14 resets)
- **Tier 6 — Molten Sanctum (3315–3327):** Magma lizards, charred bone husks, void specters, lava crawlers, ember revenants, slag golems, obsidian gargoyles, moon-touched scorpions, toxic gas wraiths (~12 resets)
- **Tier 7 — Void Throne (3329–3337):** Void sentinels, gilded moon shades, void acolytes, sovereign's tomb guardians, obsidian gargoyles (~8 resets)

### Equipment Resets (E/G after mob M)

Each trash mob is followed by one `E` reset for their associated reset gear (items delivered via reset do not need ITEM_LOOT). Boss mobs have their gear delivered through loot tables only (no E resets on bosses — loot tables handle all boss drops).

### Object Resets (O commands, items in rooms)

```
O 0 3203 1 3288 Bone-barred key in The Reliquary (Ossuary Depths)
```

### Door Resets (D commands)

Per the Doors Summary table above:
```
D 0 2558 2 1  charter road south (Great Oasis basin side, closed)
D 0 3148 2 1  moon gate (closed)
D 0 3149 0 1  moon gate reverse (closed)
D 0 3180 2 1  obsidian door (closed)
D 0 3181 0 1  obsidian door reverse (closed)
D 0 3183 2 1  lunar vault door (closed)
D 0 3184 0 1  lunar vault door reverse (closed)
D 0 3218 2 1  glyph-sealed door (closed)
D 0 3219 0 1  glyph-sealed door reverse (closed)
D 0 3199 5 1  corroded iron hatch (closed)
D 0 3220 4 1  corroded iron hatch reverse (closed)
D 0 3248 2 1  moss-covered stone door (closed)
D 0 3249 0 1  moss-covered stone door reverse (closed)
D 0 3238 5 1  rusted grate (closed)
D 0 3250 4 1  rusted grate reverse (closed)
D 0 3273 2 1  shadow-wreathed portal (closed)
D 0 3274 0 1  shadow-wreathed portal reverse (closed)
D 0 3293 2 1  skull-arch threshold (closed)
D 0 3294 0 1  skull-arch threshold reverse (closed)
D 0 3290 5 2  bone-barred gate (locked)
D 0 3295 4 2  bone-barred gate reverse (locked)
D 0 3313 2 1  venom-slick gate (closed)
D 0 3314 0 1  venom-slick gate reverse (closed)
D 0 3327 2 1  molten-iron gate (closed)
D 0 3328 0 1  molten-iron gate reverse (closed)
D 0 3323 5 2  obsidian sentinel gate (locked)
D 0 3329 4 2  obsidian sentinel gate reverse (locked)
D 0 3337 2 1  void chamber door (closed)
D 0 3338 0 1  void chamber door reverse (closed)
```

---

## Specials

```
#SPECIALS
M 3139 spec_cast_mage   Moon Gate Guardian
M 3140 spec_cast_undead   Silent Warden
M 3141 spec_cast_mage    Rot Warden
M 3142 spec_cast_bigtime  Shadow Warden
M 3143 spec_cast_mage    Bone Warden
M 3144 spec_poison     Venom Warden
M 3145 spec_breath_fire   Fire Warden
M 3146 spec_cast_bigtime  Moon Sovereign
M 3150 spec_breath_fire   magma drake
M 3155 spec_poison     venomous desert scorpion
M 3156 spec_cast_undead   grave robber's specter
M 3160 spec_poison     jade asp
M 3163 spec_cast_mage    decay acolyte
M 3164 spec_cast_undead   rot wight
M 3166 spec_poison     tomb cobra
M 3170 spec_cast_undead   creeping shadow
M 3173 spec_poison     venomous golden cobra
M 3174 spec_cast_mage    bone cultist
M 3177 spec_cast_undead   void specter
M 3178 spec_poison     carrion fly swarm
M 3179 spec_poison     skeletal desert rat
M 3181 spec_cast_undead   sand-choked revenant
M 3183 spec_cast_undead   cursed pyramid laborer
M 3184 spec_poison     desert pit viper
M 3187 spec_cast_undead   animated burial jar
M 3188 spec_cast_mage    scribal phantom
M 3190 spec_cast_mage    rot priest's thrall
M 3191 spec_cast_undead   fungal wraith
M 3199 spec_cast_mage    toxic sand elemental
M 3201 spec_cast_undead   greater bone overlord
M 3290 spec_cast_undead   whispering wraith
M 3291 spec_poison     calcified serpent
M 3293 spec_poison     mold-ridden ghoul
M 3295 spec_poison     decay spider
M 3296 spec_cast_undead   mildew phantom
M 3300 spec_cast_mage    darkfire wisp
M 3301 spec_poison     ossuary spider
M 3303 spec_cast_mage    skeletal war drummer
M 3305 spec_poison     venom puddle ooze
M 3307 spec_poison     poison dart frog construct
M 3308 spec_cast_undead   toxic gas wraith
M 3310 spec_cast_mage    ember revenant
M 3313 spec_cast_bigtime  void acolyte
M 3315 spec_poison     moon-touched scorpion
M 3317 spec_poison     cistern matriarch
M 3318 spec_cast_mage    petrified desert titan
M 3319 spec_poison     fungal colossus
S
```

**Spec compliance notes:**
- `spec_disease` does not appear in the allowed `spec_fun_name` list in `area_file_spec.md` (section 10). Carrion fly swarm (3178), skeletal desert rat (3179), mold-ridden ghoul (3293), and fungal colossus (3319) were originally assigned `spec_disease` but have been corrected to `spec_poison`, which is the closest valid behavior for disease-vector mobs.
- Color theme accent was originally `@@e` (described as "dark green") but `@@e` in the colist is `light_red`, not green. Corrected to `@@G` (green) to match the thematic intent of jade inlay, verdigris, and poisonous lichen.

---

## Item Flag Reference

| Category | Item flag composition | Numeric value |
|----------|-----------------------|---------------|
| Boss piece shards (loot) | NODROP+BOSS+MAGIC+LOOT | 201326784 |
| Assembly pieces (created) | NODROP+BOSS+MAGIC | 134217920 |
| Final Lantern | RARE+NODROP+BOSS+MAGIC+LOOT | 202375360 |
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
