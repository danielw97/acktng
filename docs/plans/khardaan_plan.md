# Area Plan: Khar'Daan, the Sunken Necropolis (Full Redesign)

## Overview

**Area Name:** `@@yKhar'Daan, the @@aSunken @@pNecropolis@@N` 
**File Name:** `khardaan.are` 
**Keyword:** `khardaan necropolis desert dune ancient burial` 
**Area Number:** `N 115` 
**Level Range:** `I 60 80` 
**Vnum Range (fixed):** **6200-6299** (`V 6200 6299`) 
**Room Count Target:** **100/100 rooms used** 
**Primary Design Goal:** Rebuild the area as a coherent drowned-funerary megasite tied directly to oasis water politics and pyramid dynasty theology, while preserving the exact vnum envelope and fully repurposing every room.

Khar'Daan becomes a former **funerary reservoir-city** built to preserve elite dead during drought eras. Its upper city is sand-scoured and broken; its middle vaults are ritual-industrial embalming halls; its lower city is partially flooded by aquifer breakthroughs linked to the Great Oasis and pyramid-era hydraulic failures.

---

## Color Theme (Multi-Color, Lore-Locked)

- **Solar Gold (`@@y`)** — old dynastic authority, sun cult iconography, legal seals.
- **Aquifer Cyan (`@@a`)** — living water, seep-lines, flood channels, oasis-linked systems.
- **Lapis Blue (`@@l`)** — noble burial regalia, canopic glaze, archive inks.
- **Funerary Purple (`@@p`)** — priesthood rites, curse text, sacred interdictions.
- **Salt Brown (`@@b`)** — natron beds, embalming dust, cracked stone, desert wear.
- **Threat Red (`@@R`)** — violent guardians, blood rites, collapse danger.
- **Reset (`@@N`)** required after colored strings.

Compliance notes for implementation:
- No `@@k` as thematic color.
- No background color codes (`@@0`-`@@7`) and no flashing (`@@f`).
- Keep color sparse enough for readability.

---

## Area Header / Spec Compliance Checklist

Use `docs/area_file_spec.md` strictly:

- `Q 16` mandatory.
- `O Virant~` must use exact owner policy casing.
- `V 6200 6299` unchanged.
- All strings `~`-terminated.
- No blank-line double newlines inside area strings.
- No vnums in in-world text.
- Mobile `long_descr` one-line format only.
- Mobile/room/object descriptions end with exactly one trailing newline before `~`.

Proposed header text:

- Name: `@@yKhar'Daan, the @@aSunken @@pNecropolis@@N~`
- Level label: `@@a{@@y60 80@@a}@@N~`
- Reset message: `@@aBrackish mist rises from drowned crypts as @@yKhar'Daan@@a stirs beneath the sand.@@N~`

---

## Narrative Pillars

1. **Necropolis-as-Machine:** Khar'Daan was engineered to process, preserve, and index the dead using salt, resin, and controlled water.
2. **Water Debt:** Its flood system was fed by the same deep aquifer network that sustains the oases; drought conflict turned funerary infrastructure into militarized storage.
3. **Dynastic Schism:** The city sat between solar and underworld cults aligned with rival pyramid traditions.
4. **The Sundering Flood:** A failed sealing rite shattered lower gates; now archives, tombs, and reliquaries are drowned.
5. **Living Factions in Ruin:** Tomb raiders, oathbound dead, cult remnants, and engineered constructs contest control.

---

## Cross-Area Integration Requirements

### With Oasis Lore

- Great/Northern/Southern oasis intake doctrine originated in Khar'Daan's mortuary accounting methods.
- Water ledgers in Khar'Daan predate modern oasis quota law.
- Oases use surviving Khar'Daani unit-marks for ration tablets.
- The flooded catacombs explain why several oasis cults treat water as both sacred and contaminating.

### With Pyramid Lore

- Khar'Daan served as neutral embalming and archive ground during early Sand/Moon sovereign period.
- Its priest-scribes encoded funerary formulae later split into northern (solar-ascendant) and southern (underworld-descendant) pyramid rites.
- Collapse followed a post-eclipse attempt to reconcile contradictory sealing geometries.

---

## Layout Redesign (All 100 Rooms Used)

The area is rebuilt into 10 themed districts of 10 rooms each.

| District | Vnums | Theme | Gameplay Role |
|---|---|---|---|
| I. Sunken Approach | 5624-5633 | Outer ruins and entry gates | Orientation, first combat pass |
| II. Scoured Barrens | 5634-5643 | Beast-dominated badlands | Fauna pressure, elite miniboss |
| III. Processional Necropolis | 5644-5653 | Tomb roads and priest sectors | Undead/caster mix |
| IV. Cistern Verge | 5654-5663 | Flood interfaces and sluices | Hazard navigation |
| V. Mortuary Workshops | 5664-5673 | Embalming industry | Debuff-heavy encounters |
| VI. Archive Sink | 5674-5683 | Drowned libraries | Lore/combat hybrid |
| VII. Oathguard Bastion | 5684-5693 | Military funerary quarter | Construct/revenant tank fights |
| VIII. Sealed Reliquaries | 5694-5703 | High sancta and curse vaults | Puzzle routing, elite packs |
| IX. Drowned Court | 5704-5713 | Flooded governance core | Faction convergence |
| X. Phylactery Crown | 5714-5723 | Final gauntlet and capstone | Endgame bosses |

