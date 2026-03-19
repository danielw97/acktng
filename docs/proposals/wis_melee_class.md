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

## Keystone Passive: Vigilance

### The Gap in Stat-22 Passives

Every primary stat at 22 grants a racial passive skill that defines its
combat identity. Currently, WIS 22 only grants spell-oriented passives:

| Stat 22 | Racial Passive | Effect | Domain |
|---------|---------------|--------|--------|
| STR 22 | Enhanced Damage | +STR\*3/100 physical damage modifier | Melee offense |
| DEX 22 | Enhanced Critical | Increased critical hit chance | Melee offense |
| CON 22 | Counter | Free attack after successful dodge/parry/block | Melee defense |
| INT 22 | Mystical Potency | +INT\*2/100 non-physical spell damage | Caster offense |
| WIS 22 | Spell Critical + Spell Crit Damage | Better spell crit chance and multiplier | **Caster offense** |

WIS 22 has no melee passive. A Khenari Sentinel with WIS 22 gets passives
that improve spellcasting — which the Sentinel barely does. Every other
melee stat rewards its melee identity through its racial passive; WIS does
not.

### Vigilance

**Vigilance** is a new WIS-granted racial passive that increases dodge and
parry chance. It represents WIS expressed as combat anticipation — the
Sentinel reads incoming attacks through perceptive depth, not through raw
speed (DEX) or toughness (CON).

```c
/* In check_avoidance(), added to parry and dodge chance calculations */
if (can_use_skill(victim, gsn_vigilance))
    chance += get_curr_wis(victim) * 2 / 5;  /* +8 at WIS 22 */
```

**Integration points in fight.c**: Added to the parry check (line ~935)
and dodge check (line ~1000) in `check_avoidance()`, the same function
where Counter already fires. Vigilance modifies the avoidance *chance*;
Counter fires *after* a successful avoidance. They are orthogonal.

**Updated stat-22 passive table:**

| Stat 22 | Melee Passive | Combat Identity |
|---------|--------------|-----------------|
| STR 22 | Enhanced Damage | Hit harder |
| DEX 22 | Enhanced Critical | Hit sharper |
| CON 22 | Counter | Hit back |
| WIS 22 | **Vigilance** | **Get hit less** |

**Universally useful**: Vigilance is not Sentinel-specific. Any class with
WIS 22 benefits — a Cleric with Vigilance dodges more, a Paladin with
Vigilance parries more. The passive is general-purpose, like Enhanced
Damage is useful on any STR 22 character regardless of class.

**Natural Sentinel synergy**: Vigilance has emergent synergy with the
Sentinel's Testimony mechanic (see below) because more successful
avoidance means faster testimony accumulation. But this synergy is not
baked in — a Cleric with Vigilance just dodges more. A Sentinel with
Vigilance dodges more *and* builds resource from it. The interaction
arises from combining two independent systems, not from one system
referencing the other.

**Racial impact**: WIS 22 races gain Vigilance as a racial skill alongside
their existing Spell Critical and Spell Critical Damage:

| Race | WIS | Current Racial Skills | With Vigilance |
|------|-----|----------------------|----------------|
| Khenari | 22 | sneak, spell crit damage, spell crit | + vigilance |
| Deltari | 22 | hunt, spell crit damage, spell crit | + vigilance |
| Ushabti | 22 | stone skin, enhanced damage, spell crit damage, spell crit | + vigilance |
| Kethari | 22 | counter, spell crit damage, spell crit | + vigilance |

Kethari (WIS 22 + CON 22) would have both Counter and Vigilance — higher
avoidance chance *and* a free attack when avoidance succeeds. This is a
strong defensive combination but not overpowered: Counter's proc rate is
already modified by the attacker's evasion piercing, and Vigilance's
+8 dodge/parry is comparable to Enhanced Damage's ~+66% physical damage
modifier in relative impact.

**Thematic source**: The silence-courts of Akh'enet, where testimony was
"clarified" through acoustic deprivation and chant intervals, produced
practitioners whose minds were organized against noise, distraction, and
psychic pressure. Vigilance is this training applied to combat: the
capacity to read an opponent's intent from postural signals before the
strike arrives.

---

## Core Mechanic: Testimony

### Overview

The Sentinel's class-specific combat resource is **Testimony** — marks
accumulated on a single target through successful defensive reading. When
enough marks are built, the Sentinel spends them to deliver a **Verdict**:
a powerful single strike whose damage and secondary effect scale with the
number of marks consumed.

Testimony is mechanically distinct from the other class resource systems:

| System | Builds On | Spend Model | Player Decision |
|--------|-----------|-------------|-----------------|
| Chi (Monk) | Offense — +1 per successful attack | Choose which chi skill to activate | Which skill, when |
| Combo (Warden) | Varied offense — track different skills used | Automatic finisher when chain fills | Which skills to chain (avoid duplicates) |
| Testimony (Sentinel) | Defense — +1 per successful dodge/parry/block | Choose when to deliver verdict | When to fire (hold for bigger verdict vs. spend now) |

