#!/usr/bin/env bash
#
# One-time setup for the week 7 CTF.
#
# Generates a UNIQUE, RANDOM flag per challenge (so flags are never committed and can't
# be shared between students), plants each flag where its challenge expects it, and
# records only the SHA-256 of each flag locally so check.sh can verify your answers
# offline. Re-run it any time to roll fresh flags.
#
# Usage:  ./setup.sh
set -euo pipefail
cd "$(dirname "$0")"

CHALLENGES=(w1-cmdi w2-objinj p1-uaf d1-forensics)

# Random 16-hex-char flag. Uses od (coreutils) for portability; falls back to openssl.
gen_flag() {
    local hex
    if hex="$(od -An -tx1 -N8 /dev/urandom 2>/dev/null)"; then
        hex="$(printf '%s' "$hex" | tr -d ' \n')"
    else
        hex="$(openssl rand -hex 8)"
    fi
    printf 'labcs2{%s}' "$hex"
}
sha256()   { printf '%s' "$1" | sha256sum | cut -d' ' -f1; }

: > .flaghashes
d1_flag=""   # only d1-forensics' flag is reused later (baked into its access log)

for ch in "${CHALLENGES[@]}"; do
    f="$(gen_flag)"
    if [ "$ch" = d1-forensics ]; then d1_flag="$f"; fi
    printf '%s' "$f" > "challenges/$ch/flag.txt"      # no trailing newline
    printf '%s %s\n' "$ch" "$(sha256 "$f")" >> .flaghashes
done

# d1-forensics is static log analysis: bake its flag (base64-encoded) into the log so
# students recover it by decoding, rather than by reading a flag file.
b64="$(printf '%s' "$d1_flag" | base64 | tr -d '\n')"
sed "s|__EXFIL__|${b64}|" \
    challenges/d1-forensics/access.log.tmpl > challenges/d1-forensics/access.log

echo "Setup complete."
echo "  - Per-challenge flags generated (gitignored): challenges/*/flag.txt"
echo "  - Verification hashes written to:             .flaghashes"
echo
echo "Next:  docker compose up --build        (then see README.md for each challenge)"
