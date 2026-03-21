# ACK!TNG Web Data Output Directory

This directory contains **game-generated data files** written by the running MUD server
for consumption by external web services, dashboards, and monitoring tools.

## Purpose

`~/web/data/` is a **runtime output** directory — the MUD server writes structured data here so
that web servers, stats pages, and third-party integrations can read it without having
to connect to or query the game directly.

**Nothing in this directory is hand-edited during normal operation.**  The files here are
owned by the server and overwritten on each update cycle.

## Files

| File | Written by | Description |
|------|-----------|-------------|
| `gsgp.json` | MUD server | **Game State / General-Purpose** JSON feed.  Contains the server name, current active player count, and leaderboard snapshots (top players by level, top PKillers, top quest-point earners).  Updated on every game tick that changes one of those values. |
| `wholist.html` | MUD server (`comm.c`) | HTML fragment — an `<h2>` heading and a `<ul>` list of currently online players, suitable for embedding directly into a web page.  Regenerated on every `who` update. |

Additional files may appear here as new web-export features are added; they follow the
same rule — machine-written, human-readable, no manual editing required.

## Output Location

All files are written to `~/web/data/` (i.e. `/home/user/web/data/`).

## What Does NOT Belong Here

- Web server application code (Python scripts, Node apps, etc.)
- HTML templates or CSS stylesheets used by a web server
- Images, audio files, or other static assets
- Compiled binaries or build artifacts
