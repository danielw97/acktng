# Area Plan: Scorched Wastes (Full Rebuild)

## Overview

- **Area file:** `scorched_wastes.are`
- **Area title:** `@@eThe @@yScorched @@rWastes@@N`
- **Keyword:** `scorched_wastes`
- **Level band:** `I 65 85`
- **Fixed vnum envelope (unchanged):** `V 5250 5349`
- **Design mandate:** fully replace current rooms, mobs, objects, and reset logic while keeping the same vnum range.
- **Room utilization mandate:** all **75 room vnums** in the envelope are used (`5250-5349`).

This redesign treats the Scorched Wastes as the **burned meridian corridor** between the oasis administration arc and the pyramid cult frontier: a place where collapsed water-law, failed eclipse rites, and desert predation overlap.

## Spec Compliance Checklist (`docs/area_file_spec.md`)

- `#AREA` contains `Q 16`.
- Owner line is `O Virant~`.
- `V 5250 5349` is preserved exactly.
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

### Zone A — Charter Fringe (4881-4895)
Former road-edge desert is rebuilt as a collapsed intake district where caravans once submitted water ledgers.

| Vnum | Room Name | Function |
|---|---|---|
|4881|The Ash Charter Gate|Primary entry hub; transition from external desert routes.|
|4882|Ledger-Split Causeway|Northbound intake lane with legal waymarkers.|
|4883|Obelisk of Measured Thirst|Branch nexus; first major routing choice.|
|4884|Broken Toll Pavilion|Administrative ruin with record fragments.|
|4885|Hall of Dry Seals|Clerk archive shell; inscription-heavy lore room.|
|4886|Cistern Verdict Court|Mini-hub where water law was adjudicated.|
|4887|Collapsed Registrar Arcade|Merchants’ claim counters and looted stalls.|
|4888|Sunken Weighbridge|Freight scale platform over buried channels.|
|4889|The Audit Stairs|Descent transition into substructure.|
|4890|First Sluice Gallery|Hydraulic control corridor, now desiccated.|
|4891|Mirage Intake Basin|False-water encounter room.|
|4892|Palm-Skeleton Ring|Withered oasis fringe and scavenger spawns.|
|4893|Salt-Writ Grotto|Legal inscriptions recut as curses.|
|4894|Rationer’s Last Camp|Narrative camp ruin with survivor traces.|
|4895|Cairn of Unpaid Water|Dead-end shrine with oathbreaker motif.|

### Zone B — Burned Meridian Dunes (4896-4910)
Open desert routes reworked into a shifting gauntlet linked to eclipse events and caravan collapse.

| Vnum | Room Name | Function |
|---|---|---|
|4896|Meridian Break|Transition from legal district to wild dunes.|
|4897|Glasswind Defile|Heatstorm corridor and ambush pathing.|
|4898|Ash Serpent Run|Predator migration lane.|
|4899|Howling Slipface|Slip-dune traversal challenge.|
|4900|Shard-Teeth Hollow|Scorpion and carrion ecology node.|
|4901|The Black Noon Flats|Eclipse-scarred exposed plain.|
|4902|Drift of Whispered Oaths|Undead memory-echo room.|
|4903|Storm-Cut Marker Field|Waystone graveyard with navigation clues.|
|4904|Dune Host Encampment|Humanoid hostile camp hub.|
|4905|Tarp Court of the Reavers|Faction social core, loot-rich resets.|
|4906|Spoils Trench|Reaver dump and salvage lane.|
|4907|Buried Caravan Spine|Half-submerged wagon train tomb.|
|4908|Titan Step Basin|Colossus signposting room.|
|4909|Arena of Fused Sand|Outdoor boss antechamber.|
|4910|Seat of the Dune Colossus|Zone B boss room (`no_mob` room flag for boss policy).|

### Zone C — Scarab and Venom Undercrawl (4911-4925)
Subterranean insect and venom complex tied to pyramid embalming byproducts.

| Vnum | Room Name | Function |
|---|---|---|
|4911|Khepric Sinkhole Mouth|Primary descent entry.|
|4912|Mandible Tunnel|Tight predator corridor.|
|4913|Carapace Vault|Scarab shell midden and crafting scrap.|
|4914|Brood Resonance Chamber|Swarm acoustics mechanic room.|
|4915|Amber Venom Sump|Toxic pool hazard space.|
|4916|Egg-Lantern Gallery|Bioluminescent route marker room.|
|4917|The Split Stinger Fork|Major branch node.|
|4918|Chitin Reliquary|Synod relic misuse site.|
|4919|Wormscript Wall|Inscription wall linking to pyramid script.|
|4920|Queen’s Forerib Hall|Pre-boss gauntlet.|
|4921|The Molting Pit|Verticality and spawn churn.|
|4922|Fume Pocket Traverse|Environmental pressure room.|
|4923|Needle Dais|Scorpion rites platform.|
|4924|Royal Venom Sepulcher|Boss antechamber.|
|4925|The Scorpion Queen Ascendant|Zone C boss room (`no_mob` room flag for boss policy).|

