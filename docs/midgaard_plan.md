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

## Spec-First Constraints (from `area_file_spec.md`)

### Area Header
1. `Q 16` directive is mandatory.
2. Owner line must be `O Virant~` (area header owner policy).
3. `V 3001 3200` defines the vnum envelope; all authored rooms, mobiles, and objects must stay inside this range.

### Vnum Allocation
4. Vnums for rooms, mobiles, and objects must be assigned in ascending sequential order with no gaps.
5. Fill existing gaps in the vnum range before assigning new higher vnums.
6. For rooms, attempt to use all room vnums in the assigned range (fill every available slot where practical).

### String and Description Rules
7. All text fields terminated by `~`. Strings must not contain back-to-back newlines (`\n\n`).
8. Vnums must never appear in any in-world description text (room descriptions, mob descriptions, object descriptions, extra descriptions, exit descriptions).
9. Mobile `long_descr` must be exactly one text line followed by a newline and a line containing only `~`. Multi-line `long_descr` is never valid.
10. Mobile `description` must end with exactly one newline immediately before the terminating `~`.
11. Object extra-description `<description>` must end with exactly one newline before `~`.
12. Room `<description>~` must end with exactly one trailing newline before `~`.

### Color Policy
13. Use intentional inline color coding with the defined Lantern-Civic palette (see below); avoid forbidden thematic black (`@@k`).
14. Do not use background color codes (`@@0`–`@@7`) or flashing (`@@f`) unless explicitly added by a human author.

### Layout Preservation
15. Maintain the existing topological city layout; expansions are additive in detail and function, not a re-architecture.
16. Preserve all external world links already implemented from Midgaard:
    - `3005 -> 30100` (Gloamvault link)
    - `3014 -> 9551` (Dungeon link)
    - `3025 -> 21000` (Ossuary/transport court link)
    - `3030 -> 30175`
    - `3041 -> 3230`
    - `3060 -> 8500`
    - `3189 -> 5600`
    - `3190 -> 7302`

### Room Constraints
17. Each room description must contain at least 3 sentences; important rooms must contain at least 5 sentences.
18. Each room description must be unique (no reuse of identical descriptions across rooms).
19. Exits must be bi-directional: if room A connects to room B via direction X, room B must provide the opposite-direction exit back to A (except in mazes).
20. When a room uses a named exit (`<exit_keyword>`), that exit name must appear in the room's main description, an object that spawns in the room, or an `E` extra description.
21. Extra-description keywords must be discoverable: at least one keyword in any chain must appear in the room's main description.
22. Rooms that are part of a maze must be flagged `ROOM_MAZE` (`524288`).

### Mobile Constraints
23. All mobs must be flagged `stay_area` (`64`) so they do not wander out of their home area.
24. Boss mobs must be flagged `sentinel` (`2`) and `boss` (`67108864`) and placed only in rooms flagged `no_mob` (`4`).
25. Strong (non-boss) mobs must be flagged `solo` (`33554432`).
26. The `invasion` flag must never be set by builders; it is runtime-only.
27. The `aggressive` flag should be set only for mobs whose intended behavior is to initiate combat on sight (feral beasts, hostile undead, demonic sentries), not for townsfolk, merchants, quest givers, or guards.
28. Sentinel mobs (gate guards, shopkeepers) should be flagged `sentinel` (`2`).

### Object Constraints
29. Every object must include `ITEM_TAKE` (`8388608`) in wear flags.
30. No object may include `ITEM_WEAR_CLAN_COLORS` (`16777216`).
31. Object `name`, `short_descr`, and `description` must be thematically consistent with the object's wear flags.
32. Object names must be unique within the area file (no duplicate item names).
33. Area-authored items must not define fixed stats; stats are generated at runtime.
34. Object weight encodes archetype: `1`–`5` caster, `6`–`10` melee, `11`–`15` tank.
35. `ITEM_GENERATED` (`1`) must not be set in area files; it is runtime-managed.
36. Items from boss mob loot tables must include `ITEM_BOSS` (`134217728`).
37. Items spawned from mob loot tables (`l`/`L` extension) must have `ITEM_LOOT` (`67108864`) set.
38. Weapons must have wear flags including both `hold` and `take`. Two-handed weapon names must include the `ITEM_TWO_HANDED` extra flag.
39. `value3 = 0` (`hit`) for weapons is only valid with the `ITEM_FIST` extra flag.

