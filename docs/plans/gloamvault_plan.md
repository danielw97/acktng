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

### Area Header (`#AREA`)
1. **`Q 16` directive is mandatory** — required area revision/version; no other value is valid.
2. **Owner line must be `O Virant~`** — per area header owner policy.
3. **`V 30100 30174` defines the vnum envelope** — all authored rooms, mobiles, and objects must stay inside this range.
4. **Keep vnum envelope exactly as assigned:** all rooms, mobs, and objects remain in `30100-30174`, with no out-of-range expansion.

### String and Color Rules
5. **All text fields must be `~`-terminated.** Missing `~` causes parse failure.
6. **Strings must not contain back-to-back newlines** (`\n\n`); no blank lines within string fields.
7. **Do not place vnums in in-world descriptions** — this includes room descriptions, mob `long_descr`/`description`, object descriptions, extra descriptions, and exit descriptions.
8. **Use valid ACK color tokens only:** thematic use of `@@d @@p @@m @@a @@W @@N`.
9. **No `@@k` black foreground** authored into area strings.
10. **No background codes (`@@0`-`@@7`) and no flashing (`@@f`)** unless explicitly added by a human author.

### Section Integrity
11. **Follow canonical saver section order:** `#AREA`, `#ROOMS`, `#MOBILES`, `#OBJECTS`, `#SHOPS`, `#RESETS`, `#SPECIALS`, `#OBJFUNS`, `#$`.
12. **`#ROOMS` must exist** and must end with `#0` before the next section header.
13. **`#MOBILES` must terminate with `#0`** before `#OBJECTS`.
14. **`#OBJECTS` must terminate with `#0`** before `#RESETS`.
15. **`#RESETS` must not contain blank lines.**
16. **Optional sections** (`#SHOPS`, `#SPECIALS`, `#OBJFUNS`) should be included as needed.

### Vnum Allocation Policy
17. **Vnums must be assigned in ascending sequential order** — use lower available vnums before higher ones. Do not leave gaps in sequences.
18. **For rooms, attempt to use all room vnums** in the assigned range (fill every available slot where practical).

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

## Room Plan Density and Room Spec Compliance

- **75 total rooms** allocated as: `8 + 8 + 8 + 8 + 8 + 9 + 9 + 9 + 8`.
- All 75 room vnums in the range must be used — fill every available slot (vnum allocation policy).

### Sector Types
- `inside` (`11`) for all rooms — the Gloamvault is entirely subterranean.

### Room Flags
- Most rooms: `indoors` (`8`).
- Boss encounter rooms (Ossuary Champion in Zone F, Dusk Prelate in Zone H, Matriarch Velastra in Zone I): `no_mob` (`4`) + `indoors` (`8`) = `12`.
- Safe lore cells (if any designated respite rooms): `safe` (`1024`) + `indoors` (`8`) = `1032`.
- Dark rooms (unlit crypts, deep descent chambers): `dark` (`1`) + `indoors` (`8`) = `9`.
- No rooms use `ROOM_MAZE` (`524288`) — the Gloamvault uses controlled disorientation, not mechanical maze exits.

### Room Description Requirements
- Every room description must contain **at least 3 sentences** of text.
- Boss rooms, lore anchor rooms, and zone entry rooms are "important rooms" and must contain **at least 5 sentences**.
- Every room description must be **unique** — no reuse of identical descriptions across rooms.
- Each room description must end with **exactly one trailing newline** immediately before the terminating `~`.

### Exit Rules
- Exits must be **bi-directional**: if room A connects to room B via direction X, room B must provide the opposite-direction exit back to A.
- The one external world link (legacy entrance point in Zone A) is exempt where the other area controls the return exit.
- Repeated movement in the same direction must not enter a directional loop.
- When a room uses a **named exit** (non-empty `<exit_keyword>`), that exit name must appear in at least one of: the room's main description, an object that spawns in the room, or an `E` extra description.
- Named exit keywords for door-style exits must be prefixed with `^` (e.g., `^bone gate`, `^iron grate`).
- Any exit set to `closed` or `locked` via `#RESETS` command `D` must have `EX_ISDOOR` (`1`) set in its room `D<door>` definition.
- If a door is reset to `locked` (state `2`), `<key_vnum>` must reference a valid key object vnum (not `-1`), and that key object must exist in `#OBJECTS`.

