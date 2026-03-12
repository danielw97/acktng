# Area Plan: Umbra Heartspire

## Overview
- **Area Name:** `@@dThe @@pUmbra @@mHeartspire@@N`
- **File Name:** `umbra_heartspire.are`
- **Keyword:** `umbra_heartspire`
- **Intended Levels:** 25-40
- **Vnum Range:** `11200-11274`
- **Vnum Count:** 75 (all room vnums used)
- **Reset Rate:** 10
- **Design Intent:** Full structural and thematic rebuild (rooms, mobiles, objects, resets) while retaining the existing vnum envelope.

## Lore Dependency
- This area should be implemented directly from `umbra_heartspire_lore.md`.
- Narrative anchors should explicitly tie to Midgaard's Violet Compact and archive politics, and to Kiess' Evermeet-afterlife identity and ritual bureaucracy.

## Color Theme (multi-color)
The Heartspire should read as a ritual-judicial cathedral where memory, blood-oaths, and starlit shadow are institutionalized:

- **Base stone / dread architecture:** `@@d` dark grey
- **Umbral sanctity / veil motifs:** `@@p` purple
- **Witchflame / stained glass / pulse conduits:** `@@m` magenta
- **Astral machinery / omen-light:** `@@l` light blue
- **Civic-record glyphwork (Midgaard tie):** `@@a` light cyan
- **Warning seals / blood tithe / punitive power:** `@@e` light red
- **Relic metal / processional authority:** `@@W` white
- **Ritual highlights / vow-candles / heraldry:** `@@y` yellow
- **Always reset with:** `@@N`

Policy notes:
- Do not use `@@k` as a thematic color.
- Keep color usage intentional; emphasize titles and key nouns, not every line.

## #AREA Header Plan
- `Q 16`
- `K umbra_heartspire~`
- `L @@W(@@a25 40@@W)@@N~`
- `N 5` (retain existing area number)
- `I 25 40`
- `V 11200 11274`
- `X 0`
- `F 10`
- `U @@dThe stones weep @@mviolet@@d shadows around you.@@N~`
- `O Virant~` (normalized owner casing)
- ACL lines (`R`/`W`) remain builder-controlled at integration time.

## Structural Layout (All 75 Rooms Used)
The rebuilt area becomes a **5-spoke vertical citadel** joined by a central nave and late-game convergence ring.

### Spine and Wings
- **Central Processional Spine (11200-11220):** Entry, liturgical gauntlet, first boss.
- **Mirror-Archive Wing (11221-11237):** Kiess/Midgaard legal memory, mirrored testimony, social elite decay.
- **Iron-Wake Bastion Wing (11238-11254):** Militarized penitence, execution logistics, martial boss route.
- **Astral-Orrery Wing (11255-11270):** Star-ritual theology, prophecy engines, cosmic corruption.
- **Heartspire Root-Crown Finale (11271-11274):** Convergence bridge, inner sanctum, final throne.

### Connectivity Requirements
- Keep `11202` as the first major junction feeding all three side wings.
- Preserve one-way progression pressure in each wing, with selective loops for recovery.
- Ensure all side wings rejoin at `11271`/`11272` before entering `11273-11274`.
- Preserve external world linkage at `11200` (to Midgaard-side route) and optional deep connector at `11274` only if lore-justified.

## Room-by-Room Rebuild Plan

### 11200-11220: Processional Spine (Entry + Ritual Judiciary)
1. **11200 The Sable Causeway** - transitional threshold from civic world into sanctioned shadow.
2. **11201 Gate of Mourning Iron** - oath checkpoint; inscriptions reference the Violet Compact.
3. **11202 Atrium of Bleeding Glass** - grand nexus with exits to all three wings.
4. **11203 Nave of Hollow Psalms** - doctrine recitation hall.
5. **11204 Chancel of Ash Veils** - veiled magistrate processional.
6. **11205 Reliquary of Cindered Saints** - saint-relic bureaucracy.
7. **11206 Crypt-Warden Gallery** - wardens who police testimony.
8. **11207 Hall of Stitched Banners** - conquest banners rewritten as legal edicts.
9. **11208 Choir Stairs of Dread** - choral command hierarchy.
10. **11209 Lantern Cloister** - Midgaard lantern-reform iconography inverted into surveillance.
11. **11210 Transept of Black Snow** - ashfall ritual crossing.
12. **11211 Reliquary of Hollow Kings** - failed dynasties catalogued as evidence.
13. **11212 Balcony of Veiled Moons** - observation ledge above penitents.
14. **11213 Antechamber of Bone Incense** - preparation chamber for sentence rites.
15. **11214 Shrine of the Last Bell** - bell marks legal finality.
16. **11215 Mausoleum Crossing** - fork-control chamber.
17. **11216 Vault of Red Candlewax** - bloodwax tithe store.
18. **11217 The Oathbound Rotunda** - testimony circle with mirrored acoustics.
19. **11218 Processional of Splintered Crowns** - anti-monarch doctrine corridor.
20. **11219 Sepulchral Verge** - pre-throne warning zone.
21. **11220 The Thorned Throne** - **Boss 1:** ceremonial adjudicator who "binds" trespassers.