### Reset Constraints
40. Resets must reference valid vnums for the target type required by the command.
41. If a door is reset to `locked` via `D` state `2`, `<key_vnum>` must be set to a valid key object vnum (not `-1`), and that key object must exist in `#OBJECTS`.
42. `#RESETS` must not contain blank lines.

### Section Order
43. Follow canonical saver order: `#AREA`, `#ROOMS`, `#MOBILES`, `#MOBPROGS`, `#OBJECTS`, `#SHOPS`, `#RESETS`, `#SPECIALS`, `#OBJFUNS`, `#$`.

### Specials Policy
44. Mob specials beginning with `spec_summon_` must never be set in area files (runtime-only).
45. Mob specials beginning with `spec_keep` may only be assigned manually by a human.

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

### 0. Geographic Context: The Crossroads Position
Midgaard occupies a strategic position at the convergence of the continent’s primary trade corridors. To the north, the Great Northern Forest (and beyond it, Kowloon) provides timber, ore, and overland access to the delta ports. To the south and east, the Roc Road connects to Kiess, itself built on the ruins of fallen Evermeet. To the west, desert caravan routes reach the oasis settlements near the ancient pyramids. Below and around the city, sealed passages connect to the Gloamvault, the Dungeon, and other subterranean spaces that Midgaard’s civic order has never fully controlled.

This crossroads position is not accidental—it is the reason Midgaard exists. The city was never founded on ideology or conquest; it grew from the practical necessity of managing traffic, resolving disputes, and securing overnight shelter at the point where multiple dangerous routes converged. Every subsequent layer of governance, from the First Stone Accord to the modern Requisition offices, is an institutional response to the pressures created by geographic centrality.

### 0a. Pre-Founding Mythology: The Watchers’ Cairn
Before Midgaard was a city, before even the caravan camps, local oral tradition holds that the site was marked by a circle of cairn stones placed by an unknown people. These stones—called the "Watchers" in folk memory—were said to face outward in six directions, each cairn carved with a single open eye. No surviving cairn has been found intact, but fragments of worked stone with eye-motifs occasionally surface during foundation repair, and the Temple of the Resounding Heart maintains a collection of these fragments in its reliquary.

The Watchers’ Cairn myth serves several narrative functions:
- It places Midgaard’s site as inherently significant before human settlement—a watched place, not merely a convenient one.
- It connects to the broader pre-civilization mystery of the pyramids (Sand Sovereign, Moon Sovereign) and the Sunken Sanctum: all are remnants of a world that preceded the current age.
- It gives weight to the folk belief that Midgaard’s lanterns are successors to the cairn eyes—"the city that never stops watching" as a continuation of something older.
- It provides quest hooks for archaeological investigation: who carved the cairns? Are they related to the pyramid builders? What did they watch for?

### 0b. The River That Was
Geological evidence (mentioned in scattered archivist records and temple-floor mosaics) suggests that a major river once flowed through or near Midgaard’s current site, long since diverted or dried. The city’s well system taps into deep aquifers that may be remnants of this ancient waterway. The "river-borne grain traffic" mentioned in First Stone Accord accounts could refer to a period when water transport was still viable at the site.

The disappeared river explains several civic oddities:
- The deep-set foundation lines of older buildings, designed for flood resistance that modern residents find excessive.
- The city’s elaborate drainage engineering, more sophisticated than a landlocked city would typically require.
- Temple references to "the drowned road" and "the river’s debt"—phrases whose literal meaning has been forgotten but whose emotional weight persists in liturgy.

### I. The First Stone Accord
Midgaard began as a fortified caravan exchange where three pressures met: river-borne grain traffic, upland ore caravans, and temple pilgrim routes. Early governance was transactional and fragile—merchant charters and militia contracts rather than true civic law. The city’s oldest surviving doctrine, called the **First Stone Accord**, established a practical principle still visible in street design: *all roads must return to a watched square*. This is why the present lattice feels repetitive yet deliberate; repetition was a security technology.

The Accord was not a constitution but a mutual-defense pact between three founding groups: the **River Factors** (grain merchants who controlled barge traffic), the **Upland Syndicate** (ore and timber traders from the northern forest routes), and the **Temple Compact** (pilgrim-route chaplains who provided neutral arbitration). Each group contributed resources to a shared militia and agreed to submit disputes to public tribunal rather than private violence. The physical expression of this pact—the requirement that all roads converge on watched squares—became the architectural DNA of the city.

