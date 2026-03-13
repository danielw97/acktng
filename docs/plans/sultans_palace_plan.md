# Area Plan: The Sultan's Palace (Full Redesign)

## Overview

**Area Name:** `@@cThe Sultan's Palace@@N`  
**File Name:** `sultans_palace.are`  
**Intended Level Range:** 30-50 (`I 30 50`)  
**Vnum Range (fixed):** **30325-30399** (`V 30325 30399`)  
**Room Count Target:** **75/75 rooms used**  
**Mob Count Target:** 75/75 mobile vnums used (re-authored roster)  
**Object Count Target:** 75/75 object vnums used (re-authored itemization)

### Redesign Goal

Rebuild the entire area as the drowned court-palace of **Sultan Kheir al-Zafir**, the last Water Regent of the Deepwell Confluence period, whose court tried to fuse oasis water-law with pyramid funerary sovereignty after the Ninth Meridian collapse. The current "haunted gothic" palette remains, but is reframed as a coherent political-religious ruin:

- The palace is a former **Water Court and Oath Archive** linked to Great Oasis legal traditions.
- Its lower halls were converted into **mortuary-administrative vaults** tied to southern pyramid funerary doctrine.
- Its upper pavilions hold unstable moonlit ritual engines that mirror theological pressure from the pyramid arc.

The redesign preserves the assigned vnum envelope while replacing all rooms, mobiles, and objects with lore-anchored content.

---

## Hard Constraints from `/docs` Specs

- Keep header and records compliant with `Q 16` area format requirements.
- Keep all authored content inside `30325-30399`.
- Use `O Virant~` in final area header authoring.
- All strings `~`-terminated; no blank-line double newlines in string payloads.
- Never mention vnums in any in-world text.
- Enforce mobile `long_descr`/`description` formatting rules exactly per spec.
- Apply in-string colors intentionally; do **not** use `@@k`, flashing, or background codes.

---

## Multi-Color Theme (Lore-Matched)

The palace uses a five-color identity that aligns with the oasis-pyramid corridor:

- **Turquoise (`@@c`)** — living spring-water authority, Water Court legitimacy, old oasis law.
- **Azure (`@@a`)** — moonlit canal light, drowned memory, ritual illumination.
- **Sun-gold (`@@y`)** — Sultanate regalia, tax seals, charter legacy from road empires.
- **Funerary purple (`@@p`)** — tribunal rites, tomb protocol, death-jurisdiction doctrine.
- **Verdigris green (`@@G`)** — oxidized bronze gates, age, stagnant reservoirs, decay overtaking order.
- **Threat red (`@@R`)** — usurpation, blood-oaths, palace-guard hostility, boss encounter signaling.

Reset with `@@N` after thematic phrases.

---

## Lore Integration Targets

This redesign directly integrates motifs established in oasis/pyramid planning:

1. **Deepwell Confluence continuity**: palace cisterns are fed by the same aquifer system that sustains the three oases.
2. **Measure-script governance**: charter tablets, oath-bowls, and calibrated sluice architecture mirror Great Oasis legal-water culture.
3. **Ninth Meridian aftermath**: declining flow forced legal courts to merge with funerary authority.
4. **North→South institutional gradient**: upper palace remains administrative; lower palace transitions into death-saturated jurisdiction spaces tied to pyramid ideology.
5. **Memory politics**: inscriptions are overwritten in layers (Keeper law -> Sultanate decrees -> cult corrections), reflecting contested historical truth.

---

## Area Structure and Full Room Usage (75/75)

The area is fully re-zoned into five 15-room bands, using every room vnum once.

## Zone A — Gate of Petitions (30325-30339, 15 rooms)
**Function:** public-facing palace threshold, toll petitions, oath verification, caravan adjudication.  
**Mood:** formal, crowded-turned-abandoned, still watched by protocol-bound remnants.

- **30325** Moonlit Antechamber (entry nexus; exterior tie retained).
- **30326** Veiled Gallery (central splitter toward all major wings).
- **30327-30331** Petition colonnades and registrar corridors.
- **30332-30335** Seal court, witness alcoves, charter wall.
- **30336-30339** Intake stair, oath basin, archive queue lanes.

### Zone A encounter profile
- Mobs: petition ghosts, oath-scribes, rusted gate wardens, ledger jackals.
- Objects: petition tablets, seal-stamps, brass oath bowls, water tokens.

---

## Zone B — Court of Measured Waters (30340-30354, 15 rooms)
**Function:** hydraulic-administrative heart of palace governance.  
**Mood:** cool stone, mirrored channels, law encoded as flow control.

- **30340** Amber Cloister (transition from intake to court interior).
- **30341-30345** Sluice observatory walk, gauge chambers, legal aqueduct.
- **30346-30349** Water court hearing halls and arbitration dais.
- **30350-30353** Seal vault galleries and tribute valuation rooms.
- **30354** Throne of Reeds and Copper (zone mini-boss room).

### Zone B encounter profile
- Mobs: canal adjudicators, bronze veil guards, aquifer auditors, court herald revenants.
- Objects: measured staves, copper decree plates, reed-crown relics, tribunal balances.

---

## Zone C — Sultan's Domestic Crescent (30355-30369, 15 rooms)
**Function:** private palace life, harem politics, steward wings, diplomatic receiving salons.  
**Mood:** decadent luxury hollowed by paranoia.

- **30355** Cedar Reception Court (zone entry from hub).
- **30356-30360** Perfumed gallery chain, lunar tea rooms, mirror antechambers.
- **30361-30364** Silk dormitories, eunuch posts, handmaiden cloisters.
- **30365-30368** Diplomatic moon loggias and private strategy salons.
- **30369** The Sleepless Bedchamber (named elite encounter).

