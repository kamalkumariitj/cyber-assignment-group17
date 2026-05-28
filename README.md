# Buffer Overflow Lab

A small educational cybersecurity project that demonstrates how a fixed-size buffer can behave when it receives normal input, boundary input, and oversized input. The Flask web app shows the behavior in a browser, the Python script in `demo3/exploit/` drives the web demo from the command line, and the C++ program in `demo1/` shows a classic stack buffer overflow example.

## What It Demonstrates

- Normal request handling
- Boundary conditions around a fixed-size buffer
- Overflow-style behavior when input exceeds the buffer length
- Service instability / denial-of-service style behavior under aggressive input and request load
- Safe stack overflow presentation in a small C++ demo

This project is intended for a controlled classroom or lab environment only.

## Project Structure

```text
.
├── Dockerfile
├── docker-compose.yml
├── requirements.txt
├── demo1/
│   └── demo.cpp
└── demo3/
    ├── exploit/
    │   └── exploit.py
    └── web/
        ├── app.py
        ├── Dockerfile
        ├── requirements.txt
        └── templates/
            └── index.html
```

## Requirements

- Docker and Docker Compose
- Python 3 if you want to run the exploit script locally

## Run the Web App

From the project root:

```bash
docker compose up --build -d
```

This starts the Flask application on:

```text
http://localhost:5000
```

To stop it:

```bash
docker compose down
```

## Run the Web Demo Script

First make sure the web app is running, then run:

```bash
python3 demo3/exploit/exploit.py
```

The script has two phases:

1. It sends normal, boundary, and oversized inputs.
2. It performs heavier request load to demonstrate service instability.

## Run the C++ Demo

The stack overflow example lives in `demo1/demo.cpp`. Compile it with:

```bash
clang++ demo1/demo.cpp -o demo1/demo
```

Then run one of these examples:

```bash
./demo1/demo "HELLO"
./demo1/demo "HELLO" --safe-only
./demo1/demo "HELLO" --secret
```

The `--secret` option is a safe presentation shortcut that calls the hidden demo function directly without relying on undefined behavior.

## Web App Details

The Flask app in `demo3/web/app.py` uses a small fixed buffer and intentionally unsafe demo logic to simulate overflow behavior. It also includes resource pressure and crash conditions so the effects are easier to observe in a short classroom demo.

## Notes

- The application is intentionally not production-safe.
- The demo is designed to be easy to explain during a presentation.
- If the service crashes during the aggressive phase, restart it with:

```bash
docker compose restart
```

## Credits

Assignment Group 18

- Chaurasia Kamalkumar Lallanprasad - G25AIT2028
- Anurag Vishwakarma - G25AIT2017
- Palagani Sai Chaitanya - G25AIT2143
- Niketh Varma Tirumalaraju - G25AIT2142
- Nalla Sai Revanth - G25AIT2141
- Anmol Pandey - G25AIT2013

## License

For academic use in the course assignment context.
