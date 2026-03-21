# CLAUDE.md — ACK!MUD TNG 4.3.1

## Design Document Requirement

For any implementation task that involves `src/` changes, you MUST first deliver a design document describing the proposed changes — including the problem, approach, affected files, and any trade-offs — and discuss it with the user. Do NOT begin implementation until the user has explicitly signed off on the design document. No code changes, no file creation, no prototyping until approval is given. Design documents should be placed in `docs/proposals/`.

Changes that do not touch `src/` (e.g. lore files, area files, documentation, help files) do NOT require a design document and may be implemented directly. Bugfixes to `src/` also do NOT require a design document and may be implemented directly.

Once a proposal has been fully implemented, move it from `docs/proposals/` to `docs/proposals/completed/`. Active proposals (pending discussion or implementation) live in `docs/proposals/`. Completed proposals (fully implemented) live in `docs/proposals/completed/`.

When writing any document (design proposals, lore files, area plans, etc.), write and deliver it in sections rather than producing the entire document at once. Continue through all sections without waiting for confirmation between them, unless the user asks for a different approach.

## Project Overview

ACK!TNG is a MUD (Multi-User Dungeon) game server written in C, descended from Diku → Merc → ACK! lineage. The server binary is called `ack` and runs from the `area/` directory, accepting telnet connections on a configurable port.

## Game World Lore

When you need information about the game world (history, factions, geography, lore, etc.), search `docs/lore/` — it is the canonical source for all world-building documentation. The directories `lore/` and `data/knowledge/` are generated from `docs/lore/` and contain no additional information.

## Specifications and Documentation

The `docs/` directory is the canonical source for all game specifications (area files, objects, mobs, rooms, quests, help files, data structures, etc.). You should never need to search `src/` to understand a specification — everything should be documented in `docs/`. If you find that a specification detail is missing from `docs/` and you had to look in `src/` to determine the answer, that information must be added to the appropriate `docs/` specification file before the task is complete.

## Build System

All build commands run from the `src/` directory:

```sh
cd src
make ack          # Build the server binary
make clean        # Remove object files and binary
make unit-tests   # Build and run all unit tests, then integration test
make integration-test  # Build server + run integration test (boot, login, crash check)
make all          # clean → format → lint → build → unit-tests (includes integration test)
make lint         # Check all source files match the .clang-format style (fails if any differ)
make format       # Auto-apply .clang-format style to all source files
```

- Compiler: GCC with `-O -g2 -Wall -DACK_43`
- Links against `-lcrypt`
- Every `.c` file depends on `ack.h` (the central header)
- The binary is built as `src/ack`; it runs from `area/` (e.g., `cd area && ../src/ack 4000`)

## Testing

### Unit Tests

Each test is a standalone binary built from a `test_*.c` file paired with the module under test. The pattern:

1. **Test file**: `test_foo.c` — contains `main()`, test functions, and test doubles (stubs/mocks)
2. **Module under test**: `foo.unit-test.o` — compiled from `foo.c` with a `UNIT_TEST_FOO` define (when needed) to stub out dependencies
3. **Shared helper**: `test_is_fighting.o` — provides `is_fighting()` stub, linked into every test binary
4. **Linker trick**: `-ffunction-sections -fdata-sections` + `-Wl,--gc-sections` to dead-strip unused symbols that would cause link errors

To add a new unit test:
1. Create `src/tests/test_foo.c` with `main()` and test functions using `assert()`
2. Add Makefile rules for `foo.unit-test.o` and `unit-test-foo` target (binary goes to `tests/unit-test-foo`)
3. Add the target to the `unit-tests` dependency list and add `./tests/unit-test-foo` to the run list

Test conventions:
- Tests use `assert()` from `<assert.h>` — no test framework
- Test doubles stub external functions directly in the test file
- Use `#define DEC_GLOBALS_H 1` before `#include "ack.h"` to skip globals when they cause link issues
- Helper functions like `clear_character()` zero-initialize structs with `memset()`

### Integration Test

`integration-test.sh` (runs from the repo root):
- Builds the server, boots it on a random ephemeral port
- Walks the full new-player login flow (name → password → sex → race → class → enter game)
- Monitors for crashes for 8 seconds, then shuts down
- Runs automatically at the end of `make unit-tests`

### Running Tests

Always validate changes with:
```sh
cd src && make unit-tests
```

This runs all unit tests and the integration test. The CI workflow (`.github/workflows/validate-open-prs.yml`) runs this same command on every open PR.

**Unit tests should always be written for changes where possible.** When modifying or adding functionality, add a corresponding unit test in `src/tests/` to cover the new or changed behavior.

## Repository Structure

