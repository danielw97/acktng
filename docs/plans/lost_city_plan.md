# Area Plan: Lost City (Spec-Aligned Full Rebuild)

## Overview

**Area Name:** `@@bLost City@@N` 
**File Name:** `lost_city.are` 
**Keyword:** `lost city desert ruins` 
**Level Range:** 35-55 (`I 35 55`) 
**Vnum Range (fixed):** **5850-5949** (`V 5850 5949`) 
**Room Count Target:** **80/80 rooms used** 
**Mob Count Target:** **80/80 mobs rewritten** 
**Object Count Target:** **80/80 objects rewritten**

This plan keeps the existing vnum envelope and performs a complete thematic rebuild of rooms, mobiles, objects, resets, and progression. The current repetitive ruin-lane pattern is replaced with a coherent, lore-anchored city-state ruin tied directly to oasis water-law history and pyramid-era sovereign conflict.

---

## Color Theme (Multi-Color, Lore-Driven)

- **Cobalt (`@@b`)**: lost dynastic authority, twilight stone, drowned memory-script.
- **Sun-gold (`@@y`)**: desert glare, charters, toll marks, fractured royal splendor.
- **Moon-purple (`@@p`)**: Moon Sovereign funerary doctrine, crypt wards, verdict sigils.
- **Dust-brown (`@@d`)**: erosion, collapsed masonry, caravan wear, old road sediment.
- **Ritual-white (`@@W`)**: oath tablets, legal witness text, sealed judgment language.
- **Hazard-red (`@@R`)**: active threats, blood-rite remnants, boss telegraphing.
- **Reset (`@@N`)** after every intentional colorized fragment.

Theme policy compliance:
- No black foreground (`@@k`) as thematic color.
- No background codes (`@@0`-`@@7`) and no flashing (`@@f`).

---

## Design Pillars

1. **Ruined administrative metropolis, not generic rubble.** Every district must reveal a civic function.
2. **Water-law collapse as core tragedy.** The city dies when canal law and crypt law become irreconcilable.
3. **Twin-Sovereign ideological fracture.** Northern ascent doctrine and southern descent doctrine are both present in architecture and faction behavior.
4. **Playable expedition arc.** Entry salvage, mid-zone legal/ritual conflict, deep crypt confrontation, surviving return route.
5. **Full-vnum utilization with unique room identity.** All 80 room vnums are used and every room has unique descriptive text.

---

## Regional Lore Anchoring (Required)

Lost City content must align with:
- **Great/Northern/Southern Oasis lore:** charter stones, ration law, oath witnesses, canal maintenance bureaucracy, and post-collapse water tribunals.
- **Northern/Southern/Great Pyramid lore:** sovereign schism symbolism, funerary statecraft, ritual jurisdiction language, and eclipse-era doctrinal scars.
- **Eastern Desert context:** caravan law remnants, predation pressures, broken road governance.

The area is framed as a former **corridor hinge-city** between oasis water administration and pyramid mortuary administration.

---

## `#AREA` Header Plan (Spec-Strict)

- `Q 16` (mandatory)
- `K lost city desert ruins @@b @@y~`
- `L @@W(@@b35 55@@W)@@N~`
- `N 8` (retain unless global area numbering pass requires migration)
- `I 35 55`
- `V 5850 5949`
- `X 0`
- `F 20`
- `U @@bCobalt@@N dusk and @@ymarket-gold@@N dust sweep through the drowned courts of a law-broken desert capital.~`
- `O Virant~` (normalize owner case to policy)
- `R all~`
- `W all~`

---

## Structural Rebuild (Rooms 5850-5949, all used)

### District A: Gate of Broken Measures (5331-5340)
Entry zone with caravan-facing ruins and legal markers.
- 5331 **Sunken Gate of Obelisks** (primary entry hub)
- 5332-5335 checkpoint lanes, toll plinths, shattered ward towers
- 5336 **Charter Dust Arcade** (named-exit branch hub)
- 5337-5339 intake pens and collapsed weigh stations
- 5340 **Plaza of Unpaid Oaths** (transition to civic core)

### District B: Cistern Quarter of the Ledger Court (5341-5350)
Water governance district; civil collapse is readable in architecture.
- 5341 **First Cistern Mouth**
- 5342-5345 sluice halls, seal archives, weight-and-volume chambers
- 5346 **Hall of Four Assessors** (lore-critical room)
- 5347-5349 flooded adjudication annexes
- 5350 **Dry Tribunal Steps** (transition to market district)

### District C: Saffron Market Necropolis (5351-5360)
Hybrid civic/funerary trade quarter where relic economy replaced living commerce.
- 5351 **Lantern Bazaar Ruin**
- 5352-5355 salvage rows, relic assay stalls, collapsed caravan inns
- 5356 **Coin-Basin Crossway** (vertical connector)
- 5357-5359 mortuary broker courts
- 5360 **Jackal Tally Yard** (faction front)

### District D: Solar Docket Avenue (5361-5370)
Sand Sovereign-aligned monumental avenue with legal propaganda.
- 5361 **Avenue of Ascending Decrees**
- 5362-5365 gnomon plazas and sun-script colonnades
- 5366 **Court of Noon Reckoning** (elite patrol node)
- 5367-5369 guardian barracks and archive choke points
- 5370 **Apex Relay Stair** (split route)

