# Proposal: Multi-Word Argument Matching for help/shelp/lore

## Problem

When a player types a multi-word query — `shelp fire blast`, `help fire blast`,
`lore disk's word` — the prefix-match fallback should use the full typed string
as its search key. Currently `do_shelp` does not do this.

### Confirmed bug: `do_shelp`

`parse_shelp_query` splits the argument into two values:

| Variable | Contents for `shelp fire blast` |
|---|---|
| `full_argument` | `"fire blast"` |
| `search_term` | `"fire"` (first word only, via `one_argument`) |

The two passes then use different values:

```c
// Pass 1 — exact match — uses full_argument ✓
if (!str_cmp(full_argument, pHelp->keyword))    // "fire blast" == keyword

// Pass 2 — prefix match — uses search_term (first word only) ✗
if (!str_prefix(search_term, pHelp->keyword))   // "fire" prefix of keyword
```

So `shelp fire blast` succeeds on pass 1 when the keyword is exactly
`"fire blast"`, but if that exact match misses, pass 2 searches for anything
starting with `"fire"` — not `"fire b"` or `"fire blast"`. That means a partial
query like `shelp fire bl` would silently match `"fire breath"` instead of
`"fire blast"`, because only the first word is used for narrowing.

### `do_help` and `do_lore`

Both already pass the full multi-word `argument` to both passes:

```c
// do_help
str_cmp(argument, pHelp->keyword)     // exact:  "fire blast" == keyword
str_prefix(argument, pHelp->keyword)  // prefix: "fire blast" prefix of keyword

// do_lore (via find_best_lore)
match_fn(argument, pHelp->keyword)    // same argument to both str_cmp and str_prefix
```

These are consistent and correct. No logic change is needed — but see the
**Consistency** note below.

## Approach

Fix `do_shelp` to use `full_argument` in the prefix-match pass:

```c
// Before
if (!str_prefix(search_term, pHelp->keyword))

// After
if (!str_prefix(full_argument, pHelp->keyword))
```

`search_term` (the first word) is no longer needed for matching after this
change. However, `parse_shelp_query` should be retained as-is because it still
does two useful things:

1. **Quote stripping** — `shelp "fire blast"` correctly strips quotes into
   `full_argument`.
2. **Empty-argument default** — sets both values to `"shelp"` when no argument
   is provided.

The `search_term` variable and the `parse_shelp_query` signature can be left
in place; only the second-pass call site changes.

## Affected Files

| File | Change |
|---|---|
| `src/act_info.c` | One-line fix in `do_shelp`: `search_term` → `full_argument` in prefix pass |

No changes to `do_help`, `do_lore`, `parse_shelp_query`, or any data structures.

## Trade-offs

**Stricter partial matching** — previously `shelp fire bl` would return the
first entry starting with `"fire"` (e.g. `"fire blast"` or `"fire breath"`
depending on load order). After the fix it only returns entries whose keyword
starts with `"fire bl"`. This is the correct and expected behavior; the old
behavior was a consequence of the bug.

**No behaviour change for single-word queries** — when the argument is a
single word, `search_term == full_argument`, so the fix is a no-op for those
queries.

## Testing

A unit test covering `parse_shelp_query` already exists for argument parsing;
the fix itself is tested by the integration test (server boot + login) which
exercises `do_shelp`. A targeted unit test for the matching logic can be added
to `tests/test_shelp_match.c` if desired.
