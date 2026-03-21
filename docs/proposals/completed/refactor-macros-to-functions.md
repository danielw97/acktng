# Design Proposal: Replace Function-Like Macros with Static Inline Functions

## Problem

The codebase uses ~30 C preprocessor macros that behave like functions — they
take typed parameters, access struct fields, and return values. Using `#define`
for these loses:

- **Type safety**: the preprocessor performs no type checking.
- **Debuggability**: macros don't appear in stack traces or debugger call frames.
- **Tooling compatibility**: IDEs, static analysers, and code coverage tools
  struggle with macro-heavy code.
- **Hygiene**: multi-argument macros are prone to double-evaluation bugs and
  operator-precedence surprises.

None of these macros require true macro semantics: none do token-pasting, none
embed `return` statements, none declare local variables, and none are
type-generic in a way that can't be expressed with a specific type.

## Approach

Replace each function-like macro with a `static inline` function of the same
name. Keeping the same name means **zero call-site changes** across the
codebase (the compiler sees `IS_NPC(ch)` and calls a typed inline function
rather than expanding a text substitution).

To resolve header-order dependencies (utils.h is included before globals.h in
ack.h), add a small set of forward declarations to utils.h for the external
symbols the inline functions reference.

## Affected Files

| File | Change |
|---|---|
| `src/headers/utils.h` | Replace 25 `#define` macros with `static inline` functions; add 5 forward declarations |
| `src/headers/config.h` | Remove 4 class-tier macros (moved to utils.h as inline functions) |
| `src/headers/weapon_bond.h` | Convert `IS_OBJ_BONDED` macro to inline function |
| `src/headers/act_mob.h` | Remove `AI_MOB` macro (pure alias for `IS_NPC`); update callers |

## Macros Being Converted

### In utils.h

| Macro | Converted to |
|---|---|
| `IS_NPC(ch)` | `static inline bool IS_NPC(CHAR_DATA *ch)` |
| `IS_UNDEAD(ch)` | `static inline bool IS_UNDEAD(CHAR_DATA *ch)` |
| `IS_AFFECTED(ch, sn)` | `static inline bool IS_AFFECTED(CHAR_DATA *ch, int sn)` |
| `IS_GOOD(ch)` | `static inline bool IS_GOOD(CHAR_DATA *ch)` |
| `IS_EVIL(ch)` | `static inline bool IS_EVIL(CHAR_DATA *ch)` |
| `IS_AWAKE(ch)` | `static inline bool IS_AWAKE(CHAR_DATA *ch)` |
| `IS_OUTSIDE(ch)` | `static inline bool IS_OUTSIDE(CHAR_DATA *ch)` |
| `IS_STAFF(ch)` | `static inline bool IS_STAFF(CHAR_DATA *ch)` |
| `IS_HERO(ch)` | `static inline bool IS_HERO(CHAR_DATA *ch)` |
| `PLAYTESTER(ch)` | `static inline bool PLAYTESTER(CHAR_DATA *ch)` |
| `HAS_BODY(ch)` | `static inline bool HAS_BODY(CHAR_DATA *ch)` |
| `HAS_MIND(ch)` | `static inline bool HAS_MIND(CHAR_DATA *ch)` |
| `ADEPT_LEVEL(ch)` | `static inline int ADEPT_LEVEL(CHAR_DATA *ch)` |
| `MANA_COST(ch, sn)` | `static inline int MANA_COST(CHAR_DATA *ch, int sn)` |
| `WAIT_STATE(ch, npulse)` | `static inline void WAIT_STATE(CHAR_DATA *ch, int npulse)` |
| `NAME(ch)` | `static inline const char *NAME(CHAR_DATA *ch)` |
| `CH(descriptor)` | `static inline CHAR_DATA *CH(DESCRIPTOR_DATA *d)` |
| `CAN_WEAR(obj, part)` | `static inline bool CAN_WEAR(OBJ_DATA *obj, int part)` |
| `IS_OBJ_STAT(obj, stat)` | `static inline bool IS_OBJ_STAT(OBJ_DATA *obj, int stat)` |
| `IS_WEAPON(eq)` | `static inline bool IS_WEAPON(OBJ_DATA *eq)` |
| `IS_SHIELD(eq)` | `static inline bool IS_SHIELD(OBJ_DATA *eq)` |
| `IS_DAYTIME()` | `static inline bool IS_DAYTIME(void)` |
| `IS_NIGHTTIME()` | `static inline bool IS_NIGHTTIME(void)` |
| `PERS(ch, looker)` | `static inline const char *PERS(CHAR_DATA *ch, CHAR_DATA *looker)` |
| `IS_MORTAL_CLASS(c)` | `static inline bool IS_MORTAL_CLASS(int c)` (moved from config.h) |
| `IS_REMORT_CLASS(c)` | `static inline bool IS_REMORT_CLASS(int c)` (moved from config.h) |
| `IS_ADEPT_CLASS(c)` | `static inline bool IS_ADEPT_CLASS(int c)` (moved from config.h) |
| `CLASS_TIER(c)` | `static inline int CLASS_TIER(int c)` (moved from config.h) |

### In weapon_bond.h

`IS_OBJ_BONDED(obj)` → `static inline bool IS_OBJ_BONDED(OBJ_DATA *obj)`

### In act_mob.h

`AI_MOB(ch)` — removed entirely. It is a semantic duplicate of `IS_NPC(ch)`.
Callers are updated to use `IS_NPC` directly.

## Macros That Stay as Macros

| Category | Examples | Reason |
|---|---|---|
| Header guards | `DEC_*_H` | Preprocessor-only |
| Type aliases | `CD`, `MID`, `OD` | Type-level aliasing |
| Array-size constants | `MAX_*`, `BIT_*`, flag constants | Used in array declarations/initializers |
| Type-generic ops | `UMIN`, `UMAX`, `IS_SET`, `SET_BIT`, `REMOVE_BIT` | Operate on multiple integer widths |
| Type-generic char ops | `UPPER`, `LOWER`, `IS_LETTER` | No specific type |
| Linked list macros | `LINK`, `UNLINK`, `SING_*`, `HALF_*` | Type-generic, embed local var declarations |
| Memory pool macros | `GET_FREE`, `PUT_FREE` | Token-pasting for destructor names |
| Reference tracking | `OREF`, `CREF`, `OUREF`, `CUREF` | Use `static` local struct trick |
| SSM string wrappers | `str_dup`, `fread_string`, etc. | Capture `__FILE__`/`__LINE__` via `_caller` |
| `dispose` / `_dispose` | — | Embeds `return;` that exits the *calling* function |
| `getmem`, `qgetmem` | — | Capture caller info |
| `STR`, `SX`, `_caller` | — | Stringification and `__LINE__` |
| `LEVELS_INIT`, `L()` | — | GCC designated-range array initializers |

## Trade-offs

**Benefits:**
- Type errors caught at compile time (e.g., passing an `OBJ_DATA*` to `IS_NPC`)
- Debugger shows function names in backtraces
- Static analysers can reason about the functions properly
- No double-evaluation risk

**No downsides:**
- Performance: `static inline` is identical to a macro for simple accessors; the
  compiler inlines them at `-O`.
- Binary size: dead-stripping (`-ffunction-sections -Wl,--gc-sections`) handles
  any unreferenced copies.
- Call-site churn: zero — all names are preserved exactly.
