// msg_oob_basic.c
// Demonstrate basic MSG_OOB behavior on AF_UNIX SOCK_STREAM sockets.

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

    printf("=== Scenario 1: Basic MSG_OOB ===\n");

    const char *normal1 = "hello";
    const char *normal2 = "world";
    const char *normal3 = "!!!";
    char oob = 'X';

    // Sender side
    if (send(socks[1], normal1, strlen(normal1), 0) < 0) die("send normal1");
    if (send(socks[1], normal2, strlen(normal2), 0) < 0) die("send normal2");
    if (send(socks[1], &oob, 1, MSG_OOB) < 0) die("send oob");
    if (send(socks[1], normal3, strlen(normal3), 0) < 0) die("send normal3");

    printf("Sent: \"%s\" + \"%s\" + OOB '%c' + \"%s\"\n",
           normal1, normal2, oob, normal3);

    // Receiver side: first, try to read the urgent byte
    memset(buf, 0, sizeof(buf));
    n = recv(socks[0], buf, 1, MSG_OOB);
    if (n < 0) {
        printf("recv(MSG_OOB) failed: %s\n", strerror(errno));
    } else {
        printf("recv(MSG_OOB): got %zd byte: '%c'\n", n, buf[0]);
    }

    // Then read the rest of the stream normally
    printf("Now reading the rest via normal recv()...\n");
    for (;;) {
        memset(buf, 0, sizeof(buf));
        n = recv(socks[0], buf, sizeof(buf) - 1, 0);
        if (n < 0) die("recv");
        if (n == 0) {
            printf("recv(): EOF\n");
            break;
        }
        printf("recv(): got %zd bytes: \"%s\"\n", n, buf);
    }

    close(socks[0]);
    close(socks[1]);
    return 0;
}

