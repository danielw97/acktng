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

## Lore

See `great_pyramid_lore.md` for extracted lore.

## `#AREA` Header Plan (spec-compliant)

All directives per `area_file_spec.md` section 3:

- `Q 16` — mandatory area version; no other value is valid
- `K greatpyramid~` — single-word keyword, no spaces, no color codes, `~`-terminated
- `L @@W(@@y90 100@@W)@@N~` — white parens, yellow level numbers, white close paren, reset, `~`-terminated
- `N <allocate at implementation time>` — area number; must be confirmed unused across all loaded area files
- `I 90 100` — min/max level; controls mob-level gating and area matching
- `V 30480 30684` — full 205-vnum envelope; all rooms, mobs, and objects must stay within this range
- `F 15` — reset frequency in minutes
- `U @@yancient stone groans as desert wind funnels through the Great Pyramid’s sealed passages@@N~` — reset message; no double-newlines, `~`-terminated
- `O Virant~` — area owner must always be Virant (owner policy)
- `T` — teleport flag line (rest ignored by loader); presence enables teleport into the area
- `R`, `W` — ACL directives omitted; set at implementation time by the builder

### Global String and Description Requirements

Per `area_file_spec.md` section 2:

- All text fields must be `~`-terminated. Missing `~` causes parse failure.
- Strings must not contain back-to-back newlines (no `\n\n` blank lines).
- Vnums must never appear in any in-world description text (room descriptions, mob descriptions, object descriptions, extra descriptions, exit descriptions).
- Mobile `long_descr` must be exactly one text line followed by a newline and a `~`-only line. Multi-line `long_descr` is never valid.
- Mobile `description` must end with exactly one trailing newline before the terminating `~`.
- Room `<description>~` must end with exactly one trailing newline before `~`.
- Object extra-description `<description>~` must end with exactly one trailing newline before `~`.

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

### Color Theme Compliance

Per `area_file_spec.md` section 2.1:

- Black foreground (`@@k`) must not be used as a thematic color.
- Background color codes (`@@0`-`@@7`) and flashing (`@@f`) must not be added by automated tools; they are allowed only when explicitly added manually by a human author.
- `@@N` reset must follow all colorized strings.

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

## Room Description Requirements (spec compliance)

Per `area_file_spec.md` section 8:

- Every room `<description>~` must contain at least 3 sentences of text.
- Important rooms (zone entry points, boss rooms, major hub rooms, lore-critical rooms) must contain at least 5 sentences.
- Every room description must be unique to that room (no reused descriptions across rooms).
- Vnums must never appear in any in-world description text.
- Named exits (`<exit_keyword>`) must be discoverable in the room's `<description>~`, an object that spawns in the room, or an `E` extra description in the room.
- Extra-description keyword chains must be anchored: at least one keyword in each chain must appear in the room's main `<description>~`.
- Room descriptions must use the area's color theme intentionally and consistently.
- Each room's `<description>~` must end with exactly one trailing newline before `~`.

## Room Flags and Sector Types

Per `area_file_spec.md` sections 8.1 and 8.2:

- **Outdoor desert rooms** (dune approach, causeway, exterior plazas): `sector_type = desert` (`10`)
- **Interior rooms** (all pyramid interior halls, chambers, galleries, crypts, vaults): `sector_type = inside` (`11`), `room_flags` includes `indoors` (`8`)
- **Boss rooms** (Zone D mini-boss 30615, Zone E bosses, Zone F final encounters): `room_flags` includes `no_mob` (`4`) per boss placement policy
- **Deep interior rooms** (Zone C through Zone F): `room_flags` includes `no_teleport` (`16384`) and `no_recall` (`8192`), except hub/transit rooms that allow recall for escape
- **Zone A outdoor rooms**: `room_flags` includes `hot` (`32`) for desert heat hazard flavor
- **No rooms use `ROOM_MAZE`** (`524288`); this area does not contain mazes

## Vnum Allocation Policy

Per `area_file_spec.md` section 13.1:

- Room vnums must be assigned in ascending sequential order with no gaps.
- All 205 room vnums (30480-30684) must be used; fill every available room-vnum slot.
- Mobile vnums must be assigned in ascending sequential order with no gaps, starting from 30480.
- Object vnums must be assigned in ascending sequential order with no gaps, starting from 30480.

## Directional Traversal Constraints

Per `area_file_spec.md` section 8:

