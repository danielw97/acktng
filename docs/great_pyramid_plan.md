# Area Plan: The Great Pyramid (Full Rebuild)

## Overview

- **Area file:** `pyramid.are`
- **Area name (retained):** `@@yThe Great Pyramid@@N`
- **Keyword direction:** move from broad `pyramid ancient egypt pharaoh` to a focused, searchable set keyed to the new identity (`greatpyramid solarcourt blacksun covenant`) while preserving legacy `pyramid` discoverability.
- **Level range (retained):** `I 90 100`
- **Vnum envelope (strictly retained):** `V 30480 30684`
- **Envelope size:** 205 vnums
- **Current envelope usage:** 205 rooms, 91 mobs, 86 objects
- **Rebuild intent:** Redesign **all rooms, all mobiles, and all objects** to align with a unified narrative of royal ascension, funerary judgment, and an imprisoned anti-solar force.

This plan keeps the existing vnum envelope intact while replacing the area’s structure with a cleaner floor progression, stronger lore continuity, and a tighter encounter arc from outer approach to apex seal.

---

## Multi-Color Theme (Area-Wide)

### Palette

- **Sun-gold / royal authority:** `@@y`
- **Sandstone / old desert stone:** `@@b`
- **Lapis / celestial ritual geometry:** `@@c`
- **Moonlit funerary silver:** `@@W`
- **Duat shadow / undercrypt pressure:** `@@d`
- **Curse / blood-ritual warnings:** `@@R`
- **Judgment / spiritual resonance:** `@@a`
- **Void-taint / Black Sun influence:** `@@p`
- **Reset:** `@@N` on all colorized strings

### Color Usage Rules

- Public rooms favor `@@y`, `@@b`, `@@W`; deep crypts shift toward `@@d`, `@@a`, `@@p`.
- Boss rooms use one dominant color and one accent to keep identity readable.
- Do **not** use `@@k`, background codes (`@@0-@@7`), or `@@f`.
- Exit keywords with `^` must be visible in room text and can be lightly color-marked for discoverability.

---

## Rebuild Goals

1. Replace scattered “cool room names” structure with a coherent pilgrimage-to-judgment progression.
2. Preserve the power fantasy of fighting iconic Egyptian-themed bosses while giving each one a clear narrative role.
3. Make objectization intentional: each gear family should map to one culture pillar (solar court, jackal tribunal, embalmer caste, war cult, black sun).
4. Improve traversal readability (clear hubs, side loops, ritual shortcuts) without collapsing exploration depth.
5. Align with broader desert arc from nearby oasis/desert plans: caravan rumor -> lost dynasty theology -> anti-solar corruption.

---

## Lore-Driven Structural Concept

The pyramid is rebuilt as a ceremonial machine with three concurrent functions:

- **State temple** (public legitimacy: tribute, priesthood, military theater)
- **Funerary engine** (soul weighing, memory preservation, noble interment)
- **Containment prison** (the Black Sun shard held beneath the throne line)

Players enter through the public façade, descend into the true death-economy, climb into royal sancta, and finally confront the force the dynasty was actually built to bind.

---

## Vnum Allocation (Strict Envelope Use)

All content remains within `30480-30684`.

### Rooms (`30480-30684`, 205 total)

Use all room vnums sequentially with a floor-based macro layout:

- **Zone A (30480-30509): Dune Approach & Processional Court** — 30 rooms
- **Zone B (30510-30544): Priestly Works & State Galleries** — 35 rooms
- **Zone C (30545-30584): Catacomb Transit & Servitor Depths** — 40 rooms
- **Zone D (30585-30624): Halls of Weighing & Royal Archives** — 40 rooms
- **Zone E (30625-30654): Crown Ascension & Divine War Sancta** — 30 rooms
- **Zone F (30655-30684): Black Sun Vault & Apex Seal** — 30 rooms

### Mobiles (`30480-30570`, 91 total)

Maintain full existing mobile range and reassign by faction families:

- 18 Processional/Surface defenders
- 16 Priestly and scribe-cult casters
- 18 Catacomb dead/construct servants
- 16 Tribunal and judgment elites
- 13 Royal warline champions
- 10 Apex/Black Sun entities (including final chain)

### Objects (`30480-30565`, 86 total)

Retain full object range; redesign into item sets and encounter drops:

- 20 common temple/catacomb drops
- 18 caster/ritual focus items
- 18 martial/defender sets
- 20 boss-signature relics
- 10 utility/keys/ritual progression pieces

---

## Zone-by-Zone Room Plan

## Zone A (30480-30509): Dune Approach & Processional Court

**Narrative role:** The visible, political face of the pyramid.

- Entry from desert caravan route into ceremonial causeway.
- Guarded plazas, tribute queues, cracked obelisks, and lion gate thresholds.
- Environmental storytelling: abandoned tribute crates, half-erased cartouches, cult graffiti hinting at hidden rites below.

