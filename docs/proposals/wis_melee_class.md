# Proposal: WIS-Based Mortal Melee Class — The Sentinel

## Overview

This proposal introduces the **Sentinel**, a new WIS-based mortal melee class
that fills a structural gap in the class table: WIS is the only primary stat
with no mortal-tier melee option.

Currently, every primary stat except WIS has a mortal melee class:

| Stat | Mortal Melee | Mortal Caster |
|------|-------------|---------------|
| STR  | Warden      | —             |
| DEX  | Cipher      | —             |
| CON  | Pugilist    | —             |
| INT  | —           | Magi, Psionicist |
| WIS  | **none**    | Cleric        |

A WIS-based player who wants to fight in melee must start as a Cleric
(WIS caster, 3 HP/level, mana-dependent) and reach remort before accessing
Paladin (8 HP/level, WIS melee). This means WIS melee is locked behind an
entire mortal career of caster gameplay — a tax no other stat pays. STR
players start as Wardens. DEX players start as Ciphers. CON players start as
Pugilists. WIS melee players start as healers and wait.

The Sentinel addresses this by offering a mortal melee path that uses WIS as
its prime stat, giving WIS-race players (Khenari, Deltari, Ushabti, Kethari)
a melee option from level 1.

---

## Lore Foundation: The Jackal Tribunal's Enforcement Arm

### The Solar Court's Fifth Office

The **Jackal Tribunal** was one of the Solar Court's five priestly offices,
responsible for adjudicating the dead's accounts and enforcing sentences. The
Tribunal operated in epistemically difficult conditions — the Black Sun
Shard's memory-corrupting effects made testimony unreliable — and developed
sophisticated systems for testimonial cross-reference and record
reconciliation.

The Tribunal issued three sentence classes: **Interment** (memory retained,
body sealed), **Service** (memory compressed, spirit bound to labor), and
**Correction** (memory rewritten for political continuity). Executing these
sentences required physical enforcement — not the advance-and-sanctify
combat of the temple-guard auxiliaries (who became Paladins and Templars),
but the deliberate, measured application of force by practitioners trained
to observe, evaluate, and act with judicial precision.

### The Sentence Enforcers

The existing lore documents two survival paths from the Jackal Tribunal's
collapse:

1. The **Jackal Synod** — degraded militant toll-priests in the Eastern
   Desert who retained ritual vocabulary but lost theological depth.
2. **Independent judicial ethics lineages** — practitioners who survived
   as "independent judicial ethics lineages" (Cleric lore, Section I).

The Sentinel tradition descends from this second path: the Tribunal's
**sentence enforcers** who carried out verdicts through disciplined physical
intervention rather than priestly magic. Where the Synod devolved into
extortion, and the Cleric tradition absorbed the Tribunal's intellectual
methods, the sentence enforcers preserved the Tribunal's **embodied
practice** — the reading of subjects, the measured application of
restraining force, the capacity to end a confrontation through precise
physical action informed by deep observation.

### Why WIS, Not STR

The Sentinel does not fight through raw physical power. The tradition's
combat methodology descends from the Tribunal's core competency:
**judgment under uncertainty**. The Black Sun Shard's memory-corruption
meant that the Tribunal could never trust a single source of information —
they developed the habit of reading multiple simultaneous signals (posture,
breath, micro-expressions, weapon angle, footing) and synthesizing them into
a predictive model of what the subject would do next.

This is WIS expressed as combat awareness: not the intelligence to devise a
clever strategy, not the strength to overpower, but the perceptive depth to
read an opponent's intent before it becomes action and position oneself
accordingly. The silence-court training that shaped the Khenari — acoustic
deprivation, chant intervals, minds organized against noise and distraction
— is the same training that produces a fighter who reads a battlefield the
way a jurist-priest reads testimony.

### The Khenari Connection

The Khenari trace their lineage to the **Jackal Balance Court** of Akh'enet,
which fused debt adjudication with funerary sentence rites. Khenari lore
explicitly states they are "not warriors" — they are "judges, scribes, and
psychopomps" whose physical frailty (STR 12) is offset by extraordinary
perceptive capacity (WIS 22).

The Sentinel tradition provides the missing piece: a martial discipline that
*uses* WIS 22 as its combat stat, allowing Khenari to fight not despite
their nature but through it. A Khenari Sentinel fights the way a Khenari
does everything else — through observation, record, and verdict — but the
verdict is delivered with a weapon instead of a ledger.

