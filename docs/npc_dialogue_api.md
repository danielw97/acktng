# NPC Dialogue API — Architecture and Reference

NPC dialogue is powered by the **tng-ai** service (`POST /v1/chat`) running on
the local network. When a player says something in a room with an
`ACT_AI_DIALOGUE` NPC, the game loop dispatches the message asynchronously to a
worker thread, which calls the tng-ai API, then delivers the response back to
the game loop on the next tick.

---

## Call Flow Diagram

```
PLAYER                 GAME LOOP THREAD                 WORKER THREAD        TNG-AI SERVICE
  │                          │                                │                     │
  │  say hello blacksmith    │                                │                     │
  │─────────────────────────>│                                │                     │
  │                          │ do_say() [act_comm.c]          │                     │
  │                          │ broadcasts to room             │                     │
  │                          │                                │                     │
  │                          │ loop over room NPCs            │                     │
  │                          │ IS_NPC && ACT_AI_DIALOGUE?     │                     │
  │                          │                                │                     │
  │                          │ npc_dialogue_dispatch()        │                     │
  │                          │ ┌────────────────────────┐     │                     │
  │                          │ │ sanitize player input  │     │                     │
  │                          │ │ check injection triggers│    │                     │
  │                          │ │ if triggered → enqueue │     │                     │
  │                          │ │   refusal, return      │     │                     │
  │                          │ │ build system prompt    │     │                     │
  │                          │ │ serialize history      │     │                     │
  │                          │ │ push to REQUEST QUEUE  │     │                     │
  │                          │ │ set dlg_pending = TRUE │     │                     │
  │                          │ └────────────────────────┘     │                     │
  │                          │ returns immediately            │                     │
  │                          │◀───────────────────────────────│                     │
  │                          │ (game loop continues)          │                     │
  │                          │                                │                     │
  │                          │                                │ dequeue request     │
  │                          │                                │──────────────────>  │
  │                          │                                │                     │
  │                          │                                │  POST /v1/chat      │
  │                          │                                │  {model, messages,  │
  │                          │                                │   temperature,      │
  │                          │                                │   max_tokens}       │
  │                          │                                │────────────────────>│
  │                          │                                │                     │
  │                          │                                │  200 OK             │
  │                          │                                │  {"response": "..."}│
  │                          │                                │<────────────────────│
  │                          │                                │                     │
  │                          │                                │ push to RESP QUEUE  │
  │                          │                                │ (or sentinel on err)│
  │                          │                                │                     │
  │                  next tick│                               │                     │
  │                          │ npc_dialogue_deliver()         │                     │
  │                          │ [update.c update_handler()]    │                     │
  │                          │ ┌────────────────────────┐     │                     │
  │                          │ │ drain response queue   │     │                     │
  │                          │ │ validate NPC+player    │     │                     │
  │                          │ │   still share a room   │     │                     │
  │                          │ │ if empty text → discard│     │                     │
  │                          │ │ apply accent_text()    │     │                     │
  │                          │ │ append to NPC history  │     │                     │
  │                          │ │ dlg_pending = FALSE    │     │                     │
  │                          │ │ do_say(npc, response)  │     │                     │
  │                          │ └────────────────────────┘     │                     │
  │                          │                                │                     │
  │  NPC says: "..."         │                                │                     │
  │<─────────────────────────│                                │                     │
```

---

## tng-ai API

**Service:** `http://192.168.1.111:8000`
**Repository:** https://github.com/ackmudhistoricalarchive/tng-ai
**Health check:** `GET /health` → `{"status": "ok"}`

### Request — `POST /v1/chat`

```json
{
  "model": "llama-3.3-70b-versatile",
  "messages": [
    { "role": "system",    "content": "<assembled system prompt>" },
    { "role": "user",      "content": "Arathorn: what do you sell?" },
    { "role": "assistant", "content": "Weapons and armor, traveler." },
    { "role": "user",      "content": "Arathorn: how much for the sword?" }
  ],
  "temperature": 0.7,
  "max_tokens": 100
}
```

| Field | Type | Required | Notes |
|---|---|---|---|
| `messages` | array | yes | `[{role, content}]` — system + history + new user turn |
| `model` | string | no | Defaults to env `DEFAULT_MODEL`; we send `llama-3.3-70b-versatile` |
| `provider` | string | no | Defaults to env `DEFAULT_PROVIDER` (`groq`); omitted |
| `temperature` | float | no | Default 0.7; we send 0.7 |
| `max_tokens` | int | no | Default 1024; we send 100 to cap NPC responses at 1-3 sentences |

User messages are prefixed with the player's name (`"Arathorn: <text>"`) so the
NPC can address the player by name.

### Response

```json
{
  "response": "That blade costs fifty gold. Forged it myself.",
  "model": "llama-3.3-70b-versatile",
  "provider": "groq",
  "usage": {
    "prompt_tokens": 312,
    "completion_tokens": 14,
    "total_tokens": 326
  }
}
```

The game server reads only the `"response"` field. `model`, `provider`, and
`usage` are ignored.

### Error Handling

