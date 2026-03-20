# Proposal: Clarify Adept Remort Requirements (EITHER, not BOTH)

## Problem

The design spec (`docs/class_design_spec.md`) clearly states that each adept class requires **either** of two associated remort prerequisites. However, multiple sources across the codebase contradict this or are ambiguous:

1. **Source code** (`src/act_info.c:4543-4550`): The `gain` command uses `||` logic, meaning it rejects the player if **either** prereq is below level 100 — effectively requiring **both**. The error message also says "both remortal classes."
2. **Help files**: 4 of 6 adept class help files use ambiguous "tied to X and Y" phrasing. Only `help/nightblade` clearly says "either...or". The `help/remort` file correctly says "one of" but the individual class files undercut this.
3. **Lore files**: Several lore files (especially `martial_artist_lore.md`) explicitly state that both apprenticeships are required, contradicting the EITHER design intent.
4. **Code comments & docs**: `src/const_class.c` comment and `docs/audit_adept_spells_skills.md` use ambiguous "two remort prerequisites" / "two completed remort paths" language.

## Approach

### 1. Fix the code logic (`src/act_info.c`)

Change the prereq check from requiring BOTH to requiring EITHER:

```c
// BEFORE (requires BOTH):
if (ch->class_level[gclass_table[cnt].prereq[0]] < MAX_MORTAL ||
    ch->class_level[gclass_table[cnt].prereq[1]] < MAX_MORTAL)

// AFTER (requires EITHER):
if (ch->class_level[gclass_table[cnt].prereq[0]] < MAX_MORTAL &&
    ch->class_level[gclass_table[cnt].prereq[1]] < MAX_MORTAL)
```

Update the error message to: "You need to be level 100 in at least one of this class's remort prerequisites before you can adept!\n\r"

### 2. Fix help files (5 files)

Update the "To become a ..." line in each adept help file to use the explicit "either...or" pattern (matching the existing `help/nightblade` style):

| File | Current | Proposed |
|------|---------|----------|
| `help/grand_magi` | "you follow the adept path tied to Sorcerer and Wizard remort progression" | "you must master either Sorcerer or Wizard (remort prerequisites for the adept path)" |
| `help/templar` | "you follow the adept path tied to Paladin and Priest remort progression" | "you must master either Paladin or Priest (remort prerequisites for the adept path)" |
| `help/crusader` | "you follow the adept path tied to Knight and Swordsman remort progression" | "you must master either Knight or Swordsman (remort prerequisites for the adept path)" |
| `help/martial_artist` | "you follow the adept path tied to Monk and Brawler remort progression" | "you must master either Monk or Brawler (remort prerequisites for the adept path)" |
| `help/kinetimancer` | "you follow the adept path tied to Necromancer and Egomancer remort progression" | "you must master either Necromancer or Egomancer (remort prerequisites for the adept path)" |

Also fix history-section lines in `help/grand_magi` and `help/templar` that say "completed both lines" / "mastered both" — soften to describe historical tradition without implying a mechanical both-requirement.

### 3. Fix help/remort prerequisite listing

Change the parenthetical format from `(Sorcerer + Wizard)` to `(Sorcerer or Wizard)` for all 6 adept entries, to match the "one of" language already in that file.

### 4. Fix lore files (docs/lore/)

Lore files describe the fictional history — traditions that *emerged* from practitioners who trained in both paths. This is fine as world-building. However, lines that state both are *required* as a mechanical/practical prerequisite need softening. Key changes:

- **`docs/lore/grand_magi_lore.md` line 5**: "completed both lines" → "completed either line" (or "completed one or both lines")
- **`docs/lore/templar_lore.md` line 5**: "mastered both the oath-vanguard and the restorative minister" → "mastered either the oath-vanguard or the restorative minister"
- **`docs/lore/martial_artist_lore.md` lines 51, 55**: The "one apprenticeship is not enough" and "requires both traditions' depth contributions" language describes the *lore origin* of the tradition. Reframe to describe this as the historical/ideal formation, while noting that modern practitioners can achieve the synthesis through mastery of either path.

### 5. Fix code comment (`src/const_class.c`)

Line 56: Change `/* Adept (18-23): tier=ADEPT, two remort prerequisites */` to `/* Adept (18-23): tier=ADEPT, either of two remort prerequisites */`

### 6. Fix audit doc (`docs/audit_adept_spells_skills.md`)

Line 10: Change "sits atop two completed remort paths" to "sits atop at least one completed remort path"

### 7. Fix proposal doc reference (`docs/proposals/ai.md`)

Line 1680: "completing two remort classes" → "completing at least one of two associated remort classes"

## Affected Files

| File | Change Type |
|------|-------------|
| `src/act_info.c` | Code logic fix + error message |
| `src/const_class.c` | Comment fix |
| `help/remort` | Parenthetical format fix |
| `help/grand_magi` | Requirement text + history text |
| `help/templar` | Requirement text + history text |
| `help/crusader` | Requirement text |
| `help/martial_artist` | Requirement text |
| `help/kinetimancer` | Requirement text |
| `docs/lore/grand_magi_lore.md` | Lore text |
| `docs/lore/templar_lore.md` | Lore text |
| `docs/lore/martial_artist_lore.md` | Lore text |
| `docs/audit_adept_spells_skills.md` | Doc text |
| `docs/proposals/ai.md` | Doc text |

## Trade-offs

- **Lore rewriting**: The lore describes traditions formed by practitioners who mastered both paths. We preserve this historical flavor while clarifying that the *game mechanic* only requires one. Some lore passages need careful rewording to maintain narrative quality.
- **Code change**: Changing `||` to `&&` is a one-character logic fix but changes actual gameplay behavior. Players who previously needed both remorts at 100 will now only need one.

## Unit Test

A unit test should be added for the adept prerequisite check to verify the EITHER logic.
