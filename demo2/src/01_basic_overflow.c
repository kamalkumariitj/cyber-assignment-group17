/*
 * Demo 1: Basic Stack Buffer Overflow
 *
 * This program has a local variable `is_admin` set to 0 (false) and the code checks its value to grant access.
 * A buffer of 8 bytes sits next to it on the stack.
 * By writing more than 8 bytes into the buffer, we overflow into `is_admin` and change its value — granting "admin" access without actually setting it to 1.
 * 
 * MEMORY LAYOUT (simplified, stack grows downward):
 *
 *   Higher addresses
 *      is_admin (4B)    ← gets overwritten by overflow
 *      buffer[8]        ← we write past this
 
 *   Lower addresses
 *
 * Compile: gcc -o 01_basic_overflow 01_basic_overflow.c -fno-stack-protector -zexecstack 2>/dev/null || \
 *          gcc -o 01_basic_overflow 01_basic_overflow.c -fno-stack-protector
 */

#include <stdio.h>
#include <string.h>

void check_access(const char *input) {
    int is_admin = 0;       /* control variable — should stay 0 */
    char buffer[8];         /* small buffer, only 8 bytes */

    printf("\n=== Before copy ===\n");
    printf("  buffer address : %p\n", (void *)buffer);
    printf("  is_admin addr  : %p\n", (void *)&is_admin);
    printf("  distance       : %ld bytes\n", (long)((char *)&is_admin - buffer));
    printf("  is_admin value : 0x%08x (%d)\n", is_admin, is_admin);

    /* VULNERABLE: strcpy does NOT check length */
    strcpy(buffer, input);

    printf("\n=== After copy ===\n");
    printf("  buffer content : \"%s\"\n", buffer);
    printf("  is_admin value : 0x%08x (%d)\n", is_admin, is_admin);

    if (is_admin != 0) {
        printf("\n  [!] ACCESS GRANTED — is_admin was overwritten!\n");
        printf("  [!] The overflow changed is_admin from 0 to %d\n", is_admin);
    } else {
        printf("\n  [*] Access denied. is_admin is still 0.\n");
    }
}

int main(int argc, char *argv[]) {
    printf("=== Buffer Overflow Demo 2.1: Stack Variable Overwrite ===\n");

    if (argc < 2) {
        printf("\nUsage: %s <input_string>\n", argv[0]);
        printf("\nTry these:\n");
        printf("  %s AAAA            (safe: fits in 8-byte buffer)\n", argv[0]);
        printf("  %s AAAAAAAAAAAAA   (overflow: 13 bytes > 8-byte buffer)\n", argv[0]);
        return 1;
    }

    printf("Input length: %lu bytes\n", strlen(argv[1]));
    check_access(argv[1]);
    return 0;
}
