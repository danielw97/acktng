# Area Plan: The Gloamvault of Kel'Shadra (Full Redesign)

## Overview

- **Area Name:** `@@dThe @@pGloamvault @@mof @@dKel'Shadra@@N`
- **Area File:** `area/gloamvault.are` (existing area to be fully rebuilt)
- **Keyword:** `kelshadra gloamvault gothic dungeon`
- **Assigned Vnum Range (unchanged):** **30100-30174** (75 rooms total)
- **Target Level Band:** 5-20 (retain onboarding role into Kel'Shadra narrative arc)
- **Reset Rate:** 20
- **Reset Message (proposed update):** `@@pViolet bells toll below the stone while ash-lanterns rekindle in the dark.@@N`
- **Owner (per spec):** `Virant`

This plan replaces the current repetitive room topology and naming with a narrative-forward, layered catacomb-cult complex that better reflects the area title, existing mob/object motifs, and the wider Kel'Shadra shadow-crypt mythos.

---

## Area Specification Compliance Checklist

1. **Keep vnum envelope exactly as assigned:** all rooms, mobs, and objects remain in `30100-30174`, with no out-of-range expansion.
2. **Use valid ACK color tokens only:** thematic use of `@@d @@p @@m @@a @@W @@N`.
3. **No `@@k` black foreground authored into area strings.**
4. **No background codes (`0-7`) and no flashing `@@f`.**
5. **Preserve section integrity for future implementation:** `#AREA, #ROOMS, #MOBILES, #OBJECTS, #RESETS` etc. remain structurally valid when built.
6. **Do not place vnums in in-world descriptions.**

---

## Multi-Color Theme Bible (Intentional, Sparse, Readable)

Primary palette and semantic role:

- `@@d` **Dark Grey**: stone, ash, death-work, old vault masonry.
- `@@p` **Purple**: Kel'Shadra devotion, liturgy, vow-magic, ceremonial presence.
- `@@m` **Magenta**: blood-ritual moments, crisis states, boss rooms, active wards.
- `@@a` **Light Cyan**: gravefire, spirit-memory, oath echoes, non-hostile spectral guidance.
- `@@W` **White**: scripture, relic purity, old covenant language, revelation points.
- `@@N` **Reset**: always used to normalize text.

Theme rule:
- **Room names** get 1-2 color accents max.
- **Descriptions** use color only on ritual anchors (objects, effects, sigils), not every sentence.
- **Boss presence, locks, and progression clues** always include a stable color cue so players can parse danger and purpose quickly.

---

## Narrative Direction (Area Intent)

The redesigned Gloamvault becomes the **initiate-facing undervault** beneath greater Kel'Shadra holdings: a place where novices are tested, dead oaths are archived, and failed penitents are stripped into reliquary labor. It is not merely "dark crypts"; it is a functioning **theological machine** with three repeating verbs:

1. **Witness** (learn the vows and history),
2. **Offer** (surrender name, blood, or memory),
3. **Bind** (become instrument of the vault).

The area should teach players that Kel'Shadra is an institution with records, ranks, and logistics—not only monsters in hallways.

---

## Full Layout Redesign (75 Rooms / 9 Subzones)

> The current 8 repeated room-name blocks are replaced by 9 thematic subzones with clear progression, loops, and cross-links. Total remains 75 rooms.

### Zone A - The Riven Threshold (30100-30107, 8 rooms)
**Function:** Entry and tonal setup.

- Collapsed breach, inverted gatehouse, ruined ward pylons.
- One external world link retained (legacy entrance point).
- Introduce the first readable symbol language: Crescent, Bell, Veil.

**Gameplay role:** low-threat acclimation, first key fragment, anti-zerg chokepoint.

---

### Zone B - Processional of Ash Bells (30108-30115, 8 rooms)
**Function:** Guided procession corridor with ritual pacing.

- Long vaulted route with branching alcoves for optional lore pickups.
- Bell-ropes, censing racks, confession niches.
- Ambush points by ushers/wardens, but line-of-sight remains readable.

**Gameplay role:** first faction split in mobs (clergy vs guard caste).

---

### Zone C - Censer Galleries (30116-30123, 8 rooms)
**Function:** Hazard wing with atmosphere-first combat.

- Fume-trough chambers, rotating censer gantries, soot script walls.
- Non-lethal hazard messaging in low tier; escalates deeper.
- Optional side vault with crafting/reagent flavor objects.

**Gameplay role:** environmental pressure and tactical pulls.

---

### Zone D - Choir Crypts (30124-30131, 8 rooms)
**Function:** Sound/echo puzzle identity.

- Catacomb aisles where chants alter perceived pathing.
- Reversible side loop themed around "false absolution".
- Mini-encounter with novice cantors and hound pack.

**Gameplay role:** controlled disorientation without true maze flags.

---

### Zone E - Scriptorium of Ash (30132-30139, 8 rooms)
**Function:** Lore-dense archive core.

- Scriveners' desks, chained codices, oath ledgers, bone-ink wells.
- Primary exposition nodes for Kel'Shadra schism and founding crisis.
- Introduce named historical figures (for future quest hooks).

**Gameplay role:** lower combat density, high story reward.

---

### Zone F - Reliquary Bastion (30140-30148, 9 rooms)
**Function:** Martial branch and armory-catacomb interface.

- Armor racks, sepulcher knight stalls, reliquary weapon cages.
- Tight tactical chambers with door control and patrol routes.
- Mid-tier boss: **Ossuary Champion** encounter in central drill nave.

**Gameplay role:** difficulty rise and gear fantasy payoff.

---

### Zone G - Sanguine Cloisters (30149-30157, 9 rooms)
**Function:** Blood-lit monastic courts.

- Cloister walks around inward garden of black thorns.
- Sacramental basins, mirrored confession pools, vow brands.
- Social-horror flavor: handmaidens, interrogators, ritual attendants.

**Gameplay role:** hybrid caster/melee packs, status pressure.

---

### Zone H - Sepulcher Descent (30158-30166, 9 rooms)
**Function:** Endgame corridor and lock-key synthesis.

- Multi-door descent requiring symbols learned earlier (Crescent/Bell/Veil).
- Spirit-memory rooms replaying failed rites.
- Penultimate chamber where prelate authority is contested.

**Gameplay role:** sequence check + narrative culmination.

---

### Zone I - The Inner Gloamvault (30167-30174, 8 rooms)
**Function:** Final sanctum.

- Starless nave, suspended reliquary heart, matriarchal throne crypt.
- Final boss arena for **Matriarch Velastra** with ceremonial guard wave.
- Post-boss reflection room and controlled egress.

**Gameplay role:** boss conclusion, lore resolution, reset-friendly geometry.

---

## Connectivity Model (Readable, Non-Repetitive)

- Main critical path moves generally south/down into depth, then inward to sanctum.
- Each zone has **at least one optional loop** and **one deliberate return connector**.
- Avoid directional paradox loops unless explicitly flagged as magical ritual chambers.
- Boss pathing:
  - Mid-boss in Zone F,
  - Gatekeeper conflict in Zone H,
  - Final boss in Zone I.

---

## Room Plan Density

- **75 total rooms** allocated as: `8 + 8 + 8 + 8 + 8 + 9 + 9 + 9 + 8`.
- Sector use (implementation target):
  - `inside` for most rooms,
  - selective `city`/`field`-like sectors not needed;
  - special room flags only where mechanically justified (boss/no-mob/safe lore cell).

---

## Mob Roster Redesign (Keep Existing Vnum Pool, Reauthor Identity)

Current pool has 20 mobs (`30100-30119`). Keep count and vnums; rewrite each with unique role and stronger ecology.

### Rank Architecture

1. **Initiates / Novices (Lv 6-10)**
   - Ash Acolyte
   - Grave Choir Novice
   - Bone Lantern Bearer

2. **Functionaries (Lv 10-14)**
   - Crypt Usher
   - Bone Scrivener
   - Crypt Interrogator
   - Vault Stalker

3. **Militant Orders (Lv 13-17)**
   - Gloom Sentry
   - Carrion Templar
   - Reliquary Knight
   - Catacomb Marshal
   - Funerary Warden

4. **Ritual Elite / Named (Lv 17-22 cap spike)**
   - Violet Lector
   - Umbral Cantor
   - Ashen Warlock
   - Dusk Prelate (boss-gate)
   - Ossuary Champion (mid-boss)
   - Matriarch Velastra (final boss)
   - Velastra Handmaiden (elite retainer)

### Mob Design Principles

- Every mob gets one clear **institutional job** reflected in description and behavior.
- Keep visual identifiers tied to color bible (purple/magenta/ashen/cyan highlights).
- Bosses should embody distinct encounter verbs:
  - Prelate = command/control,
  - Champion = frontline attrition,
  - Velastra = ritual escalation + summons.

---

## Objectization Redesign (Keep Existing Vnum Pool, Increase Story Value)

Current object pool has 23 objects (`30100-30122`). Keep count/vnums but reauthor to fit zone logic.

### Object Families

1. **Ritual Vestments & Jewelry** (halo, circlet, cowl, chain, mantle, signet)
2. **Militant Relics** (pauldrons, vambraces, gauntlets, carapace, tassets, sabatons)
3. **Office Implements** (lector rod, whisperwire, oathcord)
4. **Bestial/Monstrous Pieces** (ghoulclaw talons, barrowtail wrap, hoofguards)

### Item Story Hooks

- Add short lore snippets per item set tying gear to offices in the vault.
- Establish three mini-sets for progression fantasy:
  - **Bellkeeper set** (utility/defense),
  - **Reliquary set** (melee),
  - **Lector set** (caster/hybrid).

---

## Reset and Encounter Distribution Plan

- Zone A/B: dense but forgiving population.
- Zone C/D: medium density with occasional caster spikes.
- Zone E: lighter combat, more utility/lore NPCs.
- Zone F/G: high density, patrol overlap.
- Zone H: sparse but dangerous elites.
- Zone I: staged final cluster with boss reset choreography.

Reset logic goals:
- Avoid spawn stacking in narrow hallways.
- Ensure at least one recoverable route for lower-level parties.
- Place key lore NPC resets in predictable spaces for repeat readability.

---

## Boss Progression Structure

1. **Ossuary Champion** (Zone F): proves martial order supremacy.
2. **Dusk Prelate** (Zone H): proves doctrinal control over the descent.
3. **Matriarch Velastra** (Zone I): reveals true heresy—vault serves perpetuation, not salvation.

Boss rewards should align to role archetypes and provide visible narrative trophies.

---

## Lore Delivery in Room/Mob/Object Text

Implementation writing standards for this area rewrite:

- No repetitive shared room paragraphing; every room carries one unique memory or function.
- Use environmental storytelling before exposition dumps.
- Echo lore through all layers:
  - Room text (architecture/history),
  - Mob text (institutional role),
  - Object text (material culture).

---

## Expansion Budget Inside Same Vnum Envelope

Because range is fixed, expansion is qualitative rather than numeric:

- Add **micro-encounter scripts** (bells, censers, oaths) via room flavor/events.
- Add **cross-zone breadcrumbing** where item descriptions foreshadow later rooms.
- Add **named dead** in epitaphs to seed future quest arcs.

---

## Cross-Area Lore Alignment Targets

This redesign explicitly aligns with broader setting motifs used in other plans/lore docs:

- Institutional memory through ledgers, covenants, and doctrine.
- Catastrophe framed as systemic failure rather than random evil.
- Ruins that still function as social machines (not dead scenery).
- Rumor pathways that can bridge to Midgaard/Kiess/Kowloon style archival questing.

---

## Deliverables for Build Phase (Out of Scope for This Document)

1. Rewrite all 75 room names/descriptions/exits in `gloamvault.are`.
2. Rewrite all 20 mobile records for ecology and role fidelity.
3. Rewrite all 23 object records with set identity and lore tags.
4. Rebuild resets and placements for pacing curve.
5. Add focused extra descriptions in lore-critical rooms.

---

## Appendix: Proposed Room Title Skeleton by Vnum Block

- **30100-30107:** Breach Gate, Shard Court, Outer Bellwalk, Oath-Splinter Hall
- **30108-30115:** Ash Processional, Censer Colonnade, Confession Nicheway
- **30116-30123:** Smoke Engine Gallery, Resin Vault, Soot Reliquary
- **30124-30131:** Choir Ossuary, Echo Chantry, Bone Refrain Corridor
- **30132-30139:** Ledger Nave, Ash Scriptorium, Black Ink Treasury
- **30140-30148:** Knight Stalls, Armiger Crypt, Champion Drill Nave
- **30149-30157:** Thorn Cloister, Sanguine Arcade, Veiled Basin Court
- **30158-30166:** Triple-Sign Descent, Penitent Spiral, Prelate's Sealway
- **30167-30174:** Inner Gloamvault, Velastra's Throne Crypt, Reliquary Heart, Ashen Egress

