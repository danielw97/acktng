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

