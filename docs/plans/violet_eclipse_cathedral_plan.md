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

Per `area_file_spec.md` section 3, the `#AREA` section must open with the area name string and include `Q 16` (mandatory version directive).

```text
#AREA
@@mCathedral of the Violet Eclipse@@N~
Q 16
K violet eclipse cathedral gothic dark~
L @@W(@@a20 35@@W)@@N~
N 8
I 20 35
V 30250 30324
X 0
F 20
U @@mAmethyst@@N and @@dobsidian@@N hues dominate this terror-soaked gothic cathedral.~
O Virant~
```

Directive compliance notes:
- `Q 16`: mandatory, no other value accepted.
- `K`: keyword string, tilde-terminated.
- `L`: level label string with color codes; uses only approved `colist` codes (`@@W`, `@@a`, `@@N`).
- `N 8`: area number (integer).
- `I 20 35`: min/max level.
- `V 30250 30324`: vnum envelope. All `#ROOMS`, `#MOBILES`, and `#OBJECTS` entry vnums must stay within this range per structural constraint (section 13).
- `X 0`: map offset.
- `F 20`: reset rate (ticks between area resets).
- `U`: reset message, tilde-terminated, uses approved color codes only.
- `O Virant~`: owner per mandatory owner policy.
- `R`/`W` ACL directives: omitted (defaults). Add only if explicit access control is needed.

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
  - `30254 -> 30265` (Narthex hub south to Crimson Sanguine Choir)
  - `30255 -> 30277` (Sepulchral Procession east to Amethyst Reliquary)
  - `30256 -> 30289` (Chalice Echo Hall west to Obsidian Cloister)
  - `30257 -> 30301` (Veil of Black Incense east to Thorned Scriptorium)
  - `30258 -> 30313` (Mauve Lantern Arcade down to Moonbone Ossuary)
- Final route pressure should escalate from `30313` onward with minimal bypass.
- Keep world ingress at `30250` and no alternate external escape except intentional builder hook.

### Exit Specification Compliance (per `room_spec.md`)

All exits must follow `D<door>` format with three fields: `<exit_description>~`, `<exit_keyword>~`, and `<locks> <key_vnum> <to_room_vnum>`.

- **Bi-directional requirement:** Except in mazes, every exit from room A to room B via direction X must have a corresponding opposite-direction exit from B to A. No rooms in this area use `ROOM_MAZE`.
- **No directional loops:** Repeated movement in any single direction must not cycle back to a previously visited room.
- **Named exits:** When used, the exit keyword must appear in the room's main description, an object in the room, or an extra description. Prefix door-style named exits with `^` (e.g., `^iron gate~`).
- **Door key policy:** If a door resets to `locked` (state `2` via `#RESETS` command `D`), `<key_vnum>` must reference a valid key object (`item_type = 18`) that exists in `#OBJECTS`. Use `-1` for unlockable exits.
- **Door direction mapping:** 0=north, 1=east, 2=south, 3=west, 4=up, 5=down.

### Planned Locked Doors

| Room | Direction | Keyword | Key Vnum | Notes |
|---|---|---|---|---|
| 30264 (Eclipse High Nave) | north to 30313 | `^eclipse gate` | 30250 | Unlocks Moonbone descent; key drops from mini-elite |
| 30276 (Throne of the Crimson Matron) | south (return) | `^bloodstained door` | -1 | Closes on entry, not locked |
| 30312 (Sanctum of the Thorn Cardinal) | south (return) | `^thorned iron gate` | -1 | Closes on entry, not locked |
| 30323 (Penumbral Last Rite) | north to 30324 | `^violet threshold` | 30251 | Final boss gate; key from pre-boss encounter |

## Room Specification Details (per `room_spec.md`)

### Room Flags Policy

All rooms use these base `room_flags` values:

- **Standard cathedral rooms:** `indoors` (8) — all rooms are enclosed indoor spaces.
- **Boss rooms (30276, 30288, 30300, 30312, 30324):** `indoors | no_mob` (8 | 4 = 12) — boss rooms must be flagged `no_mob` per mob spec boss placement policy.
- **Entry room 30250 (Shattered Rose Gate):** `indoors` (8) — the entry threshold is covered but visually transitional.
- No rooms use `no_recall`, `no_teleport`, `no_magic`, `dark`, `safe`, `pk`, or `maze` unless encounter design demands it.

