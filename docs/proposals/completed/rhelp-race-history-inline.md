# Design Proposal: Inline Race History in rhelp Output

## Problem

`do_rhelp` currently outputs:

```
RHELP for race Khenari
Class Order: Cle, Psi, Mag, Cip, War, Pug
Base Attributes: Str 12 Int 18 Wis 22 Dex 16 Con 16
...
```

The new race history help files (one per player race, in `help/`) give players a brief
lore summary per race, but there is no connection between `rhelp` and those entries.
The request is to show the race's help text on what is now the second line — between
the "RHELP for race X" header and the "Class Order" line.

## Approach

Change `do_rhelp` in `src/act_info.c` to:

1. Send the `RHELP for race X` header line immediately (via `send_to_char`).
2. Look up the race's help entry by title in `first_help` and send just the entry's
   text (no keyword echo, since the header already names the race).
3. If no help entry is found, skip silently — stats still display normally.
4. Continue with the existing stats output (Class Order, Base Attributes, etc.).

Concretely, the current flow of accumulating everything into `sendBuf` and sending
once at the end is split: the header is sent first, the help text is injected next,
then the stats are accumulated into `sendBuf` and sent as before.

### Resulting Output

```
RHELP for race Khenari
Jackal-headed humanoids who emerged from the mortuary-legal culture of Akh'enet...
Class Order: Cle, Psi, Mag, Cip, War, Pug
Base Attributes: Str 12 Int 18 Wis 22 Dex 16 Con 16
...
```

## Affected Files

| File | Change |
|------|--------|
| `src/act_info.c` | `do_rhelp`: split sendBuf into header-send + help-inject + stats-send |

## Help File Keyword Matching

The lookup uses `str_cmp(race_table[i].race_title, pHelp->keyword)` — exact
case-insensitive match against the keyword field. The ten new help files use the
race title as their keyword (`Human`, `Khenari`, `Khephari`, `Ashborn`, `Umbral`,
`Rivennid`, `Deltari`, `Ushabti`, `Serathi`, `Kethari`), so the match is guaranteed
for all player races. If a race has no help entry, `rhelp` degrades gracefully.

## Trade-offs

- Splitting `sendBuf` is a mild restructure but adds no new state or logic.
- The help text is sent as a separate `send_to_char` call between the header and
  stats; in practice MUD clients receive these sequentially with no perceptible gap.
- No new structs, no new globals, no new includes required — `first_help` and
  `HELP_DATA` are already available in `act_info.c`.
- The same help entries are accessible to players via `help khenari` etc., so the
  lore text serves double duty.
