# Area Plan: Thornwood, the Harren Estate (Full Redesign)

## Overview

- **Area Name:** `@@dThornwood, @@ythe Harren Estate@@N`
- **Area File (existing):** `area/thornwood.are`
- **Keyword:** `thornwood harren estate manor forest ossuary substrate`
- **Area Number:** `N 5` (preserved)
- **Assigned Vnum Range (must remain unchanged):** **441–590**
- **Target Level Band:** **50–80** (preserved)
- **Reset Rate:** `F 15` (preserved)
- **Owner:** `O Virant~` (normalize from current lowercase `virant`)

**Room Envelope Used:** 441–590 (150/150 rooms — fills the entire envelope)
**Mobile Envelope Used:** 441–484 (44 mobs — overlaps room vnums, which is valid per spec §13)
**Object Envelope Used:** 485–511 (27 objects — overlaps room vnums, which is valid per spec §13)

**Primary Design Goal:** Rebuild Thornwood into a coherent necrotic estate-forest where all four operating logics — manorial, funerary, forest, and substrate — are expressed simultaneously and inseparably across every room, mob, and object. The design follows the lore's core principle: **depth equals age equals danger**, with the surface manor representing manorial logic, the burial fields representing funerary logic, the older gravewood representing forest logic, and the cave system below representing substrate logic. These are not four separate zones — they interpenetrate, with each layer's vocabulary bleeding into adjacent layers.

The area replaces the current square-grid layout with a **radial-branching topology** that originates at the manor forecourt and extends outward and downward through distinct ecological territories: the manor interior, the charnel verge, the older gravewood, the peripheral mire, the ossuary undercroft, the Wild Hunt patrol routes, and finally the cave system that leads to the Thornwood Lich's sealed chamber. No zone is a dead end — each connects to at least two adjacent zones via non-looping, bidirectional exits — but not every connection is obvious or safe.

---

## Spec-First Constraints (from `area_file_spec.md`)

### Area Header
1. `Q 16` directive is mandatory.
2. Owner line must be `O Virant~` — the existing file uses lowercase `virant` and must be normalized.
3. `V 441 590` defines the vnum envelope; all authored rooms, mobiles, and objects must stay within this range.
4. Level label string must be authored as themed color-coded text, not a bare numeric range.
5. Reset message (`U`) must be authored as in-world thematic text with color-code compliance.

