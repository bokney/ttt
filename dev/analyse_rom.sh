#!/bin/bash

ROM="$1"

if [ -z "$ROM" ]; then
  echo "Usage: $0 <romfile.gb>"
  exit 1
fi

TOTAL=$(stat -f%z "$ROM")

USED=$(hexdump -v -e '1/1 "%02X\n"' "$ROM" | grep -vc '^FF$')

PERCENT=$(echo "$USED $TOTAL" | awk '{printf "%.2f", ($1 / $2) * 100}')

echo "ROM: $ROM"
echo "Total size : $TOTAL bytes"
echo "Used bytes : $USED bytes"
echo "Unused (FF): $((TOTAL - USED)) bytes"
echo "Usage      : $PERCENT%"
