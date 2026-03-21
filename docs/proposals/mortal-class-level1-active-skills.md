# Proposal: Level 1 Active Skills for All Mortal Classes

**Status:** Draft — for discussion
**Scope:** `src/` changes required
**Branch:** `claude/add-level-1-skills-9tnDn`

---

## Section I: Problem

Every mortal class currently lacks any active combat ability at level 1. The earliest
offensive tools arrive at level 3 (Magi, Druid), level 4 (Cleric, Psionicist), or
level 5 (Warden, Cipher, Pugilist, Sentinel). A brand-new character entering combat
has nothing to do but auto-attack.

This is a poor new-player experience and a mechanical gap. The fix is one new active
skill or spell per mortal class, available at level 1, designed to fit each class's
documented tradition without complexity.

---

## Section II: Scope

Eight mortal classes require a new level 1 active ability:

| Class | Type | Existing lowest |
|---|---|---|
| Magi (CLASS_MAG) | Spellcaster | `magic missile` lv 3 |
| Cleric (CLASS_CLE) | Spellcaster | `divine strike` lv 4 |
| Psionicist (CLASS_PSI) | Spellcaster | `mind flail` lv 4 |
| Druid (CLASS_DRU) | Spellcaster | `thornlash` lv 3 |
| Warden (CLASS_WAR) | Skill-based | `punch` / `weapon mastery` lv 5 |
| Cipher (CLASS_CIP) | Skill-based | `sneak` lv 5 |
| Pugilist (CLASS_PUG) | Skill-based | `cistern discipline` lv 5 |
| Sentinel (CLASS_SEN) | Skill-based | `weapon mastery` / `cistern discipline` lv 5 |

Spellcasters get new entries in `spell_table_data.c` with new spell function files.
Skill-based classes get new entries in `skill_table_data.c` with new `do_*.c` files,
`gsn_` declarations, and command table registrations.

---

## Section III: Proposed Abilities

### Magi — `ember spark`

**Lore:** The Ember Canon faction of the old Spirebound Conclave worked in fire,
will-through-suffering, and lifesteal. The Wizard's Keep inherits this lineage.
The first exercise in the Keep's curriculum is controlled ignition — producing a
small, directed burst of fire. This is not a weapon; it is proof of control.

**Mechanics:**
- Spell, `TAR_CHAR_OFFENSIVE`, `POS_FIGHTING`
- Damage: `dice(2, 4) + level / 4`, `ELEMENT_FIRE`
- Mana cost: 8, beats: 12
- `can_learn: FALSE` (auto-granted at level 1)
- Noun damage: `"ember spark"`, no wear-off message

**New files:** `src/spells/spell_ember_spark.c`
**Table change:** entry in `spell_table_data.c`, `SLOT(843)`

---

### Cleric — `solar brand`

**Lore:** The Solar Court's five-office tradition centers on precision, binding, and
cosmic maintenance. The first liturgical act taught to new candidates is the Solar
Brand — a mark of the Black Sun Shard's authority pressed briefly into a target.
It deals minor holy damage and is used in the Dawn Liturgy as a ritual opener.

**Mechanics:**
- Spell, `TAR_CHAR_OFFENSIVE`, `POS_FIGHTING`
- Damage: `dice(2, 4) + level / 4`, `ELEMENT_HOLY`
- Bonus vs. evil: same pattern as `divine strike` (`alignment < -350` → +50% damage)
- Mana cost: 8, beats: 12
- `can_learn: FALSE`
- Noun damage: `"solar brand"`, no wear-off message

**New files:** `src/spells/spell_solar_brand.c`
**Table change:** entry in `spell_table_data.c`, `SLOT(844)`

---

### Psionicist — `cognitive pressure`

**Lore:** Psionicist training begins with a single discipline: applying focused will
to another mind as a disruptive force. This is not an attack in the conventional
sense — it is a probe. The practitioner identifies a surface cognitive pattern and
presses against it until the subject flinches. At low levels the damage is modest;
the technique matters more than the force.

Importantly, this skill is explicitly defensive in origin — Psionicist doctrine
teaches it as a way to interrupt a threat, not to pursue one.