### Vnum Allocation
6. Vnums for rooms, mobiles, and objects must be assigned in ascending sequential order with no gaps.
7. Cross-type overlap is allowed: a room, mobile, and object may share the same numeric vnum as long as each is unique within its own index type.
8. Rooms must attempt to fill all 150 available vnum slots (441–590).
9. Mobs use 441–484; objects use 485–511. Both ranges overlap with room vnums, which is permitted.

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
27. Boss mob rooms must be flagged `no_mob` (`4`). This applies to: room 452 (Receiving Room — Lord Harren's domain), room 559 (Sealed Chamber — Thornwood Lich's domain), and room 567 (Hunt Dispatch — Wild Hunt Master's domain).
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
- The grave tunnel hinge room (441) must include an exit reference to the broader tunnel network without naming a specific Midgaard vnum in in-world text.
- Deferred-naming records and debt ledgers found in estate rooms should use language that parallels Sepulchur Pasture's civic bureaucracy but without civic accountability: same forms, no recourse.

### Shadowmere
- Shadowmere's connection through the tunnel hinge should be legible from room 441 and from the tunnel transfer records room (584): tunnel traffic includes both Midgaard funeral convoys and Shadowmere garrison supply/intelligence routing.
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

Thornwood uses a **radial-branching topology** with the Forecourt of Inheritance (449) as the central hub. Zones extend outward and downward from this hub. No zone is a dead end. The mire wraps around the eastern and southern margins, connecting back to the gravewood and burial fields to form a loop that is not a directional loop (each connection uses a different pair of rooms). The cave system descends below the ossuary and terminates at the sealed chamber.

### Zone Summary Table

| Zone | Vnums | Theme | Dominant Logic | Threat Tier |
|------|-------|-------|---------------|-------------|
| I. Harren Approach | 441–448 | Entry corridor, grave tunnel hinge, outer perimeter | Manorial + Forest | 50–55 |
| II. The Estate Surface | 449–470 | Manor house, outbuildings, chapel, gardens — frozen at Reversal | Manorial + Funerary | 55–65 |
| III. Charnel Verge and Burial Fields | 471–488 | Death throughput zone, deferred-name graves, bone processing | Funerary dominant | 60–68 |
| IV. The Older Gravewood | 489–506 | Pre-estate ridge-clan burial ground, Root Covenant sites, shaman territory | Forest dominant | 62–70 |
| V. The Mire | 507–522 | Peripheral wetland, fog shrine, substrate drainage, unstable terrain | Forest + Substrate | 62–70 |
| VI. Ossuary Undercroft | 523–540 | Bone weaver workshops, marrow construct storage, drain passages | Funerary + Substrate | 65–72 |
| VII. Cave System and Substrate Core | 541–561 | Crystal passages, handprint cave, high ledge, sealed chamber | Substrate dominant | 68–80 |
| VIII. Wild Hunt Patrol Routes | 562–571 | Night-only enforcement paths, birch roads, Hunt departure points | Forest + Substrate | 68–74 |
| IX. Perimeter and Auxiliary Rooms | 572–590 | Wall walks, timber stands, cult assembly grounds, archive alcoves, family crypt | All four logics | 55–68 |

### Connectivity Rules

- **Main progression spine:** Entry (Zone I) → Estate Surface hub (Zone II) → branches simultaneously to Charnel Fields (III), Gravewood (IV), and Ossuary descent (VI). The mire (V) is accessible from both the burial fields (III) and the gravewood (IV). The cave (VII) is accessible only from deep within the ossuary (VI). The Wild Hunt routes (VIII) are accessible from the burial fields (III) and gravewood (IV).
- **No directional loops.** The mire connects to both zones III and IV, but the connections use different room pairs, so repeated movement in one direction does not cycle back.
- **Depth gates.** The wine cellar (460) in the estate surface connects down to the ossuary undercroft (523). The cave system entry (540) is accessible only from the deepest ossuary room (539). The sealed chamber (559) is accessible only through the threshold of the binding (558), which requires the Harren Estate Register Key (object 511).
- **Lateral shortcuts.** The tunnel hinge (441) provides an external south exit for players to leave the area. The grave tunnel surface exit (488) in the burial fields connects back up to room 441, providing an alternate egress from the burial field zone without backtracking through the manor.
- **Night-only access.** Wild Hunt patrol rooms (562–571) are not locked during the day but the Wild Hunt Master and Riders only spawn at night. The approach road phantom (mob 445) patrols Zone I by night only.
- **The mire's unstable terrain.** Mire rooms carry `cold` and `dark` room flags to signal hazard. No maze flag is used — the instability is expressed through mob pressure (mire lurkers, deep mire shades) and room descriptions, not disorienting exits.

### Topology Diagram (Schematic)

```
[Outside / Grave Tunnel Network]
          |
     down | up
          |
    441 Grave Tunnel Hinge
          | north
    442 Outer Cobblestone Road
          | north
    443 Iron Gate of Thornwood ←→ 444 Gatewarden's Lodge
          | north
    445 Forecourt Approach ←→ 446 North Thorn-Hedge Wall
          |                  ←→ 447 South Thorn-Hedge Wall
          | north
    448 Lantern Alley
          | north
    449 Forecourt of Inheritance (CENTRAL HUB)
   /      |      \       \
west    north    east    down (via 460 Wine Cellar)
  |       |       |            |
461   450     462      523 Undercroft Stairs
Service Thorn    Charnel        → ZONE VI (Ossuary)
 Yard  Portico  Verge Gate       → ZONE VII (Cave, via 540)
  |       |       |
462   451     472
  ...  Great   Charnel Verge
       Hall      → ZONE III (Burial Fields)
         |         → ZONE VIII (Wild Hunt, via 562)
    [ZONE II interior]
    (manor rooms 451-459,
     chapel 457-458,
     study 453, gallery 456,
     kitchen 459, wine cellar 460,
     outbuildings 461-470)
         |
    449 hub also connects:
         | northwest
    489 Gravewood Threshold
         → ZONE IV (Older Gravewood)
           → ZONE V (Mire, via 498 and 507)

ZONE III (Burial Fields, 471-488)
  connects east → ZONE V (Mire, via 507)
  connects south → 488 (Grave Tunnel Surface Exit → 441)
  connects north → ZONE VIII (Wild Hunt, via 562)

ZONE IV (Older Gravewood, 489-506)
  connects south → ZONE V (Mire, via 498)
  connects east → ZONE III (Burial Fields, via 505)
  connects south-deep → ZONE VIII (Wild Hunt, via 506)

ZONE V (Mire, 507-522)
  connects north → ZONE IV (Gravewood, via 520)
  connects west → ZONE III (Burial Fields, via 507)
  connects northwest → ZONE IX aux (via 522 substrate sink)

ZONE VI (Ossuary Undercroft, 523-540)
  connects up → ZONE II (Wine Cellar 460)
  connects east → ZONE III (Hillside Ossuary Entry 530, from 478)
  connects deep → ZONE VII (Cave Entry 541, from 540)

ZONE VII (Cave System, 541-561)
  connects up → ZONE VI (from 541 to 540)
  connects far-up → ZONE IV (Gravewood, via 561 deep cave egress → 504)
  559 SEALED CHAMBER (boss, no_mob, key-locked door from 558)

ZONE VIII (Wild Hunt, 562-571)
  connects south → ZONE III (Burial Fields, via 562)
  connects west → ZONE IV (Gravewood, via 571)

ZONE IX (Perimeter/Auxiliary, 572-590)
  connects to ZONE II (manor) and ZONE III (burial fields)
  at wall walks (572-573), timber stands (575-576),
  cult assembly ground (577), archive rooms (583-584),
  family crypt (585), Hunt stable (586)
```

### External World Links

- **South exit from 441 (Grave Tunnel Hinge):** Connects into the broader grave tunnel network, giving access to the graveyard area or Midgaard's burial approaches. The reverse link (graveyard area → 441) must be confirmed and preserved.
- **Up exit from 441:** Emerges at 442 (Outer Cobblestone Road) — the surface world above the tunnel. This bidirectional vertical pair is the area's canonical entry point.
- The grave tunnel hinge exit direction and destination vnum must be coordinated with the `graveyard.are` (vnum range 11000–11099) file. The specific room in `graveyard.are` that links to 441 must carry a reciprocal exit.

---

## Full Room-by-Room Plan (All 150 Rooms, 441–590)

Room flags and sector types are noted. All descriptions end with exactly one trailing newline before `~`. Important rooms (marked **★**) require at least 5 sentences. Standard rooms require at least 3. All exits are bidirectional unless noted.

---

### Zone I: Harren Approach (441–448) — 8 Rooms

Entry corridor from the grave tunnel network to the estate's iron gate. Manorial and forest logics first appear here, where the tunnel's neutral infrastructure meets the estate's territorial claim.

**441 — The Grave Tunnel Hinge** ★
`room_flags: dark indoors no_recall` | `sector: inside`
Where the old pre-civic burial tunnel meets the surface approach to the Thornwood estate. This is the transit node shared with Shadowmere's southern march supply routes and Midgaard's funeral convoy traffic — neutral infrastructure neither power controls. The tunnel ceiling is low and sweating limestone, braced with timber so old it has calcified. Carved way-markers from at least three distinct traditions overlap on the eastern wall: a civic registry glyph from Midgaard's road-standards era, a directional slash-mark used by Shadowmere's tunnel scouts, and something older and non-linguistic — a pressed handprint in ochre that may predate the tunnel's formal construction. The smell is mineral and cold, with an undertone of old bone and something faintly sweet that does not belong underground.
- Exits: `south` → graveyard.are tunnel node (external link), `up` → 442

**442 — The Outer Cobblestone Road**
`room_flags: dark no_recall` | `sector: forest`
The surface world above the tunnel hinge, where the estate's cobblestone road begins. The paving is ancient and frost-heaved, pushed up in long ridges by root masses that have been growing beneath it for generations. The stones are dark with moisture even in dry weather — the substrate draws groundwater upward through the limestone below. To the north, the road curves toward a silhouette of iron and thorn against a canopy of leafless oaks. Crows occupy every available branch, unmoving.
- Exits: `down` → 441, `north` → 443

**443 — The Iron Gate of Thornwood** ★
`room_flags: dark no_recall no_portal` | `sector: forest`
The estate's defining threshold: a pair of massive wrought-iron gates set into a wall of dark stone, their original ornamental thorn motifs long since fused with genuine briar growth so that the heraldic and the biological have become indistinguishable. The Harren crest — a thorned branch over a keyhole — is still visible on the central boss of the gate, though the @@y crown-gold finish@@N has oxidized to a greenish-black. The bars are wrapped in root-mass at the joints and base, anchoring the gate in ways that make it unclear whether the ironwork or the roots are now the structural element. Beyond the gate, the cobblestone continues into a forecourt dimly visible through the thorn-growth. The gateposts carry rune-carvings too eroded to read in full, though the shapes carry a heaviness that suggests the eye was not meant to rest on them long. A chain and bracket system suggests the gates were once mechanically operated; the mechanism is now biological, responsive not to levers but to the estate's own territorial logic.
- Exits: `south` → 442, `north` → 445, `east` → 444
- Named exit: `^iron gate` (north, door; `EX_ISDOOR` set; no lock — the gate stands open but is a real door that can be opened/closed)

**444 — The Gatewarden's Lodge**
`room_flags: dark indoors no_recall` | `sector: inside`
A small stone building pressed against the inner face of the eastern gatepost, where the gate-keeper once logged arrivals, checked credentials, and collected the estate's passage tax. The logbook desk still stands, its surface thick with root-fiber that has threaded up through the floorboards and integrated itself into the wood grain. The most recent legible entry is decades old. A hook on the wall still holds a brass bell — the notification bell used when important visitors required Lord Harren's attention — though the bell now resonates faintly with the substrate's harmonic output rather than with a clapper stroke. The lodge is occupied.
- Exits: `west` → 443

**445 — The Forecourt Approach**
`room_flags: dark no_recall` | `sector: forest`
Inside the gate, the cobblestone broadens into a formal approach lined with iron shepherd's-hook lantern posts, all dark, their fuel long oxidized to amber resin inside their housings. The lanterns mark jurisdiction — they were the estate's administrative markers — but have not illuminated anything in living memory. Thorn hedges rise on either side of the path, their growth managed (directed would be more accurate) so the approach corridor feels deliberate. Something about the hedges' angle makes it clear they are oriented toward the path rather than growing away from it. The manor's peaked roof is visible ahead.
- Exits: `south` → 443, `north` → 448, `east` → 446, `west` → 447

**446 — The North Thorn-Hedge Wall**
`room_flags: dark no_recall` | `sector: forest`
The northern run of the estate's interior perimeter hedge, where thorn growth has consumed the original stone wall so completely that the wall's outline is only visible as a slight straightness in what otherwise looks like natural woodland. The thorns here grow in layers — outer growth dark-stemmed and aggressive, inner growth pale and fused into structures that resemble architectural arches. The substrate manages this wall. It is not decorative.
- Exits: `west` → 445, `east` → 573 (Zone IX: North Wall Walk)

**447 — The South Thorn-Hedge Wall**
`room_flags: dark no_recall` | `sector: forest`
The southern run of the interior perimeter, mirroring the northern hedge wall but oriented differently — here the thorns grow inward, toward the manor grounds, rather than outward. The difference is not aesthetic. It suggests the substrate is pulling material from the south. Carved stones from the original wall are visible at the hedge's base, their surfaces still bearing Harren boundary-marks worn smooth by root pressure.
- Exits: `east` → 445, `west` → 572 (Zone IX: South Wall Walk)

**448 — The Lantern Alley**
`room_flags: dark no_recall` | `sector: forest`
A formal path between the forecourt approach and the estate's central courtyard, originally lit by lanterns on both sides for the reception of dignified visitors. Fourteen lantern posts remain, seven to each side, their iron scroll-work intact but rusted to a dull orange. The path between them is perfect — not heaved, not overgrown, each cobblestone seated cleanly. This is suspicious. The substrate maintains this path actively, and the smooth surface underfoot is the smoothness of a tongue, not of careful masonry. The manor's entrance portico is visible ahead, its dark stone columns wrapped in early thorn growth.
- Exits: `south` → 445, `north` → 449

---

### Zone II: The Estate Surface (449–470) — 22 Rooms

The manor house and its outbuildings, frozen at the moment of the Reversal. Formal spaces preserve the instant of transformation as permanent décor. The manorial and funerary logics are densest here; the forest logic intrudes through every gap.

**449 — The Forecourt of Inheritance** ★ (CENTRAL HUB)
`room_flags: dark no_recall` | `sector: city`
The broad cobblestone forecourt before the manor's entrance, where the estate's administrative life was once conducted openly — arrivals logged, debts assessed, burial contracts signed on portable writing desks that servants carried out for the purpose. It is large enough that the surrounding thorn hedges do not immediately close in; there is real sky visible above, though the canopy has begun to arch over the western edge. The @@yHarren crest@@N is inlaid in the forecourt's central stones — a thorned branch over a keyhole in pale limestone — but root growth has retraced the design in biological material, so the crest appears twice: once in stone, once in living briar pressing up through the mortar. The estate's main entrance is north. Paths lead east to the charnel verge, west to the service wing and older gravewood approaches, southeast to the chapel, and down through the manor's cellar connections to the ossuary. The air here is colder than the approach, with a faint mineral undertone.
- Exits: `south` → 448, `north` → 450 (Thorn Portico), `east` → 471 (Charnel Verge Gate), `west` → 461 (Service Yard), `northwest` → 489 (Gravewood Threshold), `southeast` → 457 (Chapel Antechamber)

**450 — The Manor's Thorn Portico**
`room_flags: dark indoors no_recall` | `sector: inside`
The formal entrance to the manor house proper: a covered portico of dark stone columns between which thorn growth has been trained into arch-shapes so precise they appear architectural. The original wooden doors behind the columns have been partly consumed by the same growth — their hinges still function but the wood grain has fused with root-fiber, so opening them produces a sound like tearing rather than creaking. A coat-of-arms plaque above the lintel shows the Harren thorn-and-keyhole device, the @@ygold finish@@N still bright where protected by the overhang.
- Exits: `south` → 449, `north` → 451 (Great Hall)

**451 — The Great Hall** ★
`room_flags: dark indoors no_recall` | `sector: inside`
The manor's primary reception space, preserved at the instant of the Reversal with the completeness of an insect in amber. A long table runs the hall's length, set for a dinner that was never finished: plates still hold desiccated food reduced to mineral shapes, goblets stand upright, and the centerpiece — a floral arrangement in a silver bowl — has been replaced by thorn growth that rises from the bowl's interior as though the thorns were always the intended arrangement. Portraits line the walls, each depicting a generation of the Harren family; the eyes have been removed from every portrait, neatly, as though the substrate decided that observation flowed in one direction only. Chandeliers of iron and bone-horn hang from the ceiling, long unlit, threaded through with root-fiber that has used the chandelier chains as a climbing scaffold. @@yHarren heraldic banners@@N along the upper walls bear the thorn-and-keyhole device, their fabric intact and unfaded, though a careful look reveals they are now partly woven from biological material rather than cloth. The hall is cold. The cold is not from the temperature but from whatever the crystal substrate radiates upward through the floor.
- Exits: `south` → 450, `north` → 452 (Receiving Room), `east` → 454 (Map Room), `west` → 453 (Lord's Study), `up` → 456 (Manor Gallery — second floor landing)

**452 — The Receiving Room** ★ (Boss room: Lord Harren — `no_mob` flag)
`room_flags: dark indoors no_recall no_mob` | `sector: inside`
Lord Harren still receives visitors here. The room is unchanged from the Harren Charter era in every physical detail — the audience chair behind the writing desk, the two visitor chairs facing it, the shelved reference volumes along the east wall (estate law, boundary records, tariff tables), the single window overlooking the forecourt, now opaque with thorn growth. What is wrong is the occupant. Lord Harren, the Estate Revenant, sits at the desk as he always did: upright, attentive, formally dressed in the estate's administrative livery. His face is recognizably human and fully alive in expression, but the skin carries a faint bark-pattern, his hands rest flat on the desk with a stillness that no living person maintains without effort, and the eyes — still his eyes, still conscious and aware — move with the deliberate economy of someone who has learned that spontaneous motion costs something. He will negotiate. He remembers being alive. He is the most dangerous thing on the estate's surface.
- Exits: `south` → 451, `east` → 455 (Butler's Pantry)

**453 — The Lord's Study**
`room_flags: dark indoors no_recall` | `sector: inside`
Harren's private working space, smaller and more disordered than the receiving room. The desk here carries the weight of real administrative work — not the performance of authority but its machinery. Papers are stacked in the substrate-preserved state they occupied at the moment of the Reversal: mid-sentence correspondence, half-completed account columns, a journal whose final entries grow increasingly erratic before stopping. The estate's operational records — budget summaries, staff rosters, interment manifests — are mixed with personal notes in a hand that becomes harder to read as the journal progresses. Root-fiber has threaded through the paper stacks but not displaced them; the substrate appears to be preserving the archive, not consuming it.
- Exits: `east` → 451, `south` → 455 (Butler's Pantry, via connecting passage)

**454 — The Map Room**
`room_flags: dark indoors no_recall` | `sector: inside`
The estate's cartographic archive, where Harren administrators managed the physical territory of their charter. Large vellum maps cover the long table and hang from the walls: the estate's original land grant with surveyed boundaries, the grave tunnel route with tributary nodes marked, the interment field plots numbered by intake period, and a partial survey of the old gravewood that stops abruptly at the point where the ridge-clan burial sites begin. Root growth has traced along the map edges as though something finds the boundaries interesting. A @@pdim inscription@@N has been added to one map in a hand that is not the Harrens' — a script that the cult uses for ritual filings, pressed into the vellum as if by something other than a quill.
- Exits: `west` → 451

**455 — The Butler's Pantry**
`room_flags: dark indoors no_recall` | `sector: inside`
The administrative hub of the manor's domestic operations: the room from which the household was managed day to day. Ledgers, keys, linen inventories, wage records, and staff duty rosters fill the shelves. The butler's own records are here — a meticulous log of every visitor received, every funeral convoy processed, every payment recorded for passage tax and burial lease. The handwriting maintains its precision until the final pages, where a single repeated annotation begins appearing on otherwise normal entries: a small pressed circle with radial lines, increasing in frequency, until the last entries show nothing else. The pantry smells of congealed beeswax and something older beneath it.
- Exits: `north` → 452, `west` → 453

**456 — The Manor Gallery**
`room_flags: dark indoors no_recall` | `sector: inside`
The manor's second-floor portrait gallery, a long corridor of framed canvases depicting the Harren lineage across six generations. Every portrait has had its eyes removed with precision — not scratched out but cleanly excised, leaving oval absences that look less like damage and more like a consistent modification applied by someone with a coherent purpose. The @@yHarren thorn-and-keyhole device@@N appears in every frame's background: worked into furniture, embroidered on clothing, carved into the landscape behind the subjects. The frames themselves are intact and ornate. The gallery is the most formally maintained space in the manor — the root-fiber that has colonized everything else has left the portrait frames untouched.
- Exits: `down` → 451, `east` → 457 (via connecting stairway)

**457 — The Chapel of the Harren Estate** ★
`room_flags: dark indoors no_recall` | `sector: inside`
The estate's institutional chapel, built under Violet Compact-adjacent auspices to serve the manor household and provide religious certification for the burial rites that formed the estate's primary revenue. The original fittings — altar, pew ranks, devotional niches — survive intact, but the symbols have been overwritten. The Compact-adjacent iconography has been systematically replaced with Root Covenant imagery: thorn patterns cut into the altar stone, bone arrangements in the devotional niches, and a central floor inscription where the compact seal once was, now bearing the cult's contract-liturgy in @@pdeep purple chisel-marks@@N. Candles burn here — real candles, maintained by living hands. The Root Covenant uses this chapel for its organized rites, and the smell of fresh blood and tannin beneath the old stone-and-incense is not from the past.
- Exits: `northwest` → 458 (Chapel Antechamber), `east` → 456 (via stairway connection)

**458 — The Chapel Antechamber**
`room_flags: dark indoors no_recall` | `sector: inside`
The waiting room where the chapel's lay community would gather before services, now serving the same function for the Root Covenant's ritual gatherings. Stone benches line the walls. A board on the north wall once held the week's service schedule in chalk; it now holds a chalk list of "pending filings" — a cultist's phrase for blood offerings owed to the substrate but not yet paid. The list is long.
- Exits: `southeast` → 457, `north` → 449 (Forecourt of Inheritance, via exterior path)

**459 — The Kitchen and Scullery**
`room_flags: dark indoors no_recall` | `sector: inside`
The manor's kitchen, frozen mid-preparation. Pots hang from iron hooks, their contents long reduced to dry mineral compounds. A cutting board still holds a knife. The fire grate is black with old ash. The scullery beyond the kitchen arch is where the household's washing was done — the basin is still half-full of water, though the water is now the color of tannin and has a faint crystal clarity that does not belong to standing water decades old. Something about the kitchen's preservation feels more intentional than the rest: the substrate keeps it cold and dry, which is precisely what you would do if you wanted to use it again.
- Exits: `east` → 449 (Forecourt, via exterior kitchen door), `north` → 451 (Great Hall, via service passage)

**460 — The Wine Cellar** ★
`room_flags: dark indoors no_recall cold` | `sector: inside`
The estate's wine cellar, reached by descending from the kitchen through a flagstone stair. This room is the coldest place in the manor — colder than the cave system below, colder than the mire, cold in the way that glass is cold when it has been storing something much older than the temperature around it. The wine racks are intact, the bottles sealed in wax that has crystallized but not cracked. On the cellar floor, occupying most of the open space between the racks, is the secondary binding inscription — a complex inscribed circle identical in geometry to the one in the sealed chamber far below, rendered in a script that is not carved but grown: the root-fiber threading between floor stones has arranged itself into the sigil's pattern. This was not done by human hands. The @@pbinding geometry@@N glows faintly in the @@acold blue@@N of substrate-connected fungal output from the walls. A iron-banded hatch in the floor descends further, into the ossuary undercroft.
- Exits: `up` → 459 (Kitchen), `down` → 523 (Undercroft Stairs)

**461 — The Service Yard**
`room_flags: dark no_recall` | `sector: city`
The open yard between the manor's domestic wing and its outbuildings, where the estate's working operations were coordinated. Loading areas, storage bays, and the connections to the stable block, distillation outbuilding, and groundskeeper's store all open onto this space. The ground is cobbled in a rougher grade than the formal forecourt — working stone, not display stone. Substrate growth here is less organized than in the manor: the service yard is where the estate's biological layer shows its seams, with root eruptions through paving, thorn growth in irregular directions, and the smell of damp iron and sweet rot stronger than elsewhere.
- Exits: `east` → 449 (Forecourt of Inheritance), `north` → 462 (Stable Block), `west` → 464 (Groundskeeper's Store), `south` → 466 (Distillation Outbuilding)

**462 — The Stable Block**
`room_flags: dark indoors no_recall` | `sector: inside`
The manor's stabling facility, built for the estate's working horses and the mounts of visiting funeral convoy escorts. The original horses are long gone. What occupies the stalls now are substrate constructions that approximate equine form: birch-bark pale, berry-red eyes, weight and movement consistent with horses but without breath or sound. They stand in the stalls with the stillness of furniture. The tack room at the stable's western end still holds the estate's original saddle-work and harness, some of it maintained in better condition than it has any right to be.
- Exits: `south` → 461, `east` → 463 (Stable Yard)

**463 — The Stable Yard**
`room_flags: dark no_recall` | `sector: city`
The yard behind the stable block, where horses were exercised and where the estate's working carts were parked. Frost-prints mark the cobblestones — the substrate horses leave these rather than hoofprints. The cart park holds two intact wagons whose wood has been fully colonized by root-fiber, making them immovable. The yard connects north to the timber stands along the estate's managed perimeter.
- Exits: `west` → 462, `north` → 575 (Zone IX: Timber Stands), `east` → 586 (Zone IX: Hunt Stable Court)

**464 — The Groundskeeper's Store**
`room_flags: dark indoors no_recall` | `sector: inside`
The tool store for the estate's groundskeeping operations — the staff responsible for maintaining the formal gardens, the thorn hedges, and the managed timber stands. Tools hang on the walls in the order their users last placed them: pruning hooks, root-cutting saws, long-handled shears for hedge work. The saws are interesting: the blade teeth are worn in patterns that suggest they were used on material harder than wood. The estate ledger kept here tracks tool condition and assignment — the last entries show items signed out but not returned.
- Exits: `east` → 461, `south` → 465 (Carpenter's Workshop)

**465 — The Carpenter's Workshop**
`room_flags: dark indoors no_recall` | `sector: inside`
Where the estate's thornwood timber was processed into finished goods for internal use and for external sale. The workbenches carry the marks of sustained craft use: saw-cut channels, chisel dents, wax-stain rings. Thornwood shavings still cover the floor, their unusual density and preservative compounds keeping them from degrading in the normal way — the shavings look fresh. A price-list on the wall details the timber grades: surface harvest, managed-stand harvest, deep-interior harvest, the last priced significantly higher with a notation that reads "Cathedral-grade; grade-sort required; consult ledger before committing."
- Exits: `north` → 464, `east` → 466 (Distillation Outbuilding)

**466 — The Distillation Outbuilding**
`room_flags: dark indoors no_recall` | `sector: inside`
The estate's alchemical processing facility, where botanical and bone-derived compounds were extracted for sale and for the estate's own burial-preparation operations. The still-apparatus is intact: copper coils, sealed collection vessels, a furnace grate. The output channels from this building connect southward through the estate's drainage system to the mire. The waste products of decades of alchemical processing — rendered bone oils, tannin concentrates, botanical extracts from thorn-growth — have been depositing in the mire's water table for generations, contributing to the wetland's toxic character. The room smells of @@asulfur and sweet rot@@N.
- Exits: `north` → 461 (Service Yard), `west` → 465, `south` → 521 (Zone V: Alchemical Runoff Channel)

**467 — The Garden Terrace**
`room_flags: dark no_recall` | `sector: forest`
The estate's formal garden terrace, originally a tiered arrangement of ornamental plantings behind the manor's west wing. The terracing is still visible as stone retaining walls, but the planting beds have been wholly consumed by thorn growth that has replaced every intended species with its own logic. The remaining ornamental structures — a sundial, a stone bench, a low fountain basin — are still present but incorporated: the thorn has grown through them rather than around them, making them part of the hedge geometry rather than decorative counterpoints to it.
- Exits: `east` → 449 (Forecourt), `north` → 468 (Hedge Labyrinth Entry), `south` → 469 (Rose Garden Ruins)

**468 — The Hedge Labyrinth Entry**
`room_flags: dark no_recall` | `sector: forest`
The entrance to what was once the estate's formal hedge labyrinth — a fashionable garden feature of the Harren period, meant to demonstrate the family's resources and aesthetic sophistication. The original hedges were yew, topiary-shaped to precise geometric patterns. They are now thornwood, and they are no longer geometric — or rather, they have a geometry, but it is not the geometry of the original design. The new pattern corresponds to the binding's inscription geometry, traceable if you can read the binding's script. The labyrinth is navigable. It is not safe.
- Exits: `south` → 467, `north` → 577 (Zone IX: Cult Assembly Ground, via labyrinth pass-through), `east` → 578 (Zone IX: Covenant Offering Site East)

**469 — The Rose Garden Ruins**
`room_flags: dark no_recall` | `sector: forest`
What the estate's records call the Rose Garden was the manor's most formal exterior space — the area reserved for family ceremonies, the signing of significant contracts, and the reception of distinguished guests who warranted outdoor hospitality. The rose beds are gone. The garden's structure — a circular central clearing with four radiating paths — survives, but the paths are now thorn-lined and the central clearing contains a bone arrangement: femurs and long bones set upright in the earth at regular intervals, too carefully placed to be accidental. This is a Root Covenant installation. The cultists have converted the estate's most ceremonial exterior space into a bone-filing rite site.
- Exits: `north` → 467, `west` → 579 (Zone IX: Covenant Offering Site West)

**470 — The Estate Orchard**
`room_flags: dark no_recall` | `sector: forest`
A former apple and plum orchard at the estate's western edge, now twisted beyond fruiting capacity. The trees still stand in rows — the original planting grid is visible — but the substrate has modified them: the bark has darkened to near-black, the branch angles have shifted toward the manor rather than toward light, and the root systems have surfaced and interlocked with each other in a net that makes walking between the trees slow and careful. The ground between the root-net sections holds small cairns of smooth stones, each one placed by a different hand over a long span of time — the pre-estate ridge-clan practice of marking a path's edge for travelers who might lose their footing.
- Exits: `east` → 467 (Garden Terrace), `north` → 489 (Gravewood Threshold)

---

### Zone III: Charnel Verge and Burial Fields (471–488) — 18 Rooms

The operational heart of the estate's death economy. Extends beyond the manor's formal perimeter into the old gravewood's margins. Funerary logic is dominant here; forest logic bleeds in from the east where the burial expansion met the older ridge-clan sites.

**471 — The Charnel Verge Gate** ★
`room_flags: dark no_recall` | `sector: city`
The formal threshold between the manor's domestic estate and the burial operation beyond. A stone arch carries a lintel inscription in Midgaard-standard administrative script: *Interments Processed on Receipt of Authorized Manifest. All Convoy Cargo Subject to Estate Audit.* The arch's stonework is intact, but the passage beneath it smells of old blood and wet earth in a combination that no amount of time has diminished. Thorn growth has traced the arch's curve from the inside, following the stone precisely, as though the substrate approved of the demarcation and wanted to reinforce it biologically. Beyond the arch, the cobblestone ends and packed earth begins — the working surface of the burial fields, not a formal approach. Iron posts to either side carry rusted hooks where the original processing lamps were hung; the lamps are gone, but the hooks remain, and something about their alignment feels like it still means something.
- Exits: `west` → 449 (Forecourt of Inheritance), `east` → 472 (Charnel Verge), `north` → 485 (Weight-Stone Platform)

**472 — The Charnel Verge**
`room_flags: dark no_recall` | `sector: forest`
The verge itself — the liminal band of ground between the estate's domestic space and the burial fields, where the transition happens. The earth here has a different texture: darker, more compacted, with a faint residual warmth that is not solar. This is the ground that received the most traffic during the estate's operational peak: body-bearing carts, grieving families, bone-processing laborers. The substrate's root-layer is dense just below the surface, and the ground occasionally produces a low sound — not a creak but a settling, as of weight being redistributed.
- Exits: `west` → 471, `east` → 473 (Intake Processing Yard), `south` → 480 (Corpse-Lily Terraces)

**473 — The Intake Processing Yard**
`room_flags: dark no_recall` | `sector: city`
Where funeral convoys were received, assessed, and sorted. A long stone platform runs the yard's length — the weigh-bench, where coffins were lifted onto the scales and their stated contents verified against the manifest. The scales are still mounted: iron balance arms, stone counterweights, a calibration marker in the stone. The platform's surface carries the stain-record of generations of use: water-marks from washing, rust from iron fittings, darker compounds that do not have a clean name. A row of iron stakes along the south wall held the processing lamps; the stakes remain, bent at uniform angles as though pressed by something coming from the direction of the burial fields.
- Exits: `west` → 472, `north` → 474 (Deferred-Name Registers), `south` → 475 (Rapid Burial Fields North), `east` → 477 (Bone Processing Station, via working path)

**474 — The Deferred-Name Registers**
`room_flags: dark no_recall` | `sector: city`
A row of provisional grave markers stretching north along the burial field's intake edge — each a numbered iron stake with a small attached plate giving a case number, an intake date, and a blank field where the name would eventually be filled in when the family paid the naming rite fee. Most plates remain blank. Some are numbered in sequences so dense that the case numbers overlap and have to be read in a specific order to make sense. A ledger-stand at the row's southern end holds the register itself: a thick book of oilskin-wrapped pages, its entries showing that only a fraction of the cases were ever closed. The entries that are closed bear a single stamp: *Named. Filed. Complete.* The entries that were never closed bear a different stamp, added later in a hand that is not the original clerk's: *Pending. Interest accruing.*
- Exits: `south` → 473, `north` → 580 (Zone IX: Bone-Path Marker Row)

**475 — The Rapid Burial Fields (North)**
`room_flags: dark no_recall` | `sector: field`
The northern section of the estate's commercial interment fields — the rapid-burial ground where bodies were processed at volume during the crisis years when Midgaard's own burial infrastructure could not absorb the throughput. The graves here have no markers; the estate's records link each plot to a case number in the deferred-name register. The plots themselves are visible as slight depressions in the earth, too regular to be natural subsidence. Root-mass threads through everything at knee height, integrating the burial density into the substrate's network. The ground sometimes moves.
- Exits: `north` → 473, `south` → 476 (Rapid Burial Fields South), `east` → 481 (Thorn-Ringed Plots), `west` → 579 (Zone IX: Covenant Offering Site West)

**476 — The Rapid Burial Fields (South)**
`room_flags: dark no_recall` | `sector: field`
The southern extension of the rapid-burial ground, where the expansion into the older gravewood's margins began. The grave depressions here are less regular — the substrate's original ridge-clan burial network was already present when the Harrens expanded southward, and the commercial interments were overlaid onto existing deep-burial sites without the original community's knowledge. This mixing is why the substrate in this section behaves differently from the northern fields: root behavior is more complex, soil temperature is anomalous, and the sound of root-creak is louder here than anywhere else on the surface.
- Exits: `north` → 475, `south` → 488 (Grave Tunnel Surface Exit), `east` → 482 (Sealed Burial Row)

**477 — The Bone Processing Station**
`room_flags: dark no_recall indoors` | `sector: inside`
A partially open structure — a stone-roofed shelter without full walls — where exhumed remains were cleaned, sorted, and prepared for ossuary storage. The processing tables are still set up: marble slabs with channels for fluid drainage, racks for drying, bins labeled by bone type in the estate's cataloguing system. The cataloguing system is thorough and unsentimental: femur, tibia, skull, rib-section, miscellaneous small, all given codes that reference the ossuary vault destination. The bins contain residue — dust, mineral compounds, the dried remains of the processing work — but the structure is otherwise clean. Whatever processed things here was meticulous.
- Exits: `west` → 473, `south` → 478 (Ossuary Hillside Cut), `east` → 582 (Zone IX: Wraith Passage)

**478 — The Ossuary Hillside Cut**
`room_flags: dark no_recall` | `sector: hills`
A cut through the low hillside east of the burial fields, leading down and inward to the hillside ossuary vault entrance. The cutting is stone-faced on both sides, the work of skilled masons — the Harrens built this seriously. Iron gate-frames set into the hillside cut hold the remains of gates that are now more thorn than iron, the original bars having been colonized so completely that the biological growth now provides structural support. The ground slopes downward at a consistent grade, and the smell of bone dust and cold mineral begins here, increasing with depth.
- Exits: `north` → 477, `down` → 530 (Zone VI: Hillside Ossuary Entry)

**479 — The Memorial Byre**
`room_flags: dark indoors no_recall` | `sector: inside`
A stone building at the burial field's western edge, where families who had paid the full naming and memorial rite fee could receive cremated remains in a sealed container and participate in a private ceremony. The byre's interior is designed for grief display: padded benches, a stone ledge for the remains container, alcoves with devotional symbols. The devotional symbols have been replaced with Root Covenant bone arrangements. The bench cushions are still there, though the fabric has decayed to threads. The ledge is empty. No families have come for some time.
- Exits: `east` → 472 (Charnel Verge), `north` → 484 (Charnel Chapel Ruin)

**480 — The Corpse-Lily Terraces**
`room_flags: dark no_recall` | `sector: field`
The terraced section of the burial fields named for the pale, fleshy flower that grows exclusively on substrate-saturated grave soil. Corpse-lilies are not natural — they are an emergent biological output of the substrate's integration with high-density remains, producing a flower with anomalous properties: cold to the touch, faintly luminescent at the petal edges, and capable of surviving conditions that kill ordinary plants. They bloom upside-down, faces toward the soil. The terracing was originally for drainage management; it now creates a series of thorn-rimmed levels descending east, each populated with corpse-lily growth at different stages.
- Exits: `north` → 472, `east` → 481 (Thorn-Ringed Plots), `south` → 487 (Annex Burial Ground)

**481 — The Thorn-Ringed Plots**
`room_flags: dark no_recall` | `sector: forest`
The oldest section of the original estate burial ground, where the pre-estate ridge-clan containment tradition survived into the commercial era and was adopted, misunderstood, and ultimately validated by the substrate. These plots have genuine containment thorn rings — grown tight over decades around specific graves, their organic material fused into structural barriers. The rings are not decorative. Exhumation attempts on these plots during the estate's ossuary expansion were abandoned when the thorn growth physically resisted digging equipment. The substrate holds these plots sealed. Whatever is inside them, the binding considers it necessary to keep there.
- Exits: `west` → 475, `south` → 482 (Sealed Burial Row), `east` → 505 (Zone IV: Gravewood Fringe East)

**482 — The Sealed Burial Row**
`room_flags: dark no_recall cold` | `sector: forest`
A straight row of graves running north-to-south where exhumation was impossible — not because of thorn-ring containment but because the substrate itself refused to yield the remains. The estate's processing logs document repeated attempts: tools broke, laborers reported pressure beneath the soil pushing back, and in one documented incident a laborer who persisted was found the following morning asleep on the surface with no memory of the night before. The row has been left alone since. The grave markers here — unlike the deferred-name stakes — are carved stone, some bearing inscriptions that predate the Harren estate entirely. The cold here is notably sharper than in surrounding areas, with a faint @@acrystal-frequency hum@@N just below audible range.
- Exits: `north` → 481, `south` → 476

**483 — The Charnel Chapel Ruin**
`room_flags: dark no_recall` | `sector: inside`
A secondary chapel that served the burial fields directly — separate from the manor's institutional chapel, this was where the grave-workers and corpse-cart laborers could attend services without entering the manor proper. The roof has partially collapsed, and the walls show root-penetration at every joint. The altar stone is intact. On it, in a script that blends Compact liturgy with Root Covenant contract-language, is an inscription that reads as both a prayer and a debt acknowledgment: *The ground has received. The ground is owed. Blood closes what bone opens.*
- Exits: `south` → 479 (Memorial Byre), `east` → 484 (Bone-Cart Path)

**484 — The Bone-Cart Path**
`room_flags: dark no_recall` | `sector: field`
The service route connecting the burial fields to the bone processing station and ossuary entrance, worn smooth by generations of cart traffic carrying exhumed remains for processing and storage. The path is wide enough for a cart and surfaced with compacted grit rather than cobblestone. Root growth has been trimmed back along the cart-lane — the substrate keeps this route clear, suggesting the bone-transit function is still considered operationally necessary. At regular intervals along the path, iron hooks are set at cart-height in wooden stakes, where lanterns would have hung for night operations.
- Exits: `west` → 483, `north` → 477 (Bone Processing Station), `south` → 485 (Weight-Stone Platform)

**485 — The Weight-Stone Platform**
`room_flags: dark no_recall` | `sector: city`
The formal weight assessment station at the burial field's main axis, where coffins were verified against manifests and assessed for the per-weight burial fee. The scale apparatus here is more substantial than the processing yard's — this is where contested weights were adjudicated, where families could challenge the estate's assessment. The adjudication logbook is still on its stand. The majority of recorded challenges were resolved in the estate's favor. A small iron placard on the scale's frame reads: *Weight disputes subject to estate arbitration. All findings final. No appeals after burial.*
- Exits: `south` → 471 (Charnel Verge Gate), `north` → 486 (Memorial Lane), `east` → 484 (Bone-Cart Path)

**486 — The Memorial Lane**
`room_flags: dark no_recall` | `sector: field`
The formal processional route reserved for families who had paid the premium memorial service — a wide, paved path lined with low stone markers bearing the estate's thorn device, leading from the charnel verge gate northward toward the burial field's prestige plots. The prestige plots no longer exist as such; the premium-service graves were exhumed along with everything else during the ossuary expansion, and the families who paid for them received no notification. The Lane itself is intact, its markers still standing, its paving still level. It leads to nowhere that corresponds to what it was built to reach.
- Exits: `south` → 485, `north` → 562 (Zone VIII: Hunt Road Entry)

**487 — The Annex Burial Ground**
`room_flags: dark no_recall` | `sector: field`
The overflow section opened when the main burial fields reached capacity — less formally planned, with narrower path access and cruder grave markers. This ground shows the estate's haste more clearly than the main fields: plots are irregular, spacing is inconsistent, and the transition from the commercial interment style into the older gravewood's ridge-clan burial zone happens gradually and without a clear boundary. Root-mass here is the most vigorous anywhere on the surface — the substrate's response to the mixing of new commercial dead with the older, deeper network.
- Exits: `north` → 480 (Corpse-Lily Terraces), `east` → 489 (Gravewood Threshold), `south` → 500 (Zone IV: Cairn of Sealed Names)

**488 — The Grave Tunnel Surface Exit**
`room_flags: dark no_recall` | `sector: field`
The surface manifestation of the old burial tunnel that connects southward through the estate's subsurface to the grave tunnel hinge at the estate's base. This exit served the estate's convenience in both directions: funeral convoys arriving from Midgaard and Shadowmere could emerge here rather than traveling overland to the main gate, and processed remains could be transferred out of the estate through the tunnel without the visible traffic of cart convoys. The surface opening is framed in old stone, covered by an iron hatch that is now welded shut by thorn growth. The hatch can still be opened by something that understands the substrate's logic — which means it is, in practice, accessible only to the estate's own dead and to the Root Covenant.
- Exits: `north` → 476 (Rapid Burial Fields South), `down` → 441 (Grave Tunnel Hinge — alternate egress)

---

### Zone IV: The Older Gravewood (489–506) — 18 Rooms

The pre-estate ridge-clan burial ground, now fully integrated into the substrate's network. Forest logic is dominant here. Every path was originally a detour around an ancestor stand; the substrate has made those detours into the only navigable routes. The shamans and Root Covenant practitioners who work this zone treat the trees as participants in any negotiation.

**489 — The Gravewood Threshold** ★
`room_flags: dark no_recall` | `sector: forest`
Where the estate's managed land gives way to something older. The tree species change abruptly: behind is the estate's managed thornwood, manipulated by cultivation and substrate direction. Ahead are trees that predate the estate by centuries — oaks with boles three meters in diameter, their bark deeply fissured and dark with age. The transition is visible as a line in the ground: on the estate side, root-mass with a grid-like organization corresponding to the estate's planting plan; on the gravewood side, root-mass in a pattern that has no human authorship, dense and complex and old. A stone post at the threshold bears two marks: the Harren thorn device (from when the estate formally annexed this ground) and an older mark beneath it — a pressed handprint in ochre, the same motif that appears in the cave system below. Both marks are still legible. Neither has priority.
- Exits: `south` → 449 (Forecourt of Inheritance), `southeast` → 487 (Annex Burial Ground), `north` → 490 (Ancestor Stand Entry), `west` → 470 (Estate Orchard)

**490 — The Ancestor Stand Entry**
`room_flags: dark no_recall` | `sector: forest`
The first ancestor grove encountered on the gravewood's main path — a stand of seven oaks growing in a rough oval around a central clearing that cannot be entered directly. The path detours around the oval's eastern edge, as the ridge-clan practice required: the dead govern that space, and the living have no right of passage through it. The detour is not superstitious avoidance. The root-mass within the oval stands visibly raised above the path level — the substrate holds that space at a different pressure. The trees' lower branches have knitted together over the clearing, forming a roof. What is inside is not visible.
- Exits: `south` → 489, `north` → 492 (Thorn-Ring Circle), `east` → 491 (Face-Down Grove approach)

**491 — The Face-Down Grove**
`room_flags: dark no_recall cold` | `sector: forest`
The first of the genuine face-down oathbreaker burial sites — a grove where the ridge-clan community interred those who broke sworn covenants, face-down so they could not rise toward the sky. The posture was punitive and prophylactic: the earth was expected to hold what the community rejected. The substrate does. Attempts to dig into this grove produce ground that actively resists — not magically, not suddenly, but with a steady, increasing counter-pressure that builds the deeper you go, as though the geological layer is pressing upward to match any downward force. The trees here lean slightly inward, their canopy closing over the graves. The cold is the same cold as the sealed burial row — @@acrystal-frequency@@N, not atmospheric.
- Exits: `west` → 490, `south` → 505 (Gravewood Fringe East)

**492 — The Thorn-Ring Circle (Old)**
`room_flags: dark no_recall` | `sector: forest`
One of the original ridge-clan containment thorn rings — grown to full maturity over centuries into a solid biological structure nearly two meters high and half a meter thick, enclosing a burial space approximately six meters in diameter. The ring was a sealed verdict: the dead within had been judged and enclosed by the land. The ring's inner surface is smooth where the thorn growth has fused solid; the outer surface is aggressive, with living growth adding new layers annually. This is the substrate's oldest infrastructure. The Root Covenant reveres it without fully understanding it. Even they do not cross inside.
- Exits: `south` → 490, `north` → 493 (Path Detour Through Graves), `east` → 496 (Second Ancestor Grove)

**493 — The Path Detour Through Graves**
`room_flags: dark no_recall` | `sector: forest`
The ritualized path that the ridge-clan community routed around the major ancestor stands — a deliberate detour that acknowledged the dead's jurisdictional claim over certain spaces. The detour is marked by flat stones set flush with the ground at path-edge intervals, their surfaces bearing directional marks worn almost smooth. Following the marks is not intuitive; the path bends counter to the direction a traveler would choose, and ignoring the marks leads to ground where the substrate's root-pressure makes footing unreliable. The correct path is navigable. The incorrect path is punitive.
- Exits: `south` → 492, `north` → 494 (Root Network Clearing), `west` → 499 (Ridge-Clan Barrow)

**494 — The Root Network Clearing**
`room_flags: dark no_recall` | `sector: forest`
A natural clearing where the gravewood's root systems have surfaced — not as individual roots but as a coherent network, visible above the ground in braided cable-structures that crisscross the clearing floor. The cables vary from finger-thick to arm-thick, their surfaces alternating smooth and splintered in the pattern that indicates substrate stress vs. stability. A shaman with the knowledge can read the network's current state from here: which directions the substrate is routing energy toward, where pressure is building, whether the binding's integrity is holding in the local section. The clearing smells more strongly of sweet rot and tannin than anywhere else above ground — this is the substrate's ventilation point, where its internal chemistry surfaces.
- Exits: `south` → 493, `north` → 502 (Shaman's Hollow), `east` → 495 (Witch's Boundary Stones), `west` → 498 (Mire Approach West)

**495 — The Witch's Boundary Stones**
`room_flags: dark no_recall` | `sector: forest`
A ring of eleven standing stones, waist-high, marking the territorial boundary of the Thornwood Witch's domain. Each stone bears a different mark: some are Harren boundary-notation in estate script, some are Root Covenant contract-glyphs, and some are in a script that predates both — the same pre-estate marking language found on the face-down grove stones and in fragments of the Midgaard Watchers' Cairn site. The Witch has collected all three vocabularies and placed them in conversation. This is not a warning boundary. It is a filing — the Witch's legal claim, expressed in every language the gravewood recognizes.
- Exits: `west` → 494, `east` → 496 (Second Ancestor Grove), `south` → 504 (Substrate Surface Seep)

**496 — The Second Ancestor Grove**
`room_flags: dark no_recall` | `sector: forest`
A second ancestor stand, older and darker than the first. The grove's canopy has completely closed, making the interior lightless even in daylight. The trees here are the same old-growth oaks, but their root-mass has incorporated a different type of material: the substrate has been building with the remains of the deeper burials below this grove for long enough that the tree roots themselves have taken on mineral properties in the sections nearest the ground — the bark is partially crystalline, carrying the same cold and faint luminescence as the cave system far below. This is the gravewood's deepest surface connection to the substrate layer.
- Exits: `west` → 492, `south` → 495 (Witch's Boundary Stones), `east` → 497 (Whisper Path)

**497 — The Whisper Path**
`room_flags: dark no_recall` | `sector: forest`
A narrow path maintained by shaman practice — the path where, according to the tradition the shamans preserve, the substrate's output can be heard directly rather than read from secondary signs. The "whisper" is not metaphorical: visitors to this path reliably report hearing a low, irregular sound at the edge of audibility, somewhere between wind and speech, that does not localize to any specific direction. The shamans distinguish between at least four distinct patterns they describe as "attending," "routing," "defending," and "hungry." The fourth pattern is the one they decline to elaborate on.
- Exits: `west` → 496, `north` → 502 (Shaman's Hollow), `south` → 506 (Night Ride Path)

**498 — The Mire Approach West**
`room_flags: dark no_recall` | `sector: forest`
The western edge of the gravewood where it meets the mire's margins — the transition zone where the forest floor becomes uncertain and the ground's moisture content increases with each step eastward. Pale reeds begin here, their stems visible at the path's edge, already rattling without wind. The trees thin out and their root-mass transitions from the gravewood's dry, complex network to something waterlogged and simplified — the mire's ecology operating on different substrate chemistry. The boundary between forest and mire is not a clean line but a gradient, and the gradient is navigable only because the substrate maintains stepping-stone paths of slightly higher ground.
- Exits: `east` → 494 (Root Network Clearing), `south` → 507 (Zone V: Mire Edge)

**499 — The Ridge-Clan Barrow**
`room_flags: dark no_recall` | `sector: hills`
A proper earthen barrow from the pre-estate burial community — a long mound of piled earth and stone faced with mortarless dry-stone walling at the entrance end. The entrance is sealed with a single upright stone that has not been moved in any documented record. The barrow's outer surface is covered in centuries of moss and thorn growth, making it appear as a natural hill until the stonework becomes visible. The ridge-clan communities buried their honored dead in barrows — facing north, unlike the oathbreaker face-down graves. This barrow is treated by every faction in Thornwood as a protected site: the Harrens' records show it was explicitly excluded from the ossuary expansion plans, the Root Covenant conducts no rites here, and the substrate does not incorporate the barrow's interior into its network. It is the one space in Thornwood that the system leaves alone.
- Exits: `east` → 493 (Path Detour Through Graves), `south` → 500 (Cairn of Sealed Names)

**500 — The Cairn of Sealed Names**
`room_flags: dark no_recall` | `sector: forest`
A cairn of flat river-stones stacked to shoulder height at a crossroads between the older gravewood and the annex burial ground — a point where the two burial traditions intersect. This cairn was built by a community that had watched the Harren estate's deferred-naming practice and understood what was at stake: if the dead could be made nameless by a payment wall, then someone needed to be keeping the names in a form that no clerk could withhold. The stones bear pressed names — dozens of them, in a script that mixes pre-estate ridge-clan marking with Midgaard administrative lettering — names that correspond to the deferred-name case numbers in the estate's register. The cairn is a counter-archive. The substrate has not disturbed it.
- Exits: `north` → 499, `east` → 487 (Annex Burial Ground), `south` → 501 (Bone-Woven Tree)

**501 — The Bone-Woven Tree**
`room_flags: dark no_recall` | `sector: forest`
A mature oak whose root system has surfaced extensively around its base, and into which — through the root network, not through cutting — the substrate has woven human remains. Femurs appear at root junctions, serving as structural reinforcement. Skulls are cupped in root-forks above head height, their interiors containing a soil-and-crystal compound that may be the substrate's medium. Ribs run along the root cables in parallel. The result is a tree that is also a building, or a building that is also a tree, depending on whether you think function or material is the more fundamental category. The bone-weaving is not crude. It is precise. Each element serves a structural purpose that a builder would recognize.
- Exits: `north` → 500, `east` → 494 (Root Network Clearing), `south` → 520 (Zone V: Mire Egress North)

**502 — The Shaman's Hollow**
`room_flags: dark indoors no_recall` | `sector: inside`
The Thornwood Witch's residence — a hollow at the base of one of the gravewood's oldest oaks, expanded by root-management over decades into a space large enough for habitation. The interior walls are living root-fiber, formed into shelves, sleeping ledge, and hanging storage. Tools of shaman practice are everywhere: bone-reading sets, thorn-growth maps of the substrate's surface expression, jars of soil samples from different depth layers, a record-keeping system that mixes drawing and inscription in three languages. The Witch herself is rarely here when visitors arrive; her boundary stones suggest she is always somewhere in the gravewood. When she is here, she is not hostile. She is not safe.
- Exits: `south` → 494 (Root Network Clearing), `east` → 497 (Whisper Path)

**503 — The Blood Offering Platform**
`room_flags: dark no_recall` | `sector: forest`
An active Root Covenant ritual site — a flat stone platform at a root-contact point where the substrate's response to blood rites has been demonstrated repeatedly and reliably. The platform's stone surface is stained @@Rdark red@@N in a pattern that shows long use: central deposit, splatter radius, the channels where the offering ran toward the root-contact cracks. Around the platform, the ground carries thorn growth in the pointed, directed pattern that indicates recent substrate attention — the thorns are oriented toward the platform's center, as though the forest is leaning in. The platform is not abandoned. It was used recently.
- Exits: `west` → 496 (Second Ancestor Grove), `south` → 504 (Substrate Surface Seep), `east` → 578 (Zone IX: Covenant Offering Site East)

**504 — The Substrate Surface Seep**
`room_flags: dark no_recall cold` | `sector: forest`
Where the crystal substrate's influence reaches the surface in visible form: a seep of pale mineral compound welling up through the leaf-litter in a crack that runs roughly east-west, carrying the same cold as the cave system below. The seep is not water — it is a mineral-fluid mixture that maintains temperatures several degrees below ambient indefinitely, crystallizing at its edges into a pale crust. The seep's color is @@alight cyan@@N against the dark forest floor, faintly luminescent. Root-fiber threading through the crack carries the substrate's harmonic in audible range here — the sound the bone-weavers learn to read is clearest at this surface exposure.
- Exits: `north` → 495 (Witch's Boundary Stones), `east` → 503 (Blood Offering Platform), `down` → 561 (Zone VII: Deep Cave Egress — vertical connection to cave system, one-way down only; the cave provides an upward exit back here)

**505 — The Gravewood Fringe East**
`room_flags: dark no_recall` | `sector: forest`
The eastern fringe where the older gravewood meets the estate's commercial burial fields — the transition zone where ridge-clan burial ground was absorbed into the Harren estate's expansion without acknowledgment. The trees here are younger and less substantial than the gravewood's interior growth; this was the fringe, not the core. The root-mass underfoot shows the mixing: gravewood root patterns from the west, estate-grid root organization from the east, neither fully dominant. @@RDark staining@@N on several trees at eye height, in thin parallel lines, may be the residue of the boundary-marking that the ridge-clan community attempted when they realized the estate was expanding into their burial ground.
- Exits: `west` → 491 (Face-Down Grove), `east` → 481 (Thorn-Ringed Plots)

**506 — The Night Ride Path**
`room_flags: dark no_recall` | `sector: forest`
The route through the southern gravewood that the Wild Hunt uses when departing the estate for its perimeter-extension patrols. The path is marked by the Hunt's passage: frost-etched patterns in the bark of the bordering trees, visible as white tracery, and frost-prints pressed into the soil at the intervals of a horse's gait. The path itself is wider than a human-made trail would be and runs with a directness — through root-mass and undergrowth — that is possible only because the substrate clears it for the Hunt's use. At night, sound does not carry normally along this path. During the day, the path is merely a route. At night, it is a jurisdiction.
- Exits: `north` → 497 (Whisper Path), `south` → 571 (Zone VIII: Night Approach Road)

---

### Zone V: The Mire (507–522) — 16 Rooms

The peripheral wetland at Thornwood's eastern and southern margins. Funerary runoff, alchemical waste, and substrate water-table seepage have produced a toxic ecology unlike anything else in the estate. Routes are unstable by design — the substrate adjusts subsurface paths to redirect travelers. The fog shrine at the mire's center is the oldest ritual site in Thornwood.

**507 — The Mire Edge**
`room_flags: dark no_recall cold` | `sector: field`
Where solid ground ends without ceremony. The transition is not a cliff or a clear waterline but a gradual yielding: the earth softens underfoot, then wets, then begins to take footprints that fill slowly with dark water. Pale reeds have advanced to this line and stopped — or rather, the reeds mark this line and the mire marks everything beyond it. The smell changes here: from the forest's rot-and-tannin to something sulfurous and flat, the smell of water that has been holding dead material for a very long time. A stepping-stone path begins at the edge, its stones mossy granite pressed into the mire's surface at irregular intervals.
- Exits: `west` → 481 (Thorn-Ringed Plots, burial field connection), `east` → 508 (Stepping Stone Path Entry), `south` → 521 (Zone IX: Alchemical Runoff Channel approach)

**508 — The Stepping Stone Path (Entry)**
`room_flags: dark no_recall cold` | `sector: water_swim`
The mire's main navigable route, beginning at the eastern edge and proceeding inward along a sequence of moss-covered granite stones that provide marginally stable footing above the waterline. The stones are set at walking intervals and are large enough for one person at a time. The water between them is dark and still, its depth indeterminate — it takes objects thrown into it without audible impact. Pale reeds grow thickly on either side of the stepping path, their stems angled slightly toward the path as though monitoring it.
- Exits: `west` → 507, `east` → 509 (Pale Reed Verge)

**509 — The Pale Reed Verge**
`room_flags: dark no_recall cold` | `sector: water_swim`
A stretch where the stepping-stone path narrows and the reed growth presses in from both sides. The reeds rattle without wind — their movement is substrate output, readable to shamans as a directional indicator. The rattling here is notably irregular, suggesting the substrate is not holding a single direction but cycling through multiple route assessments. The water between the stones is shallower here, with a bottom visible as pale mineral sediment — the crystal substrate's runoff compositing with the mire's organic accumulation into a layer that has the color and texture of old bone.
- Exits: `west` → 508, `east` → 510 (Fog Shroud), `south` → 517 (Mire Fringe East)

**510 — The Fog Shroud**
`room_flags: dark no_recall cold` | `sector: water_swim`
The fog bank that occupies the mire's central section, dense enough that depth cues become unreliable at distances beyond two arm-lengths. The fog is not weather — it does not thin in wind or burn off in morning sun. It is a permanent product of the substrate's temperature differential: the crystal substrate's cold meeting the mire's decomposition warmth produces a luminous haze that makes the mire appear to glow from inside. Objects in the fog appear at wrong distances. Sound behaves differently here — speech carries farther than expected, footfalls carry shorter. The stepping-stone path continues through the fog, but the stones are not always where you expect them.
- Exits: `west` → 509, `east` → 511 (Black Water Pool), `north` → 512 (Mire Crossing Central)

**511 — The Black Water Pool**
`room_flags: dark no_recall cold` | `sector: water_noswim`
A pool of notably still, dark water where the stepping-stone path diverts around rather than through. The pool's surface is intact, undisturbed even by the fog — it has the quality of glass, and it reflects nothing accurately. Occasionally the surface breaks without explanation: a ring, a displacement, then stillness again. The pool's chemical composition is the end-product of the mire's entire waste system: burial runoff, alchemical outflow from the distillation building, substrate seepage. Nothing lives in it visibly. Something uses it.
- Exits: `west` → 510, `south` → 515 (Sulfur Vent Channel), `north` → 512 (Mire Crossing Central)

**512 — The Mire Crossing (Central)**
`room_flags: dark no_recall cold` | `sector: water_swim`
The middle of the mire, where the stepping-stone path is least reliable and the fog is thickest. The substrate adjusts the subsurface at intervals, slightly repositioning the stones or altering the water depth between them. A traveler who memorized the crossing last time will find it subtly different this time — not enough to prevent passage, but enough to prevent certainty. This is the substrate's territorial behavior applied to navigation: it allows passage but maintains ambiguity.
- Exits: `south` → 510, `north` → 513 (Raised Peat Island)

**513 — The Raised Peat Island**
`room_flags: dark no_recall` | `sector: field`
A stable island of compressed peat and sediment near the mire's center, elevated enough above the waterline to provide reliable footing. The island is roughly oval, large enough to encamp a small group. Its edges are ringed with the pale reeds, and its center is clear — kept that way by something's preference. The fog shrine is east of here. The island's peat surface carries clear footprints: human-shaped, but with the thorn-pattern texture of substrate-integrated feet. The island is used.
- Exits: `south` → 512, `east` → 514 (Fog Shrine), `north` → 520 (Mire Egress North)

**514 — The Fog Shrine** ★
`room_flags: dark no_recall cold` | `sector: field`
The oldest ritual site in Thornwood, predating the estate, the Root Covenant, and possibly the ridge-clan communities whose burial practices partially inspired the cult's theology. The shrine is a stone platform rising from the peat of a secondary island, accessible by a narrow stepping bridge from the raised peat island. The platform bears offerings: corroded bronze pieces, antler sections, small arrangements of animal bones placed with the precision of a filing system rather than the scatter of reverence. At the platform's center, a carved stone figure has been so thoroughly fitted with iron rings driven into its surface that the original form is unguessable — the rings cover every visible surface, overlapping and pressed flush with each other, as though someone found the original figure's identity a problem and decided to file the problem shut. The shrine is @@pcold even relative to the surrounding mire@@N, and carries the strongest harmonic tone anywhere on the surface — the crystal substrate's signature frequency, amplified through the platform's stone into the surrounding peat.
- Exits: `west` → 513

**515 — The Sulfur Vent Channel**
`room_flags: dark no_recall cold` | `sector: water_swim`
A drainage channel carrying the mire's sulfurous output toward the eastern margin, its surface marked by periodic gas venting that produces a flat, dead smell and a faint haze at water level. The channel receives runoff from both the burial fields and the cave system's water table. Walking the channel edge is possible but requires attention: the bank is unstable, the water is not swimmable, and the chemical content of the outflow has produced a pale, slightly crystalline deposit along the channel walls that is slippery under foot.
- Exits: `north` → 511 (Black Water Pool), `east` → 516 (Sunken Boundary Wall)

**516 — The Sunken Boundary Wall**
`room_flags: dark no_recall cold` | `sector: water_swim`
The original estate perimeter wall's southern section, which has subsided progressively into the mire over generations as the wetland expanded. The wall's upper courses are still above waterline — visible as a line of dressed stone running east-to-west, its mortar partly replaced by root-fiber that extends into the water below. The Harren boundary marks are still legible on the above-water faces. The wall still functions as a partial barrier: the substrate uses it as a routing anchor, and the water on the estate's interior side of the wall is navigably different from the water on the exterior side.
- Exits: `west` → 515, `east` → 517 (Mire Fringe East)

**517 — The Mire Fringe East**
`room_flags: dark no_recall cold` | `sector: water_swim`
The eastern mire margin, where the water shallows and the ground firms again — the exit point for travelers crossing the mire from west to east. The reeds here are denser and taller than at the entry, and their rattling has a different quality: more insistent, less informational. The substrate does not want travelers to exit here easily. The correct path through the reeds is findable but requires the same kind of attention the crossing demanded — watching where the reeds angle, where the ground rings hollow, where the stepping stones' moss is compressed by recent use.
- Exits: `north` → 509 (Pale Reed Verge), `west` → 516, `east` → 518 (Broken Causeway)

**518 — The Broken Causeway**
`room_flags: dark no_recall` | `sector: field`
The remains of a stone causeway that the Harren estate built through the mire's eastern margin to improve transit from the burial fields to the ossuary hillside cut. The causeway succeeded for approximately one generation before the substrate began breaking it — first at the joints, then in the middle spans, until the current state: three intact sections, two collapsed sections, and a gap in the middle that requires either swimming or a different route. The collapsed sections are visible as stone rubble just below the waterline, still serviceable as rough stepping ground for those willing to go in to the knee.
- Exits: `west` → 517, `north` → 522 (Substrate Water Sink)

**519 — The Deep Mire**
`room_flags: dark no_recall cold` | `sector: water_noswim`
The mire's deepest section — the area where the water table is highest and where the accumulated material from generations of drainage has produced a depth that is genuinely dangerous. There is no safe crossing through the deep mire; the substrate maintains it as an impassable barrier in the estate's tactical geography. The water here carries a visibility of zero. The surface occasionally expresses large, slow movement — not the breaking of something coming up, but the settling of something very large shifting position far below. The deep mire connects to the cave system's water table; the crystal substrate's influence is visible as @@alight-cyan phosphorescence@@N in the deepest water, reaching up from the cave formations below.
- Exits: `south` → 517 (approach only — entering from north not possible; this room is a dangerous dead-end; the only exit is back south)

**520 — The Mire Egress North**
`room_flags: dark no_recall` | `sector: forest`
The northern mire margin where the wetland meets the southern edge of the older gravewood — the connection between the two zones. The ground here is firm enough to walk on but retains the mire's moisture, producing a soft surface where footprints are preserved indefinitely. A thorn-pattern marker post at the margin boundary carries a Root Covenant filing-glyph: a circle with four diagonal lines, indicating that this crossing is a recognized transit point in the cult's route system. The gravewood's trees begin immediately beyond the marker.
- Exits: `south` → 513 (Raised Peat Island), `north` → 501 (Zone IV: Bone-Woven Tree)

**521 — The Alchemical Runoff Channel**
`room_flags: dark no_recall` | `sector: water_swim`
The drainage channel running south from the distillation outbuilding to the mire, carrying decades of alchemical waste: bone oils, botanical extracts, tannin concentrates, and mineral compounds from the distillation process. The channel's walls carry a chemical deposit that has altered the stone's surface texture — smooth and slightly iridescent where the most reactive compounds settled. The water in the channel is not safe to contact; even the mire's residents avoid the channel itself, using the adjacent ground instead. The channel connects east to the mire proper and west to the distillation outbuilding above.
- Exits: `north` → 466 (Zone II: Distillation Outbuilding), `east` → 507 (Mire Edge), `south` → 522 (Substrate Water Sink)

**522 — The Substrate Water Sink**
`room_flags: dark no_recall cold` | `sector: water_swim`
A natural depression at the mire's southern margin where the substrate's water table surfaces as a steady seep of cold, mineral-rich water from the cave system below. The seep runs contrary to the mire's prevailing drainage: the mire flows generally eastward, but this sink takes water from below and holds it in a small, permanently cold pool that does not drain into the mire proper. The water in the pool carries @@acrystal-mediated luminescence@@N — faint, pale, and cold, identical to the bioluminescence in the cave system's deepest passages. A shaman who knows what they are looking for can read the sink's output as a direct signal from the binding's current state.
- Exits: `north` → 518 (Broken Causeway), `west` → 521 (Alchemical Runoff Channel), `northeast` → 582 (Zone IX: Wraith Passage)

---

### Zone VI: Ossuary Undercroft (523–540) — 18 Rooms

Below the manor and hillside. Funerary and substrate logics are co-dominant here. The bone weavers work in this layer; the drain passages carry the binding's inscription text toward the cave system below. The transition from ossuary to cave is the transition from human-managed funerary industry to the substrate's own domain.

**523 — The Undercroft Stairs**
`room_flags: dark indoors no_recall cold` | `sector: inside`
The stone stair descending from the wine cellar's iron hatch into the ossuary undercroft. The stairs are narrow, cold-mortared, and steep — built for function, not comfort. The descent takes a traveler from the wine cellar's ambient cold (already notable) into a qualitatively different cold: the bone-dust dryness of the ossuary layer, where temperature is maintained by the crystal substrate below rather than by any natural process. The stair walls carry the first of the drain passage inscriptions — not the urgent, small-script text of the lower passages, but the full-size, deliberate lettering of the binding's opening clauses.
- Exits: `up` → 460 (Wine Cellar), `north` → 524 (Upper Undercroft Hall)

**524 — The Upper Undercroft Hall**
`room_flags: dark indoors no_recall cold` | `sector: inside`
The formal entry to the ossuary layer — the administrative reception space for the undercroft, where intake records were processed before remains were assigned to specific vault sections. A desk and record-stand still occupy the hall's entry end; the records in the stand are organized by the estate's case-number system, cross-referenced to the deferred-name register above. The ossuary's cataloguing system here is meticulous and unsentimental: each entry notes the case number, intake date, bone condition, vault assignment, and processing notes in the bone weaver's abbreviated professional vocabulary. The hall's ceiling is vaulted, and the vault springs carry @@psubstrate-glyph decorations@@N cut into the stone — not the estate's heraldry, but the binding's inscription geometry.
- Exits: `south` → 523 (Undercroft Stairs), `north` → 525 (Bone-Sorting Station), `east` → 531 (Cut-Stone Passage)

**525 — The Bone-Sorting Station**
`room_flags: dark indoors no_recall cold` | `sector: inside`
The first active processing area — where bone weavers perform the initial categorization of material coming in from the hillside ossuary and from the exhumation transit hall. Long stone tables with partitioned sections hold sorted material by type: long bones, flat bones, small bones, skull material, mixed-origin material for the construct assembly floor. The bone weavers' tools are present: measurement gauges, density testers, selection rods for identifying structural-grade material. The station is operational. The sorting continues regardless of visitors. The bone weavers do not pause.
- Exits: `south` → 524, `north` → 526 (Femur Gallery), `east` → 529 (Bone-Weaver's Workshop)

**526 — The Femur Gallery**
`room_flags: dark indoors no_recall cold` | `sector: inside`
A long corridor whose walls have been converted into structural bone storage: femurs and tibiae stacked horizontally in recessed niches, floor to ceiling, their ends facing outward in a pattern that is simultaneously storage and load-bearing structure. The bones are not merely placed — they are integrated with the wall's limestone, held in place by root-fiber threading at the joints, the organic and mineral fused into a composite material that the bone weavers treat as more durable than either component alone. The gallery's floor is polished smooth by generations of foot traffic and cart passage. Sound in the gallery is unusual: it arrives flat, with no echo — the bone-and-root composite absorbs rather than reflects.
- Exits: `south` → 525, `north` → 527 (Skull Vault), `east` → 533 (Exhumation Transit Hall)

**527 — The Skull Vault**
`room_flags: dark indoors no_recall cold` | `sector: inside`
A domed chamber dedicated to skull storage, organized by the bone weavers' categorization system: skulls sorted by density, age-at-death, and "resonance grade" — a quality the bone weavers assess by striking the skull and listening to the harmonic output. Higher-resonance skulls are stored in the inner ring of wall-niches; lower-resonance in the outer ring. The skulls in the inner ring are all from the sealed burial row above — the substrate-connected burials whose resistance to exhumation was eventually overcome through a technique the bone weavers decline to describe in detail. The dome's interior surface carries one complete verse of the binding's inscription text, pressed into the stone in the Root Covenant's contract-liturgy script, circling the dome at eye height.
- Exits: `south` → 526, `east` → 528 (Rib-Cage Armature Chamber)

**528 — The Rib-Cage Armature Chamber**
`room_flags: dark indoors no_recall cold` | `sector: inside`
A workshop space where partially assembled construct frames are stored between working sessions. Rib-cage sections — from multiple sources, assembled into larger composite structures — hang from ceiling hooks on chains, their curved geometry holding internal volumes that will eventually house other components. Against the walls, armature sections in various stages of completion: some are clearly recognizable as enlarged human torso-structures; others have departed from the human template entirely, incorporating elements from non-human remains in configurations the substrate appears to have specified.
- Exits: `west` → 527, `south` → 529 (Bone-Weaver's Workshop)

**529 — The Bone-Weaver's Workshop**
`room_flags: dark indoors no_recall cold` | `sector: inside`
The master bone weaver's primary working space — where the most demanding construction work is done. Workbenches carry in-progress pieces; tool racks hold implements that have no counterparts in ordinary craft: resonance hammers sized to specific frequency ranges, root-threading needles with substrate-treated tips, structural gauge sets calibrated against the ossuary's material standards. The master bone weaver's bench holds a current project: a hand assembly, five fingers in varying stages of completion, the joints threaded with root-fiber at precisely the same gauge as the bone-woven tree in the gravewood above. The workshop is tidy in the way that a craftsman's space is tidy — everything where it can be found, nothing decorative.
- Exits: `north` → 525 (Bone-Sorting Station), `west` → 528, `south` → 538 (Construct Assembly Floor)

**530 — The Hillside Ossuary Entry**
`room_flags: dark indoors no_recall cold` | `sector: inside`
The connection point between the burial fields above and the ossuary undercroft: the inner face of the hillside cut, where the gated entrance opens into the undercroft's eastern section. The approach from the burial fields (via 478) deposits arrivals here. The entry chamber is fitted with intake hardware — a scale, a manifest-check stand, a lamp-hook now empty — that mirrors the surface processing equipment above. The duplicate infrastructure is deliberate: the estate maintained parallel processing systems at both the surface and undercroft levels so that remains could be received from above or below without crossing operational paths.
- Exits: `up` → 478 (Zone III: Ossuary Hillside Cut), `west` → 531 (Cut-Stone Passage)

**531 — The Cut-Stone Passage**
`room_flags: dark indoors no_recall cold` | `sector: inside`
A passage cut directly into the hillside limestone, older than the formal undercroft construction — this was the original tunnel that predated the Harren estate's systematic ossuary development, part of the same burial-route infrastructure as the grave tunnels connecting to Midgaard and Shadowmere. The walls here carry marks from three distinct eras of use: the pre-estate chisel-work in the stone, Harren-era carved administrative markers, and Root Covenant inscription additions. The passage smells differently from the formal undercroft — older, drier, more mineral, with a trace of the cave system's crystal cold coming from below.
- Exits: `east` → 530 (Hillside Ossuary Entry), `west` → 524 (Upper Undercroft Hall), `south` → 532 (Deep Ossuary Vault)

**532 — The Deep Ossuary Vault**
`room_flags: dark indoors no_recall cold` | `sector: inside`
The densest bone-storage area in the undercroft — a large vaulted chamber where the accumulated processed remains of the estate's operational peak are stacked to the ceiling in organized sections. The sheer volume is the first thing: the chamber holds more material than the surface burial fields would suggest, because the substrate's assimilation of ridge-clan remains into the network added to the estate's material inventory without any corresponding surface interment. The bone weavers treat this vault as their primary raw material supply — a library of structural elements organized by the same cataloguing system as the upper vaults, but more dense and more varied.
- Exits: `north` → 531, `south` → 533 (Exhumation Transit Hall), `down` → 535 (Drain Passage Entry)

**533 — The Exhumation Transit Hall**
`room_flags: dark indoors no_recall cold` | `sector: inside`
The routing corridor between the burial fields (via the hillside cut) and the ossuary processing stations — where remains in transit passed from intake to processing. The hall's walls carry transit records: chalk marks on a slate board updated by each shift of bone-processing laborers, recording what moved where and when. The most recent entries on the board are not in chalk. They are pressed into the slate by root-fiber — the substrate recording its own transit activity in the estate's inherited notation system.
- Exits: `north` → 526 (Femur Gallery), `east` → 530 (Hillside Ossuary Entry)

**534 — The Resonance Chamber**
`room_flags: dark indoors no_recall cold` | `sector: inside`
A circular chamber specifically designed for the bone-weaver practice of resonance testing — assessing material by its harmonic output when struck. The chamber's geometry produces natural amplification; a bone struck at the center can be clearly evaluated from the walls. The bone weavers use this chamber for high-grade material assessment, but also for something else: the resonance testing of assembled construct sections, to confirm that the substrate integration is functioning correctly in the finished work. The tone produced by a properly integrated construct section matches the crystal substrate's harmonic signature. The chamber is used regularly; the air carries a faint harmonic afterimage of recent sessions.
- Exits: `east` → 532, `south` → 538 (Construct Assembly Floor)

**535 — The Drain Passage Entry**
`room_flags: dark indoors no_recall cold` | `sector: inside`
The beginning of the drain passages — channels originally built to carry water drainage from the ossuary vaults downward to the cave system's water table, now carrying the binding's inscription text on their walls. At full height here, the text is legible as a deliberate, carefully lettered working in the binding's script: the opening of the binding's formal declaration, in large characters with generous spacing. This section reads clearly and confidently. The passage smells of cold mineral and the faint ozone-adjacent quality of the crystal substrate's chemistry.
- Exits: `up` → 532 (Deep Ossuary Vault), `down` → 536 (Upper Drain Passage)

**536 — The Upper Drain Passage**
`room_flags: dark indoors no_recall cold` | `sector: inside`
The drain passage's upper middle section, where the binding's inscription text continues. The characters here are smaller — not from a smaller hand, but from a recalibration: the author had more text to fit than the passage's length easily accommodates. The text is still fully legible and the lettering is still controlled, but the density is visibly higher than above, and the wall surface is used more completely. The content shifts here from declaration to specification: where the upper section stated the binding's purpose, this section describes its mechanisms and dependencies.
- Exits: `up` → 535, `down` → 537 (Lower Drain Passage)

**537 — The Lower Drain Passage**
`room_flags: dark indoors no_recall cold` | `sector: inside`
The drain passage's final section before the cave system, where the binding's inscription text becomes urgent. The characters are very small now — the lettering of someone who is running out of both space and, possibly, time. The text covers every available surface: walls, ceiling, and the passage floor where it is not covered by water. The content here appears to describe contingencies: what happens if specific components of the binding fail, what substitutions are permitted, what cannot be substituted. The @@pcold blue glow@@N of the crystal substrate's bioluminescent fungal output begins here, coming from below, providing the only light in the passage.
- Exits: `up` → 536, `down` → 541 (Zone VII: Cave Mouth)

**538 — The Construct Assembly Floor**
`room_flags: dark indoors no_recall cold` | `sector: inside`
A large open floor where the bone weavers assemble their larger-scale constructs — marrow constructs and bone crypt configurations that cannot be built in the workshop's limited space. The ceiling here is high, allowing for vertical assembly. Chain hoists hang from iron fixtures in the ceiling for lifting large sections. The floor surface is marked with assembly reference lines: geometric guides for positioning limb angles, torso proportions, and articulation junction placement. Current projects in various stages of completion stand along the walls. The largest is not finished. It has been in progress for some time. Its scale exceeds what the estate's ossuary alone could supply — the substrate appears to have been routing material here from sources the bone weavers' records do not document.
- Exits: `north` → 529 (Bone-Weaver's Workshop), `east` → 534 (Resonance Chamber), `south` → 539 (Marrow Fiend Vault)

**539 — The Marrow Fiend Vault**
`room_flags: dark indoors no_recall cold` | `sector: inside`
Storage for completed large-scale constructs — the finished marrow fiends and bone crypt guardians waiting for deployment. The vault is tall and long, and the constructs that occupy it are correspondingly large: four meters or more in height, with excess limbs, rib-cage torsos, and articulated bone-plate coverage that gives them the visual impression of collapsed architecture that has decided to stand up. They are still. The stillness is not inactivity — the substrate holds them in reserve, and the harmonic in this vault is notably stronger than elsewhere in the undercroft, as though the crystal is paying closer attention to this room than to the spaces above.
- Exits: `north` → 538, `east` → 540 (Cave System Entry Threshold)

**540 — The Cave System Entry Threshold**
`room_flags: dark indoors no_recall cold` | `sector: inside`
The boundary between the ossuary undercroft and the cave system below — where cut stone gives way to natural limestone and the substrate's geological layer becomes physically accessible. The threshold is marked by a change in construction: the undercroft's mortared stonework ends at a ragged line, and the cave's natural formation begins without a designed transition. Someone has added carved marks at the transition — not the estate's administrative notation, not the Root Covenant's contract script, but the same pre-linguistic pressed handprint in ochre that appears at the grave tunnel hinge, the gravewood threshold, and the cave system's interior. The substrate marked its own boundary.
- Exits: `west` → 539 (Marrow Fiend Vault), `down` → 541 (Zone VII: Cave Mouth)

---

### Zone VII: Cave System and Substrate Core (541–561) — 21 Rooms

The deepest layer. Substrate logic is total here. Lord Harren's authority does not reach below the undercroft; the Lich's authority is absolute. The crystal formation, the handprint cave, the high ledge, and the sealed chamber are the architectural heart of what Thornwood actually is. Threat tier 68–80.

**541 — The Cave Mouth**
`room_flags: dark no_recall cold` | `sector: inside`
The natural cave entrance from the ossuary undercroft — the first room of the cave system proper. The floor transitions from cut stone to cave floor: limestone smoothed by water flow over geological time, not by human work. The ceiling drops and the walls become irregular, following the natural formation rather than any planned geometry. The cold here is different from the undercroft's bone-dust cold: it is wet and mineral, carrying the cave system's water table humidity, and it comes with the faint harmonic tone that will intensify throughout the descent. The @@acold blue luminescence@@N of the substrate-connected fungal network begins on the cave walls here, in sparse patches that increase in density with depth.
- Exits: `up` → 540 (Cave System Entry Threshold), `north` → 542 (Crystal Passage Entry)

**542 — The Crystal Passage Entry**
`room_flags: dark no_recall cold` | `sector: inside`
The first sighting of the crystal formation: a vein of pale crystal running through the limestone of the passage's eastern wall, half a meter wide and running floor to ceiling before disappearing into the rock above and below. The crystal carries the same anomalous cold as the surface seep in the gravewood — several degrees below ambient — and produces a faint luminescence that colors the passage walls @@alight blue@@N. The vein is not the formation's extent; it is an exposure point, a place where the geological structure surfaces through the limestone. The formation itself continues in all directions, deeper and wider than any exposure suggests.
- Exits: `south` → 541, `north` → 543 (Low Crystal Gallery), `east` → 545 (Harmonic Alcove)

**543 — The Low Crystal Gallery**
`room_flags: dark no_recall cold` | `sector: inside`
A gallery where crystal exposures are dense enough to provide significant luminescence without any external light source — the cave walls here glow @@asoft cyan@@N from multiple points, producing a shadowless illumination that makes the space feel larger than it is. The crystal veins here are older and more complex than the passage entry exposure: they branch and interweave through the limestone, and their surfaces show the growth rings of centuries of slow crystal formation. The Root Covenant has carved offering niches into the wall below each major vein — small recesses containing bone chips and pressed handprints in ochre, the same motif as the threshold marks above.
- Exits: `south` → 542, `north` → 546 (Handprint Cave), `west` → 544 (Water Table Pool)

**544 — The Water Table Pool**
`room_flags: dark no_recall cold` | `sector: water_swim`
A natural pool where the cave system's water table surfaces. The water is cold beyond what the air temperature alone would produce — it carries the crystal substrate's thermal anomaly directly, and its surface is faintly @@aluminescent@@N where the crystal compounds have dissolved into the water column. This pool feeds the substrate water sink in the mire above: the connection is underground and not directly passable, but the water chemistry is identical. The pool's bottom is not visible. Animals do not drink from it. The bone weavers treat the pool's output as a material of specific applications.
- Exits: `east` → 543, `south` → 552 (Secondary Sigil Floor)

**545 — The Harmonic Alcove**
`room_flags: dark no_recall cold` | `sector: inside`
A natural alcove where the crystal formation produces audible harmonic output — the tone that shamans and bone weavers learn to read, here at its clearest and most accessible surface expression. The tone is constant: a single sustained note at the low edge of human hearing, with overtones that shift in a pattern the cult's bone-readers describe as the substrate's "attention state." The alcove's walls carry the crystal frequency's effect on the surrounding limestone — a slow erosion pattern that corresponds to the harmonic's geometry, producing a surface texture that a trained hand can read like Braille.
- Exits: `west` → 542, `south` → 554 (Lichen-Light Passage)

**546 — The Handprint Cave** ★
`room_flags: dark no_recall cold` | `sector: inside`
A chamber whose entire surface — floor, walls, ceiling, every accessible face — is covered with overlapping right handprints in ochre pigment, all in identical posture: palm flat, fingers extended, thumb at right angles. The prints are not all the same size; they range from a small child's to a large adult's, with every intermediate size represented. They overlap at every density, with older prints visible beneath newer ones, the layers building up over what must be centuries of continuous addition. A fresh print — ochre still damp — occupies the chamber's eastern face. Someone was here recently. The prints read, per the Root Covenant's interpretation, as a witness record: every hand that consented to or was consumed by the binding, pressed into the stone as permanent testimony. Whether the most recent print indicates a new addition to the binding's accounting, or something else, is unclear.
- Exits: `south` → 543 (Low Crystal Gallery), `east` → 547 (Consent Inscription Hall), `north` → 548 (High Cave Ledge)

**547 — The Consent Inscription Hall**
`room_flags: dark no_recall cold` | `sector: inside`
A hall adjacent to the handprint cave where the binding's consent apparatus is documented in text rather than impression. The walls carry a list — names and contract-notations — in a script that blends the binding's formal inscription language with Midgaard's administrative lettering. The entries predate the estate; the oldest are in the ridge-clan marking system. The most recent are in Root Covenant contract-liturgy. The list is long. Many of the names correspond to the deferred-name case numbers in the estate's register above. Whether this means the deferred-name dead are recorded here as binding participants — or binding material — is the central question of the archive problem that the Reckoning faction in Midgaard is trying to answer.
- Exits: `west` → 546, `south` → 553 (Cold Dark Corridor)

**548 — The High Cave Ledge** ★
`room_flags: dark no_recall cold` | `sector: inside`
A natural ledge overlooking the crystal heart basin below — a viewing point where the binding's geometry is visible as a coherent pattern in the cave system's layout. From this vantage, the crystal formation's arrangement in the basin resolves into a shape that matches the sigil inscribed on the wine cellar floor and the sealed chamber below: a complex inscribed circle, executed in geological rather than drawn form, with the crystal veins as the inscribed lines. The pattern shows unmistakable signs of stress: the crystal formation's symmetry is degrading, with several vein sections showing darkening and one major section where the luminescence has gone entirely dark. The binding is failing. It is visible from here in the way that structural damage is visible in a building when you step back far enough to see the whole facade.
- Exits: `south` → 546 (Handprint Cave), `down` → 549 (Crystal Heart Basin), `east` → 555 (Cave Echo Chamber)

**549 — The Crystal Heart Basin** ★
`room_flags: dark no_recall cold no_mob` | `sector: inside`
The crystal formation's deepest accessible point — a basin of crystal growth at cave-floor level, where the formation is densest and oldest. The basin is roughly circular, perhaps fifteen meters in diameter, with crystal growth rising from the floor in columns and branching forms. The luminescence here should be intense; it is not. The formation is dimming. The cold remains — this is the coldest room in the area — but the light is reduced to a fraction of what the column growth implies it once produced, leaving large sections of the basin in a darkness that the remaining luminescence makes more apparent by contrast. The Root Covenant's four competing theories about the binding's failure are all legible here: the Lich's theory (coherence loss) in the failing geometry, Harren's theory (expanding pressure) in the stressed crystal at the basin edges, the cult's theory (renegotiation demand) in the fresh offering-deposits on the basin floor, and the witches' theory (already failed, restarting) in the new crystal growth beginning at the base of the darkened sections — small, @@adim, and differently structured@@N from the original formation.
- Exits: `up` → 548 (High Cave Ledge), `north` → 550 (Root Network Hub), `east` → 556 (Ancient Formation Gallery)

**550 — The Root Network Hub**
`room_flags: dark no_recall cold` | `sector: inside`
The underground convergence point of the root systems that distribute the binding's effects across the estate above. The cave ceiling here is penetrated by root-mass descending from the ossuary and gravewood layers — thick cables of biological material that have grown through limestone over generations to reach the crystal formation below. The roots are connected to the crystal at contact points where the rock has been dissolved by the roots' chemistry, allowing direct mineral exchange. The hub is not a metaphor. It is an infrastructure node. The binding's effects move from crystal to root network to surface here, and the operational state of the hub is directly readable in the root cables' condition: some are vigorous; some are degraded; one major cable on the eastern wall has separated from its crystal contact point entirely.
- Exits: `south` → 549, `west` → 551 (Binding Inscription Passage)

**551 — The Binding Inscription Passage**
`room_flags: dark no_recall cold` | `sector: inside`
The lower end of the drain passage inscription system — the cave-level passage where the binding's text reaches its most urgent expression. The inscription here is not on cut stone but on natural limestone, pressed into the cave wall's surface by root-fiber rather than carved by tool. The characters are very small, very dense, and cover every available surface including the floor and ceiling. The content, where it can be read, appears to be the binding's emergency protocols: what the binding does when components fail, what the Lich is authorized to substitute, and what substitutions are explicitly forbidden. The forbidden list is longer than the permitted list.
- Exits: `east` → 550 (Root Network Hub), `south` → 557 (Sealed Chamber Approach)

**552 — The Secondary Sigil Floor**
`room_flags: dark no_recall cold` | `sector: inside`
A natural chamber where the binding's secondary inscription — the sigil identical to the wine cellar's floor inscription — appears in its geological form: the crystal veins in the cave floor have been arranged into the sigil's pattern by the same process that produced the surface seep's crystallization. The arrangement is not perfect; the geological constraints of the formation produce slight variations from the drawn version in the wine cellar. Whether the geological version or the drawn version is the original — whether the Lich inscribed the wine cellar sigil to match a formation it found, or the formation grew toward a sigil the Lich inscribed — is a question about which came first that neither version of the text answers.
- Exits: `north` → 544 (Water Table Pool), `east` → 553 (Cold Dark Corridor)

**553 — The Cold Dark Corridor**
`room_flags: dark no_recall cold` | `sector: inside`
A passage where the crystal substrate's thermal anomaly is strongest and the luminescence has failed entirely, producing absolute darkness combined with cold that registers as physically painful on exposed skin. The cold here is not natural — it is the crystal formation under stress, its thermal regulation failing in the same pattern as its luminescence. The passage connects the secondary sigil floor to the consent inscription hall, running along the binding's eastern margin. Travelers without their own light source are effectively blind; travelers with light source are navigable but uncomfortable. The substrate does not maintain this passage for convenience.
- Exits: `west` → 552 (Secondary Sigil Floor), `north` → 547 (Consent Inscription Hall)

**554 — The Lichen-Light Passage**
`room_flags: dark no_recall cold` | `sector: inside`
A passage colonized by the substrate-connected fungal network that provides the cave system's bioluminescent baseline — the cold blue light visible throughout the formation's healthier sections. Here the fungal growth is dense enough to provide reliable navigation light in @@asoft, unsteady blue@@N. The lichen has grown according to the root network's routing, covering ceiling and walls in patterns that correspond to the binding's geometry: the lichen is densest where the binding is intact and thinner where the binding is degraded, providing a direct visual map of the binding's current status for anyone who knows what they are looking at.
- Exits: `north` → 545 (Harmonic Alcove), `south` → 556 (Ancient Formation Gallery)

**555 — The Cave Echo Chamber**
`room_flags: dark no_recall cold` | `sector: inside`
A natural chamber whose geometry produces sustained echo — sound introduced here reflects and compounds for several seconds before decaying. The substrate uses this chamber's acoustic properties: the crystal harmonic tone, transmitted through the root network, arrives here amplified and sustained, producing a room-filling resonance that the bone weavers' resonance-testing methodology was calibrated against. The echo of voices here is disorienting — what you said three seconds ago is still audible as a slight distortion of what you are saying now. Experienced Root Covenant practitioners use the echo chamber for bone-reading diagnostics, checking the substrate's harmonic against their calibrated expectations.
- Exits: `west` → 548 (High Cave Ledge), `south` → 554 (Lichen-Light Passage)

**556 — The Ancient Formation Gallery**
`room_flags: dark no_recall cold` | `sector: inside`
A gallery displaying the oldest sections of the crystal formation — growth that predates human use of the site and extends back beyond any dateable layer. The crystals here are larger and more complex than the formation's other sections, with internal structures visible as banding and inclusion patterns accumulated over centuries. Their surface texture differs from the younger sections: the new growth is smooth and faceted; the ancient growth is rough and irregular, as though the formation's earliest expansion was exploratory rather than organized. In the gallery's deepest alcove, the crystal carries an internal banding pattern that matches the geometry of the Spirebound Conclave's documented Prism Thesis crystal field trial markings — a match that would be significant if confirmed, connecting Thornwood's substrate to the same experimental program that produced the Withered Depths' blight. The match has not been confirmed. It has not been investigated.
- Exits: `west` → 549 (Crystal Heart Basin), `north` → 554 (Lichen-Light Passage)

**557 — The Sealed Chamber Approach**
`room_flags: dark no_recall cold` | `sector: inside`
The final passage before the Lich's domain — a straight corridor whose walls carry the binding's inscription in its most complete and deliberate form: every clause of the binding's formal text, rendered in the original full-size lettering of the inscription's first section, as though this passage was the version produced when there was still time and space to be thorough. The passage ends at a door of stone fitted into the natural cave wall — not a manufactured door but a stone slab placed to seal the opening, bearing the binding's primary sigil carved into its outward face. The slab is cold to the touch and resonates faintly under pressure.
- Exits: `north` → 551 (Binding Inscription Passage), `south` → 558 (Threshold of the Binding)

**558 — The Threshold of the Binding** ★
`room_flags: dark no_recall cold no_recall no_portal no_teleport` | `sector: inside`
The final threshold before the sealed chamber — the door room where the Harren Estate Register Key is required for passage. The stone slab door is here, its sigil face toward this room. The threshold carries all four of the binding's operating logics simultaneously: the door's carved thorn-motif surround (manorial), the bone-inlay at the door's base (funerary), the root-fiber threading through the door's stone in the same pattern as the wine cellar inscription (forest), and the crystal formation embedded in the door's stone itself glowing at @@plow, pulsed frequency@@N (substrate). The four logics as one mechanism. The door cannot be forced by ordinary means. It responds to the register key because the key's object properties are calibrated to the binding's own authentication layer.
- Exits: `north` → 557, `south` → 559 (Sealed Chamber — key-locked door; key: object 511)
- Door: `^sealed stone door` — `EX_ISDOOR` set, reset to locked (state 2), key_vnum = 511

**559 — The Sealed Chamber** ★ (Boss room: Thornwood Lich — `no_mob` flag)
`room_flags: dark indoors no_recall cold no_mob no_portal no_teleport` | `sector: inside`
The Thornwood Lich's domain — the chamber where the binding's primary sigil is inscribed on the floor and where the Lich occupies both the sigil's center position and the binding's operational interface. The chamber is circular, the inscribed circle filling its entire floor, the lines of the sigil rendered in crystal-inlay that provides the room's only illumination — @@pdim purple@@N at the sigil edges, @@acold blue@@N at the crystal points. The Lich itself does not conform to any expected appearance. It is present, in the way that an architectural principle is present: visible in what the room does rather than in any single located form. When it chooses to speak, the sound comes from the crystal formation rather than from any specific point. It speaks in systems. It does not threaten. It does not explain. It diagnoses. If it addresses an intruder, it is because the intruder has become relevant to the binding's current failure analysis — and that is not good news for the intruder.
- Exits: `north` → 558 (key-locked door from this side: `EX_ISDOOR`, can be opened from inside without key)
- Boss: The Thornwood Lich (mob 484) — sentinel, boss, no_mob room, spec_cast_undead

**560 — The Substrate Interface**
`room_flags: dark indoors no_recall cold` | `sector: inside`
A secondary chamber adjacent to the sealed chamber, connected by a narrow passage in the natural cave wall — accessible, but not the sealed chamber's main entrance. This is where the Lich communicates with the root network: a crystal-dense wall surface carries the binding's transmission layer, and the root cables that descend from the network hub connect to this surface. The Lich's adjustments to the binding — the corrections and calibrations that keep the system in its current degraded-but-functional state — happen here, expressed as changes in the crystal's harmonic output that propagate upward through the root network to the estate's surface behavior. The room has the quality of a control room whose operator has been on continuous duty for a very long time.
- Exits: `east` → 559 (Sealed Chamber, via internal passage — does not require key from this side), `north` → 550 (Root Network Hub)

**561 — The Deep Cave Egress**
`room_flags: dark no_recall cold` | `sector: inside`
A natural upward passage at the cave system's northwestern margin — a crack in the limestone wide enough to climb, leading up through the geological layers toward the gravewood's surface. The climb is long and requires equipment or substrate familiarity to navigate safely. The Thornwood Witch knows this passage. The Root Covenant's senior practitioners know it. For everyone else, it is a crack in the rock that does not obviously lead anywhere. At the top, the passage emerges at the substrate surface seep in the older gravewood — the place where the crystal substrate's surface expression is strongest.
- Exits: `south` → 550 (Root Network Hub), `up` → 504 (Zone IV: Substrate Surface Seep)

---

### Zone VIII: Wild Hunt Patrol Routes (562–571) — 10 Rooms

The Wild Hunt's operating territory — the roads, staging grounds, and patrol routes where the estate's mobile enforcement arm projects Thornwood's jurisdiction beyond the fixed perimeter. Night-only content is concentrated here; the Wild Hunt Master and Riders only spawn at night.

**562 — The Hunt Road Entry**
`room_flags: dark no_recall` | `sector: forest`
Where the Wild Hunt's patrol routes begin, accessible from the memorial lane's northern end. The road here is unlike anything else in the burial fields: unnaturally wide, its surface clear of the root-mass that claims every other path, and bordered by trees whose bark has been marked at regular intervals with @@yharren-gold thorn devices@@N pressed in ochre — not the estate's administrative markers but the Hunt's jurisdiction claim, the equivalent of a posted notice. By day the road is simply a wide, clear path. At night the marks in the bark glow faintly, and the path becomes something that has specific rules about who may travel it.
- Exits: `south` → 486 (Zone III: Memorial Lane), `north` → 563 (Antler Marker Crossing)

**563 — The Antler Marker Crossing**
`room_flags: dark no_recall` | `sector: forest`
A crossroads where the Hunt's main patrol road is intersected by a secondary track running east-west — the Hunt's lateral route that allows it to cover the burial field margins and the gravewood's southern fringe in a single circuit. The crossing is marked by four antler installations: genuine antlers driven into the ground at the road corners, their tines oriented outward as territorial indicators. The antler-crown motif repeats: the Wild Hunt Master's authority, expressed in the decorative vocabulary of the pre-estate ridge-clan's hunting traditions. The Hunt has claimed those traditions as its own.
- Exits: `south` → 562, `north` → 564 (Shadow Run), `east` → 570 (Return Gate), `west` → 569 (Quarry Cache)

**564 — The Shadow Run**
`room_flags: dark no_recall` | `sector: forest`
A straight stretch of the Hunt road where the substrate's management of the path is most apparent: the trees on either side have had their canopy closed over the road, creating a tunnel effect, and the substrate has cleared the road surface to a smoothness that amplifies the sound of hooves — or would, if the substrate horses made sound. The Shadow Run's purpose is speed; the substrate has optimized this section for the Hunt's movement rate. During the Hunt's return from external patrols, this is where the riders' pace increases to the rate that witnesses describe as "arriving before the horn sound does."
- Exits: `south` → 563, `north` → 565 (Frost-Print Trail)

**565 — The Frost-Print Trail**
`room_flags: dark no_recall cold` | `sector: forest`
The section of the Hunt route where the shadow hounds' frost-prints are densest — the ground here retains the frost impressions of their passage even in daylight, in the pattern of a pack running in loose formation rather than in pairs. The frost-prints are notable for their detail: each print shows claw extension, pad distribution, and gait asymmetry consistent with a specific individual hound, not a template. The pack has individuals. The substrate knows which one left each print. The cold here is not atmospheric — it is localized to the print-bearing ground, radiating upward from the frost impressions rather than downward from the air.
- Exits: `south` → 564, `north` → 566 (Calling Beacon)

**566 — The Calling Beacon**
`room_flags: dark no_recall` | `sector: forest`
The point on the Hunt's road where the Hunt Master sounds the black shadow-bone horn before departing for external patrols. The beacon is not a structure but a position: a specific point where the road widens into a small clearing and where the root network is directly connected to the cave system's harmonic channel — meaning the horn's tone travels through the substrate rather than through air, arriving at its destinations before the Hunt itself does. The ground at the clearing's center is permanently frost-marked from the substrate horses' departures, their standing prints preserved in the same way as the hound prints to the south.
- Exits: `south` → 565, `north` → 567 (Hunt Dispatch Point)

**567 — The Hunt Dispatch Point** ★ (Boss room: Wild Hunt Master — `no_mob` flag)
`room_flags: dark no_recall no_mob` | `sector: forest`
The Wild Hunt Master's operational headquarters — the point from which the Hunt departs and to which it returns. The Master is here when not on patrol: mounted on his substrate horse, the antler crown's root-material tines oriented outward to receive the root network's signals, his face painted with sigils that match the binding's inscription language. He is not at rest. The antlers function as continuous receiving apparatus; he is always processing the estate's territorial status. The Hunt Dispatch connects north to the routes that lead beyond the estate's fixed perimeter — exits that go into the broader woodland and return through the same point. The Hunt's jurisdiction extends as far as its riders travel. The Master's authority is the binding's will expressed as velocity.
- Exits: `south` → 566 (Calling Beacon), `west` → 571 (Night Approach Road)
- Boss: The Wild Hunt Master (mob 482) — sentinel, boss, night_only, no_mob room, spec_cast_undead

**568 — The Birch-Bark Road**
`room_flags: dark no_recall` | `sector: forest`
The Hunt's primary patrol corridor beyond the estate's inner zone — a road bordered by white birch trees that the substrate has grown into the Hunt's route as navigation markers. The birch-bark coloring mirrors the substrate horses' coloring; in the dark, the road and its riders become the same pale against the surrounding forest. Sound does not carry here in the normal way — not because the room is quiet, but because the substrate routes ambient sound away from this road, keeping the Hunt's approach silent. The only sound that travels here correctly is the horn's substrate-transmission, which arrives through the ground rather than through air.
- Exits: `east` → 567 (Hunt Dispatch Point), `west` → 571 (Night Approach Road)

**569 — The Quarry Cache**
`room_flags: dark no_recall` | `sector: forest`
Where the Hunt brings material it retrieves from beyond the estate's perimeter — remains, artifacts, or occasionally persons the substrate has determined must be returned. The cache is a natural hollow at the road's western margin, its interior fitted with the same iron hooks and manifest-stands as the estate's surface processing systems. Items brought back by the Hunt are deposited here for processing by the bone weavers or assessment by the Root Covenant, depending on the material's nature. The cache currently holds several bundles of the pale, anomalously cold material that the Hunt retrieves from routes near the grave tunnel hinge — material that the bone weavers assess as substrate-connected but of external origin.
- Exits: `east` → 563 (Antler Marker Crossing)

**570 — The Return Gate**
`room_flags: dark no_recall` | `sector: forest`
The eastern arm of the Hunt's lateral route — the point where returning patrols re-enter the estate's inner perimeter after completing their external circuit. A formal thorn-growth arch has developed at this point, biological but architecturally deliberate, framing the transition between external patrol territory and the estate's defined interior. The arch's thorn growth is oriented inward — the same posture as the south thorn-hedge wall — suggesting the substrate considers the return passage more critical than the departure.
- Exits: `west` → 563 (Antler Marker Crossing), `south` → 581 (Zone IX: Thorn Knight Guardpost)

**571 — The Night Approach Road**
`room_flags: dark no_recall` | `sector: forest`
The outermost reach of the Hunt's established patrol territory — a road that leads into the greater woodland beyond Thornwood's fixed perimeter and connects back to the gravewood's southern edge via the Night Ride Path. This is the point where the estate's jurisdiction becomes argument rather than enforcement: the Hunt claims it as territory, the Great Northern Forest wardens' route charts mark it as avoided, and the Shadowmere scouts' maps simply note "do not stop." The road here shows no substrate management — no cleared surface, no marked trees — because the substrate's network does not extend this far with sufficient resolution. The Hunt operates here on authority alone.
- Exits: `east` → 567 (Hunt Dispatch Point), `north` → 506 (Zone IV: Night Ride Path), `west` → 568 (Birch-Bark Road)

---

### Zone IX: Perimeter and Auxiliary Rooms (572–590) — 19 Rooms

Perimeter wall walks, timber stands, cult assembly grounds, archive alcoves, the Harren family crypt, the Hunt stable court, and remaining lore-carrying auxiliary spaces. All four logics are present here at varying emphasis.

**572 — The South Wall Walk**
`room_flags: dark no_recall` | `sector: city`
The walkway atop the estate's southern perimeter wall, accessible from the south thorn-hedge wall's exterior face. The wall-walk is wide enough for two people side by side and still carries the iron bracket infrastructure for patrol lanterns, all empty. From the wall-top, the southern burial field margins are visible to the east and the estate's formal gardens to the west. The wall-walk's stone carries the thorn-growth's inward pressure visibly: thorn branches have breached the wall's inner face and grown along the parapet, oriented toward the forecourt below.
- Exits: `east` → 447 (South Thorn-Hedge Wall), `south` → 581 (Thorn Knight Guardpost)

**573 — The North Wall Walk**
`room_flags: dark no_recall` | `sector: city`
The northern perimeter wall-walk, mirroring the southern run but with a view over the approach road and the outer thorn-hedge to the north. The estate's territorial boundary is clearest from here: the managed thorn-hedge growth outside the wall marks the outermost extent of Harren administrative claims; the root-fiber threading through the wall's stone marks the substrate's claims, which extend farther. The two boundaries do not coincide.
- Exits: `west` → 446 (North Thorn-Hedge Wall), `east` → 574 (Outer Gate Station North)

**574 — The Outer Gate Station (North)**
`room_flags: dark no_recall` | `sector: city`
A secondary gate in the estate's northern perimeter wall, used historically by the managed timber operations to move lumber from the managed stands without routing through the main forecourt. The gate is iron-barred and intact, though the locking mechanism is now supplemented by thorn growth that functions as both seal and indicator — the growth pattern changes when the gate is opened, and the substrate responds accordingly. A carved stone above the gate reads: *Timber Operations Gate — Estate Authorization Required.*
- Exits: `west` → 573, `north` → 575 (Timber Stands Managed), `east` → 586 (Hunt Stable Court, via exterior path)

**575 — The Timber Stands (Managed)**
`room_flags: dark no_recall` | `sector: forest`
The estate's managed thornwood timber stands — the trees that represent the Harren estate's original commercial forestry operation. The trees here are maintained: spaced for growth, their lower branches pruned to encourage trunk development, the understory cleared for access. This management is not by human hands; the substrate has continued the groundskeepers' patterns, pruning and spacing according to whatever the estate's forestry records specified. The timber is still harvestable. No one harvests it. The Violet Eclipse Cathedral's procurement agents know where these stands are and what they are worth. They do not come here personally.
- Exits: `south` → 574, `west` → 463 (Stable Yard), `north` → 576 (Deep Timber Stands)

**576 — The Deep Timber Stands**
`room_flags: dark no_recall` | `sector: forest`
The interior timber stands, where the trees are older and the "deep harvest" grade material is found — the cathedral-grade timber grown near burial sites, which the Violet Eclipse Cathedral's procurement specifications designate as more durable and more dense than surface harvest stock. The substrate's influence on these trees is visible in the bark: the deep-stand thornwood has the same darkening and bark-pattern shift visible in the estate orchard, and the root system surfaces at intervals with the crystalline texture that indicates crystal-substrate chemistry integration. The lumber from these trees is genuinely anomalous. It is also genuinely contaminated, in whatever sense a piece of wood can carry history.
- Exits: `south` → 575, `east` → 588 (Riven Road)

**577 — The Cult Assembly Ground**
`room_flags: dark no_recall` | `sector: forest`
The Root Covenant's primary gathering space for organized collective rites — large enough for the entire living cult population to assemble, cleared of undergrowth by the substrate's management, and bordered by the hedge labyrinth's exterior growth so the site has a natural enclosure. The ground surface carries burn-marks from past assembly fires and the pressure-indentations of many feet in regular pattern — the congregation's standing positions during rite-work are encoded in the ground. A central platform of flat stones provides the presiding elder's position.
- Exits: `south` → 468 (Zone II: Hedge Labyrinth Entry), `east` → 578 (Covenant Offering Site East), `north` → 579 (Covenant Offering Site West)

**578 — The Covenant Offering Site (East)**
`room_flags: dark no_recall` | `sector: forest`
The eastern blood offering platform — a second root-contact rite site supplementing the gravewood's primary offering platform, positioned to serve the burial field area's cult activities. The platform here is more recently installed than the gravewood site: the stone surface is less stained, the surrounding thorn growth is less established, the substrate response is perceptibly less immediate. The cultists who work this platform are maintaining a station rather than a tradition — extending the cult's ritual operations infrastructure into territory that used to be administered by the estate's burial processing system rather than by the covenant.
- Exits: `west` → 577 (Cult Assembly Ground), `south` → 475 (Zone III: Rapid Burial Fields North), `north` → 503 (Zone IV: Blood Offering Platform)

**579 — The Covenant Offering Site (West)**
`room_flags: dark no_recall` | `sector: forest`
The western offering platform, positioned near the rose garden ruins — the station that serves the manor's western wing and the garden area's cult operations. The platform here is integrated with the rose garden's bone-arrangement installation, the two elements functioning as a composite site: bone-filing and blood offering in proximity, the substrate receiving both categories of input at the same contact point. The thorn growth around this platform has the most deliberate directionality of any surface-level thorn growth in the area — every stem is oriented toward the platform's center with a precision that suggests the substrate has been optimizing this site for longer than the cult has been using it.
- Exits: `east` → 577, `north` → 475 (Zone III: Rapid Burial Fields North), `south` → 469 (Zone II: Rose Garden Ruins)

**580 — The Bone-Path Marker Row**
`room_flags: dark no_recall` | `sector: field`
A row of bone markers running north from the deferred-name register stakes — the Root Covenant's territorial demarcation of the burial fields, distinguishing the zones under cult operational management from the zones under estate administrative management (as the dead household staff conceives it). The markers are femurs driven upright into the earth at regular intervals, each bearing a pressed circle-glyph: the cult's filing notation for a registered rite-claim on the ground. This is the cult's version of the estate's boundary-marks — same function, different material and language.
- Exits: `south` → 474 (Zone III: Deferred-Name Registers), `north` → 581 (Thorn Knight Guardpost)

**581 — The Thorn Knight Guardpost**
`room_flags: dark no_recall no_mob` | `sector: city`
A formal guardpost at the estate's outer perimeter crossing-point — a stone structure fitted with the iron hardware of the Thorn Knight's patrol infrastructure: weapon-rack, lantern-mount (empty), challenge-protocol plaque. The plaque reads: *All parties must present authorization of passage. Unauthorized entry subject to estate processing.* The phrase "estate processing" has acquired a meaning that its original author did not intend. The post is staffed. The Thorn Knight here enforces the challenge-protocol as though the estate's charter were in force, as though the challenge had legal standing, as though the word "processing" meant what the administrative manuals said it meant.
- Exits: `north` → 580 (Bone-Path Marker Row), `south` → 572 (South Wall Walk), `east` → 570 (Zone VIII: Return Gate)

**582 — The Wraith Passage**
`room_flags: dark indoors no_recall` | `sector: inside`
A service corridor connecting the bone processing station to the mire's eastern margin — originally a transit route for moving material between the two zones without crossing the burial field's formal approach roads. Estate Wraiths use this passage to perform the empty household routines they inherited from their living functions: carrying nothing in the pattern of carrying something, moving with the deliberate gait of a worker completing a task, stopping at points that correspond to the original workflow's stations and pausing in the same way that a worker checking a task list would pause. They are performing a role that has no remaining content. The performance continues.
- Exits: `west` → 477 (Zone III: Bone Processing Station), `east` → 522 (Zone V: Substrate Water Sink)

**583 — The Forgotten Archive Alcove**
`room_flags: dark indoors no_recall` | `sector: inside`
A storage alcove within the manor's service wing, discovered mid-process during a Root Covenant archival recovery effort and now accessible but not fully catalogued. The substrate's preservation effect has kept the documents here in better condition than their age warrants: papers from the estate's ossuary expansion period, in good legible condition, documenting the expansion's labor arrangements, the sources of that labor, and the passage records for the grave tunnel during the peak throughput years. Three specific documents in the alcove are marked with a @@ypale-gold seal@@N that matches the Violet Eclipse Cathedral's procurement authorization stamp — connecting the cathedral to the estate's operations in the period when both institutions preferred not to have the connection documented.
- Exits: `north` → 461 (Zone II: Service Yard), `east` → 584 (Tunnel Transfer Records Room)

**584 — The Tunnel Transfer Records Room**
`room_flags: dark indoors no_recall` | `sector: inside`
Where the paperwork from the grave tunnel's transit operations was held — the records of every funeral convoy, every sealed warrant transfer, every private cargo shipment that passed through the estate's tunnel access point. The records are organized by year and convoy type, cross-referenced to both the surface intake records and the ossuary vault assignments. A specific section is marked in the estate clerk's notation as "restricted — by Harren family order" — a designation that, under the estate's charter, meant external auditors could not request those files. The restriction applies to a period corresponding precisely to the sealed warrant traffic documented in Midgaard's Spirebound Conclave transfer records. The Reckoning faction has been trying to access this room for twenty years. They have not been successful.
- Exits: `west` → 583, `north` → 455 (Zone II: Butler's Pantry, via connecting passage)

**585 — The Harren Family Crypt (Surface)**
`room_flags: dark no_recall cold` | `sector: hills`
The above-ground burial vault of the Harren family itself — the mausoleum where the family's own dead were interred, distinct from the estate's commercial burial fields. The crypt's stonework is the estate's finest construction: dark stone with precise joinery, the thorn-and-keyhole device carved on every surface including the interior walls, and a floor inscription in the estate's administrative script that names each interred family member with full title and service dates. The Harren dead are all here — except Lord Harren, who is not interred anywhere because the substrate did not permit him to become a body to be interred. The @@acold@@N in this crypt is different from the cave system's crystal cold: it is the cold of a place where the substrate's attention is fixed and has been fixed for a long time, looking for something specific that the crypt does not contain.
- Exits: `west` → 486 (Zone III: Memorial Lane), `south` → 588 (Riven Road)

**586 — The Wild Hunt Stable Court**
`room_flags: dark no_recall` | `sector: city`
The court where the substrate horses are held between patrols — a formal stable yard adjacent to the estate's original stable block, repurposed for the Hunt's substrate-constructed mounts. The horses here are still: standing in the same absolute stillness as the stable block's occupants, birch-bark pale, berry-red eyes unreflective. The court's cobblestones carry frost-marks from the horses' rest positions in the same pattern as their riding frost-prints — the substrate maintains the thermal anomaly continuously. The court connects to the Hunt's patrol route infrastructure and to the main stable block at the service yard.
- Exits: `west` → 463 (Zone II: Stable Yard), `north` → 574 (Outer Gate Station North), `east` → 567 (Zone VIII: Hunt Dispatch Point)

**587 — The Canopy Seal**
`room_flags: dark no_recall` | `sector: forest`
A space where the estate's canopy has completed its closure — the thornwood growth overhead has fused into a solid biological mass with no gaps, producing an indoor-like darkness under open sky. The substrate cleared specific openings through the canopy at positions that correspond to the binding's access points; those openings allow narrow columns of light or sky-visibility directly above ritual sites. The canopy seal is the baseline: total occlusion, complete closure, the forest's assertion that it has replaced the sky with something it controls.
- Exits: `south` → 575 (Timber Stands Managed), `east` → 589 (Substrate Seep Glade), `west` → 577 (Cult Assembly Ground)

**588 — The Riven Road**
`room_flags: dark no_recall` | `sector: forest`
A former service path that has been literally split by root eruption — a section of cobblestone road heaved upward by root pressure in a single event, leaving the path bisected by a raised root-mass running perpendicular to the road's direction. The root eruption's date is unknown but the scale suggests it was not gradual; the stone displacement patterns indicate sudden pressure release. The road is still navigable by climbing over the root-mass at the crossing point. The root-mass at the eruption site carries a substrate cold that is sharper than ambient, suggesting the eruption reached through to the crystal layer below.
- Exits: `north` → 576 (Deep Timber Stands), `south` → 585 (Harren Family Crypt)

**589 — The Substrate Seep Glade**
`room_flags: dark no_recall cold` | `sector: forest`
A clearing where the substrate's surface expression is strongest above the ossuary layer — not the crystal seep of the deep gravewood, but a broader, more diffuse effect: soil temperature several degrees below ambient across the glade's entire surface, the corpse-lily growth that indicates high burial density below, and the root-mass pattern that corresponds to the resonance chamber's geometry directly below. A shaman standing in this glade would know they were standing above the ossuary's most important structural point. The glade is also the space where the substrate's bioluminescent output is visible in daylight — the cold blue of the cave system's fungal network seeping up through the soil to appear as a faint glow at ground level when conditions are right.
- Exits: `west` → 587 (Canopy Seal), `south` → 534 (Zone VI: Resonance Chamber — vertical connection; descending through a natural ground opening)

**590 — The Bone-Gate Terminus**
`room_flags: dark no_recall` | `sector: forest`
The easternmost point of the estate's accessible territory — a formal gateway of bone-woven construction rather than iron, where the substrate's bone-craft has produced an arch of assembled long bones integrated with root-fiber into a structure as load-bearing as any masonry. The gate is the estate's final territorial marker at its eastern boundary, beyond which the mire's margin and the approach routes connecting to the broader grave tunnel network begin. The bone-arch is not decorated: it is structural, functional, and legible as such. *This land's terms continue until this arch. Beyond this point, you are no longer being processed. You were never not being processed.*
- Exits: `west` → 582 (Zone IX: Wraith Passage), `north` → 518 (Zone V: Broken Causeway)

---

## Mobile Redesign Plan (441–484 — 44 Mobs)

All mobs must carry `stay_area` (`64`). Aggressive mobs initiate combat on sight. Sentinel mobs do not wander. Boss mobs are flagged `sentinel` + `boss` and placed only in `no_mob` rooms. Solo mobs are flagged `solo` (`33554432`). Undead mobs carry `undead` (`16384`). Mobs without corporeal form carry `no_body` (`65536`) and `no_blood` (`268435456`) where appropriate.

Level tier guide: Entry/Approach 50–55 | Estate Surface 55–65 | Burial Fields 60–68 | Gravewood 62–70 | Mire 62–70 | Ossuary 65–72 | Cave 68–80 | Named Elites 70–80.

---

### Tier 1 — Entry and Approach (441–445) | Level 50–55

**441 — grave tunnel toll-shade**
Level 50 | `act: is_npc aggressive stay_area undead no_body` | `aff: infrared`
Alignment: -500 | Sex: neutral
`hp_mod: 0 | ac_mod: 0 | hr_mod: 2 | dr_mod: 2`
A residual presence from the tunnel's pre-estate history — not a Harren estate product but a shadow accumulated in the transit node over generations of funeral convoy traffic. Attacks immediately. No physical form beyond a cold displacement of air and a sound like chain dragging on stone.
- Placement: Room 441 (Grave Tunnel Hinge) | Limit 3

**442 — estate gate wraith**
Level 52 | `act: is_npc sentinel stay_area undead no_body aggressive` | `aff: infrared detect_invis`
Alignment: -600 | Sex: neutral
`hp_mod: 0 | ac_mod: -20 | hr_mod: 3 | dr_mod: 3`
The Gatewarden, risen and retained at post. Challenges entry with estate protocol before attacking. Still invokes the Harren Charter's passage-authorization requirement. The challenge is brief and procedurally correct and means nothing.
- Placement: Room 444 (Gatewarden's Lodge) | Limit 1
- `spec_guard` (enforces the challenge-before-attack pattern)

**443 — thorn hedge sentinel**
Level 53 | `act: is_npc aggressive stay_area undead` | `aff: infrared`
Alignment: -400 | Sex: neutral
`hp_mod: 0 | ac_mod: -10 | hr_mod: 3 | dr_mod: 2`
A perimeter guard whose body has been partially incorporated into the estate's thorn-hedge infrastructure — thorn growth has merged with joints and armor, making the sentinel visually part of the hedge until it moves. Responds to movement through the perimeter zone.
- Placement: Rooms 446, 447 (Hedge Walls) | Limit 2 each

**444 — corpse-cart revenant**
Level 51 | `act: is_npc aggressive stay_area undead` | `aff: infrared`
Alignment: -350 | Sex: neutral
`hp_mod: 0 | ac_mod: 0 | hr_mod: 2 | dr_mod: 3`
A former cart-driver, risen still in the posture of handling a vehicle that no longer exists. Moves with the deliberate, route-following gait of a worker completing a fixed task. Attacks anything that interrupts the route.
- Placement: Room 445 (Forecourt Approach) | Limit 2

**445 — approach road phantom**
Level 54 | `act: is_npc aggressive stay_area undead no_body night_only` | `aff: infrared sneak`
Alignment: -500 | Sex: neutral
`hp_mod: 0 | ac_mod: -15 | hr_mod: 4 | dr_mod: 3`
Visible only at night, this presence patrols the lantern alley and approach road. During the day, nothing is here. At night, the dead lanterns cast no light but something moves between them. Aggressive on contact.
- Placement: Room 448 (Lantern Alley) | Limit 2 | `night_only`

---

### Tier 2 — Estate Surface (446–454) | Level 55–65

**446 — estate household wraith**
Level 57 | `act: is_npc sentinel stay_area undead no_body aggressive` | `aff: infrared invisible`
Alignment: -500 | Sex: neutral
`hp_mod: 0 | ac_mod: -20 | hr_mod: 3 | dr_mod: 4`
A domestic servant, risen to perform empty household duties. Carries nothing. Moves between service points on a fixed route. Attacks intruders who disrupt the routine. Refers to rooms by their original function names.
- Placement: Rooms 451, 459, 461 (interior manor and service yard) | Limit 1 each

**447 — thorn knight**
Level 60 | `act: is_npc aggressive stay_area undead` | `aff: infrared`
Alignment: -600 | Sex: neutral
`hp_mod: 0 | ac_mod: -40 | hr_mod: 5 | dr_mod: 5`
A former estate guard, risen as the Thorn Knight standard pattern: armored, briar growth at joints and crest, movement disciplined and tactical. Challenges before attacking (jurisdictional form, not warning). Fights like a trained soldier.
- `! 0 0 0 7 3 0 0`
- Placement: Rooms 448, 449, 450, 457 | Limit 1–2 per room

**448 — blackened thorn knight**
Level 64 | `act: is_npc aggressive stay_area undead solo` | `aff: infrared`
Alignment: -178 | Sex: neutral
`hp_mod: 0 | ac_mod: -60 | hr_mod: 7 | dr_mod: 6`
The fully fused variant — original armor consumed by biological growth that has preserved the shape while replacing the material. Gait is wrong in a specific way: the briar-grown joint replacements produce movement that is simultaneously more precise and less human. Much harder than the standard thorn knight.
- `! 0 0 0 7 318 0 0` (2_attack + nodisarm + notrip)
- Placement: Rooms 453, 454 | Limit 1 each

**449 — manor steward revenant**
Level 58 | `act: is_npc sentinel stay_area undead aggressive` | `aff: infrared detect_invis`
Alignment: -550 | Sex: neutral
`hp_mod: 0 | ac_mod: -25 | hr_mod: 4 | dr_mod: 4`
The household's administrative lead, risen to manage the dead staff with the same authority it held in life. Invokes precedence rules and household hierarchy. Attacks when its administrative authority is challenged — which trespass constitutes by definition.
- Placement: Room 451 (Great Hall) | Limit 1

**450 — chapel cultist**
Level 56 | `act: is_npc stay_area` | `aff: detect_magic`
Alignment: -35 | Sex: neutral
`hp_mod: 0 | ac_mod: 0 | hr_mod: 2 | dr_mod: 2`
A living Root Covenant practitioner, engaged in chapel rite-work. Not aggressive — the cult does not attack without cause — but will defend itself. Recites transfer formulas: prayer-clauses that sound like contract terms and contract terms that sound like prayers.
- `! 0 0 0 7 0 4 0` (mag_missile cast)
- Placement: Rooms 457, 458 | Limit 2 total

**451 — covenant elder**
Level 63 | `act: is_npc sentinel stay_area solo` | `aff: detect_magic detect_invis`
Alignment: -300 | Sex: neutral
`hp_mod: 0 | ac_mod: -30 | hr_mod: 4 | dr_mod: 4`
A senior Root Covenant practitioner with decades of substrate exposure. Not aggressive, but not safe — will attack if the assembly ground or ritual sites are disrupted. Thorn-pattern tattoos have developed real thorn growth inward through the skin. Voice carries harmonic undertones.
- `! 0 0 0 7 0 844 4` (flamestrike + faerie_fire cast; cure_serious def)
- Placement: Room 577 (Cult Assembly Ground) | Limit 1
- `spec_cast_cleric`

**452 — covenant bone-reader**
Level 60 | `act: is_npc aggressive stay_area` | `aff: detect_magic`
Alignment: -400 | Sex: neutral
`hp_mod: 0 | ac_mod: -20 | hr_mod: 3 | dr_mod: 5`
A Root Covenant specialist in substrate-reading through bone-resonance assessment. Found in the burial fields and ossuary approaches. Aggressive when the bone-reading work is interrupted. Carries bone-reading tools as improvised weapons.
- `! 0 0 0 7 0 512 0` (faerie_fire cast)
- Placement: Rooms 477, 525 | Limit 1 each

**453 — estate cook specter**
Level 55 | `act: is_npc sentinel stay_area undead no_body` | `aff: infrared`
Alignment: -400 | Sex: neutral
`hp_mod: 0 | ac_mod: -10 | hr_mod: 2 | dr_mod: 3`
The kitchen specter, performing the motions of meal preparation with nothing in its hands. Not immediately aggressive — it performs, it does not respond, until a threshold of disruption is crossed. Then it responds with the particular fury of something that has been interrupted mid-task for eternity and will not tolerate it again.
- Placement: Room 459 (Kitchen) | Limit 1

**454 — stable revenant**
Level 57 | `act: is_npc aggressive stay_area undead` | `aff: infrared`
Alignment: -500 | Sex: neutral
`hp_mod: 0 | ac_mod: -15 | hr_mod: 4 | dr_mod: 4`
The former stable hand, risen and still working. Attacks intruders in the stable area with the tools of stable work: a hoof pick, a stable fork. The substrate horses it tends do not react to combat in their stall. This is either because they have no threat-response programming or because the stable hand being attacked is not their concern.
- Placement: Room 462 (Stable Block) | Limit 1

---

### Tier 3 — Burial Fields and Charnel Verge (455–459) | Level 60–68

**455 — deferred-name revenant**
Level 61 | `act: is_npc aggressive stay_area undead` | `aff: infrared`
Alignment: -128 | Sex: neutral
`hp_mod: 0 | ac_mod: -30 | hr_mod: 5 | dr_mod: 5`
One of the deferred-name dead who was never named — interred under a case number, the naming rite never paid, the fee never collected. Rose with the Reversal carrying the compound anger of institutional abandonment. Attacks without challenge — the jurisdictional form does not apply to those the jurisdiction failed.
- Placement: Rooms 474, 475, 476 | Limit 2 per room

**456 — bone processing laborer**
Level 63 | `act: is_npc aggressive stay_area undead` | `aff: infrared`
Alignment: -550 | Sex: neutral
`hp_mod: 0 | ac_mod: -30 | hr_mod: 5 | dr_mod: 6`
A former grave laborer from the ossuary expansion era, evaluated on volume not dignity, risen still performing the processing routine. Attacks anything in the processing zone that is not material to be processed — the distinction between intruder and intake has collapsed.
- Placement: Rooms 473, 477 | Limit 2 per room

**457 — charnel verge watcher**
Level 65 | `act: is_npc aggressive stay_area undead solo` | `aff: infrared detect_invis`
Alignment: -178 | Sex: neutral
`hp_mod: 0 | ac_mod: -50 | hr_mod: 6 | dr_mod: 6`
A boundary-enforcer stationed at the charnel verge gate — the equivalent of a Thorn Knight for the burial field zone rather than the manor perimeter. Challenges entry to the burial fields with the same procedural language as the gate wraith, and attacks with comparable discipline.
- `! 0 0 0 7 99 0 0` (2_attack + nodisarm + notrip + punch)
- Placement: Room 471 (Charnel Verge Gate) | Limit 1
- `spec_undead`

**458 — exhumation crow**
Level 60 | `act: is_npc aggressive stay_area scavenger` | `aff: flying`
Alignment: -35 | Sex: neutral
`hp_mod: 0 | ac_mod: -20 | hr_mod: 6 | dr_mod: 4`
A corvid adapted to the burial field's substrate-rich environment — larger than a natural crow, with bark-textured feathers and the same berry-red eye color as the substrate horses. Scavenges the burial field's organic output and attacks intruders opportunistically. Travels in loose groups.
- Placement: Rooms 472, 475, 476, 480 | Limit 3 per room

**459 — ossuary monk**
Level 66 | `act: is_npc aggressive stay_area undead` | `aff: infrared`
Alignment: -600 | Sex: neutral
`hp_mod: 0 | ac_mod: -40 | hr_mod: 5 | dr_mod: 6`
A former religious operative from the charnel chapel, risen in the burial field. Recites the charnel chapel's inscription as attack-accompanying speech — the prayer-debt formula — as though naming the offense in the substrate's language might satisfy the filing requirement the attack is making.
- `! 0 0 0 7 0 840 4` (flamestrike cast; cure_light def)
- Placement: Rooms 483, 484 | Limit 1 each

---

### Tier 4 — Older Gravewood and Mire (460–470) | Level 62–70

**460 — ridge-clan shade**
Level 63 | `act: is_npc aggressive stay_area undead no_body` | `aff: infrared`
Alignment: -400 | Sex: neutral
`hp_mod: 0 | ac_mod: -30 | hr_mod: 5 | dr_mod: 5`
Pre-estate burial dead, disturbed by the Harren estate's expansion into their burial ground. The substrate has risen them, but not in the estate's administrative mold — they are older, different in their behavioral logic, attacking with the territorial response of ancestors defending their ground rather than guards enforcing a charter.
- `| 0 0 0 0 0 82 2` (strong: shadow; suscept: physical)
- Placement: Rooms 490, 491, 492 | Limit 2 per room

**461 — thornwood shaman**
Level 67 | `act: is_npc sentinel stay_area solo` | `aff: detect_magic detect_hidden`
Alignment: 0 | Sex: neutral
`hp_mod: 0 | ac_mod: -40 | hr_mod: 4 | dr_mod: 5`
A living shaman who negotiates with the substrate rather than serves it. Not aggressive by default — will not attack unless threatened or the boundary stones are disturbed. Speaks in precise, non-absolute language about the substrate's current state. Provides the gravewood's only non-hostile interaction point.
- `! 0 0 0 7 0 4098 12` (mag_missile + flamestrike cast; cure_serious + heal def)
- Placement: Room 502 (Shaman's Hollow) | Limit 1
- `spec_cast_mage`

**462 — root weave cultist**
Level 64 | `act: is_npc aggressive stay_area` | `aff: detect_magic`
Alignment: -500 | Sex: neutral
`hp_mod: 0 | ac_mod: -25 | hr_mod: 4 | dr_mod: 5`
A junior cultist who has spent too long in the gravewood and has drifted from the organized cult's operational discipline. The thorn-pattern tattoos have begun developing real growth inward, and the cultist's threat assessment has simplified to substrate-protective aggression.
- `! 0 0 0 7 0 512 0` (faerie_fire cast)
- Placement: Rooms 493, 497, 503 | Limit 2 per room

**463 — substrate-adapted root-crawler**
Level 65 | `act: is_npc aggressive stay_area no_body no_mind` | `aff: infrared`
Alignment: -278 | Sex: neutral
`hp_mod: 0 | ac_mod: -35 | hr_mod: 6 | dr_mod: 6`
Something that was not always this. The substrate's integration process produced a result here that exceeds the original template — what it began as is no longer guessable from what it has become. Moves through root-mass as readily as through open space. No intelligence perceptible beyond territorial aggression.
- `| 0 0 32 0 0 2 0` (strong: earth; suscept: fire)
- Placement: Rooms 494, 501 | Limit 1 each

**464 — face-down risen**
Level 64 | `act: is_npc aggressive stay_area undead no_flee` | `aff: infrared`
Alignment: -286 | Sex: neutral
`hp_mod: 0 | ac_mod: -35 | hr_mod: 6 | dr_mod: 7`
An oathbreaker burial from the pre-estate ridge-clan community, risen wrong: face-down posture preserved in animation, movement a lurching inversion of normal locomotion. Attacks with the fury of something that has been pressed into the earth face-first for centuries against its will. Will not flee.
- `| 0 0 0 0 0 82 0` (strong: shadow)
- Placement: Rooms 491, 500 | Limit 1 each

**465 — thorn ring guardian**
Level 66 | `act: is_npc aggressive sentinel stay_area` | `aff: infrared detect_invis`
Alignment: -600 | Sex: neutral
`hp_mod: 0 | ac_mod: -45 | hr_mod: 6 | dr_mod: 6`
Assigned to protect the old thorn rings. Does not roam. Attacks any approach to the thorn-ring interiors. The guardian's nature is unclear — it may be an Estate Dead variant, a substrate construct, or something the substrate assembled specifically for ring-protection that has no other category.
- Placement: Rooms 492, 481 (Thorn-Ringed Plots) | Limit 1 each

**466 — mire lurker**
Level 65 | `act: is_npc aggressive stay_area no_blood` | `aff: infrared`
Alignment: -500 | Sex: neutral
`hp_mod: 0 | ac_mod: -40 | hr_mod: 7 | dr_mod: 6`
Substrate-adapted beings whose skin has the color and texture of deep mud. Silent in water, slow on land. Effectively invisible in the mire's fog. Attack with a grip-and-pull technique that exploits the unstable mire terrain.
- `| 0 0 64 0 0 512 128` (strong: water; suscept: poison, fire)
- Placement: Rooms 508, 509, 512, 513 | Limit 2 per room

**467 — deep mire shade**
Level 67 | `act: is_npc aggressive stay_area undead no_body no_blood` | `aff: infrared`
Alignment: -178 | Sex: neutral
`hp_mod: 0 | ac_mod: -50 | hr_mod: 6 | dr_mod: 7`
Older than the mire itself — cold presences that may be substrate output expressed as localized dread. Rise from the water and affect those who pass through the mire with a psychological cold as much as thermal. Effectively impossible to see until engaged.
- `| 0 0 0 0 0 82 2` (strong: shadow; suscept: physical)
- Placement: Rooms 510, 511, 519 | Limit 1 per room

**468 — sulfur-bloated corpse**
Level 62 | `act: is_npc aggressive stay_area undead no_blood` | `aff: infrared`
Alignment: -400 | Sex: neutral
`hp_mod: 0 | ac_mod: 0 | hr_mod: 3 | dr_mod: 5`
The terminal product of the alchemical runoff channel's effect on interred remains: a body saturated with the mire's chemical deposits to the point of bloating, risen by the substrate but chemically altered enough that contact risks secondary poisoning.
- `| 0 0 0 0 0 512 128` (strong: poison; suscept: fire)
- `spec_poison`
- Placement: Rooms 515, 521 | Limit 2 per room

**469 — fog shrine attendant**
Level 68 | `act: is_npc aggressive stay_area sentinel solo` | `aff: infrared detect_magic`
Alignment: -600 | Sex: neutral
`hp_mod: 0 | ac_mod: -55 | hr_mod: 7 | dr_mod: 7`
Something that tends the fog shrine — not a cultist, not a shade, not a construct in any recognized category. Present at the shrine island. What it was before it became this is not answerable from its current appearance. Attacks those who approach the shrine platform.
- `! 0 0 0 7 0 4098 8` (mag_missile + flamestrike; cure_critic def)
- Placement: Room 514 (Fog Shrine) | Limit 1

**470 — substrate water-thing**
Level 66 | `act: is_npc aggressive stay_area no_mind no_blood no_body` | `aff: infrared`
Alignment: -278 | Sex: neutral
`hp_mod: 0 | ac_mod: -40 | hr_mod: 5 | dr_mod: 6`
An entity from the substrate's water table interface — water and crystal-compound given a minimal operational form by the substrate's processes. Has no discernible anatomy. Present near the black water pool and substrate water sink.
- `| 0 0 64 0 0 512 128` (strong: water; suscept: poison, fire)
- Placement: Rooms 511, 522 | Limit 1 each

---

### Tier 5 — Ossuary Undercroft (471–475) | Level 65–72

**471 — bone weaver journeyman**
Level 66 | `act: is_npc aggressive stay_area` | `aff: infrared`
Alignment: -400 | Sex: neutral
`hp_mod: 0 | ac_mod: -35 | hr_mod: 5 | dr_mod: 6`
A practitioner of bone craft at the journeyman level — skilled enough for sorting and basic construction, not yet authorized for high-grade resonance work. Describes combat in terms of joinery and structural failure rather than victory and defeat.
- Placement: Rooms 525, 526 | Limit 2 per room

**472 — bone weaver master**
Level 70 | `act: is_npc aggressive stay_area solo` | `aff: infrared detect_magic`
Alignment: -500 | Sex: neutral
`hp_mod: 0 | ac_mod: -65 | hr_mod: 8 | dr_mod: 8`
The senior bone weaver — responsible for high-grade work and construct assembly supervision. Fights with precise, economical violence, applying craft discipline to combat the way a master applies it to construction.
- `! 0 0 0 7 382 0 0` (2_attack + nodisarm + notrip + punch + headbutt)
- `l 100 496 0 0 0`
- `L 100 0 0 0 0`
- Placement: Room 529 (Bone-Weaver's Workshop) | Limit 1

**473 — marrow construct**
Level 68 | `act: is_npc aggressive stay_area undead no_blood no_body` | `aff: infrared`
Alignment: -600 | Sex: neutral
`hp_mod: 0 | ac_mod: -55 | hr_mod: 7 | dr_mod: 7`
A mid-scale bone construct — assembled from dozens of sources, fused at joints with substrate-mediated bonding. Was never alive as a unified being. Architecture that moves. Responds to substrate-level direction rather than vocal commands.
- `| 0 0 32 0 0 512 128` (strong: earth; suscept: poison, fire)
- Placement: Rooms 532, 538 | Limit 2 per room

**474 — marrow fiend**
Level 72 | `act: is_npc aggressive stay_area undead no_blood no_body solo` | `aff: infrared`
Alignment: -178 | Sex: neutral
`hp_mod: 0 | ac_mod: -80 | hr_mod: 9 | dr_mod: 9`
A large-scale construct, four meters in height, with excess limbs and articulated bone-plate coverage. The bone-weaver masters' finest operational product. Deployed from the marrow fiend vault when the binding requires physical enforcement at a scale beyond the estate dead.
- `! 0 0 0 7 382 0 0`
- `| 0 0 32 0 0 512 128`
- `l 100 497 0 0 0`
- `L 100 0 0 0 0`
- Placement: Room 539 (Marrow Fiend Vault) | Limit 2

**475 — ossuary elder**
Level 69 | `act: is_npc aggressive sentinel stay_area undead` | `aff: infrared detect_invis`
Alignment: -600 | Sex: neutral
`hp_mod: 0 | ac_mod: -60 | hr_mod: 7 | dr_mod: 7`
The former overseer of the ossuary system, risen as an administrative undead managing the vault operations. Challenges with ossuary protocol: "State your case-number. Unauthorized access to vault sections is subject to estate processing." Does not wait for an answer.
- `! 0 0 0 7 99 0 4` (2_attack + nodisarm + notrip + punch; cure_light def)
- Placement: Room 524 (Upper Undercroft Hall) | Limit 1

---

### Tier 6 — Cave System (476–479) | Level 68–75

**476 — crystal cave stalker**
Level 70 | `act: is_npc aggressive stay_area no_mind` | `aff: infrared`
Alignment: -278 | Sex: neutral
`hp_mod: 0 | ac_mod: -60 | hr_mod: 8 | dr_mod: 8`
A predatory presence that has developed in the cave system's crystal-rich environment — neither undead nor construct but a biological form shaped by long exposure to the substrate's chemistry. Crystalline texture along the skin's dorsal surfaces. Attacks with the efficiency of a predator rather than the purpose of a soldier.
- `| 0 0 32 0 0 512 0` (strong: earth; suscept: poison)
- Placement: Rooms 542, 543, 555 | Limit 2 per room

**477 — binding watcher**
Level 72 | `act: is_npc aggressive sentinel stay_area undead` | `aff: infrared detect_invis detect_magic`
Alignment: -178 | Sex: neutral
`hp_mod: 0 | ac_mod: -70 | hr_mod: 9 | dr_mod: 8`
Assigned to guard the cave system's binding-critical areas — the handprint cave, the secondary sigil floor, the consent inscription hall. Stationed; does not patrol. Attacks any approach to the protected surfaces with the absolute priority of something whose sole function is protection of a specific point.
- `| 0 0 0 0 0 82 0` (strong: shadow)
- Placement: Rooms 546, 552, 547 | Limit 1 each

**478 — substrate echo**
Level 71 | `act: is_npc aggressive stay_area undead no_body no_blood` | `aff: infrared`
Alignment: -286 | Sex: neutral
`hp_mod: 0 | ac_mod: -65 | hr_mod: 8 | dr_mod: 9`
An echo entity — a resonance phenomenon from the crystal formation that has achieved sufficient coherence to function as an autonomous presence. Not a product of a specific death. May represent the substrate's earliest proto-consciousness before the Reversal completed. Attacks with cold and displacement rather than physical impact.
- `| 0 0 0 0 0 82 2` (strong: shadow; suscept: physical)
- `spec_cast_undead`
- Placement: Rooms 549, 550, 556 | Limit 1 each

**479 — cave root horror**
Level 73 | `act: is_npc aggressive stay_area no_mind` | `aff: infrared`
Alignment: -178 | Sex: neutral
`hp_mod: 0 | ac_mod: -75 | hr_mod: 10 | dr_mod: 9`
A section of the root network that has achieved mobile expression — a mass of root-fiber and crystal compound that moves through the cave using the root network's internal connections as a travel medium, emerging where needed. Territorially aggressive throughout the cave system.
- `| 0 0 32 0 0 512 0` (strong: earth; suscept: fire)
- Placement: Rooms 551, 554, 561 | Limit 1 each

---

### Named Elites and Bosses (480–484) | Level 70–80

**480 — The Thornwood Witch** ★
Level 72 | `act: is_npc sentinel stay_area solo` | `aff: detect_magic detect_invis detect_hidden`
Alignment: 0 | Sex: female
`hp_mod: 50 | ac_mod: -80 | hr_mod: 8 | dr_mod: 8`
The oldest human in the wood, possibly predating the estate itself. Operates independently of both Lord Harren and the Lich, answers to neither, tolerated by both because her knowledge of the substrate's historical behavior is irreplaceable. Not aggressive. Will negotiate. Speaks in the shaman's precise non-absolute language. Not safe.
- `! 0 0 0 7 0 6146 12` (mag_missile + flamestrike + mind_bolt cast; cure_serious + heal def)
- `spec_cast_mage`
- Loot: none (she is negotiable, not a combat objective; fights back if attacked)
- Placement: Room 502 (Shaman's Hollow) | Limit 1

**481 — Lord Harren, the Estate Revenant** ★
Level 76 | `act: is_npc sentinel stay_area solo boss undead` | `aff: infrared detect_invis detect_evil`
Alignment: -278 | Sex: male
`hp_mod: 100 | ac_mod: -100 | hr_mod: 10 | dr_mod: 10`
The last living head of the Harren family, risen and retained at his estate. Fully conscious and aware, bark-patterned skin, the administrative livery intact. Receives visitors. Can negotiate. Remembers being alive and governing as a living man. This memory is his defining characteristic and his defining torment. He is the cruelest product of the Reversal: complete awareness trapped in a system he cannot modify. When combat begins, he invokes the charter. The charter has no legal standing. He invokes it anyway.
- `! 0 0 0 7 382 4096 8` (2_attack + nodisarm + notrip + punch + headbutt; earthquake cast; cure_critic def)
- `spec_cast_undead`
- `l 100 506 507 0 0`
- `L 60 40 0 0`
- Placement: Room 452 (Receiving Room — `no_mob`) | Limit 1

**482 — The Wild Hunt Master** ★
Level 74 | `act: is_npc sentinel stay_area solo boss undead night_only` | `aff: infrared detect_invis flying`
Alignment: -178 | Sex: male
`hp_mod: 80 | ac_mod: -90 | hr_mod: 10 | dr_mod: 9`
The Wild Hunt's commander — mounted on a substrate horse, antler crown's root-material tines oriented outward as sensory apparatus, face painted with the binding's sigil-marks. Present only at night. The antler crown is not decorative; it receives root-network signals even at distance, meaning the Hunt Master knows what is happening across the entire estate's perimeter while the Hunt is away. The black shadow-bone horn at his side transmits through the substrate rather than through air.
- `! 0 0 0 7 382 4096 4` (2_attack + nodisarm + notrip + punch + headbutt; earthquake cast; cure_serious def)
- `spec_cast_undead`
- `l 100 508 0 0 0`
- `L 100 0 0 0`
- Placement: Room 567 (Hunt Dispatch Point — `no_mob`) | Limit 1 | `night_only`

**483 — Bone-Weave Overseer Malsthen** ★
Level 73 | `act: is_npc sentinel stay_area solo boss` | `aff: infrared detect_magic`
Alignment: -600 | Sex: neutral
`hp_mod: 80 | ac_mod: -90 | hr_mod: 10 | dr_mod: 10`
The ossuary undercroft's apex craftsman — the practitioner who oversees the in-progress large-scale construct on the assembly floor and who has been sourcing material from undocumented substrate routes. Describes combat in the vocabulary of construction load-bearing and structural failure. Fights with the precision and economy of a master craftsman. The construct-in-progress is their life's work. Interrupting it is the only thing that produces genuine anger.
- `! 0 0 0 7 382 0 4` (2_attack + nodisarm + notrip + punch + headbutt; cure_serious def)
- `spec_cast_undead`
- `l 100 509 0 0 0`
- `L 100 0 0 0`
- Placement: Room 538 (Construct Assembly Floor — flagged `no_mob` in final area file) | Limit 1

**484 — The Thornwood Lich** ★
Level 80 | `act: is_npc sentinel stay_area boss undead no_body` | `aff: infrared detect_invis detect_magic sanctuary`
Alignment: -291 | Sex: neutral
`hp_mod: 35 | ac_mod: -150 | hr_mod: 12 | dr_mod: 12`
The substrate intelligence that emerged from the crystal formation during the Reversal — what the Root Covenant's theology made literal, or what was always latent in the geological layer, or what assembled itself from centuries of absorbed memory-traces. Not human. May never have been. Sees estate, forest, and ossuary as one body. Speaks in systems. Does not threaten. Does not explain. Diagnoses. If it addresses an intruder, it is because the intruder has become relevant to the binding's current failure analysis.
- `! 0 0 0 7 0 33554434 14` (mag_missile + mind_bolt cast; cure_light + cure_serious + cure_critic def)
- `| 0 0 0 0 0 82 2` (strong: shadow; suscept: physical)
- `spec_cast_undead`
- `l 100 510 0 0 0`
- `L 100 0 0 0`
- Placement: Room 559 (Sealed Chamber — `no_mob`) | Limit 1

---

### Mobile Spec Compliance Summary

- All 44 mobs carry `stay_area` (`64`). ✓
- All aggressive mobs (`aggressive` = `32`) are hostile undead, feral creatures, or corrupted operatives — not civilian NPCs. ✓
- Boss mobs (481, 482, 483, 484) carry `sentinel` + `boss` flags and are placed in `no_mob` rooms. ✓
- Solo mobs (448, 451, 457, 461, 465, 469, 472, 474, 480, 481, 482, 483, 484) carry `solo` flag. ✓
- Undead mobs carry `undead` flag. Corporeal-absent mobs carry `no_body` and/or `no_blood`. ✓
- The Thornwood Witch (480) is `sentinel`, `solo`, non-aggressive — consistent with interface-specialist lore. ✓
- `invasion` flag not set on any mob. ✓
- `day_only` not used on any mob. `night_only` used only on 445 (approach phantom) and 482 (Hunt Master). ✓

---

## Object Redesign Plan (485–511 — 27 Objects)

All objects carry `ITEM_TAKE` (`8388608`) in wear flags. No object carries `ITEM_WEAR_CLAN_COLORS`. `ITEM_GENERATED` is never set — stats generated at runtime. Weight encodes archetype: 1–5 caster, 6–10 melee, 11–15 tank. Boss-loot items carry `ITEM_BOSS` (`134217728`) + `ITEM_LOOT` (`67108864`). Standard mob loot items carry `ITEM_LOOT` only. Object names must be unique within the area.

Item type values: `armor` = 9, `weapon` = 5, `treasure` = 8, `key` = 18, `light` = 1.
Wear flag values: `take` = 8388608, `hold` = 32768, `head` = 8, `neck` = 128, `shoulders` = 512, `arms` = 315, `wrist` = 840, `hands` = 4096, `finger` = 8192, `about` = 65536, `waist` = 131072, `body` = 262144, `legs` = 1048576, `feet` = 2097152, `face` = 16.
Weapon attack types: `slice` = 1, `stab` = 2, `slash` = 3, `whip` = 4, `claw` = 5, `blast` = 6, `pound` = 7, `crush` = 8, `pierce` = 11.

---

### Entry-Tier Gear (485–488) | Level ~50–57 | Drops from Tier 1 and early Tier 2 mobs

**485 — thorn-motif iron bracers**
`item_type: armor (9)` | `extra_flags: ITEM_LOOT` | `wear_flags: wrist take` (840 + 8388608 = 8390656)
`value0-9: 0 0 0 0 0 0 0 0 0 0` | `weight: 11` (tank)
Name: `iron bracers thorn` | Short: `a pair of @@dthorn-motif iron bracers@@N` | Desc: `A pair of iron bracers stamped with the @@yHarren thorn device@@N at the outer cuff, worn smooth by the estate's perimeter patrols.`
Loot from: thorn hedge sentinels (443), thorn knight (447)

**486 — grave warden's hood**
`item_type: armor (9)` | `extra_flags: ITEM_LOOT` | `wear_flags: head take` (8 + 8388608 = 8388616)
`value0-9: 0 0 0 0 0 0 0 0 0 0` | `weight: 13` (tank)
Name: `hood warden grave` | Short: `a @@dgrave warden's hood@@N` | Desc: `A heavy cloth hood reinforced at the crown with a strip of iron, stitched with the same ochre-and-thorn border used on the estate's patrol livery.`
Loot from: thorn knight (447), estate gate wraith (442) — carried as estate-authority marker

**487 — rusted estate patrol blade**
`item_type: weapon (5)` | `extra_flags: ITEM_LOOT` | `wear_flags: hold take` (32768 + 8388608 = 8421376)
`value0: 0` | `value1: 0` | `value2: 0` | `value3: 3` (slash) | `value4-9: 0` | `weight: 8` (melee)
Name: `blade patrol estate` | Short: `a @@drusted estate patrol blade@@N` | Desc: `A straight-bladed short sword issued to the Harren estate's perimeter guards, the iron corroded to flaking orange at the edges but still holding an edge where the substrate's preservative chemistry reached it.`
Loot from: thorn knight (447), thorn hedge sentinel (443)

**488 — corpse-cart driver's whip**
`item_type: weapon (5)` | `extra_flags: ITEM_LOOT` | `wear_flags: hold take` (8421376)
`value0-2: 0` | `value3: 4` (whip) | `value4-9: 0` | `weight: 7` (melee)
Name: `whip driver cart corpse` | Short: `a @@dcorpse-cart driver's whip@@N` | Desc: `A long-handled driving whip whose leather has been treated with the estate's tannin compound, making it unexpectedly durable and carrying a faint smell of sweet rot that no amount of cleaning removes.`
Loot from: corpse-cart revenant (444)

---

### Estate Surface Gear (489–494) | Level ~58–64 | Drops from Tier 2 mobs

**489 — manor steward's chain of office**
`item_type: treasure (8)` | `extra_flags: ITEM_LOOT` | `wear_flags: neck take` (128 + 8388608 = 8388736)
`value0-9: 0` | `weight: 3` (caster)
Name: `chain steward manor office` | Short: `a @@ymanor steward's chain of office@@N` | Desc: `A heavy silver-link chain bearing a central medallion stamped with the Harren thorn-and-keyhole device, worn by the estate's senior administrative staff as both identifier and credential. The @@yHarren gold finish@@N on the medallion has survived intact.`
Loot from: manor steward revenant (449)

**490 — the Harren seal ring**
`item_type: treasure (8)` | `extra_flags: ITEM_LOOT ITEM_MAGIC` | `wear_flags: finger take` (8192 + 8388608 = 8396800) | `item_apply: nada (1)`
`value0-9: 0` | `weight: 2` (caster)
Name: `ring seal harren` | Short: `@@ythe Harren seal ring@@N` | Desc: `A gold ring bearing the Harren estate seal — the thorn-and-keyhole device — in intaglio. Pressing it into wax produces the seal that appeared on every document the estate issued. The ring is warm to the touch in an environment where everything else is cold. It remembers the authority it represented, even though that authority is gone.`
Loot from: manor steward revenant (449), estate household wraith (446)

**491 — thorn knight's briar pauldrons**
`item_type: armor (9)` | `extra_flags: ITEM_LOOT` | `wear_flags: shoulders take` (512 + 8388608 = 8389120)
`value0-9: 0` | `weight: 14` (tank)
Name: `pauldrons briar thorn knight` | Short: `@@Gthorn knight's briar pauldrons@@N` | Desc: `Shoulder armor from which thorn growth has erupted and set, the briars fusing with the original iron in a composite that is heavier and harder than either material alone. The thorn growth follows the pauldron's original contour, suggesting the substrate read the armor's shape and chose to reinforce rather than replace it.`
Loot from: thorn knight (447), blackened thorn knight (448)

**492 — the blackened thorn knight's crest**
`item_type: armor (9)` | `extra_flags: ITEM_LOOT` | `wear_flags: head take` (8388616)
`value0-9: 0` | `weight: 15` (tank)
Name: `crest helm blackened thorn knight` | Short: `@@dthe blackened thorn knight's crest@@N` | Desc: `A helm whose original iron shape has been entirely consumed by briar growth, the biological material preserving the helmet's form while replacing its material. The crest is fully organic, unnaturally hard, and carries the same cold as the thorn ring guardians who wear it.`
Loot from: blackened thorn knight (448)

**493 — chapel covenant staff**
`item_type: weapon (5)` | `extra_flags: ITEM_LOOT ITEM_MAGIC` | `wear_flags: hold take` (8421376)
`value0-2: 0` | `value3: 7` (pound) | `value4-9: 0` | `weight: 4` (caster)
Name: `staff covenant chapel` | Short: `a @@pchapel covenant staff@@N` | Desc: `A staff of estate thornwood, its upper end fitted with a socket of bone that holds a small crystal shard — substrate material incorporated into a ritual implement. The @@pstaffing notation@@N carved along its length reads as both a covenant clause and a prayer, in the Root Covenant's characteristic blending of legal and devotional register.`
Loot from: chapel cultist (450), covenant elder (451)

**494 — root-woven armband**
`item_type: armor (9)` | `extra_flags: ITEM_LOOT` | `wear_flags: wrist take` (8390656)
`value0-9: 0` | `weight: 9` (melee)
Name: `armband root woven` | Short: `a @@Groot-woven armband@@N` | Desc: `A wristband of compressed root-fiber, woven into a flat-plate structure by the same technique the bone weavers use for joint reinforcement. Harder than leather, lighter than iron, and carrying the faint tannin smell that marks substrate-integrated biological material.`
Loot from: covenant bone-reader (452), root weave cultist (462)

---

### Burial Field and Ossuary Gear (495–499) | Level ~62–68 | Drops from Tier 3 and Tier 5 mobs

**495 — deferred-name ledger pendant**
`item_type: treasure (8)` | `extra_flags: ITEM_LOOT ITEM_MAGIC` | `wear_flags: neck take` (8388736) | `item_apply: det_mag (512)`
`value0-9: 0` | `weight: 2` (caster)
Name: `pendant ledger deferred name` | Short: `a @@ddeferred-name ledger pendant@@N` | Desc: `A small tablet of bone-pressed material on a cord, bearing a case number from the estate's deferred-name register rather than a name — the standardized identifier issued to those whose families could not pay the naming rite fee. Carrying it produces the specific, cold awareness of existing in a system that has decided your name is contingent.`
Loot from: deferred-name revenant (455)

**496 — bone weaver's jointing awl**
`item_type: weapon (5)` | `extra_flags: ITEM_LOOT` | `wear_flags: hold take` (8421376)
`value0-2: 0` | `value3: 11` (pierce) | `value4-9: 0` | `weight: 8` (melee)
Name: `awl jointing bone weaver` | Short: `a @@dbone weaver's jointing awl@@N` | Desc: `A purpose-built tool for threading root-fiber through bone at joint junctions — long, tapered, and made of a bone-and-metal composite that takes an edge as sharp as steel. In the bone weaver's workshop it is a craft instrument. Outside it, it is simply a piercing weapon of unusually precise manufacture.`
Loot from: bone weaver journeyman (471), bone weaver master (472)
L
63

**497 — ossuary-processed bone vambrace**
`item_type: armor (9)` | `extra_flags: ITEM_LOOT` | `wear_flags: arms take` (315 + 8388608 = 8389632)
`value0-9: 0` | `weight: 10` (melee)
Name: `vambrace bone ossuary processed` | Short: `an @@dossuary-processed bone vambrace@@N` | Desc: `A forearm guard built from ossuary-grade bone — cleaned, sorted for structural density, and shaped over a root-fiber armature. The substrate's processing gives this material a denser, harder character than natural bone. It carries the faint harmonic resonance of the ossuary's crystal-adjacent storage environment.`
Loot from: marrow construct (473), marrow fiend (474)
L
66

**498 — cold-resonant bone shard**
`item_type: treasure (8)` | `extra_flags: ITEM_LOOT ITEM_MAGIC` | `wear_flags: hold take` (8421376) | `item_apply: infra (2) det_mag (512)`
`value0-9: 0` | `weight: 1` (caster)
Name: `shard bone resonant cold` | Short: `a @@acold-resonant bone shard@@N` | Desc: `A fragment of Thornwood-processed bone that has acquired the substrate's thermal anomaly — it remains several degrees below ambient temperature indefinitely. When struck it produces a tone that matches the crystal formation's harmonic signature, which the bone weavers use to authenticate genuinely substrate-processed material. The tone is faint, persistent, and slightly wrong in the way that sounds produced by things that should be silent are always slightly wrong.`
Loot from: ossuary elder (475), bone weaver master (472)
L
65

**499 — charnel verge shroud**
`item_type: armor (9)` | `extra_flags: ITEM_LOOT` | `wear_flags: about take` (65536 + 8388608 = 8454144)
`value0-9: 0` | `weight: 5` (caster)
Name: `shroud charnel verge` | Short: `a @@dcharnel verge shroud@@N` | Desc: `A heavy ceremonial over-garment worn by the estate's burial processing staff during formal receipt operations — dark fabric treated with the estate's preservative tannin compound, designed to resist moisture and chemical exposure during charnel work. The tannin treatment has darkened the fabric to near-black and given it a particular stiffness at the hem.`
Loot from: charnel verge watcher (457), ossuary monk (459)

---

### Mire and Wild Hunt Gear (500–505) | Level ~64–72 | Drops from Tier 4 and Hunt mobs

**500 — mire lurker hide wrapping**
`item_type: armor (9)` | `extra_flags: ITEM_LOOT` | `wear_flags: body take` (262144 + 8388608 = 8650752)
`value0-9: 0` | `weight: 9` (melee)
Name: `wrapping hide lurker mire` | Short: `@@Gmire lurker hide wrapping@@N` | Desc: `Body wrapping made from the substrate-adapted hide of the mire's apex predator — the same mud-dark material that makes the lurker effectively invisible in its own environment. The hide's water-resistance exceeds any natural leather, and it carries a faint sulfurous smell that is the mire's identifying chemical signature.`
Loot from: mire lurker (466)
L
62

**501 — fog shrine offering shard**
`item_type: treasure (8)` | `extra_flags: ITEM_LOOT ITEM_MAGIC` | `wear_flags: hold take` (8421376) | `item_apply: prot (128)`
`value0-9: 0` | `weight: 3` (caster)
Name: `shard offering shrine fog` | Short: `a @@pfog shrine offering shard@@N` | Desc: `A fragment of bronze from the corroded offerings on the fog shrine's platform — old enough that the metal's composition predates the estate and possibly predates the ridge-clan communities. The shard carries a faint protective quality that its original dedicants may have intended or the substrate may have added later. Its function is unclear; its age is not.`
Loot from: fog shrine attendant (469)
L
65

**502 — sulfur-chain waist links**
`item_type: armor (9)` | `extra_flags: ITEM_LOOT` | `wear_flags: waist take` (131072 + 8388608 = 8519680)
`value0-9: 0` | `weight: 8` (melee)
Name: `links chain sulfur waist` | Short: `@@dsulfur-chain waist links@@N` | Desc: `A chain of iron links chemically altered by long exposure to the mire's alchemical runoff — the sulfur compounds have produced an iridescent coating that is simultaneously corrosive to contact and strangely protective to the wearer, as though the chemical process selected for a specific surface outcome.`
Loot from: sulfur-bloated corpse (468), substrate water-thing (470)

**503 — hunt-sigil face mask**
`item_type: armor (9)` | `extra_flags: ITEM_LOOT` | `wear_flags: face take` (16 + 8388608 = 8388624)
`value0-9: 0` | `weight: 8` (melee)
Name: `mask face hunt sigil` | Short: `a @@dhunt-sigil face mask@@N` | Desc: `A leather face mask worn by the Wild Hunt Riders — the sigil markings painted on its surface match the binding's inscription language at a simplified level: the hunt-rank equivalents of the Hunt Master's full-binding face paint. The leather is treated with substrate horse material, giving it the same birch-bark pallor as the Hunt's mounts.`
Loot from: Wild Hunt Rider — reset in room 563, 564

**504 — substrate horse briar-weave bridle**
`item_type: treasure (8)` | `extra_flags: ITEM_LOOT ITEM_MAGIC` | `wear_flags: hold take` (8421376) | `item_apply: fly (16384)`
`value0-9: 0` | `weight: 4` (caster)
Name: `bridle briar weave horse substrate` | Short: `a @@Gsubstrate horse briar-weave bridle@@N` | Desc: `Tack from a substrate horse — root-fiber and briar woven into harness form, carrying the faint cold of the substrate construction it came from. The bridle's inner surface carries the berry-red color of the substrate horses' eyes, pressed in as a mark rather than dyed. Something about holding it creates a brief, unwanted awareness of being outside very large, very old territorial claims.`
Loot from: Wild Hunt Rider reset equipment

**505 — shadow hunt horn shard**
`item_type: treasure (8)` | `extra_flags: ITEM_LOOT ITEM_MAGIC` | `wear_flags: hold take` (8421376) | `item_apply: det_hid (315) det_invis (8)`
`value0-9: 0` | `weight: 3` (caster)
Name: `shard horn hunt shadow` | Short: `a @@dshadow hunt horn shard@@N` | Desc: `A fragment of the shadow-bone from which the Wild Hunt's signaling horn is constructed — the material that allows the horn's sound to travel through the substrate's root network rather than through air. Even as a fragment it retains this property at a diminished level: pressing it to the ground produces the faint, subsurface sense of distant sound arriving from below rather than above.`
Loot from: fog shrine attendant (469), deep mire shade (467)

---

### Boss Signature Items (506–510) | Boss drops only | `ITEM_BOSS` + `ITEM_LOOT`

**506 — Lord Harren's Thorn-Crest Sigil Ring**
`item_type: treasure (8)` | `extra_flags: ITEM_BOSS ITEM_LOOT ITEM_RARE ITEM_MAGIC` | `wear_flags: finger take` (8396800) | `item_apply: det_evil (840) det_mag (512)`
`value0-9: 0` | `weight: 1` (caster)
Name: `ring sigil thorn crest harren` | Short: `@@yLord Harren's Thorn-Crest Sigil Ring@@N` | Desc: `The Harren family's authority ring — not the administrative seal ring but the personal authority token, worn by the family's head as a sign of direct power rather than institutional delegation. The @@ythorn-crest device@@N on the bezel is more elaborate than the estate seal, incorporating the binding's primary sigil into the thorn pattern in a way that the Harrens' administrative records never documented. Lord Harren wore this when he attempted to negotiate with the substrate on the grounds of standing. He still wears it. He is parted from it only in defeat.`
Loot from: Lord Harren (481) — loot table slot 0, 60% weight
E
sigil ring thorn crest~
The ring's intaglio shows the Harren thorn device, but the keyhole at its center has been replaced with the binding's primary sigil. Whether this modification predates the Reversal or was made after is not answerable from the ring alone.
~

**507 — Binding Iron Gauntlets of the Estate**
`item_type: armor (9)` | `extra_flags: ITEM_BOSS ITEM_LOOT ITEM_RARE` | `wear_flags: hands take` (4096 + 8388608 = 8392704)
`value0-9: 0` | `weight: 14` (tank)
Name: `gauntlets iron binding estate` | Short: `@@dthe Binding Iron Gauntlets of the Estate@@N` | Desc: `Iron gauntlets forged with the binding's inscription text worked into the knuckle plates — not decoratively, but as functional inscription, the same way the drain passages carry the binding's text as part of their structure. The gauntlets are warm to the touch where everything in the estate is cold, because the binding's thermal output concentrates at its inscription points. Wearing them creates a faint awareness of the estate's territorial boundaries as a felt pressure.`
Loot from: Lord Harren (481) — loot table slot 1, 40% weight
E
gauntlets binding inscription~
The inscription on the knuckle plates corresponds to the middle section of the binding's formal text — the mechanism and dependencies section, the part that describes what happens when components fail.
~

**508 — the Wild Hunt Master's Antler Crown**
`item_type: armor (9)` | `extra_flags: ITEM_BOSS ITEM_LOOT ITEM_RARE ITEM_MAGIC` | `wear_flags: head take` (8388616) | `item_apply: det_hid (315) infra (2)`
`value0-9: 0` | `weight: 9` (melee)
Name: `crown antler hunt master wild` | Short: `@@Gthe Wild Hunt Master's Antler Crown@@N` | Desc: `The Hunt Master's antler crown — root-material tines grown from the substrate rather than harvested from any animal, oriented and shaped by the binding's sensory requirements rather than by any organic growth pattern. Wearing it produces a low, continuous awareness of the root network's surface expression in a radius around the wearer — the same signal the Hunt Master uses to monitor the estate's perimeter from any point on the patrol routes. The awareness is not optional. The crown does not have a passive setting.`
Loot from: Wild Hunt Master (482) — loot table slot 0, 100% weight
E
antler crown root material~
The tines are not hollow as natural antler is. They are solid root-fiber, grown to antler shape by substrate direction rather than evolved into it. The difference is not visible. It is felt when worn.
~

**509 — the Overseer's Marrow Spear**
`item_type: weapon (5)` | `extra_flags: ITEM_BOSS ITEM_LOOT ITEM_LEGENDARY ITEM_TWO_HANDED` | `wear_flags: hold take` (8421376)
`value0-2: 0` | `value3: 11` (pierce) | `value4-9: 0` | `weight: 9` (melee)
Name: `spear marrow overseer two-handed` | Short: `@@dthe Overseer's Marrow Spear@@N` | Desc: `The Bone-Weave Overseer Malsthen's primary working instrument — a two-handed spear whose shaft is a single length of ossuary-grade bone, its grain running true from base to point, the joint reinforcement at the socket rendered in root-fiber threading so precisely calibrated that the weapon achieves a resonance when swung that matches the crystal substrate's harmonic. The point is shaped, not cast: the bone's natural crystal-adjacent density taken to a tip through the same selective-grinding technique used to finish structural members in the construct assembly process.`
Loot from: Bone-Weave Overseer Malsthen (483) — loot table slot 0, 100% weight
E
spear bone resonant~
Striking with the spear produces a tone on impact. The tone matches the substrate's harmonic. This is not coincidence. It is the Overseer's signature: proof that the weapon was made with the same precision as the undercroft's finest constructs.
~

**510 — the Thornwood Lich's Substrate Crystal**
`item_type: treasure (8)` | `extra_flags: ITEM_BOSS ITEM_LOOT ITEM_LEGENDARY ITEM_MAGIC` | `wear_flags: hold take` (8421376) | `item_apply: det_mag (512) prot (128) det_hid (315)`
`value0-9: 0` | `weight: 1` (caster)
Name: `crystal substrate lich thornwood` | Short: `@@athe Thornwood Lich's Substrate Crystal@@N` | Desc: `A single crystal growth taken from the formation at the sealed chamber's sigil-center — the point where the Lich occupies the binding's operational interface. The crystal carries the full thermal anomaly and harmonic signature of the substrate, sustained indefinitely without the cave system's amplification. It is @@acold beyond cold@@N: holding it in an ungloved hand produces the same sensation as the high cave ledge, where the binding's failing geometry is visible as a coherent pattern in the formation below. The crystal sees Thornwood as the Lich sees it: estate, forest, and ossuary as one body. Carrying it transfers a fraction of that perspective to the carrier.`
Loot from: Thornwood Lich (484) — loot table slot 0, 100% weight
E
crystal substrate binding~
The crystal's harmonic output has changed since the binding began failing. What the bone-weavers describe as "the voice changing" is audible here in miniature: the tone is the same pitch but the overtone structure is different, the upper harmonics absent, the baseline sustained without the complexity that characterized the formation when the binding was intact.
~

---

### Key and Quest Items (511)

**511 — the Harren Estate Register Key**
`item_type: key (18)` | `extra_flags: ITEM_NOSAVE ITEM_NODROP` | `wear_flags: hold take` (8421376)
`value0-9: 0` | `weight: 1`
Name: `key register estate harren` | Short: `@@ythe Harren Estate Register Key@@N` | Desc: `A brass key bearing the estate's administrative seal — the specific key issued to authorized auditors of the burial records under the original Harren Charter's oversight provisions. This is the key that unlocks the sealed stone door between the binding inscription passage and the sealed chamber below. It works because the door's lock is calibrated to the estate's authentication layer, and the key carries that layer's identifier. Its continued function suggests the authentication system considers auditor access legitimate — or simply that the substrate has decided to allow it, for its own reasons.`
Drop/Reset: Reset as floor object in room 583 (Forgotten Archive Alcove) — one-time discovery; limit 1
Note: Used as `key_vnum` for the locked door at room 558 → 559. Must be defined before the `#RESETS` section for the door's key reference to validate.
E
key harren register~
The teeth of the key match the binding inscription's notation for "authorized threshold passage." Whether this was intentional on the part of whoever cut the key, or whether the key was modified by the substrate to serve this purpose, is not answerable from the key alone.
~

---

### Object Spec Compliance Summary

- All 27 objects carry `ITEM_TAKE`. ✓
- No object carries `ITEM_WEAR_CLAN_COLORS`. ✓
- `ITEM_GENERATED` not set on any object. ✓
- Weight archetype encoding applied to all objects. ✓
- Boss-signature items (506–510) carry `ITEM_BOSS` + `ITEM_LOOT`. ✓
- Standard loot items (485–505) carry `ITEM_LOOT` only. ✓
- Weapons (487, 488, 493, 496, 509) carry both `hold` and `take` in wear flags. ✓
- Two-handed weapon (509) carries `ITEM_TWO_HANDED`. ✓
- `value3 = 0` (hit) not used on any weapon; all `value3` choices are thematically consistent. ✓
- Object names are unique within the area. ✓
- No fixed stats defined; stats generated at runtime. ✓
- Object level (`L`) entries used for loot items where a level floor is appropriate. ✓
- `ITEM_LIFESTEALER` not used in this area. ✓

---

## Quest Design (satisfying `area_file_spec.md` §13.2)

Four quests satisfy all mandatory constraints: at least one boss quest, at least one cartography quest, final chain item rewards equipment, and every boss quest rewards equipment.

---

### Quest 1: Cartography of the Harren Estate
**Type:** Cartography (mandatory for new areas per spec §13.2)
**Objective:** Survey and document five key locations within Thornwood: the Iron Gate of Thornwood, the Charnel Verge Gate, the Fog Shrine, the Handprint Cave, and the Crystal Heart Basin.
**Mechanism:** A Thornwood Cartography Writ (or equivalent quest trigger outside the area) asks players to examine specific `E` extra descriptions in each of the five target rooms — each containing a surveyor's notation that, when examined, advances the quest.
**Reward:** Experience + the @@dcold-resonant bone shard@@N (object 498) as an intermediate equipment reward.
**Notes:** The five target rooms span all major zones, incentivizing the full area traversal before the boss loop. The fog shrine and crystal heart entries require navigating the mire and the cave system respectively, making completion a genuine exploration benchmark.

---

### Quest 2: The Archive Recovery
**Type:** Lore recovery / collection chain
**Objective:** Recover three specific archive fragments from the estate's frozen interior: the final entries from Lord Harren's journal (room 453, study — `E` extra), the butler's transition notation (room 455, pantry — `E` extra), and the restricted tunnel transfer register (room 584, tunnel records — `E` extra or ground object).
**Mechanism:** Each fragment is either a ground-reset object or an examined `E` extra that awards a quest item. All three fragments must be in the player's possession simultaneously to complete the chain (or examined in sequence if the quest system tracks flags).
**Reward:** The @@yHarren Estate Register Key@@N (object 511) — specifically, recovering the archive fragments leads to finding the key among the butler's pantry materials, providing narrative justification for the key's presence. This makes the key's acquisition story-integrated rather than arbitrary loot.
**Final chain item:** The key is equipment (usable, has function) — satisfies the requirement that the final quest chain item reward a piece of equipment.
**Cross-area note:** Archive fragments found here should be cross-referenced in Midgaard's Granite Arcade lore text, offering hooks for players who have already explored that area's contested archive records.

---

### Quest 3: The Lord's Reckoning (Boss Quest — Lord Harren)
**Type:** Boss quest (mandatory per spec §13.2)
**Target:** Lord Harren, the Estate Revenant (mob 481) in the Receiving Room (452)
**Prerequisite:** Reaching room 452 requires navigating through the manor's interior (Zone II), which requires passing Thorn Knight patrols and the manor steward revenant. The Receiving Room is `no_mob` flagged and requires no key — it is accessible to anyone who reaches it.
**Objective:** Defeat Lord Harren.
**Reward:** Boss loot table drops one of two boss signature items:
- **Slot 0 (60% weight):** Lord Harren's Thorn-Crest Sigil Ring (506) — `ITEM_BOSS ITEM_LOOT ITEM_RARE`
- **Slot 1 (40% weight):** Binding Iron Gauntlets of the Estate (507) — `ITEM_BOSS ITEM_LOOT ITEM_RARE`
**Narrative note:** Lord Harren will negotiate before combat if the player can be made to attempt interaction — a `speech_prog` on his mob triggers a dialogue that invokes the Harren Charter, the estate's audit provisions, and the pending sealed warrant inquiry. None of this prevents the combat. It is what the lore says it is: a man with complete awareness performing legitimacy rituals that have no legal standing, because he cannot stop.

---

### Quest 4: The Binding's Reckoning (Final Boss Quest — Thornwood Lich)
**Type:** Boss quest (mandatory per spec §13.2) + final chain quest
**Target:** The Thornwood Lich (mob 484) in the Sealed Chamber (559)
**Prerequisite:**
1. The Harren Estate Register Key (511) must be obtained (from Quest 2 or as ground reset in 583).
2. The locked door at room 558 → 559 must be opened with the key.
3. Players must navigate the full cave system (Zone VII) to reach 558.
**Objective:** Defeat the Thornwood Lich.
**Reward:** The Thornwood Lich's Substrate Crystal (510) — `ITEM_BOSS ITEM_LOOT ITEM_LEGENDARY ITEM_MAGIC`. This is the final chain item; it is equipment (held item with active `item_apply` effects). Satisfies the requirement that the final quest chain's final item reward equipment. Also satisfies the requirement that boss quests reward equipment.
**Narrative note:** The Lich's `death_prog` (if implemented) may emit the binding's failure assessment at the moment of death — the crystal formation's final diagnostic output as the Lich's operational interface goes offline. This is not a dramatic speech; it is a system log. The tone should be exactly as clinical as an engineer's final report on a structure that is about to fail.

---

### Quest Constraint Verification

- ✓ At least one boss quest: Quest 3 (Lord Harren) and Quest 4 (Thornwood Lich) both qualify.
- ✓ At least one cartography quest: Quest 1.
- ✓ Final quest chain item rewards equipment: Quest 2's final item is object 511 (functional key, worn as hold item). Quest 4's final item is object 510 (held treasure with apply effects). Both qualify.
- ✓ Every boss quest rewards equipment: Quest 3 rewards 506 or 507 (armor/treasure, both boss-flagged). Quest 4 rewards 510 (treasure with apply, boss-flagged).
- Optional bonus boss: Bone-Weave Overseer Malsthen (mob 483) and Wild Hunt Master (mob 482) also qualify as boss encounters with boss-signature loot (509 and 508 respectively), though they are not required by the spec constraint count.

---

## Reset and Encounter Structure

### Population Density by Zone

- **Zone I (Harren Approach, 441–448):** Light to moderate. Grave tunnel toll-shades (2–3), gate wraith sentinel (1), thorn hedge sentinels at perimeter walls (2 each), corpse-cart revenants on approach (2), night-only approach phantoms (2). Total: ~12 mob resets.
- **Zone II (Estate Surface, 449–470):** Moderate with named anchors. Thorn knights at key thresholds (6–8), estate wraiths in service rooms (3), chapel cultists in chapel (2), covenant elder sentinel at assembly ground (1), manor steward revenant in Great Hall (1), stable revenant (1), Lord Harren boss in Receiving Room (1). Total: ~17 mob resets.
- **Zone III (Charnel Verge and Burial Fields, 471–488):** Moderate to dense. Deferred-name revenants across burial fields (6), bone processing laborers (4), charnel verge watcher at gate (1), exhumation crows in open field rooms (9), ossuary monks in charnel chapel (2). Total: ~22 mob resets.
- **Zone IV (Older Gravewood, 489–506):** Moderate with specialist presence. Ridge-clan shades at face-down sites (4), thornwood shaman sentinel in hollow (1), root weave cultists at offering sites (4), substrate-adapted root-crawlers (2), face-down risen at oathbreaker graves (2), thorn ring guardians at old rings (2). Total: ~15 mob resets.
- **Zone V (Mire, 507–522):** Moderate hazard pressure. Mire lurkers in crossing rooms (6), deep mire shades at pool and deep sections (3), sulfur-bloated corpses at runoff channels (4), fog shrine attendant sentinel at shrine (1), substrate water-things (2). Total: ~16 mob resets.
- **Zone VI (Ossuary Undercroft, 523–540):** High threat, specialist encounters. Bone weaver journeymen (4), bone weaver master in workshop (1), marrow constructs at vault and assembly (4), marrow fiends in vault (2), ossuary elder sentinel at entry hall (1). Total: ~12 mob resets.
- **Zone VII (Cave System, 541–561):** High threat, substrate creatures. Crystal cave stalkers in crystal passages (6), binding watchers at critical cave rooms (3), substrate echoes (3), cave root horrors (3), Thornwood Lich boss in sealed chamber (1). Total: ~16 mob resets.
- **Zone VIII (Wild Hunt, 562–571):** Night-only boss content. Wild Hunt Riders at patrol stations (4, night only), Wild Hunt Master boss at dispatch (1, night only). Total: ~5 mob resets, all night_only.
- **Zone IX (Perimeter/Auxiliary, 572–590):** Low to moderate supplemental presence. Thorn knight at guardpost (1), estate wraiths in wraith passage (2), additional thorn knights at wall walks (2), covenant bone-readers at offering sites (2). Total: ~7 mob resets.

### Key Reset Commands

**Mobile resets (`M`):**
- Boss mobs (481, 482, 483, 484): limit `1` each.
- Solo mobs (448, 451, 457, 461, 465, 469, 472, 474): limit `1` each.
- Common mobs: limit `2`–`3` per room.
- Night-only mobs (445, 482): flagged with `night_only` in `act`; limits standard.

**Object resets (`O`):**
- Harren Estate Register Key (511): O reset in room 583 (Forgotten Archive Alcove), limit `1`.
- Cartography quest targets: E extra descriptions in rooms 443, 471, 514, 546, 549 serve as in-world survey anchors.

**Equipment resets (`G`/`E`):**
- Estate gate wraith (442): E reset with grave warden's hood (486), wear_loc head (4).
- Thorn knight (447): E reset with thorn-motif iron bracers (485), wear_loc wrist (14/15), and rusted estate patrol blade (487), wear_loc hold_hand_r (21).
- Blackened thorn knight (448): E reset with blackened thorn knight's crest (492), wear_loc head (4), and thorn knight's briar pauldrons (491), wear_loc shoulders (12).
- Chapel cultist (450): E reset with chapel covenant staff (493), wear_loc hold_hand_r (21).
- Lord Harren (481): loot table (l/L extension in mob definition) for 506/507.
- Wild Hunt Master (482): loot table for 508.
- Bone-Weave Overseer Malsthen (483): loot table for 509.
- Thornwood Lich (484): loot table for 510.

**Door resets (`D`):**
- Room 558 → 559 (Threshold of the Binding → Sealed Chamber): locked (state `2`), key = 511. Both sides must have matching D resets:
  - `D 0 558 1 2` (south exit from 558 to 559, locked) — direction south = 2 if 559 is south of 558; confirm direction in room layout.
  - `D 0 559 0 2` (north exit from 559 to 558, locked from inside; unlocked from inside without key — note: this is the same state, but players inside can open from within without key per room spec).
  - Both room exits must have `EX_ISDOOR` (`1`) set in their `D<door>` definitions.

### `#SHOPS` Section
No shops are planned. Thornwood is not a commercial zone. The estate's trade was conducted through the grave tunnel network and external intermediaries, and that infrastructure no longer operates.

### `#SPECIALS` Section

```
#SPECIALS
M 442 spec_guard
M 451 spec_cast_cleric
M 457 spec_undead
M 461 spec_cast_mage
M 472 spec_cast_undead
M 478 spec_cast_undead
M 479 spec_cast_undead
M 480 spec_cast_mage
M 481 spec_cast_undead
M 482 spec_cast_undead
M 483 spec_cast_undead
M 484 spec_cast_undead
M 468 spec_poison
S
```

Note: `spec_summon_*` specials are not used (runtime-only per spec). `spec_keep_*` specials are not set by automation.

### `#OBJFUNS` Section
No object functions are planned for this area. The object fun mechanics (soul_moan, infused_soul, flaming, etc.) are not required to express Thornwood's identity — the substrate's effects are represented through mob behavior and object descriptions rather than active object functions.

---

## Implementation Checklist

### Phase 1: Area Header and Structure
- [ ] Normalize `O virant~` to `O Virant~`.
- [ ] Verify `Q 16` present.
- [ ] Update area name string to `@@dThornwood, @@ythe Harren Estate@@N~`.
- [ ] Update level label to `@@y{@@d50@@y-@@d80@@y}@@N~`.
- [ ] Update reset message to `@@dDeep in @@GThornwood@@d, the roots shift and the @@yHarren gate@@d rattles in windless dark.@@N~`.
- [ ] Confirm `V 441 590` unchanged.
- [ ] Confirm `I 50 80` preserved.
- [ ] Confirm `F 15` preserved.

### Phase 2: Room Construction (441–590, 150 rooms)
- [ ] Write all 150 room names and descriptions with unique identity per this plan.
- [ ] Ensure all room descriptions end with exactly one trailing newline before `~`.
- [ ] Ensure landmark rooms (marked ★) contain at least 5 sentences; all others at least 3.
- [ ] Ensure all room descriptions are unique — no identical descriptions.
- [ ] Define all exits as bidirectional; verify each exit pair against the topology diagram.
- [ ] Verify no directional loops (no room A → B → C → A outside ROOM_MAZE).
- [ ] Add named exits with discoverability (appear in main description, object, or E extra).
- [ ] Prefix named door-style exit keywords with `^`.
- [ ] Add `E` extra descriptions with anchored keyword chains in target rooms.
- [ ] Flag boss rooms (452, 559, 567) with `no_mob` (`4`).
- [ ] Flag cave rooms with `cold` where appropriate.
- [ ] Flag mire rooms with `cold` and `dark`.
- [ ] Flag all rooms needing `no_recall`, `no_portal`, `no_teleport` correctly.
- [ ] Apply correct `sector_type` per room (forest, inside, field, hills, water_swim, water_noswim, city).
- [ ] Verify room 441 external exit matches graveyard.are connector vnum.
- [ ] Verify room 558 door exit has `EX_ISDOOR` set and `key_vnum = 511`.

### Phase 3: Mobile Construction (441–484, 44 mobs)
- [ ] Build all 44 mobs in ascending vnum order per this plan.
- [ ] Ensure `long_descr` is exactly one text line + newline + `~`. No multi-line.
- [ ] Ensure `description` ends with exactly one newline before `~`.
- [ ] No vnum appears in any mob description text.
- [ ] All mobs carry `stay_area` (`64`).
- [ ] Boss mobs (481, 482, 483, 484) carry `sentinel` + `boss` flags.
- [ ] Solo mobs carry `solo` flag.
- [ ] Undead mobs carry `undead` flag.
- [ ] Corporeal-absent mobs carry `no_body` and/or `no_blood`.
- [ ] `aggressive` set only on hostile mobs, not on the Thornwood Witch (480) or thornwood shaman (461).
- [ ] Night-only mobs (445, 482) carry `night_only` flag; verify no `day_only` conflicts.
- [ ] `!` extension lines authored for all mobs with class/skills/cast/def per tier guidelines.
- [ ] `|` extension lines authored for elemental mobs.
- [ ] Loot tables (`l`/`L`) authored for loot-carrying mobs; `loot_amount` > 0; `L` values sum to ≤ 100.
- [ ] Items referenced by loot tables have `ITEM_LOOT` set.

### Phase 4: Object Construction (485–511, 27 objects)
- [ ] Build all 27 objects in ascending vnum order per this plan.
- [ ] No blank lines between object records in `#OBJECTS`.
- [ ] All objects carry `ITEM_TAKE` in wear flags.
- [ ] No object carries `ITEM_WEAR_CLAN_COLORS`.
- [ ] `ITEM_GENERATED` not set on any object.
- [ ] Weight archetype encoding correct (1–5 caster, 6–10 melee, 11–15 tank).
- [ ] Weapons (487, 488, 493, 496, 509) carry both `hold` and `take`.
- [ ] Two-handed weapon (509) carries `ITEM_TWO_HANDED`.
- [ ] `value3` thematically consistent for all weapons; `value3 = 0` not used.
- [ ] Boss items (506–510) carry `ITEM_BOSS` + `ITEM_LOOT`.
- [ ] Standard loot items (485–505) carry `ITEM_LOOT` only.
- [ ] Object names unique within area file.
- [ ] `E` extra descriptions end with exactly one trailing newline before `~`.
- [ ] `L` level entries formatted correctly (L on its own line, then level integer on next line).

### Phase 5: Shops, Resets, Specials, Objfuns
- [ ] No `#SHOPS` section needed (no shopkeepers).
- [ ] Write all `M` resets for mob placements; verify room vnums are valid.
- [ ] Write `O` reset for object 511 in room 583, limit 1.
- [ ] Write `G`/`E` resets for mob equipment per the reset section of this plan.
- [ ] Write `D` resets for locked door (558 → 559, state 2, key 511) — both sides.
- [ ] Verify `EX_ISDOOR` is set on both sides of every door reset to closed or locked.
- [ ] Verify key object 511 exists for the locked door.
- [ ] Write `#SPECIALS` section per the spec list in this plan.
- [ ] Write `#OBJFUNS` section — none planned; include `S` terminator.
- [ ] Verify `#RESETS` contains no blank lines.
- [ ] Verify all reset vnums reference valid targets.

### Phase 6: Final Validation
- [ ] Verify all vnums in `#ROOMS`, `#MOBILES`, `#OBJECTS`, `#SHOPS`, `#SPECIALS` stay within 441–590.
- [ ] Verify no duplicate vnums within any section type.
- [ ] Verify all text fields terminated with `~`; no `\n\n` in any string.
- [ ] Verify no vnum appears in any in-world description text.
- [ ] Verify color-code policy: no `@@k`, no background codes, no `@@f`, every fragment terminated with `@@N`.
- [ ] Verify canonical section order: `#AREA`, `#ROOMS`, `#MOBILES`, `#OBJECTS`, `#SHOPS`, `#RESETS`, `#SPECIALS`, `#OBJFUNS`, `#$`.
- [ ] Verify `area.lst` ordering is preserved (441 minimum vnum, between `withered_depths.are` 291–440 and `thornwood.are` 441–590; ordering already correct since this is a rewrite of the existing entry).
- [ ] Run `cd src && make unit-tests` and confirm all tests pass including integration test.
- [ ] Confirm graveyard.are connector room has reciprocal exit to 441.

