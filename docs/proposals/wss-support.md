# WSS (WebSocket Secure) Support

## Problem

The ACK!TNG server already speaks the WebSocket protocol (RFC 6455) and currently
accepts connections on port `8890`. The web client selects `wss://` automatically
when the page is loaded over HTTPS, but browsers enforce the **mixed-content
policy**: a page served over HTTPS cannot open a plain `ws://` connection. The
server binds to all interfaces (`0.0.0.0:8890`), accepting unencrypted connections
only. As soon as `ackmud.com` is served over HTTPS, the in-browser MUD client
becomes completely non-functional.

Telnet access must remain available at all times — traditional MUD clients
connecting directly via telnet must not be disrupted by this change.

## Goals

1. Browser clients connecting via `wss://ackmud.com:8890/` can reach the game.
2. All traffic between browser and server is TLS-encrypted.
3. Telnet players can continue connecting directly on a separate public port.
4. The WebSocket protocol layer and MUD game logic inside the server are unchanged.
5. The solution is maintainable and straightforward to operate.

## Approach

The TLS handshake is handled by a **reverse proxy** (nginx, already present for
the HTTPS web server) running on the same host. The proxy terminates TLS on the
public WebSocket port and forwards plain WebSocket frames to the game server on a
loopback port. Telnet clients continue to connect on a separate public port that
the server still binds to `0.0.0.0`.

```
Telnet client
  │  telnet ackmud.com:4000  (plain TCP, public internet)
  ▼
ACK!TNG game server  ←── 0.0.0.0:4000 (telnet socket)
  ▲
  └── 127.0.0.1:18890 (WebSocket socket, loopback only)
  ▲
nginx  (same host)           ← terminates TLS
  ▲
Browser
  │  wss://ackmud.com:8890  (TLS, public internet)
```

The server opens **two listening sockets simultaneously**: one for telnet on the
public interface, one for WebSocket on loopback. The `game_loop()` select loop
already multiplexes arbitrary numbers of descriptors; it needs only to also watch
two control sockets instead of one.

## src/ Changes

### 1. Command-line arguments

Add a `--ws-loopback <port>` option. When present, the server opens a second
listening socket bound to `127.0.0.1:<port>` in addition to the existing telnet
socket.

```sh
# Telnet on 4000 (public), WebSocket on 18890 (loopback for nginx proxy)
cd area && ../src/ack 4000 --ws-loopback 18890
```

The first positional argument remains the telnet port (default 1234 if omitted).
Running without `--ws-loopback` is fully backward-compatible.

### 2. `init_socket()` — accept a bind address parameter

Add an `in_addr_t bind_addr` parameter to `init_socket()` so callers can specify
`INADDR_ANY` (telnet) or `INADDR_LOOPBACK` (WebSocket):

```c
int init_socket(int port, in_addr_t bind_addr)
{
    ...
    sa.sin_addr.s_addr = htonl(bind_addr);
    sa.sin_port        = htons(port);
    bind(fd, (struct sockaddr *)&sa, sizeof(sa));
    ...
}
```

Existing call sites pass `INADDR_ANY`; the new WebSocket call passes
`INADDR_LOOPBACK`.

### 3. `game_loop()` — select on two control sockets

`game_loop()` currently takes a single `int control` fd. Extend it to accept a
second `int control_ws` fd (or `-1` when not configured). Both fds are added to
`in_set` in the select loop, and `new_descriptor()` is called for whichever fires:

```c
void game_loop(int control, int control_ws)
{
    ...
    FD_SET(control, &in_set);
    if (control_ws >= 0)
        FD_SET(control_ws, &in_set);
    maxdesc = UMAX(control, control_ws);
    ...
    if (FD_ISSET(control, &in_set))
        new_descriptor(control);
    if (control_ws >= 0 && FD_ISSET(control_ws, &in_set))
        new_descriptor(control_ws);
    ...
}
```

The SIGUSR1 socket-reopen path also needs to reopen both sockets if both are
active.

### 4. `main()` — parse `--ws-loopback` and open both sockets

```c
int ws_port = -1;
/* argument parsing loop */
if (!strcmp(argv[i], "--ws-loopback") && i + 1 < argc)
    ws_port = atoi(argv[++i]);

control    = init_socket(port, INADDR_ANY);
control_ws = (ws_port > 0) ? init_socket(ws_port, INADDR_LOOPBACK) : -1;
game_loop(control, control_ws);
```

