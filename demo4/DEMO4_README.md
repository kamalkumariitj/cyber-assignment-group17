# Demo 4 — Buffer Overflow Attack on a TCP Server

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