### Sector Type

All rooms: `inside` (11) — the entire area is an enclosed cathedral structure.

### Room Description Requirements

Per `room_spec.md`:
- Every room description must contain at least 3 sentences (5+ for important rooms like hubs and boss chambers).
- Every room description must be unique.
- Descriptions must end with exactly one trailing newline before `~`.
- Descriptions must not contain vnums.
- Named exit keywords must be discoverable in the room description, an object, or an extra description.

### Extra Description Policy

Each room should include at least one `E` extra description for environmental detail (inscriptions, architecture, objects of interest). Extra description chains are allowed but must be anchored: at least one keyword in the chain must appear in the room's main description.

## Mobile Redesign Plan (30250-30304)

Rebuild all existing mob vnums into role-defined institutional castes. Per `mob_spec.md`, each mobile record requires: `player_name~`, `short_descr~`, `long_descr~` (strict single-line format), `description~`, main data line (`<act> <affected_by> <alignment> S`), stats line (`<level> <sex>`), and modifier line (`<hp_mod> <ac_mod> <hr_mod> <dr_mod>`).

### Mandatory Act Flags (per `mob_spec.md` builder policy)

- **All mobs:** must include `is_npc` (1) + `stay_area` (64) = minimum `act` base of 65.
- **Boss mobs (30300-30304):** must additionally include `sentinel` (2) + `boss` (67108864) = `act` includes `1 | 2 | 64 | 67108864 = 67108931`.
- **Strong non-boss mobs (mini-elites/gate guards):** must include `solo` (33554432) + `sentinel` (2).
- **Patrol mobs (ambient wanderers within a wing):** `stay_area` (64) only; `sentinel` omitted so they wander within their wing. Each wing must be enclosed by `no_mob` rooms or all patrol mobs must be `sentinel` to prevent cross-wing migration.
- **Sentinel mobs (fixed-position guards):** `sentinel` (2) + `stay_area` (64).
- **Aggressive mobs:** Add `aggressive` (32) for mobs that initiate combat on sight (most cathedral enforcers). Omit for neutral NPCs like processional vergers or ambient clergy.
- **Undead mobs (spectral entities):** Add `undead` (16384) where lore-appropriate (spectral witnesses, bound spirits).
- **`invasion` flag:** Must NEVER be set in area files.

### Wing Containment Strategy

To prevent mobs from wandering between wings, each wing branch uses one of these strategies:
- **Option A:** Flag all wing patrol mobs as `sentinel`.
- **Option B:** Flag the wing entry room with `no_mob` (4) in `room_flags`, creating a barrier. This conflicts with placing mobs in the entry room, so sentinel is preferred for entry-adjacent mobs.

Recommended approach: all mobs in this area are `sentinel` except where specific patrol behavior is designed. Patrol pairs within a wing should have their wing's rooms flagged so mobs cannot escape the wing into the spine.

### Mob Extension Lines

All mobs should include the `!` extension line for class, race, position, and combat data:
```text
! <class> <clan> <race> <position> <skills> <cast> <def>
```

Guidance per mob role:
- **Melee enforcers (sentries, marshals, custodians):** hybrid or melee profile skills, `cast` = 0 or minimal, `def` = `cure_light` (2) or `cure_serious` (4).
- **Caster clergy (scribes, clerics, cantors):** caster profile skills, `cast` includes appropriate offensive spells (e.g., `flamestrike` 2048, `faerie_fire` 512, `mind_flail` 8192), `def` includes `heal` (16) or `cure_critic` (8).
- **Boss mobs:** full extension lines (`!`, `|`, `+`) with tuned stats. Include loot table (`l`/`L`) for boss drops.

### Level Distribution (area range 20-35)

| Tier | Vnums | Level Range | Role |
|---|---|---|---|
| Spine trash | 30250-30254 | 20-23 | Intake vergers, censer bailiffs |
| Spine mid | 30255-30259 | 24-27 | Processional sentries, litany marshals |
| Wing trash | 30260-30269 | 22-26 | Choir inquisitors, blood-psalm enforcers |
| Wing mid | 30270-30279 | 25-29 | Reliquary clerics, icon wardens |
| Wing upper | 30280-30289 | 27-31 | Null scribes, seal notaries, archive suppressors |
| Wing elite | 30290-30299 | 29-33 | Thorn canonicalists, penitence marshals, ossuary custodians |
| Bosses | 30300-30304 | 30-35 | Named boss encounters |

