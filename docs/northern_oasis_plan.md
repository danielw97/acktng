# Area Plan: The Northern Oasis (Spec-Aligned Rewrite)

## Overview

- **Area Name:** `@@lThe Northern Oasis@@N`
- **File Name:** `the_northern_oasis.are`
- **Keyword:** `the northern oasis`
- **Level Range:** `45-65`
- **Vnum Range:** `8700-8899`
- **Primary Role:** Mid-tier desert transition zone between the Eastern Desert and the greater Oasis basin, with historical ties to pyramid-era water law.

This rewrite replaces repetitive lane/patrol generation with a coherent story area: a functioning but politically strained oasis quarter where water adjudication, caravan protection, and relic trafficking intersect.

---

## Design Pillars

1. **Water is governance, not scenery.** Every district should communicate who controls flow, storage, and access.
2. **Northern Oasis is an administrative frontier.** It should feel less ceremonial than the Great Oasis and less militarized than deep desert cult zones.
3. **The area bridges living trade and dead empires.** Pyramid references are indirect (records, stonework, survey lines), not full tomb aesthetics.
4. **Room graph over corridor spam.** Use loops, optional subroutes, and role-based neighborhoods.

---

## Lore Alignment Targets

- **Great Oasis:** Northern Oasis functions as an upstream intake and arbitration station feeding the Great Oasis legal-water economy.
- **Eastern Desert:** Charter-stone, caravan law, and cistern engineering motifs continue from Eastern Desert waystations.
- **Pyramids (lesser):** Presence appears through inherited measurement doctrine, salvage inscriptions, and old sovereign-era requisition marks.
- **Midgaard (lesser):** Midgaard interests appear through survey crews, buyers, and copied tablets rather than direct governance.

---

## Area Structure (Room Topology)

Target ~100 rooms in five connected bands:

1. **North Approach Dunes (entry/combat)**
   - Caravan mile stones, wind-worn watch cairns, predatory fauna.
   - Multiple ingress paths from eastern approaches.

2. **Reedline Intake Quarter (social + utility)**
   - Wells, sluice gates, maintenance walkways, ration court.
   - Neutral-to-guarded interactions.

3. **Cairn Market and Ledger Ward (trade/civil)**
   - Contract scribes, weighing courts, bonded caravans.
   - Quest hooks tied to manifests, shortages, and forged seals.

4. **Dry Cistern Undercut (dungeon loop)**
   - Partial collapses, old Keeper tunnels, scavenger nests.
   - Primary source of relic drops and mini-boss encounters.

5. **High Adjudicator Terrace (boss + resolution)**
   - Water-court chamber, shrine to oath-stones, final conflict.
   - Outcome framing should support faction choices.

---

## Factions and Encounter Identity

- **Reedline Wardens (order faction):** enforce water quotas and anti-poison protocol.
- **Cairn Brokers (neutral-pragmatic):** control permits, storage, and caravan arbitration.
- **Sunscar Reavers (hostile):** raid shipments and extort passage in outer lanes.
- **Salt-Jackal Offshoot (limited cult presence):** small cell exploiting old cistern rites.

Boss intent:
- **Primary Boss:** Warden-Commander corrupted by scarcity politics (civil tragedy angle).
- **Secondary Boss:** Cistern scavenger-matriarch tied to old flood tunnels.

---

## Content Specification Compliance Checklist

- `#AREA` uses `Q 16`.
- Header includes `K`, `L`, `N`, `I`, `V`, `F`, `U`, and `O Virant~`.
- All strings terminate with `~`.
- No blank-line doubles in string bodies.
- No vnums in any in-world prose.
- Mobile `long_descr` is exactly one line + `~` line.
- Mobile `description` ends with exactly one newline before `~`.
- No thematic `@@k`, no background color codes (`@@0-@@7`), no `@@f`.

---

## Reset and Economy Plan

- Distinct reset clusters by district (approach, market, undercut, terrace).
- Loot emphasizes practical desert gear, contract seals, water tools, and limited relic fragments.
- Rare-table items should reference legal-water and survey traditions, not full pyramid regalia.

---

## Delivery Notes

Implementation should prioritize:
1. room rewrite first,
2. faction mobs and scripts second,
3. objectization and loot balance third,
4. exits/resets polish last.