---

## Room-by-Room Allocation (6200-6299)

### I. Sunken Approach (5624-5633)

- 5624 The Sunken Threshold — collapsed marker arch; zone entry.
- 5625 Field of Broken Obelisks — shattered boundary stelae and warning glyphs.
- 5626 The Crumbled Gatehouse — first choke and guardian reset point.
- 5627 Sand-Choked Promenade — half-buried processional avenue.
- 5628 The Grand Colonnade — exposed noble route with wind traps.
- 5629 Hall of the Forgotten Sun — faded royal iconography, intro to schism lore.
- 5630 Chamber of Burial Masks — identity-loss motif; masking rites.
- 5631 The Ossuary Vaults — stacked common dead; swarm encounter.
- 5632 Descent to Khar'Daan — transition to lower city.
- 5633 Inner Sanctum Antechamber — district handoff hub.

### II. Scoured Barrens (5634-5643)

- 5634 The Desert Escarpment — exposure and ranged predator pressure.
- 5635 Ruins of the Serpent Shrine — serpent cult remains.
- 5636 The Viper Den — venom fauna nest.
- 5637 Fossil-Ribbed Alcove — predatory ambush pocket.
- 5638 The Basilisk Pit — petrification-themed hazard room.
- 5639 The Scorpion Wastes — attrition crossing.
- 5640 Scorpion Warren Entrance — swarm ingress choke.
- 5641 The Brood Chamber — concentrated venom burst fight.
- 5642 Fossil-Ribbed Grotto — hidden transit toward deeper wings.
- 5643 Lair of the Scorpion Queen — district miniboss anchor.

### III. Processional Necropolis (5644-5653)

- 5644 Court of Interred Petitioners — prayer wall and common rites.
- 5645 Natron Procession Lane — embalmer traffic path.
- 5646 Gate of Measured Silence — legal-funerary checkpoint.
- 5647 Reliquary of Opened Names — identity catalog vault.
- 5648 Chamber of Canopic Weights — body-part tithe accounting.
- 5649 Salt Mummifier's Walk — ritual labor corridor.
- 5650 Hall of Chitin Lamps — scarab-lit funerary aisle.
- 5651 Oathbreaker's Crossing — fallen guardian memorial.
- 5652 Sump of the Canopic Leech — contamination hazard.
- 5653 The Sunken Hierophant's Dais — caster miniboss node.

### IV. Cistern Verge (5654-5663)

- 5654 Colossus Intake Causeway — giant machinery approach.
- 5655 Ashen Embalmer Gallery — gas/salt cloud combat terrain.
- 5656 Skirmisher Drainway — revenant patrol route.
- 5657 Resin Hulk Pump Hall — heavy construct station.
- 5658 Drowned Annalist Niche — archive ghost node.
- 5659 Canal Stalker Trench — low-visibility melee room.
- 5660 Obelisk Shard Runoff — moving hazard corridor.
- 5661 Hieratic Furnace Chapel — war-priest liturgy chamber.
- 5662 Sarcophagus Breaker Yard — siege construct arena.
- 5663 Keeper of Sealed Tides — district cap boss.

### V. Mortuary Workshops (5664-5673)

- 5664 Phylactery Sentinel Gate — hard guard entry.
- 5665 Hall of Resin Crucibles — crafting/ritual industry.
- 5666 Chamber of Linen Hooks — preparation chamber.
- 5667 Natron Kiln Arcade — heat/salt hazard lane.
- 5668 Basin of Bitter Oils — alchemical poisons.
- 5669 Cartouche Press Foundry — inscription forging line.
- 5670 Gallery of Red Needles — surgical rite tools.
- 5671 House of Quiet Knives — embalmer elite patrol.
- 5672 Perfuming Court — incense-based caster effects.
- 5673 Workshop Overseer's Loge — miniboss and key drop.

### VI. Archive Sink (5674-5683)

- 5674 Flooded Ledger Vestibule — transition into drowned stacks.
- 5675 Scroll-Drift Arcade — debris flow and difficult movement.
- 5676 Chamber of Dissolved Ink — visibility and dot pressure.
- 5677 Scriptorium of Lost Tributaries — oasis linkage records.
- 5678 Hall of Dynastic Errata — rewritten royal decrees.
- 5679 Sunken Catalog Rotunda — radial navigation hub.
- 5680 Vault of Salt Tablets — durable legal archive.
- 5681 Whispering Index Well — lore-heavy spirit room.
- 5682 Broken Surveyor's Gallery — road/oasis calibration maps.
- 5683 Archivist's Drowned Throne — major caster boss.

### VII. Oathguard Bastion (5684-5693)

