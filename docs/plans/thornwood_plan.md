# Area Plan: Thornwood, the Harren Estate (Full Redesign)

## Overview

- **Area Name:** `@@dThornwood, @@ythe Harren Estate@@N`
- **Area File (existing):** `area/thornwood.are`
- **Keyword:** `thornwood harren estate manor forest ossuary substrate`
- **Area Number:** `N 5` (preserved)
- **Assigned Vnum Range (must remain unchanged):** **1150–1299**
- **Target Level Band:** **50–80** (preserved)
- **Reset Rate:** `F 15` (preserved)
- **Owner:** `O Virant~` (normalize from current lowercase `virant`)

**Room Envelope Used:** 1150–1299 (150/150 rooms — fills the entire envelope)
**Mobile Envelope Used:** 1150–1193 (44 mobs — overlaps room vnums, which is valid per spec §13)
**Object Envelope Used:** 1194–1220 (27 objects — overlaps room vnums, which is valid per spec §13)

**Primary Design Goal:** Rebuild Thornwood into a coherent necrotic estate-forest where all four operating logics — manorial, funerary, forest, and substrate — are expressed simultaneously and inseparably across every room, mob, and object. The design follows the lore's core principle: **depth equals age equals danger**, with the surface manor representing manorial logic, the burial fields representing funerary logic, the older gravewood representing forest logic, and the cave system below representing substrate logic. These are not four separate zones — they interpenetrate, with each layer's vocabulary bleeding into adjacent layers.

The area replaces the current square-grid layout with a **radial-branching topology** that originates at the manor forecourt and extends outward and downward through distinct ecological territories: the manor interior, the charnel verge, the older gravewood, the peripheral mire, the ossuary undercroft, the Wild Hunt patrol routes, and finally the cave system that leads to the Thornwood Lich's sealed chamber. No zone is a dead end — each connects to at least two adjacent zones via non-looping, bidirectional exits — but not every connection is obvious or safe.

---

## Spec-First Constraints (from `area_file_spec.md`)

### Area Header
1. `Q 16` directive is mandatory.
2. Owner line must be `O Virant~` — the existing file uses lowercase `virant` and must be normalized.
3. `V 1150 1299` defines the vnum envelope; all authored rooms, mobiles, and objects must stay within this range.
4. Level label string must be authored as themed color-coded text, not a bare numeric range.
5. Reset message (`U`) must be authored as in-world thematic text with color-code compliance.

### Vnum Allocation
6. Vnums for rooms, mobiles, and objects must be assigned in ascending sequential order with no gaps.
7. Cross-type overlap is allowed: a room, mobile, and object may share the same numeric vnum as long as each is unique within its own index type.
8. Rooms must attempt to fill all 150 available vnum slots (1150–1299).
9. Mobs use 1150–1193; objects use 1194–1220. Both ranges overlap with room vnums, which is permitted.

### String and Description Rules
10. All text fields terminated by `~`. Strings must not contain back-to-back newlines (`\n\n`).
11. Vnums must never appear in any in-world description text — room descriptions, mob descriptions, object descriptions, extra descriptions, or exit descriptions.
12. Mobile `long_descr` must be exactly one text line followed by a newline and a line containing only `~`. Multi-line `long_descr` is never valid.
13. Mobile `description` must end with exactly one newline immediately before the terminating `~`.
14. Object extra-description `<description>` must end with exactly one newline before `~`.
15. Room `<description>~` must end with exactly one trailing newline before `~`.
16. Mobile `description` strings must not be mass-templated boilerplate — each mob needs a concise, identity-specific description of 1–2 sentences.

### Color Policy
17. Use intentional inline color coding with the defined Necrotic Estate palette (see Color Theme section); avoid forbidden thematic black (`@@k`).
18. Do not use background color codes (`@@0`–`@@7`) or flashing (`@@f`) unless explicitly added by a human author.
19. Every color-coded fragment must be terminated with `@@N` before punctuation or whitespace that follows.

