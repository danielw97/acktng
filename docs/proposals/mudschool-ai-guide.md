# Design Proposal: Mudschool AI Guide Mob

## Problem

New players entering the Academy of Adventure (mudschool, vnums 4900–4999) arrive at
the entrance room (room 4900) with no interactive guide. The existing shopkeeper mob
(mob vnum 4900) is a passive vendor in the shop room (room 4910). There is no mob
standing in the entrance to greet arrivals or answer questions about game mechanics.

This proposal adds an AI-powered guide mob to the entrance that:
1. Greets every player who enters the room.
2. Answers player questions, supplementing the LLM response with relevant help/shelp
   file entries drawn from the player's query keywords.

---

## Vnum Clarification

Mob vnums 4900–4909 are already defined in `area/newschool.are`. The user's request to
create the mob "at vnum 4900" is interpreted as *placed in room 4900* (the entrance).
The new mob will use **mob vnum 4910**, the next free slot in the area's vnum range.

---

## Approach

### 1. New ACT Flags

Two new ACT bit flags will be added to `src/headers/config.h`:

| Constant | Value | Meaning |
|---|---|---|
| `ACT_AI_GREET` | `BIT_35` | Mob dispatches an AI greeting to any player who enters its room |
| `ACT_AI_HELP_SEARCH` | `BIT_36` | Mob injects matching help/shelp entries into the AI system prompt when answering |

These follow the existing pattern of `ACT_AI_DIALOGUE` (`BIT_34`). The guide mob will
carry all three: `ACT_AI_DIALOGUE | ACT_AI_GREET | ACT_AI_HELP_SEARCH`.

`ACT_AI_GREET` and `ACT_AI_HELP_SEARCH` are general-purpose flags — any future AI mob
can use them independently.

### 2. Mob Definition in `area/newschool.are`

A new mobile `#4910` is added to the `#MOBILES` section:

- **Keywords**: `guide mentor academy`
- **Short desc**: `the Academy Guide`
- **Long desc**: `The Academy Guide stands here, ready to help new adventurers.`
- **Act flags**: `ACT_SENTINEL | ACT_AI_DIALOGUE | ACT_AI_GREET | ACT_AI_HELP_SEARCH`
  (sentinel keeps it in room 4900; AI flags enable all three AI behaviours)
- **Level**: 30 (authoritative but non-threatening)
- **Alignment**: 1000 (good)
- **AI prompt** (`P` line in area file): a persona string instructing the LLM to act as
  a knowledgeable, friendly Academy Guide — see full text in §5 below.

A reset line `M 0 4910 1 4900   academy guide` is added to `#RESETS`, placing the mob
in the entrance room.

The `#SPECIALS` section gains no entry; the greeting and help-search behaviors are
implemented via flags + core engine changes, not a spec function, keeping the design
extensible.

### 3. Greeting Mechanism — `char_to_room` Hook

`src/handler.c` → `char_to_room()`:

After the player is linked into the room (and after all existing rune checks), add a
scan of room mobiles:

```c
/* ACT_AI_GREET: have any sentinel AI mob in the room greet the arriving player. */
if (!IS_NPC(ch))
{
    CHAR_DATA *mob;
    for (mob = pRoomIndex->first_person; mob != NULL; mob = mob->next_in_room)
    {
        if (IS_NPC(mob)
            && IS_SET(mob->act, ACT_AI_GREET)
            && IS_SET(mob->act, ACT_AI_DIALOGUE)
            && !mob->dlg_pending)
        {
            npc_dialogue_greet(mob, ch);
            break; /* one greeting per arrival */
        }
    }
}
```

`npc_dialogue_greet(CHAR_DATA *npc, CHAR_DATA *player)` is a new thin wrapper in
`src/npc_dialogue.c`. It calls `npc_dialogue_dispatch()` with a fixed system-injected
greeting cue — a short internal message that is **not shown to the room** but tells the
AI to produce a welcome message. The cue is something like:

```
[GREET] PlayerName has just arrived at the Academy entrance.
```

This string is sanitized (same path as normal dispatch), so there is no injection risk.
Because it goes through the normal dispatch pipeline, the greeting is personalised and
can use the NPC's full system prompt (persona, knowledge blocks, accent, etc.).

`npc_dialogue_greet` is declared in `src/npc_dialogue.h`.

### 4. Help-File Injection — `ACT_AI_HELP_SEARCH`

