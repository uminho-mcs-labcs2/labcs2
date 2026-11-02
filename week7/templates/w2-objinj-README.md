<!--
  TEMPLATE for challenge W2 (Preferences). Copy this file to  week7/w2-objinj/README.md
  in your TEAM repository and fill in every section. Delete these HTML comments as you go.
  Reminder (as in week 3): write this yourself - do NOT have an AI tool write it.
-->

# W2 - Preferences (PHP object injection)

**Author(s):** <!-- name(s) and, if relevant, who did what -->

## Approach

<!--
  How did you discover the format the app expects, identify the usable class/gadget, and
  decide which property to set? What did reading the source tell you?
-->

## Payload / technique

<!--
  The serialized object you crafted, its base64 form, and the exact request you sent.
  Show the response where the flag appears. Do NOT paste your actual flag string.
-->

## Root cause and fix

- **Why is `unserialize()` on user input dangerous even when you "only" expected one
  class?**
  <!-- arbitrary objects + magic methods = a gadget you did not anticipate -->
- **The correct fix:**
  <!-- what format should carry the data instead, and which function replaces unserialize?
       mention allowed_classes if serialization is unavoidable -->

## Attempts, AI interaction, and lessons learned

<!--
  Dead ends and how you recovered; any AI use and the specific value it added; and what
  you take away from the exercise.
-->
