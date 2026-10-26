<!--
  TEMPLATE for exercise E2. Copy this file to  week6/E2/README.md  in your TEAM
  repository and fill in every section. Delete these HTML comments as you go.
  Reminder (as in week 3): write this yourself — do NOT have an AI tool write it.
-->

# E2 — Provisional mitigation: virtual patching with a web application firewall

**Author(s):** <!-- name(s) -->

## What I changed and why

<!--
  Describe the rule(s) you added to waf/nginx.conf beyond the provided scaffold, and
  justify each one. What threat or behavior is each rule meant to address?
-->

## Evidence it works

<!--
  Paste the captured output showing:
  - an ordinary request succeeding (HTTP 200), and
  - the provided marker being refused (HTTP 403), and
  - a request your added rule blocks, with the appropriate status (e.g. HTTP 413
    for an oversized body), while legitimate app usage still works.
  Include the configuration-check/reload commands and exact curl commands you used.
  Also capture the marked request to port 8080 succeeding to show the proxy bypass.
-->

## Discussion

- **Does this protect against PHP2Shell?** Separate clearly:
  - what a WAF **can** do here (delay, detect/block known patterns, reduce surface);
  - what it **cannot** do (repair the underlying memory flaw).
- **Why is there "no complete mitigation"?**
  <!-- Why can't an HTTP-layer filter reliably tell a malicious request from a benign
       one for this flaw, without harming legitimate traffic? -->
- **What does the test marker prove, and why does port 8080 bypass the WAF?**
  <!-- Explain the blocking mechanism, its scope, and the backend access restriction
       needed in a real deployment. -->

## Attempts, AI interaction, and lessons learned

<!-- Dead ends; any AI use and the value it added; what you take away. -->
