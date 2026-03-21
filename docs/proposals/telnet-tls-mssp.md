# Native TLS for Telnet + MSSP Support

## Problem

ACK!TNG currently provides two connection paths:

- **Plain telnet** — traditional MUD clients connect on the public port; no encryption.
- **WebSocket** — browser clients connect through an nginx reverse proxy that terminates TLS
  (`wss://`) and forwards plain frames to the game server on a loopback port.

Two gaps remain:

1. **No encrypted telnet for traditional MUD clients.** Players using clients such as Mudlet,
   TinTin++, or raw `openssl s_client` have no way to encrypt their session. Passwords cross the
   network in plaintext.

2. **No MSSP support.** MSSP (Mud Server Status Protocol, telnet option 70) is the industry
   standard mechanism by which MUD listing sites (The Mud Connector, MudStats, etc.) automatically
   query live server statistics — player count, uptime, game features. Without MSSP, the server is
   invisible to crawlers and cannot maintain accurate listings.

## Goals

1. Accept native TLS-encrypted telnet on an optional third port (`--tls-port`), alongside the
   existing plain-telnet and WebSocket-loopback ports.
2. Implement MSSP (telnet option 70) on all telnet connections (plain and TLS), advertising the
   full recommended variable set to listing-site crawlers.
3. Introduce proper telnet IAC option negotiation so future options (GMCP, MCCP, etc.) can be
   added cleanly.
4. Make TLS support conditionally compiled — the server still builds and runs normally when OpenSSL
   is absent; `--tls-port` prints a warning and is ignored.
5. No breaking changes to existing plain-telnet or WebSocket paths.

## Approach

```
Traditional MUD client
  │  telnet ackmud.com:4000  (plain TCP, public internet)
  ▼
ACK!TNG game server  ←── 0.0.0.0:4000 (plain telnet socket, existing)
  ▲
  ├── 0.0.0.0:4443 (TLS telnet socket, new)          ← native OpenSSL
  ▲
  └── 127.0.0.1:18890 (WebSocket loopback, existing) ← nginx proxy
  ▲
Browser  ─── wss://ackmud.com:9890 ──▶  nginx (TLS termination)
```

`game_loop()` currently watches two control sockets (telnet + optional WebSocket loopback) via
`select()`. We extend it to watch three (adding the optional TLS telnet socket). After
`accept()`, TLS connections perform an OpenSSL handshake before entering the normal descriptor
lifecycle. All subsequent I/O on a TLS descriptor goes through `SSL_read`/`SSL_write` instead of
`read`/`write`.

MSSP sits entirely at the telnet application layer. When the server sends the initial greeting
to any telnet (plain or TLS) descriptor it also advertises `IAC WILL MSSP`. If the client
responds with `IAC DO MSSP` the server sends the subnegotiation block. A new
`process_telnet_options()` function strips and handles all IAC sequences from the raw input
buffer before command processing.

---

## Part A — Native TLS for Telnet

### A1. Command-line arguments (`src/comm.c`)

Add three new options to the argument-parsing loop in `main()`:

```c
--tls-port <port>   Listening port for TLS telnet (optional; >1024)
--tls-cert <path>   Path to PEM certificate file (default: ../data/tls/cert.pem)
--tls-key  <path>   Path to PEM private key file  (default: ../data/tls/key.pem)
```

Invocation example:

```sh
cd area && ../src/ack 4000 --ws-loopback 18890 \
    --tls-port 4443 --tls-cert ../data/tls/cert.pem --tls-key ../data/tls/key.pem
```

Running without `--tls-port` is fully backward-compatible.

### A2. OpenSSL initialisation — `init_tls_context()` (`src/socket.c`)

A new function creates the shared `SSL_CTX` used for all TLS connections:

```c
#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>

static SSL_CTX *global_ssl_ctx = NULL;

bool init_tls_context(const char *cert_file, const char *key_file)
{
    SSL_CTX *ctx;
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();

    ctx = SSL_CTX_new(TLS_server_method());
    if (!ctx) { ERR_print_errors_fp(stderr); return FALSE; }

    SSL_CTX_set_min_proto_version(ctx, TLS1_2_VERSION);

    if (SSL_CTX_use_certificate_file(ctx, cert_file, SSL_FILETYPE_PEM) <= 0 ||
        SSL_CTX_use_PrivateKey_file(ctx, key_file, SSL_FILETYPE_PEM) <= 0)
    {
        ERR_print_errors_fp(stderr);
        SSL_CTX_free(ctx);
        return FALSE;
    }
    global_ssl_ctx = ctx;
    return TRUE;
}
#endif
```

