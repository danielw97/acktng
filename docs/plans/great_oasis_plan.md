# Area Plan: The Great Oasis (Full Redesign)

## Overview

**Area Name:** `@@a The @@yGreat @@lOasis@@N`
**File Name:** `the_great_oasis.are`
**Keyword:** `the great oasis`
**Level Range:** 40-60 (`I 40 60`)
**Vnum Range (fixed):** **8600-8699** (`V 8600 8699`)
**Room Count Target:** 100/100 vnums used
**Design Goal:** Replace the current corridor-heavy spiral desert with a coherent oasis destination area: contested approaches through reaver-held dunes, a living oasis basin with neutral Spring Warden community, ruined Toll Pavilion occupied by Wellmouth Creed cultists, an underground cistern complex preserving deep history, and a flooded aquifer boss lair that poses a moral dilemma.

This plan keeps the existing vnum envelope exactly as assigned and fully reworks room flow, encounter identity, objectization, and narrative progression.

---

## Color Theme (Multi-Color, Lore-Driven)

- **Oasis-cyan (`@@a`)**: living water, spring flow, basin reflections, warden identity, cool shade.
- **Sun-gold (`@@y`)**: desert heat, sand glare, Ledger Kingdom relics, administrative grandeur, dried surfaces.
- **Spring-blue (`@@l`)**: deep aquifer, cistern water, Keeper engineering, wyrm presence, cold breath.
- **Earth-brown (`@@b`)**: palm bark, dried mud, cracked clay, basin margins, natural decay.
- **Cult-purple (`@@p`)**: Synod ritual, Wellmouth Creed rites, blood libation stains, censers, liturgical text.
- **Threat-red (`@@R`)**: reaver aggression, boss foreshadowing, blood, mortal danger.
- **Reset (`@@N`)** after all colorized strings.

Theme rule compliance for implementation:
- No `@@k` thematic usage.
- No background codes (`@@0`-`@@7`) and no flashing (`@@f`).

---

## `#AREA` Header Plan (spec-compliant)

All directives per `area_file_spec.md` section 3:

- `Q 16` — mandatory area version; no other value is valid
- `K the great oasis~` — keyword, no color codes, `~`-terminated
- `L @@a{@@y40 60@@a}@@N~` — cyan braces, yellow level numbers, cyan close brace, reset, `~`-terminated
- `N 101` — area number (preserved from current file)
- `I 40 60` — min/max level; controls mob-level gating and area matching
- `V 8600 8699` — full 100-vnum envelope; all rooms, mobs, and objects must stay within this range
- `F 12` — reset frequency in minutes (preserved from current file)
- `U @@aA cool rumor of water rides the @@ybright-gold@@a desert wind toward the Great Oasis.@@N~` — reset message; no double-newlines, `~`-terminated
- `O Virant~` — area owner must always be Virant (owner policy)
- `T` — teleport flag line; presence enables teleport into the area
- `R`, `W` — ACL directives set at implementation time by the builder

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

## Existing Lore Anchors to Preserve and Elevate

Current area identity implies:
1. A spiraling hunt-road approach through desert dunes toward a hidden water source.
2. Waypoint markers, cairns, and survey stones at regular intervals.
3. Blue/yellow/brown color identity linking sky, sun, and earth.
4. Jackal tracks, serpent-eye motifs, and caravan sigils on route markers.
5. Cistern ruins, trade jars, and fossil shells as environmental debris.
6. Two bosses: a Basin Tyrant and a Keeper of the Hidden Spring.

The redesign treats these not as disconnected motifs but as evidence of one historical arc:

- The Great Oasis was the **administrative heart** of the Keeper-of-Measure road network — the calibration node where all desert routes converged on the only reliable year-round water source.
- The Ledger Kingdoms built the **Toll Pavilion** and **Cistern Ring** here, transforming the oasis from a natural spring into a hydraulic engineering marvel and the seat of the **High Water Court**.
- The Ninth Meridian Rite's failure (at the Eastern Desert's Buried Court) caused spring reduction, cistern collapse, and the **Battle of the Dry Basin** that destroyed unified oasis governance.
- Three successor factions now contest the oasis: the **Spring Wardens** (pragmatic water cooperative), the **Jackal Synod's Wellmouth Creed** (militant cult occupying the Pavilion ruins), and the **Dune Reavers** (privatized charter militia controlling the approaches).
- Beneath the oasis, the flooded east cistern harbors the **Oasis Wyrm** — an aquifer predator whose presence threatens the archives but accidentally sustains the springs.

See `great_oasis_lore.md` for the complete lore bible.

---

## Cross-Area Lore Integration

### Connection to the Eastern Desert (vnums 8500-8599, levels 30-100)

The Great Oasis is the Eastern Desert's geographical destination. The old caravan road that forms the Eastern Desert's Zone A spine terminates at the oasis's western approach. The two areas share a continuous historical narrative: the Eastern Desert documents the road network's collapse, while the Great Oasis documents what happened at the network's most important node.

Specific connections:
- Charter of Passage inscriptions on oasis waystones are the destination-side counterparts of the Eastern Desert's Zone A charter stones. Together they form a complete legal circuit.
- The Jackal Synod's oasis chapter (Wellmouth Creed) is a doctrinal cousin of the three Eastern Desert Synod factions. Oasis Synod NPCs reference "the desert brothers" with kinship and contempt.
- Dune Reavers at the oasis are organizationally linked to the Eastern Desert's Reaver Confederates but better supplied and more cautious.

### Connection to the Southern Oasis (vnums 9800-9999)

The southern exit leads toward the Southern Oasis. The two oases were historically linked by the **Twin Basin Compact** — a mutual-aid water agreement that failed during the post-Ninth Meridian drought. The southern approach wadi shows evidence of compact infrastructure: relay stations, water-jar caches, and signal towers.

### Connection to the Northern Pyramid (vnums 10125-10324, levels 100-120)

The northern exits (currently connecting to vnums 8880-8888, reserved for future construction) point toward territory between the oasis and the Northern Pyramid. The oasis's Measuring House astronomical records contain the earliest known reference to the Northern Pyramid site. The Sand Sovereign used oasis-trained engineers for the pyramid's construction.

