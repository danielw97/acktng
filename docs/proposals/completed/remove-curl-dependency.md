# Remove curl Dependency

## Problem

The project links against `-lcurl` and includes `<curl/curl.h>`, but the only
usage is a single function (`call_tngai()` in `npc_dialogue.c`) that makes a
blocking HTTP POST to a local-network API endpoint over plain HTTP. This
requires `libcurl-dev` to be installed on every build machine and adds a heavy
transitive dependency for what amounts to ~20 lines of socket work.

## Approach

Replace the curl usage in `npc_dialogue.c` with a minimal HTTP/1.1 client using
POSIX sockets (`<sys/socket.h>`, `<netdb.h>`, `<unistd.h>`). The replacement
will:

1. **Parse the URL** at init time (or on first use) to extract host, port, and
   path from `TNGAI_URL`. Since this is a compile-time constant
   (`http://192.168.1.111:8000/v1/chat`), parsing is straightforward.

2. **Implement `call_tngai()` with raw sockets**: open a TCP connection, send an
   HTTP/1.1 POST with `Content-Type: application/json` and `Content-Length`,
   read the response (handling chunked transfer encoding is not needed since the
   target is a known local service, but we will handle `Content-Length` for
   correctness), and extract the body.

3. **Implement connect timeout and read timeout** using `setsockopt()`
   `SO_RCVTIMEO`/`SO_SNDTIMEO` to match the existing `TNGAI_TIMEOUT` behavior.

4. **Remove from Makefile**: drop `-lcurl` from `L_FLAGS`.

5. **Remove `#include <curl/curl.h>`** and the `curl_global_init()` call from
   `npc_dialogue_init()`.

## Affected Files

| File | Change |
|------|--------|
| `src/npc_dialogue.c` | Replace curl includes, `CURL_BUF`, `curl_write_cb()`, `call_tngai()`, and `curl_global_init()` call with POSIX socket implementation |
| `src/Makefile` | Remove `-lcurl` from `L_FLAGS` |

## Scope of Change

- The public API (`npc_dialogue_init`, `npc_dialogue_dispatch`,
  `npc_dialogue_deliver`) is unchanged.
- All other code in `npc_dialogue.c` (JSON building, response parsing, prompt
  assembly, sanitization, accent processing, queues, worker thread) is
  unchanged.
- Only the HTTP transport layer is replaced.

## Trade-offs

| Pro | Con |
|-----|-----|
| Eliminates external dependency | No HTTPS support (not needed — target is `http://` on local network) |
| Simpler build requirements | No automatic redirect following (not needed) |
| Smaller binary | Must handle basic HTTP response parsing ourselves |
| No library version compatibility concerns | Less robust than curl for edge cases (proxy, auth, etc.) |

The target is a fixed local-network HTTP endpoint, so the limitations are
acceptable. If HTTPS support is ever needed in the future, it can be added via
a lightweight TLS library or by re-introducing curl at that time.

## Testing

- `make unit-tests` (existing tests + integration test) to verify no build
  regressions.
- The NPC dialogue system itself requires a running tng-ai server to test
  end-to-end, which is outside the scope of automated tests. The socket
  implementation will be verified by code review and the successful build.