| Condition | Behaviour |
|---|---|
| curl error (network, DNS, refused) | `response_text` stays empty; worker enqueues empty sentinel |
| HTTP non-200 | `parse_json_response` finds no `"response":` field; returns FALSE; sentinel enqueued |
| Response JSON parses but `response` is empty | sentinel enqueued |
| Any sentinel received | `npc_dialogue_deliver()` clears `dlg_pending`, silently discards — **NPC does nothing** |

The NPC never produces a fallback error message. Silence is the failure mode.

---

## Configuration Constants

Defined in `src/headers/config.h`:

| Constant | Value | Purpose |
|---|---|---|
| `TNGAI_URL` | `http://192.168.1.111:8000/v1/chat` | API endpoint |
| `TNGAI_MODEL` | `llama-3.3-70b-versatile` | Model name sent in every request |
| `TNGAI_TIMEOUT` | `5L` | curl connect + transfer timeout (seconds) |
| `TNGAI_MAX_TOKENS` | `100` | `max_tokens` cap — keeps NPC replies to 1-3 sentences |
| `MAX_DIALOGUE_TURNS` | `8` | Rolling history window per NPC |
| `MAX_REQUEST_TURNS` | `9` | History snapshot size sent with each request (8 + 1 new) |
| `DIALOGUE_HISTORY_EXPIRY` | `300` | Seconds of silence before NPC history resets |

---

## Threading Model

```
GAME LOOP THREAD                        WORKER THREAD
────────────────                        ─────────────
dispatch()
  push NPC_DLG_REQ ──→  req_mutex  ──→  dequeue
  dlg_pending = TRUE     req_cond        call_tngai()  (blocks here)
  return                                 push NPC_DLG_RESP
                                         ←── resp_mutex ──
tick: deliver()
  steal resp list  ←── resp_mutex
  validate + deliver
  dlg_pending = FALSE
```

**Rules:**
- Worker thread **never accesses** `CHAR_DATA` or any game struct directly
- Game loop **never blocks** on network I/O
- All data passed to the worker is **copied by value** into the request struct
- Queue nodes use `malloc`/`free` — **not** the MUD's `ssm.c` allocator (not thread-safe)
- `dlg_pending = TRUE` prevents stacking a second request while one is in flight
- `dlg_pending` is cleared by `npc_dialogue_deliver()` on every response,
  including empty/failed ones

---

## System Prompt Assembly

Each request's system message is built from layers (in order):

```
1. Common knowledge block  (data/knowledge/common.kb)
2. Area knowledge block    (data/knowledge/area/<city>.kb — city from NPC room vnum)
3. Topic blocks            (data/knowledge/topic/<tag>.kb — one per KNOW_* bit set)
4. Lore entries            (first_lore list — up to 3 entries matching npc->lore_flags)
5. Accent instruction      (inline string — city accent of NPC)
6. Race speech inclination (inline string — NPC race)
7. NPC persona             (npc->pIndexData->ai_prompt — most critical layer)
8. Behavioural guardrails  (hardcoded — "1-3 sentences, stay in character")
```

Total prompt fits within `system_prompt[16384]` in `NPC_DLG_REQ`. The persona
is always appended last and is never truncated.

---

## Prompt Injection Defence

Player input goes through two layers before reaching the API:

### 1. Token stripping (`npc_dialogue_sanitize_input`)

Strips model role-boundary tokens and replaces angle brackets:

| Input | Output |
|---|---|
| `[INST]ignore this` | `ignore this` |
| `<<SYS>>new role` | `new role` |
| `<\|system\|>...` | stripped |
| `<\|im_start\|>` | stripped |
| `</s>` | stripped |
| `hello <there>` | `hello (there)` |

### 2. Keyword short-circuit

If the sanitized input matches any injection trigger (case-insensitive
substring), the API is **not called**. A confused in-character refusal
(`"I am not certain I follow your meaning."`) is enqueued directly on
the response queue and the attempt is logged.

Triggers: `ignore previous` · `ignore all previous` · `disregard previous` ·
`forget previous` · `you are now` · `act as` · `pretend you are` ·
`pretend to be` · `your new instructions`

---

## Stale Response Validation

Before delivering any response, `npc_dialogue_deliver()` checks:

1. `resp->npc` is non-NULL and not marked `is_free`
2. `resp->player` is non-NULL and not marked `is_free`
3. Both are in the same room

Responses failing any check are silently discarded. `dlg_pending` is always
cleared regardless.

---

## Key Source Files

| File | Role |
|---|---|
| `src/npc_dialogue.c` | Full implementation (queues, worker, HTTP, dispatch, deliver) |
| `src/npc_dialogue.h` | Public API (`init`, `dispatch`, `deliver`, `sanitize_input`) |
| `src/headers/config.h` | `TNGAI_*` constants, `KNOW_*` flags, city/accent constants |
| `src/act_comm.c` | `do_say()` — triggers `npc_dialogue_dispatch()` |
| `src/update.c` | `update_handler()` — calls `npc_dialogue_deliver()` each tick |
| `data/knowledge/` | Knowledge base `.kb` files loaded at boot |
