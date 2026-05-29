/*
 * server.c — Intentionally vulnerable TCP server for educational demo.
 *
 * The function vulnerable_function() copies attacker-controlled bytes into
 * a fixed-size stack buffer using memcpy() with no bounds check, which
 * allows the return address on the stack to be overwritten.
 *
 * Build with:
 *   gcc -m32 -fno-stack-protector -z execstack -no-pie -o server server.c
 *
 * Run in an ISOLATED VM ONLY. Disable ASLR before exploiting:
 *   sudo sysctl -w kernel.randomize_va_space=0
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define LISTEN_PORT  9090
#define BUF_SIZE     100
#define RECV_SIZE    517   /* deliberately much larger than BUF_SIZE */

/* The vulnerable function: classic stack-based buffer overflow.
 * memcpy will happily copy 517 bytes into a 100-byte buffer. */
int vulnerable_function(char *input, int len) {
    char buffer[BUF_SIZE];
    memcpy(buffer, input, len);   /* <-- no bounds check */
    return 1;
}

int main(void) {
    int server_fd, client_fd;
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    char recv_buf[RECV_SIZE];

    /* Print the address of vulnerable_function's buffer to help craft the
     * return address during the demo. In a real attack you would not have
     * this; here it is purely an educational aid. */
    char hint[BUF_SIZE];
    printf("[server] buffer address hint: %p\n", (void *)hint);
    fflush(stdout);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) { perror("socket"); exit(1); }

    int yes = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port        = htons(LISTEN_PORT);

    if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind"); exit(1);
    }
    if (listen(server_fd, 5) < 0) { perror("listen"); exit(1); }

    printf("[server] listening on port %d ...\n", LISTEN_PORT);
    fflush(stdout);

    while (1) {
        client_fd = accept(server_fd, (struct sockaddr *)&addr, &addr_len);
        if (client_fd < 0) { perror("accept"); continue; }

        printf("[server] client connected from %s\n",
               inet_ntoa(addr.sin_addr));
        fflush(stdout);

        int n = recv(client_fd, recv_buf, RECV_SIZE, 0);
        if (n > 0) {
            printf("[server] received %d bytes\n", n);
            fflush(stdout);
            vulnerable_function(recv_buf, n);
        }
        close(client_fd);
    }

    close(server_fd);
    return 0;
}