### Connection to Midgaard (vnums 3001-3200)

Midgaard's archives contain the **Oasis Folios** — copied water-court documents and astronomical tables purchased from oasis refugees. Midgaard scholars occasionally visit the oasis to compare copies against surviving inscriptions.

### Connection to Kowloon (vnums 14000-14099)

Kowloon's Jade Clerks' water-debt accounting methodology is directly borrowed from the oasis Water Court's ledger system. Kowloon traders who visit the oasis recognize the old ledger tablets with professional interest.

### Connection to Kiess (vnums 13000-13099)

The Temple of Concord in Kiess preserves a version of the Charter of Passage prayer adapted into a civic benediction — evidence of oasis legal-religious traditions diffusing westward through the trade network.

---

## Narrative Structure (Act-Based)

### Act I - The Contested Approaches (8600-8619)
Players arrive through reaver-held approach dunes, encountering privatized violence in the shell of old civic infrastructure. Charter stones, ambush gullies, and toll barricades establish the factional landscape. The distant shimmer of palm crowns on the horizon draws players forward.

### Act II - The Living Oasis (8620-8639)
Players enter the palm ring and discover the oasis basin — a living community, not a ruin. The Spring Wardens provide neutral anchor, the Last Reed Tender delivers deep lore, and the first signs of Synod encroachment become visible. Water, shade, and relative safety create a mid-area rest point.

### Act III - The Toll Pavilion and Synod Quarter (8640-8659)
Players enter the ruined Toll Pavilion, now occupied by the Wellmouth Creed. Administrative grandeur overlaid with cult appropriation. Escalating Synod hostility, ritual chambers, and the Hierophant's sanctum create the area's primary factional combat zone.

### Act IV - The Cistern Ring and Deep History (8660-8679)
Players descend into the underground cistern complex. Engineering marvels, historical inscriptions, and structural decay tell the story of the oasis's managed decline. Ancient dangers (guardians, traps, unstable chambers) replace surface faction threats.

### Act V - The Flooded East Cistern and Wyrm's Lair (8680-8699)
Players enter the flooded depths where the Oasis Wyrm has taken up residence. The dilemma is communicated: the wyrm threatens the archives but sustains the springs. The final boss fight is a choice with consequences, not merely a difficulty spike.

---

## Room Layout Redesign (All 100 Rooms)

> All room names/descriptions are fully replaced. Layout changes from a linear spiral into a hub-and-spoke overworld with underground delves and branching paths.

### Room Description Requirements (spec compliance)

Per `area_file_spec.md` section 8:

- Every room `<description>~` must contain at least 3 sentences of text.
- Important rooms (zone entry points, boss rooms, major hub rooms, lore-critical rooms) must contain at least 5 sentences.
- Every room description must be unique to that room (no reused descriptions across rooms).
- Vnums must never appear in any in-world description text.
- Named exits (`<exit_keyword>`) must be discoverable in the room's `<description>~`, an object that spawns in the room, or an `E` extra description in the room.
- Extra-description keyword chains must be anchored: at least one keyword in each chain must appear in the room's main `<description>~`.
- Room descriptions must use the area's color theme intentionally and consistently.
- Each room's `<description>~` must end with exactly one trailing newline before `~`.

### Room Flags and Sector Types

Per `area_file_spec.md` sections 8.1 and 8.2:

