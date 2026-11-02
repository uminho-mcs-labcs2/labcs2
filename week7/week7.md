# Week 7 - PHP2Shell, continued: understanding the attack (hands-on CTF)

Week 6 kept us on the **defender's** side of the PHP2Shell episode: detect where you are
exposed, mitigate temporarily, patch, and verify. Week 7 completes the picture by
crossing to the **attacker's** side - not to produce weapons, but because you cannot
fully judge a defense until you understand the attack it is meant to stop.

This is a **hands-on** session built around a small **Capture The Flag** (CTF): a set of
challenges, each guarding a secret *flag* string you can only obtain by exploiting the
target. After a short kickoff, you spend the class **working the challenges**, with the
instructor on hand, and you continue at home. The operational details (setup, ports,
submitting flags) live in [`README.md`](README.md); this document is the session plan and
the "why."

> [!IMPORTANT]
> Every target in this session is **ours**, built to be vulnerable, and runs **only on
> your own machine** on an isolated practice range. We exploit these to understand the
> *mechanisms*. We do **not** build or run a working exploit for the real
> CVE-2026-109618 / CVE-2026-109619 - those are live, in-the-wild flaws, and a
> weaponized exploit has no place in a student repository. The memory-corruption
> challenge teaches the same *bug class* as the real use-after-free on a tiny C target
> you can fully understand. Understanding an attack to defend against it is exactly the
> professional skill this session builds; using it against systems that are not yours is
> out of scope and out of bounds.

## A few terms up front

Week 6's [Glossary](../week6/week6.md#glossary) still applies. Week 7 adds the
attacker's vocabulary:

- **Command injection.** Making a web application run **operating-system commands** of
  your choosing, because it builds a shell command out of your input without separating
  data from code.
- **Deserialization.** Turning stored/transmitted bytes back into a live in-memory
  object. PHP's `unserialize()` does this; doing it on **attacker-controlled** input is
  dangerous.
- **Object injection / gadget.** When `unserialize()` runs on your input, you can
  conjure an object of any loaded class with any properties. A class whose **magic
  method** (a method PHP runs automatically, e.g. `__destruct`) does something useful to
  an attacker is called a **gadget**.
- **Control-flow hijack / "ret2win".** Forcing a program to run code of the attacker's
  choosing. In the pwn challenge this means redirecting a **function pointer** to a
  `win()` function the normal program flow never calls - the same mechanism as the week 6
  `uaf.c` demo.
- **Use-after-free (recap).** Continuing to use a chunk of memory after it has been freed
  for reuse; if what you reuse is a function pointer, you get a control-flow hijack.

## How this builds on week 6

Week 6's central claim was *the application can be fine while the interpreter is
vulnerable*. Week 7 shows you the rest of the landscape and then closes the loop back to
defense:

- **W1 / W2 invert week 6**: the PHP is **patched**, but the **application** is the bug.
  This is where the overwhelming majority of real-world web vulnerabilities actually
  live - a necessary counterweight to week 6's "the runtime was at fault."
- **P1** is the honest bridge to CVE-2026-109619: the same use-after-free *bug class*
  from the week 6 `mem-classes/uaf.c` demo, now something **you** drive end to end.
- **D1** returns you to the **blue team**: reconstruct an intrusion from a web server
  log - the "assume you may already have been hit" step week 6 ended on.

## Learning objectives

By the end of the session, each student should be able to:

- Explain and demonstrate **command injection** and **PHP object injection**, naming the
  exact line and mechanism that makes each target exploitable.
- Propose the **correct fix** for each - and argue why it is better than a weaker one
  (e.g., avoiding the shell vs. escaping; JSON vs. `unserialize`).
- Describe how a **use-after-free** becomes a **control-flow hijack**, and (as a stretch)
  drive a minimal one to call `win()`, explaining *why* the memory reuse is reliable.
- Reconstruct a simple intrusion from a web access log and justify the **indicators of
  compromise** used to single out the attacker.
- For every challenge, connect the exploit back to the **defense** that stops it.

## Session plan (2:00 PM - 4:50 PM)

The format is a short kickoff and then **you working the challenges in class**, so you
leave with real momentum and your questions already answered.

- **2:00 - 2:10 PM:** *recap and framing* - from "defend the runtime" (week 6) to
  "understand the attack" (week 7); the ground rules of the practice range.
- **2:10 - 2:35 PM:** *kickoff* - the three attack ideas by quick example: command
  injection (W1), object injection and gadgets (W2), and the use-after-free ->
  control-flow hijack, demonstrated live from week 6's
  [`mem-classes/uaf.c`](../week6/lab/mem-classes/) as the on-ramp to P1. Plus a one-minute
  tour of `setup.sh` / `check.sh` and how per-student flags work.
- **2:35 - 4:35 PM:** *supervised CTF* - start the challenges in the suggested order
  **W1 -> W2 -> D1**, then attempt the **P1 stretch**. The instructor circulates; use the
  staged hints in each brief before asking. Aim to finish **W1** and have **W2** or **D1**
  well under way before you leave.