### District E: Moonbound Descent Wards (5371-5380)
Moon Sovereign-aligned undercity interfaces and crypt administration.
- 5371 **Threshold of Seven Descents**
- 5372-5375 tier-rites offices, embalmer registries, sealed descent ramps
- 5376 **Registry of Silent Names** (lore-critical room)
- 5377-5379 venom cistern interfaces and bone-tithe vaults
- 5380 **Nadir Procession Gate**

### District F: Eclipse Fracture Concourse (5381-5390)
Schism zone where solar and moon doctrines physically collide.
- 5381 **Meridian Fracture Court**
- 5382-5385 collapsed ritual observatories and split inscription walls
- 5386 **Hall of Contradictory Verdicts** (high-threat narrative hub)
- 5387-5389 unstable geometry passages / hazard routes
- 5390 **Black-Sand Bridgehead**

### District G: Tyrant Crypt Ring (5391-5400)
Late-era authoritarian burial complex repurposed as militarized tomb-state.
- 5391 **Outer Crypt Bastion**
- 5392-5395 sarcophagus galleries, curse locks, honor guard bays
- 5396 **Chamber of the Twin Seals** (mini-boss zone)
- 5397-5399 blood-ink decree crypts
- 5400 **Inner Seal Causeway**

### District H: Deep Reliquary and Return Roads (5401-5410)
Boss endcap + extraction route reconnecting to desert graph.
- 5401 **Reliquary Sink**
- 5402-5405 drowned reliquary loops, shard vaults, oath pyres
- 5406 **Vault of the Hollow Diadem** (primary boss room)
- 5407-5409 aftermath routes, sealed adjudicator cells
- 5410 **Ruinward South Track** (external transition room)

---

## Connectivity Rules

- All exits are bi-directional.
- No one-way trap navigation in baseline traversal.
- District transitions occur at: 5340, 5350, 5360, 5370, 5380, 5390, 5400.
- At least one alternate route between each major district pair (mainline + risk path).
- Locked door states set through `#RESETS` `D` commands (not exit lock bit misuse).
- Named exits (`^...~`) are supported only where discoverability text is present in room descriptions or extras.

---

## Mobile Rebuild Plan (`#MOBILES` 5850-5949)

All 80 mobile templates are rewritten. Sequential, no gaps.

### Faction bands
- **5331-5342:** Gate scavengers, oath deserters, dust jackal handlers.
- **5343-5354:** Cistern clerks, sluice revenants, charter wardens.
- **5355-5366:** Market relic-brokers, embalmer militias, synod raiders.
- **5367-5378:** Solar decree guardians, moonbound registrars, schism zealots.
- **5379-5390:** Eclipse fracture entities, legal-rite phantoms, venom custodians.
- **5391-5402:** Crypt honor guard, tyrant priests, curse engineers.
- **5403-5410:** Boss suite and endgame attendants.

### Key mob anchors
- **5346** The Fourth Assessor (civil-law mini-boss)
- **5366** Noon Marshal of Broken Charters (solar elite)
- **5376** Registrar of Silent Names (moon elite caster)
- **5386** Meridian Heresiarch (schism enforcer)
- **5396** Twin-Seal Chamberlain (crypt mini-boss)
- **5406** The Hollow Diadem, Last Tyrant-Adjuticator (primary boss)

### Spec policies
- `is_npc` and `stay_area` on all mobs.
- Sentinel only where stationary role requires it.
- Aggressive flags restricted to hostile districts.
- Boss flag only on boss entities and only in `no_mob` boss rooms.
- `long_descr` single-line strict format; `description` trailing-newline strict format.

---

## Object Rebuild Plan (`#OBJECTS` 5850-5949)

All 80 object templates are rewritten. Sequential, no gaps.

### Object families
- **5331-5340:** Entry gear, sand-scored utility kit, novice desert salvage tools.
- **5341-5350:** Charter tablets, water seals, assessor insignia, legal keys.
- **5351-5360:** Bazaar relics, ritual trade goods, embalmer compounds.
- **5361-5370:** Solar court armor, decree staves, gnomon sigils.
- **5371-5380:** Moonbound funerary instruments, veil talismans, descent keys.
- **5381-5390:** Eclipse-split artifacts, unstable rite foci, hazard drops.
- **5391-5400:** Tyrant crypt regalia, warded sarcophagus implements.
- **5401-5410:** Boss-tier relic set, signature weapon/focus, extraction tokens.

### Keying and locks
- Distinct key chains for Cistern Quarter, Moon Wards, Crypt Ring, Reliquary Vault.
- Every locked door references a valid in-range key object.

---

## Reset and Progression Plan

- Rebuild all mob/object placements to match new district identity.
- Use encounter escalation from civil ruin -> doctrinal conflict -> crypt-state confrontation.
- Ensure object economy supports level 35-55 progression with mixed melee/caster rewards.
- Add lore-note objects as readable bridge pieces to oasis and pyramid content.

---

## Room Writing Standards (Implementation Checklist)

- Every room description: minimum 3 sentences; major hubs/lore rooms/boss rooms: minimum 5.
- No reused descriptions.
- No vnums in in-world text.
- Named exits must be discoverable in room text or extras.
- Extra descriptions used for legal stelae, oath tablets, sovereign iconography.
- Keep color usage deliberate and sparse enough for readability.

---

## Deliverables from this Plan

1. Full rewrite of `lost_city.are` within **5850-5949**.
2. 80 unique rooms, 80 rewritten mobs, 80 rewritten objects.
3. Lore-consistent crosslinks to oasis and pyramid narratives.
4. Spec-clean strings, formats, exits, locks, and reset behavior.