### Zone D — Eclipse Reliquary of the Burned Court (4926-4940)
Hybrid ruin where oasis legal architecture was overwritten by eclipse liturgy.

| Vnum | Room Name | Function |
|---|---|---|
|4926|Reliquary Threshold|Transition to interior temple-court spaces.|
|4927|Court of Ashen Litigants|Undead judge/custodian encounters.|
|4928|Gallery of Seared Cartouches|Major lore wall; script comparisons.|
|4929|The Ninth Meridian Diagram|Central historical revelation room.|
|4930|Archive of Melted Reeds|Destroyed records and memory motifs.|
|4931|Hall of Jackal Witnesses|Synod jurisprudence reinterpretation.|
|4932|Moonward Interrogatory|Moon doctrine chamber.|
|4933|Sunward Rebuttal Chamber|Sand doctrine chamber.|
|4934|Twin Sovereigns Vestibule|Doctrinal conflict set-piece.|
|4935|Chamber of the Failed Rite|Ritual blast scar core.|
|4936|Eclipsed Sluice Gate|Hydraulic-meets-theological puzzle space.|
|4937|Antechamber of Golden Dust|Final approach to pharaonic remnant.|
|4938|Sarcophagus of Cindered Law|High narrative confrontation.|
|4939|Pharaoh’s Rekindled Wrath|Zone D boss room (`no_mob`).|
|4940|Exit Shaft to the Waste|Loop-back connector to surface routes.|

### Zone E — Salt Fault and Deepwell Wound (4941-4955)
Late-route expansion connecting scorched surface myth back to oasis aquifer geology.

| Vnum | Room Name | Function |
|---|---|---|
|4941|Faultline Lip|Entry into deep geological layer.|
|4942|Salt Lung Cavern|Breath-tax hazard flavor room.|
|4943|Calcite Prayer Hall|Ritualized geology chamber.|
|4944|Deepwell Echo Cistern|Water-memory narrative node.|
|4945|Cracked Confluence Vein|Direct lore link to Great Oasis aquifer.|
|4946|The Dry Governor Wheel|Ancient flow-control machine room.|
|4947|Undertow Without Water|Psychological pressure chamber.|
|4948|Moon-Sunk Ledger Niche|Evidence of Southern Pyramid doctrine drift.|
|4949|Sand-Sworn Survey Niche|Evidence of Northern Pyramid doctrine drift.|
|4950|Collapsed Tribunal Bridge|Cross-faction ruin battleground.|
|4951|Tremor Chapel|Seismic omen room.|
|4952|Vault of Silent Springs|Endgame lore lock room.|
|4953|Antechamber of Returning Heat|Pre-final-boss prep chamber.|
|4954|Wellmouth Fracture Throne|Final narrative reveal.|
|4955|The Scorched Wastes Heart|Apex encounter room and thematic capstone.|

## Mob Redesign (full replacement)

### Vnum strategy
- Reassign mobs within area envelope as a contiguous roster `4911-4935` (25 templates), replacing all existing mob concepts.
- Faction-balanced ecology: oasis remnants, reaver opportunists, synod jurists, undead clerks, scarab/scorpion brood, faultline anomalies.

### New mob families
1. **Charter Remnants** (non-hostile to cautious players): water clerks, seal-bearers, ration wardens.
2. **Dune Host Reavers** (aggressive melee): dust pikers, tarp captains, bone auditors.
3. **Synod Eclipse Cells** (hybrid casters): jackal examiners, eclipse chanters, oath-cut adjudicators.
4. **Funerary Revenants** (undead law-keepers): ash bailiffs, seal mummies, verdict shades.
5. **Venom Brood** (beast ecology): glass scorpions, ember asps, scarab matrons.
6. **Faultline Entities** (elemental/aberrant): salt-lung wraiths, heat echoes, deepwell phantoms.

### Boss ladder
- **4910:** Dune Colossus Reforged (surface attrition boss).
- **4925:** Crown of Stingers, Scorpion Matriarch (venom/pressure boss).
- **4939:** Pharaoh of Cindered Statutes (ritual-undead boss).
- **4955:** Heartkeeper of the Burned Meridian (final synthesis boss).

## Object Redesign (full replacement)

### Vnum strategy
- Replace existing object set with contiguous item plan `4931-4955` (25 objects).
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
