# Design Proposal: Mudschool AI Guide Mob

## Problem

New players entering the Academy of Adventure (mudschool, vnums 4900–4999) arrive at
the entrance room (room 4900) with no interactive guide. The existing shopkeeper mob
(mob vnum 4900) is a passive vendor in the shop room (room 4910). There is no mob
standing in the entrance to greet arrivals or answer questions about game mechanics.

This proposal adds an AI-powered guide mob to the entrance that:
1. Greets every player who enters the room (via a spec function that detects arrivals).
2. Answers player questions, supplementing the LLM response with relevant help/shelp
   file entries drawn from the player's query keywords.

---

## Vnum Clarification

Mob vnums 4900–4909 are already defined in `area/newschool.are`. The user's request to
create the mob "at vnum 4900" is interpreted as *placed in room 4900* (the entrance).
The new mob will use **mob vnum 4910**, the next free slot in the area's vnum range.

---

## Approach

### 1. New ACT Flag — `ACT_AI_HELP_SEARCH`

One new ACT bit flag is added to `src/headers/config.h`:

| Constant | Value | Meaning |
|---|---|---|
| `ACT_AI_HELP_SEARCH` | `BIT_35` | Mob injects matching help/shelp entries into the AI system prompt when answering player questions |

This follows the pattern of `ACT_AI_DIALOGUE` (`BIT_34`). BIT_35 is the next free slot.

The greeting behaviour is handled entirely by the spec function (§3) rather than a flag
or a `char_to_room` hook — no `ACT_AI_GREET` flag is needed.

### 2. Mob Definition in `area/newschool.are`

A new mobile `#4910` is added to the `#MOBILES` section:

- **Keywords**: `guide mentor academy`
- **Short desc**: `the Academy Guide`
- **Long desc**: `The Academy Guide stands here, ready to help new adventurers.`
- **Act flags**: `ACT_SENTINEL | ACT_AI_DIALOGUE | ACT_AI_HELP_SEARCH`
  (sentinel keeps it in room 4900; AI flags enable LLM dialogue and help injection)
- **Level**: 30 (authoritative but non-threatening)
- **Alignment**: 1000 (good)
- **AI prompt** (`P` line): persona string — see §5 below.

`#RESETS` gains: `M 0 4910 1 4900   academy guide`

`#SPECIALS` gains: `M 4910 spec_mudschool_guide`

### 3. Greeting Mechanism — `spec_mudschool_guide`

A new spec function is added at `src/ai/spec_mudschool_guide.c`. It follows the same
pattern as `spec_sage` and `spec_mayor`: called once per mobile update tick for the mob.

**Greeting detection** — the function maintains a small static circular buffer of
recently-greeted character names (up to 16 entries). Each tick it walks
`ch->in_room->first_person` looking for players. Any player whose name is not in the
buffer gets a greeting dispatched and their name added to the buffer. When the buffer
fills, the oldest entry is evicted (ring semantics), allowing a player who left and
returned after some time to be greeted again.

```c
bool spec_mudschool_guide(CHAR_DATA *ch)
{
    static char greeted[16][50];
    static int  greeted_head = 0;
    static int  greeted_count = 0;

    CHAR_DATA *vch;

    if (ch->dlg_pending)
        return FALSE;

    for (vch = ch->in_room->first_person; vch != NULL; vch = vch->next_in_room)
    {
        int i;
        bool found = FALSE;

        if (IS_NPC(vch))
            continue;

        for (i = 0; i < greeted_count; i++)
            if (!str_cmp(vch->name, greeted[(greeted_head - 1 - i + 16) % 16]))
                { found = TRUE; break; }

        if (!found)
        {
            char cue[80];
            snprintf(cue, sizeof(cue), "[GREET] %s has just arrived.", vch->name);
            npc_dialogue_dispatch(ch, vch, cue);
            strncpy(greeted[greeted_head], vch->name, 49);
            greeted[greeted_head][49] = '\0';
            greeted_head = (greeted_head + 1) % 16;
            if (greeted_count < 16) greeted_count++;
            return TRUE;
        }
    }

    return FALSE;
}
```

The greeting cue `[GREET] Name has just arrived.` is passed as the `message` argument
to `npc_dialogue_dispatch`. Because it starts with `[GREET]`, the AI persona will
produce a welcome response. The cue goes through the normal sanitization path before
reaching the LLM, so no injection risk is introduced.

**Registration** — two entries are added to `src/special.c`:
- Forward lookup (`spec_lookup`): `if (!str_cmp(name, "spec_mudschool_guide")) return spec_mudschool_guide;`
- Reverse lookup (`spec_name`): `if (func == spec_mudschool_guide) return "spec_mudschool_guide";`

**Declaration** — `DECLARE_SPEC_FUN(spec_mudschool_guide);` is added to
`src/headers/special.h`.

