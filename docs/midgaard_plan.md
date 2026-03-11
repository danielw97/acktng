# Area Plan: Midgaard

## Overview
- **Area Name (current):** `@@WThe City of @@yMidgaard@@N`
- **File Name:** `midgaard.are`
- **Keyword:** `midgaard`
- **Current Vnum Range:** `3001-3200`
- **Vnum Envelope Size:** 200
- **Current Room Utilization:** 200/200 rooms used (`3001-3200`)
- **Current Mobile Utilization:** 56 mobiles in-range (up to `3161`)
- **Current Object Utilization:** 183 objects in-range (up to `3198`)
- **Layout Basis:** Preserve the existing implemented city footprint and connectivity, then expand by deepening district identity, quest/lore hooks, NPC roles, and room narrative differentiation (not by changing the core map geometry).

---

## Spec-First Constraints (from `/docs`)
1. Keep all authored content inside the assigned envelope `3001-3200`.
2. Maintain the existing topological city layout; expansions are additive in detail and function, not a re-architecture.
3. Use intentional inline color coding with a defined palette; avoid forbidden thematic black (`@@k`) and avoid flashing/background color codes.
4. Ensure future implementation aligns area-header policy (`Q 16`, owner line should be `O Virant~` when normalized).
5. Preserve all external world links already implemented from Midgaard:
   - `3005 -> 30100` (Gloamvault link)
   - `3014 -> 9551` (Dungeon link)
   - `3025 -> 21000` (Ossuary/transport court link)
   - `3030 -> 30175`
   - `3041 -> 3230`
   - `3060 -> 8500`
   - `3189 -> 5600`
   - `3190 -> 7302`

---

## Existing Layout Interpretation (Preserve and Enrich)
Midgaard already reads as a **20x10 civic lattice** with repeated avenue naming motifs and several specialized civic/service pockets. The plan preserves this shape and formalizes it into district identities:

1. **Civic Spine District (central market-law corridor)**
   - Includes major named boulevards such as Cedar/Silver/Crimson/Auburn corridors and Lantern/Sunward/Granite/Saffron branches.
   - Function: trade, legal notices, guild traffic, cityguard visibility.

2. **Reliquary + Carceral Axis**
   - Anchors: `3005` (**Violet Reliquary Arch**) and `3014` (**Dungeon Intake Yard**).
   - Function: controlled transition to high-risk/occult and penal spaces.

3. **Sanctuary and Service Quarter**
   - Existing healer/postal/trade service rooms and temple-adjacent flow.
   - Function: practical city sustainment, religious legitimacy, traveler support.

4. **Newer Arcana-Requisition Strip (already present in high vnums)**
   - Anchors: `3122-3128`, `3161-3163` style rooms/NPCs.
   - Function: modernized outfitting, proposition/quest brokerage, invasion logistics.

5. **Gateward Connectors**
   - Rooms with external exits become explicitly themed as managed boundaries between Midgaard and linked regions.

---

## Multi-Color Theme: “Lantern-Civic Spectrum”
A five-color civic palette that matches Midgaard’s existing naming and atmosphere:

- **Civic White (`@@W`)**: law, stonework, proclamations, institutions.
- **Lantern Gold (`@@y`)**: tradeflow, market warmth, wayfinding, public life.
- **Ward Cyan (`@@c`)**: sigils, registry marks, sanctioned arcana, records.
- **Reliquary Violet (`@@p`)**: old oaths, subterranean thresholds, contested memory.
- **Iron Red (`@@R`)**: militia, alarms, emergency mobilization.
- **Reset (`@@N`)** always used to prevent color bleed.

### Thematic Usage Rules
- Street names keep restrained color usage (`@@W` titles, selective accent words).
- Institutional rooms (post office, armories, broker halls) get stronger `@@c`/`@@y` accents for readability.
- Reliquary and dungeon transitions use controlled `@@p` + `@@c` interplay.
- Do not introduce `@@k`, flashing, or background color codes.

---

## Lore Framework (Extensive, Canon-Connected)