Called from `main()` before `game_loop()` when `--tls-port` is present.

### A3. `DESCRIPTOR_DATA` additions (`src/headers/ack.h`)

```c
#ifdef HAVE_OPENSSL
    struct ssl_st *ssl;    /* NULL for plain connections */
#endif
    bool tls_active;       /* TRUE if this descriptor uses TLS */
```

Using the incomplete struct pointer `struct ssl_st *` avoids pulling `<openssl/ssl.h>` into
`ack.h`; the field is opaque outside `socket.c`.

### A4. `game_loop()` — third control socket (`src/socket.c`, `src/headers/socket.h`)

Extend the signature:

```c
void game_loop(int control, int control_ws, int control_tls);
```

Inside the select loop, add the TLS socket alongside the existing two:

```c
if (control_tls >= 0)
{
    FD_SET(control_tls, &in_set);
    maxdesc = UMAX(maxdesc, control_tls);
}
...
if (control_tls >= 0 && FD_ISSET(control_tls, &in_set))
    new_descriptor(control_tls, TRUE);  /* TRUE = is_tls */
```

The SIGUSR1 reopen path (`reopen_flag`) is updated to also close and reopen `control_tls`.

### A5. `new_descriptor()` — TLS accept and handshake (`src/socket.c`)

Change signature to `void new_descriptor(int control, bool is_tls)`.

After `accept()` and `fcntl(FNDELAY)`, insert TLS handshake when `is_tls`:

```c
#ifdef HAVE_OPENSSL
if (is_tls)
{
    SSL *ssl = SSL_new(global_ssl_ctx);
    SSL_set_fd(ssl, desc);
    /* Set non-blocking; handle WANT_READ/WANT_WRITE in handshake loop */
    if (SSL_accept(ssl) <= 0)
    {
        ERR_print_errors_fp(stderr);
        SSL_free(ssl);
        close(desc);
        return;
    }
    dnew->ssl = ssl;
    dnew->tls_active = TRUE;
}
#endif
```

Note: with non-blocking sockets `SSL_accept()` may require multiple calls if it returns
`SSL_ERROR_WANT_READ`. The implementation will use a short retry loop (up to 5 iterations with
`select()`) before giving up and closing the connection.

### A6. I/O wrappers — `read_from_descriptor()` and `write_to_descriptor()` (`src/socket.c`)

**Read:** Replace the bare `read()` call with a TLS-aware wrapper:

```c
#ifdef HAVE_OPENSSL
if (d->tls_active)
    nRead = SSL_read(d->ssl, d->inbuf + iStart, sizeof(d->inbuf) - 10 - iStart);
else
#endif
    nRead = read(d->descriptor, d->inbuf + iStart, sizeof(d->inbuf) - 10 - iStart);
```

`SSL_ERROR_WANT_READ` maps to the same `EWOULDBLOCK` break.

**Write:** `write_to_descriptor()` currently takes a raw `int desc`. Change the signature to
accept `DESCRIPTOR_DATA *d` so the SSL pointer is reachable:

```c
bool write_to_descriptor(DESCRIPTOR_DATA *d, char *txt, int length);
```

The body dispatches on `d->tls_active`:

```c
#ifdef HAVE_OPENSSL
if (d->tls_active)
    nWrite = SSL_write(d->ssl, txt + iStart, nBlock);
else
#endif
    nWrite = write(d->descriptor, txt + iStart, nBlock);
```

All existing call sites pass `d->descriptor` as the first argument today; they will be updated
to pass `d` instead. The handful of early-disconnect paths that call
`write_to_descriptor(desc, ...)` before a `DESCRIPTOR_DATA` exists are TLS-ineligible by
construction (TLS handshake happens inside `new_descriptor()`) and can pass a one-off plain-fd
overload or be inlined.

### A7. `close_socket()` — TLS teardown (`src/socket.c`)

Before `close(dclose->descriptor)`:

```c
#ifdef HAVE_OPENSSL
if (dclose->tls_active && dclose->ssl)
{
    SSL_shutdown(dclose->ssl);
    SSL_free(dclose->ssl);
    dclose->ssl = NULL;
}
#endif
```

### A8. Hotreboot limitation

TLS SSL state cannot be serialised across an `exec()`. Before the hotreboot exec, any TLS
descriptor must be gracefully disconnected with a message:

> `Your connection is encrypted (TLS) and cannot survive a hotreboot. Reconnect after the reboot.`