**Makefile** — `src/ai/spec_mudschool_guide.c` is compiled into the build via the
existing `ai/` directory pattern in the Makefile.

### 4. Help-File Injection — `ACT_AI_HELP_SEARCH`

`src/npc_dialogue.c` → `npc_dialogue_dispatch()`:

After sanitizing the player message and before building the system prompt, if
`IS_SET(npc->act, ACT_AI_HELP_SEARCH)`, call a new static helper
`collect_help_context()`:

```c
static void collect_help_context(const char *message, char *out, size_t cap);
```

Algorithm:
1. Split `message` into words; skip words ≤ 3 chars and common stop-words (`the`,
   `and`, `is`, `are`, `you`, `how`, `what`, `can`, `does`, `do`, `a`, `an`, `in`,
   `to`, `of`, `it`). Also skip the `[GREET]` prefix used by the spec function.
2. For each remaining word, walk `first_help` then `first_shelp`; match using
   `str_prefix(word, pHelp->keyword)` (same logic as `do_help`). Level is ignored —
   the AI gets access to all entries but never relays staff-only text verbatim.
3. Collect up to **4 distinct** matching entries (deduplicated by pointer).
4. For each match, append to `out`:
   ```
   [HELP: keyword]
   <first 300 chars of pHelp->text, hard-truncated>
   ```
5. Cap total `out` content at **1600 bytes**.

The result is stored in a new field `char help_context[1600]` on `NPC_DLG_REQ`.
`build_system_prompt` appends it after the persona block, labelled:

```
HELP FILE CONTEXT (use this to inform your answer):
<help_context>
```

Appending after the persona ensures persona instructions take precedence.

### 5. AI Persona Text (the `P` line)

```
You are the Academy Guide, a magical construct bound to the entrance of the Academy of Adventure. Your purpose is to welcome new adventurers and answer their questions about the game. You are warm, patient, encouraging, and knowledgeable about game mechanics, commands, equipment, combat, and the world of ACK!MUD. Keep answers concise and practical. When help file information is provided in your context, use it to give accurate answers. Never reveal that you are an AI or LLM; you are a magical creation of the Academy's founders.
```

### 6. Builder Support — `buildtab.c`

One new entry is added to the `act_flags` table in `src/buildtab.c`:

```c
{"ai_help_search", ACT_AI_HELP_SEARCH, NO_USE},
```

---

## Affected Files

| File | Change |
|---|---|
| `src/headers/config.h` | Add `BIT_35`, `ACT_AI_HELP_SEARCH` |
| `src/headers/special.h` | Declare `spec_mudschool_guide` |
| `src/ai/spec_mudschool_guide.c` | New spec function (greeting detection + dispatch) |
| `src/special.c` | Register `spec_mudschool_guide` in forward and reverse lookup tables |
| `src/npc_dialogue.c` | Add `help_context` field to `NPC_DLG_REQ`; add `collect_help_context`; call in `npc_dialogue_dispatch` when flag set; inject in `build_system_prompt` |
| `src/buildtab.c` | Register `ai_help_search` act flag |
| `src/Makefile` | Add `spec_mudschool_guide.c` to build (if not already covered by wildcard) |
| `area/newschool.are` | Add mob `#4910`, `P` line AI prompt, reset `M 0 4910 1 4900`, special `M 4910 spec_mudschool_guide` |
| `src/tests/test_npc_dialogue_help.c` | Unit test for `collect_help_context` |
| `docs/mob_spec.md` | Document `ACT_AI_HELP_SEARCH` and `spec_mudschool_guide` |

No changes to `src/handler.c` — `char_to_room` is untouched.

---

## Trade-offs and Notes

- **Greeting every arrival**: The spec function fires each tick and detects any player
  in the room not yet in the greeted buffer. This covers both first-login (new player
  placed in room 4900 by `login.c`) and returning players. Since AI responses are async
  they don't block movement.

- **Static greeted buffer**: A 16-entry ring buffer is simple and sufficient — mudschool
  is a low-traffic area. Players who leave and return after 16 subsequent arrivals will
  be re-greeted, which is acceptable (and arguably desirable for returning players).

- **Greeting cue sanitization**: The `[GREET]` cue goes through the normal
  `npc_dialogue_sanitize_input` path. The injection-keyword scanner checks for
  role-boundary strings; `[GREET]` does not match any of those patterns.

- **`dlg_pending` guard**: The spec function checks `ch->dlg_pending` at entry and
  returns immediately if set, so back-to-back arrivals don't stack requests.

- **Help injection is additive**: It supplements the LLM's knowledge; the LLM
  synthesises rather than quotes the help text verbatim.

- **Staff help exposed to AI only**: `first_shelp` entries go into the system prompt
  only — never echoed raw to the player channel.

- **BIT_35 availability**: Confirmed against `config.h` — highest used ACT bit is
  `BIT_34`. BIT_35 is free.