### 11221-11237: Mirror-Archive Wing (Kiess + Midgaard Record Wars)
22. **11221 Mirror Arcade** - entry to archive-illusion wing.
23. **11222 Gallery of Rain-Black Marble** - noble epitaph corridor.
24. **11223 Scriptorium of Torn Litanies** - redacted decrees and doctrinal edits.
25. **11224 The Ink-Drowned Corridor** - flooded ledger hall.
26. **11225 Hall of Distant Organ Notes** - sonic indexing chamber.
27. **11226 Archive of Unquiet Tomes** - active legal-memory stacks.
28. **11227 Passage of Raven-Wing Drapes** - clandestine courier route.
29. **11228 Dust Chapel** - rites for erased identities.
30. **11229 Rookery Stair** - vertical relay for message ravens.
31. **11230 The Weeping Belfry** - bell tower tuned to confession cycles.
32. **11231 Gallery of Cracked Reliquaries** - broken saint contracts.
33. **11232 Shaft of Blue Night** - astral lightwell.
34. **11233 Bone-Latched Vestibule** - archive lock chamber.
35. **11234 Conservatory of Dead Roses** - Kiess aristocratic funerary motif.
36. **11235 Obsidian Service Hall** - servant routes and hidden custodial records.
37. **11236 The Casket Promenade** - procession route for condemned documents and nobles alike.
38. **11237 The Harrowed Duchess's Chamber** - **Boss 2:** mirror-duchess tied to Evermeet collapse memory.

### 11238-11254: Iron-Wake Bastion Wing (Militant Orthodoxy)
39. **11238 Soot-Fresco Passage** - military liturgy murals.
40. **11239 Anvil Cloister** - forging of sentence implements.
41. **11240 Bastion Walk of Rusted Pikes** - defensive parapet.
42. **11241 Barracks of Silent Plate** - silent oathbound troops.
43. **11242 Chamber of Severed Heraldry** - conquered-house emblems.
44. **11243 The Gallowglass Hall** - elite execution retinue.
45. **11244 Tithe Vault of Iron Teeth** - confiscation treasury.
46. **11245 Forge of Funeral Steel** - armament sanctification.
47. **11246 Smelter Steps** - heat descent/ascension choke point.
48. **11247 Dripstone Armory** - reserve weapon cavern.
49. **11248 Arsenal of Veinfire** - cursed munitions stock.
50. **11249 Warden's Dais** - command platform.
51. **11250 Bridge of Charred Pennants** - symbolic war-bridge.
52. **11251 Mercyless Guardpost** - no-retreat checkpoint.
53. **11252 The Last Portcullis** - hard gate encounter.
54. **11253 War-crypt Antechamber** - pre-execution staging.
55. **11254 Execution Chapel** - **Boss 3:** Marshal of the Iron Wake.

