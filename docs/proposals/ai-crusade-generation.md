# Proposal: AI-Generated Crusade Personalities and Quest Objects

## Problem

The current crusade personality system has three hard-coded personalities
(wimpy/cool/mean) with fixed message templates. Every quest mob uses the same
17-message script regardless of who they are — a dark stalker, a marketplace
merchant, and a court herald all say the same words. The quest object is always
a generic "a quest token," disconnected from any fiction about what was
actually stolen.

The request is to:
1. Replace the static personality/message system with AI-generated content
   that reflects the specific NPC's identity, area, and world lore
2. Expand beyond three personalities
3. Use a single quest item vnum whose name and description are generated
   at quest start to fit the NPC

---

## Architecture Overview

The game server is a single-threaded C event loop. A blocking HTTP call to
the Claude API during the main loop would freeze all players. The solution is
**asynchronous generation via fork + pipe**:

1. Quest generation selects quest_mob and quest_target as today
2. Instead of immediately starting the quest, the server forks a Python helper
   script, passing NPC context as JSON
3. The script calls the Claude API and writes results (messages + item details)
   to a temp file, then exits
4. The game polls the temp file each tick; when it appears, it reads the
   results, populates quest state, and starts the quest properly
5. If generation takes more than ~10 seconds or fails, the server falls back
   to the existing static personality system

The AI results are: a chosen personality name, 17 message pairs, an item
short description, and an item long description.

---

## Section I: New Personality Set

Rather than three fixed personalities, the AI selects from (or blends) a
larger vocabulary. The static fallback system (used on API failure) is also
expanded from 3 to 8:

| # | Name | Tone | Example NPC types |
|---|------|------|-------------------|
| 1 | `wimpy` | Tearful, helpless, childlike | young apprentices, small animals, peasants |
| 2 | `cool` | Detached, philosophical, resigned | old veterans, wandering scholars |
| 3 | `mean` | Wrathful, dramatic, threatening | dark lords, demon-touched, warlords |
| 4 | `noble` | Dignified, formal, honor-bound | knights, court heralds, paladins |
| 5 | `merchant` | Transactional, calculating, aggrieved | traders, guild factors, tax men |
| 6 | `devout` | Fervent, theological, fate-invoking | clerics, priests, templars |
| 7 | `tragic` | Mournful, poetic, world-weary | widows, exiles, the dishonored |
| 8 | `cryptic` | Indirect, riddle-speaking, ominous | mages, oracles, undead scholars |

The fallback system still uses the old numeric index (1–3 map to wimpy/cool/mean
for backward compatibility; 4–8 are new). The AI path ignores numeric indexes
entirely and uses the AI-written messages directly.

---

## Section II: Single Quest Object Vnum

All six current quest vnums (65446–65451) collapse to **one vnum: 65446**.
This object is a neutral template whose strings are overwritten at quest start:

```
#65446
crusade target item~
a crusade item~                   <- overwritten by AI short_descr
A crusade item lies here.~        <- overwritten by AI long_descr
16 16384 16777216 0
0 0 0 0 0 0 0 0 0 0
1
```

At quest start the server sets `quest_object->short_descr` and
`quest_object->long_descr` to AI-generated strings using `free_string()` /
`str_dup()` (same pattern used for `quest_mob->long_descr` today). On
`clear_crusade()` the object is extracted as today — no cleanup needed since
the instance is destroyed.

Config defines simplify to:
```c
#define OBJ_VNUM_QUEST_ITEM 65446
/* Keep MIN/MAX for backward compat with quests/commands.c filter */
#define OBJ_VNUM_QUEST_MIN  65446
#define OBJ_VNUM_QUEST_MAX  65446
```

---

## Section III: AI Helper Script

A new file `src/quests/crusade_ai_gen.py` is invoked by the server. It receives
context JSON on stdin, calls Claude, and writes results to a temp file.

### Input (stdin JSON)
```json
{
  "mob_name":       "a dark stalker",
  "mob_long_descr": "A dark stalker lurks here, seeking its prey.",
  "mob_level":      30,
  "mob_race":       "human",
  "area_name":      "The Forgotten Dungeon",
  "area_descr":     "You hear the dripping of water.",
  "tier":           2,
  "output_path":    "/tmp/crusade_ai_12345.json"
}
```

### System prompt (baked into script)
The script sends a system prompt summarizing the world lore — the five cities,
Solar Court, Crusader tradition, Keeper of Measures civilization, key factions
— drawn from `docs/lore/`. This gives the AI grounding for era-appropriate
vocabulary and item names.

### Output (written to `output_path`)
```json
{
  "personality":    "cryptic",
  "item_short":     "a salt-glass obelisk fragment",
  "item_long":      "A piece of inscribed salt-glass lies here, its surface etched with old Measure Script.",
  "messages": [
    { "m1": "Something of mine passes through many hands. It should not.", "m2": "" },
    { "m1": "I have named what was taken. I wait for those who listen.", "m2": "" },
    ...
    { "m1": "It has returned. Whether the one who brought it understood it... is another matter.", "m2": "" }
  ]
}
```

The `messages` array is always exactly 17 entries with `m1` and `m2` keys,
matching the existing `qmessage_type` layout. The script instructs Claude to
follow the timer semantics: messages 0–6 announce the theft, 7–14 name the
thief and escalate, message 15 is the expiry, message 16 is the completion.

