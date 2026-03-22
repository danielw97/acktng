#!/bin/sh
#
# Integration test: build, start, boot, log in as a new player, validate
# WebSocket connectivity, validate TLS connectivity (if available), and run
# for 2 seconds checking for crashes.
#
# Exit codes:
#   0 - MUD booted, accepted a player login, and ran without crashing
#   1 - build failed, MUD crashed, or the login happy-path was not reached

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
SRC_DIR="$SCRIPT_DIR/src"
AREA_DIR="$SCRIPT_DIR/area"
PLAYER_DIR="$SCRIPT_DIR/player"
RUN_SECONDS=2
LOG_FILE="/tmp/mud-integration-test-$$.log"

# Test player name (3-12 alpha chars, not a reserved name, unlikely to clash
# with any mob name).
TEST_PLAYER="Integrat"
TEST_PASSWORD="integrationpass"
TLS_TEST_PLAYER="Tlsconn"

# Ask the OS for a free ephemeral port to avoid collisions on shared CI hosts.
if command -v python3 >/dev/null 2>&1; then
    TEST_PORT=$(python3 -c \
        "import socket; s=socket.socket(); s.bind(('', 0)); print(s.getsockname()[1]); s.close()")
    TLS_PORT=$(python3 -c \
        "import socket; s=socket.socket(); s.bind(('', 0)); print(s.getsockname()[1]); s.close()")
    HTTP_PORT=$(python3 -c \
        "import socket; s=socket.socket(); s.bind(('', 0)); print(s.getsockname()[1]); s.close()")
else
    TEST_PORT=$((RANDOM % 16383 + 49152))
    TLS_PORT=$((RANDOM % 16383 + 49152))
    HTTP_PORT=$((RANDOM % 16383 + 49152))
fi

# Try to generate a self-signed certificate for TLS testing.
# If openssl is unavailable or cert generation fails, TLS test is skipped.
TLS_CERT="/tmp/mud-tls-cert-$$.pem"
TLS_KEY="/tmp/mud-tls-key-$$.pem"
TLS_ARGS=""
HAS_TLS=0

if command -v openssl >/dev/null 2>&1; then
    # Generate a traditional PKCS#1 RSA private key first, then the self-signed cert.
    # openssl genrsa -traditional forces "BEGIN RSA PRIVATE KEY" format on OpenSSL 3.0+;
    # older versions produce that format by default.  A PKCS#1 key is compatible with
    # all OpenSSL versions that the MUD server might have been compiled against.
    if (openssl genrsa -traditional -out "$TLS_KEY" 2048 2>/dev/null || \
        openssl genrsa -out "$TLS_KEY" 2048 2>/dev/null) && \
       openssl req -x509 -new -key "$TLS_KEY" -out "$TLS_CERT" \
           -days 1 -subj '/CN=localhost' >/dev/null 2>&1; then
        TLS_ARGS="--tls-port $TLS_PORT --tls-cert $TLS_CERT --tls-key $TLS_KEY"
    fi
fi

# ---------------------------------------------------------------------------
# Cleanup helper – always runs on exit to stop a stray server process.
# ---------------------------------------------------------------------------
cleanup() {
    if [ -n "$MUD_PID" ] && kill -0 "$MUD_PID" 2>/dev/null; then
        kill "$MUD_PID" 2>/dev/null || true
        wait "$MUD_PID" 2>/dev/null || true
    fi
    rm -f "$LOG_FILE" "$TLS_CERT" "$TLS_KEY"
}
trap cleanup EXIT

# ---------------------------------------------------------------------------
# Step 1: build
# ---------------------------------------------------------------------------
echo "integration-test: building MUD..."
if ! (cd "$SRC_DIR" && make ack); then
    echo "integration-test: FAILED - build step failed"
    exit 1
fi

# ---------------------------------------------------------------------------
# Step 2: remove any leftover player files so the login flows are always the
# new-character path (idempotent test runs).
# ---------------------------------------------------------------------------
player_lower=$(echo "$TEST_PLAYER" | tr '[:upper:]' '[:lower:]')
first_letter=$(echo "$player_lower" | cut -c1)
rm -f "$PLAYER_DIR/$first_letter/$player_lower"

tls_player_lower=$(echo "$TLS_TEST_PLAYER" | tr '[:upper:]' '[:lower:]')
tls_first_letter=$(echo "$tls_player_lower" | cut -c1)
rm -f "$PLAYER_DIR/$tls_first_letter/$tls_player_lower"

# ---------------------------------------------------------------------------
# Step 3: launch
# The startup script runs the binary from the area/ directory so that
# relative paths to area files, data files, and player directories resolve
# correctly.  TLS_ARGS is empty if cert generation failed or openssl is
# absent; the server will simply not open a TLS port in that case.
# ---------------------------------------------------------------------------
echo "integration-test: starting MUD on port $TEST_PORT..."
# shellcheck disable=SC2086
(cd "$AREA_DIR" && ../src/ack "$TEST_PORT" $TLS_ARGS --http-port "$HTTP_PORT") >"$LOG_FILE" 2>&1 &
MUD_PID=$!

