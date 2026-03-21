# Design Proposal: Replace MSSP Helper Macros with Functions

## Problem

The prior two passes of macro-to-function refactoring converted all the
obvious candidates — character/object predicates, numeric utilities, class-tier
queries, `SKILL_TIER`, `OUREF`/`CUREF`, `iseol`, and the SHA-256 arithmetic
macros. The remaining function-like macros in the codebase either require true
macro semantics (lvalue modification, token pasting, `__FILE__`/`__LINE__`
capture, `break` embedding, designated initializers) and are documented as
intentional.

One pair of locally-defined macros escaped those passes:

**`MSSP_VAR_STR(name, val)` and `MSSP_VAR_INT(name, val)`** in `src/socket.c`

These macros are defined immediately before use inside `send_mssp_data()`,
used a dozen times each, then `#undef`'d. They reference the caller's local
variables `buf` and `i` by name — an implicit-capture antipattern that makes
it non-obvious that `i` is being incremented through each call.

Concretely:

```c
/* Current macro — captures buf/i from the enclosing scope */
#define MSSP_VAR_STR(name, val)          \
  do {                                   \
    const char *__n = (name);            \
    const char *__v = (val);             \
    buf[i++] = MSSP_BYTE_VAR;           \
    while (*__n) buf[i++] = *__n++;     \
    buf[i++] = MSSP_BYTE_VAL;           \
    while (*__v) buf[i++] = *__v++;     \
  } while (0)

#define MSSP_VAR_INT(name, val)          \
  do {                                   \
    snprintf(tmp, sizeof(tmp), "%d", (val)); \
    MSSP_VAR_STR((name), tmp);           \
  } while (0)
```

Problems:
- `buf` and `i` are invisible implicit captures; a reader cannot tell the
  macros mutate `i` without reading their definitions.
- The macros are not debuggable — they disappear in stack traces.
- The `#define`/`#undef` dance is unusual and only works because both macros
  are used only within a single function body.

## Approach

Replace both macros with two `static` file-local functions that take `buf`
and `i` as explicit parameters:

```c
static void mssp_append_str(unsigned char *buf, int *i,
                             const char *name, const char *val)
{
    buf[(*i)++] = MSSP_BYTE_VAR;
    while (*name)
        buf[(*i)++] = (unsigned char)*name++;
    buf[(*i)++] = MSSP_BYTE_VAL;
    while (*val)
        buf[(*i)++] = (unsigned char)*val++;
}

static void mssp_append_int(unsigned char *buf, int *i,
                             const char *name, int val)
{
    char tmp[64];
    snprintf(tmp, sizeof(tmp), "%d", val);
    mssp_append_str(buf, i, name, tmp);
}
```

Call sites change from:

```c
MSSP_VAR_STR("NAME", mudnamenocolor);
MSSP_VAR_INT("PORT", global_port);
```

to:

```c
mssp_append_str(buf, &i, "NAME", mudnamenocolor);
mssp_append_int(buf, &i, "PORT", global_port);
```

The `#define`/`#undef` block for both macros is removed entirely.

## Affected Files

| File | Change |
|------|--------|
| `src/socket.c` | Add two `static` helper functions; replace 20 macro call sites; remove the `#define`/`#undef` block |

## Macros Explicitly Not Converted (this pass)

All remaining function-like macros have been reviewed and require macro
semantics:

| Macro | Justification |
|-------|---------------|
| `IS_SET`, `SET_BIT`, `REMOVE_BIT` | Type-generic lvalue modification |
| `LINK`, `TOPLINK`, `UNLINK`, and all `SING_*`/`HALF_*` variants | Modify `first`/`last` lvalues; use `DATA_TYPE` type parameter |
| `GET_FREE(item, freelist)` | Uses `sizeof(*(item))` at call-site type |
| `PUT_FREE(item, freelist)` | Token pasting: `freelist##_destructor` |
| `OREF`, `CREF` | `static` struct initializer requires compile-time context |
| `fread_string`, `str_dup`, `free_string`, `getmem`, `dispose` etc. | Inject `_caller` (`__FILE__`/`__LINE__`) at call site |
| `_dispose`, `dispose`, `qdispose` | Sets `(mem) = NULL` in caller's scope; `_dispose` contains `return;` |
| `STR`, `SX`, `_caller` | Use `#` stringification and `__FILE__`/`__LINE__` |
| `CMD`, `CMD_NINJA`, `CMD_WAIT`, `CMD_NINJA_WAIT` | Compound initializer rows for static command table |
| `SLOT(n)`, `L(cls, lvl)` | Designated array initializer syntax |
| `KEY`, `SKEY`, `RKEY` | Embed `break` statements — cannot be functions |
| `ELE_NAME` | Multi-statement compound in function body; embeds local logic |
| `RND`, `RNDr` | Multi-statement; modifies caller-local variables |
| `DECLARE_DO_FUN` and related | Pre-ANSI `TRADITIONAL` compatibility |
| `DEC_TRIGGER`, `DECLARE_CLUTCH_FUN` | Function declaration helpers |
| `CRS_MOVE(top, bottom)` | String literal concatenation |
| `crypt(s1, s2)` | Platform-specific no-op fallback |
| `args(list)` | Pre-ANSI compatibility |
| `be32enc_vect`, `be32dec_vect` | Conditional: function on little-endian, `memcpy` alias on big-endian |
| `str_cmp` in `scan.c`/`scanarea.c` | File-local override of `str_cmp` to `strcasecmp` |

## Trade-offs

**Benefits:**
- `buf` and `i` are now explicit parameters — the caller's intent is clear.
- Functions are visible in debugger stack frames.
- `mssp_append_int` no longer needs its own `tmp` buffer on the stack; it
  declares a local inside the function instead.
- The `#define`/`#undef` pattern is eliminated.

**Risks:**
- Pure mechanical change; no logic is altered.
- 20 call sites change, all within a single function body — easy to verify.

## Verification

```sh
cd src && make all
```

Runs: format check → lint → clean build → unit tests → integration test.