### Boss Mob Mapping

| Mob Vnum | Name | Encounter Room | Level | Act Flags |
|---|---|---|---|---|
| 30300 | The Crimson Matron Ilyra | 30276 | 30 | `is_npc sentinel stay_area boss aggressive` |
| 30301 | Lady Noctyra, Mirrored Censer Regent | 30288 | 32 | `is_npc sentinel stay_area boss aggressive` |
| 30302 | Abbot Vharos, Null-Halo Archivist | 30300 | 33 | `is_npc sentinel stay_area boss aggressive` |
| 30303 | Thorn Cardinal Seredh | 30312 | 34 | `is_npc sentinel stay_area boss aggressive` |
| 30304 | The Violet Hierophant | 30324 | 35 | `is_npc sentinel stay_area boss aggressive` |

All boss encounter rooms (30276, 30288, 30300, 30312, 30324) must have `no_mob` (4) set in `room_flags`.

### Mob `long_descr` Format Reminder

Per `mob_spec.md`: `long_descr` must be exactly one text line, followed by a newline, followed by a line containing only `~`. Multi-line `long_descr` is never valid. Example:
```text
@@mThe Crimson Matron Ilyra@@N stands here, blood-psalms dripping from her lips.
~
```

### Mob `description` Format Reminder

`description` must end with exactly one trailing newline before `~`. No extra blank lines.

## Object Redesign Plan (30250-30319)

All object vnums should be rebuilt into progression sets that reflect the multi-color doctrine. Per `object_spec.md`, each object record requires: `name~`, `short_descr~`, `description~`, header line (`<item_type> <extra_flags> <wear_flags> <item_apply>`), values line (10 integers), and `<weight>`.

### Mandatory Object Policies (per `object_spec.md`)

- **`ITEM_TAKE` (8388608):** Every object must include this in `wear_flags`.
- **`ITEM_BOSS` (134217728):** Any item that drops from a boss mob (via loot table or reset equipment) must include this in `extra_flags`.
- **`ITEM_LOOT` (67108864):** Items spawned from mob loot tables (`l`/`L` extension) must include this in `extra_flags`.
- **`ITEM_GENERATED` (1):** Must NOT be set in area files (runtime-only).
- **`ITEM_WEAR_CLAN_COLORS` (16777216):** Must NOT be included in any object.
- **Unique names:** No two objects in this area may share the same `name~` value.
- **No fixed stats:** Per item stat policy, area-authored items must not define fixed stats. Stats are generated at runtime from item level.
- **Weight encodes archetype:** 1-5 = caster item, 6-10 = melee item, 11-15 = tank item.
- **Weapon constraints:** `item_type = 5` (weapon) requires `wear_flags` to include both `hold` (32768) and `take` (8388608). `value3` must be thematically consistent with the weapon concept. Two-handed weapon presentations must include `ITEM_TWO_HANDED` (2147483648).

### Object Tier Distribution

| Tier | Vnums | Item Level (L) | Role | Weight Range |
|---|---|---|---|---|
| Entry/Spine | 30250-30259 | 20-24 | Balanced survivability gear | Mixed 1-15 |
| Spine upper | 30260-30269 | 24-28 | Progression gear, first color-themed pieces | Mixed 1-15 |
| Wing midgame | 30270-30279 | 26-30 | Crimson offense set pieces | 6-10 (melee focus) |
| Wing midgame | 30280-30289 | 26-30 | Violet control / Obsidian null set pieces | 1-5 (caster focus) |
| Boss drops | 30290-30299 | 30-34 | Named signature equipment with inscription `E` text | Mixed |
| Boss drops | 30300-30304 | 32-35 | Elite boss signature items | Mixed |
| Quest rewards | 30305-30308 | 28-35 | Chain quest reward objects (see quest section) | Per role |
| Rare relics | 30309-30319 | 28-35 | Reliquary pieces tied to Compact/eclipse lore | Mixed |

### Key Object Definitions

Key objects required for locked doors:

