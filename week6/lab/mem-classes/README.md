# Memory vulnerability classes

Two **minimal, generic** C programs introduce use-after-free and unsafe allocation-
failure handling, concepts relevant to PHP2Shell. They are classroom illustrations;
the OOM demo is a NULL-pointer crash rather than PHP's actual corruption mechanism.
They are not PHP code or exploits.

Why C? Because PHP is *written in* C. A memory flaw in the C code of the interpreter
can therefore compromise an application even when that application's own PHP code is
completely correct — the theme of the whole session.

| File      | Bug class                                   | Related CVE          |
|-----------|---------------------------------------------|-----------------------|
| `uaf.c`   | *use-after-free*                            | CVE-2026-109619       |
| `oom.c`   | unchecked allocation failure / NULL write (related error-path family) | CVE-2026-109618 |

A quick note on terms (all in the [Glossary of `week6.md`](../../week6.md#glossary)):
the **heap** is the pool of memory a program uses for dynamic allocations (`malloc`
and `free` in C); a **use-after-free** is continuing to use memory after it has been
freed; **out of memory** is when an allocation request is refused.

## How to run

You need a C compiler that supports *AddressSanitizer* — a tool, built into modern
`gcc` and `clang`, that instruments a program to catch memory errors at runtime and
report exactly where they happen. Run these commands from `week6/lab/mem-classes/`
in a Linux or WSL2 environment with the compiler and ASan runtime installed (see
[prep](../../week6-prep.md)). The PHP lab images do not install these tools or copy
the C demos into the application containers.

```bash
# use-after-free: see the DIAGNOSIS (ASan) and then the CONSEQUENCE (plain build)
gcc -fsanitize=address -g uaf.c -o uaf_asan && ./uaf_asan   # ASan reports the UAF
gcc                    -g uaf.c -o uaf      && ./uaf        # plain build; likely crash

# out-of-memory handling
gcc -fsanitize=address -g oom.c -o oom && ./oom
```

> [!NOTE]
> Read the comment block at the top of `uaf.c` before running it. AddressSanitizer
> *quarantines* freed memory, which means the "attacker reuses the freed block" step
> does **not** literally happen under ASan — ASan stops the program at the misuse
> first. To actually observe the reuse-and-crash, run the plain (non-ASan) build. The
> two builds show two complementary things: ASan gives you the precise **diagnosis**;
> the plain build shows you the **consequence**.

`oom.c` deliberately simulates an allocation returning NULL, then writes through it.
Its expected result is a NULL-pointer crash reported by ASan. It demonstrates unsafe
allocation-failure handling; it does not reproduce PHP's partially initialized state,
`longjmp()` cleanup problem, or heap corruption.

## For discussion

1. In `uaf.c`, why is calling `s->on_close()` so much more dangerous than, say, reading
   a freed integer? (Hint: it is a *control-flow hijack* — the attacker influences
   *which code runs next*, not just what a value is.)
2. In `oom.c`, why do bugs on the error path survive undetected for years? (Hint: under
   normal load that path essentially never runs, so ordinary testing never reaches it.)
3. Why does "writing secure PHP application code" protect against *neither* of these?
4. Which tools and practices in PHP's own development process could have caught these
   earlier? (Consider: *fuzzing* — automatically bombarding the code with malformed
   inputs to provoke crashes; running AddressSanitizer in *continuous integration*, the
   automated build-and-test pipeline that runs on every change; and careful code review
   of the rarely-executed error paths.)
