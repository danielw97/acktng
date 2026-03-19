# Proposal: Lore-Grounded Skill and Stance Renames

## Problem

The class lore documents establish tradition-specific names for several skills and stances, but the in-game names remain generic fantasy MUD terminology. A warden types `fortify`; a monk types `pressure point`; a cipher types `shadow step`. These names are functional but carry no connection to the world's history, factions, or traditions. The lore documents now give us better names derived from specific historical events and doctrinal frameworks.

## Proposed Changes

### Skill Renames

| Current name | Proposed name | Class | Lore source |
|---|---|---|---|
| `shadow step` | `gap transit` | Cipher | "every perimeter has a gap"; gap transit as the practitioner's term for moving through the seam between control systems |
| `garrote` | `reach silence` | Cipher | Saltglass Reach corridor tradition; technique developed where any noise during transit could alert route predators |
| `pressure point` | `applied understanding` | Pugilist / Monk | Interior Forms philosophy: "every strike not as aggression but as applied understanding" |
| `fortify` | `seven shade hold` | Warden | Named after Dunmar's tactical feat at the Seven Shade Arches; the ability to transform temporary ground into a defensible position |

### Skills Left Unchanged

- **`berserk`**: The lore frames this as "Last Watch Release" doctrine, but the skill is referenced mechanically by players across all warden lineages. "Berserk" is standard MUD vocabulary players expect. The lore integration was done through doctrine (the Last Watch conditions), not by renaming. Leave as-is.
- **`iron skin`**: The lore describes this as the "physical expression of Outer Form mastery," but doesn't establish a clean skill name. "Outer form" would be ambiguous next to the Interior Forms framework. Leave as-is.
- **`conditioning`, `counter`, `dodge`, `parry`, `bash`, `cleave`, `rend`**: Generic physical terms with no strong lore-specific equivalent proposed. Leave as-is.

### Stance Renames

| Current name | Proposed name | Class | Lore source |
|---|---|---|---|
| `Warden` | `Dunmar's Watch` | Warden (level 60) | The comprehensive positional awareness Dunmar maintained across the Forty-Three Days at Seven Shade Arches |
| `Cipher` | `Gap Intuition` | Cipher (level 60) | The point where gap-identification moves from deliberate calculation to direct perception |
| `Monk` | `Deepest Form` | Monk (level 60) | The Interior Forms' third tier: what emerges when Outer and Inner Forms are genuinely integrated |

### Stances Left Unchanged

- **`Pugilist`**: The pugilist lore describes the stance concept as "economy of motion" but the lore document does not establish a named stance term. Renaming without a clear lore-grounded name would be arbitrary. Leave for a future proposal once the tradition's stance naming is established.
- **`Healer`, `Magi`, `Psionicist`**: These either name a function directly or name the class itself. No lore-specific alternatives have been developed. Leave as-is.

## Files Affected

### For each skill rename (four renames × files):

**`src/skills/skill_table_data.c`**
The master skill definition table. The name string in each entry's second field (used for skill_lookup()) must be updated. Also update any occurrence of the skill name in that entry's hit messages where the name appears verbatim (e.g., `"garrote"` as a damage type string).

**`src/interp.c`**
The command interpreter table. Each renamed skill has a `CMD(...)` entry that specifies the string players type. Both the skill table name and the interp command string must match.

**`src/save/save_players.c`**
The load path reads `Skill X 'name'` from player files and calls `skill_lookup(name)`. Old player files will contain old skill names that will no longer resolve. A migration alias table must be added to the load handler to catch old names and remap them silently. See Migration section below.

### For each stance rename (three renames × files):

**`src/stance.c`**
The stance definition table. The first field of each stance struct is the display name string. Changing it here is sufficient — stances are not referenced by name in save files or area files.

### No changes needed:

- `src/headers/globals.h`: GSN variable names (`gsn_shadow_step`, etc.) are internal identifiers; they don't need to match the player-facing name. Renaming them would require touching every `.c` file that uses them for no gameplay benefit.
- `area/*.are`: Grep confirms no area files teach these skills to NPCs by name. The `garrote` reference in `verdantgreen.are` is an object name ("vine garrote"), not a skill reference.
- `src/db.c`: GSN assignment at boot uses `skill_lookup()` on the table name; it will automatically find the new name after the table entry is updated.

### Help files (if they exist):

Any `help/` or `shelp/` entries for renamed skills should be updated to use the new names while noting the lore origin. This is documentation work, not functional — old help entries under the old name won't break anything, they'll just be unreachable by name.

## Migration Approach for Save Files

In `src/save/save_players.c`, in the `Skill` load block (around line 1429), after `skill_lookup(skill_word)` returns -1:

```c
/* Skill name migration: map old names to new names */
static const char *skill_aliases[][2] = {
    {"shadow step",    "gap transit"},
    {"garrote",        "reach silence"},
    {"pressure point", "applied understanding"},
    {"fortify",        "seven shade hold"},
    {NULL, NULL}
};

if (sn < 0) {
    for (int i = 0; skill_aliases[i][0] != NULL; i++) {
        if (!str_cmp(skill_word, skill_aliases[i][0])) {
            sn = skill_lookup(skill_aliases[i][1]);
            break;
        }
    }
}
```

This allows old player files to load correctly on first login after the rename. The next save will write the new name, so the alias is only exercised once per character.

The same pattern applies to `save/save_objects.c` and `save/save_mobs.c` which also call `skill_lookup()` — though mob and object skill references to these specific skills are unlikely, the alias table should be applied consistently.

## Trade-offs

**For:** The new names are meaningfully thematic. A player who has read the cipher lore knows exactly what "gap transit" is — it's the doctrine in action. "Seven shade hold" has a named historical referent. These names make the world feel coherent rather than generic.

**Against:** Players who are already familiar with the current names will need to re-learn them. Prefix matching mitigates this: `gap` still resolves `gap transit`, `reach` resolves `reach silence`, `applied` resolves `applied understanding` (if no other skill starts with "applied"), `seven` resolves `seven shade hold`.

**Name length:** `applied understanding` and `seven shade hold` are longer than their predecessors. In a MUD context, players type abbreviated prefixes. The minimum unambiguous prefix is what matters, not the full name.

**"Applied understanding" prefix collision:** Need to verify no other skill starts with "applied" — if none exist, `app` or `appl` resolves it cleanly.

## Implementation Order

1. Add migration alias table to `save/save_players.c`, `save/save_objects.c`, `save/save_mobs.c`
2. Update `skill_table_data.c` entries for four skill renames
3. Update `interp.c` command strings for four skill renames
4. Update `stance.c` entries for three stance renames
5. Update help files if they exist
6. Run `make unit-tests` to verify no regressions

## Testing

- Build (`make ack`) should succeed with no warnings
- Integration test should pass (boots, login flow works)
- Manually verify: player with old skill names loads correctly (migration fires)
- Manually verify: renamed skill works when typed by new name
- Manually verify: stance display shows new name when entering stance
