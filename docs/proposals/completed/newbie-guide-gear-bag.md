# Design Proposal: Newbie Guide Gear Bag

## Problem

New players at the Academy of Adventure start with no equipment and must walk through
eight training rooms picking up scattered gear before they can experience combat. The
Academy Guide (mob vnum 4910) is a natural help point, but has no way to shortcut this
for players who ask for a bag of gear. We want players to be able to say something like
"give me a bag" or "gear" or "equipment" and receive an adventurer's bag pre-loaded with
all the newbie school equipment.

---

## Approach

### New `speech_fun` mechanism

We add a `speech_fun` function pointer to `MOB_INDEX_DATA` and `CHAR_DATA`, parallel to
the existing `spec_fun`. A speech handler is called from `do_say()` — before the LLM
dialogue dispatch — whenever a player speaks in the same room as a mob that has a
`speech_fun` set. If the handler returns `TRUE`, the LLM dispatch is skipped.

This is the most general and reusable design: future NPCs can get keyword-triggered
behaviour without touching `do_say()` again.

### Area file format

A new `#SPEECH` section in area files assigns speech handlers, in the same format as
`#SPECIALS`:

```
#SPEECH
M 4910 speech_mudschool_guide
S
```

### Gear bag contents

The guide creates one instance of the adventurer's bag (vnum 4930) and loads the
following items into it before giving it to the player:

| Vnum | Name                    | Slot       |
|------|-------------------------|------------|
| 4931 | academy lantern         | held/light |
| 4932 | training sword          | weapon     |
| 4933 | training buckler        | off-hand   |
| 4935 | iron helm               | head       |
| 4936 | iron faceguard          | face       |
| 4937 | jade earring            | ear        |
| 4938 | ruby earring            | ear        |
| 4939 | silver chain necklace   | neck       |
| 4940 | amber amulet            | neck       |
| 4941 | leather shoulderpads    | shoulder   |
| 4942 | tattered cloak          | back       |
| 4943 | chainmail vest          | body       |
| 4944 | studded belt            | waist      |
| 4945 | iron leggings           | legs       |
| 4946 | iron boots              | feet       |
| 4947 | padded sleeves          | arms       |
| 4948 | copper bracelet         | wrist      |
| 4949 | silver bracelet         | wrist      |
| 4950 | leather gauntlets       | hands      |

The two-handed great training sword (4934) and the accessory/cosmetic items (4951-4964)
are intentionally excluded: the bag should contain a full standard equipment set, not
every item in the school.

### Trigger keywords

The speech handler fires when the player's message contains any of:
`bag`, `gear`, `equipment`, `kit`, `stuff`

The handler gives the bag only once per player visit (tracked by the guide checking
whether the player already carries an adventurer's bag). If they already have one, the
guide says so politely and declines.

---

## Affected Files

| File | Change |
|------|--------|
| `src/headers/typedefs.h` | New `SPEECH_FUN` typedef |
| `src/headers/ack.h` | New `speech_fun` field in `MOB_INDEX_DATA` and `CHAR_DATA` |
| `src/save/save_area_files.c` | New `load_speech()` function; `#SPEECH` section dispatch in `boot_db()` area loader; save `speech_fun` in `write_area_file()` |
| `src/special.c` | New `speech_lookup()` and `speech_name()` functions |
| `src/act_comm.c` | Call `speech_fun` before `npc_dialogue_dispatch` in `do_say()` |
| `src/db.c` | Copy `speech_fun` from template to clone in `create_mobile()` |
| `src/ai/spec_mudschool_guide.c` | New `speech_mudschool_guide()` handler |
| `area/newschool.are` | New `#SPEECH` section |

No new files need to be created.

---

## Trade-offs

**Pros:**
- General mechanism: any future mob can get keyword-triggered speech without touching
  `do_say()` again.
- LLM-bypass is explicit: if `speech_fun` returns `TRUE`, the LLM is not called, so
  there is no race between the hardcoded give and an LLM reply.
- Follows the existing `spec_fun` / `#SPECIALS` pattern exactly — no new concepts.

**Cons:**
- More files touched than a "check vnum in do_say" approach, but those approaches
  are bad design.
- `save_area_files.c` needs both a loader and a saver; the saver must handle the new
  section gracefully for areas that have no speech functions.

---

## Implementation Plan

1. Add `SPEECH_FUN` typedef to `typedefs.h`.
2. Add `speech_fun` field to `MOB_INDEX_DATA` and `CHAR_DATA` in `ack.h`.
3. Add `speech_lookup()` / `speech_name()` and `speech_mudschool_guide` entry to `special.c`.
4. Add `speech_mudschool_guide()` to `spec_mudschool_guide.c`.
5. Add `load_speech()` to `save_area_files.c`; wire it into `boot_db()` in `db.c`.
6. Extend `write_area_file()` in `save_area_files.c` to emit `#SPEECH`.
7. Copy `speech_fun` from template to clone in `create_mobile()` in `db.c`.
8. Add speech call in `do_say()` in `act_comm.c`.
9. Add `#SPEECH` section to `newschool.are`.
10. Build, test, push.
