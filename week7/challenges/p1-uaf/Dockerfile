FROM ubuntu:22.04

# socat exposes the challenge binary over TCP, one fresh process per connection.
RUN apt-get update \
 && apt-get install -y --no-install-recommends gcc libc6-dev socat ca-certificates \
 && rm -rf /var/lib/apt/lists/*

WORKDIR /opt/chall
COPY chall.c .

# Deterministic addresses (no PIE), no stack protector - this is a heap/function-pointer
# challenge, not a stack-smash. The flag is mounted read-only at /flag by docker-compose.
# Keep this at -O0 (no -O flag): the UAF write in `set` stores into a malloc'd buffer
# that is never read back, so an optimizer could dead-eliminate it and break the exploit.
RUN gcc -no-pie -fno-stack-protector -w -o chall chall.c

EXPOSE 9100
# -T60: drop a connection after 60s idle so stale sessions don't pile up.
CMD ["socat", "-T60", "TCP-LISTEN:9100,reuseaddr,fork", "EXEC:/opt/chall/chall,stderr"]
