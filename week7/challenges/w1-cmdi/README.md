# W1 - "Nettools" | web | 100 pts | easy

> The warm-up. Real code execution, on a *patched* PHP - because this time the hole is
> in the **application**, not the interpreter. (The exact inverse of week 6's lesson.)

A small network-tools web app is running at **http://localhost:9001**. It lets you
ping a host. The flag is in a file at `/flag` inside the container.

## Goal

Read `/flag` by abusing the app, and submit it.

## Hints

1. What does the app *do* with the value you type into the "host" field?
2. On a shell, how do you run a second command after the first one?
3. `curl` is your friend for sending exactly the request you want (remember to
   URL-encode characters like `;` and spaces).

## Submit

```bash
../../check.sh w1-cmdi 'labcs2{...}'
```

> `check.sh` is a **self-check**, not a scoreboard: it only confirms your exploit worked.
> The points are for your write-up below, not the flag.

## For your write-up

- What is the precise root cause, in one sentence?
- Name two correct fixes, and say which you'd prefer and why. (Hint: think about
  *avoiding the shell altogether* vs. *escaping input*.)