This is handled in `hotreboot.c` (the `do_hotreboot` / copyover path) by iterating `first_desc`
and closing all `tls_active` descriptors before `execl()`.

### A9. `src/Makefile` — conditional OpenSSL linkage

```makefile
# Detect OpenSSL at build time
OPENSSL_CFLAGS := $(shell pkg-config --cflags openssl 2>/dev/null)
OPENSSL_LIBS   := $(shell pkg-config --libs   openssl 2>/dev/null)
ifneq ($(OPENSSL_LIBS),)
    C_FLAGS += -DHAVE_OPENSSL $(OPENSSL_CFLAGS)
    L_FLAGS += $(OPENSSL_LIBS)
endif
```

If OpenSSL is absent, `HAVE_OPENSSL` is not defined and all TLS code is excluded at compile time.

---

## Part B — MSSP (Mud Server Status Protocol)

### B1. Protocol constants (`src/headers/socket.h`)

```c
/* MSSP telnet option (http://tintin.sourceforge.net/mssp/) */
#define TELOPT_MSSP  70
#define MSSP_VAR      1
#define MSSP_VAL      2
```

### B2. Telnet IAC option parser — `process_telnet_options()` (`src/socket.c`)

`read_from_buffer()` currently copies only printable characters into `incomm[]`, which silently
discards IAC sequences. This is fine for play but means we never see `IAC DO MSSP`. A new
function scans `d->inbuf` for IAC sequences *before* the line-extraction loop, handles them, and
removes them from the buffer in-place:

```c
static void process_telnet_options(DESCRIPTOR_DATA *d)
{
    unsigned char *buf = (unsigned char *)d->inbuf;
    int i = 0, out = 0;

    while (i < d->inbuf_len)
    {
        if (buf[i] != IAC)
        {
            buf[out++] = buf[i++];
            continue;
        }
        /* IAC sequence */
        if (i + 1 >= d->inbuf_len) break;  /* incomplete, leave it */

        unsigned char cmd = buf[i + 1];

        if (cmd == GA || cmd == NOP)        /* 2-byte: IAC GA / NOP */
        {
            i += 2;
        }
        else if (cmd == SB)                 /* subnegotiation — skip to IAC SE */
        {
            int j = i + 2;
            while (j + 1 < d->inbuf_len && !(buf[j] == IAC && buf[j+1] == SE))
                j++;
            i = (j + 1 < d->inbuf_len) ? j + 2 : d->inbuf_len;
        }
        else if (cmd == WILL || cmd == WONT || cmd == DO || cmd == DONT)
        {
            if (i + 2 >= d->inbuf_len) break;  /* incomplete */
            unsigned char opt = buf[i + 2];

            if (cmd == DO && opt == TELOPT_MSSP)
                send_mssp_data(d);
            /* Respond WONT to any other WILL/DO we don't support */
            /* (silence is acceptable; WONT/DONT adds noise for unknown options) */
            i += 3;
        }
        else
        {
            i += 2;  /* unknown 2-byte IAC sequence — skip */
        }
    }

    d->inbuf_len = out;
    d->inbuf[out] = '\0';
}
```

`process_telnet_options(d)` is called at the top of `read_from_buffer()` when
`!d->websocket_active`.

### B3. MSSP advertisement on connect — `queue_login_greeting()` (`src/socket.c`)

Add `IAC WILL MSSP` to the telnet greeting, immediately after the existing `IAC WILL ECHO`:

```c
/* Existing */
write_to_descriptor(d, echo_off_str, 0);    /* IAC WILL ECHO */
/* New */
{
    const char mssp_will[] = {IAC, WILL, TELOPT_MSSP, '\0'};
    write_to_descriptor(d, (char *)mssp_will, 3);
}
```

Not sent to WebSocket descriptors (guarded by `!d->websocket_active`).

### B4. MSSP subnegotiation builder — `send_mssp_data()` (`src/socket.c`)