- **4:35 - 4:50 PM:** *debrief* - common pitfalls, and for each challenge the one-line
  **fix** that would have stopped it; what to finish at home and how to write it up.

> [!NOTE]
> Before class, complete [`week7-prep.md`](week7-prep.md) and run `./setup.sh` once, so
> class time goes to solving, not installing.

## The challenges

| id             | name        | track              | pts | difficulty         |
|----------------|-------------|--------------------|-----|--------------------|
| `w1-cmdi`      | Nettools    | web (command injection) | 100 | easy          |
| `w2-objinj`    | Preferences | web (object injection)  | 200 | medium        |
| `d1-forensics` | Aftermath   | defensive / blue team   | 150 | medium        |
| `p1-uaf`       | Dangling    | pwn (use-after-free)    | 300 | hard - **stretch** |

Full briefs, ports, and setup are in [`README.md`](README.md) and each
`challenges/<id>/README.md`.

> [!IMPORTANT]
> **P1 is a scored stretch / bonus.** The three core challenges (W1, W2, D1) total 450
> points and are what everyone is expected to attempt; P1 adds 300 on top for those who
> reach it. It is worth the most precisely because it is optional. Its brief breaks the
> exploit into checkable stages starting from the week 6 demo, and partial progress with
> a clear write-up earns partial credit - so attempt it, but do the core three first.

## Each attack maps back to a defense

The point of the offensive week is defensive understanding. Keep this mapping in view -
it is also what your write-ups should make explicit:

| Challenge | The attack | The defense it teaches |
|-----------|------------|------------------------|
| W1 | OS command injection | Don't build shell strings from input - use an argument-vector API; allowlist. |
| W2 | PHP object injection | Never `unserialize()` untrusted input - carry data as JSON. |
| P1 | Use-after-free -> control-flow hijack | Null the pointer after `free`; catch it with AddressSanitizer / fuzzing (week 6). |
| D1 | Post-exploitation exfiltration | Detection and incident response: read the logs, know your IoCs. |

## Deliverables

As in previous weeks, for each challenge you solve create a `week7/<id>` directory (for
example, `week7/w1-cmdi`) in your **team** repository, containing:

- A `README.md` recording the author(s); your approach and the payload/technique (enough
  to reproduce it); the dead ends; any interaction with AI tools and the specific value
  it added; and the **lessons learned** - including the answers to the "For your
  write-up" questions in the challenge brief. As in week 3, **do not delegate this file
  to an AI tool** - writing it yourself is where the learning consolidates.
- Any supporting code (exploit scripts, `curl` commands, decode steps).

Template `README.md` files are in [`templates/`](templates/) - copy the one for your
challenge and fill it in.

> [!IMPORTANT]
> **The flag is worth zero points - your grade is the write-up.** You don't even submit the
> flag; capturing it only proves to *you* that your exploit worked. A correct root-cause
> analysis and the *right* fix are the whole point - a flag obtained by trial and error with
> no explanation earns nothing. Honest documentation of dead ends is rewarded, not penalized.

## Prerequisites

See [`week7-prep.md`](week7-prep.md): Docker and Docker Compose and `curl` (as in week
6), plus `gcc`, `nc`, and ideally `pwntools` for the P1 stretch.

## Further reading

- OWASP - *OS Command Injection* and the *Command Injection Defense* cheat sheet.
- OWASP - *PHP Object Injection* and the *Deserialization Cheat Sheet*.
- PHP Manual - `unserialize()` (see the security note and the `allowed_classes` option)
  and `escapeshellarg()` / `proc_open()` with an argument vector.
- "ret2win"-style introductions to control-flow hijacking (any beginner pwn course), and
  AddressSanitizer's use-after-free documentation (the defender's tool from week 6).
- For D1: any primer on reading web **access logs** and basic **indicators of
  compromise**.

## Glossary (week 7 additions)

Week 6's glossary still applies; these are the terms this session adds.

| Term | Meaning |
|------|---------|
| **Capture The Flag (CTF)** | A practice format where each challenge hides a secret *flag* string you obtain only by solving it. |
| **Command injection** | Getting an application to run OS commands of your choosing because it builds a shell command from untrusted input. |
| **Deserialization** | Rebuilding an in-memory object from stored/transmitted bytes (PHP: `unserialize()`). |
| **Object injection** | Abusing `unserialize()` on attacker input to instantiate arbitrary objects with arbitrary properties. |
| **Gadget** | A class whose automatically-run **magic method** (e.g. `__destruct`) does something useful to an attacker. |
| **Magic method** | A PHP method the runtime calls automatically on certain events (construction, destruction, etc.). |
| **Control-flow hijack / ret2win** | Redirecting a program's execution to code of the attacker's choosing - here, a `win()` the normal flow never calls. |
| **Function pointer** | A variable that stores the address of a function to call; overwriting it redirects execution. |
| **tcache** | A per-thread cache in glibc's allocator that hands a just-freed chunk straight back to the next same-size `malloc` - what makes the P1 reuse deterministic. |
