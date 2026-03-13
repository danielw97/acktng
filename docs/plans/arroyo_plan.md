# Area Plan: The Arroyo (Full Thematic Rebuild Within Existing Envelope)

## Overview

- **Area Name:** `The Arroyo`
- **File Name:** `arroyo.are`
- **Keyword:** `arroyo`
- **Level Range:** `55-75` (`I 55 75`)
- **Vnum Range (fixed, unchanged):** `19900-20024` (`V 19900 20024`)
- **Total Room Envelope:** **125 rooms** (all room vnums in-range must be used)
- **Design Goal:** Rebuild the entire area (rooms, mobs, and objects) as a lore-forward transitional fault-canyon between oasis administration and pyramid underworld theology, replacing the current mostly linear hazard crawl with a coherent civilizational ruin ecology.

This plan keeps the current assigned vnum range exactly as-is and requires full-content replacement for rooms, mobiles, objects, resets, and encounter identity.

---

## Color Theme (Multi-Color, Lore-Matched)

Use a controlled multi-color palette tied to setting layers:

- `@@l` **Aquifer Azure**: hidden seep-lines, surviving cistern memory, Deepwell Confluence echoes.
- `@@y` **Sun Ledger Gold**: charter marks, survey law, toll-era inscriptions.
- `@@b` **Silt Brown**: arroyo clay, cutbanks, dried flood sediments.
- `@@R` **Rite Ember Red**: failed meridian rites, blood contracts, ward-break scars.
- `@@p` **Moon Jurisdiction Purple**: Southern Pyramid death doctrine bleed-through.
- `@@g` **Calcified Bone Gray-Green**: ossuary growths, dead-root tangles, mineralized remains.
- `@@N` reset after each themed segment.

Theme restrictions (spec aligned):
- No `@@k` as thematic foreground.
- No flashing code `@@f`.
- No background codes `@@0`-`@@7`.

---

## Core Narrative Direction

The Arroyo is no longer "just a dry riverbed with monsters." It becomes the **Jurisdictional Scar**: a canyon where three historical systems overlap and fail in visible layers:

1. **Keeper-era hydraulic survey infrastructure** (oasis legal-water civilization).
2. **Salt Ledger funerary extraction infrastructure** (caravan tax + corpse handling).
3. **Post-collapse pyramid splinter rites** (Moon-descending legal theology and curse residue).

Players traverse from surface law remnants to under-canyon rite machinery, culminating in a fault-chamber where memory, debt, and burial jurisdiction are still contested.

---

## Cross-Area Lore Anchors

The rebuild must explicitly align with established corridor lore:

- **Great / Northern / Southern Oases:** water-right language, ration marks, and oath-stone jurisprudence appear in inscriptions and room details.
- **Northern + Southern Pyramids:** competing cosmologies appear in reused stone, inverted glyphs, and necro-jurisdiction ritual spaces.
- **Eastern Desert:** charter-stone geometry, route-measure marks, and toll synod degradation are present in surface approach sectors.

See `docs/arroyo_lore.md` for implementation lore.

---

## `#AREA` Header Requirements (strict)

- `Q 16` mandatory.
- `K arroyo~`
- `L` remains level-appropriate for 55-75 presentation.
- `N 199` preserved unless global area-number migration requires otherwise.
- `I 55 75`
- `V 19900 20024` unchanged.
- `F 15`
- `O Virant~` normalized casing.
- `U` reset message rewritten to reflect jurisdictional-scar identity.

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

---

## Room Topology Rebuild (All 125 Rooms Used)

### District A: Charter Cut (19900-19924, 25 rooms)
**Theme:** entry canyon with Keeper survey relics and toll-law debris.

Key anchors:
- `19900` **Mouth of the Jurisdictional Scar** (entry, 5+ sentences).
- `19906` **Broken Charter Obelisk** (lore anchor, extra descriptions for glyph strata).
- `19912` **Dry Sluice Marker Yard** (oasis law crossover).
- `19918` **Toll Ledger Switchback** (bandit + synod pressure).
- `19924` **First Collapse Gate** (transition to buried civic ruins).

### District B: Silt-Court Ruins (19925-19949, 25 rooms)
**Theme:** partially buried civic quarter that processed water, tolls, and dead caravaneers.

Key anchors:
- `19925` **Sunken Assessors' Court**.
- `19931` **Registry of Thirst Oaths**.
- `19937` **Collapsed Cistern Tribunal**.
- `19942` **Embalmer's Through-Hall**.
- `19949` **Sealed Debt Archive Door** (named door + key flow).