**Key rooms:**

- `30480` **The Wind-Cut Causeway** (new entry anchor)
- `30484` **The Court of Tall Banners** (public military display)
- `30488` **The Sun-Lion Gate** (first hard checkpoint)
- `30502` **Hall of Tribute Tallies** (bureaucratic flavor hub)
- `30509` **The Processional Sink Stair** (drop into Zone B)

**Design notes:**

- Mostly `desert`/`inside` transitions.
- Low-risk combat teaching anti-undead and anti-poison expectations.
- Two optional side loops with early lore tablets.

## Zone B (30510-30544): Priestly Works & State Galleries

**Narrative role:** Where kingship is manufactured by ritual and recordkeeping.

- Embalmer annexes, incense cisterns, tax halls, oath chapels, and ceremonial armory.
- Vertical passages reveal “public religion” vs “restricted funerary practice.”

**Key rooms:**

- `30513` **The Chamber of Seven Inks**
- `30518` **The Embalmer’s Kiln Corridor**
- `30526` **Gallery of Oathbound Viziers**
- `30533` **The Scribe-Lattice Rotunda**
- `30544` **The Priest-King Descent** (to catacomb layers)

**Design notes:**

- Add frequent `E` descriptions for decrees, seals, and altered doctrine.
- Mid-tier caster mobs introduced here.
- First ritual-lock/key motif appears.

## Zone C (30545-30584): Catacomb Transit & Servitor Depths

**Narrative role:** Labor and death infrastructure hidden beneath the state temple.

- Ossuaries, corpse ramps, spirit drains, failed embalming bays.
- Spatial feel should be compressive and maze-adjacent without violating clear traversal logic.

**Key rooms:**

- `30548` **The Bone Conveyor Hall**
- `30556` **Crypt of Interrupted Names**
- `30563` **The Carrion Drain Junction**
- `30572` **Servitor Barracks of Linen and Salt**
- `30584` **The Gate of Weighed Breath** (transition to judgment complex)

**Design notes:**

- Strong `@@d`/`@@a` palette with occasional `@@R` warning glyphs.
- Heavier undead + construct mix.
- One optional mini-boss alcove grants tribunal key fragment.

## Zone D (30585-30624): Halls of Weighing & Royal Archives

**Narrative role:** Theology becomes machinery; souls are audited like ledgers.

- Scale halls, feather courts, confession cloisters, false-memory libraries.
- The area’s moral core: who was judged, who was rewritten, and why.

**Key rooms:**

- `30589` **The First Weighing Dais**
- `30597` **Hall of Confessions Unheard**
- `30603` **Archive of Corrected Dynasties**
- `30615` **Tribunal of the Jackal Masks** (major mini-boss)
- `30624` **The Royal Aperture Stair** (ascension start)

**Design notes:**

- This zone carries the deepest lore exposure through inscriptions and ghost echoes.
- Encounter scripts should punish pure burst and reward control/interrupt play.
- Multiple side chambers provide set-piece loot paths.

## Zone E (30625-30654): Crown Ascension & Divine War Sancta

**Narrative role:** Military-divine layer where pharaoh is recast as cosmic warlord.

- Trophy halls, war standards, divine armory galleries, solar beast pens.
- Boss chain escalates from champions to deity-avatar proxies.

**Key rooms:**

- `30627` **The Spear Vault of Dawn**
- `30634` **The Chamber of Two Crowns**
- `30641` **Sanctum of Horus-Bound Steel**
- `30648` **The Lioness Blood Court**
- `30654` **The Golden Lift of Ascension**

**Design notes:**

- High-contrast `@@y` + `@@R` + `@@c` visual identity.
- Heavy elite placement; paced safe pockets for recovery.
- Signature boss relic drops begin here.

## Zone F (30655-30684): Black Sun Vault & Apex Seal

**Narrative role:** The hidden truth—solar kingship exists to restrain a devouring star-fragment.

- Fractured seal corridors, anti-light chambers, void-polished sarcophagi.
- Final chain reveals betrayal among priesthood, vizier line, and pharaoh succession.

**Key rooms:**

- `30658` **The Inverted Sun Gallery**
- `30663` **The Chamber of Broken Cartouches**
- `30671` **Vault of the First Betrayal**
- `30679` **The Black Sun Cradle**
- `30684` **The Absolute Summit** (final encounter room retained as title anchor)

**Design notes:**

- Palette shift to `@@p`/`@@d` with constrained `@@y` remnants.
- Final encounters blend undead royalty, astral serpent imagery, and construct containment failure.
- End-state room text should clearly communicate cycle reset implications.

---

## Encounter & Mob Rebuild Plan (91 Mobiles)

## Faction Families

