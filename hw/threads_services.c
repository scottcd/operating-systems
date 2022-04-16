#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "threads_services.h"
#include "station_services.h"
#include "thread_structs.h"
#include "queue.h"
#include <semaphore.h>


int station = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
queue product_queue[MAXSTAGES + 2];
sem_t mysem[MAXSTAGES + 1];

void waitSem6 ()
{
    sem_wait(&mysem[6]);
}
void postSem6 ()
{
    sem_post(&mysem[6]);
}


void initializeSemsAndQueues()
{
    for (int i = 0; i < MAXSTAGES + 2; i++)
    {
        sem_init(&mysem[i], 0, 1);
    }
    for (int i = 0; i < MAXSTAGES + 1; i++)
    {
        product_queue[i] = *(createQueue(sizeof(struct product_record)));
    }
    
}

void *readFiles(void *args)
{
    sem_wait(&mysem[0]);
    struct accessFile_struct *af = args;
    
    accessFile(af->fileName, 0, af->records);
    
    //printf("%s %d\n", af->fileName, af->records[0].idnumber);
    
    for (int i = 0; i < getRecordCount(); i++)
    {
        enqueue(&product_queue[0], &(af->records[i]));
    }
    struct product_record rec = createLastProductRecord();
    enqueue(&product_queue[0], &rec);
    sem_post(&mysem[0]);

    pthread_exit(0);
}

void *runStation(void *args)
{
    pthread_mutex_lock(&mutex);
    // run here
    switch (station)
    {
        case 0:
        station0(mysem, product_queue);
        break;
        case 1:
        station1(mysem, product_queue);
        break;
        case 2:
        station2(mysem, product_queue);
        break;
        case 3:
        station3(mysem, product_queue);
        break;
        default:
        station4(mysem, product_queue);
        break;
    }
    station++;
    pthread_mutex_unlock(&mutex);

    pthread_exit(0);
}

void *writeFiles(void *args)
{
    char *fileName = args;
    FILE *fp;
    char path[20] = "./order-files/";
    strcat(path, fileName);
    fp = fopen(path, "w");

    // poll until our queue is written to
    while(isEmpty(&product_queue[5]) == 1)
    {
        ; 
    }
    sem_wait(&mysem[5]);
    struct product_record temp;
    
    for (int i = 0; i < getRecordCount(); i++)
    {
        dequeue(&product_queue[5], &temp);
        writeRecord(fp, &temp);
    }
    pthread_exit(0);
}

// Create the first thread to read our records.
void createReadThread(pthread_t *tid, char* fileName, struct product_record records[])
{
    accessFile_struct *args = malloc(sizeof *args);
    args->fileName = fileName;
    memcpy(args->records, records, sizeof(struct product_record));
    
    pthread_create(&*(tid), NULL, readFiles, args);
}

// Create our 5 station threads.
void createStationThreads(pthread_t *tid)
{
    pthread_create(&*(tid), NULL, runStation, NULL);
}

// Create the final thread to write our records.
void createWriteThread(pthread_t *tid, char* fileName)
{
    pthread_create(&*(tid), NULL, writeFiles, fileName);
}


struct product_record createLastProductRecord(){
    struct product_record record;
    for (int i = 0; i < MAXSTAGES; i++)
    { 
        record.stations[i] = -1;
    }

    return record;
}