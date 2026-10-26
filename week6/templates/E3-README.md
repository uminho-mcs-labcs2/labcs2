<!--
  TEMPLATE for exercise E3. Copy this file to  week6/E3/README.md  in your TEAM
  repository and fill in every section. Delete these HTML comments as you go.
  Reminder (as in week 3): write this yourself — do NOT have an AI tool write it.
-->

# E3 — Remediation and verification

**Author(s):** <!-- name(s) -->

## The patch step

<!--
  What you changed (the PHP_TAG), how you rebuilt, and the before/after proof that
  php2shell_exposed went from true to false. Paste the captured output.
  Explain that this field is calculated from the reported version. The evidence
  verifies deployment of the fixed runtime for the two lab CVEs, rather than an
  independent exploitability test or proof that no prior compromise occurred.
-->

## Evidence

<!--
  Show your E1 detection script run against the service BEFORE and AFTER the update,
  with the captured output. This is the "verify" step of detect -> mitigate ->
  patch -> verify.
-->

## Discussion

- **What is missing, in a real organization, beyond "change the version"?**
  <!-- inventory of every place PHP runs (web, CLI, background jobs, containers);
       staging tests; maintenance window; rollback plan; restarting services so the
       new interpreter is actually loaded; verifying the version afterwards. -->
- **Why is "I updated, therefore I am safe" insufficient here?**
  <!-- Exploitation attempts occurred before disclosure. Patching closes the door, but if an
       attacker entered during the exposure window they may still be inside. What would
       you look for? (indicators of compromise) -->

## Attempts, AI interaction, and lessons learned

<!-- Dead ends; any AI use and the value it added; what you take away. -->
