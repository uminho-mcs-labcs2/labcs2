<!--
  TEMPLATE for challenge D1 (Aftermath). Copy this file to  week7/d1-forensics/README.md
  in your TEAM repository and fill in every section. Delete these HTML comments as you go.
  Reminder (as in week 3): write this yourself - do NOT have an AI tool write it.
-->

# D1 - Aftermath (log forensics / blue team)

**Author(s):** <!-- name(s) and, if relevant, who did what -->

## Approach

<!--
  How did you separate the one successful attacker from the noise (shoppers, crawlers,
  scanners)? What did you filter on, and in what order?
-->

## Technique / commands

<!--
  The exact grep/decode steps you used to find and recover the exfiltrated secret, enough
  to reproduce. Do NOT paste your actual flag string.
-->

## Analysis (answer each)

- **Indicators of compromise (IoCs) you used to single out the attacker:**
  <!-- list them: non-browser user-agent, 200s from a path that shouldn't exist, etc. -->
- **Reconstruct the intrusion:** the attacker's first POSTs returned `500`, then one
  returned `200`, after which a new path started responding - what happened, in one or two
  sentences?
  <!-- -->
- **Which single log field was most useful for separating attacker from noise, and why?**
  <!-- -->

## Attempts, AI interaction, and lessons learned

<!--
  Dead ends and how you recovered; any AI use and the specific value it added; and what
  you take away from the exercise (how this connects to week 6's "assume you were hit").
-->
