# Proposal: Remove Appraise Skill Restriction and Cooldown

## Problem

`appraise` is currently implemented as a skill gated to Cipher (CLASS_CIP) at level 12
with a 24-beat cooldown. The goal is to make it a plain command available to all
characters with no delay.

## Approach

Remove `appraise` from the skill system entirely. The `do_appraise` command handler
remains in place but loses the `can_use_skill` gate and the skill cooldown mechanism.
Because `gsn_appraise` will be gone, the Harbor Syndics society skill list must be
updated to remove the reference.

## Affected Files

| File | Change |
|------|--------|
| `src/skills/skill_table_data.c` | Delete the `appraise` skill entry |
| `src/headers/globals.h` | Remove `extern sh_int gsn_appraise;` |
| `src/db.c` | Remove `sh_int gsn_appraise;` definition |
| `src/act_obj.c` | Remove `can_use_skill(ch, gsn_appraise)` check in `do_appraise` |
| `src/const_pub_society.c` | Remove `gsn_appraise` from Harbor Syndics skill slot 0; shift remaining slots down |
| `shelp/shelp_appraise` | Delete (no longer a skill) |
| `help/appraise` | Update to reflect universal availability |

## Detailed Changes

### `src/act_obj.c`
Remove the skill gate block:
```c
// DELETE:
if (!IS_NPC(ch) && !can_use_skill(ch, gsn_appraise))
{
   send_to_char("You don't know how to appraise items!\n\r", ch);
   return;
}
```
No cooldown call is made in `do_appraise` currently (wait comes from the skill table
lookup in the command dispatcher), so no other change is needed here.

### `src/const_pub_society.c`
Shift Harbor Syndics skills: remove slot 0 (`gsn_appraise`) and move remaining skills
down one index:
```c
// BEFORE:
pub_society_table[PUB_SOCIETY_HARBOR_SYNDICS].skill_gsns[0] = gsn_appraise;
pub_society_table[PUB_SOCIETY_HARBOR_SYNDICS].skill_gsns[1] = gsn_dockside_audit;
pub_society_table[PUB_SOCIETY_HARBOR_SYNDICS].skill_gsns[2] = gsn_haggle;
pub_society_table[PUB_SOCIETY_HARBOR_SYNDICS].skill_gsns[3] = gsn_longshoreman;
pub_society_table[PUB_SOCIETY_HARBOR_SYNDICS].skill_gsns[4] = gsn_bonded_cargo;
pub_society_table[PUB_SOCIETY_HARBOR_SYNDICS].skill_gsns[5] = gsn_anchor_slam;

// AFTER:
pub_society_table[PUB_SOCIETY_HARBOR_SYNDICS].skill_gsns[0] = gsn_dockside_audit;
pub_society_table[PUB_SOCIETY_HARBOR_SYNDICS].skill_gsns[1] = gsn_haggle;
pub_society_table[PUB_SOCIETY_HARBOR_SYNDICS].skill_gsns[2] = gsn_longshoreman;
pub_society_table[PUB_SOCIETY_HARBOR_SYNDICS].skill_gsns[3] = gsn_bonded_cargo;
pub_society_table[PUB_SOCIETY_HARBOR_SYNDICS].skill_gsns[4] = gsn_anchor_slam;
```

## Trade-offs

- **Society skill count**: Harbor Syndics drops from 6 to 5 displayed skills. This is
  fine since appraise is now free to everyone.
- **No skill-learn mechanics**: `appraise` had `can_learn: FALSE` already, so no player
  had it in their learned[] array from training. No save-file migration needed.
- **Wait time removed**: The 24-beat wait came from the skill table entry. Once the
  entry is gone the command has zero wait, which is the desired behavior.