```c
static void send_mssp_data(DESCRIPTOR_DATA *d)
{
    char buf[4096];
    int  n = 0;

#define MVAR(k)       buf[n++] = MSSP_VAR; \
                      memcpy(buf+n, k, strlen(k)); n += strlen(k)
#define MVAL(v)       buf[n++] = MSSP_VAL; \
                      memcpy(buf+n, v, strlen(v)); n += strlen(v)
#define MVAL_INT(i)   { char tmp[32]; sprintf(tmp, "%d", (i)); MVAL(tmp); }

    buf[n++] = IAC; buf[n++] = SB; buf[n++] = TELOPT_MSSP;

    /* --- Mandatory --- */
    MVAR("NAME");       MVAL(MUDNAME_PLAIN);
    MVAR("PLAYERS");    MVAL_INT(cur_players);
    MVAR("UPTIME");     MVAL_INT((int)boot_time);   /* global time_t set at startup */

    /* --- Server identity --- */
    MVAR("CRAWL DELAY"); MVAL("-1");
    MVAR("HOSTNAME");    MVAL("");
    MVAR("PORT");        MVAL_INT(global_port);
    MVAR("CODEBASE");    MVAL("ACK! 4.3.1");
    MVAR("CONTACT");     MVAL(MSSP_CONTACT);
    MVAR("CREATED");     MVAL("1994");
    MVAR("ICON");        MVAL(MSSP_ICON_URL);
    MVAR("IP");          MVAL("");
    MVAR("LANGUAGE");    MVAL("EN");
    MVAR("LOCATION");    MVAL(MSSP_LOCATION);
    MVAR("MINIMUM AGE"); MVAL("0");
    MVAR("WEBSITE");     MVAL(MSSP_WEBSITE);

    /* --- Gameplay counts (cached at boot) --- */
    MVAR("AREAS");    MVAL_INT(mssp_area_count);
    MVAR("HELPFILES"); MVAL_INT(mssp_help_count);
    MVAR("MOBILES");  MVAL_INT(mssp_mob_count);
    MVAR("OBJECTS");  MVAL_INT(mssp_obj_count);
    MVAR("ROOMS");    MVAL_INT(mssp_room_count);
    MVAR("CLASSES");  MVAL_INT(MAX_CLASS);
    MVAR("LEVELS");   MVAL_INT(MAX_LEVEL);
    MVAR("RACES");    MVAL_INT(mssp_race_count);
    MVAR("SKILLS");   MVAL_INT(mssp_skill_count);

    /* --- Protocol feature flags --- */
    MVAR("ANSI");              MVAL("1");
    MVAR("GMCP");              MVAL("0");
    MVAR("MCCP");              MVAL("0");
    MVAR("MCP");               MVAL("0");
    MVAR("MSDP");              MVAL("0");
    MVAR("MSSP");              MVAL("1");
    MVAR("MSP");               MVAL("0");
    MVAR("MXP");               MVAL("0");
#ifdef HAVE_OPENSSL
    MVAR("SSL");               MVAL(global_tls_port > 0 ? "1" : "0");
#else
    MVAR("SSL");               MVAL("0");
#endif
    MVAR("VT100");             MVAL("1");
    MVAR("XTERM 256 COLORS");  MVAL("0");
    MVAR("UTF-8");             MVAL("0");

    /* --- Gameplay attributes --- */
    MVAR("PAY TO PLAY");       MVAL("0");
    MVAR("PAY FOR PERKS");     MVAL("0");
    MVAR("HIRING BUILDERS");   MVAL("0");
    MVAR("HIRING CODERS");     MVAL("0");
    MVAR("ADULT MATERIAL");    MVAL("0");
    MVAR("MULTICLASSING");     MVAL("1");
    MVAR("NEWBIE FRIENDLY");   MVAL("1");
    MVAR("PLAYER CLANS");      MVAL("1");
    MVAR("PLAYER CRAFTING");   MVAL("0");
    MVAR("PLAYER KILLING");    MVAL("1");
    MVAR("QUEST SYSTEM");      MVAL("1");
    MVAR("ROLEPLAYING");       MVAL("0");
    MVAR("TRAINING SYSTEM");   MVAL("1");
    MVAR("WORLD ORIGINALITY"); MVAL("Mostly original");

    buf[n++] = IAC; buf[n++] = SE;

#undef MVAR
#undef MVAL
#undef MVAL_INT

    write_to_descriptor(d, buf, n);
}
```

### B5. Boot-time MSSP counter cache (`src/socket.c` / `src/db.c`)

The counts for AREAS, HELPFILES, MOBILES, OBJECTS, ROOMS, RACES, SKILLS are computed once after
`boot_db()` completes and stored in globals:

```c
int mssp_area_count;
int mssp_help_count;
int mssp_mob_count;
int mssp_obj_count;
int mssp_room_count;
int mssp_race_count;
int mssp_skill_count;
time_t boot_time;
```

A new `init_mssp_counts()` function in `db.c` (called from `boot_db()`) walks the relevant
tables to populate these. PLAYERS and UPTIME are always live values.

### B6. Config strings (`src/headers/config.h`)

