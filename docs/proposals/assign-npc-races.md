# Design Proposal: Assign Races to All NPCs

**Date:** 2026-03-20
**Status:** Draft — awaiting user sign-off
**Branch:** `claude/assign-npc-races-aEtbX`

---

## Problem

All 2,412 NPCs in the game currently default to race `0` (Human), or in some cases carry
incorrect legacy assignments (Ashborn used for desert animals, Rivennid used for undead, Ushabti
used for scarab insects). This means:

- Undead mummies, ghouls, and skeletons are treated as Human in combat calculations.
- Serpents, spiders, scorpions, wolves, hawks, and other fauna share the Human element profile.
- NPC racial resistances/susceptibilities, size flags (`LARGE`, `HUGE`, `TOUGH_SKIN`, etc.), and
  healing modifiers are wrong for almost every non-humanoid mob in the game.
- The race field carries no thematic meaning for 92 % of all NPCs.

---

## Approach

### Part 1 — New NPC-Only Race Entries (src/ change)

Add 13 new race entries to `const_race.c` (indices 10–22). Increase `MAX_RACE` in `config.h`
from `10` to `23`. Each new entry has `player_race = FALSE`, so no player-selection code is
affected. All wear_locs default to `TRUE` so NPC gear is never accidentally blocked by race.

### Part 2 — Area File Updates (no src/ change required for this part)

Update the `!` extension line race field for every NPC in all 45 `.are` files using the
taxonomy below. Existing wrong non-zero assignments (Ashborn/Rivennid/Ushabti on incorrect
mobs) are corrected at the same time.

### Part 3 — Test stub updates

Unit test files that define their own `race_table[MAX_RACE]` stub (via designated-initialiser
syntax or zero-init) will compile correctly with a larger `MAX_RACE` — no logic changes needed.

---

## New NPC-Only Races (indices 10–22)

| # | Code | Name | Size/Skin flags | Strong | Weak | Resist | Suscept | Theme |
|---|------|------|-----------------|--------|------|--------|---------|-------|
| 10 | Und | Undead | DARKNESS, SLOW_HEAL | SHADOW, PHYSICAL | HOLY, MENTAL | SHADOW, POISON | HOLY, FIRE | Reanimated dead — skeletons, wraiths, mummies, ghouls, liches, revenants, shades, vampires, death knights |
| 11 | Ser | Serpent | TAIL, TOUGH_SKIN, IMMUNE_POISON | POISON, WATER | FIRE, AIR | POISON | FIRE, HOLY | Actual snakes (not Kethari) — asps, vipers, cobras, constrictors, adders |
| 12 | Arc | Arachnid | TOUGH_SKIN | POISON, EARTH | FIRE, AIR | POISON, EARTH | FIRE | Spiders, scorpions, glassworms |
| 13 | Ins | Insect | TINY, TOUGH_SKIN | EARTH, POISON | FIRE, AIR | POISON | FIRE, AIR | Beetles, scarabs, mites, locusts, centipedes, cricket swarms, leeches |
| 14 | Can | Canine | FAST_HEAL | PHYSICAL, AIR | SHADOW, MENTAL | PHYSICAL | SHADOW, MENTAL | Wolves, jackals (animal), hounds, foxes, bears |
| 15 | Avi | Avian | FAST_HEAL | AIR, PHYSICAL | EARTH, SHADOW | AIR | EARTH, FIRE | Birds — hawks, vultures, eagles, ibis, ravens, owls, bats |
| 16 | Fel | Feline | FAST_HEAL | PHYSICAL, AIR | SHADOW, MENTAL | PHYSICAL | SHADOW, MENTAL | Cats, lions, panthers, tigers; also ungulates (stag, boar) as mammal catch-all |
| 17 | Rep | Reptile | TOUGH_SKIN | EARTH, POISON | AIR, MENTAL | EARTH, POISON | AIR, FIRE | Lizards, crocodiles, basilisks, frogs, tortoises |
| 18 | Drg | Dragon | HUGE, TOUGH_SKIN, FAST_HEAL | FIRE, EARTH | MENTAL, SHADOW | FIRE, PHYSICAL | MENTAL, HOLY | All dragons, drakes, wyrms, wyverns |
| 19 | Elm | Elemental | HUGE, TOUGH_SKIN, SLOW_HEAL | EARTH, PHYSICAL | HOLY, MENTAL | PHYSICAL, POISON | HOLY, MENTAL | Elementals, golems, animated constructs, dust devils, sandstorms, djinn |
| 20 | Dem | Demon | LARGE, TOUGH_SKIN, DARKNESS | SHADOW, FIRE | HOLY, MENTAL | SHADOW, FIRE, POISON | HOLY, AIR | Demons, fiends, gargoyles, infernals |
| 21 | Pla | Plant | TOUGH_SKIN, SLOW_HEAL | EARTH, WATER | FIRE, AIR | EARTH, POISON | FIRE, AIR | Treants, tendrils, vine creatures, fungal monsters, bog horrors, dryads |
| 22 | Aqu | Aquatic | IMMUNE_POISON | WATER, POISON | FIRE, EARTH | WATER, POISON | FIRE, EARTH | Fish, eels, crabs, water nymphs, bog hydras, tidal lurkers |

