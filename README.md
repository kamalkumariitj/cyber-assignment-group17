<img width="389" height="41" alt="image" src="https://github.com/user-attachments/assets/49ca5e12-726a-4126-b1df-b8938e75c03e" /># Buffer Overflow Lab

An educational cybersecurity project demonstrating classic buffer overflow vulnerabilities — stack variable overwrites, function pointer hijacking, and web-layer overflow behaviour — in a controlled classroom environment.

## Credits

**Assignment Group 17**

- **Chaurasia Kamalkumar Lallanprasad** — G25AIT2028
  - Understanding buffer overflow & why it happens
  - Exposure of unwanted data - Demo 1
  - Server attack DoS — Demo 4

- **Anurag Vishwakarma** — G25AIT2017
  - Unauthorized access - Demo 2.1
  - Function pointer hijack - Demo 2.2
  - GDB Debugger - Taking Program Control Demo2.3
  

- **Anmol Pandey** — G25AIT2013
  - Remote code execution — Demo 3

- **Palagani Sai Chaitanya** — G25AIT2143
  - Prevention / Countermeasures
  - Case Study: Heartbleed Bug (OpenSSL, 2014)

- **Niketh Varma Tirumalaraju** — G25AIT2142
  - Case Study: The Morris Worm (1998)

- **Nalla Sai Revanth** — G25AIT2141
  - Case Study: WannaCry Ransomware (2017)



## What It Demonstrates

| Demo | Language | Topic |
|------|----------|-------|
| demo1 | C++ | Safe vs unsafe buffer copy (`strcpy` / `strncpy`) |
| demo2 | C | Stack variable overwrite + function pointer hijack via overflow + Gdb |
| demo3 | C + Python | Real TCP server exploit — overflow the stack, overwrite the return address, land in shellcode |
| demo4 | Python / Flask | Web-layer overflow simulation, service crash & DoS behaviour |

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
│   │   ├── 01_basic_overflow.c          # C stack variable overwrite
│   │   └── 02_return_address_overwrite.c  # C function pointer hijack
│   ├── scripts/
│   │   └── build_and_run.sh   # compiles and runs both C demo2
│   └── gdb_demo1_stack_overwrite.gdb    # GDB script: live byte-by-byte overflow walkthrough
└── demo3/
│   ├── server.c               # vulnerable TCP server (memcpy with no bounds check)
│   ├── exploit.py             # builds and sends the 517-byte overflow payload
│    ├── Makefile               # builds server with stack protector and NX disabled
│    └── DEMO3_README.md        # standalone setup guide for the SEED Labs VM
├── demo4/
│   ├── exploit/
│   │   └── exploit.py         # drives the web demo from the command line
│   └── web/
│       ├── app.py             # Flask app with intentional overflow behaviour
│       └── templates/
│           └── index.html

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
3. Start the **demo4** Flask web app on `http://localhost:5000`

> **demo4** requires a 32-bit SEED Labs Ubuntu VM and can't run inside the same docker container.

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
------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
### Demo 2 — C Stack Overflow & Function Pointer Hijack

```bash
bash demo2/scripts/build_and_run.sh
```

This compiles both C sources and runs:
- **Test A** — safe 4-byte input into an 8-byte buffer
- **Test B** — 20-byte overflow that overwrites `is_admin`
- **Function pointer hijack** — overflow redirects execution to `secret_function()`

#### Watching it happen live in GDB

If you want to actually see the overflow in details, watch `is_admin` change value the exact moment `strcpy` write past the buffer — use the GDB scritp for it.

First build with debug symbols so GDB can see variable names:

```bash
cd demo2
gcc -g -gdwarf-4 -fno-stack-protector -O0 -o bin/01_overflow src/01_basic_overflow.c
```

Then just run:

```bash
gdb -x gdb_demo1_stack_overwrite.gdb bin/01_overflow
```

The script does everything automatically and prints labels so you know what you're looking at. Here's what happens:

1. **Disassembly first** — it shows you the assembly for `check_access` so you can check the `strcpy` call that has zero length checking.

