FROM python:3.9-slim

# Install gcc, clang, gdb, make, binutils
RUN apt-get update && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends \
        gcc clang gdb make binutils libc6-dev && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY requirements.txt .
RUN pip install -r requirements.txt

# Copy demo1 (C++ buffer overflow demo)
COPY demo1/ /demo1/

# Copy demo2 sources, GDB script, and build script
COPY demo2/ /demo2/

COPY demo4/web/app.py ./
COPY demo4/web/templates ./templates

RUN chmod +x /demo2/scripts/build_and_run.sh

# Pre-compile demo1
RUN clang++ /demo1/demo.cpp -o /demo1/demo

# Pre-compile demo2 binary with debug symbols (gdwarf-4, no stack protector)
RUN mkdir -p /demo2/bin && \
    gcc -g -gdwarf-4 -fno-stack-protector -O0 \
        -o /demo2/bin/01_overflow /demo2/src/01_basic_overflow.c

EXPOSE 5000

# Run demo1, then demo2, then start the web app
CMD ["/bin/bash", "-c", \
    "echo '=== DEMO 1: Buffer Overflow (C++) ===' && \
    /demo1/demo 'HELLO' && \
    echo '' && \
    /demo1/demo 'AAAAAAAAAAAAAAAAAAAA' --vuln-only || true && \
    echo '' && \
    echo '=== DEMO 2.1 and 2.2 ===' && \
    /demo2/scripts/build_and_run.sh || true; \
    echo '' && \
    echo '=== DEMO 2.3 GDB: Live Stack Variable Overwrite ===' && \
    gdb -batch -x /demo2/gdb_demo1_stack_overwrite.gdb /demo2/bin/01_overflow || true; \
    echo '' && \
    echo '=== DEMO 4: Web Application: Open the http://localhost:5000/ to check demo4 ===' && \
    python app.py"]
