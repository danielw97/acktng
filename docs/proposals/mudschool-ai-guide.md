# Design Proposal: Mudschool AI Guide Mob

## Problem

New players entering the Academy of Adventure (mudschool, vnums 4900–4999) arrive at
the entrance room (room 4900) with no interactive guide. The existing shopkeeper mob
(mob vnum 4900) is a passive vendor in the shop room (room 4910). There is no mob
standing in the entrance to greet arrivals or answer questions about game mechanics.

This proposal adds an AI-powered guide mob to the entrance that:
1. Greets every player who enters the room via an event-driven room-entry spec handler.
2. Answers player questions, supplementing the LLM response with relevant help/shelp
   file entries drawn from the player's query keywords.

---

## Vnum Clarification

Mob vnums 4900–4909 are already defined in `area/newschool.are`. The user's request to
create the mob "at vnum 4900" is interpreted as *placed in room 4900* (the entrance).
The new mob will use **mob vnum 4910**, the next free slot in the area's vnum range.

---

## Approach

### 1. New `ENTRY_FUN` Spec Handler Type

A new function-pointer type is added to `src/headers/typedefs.h`:

```c
typedef bool ENTRY_FUN args((CHAR_DATA *mob, CHAR_DATA *ch));
```

This parallels the existing `SPEC_FUN` type but accepts two arguments: the NPC that
owns the handler, and the player who just entered the room.

`MOB_INDEX_DATA` gains one new field (alongside `SPEC_FUN *spec_fun`):

```c
ENTRY_FUN *entry_fun;   /* called by char_to_room when a player enters the mob's room */
```

Similarly, `CHAR_DATA` gains a mirrored instance pointer (just as `spec_fun` is
mirrored from the index):

```c
ENTRY_FUN *entry_fun;
```

`create_mobile()` in `handler.c` copies `pMobIndex->entry_fun` to the instance, the
same way it copies `spec_fun`.

### 2. `char_to_room` Hook

`src/handler.c` → `char_to_room()`: after the existing rune checks, when the entering
character is a player (`!IS_NPC(ch)`), walk the room's NPC list and call any
`entry_fun` present:

```c
if (!IS_NPC(ch))
{
    CHAR_DATA *mob;
    for (mob = pRoomIndex->first_person; mob != NULL; mob = mob->next_in_room)
    {
        if (IS_NPC(mob) && mob->entry_fun != NULL)
            (*mob->entry_fun)(mob, ch);
    }
}
```

This is purely additive — no existing logic is altered.

### 3. New ACT Flag — `ACT_AI_HELP_SEARCH`

One new ACT bit flag is added to `src/headers/config.h`:

| Constant | Value | Meaning |
|---|---|---|
| `ACT_AI_HELP_SEARCH` | `BIT_35` | Mob injects matching help/shelp entries into the AI system prompt when answering |

BIT_35 is the next free slot after `ACT_AI_DIALOGUE` (BIT_34).

### 4. Mob Definition in `area/newschool.are`

A new mobile `#4910` is added to the `#MOBILES` section:

- **Keywords**: `guide mentor academy`
- **Short desc**: `the Academy Guide`
- **Long desc**: `The Academy Guide stands here, ready to help new adventurers.`
- **Act flags**: `ACT_SENTINEL | ACT_AI_DIALOGUE | ACT_AI_HELP_SEARCH`
- **Level**: 30 (authoritative but non-threatening)
- **Alignment**: 1000 (good)
- **AI prompt** (`P` line): persona string — see §6 below.

`#RESETS` gains: `M 0 4910 1 4900   academy guide`

`#SPECIALS` gains an entry using a new `E` prefix for entry functions:
```
E 4910 spec_mudschool_guide
```