2. **Breakpoint at the top of `check_access`** — the program is run with 13 `A`s as input (the buffer is only 8 bytes, so this will overflow).

3. **Before the copy** — `info locals` confirms `is_admin` is still 0, and a raw memory dump shows `buffer` and `is_admin` sitting right next to each other on the stack.

4. **Watchpoint on `is_admin`** — GDB will halt execution the instant any byte of `is_admin` changes.

5. **Watchpoint fires** — GDB stops in mid and shows you the old value vs the new value. You can see exactly which bytes `strcpy` wrote when it overflowed. `info locals` and the memory dump both confirm `is_admin` is no longer 0.



6. **GDB as the attacker** — the last section  it resets `is_admin` = 0 and then sets it to `0xDEADBEEF` directly, no overflow needed. The point is that once someone has this level of access to a process, the overflow is easy to place.

------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
### Demo 3 — 
# Demo 3 — Buffer Overflow Attack on a TCP Server

**Contributed by:** Anmol Pandey (G25AIT2013)
**Group:** 17
**Role:** Research & Theory + Server Demo

---

## What this demo shows

A vulnerable TCP server written in C that has **no bounds checking** on the data it receives from clients. An attacker sends a crafted 517-byte payload that:

1. Overflows a 100-byte stack buffer
2. Overwrites the saved Return Address on the stack
3. Redirects execution into injected shellcode
4. Opens a shell on the server

---

## Files

| File | Description |
|------|-------------|
| `server.c` | Vulnerable TCP server (intentionally unsafe for demo) |
| `exploit.py` | Python script that builds and sends the payload |
| `Makefile` | Builds the server with protections disabled |
| `README.md` | Full setup and run instructions |

---

## How to run (inside SEED Labs Ubuntu VM)

### Step 1 — Disable ASLR
```bash
sudo sysctl -w kernel.randomize_va_space=0
```

### Step 2 — Build the server
```bash
make
```

### Step 3 — Run the server (Terminal 1)
```bash
./server
```
Note the buffer address hint printed on screen.

### Step 4 — Run the exploit (Terminal 2)
```bash
python3 exploit.py 0xffffd200
```
(Replace address with the one printed by the server + ~0x60 offset)

### Step 5 — Verify
Back in Terminal 1, type:
```bash
id
```
If you see `uid=1000(seed) ...` — the attack succeeded.

---

## Key vulnerability in server.c

```c
void vulnerable_function(char *input, int len) {
    char buffer[100];
    memcpy(buffer, input, len);   // NO BOUNDS CHECK — this is the bug
}
```

`memcpy` copies exactly `len` bytes with no check that `len <= 100`. When the attacker sends 517 bytes, the extra 417 bytes overflow past the buffer and overwrite the Return Address.

---

## Environment

- Ubuntu 20.04 32-bit (SEED Labs VM)
- gcc with stack protector and NX disabled (educational demo only)
- Python 3

> ⚠️ For educational use only. Run inside an isolated VM.

------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
### Demo 4 — Flask Web App

Start the app:

```bash
docker compose up --build -d   # via Docker Compose
# or locally:
pip install -r requirements.txt
python3 demo4/web/app.py
```

Web app runs on `http://localhost:5000`.

Then run the exploit script (app must be running):

```bash
python3 demo4/exploit/exploit.py
```

The script sends:
1. Normal, boundary, and oversized inputs to show overflow behaviour
2. Concurrent requests to demonstrate service instability / DoS

To stop Docker Compose:

```bash
docker compose down
```

## Web App Details

The Flask app in `demo4/web/app.py` uses a small fixed buffer and intentionally unsafe demo logic to simulate overflow behavior. It also includes resource pressure and crash conditions so the effects are easier to observe in a short classroom demo.

## Notes

- Everything here is intentionally unsafe for educational purposes.
- The demos are designed to be short and easy to explain during a presentation.
- If the demo3 Flask service crashes during the overflow, restart it with:

```bash
docker compose restart
```
