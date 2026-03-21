# ACK!TNG Web Data Output Directory

This directory contains **game-generated data files** written by the running MUD server
for consumption by external web services, dashboards, and monitoring tools.

## Purpose

`web/` is a **runtime output** directory — the MUD server writes structured data here so
that web servers, stats pages, and third-party integrations can read it without having
to connect to or query the game directly.

**Nothing in this directory is hand-edited during normal operation.**  The files here are
owned by the server and overwritten on each update cycle.

## Files

| File | Written by | Description |
|------|-----------|-------------|
| `gsgp.json` | MUD server | **Game State / General-Purpose** JSON feed.  Contains the server name, current active player count, and leaderboard snapshots (top players by level, top PKillers, top quest-point earners).  Updated on every game tick that changes one of those values. |
| `soewholist.html` | MUD server (`act_info.c`) | HTML fragment — an `<h2>` heading and a `<ul>` list of currently online players, suitable for embedding directly into a web page.  Regenerated on every `who` update. |
| `whocount.html` | MUD server (`act_info.c`) | Single `<p>` tag with the current online player count.  Regenerated in sync with `soewholist.html`. |

Additional files may appear here as new web-export features are added; they follow the
same rule — machine-written, human-readable, no manual editing required.

## Usage

A companion web server (run separately, not part of this repository) reads these files
to serve the ACKMUD public website.  The expected environment variables it honours are:

```
ACK_WHO_HTML_FILE   path to soewholist.html  (default: web/soewholist.html)
ACK_WHO_COUNT_FILE  path to whocount.html    (default: web/whocount.html)
```

The `gsgp.json` file can be fetched directly by any HTTP client or JavaScript front-end
that wants live server statistics without maintaining a persistent telnet connection.

## What Does NOT Belong Here

- Web server application code (Python scripts, Node apps, etc.)
- HTML templates or CSS stylesheets used by a web server
- Images, audio files, or other static assets
- Compiled binaries or build artifacts