### II. The Lantern Reforms
As population outgrew ad hoc ward captains, Midgaard instituted the **Lantern Reforms**: standardized paving marks, posted tariffs, fixed public notice points, and rotating night patrol routes tied to lantern lines. This era explains Midgaard’s recurring imagery of lampstrings, railings, kiosks, and boards. The city became legible to strangers on purpose—merchants needed predictable navigation, and magistrates needed predictable enforcement.

### III. The Violet Compact
The rise of catacombs, reliquaries, and sealed vault approaches forced a compromise between civic order and forbidden knowledge. The **Violet Compact** legalized tightly bounded occult handling under registry supervision. The Violet Reliquary Arch and related ward-signage rooms are physical remnants of this compromise: Midgaard neither embraced nor eradicated the occult; it bureaucratized it.

The Compact established three principles that still govern Midgaard's relationship with arcane power:
1. **Registry primacy:** no arcane material may enter or leave the city without registry documentation. The Ward-Sigil Inspectors are direct descendants of the Compact's original checkpoint wardens.
2. **Containment over destruction:** dangerous artifacts and texts are sealed rather than destroyed, on the theory that destroyed knowledge becomes uncontrollable rumor while contained knowledge remains auditable.
3. **Civic oversight of thresholds:** every passage between Midgaard's surface and its subterranean connections (Gloamvault, Dungeon, sealed vaults) is a civic jurisdiction, not a private or temple domain.

The Compact's legacy is visible in the physical infrastructure: the Violet Reliquary Arch at room 3005 is not merely decorative but a functional registry checkpoint. Its violet lacquer, ward sigils, and posted civic notices are all elements mandated by the original Compact text. The approach rooms gain visual escalation—brighter civic colors fading into violet-cyan ward language—because the Compact required that travelers receive clear visual warning before crossing a threshold into controlled space.

The Compact also created the institutional framework that the Spirebound Conclave later exploited. When the Conclave needed experimental subjects, it was the Violet Compact's registry system that provided the administrative apparatus for documenting prisoner transfers as "arcane material handling." This is the deepest source of institutional guilt in Midgaard's history: the system designed to protect citizens was repurposed to process their condemnation.

### IV. The Ash-and-Iron Decades
Regional instability pushed Midgaard toward militarized provisioning. The specific crises were multiple and overlapping:
- **The Ashfall Monsoon:** Three years of black volcanic rain from the Cinderteeth eruptions darkened skies across the continent. Kowloon’s predecessor settlement was devastated (600 souls drowned in one night). Midgaard’s agricultural hinterland failed. Ash deposits contaminated wells. The Temple of the Resounding Heart still maintains memorial rolls from “the years the sun dimmed.”
- **The Evermeet Collapse:** The fortified city of Evermeet, Midgaard’s primary southern trade partner on the Roc Road, fell during the border wars. Refugees and displaced merchants flooded Midgaard’s intake systems. The modern city of Kiess was later built on Evermeet’s ruins, but for a generation, the Roc Road was a corridor of displacement rather than commerce.
- **Northern supply disruptions:** Ashfang warbands and other hostile groups seized control of forest road segments, cutting Midgaard’s access to northern ore and timber. The road warden system was formalized during this period as a military response to trade-route interdiction.
- **Conclave escalation:** With supply lines cut, the Spirebound Conclave in the Cinderteeth Mountains turned to coercion, demanding prisoner transfers from frontier cities in exchange for arcane defensive support. Midgaard’s sealed warrants from this period authorized the transfer of condemned debtors—a transaction that the modern Reckoning faction considers the city’s gravest institutional crime.

This phase introduced intake yards, quartermaster doctrine, and requisition language that survives in newer NPC titles (Invasion Quartermaster, Proposition Clerk, Quest Broker). Midgaard’s identity shifted from “merchant city with guards” to “civic fortress that still trades.”

The Ash-and-Iron Decades also explain why Midgaard’s newer institutional architecture (the Arcana-Requisition Strip, rooms 3122–3128 and 3161–3163) has a distinctly military-administrative character compared to the older civic-commercial core. These rooms were built under emergency charter law and never fully demilitarized.