### Extra Description Rules
- Extra-description keywords must be **discoverable in-room**: at least one keyword in any chain must appear in the room's main description.
- Chained `E` references are allowed (extra 1 text mentions extra 2 keyword, etc.) but each chain must be anchored in the room's main description.
- Extra-description text must end with exactly one trailing newline before `~`.

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

### Mobile Spec Compliance

**Required `act` flags for all mobs:**
- Every mob must have `stay_area` (`64`) set so it does not wander out of the Gloamvault.
- Stationary mobs (sentries, scribes, named bosses) must also have `sentinel` (`2`) set.
- Boss mobs (Ossuary Champion, Dusk Prelate, Matriarch Velastra) must have `sentinel` (`2`) + `boss` (`67108864`) and must be placed only in rooms flagged `no_mob` (`4`).
- Strong non-boss mobs (Violet Lector, Umbral Cantor, Ashen Warlock, Velastra Handmaiden) must be flagged `solo` (`33554432`).
- The `invasion` flag (`536870912`) must never be set by builders — it is runtime-only.

**`aggressive` flag policy:**
- Set `aggressive` (`32`) on mobs whose behavior is to initiate combat on sight: hostile undead, vault stalkers, carrion templars, and similar hostile guardians.
- Do **not** set `aggressive` on mobs that should react only when provoked or attacked: scribes, novices, and neutral functionaries.

**`undead` flag usage:**
- Mobs that are undead in nature (spectral cantors, ossuary constructs, spirit-memory loops) should have `undead` (`16384`) set.

**Mobile description format (strict per spec):**
- `long_descr` must be exactly one text line, followed by a newline, followed by a line containing only `~`. Multi-line `long_descr` is **never** valid.
- `description` must end with exactly one newline immediately before the terminating `~`.
- No vnum may appear in any mob description text.

**Extension lines (`!`, `|`, `+`):**
- All mobs should include the `!` extension line to define `class`, `clan`, `race`, `position`, `skills`, `cast`, and `def` bitvectors.
- Use the melee profile skill floor/ceiling table for militant-order mobs (Gloom Sentry, Carrion Templar, Reliquary Knight, etc.).
- Use the hybrid profile for mixed-role mobs (Crypt Interrogator, Violet Lector, Velastra Handmaiden).
- Use the caster profile for ritual/caster mobs (Ashen Warlock, Umbral Cantor).
- The `|` extension line should define elemental strong/weak/resist/suscept where thematically appropriate (e.g., `shadow` (`256`) strong magic for shadow-affiliated mobs; `holy` (`8`) susceptibility for undead vault creatures).
- The `+` extension line defines combat modifier tuning and should be set for boss and elite mobs.

**Loot table design (`l`/`L` extensions):**
- Boss and elite mobs should use loot tables to distribute rewards.
- `loot_amount` is percent-based: `100` = one guaranteed item, `150` = one guaranteed + 50% chance for second.
- Items referenced by loot tables must have `ITEM_LOOT` (`67108864`) set in `extra_flags`.
- The sum of all `loot_chance[x]` values on an `L` line must be `<= 100`.

**`#SPECIALS` assignments:**
- Militant mobs may use `spec_guard` or `spec_rewield` where appropriate.
- Caster mobs may use `spec_cast_cleric`, `spec_cast_mage`, or `spec_cast_undead` depending on role.
- Boss mobs may use `spec_cast_bigtime` for enhanced casting behavior.
- Mob specials beginning with `spec_summon_` must **never** be set in area files (runtime-only).
- Mob specials beginning with `spec_keep` may only be assigned manually by a human.

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

### Object Spec Compliance