*Note: "Serpent" (11) and "Serathi" (8) share three letters; in code they use distinct short codes
`Ser` and `Ser`…  to avoid confusion the new Serpent NPC race short code will be `Snk` (Snake).*

---

## Playable Race Assignments for Humanoid NPCs

All 10 playable races appear on at least some NPCs. Some are rare by nature of the
world — not every city has Serathi ambassadors — but every race is represented.

### Human (0)
Generic humanoids: guards, merchants, raiders, bandits, cultists, and city workers in
Midgaard, Kiess, Rakuen, Mafdet, Wood, Newadept, and Shadowmere mortal NPCs. Also covers
humanoid NPCs in desert areas who have no specific racial identity.

### Khenari (1) — jackal-folk of Akh'enet
- Akh'enet: **Vizier Nephkara**, **High Priest Amenthari** (the two named humanoid bosses of the
  Khenari homeland)
- Eastern Desert: **azure vizier** (#2060), **solar acolyte** (#2052) — Khenari who watch the
  obelisk shrines at the edge of their territory

### Khephari (2) — scarab-beetle folk of the desert
The Lost City (`lost_city.are`) is an ancient Khephari ruin — its warriors, priests, and
oathkeepers are all Khephari people preserving their old order:
- **sand reaver, ruin acolyte, glass lancer, sunken guard, dust cantor, dune stalker,
  obelisk scout, scarab knight, wind oathkeeper** (all repeating groups, vnums 5850–5928)
- Bosses: **Dune Vizier, Salt Hierophant, Sunbreaker Marshal**

Also: Khardaan **burial acolyte** (#6220) — a Khephari priest who tends the dead.

### Ashborn (3) — fire-kin of Cinderteeth
- Cinderteeth Mountains: all **ashfang** warrior/scout/shaman NPCs and all **ash cult**
  members (zealots, fire-keepers, doctrine-speakers, ritualists, heralds, oracles)
- Great Northern Forest: **ashfang scout/raider/trapper/shaman/berserker/warmaster** (#4095–4102,
  #4109, #4123, #4131, #4139, #4147, #15585 The Ashfang Prime) — Ashborn range-raiders

### Umbral (4) — shadow-walkers of the Gloamvault
- Umbra Heartspire: all named cultist, guard, duelist, and knight NPCs that are not explicitly
  undead or demonic (53 NPCs total)
- Gloamvault / Kelshadra: the mortal cultist types — **ash acolyte, crypt usher, gloom sentry,
  dark cultist** — Umbral who joined the shadow cults of their homeland

### Rivennid (5) — fungal mycelians of the Withered Depths
- Withered Depths: **withered druid** (#422), **canopy warden** (#403), **lost herbalist** (#470)
  — Rivennid who have gone deeper into fungal communion
- Great Northern Forest: **acolyte rootbound** (#4112), **ritualist rootbound** (#4113),
  **cultist rootbound crown** (#4140) — Rivennid missionaries who spread mycelial teaching
- Thornwood: **deepwood shaman** (#3035), **feral grove keeper** (#3055) — Rivennid who have
  merged further with the deep forest

### Deltari (6) — river delta folk of Kowloon
All city-worker NPCs in Kowloon (merchants, captains, guards, clerks, dock workers, etc.) — the
entire populated city of Kowloon is Deltari-run.

### Ushabti (7) — animated stone guardians of the Great Pyramid
Stone guardian types in the pyramid complexes. Distinguished from golems (which are Elemental)
by being divine animated statues rather than constructed automata:
- Akh'enet: **petrified sentinel** (#958), **catacomb guardian** (#965), **eternal sentinel** (#975)
- Northern Pyramid: **desert gate guardian** (#3151), **desert stone titan** (#3158), **stone
  scarab guardian** (#3175), **granite guardian statue** (#3168), **ancient face guardian** (#3179),
  **hieroglyph guardian** (#3196), **apex sentinel** (#3208), **golden sphinx** (#3161)
- Southern Pyramid: **basalt guardian statue** (#3469), **stone scarab sentinel** (#3476), **jade
  sphinx** (#3462), **moon gate guardian** (#3450), **ancient basalt guardian** (#3511), **sovereign
  tomb guardian** (#3625), **petrified desert titan** (#3629)
- Pyramid.are: **pyramid guard** (#5996), **chamber guardian** (#6014), **tomb warden** (#6029),
  **anubis champion** (#6034)

### Serathi (8) — avian sky-folk of the western air-roads
Serathi appear as rare travelers and specialists where their speed and aerial nature is valued:
- Forest Preserve: **preserve falconer** (#5018), **preserve tracker** (#5017) — Serathi naturalists
  who bond with raptors and range the heights of the preserve canopy
- Rakuen: **canopy compact runner** (#4651), **dispatch registrar rakuen** (#4650) — Serathi
  couriers who use the elevated canopy routes of Rakuen's terraced city, and the
  registrar who coordinates their flight-fast deliveries
- Sepulcher Pasture: **ossuary falconer** (#5556) — a rare Serathi who tends death-birds in the
  grim pasture, prized for their bird-bonding skill

### Kethari (9) — serpentine folk of Saltglass Reach
Saltglass Reach is Kethari territory. All Reach wardens, glasswalk guides, caravan provisioners,
transit clerks, red sand agents, synod operatives, and warden commanders there are Kethari.

---

## NPC Count by Assigned Race (updated)

| Race | Count | Representative examples |
|------|-------|------------------------|
| Human (0) | ~1,450 | Guards, merchants, raiders, priests, cultists |
| Undead (10) | ~305 | Skeletons, mummies, wraiths, liches, revenants, vampires, death knights |
| Elemental (19) | ~89 | Golems, elementals, constructs, dust devils, sandstorms |
| Insect (13) | ~74 | Scarabs, beetles, swarms, centipedes, leeches, crickets |
| Plant (21) | ~53 | Treants, tendrils, briar creatures, fungal horrors, bog beasts |
| Umbral (4) | ~55 | Umbra Heartspire cultists/guards, Gloamvault cultist types |
| Serpent (11) | ~45 | Asps, vipers, cobras, constrictors, adders |
| Avian (15) | ~43 | Hawks, vultures, eagles, ibis, ravens, owls, bats |
| Deltari (6) | ~41 | Kowloon city workers |
| Kethari (9) | ~41 | Saltglass Reach wardens and caravan folk |
| Canine (14) | ~40 | Wolves, jackals, hounds, foxes, bears |
| Arachnid (12) | ~39 | Spiders, scorpions, glassworms |
| Khephari (2) | ~27 | Lost City warriors, priests, oathkeepers; Khardaan burial acolyte |
| Ashborn (3) | ~28 | Cinderteeth ashfang + ash cult; Great Northern Forest ashfang raiders |
| Ushabti (7) | ~22 | Pyramid stone guardians, sphinxes, petrified sentinels |
| Feline (16) | ~20 | Cats, lions, panthers; boar/stag as mammal catch-all |
| Reptile (17) | ~16 | Lizards, crocodiles, basilisks, frogs |
| Rivennid (5) | ~10 | Withered druid, rootbound cultists, deepwood shamans |
| Aquatic (22) | ~14 | Crabs, eels, nymphs, tidal lurkers, bog hydras |
| Dragon (18) | ~11 | All dragons, drakes, wyrms |
| Khenari (1) | ~6 | Vizier Nephkara, High Priest Amenthari, Eastern Desert obelisk keepers |
| Demon (20) | ~10 | Gargoyles, demonic entities |
| Serathi (8) | ~5 | Preserve falconers/trackers, Rakuen canopy runners, ossuary falconer |

---

## Affected Files

### `src/` changes
- `src/headers/config.h` — `MAX_RACE` 10 → 23
- `src/const_race.c` — 13 new race entries appended after Kethari

### `area/` changes (no `src/` build required)
All 45 `.are` files — race field on `!` extension line updated per NPC

### Test stubs (recompile only, no logic change)
Files using `race_table[MAX_RACE] = {0}` or similar zero/designated-init stubs will automatically
accommodate the expanded array size.

---

## Trade-offs and Notes

- **No player impact:** All new races have `player_race = FALSE`, so login, reincarnation, and
  race-selection flows are unchanged.
- **Element combat:** NPCs will now gain correct elemental resistances/susceptibilities in the
  `damage.c` calculations. This slightly changes combat math for player-vs-NPC fights — undead
  become weaker to Holy/Fire, serpents become immune to poison, elementals resist physical, etc.
  This is intentional and lore-correct.
- **Size flags:** Large/Huge mobs (golems, dragons, elementals) gain LARGE/HUGE flags from their
  race, which may affect some size-based combat checks. Worth monitoring.
- **Keyword disambiguation:** The short code for the new "Serpent" race will be `Snk` to avoid
  any future confusion with "Ser" (Serathi).
- **Scale:** 2,412 NPCs across 45 files. Implementation is script-assisted and validated by
  the integration test (which boots the full server and walks the login flow).
- **Existing wrong assignments fixed:** northern_pyramid and pyramid area NPCs currently using
  Ashborn (3) for animals and Rivennid (5) for undead will be corrected to Arachnid/Avian/Reptile
  and Undead respectively.