- **All exits must be bi-directional**: if room A connects to room B through direction X, room B must provide the opposite-direction exit back to room A. No one-way exits (this area is not a maze).
- **No directional loops**: repeated movement in the same direction must not return to a previously visited room. The spec prohibits room connection loops (e.g., `a -> b -> c -> a`) unless all rooms in the set are flagged `ROOM_MAZE`.
- **Branching paths replace loops**: the plan uses hub-and-spoke branching from 5 major hub rooms (30509, 30544, 30584, 30624, 30654) rather than circular loops. Players choose branches and backtrack through hubs.
- **External exits preserved** where legacy worldgraph depends on them, but contextualized via renamed rooms/exits with bi-directional counterparts.
- **Door usage** limited to ritual gates, vault shutters, and seal barriers. Every door exit must have `EX_ISDOOR` (`1`) set in `<locks>`. Door initial state (open/closed/locked) is authored through `#RESETS` command `D`, not through room exit `<locks>` bits. Any locked door (`D` state `2`) must have `<key_vnum>` set to a valid key object vnum, and that key object must exist in `#OBJECTS`.
- **Named exits** use `^` prefix on keywords (e.g., `^sun-lion gate`, `^weighing stair`, `^golden lift`) so movement messaging treats them as standalone noun phrases. Each named exit keyword must appear in the room's `<description>~`, an object in the room, or an `E` extra description.

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

**Room flags and sector types:**

- Outdoor rooms (30480-30487): `sector_type = desert` (`10`), `room_flags` includes `hot` (`32`).
- Transitional rooms (30488-30509): `sector_type = inside` (`11`), `room_flags` includes `indoors` (`8`).
- Entry anchor 30480: important room, 5+ sentence description.
- Sun-Lion Gate 30488: named exit `^sun-lion gate` discoverable in description; `EX_ISDOOR` set; door state authored via `#RESETS` `D`.
- Hub room 30509: important room, 5+ sentence description.

**Design notes:**

- Low-risk combat teaching anti-undead and anti-poison expectations.
- Two optional side branches with early lore tablets (branch from hub, dead-end with lore payoff, backtrack).
- Multiple `E` extra descriptions on obelisks and cartouches for environmental storytelling.

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

**Room flags and sector types:**

- All rooms: `sector_type = inside` (`11`), `room_flags` includes `indoors` (`8`).
- Hub room 30544: important room, 5+ sentence description.
- Lore rooms (30513, 30526, 30533): `E` extra descriptions for inscriptions, seals, and doctrine tablets; keyword chains anchored in main `<description>~`.

**Design notes:**

- Add frequent `E` descriptions for decrees, seals, and altered doctrine.
- Mid-tier caster mobs introduced here.
- First ritual-lock/key motif appears: locked door at 30544 (Priest-King Descent) requires a key object that must exist in `#OBJECTS`.

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

**Room flags and sector types:**

- All rooms: `sector_type = inside` (`11`), `room_flags` includes `indoors` (`8`), `no_teleport` (`16384`), `no_recall` (`8192`).
- Exception: transit room 30584 (Gate of Weighed Breath) omits `no_recall` so players can escape between zones.
- Mini-boss alcove: `room_flags` includes `no_mob` (`4`).
- Hub room 30584: important room, 5+ sentence description.

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

**Room flags and sector types:**

- All rooms: `sector_type = inside` (`11`), `room_flags` includes `indoors` (`8`), `no_teleport` (`16384`), `no_recall` (`8192`).
- Exception: transit room 30624 (Royal Aperture Stair) omits `no_recall`.
- Tribunal mini-boss room 30615: `room_flags` includes `no_mob` (`4`); important room, 5+ sentence description.
- Hub room 30624: important room, 5+ sentence description.
- Lore rooms (30589, 30597, 30603): heavy `E` extra descriptions for weighing inscriptions, confession tablets, and corrected dynasty records; keyword chains anchored in main `<description>~`.

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

**Room flags and sector types:**

- All rooms: `sector_type = inside` (`11`), `room_flags` includes `indoors` (`8`), `no_teleport` (`16384`), `no_recall` (`8192`).
- Exception: transit room 30654 (Golden Lift of Ascension) omits `no_recall`.
- Boss rooms (Lady Sekhmet's Blood Court 30648, Avatar of Ra's chamber): `room_flags` includes `no_mob` (`4`); important rooms, 5+ sentence descriptions.
- Hub room 30654: important room, 5+ sentence description; named exit `^golden lift` discoverable in description.

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

**Room flags and sector types:**

- All rooms: `sector_type = inside` (`11`), `room_flags` includes `indoors` (`8`), `no_teleport` (`16384`), `no_recall` (`8192`).
- Exception: final room 30684 (The Absolute Summit) omits `no_recall` so players can escape after the final encounter.
- All boss rooms (30671 Vault of the First Betrayal, 30679 Black Sun Cradle, 30684 The Absolute Summit): `room_flags` includes `no_mob` (`4`); important rooms, 5+ sentence descriptions.
- Named exits: `^inverted sun gallery`, `^black sun cradle` discoverable in descriptions.
- Locked seal barriers: doors into 30679 and 30684 require key objects; `EX_ISDOOR` set; door state authored via `#RESETS` `D` state `2`; corresponding key objects must exist in `#OBJECTS`.

