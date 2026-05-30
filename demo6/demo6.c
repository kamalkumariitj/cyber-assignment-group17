#define _CRT_SECURE_NO_WARNINGS   // Prevents Visual Studio warning messages
#include <stdio.h>    // Standard input/output functions
#include <stdlib.h>   // Standard library functions
#include <string.h>   // String handling functions

// Function that contains the format string vulnerability
void vulnerable_print(const char *user_input) {

    /*
        Fake sensitive information stored in memory.

        This simulates real-world secrets like:
        - passwords
        - API keys
        - authentication tokens

        Attackers may try to leak this memory using format string attacks.
    */
    char secret_api_key[] = "SECRET_FLAG{FOR_CYBER_PROJECT}";

    // Buffer to store user input safely
    char buffer[100];

    /*
        Safely copies user input into the buffer.

        Prevents buffer overflow attacks by limiting copied characters.
    */
    strncpy(buffer, user_input, sizeof(buffer) - 1);

    // Ensures the string ends properly
    buffer[sizeof(buffer) - 1] = '\0';

    // Display heading
    printf("\n--- Printing Data ---\n");

    /*
        ================================
           FORMAT STRING VULNERABILITY
        ================================

         Vulnerable Code:
            printf(buffer);

        Problem:
        The user input is directly passed to printf()
        as the format string.

        This allows attackers to inject format specifiers like:
            %x  -> Read stack memory
            %s  -> Read memory as string
            %n  -> Write to memory

        Possible consequences:
        - Memory leakage
        - Program crashes
        - Information disclosure
        - Arbitrary memory modification
    */

    // Simulated crash demo for %n attack
    if (strstr(buffer, "%n") != NULL) {
        printf("\n[!] Segmentation Fault Detected!\n");
        printf("[!] Program crashed due to invalid memory write.\n");
        exit(1);
    }

    // Format String Vulnerability
    printf(buffer);

    // Display footer
    printf("\n---------------------\n");
}

int main() {

    // Buffer to store keyboard input
    char input[200];

    // Ask the user for input
    printf("Enter your input: ");

    /*
        Reads user input safely from keyboard.

        fgets() prevents buffer overflow by limiting input size.
    */
    fgets(input, sizeof(input), stdin);

    // Remove newline character added by fgets()
    input[strcspn(input, "\n")] = '\0';

    // Send input to vulnerable function
    vulnerable_print(input);

    return 0;
}
