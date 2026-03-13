# Testing — ACK!MUD TNG

## Overview

The test suite has two layers:

- **Unit tests** — 38 standalone C binaries, each testing one module in isolation
- **Integration test** — boots the full server and walks through new-player login

All tests run with a single command from `src/`:

```sh
cd src && make unit-tests
```

This builds all unit-test binaries, runs them, then runs the integration test. The CI workflow (`.github/workflows/validate-open-prs.yml`) runs this same command on every open PR.

---

## Unit Tests

### Location

Test source files live in `src/tests/`:

```
src/
├── tests/
│   ├── test_is_fighting.c     # Shared stub linked into most test binaries
│   ├── test_act_flags.c
│   ├── test_act_info.c
│   ├── test_act_move.c
│   ├── test_act_obj.c
│   ├── test_area_format.c
│   ├── test_build.c
│   ├── test_buildare.c
│   ├── test_cloak.c
│   ├── test_comm.c
│   ├── test_crusade.c
│   ├── test_damage.c
│   ├── test_db.c
│   ├── test_death.c
│   ├── test_email.c
│   ├── test_fight.c
│   ├── test_help_format.c
│   ├── test_interp.c
│   ├── test_invasion.c
│   ├── test_item_generation.c
│   ├── test_keep.c
│   ├── test_magic.c
│   ├── test_magic4.c
│   ├── test_mapper.c
│   ├── test_mob_commands.c
│   ├── test_pdelete.c
│   ├── test_quest.c
│   ├── test_reincarnate.c
│   ├── test_rulers.c
│   ├── test_save.c
│   ├── test_sha256.c
│   ├── test_skills_chi.c
│   ├── test_skills_combo.c
│   ├── test_skills_obj.c
│   ├── test_special.c
│   ├── test_spell_dam.c
│   ├── test_spendqp.c
│   ├── test_ssm.c
│   ├── test_update.c
│   └── test_websocket_validation.c
```

Built binaries are placed in `src/tests/unit-test-*` and are excluded from git via the Makefile `clean` target.

### How Each Test Is Built

Each unit test follows this pattern:

1. **Test file** (`src/tests/test_foo.c`) — contains `main()`, test functions, and test doubles (stubs/mocks for functions the module calls)
2. **Module under test** (`foo.unit-test.o`) — compiled from `src/foo.c`, sometimes with a `-DUNIT_TEST_FOO` define to stub out internal dependencies
3. **Shared helper** (`tests/test_is_fighting.o`) — provides the `is_fighting()` stub, linked into most test binaries
4. **Linker trick** — `-ffunction-sections -fdata-sections` + `-Wl,--gc-sections` dead-strips unused symbols to avoid link errors from unreferenced game-state globals

The Makefile compiles `tests/*.c` files with `-I.` so they can find headers in `src/` (e.g. `ack.h`, `config.h`, `sha256.h`).

### Test Conventions

- Tests use `assert()` from `<assert.h>` — no external test framework
- Test doubles are defined directly in the test file (not in a separate header)
- Use `#define DEC_GLOBALS_H 1` before `#include "ack.h"` to skip the global-variable declarations when they cause duplicate-symbol link errors
- Helper: `clear_character(CHAR_DATA *ch)` — zero-initializes a struct with `memset()`, used in many tests as a setup helper

### Running a Single Test

```sh
cd src
make unit-test-fight        # build only
./tests/unit-test-fight     # run
```

### Adding a New Unit Test

1. Create `src/tests/test_foo.c` with `main()` and `assert()`-based test functions.

2. Add a compilation rule for the module under test in `src/Makefile` (if not already present):
   ```makefile
   foo.unit-test.o: foo.c
       $(CC) -c $(C_FLAGS) -DUNIT_TEST_FOO -ffunction-sections -fdata-sections -o foo.unit-test.o foo.c
   ```
   Omit `-DUNIT_TEST_FOO` and `-ffunction-sections` if the module has no internal stubs and no unused symbols.

3. Add the link rule:
   ```makefile
   unit-test-foo: tests/test_foo.o foo.unit-test.o tests/test_is_fighting.o
       rm -f tests/unit-test-foo
       $(CC) -Wl,--gc-sections -o tests/unit-test-foo tests/test_foo.o foo.unit-test.o tests/test_is_fighting.o $(L_FLAGS)
   ```

4. Add `unit-test-foo` to the `unit-tests` dependency list (line starting with `unit-tests:`).

5. Add `./tests/unit-test-foo` to the run block under `unit-tests:`.

---

## Integration Tests

Two integration tests cover the two connection paths the server supports.
Both run automatically at the end of `make unit-tests`.

### WebSocket integration test

**Script:** `integration-test.sh`

**Make target:** `make integration-test`

1. **Builds** the server binary (`make ack`)
2. **Starts** the server from `area/` on a random ephemeral port
3. **Waits** up to 90 seconds for the TCP port to accept connections
4. **Runs** a Python WebSocket client (HTTP upgrade → framed messages) that walks the full new-player login flow:
   - Name prompt → name confirmation
   - Password creation → retype password
   - Character creation: sex → race → class selection
   - MOTD display → enter game (receives "Welcome" message)
5. **Monitors** the running server for 2 seconds checking for crashes
6. **Shuts down** the server and cleans up

### Telnet integration test

**Script:** `integration-test-telnet.sh`

**Make target:** `make integration-test-telnet`

Identical flow to the WebSocket test, but connects via raw TCP (telnet) instead of WebSocket. The Python client:
- Handles IAC negotiation sequences (declines all options with WONT/DONT)
- Strips ANSI escape codes from received text
- Sends commands with `\r\n` line endings

Uses a different test player name (`Telnetrat`) from the WebSocket test (`Integrat`) so both can run in the same session without save-file conflicts.

### Exit codes (both tests)

- `0` — MUD booted, accepted a player login, and ran without crashing
- `1` — build failed, MUD crashed, or login happy-path was not reached

### Running manually

```sh
cd /path/to/acktng
./integration-test.sh          # WebSocket
./integration-test-telnet.sh   # Telnet
```

Or via make (from `src/`):

```sh
make integration-test
make integration-test-telnet
```

---

## CI

The GitHub Actions workflow `.github/workflows/validate-open-prs.yml`:

- Runs hourly and on manual dispatch
- For each open PR: checks out the PR head, runs `cd src && make unit-tests`
- Auto-approves the PR when tests pass; requests changes when they fail
