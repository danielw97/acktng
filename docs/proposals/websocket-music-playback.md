# Design Proposal: WebSocket Music Playback

**Date:** 2026-03-20
**Status:** Pending approval

---

## 1. Problem

The web client connects to the MUD server via WebSocket. When a player connects through the web client, there is currently no mechanism to play background music. The feature request is:

- On WebSocket connection, the server sends a control message telling the web client to play `/web/mp3/theme.mp3`
- This must **only** happen for WebSocket connections, never for plain telnet
- The web client should expose player-accessible controls: **play**, **stop**, and **volume adjustment**

---

## 2. Approach

### 2.1 Message Protocol

A lightweight JSON envelope is used for out-of-band control messages sent from the server to the client. Regular game text is plain text; control messages start with `{` and follow this shape:

```json
{"type":"music","action":"play","url":"/web/mp3/theme.mp3"}
```

The JavaScript client inspects each incoming WebSocket message: if it is a valid JSON object with `type === "music"`, it is treated as a music command and never displayed in the game output pane. All other messages are rendered as game text as today.

This approach is safe because ACK!MUD game output never starts with a bare `{` in normal operation (it uses ANSI escape sequences and the `@@color` system).

### 2.2 Server-Side Change (`src/socket.c`)

After `handle_websocket_handshake()` completes and sets `d->websocket_active = TRUE`, a new static helper `send_music_play(DESCRIPTOR_DATA *d, const char *url)` is called. It directly calls the existing `write_websocket_frame()` with a JSON text frame (opcode `0x1`). This bypasses the `outbuf` pipeline so the music command is delivered immediately, before the login greeting text.

The function is approximately 10 lines of C. No new public API is required; it is internal to `socket.c`.

### 2.3 Web Server Change (`web/web_who_server.py`)

A new route `/web/mp3/<filename>` is added to `WhoRequestHandler.do_GET()`. It resolves the filename safely against `WEB_DIR / "mp3"` (the existing `web/mp3/` directory where `theme.mp3` already lives) and serves it with the appropriate MIME type (`audio/mpeg`). This mirrors the existing `/img/<filename>` pattern.

### 2.4 Web Client Change (`web/templates/mud_client.html`)

Three additions are made to the client template:

1. **Hidden `<audio>` element** — `<audio id="mud-audio" loop>` with a `<source>` whose `src` is set dynamically by JS.
2. **Music control bar** — a new `<div>` with three controls:
   - **Play button** — calls `audio.play()`
   - **Stop button** — calls `audio.pause()` and resets `audio.currentTime = 0`
   - **Volume slider** — `<input type="range" min="0" max="1" step="0.05">` bound to `audio.volume`
   The bar is initially hidden and shown once the server sends a music command (so it only appears for WebSocket sessions, consistent with the telnet-exclusion requirement).
3. **Message handler update** — the existing `socket.addEventListener('message', ...)` is extended with a JSON parse attempt. If parsed successfully and `data.type === 'music'` and `data.action === 'play'`, the audio `src` is set, `audio.play()` is called, and the control bar is revealed.

---

## 3. Affected Files

| File | Change |
|---|---|
| `src/socket.c` | Add `send_music_play()` helper; call it at end of `handle_websocket_handshake()` |
| `web/templates/mud_client.html` | Add `<audio>` element, music control bar, update message handler |
| `web/web_who_server.py` | Add `/web/mp3/<filename>` route |

No changes to `typedefs.h`, `ack.h`, Makefile, or any area/help files are required. No new unit test file is strictly needed for the C side (the change is a one-liner call to an already-tested frame writer), but the integration test will exercise the WebSocket handshake path.

---

## 4. Trade-offs and Constraints

**Why JSON and not a custom telnet suboption or a MUD Sound Protocol (MSP) sequence?**
MSP uses `!!SOUND(...)` syntax embedded in text, which would require stripping it before display. JSON is self-describing and easy to extend for future control messages (e.g., stop, zone-specific music). The prefix `{` is not ambiguous with any ACK!MUD output.

**Why send before the greeting?**
Browsers may buffer audio autoplay until user gesture. Sending the command first gives the maximum window for the browser's autoplay policy to be satisfied by the connection click.

**Autoplay restrictions:** Modern browsers block autoplay until the user has interacted with the page. The user already clicked "Connect", which counts as an interaction, so autoplay should succeed on all major browsers.

**Volume slider initial value:** Defaulted to 0.5 (50%) to avoid startling users.

**Loop:** The `<audio>` element has `loop` set so the theme plays continuously. The stop button provides the escape hatch.

**Telnet safety:** The `send_music_play()` helper is only called inside the `handle_websocket_handshake()` path, which is unreachable for plain telnet connections (telnet data never starts with `GET `). No guard macro is needed.