### V. The Ledger of Debts (Cross-Area Tie-in)
Consistent with broader world plans, Midgaard’s trade bureaucracy has historical entanglements with morally compromised institutions: condemned-labor transfers, sealed warrants, and arcane consulting contracts. This history survives as rumor, redacted entries, and competing narratives inside guild and temple archives. It gives Midgaard a layered identity: benevolent public order above unresolved institutional guilt.

The specific institutional chain was:
1. Midgaard’s magistrates issued sealed warrants condemning debtors, petty criminals, and "unregistered persons" to labor service.
2. The Violet Compact’s registry apparatus processed these individuals as "arcane material" rather than citizens, stripping them of civic identity.
3. The Spirebound Conclave received these subjects for experimentation in their mountain laboratories—the Ember Canon (fire/will tests), Prism Thesis (crystal implantation), and Pale Ledger (spirit extraction) programs.
4. The Conclave’s eventual catastrophic collapse (seismic event plus internal sabotage) destroyed most direct evidence, but Midgaard’s trade ledgers retain the financial records of payment received.

**Parallel guilt in Kowloon:** Proto-Kowloon’s delta settlements made similar transfers. Kowloon’s legendary "Black Ledger"—said to be sealed in an undercity cistern—contains the original unredacted census including names of those transferred. Both cities’ Reckoning factions seek to expose these records; both cities’ Continuity factions seek to keep them sealed.

**Kiess’s inheritance:** Kiess’s Syndic Council archives contain an obsidian disc bearing the Conclave’s triune seal (Ember, Prism, Pale). This artifact confirms that the Conclave’s reach extended to Evermeet before its fall. Midgaard’s archives may contain corresponding seals or seal-impressions that have been reclassified or hidden.

**The Withered Depths consequence:** The Conclave’s field-trial sites in the forests—where crystals were implanted in root systems—may have mutated into the aggressive fungal networks that now constitute the Withered Depths. If confirmed, this means Midgaard’s sealed warrants indirectly contributed to a blight that threatens the northern forest trade routes the city depends on. This irony is a potent quest hook: the city’s past crimes are literally poisoning its present commerce.

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

### XV. The Cinderteeth Shadow: Volcanic Memory
The Cinderteeth Mountains—source of the Ashfall Monsoon, home of the Spirebound Conclave, and geological backbone of the continent's western ranges—cast a long shadow over Midgaard's institutional memory. Though the mountains are distant, their influence is woven into the city's architecture, language, and anxiety:

- **Ash-proof construction:** Many of Midgaard's older buildings use a distinctive mortar mixed with volcanic aggregate (imported during the Ash-and-Iron Decades). This mortar is recognizable by its gray-purple color and is mentioned in building-inspection records. Room descriptions in older districts can reference "the old ash-mortar walls" as environmental texture.
- **Cindertooth slang:** Midgaard residents use volcanic metaphors in daily speech: "ashfall weather" for any prolonged bad period, "cinder debt" for obligations that compound dangerously, "the mountain remembers" as a warning against institutional complacency.
- **Temple seismology:** The Temple of the Resounding Heart maintains a crude seismological watch, recording tremors and correlating them with Cinderteeth activity reports from Kowloon's Wardens of Iron. This creates a lore thread connecting the temple's spiritual mission (memory, penance) with practical disaster preparedness.

### XVI. The Cartographers' Dispute
Midgaard's official city map is a contested document. Three institutions maintain competing versions:
- **The Magistrate Ledgerhouse** publishes the "Civic Survey"—a functional grid showing property boundaries, tariff zones, and patrol routes. It deliberately omits subterranean connections and religious sites.
- **The Temple of the Resounding Heart** maintains the "Memory Map"—a liturgical document that includes memorial sites, old cemetery boundaries, and "the drowned road" of the disappeared river. It is more historically complete but less practically useful.
- **The Lantern Registry** keeps the "Night Route"—a patrol-optimized map showing lamplighter circuits, sight lines, and shadow pockets. It includes details the other maps suppress (blind alleys, unlit service corridors, vulnerable entry points).

No single map tells the complete truth. This is by design: each institution controls the spatial narrative that serves its institutional interests. Quest hooks can emerge from map discrepancies—a memorial site on the Temple map that doesn't appear on the Civic Survey, a shadow pocket on the Night Route that the Magistrate Ledgerhouse has classified as "cleared."