1. **Court Guard Cohorts** (front-facing military, spears/shields, anti-rush).
2. **Embalmer and Scribe Orders** (debuff, disease, curse stacks, ritual buffs).
3. **Catacomb Servitors** (undead laborers, scarabs, wights, corpse-haulers).
4. **Jackal Tribunal** (judges, executioners, weighers, oath enforcers).
5. **Royal Warline** (named champions tied to gods/war standards).
6. **Black Sun Host** (void-tainted elite and final chain entities).

## Named Boss Ladder (retain iconic names; redefine purpose)

- **High Priest Amenhotep** -> doctrine gatekeeper at end of Zone B.
- **Vizier Khamsin the Deceiver** -> archive falsifier and political usurper in Zone D.
- **Avatar of Ra** -> unstable defense construct in Zone E.
- **Champion of Anubis** -> tribunal execution anchor in Zone D/E transition.
- **Lady Sekhmet** -> berserk blood-court boss in Zone E.
- **Pharaoh Ramesses the Eternal** -> penultimate sovereign fight in Zone F.
- **The Tomb Architect** -> mechanics-heavy seal engineer encounter in Zone F.
- **Apep the Serpent God** -> anti-solar apex predator manifestation in Zone F.
- **High Sorceress Neferu** -> ritual controller enabling Black Sun emergence.
- **The Golden Colossus** -> final containment guardian, now corrupted by Black Sun radiation.

## Mechanical Identity Targets

- Avoid redundant “melee sack” mobs by giving each family one distinct combat utility.
- Keep poison pressure in serpent lines, fear/curse in tribunal lines, burst windows in warline elites.
- Ensure final 5 bosses each have one unmistakable signature mechanic (seal phase, adds, reflect cycle, etc.).

---

## Object Rebuild Plan (86 Objects)

## Item Families

- **Set A: Processional Bronze** (starter rares, balanced stats).
- **Set B: Linen of Consecration** (caster sustain, resist utility).
- **Set C: Ossuary Iron** (tank/melee mitigation).
- **Set D: Tribunal Regalia** (hybrid control/accuracy bonuses).
- **Set E: Solar War Relics** (high offense, conditional drawbacks).
- **Set F: Black Sun Artifacts** (end-tier, risk/reward identity).

## Signature Existing Items to Retheme, Not Delete

- `ka_stone` -> progression keystone for vault access and lore artifact.
- `winged_mantle`, `halo_of_devotion`, `gauntlet_of_the_golden_colossus` -> boss identity pieces.
- `cloak_of_desert_shadows`, `void_crown`, `death_mask` -> transition bridge between funerary and void themes.

## Objectization Quality Rules

- Every major boss must drop at least one lore-recognizable signature item.
- No generic “+stats only” end boss drops; each apex item gets flavor text tied to lore events.
- Utility objects (keys, seals, tablets) should support exploration and storytelling, not just gating.

---

## Connectivity & Navigation Plan

- Mainline progression is mostly linear by zone but each zone has 2–3 meaningful side branches.
- Hubs at `30509`, `30544`, `30584`, `30624`, `30654`.
- No confusing dead-end spam: dead ends must include either lore payoff, loot payoff, or traversal unlock.
- Use named exits (`^sun-lion gate`, `^weighing stair`, `^golden lift`) with explicit mention in room descriptions.
- Maintain bi-directional logic for normal travel; one-way ritual drops only where lore justifies and recovery route exists.

---

## Reset, Progression, and Pacing

- Keep reset cadence close to current (`F 15`) unless broader world pacing needs adjustment.
- Zone A/B: denser trash, low wipe risk.
- Zone C/D: attrition core and lore density peak.
- Zone E: elite gauntlet and set-piece bosses.
- Zone F: short but intense finale chain with minimal filler.
- Optional challenge path: trigger Black Sun “unstable state” variant with stronger adds but improved relic rolls.

---

## Integration Notes with Broader Desert Arc

To harmonize with adjacent pyramid/desert plans:

- Borrow **civilization layering** style from oasis/desert plans: trade, ritual bureaucracy, then buried truth.
- Borrow **multi-stage boss escalation** from northern/southern pyramid planning patterns.
- Reinforce shared cosmology symbols (sun disk, feather judgment, jackal tribunal), but differentiate this pyramid via explicit Black Sun containment mythology.

---

## Implementation Checklist

1. Rewrite room titles/descriptions for all `30480-30684` with zone-consistent palette.
2. Reassign and rewrite all 91 mobs (`30480-30570`) to new factions and roles.
3. Rebuild all 86 objects (`30480-30565`) into coherent set families and boss signatures.
4. Rebuild resets to match zone pacing and boss ladder.
5. Validate directionality, named exits, and description discoverability.
6. Verify color policy and string formatting (`~`, no malformed long_descr).
7. Run full area load validation and in-game traversal pass from entry to final room.

