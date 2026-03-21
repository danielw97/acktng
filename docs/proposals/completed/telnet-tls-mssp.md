# Native TLS for Telnet + MSSP + MSDP Support

## Problem

ACK!TNG currently provides two connection paths:

- **Plain telnet** — traditional MUD clients connect on the public port; no encryption.
- **WebSocket** — browser clients connect through an nginx reverse proxy that terminates TLS
  (`wss://`) and forwards plain frames to the game server on a loopback port.

Three gaps remain:

1. **No encrypted telnet for traditional MUD clients.** Players using clients such as Mudlet,
   TinTin++, or raw `openssl s_client` have no way to encrypt their session. Passwords cross the
   network in plaintext.

2. **No MSSP support.** MSSP (Mud Server Status Protocol, telnet option 70) is the industry
   standard mechanism by which MUD listing sites (The Mud Connector, MudStats, etc.) automatically
   query live server statistics — player count, uptime, game features. Without MSSP, the server is
   invisible to crawlers and cannot maintain accurate listings.

3. **No MSDP support.** MSDP (Mud Server Data Protocol, telnet option 69) is the standard protocol
   by which feature-rich MUD clients (Mudlet, TinTin++, BlowTorch) receive structured game state
   — health, mana, room info, combat data — to drive gauges, maps, and triggers. Without MSDP,
   players must parse raw text; client-side scripting is fragile and unreliable.

## Goals

1. Accept native TLS-encrypted telnet on an optional third port (`--tls-port`), alongside the
   existing plain-telnet and WebSocket-loopback ports.
2. Implement MSSP (telnet option 70) on all telnet connections (plain and TLS), advertising the
   full recommended variable set to listing-site crawlers.
3. Implement MSDP (telnet option 69) on all telnet connections, delivering real-time structured
   game state to clients that negotiate it.
4. Introduce proper telnet IAC option negotiation infrastructure so MSSP, MSDP, and future options
   (GMCP, MCCP, etc.) share a single clean parsing path.
5. Make TLS support conditionally compiled — the server still builds and runs normally when OpenSSL
   is absent; `--tls-port` prints a warning and is ignored.
6. No breaking changes to existing plain-telnet or WebSocket paths.

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