**Required for all objects:**
- Every object must include `ITEM_TAKE` (`8388608`) in wear flags.
- No object may include `ITEM_WEAR_CLAN_COLORS` (`16777216`).
- Object `name`, `short_descr`, and `description` must be thematically consistent with the object's non-`take` wear flags (e.g., a `head` item must read as headgear, `wrist` as wristwear, `hold` as a held item).
- Object `<name>~` values must be **unique within the area file** — no duplicate item names.
- Do **not** define fixed stats in area files — item stats are generated at runtime.
- Do **not** set `ITEM_GENERATED` (`1`) in area files — it is runtime-managed.

**Weight archetype encoding:**
- `1`-`5`: caster item (Lector set pieces).
- `6`-`10`: melee item (Reliquary set pieces).
- `11`-`15`: tank item (Bellkeeper set pieces).

**Boss and loot item flags:**
- Items that can drop from boss mobs (Ossuary Champion, Dusk Prelate, Matriarch Velastra) must include `ITEM_BOSS` (`134217728`) in `extra_flags`, regardless of delivery method (loot table or reset equipment).
- Items spawned from mob loot tables (`l`/`L` extension) must have `ITEM_LOOT` (`67108864`) set in `extra_flags`.

**Weapon requirements:**
- Weapons must have `item_type = 5` (`ITEM_WEAPON`) and wear flags including both `hold` (`32768`) and `take` (`8388608`).
- `value3` (weapon attack type) must be thematically consistent with the weapon's name and description (e.g., a reliquary mace uses `pound` (`7`), a bone dagger uses `stab` (`2`)).
- `value3 = 0` (`hit`) must not be used unless the object also has `ITEM_FIST` extra flag.
- Two-handed weapon names (e.g., "great sword") must include `ITEM_TWO_HANDED` (`2147483648`) extra flag.

**Extra description format:**
- `E` entry descriptions must end with exactly one trailing newline before `~`.
- No vnums may appear in any object description text.

**Object family wear flag mapping:**
- Ritual Vestments & Jewelry: `halo` (`1`), `head` (`8`), `neck` (`128`), `about` (`65536`), `finger` (`8192`), `waist` (`131072`).
- Militant Relics: `shoulders` (`512`), `arms` (`1024`), `wrist` (`2048`), `hands` (`4096`), `body` (`262144`), `legs` (`1048576`), `feet` (`2097152`).
- Office Implements: `hold` (`32768`) — lector rods and whisperwires are held items.
- Bestial/Monstrous Pieces: `claws` (`16384`), `tail` (`524288`), `hooves` (`4194304`).

---

## Reset and Encounter Distribution Plan

- Zone A/B: dense but forgiving population.
- Zone C/D: medium density with occasional caster spikes.
- Zone E: lighter combat, more utility/lore NPCs.
- Zone F/G: high density, patrol overlap.
- Zone H: sparse but dangerous elites.
- Zone I: staged final cluster with boss reset choreography.

### Reset Spec Compliance

**Reset commands and format:**
- `M`: load mob (`arg1` mob vnum, `arg2` limit, `arg3` room vnum).
- `O`: load object into room (`arg1` obj vnum, `arg2` limit, `arg3` room vnum).
- `G`: give object to previous mob (`arg1` obj vnum, `arg2` limit) — no `arg3`.
- `E`: equip object on previous mob (`arg1` obj vnum, `arg2` limit, `arg3` wear_loc).
- `D`: set door state (`arg1` room vnum, `arg2` door 0..5, `arg3` state 0..2: open/closed/locked).
- `P`: put object into container (`arg1` obj vnum, `arg2` limit, `arg3` container obj vnum).
- `#RESETS` must **not contain blank lines**.

**Door and key requirements:**
- If a door is reset to `locked` via `D` state `2`, `<key_vnum>` must be set to a valid key object vnum (not `-1`).
- For every locked-on-reset door, an object record for that key vnum must exist in `#OBJECTS` (create the key item if it does not already exist).
- Key objects should use `item_type = 18` (`ITEM_KEY`) and wear flags including `take` (`8388608`).
- Any exit set to `closed` or `locked` on area reset via `D` must also have `EX_ISDOOR` (`1`) set in its room `D<door>` definition.
- The Tri-Sign lock sequence (Crescent/Bell/Veil doors in Zone H) requires three key objects in the vnum pool.