**Design notes:**

- Palette shift to `@@p`/`@@d` with constrained `@@y` remnants.
- Final encounters blend undead royalty, astral serpent imagery, and construct containment failure.
- End-state room text should clearly communicate cycle reset implications.

---

## Encounter & Mob Rebuild Plan (91 Mobiles)

### Mob Vnum Strategy

Per `area_file_spec.md` section 13.1:

- Mob vnums must be assigned in ascending sequential order with no gaps, starting from 30480.
- Fully rewrite all 91 mob templates through contiguous sequential slots **30480-30570** (staying within the 30480-30684 vnum envelope).

### Mobile Spec Compliance (all mobs)

Per `area_file_spec.md` sections 4 and 5.4:

- **All mobs** must have `act` flags including `is_npc` (`1`) and `stay_area` (`64`).
- **Sentinel mobs** (guards, gatekeepers, stationary NPCs): add `sentinel` (`2`) to `act` flags.
- **Boss mobs** (Pharaoh Ramesses, Apep, The Golden Colossus, and other named bosses placed in `no_mob` rooms): must have `act` flags including `sentinel` (`2`) and `boss` (`67108864`), and must be placed only in rooms flagged `no_mob` (`4`).
- **Strong non-boss mobs** (elite faction leaders, veteran champions, tribunal executioners): must have `act` flags including `solo` (`33554432`).
- **Aggressive mobs** (hostile guardians, undead, constructs, court remnants): set `aggressive` (`32`) in `act` flags.
- **Undead mobs** (catacomb servitors, wights, mummified priests, judgment ghosts): set `undead` (`16384`) in `act` flags.
- **`invasion`** (`536870912`) must never be set in area files; it is runtime-only.
- Mobile `long_descr` must be exactly one text line followed by a newline and a `~`-only line. Multi-line `long_descr` is never valid under any circumstances.
- Mobile `description` must end with exactly one trailing newline before the terminating `~`.
- Mobile `player_name` (keyword), `short_descr`, `long_descr`, and `description` are all `~`-terminated.

### Mobile Extension Lines

Per `area_file_spec.md` sections 4.1-5.5:

- `!` extension: `<class> <clan> <race> <position> <skills> <cast> <def>` — used for combat-capable mobs. Skill bitvectors follow the level-gated floor policy tables (melee/hybrid/caster profiles).
- `|` extension: `<strong_magic> <weak_magic> <race_mods> <power_skills> <power_cast> <resist> <suscept>` — used for elemental affinities. Pyramid mobs should generally have `resist` including `fire` (`128`); undead mobs should have `suscept` including `holy` (`8`).
- `+` extension: combat modifier tuning for significant encounters.
- `l`/`L` extensions: loot tables. `loot_amount` is percent-based (see spec). Items in loot tables must have `ITEM_LOOT` set in `extra_flags`.

## Faction Families

1. **Court Guard Cohorts** (front-facing military, spears/shields, anti-rush).
   - `act`: `is_npc` + `stay_area` + `aggressive`. Gate guards add `sentinel`.
   - Level range: 90-93.
   - Archetype: melee profile.

2. **Embalmer and Scribe Orders** (debuff, disease, curse stacks, ritual buffs).
   - `act`: `is_npc` + `stay_area` + `aggressive`. Stationary scribes add `sentinel`.
   - Level range: 91-95. Scribe-casters use caster profile; embalmers use hybrid profile.
   - `affected_by`: scribe-casters get `detect_magic` (`16`); embalmers get `detect_hidden` (`32`).

3. **Catacomb Servitors** (undead laborers, scarabs, wights, corpse-haulers).
   - `act`: `is_npc` + `stay_area` + `aggressive` + `undead` (`16384`). Construct servants add `no_mind` (`262144`).
   - Level range: 92-96.
   - Archetype: melee profile. Scarab swarms use caster profile (poison/disease casting).
   - `resist`: `fire` (`128`), `physical` (`2`). `suscept`: `holy` (`8`), `water` (`64`).

4. **Jackal Tribunal** (judges, executioners, weighers, oath enforcers).
   - `act`: `is_npc` + `stay_area` + `aggressive`. Executioners add `no_flee` (`16`).
   - Level range: 94-97. Judges use hybrid profile; executioners use melee profile.
   - Strong non-boss executioners: add `solo` (`33554432`).
   - `affected_by`: judges get `detect_invis` (`8`) + `detect_evil` (`4`).

5. **Royal Warline** (named champions tied to gods/war standards).
   - `act`: `is_npc` + `stay_area` + `aggressive` + `no_flee` (`16`).
   - Level range: 95-99. Melee profile throughout.
   - Strong non-boss champions: add `solo` (`33554432`).

