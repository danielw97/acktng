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

---

## Spatial Topology and Zone Map

Thornwood uses a **radial-branching topology** with the Forecourt of Inheritance (1158) as the central hub. Zones extend outward and downward from this hub. No zone is a dead end. The mire wraps around the eastern and southern margins, connecting back to the gravewood and burial fields to form a loop that is not a directional loop (each connection uses a different pair of rooms). The cave system descends below the ossuary and terminates at the sealed chamber.

### Zone Summary Table

| Zone | Vnums | Theme | Dominant Logic | Threat Tier |
|------|-------|-------|---------------|-------------|
| I. Harren Approach | 1150–1157 | Entry corridor, grave tunnel hinge, outer perimeter | Manorial + Forest | 50–55 |
| II. The Estate Surface | 1158–1179 | Manor house, outbuildings, chapel, gardens — frozen at Reversal | Manorial + Funerary | 55–65 |
| III. Charnel Verge and Burial Fields | 1180–1197 | Death throughput zone, deferred-name graves, bone processing | Funerary dominant | 60–68 |
| IV. The Older Gravewood | 1198–1215 | Pre-estate ridge-clan burial ground, Root Covenant sites, shaman territory | Forest dominant | 62–70 |
| V. The Mire | 1216–1231 | Peripheral wetland, fog shrine, substrate drainage, unstable terrain | Forest + Substrate | 62–70 |
| VI. Ossuary Undercroft | 1232–1249 | Bone weaver workshops, marrow construct storage, drain passages | Funerary + Substrate | 65–72 |
| VII. Cave System and Substrate Core | 1250–1270 | Crystal passages, handprint cave, high ledge, sealed chamber | Substrate dominant | 68–80 |
| VIII. Wild Hunt Patrol Routes | 1271–1280 | Night-only enforcement paths, birch roads, Hunt departure points | Forest + Substrate | 68–74 |
| IX. Perimeter and Auxiliary Rooms | 1281–1299 | Wall walks, timber stands, cult assembly grounds, archive alcoves, family crypt | All four logics | 55–68 |

### Connectivity Rules

- **Main progression spine:** Entry (Zone I) → Estate Surface hub (Zone II) → branches simultaneously to Charnel Fields (III), Gravewood (IV), and Ossuary descent (VI). The mire (V) is accessible from both the burial fields (III) and the gravewood (IV). The cave (VII) is accessible only from deep within the ossuary (VI). The Wild Hunt routes (VIII) are accessible from the burial fields (III) and gravewood (IV).
- **No directional loops.** The mire connects to both zones III and IV, but the connections use different room pairs, so repeated movement in one direction does not cycle back.
- **Depth gates.** The wine cellar (1169) in the estate surface connects down to the ossuary undercroft (1232). The cave system entry (1249) is accessible only from the deepest ossuary room (1248). The sealed chamber (1268) is accessible only through the threshold of the binding (1267), which requires the Harren Estate Register Key (object 1220).
- **Lateral shortcuts.** The tunnel hinge (1150) provides an external south exit for players to leave the area. The grave tunnel surface exit (1197) in the burial fields connects back up to room 1150, providing an alternate egress from the burial field zone without backtracking through the manor.
- **Night-only access.** Wild Hunt patrol rooms (1271–1280) are not locked during the day but the Wild Hunt Master and Riders only spawn at night. The approach road phantom (mob 1154) patrols Zone I by night only.
- **The mire's unstable terrain.** Mire rooms carry `cold` and `dark` room flags to signal hazard. No maze flag is used — the instability is expressed through mob pressure (mire lurkers, deep mire shades) and room descriptions, not disorienting exits.

### Topology Diagram (Schematic)

```
[Outside / Grave Tunnel Network]
          |
     down | up
          |
    1150 Grave Tunnel Hinge
          | north
    1151 Outer Cobblestone Road
          | north
    1152 Iron Gate of Thornwood ←→ 1153 Gatewarden's Lodge
          | north
    1154 Forecourt Approach ←→ 1155 North Thorn-Hedge Wall
          |                  ←→ 1156 South Thorn-Hedge Wall
          | north
    1157 Lantern Alley
          | north
    1158 Forecourt of Inheritance (CENTRAL HUB)
   /      |      \       \
west    north    east    down (via 1169 Wine Cellar)
  |       |       |            |
1170   1159     1171      1232 Undercroft Stairs
Service Thorn    Charnel        → ZONE VI (Ossuary)
 Yard  Portico  Verge Gate       → ZONE VII (Cave, via 1249)
  |       |       |
1171   1160     1181
  ...  Great   Charnel Verge
       Hall      → ZONE III (Burial Fields)
         |         → ZONE VIII (Wild Hunt, via 1271)
    [ZONE II interior]
    (manor rooms 1160-1168,
     chapel 1166-1167,
     study 1162, gallery 1165,
     kitchen 1168, wine cellar 1169,
     outbuildings 1170-1179)
         |
    1158 hub also connects:
         | northwest
    1198 Gravewood Threshold
         → ZONE IV (Older Gravewood)
           → ZONE V (Mire, via 1207 and 1216)

ZONE III (Burial Fields, 1180-1197)
  connects east → ZONE V (Mire, via 1216)
  connects south → 1197 (Grave Tunnel Surface Exit → 1150)
  connects north → ZONE VIII (Wild Hunt, via 1271)

ZONE IV (Older Gravewood, 1198-1215)
  connects south → ZONE V (Mire, via 1207)
  connects east → ZONE III (Burial Fields, via 1214)
  connects south-deep → ZONE VIII (Wild Hunt, via 1215)

ZONE V (Mire, 1216-1231)
  connects north → ZONE IV (Gravewood, via 1229)
  connects west → ZONE III (Burial Fields, via 1216)
  connects northwest → ZONE IX aux (via 1231 substrate sink)

ZONE VI (Ossuary Undercroft, 1232-1249)
  connects up → ZONE II (Wine Cellar 1169)
  connects east → ZONE III (Hillside Ossuary Entry 1239, from 1187)
  connects deep → ZONE VII (Cave Entry 1250, from 1249)

ZONE VII (Cave System, 1250-1270)
  connects up → ZONE VI (from 1250 to 1249)
  connects far-up → ZONE IV (Gravewood, via 1270 deep cave egress → 1213)
  1268 SEALED CHAMBER (boss, no_mob, key-locked door from 1267)

ZONE VIII (Wild Hunt, 1271-1280)
  connects south → ZONE III (Burial Fields, via 1271)
  connects west → ZONE IV (Gravewood, via 1280)

ZONE IX (Perimeter/Auxiliary, 1281-1299)
  connects to ZONE II (manor) and ZONE III (burial fields)
  at wall walks (1281-1282), timber stands (1284-1285),
  cult assembly ground (1286), archive rooms (1292-1293),
  family crypt (1294), Hunt stable (1295)
```

### External World Links

- **South exit from 1150 (Grave Tunnel Hinge):** Connects into the broader grave tunnel network, giving access to the graveyard area or Midgaard's burial approaches. The reverse link (graveyard area → 1150) must be confirmed and preserved.
- **Up exit from 1150:** Emerges at 1151 (Outer Cobblestone Road) — the surface world above the tunnel. This bidirectional vertical pair is the area's canonical entry point.
- The grave tunnel hinge exit direction and destination vnum must be coordinated with the `graveyard.are` (vnum range 11000–11099) file. The specific room in `graveyard.are` that links to 1150 must carry a reciprocal exit.

---

## Full Room-by-Room Plan (All 150 Rooms, 1150–1299)

Room flags and sector types are noted. All descriptions end with exactly one trailing newline before `~`. Important rooms (marked **★**) require at least 5 sentences. Standard rooms require at least 3. All exits are bidirectional unless noted.

---

### Zone I: Harren Approach (1150–1157) — 8 Rooms

Entry corridor from the grave tunnel network to the estate's iron gate. Manorial and forest logics first appear here, where the tunnel's neutral infrastructure meets the estate's territorial claim.

**1150 — The Grave Tunnel Hinge** ★
`room_flags: dark indoors no_recall` | `sector: inside`
Where the old pre-civic burial tunnel meets the surface approach to the Thornwood estate. This is the transit node shared with Shadowmere's southern march supply routes and Midgaard's funeral convoy traffic — neutral infrastructure neither power controls. The tunnel ceiling is low and sweating limestone, braced with timber so old it has calcified. Carved way-markers from at least three distinct traditions overlap on the eastern wall: a civic registry glyph from Midgaard's road-standards era, a directional slash-mark used by Shadowmere's tunnel scouts, and something older and non-linguistic — a pressed handprint in ochre that may predate the tunnel's formal construction. The smell is mineral and cold, with an undertone of old bone and something faintly sweet that does not belong underground.
- Exits: `south` → graveyard.are tunnel node (external link), `up` → 1151

**1151 — The Outer Cobblestone Road**
`room_flags: dark no_recall` | `sector: forest`
The surface world above the tunnel hinge, where the estate's cobblestone road begins. The paving is ancient and frost-heaved, pushed up in long ridges by root masses that have been growing beneath it for generations. The stones are dark with moisture even in dry weather — the substrate draws groundwater upward through the limestone below. To the north, the road curves toward a silhouette of iron and thorn against a canopy of leafless oaks. Crows occupy every available branch, unmoving.
- Exits: `down` → 1150, `north` → 1152

**1152 — The Iron Gate of Thornwood** ★
`room_flags: dark no_recall no_portal` | `sector: forest`
The estate's defining threshold: a pair of massive wrought-iron gates set into a wall of dark stone, their original ornamental thorn motifs long since fused with genuine briar growth so that the heraldic and the biological have become indistinguishable. The Harren crest — a thorned branch over a keyhole — is still visible on the central boss of the gate, though the @@y crown-gold finish@@N has oxidized to a greenish-black. The bars are wrapped in root-mass at the joints and base, anchoring the gate in ways that make it unclear whether the ironwork or the roots are now the structural element. Beyond the gate, the cobblestone continues into a forecourt dimly visible through the thorn-growth. The gateposts carry rune-carvings too eroded to read in full, though the shapes carry a heaviness that suggests the eye was not meant to rest on them long. A chain and bracket system suggests the gates were once mechanically operated; the mechanism is now biological, responsive not to levers but to the estate's own territorial logic.
- Exits: `south` → 1151, `north` → 1154, `east` → 1153
- Named exit: `^iron gate` (north, door; `EX_ISDOOR` set; no lock — the gate stands open but is a real door that can be opened/closed)

**1153 — The Gatewarden's Lodge**
`room_flags: dark indoors no_recall` | `sector: inside`
A small stone building pressed against the inner face of the eastern gatepost, where the gate-keeper once logged arrivals, checked credentials, and collected the estate's passage tax. The logbook desk still stands, its surface thick with root-fiber that has threaded up through the floorboards and integrated itself into the wood grain. The most recent legible entry is decades old. A hook on the wall still holds a brass bell — the notification bell used when important visitors required Lord Harren's attention — though the bell now resonates faintly with the substrate's harmonic output rather than with a clapper stroke. The lodge is occupied.
- Exits: `west` → 1152

**1154 — The Forecourt Approach**
`room_flags: dark no_recall` | `sector: forest`
Inside the gate, the cobblestone broadens into a formal approach lined with iron shepherd's-hook lantern posts, all dark, their fuel long oxidized to amber resin inside their housings. The lanterns mark jurisdiction — they were the estate's administrative markers — but have not illuminated anything in living memory. Thorn hedges rise on either side of the path, their growth managed (directed would be more accurate) so the approach corridor feels deliberate. Something about the hedges' angle makes it clear they are oriented toward the path rather than growing away from it. The manor's peaked roof is visible ahead.
- Exits: `south` → 1152, `north` → 1157, `east` → 1155, `west` → 1156

**1155 — The North Thorn-Hedge Wall**
`room_flags: dark no_recall` | `sector: forest`
The northern run of the estate's interior perimeter hedge, where thorn growth has consumed the original stone wall so completely that the wall's outline is only visible as a slight straightness in what otherwise looks like natural woodland. The thorns here grow in layers — outer growth dark-stemmed and aggressive, inner growth pale and fused into structures that resemble architectural arches. The substrate manages this wall. It is not decorative.
- Exits: `west` → 1154, `east` → 1282 (Zone IX: North Wall Walk)

**1156 — The South Thorn-Hedge Wall**
`room_flags: dark no_recall` | `sector: forest`
The southern run of the interior perimeter, mirroring the northern hedge wall but oriented differently — here the thorns grow inward, toward the manor grounds, rather than outward. The difference is not aesthetic. It suggests the substrate is pulling material from the south. Carved stones from the original wall are visible at the hedge's base, their surfaces still bearing Harren boundary-marks worn smooth by root pressure.
- Exits: `east` → 1154, `west` → 1281 (Zone IX: South Wall Walk)

**1157 — The Lantern Alley**
`room_flags: dark no_recall` | `sector: forest`
A formal path between the forecourt approach and the estate's central courtyard, originally lit by lanterns on both sides for the reception of dignified visitors. Fourteen lantern posts remain, seven to each side, their iron scroll-work intact but rusted to a dull orange. The path between them is perfect — not heaved, not overgrown, each cobblestone seated cleanly. This is suspicious. The substrate maintains this path actively, and the smooth surface underfoot is the smoothness of a tongue, not of careful masonry. The manor's entrance portico is visible ahead, its dark stone columns wrapped in early thorn growth.
- Exits: `south` → 1154, `north` → 1158

---

### Zone II: The Estate Surface (1158–1179) — 22 Rooms

The manor house and its outbuildings, frozen at the moment of the Reversal. Formal spaces preserve the instant of transformation as permanent décor. The manorial and funerary logics are densest here; the forest logic intrudes through every gap.

**1158 — The Forecourt of Inheritance** ★ (CENTRAL HUB)
`room_flags: dark no_recall` | `sector: city`
The broad cobblestone forecourt before the manor's entrance, where the estate's administrative life was once conducted openly — arrivals logged, debts assessed, burial contracts signed on portable writing desks that servants carried out for the purpose. It is large enough that the surrounding thorn hedges do not immediately close in; there is real sky visible above, though the canopy has begun to arch over the western edge. The @@yHarren crest@@N is inlaid in the forecourt's central stones — a thorned branch over a keyhole in pale limestone — but root growth has retraced the design in biological material, so the crest appears twice: once in stone, once in living briar pressing up through the mortar. The estate's main entrance is north. Paths lead east to the charnel verge, west to the service wing and older gravewood approaches, southeast to the chapel, and down through the manor's cellar connections to the ossuary. The air here is colder than the approach, with a faint mineral undertone.
- Exits: `south` → 1157, `north` → 1159 (Thorn Portico), `east` → 1180 (Charnel Verge Gate), `west` → 1170 (Service Yard), `northwest` → 1198 (Gravewood Threshold), `southeast` → 1166 (Chapel Antechamber)

