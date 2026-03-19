# Design Proposal: Fix Web Client Extra Line Breaks

## Problem

The web client (WebSocket-based) displays extra blank lines throughout gameplay — in room descriptions, character creation screens, the minimap, and anywhere else the server sends multi-line output. For example, the room display shows each line double-spaced when viewed through the browser.

**Root cause:** The MUD server uses `\n\r` (LF + CR) as its line terminator throughout the codebase, following the traditional telnet convention where `\n` advances the cursor down one line and `\r` returns it to column zero. Telnet clients interpret these as a single compound operation. Web browsers, however, treat both `\n` (line feed) and `\r` (carriage return) as independent newline characters per the HTML specification's line ending normalization rules. As a result, every `\n\r` pair renders as **two** line breaks in the browser's `<pre>` output element.

## Approach

Strip all `\r` (carriage return, `0x0D`) characters from the output before it is sent to WebSocket clients.

This is the minimal, correct fix: `\r` serves no purpose in a browser context because `\n` alone is sufficient to terminate a line. Removing `\r` turns every `\n\r` sequence into a plain `\n`, which the browser renders as exactly one line break.

The fix belongs in `sanitize_websocket_text_payload()` in `src/socket.c`, which is already the designated place where WebSocket output is cleaned up before being framed and sent.

## Affected Files

- **`src/socket.c`** — add `\r`-stripping logic to `sanitize_websocket_text_payload()` (lines 387–424)

No other files need to change. The server's internal `\n\r` convention is left intact for telnet clients.

## Implementation Detail

In `sanitize_websocket_text_payload()`, add a check in the copy loop:

```c
if (src[i] == '\r') {
    i++;
    continue;   /* strip carriage return */
}
```

This sits alongside the existing IAC/GA/negotiation stripping. All `\r` bytes are discarded; all other bytes (including `\n`) pass through unchanged.

## Trade-offs

| Consideration | Notes |
|---|---|
| Telnet clients unaffected | The function is only called for WebSocket output; telnet path is unchanged |
| Internal strings unchanged | No need to audit or modify any of the hundreds of `\n\r` string literals in the codebase |
| Binary-safe | The change only filters a single byte value; no risk to ANSI escape sequences or other control codes |
| Unit-testable | Existing `test_websocket_validation.c` infrastructure can be extended with a simple test |

## Unit Test

A test case will be added to `src/tests/test_websocket_sanitize.c` (new file) covering:
- `\n\r` → `\n` (primary case)
- `\r\n` → `\n` (also handled correctly — `\r` stripped, `\n` kept)
- Bare `\r` → stripped
- Bare `\n` → kept unchanged
- IAC sequences still stripped (regression)
- Plain text passthrough unchanged
