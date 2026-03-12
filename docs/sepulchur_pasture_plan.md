# Area Plan: Sepulchur Pasture (Full Rebuild Within Existing Vnum Envelope)

## Overview

- **Area Name:** `@@WThe @@dSepulchur @@pPasture@@N`
- **Area File (existing):** `area/sepulcher_pasture.are`
- **Keyword:** `sepulchur sepulcher pasture gravebell funerary tithe`
- **Assigned Vnum Range (must remain unchanged):** **30175-30249**
- **Target Level Band:** **15-30** (preserved)
- **Reset Rate:** 10 (preserved)
- **Owner:** `Virant` (normalize header ownership casing)

This plan fully replaces the current repetitive lane-grid presentation with a coherent funerary-agricultural district tied directly to Midgaard’s civic burial bureaucracy, Lantern Reform logistics, and Violet Compact containment doctrine.

---

## Strict Specification Commitments

1. Keep all rooms, mobiles, objects, and resets inside **30175-30249**.
2. Keep area revision and header directives compliant with `Q 16` and current area-file spec.
3. Never include vnums in in-world descriptions.
4. Use valid `@@` color tokens only; **never** use `@@k`, flashing, or background codes.
5. Keep color use intentional and sparse enough for readability.
6. Rebuild room graph, mob roster, and object catalog so they reflect one consistent lore arc rather than template repetition.

---

## Multi-Color Theme Bible: “Lantern Funeral Spectrum”

- `@@W` **Civic White**: Midgaard registry tablets, lawful process, carved decrees.
- `@@y` **Lantern Gold**: processional roads, waystones, public vigil lights.
- `@@d` **Dark Grey**: grave earth, weathered stone, ossuary walls.
- `@@p` **Purple**: oath-rites, Violet Compact seals, controlled necromancy.
- `@@a` **Light Cyan**: spirit vapor, memory wisps, sanctified ward-lines.
- `@@G` **Green**: remnant pastoral life, tithe fields, living memory of harvest.
- `@@N` **Reset**: always terminate colorized fragments.

### Theme Usage Rules

- Room titles: max two accent colors.
- Descriptions: color only on symbolic anchors (lanterns, seals, roots, tablets, bells).
- Midgaard civic references should favor `@@W` + `@@y`.
- Compact/occult pressure rooms should pivot to `@@p` + `@@a`.
- Pasture/field remnants should always include a `@@G` motif.

---

## Narrative Redirection

Sepulchur Pasture becomes Midgaard’s **outer funerary provisioning district**: historically a grain-and-cattle belt converted over generations into a burial-transit complex when wars, plague years, and dungeon attrition outpaced temple crypt capacity.

The rebuild centers on three civic functions:

1. **Receive** the dead from roads, gates, and caravans.
2. **Record** identity, debt, rank, and rite eligibility under Midgaard law.
3. **Inter or Hold** the dead in sanctified plots, warded vaults, or Compact quarantine pens.

Corruption enters where policy hardened into exploitation: tithe agents and gravewardens now prolong unrest to protect status, fees, and relic trafficking.

---

## Vnum Allocation (Complete 75-Slot Envelope)

- **Rooms:** `30175-30209` (35 rooms)
- **Mobiles:** `30210-30231` (22 mobs)
- **Objects:** `30232-30249` (18 objects)

This uses the entire existing area envelope while cleanly grouping content by type.

---

## Full Layout Redesign (35 Rooms)

## 1) Gate-Tithe Forefield (`30175-30181`)
Purpose: onboarding zone where Midgaard death-carts enter and are inspected.

- **30175** The Withered Gate (external linkage preserved)
- **30176** Cart Weighbridge of Last Burdens
- **30177** Lantern Docket Post
- **30178** Salt-Blessed Furrow Bend
- **30179** Mourners’ Queue Rail
- **30180** Gravebell Milestone (old route nexus retained as major crossroad)
- **30181** Toll of the Ninth Peal

## 2) Registry Furrows (`30182-30188`)
Purpose: legal-religious administration over remains and inheritance marks.

- **30182** Inkstone of the Unclaimed
- **30183** White Ledger Stakes
- **30184** Oath-Tablet Sluice
- **30185** Pilgrim’s Hollow (becomes clerk/scribe junction)
- **30186** Sealpress Arcade
- **30187** Tally of Ashen Sheaves
- **30188** Widow’s Furrows (reframed as claimant lane)

## 3) Reliquary Pasture (`30189-30195`)
Purpose: old farm fields converted to cenotaph plots and spirit-break hedges.

- **30189** Barrowmouth Orchard Verge
- **30190** Nightsoil Channel (retained as drainage/tithe conduit)
- **30191** Bleak Orchard of Nameposts
- **30192** Corpse-Lily Terraces
- **30193** Scarecrow Reliquary Rows
- **30194** Bellwind Irrigation Gate
- **30195** Ashen Tithe Path (elevated to subzone anchor)

