# Area Plan: Cathedral of the Violet Eclipse

## Overview
- **Area Name:** `@@mCathedral of the Violet Eclipse@@N`
- **File Name:** `violet_eclipse_cathedral.are`
- **Keyword:** `violet eclipse cathedral gothic dark`
- **Intended Levels:** 20-35
- **Vnum Range:** `30250-30324`
- **Vnum Count:** 75 (all room vnums used)
- **Reset Rate:** 20
- **Design Intent:** Full rebuild of rooms, mobiles, objects, and resets while retaining current area envelope and using every room.

## Lore Source of Truth
Implementation should follow `violet_eclipse_cathedral_lore.md` directly and maintain explicit continuity with:
- Midgaard's Lantern Reforms / Violet Compact archive politics,
- Gloamvault's witness-offer-bind institutional collapse,
- Kel'Shadra's adjudication-through-erasure legacy.

## Color Theme (strict multi-color doctrine)
The area must use a deliberate multi-color liturgical schema:
- **`@@m` Magenta / Violet:** eclipse authority, canonical doctrine.
- **`@@l` Light blue / Indigo:** archive flow, astral record systems.
- **`@@e` Light red / Crimson:** blood tithe, coercive confession, sentence force.
- **`@@W` White / Silver:** attestation, funerary legitimacy.
- **`@@d` Dark grey / Obsidian:** null seals, name erasure.
- **`@@y` Yellow / Gold:** exceptional decrees, boss regalia, final reward artifacts.
- **`@@a` Light cyan accents:** inherited civic registry markings.
- Always close with `@@N`.

Policy reminders from docs specs:
- Never use `@@k` as thematic color.
- Do not use background or flashing codes.

## #AREA Header Plan
- `Q 16`
- `K violet eclipse cathedral gothic dark~`
- `L @@W(@@a20 35@@W)@@N~`
- `N 8`
- `I 20 35`
- `V 30250 30324`
- `X 0`
- `F 20`
- `U @@mAmethyst@@N and @@dobsidian@@N hues dominate this terror-soaked gothic cathedral.~`
- `O Virant~` (normalize owner casing)
- Keep ACL (`R`/`W`) integration-safe.

## Structural Layout (All 75 Rooms Used)
The cathedral remains a central spine with five branch doctrines and a terminal throne route.

### Central Spine (30250-30264)
1. **30250 Shattered Rose Gate** - world entry from Roc Road; broken civic checkpoint stones visible.
2. **30251 Censer-Fog Vestibule** - intake haze and witness sorting.
3. **30252 Pilgrim's Lament Walk** - penitent movement lane.
4. **30253 Gargoyle Choir Landing** - surveillance balconies.
5. **30254 Narthex of Stilled Wings** - first major junction hub.
6. **30255 Sepulchral Procession** - doctrine corridor toward reliquary branch.
7. **30256 Chalice Echo Hall** - second hub with cloister branch.
8. **30257 Veil of Black Incense** - third hub toward thorn scriptoria.
9. **30258 Mauve Lantern Arcade** - fourth hub toward moonbone wing.
10. **30259 Cathedral Midspine** - convergent interior checkpoint.
11. **30260 Litany Crossing** - mnemonic inscription hall.
12. **30261 Roseglass Transept** - color doctrine teaching room.
13. **30262 Funeral Bell Gallery** - auditory indexing of rites.
14. **30263 Reliquary Axis** - final warning corridor.
15. **30264 Eclipse High Nave** - spine capstone and visual anchor.

### Crimson Sanguine Choir Wing (30265-30276)
16. **30265 Blood Psalm Entry**
17. **30266 Velvet Wound Cloister**
18. **30267 Hemorrhage Chapel**
19. **30268 Dirge-Stained Aisle**
20. **30269 Scarlet Organ Loft**
21. **30270 Sable Choir Stalls**
22. **30271 Crypt-Glass Passage**
23. **30272 Vesper Thorn Niche**
24. **30273 Mourner's Spiral**
25. **30274 Black Mass Rotunda**
26. **30275 Candled Casket Hall**
27. **30276 Throne of the Crimson Matron** (**Wing Boss A**)

### Amethyst Reliquary Wing (30277-30288)
28. **30277 Reliquary Mouth**
29. **30278 Crown of Violet Icons**
30. **30279 Lantern of Closed Eyes**
31. **30280 Cat's-Eye Apse**
32. **30281 Incantor's Crosshall**
33. **30282 Amethyst Sacristy**
34. **30283 Votive Bone Gallery**
35. **30284 Whisper Reliquary**
36. **30285 Starless Relic Court**
37. **30286 Chained Saint Corridor**
38. **30287 Miter of Night**
39. **30288 Seat of Lady Noctyra** (**Wing Boss B**)