### Chronological Reference Spine (for consistent writing)
Use this timeline backbone when authoring room text, NPC dialogue, and object lore so references remain consistent across all Midgaard sub-districts:

1. **Founding Generation (First Stone Accord):** road-security, militia compacts, barter tribunals.
2. **Standardization Century (Lantern Reforms):** paving marks, posted tariffs, district lamp-lines, watch rotations.
3. **Containment Era (Violet Compact):** registry-based occult oversight, reliquary checkpoints, oath-ledger protocols.
4. **Fortification Era (Ash-and-Iron Decades):** requisition offices, reserve depots, intake bureaucracy, emergency charter law.
5. **Modern Tension Era:** continuity-vs-reckoning politics, archive disputes, public-memory campaigns.

Every major room cluster should visibly reflect at least two eras (e.g., old stone + new signage), reinforcing Midgaard as an accreted civic system rather than a single-age city build.

### I. The First Stone Accord
Midgaard began as a fortified caravan exchange where three pressures met: river-borne grain traffic, upland ore caravans, and temple pilgrim routes. Early governance was transactional and fragile—merchant charters and militia contracts rather than true civic law. The city’s oldest surviving doctrine, called the **First Stone Accord**, established a practical principle still visible in street design: *all roads must return to a watched square*. This is why the present lattice feels repetitive yet deliberate; repetition was a security technology.

### II. The Lantern Reforms
As population outgrew ad hoc ward captains, Midgaard instituted the **Lantern Reforms**: standardized paving marks, posted tariffs, fixed public notice points, and rotating night patrol routes tied to lantern lines. This era explains Midgaard’s recurring imagery of lampstrings, railings, kiosks, and boards. The city became legible to strangers on purpose—merchants needed predictable navigation, and magistrates needed predictable enforcement.

### III. The Violet Compact
The rise of catacombs, reliquaries, and sealed vault approaches forced a compromise between civic order and forbidden knowledge. The **Violet Compact** legalized tightly bounded occult handling under registry supervision. The Violet Reliquary Arch and related ward-signage rooms are physical remnants of this compromise: Midgaard neither embraced nor eradicated the occult; it bureaucratized it.

### IV. The Ash-and-Iron Decades
Regional instability (including northern supply disruptions and cross-city conflict eras echoed in other plans) pushed Midgaard toward militarized provisioning. This phase introduced intake yards, quartermaster doctrine, and requisition language that survives in newer NPC titles (Invasion Quartermaster, Proposition Clerk, Quest Broker). Midgaard’s identity shifted from “merchant city with guards” to “civic fortress that still trades.”

### V. The Ledger of Debts (Cross-Area Tie-in)
Consistent with broader world plans, Midgaard’s trade bureaucracy has historical entanglements with morally compromised institutions: condemned-labor transfers, sealed warrants, and arcane consulting contracts. This history survives as rumor, redacted entries, and competing narratives inside guild and temple archives. It gives Midgaard a layered identity: benevolent public order above unresolved institutional guilt.

### VI. Modern Midgaard Tension
Today Midgaard is split between two civic philosophies:
- **Continuity Faction:** keep strict routines, preserve proven systems, prioritize stability.
- **Reckoning Faction:** expose buried records, revise old compacts, humanize city law.

This ideological tension drives room text, NPC dialogue, quest hooks, and factional mob behavior without altering the existing room graph.

### VII. The Seven Watches of Midgaard
Midgaard’s daily rhythm is divided into seven civic “watches” rather than simple dawn/noon/dusk references. This is both lore flavor and a practical authoring tool for ambient text variation:

1. **Kindling Watch:** lamplighters extinguish dawn lamps; bakers and carriers dominate traffic.
2. **Ledger Watch:** notaries, tariff clerks, and petitioners fill the market-law corridors.
3. **Hammer Watch:** artisans and armorers create the city’s loudest period.
4. **Heat Watch:** temple kitchens and public fountains become social centers.
5. **Bell Watch:** official notices read aloud; contracts and warrants close for the day.
6. **Lantern Watch:** patrol strength increases; reliquary routes tighten oversight.
7. **Ash Watch:** low-traffic curfew hours, janitorial and ward-inspection labor.

