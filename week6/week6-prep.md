# Initial prep for the week 6 session

The week 6 session is built around a **Docker** lab.

## Required software

- [Docker Engine](https://docs.docker.com/engine/install/) **and**
  [Docker Compose](https://docs.docker.com/compose/install/) (the `docker compose`
  plugin already ships with recent Docker Desktop / Docker Engine versions).
- [`curl`](https://curl.se) (to interact with the web services).
- For running the C memory demos yourself: a Linux or WSL2 (Windows Subsystem for Linux 2) environment with `gcc` and the AddressSanitizer runtime. On Debian/Ubuntu, install them with
  `sudo apt-get install build-essential libasan8`. If you only watch the instructor's
  demos, this compiler setup is optional. The PHP lab images do not install it for you.

> [!NOTE]
> On **Windows**, use [WSL2](https://learn.microsoft.com/en-us/windows/wsl/install)
> with Docker Desktop configured for the WSL2 *backend*. On **Linux/macOS** the
> install is straightforward.

## Verify the environment

```bash
docker --version
docker compose version
curl --version
# If running the memory demos yourself:
gcc --version
```

## Pull the images ahead of time (recommended)

The base images are the official PHP ones.

```bash
docker pull php:8.3.35-apache   # affected version (lab)
docker pull php:8.3.36-apache   # fixed version (comparison)
docker pull nginx:1.27-alpine  # proxy for E2
```

> [!IMPORTANT]
> The exact *tags* may need adjusting depending on *registry* availability on the day
> of class. What matters for the lab is the **pair**: a version **prior** to the fix
> and the matching **fixed** version (see `week6.md`). If `8.3.35` is no longer
> available, any `8.3.x` before `8.3.36` works as the "vulnerable" one for teaching
> purposes.

## Start the lab (smoke test)

From the `week6/lab/` directory:

```bash
docker compose up -d --build --wait
```

Two services should become available:

- **Vulnerable app:** <http://localhost:8080>
- **Fixed app:** <http://localhost:8081>

The `-d` option leaves services running so you can issue requests in the same
terminal. HTTP health checks make `--wait` wait until Apache serves requests.
Check both URLs with `curl` or a browser. If startup fails, inspect
`docker compose ps` and `docker compose logs`.

If both URLs respond with the sample application page, the main lab setup is ready.
If running the memory demos, also compile and run them using the
[demo guide](lab/mem-classes/README.md); the expected outcome is an error report/crash.

After checking the services, stop and clean up:

```bash
docker compose down
```