### Obsidian Cloister Wing (30289-30300)
40. **30289 Basalt Threshold**
41. **30290 Ink-Well Cloister**
42. **30291 Abjurer's Cross**
43. **30292 Obsidian Lectern Hall**
44. **30293 Sealed Homily Lane**
45. **30294 Nocturne Scriptor**
46. **30295 Blind Archivum**
47. **30296 Wax-Etched Court**
48. **30297 Ash Quire Stacks**
49. **30298 Penitent's Margin**
50. **30299 Vellum Catacomb**
51. **30300 Abbot Vharos's Reliquary** (**Wing Boss C**)

### Thorned Scriptorium Wing (30301-30312)
52. **30301 Briar Stair**
53. **30302 Gothic Script Gate**
54. **30303 Splintered Margin**
55. **30304 Thorn-Pierced Hall**
56. **30305 Cardinal's Footway**
57. **30306 Ivory Quill Court**
58. **30307 Sanguine Annotation**
59. **30308 Binder's Ossuary**
60. **30309 Inkblight Rotunda**
61. **30310 Razorleaf Balcony**
62. **30311 Needled Crossing**
63. **30312 Sanctum of the Thorn Cardinal** (**Wing Boss D**)

### Moonbone Ossuary Wing + Finale (30313-30324)
64. **30313 Moonbone Entry**
65. **30314 Silver Shroud Court**
66. **30315 Pale Sarcophagus Line**
67. **30316 Noctilucent Pillars**
68. **30317 Widow's Reliquary**
69. **30318 Echoing Ossicle Hall**
70. **30319 Sable Mortuary Walk**
71. **30320 Corpse-Lily Atrium**
72. **30321 Maw of Quiet Bells**
73. **30322 Nightglass Corridor**
74. **30323 Penumbral Last Rite**
75. **30324 Throne of the Violet Hierophant** (**Final Boss**)

## Connectivity Requirements
- Preserve linear readability through each wing while retaining return loops to the spine.
- Branch entry points remain at:
  - `30254 -> 30265`
  - `30255 -> 30277`
  - `30256 -> 30289`
  - `30257 -> 30301`
  - `30258 -> 30313`
- Final route pressure should escalate from `30313` onward with minimal bypass.
- Keep world ingress at `30250` and no alternate external escape except intentional builder hook.

## Mobile Redesign Plan (30250-30304)
Rebuild all existing mob vnums into role-defined institutional castes.

### Tier Distribution
- **30250-30259:** intake vergers, censer bailiffs, processional sentries.
- **30260-30269:** choir inquisitors and blood-psalm enforcers.
- **30270-30279:** reliquary clerics, icon wardens, relic adjudicators.
- **30280-30289:** null scribes, seal notaries, archive suppressors.
- **30290-30299:** thorn canonicalists, penitence marshals, ossuary custodians.
- **30300-30304:** elite/boss identities.

### Boss Mob Mapping
- **30300:** The Crimson Matron Ilyra (30276 encounter)
- **30301:** Lady Noctyra, Mirrored Censer Regent (30288)
- **30302:** Abbot Vharos, Null-Halo Archivist (30300)
- **30303:** Thorn Cardinal Seredh (30312)
- **30304:** The Violet Hierophant (30324)

## Object Redesign Plan (30250-30319)
All object vnums should be rebuilt into progression sets that reflect the multi-color doctrine.

- **30250-30269 (Entry/Spine):** balanced gear with survivability + hitroll.
- **30270-30289 (Wing Midgame):** identity sets by wing (crimson offense, violet control, obsidian null).
- **30290-30304 (Elite/Boss drops):** named signature equipment with inscription text.
- **30305-30319 (Quest/chain rewards and rare reliquary pieces):** high-fantasy relics tied to Compact and eclipse precedent.

## Reset and Encounter Cadence
- Each wing must include:
  - ambient patrol pair,
  - one mini-elite checkpoint,
  - one pre-boss gate fight.
- Boss rooms should include ceremonial object resets supporting encounter identity.
- Rare chance resets in central spine should hint at all five office colors.

## Proposition / Quest Plan for Violet Eclipse Cathedral

### Existing Proposition Status
No existing `.prop` templates currently target `30250-30324`; therefore new cathedral-specific propositions are required.

### Static Quest ID Allocation
Use IDs **61-65** (`62.prop` through `66.prop`) for this area's first proposition set.
- Note: if runtime cap remains `PROP_MAX_STATIC_QUESTS = 48`, these require cap increase before live load.

Offerer vnums:
- Midgaard postmaster: `3015`
- Kiess postmaster: `13001`
- Kowloon postmaster: `14021`

---