### XVII. The Merchant Tongues: Language and Code
Midgaard's market districts have developed specialized communication systems beyond Common:
- **Tally marks:** Merchants use a system of chalk marks on curbstones and doorframes to communicate price trends, credit risks, and customer reliability. These marks are not secret but are opaque to outsiders.
- **Bell code:** The city's bell system (referenced in the Bell Watch) communicates standardized messages: single strikes for time, double strikes for weather warnings, triple strikes for militia alerts, and a distinctive "rolling peal" for invasion mobilization. Two bells after midnight are memorial, not alarm—a folk custom that has become semi-official.
- **Seal language:** Official documents use wax seals with standardized iconography. A lion seal means magistrate authority, a lamp seal means Lantern Registry certification, a heart seal means temple attestation. Broken or re-pressed seals indicate document tampering—a visual cue for investigation quests.

### XVIII. The Undercity Question
Midgaard's relationship with its underground spaces is deliberately ambiguous in civic doctrine. The city acknowledges the existence of subterranean connections (Gloamvault via room 3005, Dungeon via room 3014) but treats them as managed thresholds rather than integral parts of the city. This creates a legal gray zone:
- Persons who descend are considered to have "left Midgaard" for jurisdictional purposes, even though the physical entry points are within city walls.
- The Violet Compact's registry system applies at thresholds but has no enforcement capacity below ground.
- Rumors of "shadow residents" living in service corridors and intake-adjacent spaces (see Social Strata, section XIII) blur the line between surface city and underground.

This ambiguity is intentional in both civic policy and area design: it allows Midgaard to maintain its identity as an ordered, lawful city while acknowledging that order has limits. The threshold rooms (3005, 3014) should feel like jurisdictional borders, not merely doorways.

### XIX. Narrative Pillars for Future Content
All future Midgaard lore additions should reinforce at least one of these pillars:

1. **Order as infrastructure** (roads, lamps, ledgers, routines).
2. **Mercy versus mechanism** (human judgment inside rigid systems).
3. **Memory versus utility** (truth preservation against crisis pragmatism).
4. **Threshold governance** (how the city manages dangerous edges without collapsing commerce).
5. **Consequence inheritance** (present-day effects of past institutional decisions—the Conclave transfers, the Unindexed Years, the blight in the forests).
6. **Geographic centrality as burden** (the crossroads position that sustains the city also forces it to manage every crisis that moves along its trade routes).

These pillars keep new writing coherent across dozens of rooms and many NPC authors.

### XX. The Living City: Ambient Detail Vocabulary
To prevent room descriptions from feeling generic, writers should draw from this curated vocabulary of Midgaard-specific ambient details:

**Sounds:** drill calls, bell peals, ledger-stamp thuds, fountain splashing, hymn fragments, cart-wheel rattle, militia boot-fall, lamplighter chain-clinks, baker's shutter-slam, petitioner murmuring.

**Smells:** wet stone, ash-mortar dust, beeswax polish, cedar shavings, hot iron scale, bread yeast, lamp oil, rain on flagstone, ink and parchment, temple incense.

**Visual textures:** inlaid ward sigils, painted survey marks, ashlar gutters, carved curbstones, violet lacquer, brass registry plates, prayer rails polished to a shine, memorial steles, chalk tally marks, posted tariff boards, lantern-string shadows.

**Weather/light:** lantern-glow warmth, morning fog off old foundations, ash-tinged sunset, rain pooling in gutter channels, shade cast by civic eaves, the particular quality of light through stained-glass temple windows.

**People in motion:** couriers with sealed pouches, fishmongers with hand-carts, militiamen in column, clerks with ledger-boards under arm, lamplighters with hooked poles, temple acolytes with ash-bowls, petitioners queuing at intake windows.

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

### Mob Spec Compliance and Placement Principles

**Required act flags for all mobs:**
- Every mob must have `stay_area` (`64`) set so it does not wander out of Midgaard.
- Stationary mobs (gate guards, shopkeepers, service NPCs) must also have `sentinel` (`2`) set.
- No mob should have `invasion` set; this is a runtime-only flag.

**Aggressive flag policy:**
- Only feral animals or hostile intruders should be flagged `aggressive` (`32`). Cityguards, merchants, civic NPCs, and quest givers must not be aggressive.
- Guards who react when provoked should use `spec_guard`/`spec_policeman` specials rather than the aggressive flag.