6. **Black Sun Host** (void-tainted elite and final chain entities).
   - `act`: `is_npc` + `stay_area` + `aggressive` + `undead` (`16384`). Final chain entities add `no_flee` (`16`).
   - Level range: 97-100. Hybrid profile (void casting + melee).
   - `affected_by`: `detect_invis` (`8`) + `infrared` (`512`).
   - `resist`: `shadow` (`256`), `mental` (`4`). `suscept`: `holy` (`8`).

## Named Boss Ladder (retain iconic names; redefine purpose)

All boss mobs: `act` includes `is_npc` + `stay_area` + `sentinel` + `boss` + `aggressive` + `no_flee`. Placed only in rooms flagged `no_mob`. Items dropped must include `ITEM_BOSS` flag.

- **High Priest Amenhotep** -> doctrine gatekeeper at end of Zone B. Level 93. Hybrid profile with defensive casting. Assigned `spec_cast_cleric` in `#SPECIALS`.
- **Vizier Khamsin the Deceiver** -> archive falsifier and political usurper in Zone D. Level 96. Hybrid profile with offensive casting. Assigned `spec_cast_mage` in `#SPECIALS`. `act` adds `remember` (`8`).
- **Champion of Anubis** -> tribunal execution anchor in Zone D/E transition. Level 97. Melee profile. `affected_by`: `sanctuary` (`128`), `detect_invis` (`8`).
- **Avatar of Ra** -> unstable defense construct in Zone E. Level 98. Hybrid profile. Assigned `spec_cast_bigtime` in `#SPECIALS`. `act` adds `no_body` (`65536`) + `no_blood` (`268435456`).
- **Lady Sekhmet** -> berserk blood-court boss in Zone E. Level 98. Melee profile. `act` adds `remember` (`8`). `affected_by`: `sanctuary` (`128`).
- **High Sorceress Neferu** -> ritual controller enabling Black Sun emergence. Level 99. Caster profile. Assigned `spec_cast_mage` in `#SPECIALS`. `affected_by`: `sanctuary` (`128`), `detect_invis` (`8`), `detect_magic` (`16`).
- **Pharaoh Ramesses the Eternal** -> penultimate sovereign fight in Zone F. Level 99. Hybrid profile. Assigned `spec_cast_undead` in `#SPECIALS`. `act` adds `undead` + `remember`. `affected_by`: `sanctuary` (`128`), `detect_invis` (`8`), `infrared` (`512`).
- **The Tomb Architect** -> mechanics-heavy seal engineer encounter in Zone F. Level 99. Hybrid profile. `act` adds `no_mind` (`262144`). Assigned `spec_cast_mage` in `#SPECIALS`.
- **Apep the Serpent God** -> anti-solar apex predator manifestation in Zone F. Level 100. Hybrid profile with strong offensive casting. Assigned `spec_cast_bigtime` in `#SPECIALS`. `act` adds `no_body` + `no_blood`. `affected_by`: `sanctuary` (`128`), `detect_invis` (`8`), `infrared` (`512`). Loot table (`l`/`L`) used for boss drops.
- **The Golden Colossus** -> final containment guardian, now corrupted by Black Sun radiation. Level 100. Melee profile. `act` adds `no_body` + `no_blood` + `remember`. `affected_by`: `sanctuary` (`128`), `anti_magic` (`2097152`). Loot table (`l`/`L`) used for boss drops.

## Mechanical Identity Targets

- Avoid redundant “melee sack” mobs by giving each family one distinct combat utility.
- Keep poison pressure in serpent lines, fear/curse in tribunal lines, burst windows in warline elites.
- Ensure final 5 bosses each have one unmistakable signature mechanic (seal phase, adds, reflect cycle, etc.).

---

## Object Rebuild Plan (86 Objects)

### Object Vnum Strategy

Per `area_file_spec.md` section 13.1:

- Object vnums must be assigned in ascending sequential order with no gaps, starting from 30480.
- Fully replace current object concepts beginning at **30480**.
- Expand through contiguous sequential slots (through **30565**) while staying within the 30480-30684 vnum envelope.
- Within the area, object `<name>~` values must be unique (no duplicate item names in the same area file).

### Object Spec Compliance (all objects)

Per `area_file_spec.md` section 7:

- **Every object must include `ITEM_TAKE`** (`8388608`) in `wear_flags`. No exceptions.
- **No object may include `ITEM_WEAR_CLAN_COLORS`** (`16777216`) in `wear_flags`.
- Object `name`, `short_descr`, and `description` must be thematically consistent with the object's non-`take` wear flags (e.g., a `head` item must read as headgear, `hold` as a held item).
- **Area-authored items must not define fixed stats** in the area file. Item stats are generated at runtime from item level and other runtime parameters. Author object identity/behavior data only (type/flags/wear/apply/value layout/weight/level), not handcrafted stat tuning.
- **`ITEM_GENERATED`** (`1`) must not be set in area-authored objects; it is runtime-managed.
- **`ITEM_LOOT`** (`67108864`): items spawned from a mobile's loot table (`l`/`L` extension) must have this set. Items loaded via `#RESETS` (`G`/`E`) must not have it unless explicitly intended as loot-only.
- **`ITEM_BOSS`** (`134217728`): any item that can drop from a boss mob must include this flag, regardless of delivery method (loot table or reset equipment).
- Object extra-description `<description>~` must end with exactly one trailing newline before `~`.

