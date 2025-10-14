#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include <time.h>

void *mythread(void *arg) {
    printf("%s\n", (char *) arg);
    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t p1, p2;
    int rc;

    printf("main: begin\n");

    // Create two threads
    pthread_create(&p1, NULL, mythread, "A");
    pthread_create(&p2, NULL, mythread, "B");

    // join waits for threads to finish
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);

    printf("main: end\n");
    
    return 0;
}
