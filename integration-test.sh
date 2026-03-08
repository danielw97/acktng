#!/bin/sh
#
# Integration test: build, start, and run the MUD for 8 seconds, checking for crashes.
#
# Exit codes:
#   0 - MUD ran for the full duration without crashing
#   1 - build failed or MUD crashed before the run duration elapsed

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
SRC_DIR="$SCRIPT_DIR/src"
AREA_DIR="$SCRIPT_DIR/area"
RUN_SECONDS=8

# Ask the OS for a free ephemeral port to avoid collisions on shared CI hosts.
if command -v python3 >/dev/null 2>&1; then
    TEST_PORT=$(python3 -c \
        "import socket; s=socket.socket(); s.bind(('', 0)); print(s.getsockname()[1]); s.close()")
else
    TEST_PORT=$((RANDOM % 16383 + 49152))
fi
LOG_FILE="/tmp/mud-integration-test-$$.log"

# ---------------------------------------------------------------------------
# Step 1: build
# ---------------------------------------------------------------------------
echo "integration-test: building MUD..."
if ! (cd "$SRC_DIR" && make ack); then
    echo "integration-test: FAILED - build step failed"
    exit 1
fi

# ---------------------------------------------------------------------------
# Step 2: launch
# The startup script runs the binary from the area/ directory so that
# relative paths to area files, data files, and player directories resolve
# correctly.
# ---------------------------------------------------------------------------
echo "integration-test: starting MUD on port $TEST_PORT..."
(cd "$AREA_DIR" && ../src/ack "$TEST_PORT") >"$LOG_FILE" 2>&1 &
MUD_PID=$!

echo "integration-test: MUD started (PID $MUD_PID), running for ${RUN_SECONDS}s..."

# ---------------------------------------------------------------------------
# Step 3: monitor for crashes during the run window
# ---------------------------------------------------------------------------
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

# ---------------------------------------------------------------------------
# Step 4: evaluate outcome
# ---------------------------------------------------------------------------
if [ "$crashed" -eq 1 ]; then
    wait "$MUD_PID"
    exit_code=$?
    echo "integration-test: FAILED - MUD exited after ${elapsed}s (exit code ${exit_code})"
    echo "--- MUD output ---"
    cat "$LOG_FILE"
    echo "------------------"
    rm -f "$LOG_FILE"
    exit 1
fi

# MUD is still alive – shut it down cleanly.
echo "integration-test: MUD ran for ${RUN_SECONDS}s without crashing, stopping..."
kill "$MUD_PID"
wait "$MUD_PID" 2>/dev/null || true

rm -f "$LOG_FILE"
echo "integration-test: PASSED"
exit 0