### Weight Ranges (archetype encoding)

Per `area_file_spec.md` section 7:

- `1`-`5`: caster item
- `6`-`10`: melee item
- `11`-`15`: tank item

### Weapon Constraints

Per `area_file_spec.md` sections 7.3 and 7.6:

- Weapons must have `item_type = ITEM_WEAPON` (`5`) and `wear_flags` including both `hold` (`32768`) and `take` (`8388608`).
- `value3` (attack type) must be thematically consistent with the weapon's concept and presentation (`name`, `short_descr`, `description`).
- `value3 = 0` (`hit`) must not be used unless the object also has `ITEM_FIST` extra flag.
- **Two-handed weapons**: if a weapon's identity clearly indicates a two-handed archetype (e.g., "great khopesh", "ceremonial halberd"), it must include `ITEM_TWO_HANDED` (`2147483648`) in `extra_flags`.
- **Bucklers**: `item_type = ITEM_ARMOR`, `wear_flags` includes `hold`, `extra_flags` includes `ITEM_BUCKLER` (`268435456`).
- **Shields**: `item_type = ITEM_ARMOR`, `wear_flags` includes `hold`.

## Item Families

1. **Set A: Processional Bronze** (levels 90-93, Zone A/B starter gear).
   - Ceremonial helms (`head`, weight 11-15 tank), processional shields (`hold` + `ITEM_ARMOR`, weight 11-15 tank), bronze spears (`hold` + `ITEM_WEAPON`, `value3 = 11` pierce, weight 6-10 melee), tribute censers (`hold`, `ITEM_TREASURE`, weight 1-5 caster).
   - `L` level lines: 90-93 range.

2. **Set B: Linen of Consecration** (levels 91-95, Zone B caster/ritual gear).
   - Embalmer hoods (`head`, weight 1-5 caster), consecration wraps (`about`, weight 1-5 caster), scribe staves (`hold` + `ITEM_WEAPON`, `value3 = 7` pound, weight 1-5 caster), ink vials (`hold`, `ITEM_TREASURE`, weight 1-5 caster).
   - `L` level lines: 91-95 range.

3. **Set C: Ossuary Iron** (levels 92-96, Zone C tank/melee gear).
   - Salt-forged hauberks (`body`, weight 11-15 tank), bone-plated bucklers (`hold` + `ITEM_ARMOR` + `ITEM_BUCKLER`, weight 11-15 tank), catacomb picks (`hold` + `ITEM_WEAPON`, `value3 = 11` pierce, weight 6-10 melee), carrion hook blades (`hold` + `ITEM_WEAPON`, `value3 = 3` slash, weight 6-10 melee).
   - `L` level lines: 92-96 range.

4. **Set D: Tribunal Regalia** (levels 94-97, Zone D hybrid gear).
   - Jackal judgment masks (`face`, weight 1-5 caster), tribunal robes (`about`, weight 1-5 caster), sentence rods (`hold` + `ITEM_WEAPON`, `value3 = 7` pound, weight 6-10 melee), feather-scale amulets (`neck`, weight 1-5 caster).
   - `L` level lines: 94-97 range.

5. **Set E: Solar War Relics** (levels 95-99, Zone E high-offense martial gear).
   - Sun-forged great khopeshes (`hold` + `ITEM_WEAPON` + `ITEM_TWO_HANDED`, `value3 = 3` slash, weight 6-10 melee), dawn mantles (`shoulders`, weight 6-10 melee), war crown diadems (`head`, weight 6-10 melee), lion-crested gauntlets (`hands`, weight 11-15 tank).
   - Boss drops: must include `ITEM_BOSS` in `extra_flags`.
   - `L` level lines: 95-99 range.

6. **Set F: Black Sun Artifacts** (levels 97-100, Zone F end-tier risk/reward gear).
   - Void crown (`head`, weight 1-5 caster), death mask (`face`, weight 1-5 caster), black sun scepter (`hold` + `ITEM_WEAPON`, `value3 = 6` blast, weight 1-5 caster), eclipse-forged plate (`body`, weight 11-15 tank), serpent-fang daggers (`hold` + `ITEM_WEAPON`, `value3 = 2` stab, weight 6-10 melee).
   - Boss drops: must include `ITEM_BOSS` in `extra_flags`.
   - Loot-table drops: must include `ITEM_LOOT` in `extra_flags`.
   - `L` level lines: 97-100 range.

