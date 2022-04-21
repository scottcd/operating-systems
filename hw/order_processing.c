/*
*  Chandler Scott
*  Homework 1
*  02/07/2022
*/

#include "product_record.h" 
#include "file_services.h" 
#include "station_services.h" 
#include "pipeline_services.h" 
#include "threads_services.h"
#include "thread_structs.h"
#include <pthread.h>


// entry point for the program
void main (int argc, char *argv[])
{
	if (argc != 3) 
    { 
        printf("You must provide the input and output files as two arguments\n");
        exit(1);
    }     

    char *inputFile = argv[1];
	char *outputFile = argv[2];
    struct product_record records[MAXFILES];
    
    pthread_t tid[7];

    initMutex();
    initializeSemsAndQueues();

    createReadThread(&tid[0], inputFile, records);
    
    for(int i = 0; i < MAXSTAGES; i++)
    {
        createStationThreads(&tid[i + 1]); 
    }

    createWriteThread(&tid[6], outputFile);
    
    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);
    pthread_join(tid[2], NULL);
    pthread_join(tid[3], NULL);
    pthread_join(tid[4], NULL);
    pthread_join(tid[5], NULL);
    pthread_join(tid[6], NULL);


    int* stats = getStationStatistics();
    printf("Station Statistics\n");
    printf("------------------\n");
    for(int i = 0; i < MAXSTAGES; i++)
    {
        printf("Station %d: %d\n", i, stats[i]);
    }
}
