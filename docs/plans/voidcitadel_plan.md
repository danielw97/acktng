# Area Plan: The Void Citadel of Kel'Shadra (Full Reauthor)

## Overview

- **Area Name:** `@@dThe @@bVoid @@pCitadel @@dof @@pKel'Shadra@@N`
- **Area File:** `area/voidcitadel.are` (existing area, complete redesign)
- **Assigned Vnum Range (preserved):** **88-999**
- **Total Room Capacity in Range:** **390 rooms**
- **Target Level Band:** 150-5
- **Reset Rate:** 15
- **Owner (spec-compliant target):** `Virant`
- **Design Mandate:** Rebuild the entire area layout, object ecology, and mobile roster to align with modern Kel'Shadra + Gloamvault continuity while using every room in the assigned envelope.

---

## Strict Area-Spec Compliance Requirements

1. Preserve area envelope exactly: `V 88 999`.
2. Keep all authored rooms, objects, and mobiles inside `88-999`.
3. Use `Q 16` and normalize owner header to `O Virant~`.
4. No vnums in in-world prose.
5. No blank lines inside `~`-terminated string bodies.
6. Keep mobile `long_descr` single-line format exactly.
7. Keep mobile `description` with exactly one trailing newline before `~`.
8. Use intentional ACK color coding; do not use `@@k`, background color codes, or flashing.
9. Maintain canonical section integrity and terminators (`#0`, `#$`).

---

## Multi-Color Theme Bible (Void Citadel)

The Citadel is written as a **tri-axis institution under collapse**: void pressure, liturgical authority, and mnemonic residue.

- `@@d` — Basalt, age, burdened stone, ossuary mass
- `@@b` — Void pressure, astral fracture, non-light geometry
- `@@p` — Office, vow-law, authority seals, matriarchal decree
- `@@m` — Active rupture, blood-cost rites, combat escalation
- `@@a` — Memory echoes, surviving witness fragments, release cues
- `@@W` — Pre-corruption doctrine, unredacted truth fragments
- `@@N` — reset

### Theme Rules

- Room names: max 2 accent colors.
- Directional cues in major hubs use `@@b` + `@@p` pairing.
- Boss telegraph text uses `@@m` accents.
- Mercy/release content uses `@@a` accents.
- Unredacted archival content uses `@@W` sparingly.

---

## Narrative Intent

The Void Citadel is reframed as the **high-altitude and deep-void enforcement wing** of Kel'Shadra’s memory regime. If the Gloamvault adjudicated testimony and the Crypts finalized redaction, the Citadel operationalized **extraction, projection, and recurrence**:

1. **Extraction** — draw testimony from body/soul/memory under duress.
2. **Projection** — imprint authorized narratives into institutional surfaces.
3. **Recurrence** — rerun doctrinal outcomes until resistance collapses.

Players move through a functioning dead bureaucracy where ritual machinery still processes “cases,” even though the legal order that authorized those cases is gone.

---

## Full Layout Rebuild (Use All Rooms: 88-999)

### Zone A — Blighted Gate Precinct (88-117, 30 rooms)
- Rebuild current gate/courtyard/hall cluster into a full arrival district.
- Includes breach ward, intake checkpoints, ash registry vestibule, and first Bell/Veil iconography.
- Tutorialized elite-endgame messaging: doors, warning plaques, and failure consequences.

### Zone B — The Labyrinth of False Witness (118-147, 30 rooms)
- Keep identity of existing maze but redesign into readable institutional maze.
- Structured loops, mirrored testimony cells, and deliberate dead-end lore rewards.
- Convert all dead-end rooms into either tactical traps, cache rooms, or witness fragments.

### Zone C — Sanctum Threshold & Audit Ring (148-177, 30 rooms)
- Rebuild sanctum approach, proving chambers, and command offices.
- Includes tri-sign progression courts and Dusk-Auditor control nodes.
- Ends at a fork into Catacomb Engine vs. Spire Lift.

### Zone D — Catacomb Compression Engines (178-207, 30 rooms)
- Expand catacomb path into full logistics/processing district.
- Bone road + chain halls become throughput pipelines for condemned records.
- Mid-boss: Lord of the Undercroft (custodian of forced continuity).

### Zone E — Null Spire Ascendant (208-227, 20 rooms)
- Vertical sequence that stabilizes and weaponizes void pressure.
- Moving from base to summit shifts audio, gravity language, and memory distortion cues.
- Includes optional restorative lore nodes tied to Hesparine-era doctrine remnants.

### Zone F — Eternal Corridor Recurrence Lattice (228-277, 50 rooms)
- Rebuild corridor wing as a recursive case-processing maze.
- Each loop family encodes one historical case category (heresy, inheritance, war crimes, oath fraud, null identity).
- Culminates in Warden Hall + Loop Arbiter encounter.

### Zone G — Heraldic Crucible (278-849, 50 rooms)
- Expand current Herald rooms into a full ten-office command district.
- “Hall of the Ten” becomes radial structure with ten mini-wings and doctrine tests.
- Boss chain: Void Herald remnant, Tenfold Witness Composite.

### Zone H — Hidden Memory Annexes (282-899, 50 rooms)
- Existing secret rooms become a full clandestine archive and resistance substrate.
- Includes dissident scribes’ routes, unredacted ledgers, and suppressed personnel files.
- Primary lore reward district linking directly to Gloamvault and deep Kel'Shadra schism records.

