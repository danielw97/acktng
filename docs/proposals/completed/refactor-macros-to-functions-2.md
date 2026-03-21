# Design Proposal: Refactor Macros to Functions

## Problem

The codebase inherited from the Diku/Merc/ACK lineage makes heavy use of C preprocessor macros for what are conceptually functions — predicates, accessors, and utilities. A prior refactor already converted the most-used character/object accessor macros (IS_NPC, UMIN, UMAX, IS_GOOD, etc.) to `static inline` functions in `inlines.h` and `inlines_globals.h`. Several function-like macros were overlooked in that pass and remain.

Macros that act as functions have several drawbacks:
- They are invisible to the debugger and static analysis tools
- They expand at every call site, hiding actual function semantics
- They cannot be safely used in all expression contexts (e.g., those with side effects may evaluate arguments multiple times)
- They cannot be referenced as function pointers
- Errors in them produce confusing messages pointing into macro expansions

## Scope

This proposal covers the remaining function-like macros not yet converted. Macros that require preprocessor features (lvalue modification, token pasting, `sizeof` of caller type, `__FILE__`/`__LINE__` injection, static initializers, compound designators) are explicitly excluded and documented as justified.

## Proposed Changes

### 1. Remove `SKILL_TIER(sn)` — trivial function wrapper

**File:** `src/headers/ack.h`

```c
/* Before */
#define SKILL_TIER(sn) skill_get_tier(sn)
int skill_get_tier(int sn);

/* After */
int skill_get_tier(int sn);
/* All call sites use skill_get_tier(sn) directly */
```

`SKILL_TIER` is a 1:1 alias for an already-declared function. It provides no additional abstraction, just obfuscates that a function call is occurring. Removing it and calling `skill_get_tier()` directly is clearer.

**Affected call sites (13):** `src/spell_dam.c`, `src/magic.c`, `src/macros.c`, `src/damage.c`, `src/act_info.c`

### 2. Remove `OUREF(var)` and `CUREF(var)` — hidden-`&` antipattern

**File:** `src/headers/utils.h`

```c
/* Before */
#define OUREF(var) obj_unreference(&var);
#define CUREF(var) char_unreference(&var);

/* After: call sites call functions directly */
obj_unreference(&var);
char_unreference(&var);
```

These macros exist solely to hide the `&` address-of operator from call sites. The underlying functions are already properly declared. Hiding `&` makes it non-obvious that the variable is being passed by pointer and may be mutated. Explicit `&` at call sites is standard C idiom and is clearer.

**Affected call sites (31):** `src/act_obj.c`, `src/death.c`, `src/fight.c`, `src/magic.c`, `src/update.c`, and 11 spell/skill files.

### 3. Convert `iseol(c)` — simple predicate in a .c file

**File:** `src/mapper.c`

```c
/* Before */
#define iseol(c) ((c) == '\n' || (c) == '\r')

/* After */
static inline bool iseol(char c)
{
   return c == '\n' || c == '\r';
}
```

This predicate is file-local, pure, and has no macro-only requirements. Converting to a typed `static inline` function gives it proper type-checking.

### 4. Convert SHA-256 elementary macros — pure arithmetic functions

**File:** `src/sha256.c`

The eight elementary macros in sha256.c are all pure functions of their `int` arguments with no side effects:

```c
/* Before */
#define Ch(x, y, z)  ((x & (y ^ z)) ^ z)
#define Maj(x, y, z) ((x & (y | z)) | (y & z))
#define SHR(x, n)    (x >> n)
#define ROTR(x, n)   ((x >> n) | (x << (32 - n)))
#define S0(x)        (ROTR(x, 2)  ^ ROTR(x, 13) ^ ROTR(x, 22))
#define S1(x)        (ROTR(x, 6)  ^ ROTR(x, 11) ^ ROTR(x, 25))
#define s0(x)        (ROTR(x, 7)  ^ ROTR(x, 18) ^ SHR(x, 3))
#define s1(x)        (ROTR(x, 17) ^ ROTR(x, 19) ^ SHR(x, 10))

/* After */
static inline int sha256_Ch(int x, int y, int z)  { return (x & (y ^ z)) ^ z; }
static inline int sha256_Maj(int x, int y, int z) { return (x & (y | z)) | (y & z); }
static inline int sha256_SHR(int x, int n)        { return x >> n; }
static inline int sha256_ROTR(int x, int n)       { return (x >> n) | (x << (32 - n)); }
static inline int sha256_S0(int x) { return sha256_ROTR(x, 2)  ^ sha256_ROTR(x, 13) ^ sha256_ROTR(x, 22); }
static inline int sha256_S1(int x) { return sha256_ROTR(x, 6)  ^ sha256_ROTR(x, 11) ^ sha256_ROTR(x, 25); }
static inline int sha256_s0(int x) { return sha256_ROTR(x, 7)  ^ sha256_ROTR(x, 18) ^ sha256_SHR(x, 3); }
static inline int sha256_s1(int x) { return sha256_ROTR(x, 17) ^ sha256_ROTR(x, 19) ^ sha256_SHR(x, 10); }
```

**Note:** `RND` and `RNDr` are kept as macros — they expand to multiple assignment statements that modify caller-local variables (`t0`, `t1`, `d`, `h`) and the round constants are passed as integer literals; converting these to functions would require an impractical interface.

The function names are prefixed with `sha256_` to avoid collisions with any system headers that might define `Ch`, `Maj`, `SHR`, or `ROTR`.

## Macros Explicitly Not Converted

| Macro | Justification |
|-------|---------------|
| `IS_SET(flag, bit)` | Type-generic across `int`/`long`/`sh_int`; no single concrete type |
| `SET_BIT(var, bit)` | Modifies an lvalue — requires macro |
| `REMOVE_BIT(var, bit)` | Modifies an lvalue — requires macro |
| `LINK`, `TOPLINK`, `UNLINK`, etc. (all variants) | Modify `first`/`last` lvalues; use `DATA_TYPE` type parameter |
| `GET_FREE(item, freelist)` | Uses `sizeof(*(item))` at call-site type |
| `PUT_FREE(item, freelist)` | Token pasting: `freelist##_destructor` |
| `OREF(v, type)`, `CREF(v, type)` | `static` struct initializer requires compile-time context |
| `fread_string`, `str_dup`, `free_string`, `getmem`, `dispose` etc. | Inject `_caller` (`__FILE__`/`__LINE__`) at call site |
| `_dispose`, `dispose`, `qdispose` | Sets `(mem) = NULL` in caller's scope |
| `STR(x)`, `SX(x)`, `_caller` | Use `#` stringification and `__FILE__`/`__LINE__` |
| `CMD`, `CMD_NINJA`, `CMD_WAIT`, `CMD_NINJA_WAIT` | Compound initializer rows for static command table |
| `SLOT(n)`, `L(cls, lvl)` | Designated array initializer syntax |
| `RKEY`, `ELE_NAME` | Multi-statement or table-init helpers |
| `RND`, `RNDr` | Multi-statement; modifies caller-local variables |
| `DECLARE_DO_FUN`, etc. | Pre-ANSI `TRADITIONAL` compatibility |
| `DEC_TRIGGER`, `DECLARE_CLUTCH_FUN` | Function declaration helpers |
| `CRS_MOVE(top, bottom)` | String literal concatenation |

## Trade-offs

**Benefits:**
- Improved debuggability (functions appear in stack traces, macros don't)
- Type safety (typed function parameters vs untyped macro arguments)
- Clearer call sites (explicit `&` for pointer arguments)
- Static analysis tools can reason about function bodies

**Risks:**
- Pure mechanical change; no logic is altered. Build + full test suite validates correctness.

## Verification

```sh
cd src && make all
```

Runs: format check → lint → clean build → unit tests → integration test (boot + full login flow + crash check). All must pass.
