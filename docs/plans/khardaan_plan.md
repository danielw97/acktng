# Area Plan: Khar'Daan, the Sunken Necropolis (Full Redesign)

## Overview

**Area Name:** `@@yKhar'Daan, the @@aSunken @@pNecropolis@@N` 
**File Name:** `khardaan.are` 
**Keyword:** `khardaan necropolis desert dune ancient burial` 
**Area Number:** `N 115` 
**Level Range:** `I 60 80` 
**Vnum Range (fixed):** **31000-31099** (`V 31000 31099`) 
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
- `V 31000 31099` unchanged.
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
| I. Sunken Approach | 31000-31009 | Outer ruins and entry gates | Orientation, first combat pass |
| II. Scoured Barrens | 31010-31019 | Beast-dominated badlands | Fauna pressure, elite miniboss |
| III. Processional Necropolis | 31020-31029 | Tomb roads and priest sectors | Undead/caster mix |
| IV. Cistern Verge | 31030-31039 | Flood interfaces and sluices | Hazard navigation |
| V. Mortuary Workshops | 31040-31049 | Embalming industry | Debuff-heavy encounters |
| VI. Archive Sink | 31050-31059 | Drowned libraries | Lore/combat hybrid |
| VII. Oathguard Bastion | 31060-31069 | Military funerary quarter | Construct/revenant tank fights |
| VIII. Sealed Reliquaries | 31070-31079 | High sancta and curse vaults | Puzzle routing, elite packs |
| IX. Drowned Court | 31080-31089 | Flooded governance core | Faction convergence |
| X. Phylactery Crown | 31090-31099 | Final gauntlet and capstone | Endgame bosses |

---

## Room-by-Room Allocation (31000-31099)

### I. Sunken Approach (31000-31009)

- 31000 The Sunken Threshold — collapsed marker arch; zone entry.
- 31001 Field of Broken Obelisks — shattered boundary stelae and warning glyphs.
- 31002 The Crumbled Gatehouse — first choke and guardian reset point.
- 31003 Sand-Choked Promenade — half-buried processional avenue.
- 31004 The Grand Colonnade — exposed noble route with wind traps.
- 31005 Hall of the Forgotten Sun — faded royal iconography, intro to schism lore.
- 31006 Chamber of Burial Masks — identity-loss motif; masking rites.
- 31007 The Ossuary Vaults — stacked common dead; swarm encounter.
- 31008 Descent to Khar'Daan — transition to lower city.
- 31009 Inner Sanctum Antechamber — district handoff hub.

### II. Scoured Barrens (31010-31019)

- 31010 The Desert Escarpment — exposure and ranged predator pressure.
- 31011 Ruins of the Serpent Shrine — serpent cult remains.
- 31012 The Viper Den — venom fauna nest.
- 31013 Fossil-Ribbed Alcove — predatory ambush pocket.
- 31014 The Basilisk Pit — petrification-themed hazard room.
- 31015 The Scorpion Wastes — attrition crossing.
- 31016 Scorpion Warren Entrance — swarm ingress choke.
- 31017 The Brood Chamber — concentrated venom burst fight.
- 31018 Fossil-Ribbed Grotto — hidden transit toward deeper wings.
- 31019 Lair of the Scorpion Queen — district miniboss anchor.

### III. Processional Necropolis (31020-31029)

- 31020 Court of Interred Petitioners — prayer wall and common rites.
- 31021 Natron Procession Lane — embalmer traffic path.
- 31022 Gate of Measured Silence — legal-funerary checkpoint.
- 31023 Reliquary of Opened Names — identity catalog vault.
- 31024 Chamber of Canopic Weights — body-part tithe accounting.
- 31025 Salt Mummifier's Walk — ritual labor corridor.
- 31026 Hall of Chitin Lamps — scarab-lit funerary aisle.
- 31027 Oathbreaker's Crossing — fallen guardian memorial.
- 31028 Sump of the Canopic Leech — contamination hazard.
- 31029 The Sunken Hierophant's Dais — caster miniboss node.

### IV. Cistern Verge (31030-31039)

- 31030 Colossus Intake Causeway — giant machinery approach.
- 31031 Ashen Embalmer Gallery — gas/salt cloud combat terrain.
- 31032 Skirmisher Drainway — revenant patrol route.
- 31033 Resin Hulk Pump Hall — heavy construct station.
- 31034 Drowned Annalist Niche — archive ghost node.
- 31035 Canal Stalker Trench — low-visibility melee room.
- 31036 Obelisk Shard Runoff — moving hazard corridor.
- 31037 Hieratic Furnace Chapel — war-priest liturgy chamber.
- 31038 Sarcophagus Breaker Yard — siege construct arena.
- 31039 Keeper of Sealed Tides — district cap boss.

### V. Mortuary Workshops (31040-31049)

- 31040 Phylactery Sentinel Gate — hard guard entry.
- 31041 Hall of Resin Crucibles — crafting/ritual industry.
- 31042 Chamber of Linen Hooks — preparation chamber.
- 31043 Natron Kiln Arcade — heat/salt hazard lane.
- 31044 Basin of Bitter Oils — alchemical poisons.
- 31045 Cartouche Press Foundry — inscription forging line.
- 31046 Gallery of Red Needles — surgical rite tools.
- 31047 House of Quiet Knives — embalmer elite patrol.
- 31048 Perfuming Court — incense-based caster effects.
- 31049 Workshop Overseer's Loge — miniboss and key drop.

