# ACK!MUD TNG — Port Assignments

These port numbers are **fixed**.  Do not change them without updating every
location listed under each entry.

---

## 8890 — Plain Telnet

| Field       | Value |
|-------------|-------|
| Protocol    | Raw TCP / plain telnet |
| Clients     | Any telnet client (`telnet ackmud.com 8890`) |
| Server arg  | Positional: `../src/ack 8890 ...` |
| TLS?        | **Never.**  This port is always plain text. |

**Do not** pass this port via `--sniff-port` or `--tls-port`.  It must be the
positional (first) argument to the server binary so it opens a plain-telnet
listener with no TLS context attached.

---

## 9890 — TLS Telnet ("connect securely")

| Field       | Value |
|-------------|-------|
| Protocol    | TLS-wrapped telnet |
| Clients     | Mudlet "Connect Securely", or any telnet-over-TLS client |
| Server arg  | `--tls-port 9890` |
| Cert/key    | `--tls-cert` / `--tls-key` (Let's Encrypt in production; self-signed in dev) |
| TLS?        | **Always.**  The server performs an immediate TLS handshake on accept. |

This port requires a valid TLS certificate and key.  In production these come
from Let's Encrypt (`/etc/letsencrypt/live/ackmud.com/`).  In development the
`data/tls/generate-certs.sh` script creates a self-signed cert.

---

## 18890 — WebSocket (ws://)

| Field       | Value |
|-------------|-------|
| Protocol    | Plain WebSocket (`ws://`) |
| Clients     | Browser-based clients, Mudlet WebSocket mode |
| Server arg  | `--ws-loopback 18890` (binds to 127.0.0.1 only) |
| TLS?        | No.  WSS (`wss://`) is not yet implemented; see `docs/proposals/` for design. |

`--ws-loopback` binds only to the loopback interface (127.0.0.1).  Public
WebSocket access currently requires a TLS-terminating proxy (e.g. nginx) on
the same host forwarding `wss://ackmud.com:18890` → `ws://127.0.0.1:18890`.
A native `--wss-port` option (server-side TLS WebSocket) is planned.

---

## Where These Values Appear

If you need to change a port assignment, update **all** of:

- `docs/ports.md` (this file)
- `startup` — dev startup script (`PORT`, `TLS_PORT`, `WS_PORT` defaults)
- `scripts/startup` — production startup script (`telnet_port`, `tls_port`, `ws_port`)
- `integration-test.sh` — WebSocket integration test (ephemeral ports, but comment)
- `integration-test-telnet.sh` — plain telnet integration test
- `integration-test-telnet-tls.sh` — TLS telnet integration test
- `src/Makefile` — target comments
- Any client-side configuration (Mudlet profiles, web client config, etc.)
