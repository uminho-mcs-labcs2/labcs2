<!--
  TEMPLATE for exercise E1. Copy this file to  week6/E1/README.md  in your TEAM
  repository and fill in every section. Delete these HTML comments as you go.
  Reminder (as in week 3): write this yourself — do NOT have an AI tool write it.
-->

# E1 — Reconnaissance: detect the version and judge exposure

**Author(s):** <!-- name(s) and, if relevant, who did what -->

## Approach

<!--
  How did you fingerprint the PHP version from the outside, and how did you decide
  whether it was exposed? Which signals did you rely on, and why those?
  Scope the result to the two lab CVEs: EXPOSED / PATCHED / UNKNOWN. Explain what
  version comparison can establish and when vendor/package evidence is also needed.
-->

## Detection script

<!--
  Point to your script in this directory. State:
  - how to run it (exact command),
  - the output against the vulnerable service (http://localhost:8080),
  - the output against the patched service (http://localhost:8081).
  Paste the captured output here.
-->

## Discussion

- **Other fingerprinting methods when `X-Powered-By` is turned off:**
  <!-- e.g. default session-cookie name, error-page behavior, responses to odd requests -->
- **Is disabling that header a mitigation or just hardening?**
  <!-- Argue the difference: does it reduce the actual risk of the flaw, or only raise
       the attacker's effort? -->

## Attempts, AI interaction, and lessons learned

<!--
  Dead ends and how you recovered; any AI use and the specific value it added (keep the
  key insights on your side); and what you take away from the exercise.
-->