This does not contradict the Khenari identity as non-warriors. It extends
it. A Sentinel is not a warrior in the Warden sense (overwhelming force) or
the Cipher sense (exploiting openings). A Sentinel is a practitioner of
judicial enforcement who has learned that some verdicts require physical
delivery.

### Relationship to the Existing WIS Line

The Sentinel occupies a distinct narrative space from the Cleric → Paladin →
Priest → Templar lineage:

| Tradition | Origin | Method | Relationship to WIS |
|-----------|--------|--------|---------------------|
| Cleric | Dawn Liturgists + Seal College | Faith as infrastructure maintenance | WIS as cosmic precision |
| Paladin | Temple-guard auxiliaries + Sundering Wars | Oath-bound advance-and-sanctify | WIS as covenant authority |
| Priest | Hour of Quiet Thunder schism | Sustained restoration | WIS as healing depth |
| Templar | Garrison crisis synthesis | Dual-function combat + healing | WIS as integrated service |
| **Sentinel** | **Jackal Tribunal sentence enforcers** | **Observation, reading, verdict** | **WIS as perceptive judgment** |

The Sentinel shares the Solar Court ancestry but descends from a different
office. The Cleric line inherits from the Dawn Liturgists and Seal College;
the Sentinel line inherits from the Jackal Tribunal. Both use WIS, but for
fundamentally different purposes: the Cleric line uses WIS to maintain and
heal; the Sentinel uses WIS to perceive and act.

---

## Class Design

### Mortal Tier

```
{"Sen", "Sentinel", APPLY_WIS, 6, 1, MORTAL, {-1, -1}},
```

| Field | Value | Rationale |
|-------|-------|-----------|
| Prime stat | WIS | Judicial-perceptive combat tradition |
| HP/level | 6 | Melee-capable but not a tank — between Cleric (3) and Warden (7). Reflects a tradition that avoids damage through reading rather than absorbing it |
| Mana/level | 1 | Primarily physical; minimal mana for a small number of WIS-powered observation abilities |
| Tier | MORTAL | Available at character creation |

### Combat Identity

The Sentinel's mechanical identity should emphasize **reactive precision**
over raw output:

- **Observation-based defense**: WIS contributes to dodge/parry through
  reading opponent intent, not through raw speed (DEX) or armor absorption
  (CON/STR). The tradition descends from silence-court training where
  reading hidden signals was survival.

- **Counter-striking**: The Sentinel excels at responding to enemy actions
  rather than initiating — reflecting the Tribunal's methodology of
  observing before acting, waiting for the opponent to reveal themselves
  before delivering the verdict.

- **Debuff through insight**: A small number of WIS-scaled abilities that
  represent the Sentinel reading and exploiting an opponent's weaknesses —
  identifying an injury, a tell, a pattern — rather than raw magical
  debuffing. These should feel like combat observations, not spells.

- **Single-target focus**: The Tribunal adjudicated individual cases, not
  crowds. The Sentinel should be strongest in sustained single-target
  combat, reflecting the judicial tradition of focused evaluation and
  measured response.

### Suggested Mortal Skills

These are thematic suggestions for the mortal skill set. Exact level
assignments and balance numbers require implementation testing.

| Skill | Type | Description | Thematic Source |
|-------|------|-------------|-----------------|
| **Verdict Strike** | Active (offensive) | A single precise melee strike with WIS-scaled bonus damage. Represents the delivery of judgment through physical force. | Tribunal sentence enforcement |
| **Read Opponent** | Active (self-buff) | Short-duration buff that increases dodge/parry against the current target based on WIS. The Sentinel studies the opponent's patterns. | Silence-court testimony cross-reference |
| **Measured Response** | Passive | Chance to counter-attack after a successful dodge or parry. WIS-scaled proc rate. | Tribunal methodology: observe, then act |
| **Testimonial Guard** | Passive | Resistance to fear, charm, and other mental effects. WIS-scaled. | Silence-court mind-discipline training |
| **Binding Strike** | Active (offensive + debuff) | Melee attack that reduces the target's attack speed. The Sentinel identifies and strikes a structural weakness. | Tribunal restraint and containment |
| **Ninth Descent** | Active (self-buff) | Brief period of heightened combat awareness — increased critical chance and dodge. Represents the Sentinel entering the focused perceptive state of the silence-court descent ritual. | Khenari Doctrine of Ninth Descent |

### Weapon Proficiencies

The Sentinel should favor weapons associated with judicial and ceremonial
enforcement rather than battlefield warfare:

