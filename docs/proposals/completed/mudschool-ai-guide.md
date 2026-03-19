# Design Proposal: Mudschool AI Guide Mob

## Problem

New players entering the Academy of Adventure (mudschool, vnums 4900–4999) arrive at
the entrance room (room 4900) with no interactive guide. The existing shopkeeper mob
(mob vnum 4900) is a passive vendor in the shop room (room 4910). There is no mob
standing in the entrance to greet arrivals or answer questions about game mechanics.

This proposal adds an AI-powered guide mob to the entrance that:
1. Greets every player who enters the room via an event-driven room-entry spec handler.
2. Answers player questions, supplementing the LLM response with relevant help/shelp
   file entries — triggered by the presence of an `entry_fun`, not a new ACT bit.

No new ACT flags are introduced.

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
#define DECLARE_ENTRY_FUN(fun) bool fun(CHAR_DATA *mob, CHAR_DATA *ch)
```

This parallels the existing `SPEC_FUN` type but accepts two arguments: the NPC that
owns the handler, and the player who just entered the room.

`MOB_INDEX_DATA` gains one new field alongside `SPEC_FUN *spec_fun`:

```c
ENTRY_FUN *entry_fun;   /* called by char_to_room when a player enters the mob's room */
```

`CHAR_DATA` gains a mirrored instance pointer, copied from the index by
`create_mobile()` in `handler.c` the same way `spec_fun` is.

### 2. `char_to_room` Hook

`src/handler.c` → `char_to_room()`: after the existing rune checks, when the entering
character is a player, walk the room's NPC list and fire any `entry_fun`:

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

### 3. Mob Definition in `area/newschool.are`

A new mobile `#4910` is added to the `#MOBILES` section:

- **Keywords**: `guide mentor academy`
- **Short desc**: `the Academy Guide`
- **Long desc**: `The Academy Guide stands here, ready to help new adventurers.`
- **Act flags**: `ACT_SENTINEL | ACT_AI_DIALOGUE` — both pre-existing flags, no new bits
- **Level**: 30 (authoritative but non-threatening)
- **Alignment**: 1000 (good)
- **AI prompt** (`P` line): persona string — see §5 below.

`#RESETS` gains: `M 0 4910 1 4900   academy guide`

`#SPECIALS` uses a new `E` prefix for entry functions:

```
E 4910 spec_mudschool_guide
```

The existing `M` prefix registers `spec_fun`; the new `E` prefix registers `entry_fun`.
Both are parsed in `db.c`'s `#SPECIALS` loader with one additional `case 'E':`.

### 4. `spec_mudschool_guide` — the Entry Handler

New file `src/ai/spec_mudschool_guide.c`:

```c
bool spec_mudschool_guide(CHAR_DATA *ch, CHAR_DATA *player)
{
    if (!IS_SET(ch->act, ACT_AI_DIALOGUE) || ch->dlg_pending)
        return FALSE;

    npc_dialogue_dispatch(ch, player, "[GREET] A new adventurer has arrived.");
    return TRUE;
}
```

The `[GREET]` prefix cues the AI persona to produce a welcome. The message passes
through the normal `npc_dialogue_sanitize_input` path. The `dlg_pending` guard prevents
stacking if two players enter in rapid succession.

**Registration** in `src/special.c` — a parallel lookup pair:

```c
ENTRY_FUN *entry_lookup(const char *name)
{
    if (!str_cmp(name, "spec_mudschool_guide"))
        return spec_mudschool_guide;
    return NULL;
}

const char *entry_name(ENTRY_FUN *func)
{
    if (func == spec_mudschool_guide)
        return "spec_mudschool_guide";
    return "none";
}
```

**Declaration** in `src/headers/special.h`:

```c
DECLARE_ENTRY_FUN(spec_mudschool_guide);
ENTRY_FUN *entry_lookup(const char *name);
const char *entry_name(ENTRY_FUN *func);
```

### 5. Help-File Injection — `KNOW_HELPS` knowledge bit

A new knowledge bit is added to `src/headers/config.h` alongside the existing
`KNOW_*` topic bits:

```c
#define KNOW_HELPS (1 << 10)   /* search help/shelp files at dispatch time */
```

`KNOW_HELPS` does **not** map to a `.kb` topic file and does not require expanding
`NUM_KNOW_FLAGS` (which controls the `topic_blocks` array used for `.kb` loading). It
is checked separately in `npc_dialogue_dispatch()`.