**Vnum validity:**
- Resets must reference valid vnums for the target type required by the command (room/mobile/object as applicable).
- All referenced vnums must fall within the `30100-30174` envelope.

**Reset logic goals:**
- Avoid spawn stacking in narrow hallways.
- Ensure at least one recoverable route for lower-level parties.
- Place key lore NPC resets in predictable spaces for repeat readability.
- Boss mob resets go in `no_mob` flagged rooms only.

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

### Midgaard Connection (Primary)
- The Gloamvault entrance is accessed through Midgaard room `3005` (Violet Reliquary Arch), which is a Violet Compact registry checkpoint. This makes the Gloamvault a jurisdictional threshold under Midgaard's Containment Era framework: persons who descend are considered to have "left Midgaard" for jurisdictional purposes.
- Midgaard's Violet Compact legalized bounded occult handling under registry supervision — the Gloamvault is one of the subterranean connections that the Compact was designed to manage.
- Midgaard's civic archives (Granite Arcade, Magistrate Ledgerhouse) may contain references to Kel'Shadra confession tariffs, disputed inheritance verdicts validated by Gloamvault witnesses, and sealed warrants related to Bell Synod operations.
- The Gloamvault's oath-ledger system parallels Midgaard's own civic oaths (Oath of Light, Oath of Stone, Oath of Bread) — both cities use formalized vows as instruments of social control, but the Gloamvault weaponized its system where Midgaard idealized its.
- Midgaard's Continuity-vs-Reckoning political tension has a direct parallel in the Gloamvault's own Continuance Clergy vs Ash Ledger Remnants — institutional memory contested by reform.
- Midgaard's Temple of the Resounding Heart may hold memorial rolls that reference Bell Synod confession casualties or "names removed from official rolls" during the Violet Silence festival.

### Kiess Connection
- Kiess's Syndic Council archives may preserve sanitized versions of oath law derived from Bell Synod templates, brought to Evermeet by Kel'Shadra missionaries before its fall. The punitive clauses were stripped for civic use, but the structural framework survives.
- Kiess's Temple of Concord could reference the Gloamvault's fall as a cautionary institutional example alongside the Sunken Sanctum — institutions that consumed the people they were meant to protect.
- The Central Prism's memorial functions parallel the Gloamvault's original "Last Witness" purpose: both are sites where testimony is meant to be preserved beyond the life of the speaker.

### Kowloon Connection
- Kowloon's covenant/legal scholars might circulate apocryphal extracts on the danger of single-institution memory custody, citing "southern bell crypts" without naming Kel'Shadra directly.
- Kowloon's Jade Magistracy records may reference Kel'Shadra oath-law precedents in their own founding legal corpus — the Neon Covenant's structure (witnessed oaths, public judgment) shares conceptual lineage with the Witness/Offer/Bind doctrine.
- The Ash Cult cells in Kowloon, who preach that institutions defy natural judgment, would find the Gloamvault's trajectory — from preservation to self-perpetuation — a perfect sermon illustration.

### Sunken Sanctum / Spirebound Conclave Connection
- Both the Gloamvault and the Sunken Sanctum represent institutions that began with preservation charters and ended by consuming those they were meant to protect.
- The Conclave's doctrine of "Continuance" echoes the Gloamvault's transformation from "Last Witness" to "Last Authority" — both corruptions of a preservation mandate.
- The Conclave's three factions (Ember Canon, Prism Thesis, Pale Ledger) structurally mirror the Gloamvault's three houses (Bell Synod, Ash Scriptorium, Reliquary Host) — tripartite governance that failed to maintain balance.
- Sealed warrants from Midgaard that authorized prisoner transfers to the Conclave may have been validated using Kel'Shadra oath-law frameworks — the Gloamvault's procedural machinery lubricating another institution's atrocities.

---

## Quests