### Zone C encounter profile
- Mobs: moonbound chamberlains, silk oath-sisters, poisoned cupbearers, jealous spirit courtiers.
- Objects: silk sigil veils, narcotic incense caskets, mirror charms, envoy lockboxes.

---

## Zone D — Necro-Archives Beneath the Court (30370-30384, 15 rooms)
**Function:** legal-funerary merger layer where records and remains are jointly governed.  
**Mood:** compressed air, incense ash, judicial fear.

- **30370** Spiral Descent to the Quiet Registry.
- **30371-30374** Bone index halls and mortuary record stacks.
- **30375-30378** Chamber of Corrected Names, debt ossuary, sentence crypt.
- **30379-30382** Salt-vault reliquary, embalmer tribunal, witness catacomb.
- **30383-30384** Blackwater lock and judge-wraith antechamber.

### Zone D encounter profile
- Mobs: archivist mummies, sentence-ward bailiffs, parchment wraiths, oath-breaker shades.
- Objects: funerary writs, canopic seals, bone styluses, salt-ledger codices.

---

## Zone E — Moon-Cistern Apex and Throne Collapse (30385-30399, 15 rooms)
**Function:** final convergence of water-law and death-sovereignty; climactic ritual machinery.  
**Mood:** unstable moonlight, flooded mechanics, dynastic catastrophe.

- **30385** Ascendant Cistern Lift (re-emergence into upper sanctum heights).
- **30386-30390** Lunar pressure halls, cracked observatory bridges, flood vents.
- **30391-30394** Astral decree vault, drowned minaret interior, broken regnal chapel.
- **30395-30398** Final gauntlet: mirror-sluice maze, oathfire choir, throne spillway.
- **30399** The Drowned Sultan's Diadem Throne (final boss room).

### Zone E encounter profile
- Mobs: lunar vizier phantoms, drowned janissary elites, moon-engine custodians.
- Objects: diadem shards, cistern keys, regnal astrolabes, high-court relic drops.

---

## Mobile Roster Redo (75 vnums)

All mob vnums `30325-30399` are reassigned into coherent factions:

- **Court Protocol Remnants (30325-30339)**: low-mid difficulty administrative undead/constructs.
- **Hydraulic Tribunal Corps (30340-30354)**: disciplined defenders with control utility.
- **Domestic Intrigue Circle (30355-30369)**: debuff/control specialists and assassins.
- **Necro-Archive Judiciary (30370-30384)**: attrition-heavy casters, curse inflictors, anti-recall pressure.
- **Diadem Catastrophe Guard (30385-30399)**: high-threat elites, culminating in final ruler encounter.

Named anchors:
- **Mini-boss (30354):** Regent-Adjutor of the Measured Court.
- **Elite narrative anchor (30369):** Sleepless Consort of the Moon Pavilion.
- **Sub-boss (30384):** Warden-Judge of Corrected Names.
- **Final boss (30399):** Sultan Kheir al-Zafir, Drowned in Oathlight.

---

## Objectization Redo (75 vnums)

All object vnums `30325-30399` are repurposed across progression tiers:

1. **Administrative relic set** (tablets, seals, bowls, insignia).
2. **Hydraulic implements** (sluice keys, gauge rods, copper gate tools).
3. **Courtly equipment** (veils, curved blades, regalia, jewelwork).
4. **Funerary-judicial kit** (codices, sentence bands, ash censers).
5. **Boss-tier artifacts** (diadem shards, moon astrolabe, oathfire focus).

Design intent:
- Blend caster/melee rewards for level 30-50 progression.
- Use loreful item descriptions that reference water-law, memory correction, and dynastic collapse.
- Reserve strongest pieces for 30384 and 30399 encounter loops.

---

## Reset and Encounter Flow Principles

- Rebuild resets so each zone feels inhabited by one dominant faction with local patrol overlap.
- Keep social spaces populated by non-aggro/lore mobs; reserve hard aggro for restricted wings.
- Use container/object resets to stage evidence of legal process decaying into necro-theocracy.
- Gate final throne approach with key progression from Zone B + Zone D objectives.

---

## Connectivity and Navigation Principles

- Maintain bi-directional exits and avoid non-maze directional loops.
- Keep `30326` as central navigational hub to mirror current player muscle memory.
- Preserve at least one clean return path from each deep branch to reduce dead-end frustration.
- Distinguish traversal modes by architecture:
  - Public/legal halls = broad axial movement.
  - Domestic crescent = branching side loops.
  - Necro-archives = layered descent and choke points.
  - Apex sanctum = linear escalation into boss.

---

## Narrative Arc for Players

1. **Arrival:** palace appears ceremonial but wrong; petitions continue without petitioners.
2. **Discovery:** water-law systems still operate, but judgments now feed funerary machinery.
3. **Revelation:** sultan fused legal legitimacy with death jurisdiction to survive aquifer decline.
4. **Descent/Ascent:** players move through proof archives, then into ritual superstructure.
5. **Resolution:** defeat of Drowned Sultan disrupts coercive oathlight, allowing competing oasis traditions to reclaim interpretation of law.

---

## Deliverables in Build Phase

- Fully rewritten room set: 75/75 rooms using vnums 30325-30399.
- Fully rewritten mobiles: 75/75 mobiles using vnums 30325-30399.
- Fully rewritten objects: 75/75 objects using vnums 30325-30399.
- Lore-consistent reset graph and boss progression.
- Color-consistent strings aligned with this plan's five-color theme.
