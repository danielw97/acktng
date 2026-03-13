# Area Plan: Lost City (Spec-Aligned Full Rebuild)

## Overview

**Area Name:** `@@bLost City@@N` 
**File Name:** `lost_city.are` 
**Keyword:** `lost city desert ruins` 
**Level Range:** 35-55 (`I 35 55`) 
**Vnum Range (fixed):** **30400-30479** (`V 30400 30479`) 
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
- `V 30400 30479`
- `X 0`
- `F 20`
- `U @@bCobalt@@N dusk and @@ymarket-gold@@N dust sweep through the drowned courts of a law-broken desert capital.~`
- `O Virant~` (normalize owner case to policy)
- `R all~`
- `W all~`

---

## Structural Rebuild (Rooms 30400-30479, all used)

### District A: Gate of Broken Measures (30400-30409)
Entry zone with caravan-facing ruins and legal markers.
- 30400 **Sunken Gate of Obelisks** (primary entry hub)
- 30401-30404 checkpoint lanes, toll plinths, shattered ward towers
- 30405 **Charter Dust Arcade** (named-exit branch hub)
- 30406-30408 intake pens and collapsed weigh stations
- 30409 **Plaza of Unpaid Oaths** (transition to civic core)

### District B: Cistern Quarter of the Ledger Court (30410-30419)
Water governance district; civil collapse is readable in architecture.
- 30410 **First Cistern Mouth**
- 30411-30414 sluice halls, seal archives, weight-and-volume chambers
- 30415 **Hall of Four Assessors** (lore-critical room)
- 30416-30418 flooded adjudication annexes
- 30419 **Dry Tribunal Steps** (transition to market district)

### District C: Saffron Market Necropolis (30420-30429)
Hybrid civic/funerary trade quarter where relic economy replaced living commerce.
- 30420 **Lantern Bazaar Ruin**
- 30421-30424 salvage rows, relic assay stalls, collapsed caravan inns
- 30425 **Coin-Basin Crossway** (vertical connector)
- 30426-30428 mortuary broker courts
- 30429 **Jackal Tally Yard** (faction front)

### District D: Solar Docket Avenue (30430-30439)
Sand Sovereign-aligned monumental avenue with legal propaganda.
- 30430 **Avenue of Ascending Decrees**
- 30431-30434 gnomon plazas and sun-script colonnades
- 30435 **Court of Noon Reckoning** (elite patrol node)
- 30436-30438 guardian barracks and archive choke points
- 30439 **Apex Relay Stair** (split route)

### District E: Moonbound Descent Wards (30440-30449)
Moon Sovereign-aligned undercity interfaces and crypt administration.
- 30440 **Threshold of Seven Descents**
- 30441-30444 tier-rites offices, embalmer registries, sealed descent ramps
- 30445 **Registry of Silent Names** (lore-critical room)
- 30446-30448 venom cistern interfaces and bone-tithe vaults
- 30449 **Nadir Procession Gate**

### District F: Eclipse Fracture Concourse (30450-30459)
Schism zone where solar and moon doctrines physically collide.
- 30450 **Meridian Fracture Court**
- 30451-30454 collapsed ritual observatories and split inscription walls
- 30455 **Hall of Contradictory Verdicts** (high-threat narrative hub)
- 30456-30458 unstable geometry passages / hazard routes
- 30459 **Black-Sand Bridgehead**

### District G: Tyrant Crypt Ring (30460-30469)
Late-era authoritarian burial complex repurposed as militarized tomb-state.
- 30460 **Outer Crypt Bastion**
- 30461-30464 sarcophagus galleries, curse locks, honor guard bays
- 30465 **Chamber of the Twin Seals** (mini-boss zone)
- 30466-30468 blood-ink decree crypts
- 30469 **Inner Seal Causeway**

### District H: Deep Reliquary and Return Roads (30470-30479)
Boss endcap + extraction route reconnecting to desert graph.
- 30470 **Reliquary Sink**
- 30471-30474 drowned reliquary loops, shard vaults, oath pyres
- 30475 **Vault of the Hollow Diadem** (primary boss room)
- 30476-30478 aftermath routes, sealed adjudicator cells
- 30479 **Ruinward South Track** (external transition room)

---

## Connectivity Rules

- All exits are bi-directional.
- No one-way trap navigation in baseline traversal.
- District transitions occur at: 30409, 30419, 30429, 30439, 30449, 30459, 30469.
- At least one alternate route between each major district pair (mainline + risk path).
- Locked door states set through `#RESETS` `D` commands (not exit lock bit misuse).
- Named exits (`^...~`) are supported only where discoverability text is present in room descriptions or extras.

---

## Mobile Rebuild Plan (`#MOBILES` 30400-30479)

All 80 mobile templates are rewritten. Sequential, no gaps.

### Faction bands
- **30400-30411:** Gate scavengers, oath deserters, dust jackal handlers.
- **30412-30423:** Cistern clerks, sluice revenants, charter wardens.
- **30424-30435:** Market relic-brokers, embalmer militias, synod raiders.
- **30436-30447:** Solar decree guardians, moonbound registrars, schism zealots.
- **30448-30459:** Eclipse fracture entities, legal-rite phantoms, venom custodians.
- **30460-30471:** Crypt honor guard, tyrant priests, curse engineers.
- **30472-30479:** Boss suite and endgame attendants.

### Key mob anchors
- **30415** The Fourth Assessor (civil-law mini-boss)
- **30435** Noon Marshal of Broken Charters (solar elite)
- **30445** Registrar of Silent Names (moon elite caster)
- **30455** Meridian Heresiarch (schism enforcer)
- **30465** Twin-Seal Chamberlain (crypt mini-boss)
- **30475** The Hollow Diadem, Last Tyrant-Adjuticator (primary boss)

### Spec policies
- `is_npc` and `stay_area` on all mobs.
- Sentinel only where stationary role requires it.
- Aggressive flags restricted to hostile districts.
- Boss flag only on boss entities and only in `no_mob` boss rooms.
- `long_descr` single-line strict format; `description` trailing-newline strict format.

---

## Object Rebuild Plan (`#OBJECTS` 30400-30479)

All 80 object templates are rewritten. Sequential, no gaps.

### Object families
- **30400-30409:** Entry gear, sand-scored utility kit, novice desert salvage tools.
- **30410-30419:** Charter tablets, water seals, assessor insignia, legal keys.
- **30420-30429:** Bazaar relics, ritual trade goods, embalmer compounds.
- **30430-30439:** Solar court armor, decree staves, gnomon sigils.
- **30440-30449:** Moonbound funerary instruments, veil talismans, descent keys.
- **30450-30459:** Eclipse-split artifacts, unstable rite foci, hazard drops.
- **30460-30469:** Tyrant crypt regalia, warded sarcophagus implements.
- **30470-30479:** Boss-tier relic set, signature weapon/focus, extraction tokens.

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

1. Full rewrite of `lost_city.are` within **30400-30479**.
2. 80 unique rooms, 80 rewritten mobs, 80 rewritten objects.
3. Lore-consistent crosslinks to oasis and pyramid narratives.
4. Spec-clean strings, formats, exits, locks, and reset behavior.
