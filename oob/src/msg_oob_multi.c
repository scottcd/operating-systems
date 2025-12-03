// msg_oob_multi.c
// Demonstrate that only one pending MSG_OOB byte is treated as urgent.
// Older OOB data becomes normal in-band data.

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

static void die(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main(void) {
    int socks[2];
    char buf[64];
    ssize_t n;

    if (socketpair(AF_UNIX, SOCK_STREAM, 0, socks) < 0) {
        die("socketpair");
    }

    printf("=== Scenario 2: Multiple MSG_OOB sends ===\n");

    char normalA = 'A';
    char normalB = 'B';
    char oob1 = 'X';
    char oob2 = 'Y';

    // Sender side:
    if (send(socks[1], &normalA, 1, 0) < 0) die("send A");
    if (send(socks[1], &oob1, 1, MSG_OOB) < 0) die("send OOB X");
    if (send(socks[1], &oob2, 1, MSG_OOB) < 0) die("send OOB Y");
    if (send(socks[1], &normalB, 1, 0) < 0) die("send B");

    printf("Sent sequence: normal 'A', OOB 'X', OOB 'Y', normal 'B'\n");

    // Receiver: first get the urgent byte
    memset(buf, 0, sizeof(buf));
    n = recv(socks[0], buf, 1, MSG_OOB);
    if (n < 0) {
        printf("recv(MSG_OOB) failed: %s\n", strerror(errno));
    } else {
        printf("recv(MSG_OOB): got '%c' (most recent OOB)\n", buf[0]);
    }

    // Then read the rest of the stream normally
    printf("Now reading remaining stream via recv()...\n");
    for (;;) {
        memset(buf, 0, sizeof(buf));
        n = recv(socks[0], buf, sizeof(buf) - 1, 0);
        if (n < 0) die("recv");
        if (n == 0) {
            printf("recv(): EOF\n");
            break;
        }
        printf("recv(): got %zd bytes: ", n);
        for (ssize_t i = 0; i < n; i++) {
            printf("'%c' ", buf[i]);
        }
        printf("\n");
    }

    close(socks[0]);
    close(socks[1]);
    return 0;
}