## Signature Existing Items to Retheme, Not Delete

- `ka_stone` -> progression keystone for vault access and lore artifact.
- `winged_mantle`, `halo_of_devotion`, `gauntlet_of_the_golden_colossus` -> boss identity pieces.
- `cloak_of_desert_shadows`, `void_crown`, `death_mask` -> transition bridge between funerary and void themes.

## Key Objects for Locked Doors

Per `area_file_spec.md` section 8.3: if any door is reset to locked (`D` state `2`), `<key_vnum>` must be set to a valid key object vnum, and that key object must exist in `#OBJECTS`. Plan the following key objects:

- **Sun-Lion Gate Key** (for Zone A Sun-Lion Gate door at 30488): `ITEM_KEY`, `hold` + `take`, weight 1.
- **Priest-King Descent Seal** (for Zone B descent door at 30544): `ITEM_KEY`, `hold` + `take`, weight 1.
- **Tribunal Gate Seal** (for Zone C/D mini-boss alcove door): `ITEM_KEY`, `hold` + `take`, weight 1.
- **Royal Aperture Key** (for Zone D/E transition at 30624): `ITEM_KEY`, `hold` + `take`, weight 1.
- **Black Sun Cradle Seal** (for Zone F sealed door into 30679): `ITEM_KEY`, `hold` + `take`, weight 1.
- **Apex Summit Key** (for Zone F final door into 30684): `ITEM_KEY`, `hold` + `take`, weight 1.

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

## `#RESETS` Plan (spec-compliant)

Per `area_file_spec.md` section 12:

- `#RESETS` must not contain blank lines.
- Reset commands reference valid vnums within the area's vnum envelope.

### Mob Resets (`M` command)

Format: `M 0 <mob_vnum> <limit> <room_vnum> <notes>`

- **Zone A (Dune Approach):** Reset Court Guard Cohorts at plaza/gate rooms. Reset lion-gate sentinels at 30488 (sentinel, limit 1). Limit 1-2 per mob.
- **Zone B (Priestly Works):** Reset Embalmer and Scribe Order mobs at gallery/archive rooms (30513, 30518, 30526, 30533). Reset boss High Priest Amenhotep at end of Zone B (limit 1).
- **Zone C (Catacomb Transit):** Reset Catacomb Servitors throughout (wights at 30548, 30556; scarab swarms at 30563; construct servants at 30572). Reset zone mini-boss at optional alcove (limit 1).
- **Zone D (Halls of Weighing):** Reset Jackal Tribunal mobs at weighing/archive rooms (30589, 30597, 30603). Reset boss Vizier Khamsin at 30615 (limit 1). Reset Champion of Anubis at Zone D/E transition (limit 1).
- **Zone E (Crown Ascension):** Reset Royal Warline champions at trophy/armory rooms (30627, 30634, 30641). Reset bosses Avatar of Ra and Lady Sekhmet at their respective rooms (limit 1 each). Reset High Sorceress Neferu (limit 1).
- **Zone F (Black Sun Vault):** Reset Black Sun Host entities throughout (30658, 30663, 30671). Reset bosses Pharaoh Ramesses, Tomb Architect, Apep, and Golden Colossus at their respective rooms (limit 1 each).

### Object Resets (`O` command)

Format: `O 0 <obj_vnum> <limit> <room_vnum> <notes>`

- Place lore objects (ka stones, inscription tablets, embalmer tools) in key lore rooms for atmospheric pickup.
- Place key objects near their corresponding doors or on gatekeeper mobs via `G`/`E` resets.

### Equipment Resets (`G`/`E` commands)

Format: `G 0 <obj_vnum> <limit>` (give to previous mob) / `E 0 <obj_vnum> <limit> <wear_loc>` (equip on previous mob)

- Equip Court Guard Cohorts with processional bronze spears and shields.
- Equip Embalmer/Scribe Order mobs with consecration wraps and scribe staves.
- Equip Jackal Tribunal mobs with judgment masks and sentence rods.
- Equip Royal Warline champions with solar war relics (items must include `ITEM_BOSS` flag for boss-equipped items).
- Equip Black Sun Host entities with void-themed artifacts.

### Door Resets (`D` command)

Format: `D 0 <room_vnum> <door_direction> <state>`

Per `area_file_spec.md` section 8.3: any exit set to closed or locked via `D` reset must have `EX_ISDOOR` (`1`) set in its room `D<door>` `<locks>` definition.

Planned door resets:

1. **Zone A Sun-Lion Gate (30488):** Direction into gate complex.
   - Room exit `<locks>`: `door` (`1`). `<key_vnum>`: vnum of Sun-Lion Gate Key object.
   - `D` reset: `D 0 30488 <dir> 1` (closed, not locked — first gate is a checkpoint, not a hard lock).
   - Reverse exit must also have `door` bit set.

