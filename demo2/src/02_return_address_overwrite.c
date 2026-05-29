/*
 * Demo 2: Function Pointer Overwrite via Buffer Overflow
 *
 * This program demonstrates how an overflow can redirect program execution.
 * A function pointer sits adjacent to a buffer on the stack.
 * Overflowing the buffer overwrites the function pointer,
 * causing the program to call a different function.
 *
 * This is a simplified model of how real return-address overwrites work.
 *
 * To Compile: gcc -o 02_func_ptr_overwrite 02_return_address_overwrite.c -fno-stack-protector
 */

#include <stdio.h>
#include <string.h>

void normal_function(void) {
    printf("  [*] normal_function() called — this is expected.\n");
}

void secret_function(void) {
    printf("\n  ------------------------------------------------- \n");
    printf("  --  SECRET FUNCTION REACHED VIA OVERFLOW!    --\n");
    printf("  --  In a real attack, this could be:        --\n");
    printf("  --    - spawn a shell                        --\n");
    printf("  --    - disable security checks              --\n");
    printf("  --    - execute arbitrary code (shellcode)   --\n");
    printf("  ------------------------------------------------- \n\n");
}

/*
 * The key to this exploit: buffer[] and callback sit ADJACENT in memory.
 * buffer is 16 bytes. callback is a function pointer (8 bytes on 64-bit).
 * Because they are declared sequentially in the struct, the compiler lays
 * them out contiguously — callback starts at buffer + 16.
 * Writing more than 16 bytes into buffer spills over into callback.
 */
struct vulnerable_struct {
    char buffer[16];          /* 16-byte buffer — our overflow target       */
    void (*callback)(void);   /* sits at buffer+16; overwritten by overflow */
};

void demo_function_pointer_overwrite(void) {
    struct vulnerable_struct vs;
    vs.callback = normal_function;

    /* Print addresses so you can see the layout in memory at runtime.
     * The distance between buffer and callback should always be exactly
     * 16 bytes — confirming no padding was inserted between them. */
    printf("\n=== Initial state ===\n");
    printf("  buffer addr    : %p\n", (void *)vs.buffer);
    printf("  callback addr  : %p\n", (void *)&vs.callback);
    printf("  distance       : %ld bytes\n", (long)((char *)&vs.callback - vs.buffer));
    printf("  callback points to normal_function: %p\n", (void *)(unsigned long)normal_function);
    printf("  secret_function is at             : %p\n", (void *)(unsigned long)secret_function);

    /* Legitimate call — callback still points to normal_function. */
    printf("\n=== Calling callback normally ===\n");
    vs.callback();

    printf("\n=== Now overwriting buffer with overflow... ===\n");

    /*
     * Craft the overflow payload:
     *
     *  Offset 0..15  — 16 'A' bytes that exactly fill buffer[].

     *  Offset 16..23 — the raw 8-byte address of secret_function.
     *                  These bytes land on top of callback because
     *                  callback lives at buffer+16 in the struct.
     *
     *  [ A A A A A A A A A A A A A A A A | <8-byte ptr to secret_function> ]
     *  |<-------- buffer[16] ---------->| |<-------- callback ----------->|
     */
    unsigned char payload[16 + sizeof(void *)];
    memset(payload, 'A', 16);                       /* fill the buffer region  */
    void *target = (void *)secret_function;
    printf("  Crafted payload: [ %.*s | %p ]\n", 16, payload, target);
    memcpy(payload + 16, &target, sizeof(void *));  /* append the target addr  */

    /* This memcpy copies 24 bytes into a 16-byte buffer — the overflow.
     * The 8 bytes past the buffer boundary overwrite callback in-place. */
    memcpy(vs.buffer, payload, sizeof(payload));


    /* callback no longer points to normal_function \u2014 it was overwritten.
     * It now holds the address of secret_function. */
    printf("  callback now points to: %p\n", (void *)(unsigned long)vs.callback);
    printf("\n=== Calling callback after overflow ===\n");
    /* CPU jumps to whatever address is in callback \u2014 which is now secret_function.
     * In a real exploit this could be shellcode, a system() call, or any gadget. */
    vs.callback();
}

int main(void) {
    printf("=== Buffer Overflow Demo 2.2: Function Pointer Hijack ===\n");
    demo_function_pointer_overwrite();
    return 0;
}

// Why not just payload + 16 = secret_function?
// You can't assign into the middle of a byte array directly. 
// memcpy is the way to write an arbitrary value as raw bytes at a 
// specific memory offset — which is exactly what a real overflow does: it doesn't 
// "assign" anything, it just writes bytes sequentially into memory regardless of what's there.