```
acktng/
├── src/              # C source code and Makefile
│   ├── ack.h         # Central header (includes typedefs.h, config.h, globals.h, etc.)
│   ├── typedefs.h    # Type definitions, structs, constants, macros
│   ├── config.h      # MUD configuration (name, limits, thresholds)
│   ├── globals.h     # Global variable declarations
│   ├── comm.c        # Network/connection handling, main game loop
│   ├── db.c          # Database loading (areas, objects, mobs, rooms)
│   ├── handler.c     # Object/character handler utilities
│   ├── fight.c       # Combat system
│   ├── magic*.c      # Spell system (magic.c, magic2.c, magic3.c, magic4.c)
│   ├── skills*.c     # Skills system (skills.c, skills_chi.c, skills_combo.c, etc.)
│   ├── act_*.c       # Player command handlers (info, comm, move, obj, mob, wiz, clan)
│   ├── save.c        # Player file save/load
│   ├── update.c      # Periodic game tick updates
│   ├── invasion.c    # Mob invasion event system
│   ├── keep.c        # Player keep/chest system
│   ├── tests/        # Unit test files
│   │   ├── test_*.c  # Test source files (one per module under test)
│   │   └── test_is_fighting.c  # Shared stub linked into most test binaries
│   └── Makefile      # Build rules
├── area/             # Area data files (~52 .are files) + runtime data
│   ├── area.lst      # Master list of areas to load at boot
│   └── *.are         # Individual area files (rooms, mobs, objects, resets)
├── data/             # Runtime data files (bans, clans, socials, rulers, etc.)
├── docs/             # Documentation (area file spec, data structures, licenses, testing)
├── help/             # In-game help files
├── shelp/            # Staff help files
├── player/           # Player save directory (subdirectories a-z)
├── log/              # Server log directory
├── reports/          # Report files
├── web/              # Game-generated web data output (see web/README.md)
│   ├── gsgp.json         # Game State / General-Purpose JSON feed (active players, leaderboards)
│   ├── soewholist.html   # HTML fragment: list of currently online players
│   └── whocount.html     # HTML fragment: current online player count
├── integration-test.sh  # Integration test script
└── .github/workflows/   # CI: validate-open-prs.yml
```

## Key Data Structures

Defined in `typedefs.h` and `ack.h`:

- **CHAR_DATA** — Character (player or NPC). Key fields: `name`, `level`, `hit`/`max_hit`, `mana`/`max_mana`, `gold`, `pcdata` (NULL for NPCs), `in_room`, `fighting`, `position`
- **PC_DATA** — Player-specific data (within CHAR_DATA). Fields: `pwd` (password hash), `learned[]` (skills), `quest_points`
- **OBJ_DATA** — In-game object instance. Fields: `name`, `item_type`, `value[]`, `wear_loc`
- **ROOM_INDEX_DATA** — Room definition. Fields: `vnum`, `name`, `description`, `exit[]`, `people` (linked list)
- **MOB_INDEX_DATA** / **OBJ_INDEX_DATA** — Templates for mobs/objects loaded from area files

## Currency

The game uses a single currency: `int gold` on CHAR_DATA. There is no multi-currency system.

## Code Conventions

- C89/C99 style with some GCC extensions
- `sh_int` = `short int`, `bool` = custom boolean type (TRUE/FALSE macros)
- String functions: `str_cmp()`, `str_prefix()`, `str_infix()` (case-insensitive, defined in `strfuns.c`)
- Memory: custom allocator via `ssm.c` (shared string manager). Use `str_dup()` / `free_string()` for strings
- Linked lists: manual next/prev pointers with LINK/UNLINK macros (defined in `lists.h`)
- Player output: `send_to_char()`, `act()` with format codes (`$n`, `$N`, `$p`, etc.)
- Color codes: `@@r` (red), `@@g` (green), `@@l` (blue), `@@N` (reset), etc.
- Wear locations: `WEAR_HOLD_HAND_L`, `WEAR_HOLD_HAND_R`, `WEAR_TWO_HANDED`, `WEAR_BUCKLER`, etc.
- Guard macros for headers: `#define DEC_HEADERNAME_H 1` / `#ifndef DEC_HEADERNAME_H`

## CI/CD

The GitHub Actions workflow `.github/workflows/validate-open-prs.yml`:
- Runs hourly and on manual dispatch
- For each open PR: checks out the PR head, runs `make unit-tests` from `src/`
- Auto-approves PRs where tests pass; requests changes where tests fail
- Branch protection should require this check to pass

## Spells, Skills, and Commands

Whenever a new spell or skill is added, a corresponding detailed help entry MUST be added to the `shelp/` directory. The help file should cover:
- What the spell/skill does
- How to use it (syntax, targets, etc.)
- Any relevant mechanics (damage, duration, cooldown, mana cost, etc.)
- Class/level availability if applicable

Whenever a new player command is added, a corresponding detailed help entry MUST be added to the `help/` directory. The help file should cover:
- What the command does
- Full syntax and all options/arguments
- Examples of usage
- Any restrictions (level, class, position, etc.)

Likewise, whenever a spell or skill is removed, its `shelp/` entry MUST also be removed. Whenever a command is removed, its `help/` entry MUST also be removed.

These are hard requirements — no spell, skill, or command addition is complete without its help entry, and no removal is complete without also removing the corresponding help entry.

## Common Pitfalls

- The server binary must run from the `area/` directory (relative paths to area files, data, player dirs)
- Player directories (`player/a` through `player/z`) must exist before players can save
- `ack.h` is the single include for most `.c` files — it pulls in typedefs, config, globals, lists, strfuns
- When writing unit tests, use `#define DEC_GLOBALS_H 1` before including `ack.h` to avoid link errors from global arrays
- The Makefile defines `O_FILES` twice (second definition wins) — be aware when adding new source files
- Area files use a custom text format with tilde (`~`) delimiters — see `docs/area_file_spec.md`