### Quest 61 — Compact intake disruption sweep (non-chain)
| Field | Value |
|---|---|
| File | `62.prop` |
| static_id | 61 |
| Title | Compact intake disruption sweep |
| prerequisite_static_id | -1 |
| type | 1 |
| num_targets | 3 |
| kill_needed | 0 |
| level range | 22-30 |
| offerer_vnum | 3015 |
| reward_gold | 2400 |
| reward_qp | 3 |
| reward_item_vnum | 0 |
| reward_item_count | 0 |
| targets | `30252 30255 30261` |

---

### Quest 62 — Reliquary regent injunction (chain start, boss equipment)
| Field | Value |
|---|---|
| File | `63.prop` |
| static_id | 62 |
| Title | Reliquary regent injunction |
| prerequisite_static_id | -1 |
| type | 3 |
| num_targets | 1 |
| kill_needed | 1 |
| level range | 27-34 |
| offerer_vnum | 13001 |
| reward_gold | 3800 |
| reward_qp | 4 |
| reward_item_vnum | 30305 |
| reward_item_count | 1 |
| targets | `30301` |

**Custom reward object (boss proposition equipment):**
- short: `@@mNoctyra's Mirrored Censer Chain@@N`
- keywords: `noctyra mirrored censer chain violet`
- long: `A mirrored censer chain swings in a slow, soundless arc.`
- wear flags: neck
- extra flags: glow + hum
- role: caster/control neck item with save bonus and mana focus.

---

### Quest 63 — Null-halo archive seizure (chain middle, boss equipment)
| Field | Value |
|---|---|
| File | `64.prop` |
| static_id | 63 |
| Title | Null-halo archive seizure |
| prerequisite_static_id | 62 |
| type | 1 |
| num_targets | 2 |
| kill_needed | 0 |
| level range | 29-35 |
| offerer_vnum | 13001 |
| reward_gold | 4600 |
| reward_qp | 5 |
| reward_item_vnum | 30306 |
| reward_item_count | 1 |
| targets | `30296 30302` |

**Custom reward object (boss proposition equipment):**
- short: `@@dVharos's Null-Sigil Bracers@@N`
- keywords: `vharos null sigil bracers obsidian`
- long: `A pair of obsidian bracers etched with erasure seals rests here.`
- wear flags: arms
- extra flags: glow
- role: hybrid anti-magic defensive bracers (saves + spell dampening flavor).

---

### Quest 64 — Thorn cardinal writ execution (chain middle, boss equipment)
| Field | Value |
|---|---|
| File | `65.prop` |
| static_id | 64 |
| Title | Thorn cardinal writ execution |
| prerequisite_static_id | 63 |
| type | 3 |
| num_targets | 1 |
| kill_needed | 1 |
| level range | 31-35 |
| offerer_vnum | 14021 |
| reward_gold | 5600 |
| reward_qp | 6 |
| reward_item_vnum | 30307 |
| reward_item_count | 1 |
| targets | `30303` |

**Custom reward object (boss proposition equipment):**
- short: `@@eSeredh's Thornscript Girdle@@N`
- keywords: `seredh thornscript girdle crimson`
- long: `A barbed ritual girdle lies coiled beside a torn vellum sash.`
- wear flags: waist
- extra flags: glow
- role: melee-leaning utility belt (hit/dam + bleed-themed proc flavor).

---

### Quest 65 — Violet throne final deposition (chain end, final quest equipment)
| Field | Value |
|---|---|
| File | `66.prop` |
| static_id | 65 |
| Title | Violet throne final deposition |
| prerequisite_static_id | 64 |
| type | 3 |
| num_targets | 1 |
| kill_needed | 1 |
| level range | 33-35 |
| offerer_vnum | 3015 |
| reward_gold | 7600 |
| reward_qp | 8 |
| reward_item_vnum | 30308 |
| reward_item_count | 1 |
| targets | `30304` |

**Custom reward object (final chain proposition equipment):**
- short: `@@yEclipse Verdict Regalia@@N`
- keywords: `eclipse verdict regalia gold violet hierophant`
- long: `A gold-edged violet regalia mantle is folded with ritual precision.`
- wear flags: about body
- extra flags: glow + hum + anti-evil (if available in table)
- role: final chain signature armor balancing survivability, saves, and caster-melee crossover stats.

## Implementation Checklist
1. Rewrite all room descriptions to remove repetitive templating and reflect wing-specific doctrine.
2. Ensure all 75 room vnums remain present and connected.
3. Rebuild 55 mobs and 70 objects into functional ecclesiastical castes and gear tiers.
4. Re-author resets for patrol cadence and boss ceremonial staging.
5. Add proposition files `62.prop`-`66.prop` with above templates and custom reward blocks.
6. Validate color policy compliance (`@@k` absent, no background/flashing codes).
7. Confirm owner normalization in header (`O Virant~`).
