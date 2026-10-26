# Lab — PHP2Shell (week 6)

A **defensive** lab on the PHP2Shell episode (CVE-2026-109618 / CVE-2026-109619).
It drills the response cycle a real security team runs when a serious, already-public
vulnerability lands: **detect → mitigate → patch → verify**.

> [!IMPORTANT]
> This lab does **not** include or produce a working attack tool (an *exploit*) for
> these vulnerabilities. The goal is to understand the vulnerability *class* and master
> the **defense**. Every interaction we make with the services is a legitimate, ordinary
> web request.

Terms used here (CVE, RCE, WAF, fingerprinting, and so on) are defined in the
[Glossary of `week6.md`](../week6.md#glossary); the most important ones are also
explained inline below where they are first needed.

## Architecture

The lab runs three services as Docker containers — self-contained, isolated units that
bundle a program with everything it needs to run. They are wired together by Docker
Compose (a tool that starts and connects several containers from one description file):

```
  ┌─────────────┐        ┌───────────────────────┐
  │   vuln      │  8080  │ PHP 8.3.35 (affected)  │   <- the service we study
  └─────────────┘        └───────────────────────┘
  ┌─────────────┐        ┌───────────────────────┐
  │  patched    │  8081  │ PHP 8.3.36 (fixed)     │   <- fixed for the two lab CVEs
  └─────────────┘        └───────────────────────┘
  ┌─────────────┐        ┌───────────────────────┐
  │   waf       │  8088  │ nginx  ->  vuln        │   <- virtual patching (E2)
  └─────────────┘        └───────────────────────┘
```

The three numbers (8080, 8081, 8088) are the *ports* on your own machine where each
service is reachable; a web browser or `curl` command picks a service by addressing its door.

The published ports are bound to `127.0.0.1` for local use. Port 8080 deliberately
remains accessible for direct comparison with the proxy on 8088. Requests to 8080
bypass every proxy rule. In a deployment relying on a WAF, restrict backend access so
clients can reach the application only through the proxy.

Crucially, both PHP containers run the **same application**, and the proxy forwards
requests to `vuln`. The app has no deliberately introduced application exploit; this
lab compares PHP runtimes. It is a teaching sample.

## Startup

```bash
# from the week6/lab/ directory
docker compose up -d --build --wait                 # vuln:8080 and patched:8081
docker compose --profile waf up -d --build --wait   # also waf:8088 for E2
```

These commands run in the background, leaving the terminal available for `curl`.
HTTP health checks make `--wait` wait for the app and, when enabled, the proxy to
serve requests. If startup fails, inspect `docker compose --profile waf ps` and
`docker compose --profile waf logs`. Stop and clean up, including the optional
proxy, with `docker compose --profile waf down`.

---

## E1 — Reconnaissance: detect the version and judge exposure

**Goal:** identify a service's PHP version from outside and assess its likely exposure
to the two lab CVEs. This is *fingerprinting* — inferring the software and version a
service runs from signals it leaks to any visitor.

1. Fingerprint the version **without** looking at the source code. By default, PHP
   announces itself in a response header (a line of metadata the server sends back with
   every page) called `X-Powered-By`:
   ```bash
   curl -sI http://localhost:8080 | grep -i x-powered-by
   curl -sI http://localhost:8081 | grep -i x-powered-by
   ```
   (`curl` is a command-line tool for making web requests; `-s` means "quiet" and `-I`
   means "fetch only the response headers, not the page body".)
2. Cross-check the version with the diagnostic endpoint and note the
   `php2shell_exposed` field. The response is in JSON — *JavaScript Object Notation*,
   a simple text format for structured data:
   ```bash
   curl -s http://localhost:8080/version.php
   curl -s http://localhost:8081/version.php
   ```
3. **Write a small script** (in `bash`, Python, or a language of your choice) that,
   given a URL, fetches the version and prints `EXPOSED`, `PATCHED`, or `UNKNOWN` by
   comparing it against the fixed release for that branch (`8.5.12`, `8.4.27`,
   `8.3.36`, `8.2.35`). Use `UNKNOWN` for missing/unrecognized versions, unsupported
   branches, or failed requests; never interpret an absent version as patched. Save
   it under `week6/E1/` in your team repository.

The endpoint calculates `php2shell_exposed` from that same version comparison; it
does not probe either vulnerability. For these official PHP images, `false` means
the reported version includes the fixes for the two lab CVEs. Vendor packages may
backport fixes while retaining an older version string, so real assessments also
need package provenance and the vendor advisory. `PATCHED` is scoped to these two
CVEs, rather than an overall claim of safety; see the
[October 23 follow-up](../week6.md#status-update-for-class-october-26-2026).

**Questions to answer in your E1 `README.md`:**
- What *other* ways are there to fingerprint the version when the `X-Powered-By` header
  has been turned off? (Hint: consider the default session-cookie name, error-page
  behavior, and responses to unusual requests.)
- Is turning that header off a *mitigation* — that is, does it actually reduce the risk
  of the vulnerability — or merely *hardening* that makes an attacker work a little
  harder? Justify your answer.

---

## E2 — Provisional mitigation: virtual patching with a web application firewall

**Goal:** buy time when, as here, "there is no complete mitigation" and you cannot yet
apply the real patch.

A *web application firewall* (WAF) is a filter placed in front of a web service that
inspects each incoming request and can block the ones that look malicious. Using it to
block exploitation of a specific known flaw, without touching the vulnerable software
itself, is called *virtual patching*. In this lab the WAF is a small `nginx` reverse
proxy (a server that receives requests and forwards them on to `vuln`).

1. Bring up the profile that includes the proxy:
   `docker compose --profile waf up -d --build --wait`.
2. The `waf` service (port 8088) forwards traffic to `vuln`, but first it:
   - hides the `X-Powered-By` header (surface reduction);
   - limits how many requests one client may send per second (rate limiting);
   - blocks a representative, clearly-marked test "signature" (see `waf/nginx.conf`).
3. Check the blocking mechanics. The first request should succeed (HTTP status `200 OK`);
   the second, carrying the test marker, should be refused (`403 Forbidden`):
   ```bash
   curl -sI http://localhost:8088/                                     # 200 OK
   curl -sI -H 'X-Lab-Attack: php2shell-demo' http://localhost:8088/   # 403 Forbidden
   ```
   (`-H` adds a custom request header; here it carries our test marker.)
   This marker is a synthetic test of the blocking mechanism, with no PHP2Shell
   payload. Repeat it against `http://localhost:8080/`: the app should return `200`
   because that port bypasses the proxy. Also note that hiding `X-Powered-By` leaves
   the version visible in the sample page and `/version.php`.
4. **Improve the configuration** (`waf/nginx.conf`) with at least one additional rule
   you can justify — for example, rejecting unusual HTTP request methods or limiting the
   size of the request body. Save your version under `week6/E2/`. Validate and reload
   the mounted configuration, then test both an allowed request and a request your
   new rule rejects:
   ```bash
   docker compose --profile waf exec waf nginx -t
   # Only reload if the configuration check succeeds:
   docker compose --profile waf exec waf nginx -s reload
   ```
   Record the status appropriate to your rule: the marker returns `403`; nginx's
   `client_max_body_size` normally returns `413` for an oversized body. Also check
   that legitimate app usage still works, including posting a message (`302` redirect
   followed by a `200` page). Test a body-size rule with a POST containing a body;
   the header-only `curl -I` requests above do not exercise that limit. nginx already
   has a [default body-size limit](https://nginx.org/en/docs/http/ngx_http_core_module.html#client_max_body_size),
   so explain how your setting changes that policy.

**Questions to answer in your E2 `README.md`:**
- Does your rule actually *protect* against PHP2Shell? Separate clearly what a WAF
  **can** do (delay attackers, detect and block *known* exploit patterns, reduce
  surface) from what it **cannot** do (repair the underlying memory flaw).
- Why did the maintainers say there is no complete mitigation? (Hint: the flaw is
  triggered through requests that can look perfectly ordinary, so a filter cannot
  reliably tell malicious from benign without blocking legitimate traffic too.)
- Why does direct access to port 8080 bypass the WAF, and how should backend access
  be restricted in a deployment relying on this control?

---

## E3 — Remediation and verification

**Goal:** deploy the fixed runtime and verify its version in the running service.
The before/after comparison demonstrates patch deployment for the two lab CVEs;
it does not independently test exploitability or rule out prior compromise.

1. Confirm that `patched` (port 8081) reports `php2shell_exposed: false`:
   ```bash
   curl -s http://localhost:8081/version.php
   ```
   Before changing `vuln`, run your E1 script against port 8080 and save that result
   as the **before** evidence under `week6/E3/`, including the URL and version.
2. In `docker-compose.yml`, notice that both services use the same app and Dockerfile;
   their PHP build argument differs in `PHP_TAG` — the interpreter version. Simulate
   "fixing" the vulnerable service by changing its `PHP_TAG` to `8.3.36-apache` and
   rebuilding just that service:
   ```bash
   docker compose up -d --build --wait vuln
   curl -s http://localhost:8080/version.php   # now reports exposed: false
   ```
3. Run your E1 detection script against port 8080 again and **save the captured output
   as after evidence** under `week6/E3/`, including the URL and version. Compare it
   with the before capture from step 1.
4. If you started the WAF in E2, recreate it to resolve the rebuilt backend's current
   address. nginx resolves the static `vuln` upstream when it loads the configuration;
   the old address can otherwise produce `502` responses after container replacement:
   ```bash
   docker compose --profile waf up -d --force-recreate --no-deps --wait waf
   curl --fail -s http://localhost:8088/version.php
   ```
   Check that the proxy also reports the fixed version.

To repeat the lab from its initial state, stop the services, restore **only `vuln`'s**
`PHP_TAG` to `8.3.35-apache`, and rerun the startup command. Keep `patched` on
`8.3.36-apache`.

**Questions to answer in your E3 `README.md`:**
- In a real organization, what steps are needed *beyond* "change the version"? (Think
  about: a complete inventory of every place PHP runs — web, command line, background
  jobs, containers; testing in a staging environment; a maintenance window; a rollback
  plan; restarting services so the new interpreter is actually loaded; and verifying the
  version afterwards.)
- Exploitation attempts were observed *before* public disclosure. Explain why "I updated, therefore I
  am safe" is insufficient. (Hint: patching closes the door, but if an attacker already
  walked through it during the exposure window, they may still be inside — so you must
  also hunt for *indicators of compromise*: observable evidence, such as unexpected
  files, log entries, or outbound connections, that a system may already be breached.)

---

## Memory bug classes

See [`mem-classes/`](mem-classes/) for two minimal C programs: a *use-after-free*
and an unchecked allocation failure that causes a NULL-pointer write. The latter
illustrates unsafe OOM handling, rather than the actual heap corruption in PHP.
They are teaching illustrations, not the PHP code or exploits.

---

## Final discussion (debriefing)

- As a response team, what would you do in the **first 24 hours**, and in what order?
- Where does each layer (WAF, patch, isolation, inventory) fit into *defense in depth* —
  the practice of layering independent safeguards?
- The UK National Cyber Security Centre advised taking offline anything that cannot be
  fixed right away. Under what circumstances is that drastic step the right call, and
  what does it cost?
