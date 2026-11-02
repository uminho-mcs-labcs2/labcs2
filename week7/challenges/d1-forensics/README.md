# D1 - "Aftermath" | defensive / forensics | 150 pts | medium

> The blue-team challenge. A server was compromised. Here is its web access log. Find
> out what the attacker took.

The file `access.log` (produced by `../../setup.sh`) is one web server's access log
covering a few minutes around an incident. Somewhere in the noise - ordinary shoppers,
a search-engine crawler, vulnerability scanners - one attacker went further than the
rest and **exfiltrated a secret**. Recover it; it is the flag.

> [!NOTE]
> This challenge is pure log analysis. There is nothing to exploit and no service to
> run - just data to read. Everything stays on your machine.

## Goal

Identify the successful attacker, follow what they did, and recover the data they
stole from the log.

## Hints

1. Not all clients are equal. Which single source stops *probing* and starts *operating*
   - issuing commands and getting `200` responses from a path that shouldn't exist?
2. Attackers often smuggle stolen data inside a URL parameter, encoded so it survives
   transport. What encoding leaves tell-tale `=` padding and `[A-Za-z0-9+/]` only?
3. There are a couple of decoy encoded blobs from unrelated clients. The real one sits
   in the request where the attacker runs a command that reads a secret file.
4. Decode it: `echo '<blob>' | base64 -d`.

## Submit

```bash
../../check.sh d1-forensics 'labcs2{...}'
```

> `check.sh` is a **self-check**, not a scoreboard: it only confirms your exploit worked.
> The points are for your write-up below, not the flag.

## For your write-up

- List the indicators of compromise (IoCs) you used to single out the attacker.
- The attacker's first few POSTs returned `500`, then one returned `200`, after which a
  new path started responding. Reconstruct, in one or two sentences, what happened.
- Which single log field was most useful for separating attacker from noise, and why?