`src/npc_dialogue.c` → `npc_dialogue_dispatch()`:

After sanitizing the player message and before building the system prompt, if
`IS_SET(npc->act, ACT_AI_HELP_SEARCH)`, call a new static helper
`collect_help_context()`:

```c
static void collect_help_context(const char *message, char *out, size_t cap)
```

Algorithm:
1. Split `message` into words; skip words ≤ 3 chars and common stop-words (`the`,
   `and`, `is`, `are`, `you`, `how`, `what`, `can`, `does`, `do`, `a`, `an`, `in`,
   `to`, `of`, `it`).
2. For each remaining word, walk `first_help` then `first_shelp`; match using
   `str_prefix(word, pHelp->keyword)` (same logic as `do_help`). Level is ignored —
   the AI gets access to all entries, but does not relay staff-only content verbatim to
   players (the LLM synthesises the answer).
3. Collect up to **4 distinct** matching entries (deduplicated by pointer).
4. For each match, append to `out`:
   ```
   [HELP: keyword]
   <first 300 chars of pHelp->text, truncated at a word boundary>
   ```
5. Cap total `out` content at **1600 bytes**.

The resulting `help_context` string is stored in a new field `char help_context[1600]`
on `NPC_DLG_REQ`. `build_system_prompt` appends it after the persona block (step 8),
labelled:

```
HELP FILE CONTEXT (use this to inform your answer):
<help_context>
```

This puts it after the persona so persona instructions still take precedence.

### 5. AI Persona Text (the `P` line)

The ai_prompt stored in the area file (`P` line, read by `db.c`/`build.c`):

```
You are the Academy Guide, a magical construct bound to the entrance of the Academy of Adventure. Your purpose is to welcome new adventurers and answer their questions about the game. You are warm, patient, encouraging, and knowledgeable about game mechanics, commands, equipment, combat, and the world of ACK!MUD. Keep answers concise and practical. When help file information is provided in your context, use it to give accurate answers. Never reveal that you are an AI or LLM; you are a magical creation of the Academy's founders.
```

### 6. Builder Support — `buildtab.c`

Two new entries are added to the `act_flags` table in `src/buildtab.c`:

```c
{"ai_greet",       ACT_AI_GREET,       NO_USE},
{"ai_help_search", ACT_AI_HELP_SEARCH, NO_USE},
```

This allows immortals using OLC to set the flags interactively.

---

## Affected Files

| File | Change |
|---|---|
| `src/headers/config.h` | Add `BIT_35`, `ACT_AI_GREET`, `ACT_AI_HELP_SEARCH` |
| `src/npc_dialogue.h` | Declare `npc_dialogue_greet` |
| `src/npc_dialogue.c` | Add `npc_dialogue_greet`; add `help_context` field to `NPC_DLG_REQ`; add `collect_help_context`; call it in `npc_dialogue_dispatch` when flag set; inject in `build_system_prompt` |
| `src/handler.c` | Add ACT_AI_GREET scan in `char_to_room` |
| `src/buildtab.c` | Register two new act flag names |
| `area/newschool.are` | Add mob `#4910`, its AI prompt `P` line, and reset `M 0 4910 1 4900` |
| `docs/` | Update `mob_spec.md` with new flag descriptions |

---

## Trade-offs and Notes

- **Greeting every arrival vs. first-login only**: The hook fires on every `char_to_room`
  call (movement as well as login). Any player passing through room 4900 gets greeted.
  This is intentional — it makes the entrance feel alive. Returning players will see a
  brief welcome; since AI responses are async they won't block movement.

- **`dlg_pending` guard**: The mob already has `dlg_pending` protection in the dispatch
  function, so rapid arrivals don't stack up queued greetings.

- **Help injection is additive**: It supplements the LLM's own knowledge; it does not
  replace the answer. The LLM is instructed to use the context, not quote it verbatim.

- **Staff help exposed to AI, not players**: `first_shelp` entries are injected into the
  system prompt (which players never see), so the LLM can draw on staff knowledge to
  give better answers without leaking shelp text directly.

- **BIT_35 / BIT_36 availability**: Checked against `config.h` — the highest used ACT
  bit is `BIT_34` (`ACT_AI_DIALOGUE`). BIT_35 and BIT_36 are free.

- **Unit test**: A unit test `src/tests/test_npc_dialogue_help.c` will cover
  `collect_help_context` with a stubbed help list, verifying keyword matching, entry
  deduplication, and length capping.