**Boss and solo policy:**
- Midgaard is primarily a civic area, so boss mobs are unlikely. If any are added, they must have `sentinel` + `boss` flags and be placed in `no_mob`-flagged rooms.
- Strong (non-boss) named NPCs (e.g., the Mayor, Guard Captain) should be flagged `solo` (`33554432`).

**Mobile description format (strict):**
- `long_descr` must be exactly one text line, then a newline, then `~`. Multi-line `long_descr` is never valid.
- `description` must end with exactly one newline before `~`.
- No vnum may appear in any mob description text.

**General placement principles:**
- Keep high guard visibility near portals and civic hubs.
- Use non-combat civic NPCs as breadcrumb narrators for lore delivery.
- Place wildlife/flavor mobs in quieter wards only; avoid immersion breaks in formal civic interiors.
- Maintain strict role clarity for shopkeepers and service specialists.
- Shopkeeper mobs must have corresponding `#SHOPS` entries with valid `buy_type` and hours.

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

### Object Spec Compliance

**Required for all objects:**
- Every object must include `ITEM_TAKE` (`8388608`) in wear flags.
- No object may include `ITEM_WEAR_CLAN_COLORS` (`16777216`).
- Object `name`, `short_descr`, and `description` must be thematically consistent with wear flags (e.g., a `head` item must read as headgear).
- Object names must be unique within the area file; no two objects may share the same `<name>~` value.
- Do not define fixed stats in area files; stats are generated at runtime.
- Do not set `ITEM_GENERATED` (`1`) in area files; it is runtime-managed.

**Weight archetype encoding:**
- `1`–`5`: caster item
- `6`–`10`: melee item
- `11`–`15`: tank item

**Weapon requirements:**
- Weapons must have `item_type = 5` (`ITEM_WEAPON`) and wear flags including both `hold` (`32768`) and `take` (`8388608`).
- `value3` (weapon attack type) must be thematically consistent with the weapon's name and description.
- `value3 = 0` (`hit`) is only valid with the `ITEM_FIST` extra flag.
- Two-handed weapon names (e.g., "great axe") must include `ITEM_TWO_HANDED` (`2147483648`) extra flag.

**Extra description format:**
- `E` entry descriptions must end with exactly one trailing newline before `~`.
- No vnums may appear in any object description text.

### Flavor + Quest Utility
- Ensure a subset of new objects are read/examined for lore progression.
- Use document chains to connect market, temple, reliquary, and requisition districts.
- Document objects (writs, warrants, ledger fragments) should use `E` extra descriptions to provide readable text when examined, with keywords discoverable from the object's `short_descr`.

---

## Room Rewrite and Differentiation Strategy
Because all 200 room vnums are already occupied, expansion is delivered through **qualitative room differentiation**:

1. **Tier 1 (landmarks):** major hubs receive bespoke multi-paragraph descriptions and lore anchors. These are "important rooms" under the spec and must contain at least 5 sentences.
2. **Tier 2 (corridors):** repeated base templates replaced with district-specific variants (materials, sounds, professions, signage). Must contain at least 3 sentences.
3. **Tier 3 (connectors):** external-link rooms gain directional identity and travel-state cues. Must contain at least 3 sentences.
4. **Tier 4 (service interiors):** each shop/service room gains explicit operational flavor and social context. Must contain at least 3 sentences.

This preserves existing mechanics while making traversal feel like meaningful urban progression rather than repeated text blocks.

### Room Spec Compliance Checklist
- Every room description must contain at least 3 sentences (5 for important/landmark rooms).
- Every room description must be unique; no identical descriptions across rooms.
- Room descriptions must end with exactly one trailing newline before `~`.
- No vnums may appear in room descriptions, exit descriptions, or extra descriptions.
- Exits must be bi-directional: if room A connects to room B via direction X, room B must provide the opposite-direction exit back to A. (External links to other areas are exempt where the other area controls the return exit.)
- When a room uses a named exit (non-empty `<exit_keyword>`), the exit name must appear in the room's main description, an object that spawns in the room, or an `E` extra description.
- Extra-description keyword chains must be anchored: at least one keyword in each chain must appear in the room's main description.
- Named exit keywords for door-style exits must be prefixed with `^` (e.g., `^iron gate`).
- Any exit set to `closed` or `locked` via `#RESETS` command `D` must have `EX_ISDOOR` (`1`) set in its room `D<door>` definition.
- If a door is reset to `locked` (state `2`), `<key_vnum>` must reference a valid key object, and that key must exist in `#OBJECTS`.
- Rooms that are part of a maze must be flagged `ROOM_MAZE` (`524288`). Midgaard is not expected to contain mazes.
- Boss mob rooms must be flagged `no_mob` (`4`).

