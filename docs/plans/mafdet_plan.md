# Area Plan: Mafdet (Full Thematic Build — Port City at the Edge of Law and Sea)

## Overview

- **Area Name:** `@@y@@xPort Mafdet@@N`
- **File Name:** `mafdet.are`
- **Keyword:** `mafdet`
- **Level Range:** `55-80` (`I 55 80`)
- **Vnum Range:** `14200-14399` (`V 14200 14399`)
- **Total Room Envelope:** **200 rooms** (all room vnums in-range must be used)
- **Design Goal:** Build a lore-dense maritime port city at the eastern terminus of the Saltglass Reach, where inland desert law and maritime contract law collide. The area combines a dangerous coastal approach through the Tidemouth Dunes, a factional trade city driven by the Strandline Compact's tripartite governance, and a smuggler-haunted tidewall undercroft beneath the seaward perimeter. The city is not a safe hub — it is a contested jurisdictional frontier where every corridor carries the tension between oasis charter formulas and the pragmatism of the open sea.

---

## Lore

See `docs/lore/mafdet_lore.md` for city lore, `docs/lore/saltglass_reach_lore.md` for the inland approach and Reach corridor context.

---

## Color Theme (Multi-Color, Lore-Matched)

Use a controlled multi-color palette tied to setting layers:

- `@@l` **Littoral Blue**: harbor water, tide glyphs, Strand Folk basalt foundations, Measure Script echoes.
- `@@y` **Ledger Gold**: Saltweight scales, Shoreward Tables, charter seals, tariff inscriptions.
- `@@b` **Strand Brown**: Tidemouth dunes, weathered timber piers, caulking resin, dried salt crust.
- `@@R` **Claw Red**: Shrine of the First Claw iconography, Swift Court verdicts, guardian warnings.
- `@@g` **Salt Gray-Green**: barnacle-crusted stone, tidal mineral deposits, corroded bronze fittings.
- `@@W` **Sail White**: signal tower lanterns, salt-whitened seawalls, gull feathers, Quay Concord sails.
- `@@N` reset after each themed segment.

Theme restrictions (spec aligned):
- No `@@k` as thematic foreground.
- No flashing code `@@f`.
- No background codes `@@0`-`@@7`.

---

## Core Narrative Direction

Mafdet is the **Jurisdictional Threshold**: the single point where an entire inland civilization's legal, economic, and funerary systems encounter the sea and are forced to adapt or break. The area is structured around three intersecting tensions:

1. **Inland vs. Maritime Law** — Every cargo, contract, and obligation must pass through the Shoreward Conversion, translating fixed-geography desert law into fluid-geography maritime law. The Ledger Houses manage this translation; the Red Sand Accounts exploit it.
2. **Commerce vs. Cult** — The Shrine of the First Claw enforces oath-sealed trust in a port where commercial flexibility demands speed over scrutiny. The guardian tradition says deal honestly or face the claw; the harbor says load the ship before the tide turns.
3. **Surface vs. Undercroft** — Mafdet's official institutions operate in daylight on the harbor front and the Ledger Quarter. Below, the Tidewall's flooded undercroft conceals Synod smuggling cells moving relic contraband through channels that bypass every inspection point.

Players traverse from the Saltglass Reach's final dunes through the Landward Gate, into the factional city, down to the harbor, and optionally into the dangerous tidewall undercroft where the area's hardest encounters await.

---

## Cross-Area Lore Anchors

The build must explicitly align with established corridor lore:

- **Saltglass Reach**: The Tidemouth Dunes are the Reach's terminal band. Reach Warden transit seals, cairn-station tablet notation, and Glasswalk Guide route songs appear in approach-zone room details and extra descriptions.
- **Great / Northern / Southern Oases**: Witness-seal formulas, ration-stamp conventions, and Saltweight calibration references appear in the Ledger Quarter and on cargo documentation objects.
- **Pyramids (all three traditions)**: Relic provenance marks and Quarantine Ledger hazard classifications appear in the Quarantine Ledger district and on relic-class objects.
- **Eastern Desert**: Charter-stone geometry and caravan toll notation appear on cargo arriving through the Carters' Yard.
- **Kowloon**: Jade Clerk notation parallels appear in Ledger House archives, reflecting the Littoral Branch connection.
- **Midgaard**: Violet Compact registry references appear in dual-authentication documentation and Quarantine Ledger dispute materials.
- **Khar'Daan**: Funerary-bureaucratic quarantine traditions appear in the Quarantine Ledger's containment protocols.
- **Arroyo**: Thirst-oath vocabulary fragments appear on adapted cairn tablets in the approach zone.

