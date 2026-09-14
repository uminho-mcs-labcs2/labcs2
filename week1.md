# Week 1 — Break crypto, explain the failure

Work in **groups of two**, solve **two related [Cryptopals challenges](https://cryptopals.com/)**, and present your work to the class in **eight minutes, sharp**.

**Assigned:** 14 September 2026. **Presentations:** 21 September 2026.

## Choose your challenges

Choose **one challenge** from the table and register your group and choices with the teacher.

| Option | Cryptopals challenge | Topic | Difficulty |
| --- | --- | --- | --- |
| 1 | [12: Byte-at-a-time ECB decryption](https://cryptopals.com/sets/2/challenges/12) | ECB secret recovery | Standard |
| 2 | [13: ECB cut-and-paste](https://cryptopals.com/sets/2/challenges/13) | ECB profile forgery | Standard |
| 3 | [14: ECB decryption with an unknown prefix](https://cryptopals.com/sets/2/challenges/14) | ECB secret recovery with an unknown prefix | Demanding |
| 4 | [16: CBC bitflipping](https://cryptopals.com/sets/2/challenges/16) | CBC ciphertext tampering | Standard |
| 5 | [17: CBC padding oracle](https://cryptopals.com/sets/3/challenges/17) | CBC padding oracle | Advanced |
| 6 | [19: Fixed-nonce CTR, substitutions](https://cryptopals.com/sets/3/challenges/19) | CTR keystream reuse: manual recovery | Demanding |
| 7 | [20: Fixed-nonce CTR, statistics](https://cryptopals.com/sets/3/challenges/20) | CTR keystream reuse: statistical recovery | Demanding |
| 8 | [22: Crack an MT19937 seed](https://cryptopals.com/sets/3/challenges/22) | Time-based random seeds | Demanding |
| 9 | [23: Clone MT19937](https://cryptopals.com/sets/3/challenges/23) | Cloning a random generator | Demanding |
| 10 | [24: Break an MT19937 stream cipher](https://cryptopals.com/sets/3/challenges/24) | Weak seeds in encryption and reset tokens | Demanding |
| 11 | [25: CTR edit API attack](https://cryptopals.com/sets/4/challenges/25) | Plaintext recovery through a CTR edit API | Standard |
| 12 | [26: CTR bitflipping](https://cryptopals.com/sets/4/challenges/26) | CTR ciphertext tampering | Standard |
| 13 | [27: CBC with IV=Key](https://cryptopals.com/sets/4/challenges/27) | CBC key recovery through error messages | Standard |
| 14 | [29: SHA-1 length extension](https://cryptopals.com/sets/4/challenges/29) | SHA-1 message authentication forgery | Advanced |
| 15 | [30: MD4 length extension](https://cryptopals.com/sets/4/challenges/30) | MD4 message authentication forgery | Advanced |
| 16 | [34: Diffie–Hellman parameter injection](https://cryptopals.com/sets/5/challenges/34) | Diffie–Hellman parameter injection | Advanced |
| 17 | [35: Malicious Diffie–Hellman generators](https://cryptopals.com/sets/5/challenges/35) | Diffie–Hellman generator manipulation | Advanced |
| 18 | [37: SRP zero-key attack](https://cryptopals.com/sets/5/challenges/37) | SRP authentication bypass | Advanced |
| 19 | [38: Dictionary attack on simplified SRP](https://cryptopals.com/sets/5/challenges/38) | Password recovery from simplified SRP | Advanced |
| 20 | [40: RSA broadcast attack](https://cryptopals.com/sets/5/challenges/40) | RSA broadcast plaintext recovery | Advanced |
| 21 | [41: Unpadded RSA recovery oracle](https://cryptopals.com/sets/6/challenges/41) | Unpadded RSA plaintext recovery | Advanced |
| 22 | [43: DSA key recovery from nonce](https://cryptopals.com/sets/6/challenges/43) | DSA key recovery from a weak nonce | Advanced |
| 23 | [44: DSA repeated nonce](https://cryptopals.com/sets/6/challenges/44) | DSA key recovery from nonce reuse | Advanced |
| 24 | [46: RSA parity oracle](https://cryptopals.com/sets/6/challenges/46) | RSA plaintext recovery from parity feedback | Advanced |


## Working rules

- Use a programming language you know (suggestion: Python). You may use libraries or cited reference code for cryptographic primitives and prerequisite helpers; Implement and **understand** the selected attacks.
- Run everything **locally**. For example, protocol exchanges and servers may be simulated with functions.
- Check your results and show a saved example run. Explain **any incomplete result honestly**.
- Cite external code, write-ups, and AI assistance. AI tools are allowed, but everyone in the group must understand both solutions.

## What to prepare for next week:

Bring **slides and runnable code** for both challenges, with a short README containing group members, run instructions, and references. Have these ready 18th of September, end of day.

Your presentation (designed for 8 minutes sharp) should explain:

1. The problem and its security implications.
2. How it works, with results from your implementation.
3. Quick demo.

The **eight-minute limit covers both presentation and any demo**. All members of the group must speak. As such, it is recommended to rehearse/use short examples/and keep saved output available if a live demo fails. There will be up to 2 to 5 minutes for questions from the students afterwards. 

## Presentation assessment

We will assess three things:

- **Understanding:** can you explain why the attacks work and answer questions?
- **Results:** can you show what your code achieves and explain any limitations?
- **Clarity:** can classmates follow your explanation within eight minutes, with everyone participating?


