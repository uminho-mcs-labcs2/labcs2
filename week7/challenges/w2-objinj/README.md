# W2 - "Preferences" | web | 200 pts | medium

> A step up: PHP Object Injection. Still the app's fault, but subtler than W1.

A preferences page is running at **http://localhost:9002**. It remembers your settings
by carrying a serialized PHP object in a request parameter. The flag is at `/flag`.

## Goal

Make the app disclose `/flag`, and submit it.

## Background you'll need

- PHP's `serialize()` turns an object into a string; `unserialize()` turns it back.
- Calling `unserialize()` on **attacker-controlled** input is dangerous: you can
  conjure an object of *any* class the app has loaded, with *any* property values.
- Some classes have "magic methods" (e.g. `__destruct`, `__wakeup`) that run
  automatically. A class whose magic method does something useful to an attacker is
  called a **gadget**.

## Hints

1. Visit the page with no parameters first - it shows you the exact format it expects.
2. Read the source (`app/index.php`). Which classes exist? Which one has a magic method,
   and what does that method do with its properties?
3. Hand-craft a serialized object of the *useful* class, set its property to the file
   you want, base64-encode it, and send it.

## Submit

```bash
../../check.sh w2-objinj 'labcs2{...}'
```

> `check.sh` is a **self-check**, not a scoreboard: it only confirms your exploit worked.
> The points are for your write-up below, not the flag.

## For your write-up

- Why is `unserialize()` on user input dangerous even when you "only" expected one class?
- What is the correct fix? (Hint: for carrying data, what format should you use instead,
  and which function replaces `unserialize` here?)