---

## `#AREA` Header Requirements (strict)

- `Q 16` mandatory.
- `K mafdet~`
- `L @@W(@@l55 80@@W)@@N~`
- `N` assigned during implementation (must not conflict with existing area numbers).
- `I 55 80`
- `V 14200 14399`
- `F 15`
- `O Virant~`
- `U @@lthe harbor chain of @@yPort Mafdet@@l descends with the dawn@@N~`

---

## Global Spec Compliance Rules

Implementation must follow `docs/area_file_spec.md` requirements, including:

- All strings `~`-terminated.
- No back-to-back newline runs in strings.
- No vnums in any in-world text.
- Room descriptions: minimum 3 sentences; major rooms minimum 5.
- Unique room descriptions throughout the area.
- Mobile `long_descr`: one text line only, then `~` line.
- Mobile `description`: exactly one trailing newline before `~`.
- Named exits must be text-anchored in room description, object text, or room extra description.
- Exits bi-directional and topologically coherent.
- All mobs flagged `stay_area`.
- Boss mobs flagged `sentinel` + `boss`, placed only in `no_mob` rooms.
- Solo (strong non-boss) mobs flagged `solo`.
- Mobs must be `sentinel` or enclosed by `no_mob` boundary rooms within their sub-region.
- All objects must include `ITEM_TAKE` in wear flags.
- Item stats are runtime-generated; area files define identity/behavior data only.
- Loot-table items must have `ITEM_LOOT` set; boss-drop items must have `ITEM_BOSS` set.

---

## Room Topology (All 200 Rooms Used)

The layout follows Mafdet's organic coastal geography rather than a rectangular grid. The city sits on a natural cove: the landward approach comes from the west through the Tidemouth Dunes, the harbor wraps around the eastern cove, the Shrine Terrace rises above the city center, and the Tidewall forms the seaward perimeter with a hidden undercroft below. Elevation changes use up/down exits. Multiple branching paths and loops create a navigable but non-linear topology.

### Topology Diagram (schematic, not to scale)

```
                          SHRINE TERRACE (F)
                            /    |    \
                     [up]  /     |     \  [up]
                          /      |      \
   TIDEMOUTH    LANDWARD   CARTERS    LEDGER     HARBOR FRONT (D)
   APPROACH --> GATE    --> YARD   --> QUARTER -->  /    |    \
     (A)         (A)        (B)        (C)      Piers Quays Signal
                                                  |         Tower
                                              TIDEWALL (E)
                                               /       \
                                          Upper      Lower
                                         Tidewall   Undercroft
                                                    [down]
                                                   Smuggler
                                                   Passages
```

The area has six districts connected by multiple paths. No district is a dead-end; there are always at least two routes between major districts, preventing bottleneck traps and encouraging exploration.

---

### District A: Tidemouth Approach & Landward Gate (14200-14229, 30 rooms)

**Theme:** The final band of the Saltglass Reach — brackish channels, crab-burrowed dunes, and the jurisdictional threshold where Reach authority ends and Mafdet's begins. The Shoreward Seal marks the legal boundary. The approach is dangerous: beach jackals, tidepool hazards, and opportunistic cairn scavengers prey on weary caravans.

**Sector types:** desert (10) for dune rooms, field (2) for tidal flats, city (1) for gate rooms.

**Layout concept:** A winding path through dune ridges and tidal channels, not a straight road. The approach splits into a northern high-dune path and a southern tidal-flat path that reconverge at the Landward Gate. Side branches lead to scavenger camps, abandoned assessment halls, and a collapsed cairn station.