Seven static quests target the Gloamvault, offered by three city postmasters according to each city's institutional interest in the vault. Three independent entry points (non-chain) allow players to begin from Midgaard, Kiess, or Kowloon, with Midgaard and Kowloon chains progressing toward boss-level warrants. Two quests culminate in boss kills (Ossuary Champion, Matriarch Velastra).

### Quest ID 54 — Gloamvault threshold audit: novice interdiction

| Field | Value |
|---|---|
| **File** | `55.prop` |
| **Static ID** | 54 |
| **Prerequisite** | -1 (none — chain entry point) |
| **Type** | 1 (kill variety) |
| **Num Targets** | 3 |
| **Target Vnums** | `30100 30102 30117` (Ash Acolyte, Grave Choir Novice, Bone Lantern Bearer) |
| **Kill Needed** | 0 |
| **Level Range** | 5-12 |
| **Offerer Vnum** | 13001 (Kiess Postmaster) |
| **Reward Gold** | 800 |
| **Reward QP** | 2 |
| **Reward Item** | 0 0 (none) |

**Narrative rationale:** Kiess's Syndic Council archives preserve oath-law templates derived from Bell Synod originals, brought to Evermeet by Kel'Shadra missionaries before the vault's fall. Institutional memory clerks have detected renewed activity signatures consistent with active novice intake — bells tolling on schedules that should have ceased generations ago. The postmaster needs field confirmation that the Gloamvault's onboarding apparatus is still processing initiates, and wants the lowest-tier operators disrupted so Kiess route analysts can assess whether the vault poses a threat to Roc Road traffic.

**Accept message:** Kiess institutional monitors have flagged renewed bell activity from the Gloamvault of Kel'Shadra, consistent with active novice intake. The Syndic Council wants field verification: enter the upper vault and engage the novice-tier operators — acolytes, choir novices, and lantern bearers — to confirm that the vault's onboarding machinery is still functional. Route analysts need your kill reports before they can update the threat classification for southern approaches.

**Completion message:** Your field reports confirm what the Syndic Council feared: the Gloamvault's intake apparatus is not merely residual — it is actively processing new arrivals through confession and classification routines. The postmaster has forwarded your observations to both the Temple of Concord and Wall Command for institutional review. Kiess now treats the southern vault threshold as a live hazard zone rather than a sealed ruin.

---

### Quest ID 49 — Violet Compact enforcement: stalker suppression

| Field | Value |
|---|---|
| **File** | `50.prop` |
| **Static ID** | 49 |
| **Prerequisite** | 54 |
| **Type** | 3 (kill count) |
| **Num Targets** | 1 |
| **Target Vnums** | `30115` (Vault Stalker) |
| **Kill Needed** | 5 |
| **Level Range** | 8-15 |
| **Offerer Vnum** | 3015 (Midgaard Postmaster) |
| **Reward Gold** | 1200 |
| **Reward QP** | 3 |
| **Reward Item** | 0 0 (none) |

**Narrative rationale:** Midgaard's Violet Compact registry checkpoint at the Reliquary Arch (room 3005) has reported vault stalkers breaching the jurisdictional threshold — gauze-wrapped ambush predators that move only when candlelight flickers. Under the Compact's Containment Era framework, anything that crosses the arch from below is Midgaard's enforcement problem. The magistrate's office has authorized a suppression contract: five confirmed stalker kills to re-establish the boundary. This quest chains from ID 54 because Kiess's threat assessment (confirming active vault operations) triggered Midgaard's Compact enforcement protocols.

**Accept message:** The Violet Compact registry at Midgaard's Reliquary Arch has logged multiple threshold breaches by vault stalkers — hostile entities emerging from the Gloamvault into civic jurisdiction. Under Containment Era protocols, this triggers suppression authority. Enter the vault and kill five vault stalkers to re-establish the Compact boundary. Registry inspectors cannot resume threshold patrols until the stalker population is reduced to manageable levels.

**Completion message:** Five vault stalker kills confirmed and logged against the Compact registry. Threshold breach reports have dropped to pre-alert levels, and registry inspectors have resumed patrols at the Reliquary Arch. The magistrate's office notes that your suppression work has restored enough jurisdictional confidence for the Exchange to consider deeper enforcement warrants targeting the vault's command structure.