### Affected files

| File | Change |
|------|--------|
| `src/comm.c` | Parse `--ws-loopback <port>`; open second socket; pass both fds to `game_loop()` |
| `src/socket.c` | Add `bind_addr` parameter to `init_socket()`; extend `game_loop()` to select on two control fds; update SIGUSR1 reopen path |

No other source files change. No structs change. No protocol or MUD logic changes.

### Unit test coverage

A unit test should verify that `init_socket()` correctly sets `sin_addr` for both
`INADDR_ANY` and `INADDR_LOOPBACK`. The dual-`game_loop()` select path does not
require new tests beyond the existing integration test (boot + login), which
exercises the code end-to-end.

## Infrastructure Changes (outside src/)

These are operational steps required on the server host. They do not touch the
C codebase.

### 1. nginx proxy configuration

Add a new nginx config file for the WebSocket proxy. The existing TLS certificate
used for `https://ackmud.com` is reused — no separate certificate is needed.

```nginx
# /etc/nginx/conf.d/ackmud-wss.conf
server {
    listen      8890 ssl;
    server_name ackmud.com;

    ssl_certificate     /etc/letsencrypt/live/ackmud.com/fullchain.pem;
    ssl_certificate_key /etc/letsencrypt/live/ackmud.com/privkey.pem;
    ssl_protocols       TLSv1.2 TLSv1.3;
    ssl_ciphers         HIGH:!aNULL:!MD5;

    location / {
        proxy_pass          http://127.0.0.1:18890;
        proxy_http_version  1.1;
        proxy_set_header    Upgrade    $http_upgrade;
        proxy_set_header    Connection "upgrade";
        proxy_set_header    Host       $host;
        # MUD sessions are long-lived; prevent nginx from dropping idle connections.
        proxy_read_timeout  3600s;
        proxy_send_timeout  3600s;
    }
}
```

Validate and reload:

```bash
sudo nginx -t && sudo systemctl reload nginx
```

### 2. Firewall: block inner WebSocket port from the internet

Port `18890` must not be reachable from the public internet — only the nginx
proxy (on loopback) should reach the game server on that port. The telnet port
(`4000`) remains open as before.

```bash
sudo iptables -A INPUT -p tcp --dport 18890 ! -s 127.0.0.1 -j DROP
sudo iptables-save > /etc/iptables/rules.v4
```

### 3. TLS certificate auto-renewal hook

Let's Encrypt certificates expire every 90 days. nginx must reload after renewal
to pick up the new certificate.

```bash
# /etc/letsencrypt/renewal-hooks/deploy/reload-ackmud-wss.sh
#!/bin/bash
systemctl reload nginx
```

```bash
chmod +x /etc/letsencrypt/renewal-hooks/deploy/reload-ackmud-wss.sh
sudo certbot renew --dry-run   # confirm the hook fires
```

### 4. Startup script update

Update whatever init script or process manager launches ACK!TNG:

```sh
# Before
cd /home/mud/acktng/area && ../src/ack 8890

# After
cd /home/mud/acktng/area && ../src/ack 4000 --ws-loopback 18890
```

(Replace `4000` with whatever telnet port is desired.)

## Trade-offs

| Option | Pro | Con |
|--------|-----|-----|
| nginx proxy (this proposal) | Reuses existing cert and nginx install; HTTP-level logging and rate-limiting available | nginx must be installed |
| stunnel | Simpler config; pure TCP layer | No HTTP-level features; separate install if not present |
| Native TLS in server (OpenSSL) | No proxy needed | Major src/ change; adds dependency; certificate management in C |

nginx is strongly preferred because it is already running for the HTTPS web
server, meaning no new software needs to be installed and the existing certificate
is reused directly.

## Summary

| Component | Change |
|-----------|--------|
| `src/comm.c` | Parse `--ws-loopback <port>`; open second loopback socket; pass both to `game_loop()` |
| `src/socket.c` | `init_socket()` gains `bind_addr` param; `game_loop()` selects on two control fds |
| nginx config | New `ackmud-wss.conf` — proxy `8890 (wss)` → `18890 (ws loopback)` |
| Firewall | Block `18890` from external traffic; telnet port remains open |
| Startup command | `ack 4000 --ws-loopback 18890` |
| TLS certificate | Reuse existing Let's Encrypt cert; add post-renewal reload hook |
