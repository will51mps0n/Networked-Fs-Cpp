#!/bin/bash

CORRECT_DIR="./correct"
LOG_DIR="./logs"
DIFF_OUT="diff.txt"

echo "Comparing logs to correct outputs..."
rm -f "$DIFF_OUT"

for correct_file in "$CORRECT_DIR"/*.correct; do
    base=$(basename "$correct_file" .correct)
    log_file="$LOG_DIR/$base.log"

    if [[ -f "$log_file" ]]; then
        diff_output=$(diff -a "$correct_file" "$log_file")
        status=$?

        if [[ $status -eq 0 ]]; then
            echo "$base: OK" >> "$DIFF_OUT"
        else
            echo "$base: MISMATCH" >> "$DIFF_OUT"
            echo "==== Diff for $base ====" >> "$DIFF_OUT"
            diff -a "$correct_file" "$log_file" >> "$DIFF_OUT" 2>&1
            echo "" >> "$DIFF_OUT"
        fi
    else
        echo "$base: NO LOG FILE FOUND" >> "$DIFF_OUT"
    fi
done

echo "Diff results written to $DIFF_OUT"