---

### Quest ID 50 — Covenant precedent recovery: militant purge

| Field | Value |
|---|---|
| **File** | `51.prop` |
| **Static ID** | 50 |
| **Prerequisite** | -1 (none — chain entry point) |
| **Type** | 1 (kill variety) |
| **Num Targets** | 3 |
| **Target Vnums** | `30105 30108 30116` (Carrion Templar, Thorn Reliquary Knight, Funerary Warden) |
| **Kill Needed** | 0 |
| **Level Range** | 10-17 |
| **Offerer Vnum** | 14001 (Kowloon Postmaster) |
| **Reward Gold** | 1400 |
| **Reward QP** | 3 |
| **Reward Item** | 0 0 (none) |

**Narrative rationale:** Kowloon's Jade Magistracy records reference Kel'Shadra oath-law precedents in their own founding legal corpus — the Neon Covenant's structure of witnessed oaths and public judgment shares conceptual lineage with the vault's Witness/Offer/Bind doctrine. Covenant scholars circulate apocryphal extracts citing "southern bell crypts" as cautionary examples of single-institution memory custody, but the original source documents remain inaccessible behind the Reliquary Host's militant patrols. Postmaster Lin needs the militant order forces — templars, knights, and wardens — disrupted so Kowloon's recovery teams can approach the Ash Scriptorium.

**Accept message:** Kowloon's covenant scholars have long cited the Gloamvault's oath-law frameworks as foundational precedent for the Neon Covenant, but the original source documents are locked behind the vault's militant order patrols. The Jade Magistracy has authorized a clearance contract: engage and defeat one each of the Carrion Templars, Reliquary Knights, and Funerary Wardens to create a viable approach corridor toward the Scriptorium of Ash. Recovery teams are standing by pending your field clearance.

**Completion message:** Militant order resistance along the recovery corridor has been broken. Kowloon's covenant scholars report that preliminary access to the Scriptorium approaches is now viable, and the Jade Magistracy has dispatched a sealed assessment to the Courier Lantern Office regarding next steps. The Ash Cult cells have already begun citing your clearance work in their sermons — an institution that required outside force to reach its own archives is, they argue, an institution that has already consumed its purpose.

---

### Quest ID 51 — Registry priority warrant: Ossuary Champion

| Field | Value |
|---|---|
| **File** | `52.prop` |
| **Static ID** | 51 |
| **Prerequisite** | 49 |
| **Type** | 1 (kill variety) |
| **Num Targets** | 1 |
| **Target Vnums** | `30113` (Ossuary Champion) |
| **Kill Needed** | 0 |
| **Level Range** | 15-20 |
| **Offerer Vnum** | 3015 (Midgaard Postmaster) |
| **Reward Gold** | 2000 |
| **Reward QP** | 4 |
| **Reward Item** | 0 0 (none) |

**Narrative rationale:** With stalker threats at the threshold suppressed, Midgaard's magistrates have escalated enforcement to a priority warrant targeting the Ossuary Champion — the rune-plated military enforcer whose patrol routes through the Reliquary Bastion block registry inspectors from documenting the vault's armory-catacomb interface. The Champion embodies the vault's doctrine that compliance is the highest martial virtue, and his continued presence represents an active jurisdictional challenge to the Violet Compact's authority. This is a boss-target quest: the Champion is flagged `boss` and occupies a `no_mob` room in Zone F.

**Accept message:** Midgaard's magistrate court has authorized a priority enforcement warrant targeting the Ossuary Champion of the Gloamvault — the military enforcer whose presence in the Reliquary Bastion prevents registry inspectors from completing their Compact-mandated audit. This is a high-risk contract: the Champion is a fortified combatant armored in rune-etched plate and chained relics, and he does not negotiate. Destroy him so the registry can document what the vault has been arming behind its sealed doors.

