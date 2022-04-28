#include "threads_services.h"


int station = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
queue product_queue[MAXSTAGES + 2];
sem_t mysem[MAXSTAGES + 1];

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
        sem_wait(&mysem[0]);
        
        if(isEmpty(&product_queue[5]) == 1)
        {
            sem_post(&mysem[0]);
            continue;
        }
        
        struct product_record record;
        dequeue(&product_queue[0], &record);
        
        if(record.stations[0] == -1)
        {
            enqueue(&product_queue[1], &record);
            sem_post(&mysem[0]);
            break;
        }
        writeRecord(fp, &record);
        sem_post(&mysem[0]);
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