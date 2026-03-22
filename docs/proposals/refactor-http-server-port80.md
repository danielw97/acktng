# Design Proposal: Refactor HTTP Server to Dedicated Port 80

## Problem

The MUD's two plain-HTTP endpoints (`/gsgp` and `/wholist`) are currently served inline
within `handle_websocket_handshake()` in `socket.c`.  This means:

1. HTTP requests must arrive on the WebSocket port (loopback-only), making the endpoints
   inaccessible from the public internet without proxying.
2. The HTTP logic is entangled with the WebSocket handshake path, which is confusing.
3. The endpoint name `/wholist` is verbose; callers expect `/who`.

The goal is to move the HTTP serving to a dedicated port (typically 80) that accepts
plain HTTP from any address, serving only the `gsgp` and `who` endpoints.

---

## Approach

Add a new `--http-port <port>` CLI flag that opens a dedicated listening socket bound to
`INADDR_ANY`.  Incoming connections on that socket are handled inline in `game_loop`
without creating a `DESCRIPTOR_DATA`: accept, read the request, write the response,
close.  This keeps the change minimal and avoids polluting the descriptor list with
short-lived HTTP connections.

The HTTP endpoint handling is removed from `handle_websocket_handshake()`.  The endpoint
`/wholist` is renamed to `/who`; the old name is no longer served.

---

## Affected Files

| File | Change |
|------|--------|
| `src/comm.c` | Add `--http-port` flag; init and store `control_http`; pass to `game_loop` |
| `src/socket.c` | Add `global_http_port`; add `handle_http_request(int)`; update `game_loop` signature; remove HTTP endpoint block from `handle_websocket_handshake` |
| `src/headers/socket.h` | Export `global_http_port`; update `game_loop` prototype |
| `src/tests/test_websocket_validation.c` | Rename `test_get_request_path_wholist` → `test_get_request_path_who` |
| `web/README.md` | Update endpoint table and curl examples |

---

## Detailed Changes

### `comm.c` — CLI flag and socket init

Add local variables `int control_http = -1` and `int http_port = -1` alongside the
existing `control_ws` / `ws_port` pair.

In the flag-parsing loop, recognise `--http-port <N>` and store `N` in `http_port`.
Unlike the telnet port, **no >1024 restriction** is enforced here, since port 80 is
the intended target and the operator is responsible for granting the capability
(`CAP_NET_BIND_SERVICE` or running as root).  A sanity check of `>0` is sufficient.

```c
else if (!strcmp(argv[i], "--http-port") && i + 1 < argc)
{
    http_port = atoi(argv[++i]);
    if (http_port <= 0)
    {
        fprintf(stderr, "--http-port must be a positive integer.\n");
        exit(1);
    }
}
```

Init the socket (no HOTreboot inheritance — the socket is re-opened like `control_ws`):

```c
if (http_port > 0)
    control_http = init_socket(http_port, INADDR_ANY);
global_http_port = http_port;
```

Pass `control_http` as the fifth argument to `game_loop`.

The "require at least one listening port" guard is relaxed to also count `http_port`.

### `socket.c` — HTTP handler and game loop

**New global:**
```c
int global_http_port = -1;
```

**New static function `handle_http_request(int desc)`:**

Does a single `recv` of up to ~4 KiB.  Parses the path with the existing
`get_request_path()` helper.  Serves `/gsgp` (JSON) or `/who` (HTML) and returns a
HTTP/1.0 response via `send()`, then the caller closes the fd.  Unknown paths get a
terse 404.