2. **Zone B Priest-King Descent (30544):** Direction down (D5) to catacomb layers.
   - Room exit `<locks>`: `door` (`1`). `<key_vnum>`: vnum of Priest-King Descent Seal object.
   - `D` reset: `D 0 30544 5 2` (locked).
   - Reverse exit on Zone C entry must also have `door` bit set.
   - Key object (Priest-King Descent Seal) must exist in `#OBJECTS`.

3. **Zone D/E Royal Aperture Stair (30624):** Direction up (D4) to crown sancta.
   - Room exit `<locks>`: `door` (`1`). `<key_vnum>`: vnum of Royal Aperture Key object.
   - `D` reset: `D 0 30624 4 2` (locked).
   - Reverse exit must also have `door` bit set.
   - Key object (Royal Aperture Key) must exist in `#OBJECTS`.

4. **Zone F Black Sun Cradle (approach to 30679):** Direction into cradle.
   - Room exit `<locks>`: `door` (`1`). `<key_vnum>`: vnum of Black Sun Cradle Seal object.
   - `D` reset: `D 0 <room> <dir> 2` (locked).
   - Reverse exit on 30679 must also have `door` bit set.
   - Key object (Black Sun Cradle Seal) must exist in `#OBJECTS`.

5. **Zone F Apex Summit (approach to 30684):** Direction into final encounter room.
   - Room exit `<locks>`: `door` (`1`). `<key_vnum>`: vnum of Apex Summit Key object.
   - `D` reset: `D 0 <room> <dir> 2` (locked).
   - Reverse exit on 30684 must also have `door` bit set.
   - Key object (Apex Summit Key) must exist in `#OBJECTS`.

All locked doors (state `2`) have corresponding key objects in `#OBJECTS`. Keys can be placed on gatekeeper mobs via `G` resets or in adjacent rooms via `O` resets.

### Spawn Philosophy

- Keep ambient life active on processional courts (Zone A/B) so the zone never feels empty. Use higher limits (2-3) for common guard mobs.
- Concentrate Tribunal elites at weighing/archive chokepoints (Zone D) rather than random saturation. Use limit 1 for elite mobs.
- Use higher mob density in catacomb rooms (Zone C) to create traversal urgency.
- Gate final encounters (Zone F) with locked doors requiring progression keys, providing visible progression gating.
- Zone A/B: denser trash, low wipe risk.
- Zone C/D: attrition core and lore density peak.
- Zone E: elite gauntlet and set-piece bosses.
- Zone F: short but intense finale chain with minimal filler.
- Optional challenge path: trigger Black Sun “unstable state” variant with stronger adds but improved relic rolls.

---

## `#SPECIALS` Plan

Per `area_file_spec.md` section 10:

- `spec_cast_cleric` assigned to High Priest Amenhotep (Zone B boss; ritual defensive casting).
- `spec_cast_mage` assigned to Vizier Khamsin the Deceiver (Zone D boss; offensive arcane casting).
- `spec_cast_mage` assigned to High Sorceress Neferu (Zone F; ritual offensive casting).
- `spec_cast_mage` assigned to The Tomb Architect (Zone F; engineering-themed casting).
- `spec_cast_bigtime` assigned to Avatar of Ra (Zone E boss; unstable high-power casting).
- `spec_cast_bigtime` assigned to Apep the Serpent God (Zone F boss; anti-solar apex casting).
- `spec_cast_undead` assigned to Pharaoh Ramesses the Eternal (Zone F boss; undead sovereign casting).
- `spec_poison` assigned to serpent-line mobs in Zone C/F (venom pressure ecology).
- No `spec_summon_*` specials (runtime-only, builder-prohibited).
- No `spec_keep*` specials (manual-assignment only).

---

## Canonical Section Order

Per `area_file_spec.md` section 14, the final `.are` file should emit sections in this order:

1. `#AREA`
2. `#ROOMS`
3. `#MOBILES`
4. `#MOBPROGS` (if any inline mobprogs are used)
5. `#OBJECTS`
6. `#SHOPS` (none planned for this area)
7. `#RESETS`
8. `#SPECIALS`
9. `#OBJFUNS` (none planned for this area)
10. `#$` (end-of-file marker)

---

## Integration Notes with Broader Desert Arc

To harmonize with adjacent pyramid/desert plans:

- Borrow **civilization layering** style from oasis/desert plans: trade, ritual bureaucracy, then buried truth.
- Borrow **multi-stage boss escalation** from northern/southern pyramid planning patterns.
- Reinforce shared cosmology symbols (sun disk, feather judgment, jackal tribunal), but differentiate this pyramid via explicit Black Sun containment mythology.

---

## Implementation Checklist

