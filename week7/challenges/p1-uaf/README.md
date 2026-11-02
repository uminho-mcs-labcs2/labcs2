# P1 - "Dangling" | pwn | 300 pts | hard | **stretch / bonus**

> Same bug *class* as CVE-2026-109619 (the PHP2Shell use-after-free), shrunk to a
> target you can understand end to end.

> [!IMPORTANT]
> **This is the stretch challenge.** You can earn full marks on week 7 from the three
> core challenges (W1, W2, D1) without it. P1 is binary exploitation - a real step up -
> so it is worth the most *because* it is optional reach. Work it in the **stages** below;
> each stage is a small, checkable win. Even a write-up that gets part-way and explains
> *exactly* where it stalled is worth partial credit.

A tiny session manager is listening on TCP port **9100**. Make it hand you the flag.

## Connect

```bash
nc localhost 9100
```

Commands (one per line): `new`, `del`, `set <hex>`, `go`, `exit`.

## Start here: the week 6 bridge (do this first)

You already met this exact bug class in week 6. Before touching the network target, go
back and **run the minimal demo** - it is the same mechanism with none of the noise:

```bash
cd ../../../week6/lab/mem-classes
gcc -fsanitize=address -g uaf.c -o uaf_asan && ./uaf_asan   # the DIAGNOSIS: ASan names the UAF
gcc                    -g uaf.c -o uaf      && ./uaf        # the CONSEQUENCE: the hijacked call
```

`uaf.c` frees a struct that holds a **function pointer**, then lets that pointer be
overwritten and called - a *control-flow hijack*. P1 is that same story, but **you**
supply the bytes and you aim the pointer at `win()`. If the demo makes sense, you already
understand P1; the rest is mechanics.

## What you get

- `chall.c` - the full source (read it carefully; the bug is deliberate and in plain sight).
- The binary is built inside the container with:
  ```
  gcc -no-pie -fno-stack-protector -w -o chall chall.c
  ```
  Build your **own** copy with the exact same command so addresses match what the
  server uses, or copy the server's binary out with
  `docker compose cp p1-uaf:/opt/chall/chall ./chall`.

## Goal

There is a function `win()` that reads and prints `/flag`. Nothing calls it on the
normal path. Arrange for it to be called.

## The plan, in stages

Tackle it one checkable step at a time - don't try to land the whole exploit at once.

1. **Read the bug.** In `chall.c`, find the line in `del` that frees the session but
   keeps using the pointer. Write down, in one sentence, why that is dangerous.
2. **Find the target address.** `win()`'s address is fixed (the binary is `-no-pie`).
   Read it off your local copy:
   ```bash
   nm chall | grep ' win'
   ```
3. **Find the offset.** The struct is `{ char name[16]; void (*action)(void); }`. How
   many bytes of your `set` buffer land *before* the `action` pointer?
4. **Build the payload.** It is: `offset` filler bytes, then `win()`'s address in
   **little-endian** (least-significant byte first). `set` takes **hex**, so encode the
   whole thing as a hex string.
5. **Fire the sequence.** `new` -> `del` -> `set <your hex>` -> `go`. The freed chunk is
   reused by the same-sized `set` allocation, so `go` calls *your* pointer.

## Skeleton (fill in the TODOs)

Prove each stage works before moving on. A hand version with `nc`:

```bash
# Stage 2: read the address (fill WIN in)
WIN=$(nm chall | awk '$3=="win"{print $1}')      # e.g. 0000000000401196
echo "win() at 0x$WIN"

# Stage 4+5: TODO - turn $WIN into little-endian bytes, prepend the padding
#            (see stage 3 for how many), and send new/del/set/go over nc.
# printf 'new\ndel\nset <PAD_HEX><WIN_LE_HEX>\ngo\nexit\n' | nc localhost 9100
```

Prefer `pwntools`? Its `p64()` does the little-endian packing and `ELF('./chall')`
reads the symbol for you - you still decide the offset and the command order.

## Staged hints (reveal only as needed)

1. `del` frees the session but does **not** forget the pointer to it. What can you do
   with a pointer to freed memory?
2. The session is `struct { char name[16]; void (*action)(void); }`. Where does
   `action` sit, in bytes? (This is your stage-3 offset.)
3. After `del`, which heap chunk does the next same-sized allocation (`set`) hand back?
   (glibc's *tcache* hands the just-freed chunk straight back - that is what makes this
   deterministic, not luck. Be ready to explain this in your write-up.)
4. `nm chall | grep ' win'` gives you the address to aim for. Mind the byte order
   (little-endian: write the bytes back to front).

## Submit

```bash
../../check.sh p1-uaf 'labcs2{...}'
```

> `check.sh` is a **self-check**, not a scoreboard: it only confirms your exploit worked.
> The points are for your write-up, not the flag.

## Want the defender's view?

Rebuild the source with AddressSanitizer and run your own input through it:
`gcc -fsanitize=address -g -o chall_asan chall.c`. ASan will name the use-after-free
precisely - the same tool PHP's maintainers use to hunt these bugs, and the same one you
ran on the week 6 demo.

## For your write-up

- What is the precise root cause (name the line and the mechanism), and what one-line
  change fixes it?
- *Why* is the exploit reliable rather than lucky? (Explain the same-size free/reuse.)
- Why is hijacking a **function pointer** worse than corrupting an ordinary data value?
