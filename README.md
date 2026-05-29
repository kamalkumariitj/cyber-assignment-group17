# Buffer Overflow Lab

An educational cybersecurity project demonstrating classic buffer overflow vulnerabilities — stack variable overwrites, function pointer hijacking, and web-layer overflow behaviour — in a controlled classroom environment.

## Credits

Assignment Group 17

- Chaurasia Kamalkumar Lallanprasad - G25AIT2028
- Anurag Vishwakarma - G25AIT2017
- Palagani Sai Chaitanya - G25AIT2143
- Niketh Varma Tirumalaraju - G25AIT2142
- Nalla Sai Revanth - G25AIT2141
- Anmol Pandey - G25AIT2013

## What It Demonstrates

| Demo | Language | Topic |
|------|----------|-------|
| demo1 | C++ | Safe vs unsafe buffer copy (`strcpy` / `strncpy`) |
| demo2 | C | Stack variable overwrite + function pointer hijack via overflow |
| demo3 | Python / Flask | Web-layer overflow simulation, service crash & DoS behaviour |

This project is intended for a controlled classroom or lab environment only.

## Project Structure

```text
.
├── Dockerfile
├── docker-compose.yml
├── requirements.txt
├── demo1/
│   └── demo.cpp               # C++ safe-vs-unsafe buffer demo
├── demo2/
│   ├── src/
│   │   ├── 01_basic_overflow.c          # stack variable overwrite
│   │   └── 02_return_address_overwrite.c  # function pointer hijack
│   └── scripts/
│       └── build_and_run.sh   # compiles and runs both C demos
└── demo3/
    ├── exploit/
    │   └── exploit.py         # drives the web demo from the command line
    └── web/
        ├── app.py             # Flask app with intentional overflow behaviour
        └── templates/
            └── index.html
```

## Requirements

- Docker and Docker Compose (recommended)
- `clang++` and `gcc` for running demos locally without Docker
- Python 3 for the exploit script or local Flask run

---

## Run Everything via Docker (recommended)

Build and run all demos inside a single container:

```bash
docker build -t cyber-demo .
docker run -p 5000:5000 cyber-demo
```

On startup the container will:
1. Run **demo1** — safe and overflow cases for the C++ buffer demo
2. Run **demo2** — builds and runs the stack variable overwrite and function pointer hijack demos
3. Start the **demo3** Flask web app on `http://localhost:5000`

To stop the container press `Ctrl+C`.

---

## Run Individually (local)

### Demo 1 — C++ Buffer Overflow

```bash
clang++ demo1/demo.cpp -o demo1/demo

./demo1/demo "HELLO"                          # run both safe and vulnerable cases
./demo1/demo "HELLO" --safe-only              # safe demo only
./demo1/demo "AAAAAAAAAAAAAAAAAAAA" --vuln-only  # overflow (will crash — expected)
```

### Demo 2 — C Stack Overflow & Function Pointer Hijack

```bash
bash demo2/scripts/build_and_run.sh
```

This compiles both C sources and runs:
- **Test A** — safe 4-byte input into an 8-byte buffer
- **Test B** — 20-byte overflow that overwrites `is_admin`
- **Function pointer hijack** — overflow redirects execution to `secret_function()`

### Demo 3 — Flask Web App

Start the app:

```bash
docker compose up --build -d   # via Docker Compose
# or locally:
pip install -r requirements.txt
python3 demo3/web/app.py
```

Web app runs on `http://localhost:5000`.

Then run the exploit script (app must be running):

```bash
python3 demo3/exploit/exploit.py
```

The script sends:
1. Normal, boundary, and oversized inputs to show overflow behaviour
2. Concurrent requests to demonstrate service instability / DoS

To stop Docker Compose:

```bash
docker compose down
```

## Web App Details

The Flask app in `demo3/web/app.py` uses a small fixed buffer and intentionally unsafe demo logic to simulate overflow behavior. It also includes resource pressure and crash conditions so the effects are easier to observe in a short classroom demo.

## Notes

- The application is intentionally not production-safe.
- The demo is designed to be easy to explain during a presentation.
- If the service crashes during the aggressive phase, restart it with:

```bash
docker compose restart
```
