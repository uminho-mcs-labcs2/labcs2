#!/usr/bin/env bash
#
# Offline flag checker. Compares the SHA-256 of your submitted flag against the hash
# recorded by setup.sh. No network, no central server - it just tells you if you're right.
#
# Usage:  ./check.sh <challenge-id> 'labcs2{...}'
#   e.g.  ./check.sh w1-cmdi 'labcs2{deadbeefdeadbeef}'
#
# Challenge ids: w1-cmdi  w2-objinj  p1-uaf  d1-forensics
set -euo pipefail
cd "$(dirname "$0")"

if [ $# -ne 2 ]; then
    echo "usage: $0 <challenge-id> '<flag>'" >&2
    exit 2
fi

if [ ! -f .flaghashes ]; then
    echo "No .flaghashes found - run ./setup.sh first." >&2
    exit 2
fi

id="$1"
flag="$2"

want="$(awk -v c="$id" '$1==c{print $2}' .flaghashes)"
if [ -z "$want" ]; then
    echo "Unknown challenge id: $id" >&2
    echo "Valid ids: $(awk '{printf "%s ", $1}' .flaghashes)" >&2
    exit 2
fi

got="$(printf '%s' "$flag" | sha256sum | cut -d' ' -f1)"

if [ "$got" = "$want" ]; then
    echo "CORRECT - $id solved. Nice."
else
    echo "Not quite. Check the flag format (labcs2{...}) and try again."
    exit 1
fi
