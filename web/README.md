# ACK!TNG Web Data

This directory contains web-related assets served by the MUD's HTTP layer.

## HTTP Endpoints

The MUD server exposes two plain-HTTP endpoints on a dedicated port (default: 8080).
Responses are served directly from in-memory buffers updated on every game tick —
no files are written to disk.

| Endpoint | Content-Type | Description |
|----------|-------------|-------------|
| `GET /gsgp` | `application/json` | **Game Scry Game Protocol** JSON feed. Contains the server name, current active player count, and leaderboard snapshots (top players by level, top PKillers, top quest-point earners). |
| `GET /who` | `text/html; charset=utf-8` | HTML fragment — an `<h2>` heading and a `<ul>` list of currently online players, suitable for embedding directly into a web page. |

Both endpoints return HTTP/1.0 responses and close the connection immediately after
the response body is sent.  The `Access-Control-Allow-Origin: *` header is included on
the `/gsgp` response to allow cross-origin JavaScript access.

## Usage

```sh
curl http://localhost:8080/gsgp
curl http://localhost:8080/who
```

The HTTP port defaults to 8080.  To override, start the server with `--http-port N`.
To disable the HTTP listener entirely, pass `--no-http`.

## What Does NOT Belong Here

- Web server application code (Python scripts, Node apps, etc.)
- HTML templates or CSS stylesheets used by a web server
- Images, audio files, or other static assets
- Compiled binaries or build artifacts