```c
static void handle_http_request(int desc)
{
    char buf[4096];
    int n;
    char path[64];
    const char *body;
    char header[256];

    n = recv(desc, buf, sizeof(buf) - 1, 0);
    if (n <= 0)
        return;
    buf[n] = '\0';

    if (strncmp(buf, "GET ", 4) != 0)
        return;

    get_request_path(buf, path, sizeof(path));

    if (strcmp(path, "/gsgp") == 0 || strcmp(path, "/gsgp/") == 0)
    {
        body = get_gsgp_json();
        snprintf(header, sizeof(header),
                 "HTTP/1.0 200 OK\r\n"
                 "Content-Type: application/json\r\n"
                 "Access-Control-Allow-Origin: *\r\n"
                 "Content-Length: %d\r\n"
                 "\r\n",
                 (int)strlen(body));
        send(desc, header, strlen(header), 0);
        send(desc, body, strlen(body), 0);
    }
    else if (strcmp(path, "/who") == 0 || strcmp(path, "/who/") == 0)
    {
        body = get_wholist_html();
        snprintf(header, sizeof(header),
                 "HTTP/1.0 200 OK\r\n"
                 "Content-Type: text/html; charset=utf-8\r\n"
                 "Content-Length: %d\r\n"
                 "\r\n",
                 (int)strlen(body));
        send(desc, header, strlen(header), 0);
        send(desc, body, strlen(body), 0);
    }
    else
    {
        const char *not_found = "HTTP/1.0 404 Not Found\r\nContent-Length: 0\r\n\r\n";
        send(desc, not_found, strlen(not_found), 0);
    }
}
```

**`game_loop` signature update:**
```c
void game_loop(int control, int control_ws, int control_tls, int control_sniff, int control_http)
```

In the `reopen_flag` block, add:
```c
if (control_http >= 0)
{
    close(control_http);
    control_http = init_socket(global_http_port, INADDR_ANY);
}
```

In the `FD_SET` block and `maxdesc` tracking:
```c
if (control_http >= 0)
{
    FD_SET(control_http, &in_set);
    maxdesc = UMAX(maxdesc, control_http);
}
```

In the "New connection?" block:
```c
if (control_http >= 0 && FD_ISSET(control_http, &in_set))
{
    struct sockaddr_in sa;
    socklen_t size = sizeof(sa);
    int desc = accept(control_http, (struct sockaddr *)&sa, (socklen_t *)&size);
    if (desc >= 0)
    {
        handle_http_request(desc);
        close(desc);
    }
}
```

**Remove from `handle_websocket_handshake()`:**  Delete the entire
"Check for plain HTTP endpoint requests" block (lines that check `/gsgp` and `/wholist`
and return `FALSE`).

### `socket.h` — exports and prototype

```c
extern int global_http_port;
void game_loop(int control, int control_ws, int control_tls, int control_sniff, int control_http);
```

### `test_websocket_validation.c` — rename test

Rename `test_get_request_path_wholist` → `test_get_request_path_who` and update the
path tested from `/wholist` to `/who`.

### `web/README.md` — documentation

Update the endpoint table to show `/who` instead of `/wholist`.  Update curl examples
to use `PORT=80` and the new path.  Update the introductory sentence to say the
endpoints are served on a dedicated HTTP port.

---

## Trade-offs

**Inline accept vs DESCRIPTOR_DATA:** Using an inline `recv`/`send`/`close` cycle
avoids descriptor list churn for short-lived HTTP connections.  The risk is that if a
client sends the GET request across multiple TCP segments the single `recv` will miss
part of the request and the function will silently return nothing.  In practice, GET
requests to these endpoints (< 50 bytes) always arrive in a single segment, and the
read timeout is implicitly handled by dropping silent connections.  For more robustness
a buffered read loop could be added later, but it is not necessary now.

**Port 80 privileges:** The operator must ensure the process has `CAP_NET_BIND_SERVICE`
or is started as root to bind port 80.  This is a deployment concern, not a code concern.

**HOTreboot:** The HTTP control socket is not inherited across HOTreboot (the exec
format only passes the telnet fd).  The socket is re-opened on startup, incurring a
brief gap during reboot.  This matches the existing behaviour for `control_ws`.

**`/wholist` backward compat:** The old path is dropped; callers must use `/who`.
Given the endpoint was only recently introduced and is intended for external clients,
this is an acceptable breaking change.

---

## Checklist

- [ ] `comm.c`: add `--http-port` flag, init socket, pass to `game_loop`
- [ ] `socket.c`: add `global_http_port`, `handle_http_request`, update `game_loop`
- [ ] `socket.c`: remove HTTP block from `handle_websocket_handshake`
- [ ] `socket.h`: export `global_http_port`, update `game_loop` prototype
- [ ] `test_websocket_validation.c`: rename wholist test → who
- [ ] `web/README.md`: update endpoint docs
- [ ] `make lint && make ack && make unit-tests` all pass
