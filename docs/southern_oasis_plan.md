# Area Plan: The Southern Oasis (Spec-Aligned Rewrite)

## Overview

- **Area Name:** `@@lThe Southern Oasis@@N`
- **File Name:** `the_southern_oasis.are`
- **Keyword:** `the southern oasis`
- **Level Range:** `50-70`
- **Vnum Range:** `9800-9999`
- **Primary Role:** Upper-mid desert metropolis edge where oasis civilization meets pyramid corridor pressure.

This rewrite retires the current repetitive “future pyramid road” lane pattern and rebuilds the area as an active civic-military basin linked to southern pyramid expeditions and regional trade.

---

## Design Pillars

1. **Living city-edge oasis, not a patrol template.** Districts need social, logistical, and ritual variance.
2. **Southward pressure from pyramid frontier.** The area should feel like a staging ground for dangerous deeper content.
3. **Water and law are contested institutions.** The conflict is political first, then violent.
4. **Readable traversal.** Main spine + lateral loops + optional hazard underroutes.

---

## Lore Alignment Targets

- **Great Oasis:** Southern Oasis is the militarized sister-node responsible for convoy hardening and ration enforcement.
- **Eastern Desert:** Receives caravans and legal traditions from eastern charter roads.
- **Pyramids (stronger than Northern Oasis but still secondary):** Strong logistical connection to southern pyramid expeditions; avoid full tomb takeover of area identity.
- **Midgaard:** Present via salvage contracts, scholars, and market demand for authenticated relics.

---

## Area Structure (Room Topology)

Target ~100 rooms in six district sets:

1. **Lantern Causeway (entry spine)**
   - Convoy choke points, permit checks, caravan camp edges.

2. **Canopy Bazaar (social/economic hub)**
   - Brokers, quartermasters, relic appraisers, dispute clerks.

3. **Sluice Bastion (military-water infrastructure)**
   - Gatehouses, pump works, ration locks, defensive terraces.

4. **Embalmer’s Yard Annex (lore danger pocket)**
   - Former preparation quarter repurposed for expedition dead.
   - Introduces pyramid funerary bleed-through without becoming a tomb zone.

5. **Sub-Basin Tunnels (hazard loop)**
   - Smuggler channels, sink chambers, hostile fauna/cult ambushes.

6. **South Survey Gate (boss and transition)**
   - Forward command for pyramid-road claims and legal jurisdiction conflict.

---

## Factions and Encounter Identity

- **Bastion Wardens:** state-like water soldiers and convoy marshals.
- **Bazaar Syndics:** permit and tariff guilds, politically flexible.
- **Expedition Mortuary Circle:** ritual specialists handling pyramid casualties.
- **Jackal Tribunal Splinter:** manipulates legal language and death rites for control.

Boss intent:
- **Primary Boss:** Survey Gate Marshal enforcing extreme ration doctrine.
- **Secondary Boss:** Tribunal Splinter adjudicator using curse-verdict mechanics.

---

## Content Specification Compliance Checklist

- `#AREA` with mandatory `Q 16`.
- Preserve envelope (`I 50 70`, `V 9800 9999`) unless reauthorized globally.
- Enforce canonical header owner as `O Virant~`.
- `~`-terminated strings everywhere.
- No blank-line doubles in strings.
- No vnums in in-world text.
- Mobile `long_descr` and `description` formatting compliant.
- Color policy respected (`@@k` excluded; no flashing/background codes).

---

## Reset and Progression Plan

- Early rooms: mixed social + light combat.
- Mid zones: faction conflict and key/permit gating.
- Underroutes: higher-risk optional loot and shortcut loops.
- Finale: legal-military showdown framing access toward pyramid-oriented content.

Loot profile:
- Water-law insignia, convoy gear, anti-heat kit, authenticated relic fragments.
- Controlled drop rate for pyramid-linked artifacts to avoid overshadowing dedicated pyramid areas.

