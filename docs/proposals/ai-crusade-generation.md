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

---

## Section VI: Player Conversation with the Quest Mob

### Discovery

The codebase already has a complete AI NPC dialogue system in `src/npc_dialogue.c`:

- **Worker thread** (`pthread`) makes blocking HTTP calls to a `tng-ai`
  service without freezing the game loop
- **`ACT_AI_DIALOGUE` mob flag** on any NPC causes `do_say()` to dispatch
  player speech to the AI via `npc_dialogue_dispatch()`
- **Per-instance conversation history** (`dlg_state` on `CHAR_DATA`) gives
  multi-turn coherence
- **`npc_dialogue_deliver()`** is called every tick to push AI responses out
  as `do_say()` calls
- **`ask_quest_question()`** in `act_comm.c` is the current static
  keyword-match fallback (called from the crusade channel handler when
  players say exact phrases like "who is the thief?")

The conversation feature therefore requires **no new threading, no fork/pipe,
no Python helper** for this part — only wiring the quest mob into the existing
system and injecting quest context into its system prompt.

### Approach

#### 1. New field on CHAR_DATA: `quest_extra_prompt`

```c
/* In ack.h, CHAR_DATA struct */
char *quest_extra_prompt;   /* NULL normally; set for quest mob during active quest */
```

Allocated with `str_dup()` at quest start, freed with `free_string()` in
`clear_crusade()`. Parallels the existing `long_descr_orig` pattern.

#### 2. `build_system_prompt()` appends the quest context

At the end of `build_system_prompt()` in `npc_dialogue.c`, before the
behavioral guardrails line:

```c
/* Quest context — injected when this mob is the active quest giver */
if (npc->quest_extra_prompt != NULL && npc->quest_extra_prompt[0] != '\0')
{
    safe_append(buf, cap, "\nQUEST CONTEXT:\n");
    safe_append(buf, cap, npc->quest_extra_prompt);
    safe_append(buf, cap, "\n");
}
```

#### 3. Quest context string is built at quest start

Built in `generate_auto_crusade()` (and the `iquest start` path) immediately
after the quest object strings are determined — at the point where the mob's
`long_descr` is already being rewritten. The content is:

```
You are currently participating in a crusade. An item belonging to you has
been stolen. Here is what you know:

- Your stolen item: [quest_object->short_descr]
- Item description: [quest_object->long_descr] (omit the "lies here" suffix)
- The thief (revealed after timer >= 7): [quest_target->short_descr], last
  seen in [quest_target->in_room->area->name]
- Quest timer: [quest_timer]/15 minutes elapsed

Respond to questions about the theft naturally and in character. You are
distressed/indifferent/enraged (per your personality) about the loss.
If asked who stole it before timer 7, say you don't know yet.
Do not break character. Do not use modern language.
```

The thief information is conditionally included in the string only after
`quest_timer >= 7` — but since the string is built once at quest start when
`quest_timer == 0`, it should instead say "you don't know who stole it yet"
and the AI is instructed to say the same until the thief is announced.

A simpler and correct approach: the context string is **rebuilt each time
`crusade_inform()` fires** (each minute), so it reflects the current timer
and whether the thief is known. Since `build_system_prompt` is called per
request, the updated `quest_extra_prompt` is always current.

#### 4. `ACT_AI_DIALOGUE` set on quest mob at quest start

```c
/* In generate_auto_crusade() / do_iquest start, alongside existing ACT_ sets */
SET_BIT(quest_mob->act, ACT_AI_DIALOGUE);
```

Removed in `clear_crusade()` alongside the other flag cleanups:

```c
REMOVE_BIT(quest_mob->act, ACT_AI_DIALOGUE);
```

This makes `do_say()` automatically dispatch player speech to `npc_dialogue_dispatch()`
for any player in the quest mob's room — using the full existing pipeline
with history, accent, lore, and race inclination.

#### 5. Replace `ask_quest_question()` with AI dispatch

`ask_quest_question()` is called from the crusade channel handler
(`do_crusade_channel` in `act_comm.c:756`) when players type exact keyword
phrases. This becomes redundant once the mob has `ACT_AI_DIALOGUE` — players
can just `say` their questions naturally.

The static function is replaced by a single call to `npc_dialogue_dispatch()`
if the quest mob is reachable, routing the question through the AI pipeline
instead:

```c
/* Replace ask_quest_question() call site in act_comm.c */
if (quest && quest_mob != NULL && !IS_SET(quest_mob->act, ACT_PET))
    npc_dialogue_dispatch(quest_mob, ch, argument);
```

If the quest mob is dead or `dlg_pending`, the dispatch is silently dropped
(existing `npc_dialogue_dispatch()` already guards against stacking).

The `ask_quest_question()` function definition is removed entirely.

#### 6. Context string update each crusade tick

`crusade_inform()` fires every minute. After updating `quest_timer`, it
rebuilds `quest_mob->quest_extra_prompt` so the next player conversation
request carries current state (timer, thief identity if now revealed):

```c
void crusade_update_quest_context(void)
{
    char buf[2048];
    /* build context string reflecting current quest_timer and quest_target */
    if (quest_mob->quest_extra_prompt != NULL)
        free_string(quest_mob->quest_extra_prompt);
    quest_mob->quest_extra_prompt = str_dup(buf);
}
```

Called at the end of `crusade_inform()`.

### What Players Experience

A player walking into the quest mob's room and typing:

```
say Do you know who took it?
```

Gets a response driven by the mob's full persona (race, accent, lore flags,
ai_prompt if any) **plus** the quest context, in the mob's established voice.
The mob might reply:

> *A dark stalker rasps, 'The trail goes cold. Something moved through here
> — I sensed it take the shard. I do not yet know its face.'*

Or for a noble-tier NPC:

> *Sir Aldric says, 'I have dispatched my steward to inquire. Until the
> thief's name is confirmed, I shall not give you false intelligence.'*

Multiple players in the room can each speak to the mob. The conversation
history is per-NPC-instance, so coherence is maintained across the multi-turn
exchange. History expires after `DIALOGUE_HISTORY_EXPIRY` seconds of silence
(existing behavior).

### Additional Affected Files

| File | Change |
|------|--------|
| `src/headers/ack.h` | Add `char *quest_extra_prompt` to `CHAR_DATA` |
| `src/npc_dialogue.c` | Append `quest_extra_prompt` in `build_system_prompt()` |
| `src/quests/crusade.c` | Set/clear `ACT_AI_DIALOGUE` and `quest_extra_prompt` at quest start/end; add `crusade_update_quest_context()`; call it from `crusade_inform()` |
| `src/act_comm.c` | Replace `ask_quest_question()` call with `npc_dialogue_dispatch()`; remove `ask_quest_question()` definition and declaration |
| `src/tests/test_crusade.c` | Add tests for `crusade_update_quest_context()` output at timer 0, 7, and 15 |

### Trade-offs

- **dlg_pending contention**: If the AI worker is busy (e.g. processing
  another NPC's response), `npc_dialogue_dispatch()` drops the request
  silently (existing behavior). Players get no response rather than a wrong
  one — acceptable.
- **Shared pIndexData unchanged**: `quest_extra_prompt` lives on the
  `CHAR_DATA` instance, not `pIndexData`, so other instances of the same
  mob template are unaffected.
- **History coherence**: The quest mob accumulates conversation history from
  all players in the room. Since players can see each other's `say` messages,
  this is natural — the mob is having a multi-party conversation.
- **No lore risk**: The existing `npc_dialogue_sanitize_input()` injection
  defence applies to player speech as before.