### Room Constraints
20. Each room description must contain at least 3 sentences; landmark rooms (the Iron Gate, the Great Hall, the Receiving Room, the Charnel Verge Gate, the Fog Shrine, the Handprint Cave, the Sealed Chamber) must contain at least 5 sentences.
21. Each room description must be unique — no identical or near-identical descriptions across rooms.
22. Exits must be bidirectional: if room A connects to room B via direction X, room B must provide the opposite-direction exit back to A.
23. When a room uses a named exit (non-empty `<exit_keyword>`), the exit name must appear in the room's main description, an object spawning in the room, or an `E` extra description.
24. Named exit keywords for door-style exits must be prefixed with `^` (e.g., `^iron gate`, `^sealed door`).
25. Any exit set to `closed` or `locked` via `#RESETS` command `D` must have `EX_ISDOOR` (`1`) set in its room `D<door>` definition.
26. If a door is reset to `locked` (state `2`), `<key_vnum>` must reference a valid key object, and that key object must exist in `#OBJECTS`.
27. Boss mob rooms must be flagged `no_mob` (`4`). This applies to: room 1161 (Receiving Room — Lord Harren's domain), room 1268 (Sealed Chamber — Thornwood Lich's domain), and room 1276 (Hunt Dispatch — Wild Hunt Master's domain).
28. No directional loops outside of `ROOM_MAZE`-flagged rooms. This area contains no mazes.

### Mobile Constraints
29. All mobs must have `stay_area` (`64`) set.
30. Stationary mobs (the Thornwood Witch, the Harren estate wraith desk-clerks, named NPCs) must also have `sentinel` (`2`) set.
31. Boss mobs must be flagged `sentinel` (`2`) and `boss` (`67108864`) and placed only in `no_mob`-flagged rooms.
32. Strong named non-boss mobs must be flagged `solo` (`33554432`).
33. The `invasion` flag must never be set by builders; it is runtime-only.
34. The `aggressive` flag is set only for mobs whose behavior is to initiate combat on sight. Living non-combat NPCs (the Thornwood Witch, the shaman) must not be aggressive.
35. `day_only` and `night_only` flags are used for the Wild Hunt Master and Wild Hunt Riders (night-only) and for any day-active surface patrols where lore calls for them.
36. Undead mobs must carry the `undead` (`16384`) act flag.
37. Mobs without corporeal form (substrate constructs, mire shades) must carry `no_body` (`65536`) and `no_blood` (`268435456`) where appropriate.

### Object Constraints
38. Every object must include `ITEM_TAKE` (`8388608`) in wear flags.
39. No object may include `ITEM_WEAR_CLAN_COLORS` (`16777216`).
40. Do not set `ITEM_GENERATED` (`1`); it is runtime-managed.
41. Object weight encodes archetype: `1`–`5` caster, `6`–`10` melee, `11`–`15` tank.
42. Do not define fixed stats; stats are generated at runtime.
43. Items from boss mob loot tables must include `ITEM_BOSS` (`134217728`).
44. Items spawned from mob loot tables (`l`/`L` extension) must have `ITEM_LOOT` (`67108864`) set.
45. Weapons must have wear flags including both `hold` (`32768`) and `take` (`8388608`).
46. Two-handed weapons must include `ITEM_TWO_HANDED` (`2147483648`) in extra flags.
47. `value3 = 0` (`hit`) is only valid with the `ITEM_FIST` extra flag.
48. `value3` (weapon attack type) must be thematically consistent with the weapon's presentation.
49. Any object with `ITEM_LIFESTEALER` must also include `ITEM_ANTI_GOOD`.
50. Any object assigned `objfun_healing` must include `ITEM_ANTI_EVIL`.

### Reset Constraints
51. Resets must reference valid vnums for the target type required by the command.
52. `#RESETS` must not contain blank lines.
53. Both sides of any door reset to `closed` or `locked` via `D` must have matching `D` commands on both ends.

### Section Order
54. Follow canonical saver order: `#AREA`, `#ROOMS`, `#MOBILES`, `#OBJECTS`, `#SHOPS`, `#RESETS`, `#SPECIALS`, `#OBJFUNS`, `#$`.

### Specials Policy
55. Mob specials beginning with `spec_summon_` must never be set in area files (runtime-only).
56. Mob specials beginning with `spec_keep_` may only be assigned manually by a human.

---

## Color Theme: "Necrotic Estate Palette"

Six thematic colors anchor Thornwood's visual identity. Each maps to a specific operating logic or faction layer. Usage is intentional and sparse — color only on symbolic anchors, not on every noun.

- `@@y` **Crown Gold**: Harren heraldry, estate authority, the thorn-seal crest, all symbols of the original manorial mandate. Used on gates, insignia, formal thresholds, and the estate's administrative instruments. Represents legitimacy that no longer has legal standing.
- `@@d` **Dark Grey**: Stone, mortar, old iron, the working fabric of the estate's construction. The baseline color of architecture in decay — the manor walls, the cobblestone paths, the ossuary vaults. Not ruin-collapse but slow mineral weathering.
- `@@G` **Green**: Root growth, thorn biology, living forest ecological presence. Used wherever the forest logic is overtaking the manorial layer — briars through stone, roots through masonry, canopy closing over formal paths.
- `@@p` **Purple**: Substrate resonance, binding inscriptions, Root Covenant doctrine, cult ritual. Used on runes, sigil-marked surfaces, the crystal formation's deeper glow, and the language of the Lich's binding.
- `@@a` **Light Cyan**: Crystal cold, anomalous luminescence, bioluminescent fungal output. Used in the cave system, near the crystal heart, and on objects with substrate-mediated cold resonance. The color of what happens when the substrate pays attention.
- `@@R` **Blood Red**: Active corruption events, blood rite sites, the charnel verge's visible staining, violence with intentional weight. Used sparingly — a blood offering platform should carry `@@R`, but most burial field prose should not.

### Theme Usage Rules

- Room titles: maximum two accent colors.
- Descriptions: color only on symbolic anchors — the gate's thorn crest, an inscription, a root mass, a crystal vein, a blood-offering stone.
- Manor and surface rooms should favor `@@y` + `@@d` (authority + decay).
- Burial field and charnel rooms should include `@@d` + `@@R` (stone + blood).
- Older gravewood rooms should pivot to `@@G` (forest logic dominant).
- Ossuary undercroft rooms should use `@@d` + `@@p` (stone + cult/binding).
- Cave system rooms should use `@@p` + `@@a` (binding + crystal cold).
- Wild Hunt rooms should use `@@G` + `@@a` (forest + substrate cold).
- Mire rooms should use `@@G` + `@@d` (organic decay + sediment).
- `@@N` must terminate every colorized fragment before surrounding punctuation.
- Do not use `@@k` as a thematic color.
- Do not use background codes (`@@0`–`@@7`) or flashing (`@@f`).

### Proposed Header Strings

- **Name:** `@@dThornwood, @@ythe Harren Estate@@N~`
- **Level label:** `@@y{@@d50@@y-@@d80@@y}@@N~`
- **Reset message:** `@@dDeep in @@GThornwood@@d, the roots shift and the @@yHarren gate@@d rattles in windless dark.@@N~`

---

## Narrative Pillars

1. **The Estate That Ate Itself.** Thornwood did not fall to external siege or internal rebellion. Its institutional logic — death as throughput, burial as revenue — continued past every ethical limit until the thing being administered (the substrate, the dead, the forest) absorbed the administrator. The horror is that the system kept running. It still runs. The horror is procedural, not dramatic.

2. **Four Logics, One Organism.** No room, mob, or object should express only one operating logic. A Thorn Knight in the manor hallway carries manorial logic (the challenge, the title, the protocol) and forest logic (the briar growth through the joints) and funerary logic (the dead body performing a living guard's role) and substrate logic (the binding that keeps him functional). Every authored element should be legible as the intersection of at least two logics.

3. **Depth Equals Age Equals Danger.** Surface rooms (manor, courtyard, charnel verge approaches) are the newest layer — manorial logic dominant, threat tier 50–60. Below-ground rooms (ossuary, undercroft) are older — funerary logic dominant, threat tier 65–72. Cave rooms (crystal passages, sealed chamber) are oldest — substrate logic dominant, threat tier 68–80. This gradient is not merely thematic; it determines encounter design, room vocabulary, and what kind of enemy populates each zone.

4. **The Archive Is the Weapon.** Thornwood's burial contracts, deferred-name ledgers, and sealed warrant records are not background lore — they are the reason multiple factions have strategic interest in the estate. Objects and room extras in the manor and ossuary should hint at what the archive contains: names that connect Midgaard's institutional crimes to specific interment sites, records that the Reckoning and Continuity factions in Midgaard would fight over, procurement ledgers that tie the Violet Eclipse Cathedral to contaminated-source materials. Players who find these fragments are finding pieces of a larger political truth.

5. **The Binding is Failing.** This is the area's current dramatic tension. Observable signs of the binding's decline — the crystal heart's dimmed luminescence, root network errors, the Lich's economy of speech suggesting monitoring strain — should appear throughout the cave zone and bleed upward into the ossuary. Players who reach the crystal heart are witnessing an infrastructure failure in slow motion. The four competing theories about what failure means (Lich: coherence loss; Harren: perimeter expansion; Cult: renegotiation; Witches: it already happened) can each be represented by a mob's speech or an object's extra description.

6. **The Tunnel Hinge.** Thornwood connects south to the same grave tunnel network used by Midgaard and Shadowmere. This is not decorative world-building — it is the reason Thornwood has never been quarantined. The tunnel entrance room makes this explicit: the infrastructure predates both estates and is neutral ground that neither can afford to lose.

---

## Cross-Area Integration Requirements

### Midgaard
- Room extras and lore objects in the manor study, butler's pantry, and the ossuary's exhumation transit hall should reference Midgaard's Granite Arcade archive under geographic anomaly classifications, the sealed warrant traffic through the grave tunnel, and the Reckoning/Continuity political dispute.
- The grave tunnel hinge room (1150) must include an exit reference to the broader tunnel network without naming a specific Midgaard vnum in in-world text.
- Deferred-naming records and debt ledgers found in estate rooms should use language that parallels Sepulchur Pasture's civic bureaucracy but without civic accountability: same forms, no recourse.

### Shadowmere
- Shadowmere's connection through the tunnel hinge should be legible from room 1150 and from the tunnel transfer records room (1293): tunnel traffic includes both Midgaard funeral convoys and Shadowmere garrison supply/intelligence routing.
- The thorn-motif architectural vocabulary of Thornwood should read as the ecological source of Shadowmere's borrowed funerary aesthetics — Shadowmere formalized what Thornwood produced organically.
- Tone contrast: Shadowmere's horror is bureaucratic (ledgers, warrants, emergency powers). Thornwood's horror is organic (the bureaucracy was consumed by what it administered). Writing in both areas should make this distinction legible.

### Great Northern Forest
- Thornwood is marked on Lantern Road warden maps as excluded from ordinary road-warden contract space. Rooms on the estate's northern and western perimeter should reference this exclusion through carved warning markers, defunct boundary stones, or patrol route records that end abruptly at the estate boundary.
- Thornwood's necrotic ecology is lore-distinct from the Withered Depths blight (Conclave/Prism Thesis origin) and the Verdant Depths' immune-response forest intelligence. Thornwood is substrate-mediated, not Conclave-derived — though the crystal substrate may share a common geological origin with Prism Thesis deposits. Descriptions near the crystal heart should leave this connection ambiguous.

### Violet Eclipse Cathedral
- Procurement ledgers in the manor's map room or butler's pantry should reference the Cathedral's grade-sorting specifications for thornwood timber (surface harvest vs. deep harvest) and bone components. The language shift from named sourcing to anonymous geographic labels in later-era records should be discoverable as a lore extra.
- The chapel within the estate originally operated under Midgaard's Violet Compact-adjacent institutional framework. Its current use by the Root Covenant cult represents the same doctrinal contradiction the Cathedral embodies: purity rhetoric built on contaminated materials.

### Sepulchur Pasture
- Thornwood offers the third funerary model in the regional continuum. Objects and room extras should make clear that Thornwood's deferred-naming system predates and may have inspired Sepulchur Pasture's equivalent. The difference: Sepulchur Pasture has an appeals process. Thornwood does not — the substrate is not a bureaucracy, it is geology.