MSSP and MSDP both sit at the telnet application layer. The server advertises `IAC WILL MSSP`
and `IAC WILL MSDP` in the initial greeting. Clients that support either protocol respond with
`IAC DO <option>`. A new `process_telnet_options()` function is the single dispatch point for
all incoming IAC sequences — it strips them from the input buffer and routes subnegotiations to
protocol-specific handlers.

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
        if (i + 1 >= d->inbuf_len) break;  /* incomplete — leave for next read */

        unsigned char cmd = buf[i + 1];

        if (cmd == GA || cmd == NOP)        /* 2-byte: IAC GA / NOP */
        {
            i += 2;
        }
        else if (cmd == SB)                 /* subnegotiation */
        {
            if (i + 2 >= d->inbuf_len) break;
            unsigned char opt = buf[i + 2];
            /* Find IAC SE terminator */
            int j = i + 3;
            while (j + 1 < d->inbuf_len && !(buf[j] == IAC && buf[j+1] == SE))
                j++;
            if (j + 1 >= d->inbuf_len) break;  /* incomplete subneg — leave */
            /* Dispatch payload (buf[i+3] .. buf[j-1]) to protocol handler */
            if (opt == TELOPT_MSDP)
                process_msdp_subneg(d, buf + i + 3, j - (i + 3));
            /* MSSP has no client→server subneg; ignore others */
            i = j + 2;
        }
        else if (cmd == WILL || cmd == WONT || cmd == DO || cmd == DONT)
        {
            if (i + 2 >= d->inbuf_len) break;  /* incomplete */
            unsigned char opt = buf[i + 2];

            if (cmd == DO && opt == TELOPT_MSSP)
                send_mssp_data(d);
            else if (cmd == DO && opt == TELOPT_MSDP)
            {
                d->msdp_active = TRUE;
                /* Send server identity immediately on negotiation */
                msdp_send_var(d, "SERVER_ID", "ACK!TNG");
                msdp_send_var_int(d, "SERVER_TIME", (int)current_time);
            }
            /* Silence for unsupported options is acceptable per RFC 854 */
            i += 3;
        }
        else
        {
            i += 2;  /* unknown 2-byte sequence — skip */
        }
    }

    d->inbuf_len = out;
    d->inbuf[out] = '\0';
}
```

`process_telnet_options(d)` is called at the top of `read_from_buffer()` when
`!d->websocket_active`.

### B3. MSSP advertisement on connect — `queue_login_greeting()` (`src/socket.c`)

Add `IAC WILL MSSP` and `IAC WILL MSDP` to the telnet greeting, immediately after the existing
`IAC WILL ECHO`:

```c
/* Existing */
write_to_descriptor(d, echo_off_str, 0);    /* IAC WILL ECHO */
/* New — advertise all supported telnet options */
{
    const char proto_will[] = {
        IAC, WILL, TELOPT_MSSP,
        IAC, WILL, TELOPT_MSDP,
        IAC, WILL, TELOPT_GMCP,
        IAC, WILL, TELOPT_MCCP2,
        IAC, WILL, TELOPT_MCCP3,
    };
    write_to_descriptor(d, (char *)proto_will, sizeof(proto_will));
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
    MVAR("GMCP");              MVAL("1");
    MVAR("MCCP");              MVAL("1");
    MVAR("MCP");               MVAL("0");
    MVAR("MSDP");              MVAL("1");
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

## Part C — MSDP (Mud Server Data Protocol)

MSDP (telnet option 69) is a bidirectional structured-data protocol. The client negotiates
`IAC DO MSDP`, then sends commands inside `IAC SB MSDP … IAC SE` blocks to subscribe to named
variables. The server sends current values immediately and then pushes updates whenever those
values change.

### C1. Protocol constants (`src/headers/socket.h`)

```c
/* MSDP telnet option (https://tintin.sourceforge.io/protocols/msdp/) */
#define TELOPT_MSDP        69

#define MSDP_VAR            1   /* variable name follows */
#define MSDP_VAL            2   /* value follows */
#define MSDP_TABLE_OPEN     3   /* start of a key-value table */
#define MSDP_TABLE_CLOSE    4   /* end of a key-value table */
#define MSDP_ARRAY_OPEN     5   /* start of a list */
#define MSDP_ARRAY_CLOSE    6   /* end of a list */
```

### C2. Supported variable set

Each variable has a name (sent over the wire), a bit-position in the subscription bitmask, and
a data source in the game state.

**Character variables** (only meaningful once `d->connected == CON_PLAYING`):

| Variable | Bitmask bit | Source |
|----------|-------------|--------|
| `CHARACTER_NAME` | bit 0 | `ch->name` |
| `HEALTH` | bit 1 | `ch->hit` |
| `HEALTH_MAX` | bit 2 | `ch->max_hit` |
| `MANA` | bit 3 | `ch->mana` |
| `MANA_MAX` | bit 4 | `ch->max_mana` |
| `MOVEMENT` | bit 5 | `ch->move` |
| `MOVEMENT_MAX` | bit 6 | `ch->max_move` |
| `LEVEL` | bit 7 | `ch->level` |
| `EXPERIENCE` | bit 8 | `ch->exp` |
| `EXPERIENCE_TNL` | bit 9 | `exp_per_level(ch->level+1) - ch->exp` |
| `GOLD` | bit 10 | `ch->gold` |
| `ALIGNMENT` | bit 11 | `ch->alignment` |
| `HITROLL` | bit 12 | `ch->hitroll` |
| `DAMROLL` | bit 13 | `ch->damroll` |
| `AC` | bit 14 | `ch->armor` |

**Combat variables** (sent while `ch->fighting != NULL`; zeroed/cleared when not fighting):

| Variable | Bitmask bit | Source |
|----------|-------------|--------|
| `OPPONENT_NAME` | bit 16 | `ch->fighting->name` |
| `OPPONENT_HEALTH` | bit 17 | percentage: `(ch->fighting->hit * 100) / UMAX(1, ch->fighting->max_hit)` |
| `OPPONENT_HEALTH_MAX` | bit 18 | `ch->fighting->max_hit` |
| `OPPONENT_LEVEL` | bit 19 | `ch->fighting->level` |

**Room variables** (resent whenever the player's `in_room` changes):

| Variable | Bitmask bit | Source |
|----------|-------------|--------|
| `ROOM_NAME` | bit 20 | `ch->in_room->name` |
| `ROOM_VNUM` | bit 21 | `ch->in_room->vnum` |
| `ROOM_TERRAIN` | bit 22 | sector name string from `sector_type` |
| `ROOM_EXITS` | bit 23 | table: direction → destination vnum for each open exit |
| `ROOM_FLAGS` | bit 24 | `ch->in_room->room_flags` as integer |

**World variables** (low-frequency; sent on REPORT or change):

| Variable | Bitmask bit | Source |
|----------|-------------|--------|
| `SERVER_ID` | bit 25 | `"ACK!TNG"` (static) |
| `SERVER_TIME` | bit 26 | `(int)current_time` |
| `WORLD_TIME` | bit 27 | in-game hour as integer |

Corresponding defines in `src/headers/socket.h`:

```c
#define MSDP_BIT_CHARACTER_NAME    (1U << 0)
#define MSDP_BIT_HEALTH            (1U << 1)
/* … through … */
#define MSDP_BIT_WORLD_TIME        (1U << 27)
#define MSDP_BIT_ALL_CHAR   0x00007FFF   /* bits 0-14 */
#define MSDP_BIT_ALL_COMBAT 0x000F0000   /* bits 16-19 */
#define MSDP_BIT_ALL_ROOM   0x01F00000   /* bits 20-24 */
#define MSDP_BIT_ALL_WORLD  0x0E000000   /* bits 25-27 */
```

### C3. `DESCRIPTOR_DATA` additions (`src/headers/ack.h`)

```c
    bool         msdp_active;    /* TRUE after client sends IAC DO MSDP */
    unsigned int msdp_reported;  /* bitmask of subscribed variable bits */
    unsigned short msdp_last_room; /* last room vnum sent — detects room change */
```

Only three fields are added. Change detection for character stats is handled by resending on
every combat pulse rather than caching every value individually (see C6).

### C4. Incoming subneg handler — `process_msdp_subneg()` (`src/socket.c`)

Called by `process_telnet_options()` when a complete `IAC SB MSDP … IAC SE` block arrives. The
payload is a sequence of `MSDP_VAR <name> MSDP_VAL <value>` pairs — each pair is a client
command:

| Client sends `MSDP_VAR` value | Action |
|-------------------------------|--------|
| `LIST` | Reply with the requested list (see below) |
| `REPORT` | Add the named variable's bit to `d->msdp_reported`; send current value |
| `UNREPORT` | Clear the named variable's bit from `d->msdp_reported` |
| `RESET` | Clear all bits in `d->msdp_reported` |
| `SEND` | Send the current value of the named variable immediately, no subscription |

**LIST sub-values the server supports:**

| Client sends `MSDP_VAL` | Server replies with |
|-------------------------|---------------------|
| `COMMANDS` | `LIST`, `REPORT`, `UNREPORT`, `RESET`, `SEND` |
| `LISTS` | `COMMANDS`, `LISTS`, `REPORTABLE_VARIABLES`, `SENDABLE_VARIABLES` |
| `REPORTABLE_VARIABLES` | All variable names in the supported variable set |
| `SENDABLE_VARIABLES` | Same as REPORTABLE_VARIABLES |

LIST replies use MSDP arrays:

```
IAC SB MSDP MSDP_VAR "REPORTABLE_VARIABLES" MSDP_VAL
  MSDP_ARRAY_OPEN
    MSDP_VAL "HEALTH" MSDP_VAL "HEALTH_MAX" … MSDP_VAL "WORLD_TIME"
  MSDP_ARRAY_CLOSE
IAC SE
```

### C5. Outgoing variable sender — `msdp_send_var()` and `msdp_send_room_exits()` (`src/socket.c`)

Two helpers build and send subneg blocks:

```c
/* Send a single string variable */
static void msdp_send_var(DESCRIPTOR_DATA *d, const char *name, const char *value)
{
    char buf[512];
    int n = 0;
    buf[n++] = IAC; buf[n++] = SB; buf[n++] = TELOPT_MSDP;
    buf[n++] = MSDP_VAR;
    memcpy(buf+n, name,  strlen(name));  n += strlen(name);
    buf[n++] = MSDP_VAL;
    memcpy(buf+n, value, strlen(value)); n += strlen(value);
    buf[n++] = IAC; buf[n++] = SE;
    write_to_descriptor(d, buf, n);
}

static void msdp_send_var_int(DESCRIPTOR_DATA *d, const char *name, int value)
{
    char tmp[32];
    sprintf(tmp, "%d", value);
    msdp_send_var(d, name, tmp);
}
```

`msdp_send_room_exits()` iterates `ch->in_room->exit[0..5]` (N/S/E/W/U/D) and builds a table:

```
IAC SB MSDP MSDP_VAR "ROOM_EXITS" MSDP_VAL
  MSDP_TABLE_OPEN
    MSDP_VAR "n" MSDP_VAL "1001"
    MSDP_VAR "s" MSDP_VAL "1003"
    MSDP_VAR "u" MSDP_VAL "1010"
  MSDP_TABLE_CLOSE
IAC SE
```

### C6. Periodic update — `msdp_update()` (`src/socket.c`, called from `src/update.c`)

A new function iterates `first_desc` and, for every descriptor with `msdp_active` and a playing
character, pushes subscribed variables that have changed:

```c
void msdp_update(void)
{
    DESCRIPTOR_DATA *d;
    for (d = first_desc; d != NULL; d = d->next)
    {
        CHAR_DATA *ch;
        if (!d->msdp_active || d->msdp_reported == 0)
            continue;
        if (d->connected != CON_PLAYING)
            continue;
        ch = d->original ? d->original : d->character;
        if (ch == NULL || IS_NPC(ch))
            continue;

        /* Character stats — send all subscribed ones every combat pulse */
        if (d->msdp_reported & MSDP_BIT_HEALTH)
            msdp_send_var_int(d, "HEALTH", (int)ch->hit);
        if (d->msdp_reported & MSDP_BIT_HEALTH_MAX)
            msdp_send_var_int(d, "HEALTH_MAX", (int)ch->max_hit);
        /* … repeat for MANA, MOVEMENT, LEVEL, EXPERIENCE, GOLD, etc. … */

        /* Combat — only when fighting */
        if (ch->fighting != NULL)
        {
            if (d->msdp_reported & MSDP_BIT_OPPONENT_NAME)
                msdp_send_var(d, "OPPONENT_NAME", ch->fighting->name);
            if (d->msdp_reported & MSDP_BIT_OPPONENT_HEALTH)
                msdp_send_var_int(d, "OPPONENT_HEALTH",
                    (int)((ch->fighting->hit * 100) / UMAX(1, ch->fighting->max_hit)));
            /* … etc. … */
        }
        else if (/* opponent just died — send cleared values */)
        {
            if (d->msdp_reported & MSDP_BIT_OPPONENT_NAME)
                msdp_send_var(d, "OPPONENT_NAME", "");
        }

        /* Room — only when changed */
        if (ch->in_room && ch->in_room->vnum != d->msdp_last_room)
        {
            if (d->msdp_reported & MSDP_BIT_ROOM_NAME)
                msdp_send_var(d, "ROOM_NAME", ch->in_room->name);
            if (d->msdp_reported & MSDP_BIT_ROOM_VNUM)
                msdp_send_var_int(d, "ROOM_VNUM", ch->in_room->vnum);
            if (d->msdp_reported & MSDP_BIT_ROOM_EXITS)
                msdp_send_room_exits(d, ch);
            /* … etc. … */
            d->msdp_last_room = ch->in_room->vnum;
        }

        /* World time — on SERVER_TIME change (checked by second) */
        if (d->msdp_reported & MSDP_BIT_SERVER_TIME)
            msdp_send_var_int(d, "SERVER_TIME", (int)current_time);
    }
}
```

`msdp_update()` is added to `update_handler()` in `src/update.c` at `PULSE_VIOLENCE` frequency
(every 2 pulses = 0.25 seconds). This is fast enough for responsive health/combat gauges in
Mudlet without saturating the connection.

### C7. Sector type name table (`src/socket.c`)

`ROOM_TERRAIN` is sent as a human-readable string. A small static lookup converts `sector_type`
integers to names consistent with the area file spec:

```c
static const char *sector_names[] = {
    "inside", "city", "field", "forest", "hills", "mountain",
    "swim", "noswim", "air", "desert", "underwater", "dungeon"
};
```

---

## Part D — GMCP (Generic Mud Communication Protocol)

GMCP (telnet option 201) is a bidirectional JSON-over-telnet protocol. The server sends
structured game-state data (character vitals, room info, channel messages) inside named
namespaces; clients subscribe to the namespaces they want. Feature-rich clients such as Mudlet,
MUSHclient, and BlowTorch use GMCP to drive gauges, maps, and triggers entirely from structured
data rather than parsing raw text.

### D1. Protocol constants (`src/headers/socket.h`)

```c
/* GMCP telnet option 201 */
#define TELOPT_GMCP  201

/* GMCP package subscription bits */
#define GMCP_PKG_CHAR   (1U << 0)   /* Char.* — vitals, stats, status */
#define GMCP_PKG_ROOM   (1U << 1)   /* Room.* — room info and exits */
#define GMCP_PKG_COMM   (1U << 2)   /* Comm.* — channel messages */
```

The `Core` namespace is always active once GMCP is negotiated and has no bit.

### D2. `DESCRIPTOR_DATA` additions (`src/headers/ack.h`)

```c
    bool         gmcp_active;    /* TRUE after client sends IAC DO GMCP */
    unsigned int gmcp_supports;  /* bitmask of GMCP_PKG_* the client subscribed to */
    unsigned short gmcp_last_room; /* last room vnum pushed via Room.Info */
```

### D3. Telnet negotiation

`IAC WILL GMCP` is added to the greeting sequence in `queue_login_greeting()` alongside the
existing MSSP/MSDP advertisements. In `process_telnet_options()`, `IAC DO GMCP` activates the
protocol and triggers the mandatory `Core.Hello` server→client message:

```c
else if (cmd == DO && opt == TELOPT_GMCP)
{
    d->gmcp_active = TRUE;
    gmcp_send(d, "Core.Hello",
        "{\"client\":\"" MUDNAME_PLAIN "\",\"version\":\"4.3.1\"}");
}
```

Incoming `IAC SB GMCP` payloads are routed to `process_gmcp_subneg()` from the SB branch of
`process_telnet_options()`, exactly as MSDP subneg is handled.

### D4. Incoming subneg handler — `process_gmcp_subneg()` (`src/socket.c`)

The payload of every `IAC SB GMCP … IAC SE` block is a UTF-8 string of the form
`"Package.Name"` or `"Package.Name {json}"`. The handler extracts the package name and optional
JSON body:

| Client sends | Server action |
|-------------|---------------|
| `Core.Supports.Set ["Char 1","Room 1","Comm 1"]` | Set `gmcp_supports` bitmask from array; clear and rebuild from scratch |
| `Core.Supports.Add ["Char 1"]` | OR the named package bits into `gmcp_supports` |
| `Core.Supports.Remove ["Room 1"]` | Clear the named package bits from `gmcp_supports` |
| `Core.Keepalive` | No-op (ignore; some clients send periodically) |

Unrecognised package names are silently ignored. The server never sends error responses for
unknown packages.

### D5. Outgoing sender — `gmcp_send()` (`src/socket.c`)

```c
static void gmcp_send(DESCRIPTOR_DATA *d, const char *package, const char *json)
{
    char buf[4096];
    int n = 0;
    buf[n++] = IAC; buf[n++] = SB; buf[n++] = TELOPT_GMCP;
    /* "Package.Name {json}" or just "Package.Name" */
    memcpy(buf+n, package, strlen(package)); n += strlen(package);
    if (json && *json)
    {
        buf[n++] = ' ';
        memcpy(buf+n, json, strlen(json)); n += strlen(json);
    }
    buf[n++] = IAC; buf[n++] = SE;
    write_to_descriptor(d, buf, n);
}
```

A companion `gmcp_sendf()` uses `sprintf` to build the JSON payload inline for simple cases.

### D6. Supported packages and payloads

**`Char.Vitals`** — sent after every command and at `PULSE_VIOLENCE` during combat:

```json
{"hp": 120, "maxhp": 150, "mp": 80, "maxmp": 100, "mv": 45, "maxmv": 100}
```

Sources: `ch->hit`, `ch->max_hit`, `ch->mana`, `ch->max_mana`, `ch->move`, `ch->max_move`.

**`Char.Status`** — sent once on login and when key fields change (level-up, alignment shift):

```json
{"name": "Alara", "level": 25, "class": "Mage", "race": "Elf",
 "align": 500, "gold": 1200, "tnl": 34000}
```

Sources: `ch->name`, `ch->level`, class/race name strings from tables, `ch->alignment`,
`ch->gold`, experience-to-next-level calculation.

**`Char.Stats`** — sent once on login; resent after equipment changes:

```json
{"str": 18, "int": 17, "wis": 14, "dex": 16, "con": 15,
 "hitroll": 4, "damroll": 6, "ac": -20}
```

Sources: `ch->perm_str` etc. (ability scores), `ch->hitroll`, `ch->damroll`, `ch->armor`.

**`Room.Info`** — sent when the player enters a new room (same room-change trigger as MSDP):

```json
{"num": 1001, "name": "The Town Square",
 "exits": {"n": 1002, "e": 1005, "s": 999}}
```

Sources: `ch->in_room->vnum`, `ch->in_room->name`, `ch->in_room->exit[]` direction→to_room->vnum.

**`Comm.Channel`** — sent when a public channel message is broadcast to the descriptor; allows
clients to route channels to separate panels:

```json
{"channel": "gossip", "talker": "Alara", "text": "Hello world!"}
```

This requires a hook in the channel-output path (`act_comm.c`) to call `gmcp_send_channel()`
for each descriptor that has `GMCP_PKG_COMM` set.

### D7. Update cycle

`gmcp_update()` is called from `update_handler()` at `PULSE_VIOLENCE` frequency (same as
`msdp_update()`). It sends `Char.Vitals` to every playing descriptor with `GMCP_PKG_CHAR` set.
Room pushes are driven by the same `gmcp_last_room != ch->in_room->vnum` room-change check
used for MSDP.

A post-command hook in the game loop (after `nanny()` dispatches a command) also calls
`gmcp_send_vitals(d)` so gauges update immediately after each player action, not just on the
next pulse.

### D8. JSON generation

All JSON is produced with a small set of inline helpers in `socket.c` — no external JSON library
is required. The payloads are simple flat objects with string and integer values; no nested
structures beyond the `exits` object in `Room.Info`. A minimal builder:

```c
/* Begin/end a JSON object into a fixed buffer */
static int json_begin(char *buf) { buf[0]='{'; return 1; }
static int json_end(char *buf, int n)
{
    if (buf[n-1] == ',') n--;  /* trim trailing comma */
    buf[n++] = '}';
    buf[n]   = '\0';
    return n;
}
static int json_str(char *buf, int n, const char *k, const char *v)
{
    return n + sprintf(buf+n, "\"%s\":\"%s\",", k, v);
}
static int json_int(char *buf, int n, const char *k, int v)
{
    return n + sprintf(buf+n, "\"%s\":%d,", k, v);
}
```

String values are not escaped beyond replacing `"` with `\"` — sufficient for all player names,
room names, and channel text produced by this codebase.

---

## Part E — MCCP2 (Server→Client Compression)

MCCP2 (telnet option 86 / `COMPRESS2`) allows the server to compress all output sent to a
client using zlib's DEFLATE algorithm. This substantially reduces bandwidth for verbose clients
and players in high-output situations (combat, `look` in a busy room). MCCP2 is the most
widely supported compression protocol in MUD clients.

### E1. Protocol constants (`src/headers/socket.h`)

```c
#define TELOPT_MCCP2  86   /* COMPRESS2 — server→client zlib compression */
```

### E2. `DESCRIPTOR_DATA` additions (`src/headers/ack.h`)

```c
    bool  mccp2_active;  /* TRUE after compression stream is started */
    void *mccp2_zout;    /* opaque z_stream* for deflate; NULL if inactive */
```

Using `void *` keeps `zlib.h` out of `ack.h`; the pointer is cast to `z_stream *` inside
`socket.c` only.

### E3. Telnet negotiation

`IAC WILL MCCP2` is added to the greeting sequence. When `process_telnet_options()` sees
`IAC DO MCCP2` it calls `mccp2_start(d)`:

```c
else if (cmd == DO && opt == TELOPT_MCCP2)
    mccp2_start(d);
```

### E4. Starting compression — `mccp2_start()` (`src/socket.c`)

```c
#include <zlib.h>

static void mccp2_start(DESCRIPTOR_DATA *d)
{
    z_stream *zout;
    /* Send the start-of-compression subneg UNCOMPRESSED */
    const char start[] = {IAC, SB, TELOPT_MCCP2, IAC, SE};
    write_to_descriptor(d, (char *)start, sizeof(start));

    zout = calloc(1, sizeof(z_stream));
    zout->zalloc = Z_NULL; zout->zfree = Z_NULL; zout->opaque = Z_NULL;
    if (deflateInit(zout, Z_DEFAULT_COMPRESSION) != Z_OK)
    {
        free(zout);
        log_string("mccp2_start: deflateInit failed");
        return;
    }
    d->mccp2_zout   = zout;
    d->mccp2_active = TRUE;
}
```

The `IAC SB MCCP2 IAC SE` sequence is sent **before** compression starts; all subsequent bytes
go through zlib.

### E5. Compressed write path — `write_to_descriptor()` (`src/socket.c`)

When `d->mccp2_active`, the data is deflated before the raw `write()` call:

```c
if (d->mccp2_active && d->mccp2_zout)
{
    z_stream *zout = (z_stream *)d->mccp2_zout;
    unsigned char zbuf[8192];
    zout->next_in  = (Bytef *)txt + iStart;
    zout->avail_in = (uInt)nBlock;
    do {
        zout->next_out  = zbuf;
        zout->avail_out = sizeof(zbuf);
        deflate(zout, Z_SYNC_FLUSH);
        int have = sizeof(zbuf) - zout->avail_out;
        if (have > 0 && write(d->descriptor, zbuf, have) < 0)
        {
            perror("write (mccp2)");
            return FALSE;
        }
    } while (zout->avail_in > 0);
    nWrite = nBlock;  /* all input consumed */
}
else
    nWrite = write(d->descriptor, txt + iStart, nBlock);
```

`Z_SYNC_FLUSH` ensures each write is fully flushed to the network (no buffering inside zlib).

### E6. Teardown — `close_socket()` (`src/socket.c`)

```c
if (d->mccp2_active && d->mccp2_zout)
{
    deflateEnd((z_stream *)d->mccp2_zout);
    free(d->mccp2_zout);
    d->mccp2_zout   = NULL;
    d->mccp2_active = FALSE;
}
```

### E7. Makefile — add zlib linkage (`src/Makefile`)

```makefile
L_FLAGS += -lz
```

zlib is a standard system library on all Linux distributions and requires no conditional
detection; it is always linked.

### E8. MCCP2 + TLS interaction

Combining transport-layer compression (zlib) with TLS can enable CRIME/BREACH-class attacks in
environments where an attacker can inject chosen plaintexts and observe compressed ciphertext
lengths. For a MUD the practical risk is low (no cookies or CSRF tokens), but the proposal
conservatively disables MCCP2 when TLS is active:

```c
if (cmd == DO && opt == TELOPT_MCCP2)
{
    if (d->tls_active)
    {
        /* Decline: MCCP2 + TLS is inadvisable */
        const char wont[] = {IAC, WONT, TELOPT_MCCP2};
        write_to_descriptor(d, (char *)wont, 3);
    }
    else
        mccp2_start(d);
}
```

Operators who want to override this (e.g. on a trusted internal network) can remove the guard.

### E9. Hotreboot

Like TLS, an active MCCP2 compression stream cannot be serialised across `execl()`. Before
hotreboot, MCCP2 is torn down with `deflateEnd()` for each active descriptor.

---

## Part F — MCCP3 (Client→Server Compression)

MCCP3 (telnet option 87) is the complement to MCCP2: the **client** compresses its output to
the server. This is useful primarily for players on very limited uplinks (mobile connections)
where typed commands and passwords cross the wire unencrypted without MCCP3. MCCP3 is
supported by TinTin++ and a few other clients; Mudlet does not currently support it.

### F1. Protocol constants (`src/headers/socket.h`)

```c
#define TELOPT_MCCP3  87   /* client→server zlib compression */
```

### F2. `DESCRIPTOR_DATA` additions (`src/headers/ack.h`)

```c
    bool  mccp3_active;  /* TRUE after client sends SB MCCP3 IAC SE */
    void *mccp3_zin;     /* opaque z_stream* for inflate; NULL if inactive */
```

### F3. Telnet negotiation

`IAC WILL MCCP3` is added to the greeting sequence. In `process_telnet_options()`:

```c
else if (cmd == DO && opt == TELOPT_MCCP3)
{
    /* Server acknowledges; client drives when to start compressing */
    /* Initialise the inflate stream pre-emptively */
    mccp3_init(d);
}
```

The client then sends `IAC SB MCCP3 IAC SE` when it is ready to begin compressing its output.
This is detected in the `SB` branch of `process_telnet_options()`:

```c
if (opt == TELOPT_MCCP3)
{
    /* payload is empty; client is signalling start of compression */
    d->mccp3_active = TRUE;
}
```

### F4. Inflate stream initialisation — `mccp3_init()` (`src/socket.c`)

```c
static void mccp3_init(DESCRIPTOR_DATA *d)
{
    z_stream *zin = calloc(1, sizeof(z_stream));
    zin->zalloc = Z_NULL; zin->zfree = Z_NULL; zin->opaque = Z_NULL;
    zin->next_in  = Z_NULL;
    zin->avail_in = 0;
    if (inflateInit(zin) != Z_OK)
    {
        free(zin);
        log_string("mccp3_init: inflateInit failed");
        return;
    }
    d->mccp3_zin = zin;
}
```

### F5. Decompressed read path — `read_from_descriptor()` (`src/socket.c`)

When `d->mccp3_active`, incoming bytes are decompressed after the raw `read()` into a temporary
buffer, then copied into `d->inbuf`:

```c
if (d->mccp3_active && d->mccp3_zin && nRead > 0)
{
    z_stream *zin = (z_stream *)d->mccp3_zin;
    unsigned char plain[sizeof(d->inbuf)];
    zin->next_in  = (Bytef *)(d->inbuf + iStart);
    zin->avail_in = (uInt)nRead;
    zin->next_out  = plain;
    zin->avail_out = sizeof(plain);
    int ret = inflate(zin, Z_SYNC_FLUSH);
    if (ret == Z_STREAM_ERROR || ret == Z_DATA_ERROR)
    {
        /* Decompression failure — send WONT MCCP3 and disable */
        const char wont[] = {IAC, WONT, TELOPT_MCCP3};
        write_to_descriptor(d, (char *)wont, 3);
        inflateEnd(zin); free(d->mccp3_zin);
        d->mccp3_zin = NULL; d->mccp3_active = FALSE;
        return FALSE;
    }
    int have = sizeof(plain) - zin->avail_out;
    memmove(d->inbuf + iStart, plain, have);
    nRead = have;
}
```

### F6. Teardown — `close_socket()` and hotreboot (`src/socket.c`, `src/hotreboot.c`)

```c
if (d->mccp3_active && d->mccp3_zin)
{
    inflateEnd((z_stream *)d->mccp3_zin);
    free(d->mccp3_zin);
    d->mccp3_zin   = NULL;
    d->mccp3_active = FALSE;
}
```

To signal the client to stop compressing before hotreboot, the server sends `IAC WONT MCCP3`
per the MCCP3 spec, then tears down the inflate stream.

### F7. MCCP3 + TLS interaction

Same CRIME/BREACH concern as MCCP2. MCCP3 is declined over TLS connections:

```c
if (cmd == DO && opt == TELOPT_MCCP3)
{
    if (d->tls_active)
    {
        const char wont[] = {IAC, WONT, TELOPT_MCCP3};
        write_to_descriptor(d, (char *)wont, 3);
    }
    else
        mccp3_init(d);
}
```

---

## Affected Files

| File | Change |
|------|--------|
| `src/headers/ack.h` | Add TLS (`ssl*`, `tls_active`), MSDP (`msdp_active`, `msdp_reported`, `msdp_last_room`), GMCP (`gmcp_active`, `gmcp_supports`, `gmcp_last_room`), MCCP2 (`mccp2_active`, `mccp2_zout`), MCCP3 (`mccp3_active`, `mccp3_zin`) to `DESCRIPTOR_DATA` |
| `src/headers/socket.h` | Update `game_loop`, `write_to_descriptor` prototypes; add all protocol constants (MSSP/MSDP/GMCP/MCCP2/MCCP3); declare `msdp_update()`, `gmcp_update()` |
| `src/headers/config.h` | Add `MSSP_CONTACT`, `MSSP_WEBSITE`, `MSSP_ICON_URL`, `MSSP_LOCATION` |
| `src/socket.c` | TLS: `init_tls_context()`, accept/handshake, read/write wrappers, teardown. MSSP: `send_mssp_data()`, boot counters. MSDP: `process_msdp_subneg()`, `msdp_send_var()`, `msdp_send_room_exits()`, `msdp_update()`. GMCP: `process_gmcp_subneg()`, `gmcp_send()`, `gmcp_sendf()`, `gmcp_send_vitals()`, `gmcp_send_status()`, `gmcp_send_room()`, `gmcp_update()`. MCCP2: `mccp2_start()`, compressed write path. MCCP3: `mccp3_init()`, decompressed read path. All: `process_telnet_options()`, `queue_login_greeting()`, `game_loop()` third socket, `close_socket()` teardown, JSON builder helpers |
| `src/comm.c` | CLI parsing for `--tls-port`, `--tls-cert`, `--tls-key`; `game_loop()` call updated |
| `src/act_comm.c` | `gmcp_send_channel()` hook in channel broadcast path for `Comm.Channel` |
| `src/db.c` | `init_mssp_counts()` called from `boot_db()` |
| `src/update.c` | Call `msdp_update()` and `gmcp_update()` at `PULSE_VIOLENCE` in `update_handler()` |
| `src/hotreboot.c` | Graceful teardown of TLS, MCCP2, MCCP3 before `execl()`; send `IAC WONT MCCP3` before tearing down inflate stream |
| `src/Makefile` | Conditional `HAVE_OPENSSL` via `pkg-config`; add `-lssl -lcrypto`; add `-lz` unconditionally |
| `src/tests/test_mssp.c` | Unit tests for MSSP and MSDP: `process_telnet_options()`, `send_mssp_data()`, `process_msdp_subneg()`, `msdp_send_var()` |
| `src/tests/test_gmcp.c` | Unit tests for GMCP: `process_gmcp_subneg()`, `gmcp_send()` framing, `Core.Supports.Set` bitmask, `Char.Vitals` JSON output |
| `src/tests/test_mccp.c` | Unit tests for MCCP2/3: `mccp2_start()` subneg bytes, round-trip compress/decompress of a sample string, CRIME guard (TLS + MCCP2 → WONT) |

---

## Unit Test Coverage

**`src/tests/test_mssp.c`** — MSSP and MSDP:

1. **`process_telnet_options()` IAC stripping** — buffer with `IAC DO MSSP` mixed with normal
   text; verify the IAC sequence is removed and text chars remain intact.
2. **`process_telnet_options()` unknown option** — `IAC DO 99` is stripped without crash.
3. **`process_telnet_options()` subneg routing** — buffer containing `IAC SB MSDP … IAC SE`
   verifies the payload is extracted and routed to `process_msdp_subneg()` (stubbed in test).
4. **`send_mssp_data()` framing** — verify output starts with `IAC SB TELOPT_MSSP` and ends
   with `IAC SE`.
5. **`send_mssp_data()` mandatory keys** — verify NAME, PLAYERS, and UPTIME are present.
6. **`msdp_send_var()` framing** — verify `IAC SB MSDP_VAR "x" MSDP_VAL "y" IAC SE` structure.
7. **`process_msdp_subneg()` REPORT** — feed `MSDP_VAR "REPORT" MSDP_VAL "HEALTH"`; verify
   `MSDP_BIT_HEALTH` is set in `d->msdp_reported`.
8. **`process_msdp_subneg()` UNREPORT** — verify the bit is cleared.
9. **`process_msdp_subneg()` RESET** — verify all bits cleared.
10. **`process_msdp_subneg()` LIST COMMANDS** — verify array reply contains all five command names.

**`src/tests/test_gmcp.c`** — GMCP:

11. **`gmcp_send()` framing** — verify `IAC SB GMCP "Pkg.Cmd" {json} IAC SE` structure.
12. **`process_gmcp_subneg()` Core.Supports.Set** — feed `["Char 1","Room 1"]`; verify
    `GMCP_PKG_CHAR` and `GMCP_PKG_ROOM` bits set, `GMCP_PKG_COMM` clear.
13. **`process_gmcp_subneg()` Core.Supports.Add** — verify bit OR'd in without clearing others.
14. **`process_gmcp_subneg()` Core.Supports.Remove** — verify bit cleared.
15. **`gmcp_send()` Char.Vitals JSON** — verify output contains `"hp"`, `"mp"`, `"mv"` keys
    with correct integer values from a stub `CHAR_DATA`.
16. **`process_gmcp_subneg()` unknown package** — verify no crash and no reply sent.

**`src/tests/test_mccp.c`** — MCCP2 and MCCP3:

17. **`mccp2_start()` subneg bytes** — verify the bytes `IAC SB TELOPT_MCCP2 IAC SE` are sent
    before the deflate stream begins.
18. **MCCP2 round-trip** — compress a sample string with `mccp2_start()` write path; inflate
    with raw `inflate()` in the test; verify the result matches the original string.
19. **MCCP2 + TLS guard** — set `d->tls_active = TRUE`; send `IAC DO MCCP2`; verify
    `IAC WONT MCCP2` is returned and `d->mccp2_active` remains FALSE.
20. **MCCP3 round-trip** — deflate a sample string externally; feed into `read_from_descriptor()`
    stub with `mccp3_active = TRUE`; verify the inflated result appears in `d->inbuf`.
21. **MCCP3 decompression error** — feed corrupt data; verify `IAC WONT MCCP3` is sent and
    `mccp3_active` is cleared.

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
| MSDP update frequency | Every `PULSE_VIOLENCE` (0.25 s) for all subscribed vars | Track last-sent values and only push on change — reduces bandwidth but adds per-descriptor state bloat |
| MSDP room change detection | Compare `in_room->vnum` to `msdp_last_room` | Hook into `move_char()` directly — cleaner but more invasive |
| GMCP JSON generation | Inline helpers (`json_str`, `json_int`) — no library | cJSON single-file library — cleaner API but adds a source dependency |
| GMCP update trigger | Post-command hook + `PULSE_VIOLENCE` periodic | Only periodic — less responsive; only post-command — misses combat tick changes |
| MCCP2 flush strategy | `Z_SYNC_FLUSH` per write — full flush each call | `Z_NO_FLUSH` with periodic `Z_SYNC_FLUSH` — better compression but complex buffering |
| MCCP2/3 + TLS | Disabled (CRIME/BREACH conservative guard) | Allowed with a config flag — lower security but more flexibility for operators |
| zlib linkage | `-lz` always linked | Conditional like OpenSSL — unnecessary; zlib is universally available on Linux |
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
7. **MSDP negotiation:** in Mudlet, enable MSDP; verify `IAC WILL MSDP` is received, send
   `IAC DO MSDP`, then `REPORT HEALTH` — confirm health gauge updates on combat.
8. **MSDP LIST:** send `MSDP_VAR "LIST" MSDP_VAL "REPORTABLE_VARIABLES"` — verify array reply
   contains all expected variable names.
9. **MSDP room change:** walk between rooms; verify `ROOM_NAME`, `ROOM_VNUM`, and `ROOM_EXITS`
   update only on movement, not on every pulse.
10. **GMCP negotiation:** in Mudlet, enable GMCP; confirm `Core.Hello` received on connect;
    subscribe to `Char 1` and `Room 1`; verify `Char.Vitals` updates combat HP gauge and
    `Room.Info` fires on room entry.
11. **GMCP Comm.Channel:** enable `Comm 1`; gossip on a second client; verify the first client
    receives `Comm.Channel {"channel":"gossip","talker":"...","text":"..."}`.
12. **MCCP2 negotiation:** use `telnet` with MCCP2-aware client (e.g. Mudlet); verify the data
    stream becomes compressed after `IAC SB MCCP2 IAC SE`; confirm text is still readable
    after decompression. Verify `IAC DO MCCP2` on a TLS connection returns `IAC WONT MCCP2`.
13. **MCCP3 negotiation:** use TinTin++ with MCCP3 support; verify server inflates incoming
    client commands correctly; verify a corrupt compressed byte triggers graceful disable.
14. **WebSocket:** browser client — no change to WebSocket or music behaviour.
15. **Integration test:** `make integration-test` — boot, login, 8-second crash check all pass.