```c
#define MSSP_CONTACT  "admin@yourmud.com"
#define MSSP_WEBSITE  "https://yourmud.com"
#define MSSP_ICON_URL "https://yourmud.com/icon.png"
#define MSSP_LOCATION "United States"
```

---

## Affected Files

| File | Change |
|------|--------|
| `src/headers/ack.h` | Add `struct ssl_st *ssl` and `bool tls_active` to `DESCRIPTOR_DATA` |
| `src/headers/socket.h` | Update `game_loop`, `write_to_descriptor` prototypes; add MSSP constants |
| `src/headers/config.h` | Add `MSSP_CONTACT`, `MSSP_WEBSITE`, `MSSP_ICON_URL`, `MSSP_LOCATION` |
| `src/socket.c` | `init_tls_context()`, TLS accept in `new_descriptor()`, TLS I/O in read/write, `close_socket()` teardown, `process_telnet_options()`, `send_mssp_data()`, MSSP boot counters, `game_loop()` third socket, `queue_login_greeting()` WILL MSSP |
| `src/comm.c` | CLI parsing for `--tls-port`, `--tls-cert`, `--tls-key`; `game_loop()` call updated |
| `src/db.c` | `init_mssp_counts()` called from `boot_db()` |
| `src/hotreboot.c` | Close TLS descriptors gracefully before `execl()` |
| `src/Makefile` | Conditional `HAVE_OPENSSL` detection via `pkg-config`; add `-lssl -lcrypto` |
| `src/tests/test_mssp.c` | Unit tests for `process_telnet_options()` and `send_mssp_data()` output |

---

## Unit Test Coverage

A new `src/tests/test_mssp.c` tests:

1. **`process_telnet_options()` IAC stripping** — feed a buffer containing `IAC DO MSSP` mixed
   with normal text; verify the IAC sequence is removed from the buffer and the text chars remain.
2. **`process_telnet_options()` unknown option** — `IAC DO 99` is stripped without crash.
3. **`send_mssp_data()` framing** — call `send_mssp_data()` against a stub descriptor; verify the
   output buffer starts with `IAC SB TELOPT_MSSP` and ends with `IAC SE`.
4. **`send_mssp_data()` mandatory keys** — verify NAME, PLAYERS, and UPTIME keys are present in
   the subneg payload.

The TLS handshake path is exercised by the existing integration test: if OpenSSL is available and
a self-signed certificate is placed at the default paths, the integration test can optionally
connect via `openssl s_client` to verify the TLS port.

---

## Trade-offs

| Topic | Decision | Alternative considered |
|-------|----------|----------------------|
| TLS library | OpenSSL (`libssl`) — most common on Linux, well-documented C API | mbedTLS (lighter, no system dependency), stunnel (external, like nginx approach) |
| TLS architecture | Separate `--tls-port` alongside plain telnet | STARTTLS negotiation on the same port (complex, poor MUD-client support) |
| `write_to_descriptor` signature | Change to `DESCRIPTOR_DATA *d` | Add a parallel `tls_write()` function — avoids changing callers but duplicates logic |
| Hotreboot + TLS | Disconnect TLS clients before exec | Attempt to pass SSL session state via env — not feasible without SSL session serialisation |
| MSSP count freshness | Cached at boot (static counts) | Re-counted on every crawl — unnecessary overhead; static game geometry rarely changes |
| Conditional compilation | `#ifdef HAVE_OPENSSL` throughout `socket.c` | Separate `socket_tls.c` file — cleaner but more build system complexity |

---

## Verification Plan

1. **Build without OpenSSL:** `make ack` on a host without `libssl-dev` — must succeed; `--tls-port`
   logs a warning and is ignored.
2. **Build with OpenSSL:** `make ack` with OpenSSL present — `HAVE_OPENSSL` defined, binary links
   `-lssl -lcrypto`.
3. **Unit tests:** `make unit-tests` — `test_mssp` passes all four test cases.
4. **Plain telnet unchanged:** `telnet localhost 4000` — login prompt, no regression.
5. **TLS telnet:** `openssl s_client -connect localhost:4443` — TLS handshake, login prompt.
6. **MSSP query:** connect a MSSP-capable client (e.g. `mudlet`) to the plain port; confirm
   `IAC DO MSSP` triggers a subneg response containing NAME, PLAYERS, PORT.
7. **WebSocket:** browser client — no change to WebSocket or music behaviour.
8. **Integration test:** `make integration-test` — boot, login, 8-second crash check all pass.
