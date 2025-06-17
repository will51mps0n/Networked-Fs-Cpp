#!/bin/bash

set +e  

FS_EXEC=./fs
CREATEFS=./createfs_macos
DISK_FILE="/tmp/fs_tmp.$(whoami).disk"
LOG_DIR="logs"

mkdir -p "$LOG_DIR"

echo "Rebuilding server and tests..."
make clean > /dev/null
make > /dev/null

TESTS=()
for f in test*; do
    if [[ -x "$f" && ! -d "$f" && "$f" != *.cpp ]]; then
        FILE_TYPE=$(file "$f")
        if echo "$FILE_TYPE" | grep -q "executable"; then
            TESTS+=("$f")
        fi
    fi
done

echo ""
echo "Detected test executables:"
for T in "${TESTS[@]}"; do
    echo "$T"
done

BASE_PORT=8000
i=0

for TEST in "${TESTS[@]}"; do
    TEST_NAME=$(basename "$TEST")
    TEST_BASE="${TEST_NAME%.*}"
    PORT=$((BASE_PORT + i))
    i=$((i + 1))

    echo ""
    echo "Running $TEST_NAME on port $PORT..."

    if [[ -f "${TEST_BASE}.fs" ]]; then
        echo "Loading filesystem image ${TEST_BASE}.fs"
        $CREATEFS "${TEST_BASE}.fs"
    else
        echo "Creating empty filesystem"
        $CREATEFS
    fi

    # Kill any stale process using this port
    PID_TO_KILL=$(lsof -ti tcp:"$PORT")
    if [[ -n "$PID_TO_KILL" ]]; then
        echo "Port $PORT in use (PIDs: $PID_TO_KILL), killing..."
        for pid in $PID_TO_KILL; do
            kill -9 "$pid"
        done
        sleep 0.2
    fi

    LOGFILE="${LOG_DIR}/${TEST_BASE}.log"
    $FS_EXEC $PORT > "$LOGFILE" 2>&1 &
    FS_PID=$!

    sleep 0.5

    echo "Launching client: $TEST_NAME"
    "./$TEST_NAME" localhost "$PORT" > /dev/null 2>&1
    STATUS=$?

    if [[ $STATUS -eq 0 ]]; then
        echo "$TEST_NAME completed successfully"
    else
        echo "$TEST_NAME failed with status $STATUS"
    fi

    kill "$FS_PID" > /dev/null 2>&1
    wait "$FS_PID" 2>/dev/null || true
    sleep 0.2
done

echo ""
echo "All tests complete. File server logs are in the logs/ directory."