---

## Section IV: C-Side Changes

### New globals (`src/headers/globals.h`)
```c
extern bool   quest_ai_pending;    /* AI generation in flight */
extern char   quest_ai_tmpfile[256]; /* path to results file */
extern pid_t  quest_ai_pid;         /* child PID for cleanup */
```

Matching definitions in `src/db.c`.

### New quest state struct for AI messages (`src/quests/crusade.c`)

Dynamic per-quest message storage replaces the compile-time const array:

```c
/* Allocated at quest start, freed in clear_crusade() */
struct quest_message_slot {
    char *m1;
    char *m2;
};
static struct quest_message_slot ai_messages[17];
static bool using_ai_messages = FALSE;
```

`crusade_inform()` and `crusade_complete()` check `using_ai_messages` first;
if set they use `ai_messages[]`, otherwise they use the existing static
`qmessages[]` fallback.

### New function: `start_ai_crusade_generation()`

Called in place of immediately running `load_quest_object()` after quest_mob
and quest_target are selected. Builds the JSON context, forks, and execs
`crusade_ai_gen.py`. Sets `quest_ai_pending = TRUE`.

```c
static void start_ai_crusade_generation(sh_int tier)
{
    /* build context JSON, write to pipe, fork + exec script */
    /* set quest_ai_pending = TRUE, quest_ai_tmpfile, quest_ai_pid */
}
```

### Modified `update_crusade()` (called from `update.c` each tick)

Currently `update_crusade()` calls `crusade_inform()` every minute. It gains
a new fast path:

```c
if (quest_ai_pending) {
    if (/* tmpfile exists */ ) {
        read_ai_crusade_results();   /* populate ai_messages[], item strings */
        quest_ai_pending = FALSE;
        /* now load quest object, set descrs, set quest=TRUE, announce */
    } else if (/* >10 seconds elapsed */ ) {
        /* kill child, fall back to static system */
        quest_ai_pending = FALSE;
        load_quest_object_static(quest_target, quest_personality);
        quest = TRUE;
    }
    return;  /* don't advance quest_timer yet */
}
```

### `clear_crusade()` additions

Frees `ai_messages[i].m1` / `.m2` for all 17 slots if `using_ai_messages`.
Kills the child process if `quest_ai_pending` is still set (quest was stopped
before generation finished). Resets `quest_ai_pending`, `using_ai_messages`.

### `load_quest_object()` changes

Simplified to always use `OBJ_VNUM_QUEST_ITEM`. The separate
`load_quest_object_static()` variant is what the fallback path uses (same
body, no tier dispatch needed since there's now one vnum).

After AI results are read, the object strings are set:
```c
free_string(quest_object->short_descr);
quest_object->short_descr = str_dup(ai_result.item_short);
free_string(quest_object->long_descr);
quest_object->long_descr = str_dup(ai_result.item_long);
```

---

## Section V: Lore Context in the System Prompt

The Python script embeds a condensed world summary (~800 tokens) covering:
- The five cities (Midgaard, Akhenet, Kowloon, etc.) and their characters
- Solar Court / Keeper of Measures history
- Key factions (clans, Crusader Corps, Assassin tradition)
- Tone guidance: gritty-medieval, no modern idiom, no meta-humor
- The 17-message structure and timer semantics explained explicitly

This is enough for the AI to generate an item like "a salt-glass obelisk
fragment" for a Keeper-themed mob, or "a blood-iron campaign seal" for a Solar
Court veteran, rather than something anachronistic.

---

## Affected Files

| File | Change |
|------|--------|
| `area/ceiling.are` | Remove 65447–65451; rewrite 65446 as neutral template |
| `src/headers/config.h` | Collapse vnum range to single define; add 8 fallback personality defines |
| `src/headers/globals.h` | Add `quest_ai_pending`, `quest_ai_tmpfile`, `quest_ai_pid` |
| `src/db.c` | Define the three new globals |
| `src/quests/crusade.c` | Major rewrite: async generation, dynamic messages, AI result parsing, expanded fallback personalities, single-vnum object loading |
| `src/quests/crusade_ai_gen.py` | New file: helper script calling Claude API |
| `src/tests/test_crusade.c` | Add tests for fallback personality selection, AI result parsing, timeout handling |

---

## Trade-offs

- **Latency**: Quests start ~1–3 seconds later than today (hidden by the
  "pending" state; players see nothing until generation completes).
- **API cost**: One Claude API call per auto-crusade. At typical crusade
  frequency (every 3–6 minutes) this is modest.
- **Failure modes**: API down, network error, malformed JSON → clean fallback
  to static messages. The fallback covers all 8 personalities, not just 3.
- **Temp file cleanup**: The server writes to `/tmp/crusade_ai_<pid>.json`
  and removes it after reading. If the server crashes mid-generation the file
  is orphaned but harmless.
- **Child process leak**: `clear_crusade()` kills the child PID if still
  running. A zombie is reaped with `waitpid(WNOHANG)` in the tick handler.
- **Unit testability**: `read_ai_crusade_results()` takes a file path, making
  it fully testable by pointing at a fixture JSON file.
- **Prior proposal superseded**: The tier-dispatch vnum proposal
  (`crusade-target-objects.md`) is superseded by this one. It should be
  moved to `docs/proposals/completed/` as "superseded" once this is approved.