**1159 — The Manor's Thorn Portico**
`room_flags: dark indoors no_recall` | `sector: inside`
The formal entrance to the manor house proper: a covered portico of dark stone columns between which thorn growth has been trained into arch-shapes so precise they appear architectural. The original wooden doors behind the columns have been partly consumed by the same growth — their hinges still function but the wood grain has fused with root-fiber, so opening them produces a sound like tearing rather than creaking. A coat-of-arms plaque above the lintel shows the Harren thorn-and-keyhole device, the @@ygold finish@@N still bright where protected by the overhang.
- Exits: `south` → 1158, `north` → 1160 (Great Hall)

**1160 — The Great Hall** ★
`room_flags: dark indoors no_recall` | `sector: inside`
The manor's primary reception space, preserved at the instant of the Reversal with the completeness of an insect in amber. A long table runs the hall's length, set for a dinner that was never finished: plates still hold desiccated food reduced to mineral shapes, goblets stand upright, and the centerpiece — a floral arrangement in a silver bowl — has been replaced by thorn growth that rises from the bowl's interior as though the thorns were always the intended arrangement. Portraits line the walls, each depicting a generation of the Harren family; the eyes have been removed from every portrait, neatly, as though the substrate decided that observation flowed in one direction only. Chandeliers of iron and bone-horn hang from the ceiling, long unlit, threaded through with root-fiber that has used the chandelier chains as a climbing scaffold. @@yHarren heraldic banners@@N along the upper walls bear the thorn-and-keyhole device, their fabric intact and unfaded, though a careful look reveals they are now partly woven from biological material rather than cloth. The hall is cold. The cold is not from the temperature but from whatever the crystal substrate radiates upward through the floor.
- Exits: `south` → 1159, `north` → 1161 (Receiving Room), `east` → 1163 (Map Room), `west` → 1162 (Lord's Study), `up` → 1165 (Manor Gallery — second floor landing)

**1161 — The Receiving Room** ★ (Boss room: Lord Harren — `no_mob` flag)
`room_flags: dark indoors no_recall no_mob` | `sector: inside`
Lord Harren still receives visitors here. The room is unchanged from the Harren Charter era in every physical detail — the audience chair behind the writing desk, the two visitor chairs facing it, the shelved reference volumes along the east wall (estate law, boundary records, tariff tables), the single window overlooking the forecourt, now opaque with thorn growth. What is wrong is the occupant. Lord Harren, the Estate Revenant, sits at the desk as he always did: upright, attentive, formally dressed in the estate's administrative livery. His face is recognizably human and fully alive in expression, but the skin carries a faint bark-pattern, his hands rest flat on the desk with a stillness that no living person maintains without effort, and the eyes — still his eyes, still conscious and aware — move with the deliberate economy of someone who has learned that spontaneous motion costs something. He will negotiate. He remembers being alive. He is the most dangerous thing on the estate's surface.
- Exits: `south` → 1160, `east` → 1164 (Butler's Pantry)

**1162 — The Lord's Study**
`room_flags: dark indoors no_recall` | `sector: inside`
Harren's private working space, smaller and more disordered than the receiving room. The desk here carries the weight of real administrative work — not the performance of authority but its machinery. Papers are stacked in the substrate-preserved state they occupied at the moment of the Reversal: mid-sentence correspondence, half-completed account columns, a journal whose final entries grow increasingly erratic before stopping. The estate's operational records — budget summaries, staff rosters, interment manifests — are mixed with personal notes in a hand that becomes harder to read as the journal progresses. Root-fiber has threaded through the paper stacks but not displaced them; the substrate appears to be preserving the archive, not consuming it.
- Exits: `east` → 1160, `south` → 1164 (Butler's Pantry, via connecting passage)

**1163 — The Map Room**
`room_flags: dark indoors no_recall` | `sector: inside`
The estate's cartographic archive, where Harren administrators managed the physical territory of their charter. Large vellum maps cover the long table and hang from the walls: the estate's original land grant with surveyed boundaries, the grave tunnel route with tributary nodes marked, the interment field plots numbered by intake period, and a partial survey of the old gravewood that stops abruptly at the point where the ridge-clan burial sites begin. Root growth has traced along the map edges as though something finds the boundaries interesting. A @@pdim inscription@@N has been added to one map in a hand that is not the Harrens' — a script that the cult uses for ritual filings, pressed into the vellum as if by something other than a quill.
- Exits: `west` → 1160

**1164 — The Butler's Pantry**
`room_flags: dark indoors no_recall` | `sector: inside`
The administrative hub of the manor's domestic operations: the room from which the household was managed day to day. Ledgers, keys, linen inventories, wage records, and staff duty rosters fill the shelves. The butler's own records are here — a meticulous log of every visitor received, every funeral convoy processed, every payment recorded for passage tax and burial lease. The handwriting maintains its precision until the final pages, where a single repeated annotation begins appearing on otherwise normal entries: a small pressed circle with radial lines, increasing in frequency, until the last entries show nothing else. The pantry smells of congealed beeswax and something older beneath it.
- Exits: `north` → 1161, `west` → 1162

**1165 — The Manor Gallery**
`room_flags: dark indoors no_recall` | `sector: inside`
The manor's second-floor portrait gallery, a long corridor of framed canvases depicting the Harren lineage across six generations. Every portrait has had its eyes removed with precision — not scratched out but cleanly excised, leaving oval absences that look less like damage and more like a consistent modification applied by someone with a coherent purpose. The @@yHarren thorn-and-keyhole device@@N appears in every frame's background: worked into furniture, embroidered on clothing, carved into the landscape behind the subjects. The frames themselves are intact and ornate. The gallery is the most formally maintained space in the manor — the root-fiber that has colonized everything else has left the portrait frames untouched.
- Exits: `down` → 1160, `east` → 1166 (via connecting stairway)

**1166 — The Chapel of the Harren Estate** ★
`room_flags: dark indoors no_recall` | `sector: inside`
The estate's institutional chapel, built under Violet Compact-adjacent auspices to serve the manor household and provide religious certification for the burial rites that formed the estate's primary revenue. The original fittings — altar, pew ranks, devotional niches — survive intact, but the symbols have been overwritten. The Compact-adjacent iconography has been systematically replaced with Root Covenant imagery: thorn patterns cut into the altar stone, bone arrangements in the devotional niches, and a central floor inscription where the compact seal once was, now bearing the cult's contract-liturgy in @@pdeep purple chisel-marks@@N. Candles burn here — real candles, maintained by living hands. The Root Covenant uses this chapel for its organized rites, and the smell of fresh blood and tannin beneath the old stone-and-incense is not from the past.
- Exits: `northwest` → 1167 (Chapel Antechamber), `east` → 1165 (via stairway connection)

**1167 — The Chapel Antechamber**
`room_flags: dark indoors no_recall` | `sector: inside`
The waiting room where the chapel's lay community would gather before services, now serving the same function for the Root Covenant's ritual gatherings. Stone benches line the walls. A board on the north wall once held the week's service schedule in chalk; it now holds a chalk list of "pending filings" — a cultist's phrase for blood offerings owed to the substrate but not yet paid. The list is long.
- Exits: `southeast` → 1166, `north` → 1158 (Forecourt of Inheritance, via exterior path)

**1168 — The Kitchen and Scullery**
`room_flags: dark indoors no_recall` | `sector: inside`
The manor's kitchen, frozen mid-preparation. Pots hang from iron hooks, their contents long reduced to dry mineral compounds. A cutting board still holds a knife. The fire grate is black with old ash. The scullery beyond the kitchen arch is where the household's washing was done — the basin is still half-full of water, though the water is now the color of tannin and has a faint crystal clarity that does not belong to standing water decades old. Something about the kitchen's preservation feels more intentional than the rest: the substrate keeps it cold and dry, which is precisely what you would do if you wanted to use it again.
- Exits: `east` → 1158 (Forecourt, via exterior kitchen door), `north` → 1160 (Great Hall, via service passage)

**1169 — The Wine Cellar** ★
`room_flags: dark indoors no_recall cold` | `sector: inside`
The estate's wine cellar, reached by descending from the kitchen through a flagstone stair. This room is the coldest place in the manor — colder than the cave system below, colder than the mire, cold in the way that glass is cold when it has been storing something much older than the temperature around it. The wine racks are intact, the bottles sealed in wax that has crystallized but not cracked. On the cellar floor, occupying most of the open space between the racks, is the secondary binding inscription — a complex inscribed circle identical in geometry to the one in the sealed chamber far below, rendered in a script that is not carved but grown: the root-fiber threading between floor stones has arranged itself into the sigil's pattern. This was not done by human hands. The @@pbinding geometry@@N glows faintly in the @@acold blue@@N of substrate-connected fungal output from the walls. A iron-banded hatch in the floor descends further, into the ossuary undercroft.
- Exits: `up` → 1168 (Kitchen), `down` → 1232 (Undercroft Stairs)

**1170 — The Service Yard**
`room_flags: dark no_recall` | `sector: city`
The open yard between the manor's domestic wing and its outbuildings, where the estate's working operations were coordinated. Loading areas, storage bays, and the connections to the stable block, distillation outbuilding, and groundskeeper's store all open onto this space. The ground is cobbled in a rougher grade than the formal forecourt — working stone, not display stone. Substrate growth here is less organized than in the manor: the service yard is where the estate's biological layer shows its seams, with root eruptions through paving, thorn growth in irregular directions, and the smell of damp iron and sweet rot stronger than elsewhere.
- Exits: `east` → 1158 (Forecourt of Inheritance), `north` → 1171 (Stable Block), `west` → 1173 (Groundskeeper's Store), `south` → 1175 (Distillation Outbuilding)

**1171 — The Stable Block**
`room_flags: dark indoors no_recall` | `sector: inside`
The manor's stabling facility, built for the estate's working horses and the mounts of visiting funeral convoy escorts. The original horses are long gone. What occupies the stalls now are substrate constructions that approximate equine form: birch-bark pale, berry-red eyes, weight and movement consistent with horses but without breath or sound. They stand in the stalls with the stillness of furniture. The tack room at the stable's western end still holds the estate's original saddle-work and harness, some of it maintained in better condition than it has any right to be.
- Exits: `south` → 1170, `east` → 1172 (Stable Yard)

**1172 — The Stable Yard**
`room_flags: dark no_recall` | `sector: city`
The yard behind the stable block, where horses were exercised and where the estate's working carts were parked. Frost-prints mark the cobblestones — the substrate horses leave these rather than hoofprints. The cart park holds two intact wagons whose wood has been fully colonized by root-fiber, making them immovable. The yard connects north to the timber stands along the estate's managed perimeter.
- Exits: `west` → 1171, `north` → 1284 (Zone IX: Timber Stands), `east` → 1295 (Zone IX: Hunt Stable Court)

**1173 — The Groundskeeper's Store**
`room_flags: dark indoors no_recall` | `sector: inside`
The tool store for the estate's groundskeeping operations — the staff responsible for maintaining the formal gardens, the thorn hedges, and the managed timber stands. Tools hang on the walls in the order their users last placed them: pruning hooks, root-cutting saws, long-handled shears for hedge work. The saws are interesting: the blade teeth are worn in patterns that suggest they were used on material harder than wood. The estate ledger kept here tracks tool condition and assignment — the last entries show items signed out but not returned.
- Exits: `east` → 1170, `south` → 1174 (Carpenter's Workshop)

**1174 — The Carpenter's Workshop**
`room_flags: dark indoors no_recall` | `sector: inside`
Where the estate's thornwood timber was processed into finished goods for internal use and for external sale. The workbenches carry the marks of sustained craft use: saw-cut channels, chisel dents, wax-stain rings. Thornwood shavings still cover the floor, their unusual density and preservative compounds keeping them from degrading in the normal way — the shavings look fresh. A price-list on the wall details the timber grades: surface harvest, managed-stand harvest, deep-interior harvest, the last priced significantly higher with a notation that reads "Cathedral-grade; grade-sort required; consult ledger before committing."
- Exits: `north` → 1173, `east` → 1175 (Distillation Outbuilding)

**1175 — The Distillation Outbuilding**
`room_flags: dark indoors no_recall` | `sector: inside`
The estate's alchemical processing facility, where botanical and bone-derived compounds were extracted for sale and for the estate's own burial-preparation operations. The still-apparatus is intact: copper coils, sealed collection vessels, a furnace grate. The output channels from this building connect southward through the estate's drainage system to the mire. The waste products of decades of alchemical processing — rendered bone oils, tannin concentrates, botanical extracts from thorn-growth — have been depositing in the mire's water table for generations, contributing to the wetland's toxic character. The room smells of @@asulfur and sweet rot@@N.
- Exits: `north` → 1170 (Service Yard), `west` → 1174, `south` → 1230 (Zone V: Alchemical Runoff Channel)

**1176 — The Garden Terrace**
`room_flags: dark no_recall` | `sector: forest`
The estate's formal garden terrace, originally a tiered arrangement of ornamental plantings behind the manor's west wing. The terracing is still visible as stone retaining walls, but the planting beds have been wholly consumed by thorn growth that has replaced every intended species with its own logic. The remaining ornamental structures — a sundial, a stone bench, a low fountain basin — are still present but incorporated: the thorn has grown through them rather than around them, making them part of the hedge geometry rather than decorative counterpoints to it.
- Exits: `east` → 1158 (Forecourt), `north` → 1177 (Hedge Labyrinth Entry), `south` → 1178 (Rose Garden Ruins)

**1177 — The Hedge Labyrinth Entry**
`room_flags: dark no_recall` | `sector: forest`
The entrance to what was once the estate's formal hedge labyrinth — a fashionable garden feature of the Harren period, meant to demonstrate the family's resources and aesthetic sophistication. The original hedges were yew, topiary-shaped to precise geometric patterns. They are now thornwood, and they are no longer geometric — or rather, they have a geometry, but it is not the geometry of the original design. The new pattern corresponds to the binding's inscription geometry, traceable if you can read the binding's script. The labyrinth is navigable. It is not safe.
- Exits: `south` → 1176, `north` → 1286 (Zone IX: Cult Assembly Ground, via labyrinth pass-through), `east` → 1287 (Zone IX: Covenant Offering Site East)

**1178 — The Rose Garden Ruins**
`room_flags: dark no_recall` | `sector: forest`
What the estate's records call the Rose Garden was the manor's most formal exterior space — the area reserved for family ceremonies, the signing of significant contracts, and the reception of distinguished guests who warranted outdoor hospitality. The rose beds are gone. The garden's structure — a circular central clearing with four radiating paths — survives, but the paths are now thorn-lined and the central clearing contains a bone arrangement: femurs and long bones set upright in the earth at regular intervals, too carefully placed to be accidental. This is a Root Covenant installation. The cultists have converted the estate's most ceremonial exterior space into a bone-filing rite site.
- Exits: `north` → 1176, `west` → 1288 (Zone IX: Covenant Offering Site West)

**1179 — The Estate Orchard**
`room_flags: dark no_recall` | `sector: forest`
A former apple and plum orchard at the estate's western edge, now twisted beyond fruiting capacity. The trees still stand in rows — the original planting grid is visible — but the substrate has modified them: the bark has darkened to near-black, the branch angles have shifted toward the manor rather than toward light, and the root systems have surfaced and interlocked with each other in a net that makes walking between the trees slow and careful. The ground between the root-net sections holds small cairns of smooth stones, each one placed by a different hand over a long span of time — the pre-estate ridge-clan practice of marking a path's edge for travelers who might lose their footing.
- Exits: `east` → 1176 (Garden Terrace), `north` → 1198 (Gravewood Threshold)

---

### Zone III: Charnel Verge and Burial Fields (1180–1197) — 18 Rooms

The operational heart of the estate's death economy. Extends beyond the manor's formal perimeter into the old gravewood's margins. Funerary logic is dominant here; forest logic bleeds in from the east where the burial expansion met the older ridge-clan sites.

**1180 — The Charnel Verge Gate** ★
`room_flags: dark no_recall` | `sector: city`
The formal threshold between the manor's domestic estate and the burial operation beyond. A stone arch carries a lintel inscription in Midgaard-standard administrative script: *Interments Processed on Receipt of Authorized Manifest. All Convoy Cargo Subject to Estate Audit.* The arch's stonework is intact, but the passage beneath it smells of old blood and wet earth in a combination that no amount of time has diminished. Thorn growth has traced the arch's curve from the inside, following the stone precisely, as though the substrate approved of the demarcation and wanted to reinforce it biologically. Beyond the arch, the cobblestone ends and packed earth begins — the working surface of the burial fields, not a formal approach. Iron posts to either side carry rusted hooks where the original processing lamps were hung; the lamps are gone, but the hooks remain, and something about their alignment feels like it still means something.
- Exits: `west` → 1158 (Forecourt of Inheritance), `east` → 1181 (Charnel Verge), `north` → 1194 (Weight-Stone Platform)

**1181 — The Charnel Verge**
`room_flags: dark no_recall` | `sector: forest`
The verge itself — the liminal band of ground between the estate's domestic space and the burial fields, where the transition happens. The earth here has a different texture: darker, more compacted, with a faint residual warmth that is not solar. This is the ground that received the most traffic during the estate's operational peak: body-bearing carts, grieving families, bone-processing laborers. The substrate's root-layer is dense just below the surface, and the ground occasionally produces a low sound — not a creak but a settling, as of weight being redistributed.
- Exits: `west` → 1180, `east` → 1182 (Intake Processing Yard), `south` → 1189 (Corpse-Lily Terraces)

**1182 — The Intake Processing Yard**
`room_flags: dark no_recall` | `sector: city`
Where funeral convoys were received, assessed, and sorted. A long stone platform runs the yard's length — the weigh-bench, where coffins were lifted onto the scales and their stated contents verified against the manifest. The scales are still mounted: iron balance arms, stone counterweights, a calibration marker in the stone. The platform's surface carries the stain-record of generations of use: water-marks from washing, rust from iron fittings, darker compounds that do not have a clean name. A row of iron stakes along the south wall held the processing lamps; the stakes remain, bent at uniform angles as though pressed by something coming from the direction of the burial fields.
- Exits: `west` → 1181, `north` → 1183 (Deferred-Name Registers), `south` → 1184 (Rapid Burial Fields North), `east` → 1186 (Bone Processing Station, via working path)

**1183 — The Deferred-Name Registers**
`room_flags: dark no_recall` | `sector: city`
A row of provisional grave markers stretching north along the burial field's intake edge — each a numbered iron stake with a small attached plate giving a case number, an intake date, and a blank field where the name would eventually be filled in when the family paid the naming rite fee. Most plates remain blank. Some are numbered in sequences so dense that the case numbers overlap and have to be read in a specific order to make sense. A ledger-stand at the row's southern end holds the register itself: a thick book of oilskin-wrapped pages, its entries showing that only a fraction of the cases were ever closed. The entries that are closed bear a single stamp: *Named. Filed. Complete.* The entries that were never closed bear a different stamp, added later in a hand that is not the original clerk's: *Pending. Interest accruing.*
- Exits: `south` → 1182, `north` → 1289 (Zone IX: Bone-Path Marker Row)

**1184 — The Rapid Burial Fields (North)**
`room_flags: dark no_recall` | `sector: field`
The northern section of the estate's commercial interment fields — the rapid-burial ground where bodies were processed at volume during the crisis years when Midgaard's own burial infrastructure could not absorb the throughput. The graves here have no markers; the estate's records link each plot to a case number in the deferred-name register. The plots themselves are visible as slight depressions in the earth, too regular to be natural subsidence. Root-mass threads through everything at knee height, integrating the burial density into the substrate's network. The ground sometimes moves.
- Exits: `north` → 1182, `south` → 1185 (Rapid Burial Fields South), `east` → 1190 (Thorn-Ringed Plots), `west` → 1288 (Zone IX: Covenant Offering Site West)

**1185 — The Rapid Burial Fields (South)**
`room_flags: dark no_recall` | `sector: field`
The southern extension of the rapid-burial ground, where the expansion into the older gravewood's margins began. The grave depressions here are less regular — the substrate's original ridge-clan burial network was already present when the Harrens expanded southward, and the commercial interments were overlaid onto existing deep-burial sites without the original community's knowledge. This mixing is why the substrate in this section behaves differently from the northern fields: root behavior is more complex, soil temperature is anomalous, and the sound of root-creak is louder here than anywhere else on the surface.
- Exits: `north` → 1184, `south` → 1197 (Grave Tunnel Surface Exit), `east` → 1191 (Sealed Burial Row)

**1186 — The Bone Processing Station**
`room_flags: dark no_recall indoors` | `sector: inside`
A partially open structure — a stone-roofed shelter without full walls — where exhumed remains were cleaned, sorted, and prepared for ossuary storage. The processing tables are still set up: marble slabs with channels for fluid drainage, racks for drying, bins labeled by bone type in the estate's cataloguing system. The cataloguing system is thorough and unsentimental: femur, tibia, skull, rib-section, miscellaneous small, all given codes that reference the ossuary vault destination. The bins contain residue — dust, mineral compounds, the dried remains of the processing work — but the structure is otherwise clean. Whatever processed things here was meticulous.
- Exits: `west` → 1182, `south` → 1187 (Ossuary Hillside Cut), `east` → 1291 (Zone IX: Wraith Passage)

**1187 — The Ossuary Hillside Cut**
`room_flags: dark no_recall` | `sector: hills`
A cut through the low hillside east of the burial fields, leading down and inward to the hillside ossuary vault entrance. The cutting is stone-faced on both sides, the work of skilled masons — the Harrens built this seriously. Iron gate-frames set into the hillside cut hold the remains of gates that are now more thorn than iron, the original bars having been colonized so completely that the biological growth now provides structural support. The ground slopes downward at a consistent grade, and the smell of bone dust and cold mineral begins here, increasing with depth.
- Exits: `north` → 1186, `down` → 1239 (Zone VI: Hillside Ossuary Entry)

**1188 — The Memorial Byre**
`room_flags: dark indoors no_recall` | `sector: inside`
A stone building at the burial field's western edge, where families who had paid the full naming and memorial rite fee could receive cremated remains in a sealed container and participate in a private ceremony. The byre's interior is designed for grief display: padded benches, a stone ledge for the remains container, alcoves with devotional symbols. The devotional symbols have been replaced with Root Covenant bone arrangements. The bench cushions are still there, though the fabric has decayed to threads. The ledge is empty. No families have come for some time.
- Exits: `east` → 1181 (Charnel Verge), `north` → 1193 (Charnel Chapel Ruin)

**1189 — The Corpse-Lily Terraces**
`room_flags: dark no_recall` | `sector: field`
The terraced section of the burial fields named for the pale, fleshy flower that grows exclusively on substrate-saturated grave soil. Corpse-lilies are not natural — they are an emergent biological output of the substrate's integration with high-density remains, producing a flower with anomalous properties: cold to the touch, faintly luminescent at the petal edges, and capable of surviving conditions that kill ordinary plants. They bloom upside-down, faces toward the soil. The terracing was originally for drainage management; it now creates a series of thorn-rimmed levels descending east, each populated with corpse-lily growth at different stages.
- Exits: `north` → 1181, `east` → 1190 (Thorn-Ringed Plots), `south` → 1196 (Annex Burial Ground)

**1190 — The Thorn-Ringed Plots**
`room_flags: dark no_recall` | `sector: forest`
The oldest section of the original estate burial ground, where the pre-estate ridge-clan containment tradition survived into the commercial era and was adopted, misunderstood, and ultimately validated by the substrate. These plots have genuine containment thorn rings — grown tight over decades around specific graves, their organic material fused into structural barriers. The rings are not decorative. Exhumation attempts on these plots during the estate's ossuary expansion were abandoned when the thorn growth physically resisted digging equipment. The substrate holds these plots sealed. Whatever is inside them, the binding considers it necessary to keep there.
- Exits: `west` → 1184, `south` → 1191 (Sealed Burial Row), `east` → 1214 (Zone IV: Gravewood Fringe East)

**1191 — The Sealed Burial Row**
`room_flags: dark no_recall cold` | `sector: forest`
A straight row of graves running north-to-south where exhumation was impossible — not because of thorn-ring containment but because the substrate itself refused to yield the remains. The estate's processing logs document repeated attempts: tools broke, laborers reported pressure beneath the soil pushing back, and in one documented incident a laborer who persisted was found the following morning asleep on the surface with no memory of the night before. The row has been left alone since. The grave markers here — unlike the deferred-name stakes — are carved stone, some bearing inscriptions that predate the Harren estate entirely. The cold here is notably sharper than in surrounding areas, with a faint @@acrystal-frequency hum@@N just below audible range.
- Exits: `north` → 1190, `south` → 1185

**1192 — The Charnel Chapel Ruin**
`room_flags: dark no_recall` | `sector: inside`
A secondary chapel that served the burial fields directly — separate from the manor's institutional chapel, this was where the grave-workers and corpse-cart laborers could attend services without entering the manor proper. The roof has partially collapsed, and the walls show root-penetration at every joint. The altar stone is intact. On it, in a script that blends Compact liturgy with Root Covenant contract-language, is an inscription that reads as both a prayer and a debt acknowledgment: *The ground has received. The ground is owed. Blood closes what bone opens.*
- Exits: `south` → 1188 (Memorial Byre), `east` → 1193 (Bone-Cart Path)

**1193 — The Bone-Cart Path**
`room_flags: dark no_recall` | `sector: field`
The service route connecting the burial fields to the bone processing station and ossuary entrance, worn smooth by generations of cart traffic carrying exhumed remains for processing and storage. The path is wide enough for a cart and surfaced with compacted grit rather than cobblestone. Root growth has been trimmed back along the cart-lane — the substrate keeps this route clear, suggesting the bone-transit function is still considered operationally necessary. At regular intervals along the path, iron hooks are set at cart-height in wooden stakes, where lanterns would have hung for night operations.
- Exits: `west` → 1192, `north` → 1186 (Bone Processing Station), `south` → 1194 (Weight-Stone Platform)

**1194 — The Weight-Stone Platform**
`room_flags: dark no_recall` | `sector: city`
The formal weight assessment station at the burial field's main axis, where coffins were verified against manifests and assessed for the per-weight burial fee. The scale apparatus here is more substantial than the processing yard's — this is where contested weights were adjudicated, where families could challenge the estate's assessment. The adjudication logbook is still on its stand. The majority of recorded challenges were resolved in the estate's favor. A small iron placard on the scale's frame reads: *Weight disputes subject to estate arbitration. All findings final. No appeals after burial.*
- Exits: `south` → 1180 (Charnel Verge Gate), `north` → 1195 (Memorial Lane), `east` → 1193 (Bone-Cart Path)

**1195 — The Memorial Lane**
`room_flags: dark no_recall` | `sector: field`
The formal processional route reserved for families who had paid the premium memorial service — a wide, paved path lined with low stone markers bearing the estate's thorn device, leading from the charnel verge gate northward toward the burial field's prestige plots. The prestige plots no longer exist as such; the premium-service graves were exhumed along with everything else during the ossuary expansion, and the families who paid for them received no notification. The Lane itself is intact, its markers still standing, its paving still level. It leads to nowhere that corresponds to what it was built to reach.
- Exits: `south` → 1194, `north` → 1271 (Zone VIII: Hunt Road Entry)

**1196 — The Annex Burial Ground**
`room_flags: dark no_recall` | `sector: field`
The overflow section opened when the main burial fields reached capacity — less formally planned, with narrower path access and cruder grave markers. This ground shows the estate's haste more clearly than the main fields: plots are irregular, spacing is inconsistent, and the transition from the commercial interment style into the older gravewood's ridge-clan burial zone happens gradually and without a clear boundary. Root-mass here is the most vigorous anywhere on the surface — the substrate's response to the mixing of new commercial dead with the older, deeper network.
- Exits: `north` → 1189 (Corpse-Lily Terraces), `east` → 1198 (Gravewood Threshold), `south` → 1209 (Zone IV: Cairn of Sealed Names)

**1197 — The Grave Tunnel Surface Exit**
`room_flags: dark no_recall` | `sector: field`
The surface manifestation of the old burial tunnel that connects southward through the estate's subsurface to the grave tunnel hinge at the estate's base. This exit served the estate's convenience in both directions: funeral convoys arriving from Midgaard and Shadowmere could emerge here rather than traveling overland to the main gate, and processed remains could be transferred out of the estate through the tunnel without the visible traffic of cart convoys. The surface opening is framed in old stone, covered by an iron hatch that is now welded shut by thorn growth. The hatch can still be opened by something that understands the substrate's logic — which means it is, in practice, accessible only to the estate's own dead and to the Root Covenant.
- Exits: `north` → 1185 (Rapid Burial Fields South), `down` → 1150 (Grave Tunnel Hinge — alternate egress)

---

### Zone IV: The Older Gravewood (1198–1215) — 18 Rooms

The pre-estate ridge-clan burial ground, now fully integrated into the substrate's network. Forest logic is dominant here. Every path was originally a detour around an ancestor stand; the substrate has made those detours into the only navigable routes. The shamans and Root Covenant practitioners who work this zone treat the trees as participants in any negotiation.

**1198 — The Gravewood Threshold** ★
`room_flags: dark no_recall` | `sector: forest`
Where the estate's managed land gives way to something older. The tree species change abruptly: behind is the estate's managed thornwood, manipulated by cultivation and substrate direction. Ahead are trees that predate the estate by centuries — oaks with boles three meters in diameter, their bark deeply fissured and dark with age. The transition is visible as a line in the ground: on the estate side, root-mass with a grid-like organization corresponding to the estate's planting plan; on the gravewood side, root-mass in a pattern that has no human authorship, dense and complex and old. A stone post at the threshold bears two marks: the Harren thorn device (from when the estate formally annexed this ground) and an older mark beneath it — a pressed handprint in ochre, the same motif that appears in the cave system below. Both marks are still legible. Neither has priority.
- Exits: `south` → 1158 (Forecourt of Inheritance), `southeast` → 1196 (Annex Burial Ground), `north` → 1199 (Ancestor Stand Entry), `west` → 1179 (Estate Orchard)

**1199 — The Ancestor Stand Entry**
`room_flags: dark no_recall` | `sector: forest`
The first ancestor grove encountered on the gravewood's main path — a stand of seven oaks growing in a rough oval around a central clearing that cannot be entered directly. The path detours around the oval's eastern edge, as the ridge-clan practice required: the dead govern that space, and the living have no right of passage through it. The detour is not superstitious avoidance. The root-mass within the oval stands visibly raised above the path level — the substrate holds that space at a different pressure. The trees' lower branches have knitted together over the clearing, forming a roof. What is inside is not visible.
- Exits: `south` → 1198, `north` → 1201 (Thorn-Ring Circle), `east` → 1200 (Face-Down Grove approach)

**1200 — The Face-Down Grove**
`room_flags: dark no_recall cold` | `sector: forest`
The first of the genuine face-down oathbreaker burial sites — a grove where the ridge-clan community interred those who broke sworn covenants, face-down so they could not rise toward the sky. The posture was punitive and prophylactic: the earth was expected to hold what the community rejected. The substrate does. Attempts to dig into this grove produce ground that actively resists — not magically, not suddenly, but with a steady, increasing counter-pressure that builds the deeper you go, as though the geological layer is pressing upward to match any downward force. The trees here lean slightly inward, their canopy closing over the graves. The cold is the same cold as the sealed burial row — @@acrystal-frequency@@N, not atmospheric.
- Exits: `west` → 1199, `south` → 1214 (Gravewood Fringe East)

**1201 — The Thorn-Ring Circle (Old)**
`room_flags: dark no_recall` | `sector: forest`
One of the original ridge-clan containment thorn rings — grown to full maturity over centuries into a solid biological structure nearly two meters high and half a meter thick, enclosing a burial space approximately six meters in diameter. The ring was a sealed verdict: the dead within had been judged and enclosed by the land. The ring's inner surface is smooth where the thorn growth has fused solid; the outer surface is aggressive, with living growth adding new layers annually. This is the substrate's oldest infrastructure. The Root Covenant reveres it without fully understanding it. Even they do not cross inside.
- Exits: `south` → 1199, `north` → 1202 (Path Detour Through Graves), `east` → 1205 (Second Ancestor Grove)

**1202 — The Path Detour Through Graves**
`room_flags: dark no_recall` | `sector: forest`
The ritualized path that the ridge-clan community routed around the major ancestor stands — a deliberate detour that acknowledged the dead's jurisdictional claim over certain spaces. The detour is marked by flat stones set flush with the ground at path-edge intervals, their surfaces bearing directional marks worn almost smooth. Following the marks is not intuitive; the path bends counter to the direction a traveler would choose, and ignoring the marks leads to ground where the substrate's root-pressure makes footing unreliable. The correct path is navigable. The incorrect path is punitive.
- Exits: `south` → 1201, `north` → 1203 (Root Network Clearing), `west` → 1208 (Ridge-Clan Barrow)

**1203 — The Root Network Clearing**
`room_flags: dark no_recall` | `sector: forest`
A natural clearing where the gravewood's root systems have surfaced — not as individual roots but as a coherent network, visible above the ground in braided cable-structures that crisscross the clearing floor. The cables vary from finger-thick to arm-thick, their surfaces alternating smooth and splintered in the pattern that indicates substrate stress vs. stability. A shaman with the knowledge can read the network's current state from here: which directions the substrate is routing energy toward, where pressure is building, whether the binding's integrity is holding in the local section. The clearing smells more strongly of sweet rot and tannin than anywhere else above ground — this is the substrate's ventilation point, where its internal chemistry surfaces.
- Exits: `south` → 1202, `north` → 1211 (Shaman's Hollow), `east` → 1204 (Witch's Boundary Stones), `west` → 1207 (Mire Approach West)

**1204 — The Witch's Boundary Stones**
`room_flags: dark no_recall` | `sector: forest`
A ring of eleven standing stones, waist-high, marking the territorial boundary of the Thornwood Witch's domain. Each stone bears a different mark: some are Harren boundary-notation in estate script, some are Root Covenant contract-glyphs, and some are in a script that predates both — the same pre-estate marking language found on the face-down grove stones and in fragments of the Midgaard Watchers' Cairn site. The Witch has collected all three vocabularies and placed them in conversation. This is not a warning boundary. It is a filing — the Witch's legal claim, expressed in every language the gravewood recognizes.
- Exits: `west` → 1203, `east` → 1205 (Second Ancestor Grove), `south` → 1213 (Substrate Surface Seep)

**1205 — The Second Ancestor Grove**
`room_flags: dark no_recall` | `sector: forest`
A second ancestor stand, older and darker than the first. The grove's canopy has completely closed, making the interior lightless even in daylight. The trees here are the same old-growth oaks, but their root-mass has incorporated a different type of material: the substrate has been building with the remains of the deeper burials below this grove for long enough that the tree roots themselves have taken on mineral properties in the sections nearest the ground — the bark is partially crystalline, carrying the same cold and faint luminescence as the cave system far below. This is the gravewood's deepest surface connection to the substrate layer.
- Exits: `west` → 1201, `south` → 1204 (Witch's Boundary Stones), `east` → 1206 (Whisper Path)

**1206 — The Whisper Path**
`room_flags: dark no_recall` | `sector: forest`
A narrow path maintained by shaman practice — the path where, according to the tradition the shamans preserve, the substrate's output can be heard directly rather than read from secondary signs. The "whisper" is not metaphorical: visitors to this path reliably report hearing a low, irregular sound at the edge of audibility, somewhere between wind and speech, that does not localize to any specific direction. The shamans distinguish between at least four distinct patterns they describe as "attending," "routing," "defending," and "hungry." The fourth pattern is the one they decline to elaborate on.
- Exits: `west` → 1205, `north` → 1211 (Shaman's Hollow), `south` → 1215 (Night Ride Path)

**1207 — The Mire Approach West**
`room_flags: dark no_recall` | `sector: forest`
The western edge of the gravewood where it meets the mire's margins — the transition zone where the forest floor becomes uncertain and the ground's moisture content increases with each step eastward. Pale reeds begin here, their stems visible at the path's edge, already rattling without wind. The trees thin out and their root-mass transitions from the gravewood's dry, complex network to something waterlogged and simplified — the mire's ecology operating on different substrate chemistry. The boundary between forest and mire is not a clean line but a gradient, and the gradient is navigable only because the substrate maintains stepping-stone paths of slightly higher ground.
- Exits: `east` → 1203 (Root Network Clearing), `south` → 1216 (Zone V: Mire Edge)

**1208 — The Ridge-Clan Barrow**
`room_flags: dark no_recall` | `sector: hills`
A proper earthen barrow from the pre-estate burial community — a long mound of piled earth and stone faced with mortarless dry-stone walling at the entrance end. The entrance is sealed with a single upright stone that has not been moved in any documented record. The barrow's outer surface is covered in centuries of moss and thorn growth, making it appear as a natural hill until the stonework becomes visible. The ridge-clan communities buried their honored dead in barrows — facing north, unlike the oathbreaker face-down graves. This barrow is treated by every faction in Thornwood as a protected site: the Harrens' records show it was explicitly excluded from the ossuary expansion plans, the Root Covenant conducts no rites here, and the substrate does not incorporate the barrow's interior into its network. It is the one space in Thornwood that the system leaves alone.
- Exits: `east` → 1202 (Path Detour Through Graves), `south` → 1209 (Cairn of Sealed Names)

**1209 — The Cairn of Sealed Names**
`room_flags: dark no_recall` | `sector: forest`
A cairn of flat river-stones stacked to shoulder height at a crossroads between the older gravewood and the annex burial ground — a point where the two burial traditions intersect. This cairn was built by a community that had watched the Harren estate's deferred-naming practice and understood what was at stake: if the dead could be made nameless by a payment wall, then someone needed to be keeping the names in a form that no clerk could withhold. The stones bear pressed names — dozens of them, in a script that mixes pre-estate ridge-clan marking with Midgaard administrative lettering — names that correspond to the deferred-name case numbers in the estate's register. The cairn is a counter-archive. The substrate has not disturbed it.
- Exits: `north` → 1208, `east` → 1196 (Annex Burial Ground), `south` → 1210 (Bone-Woven Tree)

**1210 — The Bone-Woven Tree**
`room_flags: dark no_recall` | `sector: forest`
A mature oak whose root system has surfaced extensively around its base, and into which — through the root network, not through cutting — the substrate has woven human remains. Femurs appear at root junctions, serving as structural reinforcement. Skulls are cupped in root-forks above head height, their interiors containing a soil-and-crystal compound that may be the substrate's medium. Ribs run along the root cables in parallel. The result is a tree that is also a building, or a building that is also a tree, depending on whether you think function or material is the more fundamental category. The bone-weaving is not crude. It is precise. Each element serves a structural purpose that a builder would recognize.
- Exits: `north` → 1209, `east` → 1203 (Root Network Clearing), `south` → 1229 (Zone V: Mire Egress North)

**1211 — The Shaman's Hollow**
`room_flags: dark indoors no_recall` | `sector: inside`
The Thornwood Witch's residence — a hollow at the base of one of the gravewood's oldest oaks, expanded by root-management over decades into a space large enough for habitation. The interior walls are living root-fiber, formed into shelves, sleeping ledge, and hanging storage. Tools of shaman practice are everywhere: bone-reading sets, thorn-growth maps of the substrate's surface expression, jars of soil samples from different depth layers, a record-keeping system that mixes drawing and inscription in three languages. The Witch herself is rarely here when visitors arrive; her boundary stones suggest she is always somewhere in the gravewood. When she is here, she is not hostile. She is not safe.
- Exits: `south` → 1203 (Root Network Clearing), `east` → 1206 (Whisper Path)

**1212 — The Blood Offering Platform**
`room_flags: dark no_recall` | `sector: forest`
An active Root Covenant ritual site — a flat stone platform at a root-contact point where the substrate's response to blood rites has been demonstrated repeatedly and reliably. The platform's stone surface is stained @@Rdark red@@N in a pattern that shows long use: central deposit, splatter radius, the channels where the offering ran toward the root-contact cracks. Around the platform, the ground carries thorn growth in the pointed, directed pattern that indicates recent substrate attention — the thorns are oriented toward the platform's center, as though the forest is leaning in. The platform is not abandoned. It was used recently.
- Exits: `west` → 1205 (Second Ancestor Grove), `south` → 1213 (Substrate Surface Seep), `east` → 1287 (Zone IX: Covenant Offering Site East)

**1213 — The Substrate Surface Seep**
`room_flags: dark no_recall cold` | `sector: forest`
Where the crystal substrate's influence reaches the surface in visible form: a seep of pale mineral compound welling up through the leaf-litter in a crack that runs roughly east-west, carrying the same cold as the cave system below. The seep is not water — it is a mineral-fluid mixture that maintains temperatures several degrees below ambient indefinitely, crystallizing at its edges into a pale crust. The seep's color is @@alight cyan@@N against the dark forest floor, faintly luminescent. Root-fiber threading through the crack carries the substrate's harmonic in audible range here — the sound the bone-weavers learn to read is clearest at this surface exposure.
- Exits: `north` → 1204 (Witch's Boundary Stones), `east` → 1212 (Blood Offering Platform), `down` → 1270 (Zone VII: Deep Cave Egress — vertical connection to cave system, one-way down only; the cave provides an upward exit back here)

**1214 — The Gravewood Fringe East**
`room_flags: dark no_recall` | `sector: forest`
The eastern fringe where the older gravewood meets the estate's commercial burial fields — the transition zone where ridge-clan burial ground was absorbed into the Harren estate's expansion without acknowledgment. The trees here are younger and less substantial than the gravewood's interior growth; this was the fringe, not the core. The root-mass underfoot shows the mixing: gravewood root patterns from the west, estate-grid root organization from the east, neither fully dominant. @@RDark staining@@N on several trees at eye height, in thin parallel lines, may be the residue of the boundary-marking that the ridge-clan community attempted when they realized the estate was expanding into their burial ground.
- Exits: `west` → 1200 (Face-Down Grove), `east` → 1190 (Thorn-Ringed Plots)

**1215 — The Night Ride Path**
`room_flags: dark no_recall` | `sector: forest`
The route through the southern gravewood that the Wild Hunt uses when departing the estate for its perimeter-extension patrols. The path is marked by the Hunt's passage: frost-etched patterns in the bark of the bordering trees, visible as white tracery, and frost-prints pressed into the soil at the intervals of a horse's gait. The path itself is wider than a human-made trail would be and runs with a directness — through root-mass and undergrowth — that is possible only because the substrate clears it for the Hunt's use. At night, sound does not carry normally along this path. During the day, the path is merely a route. At night, it is a jurisdiction.
- Exits: `north` → 1206 (Whisper Path), `south` → 1280 (Zone VIII: Night Approach Road)

---

### Zone V: The Mire (1216–1231) — 16 Rooms

The peripheral wetland at Thornwood's eastern and southern margins. Funerary runoff, alchemical waste, and substrate water-table seepage have produced a toxic ecology unlike anything else in the estate. Routes are unstable by design — the substrate adjusts subsurface paths to redirect travelers. The fog shrine at the mire's center is the oldest ritual site in Thornwood.

**1216 — The Mire Edge**
`room_flags: dark no_recall cold` | `sector: field`
Where solid ground ends without ceremony. The transition is not a cliff or a clear waterline but a gradual yielding: the earth softens underfoot, then wets, then begins to take footprints that fill slowly with dark water. Pale reeds have advanced to this line and stopped — or rather, the reeds mark this line and the mire marks everything beyond it. The smell changes here: from the forest's rot-and-tannin to something sulfurous and flat, the smell of water that has been holding dead material for a very long time. A stepping-stone path begins at the edge, its stones mossy granite pressed into the mire's surface at irregular intervals.
- Exits: `west` → 1190 (Thorn-Ringed Plots, burial field connection), `east` → 1217 (Stepping Stone Path Entry), `south` → 1230 (Zone IX: Alchemical Runoff Channel approach)

**1217 — The Stepping Stone Path (Entry)**
`room_flags: dark no_recall cold` | `sector: water_swim`
The mire's main navigable route, beginning at the eastern edge and proceeding inward along a sequence of moss-covered granite stones that provide marginally stable footing above the waterline. The stones are set at walking intervals and are large enough for one person at a time. The water between them is dark and still, its depth indeterminate — it takes objects thrown into it without audible impact. Pale reeds grow thickly on either side of the stepping path, their stems angled slightly toward the path as though monitoring it.
- Exits: `west` → 1216, `east` → 1218 (Pale Reed Verge)

**1218 — The Pale Reed Verge**
`room_flags: dark no_recall cold` | `sector: water_swim`
A stretch where the stepping-stone path narrows and the reed growth presses in from both sides. The reeds rattle without wind — their movement is substrate output, readable to shamans as a directional indicator. The rattling here is notably irregular, suggesting the substrate is not holding a single direction but cycling through multiple route assessments. The water between the stones is shallower here, with a bottom visible as pale mineral sediment — the crystal substrate's runoff compositing with the mire's organic accumulation into a layer that has the color and texture of old bone.
- Exits: `west` → 1217, `east` → 1219 (Fog Shroud), `south` → 1226 (Mire Fringe East)

**1219 — The Fog Shroud**
`room_flags: dark no_recall cold` | `sector: water_swim`
The fog bank that occupies the mire's central section, dense enough that depth cues become unreliable at distances beyond two arm-lengths. The fog is not weather — it does not thin in wind or burn off in morning sun. It is a permanent product of the substrate's temperature differential: the crystal substrate's cold meeting the mire's decomposition warmth produces a luminous haze that makes the mire appear to glow from inside. Objects in the fog appear at wrong distances. Sound behaves differently here — speech carries farther than expected, footfalls carry shorter. The stepping-stone path continues through the fog, but the stones are not always where you expect them.
- Exits: `west` → 1218, `east` → 1220 (Black Water Pool), `north` → 1221 (Mire Crossing Central)

**1220 — The Black Water Pool**
`room_flags: dark no_recall cold` | `sector: water_noswim`
A pool of notably still, dark water where the stepping-stone path diverts around rather than through. The pool's surface is intact, undisturbed even by the fog — it has the quality of glass, and it reflects nothing accurately. Occasionally the surface breaks without explanation: a ring, a displacement, then stillness again. The pool's chemical composition is the end-product of the mire's entire waste system: burial runoff, alchemical outflow from the distillation building, substrate seepage. Nothing lives in it visibly. Something uses it.
- Exits: `west` → 1219, `south` → 1224 (Sulfur Vent Channel), `north` → 1221 (Mire Crossing Central)

**1221 — The Mire Crossing (Central)**
`room_flags: dark no_recall cold` | `sector: water_swim`
The middle of the mire, where the stepping-stone path is least reliable and the fog is thickest. The substrate adjusts the subsurface at intervals, slightly repositioning the stones or altering the water depth between them. A traveler who memorized the crossing last time will find it subtly different this time — not enough to prevent passage, but enough to prevent certainty. This is the substrate's territorial behavior applied to navigation: it allows passage but maintains ambiguity.
- Exits: `south` → 1219, `north` → 1222 (Raised Peat Island)

**1222 — The Raised Peat Island**
`room_flags: dark no_recall` | `sector: field`
A stable island of compressed peat and sediment near the mire's center, elevated enough above the waterline to provide reliable footing. The island is roughly oval, large enough to encamp a small group. Its edges are ringed with the pale reeds, and its center is clear — kept that way by something's preference. The fog shrine is east of here. The island's peat surface carries clear footprints: human-shaped, but with the thorn-pattern texture of substrate-integrated feet. The island is used.
- Exits: `south` → 1221, `east` → 1223 (Fog Shrine), `north` → 1229 (Mire Egress North)

**1223 — The Fog Shrine** ★
`room_flags: dark no_recall cold` | `sector: field`
The oldest ritual site in Thornwood, predating the estate, the Root Covenant, and possibly the ridge-clan communities whose burial practices partially inspired the cult's theology. The shrine is a stone platform rising from the peat of a secondary island, accessible by a narrow stepping bridge from the raised peat island. The platform bears offerings: corroded bronze pieces, antler sections, small arrangements of animal bones placed with the precision of a filing system rather than the scatter of reverence. At the platform's center, a carved stone figure has been so thoroughly fitted with iron rings driven into its surface that the original form is unguessable — the rings cover every visible surface, overlapping and pressed flush with each other, as though someone found the original figure's identity a problem and decided to file the problem shut. The shrine is @@pcold even relative to the surrounding mire@@N, and carries the strongest harmonic tone anywhere on the surface — the crystal substrate's signature frequency, amplified through the platform's stone into the surrounding peat.
- Exits: `west` → 1222

**1224 — The Sulfur Vent Channel**
`room_flags: dark no_recall cold` | `sector: water_swim`
A drainage channel carrying the mire's sulfurous output toward the eastern margin, its surface marked by periodic gas venting that produces a flat, dead smell and a faint haze at water level. The channel receives runoff from both the burial fields and the cave system's water table. Walking the channel edge is possible but requires attention: the bank is unstable, the water is not swimmable, and the chemical content of the outflow has produced a pale, slightly crystalline deposit along the channel walls that is slippery under foot.
- Exits: `north` → 1220 (Black Water Pool), `east` → 1225 (Sunken Boundary Wall)

**1225 — The Sunken Boundary Wall**
`room_flags: dark no_recall cold` | `sector: water_swim`
The original estate perimeter wall's southern section, which has subsided progressively into the mire over generations as the wetland expanded. The wall's upper courses are still above waterline — visible as a line of dressed stone running east-to-west, its mortar partly replaced by root-fiber that extends into the water below. The Harren boundary marks are still legible on the above-water faces. The wall still functions as a partial barrier: the substrate uses it as a routing anchor, and the water on the estate's interior side of the wall is navigably different from the water on the exterior side.
- Exits: `west` → 1224, `east` → 1226 (Mire Fringe East)

**1226 — The Mire Fringe East**
`room_flags: dark no_recall cold` | `sector: water_swim`
The eastern mire margin, where the water shallows and the ground firms again — the exit point for travelers crossing the mire from west to east. The reeds here are denser and taller than at the entry, and their rattling has a different quality: more insistent, less informational. The substrate does not want travelers to exit here easily. The correct path through the reeds is findable but requires the same kind of attention the crossing demanded — watching where the reeds angle, where the ground rings hollow, where the stepping stones' moss is compressed by recent use.
- Exits: `north` → 1218 (Pale Reed Verge), `west` → 1225, `east` → 1227 (Broken Causeway)

**1227 — The Broken Causeway**
`room_flags: dark no_recall` | `sector: field`
The remains of a stone causeway that the Harren estate built through the mire's eastern margin to improve transit from the burial fields to the ossuary hillside cut. The causeway succeeded for approximately one generation before the substrate began breaking it — first at the joints, then in the middle spans, until the current state: three intact sections, two collapsed sections, and a gap in the middle that requires either swimming or a different route. The collapsed sections are visible as stone rubble just below the waterline, still serviceable as rough stepping ground for those willing to go in to the knee.
- Exits: `west` → 1226, `north` → 1231 (Substrate Water Sink)

**1228 — The Deep Mire**
`room_flags: dark no_recall cold` | `sector: water_noswim`
The mire's deepest section — the area where the water table is highest and where the accumulated material from generations of drainage has produced a depth that is genuinely dangerous. There is no safe crossing through the deep mire; the substrate maintains it as an impassable barrier in the estate's tactical geography. The water here carries a visibility of zero. The surface occasionally expresses large, slow movement — not the breaking of something coming up, but the settling of something very large shifting position far below. The deep mire connects to the cave system's water table; the crystal substrate's influence is visible as @@alight-cyan phosphorescence@@N in the deepest water, reaching up from the cave formations below.
- Exits: `south` → 1226 (approach only — entering from north not possible; this room is a dangerous dead-end; the only exit is back south)

**1229 — The Mire Egress North**
`room_flags: dark no_recall` | `sector: forest`
The northern mire margin where the wetland meets the southern edge of the older gravewood — the connection between the two zones. The ground here is firm enough to walk on but retains the mire's moisture, producing a soft surface where footprints are preserved indefinitely. A thorn-pattern marker post at the margin boundary carries a Root Covenant filing-glyph: a circle with four diagonal lines, indicating that this crossing is a recognized transit point in the cult's route system. The gravewood's trees begin immediately beyond the marker.
- Exits: `south` → 1222 (Raised Peat Island), `north` → 1210 (Zone IV: Bone-Woven Tree)

**1230 — The Alchemical Runoff Channel**
`room_flags: dark no_recall` | `sector: water_swim`
The drainage channel running south from the distillation outbuilding to the mire, carrying decades of alchemical waste: bone oils, botanical extracts, tannin concentrates, and mineral compounds from the distillation process. The channel's walls carry a chemical deposit that has altered the stone's surface texture — smooth and slightly iridescent where the most reactive compounds settled. The water in the channel is not safe to contact; even the mire's residents avoid the channel itself, using the adjacent ground instead. The channel connects east to the mire proper and west to the distillation outbuilding above.
- Exits: `north` → 1175 (Zone II: Distillation Outbuilding), `east` → 1216 (Mire Edge), `south` → 1231 (Substrate Water Sink)

**1231 — The Substrate Water Sink**
`room_flags: dark no_recall cold` | `sector: water_swim`
A natural depression at the mire's southern margin where the substrate's water table surfaces as a steady seep of cold, mineral-rich water from the cave system below. The seep runs contrary to the mire's prevailing drainage: the mire flows generally eastward, but this sink takes water from below and holds it in a small, permanently cold pool that does not drain into the mire proper. The water in the pool carries @@acrystal-mediated luminescence@@N — faint, pale, and cold, identical to the bioluminescence in the cave system's deepest passages. A shaman who knows what they are looking for can read the sink's output as a direct signal from the binding's current state.
- Exits: `north` → 1227 (Broken Causeway), `west` → 1230 (Alchemical Runoff Channel), `northeast` → 1291 (Zone IX: Wraith Passage)

---

### Zone VI: Ossuary Undercroft (1232–1249) — 18 Rooms

Below the manor and hillside. Funerary and substrate logics are co-dominant here. The bone weavers work in this layer; the drain passages carry the binding's inscription text toward the cave system below. The transition from ossuary to cave is the transition from human-managed funerary industry to the substrate's own domain.

**1232 — The Undercroft Stairs**
`room_flags: dark indoors no_recall cold` | `sector: inside`
The stone stair descending from the wine cellar's iron hatch into the ossuary undercroft. The stairs are narrow, cold-mortared, and steep — built for function, not comfort. The descent takes a traveler from the wine cellar's ambient cold (already notable) into a qualitatively different cold: the bone-dust dryness of the ossuary layer, where temperature is maintained by the crystal substrate below rather than by any natural process. The stair walls carry the first of the drain passage inscriptions — not the urgent, small-script text of the lower passages, but the full-size, deliberate lettering of the binding's opening clauses.
- Exits: `up` → 1169 (Wine Cellar), `north` → 1233 (Upper Undercroft Hall)

**1233 — The Upper Undercroft Hall**
`room_flags: dark indoors no_recall cold` | `sector: inside`
The formal entry to the ossuary layer — the administrative reception space for the undercroft, where intake records were processed before remains were assigned to specific vault sections. A desk and record-stand still occupy the hall's entry end; the records in the stand are organized by the estate's case-number system, cross-referenced to the deferred-name register above. The ossuary's cataloguing system here is meticulous and unsentimental: each entry notes the case number, intake date, bone condition, vault assignment, and processing notes in the bone weaver's abbreviated professional vocabulary. The hall's ceiling is vaulted, and the vault springs carry @@psubstrate-glyph decorations@@N cut into the stone — not the estate's heraldry, but the binding's inscription geometry.
- Exits: `south` → 1232 (Undercroft Stairs), `north` → 1234 (Bone-Sorting Station), `east` → 1240 (Cut-Stone Passage)

**1234 — The Bone-Sorting Station**
`room_flags: dark indoors no_recall cold` | `sector: inside`
The first active processing area — where bone weavers perform the initial categorization of material coming in from the hillside ossuary and from the exhumation transit hall. Long stone tables with partitioned sections hold sorted material by type: long bones, flat bones, small bones, skull material, mixed-origin material for the construct assembly floor. The bone weavers' tools are present: measurement gauges, density testers, selection rods for identifying structural-grade material. The station is operational. The sorting continues regardless of visitors. The bone weavers do not pause.
- Exits: `south` → 1233, `north` → 1235 (Femur Gallery), `east` → 1238 (Bone-Weaver's Workshop)

**1235 — The Femur Gallery**
`room_flags: dark indoors no_recall cold` | `sector: inside`
A long corridor whose walls have been converted into structural bone storage: femurs and tibiae stacked horizontally in recessed niches, floor to ceiling, their ends facing outward in a pattern that is simultaneously storage and load-bearing structure. The bones are not merely placed — they are integrated with the wall's limestone, held in place by root-fiber threading at the joints, the organic and mineral fused into a composite material that the bone weavers treat as more durable than either component alone. The gallery's floor is polished smooth by generations of foot traffic and cart passage. Sound in the gallery is unusual: it arrives flat, with no echo — the bone-and-root composite absorbs rather than reflects.
- Exits: `south` → 1234, `north` → 1236 (Skull Vault), `east` → 1242 (Exhumation Transit Hall)

**1236 — The Skull Vault**
`room_flags: dark indoors no_recall cold` | `sector: inside`
A domed chamber dedicated to skull storage, organized by the bone weavers' categorization system: skulls sorted by density, age-at-death, and "resonance grade" — a quality the bone weavers assess by striking the skull and listening to the harmonic output. Higher-resonance skulls are stored in the inner ring of wall-niches; lower-resonance in the outer ring. The skulls in the inner ring are all from the sealed burial row above — the substrate-connected burials whose resistance to exhumation was eventually overcome through a technique the bone weavers decline to describe in detail. The dome's interior surface carries one complete verse of the binding's inscription text, pressed into the stone in the Root Covenant's contract-liturgy script, circling the dome at eye height.
- Exits: `south` → 1235, `east` → 1237 (Rib-Cage Armature Chamber)

**1237 — The Rib-Cage Armature Chamber**
`room_flags: dark indoors no_recall cold` | `sector: inside`
A workshop space where partially assembled construct frames are stored between working sessions. Rib-cage sections — from multiple sources, assembled into larger composite structures — hang from ceiling hooks on chains, their curved geometry holding internal volumes that will eventually house other components. Against the walls, armature sections in various stages of completion: some are clearly recognizable as enlarged human torso-structures; others have departed from the human template entirely, incorporating elements from non-human remains in configurations the substrate appears to have specified.
- Exits: `west` → 1236, `south` → 1238 (Bone-Weaver's Workshop)

**1238 — The Bone-Weaver's Workshop**
`room_flags: dark indoors no_recall cold` | `sector: inside`
The master bone weaver's primary working space — where the most demanding construction work is done. Workbenches carry in-progress pieces; tool racks hold implements that have no counterparts in ordinary craft: resonance hammers sized to specific frequency ranges, root-threading needles with substrate-treated tips, structural gauge sets calibrated against the ossuary's material standards. The master bone weaver's bench holds a current project: a hand assembly, five fingers in varying stages of completion, the joints threaded with root-fiber at precisely the same gauge as the bone-woven tree in the gravewood above. The workshop is tidy in the way that a craftsman's space is tidy — everything where it can be found, nothing decorative.
- Exits: `north` → 1234 (Bone-Sorting Station), `west` → 1237, `south` → 1247 (Construct Assembly Floor)

**1239 — The Hillside Ossuary Entry**
`room_flags: dark indoors no_recall cold` | `sector: inside`
The connection point between the burial fields above and the ossuary undercroft: the inner face of the hillside cut, where the gated entrance opens into the undercroft's eastern section. The approach from the burial fields (via 1187) deposits arrivals here. The entry chamber is fitted with intake hardware — a scale, a manifest-check stand, a lamp-hook now empty — that mirrors the surface processing equipment above. The duplicate infrastructure is deliberate: the estate maintained parallel processing systems at both the surface and undercroft levels so that remains could be received from above or below without crossing operational paths.
- Exits: `up` → 1187 (Zone III: Ossuary Hillside Cut), `west` → 1240 (Cut-Stone Passage)

**1240 — The Cut-Stone Passage**
`room_flags: dark indoors no_recall cold` | `sector: inside`
A passage cut directly into the hillside limestone, older than the formal undercroft construction — this was the original tunnel that predated the Harren estate's systematic ossuary development, part of the same burial-route infrastructure as the grave tunnels connecting to Midgaard and Shadowmere. The walls here carry marks from three distinct eras of use: the pre-estate chisel-work in the stone, Harren-era carved administrative markers, and Root Covenant inscription additions. The passage smells differently from the formal undercroft — older, drier, more mineral, with a trace of the cave system's crystal cold coming from below.
- Exits: `east` → 1239 (Hillside Ossuary Entry), `west` → 1233 (Upper Undercroft Hall), `south` → 1241 (Deep Ossuary Vault)

**1241 — The Deep Ossuary Vault**
`room_flags: dark indoors no_recall cold` | `sector: inside`
The densest bone-storage area in the undercroft — a large vaulted chamber where the accumulated processed remains of the estate's operational peak are stacked to the ceiling in organized sections. The sheer volume is the first thing: the chamber holds more material than the surface burial fields would suggest, because the substrate's assimilation of ridge-clan remains into the network added to the estate's material inventory without any corresponding surface interment. The bone weavers treat this vault as their primary raw material supply — a library of structural elements organized by the same cataloguing system as the upper vaults, but more dense and more varied.
- Exits: `north` → 1240, `south` → 1242 (Exhumation Transit Hall), `down` → 1244 (Drain Passage Entry)

**1242 — The Exhumation Transit Hall**
`room_flags: dark indoors no_recall cold` | `sector: inside`
The routing corridor between the burial fields (via the hillside cut) and the ossuary processing stations — where remains in transit passed from intake to processing. The hall's walls carry transit records: chalk marks on a slate board updated by each shift of bone-processing laborers, recording what moved where and when. The most recent entries on the board are not in chalk. They are pressed into the slate by root-fiber — the substrate recording its own transit activity in the estate's inherited notation system.
- Exits: `north` → 1235 (Femur Gallery), `east` → 1239 (Hillside Ossuary Entry)

**1243 — The Resonance Chamber**
`room_flags: dark indoors no_recall cold` | `sector: inside`
A circular chamber specifically designed for the bone-weaver practice of resonance testing — assessing material by its harmonic output when struck. The chamber's geometry produces natural amplification; a bone struck at the center can be clearly evaluated from the walls. The bone weavers use this chamber for high-grade material assessment, but also for something else: the resonance testing of assembled construct sections, to confirm that the substrate integration is functioning correctly in the finished work. The tone produced by a properly integrated construct section matches the crystal substrate's harmonic signature. The chamber is used regularly; the air carries a faint harmonic afterimage of recent sessions.
- Exits: `east` → 1241, `south` → 1247 (Construct Assembly Floor)

**1244 — The Drain Passage Entry**
`room_flags: dark indoors no_recall cold` | `sector: inside`
The beginning of the drain passages — channels originally built to carry water drainage from the ossuary vaults downward to the cave system's water table, now carrying the binding's inscription text on their walls. At full height here, the text is legible as a deliberate, carefully lettered working in the binding's script: the opening of the binding's formal declaration, in large characters with generous spacing. This section reads clearly and confidently. The passage smells of cold mineral and the faint ozone-adjacent quality of the crystal substrate's chemistry.
- Exits: `up` → 1241 (Deep Ossuary Vault), `down` → 1245 (Upper Drain Passage)

**1245 — The Upper Drain Passage**
`room_flags: dark indoors no_recall cold` | `sector: inside`
The drain passage's upper middle section, where the binding's inscription text continues. The characters here are smaller — not from a smaller hand, but from a recalibration: the author had more text to fit than the passage's length easily accommodates. The text is still fully legible and the lettering is still controlled, but the density is visibly higher than above, and the wall surface is used more completely. The content shifts here from declaration to specification: where the upper section stated the binding's purpose, this section describes its mechanisms and dependencies.
- Exits: `up` → 1244, `down` → 1246 (Lower Drain Passage)

**1246 — The Lower Drain Passage**
`room_flags: dark indoors no_recall cold` | `sector: inside`
The drain passage's final section before the cave system, where the binding's inscription text becomes urgent. The characters are very small now — the lettering of someone who is running out of both space and, possibly, time. The text covers every available surface: walls, ceiling, and the passage floor where it is not covered by water. The content here appears to describe contingencies: what happens if specific components of the binding fail, what substitutions are permitted, what cannot be substituted. The @@pcold blue glow@@N of the crystal substrate's bioluminescent fungal output begins here, coming from below, providing the only light in the passage.
- Exits: `up` → 1245, `down` → 1250 (Zone VII: Cave Mouth)

**1247 — The Construct Assembly Floor**
`room_flags: dark indoors no_recall cold` | `sector: inside`
A large open floor where the bone weavers assemble their larger-scale constructs — marrow constructs and bone crypt configurations that cannot be built in the workshop's limited space. The ceiling here is high, allowing for vertical assembly. Chain hoists hang from iron fixtures in the ceiling for lifting large sections. The floor surface is marked with assembly reference lines: geometric guides for positioning limb angles, torso proportions, and articulation junction placement. Current projects in various stages of completion stand along the walls. The largest is not finished. It has been in progress for some time. Its scale exceeds what the estate's ossuary alone could supply — the substrate appears to have been routing material here from sources the bone weavers' records do not document.
- Exits: `north` → 1238 (Bone-Weaver's Workshop), `east` → 1243 (Resonance Chamber), `south` → 1248 (Marrow Fiend Vault)

**1248 — The Marrow Fiend Vault**
`room_flags: dark indoors no_recall cold` | `sector: inside`
Storage for completed large-scale constructs — the finished marrow fiends and bone crypt guardians waiting for deployment. The vault is tall and long, and the constructs that occupy it are correspondingly large: four meters or more in height, with excess limbs, rib-cage torsos, and articulated bone-plate coverage that gives them the visual impression of collapsed architecture that has decided to stand up. They are still. The stillness is not inactivity — the substrate holds them in reserve, and the harmonic in this vault is notably stronger than elsewhere in the undercroft, as though the crystal is paying closer attention to this room than to the spaces above.
- Exits: `north` → 1247, `east` → 1249 (Cave System Entry Threshold)

**1249 — The Cave System Entry Threshold**
`room_flags: dark indoors no_recall cold` | `sector: inside`
The boundary between the ossuary undercroft and the cave system below — where cut stone gives way to natural limestone and the substrate's geological layer becomes physically accessible. The threshold is marked by a change in construction: the undercroft's mortared stonework ends at a ragged line, and the cave's natural formation begins without a designed transition. Someone has added carved marks at the transition — not the estate's administrative notation, not the Root Covenant's contract script, but the same pre-linguistic pressed handprint in ochre that appears at the grave tunnel hinge, the gravewood threshold, and the cave system's interior. The substrate marked its own boundary.
- Exits: `west` → 1248 (Marrow Fiend Vault), `down` → 1250 (Zone VII: Cave Mouth)

---

### Zone VII: Cave System and Substrate Core (1250–1270) — 21 Rooms

The deepest layer. Substrate logic is total here. Lord Harren's authority does not reach below the undercroft; the Lich's authority is absolute. The crystal formation, the handprint cave, the high ledge, and the sealed chamber are the architectural heart of what Thornwood actually is. Threat tier 68–80.

**1250 — The Cave Mouth**
`room_flags: dark no_recall cold` | `sector: inside`
The natural cave entrance from the ossuary undercroft — the first room of the cave system proper. The floor transitions from cut stone to cave floor: limestone smoothed by water flow over geological time, not by human work. The ceiling drops and the walls become irregular, following the natural formation rather than any planned geometry. The cold here is different from the undercroft's bone-dust cold: it is wet and mineral, carrying the cave system's water table humidity, and it comes with the faint harmonic tone that will intensify throughout the descent. The @@acold blue luminescence@@N of the substrate-connected fungal network begins on the cave walls here, in sparse patches that increase in density with depth.
- Exits: `up` → 1249 (Cave System Entry Threshold), `north` → 1251 (Crystal Passage Entry)

**1251 — The Crystal Passage Entry**
`room_flags: dark no_recall cold` | `sector: inside`
The first sighting of the crystal formation: a vein of pale crystal running through the limestone of the passage's eastern wall, half a meter wide and running floor to ceiling before disappearing into the rock above and below. The crystal carries the same anomalous cold as the surface seep in the gravewood — several degrees below ambient — and produces a faint luminescence that colors the passage walls @@alight blue@@N. The vein is not the formation's extent; it is an exposure point, a place where the geological structure surfaces through the limestone. The formation itself continues in all directions, deeper and wider than any exposure suggests.
- Exits: `south` → 1250, `north` → 1252 (Low Crystal Gallery), `east` → 1254 (Harmonic Alcove)

**1252 — The Low Crystal Gallery**
`room_flags: dark no_recall cold` | `sector: inside`
A gallery where crystal exposures are dense enough to provide significant luminescence without any external light source — the cave walls here glow @@asoft cyan@@N from multiple points, producing a shadowless illumination that makes the space feel larger than it is. The crystal veins here are older and more complex than the passage entry exposure: they branch and interweave through the limestone, and their surfaces show the growth rings of centuries of slow crystal formation. The Root Covenant has carved offering niches into the wall below each major vein — small recesses containing bone chips and pressed handprints in ochre, the same motif as the threshold marks above.
- Exits: `south` → 1251, `north` → 1255 (Handprint Cave), `west` → 1253 (Water Table Pool)

**1253 — The Water Table Pool**
`room_flags: dark no_recall cold` | `sector: water_swim`
A natural pool where the cave system's water table surfaces. The water is cold beyond what the air temperature alone would produce — it carries the crystal substrate's thermal anomaly directly, and its surface is faintly @@aluminescent@@N where the crystal compounds have dissolved into the water column. This pool feeds the substrate water sink in the mire above: the connection is underground and not directly passable, but the water chemistry is identical. The pool's bottom is not visible. Animals do not drink from it. The bone weavers treat the pool's output as a material of specific applications.
- Exits: `east` → 1252, `south` → 1261 (Secondary Sigil Floor)

**1254 — The Harmonic Alcove**
`room_flags: dark no_recall cold` | `sector: inside`
A natural alcove where the crystal formation produces audible harmonic output — the tone that shamans and bone weavers learn to read, here at its clearest and most accessible surface expression. The tone is constant: a single sustained note at the low edge of human hearing, with overtones that shift in a pattern the cult's bone-readers describe as the substrate's "attention state." The alcove's walls carry the crystal frequency's effect on the surrounding limestone — a slow erosion pattern that corresponds to the harmonic's geometry, producing a surface texture that a trained hand can read like Braille.
- Exits: `west` → 1251, `south` → 1263 (Lichen-Light Passage)

**1255 — The Handprint Cave** ★
`room_flags: dark no_recall cold` | `sector: inside`
A chamber whose entire surface — floor, walls, ceiling, every accessible face — is covered with overlapping right handprints in ochre pigment, all in identical posture: palm flat, fingers extended, thumb at right angles. The prints are not all the same size; they range from a small child's to a large adult's, with every intermediate size represented. They overlap at every density, with older prints visible beneath newer ones, the layers building up over what must be centuries of continuous addition. A fresh print — ochre still damp — occupies the chamber's eastern face. Someone was here recently. The prints read, per the Root Covenant's interpretation, as a witness record: every hand that consented to or was consumed by the binding, pressed into the stone as permanent testimony. Whether the most recent print indicates a new addition to the binding's accounting, or something else, is unclear.
- Exits: `south` → 1252 (Low Crystal Gallery), `east` → 1256 (Consent Inscription Hall), `north` → 1257 (High Cave Ledge)

**1256 — The Consent Inscription Hall**
`room_flags: dark no_recall cold` | `sector: inside`
A hall adjacent to the handprint cave where the binding's consent apparatus is documented in text rather than impression. The walls carry a list — names and contract-notations — in a script that blends the binding's formal inscription language with Midgaard's administrative lettering. The entries predate the estate; the oldest are in the ridge-clan marking system. The most recent are in Root Covenant contract-liturgy. The list is long. Many of the names correspond to the deferred-name case numbers in the estate's register above. Whether this means the deferred-name dead are recorded here as binding participants — or binding material — is the central question of the archive problem that the Reckoning faction in Midgaard is trying to answer.
- Exits: `west` → 1255, `south` → 1262 (Cold Dark Corridor)

**1257 — The High Cave Ledge** ★
`room_flags: dark no_recall cold` | `sector: inside`
A natural ledge overlooking the crystal heart basin below — a viewing point where the binding's geometry is visible as a coherent pattern in the cave system's layout. From this vantage, the crystal formation's arrangement in the basin resolves into a shape that matches the sigil inscribed on the wine cellar floor and the sealed chamber below: a complex inscribed circle, executed in geological rather than drawn form, with the crystal veins as the inscribed lines. The pattern shows unmistakable signs of stress: the crystal formation's symmetry is degrading, with several vein sections showing darkening and one major section where the luminescence has gone entirely dark. The binding is failing. It is visible from here in the way that structural damage is visible in a building when you step back far enough to see the whole facade.
- Exits: `south` → 1255 (Handprint Cave), `down` → 1258 (Crystal Heart Basin), `east` → 1264 (Cave Echo Chamber)

**1258 — The Crystal Heart Basin** ★
`room_flags: dark no_recall cold no_mob` | `sector: inside`
The crystal formation's deepest accessible point — a basin of crystal growth at cave-floor level, where the formation is densest and oldest. The basin is roughly circular, perhaps fifteen meters in diameter, with crystal growth rising from the floor in columns and branching forms. The luminescence here should be intense; it is not. The formation is dimming. The cold remains — this is the coldest room in the area — but the light is reduced to a fraction of what the column growth implies it once produced, leaving large sections of the basin in a darkness that the remaining luminescence makes more apparent by contrast. The Root Covenant's four competing theories about the binding's failure are all legible here: the Lich's theory (coherence loss) in the failing geometry, Harren's theory (expanding pressure) in the stressed crystal at the basin edges, the cult's theory (renegotiation demand) in the fresh offering-deposits on the basin floor, and the witches' theory (already failed, restarting) in the new crystal growth beginning at the base of the darkened sections — small, @@adim, and differently structured@@N from the original formation.
- Exits: `up` → 1257 (High Cave Ledge), `north` → 1259 (Root Network Hub), `east` → 1265 (Ancient Formation Gallery)

**1259 — The Root Network Hub**
`room_flags: dark no_recall cold` | `sector: inside`
The underground convergence point of the root systems that distribute the binding's effects across the estate above. The cave ceiling here is penetrated by root-mass descending from the ossuary and gravewood layers — thick cables of biological material that have grown through limestone over generations to reach the crystal formation below. The roots are connected to the crystal at contact points where the rock has been dissolved by the roots' chemistry, allowing direct mineral exchange. The hub is not a metaphor. It is an infrastructure node. The binding's effects move from crystal to root network to surface here, and the operational state of the hub is directly readable in the root cables' condition: some are vigorous; some are degraded; one major cable on the eastern wall has separated from its crystal contact point entirely.
- Exits: `south` → 1258, `west` → 1260 (Binding Inscription Passage)

**1260 — The Binding Inscription Passage**
`room_flags: dark no_recall cold` | `sector: inside`
The lower end of the drain passage inscription system — the cave-level passage where the binding's text reaches its most urgent expression. The inscription here is not on cut stone but on natural limestone, pressed into the cave wall's surface by root-fiber rather than carved by tool. The characters are very small, very dense, and cover every available surface including the floor and ceiling. The content, where it can be read, appears to be the binding's emergency protocols: what the binding does when components fail, what the Lich is authorized to substitute, and what substitutions are explicitly forbidden. The forbidden list is longer than the permitted list.
- Exits: `east` → 1259 (Root Network Hub), `south` → 1266 (Sealed Chamber Approach)

**1261 — The Secondary Sigil Floor**
`room_flags: dark no_recall cold` | `sector: inside`
A natural chamber where the binding's secondary inscription — the sigil identical to the wine cellar's floor inscription — appears in its geological form: the crystal veins in the cave floor have been arranged into the sigil's pattern by the same process that produced the surface seep's crystallization. The arrangement is not perfect; the geological constraints of the formation produce slight variations from the drawn version in the wine cellar. Whether the geological version or the drawn version is the original — whether the Lich inscribed the wine cellar sigil to match a formation it found, or the formation grew toward a sigil the Lich inscribed — is a question about which came first that neither version of the text answers.
- Exits: `north` → 1253 (Water Table Pool), `east` → 1262 (Cold Dark Corridor)

**1262 — The Cold Dark Corridor**
`room_flags: dark no_recall cold` | `sector: inside`
A passage where the crystal substrate's thermal anomaly is strongest and the luminescence has failed entirely, producing absolute darkness combined with cold that registers as physically painful on exposed skin. The cold here is not natural — it is the crystal formation under stress, its thermal regulation failing in the same pattern as its luminescence. The passage connects the secondary sigil floor to the consent inscription hall, running along the binding's eastern margin. Travelers without their own light source are effectively blind; travelers with light source are navigable but uncomfortable. The substrate does not maintain this passage for convenience.
- Exits: `west` → 1261 (Secondary Sigil Floor), `north` → 1256 (Consent Inscription Hall)

**1263 — The Lichen-Light Passage**
`room_flags: dark no_recall cold` | `sector: inside`
A passage colonized by the substrate-connected fungal network that provides the cave system's bioluminescent baseline — the cold blue light visible throughout the formation's healthier sections. Here the fungal growth is dense enough to provide reliable navigation light in @@asoft, unsteady blue@@N. The lichen has grown according to the root network's routing, covering ceiling and walls in patterns that correspond to the binding's geometry: the lichen is densest where the binding is intact and thinner where the binding is degraded, providing a direct visual map of the binding's current status for anyone who knows what they are looking at.
- Exits: `north` → 1254 (Harmonic Alcove), `south` → 1265 (Ancient Formation Gallery)

**1264 — The Cave Echo Chamber**
`room_flags: dark no_recall cold` | `sector: inside`
A natural chamber whose geometry produces sustained echo — sound introduced here reflects and compounds for several seconds before decaying. The substrate uses this chamber's acoustic properties: the crystal harmonic tone, transmitted through the root network, arrives here amplified and sustained, producing a room-filling resonance that the bone weavers' resonance-testing methodology was calibrated against. The echo of voices here is disorienting — what you said three seconds ago is still audible as a slight distortion of what you are saying now. Experienced Root Covenant practitioners use the echo chamber for bone-reading diagnostics, checking the substrate's harmonic against their calibrated expectations.
- Exits: `west` → 1257 (High Cave Ledge), `south` → 1263 (Lichen-Light Passage)

**1265 — The Ancient Formation Gallery**
`room_flags: dark no_recall cold` | `sector: inside`
A gallery displaying the oldest sections of the crystal formation — growth that predates human use of the site and extends back beyond any dateable layer. The crystals here are larger and more complex than the formation's other sections, with internal structures visible as banding and inclusion patterns accumulated over centuries. Their surface texture differs from the younger sections: the new growth is smooth and faceted; the ancient growth is rough and irregular, as though the formation's earliest expansion was exploratory rather than organized. In the gallery's deepest alcove, the crystal carries an internal banding pattern that matches the geometry of the Spirebound Conclave's documented Prism Thesis crystal field trial markings — a match that would be significant if confirmed, connecting Thornwood's substrate to the same experimental program that produced the Withered Depths' blight. The match has not been confirmed. It has not been investigated.
- Exits: `west` → 1258 (Crystal Heart Basin), `north` → 1263 (Lichen-Light Passage)

**1266 — The Sealed Chamber Approach**
`room_flags: dark no_recall cold` | `sector: inside`
The final passage before the Lich's domain — a straight corridor whose walls carry the binding's inscription in its most complete and deliberate form: every clause of the binding's formal text, rendered in the original full-size lettering of the inscription's first section, as though this passage was the version produced when there was still time and space to be thorough. The passage ends at a door of stone fitted into the natural cave wall — not a manufactured door but a stone slab placed to seal the opening, bearing the binding's primary sigil carved into its outward face. The slab is cold to the touch and resonates faintly under pressure.
- Exits: `north` → 1260 (Binding Inscription Passage), `south` → 1267 (Threshold of the Binding)

**1267 — The Threshold of the Binding** ★
`room_flags: dark no_recall cold no_recall no_portal no_teleport` | `sector: inside`
The final threshold before the sealed chamber — the door room where the Harren Estate Register Key is required for passage. The stone slab door is here, its sigil face toward this room. The threshold carries all four of the binding's operating logics simultaneously: the door's carved thorn-motif surround (manorial), the bone-inlay at the door's base (funerary), the root-fiber threading through the door's stone in the same pattern as the wine cellar inscription (forest), and the crystal formation embedded in the door's stone itself glowing at @@plow, pulsed frequency@@N (substrate). The four logics as one mechanism. The door cannot be forced by ordinary means. It responds to the register key because the key's object properties are calibrated to the binding's own authentication layer.
- Exits: `north` → 1266, `south` → 1268 (Sealed Chamber — key-locked door; key: object 1220)
- Door: `^sealed stone door` — `EX_ISDOOR` set, reset to locked (state 2), key_vnum = 1220

**1268 — The Sealed Chamber** ★ (Boss room: Thornwood Lich — `no_mob` flag)
`room_flags: dark indoors no_recall cold no_mob no_portal no_teleport` | `sector: inside`
The Thornwood Lich's domain — the chamber where the binding's primary sigil is inscribed on the floor and where the Lich occupies both the sigil's center position and the binding's operational interface. The chamber is circular, the inscribed circle filling its entire floor, the lines of the sigil rendered in crystal-inlay that provides the room's only illumination — @@pdim purple@@N at the sigil edges, @@acold blue@@N at the crystal points. The Lich itself does not conform to any expected appearance. It is present, in the way that an architectural principle is present: visible in what the room does rather than in any single located form. When it chooses to speak, the sound comes from the crystal formation rather than from any specific point. It speaks in systems. It does not threaten. It does not explain. It diagnoses. If it addresses an intruder, it is because the intruder has become relevant to the binding's current failure analysis — and that is not good news for the intruder.
- Exits: `north` → 1267 (key-locked door from this side: `EX_ISDOOR`, can be opened from inside without key)
- Boss: The Thornwood Lich (mob 1193) — sentinel, boss, no_mob room, spec_cast_undead

**1269 — The Substrate Interface**
`room_flags: dark indoors no_recall cold` | `sector: inside`
A secondary chamber adjacent to the sealed chamber, connected by a narrow passage in the natural cave wall — accessible, but not the sealed chamber's main entrance. This is where the Lich communicates with the root network: a crystal-dense wall surface carries the binding's transmission layer, and the root cables that descend from the network hub connect to this surface. The Lich's adjustments to the binding — the corrections and calibrations that keep the system in its current degraded-but-functional state — happen here, expressed as changes in the crystal's harmonic output that propagate upward through the root network to the estate's surface behavior. The room has the quality of a control room whose operator has been on continuous duty for a very long time.
- Exits: `east` → 1268 (Sealed Chamber, via internal passage — does not require key from this side), `north` → 1259 (Root Network Hub)

**1270 — The Deep Cave Egress**
`room_flags: dark no_recall cold` | `sector: inside`
A natural upward passage at the cave system's northwestern margin — a crack in the limestone wide enough to climb, leading up through the geological layers toward the gravewood's surface. The climb is long and requires equipment or substrate familiarity to navigate safely. The Thornwood Witch knows this passage. The Root Covenant's senior practitioners know it. For everyone else, it is a crack in the rock that does not obviously lead anywhere. At the top, the passage emerges at the substrate surface seep in the older gravewood — the place where the crystal substrate's surface expression is strongest.
- Exits: `south` → 1259 (Root Network Hub), `up` → 1213 (Zone IV: Substrate Surface Seep)

---

### Zone VIII: Wild Hunt Patrol Routes (1271–1280) — 10 Rooms

The Wild Hunt's operating territory — the roads, staging grounds, and patrol routes where the estate's mobile enforcement arm projects Thornwood's jurisdiction beyond the fixed perimeter. Night-only content is concentrated here; the Wild Hunt Master and Riders only spawn at night.

**1271 — The Hunt Road Entry**
`room_flags: dark no_recall` | `sector: forest`
Where the Wild Hunt's patrol routes begin, accessible from the memorial lane's northern end. The road here is unlike anything else in the burial fields: unnaturally wide, its surface clear of the root-mass that claims every other path, and bordered by trees whose bark has been marked at regular intervals with @@yharren-gold thorn devices@@N pressed in ochre — not the estate's administrative markers but the Hunt's jurisdiction claim, the equivalent of a posted notice. By day the road is simply a wide, clear path. At night the marks in the bark glow faintly, and the path becomes something that has specific rules about who may travel it.
- Exits: `south` → 1195 (Zone III: Memorial Lane), `north` → 1272 (Antler Marker Crossing)

**1272 — The Antler Marker Crossing**
`room_flags: dark no_recall` | `sector: forest`
A crossroads where the Hunt's main patrol road is intersected by a secondary track running east-west — the Hunt's lateral route that allows it to cover the burial field margins and the gravewood's southern fringe in a single circuit. The crossing is marked by four antler installations: genuine antlers driven into the ground at the road corners, their tines oriented outward as territorial indicators. The antler-crown motif repeats: the Wild Hunt Master's authority, expressed in the decorative vocabulary of the pre-estate ridge-clan's hunting traditions. The Hunt has claimed those traditions as its own.
- Exits: `south` → 1271, `north` → 1273 (Shadow Run), `east` → 1279 (Return Gate), `west` → 1278 (Quarry Cache)

**1273 — The Shadow Run**
`room_flags: dark no_recall` | `sector: forest`
A straight stretch of the Hunt road where the substrate's management of the path is most apparent: the trees on either side have had their canopy closed over the road, creating a tunnel effect, and the substrate has cleared the road surface to a smoothness that amplifies the sound of hooves — or would, if the substrate horses made sound. The Shadow Run's purpose is speed; the substrate has optimized this section for the Hunt's movement rate. During the Hunt's return from external patrols, this is where the riders' pace increases to the rate that witnesses describe as "arriving before the horn sound does."
- Exits: `south` → 1272, `north` → 1274 (Frost-Print Trail)

**1274 — The Frost-Print Trail**
`room_flags: dark no_recall cold` | `sector: forest`
The section of the Hunt route where the shadow hounds' frost-prints are densest — the ground here retains the frost impressions of their passage even in daylight, in the pattern of a pack running in loose formation rather than in pairs. The frost-prints are notable for their detail: each print shows claw extension, pad distribution, and gait asymmetry consistent with a specific individual hound, not a template. The pack has individuals. The substrate knows which one left each print. The cold here is not atmospheric — it is localized to the print-bearing ground, radiating upward from the frost impressions rather than downward from the air.
- Exits: `south` → 1273, `north` → 1275 (Calling Beacon)

**1275 — The Calling Beacon**
`room_flags: dark no_recall` | `sector: forest`
The point on the Hunt's road where the Hunt Master sounds the black shadow-bone horn before departing for external patrols. The beacon is not a structure but a position: a specific point where the road widens into a small clearing and where the root network is directly connected to the cave system's harmonic channel — meaning the horn's tone travels through the substrate rather than through air, arriving at its destinations before the Hunt itself does. The ground at the clearing's center is permanently frost-marked from the substrate horses' departures, their standing prints preserved in the same way as the hound prints to the south.
- Exits: `south` → 1274, `north` → 1276 (Hunt Dispatch Point)

**1276 — The Hunt Dispatch Point** ★ (Boss room: Wild Hunt Master — `no_mob` flag)
`room_flags: dark no_recall no_mob` | `sector: forest`
The Wild Hunt Master's operational headquarters — the point from which the Hunt departs and to which it returns. The Master is here when not on patrol: mounted on his substrate horse, the antler crown's root-material tines oriented outward to receive the root network's signals, his face painted with sigils that match the binding's inscription language. He is not at rest. The antlers function as continuous receiving apparatus; he is always processing the estate's territorial status. The Hunt Dispatch connects north to the routes that lead beyond the estate's fixed perimeter — exits that go into the broader woodland and return through the same point. The Hunt's jurisdiction extends as far as its riders travel. The Master's authority is the binding's will expressed as velocity.
- Exits: `south` → 1275 (Calling Beacon), `west` → 1280 (Night Approach Road)
- Boss: The Wild Hunt Master (mob 1191) — sentinel, boss, night_only, no_mob room, spec_cast_undead

**1277 — The Birch-Bark Road**
`room_flags: dark no_recall` | `sector: forest`
The Hunt's primary patrol corridor beyond the estate's inner zone — a road bordered by white birch trees that the substrate has grown into the Hunt's route as navigation markers. The birch-bark coloring mirrors the substrate horses' coloring; in the dark, the road and its riders become the same pale against the surrounding forest. Sound does not carry here in the normal way — not because the room is quiet, but because the substrate routes ambient sound away from this road, keeping the Hunt's approach silent. The only sound that travels here correctly is the horn's substrate-transmission, which arrives through the ground rather than through air.
- Exits: `east` → 1276 (Hunt Dispatch Point), `west` → 1280 (Night Approach Road)

**1278 — The Quarry Cache**
`room_flags: dark no_recall` | `sector: forest`
Where the Hunt brings material it retrieves from beyond the estate's perimeter — remains, artifacts, or occasionally persons the substrate has determined must be returned. The cache is a natural hollow at the road's western margin, its interior fitted with the same iron hooks and manifest-stands as the estate's surface processing systems. Items brought back by the Hunt are deposited here for processing by the bone weavers or assessment by the Root Covenant, depending on the material's nature. The cache currently holds several bundles of the pale, anomalously cold material that the Hunt retrieves from routes near the grave tunnel hinge — material that the bone weavers assess as substrate-connected but of external origin.
- Exits: `east` → 1272 (Antler Marker Crossing)

**1279 — The Return Gate**
`room_flags: dark no_recall` | `sector: forest`
The eastern arm of the Hunt's lateral route — the point where returning patrols re-enter the estate's inner perimeter after completing their external circuit. A formal thorn-growth arch has developed at this point, biological but architecturally deliberate, framing the transition between external patrol territory and the estate's defined interior. The arch's thorn growth is oriented inward — the same posture as the south thorn-hedge wall — suggesting the substrate considers the return passage more critical than the departure.
- Exits: `west` → 1272 (Antler Marker Crossing), `south` → 1290 (Zone IX: Thorn Knight Guardpost)

**1280 — The Night Approach Road**
`room_flags: dark no_recall` | `sector: forest`
The outermost reach of the Hunt's established patrol territory — a road that leads into the greater woodland beyond Thornwood's fixed perimeter and connects back to the gravewood's southern edge via the Night Ride Path. This is the point where the estate's jurisdiction becomes argument rather than enforcement: the Hunt claims it as territory, the Great Northern Forest wardens' route charts mark it as avoided, and the Shadowmere scouts' maps simply note "do not stop." The road here shows no substrate management — no cleared surface, no marked trees — because the substrate's network does not extend this far with sufficient resolution. The Hunt operates here on authority alone.
- Exits: `east` → 1276 (Hunt Dispatch Point), `north` → 1215 (Zone IV: Night Ride Path), `west` → 1277 (Birch-Bark Road)

---

### Zone IX: Perimeter and Auxiliary Rooms (1281–1299) — 19 Rooms

Perimeter wall walks, timber stands, cult assembly grounds, archive alcoves, the Harren family crypt, the Hunt stable court, and remaining lore-carrying auxiliary spaces. All four logics are present here at varying emphasis.

**1281 — The South Wall Walk**
`room_flags: dark no_recall` | `sector: city`
The walkway atop the estate's southern perimeter wall, accessible from the south thorn-hedge wall's exterior face. The wall-walk is wide enough for two people side by side and still carries the iron bracket infrastructure for patrol lanterns, all empty. From the wall-top, the southern burial field margins are visible to the east and the estate's formal gardens to the west. The wall-walk's stone carries the thorn-growth's inward pressure visibly: thorn branches have breached the wall's inner face and grown along the parapet, oriented toward the forecourt below.
- Exits: `east` → 1156 (South Thorn-Hedge Wall), `south` → 1290 (Thorn Knight Guardpost)

**1282 — The North Wall Walk**
`room_flags: dark no_recall` | `sector: city`
The northern perimeter wall-walk, mirroring the southern run but with a view over the approach road and the outer thorn-hedge to the north. The estate's territorial boundary is clearest from here: the managed thorn-hedge growth outside the wall marks the outermost extent of Harren administrative claims; the root-fiber threading through the wall's stone marks the substrate's claims, which extend farther. The two boundaries do not coincide.
- Exits: `west` → 1155 (North Thorn-Hedge Wall), `east` → 1283 (Outer Gate Station North)

**1283 — The Outer Gate Station (North)**
`room_flags: dark no_recall` | `sector: city`
A secondary gate in the estate's northern perimeter wall, used historically by the managed timber operations to move lumber from the managed stands without routing through the main forecourt. The gate is iron-barred and intact, though the locking mechanism is now supplemented by thorn growth that functions as both seal and indicator — the growth pattern changes when the gate is opened, and the substrate responds accordingly. A carved stone above the gate reads: *Timber Operations Gate — Estate Authorization Required.*
- Exits: `west` → 1282, `north` → 1284 (Timber Stands Managed), `east` → 1295 (Hunt Stable Court, via exterior path)

**1284 — The Timber Stands (Managed)**
`room_flags: dark no_recall` | `sector: forest`
The estate's managed thornwood timber stands — the trees that represent the Harren estate's original commercial forestry operation. The trees here are maintained: spaced for growth, their lower branches pruned to encourage trunk development, the understory cleared for access. This management is not by human hands; the substrate has continued the groundskeepers' patterns, pruning and spacing according to whatever the estate's forestry records specified. The timber is still harvestable. No one harvests it. The Violet Eclipse Cathedral's procurement agents know where these stands are and what they are worth. They do not come here personally.
- Exits: `south` → 1283, `west` → 1172 (Stable Yard), `north` → 1285 (Deep Timber Stands)

**1285 — The Deep Timber Stands**
`room_flags: dark no_recall` | `sector: forest`
The interior timber stands, where the trees are older and the "deep harvest" grade material is found — the cathedral-grade timber grown near burial sites, which the Violet Eclipse Cathedral's procurement specifications designate as more durable and more dense than surface harvest stock. The substrate's influence on these trees is visible in the bark: the deep-stand thornwood has the same darkening and bark-pattern shift visible in the estate orchard, and the root system surfaces at intervals with the crystalline texture that indicates crystal-substrate chemistry integration. The lumber from these trees is genuinely anomalous. It is also genuinely contaminated, in whatever sense a piece of wood can carry history.
- Exits: `south` → 1284, `east` → 1297 (Riven Road)

**1286 — The Cult Assembly Ground**
`room_flags: dark no_recall` | `sector: forest`
The Root Covenant's primary gathering space for organized collective rites — large enough for the entire living cult population to assemble, cleared of undergrowth by the substrate's management, and bordered by the hedge labyrinth's exterior growth so the site has a natural enclosure. The ground surface carries burn-marks from past assembly fires and the pressure-indentations of many feet in regular pattern — the congregation's standing positions during rite-work are encoded in the ground. A central platform of flat stones provides the presiding elder's position.
- Exits: `south` → 1177 (Zone II: Hedge Labyrinth Entry), `east` → 1287 (Covenant Offering Site East), `north` → 1288 (Covenant Offering Site West)

**1287 — The Covenant Offering Site (East)**
`room_flags: dark no_recall` | `sector: forest`
The eastern blood offering platform — a second root-contact rite site supplementing the gravewood's primary offering platform, positioned to serve the burial field area's cult activities. The platform here is more recently installed than the gravewood site: the stone surface is less stained, the surrounding thorn growth is less established, the substrate response is perceptibly less immediate. The cultists who work this platform are maintaining a station rather than a tradition — extending the cult's ritual operations infrastructure into territory that used to be administered by the estate's burial processing system rather than by the covenant.
- Exits: `west` → 1286 (Cult Assembly Ground), `south` → 1184 (Zone III: Rapid Burial Fields North), `north` → 1212 (Zone IV: Blood Offering Platform)

**1288 — The Covenant Offering Site (West)**
`room_flags: dark no_recall` | `sector: forest`
The western offering platform, positioned near the rose garden ruins — the station that serves the manor's western wing and the garden area's cult operations. The platform here is integrated with the rose garden's bone-arrangement installation, the two elements functioning as a composite site: bone-filing and blood offering in proximity, the substrate receiving both categories of input at the same contact point. The thorn growth around this platform has the most deliberate directionality of any surface-level thorn growth in the area — every stem is oriented toward the platform's center with a precision that suggests the substrate has been optimizing this site for longer than the cult has been using it.
- Exits: `east` → 1286, `north` → 1184 (Zone III: Rapid Burial Fields North), `south` → 1178 (Zone II: Rose Garden Ruins)

**1289 — The Bone-Path Marker Row**
`room_flags: dark no_recall` | `sector: field`
A row of bone markers running north from the deferred-name register stakes — the Root Covenant's territorial demarcation of the burial fields, distinguishing the zones under cult operational management from the zones under estate administrative management (as the dead household staff conceives it). The markers are femurs driven upright into the earth at regular intervals, each bearing a pressed circle-glyph: the cult's filing notation for a registered rite-claim on the ground. This is the cult's version of the estate's boundary-marks — same function, different material and language.
- Exits: `south` → 1183 (Zone III: Deferred-Name Registers), `north` → 1290 (Thorn Knight Guardpost)

**1290 — The Thorn Knight Guardpost**
`room_flags: dark no_recall no_mob` | `sector: city`
A formal guardpost at the estate's outer perimeter crossing-point — a stone structure fitted with the iron hardware of the Thorn Knight's patrol infrastructure: weapon-rack, lantern-mount (empty), challenge-protocol plaque. The plaque reads: *All parties must present authorization of passage. Unauthorized entry subject to estate processing.* The phrase "estate processing" has acquired a meaning that its original author did not intend. The post is staffed. The Thorn Knight here enforces the challenge-protocol as though the estate's charter were in force, as though the challenge had legal standing, as though the word "processing" meant what the administrative manuals said it meant.
- Exits: `north` → 1289 (Bone-Path Marker Row), `south` → 1281 (South Wall Walk), `east` → 1279 (Zone VIII: Return Gate)

**1291 — The Wraith Passage**
`room_flags: dark indoors no_recall` | `sector: inside`
A service corridor connecting the bone processing station to the mire's eastern margin — originally a transit route for moving material between the two zones without crossing the burial field's formal approach roads. Estate Wraiths use this passage to perform the empty household routines they inherited from their living functions: carrying nothing in the pattern of carrying something, moving with the deliberate gait of a worker completing a task, stopping at points that correspond to the original workflow's stations and pausing in the same way that a worker checking a task list would pause. They are performing a role that has no remaining content. The performance continues.
- Exits: `west` → 1186 (Zone III: Bone Processing Station), `east` → 1231 (Zone V: Substrate Water Sink)

**1292 — The Forgotten Archive Alcove**
`room_flags: dark indoors no_recall` | `sector: inside`
A storage alcove within the manor's service wing, discovered mid-process during a Root Covenant archival recovery effort and now accessible but not fully catalogued. The substrate's preservation effect has kept the documents here in better condition than their age warrants: papers from the estate's ossuary expansion period, in good legible condition, documenting the expansion's labor arrangements, the sources of that labor, and the passage records for the grave tunnel during the peak throughput years. Three specific documents in the alcove are marked with a @@ypale-gold seal@@N that matches the Violet Eclipse Cathedral's procurement authorization stamp — connecting the cathedral to the estate's operations in the period when both institutions preferred not to have the connection documented.
- Exits: `north` → 1170 (Zone II: Service Yard), `east` → 1293 (Tunnel Transfer Records Room)

**1293 — The Tunnel Transfer Records Room**
`room_flags: dark indoors no_recall` | `sector: inside`
Where the paperwork from the grave tunnel's transit operations was held — the records of every funeral convoy, every sealed warrant transfer, every private cargo shipment that passed through the estate's tunnel access point. The records are organized by year and convoy type, cross-referenced to both the surface intake records and the ossuary vault assignments. A specific section is marked in the estate clerk's notation as "restricted — by Harren family order" — a designation that, under the estate's charter, meant external auditors could not request those files. The restriction applies to a period corresponding precisely to the sealed warrant traffic documented in Midgaard's Spirebound Conclave transfer records. The Reckoning faction has been trying to access this room for twenty years. They have not been successful.
- Exits: `west` → 1292, `north` → 1164 (Zone II: Butler's Pantry, via connecting passage)

**1294 — The Harren Family Crypt (Surface)**
`room_flags: dark no_recall cold` | `sector: hills`
The above-ground burial vault of the Harren family itself — the mausoleum where the family's own dead were interred, distinct from the estate's commercial burial fields. The crypt's stonework is the estate's finest construction: dark stone with precise joinery, the thorn-and-keyhole device carved on every surface including the interior walls, and a floor inscription in the estate's administrative script that names each interred family member with full title and service dates. The Harren dead are all here — except Lord Harren, who is not interred anywhere because the substrate did not permit him to become a body to be interred. The @@acold@@N in this crypt is different from the cave system's crystal cold: it is the cold of a place where the substrate's attention is fixed and has been fixed for a long time, looking for something specific that the crypt does not contain.
- Exits: `west` → 1195 (Zone III: Memorial Lane), `south` → 1297 (Riven Road)

**1295 — The Wild Hunt Stable Court**
`room_flags: dark no_recall` | `sector: city`
The court where the substrate horses are held between patrols — a formal stable yard adjacent to the estate's original stable block, repurposed for the Hunt's substrate-constructed mounts. The horses here are still: standing in the same absolute stillness as the stable block's occupants, birch-bark pale, berry-red eyes unreflective. The court's cobblestones carry frost-marks from the horses' rest positions in the same pattern as their riding frost-prints — the substrate maintains the thermal anomaly continuously. The court connects to the Hunt's patrol route infrastructure and to the main stable block at the service yard.
- Exits: `west` → 1172 (Zone II: Stable Yard), `north` → 1283 (Outer Gate Station North), `east` → 1276 (Zone VIII: Hunt Dispatch Point)

**1296 — The Canopy Seal**
`room_flags: dark no_recall` | `sector: forest`
A space where the estate's canopy has completed its closure — the thornwood growth overhead has fused into a solid biological mass with no gaps, producing an indoor-like darkness under open sky. The substrate cleared specific openings through the canopy at positions that correspond to the binding's access points; those openings allow narrow columns of light or sky-visibility directly above ritual sites. The canopy seal is the baseline: total occlusion, complete closure, the forest's assertion that it has replaced the sky with something it controls.
- Exits: `south` → 1284 (Timber Stands Managed), `east` → 1298 (Substrate Seep Glade), `west` → 1286 (Cult Assembly Ground)

**1297 — The Riven Road**
`room_flags: dark no_recall` | `sector: forest`
A former service path that has been literally split by root eruption — a section of cobblestone road heaved upward by root pressure in a single event, leaving the path bisected by a raised root-mass running perpendicular to the road's direction. The root eruption's date is unknown but the scale suggests it was not gradual; the stone displacement patterns indicate sudden pressure release. The road is still navigable by climbing over the root-mass at the crossing point. The root-mass at the eruption site carries a substrate cold that is sharper than ambient, suggesting the eruption reached through to the crystal layer below.
- Exits: `north` → 1285 (Deep Timber Stands), `south` → 1294 (Harren Family Crypt)

**1298 — The Substrate Seep Glade**
`room_flags: dark no_recall cold` | `sector: forest`
A clearing where the substrate's surface expression is strongest above the ossuary layer — not the crystal seep of the deep gravewood, but a broader, more diffuse effect: soil temperature several degrees below ambient across the glade's entire surface, the corpse-lily growth that indicates high burial density below, and the root-mass pattern that corresponds to the resonance chamber's geometry directly below. A shaman standing in this glade would know they were standing above the ossuary's most important structural point. The glade is also the space where the substrate's bioluminescent output is visible in daylight — the cold blue of the cave system's fungal network seeping up through the soil to appear as a faint glow at ground level when conditions are right.
- Exits: `west` → 1296 (Canopy Seal), `south` → 1243 (Zone VI: Resonance Chamber — vertical connection; descending through a natural ground opening)

**1299 — The Bone-Gate Terminus**
`room_flags: dark no_recall` | `sector: forest`
The easternmost point of the estate's accessible territory — a formal gateway of bone-woven construction rather than iron, where the substrate's bone-craft has produced an arch of assembled long bones integrated with root-fiber into a structure as load-bearing as any masonry. The gate is the estate's final territorial marker at its eastern boundary, beyond which the mire's margin and the approach routes connecting to the broader grave tunnel network begin. The bone-arch is not decorated: it is structural, functional, and legible as such. *This land's terms continue until this arch. Beyond this point, you are no longer being processed. You were never not being processed.*
- Exits: `west` → 1291 (Zone IX: Wraith Passage), `north` → 1227 (Zone V: Broken Causeway)