## 4) Compact Holding Grounds (`30196-30202`)
Purpose: Violet Compact quarantine pens for cursed or unstable dead.

- **30196** Violet Stake Circle
- **30197** Wardwire Crossfield
- **30198** Censer Trench of Compliance
- **30199** Pen of Refused Rites
- **30200** Witness Lantern Walk
- **30201** Tribunal Cairn
- **30202** Quietus Chute

## 5) Sepulcher Core (`30203-30209`)
Purpose: climax wing with command crypt, bell tower, and release route.

- **30203** Chapel of the Last Acre
- **30204** Bone-Mill Antechamber
- **30205** Ossuary Lift House
- **30206** Bellmaster’s Gallery
- **30207** Reliquary of Deferred Names
- **30208** Heartcrypt of the Pasture
- **30209** Lantern Return Gate (unlockable shortcut/egress)

### Connectivity Rules

- Main forward spine from `30175` to `30208` with clear progression.
- Cross-links at `30180`, `30185`, `30190`, and `30201` to avoid one-way corpse runs.
- Compact wing (`30196+`) gates final core access via three legal tokens.
- At least one non-hostile retreat route from every high-danger cluster.

---

## Mobile Redesign Plan (`30210-30231`)

## Population Tiers

### Tier 1 (Lv 15-19: ambient/entry)
- toll-bell acolyte
- cart-draft mourner
- ditch lantern-keeper
- tithe crow swarmling

### Tier 2 (Lv 19-23: core pasture pressure)
- ledger-furrow clerk
- scaremask reaper
- corpse-lily groundsman
- ash-sleeved pallbearer
- gravebell drudge

### Tier 3 (Lv 23-27: specialists/elites)
- violet compact warden
- wardwire adjudicator
- oath-ink embalmer
- reliquary scytheknight
- pilgrim-claim extortioner

### Named Lore NPCs / Bosses (Lv 27-30)
- **Marshal Edrin Vale, Gate of Final Tariffs** (lawful mini-boss)
- **Sister Calvea of the Ninth Peal** (ritual caster, grief weaponization)
- **Archivist Mor Taland, Keeper of Deferred Names** (record-warden caster)
- **The Bell-Eaten Steward** (mutated final custodian)
- **Pasture Heart Wraith** (phase-two spirit boss tied to broken oaths)

### Mob Ecosystem Rules

- Each subzone has a dominant labor caste (porters, clerks, grounds, wardens, custodians).
- Mixed undead/human corruption, not pure undead spam.
- Midgaard institutional language appears in names/titles.
- Replace all generic repeated fillers with role-driven identities.

---

## Object Redesign Plan (`30232-30249`)

## Item Families

1. **Procession Implements** (lantern hooks, toll cords, mourner mantles)
2. **Registry Instruments** (sealpress signets, ash ledgers, claimant tablets)
3. **Pasture Relics** (corpse-lily charms, scaremask talismans, furrow medals)
4. **Compact Wards** (violet irons, witness censers, compliance bands)
5. **Boss Signatures** (bell-eaten crook, deferred-name reliquary shard)

## Progression / Utility Objects

- **3 legal tokens** used to open Sepulcher Core:
  - Token of Receipt
  - Token of Witness
  - Token of Interment
- **1 fused writ** assembled from the three tokens to unlock `30207 -> 30208` access.

---

## Reset and Encounter Structure

- Entry and registry zones: dense low-threat population with civic ambience.
- Reliquary fields: medium density + roaming spirit disturbances.
- Compact grounds: controlled pulls, fewer but stronger wardens/casters.
- Core: staged named encounters culminating in stewardship collapse + oath-wraith manifestation.
- Post-clear reset opens a short return path to reduce repetition.

---

## Midgaard Lore Integration Requirements

1. Explicit references to the **Lantern Reforms** in signage, route markers, and patrol habits.
2. Explicit references to **Violet Compact** containment law in compact pens and ward devices.
3. Historical tie to Midgaard’s burden as crossroads city: caravan dead, dungeon dead, militia dead.
4. Tension between Temple mercy doctrine and civic tariff bureaucracy.
5. Lore objects that mention records disputes, missing names, and forged interment claims.

---

## Implementation Checklist

- [ ] Rewrite all 35 room names and descriptions with unique identity and themed anchors.
- [ ] Replace existing repetitive road template with subzone-specific prose.
- [ ] Rebuild all mobs and objects in assigned blocks (`30210-30249`).
- [ ] Update resets to reflect new progression, key tokens, and boss chain.
- [ ] Validate all descriptions for area-spec compliance (`~`, no blank double-newline blocks, no vnum mentions).
- [ ] Validate color-code policy and readability pass.