| Vnum | Name | Item Type | Wear Flags | Notes |
|---|---|---|---|---|
| 30250 | `eclipse gate key` | 18 (key) | `take` (8388608) | Unlocks 30264->30313 eclipse gate |
| 30251 | `violet threshold key` | 18 (key) | `take` (8388608) | Unlocks 30323->30324 final boss gate |

### Boss Drop Objects (must include `ITEM_BOSS`)

All items dropped by or equipped on boss mobs must have `ITEM_BOSS` (134217728) in `extra_flags`. If delivered via loot table, also add `ITEM_LOOT` (67108864).

### Extra Description (`E` entry) Policy

Named signature items (boss drops, quest rewards) should include at least one `E` extra description with inscription text tying the item to Compact-era or eclipse-doctrine lore. Example:
```text
E
inscription~
Etched in ash shorthand beneath the violet lacquer: "What is sentenced
endures beyond the speaker. — Decree of the Veiled Office, Year of
Unsounded Bells."
~
```

### Item Type Reference (commonly used in this area)

- `9` (armor): body armor, shields, worn equipment
- `5` (weapon): held weapons (requires `hold` + `take` in wear_flags)
- `1` (light): light sources
- `18` (key): door keys
- `8` (treasure): decorative/trade items
- `15` (container): chests, reliquary boxes
- `10` (potion): consumable vials
- `25` (fountain): fixed water sources

## Reset and Encounter Cadence

Per `area_file_spec.md` section 12, resets use the `#RESETS` section terminated by `S`. All reset vnums must reference valid entries.

### Reset Command Reference

- `M <ifflag> <mob_vnum> <limit> <room_vnum>` — Load mob into room.
- `O <ifflag> <obj_vnum> <limit> <room_vnum>` — Load object into room.
- `G <ifflag> <obj_vnum> <limit>` — Give object to previous mob (no arg3).
- `E <ifflag> <obj_vnum> <limit> <wear_loc>` — Equip object on previous mob.
- `D <ifflag> <room_vnum> <door> <state>` — Set door state (0=open, 1=closed, 2=locked).
- `P <ifflag> <obj_vnum> <limit> <container_vnum>` — Put object into container.
- No blank lines allowed within `#RESETS`.

### Door Reset Plan

| Room | Door Dir | State | Key Vnum | Notes |
|---|---|---|---|---|
| 30264 | north (0) | 2 (locked) | 30250 | Eclipse gate to Moonbone wing |
| 30276 | south (2) | 1 (closed) | -1 | Crimson Matron boss room return |
| 30312 | south (2) | 1 (closed) | -1 | Thorn Cardinal boss room return |
| 30323 | north (0) | 2 (locked) | 30251 | Violet threshold to final boss |

All doors reset via `D` must have `EX_ISDOOR` (1) set in the room's `D<door>` exit `<locks>` field. Locked doors (state 2) must have a valid key vnum pointing to an existing key object in `#OBJECTS`.

### Wing Encounter Cadence

Each wing must include:
- **Ambient patrol pair:** 2 standard-tier mobs reset in non-boss rooms, providing consistent combat density.
- **One mini-elite checkpoint:** A `solo`-flagged mob guarding progression deeper into the wing. Reset in a room before the wing's midpoint.
- **One pre-boss gate fight:** A `solo`-flagged mob in the penultimate room before the boss chamber.
- **Boss reset:** Single `M` reset for the boss mob in its `no_mob`-flagged boss room. Boss equipment loaded via `E` resets (equip on boss) or defined in the mob's loot table (`l`/`L` extension).

### Spine Encounter Cadence

- Rooms 30250-30254 (entry zone): 2-3 trash mobs per room, intake vergers and censer bailiffs.
- Rooms 30255-30264 (mid-spine): 1-2 mobs per room with escalating levels, processional sentries.
- Decorative/atmospheric object resets (`O`) in spine rooms for ambiance (censers, ledgers, broken seals).

### Boss Encounter Object Resets

Each boss room should include 1-2 `O` resets for ceremonial objects (thematic props that reinforce the boss identity):
- 30276 (Crimson Matron): blood-psalm lectern, crimson candelabra.
- 30288 (Lady Noctyra): mirrored censer stand, violet icon shelf.
- 30300 (Abbot Vharos): null-seal writing desk, obsidian ledger rack.
- 30312 (Thorn Cardinal): thorn-pierced confession rail, ink-stained vellum stack.
- 30324 (Violet Hierophant): eclipse throne pedestal, gold-edged verdict scroll rack.

