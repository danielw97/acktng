# Design Proposal: Fix Hotreboot WebSocket Disconnect

## Problem

When a hotreboot (copyover) is performed, web client connections are disconnected. Telnet clients survive the reboot seamlessly, but WebSocket (browser) clients lose their connection.

## Root Cause

The hotreboot mechanism works by:
1. Writing active descriptor state to `COPYOVER_FILE` (descriptor fd, character name, host)
2. Calling `execl()` to replace the process — all open file descriptors are inherited
3. On startup, `copyover_recover()` reads the file and reconnects each descriptor to its character

The bug is in steps 1 and 3:

- **Step 1** (`do_hotreboot()`): The format written to `COPYOVER_FILE` is:
  ```
  <fd> <name> <host>\n
  ```
  This does **not** record whether the connection was a WebSocket connection.

- **Step 3** (`copyover_recover()`): After restoring the descriptor, the code sets:
  ```c
  d->websocket_handshake_complete = TRUE;
  d->greeting_sent = TRUE;
  ```
  But it does **not** set `d->websocket_active = TRUE`.

`websocket_active` is the flag that governs whether output is framed as WebSocket frames or sent as raw text. Without it, `process_output()` and `write_to_buffer()` treat the recovered connection as a plain telnet connection and send raw bytes. The browser WebSocket client receives invalid framing and closes the connection.

## Approach

### 1. Extend the COPYOVER_FILE format

Add a `ws` integer field (0 or 1) to each line:

```
<fd> <name> <host> <ws>\n
```

Where `ws` is `1` if `d->websocket_active` was true, `0` otherwise.

The `-1` sentinel line remains unchanged (it terminates the scan loop before reading `ws`).

### 2. Save WebSocket flag in `do_hotreboot()`

```c
fprintf(fp, "%d %s %s %d\n", d->descriptor, och->name, d->host,
        d->websocket_active ? 1 : 0);
```

### 3. Restore WebSocket flag in `copyover_recover()`

Change `fscanf` to read the new field:

```c
int is_websocket = 0;
if (fscanf(fp, "%d %99s %8191s %d\n", &desc, name, host, &is_websocket) < 3)
    break;
```

(Reading 3 fields is still sufficient for a valid record; `is_websocket` defaults to 0 for any old-format lines that only have 3 fields.)

Then after `init_descriptor()`:

```c
d->websocket_handshake_complete = TRUE;
d->greeting_sent = TRUE;
if (is_websocket)
    d->websocket_active = TRUE;
```

### 4. Recovery messages must use WebSocket framing

The current recovery code calls `write_to_descriptor()` with raw text before `websocket_active` is set. After the fix, `websocket_active` is set right after `init_descriptor()`, so all subsequent `write_to_descriptor()` calls for WebSocket clients will automatically use proper framing.

The initial `"\n\rRestoring from HOTreboot...\n\r"` message is sent *before* `init_descriptor()` completes and before `websocket_active` is set. However, since the `wsocket_active` flag isn't set until after we read it from the file, we need to set the flag before calling any output functions. The recovery message should be sent after setting `websocket_active`.

## Affected Files

- `src/hotreboot.c` — only file changed

## Trade-offs

- **Format change**: The `COPYOVER_FILE` format gains a 4th field. If an old binary writes 3-field lines and a new binary reads them, `fscanf` with `< 3` guard still accepts the record (treating `is_websocket` as 0 = telnet). This is a safe, forward-compatible change.
- **Minimal scope**: No changes to socket.c, no new functions, no struct changes.

## Testing

- `make unit-tests` (builds + runs all unit tests + integration test)
- Manual test: connect web client, run `hotreboot`, verify client remains connected and output continues normally
- Manual test: connect telnet client, run `hotreboot`, verify telnet client still works

## Status

Approved — implementing.
