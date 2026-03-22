# Design Proposal: Single-Port TLS + Plain Telnet via Protocol Sniffing

**Date:** 2026-03-22
**Status:** Pending approval

---

## Problem

Currently the server requires two separate ports: one for plain telnet and one for TLS (e.g. `--tls-port 8880`). Players and clients must know which port to connect to based on whether they want encryption. This creates friction and requires firewall rules for both ports.

The goal is to allow a single port to accept both plain telnet and TLS connections transparently, without changing client behaviour.

---

## Approach: Protocol Sniffing via MSG_PEEK

A TLS `ClientHello` always begins with the byte `0x16` (decimal 22), which is the TLS record-layer content type for "Handshake". A plain telnet connection never legitimately starts with `0x16` — telnet IAC (`0xFF`) and ASCII printable text are the normal first bytes.

After `accept()`, before doing anything else with the new connection, we peek at the first byte using `recv(..., MSG_PEEK)`. Because the socket is set to non-blocking mode, we loop with a short `select()` timeout (up to ~1 second) waiting for at least one byte to arrive. Based on that byte:

- `0x16` → treat as TLS: call `SSL_new` / `SSL_accept` as today
- anything else → treat as plain telnet: proceed as today

This approach requires **no changes to clients** and **no new listening socket**.

---

## Current Architecture Summary

Relevant code lives entirely in `src/socket.c` and `src/comm.c`:

- `init_socket(port, addr)` — creates a listening socket
- `game_loop(control, control_ws, control_tls)` — `select()` loop; calls `new_descriptor(fd, is_tls)` when a listening socket fires
- `new_descriptor(int control, bool is_tls)` — `accept()`s the connection, optionally does `SSL_accept` based on `is_tls`
- `DESCRIPTOR_DATA` — per-connection struct; `tls_active` + `ssl` fields already exist
- Read/write already branch on `d->tls_active`

---

## Proposed Changes

### 1. New function: `sniff_is_tls(int fd)` in `src/socket.c`

```c
/* Returns TRUE if the first byte on fd looks like a TLS ClientHello.
 * Uses MSG_PEEK so the byte stays in the socket buffer.
 * Waits up to ~1 second for data to arrive (non-blocking socket). */
static bool sniff_is_tls(int fd);
```

Implementation outline:
1. Use `select()` with a 1-second timeout waiting for `fd` to become readable.
2. `recv(fd, &byte, 1, MSG_PEEK)` — one byte, non-consuming.
3. Return `byte == 0x16`.
4. On timeout or error, return `FALSE` (fail safe to plain telnet).

### 2. New command-line flag: `--sniff-port <N>` in `src/comm.c`

Add a fourth listening socket `control_sniff` bound to the given port on `INADDR_ANY`. This is distinct from `--tls-port` (dedicated TLS-only) so both modes can coexist and existing deployments are not broken.

Alternatively, the feature can reuse the `--tls-port` flag with a new `--tls-sniff` boolean, but a dedicated `--sniff-port` flag is cleaner and more explicit.

### 3. `game_loop` changes in `src/socket.c`

Add `control_sniff` to the signature and `select()` fd_set. When it fires, call `new_descriptor(control_sniff, FALSE)` — but `new_descriptor` will be modified (see below).

Signature change:
```c
void game_loop(int control, int control_ws, int control_tls, int control_sniff);
```

### 4. `new_descriptor` changes in `src/socket.c`

Add a third parameter `bool do_sniff`. When `do_sniff` is `TRUE`, after `accept()` and before any login processing, call `sniff_is_tls(desc)` and set `is_tls` accordingly.

```c
static bool new_descriptor(int control, bool is_tls, bool do_sniff);
```

When `do_sniff=TRUE, is_tls=FALSE`: sniff the byte and override `is_tls` if needed.
All existing call sites pass `do_sniff=FALSE` to preserve current behaviour.

### 5. Global state and startup logging in `src/comm.c`

Add `global_sniff_port` analogous to `global_tls_port`. Update startup log messages to include the sniff port when active. The TLS context must already be initialised (cert + key required) for sniff mode to do TLS, so the existing `--tls-cert` / `--tls-key` flags supply those.

---

## Affected Files

| File | Change |
|---|---|
| `src/socket.c` | Add `sniff_is_tls()`; update `new_descriptor()` signature; update `game_loop()` signature and fd_set handling |
| `src/comm.c` | Parse `--sniff-port` arg; init fourth listening socket; pass to `game_loop()` |
| `src/headers/socket.h` | Declare `global_sniff_port`; update `game_loop()` prototype |

No changes to `ack.h`, `typedefs.h`, or any other files.

---

## Trade-offs and Considerations

**Sniff latency:** The `select()` wait in `sniff_is_tls()` adds up to 1 second on connection if the client is very slow to send its first byte. In practice, TLS clients send `ClientHello` immediately, and telnet clients either send nothing (wait for server prompt) or send immediately too. A timeout of ~500ms is probably sufficient.

**Telnet client sends nothing first:** Some telnet clients wait for the server to send a prompt before sending anything. In this case the 1-second `select()` will time out, we return `FALSE`, and proceed as plain telnet. The server then sends the login banner, the client responds — this works correctly.

**No PROXY protocol / HAProxy support needed:** The sniff approach works end-to-end without a TLS-terminating proxy.

**`--tls-port` unchanged:** Existing deployments using a dedicated TLS port continue to work. `--sniff-port` is an additive option.

**WebSocket port unaffected:** The WebSocket loopback port (`--ws-loopback`) is not changed.

**Unit test:** A unit test can exercise `sniff_is_tls()` by creating a socket pair, writing `0x16` to one end, and asserting `TRUE` is returned; writing `0xFF` and asserting `FALSE`.

---

## Out of Scope

- STARTTLS negotiation
- TLS on the WebSocket loopback port
- IPv6 support (not currently supported by the server)