**Mechanics:**
- Spell, `TAR_CHAR_OFFENSIVE`, `POS_FIGHTING`
- Damage: `dice(2, 4) + level / 4`, `ELEMENT_MENTAL`
- Save check: `saves_spell()` halves damage (mind can resist surface pressure)
- Mana cost: 8, beats: 12
- `can_learn: FALSE`
- Noun damage: `"cognitive pressure"`, no wear-off message

**New files:** `src/spells/spell_cognitive_pressure.c`
**Table change:** entry in `spell_table_data.c`, `SLOT(845)`

---

### Druid — `root touch`

**Lore:** The Druid's Overgrowth Doctrine holds that spells draw on the practitioner's
own life force — constitution as covenant. The most basic expression of this is direct
contact: the druid channels living-world vitality into a physical strike, momentarily
extending the root-network's reach through their own body. At level 1, this is
undirected and inefficient; it costs the druid more than it should.

**Mechanics:**
- Spell, `TAR_CHAR_OFFENSIVE`, `POS_FIGHTING`
- Damage: `dice(2, 4)` (does not scale with level — the druid's control is limited at first)
- Element: `ELEMENT_EARTH`
- HP cost via `growth` field: 3 (paid from caster's hit points, consistent with Overgrowth mechanic)
- Mana cost: 0 (HP-only)
- beats: 12
- `can_learn: FALSE`
- Noun damage: `"root touch"`, no wear-off message

**New files:** `src/spells/spell_root_touch.c`
**Table change:** entry in `spell_table_data.c`, `SLOT(846)`

---

### Warden — `duty strike`

**Lore:** Dunmar's Watch doctrine teaches that the first blow from held ground is
not an attack — it is a statement of position. The duty strike is the foundational
technique of the Registry School curriculum: a direct, committed blow delivered
from a stable stance. It does no more damage than a heavy punch. Its purpose is
to force the enemy to acknowledge that the ground is held.

**Mechanics:**
- Skill, `TAR_CHAR_OFFENSIVE`, `POS_FIGHTING`, `spell_null`
- Implementation: `war_attack(ch, argument, gsn_duty_strike)`
- Damage handled by `war_attack()` (same as kick, headbutt)
- Mana cost: 1 (stamina), beats: 18
- `can_learn: TRUE` (practiced, starts at 25% — low but functional)
- Noun damage: `"duty strike"`, wear-off: `"!Duty Strike!"`

**New files:** `src/skills/do_duty_strike.c`
**Table change:** entry in `skill_table_data.c`
**Other changes:** `gsn_duty_strike` in `db.c` + `globals.h`, CMD entry in `interp.c`

---

### Cipher — `gap probe`

**Lore:** Gap Intuition — the Cipher's foundational doctrine — begins not with
exploitation but with assessment. Before a gap can be used, it must be found.
The gap probe is that assessment: a testing strike that searches for structural
fault in the target's defense. At level 1 the Cipher cannot yet exploit what they
find, but they can identify it, and the strike itself carries force.

**Mechanics:**
- Skill, `TAR_CHAR_OFFENSIVE`, `POS_FIGHTING`, `spell_null`
- Implementation: direct melee attack using `damage()` with `gsn_gap_probe`,
  damage `dice(1, ch->level + 2)`, adjusted by skill percentage
- Mana cost: 0, beats: 18
- `can_learn: TRUE` (starts at 25%)
- Noun damage: `"gap probe"`, wear-off: `"!Gap Probe!"`

Note: Cipher has no dedicated `cip_attack()` helper (unlike Warden/Pugilist). The
`do_gap_probe.c` implementation uses the same direct `damage()` call pattern as
`do_garrote.c`.

**New files:** `src/skills/do_gap_probe.c`
**Table change:** entry in `skill_table_data.c`
**Other changes:** `gsn_gap_probe` in `db.c` + `globals.h`, CMD entry in `interp.c`

---

### Pugilist — `primary motion`

**Lore:** Economy of Motion is the Pugilist's core principle: no wasted movement,
every action deliberate. The primary motion is the first thing taught — a single,
clean, efficient strike. It is not powerful. It is correct. The pyramid-construction
labor tradition that produces Pugilists does not celebrate force; it celebrates
precision of application. This is where that application begins.

**Mechanics:**
- Skill, `TAR_CHAR_OFFENSIVE`, `POS_FIGHTING`, `spell_null`
- Implementation: `pug_attack(ch, argument, gsn_primary_motion)`
- Damage handled by `pug_attack()`
- Mana cost: 1, beats: 18
- `can_learn: TRUE` (starts at 25%)
- Noun damage: `"primary motion"`, wear-off: `"!Primary Motion!"`

**New files:** `src/skills/do_primary_motion.c`
**Table change:** entry in `skill_table_data.c`
**Other changes:** `gsn_primary_motion` in `db.c` + `globals.h`, CMD entry in `interp.c`

---

### Sentinel — `vigil strike`

**Lore:** The Sentinel class has no lore file yet (noted as a gap in `docs/lore/`).
Based on the class name and attribute (WIS-primary), Sentinels are watchful guardians
— positioned protectors whose combat begins from observation. The vigil strike is
a deliberate blow delivered from a watched position: the Sentinel sees the opening
before they take it.

Implementation-wise, Sentinel shares physical orientation with Warden; `war_attack()`
is used here pending a dedicated Sentinel mechanic in a later proposal.

**Mechanics:**
- Skill, `TAR_CHAR_OFFENSIVE`, `POS_FIGHTING`, `spell_null`
- Implementation: `war_attack(ch, argument, gsn_vigil_strike)`
- Mana cost: 1, beats: 18
- `can_learn: TRUE` (starts at 25%)
- Noun damage: `"vigil strike"`, wear-off: `"!Vigil Strike!"`

**New files:** `src/skills/do_vigil_strike.c`
**Table change:** entry in `skill_table_data.c`
**Other changes:** `gsn_vigil_strike` in `db.c` + `globals.h`, CMD entry in `interp.c`

---

## Section IV: Files Changed

### New source files (12)

```
src/spells/spell_ember_spark.c
src/spells/spell_solar_brand.c
src/spells/spell_cognitive_pressure.c
src/spells/spell_root_touch.c
src/skills/do_duty_strike.c
src/skills/do_gap_probe.c
src/skills/do_primary_motion.c
src/skills/do_vigil_strike.c
```

### Modified source files

| File | Change |
|---|---|
| `src/spells/spell_table_data.c` | 4 new spell entries (SLOT 843–846) |
| `src/skills/skill_table_data.c` | 4 new skill entries |
| `src/db.c` | 4 new `gsn_` variable definitions |
| `src/headers/globals.h` | 4 new `extern sh_int gsn_*` + 4 `DECLARE_DO_FUN` |
| `src/interp.c` | 4 new CMD entries |
| `src/Makefile` | 8 new `.c` files added to build |

### New help files (8)

One entry per new ability in `shelp/` (as required by CLAUDE.md).

---

## Section V: Trade-offs and Notes

**Damage scaling:** All level 1 spells use `dice(2,4) + level/4`. At level 1 this
is 2–8 + 0 = 2–8 damage. This is intentionally weak — these are starter abilities,
not progression milestones. The existing level 3–5 spells hit harder.

**`can_learn` for skills:** The four new skills use `can_learn: TRUE`, which means
players must practice them. At 25% starting proficiency the skill will sometimes
fail. This is intentional — level 1 is rough — but the skill always gives the
character *something to type* even when it fails. An alternative is `FALSE`
(auto-granted at 100%), which removes the practice element. Either works; `TRUE`
is proposed here to keep the practice system consistent with other early skills.

**Sentinel lore gap:** The vigil strike is necessarily thin because no Sentinel lore
file exists. This proposal notes that gap and uses a Warden-style implementation as
a placeholder. A richer Sentinel-specific mechanic should wait for lore development.

**Root touch HP cost:** The `growth` field is what makes Druid spells cost HP instead
of mana. Setting `growth=3` means 3 HP per cast — trivial at any level, but thematic.
If the Druid mechanic for growth works differently than expected (it needs verification
against the actual Overgrowth implementation), the fallback is a small mana cost (8)
instead.

**Slots:** Spell slots 843–846 are assigned sequentially above the current maximum
(842). Skill table entries use `SLOT(0)` consistent with all other non-object skills.

---

## Section VI: Out of Scope

- No changes to existing spells or skills
- No remort class changes
- No lore file creation (Sentinel lore is noted as a gap but not addressed here)
- No changes to the practice system or starting skill percentages globally
