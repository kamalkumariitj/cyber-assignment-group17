FROM python:3.9-slim

# Installing gcc and clang++ for running C/C++ code
RUN apt-get update && apt-get install -y --no-install-recommends gcc clang && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY requirements.txt .
RUN pip install -r requirements.txt


# Copy demo1 (C++ buffer overflow demo)
COPY demo1/ /demo1/

# Copy demo2 sources and build script
COPY demo2/ /demo2/

COPY demo3/web/app.py ./
COPY demo3/web/templates ./templates

RUN chmod +x /demo2/scripts/build_and_run.sh

# Pre-compile demo1
RUN clang++ /demo1/demo.cpp -o /demo1/demo

EXPOSE 5000

# Run demo1, then demo2, then start the web app
CMD ["/bin/bash", "-c", \
    "echo '=== DEMO 1: Buffer Overflow (C++) ===' && \
    /demo1/demo 'HELLO' && \
    echo '' && \
    /demo1/demo 'AAAAAAAAAAAAAAAAAAAA' --vuln-only || true && \
    echo '' && \
    /demo2/scripts/build_and_run.sh; \
    python app.py"]