Chi builds on hitting things. Combo builds on hitting with variety.
Testimony builds on *not getting hit*. This creates a fundamentally
different combat feel: the Sentinel is the only melee class whose power
increases the longer they fight the same target, and whose resource
generation rewards being attacked rather than attacking.

### Data Structure

```c
/* In CHAR_DATA */
int testimony;               /* accumulated marks on current target, 0-9 */
CHAR_DATA *testimony_target; /* who marks are tracked against */
```

### Building Testimony

**Primary source — successful avoidance:**

Each successful dodge, parry, or block against the Sentinel's current
combat target adds +1 testimony. This hooks into the three avoidance
check locations in `check_avoidance()` (fight.c) where Counter already
fires:

```c
/* After successful parry — existing counter check is here */
if (number_percent() < (get_counter(victim) - get_evasion_piercing(ch)))
    one_hit(victim, ch, gsn_counter);

/* NEW: Sentinel testimony accumulation */
if (is_sentinel_class(victim) && victim->fighting == ch)
    add_testimony(victim, 1);
```

Same hook at the block and dodge check locations. Testimony and Counter
coexist — a Kethari Sentinel (CON 22 granting Counter, WIS 22 granting
Vigilance) would proc both the counter-attack and the testimony mark on
the same avoidance event.

**Secondary source — passive observation tick:**

Against enemies that attack infrequently or die quickly, pure defensive
building would be too slow. The Sentinel gains +1 testimony per 3 combat
rounds automatically while in combat with the same target. This is a
floor, not the primary build path — against a hard-hitting boss that
swings 6 times per round, defensive procs build testimony far faster.

**Active source — Read Opponent skill:**

The `read opponent` skill (see Skills section) instantly grants +2
testimony marks at the cost of an action. This is the fast-charge option
for situations where the Sentinel needs to reach a verdict threshold
quickly.

**Cap**: 9 marks maximum (the Nine Descents of Khenari funerary
tradition).

### Losing Testimony

- **Target switch**: Changing targets resets testimony to 0. The Tribunal
  adjudicated individual cases — the reading is specific to *that*
  opponent.
- **Target death**: Resets to 0.
- **Stun**: Lose half testimony (rounded down). Concentration broken, but
  silence-court training prevents total loss.
- **Flee/recall**: Resets to 0.
- **Out of combat**: Testimony decays by 1 per tick (not instant reset —
  the observation lingers briefly, allowing re-engagement).

### Spending Testimony: The Verdict Command

`verdict` is the Sentinel's signature active skill. It consumes all
accumulated testimony marks and delivers a single melee strike whose
damage and secondary effect scale with marks consumed.

The mark count determines the verdict tier automatically. The player
decision is *when* to fire, not *what* to fire.