- 5684 Parade Ground of Last Vows — revenant muster square.
- 5685 Barracks of Sealed Helms — military dead storage.
- 5686 Hall of Rusted Khopeshes — weapon reliquary.
- 5687 Shield-Ring Portico — defensive gauntlet.
- 5688 Chamber of Three Standards — faction war memorial.
- 5689 Dune Oath Tribunal — judgement chamber.
- 5690 Sentinel Assembly Vault — construct spawning floor.
- 5691 Bridge of Iron Bands — narrow tactical crossing.
- 5692 Reliquary Guardwell — high-density patrol room.
- 5693 Commander of the Bound Cohort — martial miniboss.

### VIII. Sealed Reliquaries (5694-5703)

- 5694 First Reliquary Ring — curse-gated outer circle.
- 5695 Hall of Closed Canopi — contamination containment.
- 5696 Shrine of Sunless Coronation — illegitimate succession lore.
- 5697 Mirror of Dry Tides — puzzle reflection chamber.
- 5698 Chamber of Hanging Cartouches — name-binding hazards.
- 5699 Vault of Gilded Wrappings — elite burial cache.
- 5700 Corridor of Recited Curses — chant-trigger ambushes.
- 5701 Second Reliquary Ring — tightening navigation loop.
- 5702 Antechamber of Seventy Seals — pre-capstone lock.
- 5703 Reliquary Adjudicator — elite gatekeeper fight.

### IX. Drowned Court (5704-5713)

- 5704 Runed Flood Sluice — active control mechanism.
- 5705 Coffer Hall of Night Sand — treasury under silt.
- 5706 Vowkeeper Portico — oath inscription plaza.
- 5707 Basin of Stilled Echoes — acoustics-based hazard.
- 5708 Hanging Gallery of Masks — elite ancestry display.
- 5709 Vault of Sundered Cartouches — identity rupture lore.
- 5710 Corridor of Blackened Reeds — marshlike intrusion zone.
- 5711 Third Reliquary Ring — final routing ring.
- 5712 Annalist's Spiral — lore staircase toward finale.
- 5713 Lower Ward Concourse — convergence staging room.

### X. Phylactery Crown (5714-5723)

- 5714 Catacomb of Bent Obelisks — unstable final ascent.
- 5715 Reliquary Verge — no-return threshold.
- 5716 Sealed Tide Rotunda — flood-pressure boss prelude.
- 5717 Chamber of Iron Papyrus — final ritual text engine.
- 5718 Sanctified Intake — water-heart of necropolis.
- 5719 Reliquary Crown — high sanctum platform.
- 5720 Hall of Immured Names — bound-soul corridor.
- 5721 Bridge of Gilded Dust — exposed transit into apex.
- 5722 Tomb of the Seventh Seal — penultimate lock chamber.
- 5723 Phylactery Apex — final encounter and area climax.

---

## Mob Redesign Plan

Keep current mobile vnums (`5624-5664`) but rewrite identities into a coherent ecology/faction stack:

- **Fauna Layer:** scorpions, vipers, basilisks, scarabs adapted to mineralized flood residue.
- **Labor-Dead Layer:** embalmers, acolytes, mummifiers maintaining obsolete rites.
- **Guardian Layer:** oathbreakers, sentinels, revenants, colossi bound to legal seals.
- **Scholar Layer:** archivists, annalists, hierophants preserving/twisting doctrine.
- **Apex Layer:** Dread Pharaoh complex reframed as phylactery-state rulership.

Boss cadence across route:
- Mid bosses: 5643, 5653, 5663, 5673, 5683, 5693, 5703.
- Final chain: 5719-5723 tied to 5639/5647/5648/5654/5664 equivalents.

---

## Objectization Plan

Use existing object envelope (`5624-5694`) to support district progression:

- Early set: sand-worn expedition gear with anti-venom and movement utility.
- Mid set: embalmer/archivist relics with resist poison, resist curse, mana sustain.
- Late set: oathguard and reliquary regalia with defensive set bonuses.
- Apex relics: sunken crown, phylactery chain, sealed-tide focus, dynastic sigil blade.

Object flavor should visibly mix gold/lapis/salt/water motifs and reference oasis quota sigils plus pyramid dynastic cartouches.

---

## Resets / Flow Rules

- Ensure each district has ambient population, patrols, and one identity-defining anchor encounter.
- Avoid overstacking hard casters in narrow rooms; alternate control and burst threats.
- Place anti-poison and anti-curse gear before major venom/hex checkpoints.
- Add reset storytelling: lit braziers, active sluice sounds, drifting papyrus, moving floodline.

---

## Implementation Guardrails

1. Preserve all area-owned content strictly inside `6200-6299`.
2. Use all 100 room vnums with distinct purpose.
3. Keep room graph readable: one main spine, multiple loops, optional challenge branches.
4. Encode lore in room text, mob descriptions, object extra descriptions, and reset composition.
5. Maintain coherent multi-color usage without over-coloring.
6. Preserve difficulty target for levels 60-80 with escalating end-cap encounters.

---

## Deliverables

- `docs/khardaan_lore.md` (canon lore bible).
- Updated `area/khardaan.are` (future implementation pass) aligning with this plan and spec constraints.

