#!/bin/bash

CORRECT_DIR="./correct"
LOG_DIR="./logs"

echo "Updating .correct files from logs"

for correct_file in "$CORRECT_DIR"/*.correct; do
    base=$(basename "$correct_file" .correct)
    log_file="$LOG_DIR/$base.log"

    if [[ -f "$log_file" ]]; then
        echo "Updating $correct_file with output from $log_file"
        cp "$log_file" "$correct_file"
    else
        echo "Skipping $base: no log file found"
    fi
done

echo "All matching"