- **Maces and flails** — traditional instruments of judgment and
  enforcement, consistent with the Cleric-adjacent WIS tradition
- **Polearms (staves, halberds)** — threshold-holding weapons, consistent
  with the Tribunal's role as boundary enforcers
- **No two-handed swords or axes** — these are Warden/Knight battlefield
  weapons that do not fit the judicial enforcement identity

---

## Remort Paths

The Sentinel enables a new remort branch that complements but does not
overlap with the existing Paladin/Priest paths from Cleric:

### Option A: Sentinel → Justicar + Arbiter

```
Sentinel (mortal, WIS melee)
  ├── Justicar (remort, WIS melee-offense)
  └── Arbiter  (remort, WIS melee-defense/utility)

Justicar + Arbiter → Inquisitor (adept, WIS melee synthesis)
```

**Justicar** — The Tribunal's sentence executors. Offense-focused WIS
melee: enhanced verdict strikes, WIS-scaled physical damage bonuses, the
ability to mark targets for amplified damage (the formal "sentencing" of a
target in combat). Growth: ~12 HP / 2 Mana per level.

**Arbiter** — The Tribunal's record-keepers and process guardians.
Defense/utility-focused WIS melee: enhanced counter-attacks, party-wide
defensive auras from reading enemy formations, the ability to suppress enemy
abilities through WIS-scaled debuffs. Growth: ~10 HP / 4 Mana per level.

**Inquisitor** (adept) — The synthesis: a WIS melee practitioner who can
both sentence (amplified single-target damage) and arbitrate (suppress enemy
capabilities), reflecting the Jackal Tribunal's full judicial authority —
not just verdict delivery but the entire process of investigation,
adjudication, and enforcement in a single practitioner. Growth: ~20 HP / 6
Mana per level.

### Option B: Sentinel → Two Existing Remort Slots

If expanding the class table beyond 24 classes is undesirable, the Sentinel
could feed into the existing Paladin and/or Priest remort paths as an
alternative mortal prerequisite. This would mean:

- Sentinel → Paladin (WIS melee → WIS oath-vanguard, a natural progression)
- Sentinel → Priest (WIS melee → WIS restoration, a less natural but
  narratively defensible pivot — the enforcer who turns to healing)

This approach requires no new remort or adept classes but limits the design
space. Option A is recommended if the class table can be expanded.

---

## Impact on Game Balance

### Stat Line Competition

WIS 22 races that benefit most from a mortal WIS melee class:

| Race | STR | INT | WIS | DEX | CON | Current Best Mortal | Sentinel Fit |
|------|-----|-----|-----|-----|-----|---------------------|--------------|
| Khenari | 12 | 18 | 22 | 16 | 16 | Cleric (caster) | Strong — WIS 22 as combat stat, lore alignment |
| Deltari | 12 | 18 | 22 | 18 | 14 | Cleric (caster) | Strong — WIS 22, good DEX for dodge synergy |
| Ushabti | 22 | 12 | 22 | 10 | 18 | Warden (STR melee) | Moderate — already has STR 22 for Warden; Sentinel offers an alternative identity |
| Kethari | 12 | 14 | 22 | 14 | 22 | Pugilist (CON melee) or Cleric | Strong — WIS 22 + CON 22, could be very durable Sentinel |

The Sentinel does not obsolete any existing mortal class. Cleric remains
the superior choice for players who want to heal or cast offensively.
Warden remains superior for raw physical damage. The Sentinel occupies the
specific niche of WIS-based melee that currently has no mortal-tier option.

### HP/Mana Balance

At 6 HP / 1 Mana per level, the Sentinel is:

- **Tougher than**: Cleric (3/4), Magi (2/5), Psionicist (3/4)
- **Comparable to**: Cipher (6/0)
- **Softer than**: Warden (7/0), Pugilist (7/0)

This positions the Sentinel as a medium-durability melee class that survives
through WIS-based avoidance rather than raw HP, similar to how the Cipher
survives through DEX-based evasion rather than the Warden's HP pool.

### Damage Output

The Sentinel should deal moderate sustained damage with occasional burst
from Verdict Strike. WIS-scaled damage should be competitive with but not
exceed STR-scaled damage at equivalent levels — the Sentinel's advantage is
defensive reading and counter-striking, not raw output.

---

## Implementation Scope

### Class Table Changes

1. Add `CLASS_SEN` constant to `typedefs.h` (index 24 if appended, or
   index 6 if inserted after Pugilist — insertion requires renumbering
   all subsequent class indices)
