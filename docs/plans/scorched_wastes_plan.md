# Area Plan: Scorched Wastes (Full Rebuild)

## Overview

- **Area file:** `scorched_wastes.are`
- **Area title:** `@@eThe @@yScorched @@rWastes@@N`
- **Keyword:** `scorched_wastes`
- **Level band:** `I 65 85`
- **Fixed vnum envelope (unchanged):** `V 22500 22574`
- **Design mandate:** fully replace current rooms, mobs, objects, and reset logic while keeping the same vnum range.
- **Room utilization mandate:** all **75 room vnums** in the envelope are used (`22500-22574`).

This redesign treats the Scorched Wastes as the **burned meridian corridor** between the oasis administration arc and the pyramid cult frontier: a place where collapsed water-law, failed eclipse rites, and desert predation overlap.

## Spec Compliance Checklist (`docs/area_file_spec.md`)

- `#AREA` contains `Q 16`.
- Owner line is `O Virant~`.
- `V 22500 22574` is preserved exactly.
- Strings are `~` terminated and avoid double blank lines.
- Room descriptions are unique, with minimum sentence count and named-exit anchoring.
- Mobile long descriptions remain one-line format; mobile descriptions end with a single trailing newline before `~`.
- No in-world text references numeric vnums.
- Color coding uses a defined area palette and avoids `@@k`, flashing, and background codes.

## Multi-Color Theme (intentional and consistent)

Primary palette for authored strings:
- `@@y` sun-bleached gold (law stones, ancient authority)
- `@@e` light red (scorch, heat fractures, ritual burn)
- `@@r` light green (mirage-glass algae, false oasis life)
- `@@c` cyan (deepwater memory, lost aquifer traces)
- `@@p` purple (eclipse corruption, underworld bleed)
- `@@W` white (salt, bone, legal glyph clarity)
- `@@N` reset

Usage rule: room names use 1-2 dominant colors; descriptions use sparse accents to signal faction/lore ownership.

## Lore Anchors Used for Rebuild

- Oasis corridor governance (charter stones, water-right law, ration policy) is imported from oasis plans/lore.
- Pyramid cosmology (Sand vs Moon sovereignty, eclipse fracture doctrine, funerary bureaucracy) is imported from pyramid lore.
- Scorched Wastes becomes the historical **failure zone** where those systems collided and burned.

## New Area Structure (All Rooms Used)

### Zone A — Charter Fringe (22500-22514)
Former road-edge desert is rebuilt as a collapsed intake district where caravans once submitted water ledgers.

| Vnum | Room Name | Function |
|---|---|---|
|22500|The Ash Charter Gate|Primary entry hub; transition from external desert routes.|
|22501|Ledger-Split Causeway|Northbound intake lane with legal waymarkers.|
|22502|Obelisk of Measured Thirst|Branch nexus; first major routing choice.|
|22503|Broken Toll Pavilion|Administrative ruin with record fragments.|
|22504|Hall of Dry Seals|Clerk archive shell; inscription-heavy lore room.|
|22505|Cistern Verdict Court|Mini-hub where water law was adjudicated.|
|22506|Collapsed Registrar Arcade|Merchants’ claim counters and looted stalls.|
|22507|Sunken Weighbridge|Freight scale platform over buried channels.|
|22508|The Audit Stairs|Descent transition into substructure.|
|22509|First Sluice Gallery|Hydraulic control corridor, now desiccated.|
|22510|Mirage Intake Basin|False-water encounter room.|
|22511|Palm-Skeleton Ring|Withered oasis fringe and scavenger spawns.|
|22512|Salt-Writ Grotto|Legal inscriptions recut as curses.|
|22513|Rationer’s Last Camp|Narrative camp ruin with survivor traces.|
|22514|Cairn of Unpaid Water|Dead-end shrine with oathbreaker motif.|

### Zone B — Burned Meridian Dunes (22515-22529)
Open desert routes reworked into a shifting gauntlet linked to eclipse events and caravan collapse.