### District C: Brood and Ossuary Interface (19950-19974, 25 rooms)
**Theme:** natural predator nest grown through ritual ossuary remains.

Key anchors:
- `19950` **Resin Fan Approach**.
- `19956` **Matriarch Egg Trench**.
- `19961` **Bone-Resin Lattice**.
- `19967` **Stinger Reliquary** (ritualized scorpion remains).
- `19974` **Ossuary Windbreak** (transition to deep descent).

### District D: Meridian Fracture Galleries (19975-19999, 25 rooms)
**Theme:** deepest ritual-rent canyon where failed eclipse/meridian rites scorched bedrock.

Key anchors:
- `19975` **Rite-Fall Descent**.
- `19982` **Split Meridian Vault**.
- `19988` **Moon-Inverted Survey Hall**.
- `19993` **Calcified Witness Pit**.
- `19999` **Precipice of Unpaid Names**.

### District E: Heart Fault Reliquary (20000-20024, 25 rooms)
**Theme:** under-canyon jurisdiction engine where debt, memory, and burial law persist as hostile metaphysics.

Key anchors:
- `20000` **Rattle Span of the Deep Court**.
- `20007` **Silent Aquifer Echo Chamber**.
- `20012` **Basalt Adjudication Colonnade**.
- `20018` **Red Seep of the Ninth Contradiction**.
- `20020` **Matriarch of Sand-Contracts** (major boss).
- `20022` **Throne of Reversed Passage** (secondary boss / ritual adjudicator).
- `20024` **Heart of the Arroyo** (final encounter room, 5+ sentences, no_mob room flag).

---

## Encounter and Faction Redesign

### Primary Factions

1. **Charter Reclaimers** (hostile synod-derived toll zealots):
   - reclaim old caravan law with coercive "authorized thirst" doctrine.
2. **Dustbound Court-Dead** (jurisdiction undead):
   - former clerks, embalmers, oath-witnesses animated by unresolved ledgers.
3. **Brood of the Resin Matriarch** (scorpion ecosystem):
   - integrated into ritual spaces; not random wildlife.
4. **Fault Elementals** (rite-scar manifestations):
   - ash, dust, and mineral spirits tied to meridian failure zones.

### Mob Vnum Plan

- Use mob vnums sequentially within `19900-20024`.
- Target **34-40 mob templates** with strict role separation:
  - trash/core combatants,
  - elite enforcers (solo flagged),
  - 2-3 bosses (boss flagged, sentinel, no_flee; in no_mob rooms).

---

## Objectization Rebuild

### Object Vnum Plan

- Use object vnums sequentially in-range (same envelope; per-type namespace).
- Target **45-55 objects**, including:
  - charter relics (scroll cases, oath tablets, measure rods),
  - ritual implements (seal knives, moon-ink censers, witness masks),
  - brood craft drops (resin shields, stinger daggers),
  - boss-signature gear.

### Itemization Intent

- Blend oasis-administrative motifs with pyramid funerary motifs.
- Include visible inscriptional language in `E` extra descriptions.
- Keyed doors in Districts B/D/E each have explicit key objects.

---

## Progression and Difficulty Curve

- **19900-19924:** onboarding (55-60).
- **19925-19949:** legal-ruin pressure and mixed caster/melee packs (58-64).
- **19950-19974:** venom/attrition ecosystem (61-68).
- **19975-19999:** rite hazard and undead-control encounters (66-73).
- **20000-20024:** elite + boss gauntlet (70-75).

Boss cadence:
- Mini-boss at `19988` (glyph judge revenant).
- Major boss at `20020` (Resin Contract Matriarch).
- Final boss/culmination at `20024` (Heart adjudicator manifestation).

---

## Reset and Door Plan

- Distinct spawn pockets per district to avoid uniform density.
- Locked doors only where lore supports institutional barriers:
  - archive seal door (B),
  - rite vault gate (D),
  - reliquary adjudication gate (E).
- Any locked reset state requires valid key vnum object and `EX_ISDOOR` on both sides.

---

## Implementation Checklist

1. Replace all current room texts/topology while preserving full room vnum usage `19900-20024`.
2. Replace all current mobiles with new faction-driven roster.
3. Replace all current objects and drop tables to match new narrative economy.
4. Align all strings and structures to `area_file_spec.md` constraints.
5. Validate bi-directional exits, door flags, key existence, and no-vnum-in-text policy.
6. Run area parser/load checks and in-game traversal sanity pass.