**Completion message:** The Ossuary Champion has been destroyed, and registry inspectors have entered the Reliquary Bastion for the first time since the Violet Compact was signed. Preliminary audit reports describe armory-scale weapon cages and sepulcher knight stalls — a standing military infrastructure that the Compact was never informed of. The magistrate's office has classified the audit findings and is preparing civic briefings. Your warrant execution has opened a door that Midgaard's institutions will not be able to close again.

---

### Quest ID 52 — Jade Magistracy deep audit: warlock suppression

| Field | Value |
|---|---|
| **File** | `53.prop` |
| **Static ID** | 52 |
| **Prerequisite** | 50 |
| **Type** | 3 (kill count) |
| **Num Targets** | 1 |
| **Target Vnums** | `30110` (Ashen Warlock) |
| **Kill Needed** | 4 |
| **Level Range** | 14-20 |
| **Offerer Vnum** | 14001 (Kowloon Postmaster) |
| **Reward Gold** | 1800 |
| **Reward QP** | 4 |
| **Reward Item** | 0 0 (none) |

**Narrative rationale:** With the militant order's patrol routes disrupted, Kowloon's recovery teams have identified a secondary obstacle: the Ashen Warlocks maintain ward networks throughout the Sanguine Cloisters and deeper zones that prevent sustained access to the vault's archival core. Their fingertips leak violet sparks that re-seal passages and corrupt recovered documents. The Jade Magistracy needs four confirmed warlock kills to collapse enough of the ward lattice for covenant scholars to reach the original oath-law templates that predate Velastra's doctrinal revisions.

**Accept message:** Kowloon's recovery teams report that the Ashen Warlocks maintain active ward networks that re-seal passages and corrupt any documents carried through their zones. The Jade Magistracy needs the ward lattice collapsed before covenant scholars can safely extract the original oath-law templates. Kill four Ashen Warlocks to disrupt the ward infrastructure sufficiently for archival recovery operations.

**Completion message:** Four Ashen Warlocks have been destroyed and the ward lattice across the vault's middle zones has collapsed into inert fragments. Kowloon's covenant scholars have begun extracting documents from the Scriptorium approaches, and preliminary translations confirm what the Jade Magistracy suspected: the original Kel'Shadra oath-law templates predate Velastra's doctrinal revisions by centuries, and the differences are significant enough to challenge several foundational precedents in the Neon Covenant's own legal corpus. The Magistracy has sealed these findings pending further review — and further action.

---

### Quest ID 53 — Final covenant judgment: Matriarch Velastra

| Field | Value |
|---|---|
| **File** | `54.prop` |
| **Static ID** | 53 |
| **Prerequisite** | 52 |
| **Type** | 1 (kill variety) |
| **Num Targets** | 1 |
| **Target Vnums** | `30114` (Matriarch Velastra) |
| **Kill Needed** | 0 |
| **Level Range** | 18-20 |
| **Offerer Vnum** | 14001 (Kowloon Postmaster) |
| **Reward Gold** | 3000 |
| **Reward QP** | 5 |
| **Reward Item** | 0 0 (none) |

**Narrative rationale:** The Jade Magistracy has determined that the Gloamvault's recursive institutional machinery cannot be interrupted while Matriarch Velastra maintains her throne in the Inner Gloamvault. She is the administrative apex that replaced "Last Witness" with "Last Authority," and her continued presence processes every recovered document through self-serving doctrinal filters before it can reach the outside world. Kowloon's covenant tradition considers her removal a covenant-level obligation: the original oath-law templates that the Neon Covenant's founding legal corpus derives from cannot be authentically recovered while her apparatus recycles them into justifications for her own rule. This is a boss-target quest: Velastra is flagged `boss` and occupies a `no_mob` room in Zone I.

**Accept message:** The Jade Magistracy has issued a covenant-level judgment: Matriarch Velastra of the Gloamvault must be removed. She is the administrative intelligence that transformed Kel'Shadra's preservation mandate into an engine of self-perpetuation, and every document recovered from the vault passes through her doctrinal apparatus before it reaches the outside world. Descend to the Inner Gloamvault and destroy her. The covenant scholars warn that defeating Velastra does not destroy the vault — but it interrupts its recursive self-justification long enough for original records to become accessible again.