| Vnum | Room Name | Function |
|---|---|---|
|22515|Meridian Break|Transition from legal district to wild dunes.|
|22516|Glasswind Defile|Heatstorm corridor and ambush pathing.|
|22517|Ash Serpent Run|Predator migration lane.|
|22518|Howling Slipface|Slip-dune traversal challenge.|
|22519|Shard-Teeth Hollow|Scorpion and carrion ecology node.|
|22520|The Black Noon Flats|Eclipse-scarred exposed plain.|
|22521|Drift of Whispered Oaths|Undead memory-echo room.|
|22522|Storm-Cut Marker Field|Waystone graveyard with navigation clues.|
|22523|Dune Host Encampment|Humanoid hostile camp hub.|
|22524|Tarp Court of the Reavers|Faction social core, loot-rich resets.|
|22525|Spoils Trench|Reaver dump and salvage lane.|
|22526|Buried Caravan Spine|Half-submerged wagon train tomb.|
|22527|Titan Step Basin|Colossus signposting room.|
|22528|Arena of Fused Sand|Outdoor boss antechamber.|
|22529|Seat of the Dune Colossus|Zone B boss room (`no_mob` room flag for boss policy).|

### Zone C — Scarab and Venom Undercrawl (22530-22544)
Subterranean insect and venom complex tied to pyramid embalming byproducts.

| Vnum | Room Name | Function |
|---|---|---|
|22530|Khepric Sinkhole Mouth|Primary descent entry.|
|22531|Mandible Tunnel|Tight predator corridor.|
|22532|Carapace Vault|Scarab shell midden and crafting scrap.|
|22533|Brood Resonance Chamber|Swarm acoustics mechanic room.|
|22534|Amber Venom Sump|Toxic pool hazard space.|
|22535|Egg-Lantern Gallery|Bioluminescent route marker room.|
|22536|The Split Stinger Fork|Major branch node.|
|22537|Chitin Reliquary|Synod relic misuse site.|
|22538|Wormscript Wall|Inscription wall linking to pyramid script.|
|22539|Queen’s Forerib Hall|Pre-boss gauntlet.|
|22540|The Molting Pit|Verticality and spawn churn.|
|22541|Fume Pocket Traverse|Environmental pressure room.|
|22542|Needle Dais|Scorpion rites platform.|
|22543|Royal Venom Sepulcher|Boss antechamber.|
|22544|The Scorpion Queen Ascendant|Zone C boss room (`no_mob` room flag for boss policy).|

### Zone D — Eclipse Reliquary of the Burned Court (22545-22559)
Hybrid ruin where oasis legal architecture was overwritten by eclipse liturgy.

| Vnum | Room Name | Function |
|---|---|---|
|22545|Reliquary Threshold|Transition to interior temple-court spaces.|
|22546|Court of Ashen Litigants|Undead judge/custodian encounters.|
|22547|Gallery of Seared Cartouches|Major lore wall; script comparisons.|
|22548|The Ninth Meridian Diagram|Central historical revelation room.|
|22549|Archive of Melted Reeds|Destroyed records and memory motifs.|
|22550|Hall of Jackal Witnesses|Synod jurisprudence reinterpretation.|
|22551|Moonward Interrogatory|Moon doctrine chamber.|
|22552|Sunward Rebuttal Chamber|Sand doctrine chamber.|
|22553|Twin Sovereigns Vestibule|Doctrinal conflict set-piece.|
|22554|Chamber of the Failed Rite|Ritual blast scar core.|
|22555|Eclipsed Sluice Gate|Hydraulic-meets-theological puzzle space.|
|22556|Antechamber of Golden Dust|Final approach to pharaonic remnant.|
|22557|Sarcophagus of Cindered Law|High narrative confrontation.|
|22558|Pharaoh’s Rekindled Wrath|Zone D boss room (`no_mob`).|
|22559|Exit Shaft to the Waste|Loop-back connector to surface routes.|

### Zone E — Salt Fault and Deepwell Wound (22560-22574)
Late-route expansion connecting scorched surface myth back to oasis aquifer geology.