2. Add Sentinel entry to `gclass_table[]` in `const_class.c`
3. Increment `MAX_TOTAL_CLASS` (currently 24 → 25, or more if remort
   paths are included)
4. Add Sentinel to the integration test's class selection flow

### Skill Implementation

1. Add Sentinel skills to the skill table in `const.c`
2. Implement skill functions in a new `skills_sentinel.c` or within
   existing `skills.c`
3. Add WIS-based melee scaling in `fight.c` for Sentinel-specific
   calculations

### Lore Documentation

1. Write `docs/lore/sentinel_lore.md` — full tradition history from Jackal
   Tribunal sentence enforcers through diaspora to modern practice
2. Update `docs/lore/cleric_lore.md` — add cross-reference to the
   Sentinel's distinct Tribunal lineage
3. Update `docs/lore/khenari_lore.md` — add section on Khenari Sentinels
   and the cultural significance of judicial enforcement as martial practice
4. Add Sentinel to the `KNOW_NEWPLAYER` block in the AI dialogue system

### Area File Updates

1. Add Sentinel guildmaster mob to Midgaard (near existing guildmasters)
2. Add Sentinel trainer mob
3. Update help files for class selection to include Sentinel

### Testing

1. Unit tests for WIS-based melee scaling calculations
2. Unit tests for Sentinel-specific skill functions
3. Update integration test to include Sentinel in class selection
4. Balance testing at mortal levels 1-100 against equivalent-level Wardens,
   Ciphers, and Pugilists

---

## Open Questions

1. **Class table expansion**: Should the Sentinel be appended (index 24+)
   or inserted at index 6 (after Pugilist, before remort tier)? Insertion
   preserves the mortal/remort/adept grouping but requires renumbering
   existing class constants. Appending avoids renumbering but breaks the
   tier grouping.

2. **Remort paths**: Should the Sentinel get its own dedicated remort
   classes (Justicar/Arbiter, Option A) or feed into existing Paladin/Priest
   slots (Option B)? Option A is thematically richer but doubles the
   implementation scope. Option B is achievable with minimal class table
   changes.

3. **Mana model**: The proposal gives the Sentinel 1 Mana/level — enough
   for a few observation-type abilities but not enough for sustained casting.
   Should observation abilities cost mana at all, or should they use a
   cooldown/charge system instead?

4. **WIS melee damage formula**: How should WIS contribute to melee damage?
   Options include: direct WIS-to-damage scaling (like STR), a WIS-based
   critical hit bonus (reading the opponent for openings), or a WIS-based
   counter-damage bonus (rewarding reactive play). The choice shapes whether
   the Sentinel feels like a WIS-reskinned Warden or a genuinely distinct
   melee archetype.

---

## Cross-References

- **Cleric lore** (`docs/lore/cleric_lore.md`): Section I documents the
  Jackal Tribunal as Solar Court's fifth office; Section I notes the
  Tribunal tradition "partially survived as independent judicial ethics
  lineages"
- **Khenari lore** (`docs/lore/khenari_lore.md`): Jackal Balance Court
  origin; silence-court training; WIS 22 stat profile; "not warriors"
  identity that the Sentinel extends rather than contradicts
- **Great Pyramid lore** (`docs/lore/great_pyramid_lore.md`): Sections II,
  IV document the Jackal Tribunal's role, sentence classes, and the
  "Champion of Anubis" as a tribunal executor — a direct mythic ancestor
  for the Sentinel
- **Akh'enet lore** (`docs/lore/akhenet_lore.md`): Ledger Kingdom Era
  Jackal Balance Court; Doctrine of Ninth Descent; silence-courts as
  Khenari invention
- **Templar lore** (`docs/lore/templar_lore.md`): Solar Court temple-guard
  auxiliaries — the Sentinel descends from a *different* Solar Court office
  (Jackal Tribunal) than the Paladin/Templar line (temple-guard auxiliaries)
- **Paladin lore** (`docs/lore/paladin_lore.md`): Sundering Wars origin —
  the Sentinel predates this, tracing to the Solar Court era directly
- **Ushabti lore** (`docs/lore/ushabti_lore.md`): Cosmic-law judges with
  STR 22 / WIS 22 — the Sentinel class gives Ushabti a WIS-primary
  alternative to their STR-primary Warden path
- **Class table** (`src/const_class.c`): Current 24-class structure
- **Audit document** (`docs/audit_adept_spells_skills.md`): Existing adept
  class abilities and thematic assessment
