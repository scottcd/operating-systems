#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "threads_services.h"
#include "thread_structs.h"


int station = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *readFiles(void *args)
{
    struct accessFile_struct *af = args;
    
    accessFile(af->fileName, 0, af->records);
    printf("%s %d\n", af->fileName, af->records[0].idnumber);
}

void *runStation(void *args)
{
    pthread_mutex_lock(&mutex);
    printf("Station: %d\n", station);
    station++;
    pthread_mutex_unlock(&mutex);
}

void *writeFiles(void *args)
{
    
    printf("Write Station: %d\n", -1);
    
}

// Create the first thread to read our records.
void createReadThread(pthread_t tid, char* fileName, struct product_record records[])
{
    accessFile_struct *args = malloc(sizeof *args);
    args->fileName = fileName;
    memcpy(args->records, records, sizeof(struct product_record));
    
    pthread_create(&tid, NULL, readFiles, args);
}

// Create our 5 station threads.
void createStationThreads(pthread_t tid)
{
    pthread_create(&tid, NULL, runStation, NULL);
}

// Create the final thread to write our records.
void createWriteThread(pthread_t tid)
{
    pthread_create(&tid, NULL, writeFiles, NULL);
}