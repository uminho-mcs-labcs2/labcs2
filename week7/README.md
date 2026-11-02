# week 7 - PHP2Shell CTF: from defending to understanding the attack

A small, self-hosted **Capture The Flag** that is the hands-on core of the week 7
session. Week 6 had you *defend* a vulnerable service; week 7 crosses to the other
side - you *exploit* targets built to be vulnerable, so you can understand each
mechanism completely. We **start it together in class** (see
[`week7.md`](week7.md)) and you continue at home.

A *flag* is a secret string, in the format `labcs2{...}`, that you can only obtain by
solving a challenge. You submit it to the local checker to confirm the solve.

## Start here - your checklist

Do these in order. Each step links to the section with the full details.

**One-time setup (on your own machine; see the safety note below):**

1. Install/check installation of **Docker + Docker Compose** and **`curl`** - on Windows, use WSL2.
   -> [`week7-prep.md`](week7-prep.md), [Requirements](#requirements)
2. "Pre-pull" the base images:
   `docker pull php:8.3.36-apache` (W1/W2) and `docker pull ubuntu:22.04` (P1 stretch).
3. Rehearse the stack once, then stop it: `./setup.sh`, then `docker compose up --build -d`,
   confirm ports **9001 / 9002 / 9100** answer, then `docker compose down`.
   -> [Setup (once)](#setup-once)

**In and after class - play the CTF:**

4. Read this file and the session plan in [`week7.md`](week7.md).
5. Start the stack: `./setup.sh`, then `docker compose up --build`. -> [Setup (once)](#setup-once)
6. Solve the challenges in order **W1 -> W2 -> D1 -> P1** - each brief is in
   `challenges/<id>/README.md`. -> [The challenges](#the-challenges)
7. Check each flag offline: `./check.sh <id> 'labcs2{...}'`. -> [Submitting flags](#submitting-flags)
8. Write up every challenge you attempt, using the matching template - **the write-up is
   your entire grade; the flag is worth nothing.** -> [What to hand in](#what-to-hand-in)

> [!CAUTION]
> **Every target here is deliberately vulnerable. Run it only on your own computer and
> never expose these ports to the Internet or a shared network.** This whole directory
> is an isolated practice range; treat it like live ammunition on a closed range.

**About the challenge IDs (`W1`, `W2`, `D1`, `P1`).** These are **track codes plus a number** -
they are *not* week numbers. The letter is the challenge category (**W** = web, **D** =
defensive/blue, **P** = pwn) and the digit just counts challenges within that track, so `W1`
and `W2` are the first and second *web* challenges (not weeks 1 and 2). This is the usual way
CTF categories are labelled. Each full id also carries a suffix naming the bug class - `w1-cmdi`
(command injection), `w2-objinj` (object injection), `d1-forensics` (log analysis), `p1-uaf`
(use-after-free).

## How this connects to weeks 6 and 7

Week 6 made one point central: *the application can be fine while the interpreter is
vulnerable*, and it kept you on the **defender's** side (detect -> mitigate -> patch ->
verify). Week 7 completes the picture by having you *understand the attacker* - the only
honest way to know why those defenses matter:

- **W1 and W2** flip week 6 around - a **patched** PHP, but a vulnerable **application**.
  This is where most real bugs actually live.
- **P1** is the same *bug class* as the PHP2Shell use-after-free (CVE-2026-109619),
  shrunk to a C target small enough to exploit end to end. It is the **stretch**
  challenge (see below).
- **D1** returns to the **defender's** side: reconstruct an intrusion from a web server
  log - the "assume you were hit" step week 6 ended on.

> [!NOTE]
> We deliberately do **not** ship a working exploit for the real CVE-2026-109618 /
> CVE-2026-109619. Those are live, in-the-wild flaws; a weaponized exploit has little
> teaching value (the public ones are fragile and version-specific) and does not belong
> in a student repository. P1 teaches the *mechanism* honestly and safely instead.

## Setup (once)

```bash
cd week7
./setup.sh                 # generates your personal flags (gitignored) + verify hashes
docker compose up --build  # builds and starts the three services
```

Stop with `docker compose down`. Re-run `./setup.sh` any time to roll fresh flags.

> `./setup.sh` must run **before** `docker compose up`: it creates the `flag.txt` files
> that the containers mount. If you skip it, the flag mounts won't exist.

## The challenges

| id             | name        | track              | port / data                         | pts | difficulty       |
|----------------|-------------|--------------------|-------------------------------------|-----|------------------|
| `w1-cmdi`      | Nettools    | web                | <http://localhost:9001>             | 100 | easy             |
| `w2-objinj`    | Preferences | web                | <http://localhost:9002>             | 200 | medium           |
| `d1-forensics` | Aftermath   | defensive / blue   | `challenges/d1-forensics/access.log`| 150 | medium           |
| `p1-uaf`       | Dangling    | pwn (binary)       | `nc localhost 9100`                 | 300 | hard - **stretch** |

**The flag is worth zero points.** Read that again before you trust the `pts` column above:
those numbers are the *maximum for your write-up* of each challenge - the exact vulnerable
line, the mechanism, and the fix. Capturing the flag earns you **nothing**, and you never
hand it in. It is only a **self-check** that your exploit actually worked, plus a bit of
motivation. A half-finished but honest write-up beats a captured flag with no explanation,
every single time.

Each challenge has its own `README.md` under `challenges/<id>/` with the brief and hints.
Suggested order: **W1 -> W2 -> D1 -> P1**.

> [!IMPORTANT]
> **P1 is a scored stretch / bonus challenge.** The three core challenges (W1, W2, D1)
> are worth 450 points and are what everyone is expected to attempt. P1 is binary
> exploitation - a bit harder, and worth the most *because* it is optional reach.
> Its brief walks you up to it in stages, starting from a demo you already met in week 6.
> Do not let it block you: solve the core three first, then stretch.

## Submitting flags

Check your answers offline, against the hashes `setup.sh` recorded:

```bash
./check.sh w1-cmdi 'labcs2{...}'
```

Because flags are random per student, you can't copy a classmate's - compare **methods**
instead. But be blunt with yourself about what this check is for: **the flag scores
nothing.** You don't submit it, and capturing it earns zero points - it only tells *you*
that your exploit actually worked. Your grade is **the write-up**: how you did it, what
failed, and what you learned.

## What to hand in

For each challenge you solve, create `week7/<id>/` in your **team** repository with a
`README.md` covering: your approach, the payload/technique (enough to reproduce it),
dead ends, any AI interaction and the value it added, and the lessons learned. Answer the
"For your write-up" questions in each challenge brief. Template `README.md` files are in
[`templates/`](templates/) - copy the relevant one into your challenge directory and fill
it in. Write this yourself - do **not** have an AI tool write it.

## Requirements

- Docker + Docker Compose, plus `curl` (see [`week7-prep.md`](week7-prep.md)).
- For P1: `gcc`, `nc` (netcat), and ideally `pwntools` (installed back in week 3).
- Standard shell tools for D1: `grep`, `base64`.