### Zone I — Void-Gate Consequence Core (286-949, 50 rooms)
- Rebuild gate + observation cluster into endgame cosmological mechanism.
- Includes gate calibration rings, seal pylons, and authority throne array.
- Final confrontation with **The Citadel Recursor** (institutional self-defense intelligence).

### Zone J — Aftermath, Verdict, and Exit Strata (289-999, 50 rooms)
- Replace short aftermath with large consequence wing.
- Two post-boss branches:
 - **Seal Continuity Route:** preserve containment; stabilize dangerous recurrence.
 - **Witness Release Route:** break recurrence; release partitioned witness-fragments.
- Rejoin into shared extraction and epilogue chambers.

**Total rooms:** 30 + 30 + 30 + 30 + 20 + 50 + 50 + 50 + 50 + 50 = **390** (all room vnums used).

---

## Reauthored Mob Ecosystem Plan (88-999)

All existing mobs are rewritten around institutional castes and expanded in-range.

### Caste Families

1. **Gate Functionaries (88-117)**
  - Ash Registrars, Bell Marshals, Veil Sentinels.
  - Role: control ingress/egress and assign classification tags.

2. **Witness Predators (118-177)**
  - False Witness Hounds, Mirror Bailiffs, Recitation Stalkers.
  - Role: punish contradiction and force testimony loops.

3. **Ossuary Executors (178-207)**
  - Bone Compressors, Chain Gendarmes, Undercroft Captains.
  - Role: enforce terminal archival outcomes.

4. **Spire Instruments (208-227)**
  - Null Conductors, Pressure Adepts, Summit Wardens.
  - Role: maintain void-field harmonics.

5. **Corridor Recursors (228-277)**
  - Loop Clerks, Docket Revenants, Iteration Judges.
  - Role: replay unresolved verdicts.

6. **Heraldic Officers (278-849)**
  - Tenfold Emissaries, Oath Heralds, Office Inquisitors.
  - Role: execute top-tier doctrinal authority.

7. **Annex Dissidents and Remnants (282-899)**
  - Shade Scribes, Unbound Witness Fragments, Defected Handmaidens.
  - Role: reveal hidden institutional history.

8. **Core Singulars (286-999)**
  - Gate Custodians, Pylon Choirs, Recursor Avatar.
  - Role: final systems defense and consequence logic.

### Encounter Philosophy

- Replace generic undead spam with role-specific institutions.
- Elite mobs should communicate office identity through title and behavior.
- Bosses use stage language tied to doctrine verbs: Extraction / Projection / Recurrence.

---

## Object Reauthor Plan (88-999)

### Object Families

- **Office Regalia:** seals, censers, bell-gongs, veil-signets.
- **Enforcement Gear:** chain-blades, blackglass bastions, oath-hook polearms.
- **Witness Artifacts:** redaction tablets, unsealed folios, memory ampoules.
- **Void Instruments:** harmonic rods, pylon keys, null compass arrays.
- **Post-Boss Consequence Items:** continuity sigils and release sigils for alternate reward identity.

### Mechanical Notes

- Ensure loot objects tied to new mobs are flagged appropriately for loot systems.
- Distribute resist/utility gear across all major sub-districts so progression is spatially fair.
- Endgame set pieces split across Zone F / G / I to prevent single-wing farm dominance.

---

## Reset and Flow Strategy

- Use layered resets to create patrol overlap in institutional hubs.
- Keep one readable primary progression route plus optional lore loops in every zone.
- Telegraph lock states via icon language (Bell / Veil / Chain / Null Gate).
- Secret-route resets should support lore discovery rather than pure loot bypass.

---

## Lore Integration Anchors (from Gloamvault + Kel'Shadra)

Embed the following continuity points throughout room text, mob dialogue, and object flavor:

1. **Night of Unsounded Bells** as system-collapse catalyst.
2. **Shift from Last Witness to Last Authority** under Velastran consolidation.
3. **Bell Synod / Ash Scriptorium / Reliquary Host inheritance conflict.**
4. **Deep redaction apparatus and utility indexing** as moral corruption engine.
5. **Unbound Witness fragments** as evidence that redaction failed.
6. **Interface with Crypts and Gloamvault** as sequential bureaucratic filters.

---

## Implementation Sequence

1. Rebuild area header compliance and vnum map ledger.
2. Author room matrix by zone (390 total), connectivity first.
3. Draft mobile roster by caste and assign patrol ecosystems.
4. Author object families and distribute per district economy.
5. Script resets for path readability + encounter pacing.
6. Run strict format/spec validation pass (strings, terminators, descriptors, section order).
7. Lore polish pass: ensure tone, color consistency, and cross-area references.

---

## Content Quality Checklist

- [ ] Every room vnum in `88-999` is authored and reachable.
- [ ] No in-world text mentions vnums.
- [ ] Color usage follows theme policy and forbidden-code constraints.
- [ ] Mobile descriptor formats are fully spec-valid.
- [ ] Narrative continuity with `gloamvault_lore.md` and `kelshadra_lore.md` is explicit.
- [ ] End-state supports both continuity and release interpretations.