### Rooms
- [ ] Populate all 205 room vnums (30480-30684) in ascending sequential order with no gaps.
- [ ] Write unique `<description>~` for every room (minimum 3 sentences; 5+ for important rooms).
- [ ] Assign correct `sector_type` (`desert` for outdoor Zone A rooms, `inside` for all interior rooms) and `room_flags` per zone requirements.
- [ ] Set `no_mob` on all boss rooms.
- [ ] Set `no_teleport` + `no_recall` on Zone C through Zone F rooms (except hub/transit rooms that allow recall for escape).
- [ ] Set `hot` on Zone A outdoor desert rooms.
- [ ] Set `indoors` on all `inside` sector rooms.
- [ ] Ensure all exits are bi-directional with no directional loops.
- [ ] Ensure all named exits use `^` prefix and are discoverable in room description, objects, or extra descriptions.
- [ ] Ensure all extra-description keyword chains are anchored in the room's main `<description>~`.
- [ ] Preserve required worldgraph external connectors while renaming and recontextualizing local rooms.

### Mobiles
- [ ] Write all 91 mob templates (vnums 30480-30570) in ascending sequential order with no gaps.
- [ ] Ensure all mobs have `is_npc` + `stay_area` in `act` flags.
- [ ] Ensure boss mobs have `sentinel` + `boss` + `no_flee` and are placed only in `no_mob` rooms.
- [ ] Ensure strong non-boss mobs have `solo` flag.
- [ ] Ensure undead mobs have `undead` flag.
- [ ] Ensure all `long_descr` are exactly one text line + newline + `~`-only line.
- [ ] Ensure all `description` end with exactly one trailing newline before `~`.
- [ ] Assign `!`/`|`/`+` extension lines for combat-capable mobs with appropriate skill profiles (melee/hybrid/caster per faction).
- [ ] Set `l`/`L` loot tables on boss mobs with valid object vnums; loot items have `ITEM_LOOT`.
- [ ] Assign elemental affinities via `|` extension: undead mobs `suscept` includes `holy`; construct mobs `resist` includes `fire`.

### Objects
- [ ] Write all 86 object templates (vnums 30480-30565) in ascending sequential order with no gaps.
- [ ] Ensure unique `<name>~` for every object in the area.
- [ ] Ensure all objects include `ITEM_TAKE` in `wear_flags`.
- [ ] Ensure no objects include `ITEM_WEAR_CLAN_COLORS`.
- [ ] Ensure no objects set `ITEM_GENERATED`.
- [ ] Ensure boss-drop items include `ITEM_BOSS` flag.
- [ ] Ensure loot-table items include `ITEM_LOOT` flag.
- [ ] Assign correct weight ranges (1-5 caster, 6-10 melee, 11-15 tank).
- [ ] Ensure weapon `value3` is thematically consistent; no `value3=0` without `ITEM_FIST`.
- [ ] Ensure two-handed weapons have `ITEM_TWO_HANDED` flag.
- [ ] Create all six key objects (Sun-Lion Gate Key, Priest-King Descent Seal, Tribunal Gate Seal, Royal Aperture Key, Black Sun Cradle Seal, Apex Summit Key).
- [ ] Ensure object `name`, `short_descr`, and `description` are thematically consistent with wear flags.

### Resets
- [ ] Write `M` resets for all planned mob placements with appropriate limits.
- [ ] Write `G`/`E` resets for mob equipment.
- [ ] Write `O` resets for atmospheric/lore objects.
- [ ] Write `D` resets for all locked/closed doors with appropriate states.
- [ ] Verify every locked door has `EX_ISDOOR` in room exit `<locks>` and valid `<key_vnum>`.
- [ ] Verify every locked door (`D` state `2`) has corresponding key object in `#OBJECTS`.
- [ ] Verify `#RESETS` section contains no blank lines.

### Specials
- [ ] Assign `spec_cast_cleric` to High Priest Amenhotep.
- [ ] Assign `spec_cast_mage` to Vizier Khamsin, High Sorceress Neferu, and The Tomb Architect.
- [ ] Assign `spec_cast_bigtime` to Avatar of Ra and Apep the Serpent God.
- [ ] Assign `spec_cast_undead` to Pharaoh Ramesses the Eternal.
- [ ] Assign `spec_poison` to serpent-line mobs.

### Color Theme
- [ ] Apply color theme consistently across all room, mob, and object authored strings.
- [ ] Verify no `@@k` thematic usage, no background codes (`@@0`-`@@7`), no flashing (`@@f`).
- [ ] Verify `@@N` reset after all colorized strings.

### Final Validation
- [ ] Validate complete area file format against `docs/area_file_spec.md`.
- [ ] Verify all vnums within `V 30480 30684` envelope.
- [ ] Verify `#ROOMS` and `#MOBILES` and `#OBJECTS` each terminate with `#0`.
- [ ] Verify `#RESETS`, `#SPECIALS` each terminate with `S`.
- [ ] Verify canonical section order.
- [ ] Run `make unit-tests` from `src/` to validate.