- **Outdoor desert rooms** (approach dunes, reaver camps, waystone circle): `sector_type = desert` (`10`)
- **Outdoor oasis rooms** (palm ring, basin margin, garden terraces): `sector_type = forest` (`3`) — the palm canopy and vegetation justify non-desert classification
- **Water rooms** (basin pool, flooded cistern passages): `sector_type = water_swim` (`6`)
- **Interior rooms** (cistern vaults, pavilion interiors, Synod chambers, wyrm's lair): `sector_type = inside` (`11`), `room_flags` includes `indoors` (`8`)
- **Boss rooms** (8659, 8679, 8699): `room_flags` includes `no_mob` (`4`) per boss placement policy
- **Flooded cistern rooms** (Zone E, 8680-8699): `room_flags` includes `no_teleport` (`16384`) and `no_recall` (`8192`), except entry room 8680 and exit room 8699 which allow recall for escape
- **No rooms use `ROOM_MAZE`** (`524288`); this area does not contain mazes

### Vnum Allocation Policy

Per `area_file_spec.md` section 13.1:

- Room vnums must be assigned in ascending sequential order with no gaps.
- All 100 room vnums (8600-8699) must be used; fill every available room-vnum slot.

### Zone A: Approach Dunes and Reaver Territory (8600-8619, 20 rooms)
- Western and southern approach routes through reaver-controlled desert.
- Sector: `desert` throughout.
- Includes toll barricades, ambush gullies, charter stone ruins, reaver camps, and the first distant views of the palm ring.
- Key rooms:
 - **8600** Western Gate — entry from the Eastern Desert (vnum 8519 west). Important room, 5+ sentence description. First view of the distant oasis shimmer. Preserves external connection.
 - **8601-8609** Northern approach exits — connect north to vnums 8880-8888 (future construction). Preserves all nine northern external connections from the current area (with 8609 as both connector and bend waypoint).
 - **8609** Southeastern bend — preserved from current layout as a structural waypoint linking the northern connectors to the southern approach.
 - **8610** Reaver Toll Barricade — first hostile chokepoint. Important room, 5+ sentence description. Salvaged timber walls, seized trade goods, armed sentries.
 - **8614** Burned Convoy Basin — evidence of recent reaver violence. Overturned wagons, scattered cargo, bloodstained sand.
 - **8618** Reave-Captain's Yard — the reaver leader's fortified camp in the old caravan staging area. Important room, 5+ sentence description. The Reave-Captain's command post.
 - **8619** Palm Ring Threshold — the transition from harsh desert to sheltered oasis. Important room, 5+ sentence description. Temperature drops, wind dies, palm fronds arch overhead.

### Zone B: The Living Oasis (8620-8639, 20 rooms)
- The oasis proper: palm ring, basin margin, Spring Warden settlement, garden terraces.
- Mix of `forest` (palm canopy rooms) and `water_swim` (basin edge rooms) sectors.
- Includes the neutral warden community, Last Reed Tender's garden, waystone circle fragments, and the first views of the Toll Pavilion ruins.
- Key rooms:
 - **8620** North Cistern Wellhead — Spring Warden water distribution point. Important room, 5+ sentence description. The rationing queue, the clerk's ledger table, the functional sluice.
 - **8625** Central Basin Shore — the heart of the oasis. Important room, 5+ sentence description with `E` extra descriptions for the basin, the water, and the distant ruins. `sector_type = water_swim`.
 - **8628** The Last Garden — the Reed Tender's surviving terrace. Important room, 5+ sentence description. Date palms, medicinal herbs, flowers. The last cultivated garden in the eastern desert.
 - **8630** Western Waystone — one of the twelve charter stones, pointing toward the Eastern Desert road. `E` extra description for the inscription.
 - **8632** Southwestern Waystone — pointing toward the Southern Oasis. `E` extra description referencing the Twin Basin Compact.
 - **8635** Warden Watch Tower — the militia's observation post overlooking the basin. Important room, 5+ sentence description.
 - **8639** Pavilion Approach — the boundary between warden territory and Synod-held ruins. Important room, 5+ sentence description. Visible Synod modifications begin: hanging censers, painted symbols.

### Zone C: Toll Pavilion Ruins and Synod Quarter (8640-8659, 20 rooms)
- The ruined Toll Pavilion, now the Wellmouth Creed's stronghold.
- Sector: `inside` throughout (roofed colonnade and pavilion interiors).
- `room_flags` includes `indoors` (`8`).
- Escalating Synod hostility. Ritual chambers, chanting halls, the Hierophant's sanctum.
- Key rooms:
 - **8640** Shattered Colonnade Entry — the pavilion's former grand entrance. Important room, 5+ sentence description. Broken columns, hanging censers, painted jackal sigils. Named exit `^colonnade~` discoverable in description.
 - **8644** The Broken Dais — where the High Water Judge once presided. Important room, 5+ sentence description with `E` extra descriptions for the cracked judicial platform and the crude altar built atop it.
 - **8648** Basin Chanting Hall — where Synod chanters maintain the continuous liturgy. Important room, 5+ sentence description. The sound of rhythmic chanting fills the space.
 - **8652** Salt Oath Chamber — where Wellmouth Priests perform consecrated salt rituals. Important room, 5+ sentence description with `E` extra descriptions for the salt circles and ritual basins.
 - **8656** Blood Libation Pool — where offerings are poured into the spring channels. Important room, 5+ sentence description. Blood-stain patterns on limestone, the metallic smell of old offerings.
 - **8659** Wellmouth Hierophant's Sanctum — mini-boss room. `room_flags` includes `no_mob`. Important room, 5+ sentence description. The cult leader's private chamber, decorated with spring-water relics and dream-interpretation charts.

### Zone D: Cistern Ring and Deep History (8660-8679, 20 rooms)
- The underground cistern complex beneath the oasis.
- Sector: `inside` throughout.
- `room_flags` includes `indoors` (`8`).
- Engineering marvels, historical inscriptions, structural decay, ancient guardians.
- Key rooms:
 - **8660** North Cistern Descent — stairway down from the warden wellhead. Important room, 5+ sentence description. Fitted limestone walls, sluice channels, water-level gauges.
 - **8664** Sluice Control Chamber — the old hydraulic regulation center. Important room, 5+ sentence description with `E` extra descriptions for the sluice mechanisms, calibration marks, and engineering inscriptions.
 - **8668** The Ledger Vault — archive room containing surviving salt tablets and charter documents. Important room, 5+ sentence description with multiple `E` extra descriptions for individual tablets and ledger entries.
 - **8672** Collapsed South Cistern — the war-damaged chamber. Rubble, cracked vaults, evidence of the Battle of the Dry Basin. Structural instability.
 - **8675** West Cistern (Dry) — the old agricultural irrigation reservoir, now bone-dry. Inscription on the wall records the last day water flowed here.
 - **8678** East Cistern Access — the threshold to the flooded depths. Important room, 5+ sentence description. Water laps at the stone floor. The temperature drops sharply. Something moves below.
 - **8679** Cistern Guardian Chamber — mini-boss room. `room_flags` includes `no_mob`. Important room, 5+ sentence description. An ancient construct or preserved sentinel still executing centuries-old guard protocols.

### Zone E: Flooded East Cistern and Wyrm's Lair (8680-8699, 20 rooms)
- The flooded cistern depths where the Oasis Wyrm has taken residence.
- Sector: `water_swim` for flooded passages, `inside` for partially-drained chambers.
- `room_flags` includes `indoors` (`8`), `no_teleport` (`16384`), `no_recall` (`8192`).
- Exception: entry room 8680 and boss room 8699 allow recall (omit `no_recall`).
- Key rooms:
 - **8680** Flooded Stairwell — zone entry, partially submerged. Important room, 5+ sentence description. Exempt from `no_recall`. Water chest-deep and cold. Keeper inscriptions visible above the waterline.
 - **8684** Submerged Archive — the drowned record hall. Important room, 5+ sentence description with `E` extra descriptions for waterlogged tablets and ruined scrolls. The lore the wyrm guards.
 - **8688** Aquifer Channel — a natural waterway the wyrm uses to access the deep aquifer. Important room, 5+ sentence description. Current pulls southward. Mineral deposits on the walls glow faintly.
 - **8692** Wyrm's Antechamber — final pre-boss staging room. Important room, 5+ sentence description. Shed scales, mineral-encrusted bones, the temperature plunges. The water here is unnaturally still.
 - **8694** Spring Vent Chamber — a side room where one of the surviving springs rises. `E` extra description explaining the wyrm's accidental role in keeping the vent clear. Lore object placement.
 - **8698** The Deep Pool — the largest flooded chamber. Important room, 5+ sentence description. The wyrm's territory marker: the water is clear enough to see the bottom forty feet below.
 - **8699** Lair of the Oasis Wyrm — final boss room. `room_flags` includes `no_mob`. Exempt from `no_recall`. Important room, 5+ sentence description. The wyrm's nest: mineral-encrusted walls, aquifer inflow channel, the massive creature coiled around the deepest spring vent.

### Connectivity Principles

Per `area_file_spec.md` section 8 directional traversal constraints:

- **All exits must be bi-directional**: if room A connects to room B through direction X, room B must provide the opposite-direction exit back to room A. No one-way exits (this area is not a maze).
- **No directional loops**: repeated movement in the same direction must not return to a previously visited room. The spec prohibits room connection loops (e.g., `a -> b -> c -> a`) unless all rooms in the set are flagged `ROOM_MAZE`.
- **Branching paths replace loops**: the plan uses hub-and-spoke branching from 4 major hub rooms (Reaver Toll Barricade 8610, Central Basin Shore 8625, Shattered Colonnade Entry 8640, North Cistern Descent 8660) rather than circular loops. Players choose branches and backtrack through hubs.
- **Multiple traversal options between macro-zones**: at least 2 branching paths connect each pair of adjacent zones, providing route variety without circular loops.
- **External exits preserved** at all current connection points:
 - Room 8600 west to vnum 8519 (Eastern Desert)
 - Rooms 8601-8609 north to vnums 8880-8888 (future construction). This preserves the full existing northern connector count while keeping 8600 focused on the primary western entry role.
 - Room 8699 south to vnum 9800 (Southern Oasis)
- **Door usage** limited to the Hierophant's sanctum and the east cistern access. Every door exit must have `EX_ISDOOR` (`1`) set in `<locks>`. Door initial state authored through `#RESETS` `D` command. Any locked door (`D` state `2`) must have `<key_vnum>` set to a valid key object vnum.
- **Named exits** use `^` prefix on keywords (e.g., `^colonnade~`, `^cistern gate~`) so movement messaging treats them as standalone noun phrases. Each named exit keyword must appear in the room's `<description>~`, an object in the room, or an `E` extra description.

### External Connection Map

```
          [8880] [8881] [8882] [8883] [8884] [8885] [8886] [8887] [8888]
           |   |   |   |   |   |   |   |   |
          [8601] [8602] [8603] [8604] [8605] [8606] [8607] [8608] [8609]
           \   \   \   |   /   /   /   /
[8519] ---W--- [8600] -------- ZONE A: APPROACH DUNES -------- [8610] Toll Barricade
 (Eastern               |               |
  Desert)           [8619] Palm Ring Threshold   Reaver Camps
                    |
                ZONE B: LIVING OASIS
             [8620]-[8625 Basin]-[8639 Pavilion Approach]
                 |          |
             [8660] Cistern    ZONE C: TOLL PAVILION
             Descent       [8640]-[8659 Hierophant]
                |
            ZONE D: CISTERN RING
             [8660]-[8678]-[8679 Guardian]
                 |
            ZONE E: FLOODED DEPTHS
             [8680]-[8699 Wyrm Lair]
                     |
                    [9800]
                  (Southern Oasis)
```

---

## Mob Redesign Plan

### Mob Vnum Strategy

Per `area_file_spec.md` section 13.1:

- Mob vnums must be assigned in ascending sequential order with no gaps, starting from 8600.
- Fully rewrite existing vnums **8600-8611**.
- Expand roster through contiguous sequential slots **8612-8631** (staying within the 8600-8699 vnum envelope).
- Target composition: ~32 mob templates (vnums 8600-8631).

### Mobile Spec Compliance (all mobs)

Per `area_file_spec.md` sections 4 and 5.4:

- **All mobs** must have `act` flags including `is_npc` (`1`) and `stay_area` (`64`).
- **Sentinel mobs** (guards, shopkeepers, stationary NPCs): add `sentinel` (`2`) to `act` flags.
- **Boss mobs** (Wellmouth Hierophant, Cistern Guardian, Oasis Wyrm): must have `act` flags including `sentinel` (`2`) and `boss` (`67108864`), and must be placed only in rooms flagged `no_mob` (`4`).
- **Strong non-boss mobs** (Reave-Captain, veteran reavers, apex predators): must have `act` flags including `solo` (`33554432`).
- **Aggressive mobs** (reavers, Synod combatants, feral beasts, cistern guardians): set `aggressive` (`32`) in `act` flags.
- **`invasion`** (`536870912`) must never be set in area files; it is runtime-only.
- Mobile `long_descr` must be exactly one text line followed by a newline and a `~`-only line.
- Mobile `description` must end with exactly one trailing newline before the terminating `~`.
- Mobile `player_name` (keyword), `short_descr`, `long_descr`, and `description` are all `~`-terminated.

### Mobile Extension Lines

Per `area_file_spec.md` sections 4.1-5.5:

- `!` extension: `<class> <clan> <race> <position> <skills> <cast> <def>` — used for combat-capable mobs.
- `|` extension: `<strong_magic> <weak_magic> <race_mods> <power_skills> <power_cast> <resist> <suscept>` — used for elemental affinities. Oasis mobs should generally have `resist` including `water` (`64`) for aquatic creatures or `fire` (`128`) for desert creatures. The wyrm should resist `water` and be susceptible to `fire`.
- `+` extension: combat modifier tuning for significant encounters.
- `l`/`L` extensions: loot tables. `loot_amount` is percent-based. Items in loot tables must have `ITEM_LOOT` set in `extra_flags`.

### Factions and Ecology

1. **Spring Wardens** (neutral NPC ecosystem)
  - Warden of the Last Spring, Water Clerk, Watch Captain, Basin Keeper, warden militia.
  - `act`: `is_npc` + `stay_area` + `sentinel` (for stationary NPCs). Non-aggressive.
  - Level range: 40-50.
  - Archetype: hybrid profile (light arms, some defensive casting).
  - Mob vnums: **8600-8604** (5 templates).

2. **Lore NPCs** (neutral, non-combat)
  - Last Reed Tender, Midgaard Scholar (visiting).
  - `act`: `is_npc` + `stay_area` + `sentinel`. Non-aggressive.
  - Level range: 40-45.
  - No combat extensions needed (flavor NPCs).
  - Mob vnums: **8605-8606** (2 templates).

3. **Dune Reavers (Oasis Band)** (hostile opportunists)
  - Reaver sentries, toll enforcers, beast-handlers, the Reave-Captain of the Yard.
  - `act`: `is_npc` + `stay_area` + `aggressive`. Sentries add `sentinel`. Beast-handlers add `no_flee` (`16`).
  - Level range: 40-55 (sentries low, Reave-Captain high).
  - Archetype: melee profile. Beast-handlers use hybrid profile.
  - The Reave-Captain: `act` adds `solo` (`33554432`) + `no_flee` (`16`). Level 55.
  - Mob vnums: **8607-8612** (6 templates).

4. **Oasis Fauna** (hostile predators)
  - Glass scorpions, basin vipers, sand jackals, dune stalkers.
  - `act`: `is_npc` + `stay_area` + `aggressive` + `no_mind` (`262144`).
  - Level range: 40-50.
  - Archetype: melee profile. Basin vipers add poison spec (`spec_poison` in `#SPECIALS`).
  - Mob vnums: **8613-8616** (4 templates).

5. **Jackal Synod — Wellmouth Creed** (hostile cult faction)
  - Spring Acolytes, Basin Chanters, Wellmouth Priests, the Wellmouth Hierophant.
  - `act`: `is_npc` + `stay_area` + `aggressive`. Acolytes add `sentinel` (stationed at ritual nodes).
  - Level range: 45-60 (acolytes low, Hierophant boss-level).
  - Archetype: hybrid profile (ritual casting + melee). Basin Chanters use caster profile.
  - `affected_by`: Wellmouth Priests get `detect_hidden` (`32`); Hierophant gets `detect_magic` (`16`) + `detect_hidden` (`32`).
  - Mob vnums: **8617-8622** (6 templates including Hierophant).

6. **Cistern Guardians** (ancient construct/preserved defenders)
  - Oathbound Cistern Sentinels, Sluice Wardens, Archive Keepers.
  - `act`: `is_npc` + `stay_area` + `aggressive` + `sentinel`.
  - Level range: 50-60.
  - Archetype: melee profile (sentinels), hybrid profile (Archive Keepers with defensive casting).
  - `affected_by`: `infrared` (`512`).
  - `resist`: `physical` (`2`). `suscept`: `water` (`64`).
  - Mob vnums: **8623-8627** (5 templates).

7. **Flooded Depths Fauna** (aquifer creatures)
  - Aquifer eels, mineral crabs, blind cave fish swarms.
  - `act`: `is_npc` + `stay_area` + `aggressive` + `no_mind` (`262144`).
  - Level range: 48-58.
  - Archetype: melee profile.
  - `resist`: `water` (`64`). `suscept`: `fire` (`128`).
  - Mob vnums: **8628-8630** (3 templates).

8. **The Oasis Wyrm** (final boss)
  - Mob vnum: **8631**.
  - Details in Boss Ladder section below.

### Boss Ladder

All boss mobs: `act` includes `is_npc` + `stay_area` + `sentinel` + `boss` (`67108864`) + `aggressive` + `no_flee`. Placed only in rooms flagged `no_mob`.

- **Mini-boss 1 (8659, Zone C):** The Wellmouth Hierophant. Mob vnum **8622**. Level 55. Hybrid profile with offensive ritual casting. Assigned `spec_cast_cleric` in `#SPECIALS`. `act` adds `remember` (`8`). `affected_by`: `detect_hidden` (`32`) + `detect_magic` (`16`). Items dropped must include `ITEM_BOSS` flag.

- **Mini-boss 2 (8679, Zone D):** The Cistern Guardian Eternal. Mob vnum **8627**. Level 58. Melee profile with heavy defense. Assigned `spec_cast_cleric` in `#SPECIALS`. `affected_by`: `infrared` (`512`) + `detect_invis` (`8`). Items dropped must include `ITEM_BOSS` flag.

- **Final Boss (8699, Zone E):** The Oasis Wyrm. Mob vnum **8631**. Level 60. Melee profile with strong area attacks. `act` adds `remember` (`8`). `affected_by`: `infrared` (`512`) + `detect_invis` (`8`). `resist`: `water` (`64`) + `physical` (`2`). `suscept`: `fire` (`128`). Items dropped must include `ITEM_BOSS` flag. Loot table (`l`/`L`) used for boss drops.

### Rewrite of Current Named Mobs

- Blue-Sashed Oasis Scout -> **Spring Warden Militia** (neutral patrol; non-aggressive).
- Yellow-Scarf Dune Skirmisher -> **Reaver Toll Enforcer** (hostile; melee profile).
- Brown-Cloaked Sand Stalker -> **Dune Stalker** (hostile predator; melee profile).
- Parched Caravan Deserter -> **Reaver Sentry** (hostile; melee profile, sentinel).
- Mirage Adept of the Basin -> **Basin Chanter** (hostile Synod mid-rank; caster profile).
- Sunwell Guardian -> **Oathbound Cistern Sentinel** (hostile ancient guardian; melee profile).
- Blue-Eyed Jackal Raider -> **Wellmouth Priest** (hostile Synod senior; hybrid profile).
- Dust Prophet of Thirst -> **Spring Acolyte** (hostile Synod low-rank; hybrid profile).
- Reedknife Oasis Hunter -> **Basin Viper** (hostile predator; melee + poison).
- Salt-Crust Marauder -> **Reaver Beast-Handler** (hostile; hybrid profile).
- The Basin Tyrant -> **The Wellmouth Hierophant** (mini-boss; hybrid profile with ritual casting).
- Keeper of the Hidden Spring -> **The Oasis Wyrm** (final boss; melee profile with area attacks).

---

## Object Redesign Plan

### Object Vnum Strategy

Per `area_file_spec.md` section 13.1:

- Object vnums must be assigned in ascending sequential order with no gaps, starting from 8600.
- Fully replace current object concepts beginning at **8600**.
- Expand through contiguous sequential slots (suggested through **8649**) while staying within the 8600-8699 vnum envelope.
- Target composition: ~50 object templates (vnums 8600-8649).
- Within the area, object `<name>~` values must be unique (no duplicate item names in the same area file).

### Object Spec Compliance (all objects)

Per `area_file_spec.md` section 7:

- **Every object must include `ITEM_TAKE`** (`8388608`) in `wear_flags`. No exceptions.
- **No object may include `ITEM_WEAR_CLAN_COLORS`** (`16777216`) in `wear_flags`.
- Object `name`, `short_descr`, and `description` must be thematically consistent with the object's non-`take` wear flags.
- **Area-authored items must not define fixed stats** in the area file. Item stats are generated at runtime.
- **`ITEM_GENERATED`** (`1`) must not be set in area-authored objects.
- **`ITEM_LOOT`** (`67108864`): items spawned from a mobile's loot table (`l`/`L` extension) must have this set.
- **`ITEM_BOSS`** (`134217728`): any item that can drop from a boss mob must include this flag.
- Object extra-description `<description>~` must end with exactly one trailing newline before `~`.

### Weight Ranges (archetype encoding)

Per `area_file_spec.md` section 7:

- `1`-`5`: caster item
- `6`-`10`: melee item
- `11`-`15`: tank item

### Weapon Constraints

Per `area_file_spec.md` sections 7.3 and 7.6:

- Weapons must have `item_type = ITEM_WEAPON` (`5`) and `wear_flags` including both `hold` (`32768`) and `take` (`8388608`).
- `value3` (attack type) must be thematically consistent with the weapon's concept.
- `value3 = 0` (`hit`) must not be used unless the object also has `ITEM_FIST` extra flag.
- **Two-handed weapons**: must include `ITEM_TWO_HANDED` (`2147483648`) in `extra_flags`.
- **Bucklers**: `item_type = ITEM_ARMOR`, `wear_flags` includes `hold`, `extra_flags` includes `ITEM_BUCKLER` (`268435456`).

### Item Families

1. **Reaver Salvage Set** (levels 40-50, martial desert gear)
  - Salvaged toll-blades (`hold` + `ITEM_WEAPON`, `value3 = 3` slash, weight 6-10 melee), sand cloaks (`about`, weight 11-15 tank), reaver bucklers (`hold` + `ITEM_BUCKLER`, weight 11-15 tank), dune boots (`feet`, weight 6-10 melee), toll-chain hauberks (`body`, weight 11-15 tank).
  - `L` level lines: 40-50 range.
  - Vnums: **8600-8609** (10 templates).

2. **Spring Warden Issue** (levels 40-48, utility/defensive)
  - Warden wraps (`about`, weight 1-5 caster), cistern lanterns (`hold`, `ITEM_LIGHT`, weight 1-5), basin-clay shields (`hold`, `ITEM_ARMOR`, weight 11-15 tank), warden staffs (`hold` + `ITEM_WEAPON`, `value3 = 7` pound, weight 6-10 melee).
  - `L` level lines: 40-48 range.
  - Vnums: **8610-8615** (6 templates).

3. **Wellmouth Creed Relics** (levels 45-55, cult-themed)
  - Jackal censers (`hold`, `ITEM_TREASURE`, weight 1-5 caster), wellmouth masks (`face`, weight 1-5 caster), anointing blades (`hold` + `ITEM_WEAPON`, `value3 = 1` slice, weight 6-10 melee), salt-oath rings (`finger`, weight 1-5 caster), chanter's robes (`body`, weight 1-5 caster), blood libation bowls (`hold`, `ITEM_TREASURE`, weight 1-5 caster).
  - `L` level lines: 45-55 range.
  - Vnums: **8616-8625** (10 templates).

4. **Cistern Antiquities** (levels 50-58, ancient engineering relics)
  - Calibration rods (`hold` + `ITEM_WEAPON`, `value3 = 7` pound, weight 6-10 melee), sluice-gate levers (`hold` + `ITEM_WEAPON` + `ITEM_TWO_HANDED`, `value3 = 11` pierce, weight 6-10 melee), cistern mail (`body`, weight 11-15 tank), limestone gauntlets (`hands`, weight 11-15 tank), keeper's circlet (`head`, weight 1-5 caster).
  - `L` level lines: 50-58 range.
  - Vnums: **8626-8633** (8 templates).

5. **Wyrm's Hoard / Boss Drops** (levels 55-60, elite)
  - Wyrm-scale hauberk (`body`, weight 11-15 tank), mineral-fang blade (`hold` + `ITEM_WEAPON`, `value3 = 11` pierce, weight 6-10 melee), aquifer-stone ring (`finger`, weight 1-5 caster), wyrm-eye amulet (`neck`, weight 1-5 caster), deep-spring mantle (`about`, weight 1-5 caster), hierophant's eclipse staff (`hold` + `ITEM_WEAPON` + `ITEM_TWO_HANDED`, `value3 = 7` pound, weight 1-5 caster), guardian's eternal plate (`body`, weight 11-15 tank).
  - Boss drops: must include `ITEM_BOSS` in `extra_flags`.
  - Loot-table drops: must include `ITEM_LOOT` in `extra_flags`.
  - `L` level lines: 55-60 range.
  - Vnums: **8634-8643** (10 templates).

6. **Lore Objects and Keys** (various levels)
  - Salt ledger tablet (`ITEM_TREASURE`, `hold`, weight 1-5), charter passage stone (`ITEM_TREASURE`, `hold`, weight 6-10), Keeper's water-level gauge (`ITEM_TREASURE`, `hold`, weight 1-5), Synod sanctum key (`ITEM_KEY`, `hold`, weight 1-5), cistern gate key (`ITEM_KEY`, `hold`, weight 1-5), wyrm lair key (`ITEM_KEY`, `hold`, weight 1-5).
  - Key objects: any key used as `<key_vnum>` for a locked door must exist in `#OBJECTS` with `item_type = ITEM_KEY` (`18`).
  - Vnums: **8644-8649** (6 templates).

### Key Objects for Locked Doors

Per `area_file_spec.md` section 8.3: if any door is reset to locked (`D` state `2`), `<key_vnum>` must be set to a valid key object vnum, and that key object must exist in `#OBJECTS`. Plan the following key objects:

- **Synod Sanctum Key** (vnum 8647, for Zone C Hierophant's sanctum door at room 8658): `ITEM_KEY`, `hold` + `take`, weight 1.
- **Cistern Gate Key** (vnum 8648, for Zone D east cistern access door at room 8678): `ITEM_KEY`, `hold` + `take`, weight 1.
- **Wyrm Lair Key** (vnum 8649, for Zone E deep pool door at room 8697): `ITEM_KEY`, `hold` + `take`, weight 1.

### Mechanical Tone

- Gear progression supports levels 40-60 with meaningful spikes at zone transitions (40-48 Zone A/B, 48-55 Zone C/D, 55-60 Zone E).
- Oasis thematic identity expressed via object names, descriptions, and `item_apply` choices (e.g., `infra` for cistern adaptation, `det_hid` for underwater awareness), not via handcrafted stat values.
- Boss drops reflect aquifer/water duality and visually match the multi-color theme in `short_descr` and `description` color code usage.

---

## `#RESETS` Plan (spec-compliant)

Per `area_file_spec.md` section 12:

- `#RESETS` must not contain blank lines.
- Reset commands reference valid vnums within the area's vnum envelope.

### Mob Resets (`M` command)

Format: `M 0 <mob_vnum> <limit> <room_vnum> <notes>`

- **Zone A (Approach Dunes):** Reset Reaver sentries at toll points (8610, 8612). Reset Reaver toll enforcers at approach rooms (8604, 8606, 8614). Reset Reave-Captain at 8618. Reset dune stalkers at outer dune rooms (8602, 8608). Reset glass scorpions at margin rooms (8605, 8615). Limit 1-2 per mob.
- **Zone B (Living Oasis):** Reset Spring Warden NPCs at community rooms (Warden of the Last Spring at 8620, Water Clerk at 8620, Watch Captain at 8635, Basin Keeper at 8625). Reset Last Reed Tender at 8628. Reset Midgaard Scholar at 8630. Reset sand jackals at palm-ring margin rooms (8622, 8637). Reset basin vipers at water-edge rooms (8624, 8626). Limit 1 per mob.
- **Zone C (Toll Pavilion):** Reset Spring Acolytes at entry rooms (8641, 8643). Reset Basin Chanters at chanting hall (8648). Reset Wellmouth Priests at ritual rooms (8652, 8656). Reset mini-boss Wellmouth Hierophant at 8659 (limit 1).
- **Zone D (Cistern Ring):** Reset Oathbound Cistern Sentinels at guard rooms (8662, 8666, 8674). Reset Sluice Wardens at mechanism rooms (8664, 8670). Reset Archive Keepers at ledger rooms (8668). Reset mini-boss Cistern Guardian Eternal at 8679 (limit 1).
- **Zone E (Flooded Depths):** Reset aquifer eels at flooded passages (8682, 8686, 8690). Reset mineral crabs at side chambers (8684, 8694). Reset blind cave fish swarms at channel rooms (8688, 8696). Reset final boss Oasis Wyrm at 8699 (limit 1).

### Object Resets (`O` command)

Format: `O 0 <obj_vnum> <limit> <room_vnum> <notes>`

- Place lore objects (salt ledger tablet, charter passage stone, Keeper's water-level gauge) at key lore rooms (8630, 8668, 8694) for atmospheric pickup.
- Place key objects on gatekeeper mobs via `G`/`E` resets or in adjacent rooms via `O` resets.

### Equipment Resets (`G`/`E` commands)

Format: `G 0 <obj_vnum> <limit>` (give to previous mob) / `E 0 <obj_vnum> <limit> <wear_loc>` (equip on previous mob)

- Equip Reaver toll enforcers with salvaged toll-blades and sand cloaks.
- Equip Reaver sentries with reaver bucklers and dune boots.
- Equip Reave-Captain with toll-chain hauberk and salvaged toll-blade.
- Equip Wellmouth Priests with anointing blades and wellmouth masks.
- Equip Basin Chanters with chanter's robes and salt-oath rings.
- Equip Wellmouth Hierophant with hierophant's eclipse staff and wellmouth mask (items include `ITEM_BOSS`).
- Equip Cistern Guardian Eternal with guardian's eternal plate and calibration rod (items include `ITEM_BOSS`).
- Equip Oasis Wyrm via loot table for wyrm-scale hauberk, mineral-fang blade, aquifer-stone ring, wyrm-eye amulet (items include `ITEM_BOSS` + `ITEM_LOOT`).

### Door Resets (`D` command)

Format: `D 0 <room_vnum> <door_direction> <state>`

Per `area_file_spec.md` section 8.3: any exit set to closed or locked via `D` reset must have `EX_ISDOOR` (`1`) set in its room `D<door>` `<locks>` definition.

Planned door resets:

1. **Zone C Synod Sanctum (room 8658):** Direction to 8659 (Hierophant's sanctum).
  - Room exit `<locks>`: `door` (`1`). `<key_vnum>`: 8647 (Synod Sanctum Key).
  - `D` reset: `D 0 8658 <dir> 2` (locked).
  - Reverse exit on 8659 must also have `door` bit set.
  - Key object (Synod Sanctum Key, vnum 8647) exists in `#OBJECTS`.

2. **Zone D East Cistern Access (room 8678):** Direction to 8680 (flooded stairwell).
  - Room exit `<locks>`: `door` (`1`). `<key_vnum>`: 8648 (Cistern Gate Key).
  - `D` reset: `D 0 8678 5 2` (locked, direction down).
  - Reverse exit on 8680 must also have `door` bit set.
  - Key object (Cistern Gate Key, vnum 8648) exists in `#OBJECTS`.

3. **Zone E Deep Pool (room 8697):** Direction to 8699 (Wyrm's lair).
  - Room exit `<locks>`: `door` (`1`). `<key_vnum>`: 8649 (Wyrm Lair Key).
  - `D` reset: `D 0 8697 <dir> 2` (locked).
  - Reverse exit on 8699 must also have `door` bit set.
  - Key object (Wyrm Lair Key, vnum 8649) exists in `#OBJECTS`.

All three doors reset to **locked** (state `2`). Keys can be placed on gatekeeper mobs via `G` resets or in adjacent rooms via `O` resets:
- Synod Sanctum Key: dropped by or equipped on a Wellmouth Priest in a Zone C room.
- Cistern Gate Key: found in the Ledger Vault (8668) or on an Archive Keeper mob.
- Wyrm Lair Key: found in the Spring Vent Chamber (8694) or on a mineral crab mob.

### Spawn Philosophy

- Keep the living oasis (Zone B) populated with neutral wardens and flavor NPCs so it always feels inhabited.
- Concentrate Synod hostiles at pavilion chokepoints (Zone C) rather than random saturation. Use limit 1 for elite mobs.
- Use higher mob density in flooded rooms (Zone E) to create traversal urgency and aquifer ecology atmosphere.
- Gate boss encounters with locked doors requiring keys, providing visible progression gating.

---

## `#SPECIALS` Plan

Per `area_file_spec.md` section 10:

- `spec_poison` assigned to Basin Viper mobs (aquatic ambush predator ecology).
- `spec_cast_cleric` assigned to the Wellmouth Hierophant (mini-boss; ritual offensive casting).
- `spec_cast_cleric` assigned to the Cistern Guardian Eternal (mini-boss; ancient defensive casting).
- `spec_cast_mage` assigned to Wellmouth Priests (offensive arcane casting).
- No `spec_summon_*` specials (runtime-only, builder-prohibited).
- No `spec_keep*` specials (manual-assignment only).

---

## Canonical Section Order

Per `area_file_spec.md` section 14, the final `.are` file should emit sections in this order:

1. `#AREA`
2. `#ROOMS`
3. `#MOBILES`

5. `#OBJECTS`
6. `#SHOPS` (none planned for this area; no merchant NPCs with shop functionality)
7. `#RESETS`
8. `#SPECIALS`
9. `#OBJFUNS` (none planned for this area)
10. `#$` (end-of-file marker)

---

## Implementation Checklist

### Rooms
- [ ] Populate all 100 room vnums (8600-8699) in ascending sequential order with no gaps.
- [ ] Write unique `<description>~` for every room (minimum 3 sentences; 5+ for important rooms).
- [ ] Assign correct `sector_type` (`desert` for approach, `forest` for palm ring, `water_swim` for basin/flooded, `inside` for interiors) and `room_flags` per zone requirements.
- [ ] Set `no_mob` on all boss rooms (8659, 8679, 8699).
- [ ] Set `no_teleport` + `no_recall` on Zone E rooms (except 8680, 8699).
- [ ] Set `indoors` on all interior rooms (Zones C, D, E interior).
- [ ] Ensure all exits are bi-directional with no directional loops.
- [ ] Ensure all named exits use `^` prefix and are discoverable in room description, objects, or extra descriptions.
- [ ] Ensure all extra-description keyword chains are anchored in the room's main `<description>~`.
- [ ] Preserve all required worldgraph external connectors:
 - 8600 west to 8519 (Eastern Desert)
 - 8601-8609 north to 8880-8888 (future construction)
 - 8699 south to 9800 (Southern Oasis)

### Mobiles
- [ ] Write all ~32 mob templates (vnums 8600-8631) in ascending sequential order with no gaps.
- [ ] Ensure all mobs have `is_npc` + `stay_area` in `act` flags.
- [ ] Ensure boss mobs have `sentinel` + `boss` + `no_flee` and are placed only in `no_mob` rooms.
- [ ] Ensure strong non-boss mobs have `solo` flag.
- [ ] Ensure all `long_descr` are exactly one text line + newline + `~`-only line.
- [ ] Ensure all `description` end with exactly one trailing newline before `~`.
- [ ] Assign `!`/`|`/`+` extension lines for combat-capable mobs with appropriate skill profiles.
- [ ] Set `l`/`L` loot tables on boss mobs with valid object vnums; loot items have `ITEM_LOOT`.

### Objects
- [ ] Write all ~50 object templates (vnums 8600-8649) in ascending sequential order with no gaps.
- [ ] Ensure unique `<name>~` for every object in the area.
- [ ] Ensure all objects include `ITEM_TAKE` in `wear_flags`.
- [ ] Ensure no objects include `ITEM_WEAR_CLAN_COLORS`.
- [ ] Ensure no objects set `ITEM_GENERATED`.
- [ ] Ensure boss-drop items include `ITEM_BOSS` flag.
- [ ] Ensure loot-table items include `ITEM_LOOT` flag.
- [ ] Assign correct weight ranges (1-5 caster, 6-10 melee, 11-15 tank).
- [ ] Ensure weapon `value3` is thematically consistent; no `value3=0` without `ITEM_FIST`.
- [ ] Ensure two-handed weapons have `ITEM_TWO_HANDED` flag.
- [ ] Create all three key objects (Synod Sanctum Key 8647, Cistern Gate Key 8648, Wyrm Lair Key 8649).

### Resets
- [ ] Write `M` resets for all planned mob placements with appropriate limits.
- [ ] Write `G`/`E` resets for mob equipment.
- [ ] Write `O` resets for atmospheric/lore objects.
- [ ] Write `D` resets for all three locked doors with state `2`.
- [ ] Verify every locked door has `EX_ISDOOR` in room exit `<locks>` and valid `<key_vnum>`.

### Specials
- [ ] Assign `spec_poison` to Basin Viper mobs.
- [ ] Assign `spec_cast_cleric` to Wellmouth Hierophant.
- [ ] Assign `spec_cast_cleric` to Cistern Guardian Eternal.
- [ ] Assign `spec_cast_mage` to Wellmouth Priests.

### Color Theme
- [ ] Apply color theme consistently across all room, mob, and object authored strings.
- [ ] Verify no `@@k` thematic usage, no background codes (`@@0`-`@@7`), no flashing (`@@f`).
- [ ] Verify `@@N` reset after all colorized strings.

### Final Validation
- [ ] Validate complete area file format against `docs/area_file_spec.md`.
- [ ] Verify all vnums within `V 8600 8699` envelope.
- [ ] Verify `#ROOMS` and `#MOBILES` and `#OBJECTS` each terminate with `#0`.
- [ ] Verify `#RESETS`, `#SPECIALS` each terminate with `S`.
- [ ] Verify canonical section order.
- [ ] Run `make unit-tests` from `src/` to validate.
