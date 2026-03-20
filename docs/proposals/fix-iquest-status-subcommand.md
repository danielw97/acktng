# Fix: `iquest status` Subcommand Not Displaying Quest Status

## Problem

Typing `iquest status` produces no output. The crusade quest status is only displayed when `iquest` is invoked with **no arguments** (`argument[0] == '\0'`). When the argument `"status"` is passed, it falls through all subcommand checks (`stop`, `start`, `auto`) and hits a bare `return` at line 749 of `src/quests/crusade.c`, silently doing nothing.

This is confusing for immortals who naturally try `iquest status` to check whether a crusade quest is running.

## Approach

Add a `!str_cmp(argument, "status")` branch near the top of `do_iquest` (right after the empty-argument check) that executes the same status display logic. To avoid duplicating the ~50-line block, extract it into a small static helper `show_iquest_status(CHAR_DATA *ch)` and call it from both the empty-argument path and the new `"status"` path.

## Affected Files

- `src/quests/crusade.c` — extract status display into a helper, call from both paths

## Trade-offs

- **Minimal change:** Only the one function is touched; no new data structures or headers needed.
- **Helper function:** A small static helper avoids code duplication and keeps both call sites in sync if the status output ever changes.
- **No behaviour change for existing usage:** `iquest` with no args continues to work identically.

## Unit Test

A unit test for `do_iquest` would require heavy stubs for the global quest state variables and `send_to_char`. The logic being changed is routing-only (which branch is taken), and the actual display code is unchanged. A targeted unit test for the argument dispatch can be added in `src/tests/test_iquest.c` if desired, but the change is low-risk enough that the integration test (boot + login + crash check) provides sufficient regression coverage.