echo "integration-test: MUD started (PID $MUD_PID), waiting for boot..."

# ---------------------------------------------------------------------------
# Step 4: wait until the server is ready (game loop started, max 90 s).
# The server logs "ACK! MUD is ready on port N." just before entering the
# game loop.  Waiting for TCP connectivity is not sufficient because the
# port is opened before area files are loaded, so WebSocket handshakes
# sent too early are never processed.
# ---------------------------------------------------------------------------
boot_wait=0
while [ "$boot_wait" -lt 90 ]; do
    if grep -q "MUD is ready on port" "$LOG_FILE" 2>/dev/null; then
        break
    fi
    if ! kill -0 "$MUD_PID" 2>/dev/null; then
        wait "$MUD_PID"
        echo "integration-test: FAILED - MUD crashed during boot"
        echo "--- MUD output ---"
        cat "$LOG_FILE"
        echo "------------------"
        exit 1
    fi
    sleep 1
    boot_wait=$((boot_wait + 1))
done

if [ "$boot_wait" -ge 90 ]; then
    echo "integration-test: FAILED - MUD did not reach ready state after 90s"
    echo "--- MUD output ---"
    cat "$LOG_FILE"
    echo "------------------"
    exit 1
fi

# Determine whether the server actually opened the TLS port.  It will if and
# only if it was compiled with OpenSSL AND cert/key loading succeeded.
if [ -n "$TLS_ARGS" ] && grep -q "(TLS)" "$LOG_FILE" 2>/dev/null; then
    HAS_TLS=1
    echo "integration-test: TLS port $TLS_PORT is active."
else
    echo "integration-test: TLS not available in this build; skipping TLS test."
fi

echo "integration-test: MUD is up, validating websocket login flow for '${TEST_PLAYER}'..."

# ---------------------------------------------------------------------------
# Step 5: WebSocket handshake + walk the new-player login flow (happy path).
#   greeting0 ends with "What is your name?"
#   CON_CONFIRM_NEW_NAME  -> "Did I get that right, <name> (Y/N)?"
#   CON_GET_NEW_PASSWORD  -> "Give me a password for <name>:"
#   CON_CONFIRM_NEW_PASSWORD -> "Please retype password:"
#   CON_MENU (show_menu_to) -> shows numbered options including "elect"
#     1 -> sex menu     -> send M
#     2 -> race menu    -> send Hmn  (Human, 3-letter race_name)
#     3 -> class menu   -> send "War Mag Cle Cip"  (MAX_PC_CLASS = 4)
#     4 -> CON_READ_MOTD
#   CON_READ_MOTD         -> any input -> CON_PLAYING
#   CON_PLAYING           -> server sends "Welcome to ACK!TNG" + room look
# ---------------------------------------------------------------------------
python3 "$SCRIPT_DIR/websocket-test-client.py" "$TEST_PORT" "$TEST_PLAYER" "$TEST_PASSWORD"

LOGIN_STATUS=$?
if [ "$LOGIN_STATUS" -ne 0 ]; then
    echo "integration-test: FAILED - websocket login sequence did not complete"
    echo "--- MUD output ---"
    cat "$LOG_FILE"
    echo "------------------"
    exit 1
fi

# ---------------------------------------------------------------------------
# Step 6: TLS login flow (skipped if OpenSSL not compiled in).
#
# Connects via raw TLS telnet (no WebSocket) and walks the same new-player
# creation flow, stripping telnet IAC negotiation bytes along the way.
# ---------------------------------------------------------------------------
if [ "$HAS_TLS" -eq 1 ]; then
    echo "integration-test: validating TLS login flow for '${TLS_TEST_PLAYER}'..."
    python3 "$SCRIPT_DIR/tls-test-client.py" "$TLS_PORT" "$TLS_TEST_PLAYER" "$TEST_PASSWORD"

    TLS_STATUS=$?
    if [ "$TLS_STATUS" -ne 0 ]; then
        echo "integration-test: FAILED - TLS login sequence did not complete"
        echo "--- MUD output ---"
        cat "$LOG_FILE"
        echo "------------------"
        exit 1
    fi
fi

# ---------------------------------------------------------------------------
# Step 7: let the MUD keep running and watch for crashes.
# ---------------------------------------------------------------------------
echo "integration-test: monitoring MUD for ${RUN_SECONDS}s..."
elapsed=0
crashed=0
while [ "$elapsed" -lt "$RUN_SECONDS" ]; do
    sleep 1
    elapsed=$((elapsed + 1))
    if ! kill -0 "$MUD_PID" 2>/dev/null; then
        crashed=1
        break
    fi
done

if [ "$crashed" -eq 1 ]; then
    wait "$MUD_PID"
    exit_code=$?
    echo "integration-test: FAILED - MUD exited after ${elapsed}s (exit code ${exit_code})"
    echo "--- MUD output ---"
    cat "$LOG_FILE"
    echo "------------------"
    exit 1
fi

echo "integration-test: MUD ran for ${RUN_SECONDS}s without crashing, stopping..."
# cleanup trap will kill the server and remove the log file
exit 0
