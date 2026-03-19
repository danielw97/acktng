# Proposal: Increase NPC AI Response Limit to 1024 Bytes

## Problem

NPC AI responses from `tng-ai` are currently capped at 512 bytes across all
buffers in the dialogue pipeline. This truncates longer, more nuanced NPC
replies before they reach the player.

## Approach

Change every response-path buffer from 512 to 1024 bytes. Input sanitization
(`npc_dialogue_sanitize_input`) handles player-sent text and is intentionally
kept at 512 — it is unaffected by this change.

## Affected Files

### `src/npc_dialogue.c`

| Location | Symbol | Change |
|---|---|---|
| `DIALOGUE_TURN` struct (~line 291) | `char content[512]` | → `[1024]` |
| `DIALOGUE_TURN_COPY` struct (~line 315) | `char content[512]` | → `[1024]` |
| `NPC_DLG_RESP` struct (~line 334) | `char response_text[512]` | → `[1024]` |
| Worker thread local (~line 734) | `char response_text[512]` | → `[1024]` |
| `npc_dialogue_deliver` local (~line 1442) | `char accented[512]` | → `[1024]` |

### `src/npc_dialogue.h`

No size constants live in the header. No changes required.

## Out of Scope

- `npc_dialogue_sanitize_input` and its associated buffers (`sanitized[512]`,
  `user_turn[512]` in `npc_dialogue_dispatch`) handle **player input**, not NPC
  responses. These stay at 512.
- The `size_t cap = 512` hardcode inside `npc_dialogue_sanitize_input` (~line
  1239) also handles player input and is unchanged.

## Trade-offs

- **Memory**: Each `DIALOGUE_TURN` grows by 512 bytes. With `MAX_DIALOGUE_TURNS`
  turns per NPC and potentially many NPCs with active `dlg_state`, total heap
  growth is small (a few KB per NPC at most).
- **Correctness**: `parse_json_response` and `call_tngai` already receive `cap`
  as a parameter, so they automatically benefit from the larger buffer.
- **No protocol changes**: `tng-ai` responses are already allowed to be longer
  than 512 bytes by the HTTP layer; only the destination buffers were the
  bottleneck.

## Unit Tests

The existing unit test for `npc_dialogue` exercises the sanitize path and
dispatch flow. The buffer sizes are tested implicitly via the assert-based
tests. No new test file is needed, but the existing test build will confirm
the structs and locals compile cleanly at 1024.
