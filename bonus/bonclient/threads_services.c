#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "threads_services.h"
#include "thread_structs.h"
#include "queue.h"
#include <semaphore.h>


int station = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
queue product_queue[MAXSTAGES + 2];
sem_t mysem[MAXSTAGES + 1];
int sockdesc = -1;
int connection= -1;

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
    struct accessFile_struct *af = args;
    struct product_record record;
    
    accessFile(af->fileName, 0, af->records);
    for (int i = 0; i < getRecordCount(); i++)
    {
        sem_wait(&mysem[0]);
        enqueue(&product_queue[0], &(af->records[i]));
        sem_post(&mysem[0]);
    }

    record = createLastProductRecord();
    sem_wait(&mysem[0]);
    enqueue(&product_queue[0], &record);
    sem_post(&mysem[0]);

    pthread_exit(0);
}

void *sendRecords(void *args)
{
    struct writeSocket_struct *af = args;
    // connect to socket
    struct addrinfo* myinfo;
    
    
    // poll until our queue is written to
    while(1)
    {
        
        sem_wait(&mysem[0]);
        sem_wait(&mysem[1]);
        
        if(isEmpty(&product_queue[0]) == 1)
        {
            sem_post(&mysem[1]);
            sem_post(&mysem[0]);
            continue;
        }
       
        struct product_record record, rcv_record;
        dequeue(&product_queue[0], &record);
        
        // set up socket connection
        sockdesc = socket(AF_INET, SOCK_STREAM, 0);
        getaddrinfo(af->serverName, af->serverPort, NULL, &myinfo);
        
        connection = connect(sockdesc, myinfo->ai_addr, myinfo->ai_addrlen);
        
        printf("Sending %s to socket %d\n",record.name, sockdesc);
        send(sockdesc, &record, sizeof(struct product_record), 0);
        
        read(sockdesc, &rcv_record, sizeof(struct product_record));
        printf("Reading %s from socket %d\n", record.name, sockdesc);
        enqueue(&product_queue[1], &rcv_record);

        sem_post(&mysem[0]);
        sem_post(&mysem[1]);
        
        if(record.stations[0] == -1)
        {
            break;
        }
    }
    
    close(sockdesc);
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
    while(1)
    {
        sem_wait(&mysem[1]);
        if(isEmpty(&product_queue[1]) == 1)
        {
            sem_post(&mysem[1]);
            continue;
        }
        
        struct product_record record;
        dequeue(&product_queue[1], &record);
        
        if(record.stations[0] == -1)
        {
            sem_post(&mysem[1]);
            break;
        }
        
        writeRecord(fp, &record);
        sem_post(&mysem[1]);
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

// Create the first thread to read our records.
void createSocketSendThread(pthread_t *tid, char* serverName, char* serverPort)
{
    writeSocket_struct *args = malloc(sizeof *args);
    args->serverName = serverName;
    args->serverPort = serverPort;
    
    pthread_create(&*(tid), NULL, sendRecords, args);
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