---

## Cross-Area Lore Integration Plan

Tie Midgaard’s updated narrative to the established world plans without changing physical exits. Each connection below includes specific lore threads to embed in room descriptions, NPC dialogue, object flavor text, and extra descriptions.

### Great Northern Forest (vnums 15500–15999)
- **Physical link:** Room 15512 south exit connects to Midgaard room 3009 (Granite Arcade). This is Midgaard’s primary northern gate.
- **Road Wardens:** Midgaard funds and dispatches road wardens who maintain the Lantern Road through the forest. The Granite Arcade should reference ranger guild contracts, supply manifests, and patrol postings.
- **Trade traffic:** Northern ore caravans and timber shipments pass through the forest to reach Midgaard. Market rooms should reference seasonal supply disruptions when Ashfang warbands or Rootbound cultists block the road.
- **Intelligence flow:** Forest scouts report sightings to Midgaard’s Guard Command. Requisition rooms should include mentions of bounty postings for Ashfang raiders and warnings about Thornmother cultist activity.
- **Historical roots:** Midgaard’s founding as a "fortified caravan exchange" is directly tied to the forest corridor’s role as a trade route. The First Stone Accord’s principle of "all roads must return to a watched square" was developed in response to ambush risks on forest roads.

### Kowloon (vnums 14000–14099)
- **Trade corridor:** Midgaard and Kowloon are connected via the Great Northern Forest (Midgaard → forest → Kowloon Iron Gate). Merchant traffic between the two cities sustains both economies.
- **Shared bureaucratic lineage:** Both cities developed registry systems, lantern-based wayfinding, and tariff frameworks independently but in parallel. Midgaard’s Lantern Reforms and Kowloon’s Courier Lantern Office share conceptual DNA from frontier logistics needs.
- **Ashfall Monsoon echoes:** Kowloon was founded after the devastating three-year Ashfall Monsoon from the Cinderteeth eruptions. Midgaard’s archives should contain contemporary accounts of ash-darkened skies, disrupted trade, and refugee columns. Temple memorial rooms should reference "the years the sun dimmed."
- **Neon Covenant parallel:** Kowloon’s founding Neon Covenant (six houses) parallels Midgaard’s First Stone Accord. Both are founding-generation compacts born from crisis. NPC dialogue can draw this comparison explicitly.
- **Shared Conclave guilt:** Both proto-Midgaard and proto-Kowloon supplied condemned debtors to the Spirebound Conclave (see Sunken Sanctum). Midgaard’s trade ledgers and Kowloon’s Black Ledger both contain records of these transfers. This shared institutional shame is a key lore thread for both cities’ Reckoning factions.
- **Wardens of Iron:** Kowloon’s Wardens of Iron maintain the Iron Gate and frontier patrols. Midgaard’s Guard Command cooperates with them through dispatches carried by road wardens.

### Kiess (vnums 13000–13099)
- **Roc Road:** The primary overland trade route connecting Kiess and Midgaard. Kiess’s south gate opens onto the Roc Road. Market rooms and trade-corridor descriptions should reference Roc Road commerce, caravan schedules, and trade-agreement treaties.
- **Evermeet legacy:** Kiess was built on the ruins of Evermeet, which fell during the same frontier chaos that shaped Midgaard’s Ash-and-Iron era. Midgaard’s archives may contain pre-collapse correspondence with Evermeet administrators—now of antiquarian interest and political sensitivity.
- **Military doctrine exchange:** Kiess’s Wall Command and Midgaard’s Guard Command share provisioning doctrine and emergency charter frameworks developed during the border wars. Requisition rooms should reference "Kiessward supply protocol" or similar cross-city military coordination.
- **Central Prism:** Kiess’s central plaza houses a recovered Evermeet artifact. Midgaard’s temple or archivist NPCs could reference rumors about the Prism’s origin and power.
- **Shared three-power governance:** Kiess is governed by Compact Temples, Trade Syndics, and Wall Command—a tripartite structure that mirrors Midgaard’s own civic/temple/military institutional balance.