### 11255-11270: Astral-Orrery Wing (Cosmic Doctrine)
56. **11255 Spiral of Mourning Stars** - ascent into cosmic theology.
57. **11256 Observatory of Drowned Constellations** - dead-sky maps.
58. **11257 Arcane Choir Loft** - spell-chorus controls.
59. **11258 The Gloom Compass Hall** - directional omen engine.
60. **11259 Astral Ossuary** - preserved star-seers.
61. **11260 Moonwell of Black Milk** - alchemical basin.
62. **11261 Phial Laboratory** - extractive rite workshop.
63. **11262 Rune-Braided Passage** - warded transit hall.
64. **11263 Shadow Orrery** - rotating fate-machine.
65. **11264 Hematite Planetarium** - blood-metal celestial doctrine.
66. **11265 Aetheric Causeway** - unstable bridge of sigils.
67. **11266 Vault of Crystalline Screams** - prophetic failure archive.
68. **11267 The Null-Sigil Gallery** - erased destiny proofs.
69. **11268 Night Prism Landing** - refracted omen nexus.
70. **11269 Apse of Broken Prophecies** - failed revelations sanctum.
71. **11270 The Starved Abbot's Pinnacle** - **Boss 4:** astral mortifier archetype.

### 11271-11274: Root-Crown Finale
72. **11271 Root of the Heartspire** - all-wing convergence root.
73. **11272 Causeway of Converging Wings** - final prep bridge from main atrium path.
74. **11273 Sanctum of the Ebon Pulse** - penultimate ritual machine chamber.
75. **11274 The Umbral Crownseat** - **Final Boss:** Abbot Noctivar, Ebon Hierophant.

## Mobile Redesign Plan (11200-11254)
Rebuild all existing 55 mob vnums into coherent caste ecology:

- **11200-11210:** Entry clergy, wardens, scouts, and low-rank penitents.
- **11211-11220:** Mid-tier reliquarists, duelists, ritual enforcers.
- **11221-11230:** Archive specialists (cantors, scribes, memory butchers, raven-keepers).
- **11231-11240:** Kiess-derived nobles, chamber officials, mirror retainers.
- **11241-11249:** Iron-Wake military chain (pikemen -> wardens -> castellan).
- **11250-11254 (boss set):**
  - 11250 Thorn-Crowned Castellan
  - 11251 Lady Vespera, Mirror-Queen
  - 11252 Marshal of the Iron Wake
  - 11253 The Astral Mortifier
  - 11254 Abbot Noctivar, Ebon Hierophant

Design rules:
- Maintain progression from disciplined humanoid orders into warped ritual specialists.
- Each wing should have one identity-defining elite mob with unique emotes/progs.
- Boss dialogue should reference legal memory, civic control, and broken prophecy.

## Object Redesign Plan (11200-11259)
Rebuild all 60 object vnums around tiered progression and wing identity:

- **11200-11219:** Entry-tier armor/jewelry with shadow-resist and accuracy flavors.
- **11220-11239:** Archive/mirror set (int/wis, mana, anti-illusion, lore procs).
- **11240-11249:** Bastion wargear (str/con, hit/dam, anti-fear/anti-disarm profiles).
- **11250-11256:** Boss-signature relics tied to each wing boss.
- **11257-11259:** Final triad (greatblade, warplate, oathbelt) from Noctivar line.

Itemization principles:
- Keep evocative names; rewrite descriptions to align with new lore chronology.
- Distribute class support across martial, rogue, and caster paths.
- Embed Midgaard/Kiess references into inscriptions and provenance text.

## Reset and Encounter Cadence
- Preserve deterministic wing progression with periodic patrol variance.
- Ensure each wing has:
  - 1 ambient loop reset,
  - 1 mini-elite cluster,
  - 1 gate encounter near wing end.
- Final convergence should require clearing at least two wing bosses before Noctivar opens to full power (implemented via prog state or reset substitution).

## Cross-Area Lore Integration Requirements
- **Midgaard:**
  - Explicit references to the Violet Compact, Lantern Reforms, and archive legitimacy battles.
  - Architectural signs that Heartspire once served civic evidence storage before sect capture.
- **Kiess:**
  - Echoes of Evermeet ruin-memory, noble funerary protocol, and mirror-court ritualization.
  - At least one recurring inscription naming Kiess as both heir and warning.

## Build Checklist (Strict)
- [ ] Keep all room vnums `11200-11274` present and used.
- [ ] Keep all mob vnums `11200-11254` present and reassigned.
- [ ] Keep all object vnums `11200-11259` present and reassigned.
- [ ] Maintain level band 25-40.
- [ ] Apply multi-color theme consistently with `@@N` resets.
- [ ] No thematic `@@k` usage.
- [ ] Ensure room descriptions avoid mentioning numeric vnums.
- [ ] Preserve clear progression: entry -> three wings -> convergence -> crownseat.
