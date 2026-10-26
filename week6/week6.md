# Week 6 — PHP2Shell: anatomy of an emergency (CVE-2026-109618 / CVE-2026-109619)

Week 6 is devoted to a real and **very recent** case. On October 21, 2026, the team
that maintains **PHP** (a widely used programming language for building web sites and
web applications) shipped emergency fixes for two critical vulnerabilities — not in
any particular web site, but in the PHP *interpreter* itself: the program that reads
and executes PHP code on the server. The pair was collectively nicknamed
**PHP2Shell**.

We will use this episode as a case study to practice what, in practice, is the most
common job of a security team when a serious vulnerability of this kind breaks:
**understand it, detect where you are exposed, mitigate temporarily, and patch** —
all under time pressure and with incomplete information.

> [!IMPORTANT]
> This session is **not** about producing or distributing a working attack tool (an
> *exploit*) for these vulnerabilities. They are memory-corruption flaws in the
> interpreter, they are already being attacked on the public Internet, and
> ready-made demonstration code is circulating. The learning value we are after is in
> understanding the *class* of vulnerability, knowing how to **detect** exposure, and
> knowing how to **defend** — which is exactly what is expected of a professional in
> the first hours of an incident like this.

## A few terms up front

To keep the discussion precise, here are the key terms this session uses. Each is also
explained again where it first appears, and they are collected in the
[Glossary](#glossary) at the end.

- **Vulnerability vs. exploit.** A *vulnerability* is a flaw that could be abused; an
  *exploit* is a concrete piece of code or technique that actually abuses it.
- **CVE identifier.** *Common Vulnerabilities and Exposures* (CVE) is a cataloguing
  scheme that gives each publicly known vulnerability a unique public identifier, such
  as `CVE-2026-109618`.
- **Interpreter / runtime.** The PHP *interpreter* (also called the *runtime*) is the
  program that executes your PHP code. If it is flawed, every application it runs can
  be affected — even an application whose own code is perfect.

## Background: what happened

- **When:** fixes released on October 21, 2026; recorded exploitation attempts
  predate that release (see the evidence below).
- **What:** two critical vulnerabilities in the PHP core, each scored **9.2** on the
  *Common Vulnerability Scoring System*, version 4 (CVSS v4 — a standard 0–10 severity
  scale on which anything from 9.0 up is rated "critical"):
  + **CVE-2026-109618** — memory corruption during *out-of-memory* (OOM) handling.
    An OOM condition can arise when PHP reaches its configured per-request
    `memory_limit`, or when an underlying allocation fails. PHP's early-exit and
    cleanup logic can then consume partially initialized request data. The problem
    is more involved than simply forgetting to check a C `malloc()` return value.
  + **CVE-2026-109619** — a *use-after-free* flaw involving state that the interpreter
    keeps between requests. "Use-after-free" means the program keeps using a chunk of
    memory after it has already been released for reuse (explained in detail in the lab).
- **Impact:** *remote code execution* (RCE) — the ability for an attacker to run their
  own code on the server — and, crucially, **without needing to log in**
  (*unauthenticated*). The attacker's code runs with the privileges of the web
  *worker process* (the operating-system process the web server uses to execute PHP
  and serve a request), so it can do anything that process can.
- **Reach:** affected PHP installations serving web requests, with the highest risk
  in Apache `mod_php`; other server interfaces have different exploitation conditions.
  PHP 8.1 and older are *end-of-life* for normal upstream releases. Some distributions
  and LTS vendors provide backported fixes, and PHP supplied minimal source backports
  for 7.4, 8.0 and 8.1. Check the vendor advisory for the installed package, or upgrade
  to a supported branch; an older version string alone cannot establish patch status.
- **Fixes:** `8.5.12`, `8.4.27`, `8.3.36`, `8.2.35`. The maintainers explicitly stated
  that there is **no complete generic configuration mitigation** — installing the
  fixes is the complete remedy for these two CVEs.
- **Exploitation evidence:** Amazon's MadPot honeypots recorded exploitation
  **attempts** as early as October 17, before public disclosure. A *honeypot* is a
  deliberately exposed, instrumented system used to record attacker activity.
  Amazon's report says it was unclear whether those early attempts would have
  succeeded on real systems. Public *proof-of-concept* (PoC) exploits appeared
  quickly, with portability and reliability depending on the deployment.
  Distinguish observed attempts, demonstrated exploitability, and confirmed compromise.
  [Amazon's first-hand report](https://aws.amazon.com/blogs/security/amazon-threat-intelligence-identifies-php2shell-exploitation-prior-to-public-disclosure-in-madpot/)

### Status update for class (October 26, 2026)

PHP's [October 21 announcement](https://www.php.net/archive/2026.php#2026-10-21-5)
and the upstream [OOM advisory](https://github.com/php/php-src/security/advisories/GHSA-w8hw-w9fx-j79w)
and [stale-state advisory](https://github.com/php/php-src/security/advisories/GHSA-6wmf-4rv5-p3rp)
are the references for affected configurations, fixes, and temporary controls.

The [October 23 follow-up](https://www.php.net/archive/2026.php#2026-10-23-1)
describes a separate issue, CVE-2026-115560, involving session upload progress and an
unusual change of session serialization handler at runtime. PHP advises disabling
`session.upload_progress.enabled` until the next regular bugfix release, unless the
application is known not to use that handler-switching pattern. The sample app does
not switch handlers. E1/E3 (exercises) assess the original two CVEs only; their result is not a
general security verdict or an assessment of this follow-up issue.

### Why is this case instructive?

1. **The application is not at fault.** A victim's own PHP code can be flawless — the
   flaw is in the interpreter underneath it. This is an excellent counterexample to
   the common belief that "security equals writing bug-free application code." The
   chain of trust also includes the runtime, the web server, the operating system, and
   every library in between.
2. **"No complete mitigation" forces defense-in-depth thinking.** *Defense in depth*
   is the practice of layering several independent safeguards so that if one fails,
   the others still stand: temporary filtering, reducing what is exposed, isolating
   the service, and finally the patch itself.
3. **Classic memory-bug classes in an unexpected place.** *Use-after-free* and
   out-of-memory-handling corruption are textbook low-level bugs — and PHP is written
   in the C programming language, where such bugs live. Students do not usually
   associate PHP with manual memory management; this is a chance to connect the two.
4. **Realistic incident dynamics.** A short response window, honeypot telemetry,
   exploits of varying portability and reliability, and a
   flurry of vendor advice (from Cloudflare, SiteGround, the UK National Cyber Security
   Centre, and others).

## Learning objectives

By the end of the session, each student should be able to:

- Explain, in their own words, the difference between a flaw **in the application** and
  a flaw **in the interpreter/runtime**, and why the latter is harder to reason about.
- Explain *use-after-free* and unsafe *out-of-memory handling* using minimal examples,
  distinguishing the OOM demo's NULL-pointer crash from the real PHP corruption.
- **Detect** a service's PHP version from the outside and judge whether it is exposed.
- Apply and justify provisional mitigations — specifically *virtual patching* with a
  *web application firewall* — and clearly state their limits.
- Carry out the full **detect → mitigate → patch → verify** cycle in the provided lab.

## Session plan (2:00 PM – 4:50 PM)

- **2:00 – 2:25 PM:** *briefing* — the PHP2Shell episode, timeline, and background.
- **2:25 – 2:45 PM:** the two memory-bug classes (use-after-free and out-of-memory
  handling) — discussion using the minimal C examples in [`lab/mem-classes/`](lab/mem-classes/).
- **2:45 – 4:15 PM:** the lab (see [`lab/README.md`](lab/README.md)):
  + **E1** — reconnaissance: fingerprint the version and judge exposure.
  + **E2** — virtual patching: write and test a web-application-firewall rule.
  + **E3** — remediation: update the container and verify the running version.
- **4:15 – 4:40 PM:** guided discussion — the limits of mitigations, the exposure
  window, and what an incident-response team would do in the first 24 hours.
- **4:40 – 4:50 PM:** *debriefing* and pointers for the exercise write-ups.

Here, **E** means **exercise**: E1, E2 and E3 are exercises 1, 2 and 3.

## Deliverables

As in previous weeks, for each exercise every student or group should create a
`week6/<exercise>` directory (for example, `week6/E1`) in their *team* repository,
containing:

- A `README.md` file recording the author(s); the strategy followed; attempts,
  including the ones that failed; any interaction with AI tools and the value it added;
  and the **lessons learned**. As in week 3, **do not delegate this file to an AI tool**
  — writing it yourself is where much of the learning is consolidated.
- All supporting code and configuration (firewall rules, detection scripts, and so on).

Template `README.md` files for the three exercises are provided in
[`templates/`](templates/) — copy the relevant one into your exercise directory and
fill it in.

> [!NOTE]
> As always, the reasoning and the "how" are valued **far more** than mechanically
> obtaining a result. In a *defensive* session in particular, the quality of your
> justification for each mitigation is what counts most.

## Prerequisites

See [`week6-prep.md`](week6-prep.md) — essentially Docker and Docker Compose
(tools for packaging and running the lab's services in isolated containers).

## Looking ahead — week 7

This week you stayed on the **defender's** side. Week 7 completes
the picture: a hands-on **Capture The Flag** where you cross to the attacker's side on
*purpose-built*, intentionally vulnerable targets — because understanding how the
exploit works is what makes a defender's choices (and their limits) real. We start it
together in class and you continue at home. It still ships **no** weaponized exploit for
the real CVEs; the memory-corruption challenge (`p1-uaf`) teaches the same *bug class* as
CVE-2026-109619 on a tiny C target you can fully understand — a direct continuation of
the [`mem-classes/`](lab/mem-classes/) demos.

## Further reading

- PHP — [October 21 security announcement and deployment guidance](https://www.php.net/archive/2026.php#2026-10-21-5).
- PHP Security Team — [CVE-2026-109618: RCE via OOM corruption](https://github.com/php/php-src/security/advisories/GHSA-w8hw-w9fx-j79w).
- PHP Security Team — [CVE-2026-109619: RCE via stale global state](https://github.com/php/php-src/security/advisories/GHSA-6wmf-4rv5-p3rp).
- PHP — [October 23 session upload progress follow-up](https://www.php.net/archive/2026.php#2026-10-23-1).
- Amazon Threat Intelligence — [MadPot observations and incident timeline](https://aws.amazon.com/blogs/security/amazon-threat-intelligence-identifies-php2shell-exploitation-prior-to-public-disclosure-in-madpot/).

## Glossary

Quick reference for the terms used above and in the lab materials.

| Term | Meaning |
|------|---------|
| **Vulnerability** | A flaw that could be abused to violate a security property. |
| **Exploit** | Concrete code or a technique that actually abuses a vulnerability. |
| **CVE** | *Common Vulnerabilities and Exposures* — the scheme that assigns each public vulnerability a unique identifier (e.g., `CVE-2026-109618`). |
| **CVSS** | *Common Vulnerability Scoring System* — a 0–10 severity scale; 9.0+ is "critical". |
| **Interpreter / runtime** | The program (here, PHP) that executes an application's code. |
| **RCE** | *Remote code execution* — an attacker running their own code on a target machine. |
| **Unauthenticated** | Achievable without any valid login or credentials. |
| **Worker process** | The operating-system process the web server uses to run PHP and serve a request. |
| **OOM** | *Out of memory* — reaching a runtime's memory limit or an underlying allocation failure; handling these conditions safely is essential. |
| **Use-after-free** | Continuing to use a region of memory after it has been released for reuse. |
| **Heap** | The region of memory used for dynamic allocations (in C, via `malloc`/`free`). |
| **Control-flow hijacking** | Forcing a program to execute code or jump to an address the attacker chose. |
| **End-of-life (EOL)** | A branch no longer receiving normal upstream updates; vendor support and exceptional backports must be checked separately. |
| **n-day** | A vulnerability already publicly known and/or fixed (contrast *zero-day*, unknown to defenders when first exploited). |
| **Honeypot** | A deliberately exposed, instrumented system that exists only to attract and log attacker activity. |
| **PoC** | *Proof-of-concept* — demonstration code showing a flaw can be triggered. |
| **Fingerprinting** | Inferring which software and version a service runs from externally observable signals. |
| **WAF** | *Web application firewall* — a filter that inspects incoming web (HTTP) requests and can block ones matching malicious patterns. |
| **Virtual patching** | Blocking exploitation of a flaw with external controls (typically WAF rules) without changing the vulnerable software itself. |
| **Defense in depth** | Layering independent safeguards so that if one fails, others still protect. |
| **Fuzzing** | Automatically feeding a program malformed or random inputs to surface crashes and memory errors. |
| **AddressSanitizer (ASan)** | A compiler tool that detects memory errors (use-after-free, overflows, etc.) at runtime. |
| **IoC** | *Indicator of compromise* — observable evidence (a file, a log entry, a network connection) that a system may have been breached. |