### Sunken Sanctum / Spirebound Conclave (vnums 9500–9599)
- **The Conclave connection:** The Spirebound Conclave was a mountain academy in the Cinderteeth Range tasked with preserving dangerous magical knowledge. When border wars cut their supply lines, they turned to coercion—accepting condemned debtors and prisoners from frontier cities (including proto-Midgaard) as experimental subjects.
- **Midgaard’s trade ledger:** Midgaard’s archives contain entries recording payment of condemned debtors to the Conclave. These records are central to the Continuity-vs-Reckoning political tension. The Continuity faction claims the transfers were wartime triage under duress; the Reckoning faction argues they were knowing complicity in atrocity.
- **The Unindexed Years:** The gap in Midgaard’s year-ledgers (between late Containment Era and early Ash-and-Iron period) likely coincides with the peak of Conclave transfers. Archive rooms should include redacted entries, replaced shelf labels, and contradictory inventory marks that hint at deliberate sanitization.
- **Conclave’s three factions:** The Ember Canon (fire/will), Prism Thesis (crystal/structure), and Pale Ledger (spirit/testimony) each left traces in Midgaard’s institutional memory. Temple records may reference "the fire scholars," arcanist registries may note "prism-sealed reagent shipments," and judicial archives may contain testimony transcripts from "the pale clerks."
- **Sealed warrants:** Midgaard issued sealed warrants authorizing prisoner transfers. Some of these warrants may still exist in archive rooms, redacted but recoverable—a quest hook for players investigating the Conclave legacy.

### Withered Depths (vnums 1000–1149) and Verdant Depths (vnums 10000–10124)
- **Indirect connection:** Both areas are accessible via the Forest of Confusion, itself connected to the Great Northern Forest network. Midgaard has no direct physical link but is affected by the ecological consequences.
- **Conclave blight theory:** The Withered Depths’ corruption may originate from orphaned Conclave field-trial sites where implanted crystals mutated into aggressive fungal networks. Midgaard’s arcanist registry should contain reports of "anomalous decay patterns" forwarded by forest scouts.
- **Supply chain impact:** Blight spreading from the Withered Depths affects timber and herb quality along forest trade routes. Market NPCs could reference "tainted northern timber" or "short herbalist shipments" as ambient economic flavor.
- **Kiess scout reports:** Kiess scouts have reported both Verdant Depths and Withered Depths accessible via the Roc Road and Forest of Confusion. These reports would be forwarded to Midgaard’s road warden command as intelligence bulletins.

### Northern Pyramid (vnums 10125–10324) and Southern Pyramid (vnums 11450–11649)
- **Pre-civilization mysteries:** Both pyramids predate all living civilizations. The Northern Pyramid (built by the Sand Sovereign) rises into the sky; the Southern Pyramid (built by the Moon Sovereign) descends into the earth. These structures are beyond Midgaard’s institutional memory but exist as myths and traveler tales.
- **Scholar interest:** Midgaard’s archivist and temple NPCs could reference "the twin monuments of the deep desert" as subjects of academic curiosity—structures that no living civilization claims credit for building.
- **Desert trade rumors:** Traders arriving from the southern oasis routes bring stories of the pyramids. Market-district NPCs could relay fragmented accounts of "a tower that reaches the sun" and "a pit that swallows the moon."
- **Ashfall connection:** The Cinderteeth volcanic range that caused the Ashfall Monsoon is geologically connected to the desert regions where the pyramids stand. Temple scholars might speculate about whether the Sand Sovereign and Moon Sovereign understood volcanic forces that modern civilizations still fear.

### Integration Methods
Embed cross-area references through:
- **Room inscriptions and plaques:** carved names, dates, and treaty references in civic architecture.
- **NPC rumor lines:** ambient dialogue fragments that reference events, places, and people from connected areas.
- **Document objects:** stamped writs, intelligence bulletins, trade manifests, sealed warrants, and archived correspondence.
- **Extra descriptions:** wall carvings, notice boards, shelf labels, and redacted archive entries discoverable through `look` commands.
- **Quest hooks:** conflicting accounts of shared historical events that point players toward investigation in connected areas.

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
