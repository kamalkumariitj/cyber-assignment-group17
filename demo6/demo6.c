#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void vulnerable_print(const char *user_input) {

    // Fake sensitive data
    char secret_api_key[] = "SECRET_FLAG{FOR_YOUR_CYBER_PROJECT}";

    char buffer[100];

    // Safe copy
    strncpy(buffer, user_input, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';

    printf("\n--- Printing Data ---\n");

    // Vulnerable line
    printf(buffer);

    printf("\n---------------------\n");
}

int main() {

    char input[200];

    printf("Enter your input: ");
    fgets(input, sizeof(input), stdin);

    // Remove newline
    input[strcspn(input, "\n")] = '\0';

    vulnerable_print(input);

    return 0;
}