`buildtab.c` gains one entry in `tab_knowledge`:

```c
{"helps", KNOW_HELPS, 0},
```

In `npc_dialogue_dispatch()`, if `npc->pIndexData->ai_knowledge & KNOW_HELPS` and the
message does not start with `[GREET]`, call:

```c
static void collect_help_context(const char *message, char *out, size_t cap);
```

Algorithm:
1. Split `message` into words; skip words ≤ 3 chars and common stop-words.
2. Walk `first_help` then `first_shelp`; match via `str_prefix(word, pHelp->keyword)`.
   Level is ignored — the AI synthesises and never echoes staff text verbatim.
3. Collect up to **4 distinct** entries (deduplicated by pointer).
4. Append `[HELP: keyword]\n<first 300 chars of text>` per match.
5. Cap total at **1600 bytes**.

The result is stored in a new `char help_context[1600]` field on `NPC_DLG_REQ` and
injected into the system prompt after the persona block:

```
HELP FILE CONTEXT (use this to inform your answer):
<help_context>
```

### 6. AI Persona Text (the `P` line)

```
You are the Academy Guide, a magical construct bound to the entrance of the Academy of Adventure. Your purpose is to welcome new adventurers and answer their questions about the game. You are warm, patient, encouraging, and knowledgeable about game mechanics, commands, equipment, combat, and the world of ACK!MUD. Keep answers concise and practical. When help file information is provided in your context, use it to give accurate answers. Never reveal that you are an AI or LLM; you are a magical creation of the Academy's founders.
```

### 7. Builder Support — `buildtab.c`

No new ACT flag entries are needed. The `KNOW_HELPS` bit is registered in
`tab_knowledge` (see §5). OLC builder support for `entry_fun` assignment is out of
scope; the function is set via the area file `E` prefix in `#SPECIALS`.

---

## Affected Files

| File | Change |
|---|---|
| `src/headers/typedefs.h` | Add `ENTRY_FUN` typedef and `DECLARE_ENTRY_FUN` macro |
| `src/headers/ack.h` | Add `ENTRY_FUN *entry_fun` to `MOB_INDEX_DATA` and `CHAR_DATA` |
| `src/headers/special.h` | Declare `spec_mudschool_guide`, `entry_lookup`, `entry_name` |
| `src/handler.c` | `char_to_room`: fire `entry_fun` on room NPCs when player enters; `create_mobile`: copy `entry_fun` from index |
| `src/db.c` | `#SPECIALS` loader: handle `E` prefix → `entry_lookup` → `pMob->entry_fun` |
| `src/special.c` | Add `entry_lookup` and `entry_name`; register `spec_mudschool_guide` |
| `src/ai/spec_mudschool_guide.c` | New `ENTRY_FUN` implementation |
| `src/npc_dialogue.c` | Add `help_context` to `NPC_DLG_REQ`; add `collect_help_context`; trigger on `entry_fun != NULL`; inject in `build_system_prompt`; skip for `[GREET]` messages |
| `area/newschool.are` | Add mob `#4910`, `P` line, reset, `E 4910 spec_mudschool_guide` special |
| `src/tests/test_npc_dialogue_help.c` | Unit test for `collect_help_context` |
| `docs/mob_spec.md` | Document `ENTRY_FUN`, `entry_fun`, `spec_mudschool_guide` |

---

## Trade-offs and Notes

- **No new ACT bits**: The mob carries only `ACT_SENTINEL | ACT_AI_DIALOGUE`. Help
  injection is conditioned on `entry_fun != NULL`, which is a clean structural signal.

- **Event-driven greeting**: Fires precisely when a player enters the room — no polling,
  no greeted-name buffer.

- **Every arrival is greeted**: Both first-time and returning players receive a welcome.

- **`dlg_pending` guard**: Prevents stacked greeting requests on rapid arrivals.

- **`ENTRY_FUN` is general**: Any future mob can be given an `entry_fun` for room-entry
  behaviour (shop greeters, dungeon guardians, puzzle triggers, etc.).

- **Help injection skips `[GREET]`**: The cue contains no question keywords, so
  `collect_help_context` is skipped for greeting dispatches.

- **Staff help exposed to AI only**: `first_shelp` entries go into the system prompt
  only — never echoed to the player channel.