### VI. Archive Sink (31050-31059)

- 31050 Flooded Ledger Vestibule — transition into drowned stacks.
- 31051 Scroll-Drift Arcade — debris flow and difficult movement.
- 31052 Chamber of Dissolved Ink — visibility and dot pressure.
- 31053 Scriptorium of Lost Tributaries — oasis linkage records.
- 31054 Hall of Dynastic Errata — rewritten royal decrees.
- 31055 Sunken Catalog Rotunda — radial navigation hub.
- 31056 Vault of Salt Tablets — durable legal archive.
- 31057 Whispering Index Well — lore-heavy spirit room.
- 31058 Broken Surveyor's Gallery — road/oasis calibration maps.
- 31059 Archivist's Drowned Throne — major caster boss.

### VII. Oathguard Bastion (31060-31069)

- 31060 Parade Ground of Last Vows — revenant muster square.
- 31061 Barracks of Sealed Helms — military dead storage.
- 31062 Hall of Rusted Khopeshes — weapon reliquary.
- 31063 Shield-Ring Portico — defensive gauntlet.
- 31064 Chamber of Three Standards — faction war memorial.
- 31065 Dune Oath Tribunal — judgement chamber.
- 31066 Sentinel Assembly Vault — construct spawning floor.
- 31067 Bridge of Iron Bands — narrow tactical crossing.
- 31068 Reliquary Guardwell — high-density patrol room.
- 31069 Commander of the Bound Cohort — martial miniboss.

### VIII. Sealed Reliquaries (31070-31079)

- 31070 First Reliquary Ring — curse-gated outer circle.
- 31071 Hall of Closed Canopi — contamination containment.
- 31072 Shrine of Sunless Coronation — illegitimate succession lore.
- 31073 Mirror of Dry Tides — puzzle reflection chamber.
- 31074 Chamber of Hanging Cartouches — name-binding hazards.
- 31075 Vault of Gilded Wrappings — elite burial cache.
- 31076 Corridor of Recited Curses — chant-trigger ambushes.
- 31077 Second Reliquary Ring — tightening navigation loop.
- 31078 Antechamber of Seventy Seals — pre-capstone lock.
- 31079 Reliquary Adjudicator — elite gatekeeper fight.

### IX. Drowned Court (31080-31089)

- 31080 Runed Flood Sluice — active control mechanism.
- 31081 Coffer Hall of Night Sand — treasury under silt.
- 31082 Vowkeeper Portico — oath inscription plaza.
- 31083 Basin of Stilled Echoes — acoustics-based hazard.
- 31084 Hanging Gallery of Masks — elite ancestry display.
- 31085 Vault of Sundered Cartouches — identity rupture lore.
- 31086 Corridor of Blackened Reeds — marshlike intrusion zone.
- 31087 Third Reliquary Ring — final routing ring.
- 31088 Annalist's Spiral — lore staircase toward finale.
- 31089 Lower Ward Concourse — convergence staging room.

### X. Phylactery Crown (31090-31099)

- 31090 Catacomb of Bent Obelisks — unstable final ascent.
- 31091 Reliquary Verge — no-return threshold.
- 31092 Sealed Tide Rotunda — flood-pressure boss prelude.
- 31093 Chamber of Iron Papyrus — final ritual text engine.
- 31094 Sanctified Intake — water-heart of necropolis.
- 31095 Reliquary Crown — high sanctum platform.
- 31096 Hall of Immured Names — bound-soul corridor.
- 31097 Bridge of Gilded Dust — exposed transit into apex.
- 31098 Tomb of the Seventh Seal — penultimate lock chamber.
- 31099 Phylactery Apex — final encounter and area climax.

---

## Mob Redesign Plan

Keep current mobile vnums (`31000-31040`) but rewrite identities into a coherent ecology/faction stack:

- **Fauna Layer:** scorpions, vipers, basilisks, scarabs adapted to mineralized flood residue.
- **Labor-Dead Layer:** embalmers, acolytes, mummifiers maintaining obsolete rites.
- **Guardian Layer:** oathbreakers, sentinels, revenants, colossi bound to legal seals.
- **Scholar Layer:** archivists, annalists, hierophants preserving/twisting doctrine.
- **Apex Layer:** Dread Pharaoh complex reframed as phylactery-state rulership.

Boss cadence across route:
- Mid bosses: 31019, 31029, 31039, 31049, 31059, 31069, 31079.
- Final chain: 31095-31099 tied to 31015/31023/31024/31030/31040 equivalents.

---

## Objectization Plan

Use existing object envelope (`31000-31070`) to support district progression:

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

1. Preserve all area-owned content strictly inside `31000-31099`.
2. Use all 100 room vnums with distinct purpose.
3. Keep room graph readable: one main spine, multiple loops, optional challenge branches.
4. Encode lore in room text, mob descriptions, object extra descriptions, and reset composition.
5. Maintain coherent multi-color usage without over-coloring.
6. Preserve difficulty target for levels 60-80 with escalating end-cap encounters.

---

## Deliverables

- `docs/khardaan_lore.md` (canon lore bible).
- Updated `area/khardaan.are` (future implementation pass) aligning with this plan and spec constraints.

