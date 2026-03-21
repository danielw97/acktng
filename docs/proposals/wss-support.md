# WSS (WebSocket Secure) Support

## Problem

The ACK!TNG server already speaks the WebSocket protocol (RFC 6455) and currently
accepts connections on port `8890`. The web client selects `wss://` automatically
when the page is loaded over HTTPS, but browsers enforce the **mixed-content
policy**: a page served over HTTPS cannot open a plain `ws://` connection. The
server binds to all interfaces (`0.0.0.0:8890`), accepting unencrypted connections
only. As soon as `ackmud.com` is served over HTTPS, the in-browser MUD client
becomes completely non-functional.

## Goals

1. Browser clients connecting via `wss://ackmud.com:8890/` can reach the game.
2. All traffic between browser and server is TLS-encrypted.
3. The WebSocket protocol layer and MUD game logic inside the server are unchanged.
4. The solution is maintainable and straightforward to operate.

## Approach

The TLS handshake is handled by a **reverse proxy** (nginx, already present for
the HTTPS web server) running on the same host. The proxy terminates TLS on the
public port and forwards plain WebSocket frames to the game server on a loopback
port. This is the architecture the web-client proposal documents:

```
Browser
  │  wss://ackmud.com:8890  (TLS, public internet)
  ▼
nginx  (same host)           ← terminates TLS
  │  ws://127.0.0.1:18890   (plain WebSocket, loopback)
  ▼
ACK!TNG game server
```

The game server's WebSocket implementation, MUD logic, and message protocol are
completely unchanged. The only code change is to the bind address so the server
listens on `127.0.0.1:18890` instead of `0.0.0.0:8890`.

## src/ Change: Bind Address

### Current behaviour

`init_socket()` in `socket.c` zero-initialises the `sockaddr_in` struct, leaving
`sin_addr` as `INADDR_ANY` (0.0.0.0):

```c
sa = sa_zero;          /* sin_addr is 0 → INADDR_ANY */
sa.sin_family = AF_INET;
sa.sin_port = htons(port);
bind(fd, (struct sockaddr *)&sa, sizeof(sa));
```

### Proposed change

Add an optional `--loopback` / `-l` command-line flag. When present, the server
sets `sin_addr` to `INADDR_LOOPBACK` (127.0.0.1) before calling `bind()`. The
default (no flag) keeps the existing all-interfaces behaviour so local development
and the telnet port are unaffected.

```c
/* in main(), alongside the existing port argument parsing */
bool bind_loopback = FALSE;
for (int i = 1; i < argc; i++) {
    if (!strcmp(argv[i], "--loopback") || !strcmp(argv[i], "-l"))
        bind_loopback = TRUE;
    else if (is_number(argv[i]))
        port = atoi(argv[i]);
}
```

```c
/* in init_socket(), new parameter or use a global like global_port */
sa.sin_addr.s_addr = bind_loopback ? htonl(INADDR_LOOPBACK) : htonl(INADDR_ANY);
```

Production startup becomes:

```sh
cd area && ../src/ack 18890 --loopback
```

### Affected files

| File | Change |
|------|--------|
| `src/comm.c` | Parse `--loopback` / `-l` flag in `main()`; store in a global (`bind_loopback`) |
| `src/socket.c` | Use `bind_loopback` in `init_socket()` to set `sin_addr` |

No other source files change. No struct changes. No protocol changes.

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

### 2. Firewall: block inner port from the internet

Port `18890` must not be reachable from the public internet — only the nginx
proxy (on loopback) should reach the game server directly.

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

Update whatever init script or process manager launches ACK!TNG to pass the new
port and flag:

```sh
# Before
cd /home/mud/acktng/area && ../src/ack 8890

# After
cd /home/mud/acktng/area && ../src/ack 18890 --loopback
```

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
| `src/comm.c` | Parse `--loopback` / `-l` flag |
| `src/socket.c` | Bind to `127.0.0.1` when flag is set |
| nginx config | New `ackmud-wss.conf` — proxy `8890 (wss)` → `18890 (ws loopback)` |
| Firewall | Block `18890` from external traffic |
| Startup command | `ack 18890 --loopback` |
| TLS certificate | Reuse existing Let's Encrypt cert; add post-renewal reload hook |