| Vnum | Room Name | Function |
|---|---|---|
|22560|Faultline Lip|Entry into deep geological layer.|
|22561|Salt Lung Cavern|Breath-tax hazard flavor room.|
|22562|Calcite Prayer Hall|Ritualized geology chamber.|
|22563|Deepwell Echo Cistern|Water-memory narrative node.|
|22564|Cracked Confluence Vein|Direct lore link to Great Oasis aquifer.|
|22565|The Dry Governor Wheel|Ancient flow-control machine room.|
|22566|Undertow Without Water|Psychological pressure chamber.|
|22567|Moon-Sunk Ledger Niche|Evidence of Southern Pyramid doctrine drift.|
|22568|Sand-Sworn Survey Niche|Evidence of Northern Pyramid doctrine drift.|
|22569|Collapsed Tribunal Bridge|Cross-faction ruin battleground.|
|22570|Tremor Chapel|Seismic omen room.|
|22571|Vault of Silent Springs|Endgame lore lock room.|
|22572|Antechamber of Returning Heat|Pre-final-boss prep chamber.|
|22573|Wellmouth Fracture Throne|Final narrative reveal.|
|22574|The Scorched Wastes Heart|Apex encounter room and thematic capstone.|

## Mob Redesign (full replacement)

### Vnum strategy
- Reassign mobs within area envelope as a contiguous roster `22530-22554` (25 templates), replacing all existing mob concepts.
- Faction-balanced ecology: oasis remnants, reaver opportunists, synod jurists, undead clerks, scarab/scorpion brood, faultline anomalies.

### New mob families
1. **Charter Remnants** (non-hostile to cautious players): water clerks, seal-bearers, ration wardens.
2. **Dune Host Reavers** (aggressive melee): dust pikers, tarp captains, bone auditors.
3. **Synod Eclipse Cells** (hybrid casters): jackal examiners, eclipse chanters, oath-cut adjudicators.
4. **Funerary Revenants** (undead law-keepers): ash bailiffs, seal mummies, verdict shades.
5. **Venom Brood** (beast ecology): glass scorpions, ember asps, scarab matrons.
6. **Faultline Entities** (elemental/aberrant): salt-lung wraiths, heat echoes, deepwell phantoms.

### Boss ladder
- **22529:** Dune Colossus Reforged (surface attrition boss).
- **22544:** Crown of Stingers, Scorpion Matriarch (venom/pressure boss).
- **22558:** Pharaoh of Cindered Statutes (ritual-undead boss).
- **22574:** Heartkeeper of the Burned Meridian (final synthesis boss).

## Object Redesign (full replacement)

### Vnum strategy
- Replace existing object set with contiguous item plan `22550-22574` (25 objects).
- Distribution: 14 standard drops, 7 elite drops, 4 boss-signature drops.

### Item themes
- **Measured Survival Gear:** heat veils, salt greaves, ledger straps.
- **Synod-Ritual Implements:** eclipse seals, jackal script fetishes, verdict brands.
- **Brood-Trophies:** chitin lamellar, venomglass bucklers, scarab-core amulets.
- **Court Relics:** cindered crown, sluice-key scepter, decree tablets.
- **Deepwell Artifacts:** confluence prism, dry governor cog, heart-flame vessel.

All item text follows color-theme consistency and area spec string constraints.

## Reset and Flow Plan

- Replace resets to support a **looping expedition route** instead of dead-end chains.
- Ensure each zone has at least one repopulated anchor room and one pressure corridor.
- Boss resets occur in `no_mob` flagged rooms only.
- Add object-to-mob thematic matching (court relics on revenants, chitin drops on brood, etc.).

## Connectivity Goals

- Preserve at least one legacy external linkage for continuity with existing world travel.
- Add intentional loops between Zone B, Zone C, and Zone D so runs do not hard-stop.
- Zone E functions as optional deep-lore extension but reconnects to earlier surface flow to avoid trap routing.

## Deliverables in Build Phase

1. Rewrite `#ROOMS` for all 75 room vnums in-range.
2. Rewrite `#MOBILES` roster and encounter identity.
3. Rewrite `#OBJECTS` roster and progression.
4. Replace `#RESETS` for new ecosystem and boss cadence.
5. Add `#SPECIALS` entries only where mechanically justified.
6. Validate parse compliance and string-format rules from `area_file_spec.md`.