**Plan usage:** assign each district two dominant watches and one “stress watch” (when conflict is most likely), then reflect this in room sensory detail and NPC schedule phrasing.

### VIII. Founding Institutions and Their Cultural Memory
Midgaard’s institutions are not only functional structures but competing memory engines. Expansion should reflect how each institution narrates the same history differently:

- **The Magistrate Ledgerhouse:** teaches that order created prosperity; minimizes moral compromise in archive records.
- **The Temple of the Resounding Heart:** frames city history as collective penance and duty to the uncounted dead.
- **The Guard Command:** elevates siege-memory and defensive necessity; treats legal ambiguity as wartime pragmatism.
- **Guild Concordat Halls:** prioritize contract continuity and supply-chain stability over ideological purity.
- **Lantern Registry Clerks:** preserve mundane civic truth (routes, names, dates) and become accidental custodians of inconvenient facts.

This layered memory model should inform quest sources: the same historical event can produce conflicting but valid mission briefings.

### IX. The Three Civic Oaths (living doctrine)
Midgaard’s public trust is anchored in three spoken oaths still used in ceremonies, sentencing, and recruitment:

1. **Oath of Light:** “No ward unlit, no traveler uncounted.”
2. **Oath of Stone:** “No judgment hidden, no sentence unrecorded.”
3. **Oath of Bread:** “No levy taken that cannot be borne.”

These oaths are idealized and often violated in practice; this gap is a key source of narrative friction and side-quest design.

### X. The Quiet Scandal: The Unindexed Years
Between the late Containment Era and early Ash-and-Iron period, several year-ledgers are missing, altered, or split into duplicate books with different signatures. Publicly this is attributed to fire, mold, and wartime relocation. Privately, both political camps weaponize the uncertainty:

- **Continuity argument:** records were triaged to preserve core governance during crisis.
- **Reckoning argument:** records were intentionally sanitized to conceal unlawful transfers and punitive abuses.

Area-writing impact:
- Archive rooms should include redacted plaques, replaced shelf labels, and contradictory inventory marks.
- NPC rumor chains should never deliver total certainty; they should deliver credible fragments.

### XI. Midgaard Folk Belief and Street Myth
Beyond formal institutions, Midgaard’s residents carry practical superstitions that can be woven into object flavor and room atmosphere:

- Lanterns facing outward ward off debt-collectors and bad verdicts.
- A coin left on a fountain lion requests safe return for a traveling companion.
- Violet chalk on a threshold means “witness requested” in old neighborhood custom.
- Bells rung twice after midnight are interpreted as memorial, not alarm.

These beliefs humanize otherwise bureaucratic spaces and create non-combat interaction opportunities.

### XII. Seasonal Cycle and Civic Festivals
To deepen lore cadence, Midgaard uses civic festivals tied to infrastructure and memory rather than pure harvest mythology:

1. **Night of First Lamps:** commemorates the Lantern Reforms; routes are relit in ceremonial order.
2. **Day of Open Ledgers:** public reading of tariffs, pardons, and civic debts.
3. **Ironfast Week:** reserve drills, ration practice, and volunteer wall-watch.
4. **Violet Silence:** one evening of quiet observance for names removed from official rolls.

Plan application:
- Festival traces appear year-round in room details (faded pennants, archived notices, carved tally marks).
- Select NPCs reference “last festival incident” to seed ongoing quest continuity.

### XIII. Social Strata and Daily Survival
Midgaard’s prosperity is uneven; expansion should foreground social texture across districts:

- **Ledgered citizens:** guild-registered, legally visible, tax-enrolled; stable but scrutinized.
- **Day-mark laborers:** legal but precarious workers paid by stamped day tokens.
- **Shadow residents:** undocumented dwellers in service corridors and intake-adjacent districts.

This structure supports grounded quest design (identity papers, wages withheld, mistaken warrants, mercy appeals) and prevents lore from becoming purely top-down institutional history.

### XIV. External Diplomacy Without Borders Myth
Midgaard presents itself as neutral and practical, but diplomacy is managed as controlled permeability:

- Entry routes are tolerated when they sustain civic logistics.
- Arcane inflow is accepted only through documented chains.
- Foreign militias are welcomed as temporary auxiliaries, never as autonomous power.

This doctrinal posture ties naturally to exit-room flavor and clarifies why boundary rooms should feel administratively dense.

### XV. Narrative Pillars for Future Content
All future Midgaard lore additions should reinforce at least one of these pillars:

1. **Order as infrastructure** (roads, lamps, ledgers, routines).
2. **Mercy versus mechanism** (human judgment inside rigid systems).
3. **Memory versus utility** (truth preservation against crisis pragmatism).
4. **Threshold governance** (how the city manages dangerous edges without collapsing commerce).

These pillars keep new writing coherent across dozens of rooms and many NPC authors.

---

## District-by-District Content Expansion Plan (Using Existing Rooms)

## A. Lantern Market Belt (western/central commercial corridors)
**Goal:** convert currently repetitive avenue descriptions into sub-district personalities while preserving traffic flow.

- Differentiate contiguous streets into micro-themes:
  - weights/measures lane
  - food and staples lane
  - contracts and writ lane
  - durable goods lane
- Add recurring environmental markers every 3-4 rooms (notice poles, water troughs, militia chalk marks, shrine niches).
- Tie ambient text to time-of-day reset messaging (market dawn setup, dusk closure bells).

**Room update style:**
- Keep existing names where strong.
- Rewrite generic repeated description blocks so each corridor segment signals function and social class.

## B. Reliquary-Dungeon Threshold Arc (`3005`, `3014`, connected approaches)
**Goal:** make the city’s occult/carceral threshold narratively coherent.

- `3005` remains the formal warded descent point; expand description to include registry sigils, oath plaques, and controlled access protocol.
- `3014` becomes intake bureaucracy + moral pressure point (scribes, bonds, mercy petitions).
- Approach rooms gain visual escalation: brighter civic colors fading into violet-cyan ward language.
- Add rumor-bearing low-level NPCs (clerks, petitioners, exhausted guards).

## C. Temple and Civic Care Arc
**Goal:** unify healer, sexton, and sanctuary-adjacent rooms as a visible social safety network.

- Clarify distinction between charitable healing, paid treatment, and funerary duties.
- Introduce memorial boards and public rites references in room descriptions.
- Create light quest hooks around missing persons, burial records, and ward maintenance offerings.

## D. Arcana / Outfitters / Requisitions Expansion (`3122-3128`, `3161-3163` cluster)
**Goal:** formalize this as Midgaard’s “new bureaucracy of conflict readiness.”

- Themed as a post-crisis civic complex where adventurers process gear, contracts, and sanctioned risk.
- Distinct counters/offices:
  - proposition adjudication
  - invasion reserve logistics
  - quest brokerage archives
  - armor specialization courts (cloth/melee/tank)
- Make each service room textually unique with queue behavior and document culture.

## E. Gateward Connections (all out-of-area exit rooms)
**Goal:** make each external link unmistakable in lore and wayfinding.

- Each link room receives a canonical “boundary identity” (signage, customs marks, patrol notes, travel advisories).
- Add consistent boundary objects: milestone plaques, city seal pillars, posted risk bulletins.
- NPC patrol dialogue references destination-specific threats/opportunities.

---

## Mobile/NPC Plan

### Preserve and Recontextualize Existing Core NPCs
- Keep iconic Midgaard roles: mayor, town crier, cityguards, postmaster, healer, bartender, artisan vendors, janitor, flavor civilians/animals.
- Refresh descriptions/dialogue to reflect district identity and current civic tensions.

### New NPC Additions (within free vnums `3162-3200`)
Proposed additions focus on lore delivery and district function:

1. **Registry Archivist of Lantern Hall** (lawful lore source, ledger quests)
2. **Ward-Sigil Inspector** (reliquary threshold gatekeeping)
3. **Mercy Petitioner** (dungeon intake empathy hook)
4. **Supply Sergeant of the Reserve** (requisition chain quests)
5. **Night Lamplighter-Captain** (patrol and city rumor delivery)
6. **Temple Memorist** (names of dead, continuity-vs-reckoning arc)
7. **Road Warden Liaison** (links to northern forest traffic)
8. **Contract Notary of Granite Arcade** (trade/legal missions)

