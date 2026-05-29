#!/bin/bash
# Build and run all buffer overflow demos
# Disables stack protector so the overflow is visible (for educational purposes)

set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_DIR="$(dirname "$SCRIPT_DIR")"
SRC_DIR="$PROJECT_DIR/src"
BIN_DIR="$PROJECT_DIR/bin"

mkdir -p "$BIN_DIR"

echo "===================================================================================DEMO2============================================================================================="
echo " Building Buffer Overflow Demos"
echo " WARNING: These are intentionally vulnerable!"
echo "============================================"
echo ""

#Important for demo to run correctly:
#   -fno-stack-protector  : disable canaries so overflow works
#   -Wno-deprecated-declarations : suppress gets() warnings
#   -D_FORTIFY_SOURCE=0  : disable glibc overflow detection

CFLAGS="-fno-stack-protector -Wno-deprecated-declarations -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=0 -O0 -g"

echo "[1/3] Building 01_basic_overflow..."
gcc $CFLAGS -o "$BIN_DIR/01_basic_overflow" "$SRC_DIR/01_basic_overflow.c"

echo "[2/3] Building 02_func_ptr_overwrite..."
gcc $CFLAGS -o "$BIN_DIR/02_func_ptr_overwrite" "$SRC_DIR/02_return_address_overwrite.c"

echo ""
echo "Build complete! Binaries in $BIN_DIR/"
echo ""

echo "============================================"
echo " DEMO 2.1: Stack Variable Overwrite"
echo "============================================"

echo ""
echo "--- Test A: Safe input (4 bytes) ---"
"$BIN_DIR/01_basic_overflow" "AAAA"

echo ""
echo "--- Test B: Overflow input (20 bytes) ---"
"$BIN_DIR/01_basic_overflow" "AAAAAAAAAAAAAAAAAAAAA" || echo "  (program may crash — that's expected with overflow!)"

echo ""
echo "============================================"
echo " DEMO 2.2: Function Pointer Hijack"
echo "============================================"
"$BIN_DIR/02_func_ptr_overwrite"