### Spine Atmospheric Object Resets

Rare chance objects in the central spine should hint at all five office colors (one per color doctrine):
- A crimson-stained psalm fragment (crimson/light red).
- A violet-lacquered registry seal (magenta).
- An obsidian null-stamp (dark grey).
- A silver funerary attestation token (white).
- A gold-edged decree fragment (yellow).

### #SPECIALS and #OBJFUNS Plan

Per `area_file_spec.md` sections 10-11:
- **Boss caster mobs:** Assign `spec_cast_cleric` or `spec_cast_undead` where lore-appropriate.
- **Boss melee mobs:** No special needed unless specific behavior is desired.
- **`spec_summon_*` specials:** Must NEVER be set in area files (runtime only).
- **`spec_keep*` specials:** Must NOT be set by automation.
- **Object functions:** Consider `objfun_soul_moan` for spectral boss-drop items, `objfun_healing` for recovery relics. Assignment is optional.

Proposed `#SPECIALS`:
```text
#SPECIALS
M 30300 spec_cast_cleric
M 30301 spec_cast_cleric
M 30302 spec_cast_undead
M 30303 spec_cast_cleric
M 30304 spec_cast_judge
S
```

## Proposition / Quest Plan for Violet Eclipse Cathedral

### Existing Proposition Status
No existing `.prop` templates currently target `30250-30324`; therefore new cathedral-specific propositions are required.

### Static Quest ID Allocation
Use IDs **61-65** (`62.prop` through `66.prop`) for this area's first proposition set.
- **IMPORTANT:** The runtime cap is `PROP_MAX_STATIC_QUESTS = 48`, meaning files above `48.prop` are not loaded. These quests (62-66) require the cap to be increased before live load. Files 50-56 already exist above the cap and have the same constraint.

### Quest File Format Reference (per `quests_spec.md`)

Each `.prop` file structure:
1. Title line (freeform text)
2. Numeric definition line: `prerequisite_static_id type num_targets kill_needed min_level max_level offerer_vnum reward_gold reward_qp reward_item_vnum reward_item_count`
3. Target vnum line (space-delimited, up to 5 vnums)
4. Accept message line (optional)
5. Completion message line (optional)
6. Custom reward-object block (optional, 7 lines when present):
   - short description
   - keywords
   - long description
   - wear flags (integer)
   - extra flags (integer)
   - weight (integer)
   - item-apply selector (integer)

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
| type | 1 (kill-variety: kill one of each target) |
| num_targets | 3 |
| kill_needed | 0 |
| level range | 22-30 |
| offerer_vnum | 3015 |
| reward_gold | 2400 |
| reward_qp | 3 |
| reward_item_vnum | 0 |
| reward_item_count | 0 |
| targets | `30252 30255 30261` |

Prop file content:
```text
Compact intake disruption sweep
-1 1 3 0 22 30 3015 2400 3 0 0
30252 30255 30261
The Violet Compact's intake tribunal still processes the unwilling. Disrupt their processional enforcers.
The intake lines have gone quiet. For now.
```

---

### Quest 62 — Reliquary regent injunction (chain start, boss equipment)

| Field | Value |
|---|---|
| File | `63.prop` |
| static_id | 62 |
| Title | Reliquary regent injunction |
| prerequisite_static_id | -1 |
| type | 3 (kill-count against single target) |
| num_targets | 1 |
| kill_needed | 1 |
| level range | 27-34 |
| offerer_vnum | 13001 |
| reward_gold | 3800 |
| reward_qp | 4 |
| reward_item_vnum | 30305 |
| reward_item_count | 1 |
| targets | `30301` |

Custom reward object block (7 lines):
- short: `@@mNoctyra's Mirrored Censer Chain@@N`
- keywords: `noctyra mirrored censer chain violet`
- long: `A mirrored censer chain swings in a slow, soundless arc.`
- wear_flags: `8388736` (`neck` 128 | `take` 8388608)
- extra_flags: `134217792` (`ITEM_MAGIC` 64 | `ITEM_BOSS` 134217728)
- weight: `3` (caster item)
- item_apply: `1` (`nada` — runtime generates stats from level)

