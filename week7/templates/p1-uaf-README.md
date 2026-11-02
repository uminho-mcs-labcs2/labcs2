<!--
  TEMPLATE for challenge P1 (Dangling) - the STRETCH / BONUS challenge. Copy this file to
  week7/p1-uaf/README.md in your TEAM repository and fill in every section. Delete these
  HTML comments as you go. Partial progress counts: if you did not land the full exploit,
  say exactly which stage you reached and what blocked you.
  Reminder (as in week 3): write this yourself - do NOT have an AI tool write it.
-->

# P1 - Dangling (use-after-free -> control-flow hijack) | stretch

**Author(s):** <!-- name(s) and, if relevant, who did what -->

## How far I got

<!--
  Be honest and specific. Which of the brief's stages did you complete (read the bug /
  find the address / find the offset / build the payload / fire the sequence)? If you
  stalled, where and why? This is where partial credit is earned.
-->

## Approach

<!--
  How did the week 6 mem-classes/uaf.c demo help? How did you read win()'s address, work
  out the offset to the function pointer, and order the new/del/set/go sequence?
-->

## Payload / technique

<!--
  Your exploit (pwntools script or the by-hand nc version), enough to reproduce. Show the
  output where win() runs and the flag prints. Do NOT paste your actual flag string.
-->

## Root cause and analysis (answer each)

- **Precise root cause** (name the line and mechanism) and the **one-line fix:**
  <!-- the dangling pointer after free; null it after free -->
- **Why is the exploit reliable rather than lucky?**
  <!-- explain the same-size free/reuse via glibc tcache -->
- **Why is hijacking a function pointer worse than corrupting an ordinary data value?**
  <!-- control-flow hijack vs. data corruption -->

## Attempts, AI interaction, and lessons learned

<!--
  Dead ends and how you recovered; any AI use and the specific value it added; and what
  you take away - including how AddressSanitizer (week 6) would have caught this bug.
-->