Key anchors:
- `14200` **Tidemouth Trail, Western Edge** (entry room, 5+ sentences, connection to Saltglass Reach area).
- `14204` **Brackish Channel Crossing** (tidal flat hazard, crab burrows, movement challenge).
- `14208` **Collapsed Cairn Station** (Reach Warden ruin, lore extra descriptions for tablet fragments and toll-schedule inscriptions).
- `14212` **Scavenger Ridge Camp** (cairn scavenger encampment, hostile).
- `14216` **The Shoreward Seal** (basalt boundary stone, legal threshold landmark, 5+ sentences, extra descriptions for both inland charter notation and maritime loading symbols).
- `14220` **Landward Gate Approach** (caravan staging yard, first view of Mafdet's walls).
- `14224` **The Landward Gate of Mafdet** (fortified entry, 5+ sentences, no_mob, door reset closed).
- `14228` **Gate Customs Yard** (inside the wall, first city room, transition to District B).

Sub-region boundary: rooms 14224 (no_mob) and 14228 (no_mob) prevent approach-zone mobs from wandering into the city.

### District B: Carters' Yard & Market Streets (14230-14264, 35 rooms)

**Theme:** The Sand-Sea Carters' Guild's operational headquarters and the commercial strip that connects the gate to the city interior. Noisy, dusty, and crowded — the smell of draft animals, leather, and chalky Reach dust mixes with the first hints of salt air from the harbor. The Guild controls throughput; the Red Sand Accounts watch for distressed cargo to acquire.

**Sector types:** city (1) for streets/yards, inside (11) for buildings.

**Layout concept:** A broad main road (the Carter's Way) runs east from the gate toward the harbor, with the Carters' Yard sprawling to the north and a market bazaar branching to the south. Side streets create a loose grid in this district, but building footprints break the regularity. The Yard has a stable compound, harness workshops, and a waystation depot.

Key anchors:
- `14230` **Carter's Way, Western End** (main road beginning, dust and noise).
- `14234` **Sand-Sea Carters' Guild Hall** (Guild headquarters, inside, sentinel NPCs, extra descriptions for guild charter and waystation maps).
- `14238` **Caravan Assembly Yard** (open staging area, draft animals, wagon repair).
- `14242` **Red Sand Accounts Field Office** (inside, Red Sand broker NPC, ominous legal documents visible).
- `14246` **Market of the Shoreward Tables** (open-air bazaar, stone conversion-ratio charts mounted on walls, 5+ sentences).
- `14250` **Provisioner's Row** (food, water, and travel supplies).
- `14254` **Waystation Supply Depot** (Guild emergency stores, glasswalk shoes, cached water).
- `14258` **Carter's Way, Eastern End** (transition toward Ledger Quarter, salt air strengthening).
- `14262` **Southern Market Alley** (narrow commercial lane, pickpocket danger).

### District C: Ledger Quarter (14265-14304, 40 rooms)

**Theme:** The documentary heart of Mafdet — dense, functional architecture built for fire resistance and archive preservation. Thick walls, deep basements, wind-baffled courtyards. The air smells of wax, bitumen, lamp oil, and old paper. Four Ledger Houses compete for commercial influence, and the Quarantine Ledger processes the most dangerous artifacts in the corridor.

**Sector types:** city (1) for streets/courtyards, inside (11) for ledger halls and archives.

**Layout concept:** A dense cluster of interconnected courtyards and covered passages, with Ledger House buildings branching off as dead-end interiors. The district is organized around a central Assessment Courtyard where bulk cargo is weighed against the Saltweight standard. The Quarantine Ledger occupies the district's southeastern corner, deliberately isolated from the main document halls. A locked archive vault (key required) protects the most valuable records.

Key anchors:
- `14265` **Ledger Quarter Gate** (entry from Carter's Way, thick walls, wind baffles).
- `14269` **Tide Ledger Hall** (largest Ledger House, inside, 5+ sentences, extra descriptions for Saltweight reference scales and calibration marks).
- `14273` **Shoreward House** (inland-maritime conversion specialists, inside, bilingual legal inscriptions).
- `14277` **Assessment Courtyard** (central open space, bronze scales, cargo weighing, 5+ sentences).
- `14281` **Storm Ledger Vault** (bonded escrow storage, inside, locked door requiring Storm Ledger Key).
- `14285` **Quarantine Ledger Office** (relic hazard assessment, inside, sealed containers, cold-radiating artifacts).
- `14289` **Sealed Archive Passage** (locked door requiring Archive Key, leads to deep record vaults).
- `14293` **Deep Archive Vault** (oldest records, Littoral Branch tablets, Kowloon connection references, inside, 5+ sentences).
- `14298` **Quarantine Isolation Chamber** (Class Three containment, dangerous relic storage, no_mob, 5+ sentences).
- `14302` **Ledger Quarter, Harbor Approach** (transition to Harbor Front, first view of the water).

### District D: Harbor Front & Piers (14305-14349, 45 rooms)

**Theme:** The working heart of Mafdet — salt-whitened seawalls, timber piers on stone foundations, rope-burned bollards, and the constant sound of wave impact, gull cries, and loading operations. The harbor is a natural cove reinforced with stone breakwaters. The harbor chain stretches across the mouth, raised at dusk and lowered at dawn. Ship captains of the Quay Concord negotiate cargo terms while dock workers load under the Harbor Wardens' supervision.

**Sector types:** city (1) for harbor streets, water_swim (6) for shallow harbor, water_noswim (7) for deep harbor/breakwater, inside (11) for harbor buildings.

**Layout concept:** The harbor wraps around the cove in a crescent shape. The northern arm holds the main commercial piers and loading quays. The central harbor front is the widest section with the Harbormaster's office, signal tower, and Captain's Register hall. The southern arm curves toward the Tidewall. A breakwater extends east into the water, accessible by a narrow stone causeway. The harbor floor drops from shallow (swim) to deep (no-swim) water at the breakwater line.

Key anchors:
- `14305` **Harbor Gate** (entry from Ledger Quarter, first full view of the cove, 5+ sentences).
- `14309` **North Pier, Cargo Loading** (timber on stone, Strand Folk basalt visible at low waterline).
- `14313` **Commercial Quay** (bulk cargo transfer, Saltweight verification point).
- `14317` **Harbormaster's Office** (inside, sentinel NPC, Harbor Register, 5+ sentences).
- `14321` **Signal Tower Base** (stone tower, colored glass lanterns, extra descriptions for signal codes).
- `14325` **Signal Tower Crown** (up from base, panoramic view, extra descriptions for harbor chain and breakwater, 5+ sentences).
- `14329` **Captain's Register Hall** (inside, Quay Concord headquarters, parallel record to Harbor Register).
- `14333` **South Pier, Bonded Cargo** (escrow-sealed shipments, Storm Ledger jurisdiction).
- `14337` **Harbor Chain Winch House** (mechanism for raising/lowering the harbor chain, inside).
- `14341` **Breakwater Causeway** (narrow stone path extending into the water, exposed to weather).
- `14345` **Breakwater End** (furthest point into the harbor, deep water on three sides, no_mob, 5+ sentences).
- `14349` **Harbor Front, Southern Curve** (transition toward Tidewall district).

### District E: Tidewall & Undercroft (14350-14384, 35 rooms)

**Theme:** The seaward defensive perimeter. The upper Tidewall is built into the wall's thickness — expensive, wet, and prestigious residential and commercial space. Below, the Tidewall's foundations conceal a flooded undercroft: tidal pools, collapsed passages, and smuggler routes used by Synod cells to move relic contraband. This is the area's primary danger zone.

**Sector types:** city (1) for upper Tidewall, inside (11) for Tidewall buildings, water_swim (6) for flooded undercroft passages, dark rooms for deep undercroft.

**Layout concept:** The upper Tidewall runs north-south along the seaward side, with residential and commercial rooms branching off the wall-walk. Access to the undercroft is through a hidden stairway (down exit with named keyword, discoverable via extra description). The undercroft is a network of flooded passages, tidal pools, and collapsed chambers. It is NOT a maze (no ROOM_MAZE flags) — exits are bi-directional and logically consistent — but the tight, branching layout and dark rooms create navigational challenge. The deepest section contains the Synod smuggling cell's operations center.

Key anchors:
- `14350` **Tidewall Walk, Northern End** (entry from harbor, salt spray, barnacle-crusted stone).
- `14354` **Tidewall Residences** (shell-inlay facades, beacon sconces, Strand Folk remnant architecture).
- `14358` **Strand Rememberer's Alcove** (inside, Strand Rememberer elder NPC, oral tradition keeper, 5+ sentences, extra descriptions for coastal navigation songs and guardian origin stories).
- `14362` **Tidewall Walk, Southern End** (the wall curves, older construction visible).
- `14364` **Tidewall Undercroft Stair** (hidden down exit, discoverable, transition to lower level).
- `14366` **Flooded Foundation Passage** (dark, ankle-deep water, tidepool eels).
- `14370` **Collapsed Tidal Chamber** (partially flooded, unstable footing).
- `14374` **Smuggler's Cache** (hidden storage, Synod contraband, extra descriptions for mislabeled relic crates).
- `14378` **Synod Operations Cell** (no_mob, 5+ sentences, the smuggling cell's command post, ritual materials and funerary components visible).
- `14382` **Flooded Relic Vault** (deepest undercroft room, Class Three artifact sealed here, humming at dawn, cold to the touch, no_mob, 5+ sentences — boss encounter).

### District F: Shrine Terrace & Upper City (14385-14399, 15 rooms)

**Theme:** The elevated judicial and religious center of Mafdet. The Shrine of the First Claw sits on a basalt terrace visible from both the harbor and the Ledger Quarter. The claw-marked spire is the tallest structure in the city. The terrace is public space: anyone can approach to file a complaint, witness an oath, or observe a Swift Court session. The guardian tradition provides moral gravity without theological complexity.

**Sector types:** city (1) for terrace and approaches, inside (11) for Shrine interiors.

**Layout concept:** The Shrine Terrace is accessed via stone stairways (up exits) from the Ledger Quarter and the Harbor Front. The terrace is a broad platform with the Shrine at its center, the Swift Court on the western face, and the Oath Chamber in the Shrine's interior. The terrace offers views in multiple directions. The area culminates here with the most powerful guardian-cult encounters.

Key anchors:
- `14385` **Terrace Stair, Ledger Side** (up from Ledger Quarter, carved basalt steps).
- `14387` **Terrace Stair, Harbor Side** (up from Harbor Front, steeper ascent).
- `14389` **The Shrine Terrace** (broad basalt platform, claw-marked spire visible, 5+ sentences, extra descriptions for claw iconography and public verdict boards).
- `14391` **Swift Court of the First Claw** (open-air judicial space, verdict tablets on display boards, 5+ sentences).
- `14393` **The Oath Chamber** (inside, the First Claw sculpture — worn basalt feline forepaw, 5+ sentences, extra descriptions for oath-sealing ritual and claw marks).
- `14395` **Shrine Sanctum** (inner shrine, no_mob, guardian presence, 5+ sentences).
- `14397` **Terrace Overlook, Harbor View** (panoramic view of the harbor and breakwater).
- `14399` **Terrace Overlook, Reach View** (panoramic view of the Tidemouth Dunes and Saltglass Reach).

---

## Encounter and Faction Design

### Primary Factions

1. **Harbor Wardens** (law enforcement, sentinel):
   - Enforce the Strandline Compact's authority over port operations.
   - Patrol the Harbor Front and Landward Gate.
   - Non-aggressive unless provoked; some rooms are safe due to Warden presence.

2. **Ledger House Clerks** (bureaucratic NPCs, sentinel):
   - Staff the four Ledger Houses and the Quarantine Ledger.
   - Non-combatant service NPCs providing quest hooks and commercial context.

3. **Sand-Sea Carters' Guild** (caravan operators):
   - Control throughput between the Reach and the harbor.
   - Guild guards protect the Carters' Yard; some are aggressive to thieves.

4. **Red Sand Accounts** (predatory financiers):
   - Field agents and enforcers operating from their office in the Carters' Yard.
   - Red Sand Outriders patrol the Tidemouth approach, targeting distressed caravans.
   - Aggressive enforcers in contested commercial zones.

5. **Shrine of the First Claw** (guardian cult, judicial enforcers):
   - Oath-enforcement tradition descended from the guardian Mafdet.
   - Shrine adjudicators and claw-marked enforcers on the Terrace.
   - Non-aggressive but formidable if oath-breaking is detected.

6. **Quay Concord** (ship captains and sailors):
   - Professional maritime alliance prioritizing harbor reliability.
   - Crews on the piers and in the Captain's Register Hall.
   - Mostly non-aggressive; drunk sailors can be hostile.

7. **Synod Smuggling Cells** (hidden enemies):
   - Eclipsed Tongues faction operating in the Tidewall Undercroft.
   - Moving relic contraband by sea to avoid oasis scrutiny.
   - Aggressive throughout the undercroft; the primary combat faction.

8. **Strand Rememberers** (cultural preservers):
   - Small, non-combatant group in the Tidewall.
   - Provide lore context and quest hooks through oral tradition.

9. **Tidemouth Wildlife** (hostile fauna):
   - Beach jackals, tidepool eels, harbor gulls (approach zone and harbor edges).
   - Tidemouth crabs (approach zone, burrow-collapse hazard).

### Mob Vnum Plan

- Use mob vnums sequentially within `14200-14399`.
- Target **48-55 mob templates** with strict role separation:
  - Approach-zone wildlife (6-8 templates): beach jackals, tidemouth crabs, tidepool eels, cairn kites, glass scorpion stragglers.
  - Approach-zone hostiles (4-5 templates): cairn scavengers, Red Sand outriders, Reach patrol deserters.
  - City ambient/non-combatant (8-10 templates): dock workers, caravan drovers, merchants, clerks, sailors, guild workers.
  - City law/sentinel (4-5 templates): Harbor Wardens, gate guards, Guild guards, Shrine enforcers.
  - City service NPCs (8-10 templates): shopkeepers, Harbormaster, Ledger House heads, Strand Rememberer elder.
  - Undercroft hostiles (6-8 templates): Synod smugglers, Synod couriers, Synod ritual acolytes, Synod cell leader.
  - Bosses (3 templates):
    - **Mini-boss** at `14298` — Quarantine Ledger Abomination (containment breach, animated hazard-class relic).
    - **Major boss** at `14382` — Synod Eclipsed Tongue Hierophant (cell leader, caster-profile, sanctuaried).
    - **Final boss** at `14395` — The Wrath of Mafdet (guardian manifestation, feline-form spirit, triggered by oath-violation or Synod presence, boss+sentinel+no_flee, in no_mob room).

### Mob Level Targets

| District | Level Range | Notes |
|----------|------------|-------|
| A: Tidemouth Approach | 55-62 | Wildlife and scavengers; onboarding difficulty |
| B: Carters' Yard | 58-65 | Red Sand enforcers and commercial-zone threats |
| C: Ledger Quarter | 60-68 | Mixed; mostly safe interiors with dangerous archive guardians |
| D: Harbor Front | 62-72 | Hostile sailors, dock thugs, harbor-edge wildlife |
| E: Tidewall Upper | 64-70 | Moderate; transitional |
| E: Tidewall Undercroft | 68-78 | Synod cells, flooded-passage hazards; primary combat zone |
| F: Shrine Terrace | 72-80 | Elite shrine enforcers and boss encounters |

### Service NPC Levels

- **Service/sentinel NPCs** (shopkeepers, Harbormaster, Ledger House heads): level `150` baseline for resilience.
- **Law enforcement NPCs** (Harbor Wardens, gate guards): level `120-140`.
- **Ambient city NPCs** (dock workers, merchants, clerks): level `40-55`.

---

## Object Plan

### Object Vnum Plan

- Use object vnums sequentially in-range (`14200-14399`).
- Target **55-65 objects**, including:

**Approach-zone items (vnums 14200-14214):**
- Glasswalk shoes (footwear, chitin-and-resin soled sandals).
- Tidemouth crab shell fragment (armor piece or treasure).
- Cairn scavenger's pry bar (weapon).
- Shattered cairn tablet fragment (treasure, extra description with toll-schedule inscription in Survey-Administrative voice).
- Salt-crusted waterskin (drink container).
- Red Sand debt instrument (scroll/quest item, extra description with exploitative loan terms).
- Reach Warden transit seal (quest item, extra description with cairn-station certification language).
- Beach jackal pelt (treasure).

**City equipment and trade goods (vnums 14215-14249):**
- Saltweight bronze reference measure (treasure, extra descriptions with Great Oasis Measure calibration marks).
- Shoreward Table fragment (treasure, stone slab with conversion-ratio charts, extra description in Legal-Conversion voice).
- Ledger House wax seal stamp (held item).
- Harbor Warden's enforcement cudgel (weapon, pound type).
- Storm Ledger escrow token (quest item).
- Quarantine Ledger hazard tag (quest item, containment class notation).
- Sand-Sea Carter's harness kit (treasure).
- Draft-depth marker rod (held item, Littoral Branch notation).
- Various shop items for provisioners and traders.

**Harbor and maritime items (vnums 14250-14269):**
- Harbor chain link (treasure, inscribed with Littoral Branch weather glyphs, extra description).
- Signal tower lantern glass (colored, different types for different messages).
- Captain's Register entry scroll (quest item).
- Dock worker's cargo hook (weapon, pierce type).
- Caulking resin block (treasure/trade good).
- Sailcloth wrap (armor, about-wear).
- Anchor stone fragment (treasure, Strand Folk era).

**Shrine and guardian items (vnums 14270-14284):**
- Swift Court verdict tablet (held item, wax-sealed, extra description with verdict text in Guardian-Judicial voice).
- Claw-marked oath stone (quest item, four parallel grooves).
- Shrine adjudicator's blade (weapon, slice type, ceremonial but functional).
- Guardian talisman (neck-wear, feline iconography).
- First Claw inscription rubbing (scroll, extra description with guardian origin text).

**Undercroft and Synod items (vnums 14285-14304):**
- Synod ritual mask (face-wear, Eclipsed Tongues iconography).
- Smuggler's manifest (scroll, false cargo entries, extra description).
- Mislabeled relic crate lid (treasure, Synod classification forgery visible).
- Eclipsed tongue amulet (neck-wear, glass-shard pendant from Glasswind Belts).
- Funerary ritual component (quest item, relic-class material the Synod is smuggling).
- Waterlogged quarantine seal (treasure, dissolved Class Two containment tag).

**Key objects (required for locked doors):**
- Storm Ledger Vault Key (key, opens the Storm Ledger Vault door in District C).
- Archive Passage Key (key, opens the Sealed Archive Passage door in District C).
- Undercroft Grate Key (key, opens the grate to the deep undercroft in District E).

**Boss-signature items (vnums 14305-14314, ITEM_BOSS + ITEM_LOOT):**
- Quarantine Abomination's Containment Shard (weapon or held, from mini-boss).
- Hierophant's Eclipsed Tongue Stave (weapon, staff-like, from major boss, two-handed).
- Mafdet's Claw (weapon, claw type, from final boss — guardian-blessed, swift justice motif).
- Relic of the Shoreward Seal (held item, from final boss — basalt fragment with dual-jurisdiction inscription).
- Strandline Compact Signet (finger-wear, from major boss — legal authority symbol).

### Itemization Intent

- Blend harbor-operational motifs (rope, tar, bronze, salt-whitened stone) with oasis-administrative motifs (wax seals, measure rods, charter tablets).
- Include visible inscriptional language in `E` extra descriptions using the four diegetic voices from lore:
  - **Harbor-Operational** on pier and loading equipment.
  - **Legal-Commercial** on Ledger House and assessment objects.
  - **Guardian-Judicial** on Shrine and oath objects.
  - **Strand Folk-Oral** on the oldest items and Tidewall artifacts.
- Keyed doors (Storm Ledger Vault, Sealed Archive, Undercroft Grate) each have explicit key objects that drop from appropriate mobs or are found in specific rooms.

---

## Progression and Difficulty Curve

- **14200-14229 (District A):** Onboarding (55-62). Wildlife, scavengers, environmental hazard rooms. Introduces the Saltglass Reach transition and the Shoreward Seal landmark.
- **14230-14264 (District B):** Commercial pressure (58-65). Red Sand encounters, pickpocket danger, caravan-lane control. Introduces factional tension.
- **14265-14304 (District C):** Bureaucratic danger (60-68). Mostly safe interiors with dangerous archive guardians and the Quarantine containment breach mini-boss. Key-locked progression gates.
- **14305-14349 (District D):** Harbor operations (62-72). Hostile sailors, dock-area ambushes, water-based encounters. The breakwater is exposed and dangerous.
- **14350-14384 (District E):** Tidewall escalation (64-78). Upper Tidewall is moderate; the undercroft ramps to the area's highest sustained difficulty. Synod cells are organized and dangerous.
- **14385-14399 (District F):** Culmination (72-80). Shrine Terrace elite encounters and the final boss.

Boss cadence:
- Mini-boss at `14298` (Quarantine Abomination, level 70, solo-flagged).
- Major boss at `14382` (Synod Hierophant, level 76, boss-flagged).
- Final boss at `14395` (Wrath of Mafdet, level 80, boss-flagged, no_flee, sentinel).

---

## Reset and Door Plan

### Door Plan

Locked doors only where lore supports institutional barriers. All locked doors require valid key objects.

1. **Landward Gate** (District A, `14224`):
   - Direction: east (into city) / west (out to approach).
   - `<locks>`: `door` bit set. `<key_vnum>`: `-1` (no key, unlockable).
   - `#RESETS` command `D`: state `1` (closed on reset, not locked).
   - Reverse exit on `14228` (or connected room) must also have `door` bit set.

2. **Storm Ledger Vault** (District C, `14281`):
   - Direction: contextual (into vault).
   - `<locks>`: `door` + `locked` + `pickproof`. `<key_vnum>`: Storm Ledger Vault Key vnum.
   - `#RESETS` command `D`: state `2` (locked on reset).
   - Key drops from Storm Ledger Clerk mob or is found in the Ledger Quarter.

3. **Sealed Archive Passage** (District C, `14289`):
   - Direction: contextual (into deep archive).
   - `<locks>`: `door` + `locked` + `pickproof`. `<key_vnum>`: Archive Passage Key vnum.
   - `#RESETS` command `D`: state `2` (locked on reset).
   - Key drops from Tide Ledger Hall head or is found in the assessment courtyard.

4. **Undercroft Grate** (District E, `14364`):
   - Direction: down (into undercroft).
   - `<locks>`: `door` + `locked`. `<key_vnum>`: Undercroft Grate Key vnum.
   - `#RESETS` command `D`: state `2` (locked on reset).
   - Key drops from a Tidewall NPC or is discoverable in the upper Tidewall.

### Spawn Distribution

- **District A**: Sparse wildlife spawns on approach paths; denser scavenger spawns near camp rooms. 12-15 mob resets.
- **District B**: Moderate city ambient spawns; Red Sand enforcers in 2-3 rooms. 15-18 mob resets.
- **District C**: Low combat density; Ledger House service NPCs fill most rooms. 10-12 mob resets.
- **District D**: Moderate harbor ambient spawns; hostile encounters on pier edges and breakwater. 18-22 mob resets.
- **District E upper**: Light ambient spawns. 5-7 mob resets.
- **District E undercroft**: Dense Synod combat spawns; the area's heaviest encounter concentration. 12-15 mob resets.
- **District F**: Sparse but elite; boss rooms have single powerful spawns. 6-8 mob resets.

---

## Specials Plan

| Mob Role | Spec Function | Location | Notes |
|----------|--------------|----------|-------|
| Gate Guard Captain | `spec_guard` | 14224 | Law enforcement at Landward Gate |
| Harbor Warden Captain | `spec_guard` | 14317 | Law enforcement on Harbor Front |
| Shrine Enforcer | `spec_cast_judge` | 14391 | Swift Court judicial caster |
| Quarantine Abomination | `spec_cast_undead` | 14298 | Mini-boss, relic-animated entity |
| Synod Hierophant | `spec_cast_cleric` | 14382 | Major boss, Eclipsed Tongues leader |
| Wrath of Mafdet | `spec_cast_bigtime` | 14395 | Final boss, guardian manifestation |
| Synod Courier | `spec_thief` | Undercroft | Smuggler with theft behavior |
| Dock Scavenger | `spec_fido` | Harbor edges | Picks up dropped items |
| Beach Jackal | `spec_poison` | Approach zone | Venomous bite |
| Glass Scorpion Straggler | `spec_poison` | Approach zone | Highly venomous |

---

## Connection Plan (External Links)

- **West (14200):** Connection to the Saltglass Reach area (when built). Currently a terminal room with descriptive text indicating the Reach extends westward.
- **No other external connections initially.** Mafdet is the eastern terminus; the sea is east, not another area. Future connections could include a sea-route link to Kowloon or Rakuen.

---

## Room Description Requirements (Spec Compliance)

- Every room `<description>~` must contain at least 3 sentences.
- Important rooms (entry, gate, Shoreward Seal, Assessment Courtyard, Harbormaster, Signal Tower Crown, Shrine Terrace, Oath Chamber, boss rooms, breakwater end) must contain at least 5 sentences.
- Every room description must be unique (no reused descriptions across rooms).
- Vnums must never appear in any in-world description text.
- Named exits (`<exit_keyword>`) must be discoverable in the room's `<description>~`, an object in the room, or an `E` extra description.
- Extra-description keyword chains must be anchored: at least one keyword in each chain must appear in the room's main `<description>~`.
- Room descriptions must use the area's color theme intentionally and consistently.
- Each district should employ the appropriate diegetic voice(s) from the lore style guide:
  - **District A**: Survey-Administrative voice (cairn inscriptions) + Guide-Oral voice (wayfinding fragments).
  - **District B**: Harbor-Operational voice (loading orders, Guild directives).
  - **District C**: Legal-Commercial voice (manifest certifications, conversion documentation).
  - **District D**: Harbor-Operational voice (berth orders, storm advisories, chain signals).
  - **District E**: Strand Folk-Oral voice (oldest carvings, folk wisdom) + Survey-Administrative voice (Synod's corrupted versions).
  - **District F**: Guardian-Judicial voice (oaths, verdicts, warnings).

---

## Implementation Checklist

1. Populate all 200 rooms with unique names, descriptions, exits, and extra descriptions within vnum range `14200-14399`.
2. Build topology following the organic coastal layout — verify bi-directional exits, no illegal loops, no ROOM_MAZE unless intentional maze.
3. Create all mob templates (48-55) with proper act flags, level assignments, and faction alignment.
4. Create all object templates (55-65) with proper item types, wear flags, extra descriptions, and inscriptional language.
5. Create 3 key objects for locked doors; verify every locked-on-reset door has a valid key vnum and corresponding key object.
6. Build `#SHOPS` entries for provisioners and traders in Districts B and D.
7. Build `#RESETS` with spawn distribution matching the plan density per district.
8. Build `#SPECIALS` entries for all mobs with special functions.
9. Assign `#OBJFUNS` if any objects warrant special behavior.
10. Align all strings and structures to `docs/area_file_spec.md` constraints.
11. Validate no-vnum-in-text policy across all descriptions.
12. Verify boss mobs are `sentinel` + `boss` + `no_flee` in `no_mob` rooms.
13. Verify all mobs have `stay_area` and are either `sentinel` or enclosed by `no_mob` boundary rooms.
14. Run area parser/load checks and in-game traversal sanity pass.
15. Run `cd src && make unit-tests` to validate.
