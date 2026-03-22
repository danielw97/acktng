# GSGP and Wholist HTTP Endpoints

## Problem

The current architecture uses a file bridge between the MUD server and the web server:

- The MUD server writes `~/web/data/gsgp.json` and `~/web/data/wholist.html` on every update tick.
- The Python web server reads these static files and serves them at `/gsgp` and `/players`.

With the DB migration in progress, the file bridge needs to be replaced. The web server should
instead fetch live data by calling HTTP endpoints on the MUD server directly, and the MUD server
should stop writing those files altogether.

## Approach

The MUD server already handles HTTP connections on its game port — `socket.c` currently parses
incoming HTTP `GET` requests and upgrades them to WebSocket if the headers indicate a WebSocket
client. Non-WebSocket, non-telnet traffic falls through.

We extend this to add two plain-HTTP endpoints:

- `GET /gsgp` → `application/json` — the GSGP leaderboard JSON
- `GET /wholist` → `text/html` — the HTML player list fragment

**In-memory caching:** Instead of writing files, `list_who_to_output()` in `comm.c` will render
the data into two static global char buffers (`gsgp_json_cache` and `wholist_html_cache`). These
are updated every update tick as before. The HTTP handlers in `socket.c` read from these buffers.

**Connection lifecycle for HTTP requests:** When `handle_websocket_handshake()` detects that a
GET request targets `/gsgp` or `/wholist` (and has no `Upgrade: websocket` header), it sends a
complete HTTP/1.0 response and returns `FALSE` (which causes `comm.c` to close and free the
descriptor immediately, same as a failed WebSocket handshake).

## Affected Files

| File | Change |
|------|--------|
| `src/comm.c` | Remove file I/O from `list_who_to_output()` and `write_gsgp_data()`; render into global buffers instead. Remove `expand_tilde()` and `write_gsgp_board()` (or refactor them to write to `FILE*` that is a `fmemopen` buffer — see below). Remove `write_gsgp_data()` file-writing path. |
| `src/socket.c` | Add HTTP endpoint routing in `handle_websocket_handshake()`: check request path before WebSocket upgrade; if `/gsgp` or `/wholist`, send buffered response and return `FALSE`. |
| `src/headers/config.h` | Remove `WHO_HTML_FILE` and `GSGP_JSON_FILE` defines. |
| `web/README.md` | Update to document the new endpoint-based approach; remove file entries. |

## Detailed Design

### `comm.c` — in-memory buffers

Two new static globals hold the most recently rendered content:

```c
#define GSGP_BUF_MAX   8192
#define WHO_BUF_MAX    16384

static char gsgp_json_cache[GSGP_BUF_MAX];
static char wholist_html_cache[WHO_BUF_MAX];
```

`write_gsgp_data()` is refactored to write into `gsgp_json_cache` using `snprintf`/`memcpy`
(or `fmemopen` + existing `fprintf` calls if available). The `rename`/`fopen`/`fclose` calls
are removed.

`list_who_to_output()` writes into `wholist_html_cache` using a growing string or `snprintf`
instead of `fopen`. The `fopen`/`fprintf`/`fclose` calls for the HTML file are removed.

Both buffers are initialised to empty strings at startup (zero-init by default since they are
static), so the endpoints return an empty-but-valid response before the first update tick runs.

Two accessor functions exposed via `comm.h` (or `socket.c` directly if `socket.c` includes
`comm.h`):

```c
const char *get_gsgp_json(void);     /* returns gsgp_json_cache */
const char *get_wholist_html(void);  /* returns wholist_html_cache */
```

### `socket.c` — HTTP endpoint routing

In `handle_websocket_handshake()`, after we have confirmed the request starts with `GET ` and
have received the full headers (`\r\n\r\n` seen), extract the request path before checking for
the WebSocket upgrade header:

```
GET /gsgp HTTP/1.x\r\n
GET /wholist HTTP/1.x\r\n
```

If the path is `/gsgp` or `/gsgp/`:
- Fetch `get_gsgp_json()`
- Write HTTP/1.0 200 response with `Content-Type: application/json` and
  `Access-Control-Allow-Origin: *`
- Return `FALSE` (closes the descriptor)

If the path is `/wholist` or `/wholist/`:
- Fetch `get_wholist_html()`
- Write HTTP/1.0 200 response with `Content-Type: text/html; charset=utf-8`
- Return `FALSE`

If neither, continue to the existing WebSocket upgrade logic (no behaviour change).

HTTP/1.0 is used deliberately to avoid needing `Connection: close` or keep-alive handling —
the connection closes naturally after the response.

### Path extraction helper

A small static helper extracts the path token from the request line:

```c
static void get_request_path(const char *request, char *out, size_t outsz);
```

Copies characters from after `GET ` up to the first space or `\r`.

## Trade-offs

- **Dual-purpose port**: The game port already serves WebSocket and telnet. Adding plain HTTP
  routes is a natural extension of existing behaviour.
- **No file I/O**: Removes the `~/web/data/` directory dependency and eliminates the
  temp-file rename dance.
- **Stale data window**: Unchanged — data is as fresh as the last update tick (~5–8 s).
- **Buffer sizing**: `GSGP_BUF_MAX = 8192` comfortably holds the JSON for 200 players with
  10-entry leaderboards. `WHO_BUF_MAX = 16384` handles 200 players with name tags. Both are
  static and never heap-allocated.
- **Thread safety**: The MUD is single-threaded; no locking needed.
- **`fmemopen` availability**: POSIX.1-2008, available on Linux. If `fmemopen` is unavailable
  the board-writer can be refactored to use a simple char-pointer `snprintf` accumulator instead.
- **Web server changes**: The web server must stop reading files and instead call
  `http://MUD_HOST:MUD_PORT/gsgp` and `http://MUD_HOST:MUD_PORT/wholist`. That change is in
  the web server repo and is out of scope for this proposal.

## Verification

1. `cd src && make unit-tests` — all tests pass.
2. Boot the server. `curl http://localhost:PORT/gsgp` → valid JSON (`active_players: 0`).
3. `curl http://localhost:PORT/wholist` → `<h2>Players Online</h2><ul></ul>` (or similar).
4. Log in a test character; re-curl both endpoints — player appears.
5. Confirm regular telnet login and WebSocket connections still work on the same port.
6. Confirm `~/web/data/gsgp.json` and `~/web/data/wholist.html` are no longer written.