Prop file content:
```text
Reliquary regent injunction
-1 3 1 1 27 34 13001 3800 4 30305 1
30301
Lady Noctyra's mirrored censer still swings in the reliquary nave. End her regency.
The censer's arc has stopped. Noctyra's liturgy is broken.
@@mNoctyra's Mirrored Censer Chain@@N
noctyra mirrored censer chain violet
A mirrored censer chain swings in a slow, soundless arc.
8388736
134217792
3
1
```

---

### Quest 63 — Null-halo archive seizure (chain middle, boss equipment)

| Field | Value |
|---|---|
| File | `64.prop` |
| static_id | 63 |
| Title | Null-halo archive seizure |
| prerequisite_static_id | 62 |
| type | 1 (kill-variety) |
| num_targets | 2 |
| kill_needed | 0 |
| level range | 29-35 |
| offerer_vnum | 13001 |
| reward_gold | 4600 |
| reward_qp | 5 |
| reward_item_vnum | 30306 |
| reward_item_count | 1 |
| targets | `30296 30302` |

Custom reward object block:
- short: `@@dVharos's Null-Sigil Bracers@@N`
- keywords: `vharos null sigil bracers obsidian`
- long: `A pair of obsidian bracers etched with erasure seals rests here.`
- wear_flags: `8389632` (`arms` 1024 | `take` 8388608)
- extra_flags: `134217792` (`ITEM_MAGIC` 64 | `ITEM_BOSS` 134217728)
- weight: `12` (tank item)
- item_apply: `1` (`nada`)

Prop file content:
```text
Null-halo archive seizure
62 1 2 0 29 35 13001 4600 5 30306 1
30296 30302
Abbot Vharos's null-seal archivum suppresses all unredacted witness. Seize the archive stacks and silence his scribes.
The null-halos have dimmed. Vharos's archive yields its secrets.
@@dVharos's Null-Sigil Bracers@@N
vharos null sigil bracers obsidian
A pair of obsidian bracers etched with erasure seals rests here.
8389632
134217792
12
1
```

---

### Quest 64 — Thorn cardinal writ execution (chain middle, boss equipment)

| Field | Value |
|---|---|
| File | `65.prop` |
| static_id | 64 |
| Title | Thorn cardinal writ execution |
| prerequisite_static_id | 63 |
| type | 3 (kill-count) |
| num_targets | 1 |
| kill_needed | 1 |
| level range | 31-35 |
| offerer_vnum | 14021 |
| reward_gold | 5600 |
| reward_qp | 6 |
| reward_item_vnum | 30307 |
| reward_item_count | 1 |
| targets | `30303` |

Custom reward object block:
- short: `@@eSeredh's Thornscript Girdle@@N`
- keywords: `seredh thornscript girdle crimson`
- long: `A barbed ritual girdle lies coiled beside a torn vellum sash.`
- wear_flags: `8519680` (`waist` 131072 | `take` 8388608)
- extra_flags: `134217792` (`ITEM_MAGIC` 64 | `ITEM_BOSS` 134217728)
- weight: `8` (melee item)
- item_apply: `1` (`nada`)

Prop file content:
```text
Thorn cardinal writ execution
63 3 1 1 31 35 14021 5600 6 30307 1
30303
Thorn Cardinal Seredh militarized the confession halls. Execute the outstanding writ against his office.
Seredh's thorned quill has scratched its last sentence.
@@eSeredh's Thornscript Girdle@@N
seredh thornscript girdle crimson
A barbed ritual girdle lies coiled beside a torn vellum sash.
8519680
134217792
8
1
```

---

### Quest 65 — Violet throne final deposition (chain end, final quest equipment)

| Field | Value |
|---|---|
| File | `66.prop` |
| static_id | 65 |
| Title | Violet throne final deposition |
| prerequisite_static_id | 64 |
| type | 3 (kill-count) |
| num_targets | 1 |
| kill_needed | 1 |
| level range | 33-35 |
| offerer_vnum | 3015 |
| reward_gold | 7600 |
| reward_qp | 8 |
| reward_item_vnum | 30308 |
| reward_item_count | 1 |
| targets | `30304` |

Custom reward object block:
- short: `@@yEclipse Verdict Regalia@@N`
- keywords: `eclipse verdict regalia gold violet hierophant`
- long: `A gold-edged violet regalia mantle is folded with ritual precision.`
- wear_flags: `8454144` (`about` 65536 | `take` 8388608)
- extra_flags: `134218880` (`ITEM_MAGIC` 64 | `ITEM_ANTI_EVIL` 1024 | `ITEM_BOSS` 134217728)
- weight: `11` (tank item)
- item_apply: `1` (`nada`)