### Mob Behavior and Placement Principles
- Keep high guard visibility near portals and civic hubs.
- Use non-combat civic NPCs as breadcrumb narrators.
- Place wildlife/flavor mobs in quieter wards only; avoid immersion breaks in formal civic interiors.
- Maintain strict role clarity for shopkeepers and service specialists.

---

## Object Plan

### Preserve Existing Economy Objects
- Keep core shop stock identities (weapons, armor, provisions, arcana, consumables).

### Additive Object Themes (remaining free object vnums in-range)
1. **Civic documents:** stamped writs, tariff forms, sealed warrants, pardon requests.
2. **Wayfinding assets:** district maps, milestone slates, lantern registry tags.
3. **Ritual/ward items:** chalk sigils, ward tethers, reliquary tokens.
4. **Memory artifacts:** memorial ribbons, ledger fragments, old oath medallions.
5. **Boundary markers:** gate advisories tied to each external route.

### Flavor + Quest Utility
- Ensure a subset of new objects are read/examined for lore progression.
- Use document chains to connect market, temple, reliquary, and requisition districts.

---

## Room Rewrite and Differentiation Strategy
Because all 200 room vnums are already occupied, expansion is delivered through **qualitative room differentiation**:

1. **Tier 1 (landmarks):** major hubs receive bespoke multi-paragraph descriptions and lore anchors.
2. **Tier 2 (corridors):** repeated base templates replaced with district-specific variants (materials, sounds, professions, signage).
3. **Tier 3 (connectors):** external-link rooms gain directional identity and travel-state cues.
4. **Tier 4 (service interiors):** each shop/service room gains explicit operational flavor and social context.

This preserves existing mechanics while making traversal feel like meaningful urban progression rather than repeated text blocks.

---

## Cross-Area Lore Integration Plan
Tie Midgaard’s updated narrative to established world plans without changing physical exits:

- **Great Northern Forest:** Granite Arcade records and roadwarden traffic reporting.
- **Kowloon:** legal/registry parallels, shared bureaucratic lineage, comparative civic doctrine.
- **Kiess:** Roc Road commerce continuity, military and provisioning doctrine exchange.
- **Sunken Sanctum / deeper arcane history:** legacy of sealed warrants and ethically compromised archival records.

Integration method:
- rumor lines,
- document objects,
- NPC dialogue trees,
- room inscriptions and plaques.

---

## Implementation Phases (Plan Only)

### Phase 1: Canon Audit and Theming Pass
- Inventory all Midgaard room texts and classify by district.
- Assign Lantern-Civic palette usage per district.
- Mark existing repetitive descriptions for rewrite priority.

### Phase 2: Landmark Rewrite
- Rewrite key anchor rooms first (`3005`, `3014`, `3049`, temple/post office/arcana hubs, external connectors).
- Align lore vocabulary across civic, temple, and requisition hubs.

### Phase 3: NPC + Dialogue Expansion
- Update existing NPC descriptions/dialogue.
- Add new civic/lore NPCs in free vnum space (`3162-3200`).
- Wire dialogue cues for cross-area hooks.

### Phase 4: Object + Reset Enrichment
- Add lore documents and boundary markers in remaining object vnums.
- Update resets to populate new ambient civic life without overcrowding.

### Phase 5: Cohesion and Spec Validation
- Verify vnum containment (`3001-3200` only).
- Verify color policy compliance and readability.
- Verify external links unchanged and clearly signposted.
- Verify room/mob/object text avoids vnum mentions.

---

## Success Criteria
1. Midgaard remains mechanically recognizable and fully compatible with existing world routes.
2. Traversal now communicates clear district identity and historical depth.
3. Lore links Midgaard naturally to the wider campaign geography and institutional history.
4. Service/NPC ecosystem feels civic, lived-in, and narratively purposeful.
5. All planned additions remain within current Midgaard vnum envelope and area specification constraints.
