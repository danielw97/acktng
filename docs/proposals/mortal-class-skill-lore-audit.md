# Proposal: Mortal Class Skill Lore Audit

**Status:** Draft — for discussion  
**Scope:** `src/` changes required (spell/skill table modifications, new skill implementations)  
**Depends on:** Completed work in `lore-skill-stance-renames.md` (gap transit, reach silence, applied understanding, seven shade hold, Dunmar's Watch, Gap Intuition, Deepest Form already implemented)

---

## Section I: Overview and Audit Methodology

### Background

The previous lore-skill work (`lore-skill-stance-renames.md`, now completed) renamed specific skills and stances whose in-game names were generic where the lore had established better ones. That work was targeted: four skill renames and three stance renames, all cases where a clear lore-grounded name existed and implementation was straightforward.

This audit goes deeper. It examines every skill and spell assigned to each mortal class and asks three questions:

1. **Is this skill lore-hostile?** Does it actively contradict the class's documented tradition — using the wrong magical lineage, employing concepts that belong to a different institution, or doing things the lore says this tradition specifically does not do?
2. **Is this skill lore-neutral?** Is it generic fantasy vocabulary with no connection to the class's documented world — present because every MUD needs a healing spell, not because it means something?
3. **Is this skill lore-grounded?** Does the name, mechanism, or flavor connect to the specific historical institutions, doctrines, practitioners, or events that define the class?

The goal is not to replace every generic skill — some generic names are fine. The goal is to identify the gaps and contradictions that make a class feel like a list of D&D spell names rather than a tradition with a specific history.

### Audit Categories

**Tier A — Lore-hostile (highest priority):** The skill actively contradicts the class's documented tradition. Remove or replace.

**Tier B — Lore-neutral (medium priority):** The skill is generic but not wrong. Could be renamed or replaced with something meaningful, but the current name causes no confusion.

**Tier C — Missing lore (medium priority):** An important element of the class's documented tradition has no mechanical representation. The class lacks something it should have.

**Tier D — Rename opportunity (lower priority):** The skill is correctly placed but has a generic name where the lore offers a better one. The completed renames proposal handled the clearest cases; this tier captures remaining opportunities.

### What Was Already Done

The following changes are already implemented and not revisited here:

| Old name | New name | Class |
|---|---|---|
| `shadow step` | `gap transit` | Cipher |
| `garrote` | `reach silence` | Cipher |
| `pressure point` | `applied understanding` | Pugilist / Monk |
| `fortify` | `seven shade hold` | Warden |

Stances already renamed: `Warden` → `Dunmar's Watch`, `Cipher` → `Gap Intuition`, `Monk` → `Deepest Form`.

The four remaining mortal stances — **Magi**, **Healer** (Cleric), **Psionicist**, and **Pugilist** — were left in the previous proposal because no lore-grounded alternative had been developed. This audit proposes names for all four.

### Audit Scope

Six mortal classes audited:

- **Magi** — 35 spells, 1 skill (`mystical potency`), 1 stance (`Magi`)
- **Cleric** — 30 spells, 1 skill (`spell critical damage`), 1 stance (`Healer`)
- **Psionicist** — 26 spells, 2 skills (`spell critical`, `shadowform`), 1 stance (`Psionicist`)
- **Warden** — 22 skills, 1 stance (`Dunmar's Watch` — already done)
- **Cipher** — 24 skills, 1 stance (`Gap Intuition` — already done)
- **Pugilist** — 16 skills, 1 stance (`Pugilist`)

---
