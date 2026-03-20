# Proposal: Remove Old Clan Management Commands

## Problem

The current clan system exposes a set of player-facing management commands that
were designed for a player-run clan model: `accept`, `banish`, `make`,
`ctoggle`, `negotiate`, `politics`, `leave`, `cwhere`, and `council`. These
commands pre-date the redesigned affiliation system described in the
public-societies and secret-societies proposals.

The existing commands are:
- Tightly coupled to pflags (`PFLAG_CLAN_LEADER`, `PFLAG_CLAN_ARMOURER`,
  `PFLAG_CLAN_DIPLOMAT`, `PFLAG_CLAN_BOSS`) that represent the old role model
- No longer consistent with the design direction where clans are background
  political entities rather than player-managed organizations
- Documented in help files that describe a system no longer being supported

## Approach

Remove the following player commands and their implementations:

| Command | Function | Reason |
|---------|----------|--------|
| `accept` | `do_accept` | Clan membership is now managed by immortals via `cset` |
| `banish` | `do_banish` | Same as above |
| `make` | `do_make` | Clan armourer equipment crafting no longer supported |
| `ctoggle` | `do_ctoggle` | Clan job roles no longer player-assigned |
| `negotiate` | `do_negotiate` | Player-driven diplomacy removed |
| `politics` | `do_politics` | Clan diplomacy matrix display removed |
| `leave` | `do_leave` | Clan departure now via `cset` (imm command) |
| `leav` | `do_leav` | Stub for mistyping `leave`; no longer needed |
| `cwhere` | `do_cwhere` | Clan member locator removed |
| `council` | `do_council` | Super council system removed |

Also remove the internal helpers that only serve the removed commands:
- `save_clan_table()` — only called from `do_negotiate`
- `get_diplo_name()` — only called from `do_politics` and `do_negotiate`

### Commands Retained

| Command | Function | Reason |
|---------|----------|--------|
| `clan` | `do_clan` | Clan communication channel; still relevant |
| `clist` | `do_clan_list` | Informational listing of clans |
| `home` | `do_clan_recall` | Navigation to clan headquarters |
| `claneq` | `do_claneq` | Equipment style preference for clan colors |
| `cset` | `do_cset` | Immortal command for setting clan membership |

## Affected Files

- `src/act_clan.c` — remove command and helper functions
- `src/interp.c` — remove CMD entries
- `src/headers/globals.h` — remove DECLARE_DO_FUN declarations
- `help/accept` — remove (covered accept/banish/make)
- `help/clan-eq` — remove (documented the make command)
- `help/clan__2` — remove (old general clan help listing removed commands)
- `help/cwhere` — remove (documented cwhere/cdonate/home system)
- `help/ctoggle` — remove
- `help/negotiate` — remove
- `help/politics` — remove
- `help/council` — remove
- `help/clan` — rewrite to describe the current clan system

## Trade-offs

- Removing `politics` and `save_clan_table` means the diplomacy matrix in
  `data/clandata.dat` is still loaded at boot but never updated. The
  `politics_data` struct remains in memory but is effectively static. This is
  acceptable as the data structures may be repurposed by future proposals.
- The `do_leave` removal means players cannot self-remove from clans; only
  immortals can change clan membership via `cset`. This is intentional.
