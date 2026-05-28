#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SIZE 10

// Function that should NOT be executed normally
// void secret_function() {
//     printf("⚠️  Secret function reached! (Simulating malicious execution)\n");
// }

// ❌ Vulnerable function — no bounds checking
void vulnerable(const char *input) {
    char buffer[BUFFER_SIZE];

    printf("\n[Running Vulnerable Case]\n");
    printf("Input  : \"%s\" (length: %zu)\n", input, strlen(input));
    printf("Buffer size: %d\n", BUFFER_SIZE);

    // Unsafe copy — overflows if input > BUFFER_SIZE
    strcpy(buffer, input);

    printf("Buffer content: \"%s\"\n", buffer);
}

// ✅ Safe function — bounds-checked copy
void safe(const char *input) {
    char buffer[BUFFER_SIZE];

    printf("\n[Running Safe Case]\n");
    printf("Input  : \"%s\" (length: %zu)\n", input, strlen(input));
    printf("Buffer size: %d\n", BUFFER_SIZE);

    strncpy(buffer, input, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';  // Guarantee null termination

    printf("Buffer content: \"%s\"%s\n", buffer,
           strlen(input) >= BUFFER_SIZE ? "  [truncated]" : "");
}

static void print_usage(const char *prog) {
    fprintf(stderr, "Usage: %s <input_string> [--safe-only | --vuln-only]\n", prog);
    fprintf(stderr, "  --safe-only   Run only the safe demo\n");
    fprintf(stderr, "  --vuln-only   Run only the vulnerable demo\n");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    const char *input = argv[1];
    const char *mode  = argc >= 3 ? argv[2] : NULL;

    int run_vuln = (mode == NULL || strcmp(mode, "--safe-only") != 0);
    int run_safe = (mode == NULL || strcmp(mode, "--vuln-only") != 0);

    printf("=== Buffer Overflow Demo ===");

    if (run_vuln) vulnerable(input);
    if (run_safe) safe(input);

    printf("\nProgram finished.\n");
    return EXIT_SUCCESS;
}

//to run above code, use the following commands in terminal:

/*
clang++ demo1/demo.cpp -o demo1/demo && ./demo1/demo "HELLO"                         # run both demos
clang++ demo1/demo.cpp -o demo1/demo && ./demo1/demo "HELLO" --safe-only             # safe demo only
clang++ demo1/demo.cpp -o demo1/demo && ./demo1/demo "AAAAAAAAAAAAAAAAAAAA" --vuln-only  # overflow demo (will crash)
*/