Prop file content:
```text
Violet throne final deposition
64 3 1 1 33 35 3015 7600 8 30308 1
30304
The Violet Hierophant issues irreversible eclipse judgments from the Moonbone throne. Depose the final authority.
The eclipse is over. The throne sits empty, its sentences voided.
@@yEclipse Verdict Regalia@@N
eclipse verdict regalia gold violet hierophant
A gold-edged violet regalia mantle is folded with ritual precision.
8454144
134218880
11
1
```

## Section Order (per `area_file_spec.md` section 14)

The canonical section order emitted by the area saver:
1. `#AREA`
2. `#ROOMS`
3. `#MOBILES`
4. `#MOBPROGS`
5. `#OBJECTS`
6. `#SHOPS`
7. `#RESETS`
8. `#SPECIALS`
9. `#OBJFUNS`
10. `#$`

This area does not currently plan a `#SHOPS` section (no merchant mobs). `#MOBPROGS` is optional and may be added for boss dialogue triggers. `#OBJFUNS` is optional.

## Implementation Checklist

### Structure and Format
1. Write `#AREA` header with all mandatory directives (`Q 16`, `O Virant~`, `V 30250 30324`).
2. Ensure all 75 room vnums (30250-30324) are present in `#ROOMS`, terminated by `#0`.
3. Ensure all room descriptions contain 3+ sentences (5+ for hubs and boss rooms), are unique, and end with one trailing newline before `~`.
4. All exits are bi-directional (no maze rooms). Named exit keywords are discoverable in descriptions.
5. All rooms use `sector_type = 11` (inside) and `room_flags` include `indoors` (8).
6. Boss rooms (30276, 30288, 30300, 30312, 30324) include `no_mob` (4) in `room_flags`.

### Mobiles
7. Rebuild 55 mobs (30250-30304) in `#MOBILES`, terminated by `#0`.
8. All mobs include `is_npc` (1) + `stay_area` (64) in `act` flags.
9. Boss mobs include `sentinel` (2) + `boss` (67108864) in `act` flags.
10. Strong non-boss mobs include `solo` (33554432) in `act` flags.
11. All mob `long_descr` is single-line format only.
12. All mob `description` ends with one trailing newline before `~`.
13. Include `!` extension lines for all mobs with appropriate skill/cast/def profiles.
14. Include loot tables (`l`/`L`) for boss mobs.

### Objects
15. Rebuild 70 objects (30250-30319) in `#OBJECTS`, terminated by `#0`.
16. All objects include `ITEM_TAKE` (8388608) in `wear_flags`.
17. Boss-drop items include `ITEM_BOSS` (134217728) in `extra_flags`.
18. Loot-table items include `ITEM_LOOT` (67108864) in `extra_flags`.
19. No object sets `ITEM_GENERATED` (1) or `ITEM_WEAR_CLAN_COLORS` (16777216).
20. All object names are unique within the area.
21. Weapon objects have `hold` (32768) + `take` (8388608) in `wear_flags`, and thematically consistent `value3`.
22. Key objects (30250, 30251) use `item_type = 18`.
23. All objects include `L <level>` entry.

### Resets
24. Author `#RESETS` with mob placements (`M`), object placements (`O`), equipment (`E`/`G`), and door states (`D`).
25. Locked doors (`D` state 2) reference valid key vnums with matching key objects in `#OBJECTS`.
26. No blank lines within `#RESETS`.

### Specials and Object Functions
27. Author `#SPECIALS` for boss mob special functions.
28. Author `#OBJFUNS` if object functions are used (optional).

### Quests
29. Create proposition files `62.prop`-`66.prop` with full numeric definition lines and custom reward blocks.
30. Note: requires `PROP_MAX_STATIC_QUESTS` increase from 48 before these quests load at runtime.

### Color and Policy
31. Validate color policy: `@@k` absent, no background codes (`@@0`-`@@7`), no flashing code (`@@f`).
32. All color sequences close with `@@N`.
33. Confirm all vnums stay within `V 30250 30324` envelope.
34. File ends with `#$` marker.
