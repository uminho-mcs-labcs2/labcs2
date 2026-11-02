# Prep for the week 7 session

Week 7 is a hands-on **CTF** session: after a short kickoff you start solving in class.
To spend that time solving rather than installing, complete the setup below **before
class**. Most of it carries over from week 6.

## Required software

- [Docker Engine](https://docs.docker.com/engine/install/) **and**
  [Docker Compose](https://docs.docker.com/compose/install/) - as in week 6; they run the
  three vulnerable services.
- [`curl`](https://curl.se) - to send exactly the HTTP requests the web challenges need.
- Standard shell tools: `grep`, `base64`, `sed` (for the forensics challenge, D1).

### For the P1 stretch challenge (optional but recommended)

- A C compiler: **`gcc`** (ships with the `build-essential` package on Debian/Ubuntu;
  already available inside the lab container).
- **`nc`** (netcat) - to talk to the pwn service by hand.
- Ideally **`pwntools`** (the Python exploitation toolkit you installed back in week 3):
  ```bash
  python3 -m pip install --user pwntools
  ```
  P1 is solvable **without** pwntools (its brief shows a plain `nc` path), but pwntools
  makes the address packing painless.

> [!NOTE]
> On **Windows**, use [WSL2](https://learn.microsoft.com/en-us/windows/wsl/install) with
> Docker Desktop on the WSL2 *backend*; run all of the above inside the WSL2 shell. On
> **Linux/macOS** the install is straightforward.

## Verify the environment

```bash
docker --version
docker compose version
curl --version
gcc --version          # only needed for the P1 stretch
```

## One-time setup (do this before class)

From the `week7/` directory:

```bash
cd week7
./setup.sh                 # generates YOUR random per-challenge flags + verification hashes
docker compose up --build  # builds and starts the three services
```

`./setup.sh` must run **before** `docker compose up` - it creates the `flag.txt` files
the containers mount. Pulling the base images ahead of time avoids congesting the
classroom network. Stop everything with `docker compose down`.

You should then have:

- **W1 - Nettools:** <http://localhost:9001>
- **W2 - Preferences:** <http://localhost:9002>
- **P1 - Dangling:** `nc localhost 9100`
- **D1 - Aftermath:** no service - analyze `challenges/d1-forensics/access.log`

If the two web pages load and the `nc` port answers, you are ready for class. See
[`README.md`](README.md) for how to submit flags and what to hand in.

> [!CAUTION]
> Every target is deliberately vulnerable. Run it **only** on your own machine and never
> expose these ports to a network.