| Marks | Name | Damage | Secondary Effect |
|-------|------|--------|-----------------|
| 1-2 | Rebuke | 1.5x base + WIS bonus | None — preliminary observation, insufficient for a ruling |
| 3-4 | Censure | 2.5x base + WIS bonus | Target -2 HR for 3 rounds (you've read their form and disrupted it) |
| 5-6 | Binding Verdict | 4x base + WIS bonus | Target -15% attack speed for 4 rounds (structural weakness identified and struck) |
| 7-8 | Sealing Verdict | 6x base + WIS bonus | All above + target cannot use skills/spells for 2 rounds (options sealed through positional control) |
| 9 | Final Verdict | 9x base + WIS bonus | All above + 1 round stun (the complete judicial process: observation, evidence, cross-reference, irrevocable sentence) |

**WIS damage scaling**: The WIS bonus component scales with
`get_curr_wis(ch) * get_pseudo_level(ch) / 80`, making WIS the primary
damage stat for verdicts.

**Cooldown**: After spending testimony, a 2-round cooldown before
testimony begins accumulating again. The Tribunal does not rush between
cases.

### Combat Loop: How a Sentinel Plays

**Early fight (rounds 1-4):**
Enter combat at testimony 0. Basic melee attacks deal moderate damage (no
Enhanced Damage, so weaker hit-for-hit than a Warden). Enemy swings back.
Sentinel dodges — +1 testimony. Parries — +1 testimony. Passive tick at
round 3: +1. By round 4, testimony is at 3-5 depending on avoidance rate.

**Mid fight (rounds 5-8):**
Testimony at 5-6. The Sentinel has a choice: fire a Binding Verdict now
(4x damage + speed debuff) or keep building. If the fight is dangerous and
the debuff matters, fire now. If the fight is going well, hold. Optionally
pop Ninth Descent to double testimony accumulation and push for Sealing
Verdict.

**Climax (marks 7-9):**
Fire Sealing or Final Verdict. Massive single hit + powerful debuff/stun.
After spending, testimony resets. 2-round cooldown, then the cycle begins
again. In a long boss fight, the Sentinel cycles 2-3 times: build →
verdict → build → verdict.

**Against trash mobs:**
Mobs die in 3-4 rounds. Sentinel fires a Rebuke or Censure for bonus
damage on what would be the killing blow. Not optimal for AoE grinding —
the Sentinel is a single-target sustained fighter, strongest against tough
enemies. The Tribunal adjudicated important cases, not petty theft.

### What Makes It Feel Different

A **Warden** player thinks: "What skill do I use next to avoid duplicates
and fill my combo chain?"

A **Pugilist** player thinks: "Which chi skill do I use, and is it off
cooldown?"

A **Sentinel** player thinks: "How many marks do I have? Do I fire now for
the debuff my group needs, or hold for a bigger verdict? Should I burn
Ninth Descent to accelerate? Can I survive long enough to reach 9?"

The Sentinel is the only melee class whose power *increases the longer
they fight the same target*. Wardens front-load through combo finisher
resets. Monks fire chi skills whenever available. The Sentinel ramps up.
Against trash this is a disadvantage. Against a boss, it is their defining
strength.

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

### Suggested Mortal Skills

| Skill | Type | Description | Thematic Source |
|-------|------|-------------|-----------------|
| **Verdict** | Active (offensive) | Consume all testimony marks for a single strike whose damage and effect scale with marks (see Testimony section). The Sentinel's signature spend ability. | Tribunal sentence enforcement |
| **Read Opponent** | Active (self-buff) | Instantly grants +2 testimony marks. 8-round cooldown. Costs an action — the Sentinel studies rather than attacks. | Silence-court testimony cross-reference |
| **Binding Strike** | Active (offensive + debuff) | Melee attack that deals modest damage and reduces target dodge/parry for 3 rounds. Does not generate testimony. Bread-and-butter offensive skill that softens opponents for the verdict. | Tribunal restraint and containment |
| **Testimonial Guard** | Passive | WIS-scaled resistance to fear, charm, and mental effects. At 5+ testimony marks, immunity to fear — the Sentinel's focus is too deep to be shaken. | Silence-court mind-discipline training |
| **Ninth Descent** | Active (self-buff) | For 4 combat rounds, each successful avoidance grants +2 testimony instead of +1. High mana cost (uses the Sentinel's small mana pool). 20-round cooldown. The burst window for reaching high verdict tiers quickly. | Khenari Doctrine of Ninth Descent |

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

### Vigilance Passive

1. Add `gsn_vigilance` to `globals.h` and `db.c`
2. Add Vigilance skill entry to the skill table in
   `skills/skill_table_data.c` (passive, no cost)
3. Add `gsn_vigilance` check to parry and dodge calculations in
   `check_avoidance()` in `fight.c`:
   `chance += get_curr_wis(victim) * 2 / 5`
4. Add `vigilance` to racial skill strings for WIS 22 races in
   `const_race.c` (Khenari, Deltari, Ushabti, Kethari)

### Testimony System

1. Add `int testimony` and `CHAR_DATA *testimony_target` fields to
   `CHAR_DATA` in `ack.h`
2. Add testimony accumulation hooks in `check_avoidance()` at the three
   avoidance success points (parry, block, dodge)
3. Add passive testimony tick (+1 per 3 rounds) in `violence_update()`
   or `round_update()`
4. Add testimony reset logic: target switch, target death, stun (half),
   flee/recall, out-of-combat decay
5. Null out `testimony_target` pointer in `extract_char()` for any
   Sentinel whose target is being extracted

### Skill Implementation

1. Add Sentinel skills (verdict, read opponent, binding strike,
   testimonial guard, ninth descent) to the skill table
2. Implement skill functions in a new `skills_sentinel.c`
3. Add WIS-based verdict damage scaling in `damage.c`

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
   for Ninth Descent and a few other abilities but not sustained casting.
   Should observation abilities cost mana at all, or should they use a
   cooldown/movement-cost system (like chi skills use movement) instead?

4. **Testimony build rate tuning**: The current design gives +1 per
   successful avoidance, +1 per 3 rounds passive, and +2 from Read
   Opponent. These rates need playtesting to determine whether the
   Sentinel reaches meaningful verdict tiers (5-6 marks) in a reasonable
   timeframe against typical mortal-tier content without being too fast
   against bosses.

5. **Vigilance balance**: The proposed +8 dodge/parry at WIS 22 needs
   testing against the avoidance cap system. `check_avoidance()` uses a
   `max_avoidance` pool (75 + speed differential) shared across parry,
   block, and dodge. Vigilance increases individual check chances but the
   pool cap may limit its effective impact at high avoidance values.
   Determine whether Vigilance should add to individual check chance
   (current proposal), increase the `max_avoidance` pool, or both.

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