(The existing `M` prefix in `#SPECIALS` registers `spec_fun`; the new `E` prefix
registers `entry_fun`. Both are parsed by `db.c`'s `#SPECIALS` loader.)

### 5. `spec_mudschool_guide` — the Entry Handler

A new file `src/ai/spec_mudschool_guide.c` defines an `ENTRY_FUN`:

```c
bool spec_mudschool_guide(CHAR_DATA *ch, CHAR_DATA *player)
{
    if (!IS_SET(ch->act, ACT_AI_DIALOGUE) || ch->dlg_pending)
        return FALSE;

    npc_dialogue_dispatch(ch, player, "[GREET] A new adventurer has arrived.");
    return TRUE;
}
```

The `[GREET]` prefix in the dispatch message cues the AI persona to produce a welcome.
Because it passes through the normal `npc_dialogue_sanitize_input` path there is no
injection risk. The `dlg_pending` guard prevents stacking if two players enter in
rapid succession.

**Registration** — entry functions use a parallel lookup table in `src/special.c`:

```c
/* Forward lookup */
ENTRY_FUN *entry_lookup(const char *name)
{
    if (!str_cmp(name, "spec_mudschool_guide"))
        return spec_mudschool_guide;
    return NULL;
}

/* Reverse lookup */
const char *entry_name(ENTRY_FUN *func)
{
    if (func == spec_mudschool_guide)
        return "spec_mudschool_guide";
    return "none";
}
```

**Declaration** — `src/headers/special.h` gains:

```c
DECLARE_ENTRY_FUN(spec_mudschool_guide);
ENTRY_FUN *entry_lookup(const char *name);
const char *entry_name(ENTRY_FUN *func);
```

where `DECLARE_ENTRY_FUN(x)` expands to `bool x(CHAR_DATA *, CHAR_DATA *)`.

### 6. Help-File Injection — `ACT_AI_HELP_SEARCH`

`src/npc_dialogue.c` → `npc_dialogue_dispatch()`:

After sanitizing the player message, if `IS_SET(npc->act, ACT_AI_HELP_SEARCH)` and the
message does not start with `[GREET]`, call a new static helper:

```c
static void collect_help_context(const char *message, char *out, size_t cap);
```

Algorithm:
1. Split `message` into words; skip words ≤ 3 chars and common stop-words.
2. For each remaining word, walk `first_help` then `first_shelp`; match via
   `str_prefix(word, pHelp->keyword)` (same as `do_help`). Level is ignored — the AI
   synthesises the answer and never echoes staff text verbatim.
3. Collect up to **4 distinct** entries (deduplicated by pointer).
4. For each, append `[HELP: keyword]\n<first 300 chars of text>` to `out`.
5. Cap total at **1600 bytes**.

The result is stored in a new `char help_context[1600]` field on `NPC_DLG_REQ` and
injected into the system prompt after the persona block:

```
HELP FILE CONTEXT (use this to inform your answer):
<help_context>
```

### 7. AI Persona Text (the `P` line)

```
You are the Academy Guide, a magical construct bound to the entrance of the Academy of Adventure. Your purpose is to welcome new adventurers and answer their questions about the game. You are warm, patient, encouraging, and knowledgeable about game mechanics, commands, equipment, combat, and the world of ACK!MUD. Keep answers concise and practical. When help file information is provided in your context, use it to give accurate answers. Never reveal that you are an AI or LLM; you are a magical creation of the Academy's founders.
```

### 8. Builder Support — `buildtab.c`

```c
{"ai_help_search", ACT_AI_HELP_SEARCH, NO_USE},
```

---

## Affected Files

| File | Change |
|---|---|
| `src/headers/typedefs.h` | Add `ENTRY_FUN` typedef; add `DECLARE_ENTRY_FUN` macro |
| `src/headers/ack.h` | Add `ENTRY_FUN *entry_fun` to `MOB_INDEX_DATA` and `CHAR_DATA` |
| `src/headers/config.h` | Add `BIT_35`, `ACT_AI_HELP_SEARCH` |
| `src/headers/special.h` | Declare `spec_mudschool_guide`, `entry_lookup`, `entry_name` |
| `src/handler.c` | `char_to_room`: call `entry_fun` on room NPCs when player enters; `create_mobile`: copy `entry_fun` from index |
| `src/db.c` | `#SPECIALS` loader: handle `E` prefix → `entry_lookup` → `pMob->entry_fun` |
| `src/special.c` | Add `entry_lookup` and `entry_name` functions with `spec_mudschool_guide` registered |
| `src/ai/spec_mudschool_guide.c` | New `ENTRY_FUN` implementation |
| `src/npc_dialogue.c` | Add `help_context` to `NPC_DLG_REQ`; add `collect_help_context`; inject in `build_system_prompt`; skip for `[GREET]` messages |
| `src/buildtab.c` | Register `ai_help_search` act flag |
| `area/newschool.are` | Add mob `#4910`, `P` line, reset, `E 4910 spec_mudschool_guide` special |
| `src/tests/test_npc_dialogue_help.c` | Unit test for `collect_help_context` |
| `docs/mob_spec.md` | Document `ENTRY_FUN`, `ACT_AI_HELP_SEARCH`, `spec_mudschool_guide` |

---

## Trade-offs and Notes

- **Event-driven, not tick-based**: The entry handler fires precisely when a player
  enters the room. No polling, no circular greeted-name buffer, no missed arrivals.

- **Every arrival is greeted**: Any player entering room 4900 (first-time or returning)
  receives a greeting. This is intentional — the entrance should feel alive.

- **`dlg_pending` guard**: Prevents stacked greeting requests if two players enter in
  quick succession. The second player's greeting will be missed in that narrow window,
  which is acceptable.

- **`E` prefix in `#SPECIALS`**: Minimal parser change — the existing `#SPECIALS` loop
  in `db.c` already switches on the leading character (`M` for mob spec). Adding `E` for
  entry functions requires one additional case, no new file section.

- **`ENTRY_FUN` is general**: Any mob can be given an `entry_fun`. Future mobs (shop
  greeters, dungeon guardians, etc.) can reuse the mechanism.

- **Help injection skips `[GREET]`**: The greeting cue contains no player question
  keywords, so skipping `collect_help_context` for it avoids pointless help-file scans.

- **BIT_35 availability**: Confirmed — highest used ACT bit is BIT_34. BIT_35 is free.