**Completion message:** Matriarch Velastra has been destroyed in her throne crypt, and the Inner Gloamvault's administrative recursion has stalled. Kowloon's covenant scholars report that original Kel'Shadra records are surfacing unfiltered for the first time in living memory — oath-law templates, confession ledgers, and founding charter fragments that predate every doctrinal revision Velastra imposed. The Jade Magistracy has classified the recovery as the most significant legal discovery since the Neon Covenant's ratification. The Ash Cult preachers are already composing sermons: the vault that existed to preserve witness was itself the final thing that needed witnessing.

---

### Quest ID 55 — Compact jurisdiction survey: vault functionary assessment

| Field | Value |
|---|---|
| **File** | `56.prop` |
| **Static ID** | 55 |
| **Prerequisite** | -1 (none — chain entry point) |
| **Type** | 1 (kill variety) |
| **Num Targets** | 3 |
| **Target Vnums** | `30101 30103 30118` (Crypt Usher, Bone Scrivener, Crypt Interrogator) |
| **Kill Needed** | 0 |
| **Level Range** | 8-14 |
| **Offerer Vnum** | 3015 (Midgaard Postmaster) |
| **Reward Gold** | 1000 |
| **Reward QP** | 2 |
| **Reward Item** | 0 0 (none) |

**Narrative rationale:** Midgaard's Violet Compact registry at the Reliquary Arch has jurisdiction over anything that crosses the threshold, but the registry has never classified the vault's internal administrative hierarchy. The functionary tier — ushers who direct traffic through processional corridors, scriveners who maintain oath ledgers in bone-ink, and interrogators who process confessions into sentencing queues — represents the vault's bureaucratic backbone. The magistrate's office needs field intelligence on whether these roles are still staffed and operationally active, because a vault with functioning administration is a jurisdictional entity under Compact law, not merely a ruin with squatters. This is an independent Midgaard entry point that does not require prior Kiess or Kowloon intelligence.

**Accept message:** The Violet Compact registry needs to determine whether the Gloamvault operates as a functioning institution or an abandoned ruin — the legal distinction matters for jurisdictional classification. Enter the vault and engage one each of the administrative functionaries: crypt ushers who manage processional traffic, bone scriveners who maintain the oath ledgers, and crypt interrogators who run the confession apparatus. Your kill reports will tell the magistrate's office whether the vault still has a working chain of command.

**Completion message:** Your field reports confirm that the Gloamvault's administrative apparatus is fully operational: ushers direct traffic on standing schedules, scriveners maintain active ledgers, and interrogators process confessions into real sentencing queues. The magistrate's office has formally reclassified the vault from "sealed ruin" to "active extra-jurisdictional institution" under Compact law — a designation that authorizes deeper enforcement actions and obligates the registry to maintain ongoing surveillance. The Reliquary Arch checkpoint is being upgraded accordingly.

---

### Quest Chain Summary

```
Chain A (Kiess → Midgaard):
 54 (Kiess, non-chain) → 49 (Midgaard, chain) → 51 (Midgaard, boss: Ossuary Champion)

Chain B (Kowloon):
 50 (Kowloon, non-chain) → 52 (Kowloon, chain) → 53 (Kowloon, boss: Matriarch Velastra)

Standalone (Midgaard):
 55 (Midgaard, non-chain)
```

**Design notes:**
- Chain A begins at Kiess (institutional detection) and escalates through Midgaard's Violet Compact enforcement machinery, culminating in the mid-boss.
- Chain B operates entirely through Kowloon's covenant scholarship apparatus, escalating from militant clearance through ward suppression to the final boss.
- Quest 55 is a standalone Midgaard entry point focused on jurisdictional classification of the vault's administrative tier — it does not chain into either boss sequence.
- All three chains are independent — players can pursue any or all without cross-chain prerequisites.
- Level ranges respect the Gloamvault's 5-20 target band, with boss quests gated at the upper end.
- No item rewards are specified; gold and QP scale with difficulty and chain depth.

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

